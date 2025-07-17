//
// Created by licks on 24-7-15.
//

#include "onvif_discovery_client.h"

#include "soapwsddProxy.h"
#include "wsaapi.h"

struct OnvifDiscoveryClientImplementation
{
    explicit OnvifDiscoveryClientImplementation(OnvifDiscoveryClient* dc) : discovery_client_ptr(dc),
                                                                    wdd_proxy(
                                                                        discovery_client_ptr->GetContext()->Acquire())
    {
        discovery_client_ptr->GetContext()->Release();
    }

    OnvifDiscoveryClient* discovery_client_ptr;
    wsddProxy wdd_proxy;
};

OnvifDiscoveryClient::OnvifDiscoveryClient(const std::string& endpoint, std::shared_ptr<SoapContext> shared_ctx)
    : OnvifClient(endpoint, std::move(shared_ctx)),
      impl_(new (std::nothrow) OnvifDiscoveryClientImplementation(this))
{
    const auto soap = GetContext()->Acquire();
    (void)soap;
    GetContext()->Release();
}

OnvifDiscoveryClient::~OnvifDiscoveryClient()
{
    delete impl_;
}

void OnvifDiscoveryClient::SetWaitTime(const int wait_time) const
{
    const auto soap = AcquireContext();
    soap->send_timeout = wait_time;
    soap->recv_timeout = wait_time;
    soap->connect_timeout = wait_time;
}

void OnvifDiscoveryClient::SetProbeType(ProbeTypeRequest &request, const std::string& scopes, const std::string& types) const
{
    const auto soap = AcquireContext();
    const auto scope = static_cast<struct wsdd__ScopesType*>(soap_malloc(soap, sizeof(struct wsdd__ScopesType)));
    soap_default_wsdd__ScopesType(soap, scope);
    scope->__item = static_cast<char*>(soap_malloc(soap, scopes.length() + 1));
    strcpy(scope->__item, scopes.c_str());

    soap_default_wsdd__ProbeType(soap, &request);
    request.Scopes = scope;
    request.Types = static_cast<char*>(soap_malloc(soap, types.length() + 1));
    strcpy(request.Types, types.c_str());
}

DetailedResponse OnvifDiscoveryClient::Probe(ProbeTypeRequest& request, const std::string& message_id) const
{
    auto ret = SOAP_OK;
    const auto soap = AcquireContext();
    soap_wsa_request(soap, message_id.c_str(), "urn:schemas-xmlsoap-org:ws:2005:04:discovery",
                     "http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe");
    do
    {
        ret = impl_->wdd_proxy.Probe(GetEndpoint().c_str(),
                               !request.GetSoapAction().empty() ? request.GetSoapAction().c_str() : nullptr
                               , &request);
    }
    while (ReTry(soap));
    DetailedResponse response(ret, GetFaultString(), GetFaultDetail(),
                              ret != SOAP_OK && soap->fault ? soap->fault->SOAP_ENV__Detail : nullptr);
    ReleaseContext(soap);
    return response;
}

ProbeMatchesResponse OnvifDiscoveryClient::ReceiveProbeMatches() const
{
    __wsdd__ProbeMatches responseObject;
    const auto pSoap = AcquireContext();
    do
    {
        if (SOAP_OK != impl_->wdd_proxy.recv_ProbeMatches(responseObject))
        {
            break;
        }
    }while (ReTry(pSoap));
    auto builder = ProbeMatchesResponse::Builder();
    builder.From(GetContext(), &responseObject);
    ReleaseContext(pSoap);
    return builder.Build();
}
