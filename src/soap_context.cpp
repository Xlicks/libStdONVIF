//
// Created by licks on 24-7-1.
//

#include "soap_context.h"
#ifdef WITH_OPENSSL
#include "httpda.h"
#endif // WITH_OPENSSL
#include <algorithm>
#include <cstring>

#include "namespaces.nsmap"
#include "wsaapi.h"
#include <memory>
#include <string>
#include <mutex>

struct arbData
{
    bool enableDebug = false;
    int (*fsend)(struct soap*, const char*, size_t) = nullptr;
    size_t (*frecv)(struct soap*, char*, size_t) = nullptr;
};

int fsend(struct soap* soap, const char* s, const size_t n)
{
    const auto* user = static_cast<arbData*>(soap->user);
    const int ret = user->fsend(soap, s, n);

    if (user->enableDebug)
    {
        std::string out(s, n);

        // Trim leading and trailing whitespace
        auto not_space = [](const unsigned char ch) { return !std::isspace(ch); };
        const auto begin = std::find_if(out.begin(), out.end(), not_space);

        if (const auto end = std::find_if(out.rbegin(), out.rend(), not_space).base(); begin < end)
        {
            const std::string trimmed(begin, end);
            std::cout << trimmed << std::endl;
        }
    }

    return ret;
}

size_t frecv(struct soap* soap, char* s, const size_t n)
{
    const auto* user = static_cast<arbData*>(soap->user);
    const size_t length = user->frecv(soap, s, n);

    if (user->enableDebug)
    {
        std::string out(s, length);

        // Trim leading and trailing whitespace
        auto not_space = [](const unsigned char ch) { return !std::isspace(ch); };
        const auto begin = std::find_if(out.begin(), out.end(), not_space);

        if (const auto end = std::find_if(out.rbegin(), out.rend(), not_space).base(); begin < end)
        {
            const std::string trimmed(begin, end);
            std::cout << trimmed << std::endl;
        }
    }

    return length;
}

struct CtxPrivate
{
    explicit CtxPrivate(SoapContext* soap_context) :
        soap_context(soap_context),
        soap_ptr(nullptr),
        mutex(),
        is_saved(false),
        imode_saved(),
        omode_saved(),
        connect_timeout_saved(),
        send_timeout(),
        receive_timeout(),
        so_flags(),
        con_flags(),
        bind_flags(),
        accept_flags(),
        retry_counter(0),
        auth_handler(nullptr)
    {
    }

    SoapContext* soap_context;
    soap* soap_ptr;
    std::recursive_mutex mutex;
    bool is_saved;
    soap_mode imode_saved;
    soap_mode omode_saved;
    int connect_timeout_saved;
    int send_timeout;
    int receive_timeout;
    int so_flags;
    int con_flags;
    int bind_flags;
    int accept_flags;
    int retry_counter;
    std::unique_ptr<SoapAuthHandler> auth_handler;
};


SoapContext::SoapContext() : context_impl_(new (std::nothrow) CtxPrivate(this))
{
    context_impl_->soap_ptr = soap_new();
    soap_init2(context_impl_->soap_ptr, SOAP_NEW_IO_DEFAULT, SOAP_NEW_IO_DEFAULT);
    InitCtx();
}

SoapContext::SoapContext(const soap_mode imode, const soap_mode omode) : context_impl_(new (std::nothrow) CtxPrivate(this))
{
    context_impl_->soap_ptr = soap_new();
    soap_init2(context_impl_->soap_ptr, imode, omode);
    InitCtx();
}

SoapContext::~SoapContext()
{
    if (context_impl_->auth_handler)
    {
        context_impl_->auth_handler->FreeAuth(context_impl_->soap_ptr);
        context_impl_->auth_handler.reset(nullptr);
    }
    delete static_cast<arbData*>(context_impl_->soap_ptr->user);
    context_impl_->soap_ptr->user = nullptr;
    soap_free(context_impl_->soap_ptr);
    delete context_impl_;
}

void SoapContext::SetIMode(const soap_mode imode) const
{
    std::lock_guard locker(context_impl_->mutex);
    soap_imode(context_impl_->soap_ptr, imode);
}

void SoapContext::SetOMode(const soap_mode omode) const
{
    std::lock_guard locker(context_impl_->mutex);
    soap_omode(context_impl_->soap_ptr, omode);
}

