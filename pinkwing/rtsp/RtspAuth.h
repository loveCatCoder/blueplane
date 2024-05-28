/*
 * =====================================================================================
 *
 *	Filename:  RtspAuth.h
 *
 *	Description: 			
 *	Version:  1.0
 *	Created: 2019-04-18
 *
 *	Author:  wty
 *	Organization:  ICHINAE(BEIJING) TECH.CO.,LTD. 
 *
 * =====================================================================================
*/
#ifndef __RTSP_AUTH_H__
#define __RTSP_AUTH_H__
#include "include/private.h"

typedef enum
{
	AUTH_DIGEST,
	AUTH_BASIC,
}E_AUTH_TYPE;

class CRtspAuth
{
public:
	CRtspAuth( const char* strUser, const char* strPassword );
	virtual ~CRtspAuth( );
public:
	int ParseAuthData( const char* pMsg );
	int ComputeDigestRequest( char* pDigest, const char* strUserName, const char* strPassword, const char* strCmd, const char* strUrl, const char* strRealm, const char* strNonce );
	int ComputeBasicRequest( char* pBasic, const char* strUserName, const char* strPassword );
	int GetAuth( char* pBuf, int nBufLen, const char* strCmd, const char* strUrl );
private:
	void AllCharToLower( char *pBuf );
	int GetStringAttribute( const char* pMsg, const char *strAttr, char* pValue );
	int CheckStringAttributeExist( const char* pMsg, const char *strAttr );
private:
	int m_nAuthType;
	std::string m_strUserName;
	std::string m_strPassword;
	
	std::string m_strRealm;
	std::string m_strNonce;
};

#endif
