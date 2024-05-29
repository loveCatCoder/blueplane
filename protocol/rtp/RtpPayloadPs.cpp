/*
 * =====================================================================================
 *
 *	Filename:  RtpPayloadPs.h
 *
 *	Description: 			
 *	Version:  1.0
 *	Created: 2020-04-14
 *
 *	Author:  wty
 *	Organization:  ICHINAE(BEIJING) TECH.CO.,LTD. 
 *
 * =====================================================================================
*/
#include "RtpPayloadPs.h"

CRtpPayloadPs::CRtpPayloadPs( )
{
	m_pStream = NULL;
	m_nStreamLen = 0;
	
	m_nCurrentPesLen = 0;
	m_nPsKeyFrame = false;
	m_nPsFrameType = VIDEO_DATA;
}

CRtpPayloadPs::~CRtpPayloadPs( )
{
	if( m_pStream != NULL )
	{
		free( m_pStream );
		m_pStream = NULL;
	}
}
int CRtpPayloadPs::Init( RTP_SESSION_INFO& tRtpSessionInfo )
{
	m_pStream = (unsigned char*)malloc( RTP_VIDEO_FRAME_MAX_LEN );
	if( m_pStream == NULL )
	{
		RTP_LOGWRN( "ps decode create buffer error");
		return RTP_FAILURE;
	}
	return RTP_SUCCESS;
}

int CRtpPayloadPs::Deinit( )
{
	return RTP_SUCCESS;
}

int CRtpPayloadPs::RtpUnpack( unsigned char* pRtpStream, int nLen, RTP_STREAM_INFO & tRtpInfo )
{
	if( ( NULL == pRtpStream ) || ( nLen <= 0 ) )
	{
		RTP_LOGWRN("ps parse parameter error , nLen = %d", nLen);
		return RTP_FAILURE;
	}

	//RTP_LOGDBG("RtpUnpack pRtpStream nLen = %d", nLen);
	//check frame ok
	int nFrameRet = RTP_SUCCESS;
	nFrameRet = CheckFrameOk( pRtpStream, nLen );
	if( RTP_FRAME_OK == nFrameRet )
	{
		if( m_nStreamLen > 0 )
		{
			int nSkip = 0;
			if( (m_nPsFrameType == VIDEO_DATA) && (m_nStreamLen > 6) )
			{
				if( (m_pStream[4] & 0x1f) == 9 )
				{
					nSkip = 6;
				}
			}
		
			memcpy( tRtpInfo.m_pFrameBuffer, m_pStream + nSkip, m_nStreamLen - nSkip );
			tRtpInfo.m_nDataLen = (m_nStreamLen - nSkip);
			tRtpInfo.m_nMediaType = m_nPsFrameType;
			if( tRtpInfo.m_nMediaType == AUDIO_DATA )
			{
				tRtpInfo.m_bKeyFrame = 0;
			}
			else
			{
				tRtpInfo.m_bKeyFrame = m_nPsKeyFrame;
			}
			m_nStreamLen = 0;
		}
		else
		{
			nFrameRet = RTP_SUCCESS;
		}
	}
	
	//parse header
	unsigned char* pData = pRtpStream;
	int nDataLen = nLen;
	int nHeaderLen = 0;
	int nRet = RTP_SUCCESS;

	nHeaderLen = ParsePsHeader( pData, nDataLen );
	if( nHeaderLen > 0 )
	{
		nDataLen = nLen - nHeaderLen;
		if( nDataLen < 0 )
		{
			nRet = RTP_FAILURE;
		}
		else
		{
			m_nCurrentPesLen = 0;
			pData += nHeaderLen;
		}	
	}
	
	//parse pes
	if( nRet == RTP_SUCCESS )
	{
		//SD_LOGPRI( "parse pes nDataLen = %d, ", nDataLen );
		nRet = ParsePesData( pData, nDataLen );
	}
	
	if( nFrameRet == RTP_FRAME_OK )
	{
		return RTP_FRAME_OK;
	}

	return RTP_SUCCESS;
}

