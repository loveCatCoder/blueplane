/*
 * =====================================================================================
 *
 *	Filename:  RtpSession.cpp
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
#include "RtpSession.h"
#include "RtpPayloadFactory.h"


CRtpSession::CRtpSession( )
{
	m_pRtpPayload = NULL;

	m_nSSRC = 0;
	RtpHeaderExVersion = 0;
	m_nRtpCseq = 0;
	
	m_funFrameReadyCallback = NULL;
	m_pFrameReadyUserData = NULL;

	m_nPackageTotal = 0;
	m_nExtendNum = 0;
	m_nSeqNum = 0;
	m_nLastTimeStamp = 0;
	m_nArriveTime = 0;
	m_nLastArriveTime = 0;
	m_nJitter = 0;

	m_nRtpTimeStamp = 0;
	m_nRtpPackageNum = 0;
	m_nRtpPackageByte = 0;
}

CRtpSession::~CRtpSession( )
{
	m_funFrameReadyCallback = NULL;
	m_pFrameReadyUserData = NULL;
}

int CRtpSession::InitRtpSession( RTP_SESSION_INFO& tRtpSessionInfo )
{
	m_tRtpSessionInfo = tRtpSessionInfo;
	//init ssrc 
	srand((unsigned)time(NULL));
	m_nSSRC = rand( );
	m_nSSRC = htonl( m_nSSRC );

	//get cPayload name
	int nRet = RTP_SUCCESS;
	nRet = CreateRtpPayLoad( tRtpSessionInfo );
	if( RTP_SUCCESS != nRet )
	{
		return RTP_FAILURE;
	}
	
	//init rtpex header
	memset( RtpHeaderExName, 0, sizeof(RtpHeaderExName) );
	strncpy( RtpHeaderExName, RTPHEADER_EX_COMPANY, strlen(RTPHEADER_EX_COMPANY) );
	RtpHeaderExVersion = RTPHEADER_EX_VERSION;

	return RTP_SUCCESS;
}

int CRtpSession::DeinitRtpSession( )
{
	if( m_pRtpPayload != NULL )
	{
		m_pRtpPayload->Deinit( );
		delete m_pRtpPayload;
		m_pRtpPayload = NULL;
	}
	return RTP_SUCCESS;
}

int CRtpSession::SetFrameReadyCallBack( RtpOneFrameReady funFrameReadyCallback , void *pUserData )
{
	m_funFrameReadyCallback = funFrameReadyCallback ;
	m_pFrameReadyUserData = pUserData ;
	return RTP_SUCCESS;
}

int CRtpSession::ParsePackage( char* pRtpBuf, int nLen, RTP_STREAM_INFO &tRtpStreamInfo )
{
	if( ( NULL == pRtpBuf ) || ( nLen <= 0 ) || (tRtpStreamInfo.m_pFrameBuffer == NULL) )
	{
		log_warn("ParsePackage error pRtpBuf = %p , nLen = %d, tRtpStreamInfo.m_pFrameBuffer = %p", pRtpBuf, nLen, tRtpStreamInfo.m_pFrameBuffer );
		return RTP_FAILURE;
	}

	if( NULL == m_pRtpPayload )
	{
		log_warn("ParsePackage error m_pRtpPayload = %p", m_pRtpPayload );
		return RTP_FAILURE;
	}

	int nRet = RTP_FAILURE;
	int nRtpHeaderLen = 0;
	//parse rtp header
	unsigned int nTimeStamp = 0;
	unsigned short SeqNum = 0;
	
	int nRtpExHeaderMark = (pRtpBuf[0] & 0x10) >> 4;
	int nCsrcCount = pRtpBuf[0] & 0x0f;
	int nMark = ( pRtpBuf[1] & 0x80 ) >> 7;
	
	memcpy( &SeqNum, pRtpBuf + 2, sizeof(unsigned short) );
	memcpy( &nTimeStamp, pRtpBuf + 4, sizeof(unsigned int) );
	SeqNum = ntohs( SeqNum );
	nTimeStamp = ntohl( nTimeStamp );
	tRtpStreamInfo.m_nTimeStamp = nTimeStamp;
	nRtpHeaderLen += 12;
	//rtcp dispose
	RtpRecvDispose( SeqNum, nTimeStamp );
	
	//parse CSRC 
	if( nCsrcCount > 0 )
	{
		nRtpHeaderLen += ( nCsrcCount * 4 );
	}

	//parse rtpex header
	if( nRtpExHeaderMark == 1 )
	{
		unsigned short nRtpExHeaderLen = 0;
		memcpy( &nRtpExHeaderLen, pRtpBuf + nRtpHeaderLen + 2, sizeof(unsigned short));
		nRtpExHeaderLen = ntohs( nRtpExHeaderLen );
		if( nRtpExHeaderLen > 0 )
		{
			ParseRtpExHeader( (pRtpBuf + nRtpHeaderLen + 4), (nRtpExHeaderLen * 4), tRtpStreamInfo.m_PrivateTimeStamp );
		}

		nRtpHeaderLen += ((1 + nRtpExHeaderLen) * 4);
	}
	
	//parse rtp body
	char *pRtpStream = pRtpBuf + nRtpHeaderLen;
	int nRtpStreamLen = nLen - nRtpHeaderLen;
	
	nRet = m_pRtpPayload->RtpUnpack( (unsigned char*)pRtpStream, nRtpStreamLen, tRtpStreamInfo );
	//callback 
	if( RTP_FRAME_OK == nRet )
	{
		//RTP_LOGPRI("RTP_FRAME_OK, frame len = %d, frame key = %d ", tRtpStreamInfo.m_nDataLen, tRtpStreamInfo.m_bKeyFrame );
		m_funFrameReadyCallback( tRtpStreamInfo, m_pFrameReadyUserData );
		tRtpStreamInfo.m_nDataLen = 0;
	}
	return RTP_SUCCESS;
}
int CRtpSession::CreatePackage( char* pRtpBuf, int *pnLen, RTP_STREAM_INFO &tRtpStreamInfo, RTP_EXTENSION_INFO &RtpExtensionInfo )
{
	if( (pRtpBuf == NULL) || (pnLen == NULL) || (tRtpStreamInfo.m_pFrameBuffer == NULL) )
	{
		return RTP_FAILURE;
	}
	//make rtp header
	int nRtpHeaderLen = 0;
	pRtpBuf[0]  = 0;
	pRtpBuf[0] |= 2 << 6; // v
	pRtpBuf[0] |= 0 << 5; // p
	if( RtpExtensionInfo.m_nEnableRtpExtension == 1 )
	{
		pRtpBuf[0] |= 1 << 4; // x
	}
	else
	{
		pRtpBuf[0] |= 0 << 4; // x
	}

	pRtpBuf[0] |= 0;      //cc

	pRtpBuf[1]  = 0;
	if( (tRtpStreamInfo.m_nStreamFlag >> 1 ) == 1 )
	{
		pRtpBuf[1] = 1 << 7; //m
	}
	else
	{
		pRtpBuf[1] = 0 << 7; //m
	}
	
	//pRtpBuf[1] |= m_cPayload;
	pRtpBuf[1] |= 96;
	unsigned short sCseq = htons( m_nRtpCseq );
	memcpy( &pRtpBuf[2] , &sCseq , sizeof( unsigned short ) );
	m_nRtpCseq ++;
	
	unsigned int nTimeStamp = htonl( tRtpStreamInfo.m_nTimeStamp );
	memcpy( &pRtpBuf[4] , &nTimeStamp , sizeof( int ) );

	int nSSRC = htonl( m_nSSRC );
	memcpy( &pRtpBuf[8] , &nSSRC , sizeof( int ) );
	nRtpHeaderLen = 12;

	//make rtp ex header
	if( RtpExtensionInfo.m_nEnableRtpExtension == 1 )
	{
		unsigned short sExHeader = 0;
		memcpy( pRtpBuf + 12, &sExHeader , sizeof( unsigned short ) );
		unsigned short sExLen = htons(RTP_EXTENSION_INFO_LEN);
		memcpy( pRtpBuf + 14, &sExLen , sizeof( unsigned short ) );

		memcpy( pRtpBuf + 16,RtpHeaderExName , 8 );
		
		pRtpBuf[24] = 0x24;
		pRtpBuf[25] = RtpHeaderExVersion;
		pRtpBuf[26] = 0;
		pRtpBuf[27] = 0;

		unsigned int nDateInfo = htonl( RtpExtensionInfo.m_nDateInfo );
		memcpy( pRtpBuf + 28, &nDateInfo , sizeof( int ) );
		unsigned int nTimeInfo = htonl( RtpExtensionInfo.m_nTimeInfo );
		memcpy( pRtpBuf + 32, &nTimeInfo , sizeof( int ) );
		nRtpHeaderLen = 36;
	}

	int nRet = 0;
	int nRtpBodyLen = 0;
	
	nRet = m_pRtpPayload->RtpPacket( (unsigned char*)pRtpBuf + nRtpHeaderLen, nRtpBodyLen, tRtpStreamInfo );
	if( nRet != RTP_SUCCESS )
	{
		return RTP_FAILURE;
	}
	

	*pnLen = (nRtpBodyLen + nRtpHeaderLen);

	RtpSendDispose( tRtpStreamInfo.m_nTimeStamp, *pnLen );

	return RTP_SUCCESS;
}

unsigned short CRtpSession::ParsePackageNum( char* pRtpBuf )
{
	unsigned short SeqNum = 0;
	memcpy( &SeqNum, pRtpBuf + 2, sizeof(unsigned short) );
	SeqNum = ntohs( SeqNum );
	return SeqNum;
}

int CRtpSession::RtpRecvDispose( unsigned short SeqNum, unsigned int nTimeStamp )
{
	if( m_nSeqNum > SeqNum )
	{
		m_nExtendNum += 1;
	}
	m_nSeqNum = SeqNum;
	m_nPackageTotal++;

	unsigned int nArriveTime = 0;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	nArriveTime = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	if( ( m_nLastTimeStamp > 0 ) && ( m_nLastArriveTime > 0 ) )
	{
		m_nJitter += (std::abs((int)(nArriveTime - m_nLastArriveTime)) - m_nJitter ) / 16;
	}
	m_nLastArriveTime = nArriveTime;
	m_nLastTimeStamp = nTimeStamp;
	return RTP_SUCCESS ;
}

int CRtpSession::RtpSendDispose( unsigned int nTimeStamp , int nRtpDataLen )
{
	m_nRtpTimeStamp = nTimeStamp;
	m_nRtpPackageNum ++;
	m_nRtpPackageByte += nRtpDataLen;
	return RTP_SUCCESS;
}

int CRtpSession::CreateRtpPayLoad( RTP_SESSION_INFO& tRtpSessionInfo )
{
	if( m_pRtpPayload != NULL )
	{
		delete m_pRtpPayload;
		m_pRtpPayload = NULL;
	}
	
	std::string strRtpPayloadName;
	switch( tRtpSessionInfo.m_cPayload )
	{
		case PAYLOAD_FORMAT_H264:
		{
			strRtpPayloadName = "CRtpPayloadH264"; //H264
			m_pRtpPayload = (CRtpPayload *)CRtpPayloadFactory::GetInstance( )->GreateObject( strRtpPayloadName );
			if( NULL == m_pRtpPayload )
			{
				return RTP_FAILURE;
			}
			break;
		}
		case PAYLOAD_FORMAT_SVAC:
		{
			strRtpPayloadName = "CRtpPayloadSvac";  //SVAC
			m_pRtpPayload = (CRtpPayload *)CRtpPayloadFactory::GetInstance( )->GreateObject( strRtpPayloadName );
			if( NULL == m_pRtpPayload )
			{
				return RTP_FAILURE;
			}
			break;
		}
		case PAYLOAD_FORMAT_H265:
		{
			strRtpPayloadName = "CRtpPayloadH265"; //H265
			m_pRtpPayload = (CRtpPayload *)CRtpPayloadFactory::GetInstance( )->GreateObject( strRtpPayloadName );
			if( NULL == m_pRtpPayload )
			{
				return RTP_FAILURE;
			}
			break;
		}
		case PAYLOAD_FORMAT_PCMA:
		{
			strRtpPayloadName = "CRtpPayloadPcm";//PCMA
			m_pRtpPayload = (CRtpPayload *)CRtpPayloadFactory::GetInstance( )->GreateObject( strRtpPayloadName );
			if( NULL == m_pRtpPayload )
			{
				return RTP_FAILURE;
			}
			break;
		}
		case PAYLOAD_FORMAT_PCMU:
		{
			strRtpPayloadName = "CRtpPayloadPcm";//PCMU
			m_pRtpPayload = (CRtpPayload *)CRtpPayloadFactory::GetInstance( )->GreateObject( strRtpPayloadName );
			if( NULL == m_pRtpPayload )
			{
				return RTP_FAILURE;
			}
			break;
		}
		case PAYLOAD_FORMAT_S16_LE_8000:
		{
			strRtpPayloadName = "CRtpPayloadS16LE"; //S16LE
			m_pRtpPayload = (CRtpPayload *)CRtpPayloadFactory::GetInstance( )->GreateObject( strRtpPayloadName );
			if( NULL == m_pRtpPayload )
			{
				return RTP_FAILURE;
			}
			break;
		}
		case PAYLOAD_FORMAT_S16_LE_16000:
		{
			strRtpPayloadName = "CRtpPayloadS16LE"; //S16LE
			m_pRtpPayload = (CRtpPayload *)CRtpPayloadFactory::GetInstance( )->GreateObject( strRtpPayloadName );
			if( NULL == m_pRtpPayload )
			{
				return RTP_FAILURE;
			}
			break;
		}
		case PAYLOAD_FORMAT_PS:
		{
			strRtpPayloadName = "CRtpPayloadPs"; //PS
			m_pRtpPayload = (CRtpPayload *)CRtpPayloadFactory::GetInstance( )->GreateObject( strRtpPayloadName );
			if( NULL == m_pRtpPayload )
			{
				return RTP_FAILURE;
			}
			break;
		}
		default:
		{
			return RTP_FAILURE;
		}
	}
	m_pRtpPayload->Init( tRtpSessionInfo );

	return RTP_SUCCESS;
}

int CRtpSession::ParseRtpExHeader( char* pBuf , int nRtpExHeaderLen, FRAME_TIME_STAMP &tTimeStamp )
{
	if( nRtpExHeaderLen < 5 )
	{
		return RTP_FAILURE;
	}
	
	int nMark = pBuf[8];
	int nVer = pBuf[9];

	if( nMark == 0x24 )
	{
		if( nVer == 1 )
		{
			unsigned int nDateInfo;
			unsigned int nTimeInfo;

			memcpy( &nDateInfo, pBuf + 12, sizeof(unsigned int));
			memcpy( &nTimeInfo, pBuf + 16, sizeof(unsigned int));

			nDateInfo = ntohl( nDateInfo );
			nTimeInfo = ntohl( nTimeInfo );
			
			tTimeStamp.m_nYear = (nDateInfo & 0xffff0000) >> 16;
			tTimeStamp.m_nMonth = (nDateInfo & 0xff00) >> 8;
			tTimeStamp.m_nDay = nDateInfo & 0xff ;
			tTimeStamp.m_nTimeStamp = nTimeInfo;

			//SD_LOGWRN(" TimeStamp.m_nYear = %d, TimeStamp.m_nMonth = %d, TimeStamp.m_nDay = %d,TimeStamp.m_nTimeStamp = %d", 
			//TimeStamp.m_nYear, TimeStamp.m_nMonth, TimeStamp.m_nDay, TimeStamp.m_nTimeStamp);
			tTimeStamp.m_bUsable = true;
		}
		else
		{
			return RTP_FAILURE;
		}
	}
	return RTP_SUCCESS;
}

