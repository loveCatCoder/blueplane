#ifndef __20190529_SIP_FSM_H__
#define __20190529_SIP_FSM_H__
		
#include "sip_message.h"
#include "sip_type.h"
		
#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	SIP_REQUEST_NONE            = 1,
	SIP_REQUEST_REGISTER        ,
	SIP_REQUEST_OPTIONS         ,
	SIP_REQUEST_MESSAGE         ,
	SIP_REQUEST_SUBSCRIBE       ,
	SIP_REQUEST_NOTIFY          ,
	SIP_REQUEST_INFO            ,
	SIP_REQUEST_INVITE          ,
	SIP_REQUEST_BYE             ,
	SIP_REQUEST_CANCEL          ,
	
	SIP_REQUEST_ACK_301         = 21,
	SIP_REQUEST_ACK_400         ,
	SIP_REQUEST_ACK_401         ,
	SIP_REQUEST_ACK_200         ,
	SIP_REQUEST_ACK             ,
	SIP_REQUEST_200OK           ,
	SIP_REQUEST_GET_MESSAGE     ,
	SIP_REQUEST_ACK_403         ,
	SIP_REQUEST_100TRYING       ,
	SIP_REQUEST_ACK_487         ,
	SIP_REQUEST_ACK_200_INVITE  ,	
	SIP_REQUEST_MAX         

}SIP_REQUEST_TYPE;


typedef enum
{
	SIP_XML_CMD_NONE					= 1,
	SIP_XML_CMD_CONTROL				,
	SIP_XML_CMD_QUERY				,
	SIP_XML_CMD_NOTIFY				,
	SIP_XML_CMD_RESPONSE			,
	SIP_XML_CMD_MAX
	
}SIP_XML_CMD_TYPE;


typedef enum
{
	SIP_XML_CMD_CTL_NONE					= 1,
	SIP_XML_CMD_CTL_DEVICE_CONTROL				,
	SIP_XML_CMD_CTL_DEVICE_CONFIG				,
	SIP_XML_CMD_CTL_MAX
	
}SIP_XML_CMD_CONTROL_TYPE;

typedef enum
{
	SIP_XML_CMD_QUERY_NONE					= 1,
	SIP_XML_CMD_QUERY_DEVICE_STATUS				,
	SIP_XML_CMD_QUERY_CATALOG				,
	SIP_XML_CMD_QUERY_DEVICE_INFO				,
	SIP_XML_CMD_QUERY_RECORD_INFO				,
	SIP_XML_CMD_QUERY_ALARM				,
	SIP_XML_CMD_QUERY_CONFIG_DOWNLOAD		,
	SIP_XML_CMD_QUERY_PRESET_QUERY		,
	SIP_XML_CMD_QUERY_MOBILE_POSITION		,

	SIP_XML_CMD_QUERY_MAX
	
}SIP_XML_CMD_QUERY_TYPE;

typedef enum
{
	SIP_XML_CMD_NOTIFY_NONE					= 1,
	SIP_XML_CMD_NOTIFY_KEEPALIVE				,
	SIP_XML_CMD_NOTIFY_ALARM				,
	SIP_XML_CMD_NOTIFY_MEDIA_STATUS				,
	SIP_XML_CMD_NOTIFY_BROADCAST				,	
	SIP_XML_CMD_NOTIFY_MOBILE_POSITION				,	
	SIP_XML_CMD_NOTIFY_CATALOG				,
	SIP_XML_CMD_NOTIFY_MAX
	
}SIP_XML_CMD_NOTIFY_TYPE;
typedef enum
{
	SIP_XML_CMD_RES_NONE					= 1,
	SIP_XML_CMD_RES_DEVICE_CONTROL				,
	SIP_XML_CMD_RES_ALARM				,
	SIP_XML_CMD_RES_QUERY_CATALOG				,
	SIP_XML_CMD_RES_RECV_CATALOG				,	
	SIP_XML_CMD_RES_DEVICE_INFO				,	
	SIP_XML_CMD_RES_DEVICE_STATUS				,	
	SIP_XML_CMD_RES_RECORD_INFO				,	
	SIP_XML_CMD_RES_DEVICE_CONFIG				,
	SIP_XML_CMD_RES_CONFIG_DOWNLOAD				,
	SIP_XML_CMD_RES_PRESET_QUERY				,
	SIP_XML_CMD_RES_BROADCAST				,
					
	SIP_XML_CMD_RES_MAX
	
}SIP_XML_CMD_RESPONSE_TYPE;

//device alarm 
typedef struct __SIP_CMD_TPYE
{
	int sipRequestType;
	int sipXmlCmdType;	
	int sipXmlCmdCtlType;
	int sipXmlCmdQueryType;
	int sipXmlCmdNotifyType;
	int sipXmlCmdResponseType;
}SIP_CMD_TPYE, *PSIP_CMD_TPYE;



int GetSipRequestType( sip_message_t *pReq );
int GetSipXmlCmdType( char * pBody );
int GetSipXmlCmdControl( char * pBody );
int GetSipXmlCmdQuery( char * pBody );
int GetSipXmlCmdNotify( char * pBody );
int GetSipXmlCmdResponse( char * pBody );

SIP_CMD_TPYE GetSipCmdType( sip_message_t *pReq );


#ifdef __cplusplus
}
#endif

#endif 

