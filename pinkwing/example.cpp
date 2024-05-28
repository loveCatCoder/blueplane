#include "onvif/soapH.h"
#include "onvif/wsaapi.h"
#include "onvif/wsseapi.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


#include "onvif/wsdd.nsmap"



std::string USERNAME = "admin";
std::string PASSWORD = "admin123";

#define NAMESPACE_MEDIA_H264 "http://www.onvif.org/ver10/media/wsdl"
#define NAMESPACE_MEDIA_H265 "http://www.onvif.org/ver20/media/wsdl"

#define SOAP_ASSERT assert
#define SOAP_DBGLOG printf
#define SOAP_DBGERR printf

#define SOAP_TO "urn:schemas-xmlsoap-org:ws:2005:04:discovery"
#define SOAP_ACTION "http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe"

#define SOAP_MCAST_ADDR "soap.udp://239.255.255.250:3702" // onvif规定的组播地址

#define SOAP_ITEM ""                            // 寻找的设备范围
#define SOAP_TYPES "dn:NetworkVideoTransmitter" // 寻找的设备类型

#define SOAP_SOCK_TIMEOUT (10) // socket超时时间（单秒秒）

#define SOAP_CHECK_ERROR(result, soap, str)                                    \
    do {                                                                       \
        if (SOAP_OK != (result) || SOAP_OK != (soap)->error) {                 \
            soap_perror((soap), (str));                                        \
            if (SOAP_OK == (result)) {                                         \
                (result) = (soap)->error;                                      \
            }                                                                  \
            goto EXIT;                                                         \
        }                                                                      \
    } while (0)

void soap_perror(struct soap *soap, const char *str) {
    if (NULL == str) {
        SOAP_DBGERR("[soap] error: %d, %s, %s\n", soap->error,
                    *soap_faultcode(soap), *soap_faultstring(soap));
    } else {
        SOAP_DBGERR("[soap] %s error: %d, %s, %s\n", str, soap->error,
                    *soap_faultcode(soap), *soap_faultstring(soap));
    }
    return;
}

void *ONVIF_soap_malloc(struct soap *soap, unsigned int n) {
    void *p = NULL;

    if (n > 0) {
        p = soap_malloc(soap, n);
        SOAP_ASSERT(NULL != p);
        memset(p, 0x00, n);
    }
    return p;
}

struct soap *ONVIF_soap_new(int timeout) {
    struct soap *soap = NULL; // soap环境变量

    SOAP_ASSERT(NULL != (soap = soap_new()));

    soap_set_namespaces(soap, namespaces); // 设置soap的namespaces
    soap->recv_timeout = timeout; // 设置超时（超过指定时间没有数据就退出）
    soap->send_timeout = timeout;
    soap->connect_timeout = timeout;

#if defined(__linux__) ||                                                      \
    defined(__linux) // 参考https://www.genivia.com/dev.html#client-c的修改：
    soap->socket_flags = MSG_NOSIGNAL; // To prevent connection reset errors
#endif

    soap_set_mode(soap,
                  SOAP_C_UTFSTRING); // 设置为UTF-8编码，否则叠加中文OSD会乱码

    return soap;
}

void ONVIF_soap_delete(struct soap *soap) {
    soap_destroy(soap); // remove deserialized class instances (C++ only)
    soap_end(soap);  // Clean up deserialized data (except class instances) and
                     // temporary data
    soap_done(soap); // Reset, close communications, and remove callbacks
    soap_free(soap); // Reset and deallocate the context created with soap_new
                     // or soap_copy
}

/************************************************************************
**函数：ONVIF_init_header
**功能：初始化soap描述消息头
**参数：
        [in] soap - soap环境变量
**返回：无
**备注：
    1).
在本函数内部通过ONVIF_soap_malloc分配的内存，将在ONVIF_soap_delete中被释放
************************************************************************/
void ONVIF_init_header(struct soap *soap) {
    struct SOAP_ENV__Header *header = NULL;

    SOAP_ASSERT(NULL != soap);

    header = (struct SOAP_ENV__Header *)ONVIF_soap_malloc(
        soap, sizeof(struct SOAP_ENV__Header));
    soap_default_SOAP_ENV__Header(soap, header);
    header->wsa5__MessageID = (char *)soap_wsa_rand_uuid(soap);
    header->wsa5__To = (char *)ONVIF_soap_malloc(soap, strlen(SOAP_TO) + 1);
    header->wsa5__Action =
        (char *)ONVIF_soap_malloc(soap, strlen(SOAP_ACTION) + 1);
    strcpy(header->wsa5__To, SOAP_TO);
    strcpy(header->wsa5__Action, SOAP_ACTION);
    soap->header = header;

    return;
}