int CRtpPayloadPs::RtpPacket( unsigned char* pRtpStream, int &nLen, RTP_STREAM_INFO & tRtpInfo )
{
	return RTP_SUCCESS;
}

int CRtpPayloadPs::CheckFrameOk( unsigned char* pStream, int nStreamLen )
{
	if( (pStream == NULL) || (nStreamLen < 4) )
	{
		return RTP_FAILURE;
	}

	if( (pStream[0] == 0x00) && (pStream[1] == 0x00) && (pStream[2] == 0x01) && (pStream[3] == 0xba) )
	{
		return RTP_FRAME_OK;
	}
	else if( (pStream[0] == 0x00) && (pStream[1] == 0x00) && (pStream[2] == 0x01) && (pStream[3] == 0xc0) ) 
	{
		return RTP_FRAME_OK;
	}
	else
	{
		return RTP_FAILURE;
	}
	return RTP_SUCCESS;
}

int CRtpPayloadPs::ParsePsHeader( unsigned char* pStream, int nStreamLen )
{
	int nPsHeaderLen = 0;
	int nHeaderLen = 0;
	unsigned char* pData = pStream;
	
	if( (pData[0] == 0x00) && (pData[1] == 0x00) && (pData[2] == 0x01) && (pData[3] == 0xba) )
	{
		
		m_nPsKeyFrame = false;
		nHeaderLen = ParsePsHeaderBA( pData );
		//RTP_LOGDBG( "ParsePsHeader 0xba HeaderLen = %d, streamLen = %d", nHeaderLen, nStreamLen );
		//skip ba
		nPsHeaderLen += nHeaderLen;
		pData += nHeaderLen;
		if( nStreamLen == nPsHeaderLen )
		{
			return nPsHeaderLen;
		}
		
	}

	if( (pData[0] == 0x00) && (pData[1] == 0x00) && (pData[2] == 0x01) && (pData[3] == 0xbb) )
	{
		m_nPsKeyFrame = true;
		nHeaderLen = ParsePsHeaderBB( pData );
		//RTP_LOGDBG( "ParsePsHeader 0xbb HeaderLen = %d, streamLen = %d", nHeaderLen, nStreamLen );
		//skip bb
		nPsHeaderLen += nHeaderLen;
		pData += nHeaderLen;
		if( nStreamLen == nPsHeaderLen )
		{
			return nPsHeaderLen;
		}
	}
	
	if( (pData[0] == 0x00) && (pData[1] == 0x00) && (pData[2] == 0x01) && (pData[3] == 0xbc) )
	{
		nHeaderLen = ParsePsHeaderBC( pData );
		//RTP_LOGDBG( "ParsePsHeader 0xbc HeaderLen = %d, streamLen = %d", nHeaderLen, nStreamLen );
		//skip bc
		nPsHeaderLen += nHeaderLen;
		pData += nHeaderLen;
		if( nStreamLen == nPsHeaderLen )
		{
			return nPsHeaderLen;
		}
	}
	return nPsHeaderLen;
}

int CRtpPayloadPs::ParsePsHeaderBA( unsigned char* pStream )
{
	if( NULL == pStream )
	{
		return 0;
	}

	int nLen = 0;
	int nExtLen = 0;
	nExtLen = pStream[13] & 0x07;
	nLen = 14 + nExtLen;
	return nLen;
}

int CRtpPayloadPs::ParsePsHeaderBB( unsigned char* pStream )
{
	if( NULL == pStream )
	{
		return 0;
	}

	int nLen = 0;
	unsigned short DataLen = 0;

	memcpy( &DataLen, pStream + 4, 2 );
	DataLen = htons(DataLen);
	nLen = 6 + DataLen;
	return nLen;
}

