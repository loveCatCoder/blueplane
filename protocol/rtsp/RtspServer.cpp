/*
 * =====================================================================================
 *
 *	Filename:  STS_RtspServer.cpp
 *
 *	Description: 			
 *	Version:  1.0
 *	Created: 2019-06-12
 *
 *	Author:  wty
 *	Organization:  ICHINAE(BEIJING) TECH.CO.,LTD. 
 *
 * =====================================================================================
*/
#include "RtspServer.h"

CRtspServer::CRtspServer( )
{
	
}

CRtspServer::~CRtspServer( )
{
	
}

int CRtspServer::InitRtspServer( const char* strUrl, const char* strServerIp )
{
	if( (strUrl == NULL) || (strServerIp == NULL) )
	m_strUrl = strUrl;
	m_strServerIp = strServerIp;
	//init session id
	srand((unsigned)time(NULL));
	int nSession = rand();
	char tempBuf[64] = {0};
	snprintf( tempBuf, 64, "%x", nSession );
	m_strSession = tempBuf;

	return SUCCESS;
}

int CRtspServer::DeinitRtspServer( )
{
	return SUCCESS;
}

int CRtspServer::ParseRtspCommand( const char* strRtspMsg )
{
	if( NULL == strRtspMsg )
    {
        log_warn( "ParseRtspCommand parameter is error" );
        return FAILURE;
    }
    int nCommand = 0;
    if(strncmp( strRtspMsg ,ACTION_OPTIONS, strlen(ACTION_OPTIONS) ) == 0)
	{
		nCommand = RTSP_CMD_TYPE_OPTIONS;
	}
	else if(strncmp( strRtspMsg ,ACTION_DESCRIBE, strlen(ACTION_DESCRIBE) ) == 0)
	{
		nCommand = RTSP_CMD_TYPE_DESCRIBE;
	}
	else if(strncmp( strRtspMsg ,ACTION_SETUP, strlen(ACTION_SETUP) ) == 0)
	{
		nCommand = RTSP_CMD_TYPE_SETUP;
	}
	else if(strncmp( strRtspMsg ,ACTION_PLAY, strlen(ACTION_PLAY) ) == 0)
	{
		nCommand = RTSP_CMD_TYPE_PLAY;
	}
	else if(strncmp( strRtspMsg ,ACTION_TEARDOWN, strlen(ACTION_TEARDOWN) ) == 0)
	{
		nCommand = RTSP_CMD_TYPE_TEARDOWN;
	}
	else if(strncmp( strRtspMsg ,ACTION_PAUSE, strlen(ACTION_PAUSE) ) == 0)
	{
		nCommand = RTSP_CMD_TYPE_PAUSE;
	}
	else
	{
		log_warn( "rtsp server parse method error" );
	}
    return nCommand;
}

int CRtspServer::CreateOptionReply( char* pRtspMsg, int nRtspBufLen )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}
	if( SetReplyHeader( pRtspMsg, nRtspBufLen, 200 ) == -1 )
	{
		return FAILURE;
	}
	if( SetIntegerAttribute( pRtspMsg, nRtspBufLen, "CSeq", m_nCseq ) == -1 )
	{
		return FAILURE;
	}
	//create date
	char date[128] = {0};
    MakeDateHeader( date, 128 );
	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Date", date ) == -1 )
	{
		return FAILURE;
	}
	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Public", PUBLIC ) == -1 )
	{
		return FAILURE;
	}
	if( SetTail( pRtspMsg, nRtspBufLen ) == -1 )
	{
		return FAILURE;
	}
	return strlen( pRtspMsg );
}