void SoapContext::EnableIModeFlags(const soap_mode imode) const
{
    std::lock_guard locker(context_impl_->mutex);
    soap_set_imode(context_impl_->soap_ptr, imode);
}

void SoapContext::EnableModeFlags(const soap_mode mode) const
{
    std::lock_guard locker(context_impl_->mutex);
    soap_set_mode(context_impl_->soap_ptr, mode);
}

void SoapContext::EnableOModeFlags(const soap_mode omode) const
{
    std::lock_guard locker(context_impl_->mutex);
    soap_set_omode(context_impl_->soap_ptr, omode);
}

void SoapContext::DisableIModeFlags(const soap_mode imode) const
{
    std::lock_guard locker(context_impl_->mutex);
    soap_clr_imode(context_impl_->soap_ptr, imode);
}

void SoapContext::DisableModeFlags(const soap_mode mode) const
{
    std::lock_guard locker(context_impl_->mutex);
    soap_clr_mode(context_impl_->soap_ptr, mode);
}

void SoapContext::DisableOModeFlags(const soap_mode omode) const
{
    std::lock_guard locker(context_impl_->mutex);
    soap_clr_omode(context_impl_->soap_ptr, omode);
}

void SoapContext::EnablePrintRawSoap() const
{
    std::lock_guard locker(context_impl_->mutex);
    if (auto* user = static_cast<arbData*>(context_impl_->soap_ptr->user))
    {
        user->enableDebug = true;
    }
}

void SoapContext::DisablePrintRawSoap() const
{
    std::lock_guard locker(context_impl_->mutex);
    if (auto* user = static_cast<arbData*>(context_impl_->soap_ptr->user))
    {
        user->enableDebug = false;
    }
}

std::string SoapContext::GetFaultString() const
{
    std::lock_guard lock(context_impl_->mutex);
    std::string ret;

    if (context_impl_->soap_ptr->error != SOAP_OK)
    {
        if (!*soap_faultcode(context_impl_->soap_ptr))
            soap_set_fault(context_impl_->soap_ptr);

        if (const char* faultCode = *soap_faultcode(context_impl_->soap_ptr))
            ret += faultCode;

        if (!*soap_faultstring(context_impl_->soap_ptr))
            *soap_faultstring(context_impl_->soap_ptr) = const_cast<char*>("");

        if (const char* faultDetail = *soap_faultstring(context_impl_->soap_ptr); faultDetail && std::strlen(faultDetail) > 0)
        {
            ret += ": ";
            ret += faultDetail;
        }
    }

    return ret;
}

std::string SoapContext::GetFaultDetail() const
{
    std::lock_guard lock(context_impl_->mutex);
    std::string ret;
    if (context_impl_->soap_ptr->error!= SOAP_OK)
    {
        if (nullptr != *soap_faultdetail(context_impl_->soap_ptr))
        {
            return {*soap_faultdetail(context_impl_->soap_ptr)};
        }
    }
    return {};
}

int SoapContext::GetFaultCode() const
{
    std::lock_guard lock(context_impl_->mutex);
    return context_impl_->soap_ptr->error;
}

std::string SoapContext::GetFaultSubCode() const
{
    std::lock_guard lock(context_impl_->mutex);
    if (context_impl_->soap_ptr->error != SOAP_OK)
    {
        if (nullptr != *soap_faultsubcode(context_impl_->soap_ptr))
        {
            return {*soap_faultsubcode(context_impl_->soap_ptr)};
        }
    }
    return {};
}

#ifdef WITH_OPENSSL
bool SoapContext::EnableSSL() const
{
    std::lock_guard lock(context_impl_->mutex);
    const auto result = soap_ssl_client_context(context_impl_->soap_ptr, SOAP_NEW_SSL_DEFAULT, nullptr, nullptr, nullptr, nullptr, nullptr);
    if(result != SOAP_OK)
    {
        std::cerr << GetFaultString();
    }
    return result == SOAP_OK;
}
#endif

void SoapContext::SoapDelete(void* p) const
{
    std::lock_guard locker(context_impl_->mutex);
    soap_delete(context_impl_->soap_ptr, p);
}