/************************************************************************
**函数：ONVIF_SetAuthInfo
**功能：设置认证信息
**参数：
        [in] soap     - soap环境变量
        [in] username - 用户名
        [in] password - 密码
**返回：
        0表明成功，非0表明失败
**备注：
************************************************************************/
static int ONVIF_SetAuthInfo(struct soap *soap, const char *username,
                             const char *password) {
    int result = 0;

    SOAP_ASSERT(NULL != username);
    SOAP_ASSERT(NULL != password);

    result = soap_wsse_add_UsernameTokenDigest(soap, NULL, username, password);
    SOAP_CHECK_ERROR(result, soap, "add_UsernameTokenDigest");

EXIT:

    return result;
}

/************************************************************************
**函数：ONVIF_GetDeviceInformation
**功能：获取设备基本信息
**参数：
        [in] DeviceXAddr - 设备服务地址
**返回：
        0表明成功，非0表明失败
**备注：
************************************************************************/
int ONVIF_GetDeviceInformation(const char *DeviceXAddr) {
    printf("NTP --\n");
    int result = 0;
    struct soap *soap = NULL;
    struct _tds__GetDeviceInformation devinfo_req;
    struct _tds__GetDeviceInformationResponse devinfo_resp;

    SOAP_ASSERT(NULL != DeviceXAddr);
    SOAP_ASSERT(NULL != (soap = ONVIF_soap_new(SOAP_SOCK_TIMEOUT)));

    ONVIF_SetAuthInfo(soap, USERNAME.c_str(), PASSWORD.c_str());

    result = soap_call___tds__GetDeviceInformation(soap, DeviceXAddr, NULL,
                                                   &devinfo_req, devinfo_resp);
    SOAP_CHECK_ERROR(result, soap, "GetDeviceInformation");

    // dump_tds__GetDeviceInformationResponse(&devinfo_resp);
    printf("Manufacturer:%s\n", devinfo_resp.Manufacturer.data());
    printf("Model:%s\n", devinfo_resp.Model.data());
    printf("FirmwareVersion:%s\n", devinfo_resp.FirmwareVersion.data());
    printf("SerialNumber:%s\n", devinfo_resp.SerialNumber.data());
    printf("HardwareId:%s\n", devinfo_resp.HardwareId.data());

EXIT:

    if (NULL != soap) {
        ONVIF_soap_delete(soap);
    }
    return result;
}

/************************************************************************
**函数：ONVIF_GetDeviceInformation
**功能：获取设备基本信息
**参数：
        [in] DeviceXAddr - 设备服务地址
**返回：
        0表明成功，非0表明失败
**备注：
************************************************************************/
int ONVIF_GetNtp(const char *DeviceXAddr) {
    printf("NTP --\n");
    int result = 0;
    struct soap *soap = NULL;
    struct _tds__GetNTP devntp_req;
    struct _tds__GetNTPResponse devntp_resp;

    SOAP_ASSERT(NULL != DeviceXAddr);
    SOAP_ASSERT(NULL != (soap = ONVIF_soap_new(SOAP_SOCK_TIMEOUT)));

    ONVIF_SetAuthInfo(soap, USERNAME.c_str(), PASSWORD.c_str());

    result = soap_call___tds__GetNTP(soap, DeviceXAddr, NULL, &devntp_req,
                                     devntp_resp);
    SOAP_CHECK_ERROR(result, soap, "ONVIF_GetNtp");

    if (devntp_resp.NTPInformation) {
        for (auto &i : devntp_resp.NTPInformation->NTPManual) {
            if (i->IPv4Address) {
                printf("ntp manual :%s\n", i->IPv4Address->data());
            }
        }
    } 

EXIT:

    if (NULL != soap) {
        ONVIF_soap_delete(soap);
    }
    return result;
}

