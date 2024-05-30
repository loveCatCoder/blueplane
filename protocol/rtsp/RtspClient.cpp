/*
 * =====================================================================================
 *
 *	Filename:  RtspClient.cpp
 *
 *	Description: 			
 *	Version:  1.0
 *	Created: 2020-12-17
 *
 *	Author:  wty
 *	Organization:  ICHINAE(BEIJING) TECH.CO.,LTD. 
 *
 * =====================================================================================
*/

#include "string.h"

#include "RtspClient.h"

CRtspClient::CRtspClient( )
{
	m_nCseq = 1;
	m_nNetType = -1;
	m_pAuth = NULL;
}

CRtspClient::~CRtspClient( )
{
	if( m_pAuth != NULL )
	{
		delete m_pAuth;
		m_pAuth = NULL;
	}
}

int CRtspClient::InitRtspClient( const char* strUrl, const char * strUser, const char * strPassword )
{
	if( NULL == strUrl )
	{
		return FAILURE;
	}
	m_strUrl = strUrl;
	
	if( strUser != NULL )
	{
		m_strUser = strUser;
	}
	
	if( strPassword != NULL )
	{
		m_strPassword = strPassword;
	}
	
	return SUCCESS;
}

int CRtspClient::DeinitRtspClinet( )
{
	return SUCCESS;
}

int CRtspClient::CreateOptionRequest( char* pRtspMsg, int nRtspBufLen )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}
	
	if( SetRequestHeader( pRtspMsg, nRtspBufLen, "OPTIONS", m_strUrl.c_str() ) == -1 )
	{
		return FAILURE;
	}
	if( SetIntegerAttribute( pRtspMsg, nRtspBufLen, "CSeq", m_nCseq++ ) == -1 )
	{
		return FAILURE;
	}
	if( NULL != m_pAuth )
	{
		char strAuth[AUTH_LEN] = {0};
		m_pAuth->GetAuth( strAuth, AUTH_LEN, "OPTIONS", m_strUrl.c_str() );
		if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Authorization", strAuth ) == -1 )
		{
			return FAILURE;
		}
	}
	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "User-Agent", USER_AGENT ) == -1 )
	{
		return FAILURE;
	}
	if( !m_SessionId.empty() )
	{
		if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Session", m_SessionId.c_str() ) == -1 )
		{
			return FAILURE;
		}
	}
	if( SetTail( pRtspMsg, nRtspBufLen ) == -1 )
	{
		return FAILURE;
	}

	return strlen( pRtspMsg );
}

int CRtspClient::CreateDescribeRequest( char* pRtspMsg, int nRtspBufLen )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}

	if( SetRequestHeader( pRtspMsg, nRtspBufLen, "DESCRIBE", m_strUrl.c_str() ) == -1 )
	{
		return FAILURE;
	}
	if( SetIntegerAttribute( pRtspMsg, nRtspBufLen, "CSeq", m_nCseq++ ) == -1 )
	{
		return FAILURE;
	}
	if( NULL != m_pAuth )
	{
		char strAuth[AUTH_LEN] = {0};
		m_pAuth->GetAuth( strAuth, AUTH_LEN, "DESCRIBE", m_strUrl.c_str() );
		if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Authorization", strAuth ) == -1 )
		{
			return FAILURE;
		}
	}
	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "User-Agent", USER_AGENT ) == -1 )
	{
		return FAILURE;
	}
	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Accept", ACCPET_SDP ) == -1 )
	{
		return FAILURE;
	}
	if( !m_SessionId.empty() )
	{
		if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Session", m_SessionId.c_str() ) == -1 )
		{
			return FAILURE;
		}
	}
	if( SetTail( pRtspMsg, nRtspBufLen ) == -1 )
	{
		return FAILURE;
	}
	return strlen( pRtspMsg );
}

