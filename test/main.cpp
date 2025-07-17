//
// Created by licks on 25-7-17.
//

#include <memory>
#include <iostream>
#include <string>
#include <regex>

#include "onvif_discovery_client.h"
#include "soap_context.h"
#include "soap_helper.h"

int main(int argc, const char * argv[])
{
    auto soap_ctx = std::make_shared<SoapContext>();
    std::string msg_id("urn:uuid:");
    msg_id += SoapHelper::GenerateUuid();
    std::cout << "msg_id: " << msg_id << std::endl;
    OnvifDiscoveryClient client("soap.udp://239.255.255.250:3702", soap_ctx);
    ProbeTypeRequest request;
    client.SetWaitTime(5);
    client.SetProbeType(request);
    auto ret = client.Probe(request, msg_id);
    while (ret.isSuccess())
    {
        auto response = client.ReceiveProbeMatches();
        if (response.isSuccess())
        {
            if (nullptr == response.GetResultObject()->wsdd__ProbeMatches)
            {
                continue;
            }
            if (nullptr == response.GetResultObject()->wsdd__ProbeMatches->ProbeMatch)
            {
                continue;
            }
            for (int i = 0; i < response.GetResultObject()->wsdd__ProbeMatches->__sizeProbeMatch; i++)
            {
                auto probe_match = response.GetResultObject()->wsdd__ProbeMatches->ProbeMatch + i;
                if (nullptr == probe_match) continue;
                std::cout << "ProbeMatch.Address: " << probe_match->XAddrs << std::endl;
            }
            std::cout << "ProbeMatches: " << response.GetResultObject()->wsdd__ProbeMatches->ProbeMatch->XAddrs << std::endl;
        }
        else
        {
            std::cerr << "ProbeMatches error: " << std::endl;
            break;
        }
    }
    return 0;
}