int ONVIF_GetCapabilities(const char *DeviceXAddr) {
    printf("GetCapabilities --\n");
    int result = 0;
    struct soap *soap = NULL;
    struct _tds__GetCapabilities devcap_req;
    struct _tds__GetCapabilitiesResponse devcap_resp;

    SOAP_ASSERT(NULL != DeviceXAddr);
    SOAP_ASSERT(NULL != (soap = ONVIF_soap_new(SOAP_SOCK_TIMEOUT)));

    ONVIF_SetAuthInfo(soap, USERNAME.c_str(), PASSWORD.c_str());

    // memset(&devinfo_req, 0x00, sizeof(devinfo_req));
    // memset(&devinfo_resp, 0x00, sizeof(devinfo_resp));
    result = soap_call___tds__GetCapabilities(soap, DeviceXAddr, NULL,
                                              &devcap_req, devcap_resp);
    SOAP_CHECK_ERROR(result, soap, "GetCapabilities");

    // tt__AnalyticsCapabilities *Analytics;
    //     /// Optional element 'tt:Device' of XML schema type
    //     'tt:DeviceCapabilities' tt__DeviceCapabilities *Device;
    //     /// Optional element 'tt:Events' of XML schema type
    //     'tt:EventCapabilities' tt__EventCapabilities *Events;
    //     /// Optional element 'tt:Imaging' of XML schema type
    //     'tt:ImagingCapabilities' tt__ImagingCapabilities *Imaging;
    //     /// Optional element 'tt:Media' of XML schema type
    //     'tt:MediaCapabilities' tt__MediaCapabilities *Media;
    //     /// Optional element 'tt:PTZ' of XML schema type 'tt:PTZCapabilities'
    //     tt__PTZCapabilities *PTZ;

    if (devcap_resp.Capabilities->Analytics) {
        printf("%s capabilities includes tt__AnalyticsCapabilities %s\n",
               DeviceXAddr, devcap_resp.Capabilities->Analytics->XAddr.data());
    }

    if (devcap_resp.Capabilities->Device) {
        printf("%s capabilities includes tt__DeviceCapabilities %s\n",
               DeviceXAddr, devcap_resp.Capabilities->Device->XAddr.data());
    }

    if (devcap_resp.Capabilities->Events) {
        printf("%s capabilities includes tt__EventCapabilities %s\n",
               DeviceXAddr, devcap_resp.Capabilities->Events->XAddr.data());
    }

    if (devcap_resp.Capabilities->Imaging) {
        printf("%s capabilities includes tt__ImagingCapabilities %s\n",
               DeviceXAddr, devcap_resp.Capabilities->Imaging->XAddr.data());
    }

    if (devcap_resp.Capabilities->Media) {
        printf("%s capabilities includes tt__MediaCapabilities %s\n",
               DeviceXAddr, devcap_resp.Capabilities->Media->XAddr.data());
    }

    if (devcap_resp.Capabilities->PTZ != nullptr) {
        printf("%s capabilities includes tt__PTZCapabilities %s\n", DeviceXAddr,
               devcap_resp.Capabilities->PTZ->XAddr.data());
    }

EXIT:

    if (NULL != soap) {
        ONVIF_soap_delete(soap);
    }
    return result;
}

