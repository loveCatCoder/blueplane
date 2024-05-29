/*
 * =====================================================================================
 *
 *	Filename:  Sdp.cpp
 *
 *	Description: 			
 *	Version:  1.0
 *	Created: 2019-04-11
 *
 *	Author:  wty
 *	Organization:  ICHINAE(BEIJING) TECH.CO.,LTD. 
 *
 * =====================================================================================
*/
#include "Sdp.h"

CSdpSession::CSdpSession( )
{
	m_pVideoMediaInfo = NULL;
	m_pAudioMediaInfo = NULL;
}

CSdpSession::~CSdpSession( )
{
	if( NULL != m_pVideoMediaInfo )
	{
		delete m_pVideoMediaInfo;
		m_pVideoMediaInfo = NULL;
	}
	if( NULL != m_pAudioMediaInfo )
	{
		delete m_pAudioMediaInfo;
		m_pAudioMediaInfo = NULL;
	}
}
int CSdpSession::SdpMessageParse( const char* strSdp )
{
	int nRet = 0;
	sdp_message_t * pSdpMsg = NULL;
	
	nRet = sdp_message_init( &pSdpMsg );
	if( 0 != nRet )
	{
		log_error(" init sdp error ");
		return nRet;
	}
	nRet = sdp_message_parse( pSdpMsg, strSdp );
	if( 0 != nRet )
	{
		log_error(" parse sdp error ");
		sdp_message_free( pSdpMsg );
		return nRet;
	}

	int i = 0;
	char* pValue = NULL;
	while( ( pValue = sdp_message_m_media_get( pSdpMsg, i ) ) != NULL )
	{
		if(strncmp( pValue, "video", strlen("video") ) == 0)
		{
			if( m_pVideoMediaInfo == NULL  )
			{
				m_pVideoMediaInfo = new SDP_MEDIA_INFO;
			}
			ParseMedia( pSdpMsg, m_pVideoMediaInfo, i );
			
		}
		else if( strncmp( pValue, "audio", strlen("audio")) == 0 )
		{
			if( m_pAudioMediaInfo == NULL  )
			{
				m_pAudioMediaInfo = new SDP_MEDIA_INFO;
			}
			ParseMedia( pSdpMsg, m_pAudioMediaInfo, i );
		}
		else
		{
			log_info("m=%s (not parse)",pValue);
		}
		i++;
	}
	
	sdp_message_free( pSdpMsg );
	return SUCCESS;
}

int CSdpSession::ParseMedia( sdp_message_t * pSdpMsg, SDP_MEDIA_INFO * pMediaInfo,  int nMediaPos )
{
	if( (NULL == pSdpMsg) || (NULL == pMediaInfo) )
	{
		return FAILURE;
	}
	char* pValue = NULL;
	pValue = sdp_message_m_media_get( pSdpMsg, nMediaPos );
	if( NULL != pValue )
	{
		pMediaInfo->m_strMediaType = pValue;
	}
	log_debug("MediaType=========%s", pMediaInfo->m_strMediaType.c_str());

	pValue = sdp_message_m_proto_get( pSdpMsg, nMediaPos );
	if( NULL != pValue )
	{
		pMediaInfo->m_strMediaProtocol = pValue;
	}
	log_debug("MediaProtocol=========%s", pMediaInfo->m_strMediaProtocol.c_str());
	
	pValue = sdp_message_m_fmt_list_get( pSdpMsg, nMediaPos, 0 );
	if( NULL != pValue )
	{
		pMediaInfo->m_nMediaFormat = atoi( pValue );
	}
	log_debug("MediaProtocol=========%d", pMediaInfo->m_nMediaFormat);

	pValue = sdp_message_c_nettype_get( pSdpMsg, nMediaPos, 0 );
	if( NULL != pValue)
	{
		pMediaInfo->m_strNetType = pValue;
	}
	log_debug("NetType=========%s", pMediaInfo->m_strNetType.c_str());

	pValue = sdp_message_c_addrtype_get( pSdpMsg, nMediaPos, 0 );
	if( NULL != pValue )
	{
		pMediaInfo->m_strAddrType = pValue;
	}
	log_debug("AddrType=========%s", pMediaInfo->m_strAddrType.c_str());

	pValue = sdp_message_c_addr_get( pSdpMsg, nMediaPos, 0 );
	if( NULL != pValue )
	{
		pMediaInfo->m_strAddr = pValue;
	}
	log_debug("Addr=========%s", pMediaInfo->m_strAddr.c_str());

	pValue = sdp_message_b_bwtype_get( pSdpMsg, nMediaPos, 0 );
	if( NULL != pValue )
	{
		pMediaInfo->m_strBandWidthType = pValue;
	}
	log_debug("BandWidthType=========%s", pMediaInfo->m_strBandWidthType.c_str());

	pValue = sdp_message_b_bandwidth_get( pSdpMsg, nMediaPos, 0 );
	if( NULL != pValue )
	{
		pMediaInfo->m_nBandWidth = atoi( pValue );
	}
	log_debug("BandWidth=========%d", pMediaInfo->m_nBandWidth);


	int i = 0;
	while( sdp_message_a_att_field_get( pSdpMsg, nMediaPos, i) != NULL )
	{
		pValue = sdp_message_a_att_field_get( pSdpMsg, nMediaPos, i );
		
		if( strncmp( pValue, "rtpmap", strlen("rtpmap")) == 0 )
		{
			ParseRtpmap( pSdpMsg, pMediaInfo, nMediaPos, i );
		}
		else if( strncmp( pValue, "fmtp", strlen("fmtp")) == 0 )
		{
			ParseFmtp( pSdpMsg, pMediaInfo, nMediaPos, i );
		}
		else if( strncmp( pValue, "control", strlen("control")) == 0 )
		{
			ParseControl( pSdpMsg, pMediaInfo, nMediaPos, i );
		}
		i++;
	}
	return SUCCESS;
}

