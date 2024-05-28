/*
 * =====================================================================================
 *
 *	Filename:  RtcpSession.cpp
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
#include "RtcpSession.h"

CRtcpSession::CRtcpSession( )
{
	m_pRtpSession = NULL;
	m_nOwnSSRC = 0;
	m_nSenderSSRC = 0;
	m_llNtpTime = 0;
	m_nRtpTime = 0;
	m_nRtpPackageTotal = 0;
	m_nRecvTime = 0;
	m_nCNameLen = 0;
}

CRtcpSession::~CRtcpSession( )
{
	m_pRtpSession = NULL;
}

int CRtcpSession::InitRtcpSession( CRtpSession* pRtpSession )
{
	if( NULL == pRtpSession )
	{
		return RTCP_FAILURE;
	}
	m_pRtpSession = pRtpSession;
	//init cname item
	int nTempLen;
	memset( m_pCName, 0, 128 );
	m_pCName[0] = 1;
	m_pCName[1] = strlen(RTCP_CNAME);
	snprintf( m_pCName + 2, CNAME_MAX_LEN - 2, "%s", RTCP_CNAME );
	
	nTempLen = 2 + m_pCName[1];
	if( 0 != (nTempLen % 4) )
	{
		m_nCNameLen = (nTempLen>>2) + 1;
	}
	else
	{
		m_nCNameLen = nTempLen>>2;
	}

	//init ssrc
	struct timeval tv;
	gettimeofday(&tv, NULL);
	srand(tv.tv_usec);
	m_nOwnSSRC = rand();
	
	return RTCP_SUCCESS;
}

int CRtcpSession::DeinitRtcpSession( )
{
	return RTCP_SUCCESS;
}

int CRtcpSession::CreateSRPackage( unsigned char* pBuf, int *pnLen )
{
	if( ( NULL == pBuf ) || ( NULL == pnLen ) )
	{
		return RTCP_FAILURE;
	}
	unsigned int * p32Writer = reinterpret_cast<unsigned int*>( pBuf );
	//unsigned long long * p64Writer = NULL;
	int nCurrentLen = 0;
	//header
	*p32Writer++ = htonl( 0x80c80006 );
	nCurrentLen += 4;
	
	//ssrc
	*p32Writer++ = htonl( m_nOwnSSRC );
	nCurrentLen += 4;
	
	//NTP time
	struct timeval time_now;
	gettimeofday(&time_now, NULL);
	double temp = (time_now.tv_usec/15625.0)*0x04000000; 
	//sr_packet->ntp_timstamp_high = htonl(time_now.tv_sec + 8*3600 + 0x83AA7E80);
	*p32Writer++ = htonl(time_now.tv_sec + 0x83AA7E80);
	*p32Writer++ = htonl((unsigned int)(temp+0.5));
	nCurrentLen += 8;
	
	//rtp time
	unsigned int nRtpTimeStamp = m_pRtpSession->GetRtpTimeStamp( );
	*p32Writer++ = htonl( nRtpTimeStamp );
	nCurrentLen += 4;

	//rtp package num
	unsigned int nSendPackageNum = m_pRtpSession->GetRtpSendPackageNum( );
	*p32Writer++ = htonl( nSendPackageNum );
	nCurrentLen += 4;

	//rtp package byte
	unsigned int nSendPackageBtye = m_pRtpSession->GetRtpSendPackageByte( );
	*p32Writer++ = htonl( nSendPackageBtye );
	nCurrentLen += 4;

	*pnLen = nCurrentLen;
	return RTCP_SUCCESS;
}

int CRtcpSession::CreateRRPackage( unsigned char* pBuf, int *pnLen )
{
	if( ( NULL == pBuf ) || ( NULL == pnLen ) )
	{
		return RTCP_FAILURE;
	}
	unsigned int * p32Writer = reinterpret_cast<unsigned int*>( pBuf );
	int nCurrentLen = 0;


	//header
	*p32Writer++ = htonl( 0x81c90007 );
	nCurrentLen += 4;
	//ssrc
	*p32Writer++ = htonl( m_nOwnSSRC );
	nCurrentLen += 4;
	*p32Writer++ = htonl( m_nSenderSSRC );
	nCurrentLen += 4;
	//lost packets
	unsigned int nLost;
	unsigned char cLostRate = 0;
	unsigned int nRecvPackets =  m_pRtpSession->GetRtpPackage();
	int nLostPackets = m_nRtpPackageTotal - nRecvPackets;
	if( nLostPackets <= 0 )
	{
		nLost = 0;
	}
	else if( m_nRtpPackageTotal != 0 )
	{
		cLostRate = (nLostPackets<<8)/m_nRtpPackageTotal;
		nLost = (cLostRate << 24) |  nLostPackets ;
	}
	else
	{
		nLost = 0;
	}
	*p32Writer++ = htonl( nLost );
	nCurrentLen += 4;
	//last rtp seq 
	int nRtpExtendSeq = m_pRtpSession->GetExtendSeq( );
	*p32Writer++ = htonl( nRtpExtendSeq );
	nCurrentLen += 4;
	//jitter
	int nJitter = m_pRtpSession->GetJitter( );
	*p32Writer++ = htonl( nJitter );
	nCurrentLen += 4;
	//lsr
	unsigned int unLsr = ( m_llNtpTime & 0xffffffff0000 ) >> 16;
	*p32Writer++ = htonl( unLsr );
	nCurrentLen += 4;
	//dlsr
	unsigned int nCurrentTime;
	nCurrentTime = GetTimeMsec( );
	unsigned int unDlsr = ( ( nCurrentTime - m_nRecvTime ) << 16 ) / 1000;
	*p32Writer++ = htonl( unDlsr );
	nCurrentLen += 4;

	*pnLen = nCurrentLen;
	return RTCP_SUCCESS;
}

int CRtcpSession::CreateSDESPackage( unsigned char* pBuf, int *pnLen )
{
	if( ( NULL == pBuf ) || ( NULL == pnLen ) )
	{
		return RTCP_FAILURE;
	}
	unsigned int * p32Writer = reinterpret_cast<unsigned int*>( pBuf );
	int nCurrentLen = 0;
	int nCNameByte = m_nCNameLen*4;
	//SD_LOGDBG("m_nCNameLen = %d, m_pCName = %s", m_nCNameLen, m_pCName + 2);
	//header
	*p32Writer++ = htonl( 0x81ca0001 + m_nCNameLen );
	nCurrentLen += 4;
	//ssrc
	*p32Writer = htonl( m_nOwnSSRC );
	nCurrentLen += 4;
	//cname
	memcpy( pBuf + nCurrentLen , m_pCName, nCNameByte );
	nCurrentLen += nCNameByte;
	
	*pnLen = nCurrentLen;
	return RTCP_SUCCESS;
}

int CRtcpSession::CreateBYEPackage( unsigned char* pBuf, int *pnLen )
{
	if( ( NULL == pBuf ) || ( NULL == pnLen ) )
	{
		return RTCP_FAILURE;
	}
	unsigned int * p32Writer = reinterpret_cast<unsigned int*>( pBuf );
	int nCurrentLen = 0;
	//header
	*p32Writer++ = htonl( 0x81cb0001 );
	nCurrentLen += 4;
	//ssrc
	*p32Writer = htonl( m_nOwnSSRC );
	nCurrentLen += 4;	

	*pnLen = nCurrentLen;
	return RTCP_SUCCESS;
}

int CRtcpSession::ParseSRPackage( const unsigned char* pBuf )
{
	if( (NULL == pBuf) )
	{
		return RTCP_FAILURE;
	}
	const unsigned int * p32Reader = reinterpret_cast<const unsigned int *>( pBuf );
	const unsigned long long * p64Reader = NULL;

	//header
	p32Reader++;
	//sr ssrc
	m_nSenderSSRC = ntohl(*p32Reader);
	p32Reader++;
	//ntp_time
	p64Reader = reinterpret_cast<const unsigned long long *>( p32Reader );
	m_llNtpTime = RtcpNtohll( *p64Reader );
	p32Reader += 2;
	//rtp_time
	m_nRtpTime = ntohl(*p32Reader);
	p32Reader++;
	//send rtp package tatol
	m_nRtpPackageTotal = ntohl(*p32Reader);

	return RTCP_SUCCESS;
}

int CRtcpSession::ParseRRPackage( const unsigned char* pBuf )
{
	if( NULL == pBuf )
	{
		return RTCP_FAILURE;
	}
	//const unsigned int * p32Reader = reinterpret_cast<const unsigned int *>( pBuf );
	return RTCP_SUCCESS;
}

int CRtcpSession::ParseSDESPackage( const unsigned char* pBuf )
{
	if( NULL == pBuf )
	{
		return RTCP_FAILURE;
	}
	//const unsigned int * p32Reader = reinterpret_cast<const unsigned int *>( pBuf );

	return RTCP_SUCCESS;
}

int CRtcpSession::ParseBYEPackage( const unsigned char* pBuf )
{
	if( NULL == pBuf )
	{
		return RTCP_FAILURE;
	}
	//const unsigned int * p32Reader = reinterpret_cast<const unsigned int *>( pBuf );

	return RTCP_SUCCESS;
}

unsigned long long CRtcpSession::RtcpHtonll( unsigned long long HostValue )
{
	unsigned long long NetValue;
	NetValue = (HostValue << 56)| 
				(0x00ff000000000000 & (HostValue << 40)) |   //0x00ff0000 00000000
				(0x0000ff0000000000 & (HostValue << 24)) |
				(0x000000ff00000000 & (HostValue << 8)) |
				((0xff000000) & (HostValue >> 8)) |
				((0x00ff0000) & (HostValue >> 24)) |
				((0x0000ff00) & (HostValue >> 40)) |
				((0x000000ff) & (HostValue>> 56));
	return NetValue;
}

unsigned long long CRtcpSession::RtcpNtohll( unsigned long long NetValue )
{
	unsigned long long HostValue;
	HostValue = (NetValue << 56)| 
				(0x00ff000000000000 & (NetValue << 40)) |
				(0x0000ff0000000000 & (NetValue << 24)) |
				(0x000000ff00000000 & (NetValue << 8)) |
				((0xff000000) & (NetValue >> 8)) |
				((0x00ff0000) & (NetValue >> 24)) |
				((0x0000ff00) & (NetValue >> 40)) |
				((0x000000ff) & (NetValue>> 56));
	return HostValue;
}

unsigned int CRtcpSession::GetTimeMsec( )
{
	unsigned int nMsec = 0;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	nMsec = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return nMsec;
}

int CRtcpSession::SetRecvTime( )
{
	m_nRecvTime = GetTimeMsec( );
	return RTCP_SUCCESS;
}

int CRtcpSession::CreateRtcpPackage( E_RTCP_PACKET_TYPE Type, unsigned char* pBuf, int *pnLen )
{
	if( (NULL == pBuf) || (NULL == pnLen) )
	{
		return RTCP_FAILURE;
	}
	int nRet = RTCP_FAILURE;
    if( Type == RTCP_SR )
	{
		nRet = CreateSRPackage( pBuf, pnLen );
	}
	else if( Type == RTCP_RR )
	{
		nRet = CreateRRPackage( pBuf, pnLen );
	}
	else if( Type == RTCP_SDES )
	{
		nRet = CreateSDESPackage( pBuf, pnLen );
	}
	else if( Type == RTCP_BYE )
	{
		nRet = CreateBYEPackage( pBuf, pnLen );
	}
	else
	{
		//error
		nRet = RTCP_FAILURE;
	}
	return nRet;
}

int CRtcpSession::GetPackageLen( const char* pBuf )
{
	unsigned short nLen = 0;
	memcpy( &nLen, pBuf+ 2, sizeof(unsigned short));
	nLen = ntohs(nLen);
	return (nLen*4 + 4);
}

int CRtcpSession::ParseRtcpPackage( const char* pBuf )
{
	if( NULL == pBuf)
	{
		return RTCP_FAILURE;
	}
	int nType = 0;
	int nRet = RTCP_SUCCESS;
	unsigned char * pos = (unsigned char*)pBuf;
	nType = *(pos + 1);
	if( nType == RTCP_SR )
    {
        nRet = ParseSRPackage( (unsigned char*)pBuf );
    }
    else if( nType == RTCP_RR )
    {
        nRet = ParseRRPackage( (unsigned char*)pBuf );
    }
    else if( nType == RTCP_SDES )
    {
        nRet = ParseSDESPackage( (unsigned char*)pBuf );
    }
    else if( nType == RTCP_BYE )
    {
    	nRet = ParseBYEPackage( (unsigned char*) pBuf );
    }
    else
    {
        nRet = RTCP_FAILURE;
    }
    return nRet;
}