int CRtspServer::CreateDescribeReply( char* pRtspMsg, int nRtspBufLen, STREAM_INFO &tStreamInfo )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}
	if( SetReplyHeader( pRtspMsg, nRtspBufLen, 200 ) == -1 )
	{
		return FAILURE;
	}
	if( SetIntegerAttribute( pRtspMsg, nRtspBufLen, "CSeq", m_nCseq ) == -1 )
	{
		return FAILURE;
	}
	//create date
	char date[128] = {0};
    MakeDateHeader( date, 128 );
	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Date", date ) == -1 )
	{
		return FAILURE;
	}
	
	//create sdp
	char SdpMsg[1024] = {0};
	int SdpLen = 0;
	SdpLen = CreateSdpMsg( SdpMsg, 1024, m_strServerIp.c_str(), tStreamInfo );
	if( SdpLen < 0 )
	{
		return FAILURE;
	}
	
	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Content-Base", m_strUrl.c_str() ) == -1 )
	{
		return FAILURE;
	}

	std::string strContentType = "applocation/sdp";
	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Content-Type", strContentType.c_str() ) == -1 )
	{
		return FAILURE;
	}
	
	if( SetIntegerAttribute( pRtspMsg, nRtspBufLen, "Content-Length", SdpLen ) == -1 )
	{
		return FAILURE;
	}

	AddSdpMsg( pRtspMsg, nRtspBufLen, SdpMsg );
	
	return strlen( pRtspMsg );
}

int CRtspServer::CreateSetupReply( char* pRtspMsg, int nRtspBufLen, RTSP_TRACK & tRtspTrack )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}
	if( SetReplyHeader( pRtspMsg, nRtspBufLen, 200 ) == -1 )
	{
		return FAILURE;
	}
	if( SetIntegerAttribute( pRtspMsg, nRtspBufLen, "CSeq", m_nCseq ) == -1 )
	{
		return FAILURE;
	}
	//create date
	char date[128] = {0};
    MakeDateHeader( date, 128 );
	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Date", date ) == -1 )
	{
		return FAILURE;
	}

	//create transport
	char transport[128] = {0};
	if( LINK_TYPE_UDP == tRtspTrack.m_nLinkType )
	{
		snprintf( transport, 128, "RTP/AVP;unicast;destination=%s;source=%s;client_port=%d-%d;server_port=%d-%d",
									tRtspTrack.m_strClientIp.c_str(), tRtspTrack.m_strServerIp.c_str(), 
									tRtspTrack.m_nClientRtpPort, tRtspTrack.m_nClientRtcpPort, 
									tRtspTrack.m_nServerRtpPort, tRtspTrack.m_nServerRtcpPort );
	}
	else if( LINK_TYPE_TCP == tRtspTrack.m_nLinkType )
	{
		snprintf( transport, 128, "RTP/AVP/TCP;unicast;destination=%s;source=%s;interleaved=%d-%d",
									tRtspTrack.m_strClientIp.c_str(), tRtspTrack.m_strServerIp.c_str(), 
									tRtspTrack.m_nClientRtpPort, tRtspTrack.m_nClientRtcpPort );
	}
	else
	{
		return FAILURE;
	}
	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Transport", transport ) == -1 )
	{
		return FAILURE;
	}
	
	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Session", m_strSession.c_str( ) ) == -1 )
	{
		return FAILURE;
	}

	if( SetTail( pRtspMsg, nRtspBufLen ) == -1 )
	{
		return FAILURE;
	}

	return strlen( pRtspMsg );
}

int CRtspServer::CreatePlayReply( char* pRtspMsg, int nRtspBufLen, float fStartTime, float fEndTime, const char *strRtpInfo )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}
	if( SetReplyHeader( pRtspMsg, nRtspBufLen, 200 ) == -1 )
	{
		return FAILURE;
	}
	if( SetIntegerAttribute( pRtspMsg, nRtspBufLen, "CSeq", m_nCseq ) == -1 )
	{
		return FAILURE;
	}
	//create date
	char date[128] = {0};
    MakeDateHeader( date, 128 );
	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Date", date ) == -1 )
	{
		return FAILURE;
	}
	//create range
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
	
	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "RTP-Info", strRtpInfo ) == -1 )
	{
		return FAILURE;
	}

	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Session", m_strSession.c_str( ) ) == -1 )
	{
		return FAILURE;
	}
	
	if( SetTail( pRtspMsg, nRtspBufLen ) == -1 )
	{
		return FAILURE;
	}

	return strlen( pRtspMsg );
}

int CRtspServer::CreatePauseReply( char* pRtspMsg, int nRtspBufLen )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}
	if( SetReplyHeader( pRtspMsg, nRtspBufLen, 200 ) == -1 )
	{
		return FAILURE;
	}
	if( SetIntegerAttribute( pRtspMsg, nRtspBufLen, "CSeq", m_nCseq ) == -1 )
	{
		return FAILURE;
	}
	//create date
	char date[128] = {0};
    MakeDateHeader( date, 128 );
	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Date", date ) == -1 )
	{
		return FAILURE;
	}
	if( SetTail( pRtspMsg, nRtspBufLen ) == -1 )
	{
		return FAILURE;
	}

	return strlen( pRtspMsg );
}

