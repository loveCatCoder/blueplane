/*
 * =====================================================================================
 *
 *	Filename:  STS_RtspServer.h
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
#ifndef __STS_RTSP_SERVER_H__
#define __STS_RTSP_SERVER_H__

#include "include/private.h"



struct RTSP_TRACK
{
public:
	RTSP_TRACK( )
	{
		m_nLinkType = 0;
		m_nClientRtpPort = 0;
		m_nClientRtcpPort = 0;
		m_nServerRtpPort = 0;
		m_nServerRtcpPort = 0;
	}
	int m_nLinkType;
	std::string m_strClientIp;
	std::string m_strServerIp;

	int m_nClientRtpPort;
	int m_nClientRtcpPort;
	int m_nServerRtpPort;
	int m_nServerRtcpPort;
};


typedef enum
{
	RTSP_CMD_TYPE_OPTIONS = 1,
	RTSP_CMD_TYPE_DESCRIBE = 2,
	RTSP_CMD_TYPE_SETUP = 3,
	RTSP_CMD_TYPE_PLAY = 4,
	RTSP_CMD_TYPE_PAUSE = 5,
	RTSP_CMD_TYPE_TEARDOWN = 6,
}E_RTSP_CMD_TYPE;




#define AUTH_MAX_LEN  512

#define ACTION_OPTIONS			"OPTIONS"
#define ACTION_DESCRIBE			"DESCRIBE"
#define ACTION_SETUP			"SETUP"
#define ACTION_TEARDOWN			"TEARDOWN"
#define ACTION_PLAY				"PLAY"
#define ACTION_PAUSE			"PAUSE"
#define ACTION_GET_PARAMETER	"GET_PARAMETER"
#define ACTION_SET_PARAMETER	"SET_PARAMETER"

#define USER_AGENT    "IChinaE RTSP STREAM ( Streaming Media v2019.04.12 )"
#define ACCPET_SDP    "application/sdp"
#define PUBLIC        "OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE"

#define SDP_SESSION_ID "20190613"
#define SDP_VERSION 1
#define SDP_SESSION_NAME "StreamTS Transfer Server"
#define SDP_SESSION_INFO "StreamTS Streaming Media"


class CRtspServer
{
public:
	CRtspServer( );
	virtual ~CRtspServer( );
public:
	int InitRtspServer( const char* strUrl, const char* strServerIp );
	int DeinitRtspServer( );

	int ParseRtspCommand( const char* strRtspMsg );

	int CreateOptionReply( char* pRtspMsg, int nRtspBufLen );
	int CreateDescribeReply( char* pRtspMsg, int nRtspBufLen, STREAM_INFO &tStreamInfo );
	int CreateSetupReply( char* pRtspMsg, int nRtspBufLen, RTSP_TRACK & tRtspTrack );
	int CreatePlayReply( char* pRtspMsg, int nRtspBufLen, float fStartTime, float fEndTime, const char *strRtpInfo );
	int CreatePauseReply( char* pRtspMsg, int nRtspBufLen );
	int CreateTeardownReply( char* pRtspMsg, int nRtspBufLen );
	int CreateErrorReply( char* pRtspMsg, int nRtspBufLen, int nReturnValue );

	int ParseOptionRequest( const char* pRtspMsg );
	int ParseDescribeRequest( const char* pRtspMsg );
	int ParseSetupRequest( const char* pRtspMsg, int *nTrack, int *nLinkType, int *nRtpPort, int *nRtcpPort );
	int ParsePlayRequest( const char* pRtspMsg, float* nStartTime, float* nEndTime, float* fScale );
	int ParsePauseRequest( const char* pRtspMsg );
	int ParseTeardownRequest( const char* pRtspMsg );

private:
	int AppendInteger( STRING_INFO *pInfo, int nAttr );
	int AppendString( STRING_INFO *pInfo, const char *strAttr );
	int SetIntegerAttribute( char* strRtspMsg, int nLength, const char *strAttr, int nValue );
	int SetStringAttribute( char *strRtspMsg, int nLength, const char *strAttr, const char *strValue );
	int SetReplyHeader( char *strRtspMsg, int nLength, int nReturnValue );
	int SetTail( char *strRtspMsg, int nLength );
	void AllCharToLower( char *pBuf ) ;
	int GetIntegerAttribute( const char* strRtspMsg, const char *strAttr, int* pValue );
	int GetFloatAttribute( const char* strRtspMsg, const char *strAttr, float* pValue );
	int GetStringAttribute( const char* strRtspMsg, const char *strAttr, char* pValue );
	int MakeDateHeader( char *pDate , int nDateLen );

	int CreareSdpVideoMsg( char* pRtspMsg, int nRtspMsgLen, STREAM_INFO &tStreamInfo );
	int CreateSdpAudioMsg( char* pRtspMsg, int nRtspMsgLen, STREAM_INFO &tStreamInfo );
	int CreateSdpMsg( char* pRtspMsg, int nRtspMsgLen, const char* strServerIp, STREAM_INFO &tStreamInfo );
	int AddSdpMsg( char* pRtspMsg, int nRtspMsgLen, const char* SdpMsg );
	int ParseTransport( const char* pRtspMsg, int *nLinkType, int *nRtpPort, int *nRtcpPort );
private:
	int m_nCseq;
	std::string m_strUrl;
	std::string m_strServerIp;
	std::string m_strSession;
};

#endif
