//
// Created by licks on 24-7-15.
//

#ifndef ONVIF_DISCOVERY_CLIENT_H
#define ONVIF_DISCOVERY_CLIENT_H

#include "global.h"
#include "request.h"
#include "response.h"
#include "onvif_client.h"
#include "deleter_duplicator.h"

using ProbeMatchesResponse =
 Response<__wsdd__ProbeMatches, ProbeMatchesDeleter<__wsdd__ProbeMatches>, ProbeMatchesDuplicator<__wsdd__ProbeMatches>>;
using ResolveTypeResponse = Response<wsdd__ResolveType, ResolveTypeDeleter<wsdd__ResolveType>, ResolveTypeDuplicator<wsdd__ResolveType>>;
using ProbeMatchTypeResponse =
 Response<wsdd__ProbeMatchesType, ProbeMatchTypeDeleter<wsdd__ProbeMatchesType>, ProbeMatchTypeDuplicator<wsdd__ProbeMatchesType>>;
using HelloTypeRequest = Request<wsdd__HelloType, HelloTypeDeleterReq<wsdd__HelloType>>;
using ProbeTypeRequest = Request<wsdd__ProbeType, ProbeTypeDeleterReq<wsdd__ProbeType>>;
using ByeTypeRequest = Request<wsdd__ByeType, ByeTypeDeleterReq<wsdd__ByeType>>;

struct OnvifDiscoveryClientImplementation;

class OnvifDiscoveryClient final : public OnvifClient
{
public:
    DISALLOW_COPY_AND_ASSIGN(OnvifDiscoveryClient);
    explicit OnvifDiscoveryClient(const std::string &endpoint = "soap.udp://239.255.255.250:3702", std::shared_ptr<SoapContext> shared_ctx = std::make_shared<SoapContext>());
    ~OnvifDiscoveryClient() override;
    void SetWaitTime(int wait_time = 5) const;
    void SetProbeType(ProbeTypeRequest &request, const std::string& scopes = {}, const std::string& types = "dn:NetworkVideoTransmitter") const;
    DetailedResponse Probe(ProbeTypeRequest &request, const std::string& message_id) const;
    [[nodiscard]] ProbeMatchesResponse ReceiveProbeMatches() const;
private:
    OnvifDiscoveryClientImplementation* impl_;
}; //OnvifDiscoveryClient

#endif //ONVIF_DISCOVERY_CLIENT_H