void SoapContext::SoapDealloc(void* p) const
{
    std::lock_guard locker(context_impl_->mutex);
    soap_dealloc(context_impl_->soap_ptr, p);
}

void SoapContext::SoapUnlink(const void* p) const
{
    std::lock_guard locker(context_impl_->mutex);
    soap_unlink(context_impl_->soap_ptr, p);
}

void SoapContext::SoapFreeTemp() const
{
    std::lock_guard locker(context_impl_->mutex);
    soap_free_temp(context_impl_->soap_ptr);
}

void SoapContext::SetConnectTimeout(const int timeoutMs) const
{
    std::lock_guard locker(context_impl_->mutex);
    context_impl_->soap_ptr->connect_timeout = timeoutMs * -1000;
}

void SoapContext::SetSendTimeout(const int timeoutMs) const
{
    std::lock_guard locker(context_impl_->mutex);
    context_impl_->soap_ptr->send_timeout = timeoutMs * -1000;
}

void SoapContext::SetReceiveTimeout(const int timeoutMs) const
{
    std::lock_guard locker(context_impl_->mutex);
    context_impl_->soap_ptr->recv_timeout = timeoutMs * -1000;
}

void SoapContext::SetSocketFlags(const int soFlags) const
{
    std::lock_guard locker(context_impl_->mutex);
    context_impl_->soap_ptr->socket_flags = soFlags;
}

void SoapContext::SetConnectFlags(const int conFlags) const
{
    std::lock_guard locker(context_impl_->mutex);
    context_impl_->soap_ptr->connect_flags = conFlags;
}

void SoapContext::SetBindFlags(const int bindFlags) const
{
    std::lock_guard locker(context_impl_->mutex);
    context_impl_->soap_ptr->bind_flags = bindFlags;
}

void SoapContext::SetAcceptFlags(const int acceptFlags) const
{
    std::lock_guard locker(context_impl_->mutex);
    context_impl_->soap_ptr->accept_flags = acceptFlags;
}

void SoapContext::ForceSocketClose()
{
#ifdef WITH_SELF_PIPE
    soap_close_connection(context_impl_->ptr_soap_);
#endif
}

void SoapContext::Save() const
{
    std::lock_guard locker(context_impl_->mutex);
    context_impl_->imode_saved = context_impl_->soap_ptr->imode;
    context_impl_->omode_saved = context_impl_->soap_ptr->omode;
    context_impl_->connect_timeout_saved = context_impl_->soap_ptr->connect_timeout;
    context_impl_->receive_timeout = context_impl_->soap_ptr->recv_timeout;
    context_impl_->send_timeout = context_impl_->soap_ptr->send_timeout;
    context_impl_->so_flags = context_impl_->soap_ptr->socket_flags;
    context_impl_->con_flags = context_impl_->soap_ptr->connect_flags;
    context_impl_->bind_flags = context_impl_->soap_ptr->bind_flags;
    context_impl_->accept_flags = context_impl_->soap_ptr->accept_flags;
    context_impl_->is_saved = true;
}

void SoapContext::Restore() const
{
    std::lock_guard locker(context_impl_->mutex);
    if (context_impl_->is_saved)
    {
        soap_set_imode(context_impl_->soap_ptr, context_impl_->imode_saved);
        soap_set_omode(context_impl_->soap_ptr, context_impl_->omode_saved);
        context_impl_->soap_ptr->connect_timeout = context_impl_->connect_timeout_saved;
        context_impl_->soap_ptr->recv_timeout = context_impl_->receive_timeout;
        context_impl_->soap_ptr->send_timeout = context_impl_->send_timeout;
        context_impl_->soap_ptr->socket_flags = context_impl_->so_flags;
        context_impl_->soap_ptr->connect_flags = context_impl_->con_flags;
        context_impl_->soap_ptr->bind_flags = context_impl_->bind_flags;
        context_impl_->soap_ptr->accept_flags = context_impl_->accept_flags;
        context_impl_->is_saved= false;
    }
}

soap* SoapContext::Acquire() const
{
    context_impl_->mutex.lock();
    context_impl_->retry_counter = 0;
    return context_impl_->soap_ptr;
}

