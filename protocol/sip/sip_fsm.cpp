#include "sip_fsm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include "XmlHelper.h"
#include "tinyxml2.h"

using namespace tinyxml2;

//#define USING_TINYXML

bool FindElementExist(XMLDocument& doc,const char * name)
{
	  XMLElement* ele = doc.FirstChildElement(name);
	  if(ele)
	  {
		  return true;
	  }
	  return false;
}


int GetSipRequestType( sip_message_t *pReq )
{
	if( pReq->status_code != NULL )
	{
		//DA_LOGDBG( "return status code %s " , pReq->status_code );
		if(  0 == strncmp( pReq->status_code , "400" , strlen( pReq->status_code ) )	)
		{
			return SIP_REQUEST_ACK_400;
		}
		if(  0 == strncmp( pReq->status_code , "401" , strlen( pReq->status_code ) )	)
		{
			return SIP_REQUEST_ACK_401;
		}
		if(  0 == strncmp( pReq->status_code , "301" , strlen( pReq->status_code ) )	)
		{
			return SIP_REQUEST_ACK_301;
		}
		if(  0 == strncmp( pReq->status_code , "200" , strlen( pReq->status_code ) )	)
		{
			return SIP_REQUEST_ACK_200;
		}
		if(  0 == strncmp( pReq->status_code , "403" , strlen( pReq->status_code ) )	)
		{
			return SIP_REQUEST_ACK_403;
		}
		if(  0 == strncmp( pReq->status_code , "100" , strlen( pReq->status_code ) )	)
		{
			return SIP_REQUEST_100TRYING;
		} 
		if(  0 == strncmp( pReq->status_code , "487" , strlen( pReq->status_code ) )	)
		{
			return SIP_REQUEST_ACK_487;
		}
	}
	if( pReq->sip_method != NULL )
	{
		//DA_LOGDBG( "return sip method %s " , pReq->sip_method );
		if( 0 == strncmp( pReq->sip_method , "REGISTER" , strlen( pReq->sip_method ) ) )
		{
			return SIP_REQUEST_REGISTER ;
		}
		if( 0 == strncmp( pReq->sip_method , "MESSAGE" , strlen( pReq->sip_method ) ) )
		{
			return SIP_REQUEST_MESSAGE ;
		}
		if( 0 == strncmp( pReq->sip_method , "OPTIONS" , strlen( pReq->sip_method ) ) )
		{
			return SIP_REQUEST_OPTIONS ;
		}
		if( 0 == strncmp( pReq->sip_method , "INVITE" , strlen( pReq->sip_method ) ) )
		{
			return SIP_REQUEST_INVITE ;
		}
		if( 0 == strncmp( pReq->sip_method , "ACK" , strlen( pReq->sip_method ) ) )
		{
			return SIP_REQUEST_ACK ;
		}
		if( 0 == strncmp( pReq->sip_method , "BYE" , strlen( pReq->sip_method ) ) )
		{
			return SIP_REQUEST_BYE ;
		}
		if( 0 == strncmp( pReq->sip_method , "CANCEL" , strlen( pReq->sip_method ) ) )
		{
			return SIP_REQUEST_CANCEL ;
		}
		if( 0 == strncmp( pReq->sip_method , "INFO" , strlen( pReq->sip_method ) ) )
		{
			return SIP_REQUEST_INFO ;
		}
		if( 0 == strncmp( pReq->sip_method , "SUBSCRIBE" , strlen( pReq->sip_method ) ) )
		{
			return SIP_REQUEST_SUBSCRIBE ;
		}
		if( 0 == strncmp( pReq->sip_method , "NOTIFY" , strlen( pReq->sip_method ) ) )
		{
			return SIP_REQUEST_NOTIFY ;
		}
	}
	return SIP_REQUEST_NONE;
}
int GetSipXmlCmdType( char * pBody )
{
	if(NULL == pBody)
	{
		return SIP_XML_CMD_NONE;
	}


#ifdef USING_TINYXML
	XMLDocument doc;
    doc.Parse( pBody );
	if(FindElementExist(doc,"Control"))
	{
		return SIP_XML_CMD_CONTROL ;
	}
	if(FindElementExist(doc,"Query"))
	{
		return SIP_XML_CMD_QUERY ;
	}
	if(FindElementExist(doc,"Notify"))
	{
		return SIP_XML_CMD_NOTIFY ;
	}
	if(FindElementExist(doc,"Response"))
	{
		return SIP_XML_CMD_RESPONSE ;
	}
	return SIP_XML_CMD_NONE;
#else
	XmlHelper xmlIn;	
	xmlIn.Load(pBody);
	if(xmlIn.IsNodeExist("Control"))
	{
		return SIP_XML_CMD_CONTROL ;
	}
	if(xmlIn.IsNodeExist("Query"))
	{
		return SIP_XML_CMD_QUERY ;
	}
	if(xmlIn.IsNodeExist("Notify"))
	{
		return SIP_XML_CMD_NOTIFY ;
	}
	if(xmlIn.IsNodeExist("Response"))
	{
		return SIP_XML_CMD_RESPONSE ;
	}
	return SIP_XML_CMD_NONE;
#endif
}
int GetSipXmlCmdControl( char * pBody )
{
	if(NULL == pBody)
	{
		return SIP_XML_CMD_CTL_NONE;
	}

#ifdef USING_TINYXML


#else

	XmlHelper xmlIn;	
	xmlIn.Load(pBody);
	if("DeviceControl" == xmlIn.GetNodeValue("Control.CmdType"))
	{
		return SIP_XML_CMD_CTL_DEVICE_CONTROL ;
	}
	if("DeviceConfig" == xmlIn.GetNodeValue("Control.CmdType"))
	{
		return SIP_XML_CMD_CTL_DEVICE_CONFIG ;
	}
	return SIP_XML_CMD_CTL_NONE;
#endif
}
int GetSipXmlCmdQuery( char * pBody )
{
	if(NULL == pBody)
	{
		return SIP_XML_CMD_QUERY_NONE;
	}
	XmlHelper xmlIn;	
	xmlIn.Load(pBody);
	if("DeviceStatus" == xmlIn.GetNodeValue("Query.CmdType"))
	{
		return SIP_XML_CMD_QUERY_DEVICE_STATUS ;
	}
	if("Catalog" == xmlIn.GetNodeValue("Query.CmdType"))
	{
		return SIP_XML_CMD_QUERY_CATALOG ;
	}
	if("DeviceInfo" == xmlIn.GetNodeValue("Query.CmdType"))
	{
		return SIP_XML_CMD_QUERY_DEVICE_INFO ;
	}
	if("RecordInfo" == xmlIn.GetNodeValue("Query.CmdType"))	
	{
		return SIP_XML_CMD_QUERY_RECORD_INFO ;
	}
	if("Alarm" == xmlIn.GetNodeValue("Query.CmdType"))	
	{
		return SIP_XML_CMD_QUERY_ALARM ;
	}
	if("ConfigDownload" == xmlIn.GetNodeValue("Query.CmdType"))	
	{
		return SIP_XML_CMD_QUERY_CONFIG_DOWNLOAD ;
	}
	if("PresetQuery" == xmlIn.GetNodeValue("Query.CmdType"))	
	{
		return SIP_XML_CMD_QUERY_PRESET_QUERY ;
	}
	if("MobilePosition" == xmlIn.GetNodeValue("Query.CmdType"))		
	{
		return SIP_XML_CMD_QUERY_MOBILE_POSITION ;
	}

	return SIP_XML_CMD_QUERY_NONE;

}
int GetSipXmlCmdNotify( char * pBody )
{
	if(NULL == pBody)
	{
		return SIP_XML_CMD_NOTIFY_NONE;
	}
	XmlHelper xmlIn;	
	xmlIn.Load(pBody);
	if("Keepalive" == xmlIn.GetNodeValue("Notify.CmdType"))
	{
		return SIP_XML_CMD_NOTIFY_KEEPALIVE ;
	}
	if("Alarm" == xmlIn.GetNodeValue("Notify.CmdType"))
	{
		return SIP_XML_CMD_NOTIFY_ALARM ;
	}
	if("MediaStatus" == xmlIn.GetNodeValue("Notify.CmdType"))
	{
		return SIP_XML_CMD_NOTIFY_MEDIA_STATUS ;
	}
	if("Broadcast" == xmlIn.GetNodeValue("Notify.CmdType"))
	{
		return SIP_XML_CMD_NOTIFY_BROADCAST ;
	}
	if("MobilePosition" == xmlIn.GetNodeValue("Notify.CmdType"))
	{
		return SIP_XML_CMD_NOTIFY_MOBILE_POSITION ;
	}
	if("Catalog" == xmlIn.GetNodeValue("Notify.CmdType"))
	{
		return SIP_XML_CMD_NOTIFY_CATALOG ;
	}	
	return SIP_XML_CMD_NOTIFY_NONE;

}
int GetSipXmlCmdResponse( char * pBody )
{
	if(NULL == pBody)
	{
		return SIP_XML_CMD_RES_NONE;
	}
	XmlHelper xmlIn;	
	xmlIn.Load(pBody);
	if("DeviceControl" == xmlIn.GetNodeValue("Response.CmdType"))
	{
		return SIP_XML_CMD_RES_DEVICE_CONTROL ;
	}
	if("DeviceStatus" == xmlIn.GetNodeValue("Response.CmdType"))
	{
		return SIP_XML_CMD_RES_DEVICE_STATUS ;
	}
	if("Alarm" == xmlIn.GetNodeValue("Response.CmdType"))
	{
		return SIP_XML_CMD_RES_ALARM ;
	}
	if(("Catalog" == xmlIn.GetNodeValue("Response.CmdType"))
		&&(true == xmlIn.IsNodeExist("Response.SumNum")))
	{
		return SIP_XML_CMD_RES_QUERY_CATALOG ;
	}
	if(("Catalog" == xmlIn.GetNodeValue("Response.CmdType"))
		&&(true == xmlIn.IsNodeExist("Response.Result")))
	{
		return SIP_XML_CMD_RES_RECV_CATALOG ;
	}
	if("DeviceInfo" == xmlIn.GetNodeValue("Response.CmdType"))
	{
		return SIP_XML_CMD_RES_DEVICE_INFO ;
	}
	if("RecordInfo" == xmlIn.GetNodeValue("Response.CmdType"))
	{
		return SIP_XML_CMD_RES_RECORD_INFO ;
	}
	if("DeviceConfig" == xmlIn.GetNodeValue("Response.CmdType"))	
	{
		return SIP_XML_CMD_RES_DEVICE_CONFIG ;
	}
	if("ConfigDownload" == xmlIn.GetNodeValue("Response.CmdType"))		
	{
		return SIP_XML_CMD_RES_CONFIG_DOWNLOAD ;
	}
	if("PresetQuery" == xmlIn.GetNodeValue("Response.CmdType"))		
	{
		return SIP_XML_CMD_RES_PRESET_QUERY ;
	}
	if("Broadcast" == xmlIn.GetNodeValue("Response.CmdType"))	
	{
		return SIP_XML_CMD_RES_BROADCAST ;
	}
	
	return SIP_XML_CMD_RES_NONE;

}