int CRtspClient::CreateSetupRequest( char* pRtspMsg, int nRtspBufLen, E_TRACK_TYPE nTrackType, int nNetType, int nRtpPort, int nRtcpPort )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}
	m_nNetType = nNetType;

	if( VIDEO_TRACK == nTrackType )
	{
		if( m_strVideoUrl.empty() )
		{
			return FAILURE;
		}
		if( SetRequestHeader( pRtspMsg, nRtspBufLen, "SETUP", m_strVideoUrl.c_str() ) == -1 )
		{
			return FAILURE;
		}
	}
	else if( AUDIO_TRACK == nTrackType )
	{
		if( m_strAudioUrl.empty() )
		{
			return FAILURE;
		}
		if( SetRequestHeader( pRtspMsg, nRtspBufLen, "SETUP", m_strAudioUrl.c_str() ) == -1 )
		{
			return FAILURE;
		}
	}
	else
	{
		return FAILURE;
	}
	
	if( SetIntegerAttribute( pRtspMsg, nRtspBufLen, "CSeq", m_nCseq++ ) == -1 )
	{
		return FAILURE;
	}
	if( NULL != m_pAuth )
	{
		char strAuth[AUTH_LEN] = {0};
		if( VIDEO_TRACK == nTrackType )
		{
			m_pAuth->GetAuth( strAuth, AUTH_LEN, "SETUP", m_strVideoUrl.c_str() );
		}
		else
		{
			m_pAuth->GetAuth( strAuth, AUTH_LEN, "SETUP", m_strAudioUrl.c_str() );
		}
		if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Authorization", strAuth ) == -1 )
		{
			return FAILURE;
		}
	}
	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "User-Agent", USER_AGENT ) == -1 )
	{
		return FAILURE;
	}
	//create transport
	char transport[128] = {0};
	if( nNetType  == RTSP_LINK_UDP )
	{
		snprintf( transport, 128, "RTP/AVP;unicast;client_port=%d-%d", nRtpPort , nRtcpPort );
	}
	else if( nNetType == RTSP_LINK_TCP )
	{
		snprintf( transport, 128, "RTP/AVP/TCP;unicast;interleaved=%d-%d", nRtpPort, nRtcpPort );
	}
	else
	{
		return FAILURE;
	}

	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Transport", transport ) == -1 )
	{
		return FAILURE;
	}
	if( !m_SessionId.empty() )
	{
		if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Session", m_SessionId.c_str() ) == -1 )
		{
			return FAILURE;
		}
	}
	if( SetTail( pRtspMsg, nRtspBufLen ) == -1 )
	{
		return FAILURE;
	}
	
	return strlen( pRtspMsg );
}

int CRtspClient::CreatePlayRequest( char* pRtspMsg, int nRtspBufLen, float fStartTime, float fEndTime )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}

	if( SetRequestHeader( pRtspMsg, nRtspBufLen, "PLAY", m_strContentBase.c_str() ) == -1 )
	{
		return FAILURE;
	}
	if( SetIntegerAttribute( pRtspMsg, nRtspBufLen, "CSeq", m_nCseq++ ) == -1 )
	{
		return FAILURE;
	}
	if( NULL != m_pAuth )
	{
		char strAuth[AUTH_LEN] = {0};
		m_pAuth->GetAuth( strAuth, AUTH_LEN, "PLAY", m_strContentBase.c_str() );
		if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Authorization", strAuth ) == -1 )
		{
			return FAILURE;
		}
	}
	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "User-Agent", USER_AGENT ) == -1 )
	{
		return FAILURE;
	}

	//creare range
	char range[128] = {0};
	if( fEndTime == 0 )
	{
		snprintf( range , 128, "npt=%.3f-", fStartTime );
	}
	else
	{
		snprintf( range , 128, "npt=%.3f-%.3f", fStartTime, fEndTime );
	}
	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Range", range ) == -1 )
	{
		return FAILURE;
	}
	if( !m_SessionId.empty() )
	{
		if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Session", m_SessionId.c_str() ) == -1 )
		{
			return FAILURE;
		}
	}
	if( SetTail( pRtspMsg, nRtspBufLen ) == -1 )
	{
		return FAILURE;
	}
	return strlen( pRtspMsg );
}