int ONVIF_GetStreamUri(const char *DeviceXAddr, char *token) {
    printf("ONVIF_GetStreamUri --\n");
    int result = 0;
    struct soap *soap = NULL;
    struct tt__StreamSetup ttStreamSetup;
    struct tt__Transport ttTransport;

    struct _trt__GetStreamUri devstrm_req;
    struct _trt__GetStreamUriResponse devstrm_resp;

    SOAP_ASSERT(NULL != DeviceXAddr);
    SOAP_ASSERT(NULL != (soap = ONVIF_soap_new(SOAP_SOCK_TIMEOUT)));

    ONVIF_SetAuthInfo(soap, USERNAME.c_str(), PASSWORD.c_str());

    //     enum class tt__StreamType {
    // 	RTP_Unicast = 0,
    // 	RTP_Multicast = 1
    // };
    // enum class tt__TransportProtocol {
    // 	UDP = 0,
    // 	TCP = 1,
    // 	RTSP = 2,
    // 	HTTP = 3
    // };
    ttStreamSetup.Stream = tt__StreamType::tt__StreamType__RTP_Unicast;
    ttStreamSetup.Transport = &ttTransport;
    ttStreamSetup.Transport->Protocol = tt__TransportProtocol::tt__TransportProtocol__RTSP;
    ttStreamSetup.Transport->Tunnel = NULL;
    devstrm_req.StreamSetup = &ttStreamSetup;
    devstrm_req.ProfileToken = token;

    result = soap_call___trt__GetStreamUri(soap, DeviceXAddr, NULL,
                                           &devstrm_req, devstrm_resp);
    SOAP_CHECK_ERROR(result, soap, "ONVIF_GetStreamUri");

    if (devstrm_resp.MediaUri) {
        printf("%s GetStreamUri %s\n", DeviceXAddr,
               devstrm_resp.MediaUri->Uri.data());
    }

EXIT:

    if (NULL != soap) {
        ONVIF_soap_delete(soap);
    }
    return result;
}

int ONVIF_GetStreamUri2(const char *media_xaddr, char *token) {
    printf("ONVIF_GetStreamUri2 --\n");
    int result = 0;
    struct soap *soap = NULL;

    struct _ns3__GetStreamUri devstrm_req;
    struct _ns3__GetStreamUriResponse devstrm_resp;

    SOAP_ASSERT(NULL != media_xaddr);
    SOAP_ASSERT(NULL != (soap = ONVIF_soap_new(SOAP_SOCK_TIMEOUT)));

    ONVIF_SetAuthInfo(soap, USERNAME.c_str(), PASSWORD.c_str());

    devstrm_req.Protocol = "RTSP";
    devstrm_req.ProfileToken = token;

    result = soap_call___ns3__GetStreamUri(soap, media_xaddr, NULL,
                                           &devstrm_req, devstrm_resp);
    SOAP_CHECK_ERROR(result, soap, "ONVIF_GetStreamUri2");

    printf("%s ONVIF_GetStreamUri2 %s\n", media_xaddr, devstrm_resp.Uri.data());

EXIT:

    if (NULL != soap) {
        ONVIF_soap_delete(soap);
    }
    return result;
}

int ONVIF_GetProfiles(const char *media_xaddr) {
    //  xmlns:trt="http://www.onvif.org/ver10/media/wsdl
    //  xmlns:ns1="http://www.onvif.org/ver20/media/wsdl"
    printf("GetProfiles --\n");
    int result = 0;
    struct soap *soap = NULL;
    // 不支持H265
    struct _trt__GetProfiles devpro_req;
    struct _trt__GetProfilesResponse devpro_resp;

    SOAP_ASSERT(NULL != media_xaddr);
    SOAP_ASSERT(NULL != (soap = ONVIF_soap_new(SOAP_SOCK_TIMEOUT)));

    ONVIF_SetAuthInfo(soap, USERNAME.c_str(), PASSWORD.c_str());

    result = soap_call___trt__GetProfiles(soap, media_xaddr, NULL, &devpro_req,
                                          devpro_resp);

    SOAP_CHECK_ERROR(result, soap, "ONVIF_GetProfiles");

    for (auto &i : devpro_resp.Profiles) {
        printf("%s profile %s encoder:%d ,width:%d ,height:%d frameRate:%d\n", media_xaddr, i->token.data(),i->VideoEncoderConfiguration->Encoding,
        i->VideoEncoderConfiguration->Resolution->Width,i->VideoEncoderConfiguration->Resolution->Height,i->VideoEncoderConfiguration->RateControl->FrameRateLimit);
        ONVIF_GetStreamUri(media_xaddr,const_cast<char*> (i->token.data()));
    }

EXIT:

    if (NULL != soap) {
        ONVIF_soap_delete(soap);
    }
    return result;
}