SIP_CMD_TPYE GetSipCmdType( sip_message_t *pReq )
{
	SIP_CMD_TPYE tSipCmdType;
	tSipCmdType.sipRequestType = SIP_REQUEST_NONE;
	tSipCmdType.sipXmlCmdType = SIP_XML_CMD_NONE;
	tSipCmdType.sipXmlCmdCtlType = SIP_XML_CMD_CTL_NONE;
	tSipCmdType.sipXmlCmdQueryType = SIP_XML_CMD_QUERY_NONE;
	tSipCmdType.sipXmlCmdNotifyType = SIP_XML_CMD_NOTIFY_NONE;
	tSipCmdType.sipXmlCmdResponseType = SIP_XML_CMD_RES_NONE;	
	tSipCmdType.sipRequestType = GetSipRequestType(pReq);
	tSipCmdType.sipXmlCmdType = GetSipXmlCmdType(pReq->sip_body);

	switch(tSipCmdType.sipXmlCmdType)
	{
		case SIP_XML_CMD_CONTROL:
		{
			tSipCmdType.sipXmlCmdCtlType = GetSipXmlCmdControl(pReq->sip_body);
			break;
		}
		case SIP_XML_CMD_QUERY:
		{
			tSipCmdType.sipXmlCmdQueryType = GetSipXmlCmdQuery(pReq->sip_body);
			break;
		}
		case SIP_XML_CMD_NOTIFY:
		{
			tSipCmdType.sipXmlCmdNotifyType = GetSipXmlCmdNotify(pReq->sip_body);
			break;
		}
		case SIP_XML_CMD_RESPONSE:
		{
		
			tSipCmdType.sipXmlCmdResponseType = GetSipXmlCmdResponse(pReq->sip_body);
			break;
		}
		default :
		{
			break;
		}

	}

	return tSipCmdType;

}