int CRtspClient::CreateTeardownRequest( char* pRtspMsg, int nRtspBufLen )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}

	if( !m_strContentBase.empty() )
	{
		if( SetRequestHeader( pRtspMsg, nRtspBufLen, "TEARDOWN", m_strContentBase.c_str() ) == -1 )
		{
			return FAILURE;
		}
		
	}
	else
	{
		if( SetRequestHeader( pRtspMsg, nRtspBufLen, "TEARDOWN", m_strUrl.c_str() ) == -1 )
		{
			return FAILURE;
		}
	}
	
	if( SetIntegerAttribute( pRtspMsg, nRtspBufLen, "CSeq", m_nCseq++ ) == -1 )
	{
		return FAILURE;
	}
	if( NULL != m_pAuth )
	{
		char strAuth[AUTH_LEN] = {0};
		m_pAuth->GetAuth( strAuth, AUTH_LEN, "TEARDOWN", m_strContentBase.c_str() );
		if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Authorization", strAuth ) == -1 )
		{
			return FAILURE;
		}
	}
	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "User-Agent", USER_AGENT ) == -1 )
	{
		return FAILURE;
	}
	if( !m_SessionId.empty() )
	{
		if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Session", m_SessionId.c_str() ) == -1 )
		{
			return FAILURE;
		}
		m_SessionId = "";
	}
	if( SetTail( pRtspMsg, nRtspBufLen ) == -1 )
	{
		return FAILURE;
	}

	return strlen( pRtspMsg );
}

int CRtspClient::ParseOptionReply( const char* pRtspMsg )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}
	int nRet = 0;
	nRet = ParseResponseReply( pRtspMsg );
	if( nRet != RTSP_TYPE_OK )
	{
		if( RTSP_TYPE_UNAUTHORIZED == nRet )
		{
			InitRtspAuth( );
			if( NULL != m_pAuth )
			{
				m_pAuth->ParseAuthData( pRtspMsg );
			}
		}
		return nRet;
	}
	char strValue[256] = {0};
	nRet = GetStringAttribute( pRtspMsg, "public", strValue );
	if( SUCCESS == nRet )
	{
		m_strOption = strValue;
	}

	return nRet;
}