int CSdpSession::ParseRtpmap( sdp_message_t * pSdpMsg, SDP_MEDIA_INFO * pMediaInfo, int nMediaPos, int nPos )
{
	if( (NULL == pSdpMsg) || (NULL == pMediaInfo) )
	{
		return FAILURE;
	}
	char* pValue = NULL;
	pValue = sdp_message_a_att_value_get( pSdpMsg, nMediaPos, nPos );
	if( pValue == NULL )
	{
		return FAILURE;
	}
	//skip first value
	while( (*pValue != ' ') && (*pValue != '\r') && (*pValue != '\n') ) pValue ++;
	//skip ' '
	while( *pValue == ' ' ) pValue ++;	
	pMediaInfo->m_strPayload = pValue;

	log_debug("Payload=========%s", pMediaInfo->m_strPayload.c_str());
	return SUCCESS;
}

int CSdpSession::ParseFmtp( sdp_message_t * pSdpMsg, SDP_MEDIA_INFO * pMediaInfo, int nMediaPos, int nPos )
{
	if( (NULL == pSdpMsg) || (NULL == pMediaInfo) )
	{
		return FAILURE;
	}
	char* pValue = NULL;
	char* pStartPos = NULL;
	char* pEndPos = NULL;
	pValue = sdp_message_a_att_value_get( pSdpMsg, nMediaPos, nPos );
	if( pValue == NULL )
	{
		return FAILURE;
	}
	pStartPos = strstr( pValue, "packetization-mode" );
	if( pStartPos != NULL )
	{
		sscanf(pStartPos, "packetization-mode=%d", &pMediaInfo->m_nPacketizationMode );
	}
	
	pStartPos = strstr( pValue, "profile-level-id" );
	if( pStartPos != NULL )
	{
		sscanf(pStartPos, "profile-level-id=%x", &pMediaInfo->m_nProfileLevelId );
	}

	pStartPos = strstr( pValue, "sprop-parameter-sets=" );
	unsigned int nLen = strlen("sprop-parameter-sets=");
	if( (pStartPos != NULL) && ( strlen(pStartPos) != nLen ))
	{
		pStartPos += nLen;
		pEndPos = pStartPos;
		char tempBuf[2048] = {0};
		//skip sps frame
		while( (*pEndPos != ' ') && (*pEndPos != ',') && (*pEndPos != '\r') && (*pEndPos != '\n') ) pEndPos ++;
		strncpy( tempBuf, pStartPos, pEndPos-pStartPos );
		pMediaInfo->m_strSPS = tempBuf;

		//skip ','
		pEndPos++;
		pStartPos = pEndPos;
		//skip pps frame
		while( (*pEndPos != ' ') && (*pEndPos != ',') && (*pEndPos != '\r') && (*pEndPos != '\n') && ( *pEndPos != '\0' ) ) pEndPos ++;
		memset( tempBuf, 0, sizeof(tempBuf) );
		strncpy( tempBuf, pStartPos, pEndPos-pStartPos );
		pMediaInfo->m_strPPS = tempBuf;
	}
	

	log_debug("PacketizationMode=========%d", pMediaInfo->m_nPacketizationMode );
	log_debug("PacketizationMode=========%x", pMediaInfo->m_nProfileLevelId);
	log_debug("SPS=========%s", pMediaInfo->m_strSPS.c_str());
	log_debug("PPS=========%s", pMediaInfo->m_strPPS.c_str());

	return SUCCESS;
}

int CSdpSession::ParseControl( sdp_message_t * pSdpMsg, SDP_MEDIA_INFO * pMediaInfo, int nMediaPos, int nPos )
{
	if( (NULL == pSdpMsg) || (NULL == pMediaInfo) )
	{
		return FAILURE;
	}
	char* pValue = NULL;
	
	pValue = sdp_message_a_att_value_get( pSdpMsg, nMediaPos, nPos );

	pMediaInfo->m_strTrack = pValue;
	log_info("Track=========%s", pMediaInfo->m_strTrack.c_str());
	return SUCCESS;
}