int CRtspServer::CreateTeardownReply( char* pRtspMsg, int nRtspBufLen )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}
	if( SetReplyHeader( pRtspMsg, nRtspBufLen, 200 ) == -1 )
	{
		return FAILURE;
	}
	if( SetIntegerAttribute( pRtspMsg, nRtspBufLen, "CSeq", m_nCseq ) == -1 )
	{
		return FAILURE;
	}
	//create date
	char date[128] = {0};
    MakeDateHeader( date, 128 );
	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Date", date ) == -1 )
	{
		return FAILURE;
	}
	
	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Session", m_strSession.c_str( ) ) == -1 )
	{
		return FAILURE;
	}
	
	if( SetTail( pRtspMsg, nRtspBufLen ) == -1 )
	{
		return FAILURE;
	}

	return strlen( pRtspMsg );
}

int CRtspServer::CreateErrorReply( char* pRtspMsg, int nRtspBufLen, int nReturnValue )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}
	if( SetReplyHeader( pRtspMsg, nRtspBufLen, nReturnValue ) == -1 )
	{
		return FAILURE;
	}
	if( SetIntegerAttribute( pRtspMsg, nRtspBufLen, "CSeq", m_nCseq ) == -1 )
	{
		return FAILURE;
	}
	//create date
	char date[128] = {0};
    MakeDateHeader( date, 128 );
	if( SetStringAttribute( pRtspMsg, nRtspBufLen, "Date", date ) == -1 )
	{
		return FAILURE;
	}
	if( SetTail( pRtspMsg, nRtspBufLen ) == -1 )
	{
		return FAILURE;
	}

	return strlen( pRtspMsg );
}

int CRtspServer::ParseOptionRequest( const char* pRtspMsg )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}
	int nRet = 0;
	nRet = GetIntegerAttribute( pRtspMsg, "cseq", &m_nCseq );
	if( nRet != SUCCESS )
	{
		return FAILURE;
	}

	log_info( "nRet = %d m_nCseq = %d", nRet, m_nCseq );
	return SUCCESS;
}

int CRtspServer::ParseDescribeRequest( const char* pRtspMsg )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}
	int nRet = 0;
	nRet = GetIntegerAttribute( pRtspMsg, "cseq", &m_nCseq );
	if( nRet != SUCCESS )
	{
		return FAILURE;
	}
	if( strstr( pRtspMsg, "sdp") == NULL )
	{
		return FAILURE;
	}
	return SUCCESS;
}

int CRtspServer::ParseSetupRequest( const char* pRtspMsg, int *nTrack, int *nLinkType, int *nRtpPort, int *nRtcpPort )
{
	if( (NULL == pRtspMsg) || (NULL == nTrack) || (NULL == nLinkType) || (NULL == nRtpPort) || (NULL == nRtcpPort) )
	{
		return FAILURE;
	}
	int nRet = 0;
	nRet = GetIntegerAttribute( pRtspMsg, "cseq", &m_nCseq );
	if( nRet != SUCCESS )
	{
		return FAILURE;
	}
	
	if( strstr(pRtspMsg, "track1") != NULL )
	{
		*nTrack = VIDEO_TRACK;
		nRet = ParseTransport( pRtspMsg, nLinkType, nRtpPort, nRtcpPort );
	}
	else if( strstr(pRtspMsg, "track2") != NULL )
	{
		*nTrack = AUDIO_TRACK;
		nRet = ParseTransport( pRtspMsg, nLinkType, nRtpPort, nRtcpPort );
	}
	else
	{
		nRet = FAILURE;
	}

	return nRet;
}