int CRtspClient::ParseDescribeReply( const char* pRtspMsg )
{
	if( NULL == pRtspMsg )
    {
        return FAILURE;
    }
    int nRet = 0;
	nRet = ParseResponseReply( pRtspMsg );
	if( nRet != RTSP_TYPE_OK )
	{
		if( RTSP_TYPE_UNAUTHORIZED == nRet )
		{
			InitRtspAuth( );
			if( NULL != m_pAuth )
			{
				m_pAuth->ParseAuthData( pRtspMsg );
			}
		}
		return nRet;
	}

	char tempBuf[1024] = {0};
	nRet = GetStringAttribute( pRtspMsg, "content-base", tempBuf );
	if( nRet != SUCCESS )
	{
		log_warn("content-base parse error, use url");
		m_strContentBase = m_strUrl;
	}
	else
	{
		log_debug("content-base = %s",tempBuf);
		m_strContentBase = tempBuf;
	}
	
	//sdp
	int nSdpLen = 0;
	const char * strSdpMsg = NULL;
	GetIntegerAttribute( pRtspMsg, "content-length", &nSdpLen );
	if( nSdpLen > 0 )
	{
		//find sdp version
		strSdpMsg = strstr( pRtspMsg, "v=" );
		if( NULL != strSdpMsg )
		{
			m_SdpSession.SdpMessageParse( strSdpMsg );
		}
	}
	SDP_MEDIA_INFO * pSdpVideoMediaInfo = m_SdpSession.GetVideoMediaInfo( );
	if( NULL != pSdpVideoMediaInfo )
	{
		if( strncmp( pSdpVideoMediaInfo->m_strTrack.c_str(), m_strContentBase.c_str(), m_strContentBase.size() ) == 0 )
	    {
	    	m_strVideoUrl = pSdpVideoMediaInfo->m_strTrack;
	    }
	    else
	    {
	    	if( m_strContentBase[m_strContentBase.size()-1] == '/')
	    	{
	    		m_strVideoUrl = m_strContentBase + pSdpVideoMediaInfo->m_strTrack;
	    	}
	    	else
	    	{
	    		m_strVideoUrl = m_strContentBase + "/" + pSdpVideoMediaInfo->m_strTrack;
	    	}	    	
	    }
	}
	SDP_MEDIA_INFO * pSdpAudioMediaInfo = m_SdpSession.GetAudioMediaInfo( );
	if( NULL != pSdpAudioMediaInfo )
	{
		if( strncmp( pSdpAudioMediaInfo->m_strTrack.c_str(), m_strContentBase.c_str(), m_strContentBase.size() ) == 0 )
	    {
	    	m_strAudioUrl = pSdpAudioMediaInfo->m_strTrack;
	    }
	    else
	    {
	    	if( m_strContentBase[m_strContentBase.size()-1] == '/')
	    	{
	    		m_strAudioUrl = m_strContentBase + pSdpAudioMediaInfo->m_strTrack;
	    	}
	    	else
	    	{
	    		m_strAudioUrl = m_strContentBase + "/" + pSdpAudioMediaInfo->m_strTrack;
	    	}
	    	
	    }
	}
	log_debug(" m_strVideoUrl = %s ",m_strVideoUrl.c_str());
	log_debug(" m_strAudioUrl = %s ",m_strAudioUrl.c_str());
	return SUCCESS;
}

int CRtspClient::ParseSetupReply( const char* pRtspMsg, int* pRtpPort, int* pRtcpPort )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}
	int nRet = 0;
	nRet = ParseResponseReply( pRtspMsg );
	if( nRet != RTSP_TYPE_OK )
	{
		return nRet;
	}

	char tempBuf[1024] = {0};
	//parse sessionid
	ParseSessionId( pRtspMsg );
	
	GetStringAttribute( pRtspMsg, "transport", tempBuf );

	//parse transport
	char* pStartPos = NULL;
	AllCharToLower( tempBuf );
	if( RTSP_LINK_UDP == m_nNetType )
	{
		pStartPos = strstr( tempBuf , "server_port=");
		if( NULL == pStartPos )
		{
			return FAILURE;
		}
		pStartPos += strlen("server_port=");
	}
	else if( RTSP_LINK_TCP == m_nNetType )
	{
		pStartPos = strstr( tempBuf , "interleaved=");
		if( NULL == pStartPos )
		{
			return FAILURE;
		}
		pStartPos += strlen("interleaved=");
	}
	sscanf( pStartPos , "%d-%d\r\n" , pRtpPort, pRtcpPort );
	return SUCCESS;
}