int ONVIF_GetProfiles2(const char *media_xaddr) {
    //  xmlns:trt="http://www.onvif.org/ver10/media/wsdl
    //  xmlns:ns1="http://www.onvif.org/ver20/media/wsdl"
    printf("GetProfiles2 --\n");
    int result = 0;
    struct soap *soap = NULL;

    SOAP_ASSERT(NULL != media_xaddr);
    SOAP_ASSERT(NULL != (soap = ONVIF_soap_new(SOAP_SOCK_TIMEOUT)));

    // 支持H265
    struct _ns3__GetProfiles devpro2_req;
    struct _ns3__GetProfilesResponse devpro2_resp;

    devpro2_req.Type.push_back("VideoEncoder");
    devpro2_req.Type.push_back("Metadata");

    ONVIF_SetAuthInfo(soap, USERNAME.c_str(), PASSWORD.c_str());
    result = soap_call___ns3__GetProfiles(soap, media_xaddr, NULL, &devpro2_req,
                                          devpro2_resp);

    SOAP_CHECK_ERROR(result, soap, "ONVIF_GetProfiles2");

    for (auto &i : devpro2_resp.Profiles) {
        printf("%s profile2 %s, %s\n", media_xaddr, i->Name.data(),
               i->token.data());
        printf("format:%s,width:%d,height:%d,rate:%f\n",i->Configurations->VideoEncoder->Encoding.c_str(),i->Configurations->VideoEncoder->Resolution->Width,
        i->Configurations->VideoEncoder->Resolution->Height,i->Configurations->VideoEncoder->RateControl->FrameRateLimit);
        ONVIF_GetStreamUri2(media_xaddr, const_cast<char*> (i->token.data()));
    }

EXIT:

    if (NULL != soap) {
        ONVIF_soap_delete(soap);
    }
    return result;
}

int ONVIF_GetServices(const char *DeviceXAddr) {
    printf("ONVIF_GetServices --\n");
    int result = 0;
    struct soap *soap = NULL;
    struct _tds__GetServices devsrv_req;
    struct _tds__GetServicesResponse devsrv_resp;

    SOAP_ASSERT(NULL != DeviceXAddr);
    SOAP_ASSERT(NULL != (soap = ONVIF_soap_new(SOAP_SOCK_TIMEOUT)));

    ONVIF_SetAuthInfo(soap, USERNAME.c_str(), PASSWORD.c_str());

    devsrv_req.IncludeCapability = true;

    result = soap_call___tds__GetServices(soap, DeviceXAddr, NULL, &devsrv_req,
                                          devsrv_resp);
    SOAP_CHECK_ERROR(result, soap, "ONVIF_GetServices");

    for (auto &i : devsrv_resp.Service) {
        printf("%s service %s, %s\n", DeviceXAddr, i->Namespace.data(),
               i->XAddr.data());

        if (i->Namespace == NAMESPACE_MEDIA_H265) {
            ONVIF_GetProfiles2(i->XAddr.data());
        } else if (i->Namespace == NAMESPACE_MEDIA_H264) {
            ONVIF_GetProfiles(i->XAddr.data());
        }
    }

EXIT:

    if (NULL != soap) {
        ONVIF_soap_delete(soap);
    }
    return result;
}


// ONVIF_GetDeviceInformation(DeviceXAddr);
// ONVIF_GetNtp(DeviceXAddr);
// ONVIF_GetCapabilities(DeviceXAddr);
// ONVIF_GetServices(DeviceXAddr);


int main(int argc, char **argv) {
    if(argc == 1)
    {
        return 0;
    }
    char DeviceXAddr[255] = {0};
    sprintf(DeviceXAddr, "http://%s/onvif/device_service", argv[1]);

    if(argc ==4 )
    {
        USERNAME = argv[2];
        PASSWORD = argv[3];
    }

    ONVIF_GetDeviceInformation(DeviceXAddr);
    ONVIF_GetNtp(DeviceXAddr);
    ONVIF_GetCapabilities(DeviceXAddr);
    ONVIF_GetServices(DeviceXAddr);
    return 0;
}