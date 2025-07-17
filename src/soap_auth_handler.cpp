//
// Created by licks on 24-7-1.
//

#include "soap_auth_handler.h"
#include "soapH.h"
#include "global.h"
#ifdef WITH_OPENSSL
#include "wsseapi.h"
#include "httpda.h"
#else
#include "wsseapi-lite.h"
#endif // WITH_OPENSSL

struct DefaultAuthHandlerPrivate
{
    explicit DefaultAuthHandlerPrivate(SoapAuthHandler* pQ) :
        auth_handler(pQ),
        auth_mode(AuthHandlerMode::NO_AUTH),
        user_name(),
        password(),
#ifdef WITH_OPENSSL
     digest_store(),
#endif // WITH_OPENSSL
        auth_processed(false)
    {
    }

    SoapAuthHandler* auth_handler;
    AuthHandlerMode auth_mode;
    std::string user_name;
    std::string password;
#ifdef WITH_OPENSSL
    http_da_info digest_store;
#endif // WITH_OPENSSL
    bool auth_processed;
};

DefaultAuthHandler::DefaultAuthHandler() : auth_handler_impl_(new DefaultAuthHandlerPrivate(this))
{
}

DefaultAuthHandler::~DefaultAuthHandler()
{
    delete auth_handler_impl_;
}

void DefaultAuthHandler::SetAuth(soap* ctx, const std::string& user_name, const std::string& password,
                                 AuthHandlerMode mode)
{
    if (nullptr != ctx)
    {
#ifdef WITH_OPENSSL
        if(!soap_lookup_plugin(ctx, HTTP_DA_ID))
        {
            soap_register_plugin(ctx, http_da);
        }
#endif
        FreeAuth(ctx);
        auth_handler_impl_->auth_processed = false;
#ifndef WITH_OPENSSL
        if (mode != AuthHandlerMode::NO_AUTH && mode != AuthHandlerMode::WS_USERNAME_TOKEN)
        {
            mode = AuthHandlerMode::WS_USERNAME_TOKEN;
            std::cerr << "This build doesn't support http digest auth. Switching back to ws token";
        }
#endif // WITH_OPENSSL
        auth_handler_impl_->auth_mode = mode;
        auth_handler_impl_->user_name = user_name;
        auth_handler_impl_->password = password;
    }
}

void DefaultAuthHandler::FreeAuth(soap* ctx)
{
    auth_handler_impl_->auth_mode = AuthHandlerMode::NO_AUTH;
    auth_handler_impl_->auth_processed = false;
    if (ctx)
    {
#ifdef WITH_OPENSSL
        if(auth_handler_impl_->digest_store.authrealm)
        {
            http_da_release(ctx, &auth_handler_impl_->digest_store);
        }
#endif // WITH_OPENSSL
        soap_wsse_delete_Security(ctx);
    }
}

bool DefaultAuthHandler::IsAuthFault(soap* ctx)
{
    return isHttpAuthFault(ctx) || isWssTokenAuthFault(ctx);
}

bool DefaultAuthHandler::isHttpAuthFault(const soap* ctx)
{
    if (ctx) return ctx->error == HTTP_UNAUTHORIZED;
    return false;
}

bool DefaultAuthHandler::isWssTokenAuthFault(soap* ctx)
{
    if (!ctx)
    {
        return false;
    }

    // gSOAP 设定：SOAP_OK 为 0，SOAP_CLI_FAULT 表示客户端错误
    if (ctx->error != SOAP_OK)
    {
        if (const char** faultSubcode = soap_faultsubcode(ctx); faultSubcode && *faultSubcode)
        {
            const std::string subcode(*faultSubcode);
            const std::string expected = "\"http://www.onvif.org/ver10/error\":NotAuthorized";
            return ctx->error == SOAP_CLI_FAULT && subcode == expected;
        }
    }

    return false;
}

void DefaultAuthHandler::RestoreAuthData(soap* ctx)
{
    if (ctx)
    {
        if (auth_handler_impl_->auth_processed)
        {
            if (auth_handler_impl_->auth_mode == AuthHandlerMode::HTTP_DIGEST || auth_handler_impl_->auth_mode == AuthHandlerMode::BOTH)
            {
#ifdef WITH_OPENSSL
                if(auth_handler_impl_->digest_store.authrealm)
                {
                    http_da_restore(ctx, &auth_handler_impl_->digest_store);
                }
                else
                {
                    std::cerr << "The authrealm is missing in http_da_restore. Falling back to WS token auth.";
                    auth_handler_impl_->auth_mode = AuthHandlerMode::WS_USERNAME_TOKEN;
                }
#endif // WITH_OPENSSL
            }
            if (auth_handler_impl_->auth_mode == AuthHandlerMode::WS_USERNAME_TOKEN || auth_handler_impl_->auth_mode == AuthHandlerMode::BOTH)
            {
                soap_wsse_delete_Security(ctx);
#ifdef WITH_OPENSSL
                soap_wsse_add_UsernameTokenDigest(ctx, nullptr, auth_handler_impl_->user_name.c_str(), auth_handler_impl_->password.c_str());
#else
                soap_wsse_add_UsernameTokenText(ctx, nullptr, auth_handler_impl_->user_name.c_str(), auth_handler_impl_->password.c_str());
#endif
                soap_wsse_add_Timestamp(ctx, nullptr, 0);
            }
        }
    }
}

bool DefaultAuthHandler::ProcessAuthFaultAndRetry(soap* ctx, const int num_retry)
{
    if (ctx)
    {
        bool ret = false;
        if (IsAuthFault(ctx))
        {
            if (isHttpAuthFault(ctx) && auth_handler_impl_->auth_processed && ctx->authrealm && num_retry == 0)
            {
                // nonce probably changed
                auth_handler_impl_->auth_processed = false;
            }

            if (!auth_handler_impl_->auth_processed)
            {
                if ((auth_handler_impl_->auth_mode == AuthHandlerMode::HTTP_DIGEST || auth_handler_impl_->auth_mode == AuthHandlerMode::BOTH ||
                        auth_handler_impl_->auth_mode == AuthHandlerMode::AUTO) &&
                    ctx->authrealm)
                {
                    // HTTP digest auth.
                    if (auth_handler_impl_->auth_mode == AuthHandlerMode::AUTO) auth_handler_impl_->auth_mode = AuthHandlerMode::HTTP_DIGEST;
#ifdef WITH_OPENSSL
                    http_da_save(ctx, &auth_handler_impl_->digest_store, ctx->authrealm, auth_handler_impl_->user_name.c_str(), auth_handler_impl_->password.c_str());
#endif // WITH_OPENSSL
                }
                if (auth_handler_impl_->auth_mode == AuthHandlerMode::WS_USERNAME_TOKEN || auth_handler_impl_->auth_mode == AuthHandlerMode::BOTH ||
                    auth_handler_impl_->auth_mode == AuthHandlerMode::AUTO)
                {
                    // WS token auth.
                    if (auth_handler_impl_->auth_mode == AuthHandlerMode::AUTO) auth_handler_impl_->auth_mode = AuthHandlerMode::WS_USERNAME_TOKEN;
                }
                ret = auth_handler_impl_->auth_processed = true;
            }
            else
            {
                ret = false;
            }
        }
        return ret;
    }
    return false;
}
