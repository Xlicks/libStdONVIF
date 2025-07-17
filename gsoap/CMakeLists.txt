include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../gsoap)
set(GENERATED
        generated/soapAnalyticsDeviceBindingProxy.cpp
        generated/soapAnalyticsDeviceBindingProxy.h
        generated/soapAnalyticsDeviceBindingService.cpp
        generated/soapAnalyticsDeviceBindingService.h
        generated/soapC.cpp
        generated/soapDeviceBindingProxy.cpp
        generated/soapDeviceBindingProxy.h
        generated/soapDeviceBindingService.cpp
        generated/soapDeviceBindingService.h
        generated/soapDisplayBindingProxy.cpp
        generated/soapDisplayBindingProxy.h
        generated/soapDisplayBindingService.cpp
        generated/soapDisplayBindingService.h
        generated/soapH.h
        generated/soapImagingBindingProxy.cpp
        generated/soapImagingBindingProxy.h
        generated/soapImagingBindingService.cpp
        generated/soapImagingBindingService.h
        generated/soapMedia2BindingProxy.cpp
        generated/soapMedia2BindingProxy.h
        generated/soapMedia2BindingService.cpp
        generated/soapMedia2BindingService.h
        generated/soapMediaBindingProxy.cpp
        generated/soapMediaBindingProxy.h
        generated/soapMediaBindingService.cpp
        generated/soapMediaBindingService.h
        generated/soapPTZBindingProxy.cpp
        generated/soapPTZBindingProxy.h
        generated/soapPTZBindingService.cpp
        generated/soapPTZBindingService.h
        generated/soapPullPointSubscriptionBindingProxy.cpp
        generated/soapPullPointSubscriptionBindingProxy.h
        generated/soapPullPointSubscriptionBindingService.cpp
        generated/soapPullPointSubscriptionBindingService.h
        generated/soapReceiverBindingProxy.cpp
        generated/soapReceiverBindingProxy.h
        generated/soapReceiverBindingService.cpp
        generated/soapReceiverBindingService.h
        generated/soapRecordingBindingProxy.cpp
        generated/soapRecordingBindingProxy.h
        generated/soapRecordingBindingService.cpp
        generated/soapRecordingBindingService.h
        generated/soapRemoteDiscoveryBindingProxy.cpp
        generated/soapRemoteDiscoveryBindingProxy.h
        generated/soapRemoteDiscoveryBindingService.cpp
        generated/soapRemoteDiscoveryBindingService.h
        generated/soapReplayBindingProxy.cpp
        generated/soapReplayBindingProxy.h
        generated/soapReplayBindingService.cpp
        generated/soapReplayBindingService.h
        generated/soapRuleEngineBindingProxy.cpp
        generated/soapRuleEngineBindingProxy.h
        generated/soapRuleEngineBindingService.cpp
        generated/soapRuleEngineBindingService.h
        generated/soapSearchBindingProxy.cpp
        generated/soapSearchBindingProxy.h
        generated/soapSearchBindingService.cpp
        generated/soapSearchBindingService.h
        generated/soapStub.h
        generated/soapwsddProxy.cpp
        generated/soapwsddProxy.h
        generated/soapwsddService.cpp
        generated/soapwsddService.h
)

set(GSOAP gsoap/chrono_duration.cpp
        gsoap/chrono_duration.h
        gsoap/chrono_time_point.cpp
        gsoap/chrono_time_point.h
        gsoap/dom.cpp
        gsoap/duration.cpp
        gsoap/duration.h
        gsoap/stdsoap2.cpp
        gsoap/stdsoap2.h
        gsoap/stlvector.h
        gsoap/wsaapi.cpp
        gsoap/wsaapi.h
        gsoap/struct_timeval.h
        gsoap/struct_timeval.cpp
)

if(HAS_OPENSSL)
    set(GSOAP ${GSOAP} gsoap/smdevp.h
            gsoap/httpda.h
            gsoap/mecevp.h
            gsoap/wsseapi.h
            gsoap/smdevp.cpp
            gsoap/httpda.cpp
            gsoap/mecevp.cpp
            gsoap/wsseapi.cpp)
    add_definitions(-DWITH_OPENSSL)
else()
    set(GSOAP ${GSOAP} gsoap/wsseapi-lite.h gsoap/wsseapi-lite.cpp)
    message(STATUS "SSL/TLS disabled because of missing OpenSSL library")
endif()

add_definitions(-DWITH_DOM)
add_library(onvif_base STATIC ${GENERATED} ${GSOAP})
target_include_directories(onvif_base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/gsoap/ ${CMAKE_CURRENT_SOURCE_DIR}/generated/)
if(HAS_OPENSSL)
    target_link_libraries(onvif_base "${OpenSSL_ssl_PATH}" "${OpenSSL_cryptro_PATH}")
endif()