int CRtspClient::ParsePlayReply( const char* pRtspMsg, int* pVideoSeq, unsigned int* pVideoRtpTime, int * pAudioSeq, unsigned int* pAudioRtpTime )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}
	int nRet = 0;
	nRet = ParseResponseReply( pRtspMsg );
	if( nRet != RTSP_TYPE_OK )
	{
		return nRet;
	}
	char tempBuf[1024] = {0};
	GetStringAttribute( pRtspMsg, "rtp-info", tempBuf );
	const char* pStartPos = NULL;

	SDP_MEDIA_INFO * pSdpVideoMediaInfo = m_SdpSession.GetVideoMediaInfo( );
	if( pSdpVideoMediaInfo != NULL )
	{
		pStartPos = strstr( tempBuf, pSdpVideoMediaInfo->m_strTrack.c_str() );
		if( NULL != pStartPos )
		{
			pStartPos = strstr( pStartPos, "seq=" );
			if( NULL != pStartPos )
			{
				pStartPos += strlen("seq=");
				*pVideoSeq = atoi( pStartPos );

				//find rtptime
				pStartPos = strstr( pStartPos, "rtptime=" );
				if( NULL != pStartPos )
				{
					pStartPos += strlen("rtptime=");
					*pVideoRtpTime = atoi( pStartPos );
				}
			}
			
		}
	}

	
	SDP_MEDIA_INFO * pSdpAudioMediaInfo = m_SdpSession.GetAudioMediaInfo( );
	if( pSdpAudioMediaInfo != NULL )
	{
		pStartPos = strstr( tempBuf, pSdpAudioMediaInfo->m_strTrack.c_str() );
		if( NULL != pStartPos )
		{
			pStartPos = strstr( pStartPos, "seq=" );
			if( NULL != pStartPos )
			{
				pStartPos += strlen("seq=");
				*pAudioSeq = atoi( pStartPos );

				//find rtptime
				pStartPos = strstr( pStartPos, "rtptime=" );
				if( NULL != pStartPos )
				{
					pStartPos += strlen("rtptime=");
					*pAudioRtpTime = atoi( pStartPos );
				}
			}
		}
	}
	
	return SUCCESS;
}

int CRtspClient::ParseTeardownReply( const char* pRtspMsg )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}
	int nRet = 0;
	nRet = ParseResponseReply( pRtspMsg );
	if( nRet != RTSP_TYPE_OK )
	{
		return nRet;
	}
	return SUCCESS;
}

std::string CRtspClient::GetOptions( )
{
	return m_strOption;
}

std::string CRtspClient::GetSessionId( )
{
	return m_SessionId;
}

std::string CRtspClient::GetVideoTrack( )
{
	return m_strVideoUrl;
}

std::string CRtspClient::GetAudioTrack( )
{
	return m_strAudioUrl;
}

SDP_MEDIA_INFO * CRtspClient::GetVideoMediaInfo( )
{
	return m_SdpSession.GetVideoMediaInfo( );
}

SDP_MEDIA_INFO * CRtspClient::GetAudioMediaInfo( )
{
	return m_SdpSession.GetAudioMediaInfo( );
}

int CRtspClient::AppendInteger( STRING_INFO *pInfo, int nAttr )
{
	int strLength = 0;
	char tempBuf[128] = {0};

	strLength = sprintf( tempBuf, "%d", nAttr );
	if( (-1 == strLength) || (strLength > pInfo->nLength) )
	{
		return FAILURE;
	}
	strcat( pInfo->strBuf, tempBuf );
	pInfo->strBuf += strLength;
	pInfo->nLength -= strLength;
	return SUCCESS;
}

int CRtspClient::AppendString( STRING_INFO *pInfo, const char *strAttr )
{
	int strLength = strlen( strAttr );
	if( strLength > pInfo->nLength )
	{
		return FAILURE;
	}
	strcat( pInfo->strBuf, strAttr );
	pInfo->strBuf += strLength;
	pInfo->nLength -= strLength;
	return SUCCESS;
}

int CRtspClient::SetIntegerAttribute( char* strRtspMsg, int nLength, const char *strAttr, int nValue )
{
	if( (NULL == strRtspMsg) || (NULL == strAttr) )
	{
		return FAILURE;
	}
	STRING_INFO strInfo = {};
	int nRet = FAILURE;
	
	strInfo.strBuf = strRtspMsg + strlen( strRtspMsg );
	strInfo.nLength = nLength - strlen( strRtspMsg );

	do{
		if( AppendString( &strInfo, strAttr ) == -1 )
		{
			break ;
		}
		if( AppendString( &strInfo, ": " ) == -1 )
		{
			break;
		}
		if( AppendInteger( &strInfo, nValue ) == -1 )
		{
			break;
		}
		if( AppendString( &strInfo, "\r\n" ) == -1 )
		{
			break;
		}
		
		nRet = strlen( strRtspMsg );
	}while( 0 );

	return nRet;
}

