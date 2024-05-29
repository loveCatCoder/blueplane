/*
 * =====================================================================================
 *
 *	Filename:  RtcpSession.h
 *
 *	Description: 
 *	Version:  1.0
 *	Created: 2019-04-16
 *
 *	Author:  wty
 *	Organization:  ICHINAE(BEIJING) TECH.CO.,LTD. 
 *
 * =====================================================================================
*/
#ifndef __RTCP_SESSION_H__
#define __RTCP_SESSION_H__

#include "RtpSession.h"
#include "RtcpPrivate.h"

typedef enum
{
	RTCP_SR = 200,
	RTCP_RR = 201,
	RTCP_SDES = 202,
	RTCP_BYE = 203,
}E_RTCP_PACKET_TYPE;

class CRtcpSession
{
public:
	CRtcpSession( );
	virtual ~CRtcpSession( );
public:	
	int InitRtcpSession( CRtpSession* pRtpSession );
	int DeinitRtcpSession( );
	int SetRecvTime( );
	
	int CreateRtcpPackage( E_RTCP_PACKET_TYPE Type, unsigned char* pBuf, int *pnLen );
	int GetPackageLen( const char* pBuf );
	int ParseRtcpPackage( const char* pBuf );
private:
	int CreateSRPackage( unsigned char* pBuf, int *pnLen );
	int CreateRRPackage( unsigned char* pBuf, int *pnLen );
	int CreateSDESPackage( unsigned char* pBuf, int *pnLen );
	int CreateBYEPackage( unsigned char* pBuf, int *pnLen );
	
	int ParseSRPackage( const unsigned char* pBuf );
	int ParseRRPackage( const unsigned char* pBuf );
	int ParseSDESPackage( const unsigned char* pBuf );
	int ParseBYEPackage( const unsigned char* pBuf );
	unsigned long long RtcpHtonll( unsigned long long HostValue );
	unsigned long long RtcpNtohll( unsigned long long NetValue );
	unsigned int GetTimeMsec( );
private:
	CRtpSession * m_pRtpSession;

	unsigned int m_nOwnSSRC;
	unsigned int m_nSenderSSRC;
	unsigned long long m_llNtpTime;
	unsigned int m_nRtpTime;
	
	unsigned int m_nRtpPackageTotal;
	unsigned int m_nRecvTime;
	int m_nCNameLen;
	char m_pCName[CNAME_MAX_LEN];
};

#endif
