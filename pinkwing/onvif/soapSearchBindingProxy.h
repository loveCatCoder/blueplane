/* soapSearchBindingProxy.h
   Generated by gSOAP 2.8.134 for samples/onvif/onvif.h

gSOAP XML Web services tools
Copyright (C) 2000-2024, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#ifndef soapSearchBindingProxy_H
#define soapSearchBindingProxy_H
#include "soapH.h"

    class SOAP_CMAC SearchBindingProxy {
      public:
        /// Context to manage proxy IO and data
        struct soap *soap;
        /// flag indicating that this context is owned by this proxy and should be deleted by the destructor
        bool soap_own;
        /// Endpoint URL of service 'SearchBindingProxy' (change as needed)
        const char *soap_endpoint;
        /// Variables globally declared in samples/onvif/onvif.h, if any
        /// Construct a proxy with new managing context
        SearchBindingProxy();
        /// Copy constructor
        SearchBindingProxy(const SearchBindingProxy& rhs);
        /// Construct proxy given a shared managing context
        SearchBindingProxy(struct soap*);
        /// Construct proxy given a shared managing context and endpoint URL
        SearchBindingProxy(struct soap*, const char *soap_endpoint_url);
        /// Constructor taking an endpoint URL
        SearchBindingProxy(const char *soap_endpoint_url);
        /// Constructor taking input and output mode flags for the new managing context
        SearchBindingProxy(soap_mode iomode);
        /// Constructor taking endpoint URL and input and output mode flags for the new managing context
        SearchBindingProxy(const char *soap_endpoint_url, soap_mode iomode);
        /// Constructor taking input and output mode flags for the new managing context
        SearchBindingProxy(soap_mode imode, soap_mode omode);
        /// Destructor deletes deserialized data and its managing context, when the context was allocated by the constructor
        virtual ~SearchBindingProxy();
        /// Initializer used by constructors
        virtual void SearchBindingProxy_init(soap_mode imode, soap_mode omode);
        /// Return a copy that has a new managing context with the same engine state
        virtual SearchBindingProxy *copy();
        /// Copy assignment
        SearchBindingProxy& operator=(const SearchBindingProxy&);
        /// Delete all deserialized data (uses soap_destroy() and soap_end())
        virtual void destroy();
        /// Delete all deserialized data and reset to default
        virtual void reset();
        /// Disables and removes SOAP Header from message by setting soap->header = NULL
        virtual void soap_noheader();
        /// Add SOAP Header to message
        virtual void soap_header(struct _wsse__Security *wsse__Security, char *wsa__MessageID, struct wsa__Relationship *wsa__RelatesTo, struct wsa__EndpointReferenceType *wsa__From, struct wsa__EndpointReferenceType *wsa__ReplyTo, struct wsa__EndpointReferenceType *wsa__FaultTo, char *wsa__To, char *wsa__Action, struct wsdd__AppSequenceType *wsdd__AppSequence, char *wsa5__MessageID, struct wsa5__RelatesToType *wsa5__RelatesTo, struct wsa5__EndpointReferenceType *wsa5__From, struct wsa5__EndpointReferenceType *wsa5__ReplyTo, struct wsa5__EndpointReferenceType *wsa5__FaultTo, char *wsa5__To, char *wsa5__Action, struct chan__ChannelInstanceType *chan__ChannelInstance);
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
        virtual int GetServiceCapabilities(_tse__GetServiceCapabilities *tse__GetServiceCapabilities, _tse__GetServiceCapabilitiesResponse &tse__GetServiceCapabilitiesResponse) { return this->GetServiceCapabilities(NULL, NULL, tse__GetServiceCapabilities, tse__GetServiceCapabilitiesResponse); }
        /// Web service synchronous operation 'GetServiceCapabilities' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetServiceCapabilities(const char *soap_endpoint_url, const char *soap_action, _tse__GetServiceCapabilities *tse__GetServiceCapabilities, _tse__GetServiceCapabilitiesResponse &tse__GetServiceCapabilitiesResponse) { return this->send_GetServiceCapabilities(soap_endpoint_url, soap_action, tse__GetServiceCapabilities) || this->recv_GetServiceCapabilities(tse__GetServiceCapabilitiesResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetServiceCapabilities' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetServiceCapabilities(const char *soap_endpoint_url, const char *soap_action, _tse__GetServiceCapabilities *tse__GetServiceCapabilities);
        /// Web service asynchronous operation 'recv_GetServiceCapabilities' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetServiceCapabilities(_tse__GetServiceCapabilitiesResponse &tse__GetServiceCapabilitiesResponse);
        //
        /// Web service synchronous operation 'GetRecordingSummary' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetRecordingSummary(_tse__GetRecordingSummary *tse__GetRecordingSummary, _tse__GetRecordingSummaryResponse &tse__GetRecordingSummaryResponse) { return this->GetRecordingSummary(NULL, NULL, tse__GetRecordingSummary, tse__GetRecordingSummaryResponse); }
        /// Web service synchronous operation 'GetRecordingSummary' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetRecordingSummary(const char *soap_endpoint_url, const char *soap_action, _tse__GetRecordingSummary *tse__GetRecordingSummary, _tse__GetRecordingSummaryResponse &tse__GetRecordingSummaryResponse) { return this->send_GetRecordingSummary(soap_endpoint_url, soap_action, tse__GetRecordingSummary) || this->recv_GetRecordingSummary(tse__GetRecordingSummaryResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetRecordingSummary' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetRecordingSummary(const char *soap_endpoint_url, const char *soap_action, _tse__GetRecordingSummary *tse__GetRecordingSummary);
        /// Web service asynchronous operation 'recv_GetRecordingSummary' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetRecordingSummary(_tse__GetRecordingSummaryResponse &tse__GetRecordingSummaryResponse);
        //
        /// Web service synchronous operation 'GetRecordingInformation' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetRecordingInformation(_tse__GetRecordingInformation *tse__GetRecordingInformation, _tse__GetRecordingInformationResponse &tse__GetRecordingInformationResponse) { return this->GetRecordingInformation(NULL, NULL, tse__GetRecordingInformation, tse__GetRecordingInformationResponse); }
        /// Web service synchronous operation 'GetRecordingInformation' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetRecordingInformation(const char *soap_endpoint_url, const char *soap_action, _tse__GetRecordingInformation *tse__GetRecordingInformation, _tse__GetRecordingInformationResponse &tse__GetRecordingInformationResponse) { return this->send_GetRecordingInformation(soap_endpoint_url, soap_action, tse__GetRecordingInformation) || this->recv_GetRecordingInformation(tse__GetRecordingInformationResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetRecordingInformation' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetRecordingInformation(const char *soap_endpoint_url, const char *soap_action, _tse__GetRecordingInformation *tse__GetRecordingInformation);
        /// Web service asynchronous operation 'recv_GetRecordingInformation' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetRecordingInformation(_tse__GetRecordingInformationResponse &tse__GetRecordingInformationResponse);
        //
        /// Web service synchronous operation 'GetMediaAttributes' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetMediaAttributes(_tse__GetMediaAttributes *tse__GetMediaAttributes, _tse__GetMediaAttributesResponse &tse__GetMediaAttributesResponse) { return this->GetMediaAttributes(NULL, NULL, tse__GetMediaAttributes, tse__GetMediaAttributesResponse); }
        /// Web service synchronous operation 'GetMediaAttributes' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetMediaAttributes(const char *soap_endpoint_url, const char *soap_action, _tse__GetMediaAttributes *tse__GetMediaAttributes, _tse__GetMediaAttributesResponse &tse__GetMediaAttributesResponse) { return this->send_GetMediaAttributes(soap_endpoint_url, soap_action, tse__GetMediaAttributes) || this->recv_GetMediaAttributes(tse__GetMediaAttributesResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetMediaAttributes' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetMediaAttributes(const char *soap_endpoint_url, const char *soap_action, _tse__GetMediaAttributes *tse__GetMediaAttributes);
        /// Web service asynchronous operation 'recv_GetMediaAttributes' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetMediaAttributes(_tse__GetMediaAttributesResponse &tse__GetMediaAttributesResponse);
        //
        /// Web service synchronous operation 'FindRecordings' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int FindRecordings(_tse__FindRecordings *tse__FindRecordings, _tse__FindRecordingsResponse &tse__FindRecordingsResponse) { return this->FindRecordings(NULL, NULL, tse__FindRecordings, tse__FindRecordingsResponse); }
        /// Web service synchronous operation 'FindRecordings' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int FindRecordings(const char *soap_endpoint_url, const char *soap_action, _tse__FindRecordings *tse__FindRecordings, _tse__FindRecordingsResponse &tse__FindRecordingsResponse) { return this->send_FindRecordings(soap_endpoint_url, soap_action, tse__FindRecordings) || this->recv_FindRecordings(tse__FindRecordingsResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_FindRecordings' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_FindRecordings(const char *soap_endpoint_url, const char *soap_action, _tse__FindRecordings *tse__FindRecordings);
        /// Web service asynchronous operation 'recv_FindRecordings' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_FindRecordings(_tse__FindRecordingsResponse &tse__FindRecordingsResponse);
        //
        /// Web service synchronous operation 'GetRecordingSearchResults' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetRecordingSearchResults(_tse__GetRecordingSearchResults *tse__GetRecordingSearchResults, _tse__GetRecordingSearchResultsResponse &tse__GetRecordingSearchResultsResponse) { return this->GetRecordingSearchResults(NULL, NULL, tse__GetRecordingSearchResults, tse__GetRecordingSearchResultsResponse); }
        /// Web service synchronous operation 'GetRecordingSearchResults' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetRecordingSearchResults(const char *soap_endpoint_url, const char *soap_action, _tse__GetRecordingSearchResults *tse__GetRecordingSearchResults, _tse__GetRecordingSearchResultsResponse &tse__GetRecordingSearchResultsResponse) { return this->send_GetRecordingSearchResults(soap_endpoint_url, soap_action, tse__GetRecordingSearchResults) || this->recv_GetRecordingSearchResults(tse__GetRecordingSearchResultsResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetRecordingSearchResults' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetRecordingSearchResults(const char *soap_endpoint_url, const char *soap_action, _tse__GetRecordingSearchResults *tse__GetRecordingSearchResults);
        /// Web service asynchronous operation 'recv_GetRecordingSearchResults' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetRecordingSearchResults(_tse__GetRecordingSearchResultsResponse &tse__GetRecordingSearchResultsResponse);
        //
        /// Web service synchronous operation 'FindEvents' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int FindEvents(_tse__FindEvents *tse__FindEvents, _tse__FindEventsResponse &tse__FindEventsResponse) { return this->FindEvents(NULL, NULL, tse__FindEvents, tse__FindEventsResponse); }
        /// Web service synchronous operation 'FindEvents' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int FindEvents(const char *soap_endpoint_url, const char *soap_action, _tse__FindEvents *tse__FindEvents, _tse__FindEventsResponse &tse__FindEventsResponse) { return this->send_FindEvents(soap_endpoint_url, soap_action, tse__FindEvents) || this->recv_FindEvents(tse__FindEventsResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_FindEvents' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_FindEvents(const char *soap_endpoint_url, const char *soap_action, _tse__FindEvents *tse__FindEvents);
        /// Web service asynchronous operation 'recv_FindEvents' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_FindEvents(_tse__FindEventsResponse &tse__FindEventsResponse);
        //
        /// Web service synchronous operation 'GetEventSearchResults' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetEventSearchResults(_tse__GetEventSearchResults *tse__GetEventSearchResults, _tse__GetEventSearchResultsResponse &tse__GetEventSearchResultsResponse) { return this->GetEventSearchResults(NULL, NULL, tse__GetEventSearchResults, tse__GetEventSearchResultsResponse); }
        /// Web service synchronous operation 'GetEventSearchResults' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetEventSearchResults(const char *soap_endpoint_url, const char *soap_action, _tse__GetEventSearchResults *tse__GetEventSearchResults, _tse__GetEventSearchResultsResponse &tse__GetEventSearchResultsResponse) { return this->send_GetEventSearchResults(soap_endpoint_url, soap_action, tse__GetEventSearchResults) || this->recv_GetEventSearchResults(tse__GetEventSearchResultsResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetEventSearchResults' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetEventSearchResults(const char *soap_endpoint_url, const char *soap_action, _tse__GetEventSearchResults *tse__GetEventSearchResults);
        /// Web service asynchronous operation 'recv_GetEventSearchResults' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetEventSearchResults(_tse__GetEventSearchResultsResponse &tse__GetEventSearchResultsResponse);
        //
        /// Web service synchronous operation 'FindPTZPosition' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int FindPTZPosition(_tse__FindPTZPosition *tse__FindPTZPosition, _tse__FindPTZPositionResponse &tse__FindPTZPositionResponse) { return this->FindPTZPosition(NULL, NULL, tse__FindPTZPosition, tse__FindPTZPositionResponse); }
        /// Web service synchronous operation 'FindPTZPosition' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int FindPTZPosition(const char *soap_endpoint_url, const char *soap_action, _tse__FindPTZPosition *tse__FindPTZPosition, _tse__FindPTZPositionResponse &tse__FindPTZPositionResponse) { return this->send_FindPTZPosition(soap_endpoint_url, soap_action, tse__FindPTZPosition) || this->recv_FindPTZPosition(tse__FindPTZPositionResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_FindPTZPosition' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_FindPTZPosition(const char *soap_endpoint_url, const char *soap_action, _tse__FindPTZPosition *tse__FindPTZPosition);
        /// Web service asynchronous operation 'recv_FindPTZPosition' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_FindPTZPosition(_tse__FindPTZPositionResponse &tse__FindPTZPositionResponse);
        //
        /// Web service synchronous operation 'GetPTZPositionSearchResults' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetPTZPositionSearchResults(_tse__GetPTZPositionSearchResults *tse__GetPTZPositionSearchResults, _tse__GetPTZPositionSearchResultsResponse &tse__GetPTZPositionSearchResultsResponse) { return this->GetPTZPositionSearchResults(NULL, NULL, tse__GetPTZPositionSearchResults, tse__GetPTZPositionSearchResultsResponse); }
        /// Web service synchronous operation 'GetPTZPositionSearchResults' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetPTZPositionSearchResults(const char *soap_endpoint_url, const char *soap_action, _tse__GetPTZPositionSearchResults *tse__GetPTZPositionSearchResults, _tse__GetPTZPositionSearchResultsResponse &tse__GetPTZPositionSearchResultsResponse) { return this->send_GetPTZPositionSearchResults(soap_endpoint_url, soap_action, tse__GetPTZPositionSearchResults) || this->recv_GetPTZPositionSearchResults(tse__GetPTZPositionSearchResultsResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetPTZPositionSearchResults' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetPTZPositionSearchResults(const char *soap_endpoint_url, const char *soap_action, _tse__GetPTZPositionSearchResults *tse__GetPTZPositionSearchResults);
        /// Web service asynchronous operation 'recv_GetPTZPositionSearchResults' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetPTZPositionSearchResults(_tse__GetPTZPositionSearchResultsResponse &tse__GetPTZPositionSearchResultsResponse);
        //
        /// Web service synchronous operation 'GetSearchState' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetSearchState(_tse__GetSearchState *tse__GetSearchState, _tse__GetSearchStateResponse &tse__GetSearchStateResponse) { return this->GetSearchState(NULL, NULL, tse__GetSearchState, tse__GetSearchStateResponse); }
        /// Web service synchronous operation 'GetSearchState' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetSearchState(const char *soap_endpoint_url, const char *soap_action, _tse__GetSearchState *tse__GetSearchState, _tse__GetSearchStateResponse &tse__GetSearchStateResponse) { return this->send_GetSearchState(soap_endpoint_url, soap_action, tse__GetSearchState) || this->recv_GetSearchState(tse__GetSearchStateResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetSearchState' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetSearchState(const char *soap_endpoint_url, const char *soap_action, _tse__GetSearchState *tse__GetSearchState);
        /// Web service asynchronous operation 'recv_GetSearchState' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetSearchState(_tse__GetSearchStateResponse &tse__GetSearchStateResponse);
        //
        /// Web service synchronous operation 'EndSearch' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int EndSearch(_tse__EndSearch *tse__EndSearch, _tse__EndSearchResponse &tse__EndSearchResponse) { return this->EndSearch(NULL, NULL, tse__EndSearch, tse__EndSearchResponse); }
        /// Web service synchronous operation 'EndSearch' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int EndSearch(const char *soap_endpoint_url, const char *soap_action, _tse__EndSearch *tse__EndSearch, _tse__EndSearchResponse &tse__EndSearchResponse) { return this->send_EndSearch(soap_endpoint_url, soap_action, tse__EndSearch) || this->recv_EndSearch(tse__EndSearchResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_EndSearch' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_EndSearch(const char *soap_endpoint_url, const char *soap_action, _tse__EndSearch *tse__EndSearch);
        /// Web service asynchronous operation 'recv_EndSearch' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_EndSearch(_tse__EndSearchResponse &tse__EndSearchResponse);
        //
        /// Web service synchronous operation 'FindMetadata' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int FindMetadata(_tse__FindMetadata *tse__FindMetadata, _tse__FindMetadataResponse &tse__FindMetadataResponse) { return this->FindMetadata(NULL, NULL, tse__FindMetadata, tse__FindMetadataResponse); }
        /// Web service synchronous operation 'FindMetadata' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int FindMetadata(const char *soap_endpoint_url, const char *soap_action, _tse__FindMetadata *tse__FindMetadata, _tse__FindMetadataResponse &tse__FindMetadataResponse) { return this->send_FindMetadata(soap_endpoint_url, soap_action, tse__FindMetadata) || this->recv_FindMetadata(tse__FindMetadataResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_FindMetadata' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_FindMetadata(const char *soap_endpoint_url, const char *soap_action, _tse__FindMetadata *tse__FindMetadata);
        /// Web service asynchronous operation 'recv_FindMetadata' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_FindMetadata(_tse__FindMetadataResponse &tse__FindMetadataResponse);
        //
        /// Web service synchronous operation 'GetMetadataSearchResults' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetMetadataSearchResults(_tse__GetMetadataSearchResults *tse__GetMetadataSearchResults, _tse__GetMetadataSearchResultsResponse &tse__GetMetadataSearchResultsResponse) { return this->GetMetadataSearchResults(NULL, NULL, tse__GetMetadataSearchResults, tse__GetMetadataSearchResultsResponse); }
        /// Web service synchronous operation 'GetMetadataSearchResults' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetMetadataSearchResults(const char *soap_endpoint_url, const char *soap_action, _tse__GetMetadataSearchResults *tse__GetMetadataSearchResults, _tse__GetMetadataSearchResultsResponse &tse__GetMetadataSearchResultsResponse) { return this->send_GetMetadataSearchResults(soap_endpoint_url, soap_action, tse__GetMetadataSearchResults) || this->recv_GetMetadataSearchResults(tse__GetMetadataSearchResultsResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetMetadataSearchResults' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetMetadataSearchResults(const char *soap_endpoint_url, const char *soap_action, _tse__GetMetadataSearchResults *tse__GetMetadataSearchResults);
        /// Web service asynchronous operation 'recv_GetMetadataSearchResults' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetMetadataSearchResults(_tse__GetMetadataSearchResultsResponse &tse__GetMetadataSearchResultsResponse);
    };
#endif