int CRtspServer::ParsePlayRequest( const char* pRtspMsg, float* nStartTime, float* nEndTime, float* fScale )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}
	int nRet = 0;
	nRet = GetIntegerAttribute( pRtspMsg, "cseq", &m_nCseq );
	if( nRet != SUCCESS )
	{
		return FAILURE;
	}
	
	char tempBuf[1024] = {0};
	//get range
	nRet = GetStringAttribute( pRtspMsg, "range", tempBuf );
	if( SUCCESS != nRet )
	{
		return nRet;
	}
	
	if( strstr( tempBuf, "npt" ) == NULL )
	{
		return FAILURE;
	}

	sscanf( tempBuf, "npt=%f-%f\r\n" , nStartTime , nEndTime );

	//get scale
	nRet = GetFloatAttribute( pRtspMsg, "scale", fScale );
	if( nRet != SUCCESS )
	{
		nRet = GetFloatAttribute( pRtspMsg, "speed", fScale );
		if( nRet != SUCCESS )
		{
			*fScale = 0;
		}
	}
	
	return SUCCESS;
}

int CRtspServer::ParsePauseRequest( const char* pRtspMsg )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}
	int nRet = 0;
	nRet = GetIntegerAttribute( pRtspMsg, "cseq", &m_nCseq );
	if( nRet != SUCCESS )
	{
		return FAILURE;
	}
	return SUCCESS;
}

int CRtspServer::ParseTeardownRequest( const char* pRtspMsg )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}
	int nRet = 0;
	nRet = GetIntegerAttribute( pRtspMsg, "cseq", &m_nCseq );
	if( nRet != SUCCESS )
	{
		return FAILURE;
	}
	return SUCCESS;
}


int CRtspServer::AppendInteger( STRING_INFO *pInfo, int nAttr )
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

int CRtspServer::AppendString( STRING_INFO *pInfo, const char *strAttr )
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

int CRtspServer::SetIntegerAttribute( char* strRtspMsg, int nLength, const char *strAttr, int nValue )
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

int CRtspServer::SetStringAttribute( char *strRtspMsg, int nLength, const char *strAttr, const char *strValue )
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

int CRtspServer::SetReplyHeader( char *strRtspMsg, int nLength, int nReturnValue )
{
	if( NULL == strRtspMsg )
	{
		return FAILURE;
	}

	if( 200 == nReturnValue )
	{
		snprintf( strRtspMsg, nLength, "RTSP/1.0 200 OK\r\n" );
	}
	else
	{
		snprintf( strRtspMsg, nLength, "RTSP/1.0 %d ERROR\r\n", nReturnValue );
	}
	
	return strlen( strRtspMsg );
}

int CRtspServer::SetTail( char *strRtspMsg, int nLength )
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


void CRtspServer::AllCharToLower( char *pBuf ) 
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

int CRtspServer::GetIntegerAttribute( const char* strRtspMsg, const char *strAttr, int* pValue )
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

int CRtspServer::GetFloatAttribute( const char* strRtspMsg, const char *strAttr, float* pValue )
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

	sscanf( pStartPos, "%f", pValue );

	free( strTempRtsp );
	return SUCCESS;
}


int CRtspServer::GetStringAttribute( const char* strRtspMsg, const char *strAttr, char* pValue )
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


int CRtspServer::MakeDateHeader( char *pDate , int nDateLen ) 
{
    if( NULL == pDate )
    {
        return FAILURE;
    }
	time_t tt = time(NULL);
	struct tm ptm = {0};
	localtime_r(&tt, &ptm);
	//strftime(buf, TIME_HEADER_LEN, "Date: %a, %b %d %Y %H:%M:%S GMT", gmtime(&tt));
	strftime( pDate, nDateLen, "%a, %b %d %Y %H:%M:%S GMT", &ptm );
	return SUCCESS;
}

int CRtspServer::CreareSdpVideoMsg( char* pRtspMsg, int nRtspMsgLen, STREAM_INFO &tStreamInfo )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}

	std::string strLoadType;
	if( ENCODE_H264 == tStreamInfo.m_nVideoFormat )
	{
		strLoadType = "H264/90000";
	}
	else if( ENCODE_H264 == tStreamInfo.m_nVideoFormat )
	{
		strLoadType = "SVAC";
	}
	else
	{
		return FAILURE;
	}
	
	int nLength = 0;
	nLength = snprintf( pRtspMsg, nRtspMsgLen,
		"m=video 0 RTP/AVP 96\r\n"
		"c=IN IP4 0.0.0.0\r\n"
		"b=AS:12000\r\n"
		"a=rtpmap:96 %s\r\n"
		"a=fmtp:96 packetization=1;profile-level-id=64002A;sprop-parameter-sets=%s,%s\r\n"
		"a=control:track1\r\n",
		strLoadType.c_str(),
		tStreamInfo.m_strBase64Sps.c_str(), 
		tStreamInfo.m_strBase64Pps.c_str());
	return nLength;
}