int CRtpPayloadPs::ParsePsHeaderBC( unsigned char* pStream )
{
	if( NULL == pStream )
	{
		return 0;
	}

	int nLen = 0;
	unsigned short DataLen = 0;

	memcpy( &DataLen, pStream + 4, 2 );
	DataLen = htons(DataLen);
	nLen = 6 + DataLen;
	return nLen;
}

int CRtpPayloadPs::ParsePesData( unsigned char* pStream, int nLen )
{
	unsigned char* pData = pStream;
	int nDataLen = nLen;

	int nPesLen = 0;
	int nPesHeaderLen = 0;

	do{
		//parse header
		if( (pData[0] == 0x00) && (pData[1] == 0x00) && (pData[2] == 0x01) && (pData[3] == 0xe0) )
		{
			m_nPsFrameType = VIDEO_DATA;
			nPesLen = ParsePesDateLen( pData );
			nPesHeaderLen = ParsePesHeader( pData );
			m_nCurrentPesLen = nPesLen - nPesHeaderLen;
			//RTP_LOGDBG( "video pes len = %d", m_nCurrentPesLen );
			if( nPesHeaderLen > nDataLen )
			{
				RTP_LOGWRN( "error nPesHeaderLen = %d", nPesHeaderLen );
				m_nCurrentPesLen = 0;
				return RTP_FAILURE;
			}
			
			pData += nPesHeaderLen;
			nDataLen -= nPesHeaderLen;
		}
		else if( (pData[0] == 0x00) && (pData[1] == 0x00) && (pData[2] == 0x01) && (pData[3] == 0xc0) ) 
		{
			if( !m_tRtpSessionInfo.m_bSupportAudio )
			{
				break;
			}
			//g711
			m_nPsFrameType = AUDIO_DATA;
			nPesLen = ParsePesDateLen( pData );
			nPesHeaderLen = ParsePesHeader( pData );
			m_nCurrentPesLen = nPesLen - nPesHeaderLen;
			//RTP_LOGDBG( "audio pes len = %d", m_nCurrentPesLen );
			if( nPesHeaderLen > nDataLen )
			{
				RTP_LOGWRN( "error nPesHeaderLen = %d", nPesHeaderLen );
				m_nCurrentPesLen = 0;
				return RTP_FAILURE;
			}
			
			pData += nPesHeaderLen;
			nDataLen -= nPesHeaderLen;
		}
		else
		{
			//pes data
		}


		if( nDataLen <= m_nCurrentPesLen )
		{
			memcpy( m_pStream + m_nStreamLen , pData, nDataLen );
			m_nStreamLen += nDataLen;
			m_nCurrentPesLen -= nDataLen;
			nDataLen = 0;
		}
		else
		{
			if( m_nCurrentPesLen != 0 )
			{
				memcpy( m_pStream + m_nStreamLen, pData, m_nCurrentPesLen );
				m_nStreamLen += m_nCurrentPesLen;
				nDataLen -= m_nCurrentPesLen;
				pData += m_nCurrentPesLen;
				m_nCurrentPesLen = 0;
			}
			else
			{
				nDataLen = 0;
			}
		}

	}while( nDataLen > 0 );
	return RTP_SUCCESS;
}

int CRtpPayloadPs::ParsePesDateLen( unsigned char* pStream )
{
	if( NULL == pStream )
	{
		return 0;
	}

	int nLen = 0;
	unsigned short PesLen = 0;

	memcpy( &PesLen, pStream + 4, 2 );
	PesLen = htons(PesLen);
	nLen = 6 + PesLen;
	return nLen;
}

int CRtpPayloadPs::ParsePesHeader( unsigned char* pStream )
{
	if( NULL == pStream )
	{
		return 0;
	}
	int nLen = 0;
	int nDataLen = 0;

	nDataLen = pStream[8];

	nLen = 9 + nDataLen;
	return nLen;
}

