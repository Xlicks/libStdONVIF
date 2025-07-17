set(INC global.h
        onvif_analytics_client.h
        onvif_client.h
        onvif_device_client.h
        onvif_discovery_client.h
        onvif_display_client.h
        onvif_event_client.h
        onvif_imaging_client.h
        onvif_media2_client.h
        onvif_media_client.h
        onvif_ptz_client.h
        onvif_receiver_client.h
        onvif_recording_client.h
        onvif_replay_client.h
        request.h
        response.h
        soap_auth_handler.h
        soap_context.h
        soap_helper.h
        topics.h
)

set(SRC onvif_analytics_client.cpp
        onvif_client.cpp
        onvif_device_client.cpp
        onvif_discovery_client.cpp
        onvif_display_client.cpp
        onvif_event_client.cpp
        onvif_imaging_client.cpp
        onvif_media2_client.cpp
        onvif_media_client.cpp
        onvif_ptz_client.cpp
        onvif_receiver_client.cpp
        onvif_recording_client.cpp
        onvif_replay_client.cpp
        soap_auth_handler.cpp
        soap_context.cpp
        soap_helper.cpp
        topics.cpp
)

if(HAS_OPENSSL)
    add_definitions(-DWITH_OPENSSL)
else()
    message(STATUS "SSL/TLS disabled because of missing OpenSSL library")
endif()

add_library(onvif STATIC ${INC} ${SRC})
target_include_directories(onvif PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(onvif onvif_base)