int CRtspServer::CreateSdpAudioMsg( char* pRtspMsg, int nRtspMsgLen, STREAM_INFO &tStreamInfo )
{
	if( NULL == pRtspMsg )
	{
		return FAILURE;
	}
	int nLength = 0;
	return nLength;
}

int CRtspServer::CreateSdpMsg( char* pRtspMsg, int nRtspMsgLen, const char* strServerIp, STREAM_INFO &tStreamInfo )
{
	if( (NULL == pRtspMsg) || (NULL == strServerIp) )
	{
		return FAILURE;
	}

	char SdpVideoInfo[512] = {0};
	char SdpAudioInfo[512] = {0};
	CreareSdpVideoMsg( SdpVideoInfo, 512, tStreamInfo );
	if( tStreamInfo.m_bAudioEnable )
	{
		CreateSdpAudioMsg( SdpAudioInfo, 512, tStreamInfo );
	}

	int nLength = 0;
	nLength = snprintf( pRtspMsg, nRtspMsgLen,
				"v=0\r\n"
				"o=%s %d IN IP4 %s\r\n"
				"s=%s\r\n"
				"i=%s\r\n"
				"t=0 0\r\n"
				"a=x-qt-text-nam:%s\r\n"
				"a=x-qt-text-inf:%s\r\n"
				"%s"
				"%s",
				SDP_SESSION_ID, SDP_VERSION, strServerIp,
				SDP_SESSION_NAME,
				SDP_SESSION_INFO,
				SDP_SESSION_NAME,
				SDP_SESSION_INFO,
				SdpVideoInfo,
				SdpAudioInfo);
	return nLength;	
}

int CRtspServer::AddSdpMsg( char* pRtspMsg, int nRtspMsgLen, const char* SdpMsg )
{
	if( (NULL == pRtspMsg) || (NULL == SdpMsg) )
	{
		return FAILURE;
	}

	strcat( pRtspMsg, "\r\n");
	strcat( pRtspMsg, SdpMsg );
	return strlen( pRtspMsg );
}

int CRtspServer::ParseTransport( const char* pRtspMsg, int *nLinkType, int *nRtpPort, int *nRtcpPort )
{
	if( (NULL == pRtspMsg) || (NULL == nLinkType) || (NULL == nRtpPort) || (NULL == nRtcpPort) )
	{
		return FAILURE;
	}
	char Transport[512] = {0};
	if( GetStringAttribute( pRtspMsg, "transport", Transport ) == FAILURE )
	{
		log_warn("error : not fount transport");
		return FAILURE;
	}

	if(strncmp( Transport, "RTP/AVP;", strlen("RTP/AVP;")) == 0 )
	{
		*nLinkType = LINK_TYPE_UDP;
	}
	else if(strncmp( Transport, "RTP/AVP/UDP", strlen("RTP/AVP/UDP")) == 0 )
	{
		*nLinkType = LINK_TYPE_UDP;
	}
	else if(strncmp( Transport, "RTP/AVP/TCP", strlen("RTP/AVP/TCP")) == 0 )
	{
		*nLinkType = LINK_TYPE_TCP;
	}
	else
	{
		log_warn("net type is not tcp and udp");
		return FAILURE;
	}

	char * pStartPos = NULL;
	if( *nLinkType == LINK_TYPE_UDP )
	{
		pStartPos = strstr( Transport, "client_port=" );
		if( pStartPos == NULL )
		{
			log_warn("not found client port");
			return FAILURE;
		}
		pStartPos += strlen("client_port=");
		sscanf( pStartPos , "%d-%d\r\n" , nRtpPort, nRtcpPort );
	}
	else
	{
		pStartPos = strstr( Transport, "interleaved=" );
		if( pStartPos == NULL )
		{
			log_warn("not found interleaved");
			return FAILURE;
		}
		pStartPos += strlen("interleaved=");
		sscanf( pStartPos , "%d-%d\r\n" , nRtpPort, nRtcpPort );
	}

	return SUCCESS;
}