int CRtspClient::SetStringAttribute( char *strRtspMsg, int nLength, const char *strAttr, const char *strValue )
{
	if( (NULL == strRtspMsg) || (NULL == strAttr) || (NULL == strValue))
	{
		return FAILURE;
	}
	STRING_INFO strInfo = {};
	int nRet = FAILURE;
	
	strInfo.strBuf = strRtspMsg + strlen( strRtspMsg );
	strInfo.nLength = nLength - strlen( strRtspMsg );

	do{
		if( AppendString( &strInfo, strAttr ) == -1 )
		{
			break ;
		}
		if( AppendString( &strInfo, ": " ) == -1 )
		{
			break;
		}
		if( AppendString( &strInfo, strValue ) == -1 )
		{
			break;
		}
		if( AppendString( &strInfo, "\r\n" ) == -1 )
		{
			break;
		}
		nRet = strlen( strRtspMsg );
	}while( 0 );

	return nRet;
}

int CRtspClient::SetRequestHeader( char *strRtspMsg, int nLength, const char* strMethod , const char *strUrl )
{
	if( (NULL == strRtspMsg) || (NULL == strMethod) || (NULL == strUrl) )
	{
		return FAILURE;
	}

	snprintf( strRtspMsg, nLength, "%s %s RTSP/1.0\r\n", strMethod, strUrl);
	return strlen( strRtspMsg );
}

int CRtspClient::SetTail( char *strRtspMsg, int nLength )
{
	if( NULL == strRtspMsg )
	{
		return FAILURE;
	}
	STRING_INFO strInfo = {};
	int nRet = FAILURE;

	strInfo.strBuf = strRtspMsg + strlen( strRtspMsg );
	strInfo.nLength = nLength - strlen( strRtspMsg );

	if( AppendString( &strInfo, "\r\n" ) == -1 )
	{
		return FAILURE;
	}
	nRet = strlen( strRtspMsg );
	return nRet;
}

void CRtspClient::AllCharToLower( char *pBuf ) 
{ 
	int i=0;
	while( pBuf[i] != 0 ) 
	{	
		if((pBuf[i] >= 'A')&&(pBuf[i] <= 'Z'))
		{
			pBuf[i] +=32;
		}
		i++;
	}
}

int CRtspClient::GetIntegerAttribute( const char* strRtspMsg, const char *strAttr, int* pValue )
{
	if( (NULL == strRtspMsg) || (NULL == strAttr) || (NULL == pValue) )
	{
		return FAILURE;
	}
	
	char *strTempRtsp = strdup( strRtspMsg );
	if( NULL == strTempRtsp )
	{
		return FAILURE;
	}
	AllCharToLower( strTempRtsp );
	const char* pStartPos = strstr( strTempRtsp, strAttr );
	if( pStartPos == NULL )
	{
		free( strTempRtsp );
		return FAILURE;
	}
	pStartPos += strlen(strAttr);
	pStartPos = strRtspMsg + (pStartPos - strTempRtsp);
	
	//skip blank
	while( (*pStartPos  == ' ') || (*pStartPos  == ':'))
	{
		pStartPos++;
	}

	*pValue = atoi(pStartPos);
	//sscanf( pStratPos, "%d", value );

	free( strTempRtsp );
	return SUCCESS;
}

