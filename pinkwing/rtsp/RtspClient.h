/*
 * =====================================================================================
 *
 *	Filename:  RtspClient.h
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
#ifndef __RTSP_CLIENT_H__
#define __RTSP_CLIENT_H__
#include "RtspAuth.h"
#include "Sdp.h"
#include "include/private.h"



#define USER_AGENT    "IChinaE RTSP STREAM ( Streaming Media v2019.04.12 )"
#define ACCPET_SDP    "application/sdp"
#define PUBLIC        "OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE"

#define RTSP_LINK_UDP  1
#define RTSP_LINK_TCP  2

#define AUTH_LEN       512




typedef enum
{
	RTSP_TYPE_OK = 200,
	RTSP_TYPE_BAD = 400,
	RTSP_TYPE_UNAUTHORIZED = 401,
    RTSP_TYPE_NOT_FOUND = 404,
    RTSP_TYPE_NOT_SUPPORTED = 405,
    RTSP_TYPE_NOT_ALLOWED = 459,
    RTSP_TYPE_UNSUPPORTED_TRANSPORT = 461,
}RTSP_RETURN_CODE;

class CRtspClient
{
public:
	CRtspClient( );
	virtual ~CRtspClient( );
public:
	int InitRtspClient( const char* strUrl, const char * strUser, const char * strPassword );
	int DeinitRtspClinet( );

	int CreateOptionRequest( char* pRtspMsg, int nRtspBufLen );
	int CreateDescribeRequest( char* pRtspMsg, int nRtspBufLen );
	int CreateSetupRequest( char* pRtspMsg, int nRtspBufLen, E_TRACK_TYPE nTrackType, int nNetType, int nRtpPort, int nRtcpPort );
	int CreatePlayRequest( char* pRtspMsg, int nRtspBufLen, float fStartTime, float fEndTime );
	int CreateTeardownRequest( char* pRtspMsg, int nRtspBufLen );

	int ParseOptionReply( const char* pRtspMsg );
	int ParseDescribeReply( const char* pRtspMsg );
	int ParseSetupReply( const char* pRtspMsg, int* pRtpPort, int* pRtcpPort );
	int ParsePlayReply( const char* pRtspMsg, int* pVideoSeq, unsigned int* pVideoRtpTime, int * pAudioSeq, unsigned int* pAudioRtpTime );
	int ParseTeardownReply( const char* pRtspMsg );

	std::string GetOptions( );
	std::string GetSessionId( );
	std::string GetVideoTrack( );
	std::string GetAudioTrack( );
	SDP_MEDIA_INFO * GetVideoMediaInfo( );
	SDP_MEDIA_INFO * GetAudioMediaInfo( );
	int ParseResponseReply( const char* pRtspMsg );
private:
	int AppendInteger( STRING_INFO *pInfo, int nAttr );
	int AppendString( STRING_INFO *pInfo, const char *strAttr );
	int SetIntegerAttribute( char* strRtspMsg, int nLength, const char *strAttr, int nValue );
	int SetStringAttribute( char *strRtspMsg, int nLength, const char *strAttr, const char *strValue );
	int SetRequestHeader( char *strRtspMsg, int nLength, const char* strMethod , const char *strUrl );
	int SetTail( char *strRtspMsg, int nLength );

	void AllCharToLower( char *pBuf );
	int GetIntegerAttribute( const char* strRtspMsg, const char *strAttr, int* pValue );
	int GetStringAttribute( const char* strRtspMsg, const char *strAttr, char* pValue );
	int InitRtspAuth( );
	int ParseSessionId( const char* pstrRtspMsg );

private:
	std::string m_strUrl;
	std::string m_strUser;
	std::string m_strPassword;

	std::string m_strOption;
	std::string m_strContentBase;
	std::string m_strVideoUrl;
	std::string m_strAudioUrl;

	int m_nCseq;
	int m_nNetType;
	std::string m_SessionId;
	
	CRtspAuth * m_pAuth;
	CSdpSession m_SdpSession;
};

#endif