soap* SoapContext::TryAcquire(int timeoutMs) const
{
    if (context_impl_->mutex.try_lock())
    {
        context_impl_->retry_counter = 0;
        return context_impl_->soap_ptr;
    }
    return nullptr;
}

void SoapContext::Release() const
{
    context_impl_->retry_counter = 0;
    context_impl_->mutex.unlock();
}

const Namespace* SoapContext::GetDefaultNamespaces()
{
    return namespaces;
}

std::string SoapContext::GetPrefix(const std::string& ns)
{
    const struct Namespace* p = GetDefaultNamespaces();
    while (p && p->id)
    {
        if (p->ns && ns == p->ns)
        {
            return {p->id};
        }
        ++p;
    }
    return {}; // 返回空字符串
}

std::string SoapContext::GetNamespace(const std::string& prefix)
{
    const struct Namespace* p = GetDefaultNamespaces();
    while (p && p->id)
    {
        if (p->id && prefix == p->id)
        {
            return {p->ns ? p->ns : ""};
        }
        ++p;
    }
    return {}; // 返回空字符串
}

void SoapContext::SetSoapAuthHandler(std::unique_ptr<SoapAuthHandler> handler) const
{
    std::lock_guard locker(context_impl_->mutex);
    context_impl_->auth_handler = std::move(handler);
}

void SoapContext::SetAuth(const std::string& user_name, const std::string& password, const AuthHandlerMode mode) const
{
    std::lock_guard lock(context_impl_->mutex);
    if (context_impl_->auth_handler)
    {
        context_impl_->auth_handler->FreeAuth(context_impl_->soap_ptr);
        context_impl_->auth_handler->SetAuth(context_impl_->soap_ptr, user_name, password, mode);
    }
}

void SoapContext::DisableAuth() const
{
    std::lock_guard lock(context_impl_->mutex);
    if(context_impl_->auth_handler)
    {
        context_impl_->auth_handler->FreeAuth(context_impl_->soap_ptr);
    }
}

bool SoapContext::IsAuthFault() const
{
    std::lock_guard lock(context_impl_->mutex);
    if(context_impl_->auth_handler) return context_impl_->auth_handler->IsAuthFault(context_impl_->soap_ptr);
    return false;
}

void SoapContext::Unauthorized()
{
}

void SoapContext::InitCtx() const
{
    soap_register_plugin(context_impl_->soap_ptr, soap_wsa);
    context_impl_->soap_ptr->connect_retry = 1;
    context_impl_->soap_ptr->connect_timeout = SOAP_DEFAULT_CONNECT_TIMEOUT * -1000;
    context_impl_->soap_ptr->recv_timeout = SOAP_DEFAULT_RECEIVE_TIMEOUT * -1000;
    context_impl_->soap_ptr->send_timeout = SOAP_DEFAULT_SEND_TIMEOUT * -1000;
    soap_set_namespaces(context_impl_->soap_ptr, SoapContext::GetDefaultNamespaces());

    int (*pFsend)(struct soap*, const char*, size_t) = &fsend;
    size_t (*pFrecv)(struct soap* soap, char* s, size_t n) = &frecv;
    const auto ud = new arbData();
    ud->frecv = context_impl_->soap_ptr->frecv;
    ud->fsend = context_impl_->soap_ptr->fsend;
    context_impl_->soap_ptr->user = ud;
    context_impl_->soap_ptr->frecv = pFrecv;
    context_impl_->soap_ptr->fsend = pFsend;
    SetSoapAuthHandler(std::make_unique<DefaultAuthHandler>());
}

void SoapContext::RestoreAuthData() const
{
    std::lock_guard lock(context_impl_->mutex);
    if(context_impl_->auth_handler) context_impl_->auth_handler->RestoreAuthData(context_impl_->soap_ptr);
}

bool SoapContext::ProcessAuthFaultAndRetry() const
{
    auto retry = false;
    std::lock_guard lock(context_impl_->mutex);
    if (context_impl_->auth_handler)
    {
        if (context_impl_->auth_handler->IsAuthFault(context_impl_->soap_ptr))
        {
            retry = context_impl_->auth_handler->ProcessAuthFaultAndRetry(context_impl_->soap_ptr, context_impl_->retry_counter++);
        }
        context_impl_->auth_handler->ProcessResponse(context_impl_->soap_ptr);
    }
    return retry;
}