int CRtspClient::GetStringAttribute( const char* strRtspMsg, const char *strAttr, char* pValue )
{
	if( (NULL == strRtspMsg) || (NULL == strAttr) || (NULL == pValue) )
	{
		return FAILURE;
	}

	char *strTempRtsp = strdup( strRtspMsg );
	if( NULL == strTempRtsp )
	{
		return FAILURE;
	}
	AllCharToLower( strTempRtsp );
	
	const char* pEndPos = NULL;
	const char* pStartPos = strstr( strTempRtsp, strAttr );
	if( NULL == pStartPos )
	{
		free( strTempRtsp );
		return FAILURE;
	}
	pStartPos += strlen(strAttr);
	pStartPos = strRtspMsg + (pStartPos - strTempRtsp);

	//skip blank
	while( (*pStartPos  == ' ') || (*pStartPos  == ':'))
	{
		pStartPos++;
	}
	//skip value
	pEndPos = pStartPos;
	while( (*pEndPos  != '\r') && (*pEndPos  != '\n') )
	{
		pEndPos++;
	}

	memcpy( pValue, pStartPos, pEndPos-pStartPos );
	pValue[pEndPos-pStartPos] = 0 ;
	free( strTempRtsp );
	return SUCCESS;
}


int CRtspClient::ParseResponseReply( const char* pRtspMsg )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}
	const char* strfind = strstr( pRtspMsg , "RTSP/1.0" );
	if( strfind == NULL )
	{
		return FAILURE;
    }
	strfind += strlen( "RTSP/1.0" );
	while( *strfind == ' ' ) 
    {
        strfind ++ ;
    }
	if( strncmp( strfind , "200" , strlen( "200" )) == 0 )
    {
		return RTSP_TYPE_OK;
    }
	else if( strncmp( strfind , "400" , strlen( "400") ) == 0 )
    {
		return RTSP_TYPE_BAD;
    }
    else if( strncmp( strfind , "401" , strlen( "401") ) == 0 )
    {
        return RTSP_TYPE_UNAUTHORIZED;
    }
    else if( strncmp( strfind , "404" , strlen( "404") ) == 0 )
    {
        return RTSP_TYPE_NOT_FOUND;
    }
    else if( strncmp( strfind , "405" , strlen( "405") ) == 0 )
    {
        return RTSP_TYPE_NOT_SUPPORTED;
    }
    else if( strncmp( strfind , "459" , strlen( "459") ) == 0 )
    {
        return RTSP_TYPE_NOT_ALLOWED;
    }
    else if( strncmp( strfind , "461" , strlen( "461") ) == 0 )
    {
        return RTSP_TYPE_UNSUPPORTED_TRANSPORT;
    }
	return FAILURE;
}

int CRtspClient::InitRtspAuth( )
{
	if( NULL == m_pAuth )
	{
		if( (!m_strUser.empty()) && (!m_strPassword.empty()))
		{
			m_pAuth = new CRtspAuth( m_strUser.c_str(), m_strPassword.c_str() );
		}
	}
	return SUCCESS;
}

int CRtspClient::ParseSessionId( const char* pstrRtspMsg )
{
	char *strTempRtsp = strdup( pstrRtspMsg );
	char pValue[128] = {0};
	if( NULL == strTempRtsp )
	{
		return FAILURE;
	}
	AllCharToLower( strTempRtsp );
	
	const char* pEndPos = NULL;
	const char* pStratPos = strstr( strTempRtsp, "session" );
	if( NULL == pStratPos )
	{
		free( strTempRtsp );
		return FAILURE;
	}
	pStratPos += strlen("session");
	pStratPos = pstrRtspMsg + (pStratPos - strTempRtsp);

	//skip blank
	while( (*pStratPos  == ' ') || (*pStratPos  == ':'))
	{
		pStratPos++;
	}
	//skip value
	pEndPos = pStratPos;
	while( (*pEndPos  != '\r') && (*pEndPos != '\n') && (*pEndPos  != ';'))
	{
		pEndPos++;
	}

	memcpy( pValue, pStratPos, pEndPos-pStratPos );
	pValue[pEndPos-pStratPos] = 0 ;
	free( strTempRtsp );

	m_SessionId = pValue;
	return SUCCESS;
}



