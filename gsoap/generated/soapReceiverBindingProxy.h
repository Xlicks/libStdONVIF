/* soapReceiverBindingProxy.h
   Generated by gSOAP 2.8.138 for generated.h

gSOAP XML Web services tools
Copyright (C) 2000-2025, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#ifndef soapReceiverBindingProxy_H
#define soapReceiverBindingProxy_H
#include "soapH.h"

    class SOAP_CMAC ReceiverBindingProxy {
      public:
        /// Context to manage proxy IO and data
        struct soap *soap;
        /// flag indicating that this context is owned by this proxy and should be deleted by the destructor
        bool soap_own;
        /// Endpoint URL of service 'ReceiverBindingProxy' (change as needed)
        const char *soap_endpoint;
        /// Variables globally declared in generated.h, if any
        /// Construct a proxy with new managing context
        ReceiverBindingProxy();
        /// Copy constructor
        ReceiverBindingProxy(const ReceiverBindingProxy& rhs);
        /// Construct proxy given a shared managing context
        ReceiverBindingProxy(struct soap*);
        /// Construct proxy given a shared managing context and endpoint URL
        ReceiverBindingProxy(struct soap*, const char *soap_endpoint_url);
        /// Constructor taking an endpoint URL
        ReceiverBindingProxy(const char *soap_endpoint_url);
        /// Constructor taking input and output mode flags for the new managing context
        ReceiverBindingProxy(soap_mode iomode);
        /// Constructor taking endpoint URL and input and output mode flags for the new managing context
        ReceiverBindingProxy(const char *soap_endpoint_url, soap_mode iomode);
        /// Constructor taking input and output mode flags for the new managing context
        ReceiverBindingProxy(soap_mode imode, soap_mode omode);
        /// Destructor deletes deserialized data and its managing context, when the context was allocated by the constructor
        virtual ~ReceiverBindingProxy();
        /// Initializer used by constructors
        virtual void ReceiverBindingProxy_init(soap_mode imode, soap_mode omode);
        /// Return a copy that has a new managing context with the same engine state
        virtual ReceiverBindingProxy *copy();
        /// Copy assignment
        ReceiverBindingProxy& operator=(const ReceiverBindingProxy&);
        /// Delete all deserialized data (uses soap_destroy() and soap_end())
        virtual void destroy();
        /// Delete all deserialized data and reset to default
        virtual void reset();
        /// Disables and removes SOAP Header from message by setting soap->header = NULL
        virtual void soap_noheader();
        /// Add SOAP Header to message
        virtual void soap_header(struct _wsse__Security *wsse__Security, char *wsa5__MessageID, struct wsa5__RelatesToType *wsa5__RelatesTo, struct wsa5__EndpointReferenceType *wsa5__From, struct wsa5__EndpointReferenceType *wsa5__ReplyTo, struct wsa5__EndpointReferenceType *wsa5__FaultTo, char *wsa5__To, char *wsa5__Action, struct chan__ChannelInstanceType *chan__ChannelInstance);
        /// Get SOAP Header structure (i.e. soap->header, which is NULL when absent)
        virtual ::SOAP_ENV__Header *soap_header();
        /// Get SOAP Fault structure (i.e. soap->fault, which is NULL when absent)
        virtual ::SOAP_ENV__Fault *soap_fault();
        /// Get SOAP Fault subcode QName string (NULL when absent)
        virtual const char *soap_fault_subcode();
        /// Get SOAP Fault string/reason (NULL when absent)
        virtual const char *soap_fault_string();
        /// Get SOAP Fault detail XML string (NULL when absent)
        virtual const char *soap_fault_detail();
        /// Close connection (normally automatic, except for send_X ops)
        virtual int soap_close_socket();
        /// Force close connection (can kill a thread blocked on IO)
        virtual int soap_force_close_socket();
        /// Print fault
        virtual void soap_print_fault(FILE*);
    #ifndef WITH_LEAN
    #ifndef WITH_COMPAT
        /// Print fault to stream
        virtual void soap_stream_fault(std::ostream&);
    #endif
        /// Write fault to buffer
        virtual char *soap_sprint_fault(char *buf, size_t len);
    #endif
        //
        /// Web service synchronous operation 'GetServiceCapabilities' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetServiceCapabilities(_trv__GetServiceCapabilities *trv__GetServiceCapabilities, _trv__GetServiceCapabilitiesResponse &trv__GetServiceCapabilitiesResponse) { return this->GetServiceCapabilities(NULL, NULL, trv__GetServiceCapabilities, trv__GetServiceCapabilitiesResponse); }
        /// Web service synchronous operation 'GetServiceCapabilities' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetServiceCapabilities(const char *soap_endpoint_url, const char *soap_action, _trv__GetServiceCapabilities *trv__GetServiceCapabilities, _trv__GetServiceCapabilitiesResponse &trv__GetServiceCapabilitiesResponse) { return this->send_GetServiceCapabilities(soap_endpoint_url, soap_action, trv__GetServiceCapabilities) || this->recv_GetServiceCapabilities(trv__GetServiceCapabilitiesResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetServiceCapabilities' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetServiceCapabilities(const char *soap_endpoint_url, const char *soap_action, _trv__GetServiceCapabilities *trv__GetServiceCapabilities);
        /// Web service asynchronous operation 'recv_GetServiceCapabilities' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetServiceCapabilities(_trv__GetServiceCapabilitiesResponse &trv__GetServiceCapabilitiesResponse);
        //
        /// Web service synchronous operation 'GetReceivers' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetReceivers(_trv__GetReceivers *trv__GetReceivers, _trv__GetReceiversResponse &trv__GetReceiversResponse) { return this->GetReceivers(NULL, NULL, trv__GetReceivers, trv__GetReceiversResponse); }
        /// Web service synchronous operation 'GetReceivers' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetReceivers(const char *soap_endpoint_url, const char *soap_action, _trv__GetReceivers *trv__GetReceivers, _trv__GetReceiversResponse &trv__GetReceiversResponse) { return this->send_GetReceivers(soap_endpoint_url, soap_action, trv__GetReceivers) || this->recv_GetReceivers(trv__GetReceiversResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetReceivers' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetReceivers(const char *soap_endpoint_url, const char *soap_action, _trv__GetReceivers *trv__GetReceivers);
        /// Web service asynchronous operation 'recv_GetReceivers' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetReceivers(_trv__GetReceiversResponse &trv__GetReceiversResponse);
        //
        /// Web service synchronous operation 'GetReceiver' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetReceiver(_trv__GetReceiver *trv__GetReceiver, _trv__GetReceiverResponse &trv__GetReceiverResponse) { return this->GetReceiver(NULL, NULL, trv__GetReceiver, trv__GetReceiverResponse); }
        /// Web service synchronous operation 'GetReceiver' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetReceiver(const char *soap_endpoint_url, const char *soap_action, _trv__GetReceiver *trv__GetReceiver, _trv__GetReceiverResponse &trv__GetReceiverResponse) { return this->send_GetReceiver(soap_endpoint_url, soap_action, trv__GetReceiver) || this->recv_GetReceiver(trv__GetReceiverResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetReceiver' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetReceiver(const char *soap_endpoint_url, const char *soap_action, _trv__GetReceiver *trv__GetReceiver);
        /// Web service asynchronous operation 'recv_GetReceiver' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetReceiver(_trv__GetReceiverResponse &trv__GetReceiverResponse);
        //
        /// Web service synchronous operation 'CreateReceiver' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int CreateReceiver(_trv__CreateReceiver *trv__CreateReceiver, _trv__CreateReceiverResponse &trv__CreateReceiverResponse) { return this->CreateReceiver(NULL, NULL, trv__CreateReceiver, trv__CreateReceiverResponse); }
        /// Web service synchronous operation 'CreateReceiver' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int CreateReceiver(const char *soap_endpoint_url, const char *soap_action, _trv__CreateReceiver *trv__CreateReceiver, _trv__CreateReceiverResponse &trv__CreateReceiverResponse) { return this->send_CreateReceiver(soap_endpoint_url, soap_action, trv__CreateReceiver) || this->recv_CreateReceiver(trv__CreateReceiverResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_CreateReceiver' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_CreateReceiver(const char *soap_endpoint_url, const char *soap_action, _trv__CreateReceiver *trv__CreateReceiver);
        /// Web service asynchronous operation 'recv_CreateReceiver' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_CreateReceiver(_trv__CreateReceiverResponse &trv__CreateReceiverResponse);
        //
        /// Web service synchronous operation 'DeleteReceiver' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int DeleteReceiver(_trv__DeleteReceiver *trv__DeleteReceiver, _trv__DeleteReceiverResponse &trv__DeleteReceiverResponse) { return this->DeleteReceiver(NULL, NULL, trv__DeleteReceiver, trv__DeleteReceiverResponse); }
        /// Web service synchronous operation 'DeleteReceiver' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int DeleteReceiver(const char *soap_endpoint_url, const char *soap_action, _trv__DeleteReceiver *trv__DeleteReceiver, _trv__DeleteReceiverResponse &trv__DeleteReceiverResponse) { return this->send_DeleteReceiver(soap_endpoint_url, soap_action, trv__DeleteReceiver) || this->recv_DeleteReceiver(trv__DeleteReceiverResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_DeleteReceiver' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_DeleteReceiver(const char *soap_endpoint_url, const char *soap_action, _trv__DeleteReceiver *trv__DeleteReceiver);
        /// Web service asynchronous operation 'recv_DeleteReceiver' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_DeleteReceiver(_trv__DeleteReceiverResponse &trv__DeleteReceiverResponse);
        //
        /// Web service synchronous operation 'ConfigureReceiver' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int ConfigureReceiver(_trv__ConfigureReceiver *trv__ConfigureReceiver, _trv__ConfigureReceiverResponse &trv__ConfigureReceiverResponse) { return this->ConfigureReceiver(NULL, NULL, trv__ConfigureReceiver, trv__ConfigureReceiverResponse); }
        /// Web service synchronous operation 'ConfigureReceiver' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int ConfigureReceiver(const char *soap_endpoint_url, const char *soap_action, _trv__ConfigureReceiver *trv__ConfigureReceiver, _trv__ConfigureReceiverResponse &trv__ConfigureReceiverResponse) { return this->send_ConfigureReceiver(soap_endpoint_url, soap_action, trv__ConfigureReceiver) || this->recv_ConfigureReceiver(trv__ConfigureReceiverResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_ConfigureReceiver' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_ConfigureReceiver(const char *soap_endpoint_url, const char *soap_action, _trv__ConfigureReceiver *trv__ConfigureReceiver);
        /// Web service asynchronous operation 'recv_ConfigureReceiver' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_ConfigureReceiver(_trv__ConfigureReceiverResponse &trv__ConfigureReceiverResponse);
        //
        /// Web service synchronous operation 'SetReceiverMode' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int SetReceiverMode(_trv__SetReceiverMode *trv__SetReceiverMode, _trv__SetReceiverModeResponse &trv__SetReceiverModeResponse) { return this->SetReceiverMode(NULL, NULL, trv__SetReceiverMode, trv__SetReceiverModeResponse); }
        /// Web service synchronous operation 'SetReceiverMode' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int SetReceiverMode(const char *soap_endpoint_url, const char *soap_action, _trv__SetReceiverMode *trv__SetReceiverMode, _trv__SetReceiverModeResponse &trv__SetReceiverModeResponse) { return this->send_SetReceiverMode(soap_endpoint_url, soap_action, trv__SetReceiverMode) || this->recv_SetReceiverMode(trv__SetReceiverModeResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_SetReceiverMode' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_SetReceiverMode(const char *soap_endpoint_url, const char *soap_action, _trv__SetReceiverMode *trv__SetReceiverMode);
        /// Web service asynchronous operation 'recv_SetReceiverMode' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_SetReceiverMode(_trv__SetReceiverModeResponse &trv__SetReceiverModeResponse);
        //
        /// Web service synchronous operation 'GetReceiverState' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetReceiverState(_trv__GetReceiverState *trv__GetReceiverState, _trv__GetReceiverStateResponse &trv__GetReceiverStateResponse) { return this->GetReceiverState(NULL, NULL, trv__GetReceiverState, trv__GetReceiverStateResponse); }
        /// Web service synchronous operation 'GetReceiverState' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetReceiverState(const char *soap_endpoint_url, const char *soap_action, _trv__GetReceiverState *trv__GetReceiverState, _trv__GetReceiverStateResponse &trv__GetReceiverStateResponse) { return this->send_GetReceiverState(soap_endpoint_url, soap_action, trv__GetReceiverState) || this->recv_GetReceiverState(trv__GetReceiverStateResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetReceiverState' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetReceiverState(const char *soap_endpoint_url, const char *soap_action, _trv__GetReceiverState *trv__GetReceiverState);
        /// Web service asynchronous operation 'recv_GetReceiverState' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetReceiverState(_trv__GetReceiverStateResponse &trv__GetReceiverStateResponse);
    };
#endif
