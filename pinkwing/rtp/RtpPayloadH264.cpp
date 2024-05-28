/*
 * =====================================================================================
 *
 *	Filename:  RtpPayloadH264.cpp
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
#include "RtpPayloadH264.h"

CRtpPayloadH264::CRtpPayloadH264( )
{
	m_nMediaType = RTP_VIDEO_FRAME;
}

CRtpPayloadH264::~CRtpPayloadH264( )
{
	
}


int CRtpPayloadH264::RtpUnpack( unsigned char * pRtpStream, int nLen, RTP_STREAM_INFO & tRtpInfo )
{
	if( ( NULL == pRtpStream ) || ( nLen <= 0 ) )
	{
		RTP_LOGWRN("h264 parse parameter error , nLen = %d", nLen);
		return RTP_FAILURE;
	}

	tRtpInfo.m_nMediaType = m_nMediaType;

	unsigned char ucPh1 = pRtpStream[0];
	unsigned char ucPh2 = pRtpStream[1];
	int nSkip = 0;
	int nRet = RTP_SUCCESS;

	if( (ucPh1 & 0x1f) == 28 )
	{
		nSkip = 2;
		if( ((ucPh2 & 0x80) >> 7) == 1 )  //start
	    {
	        tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen] = 0x00;
	        tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 1] = 0x00;
	        tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 2] = 0x00;
	        tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 3] = 0x01;
	        tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 4] = (ucPh1 & 0xe0)|(ucPh2 & 0x1f) ;
	        tRtpInfo.m_nDataLen += 5;
	    }
		if( tRtpInfo.m_nDataLen < (RTP_VIDEO_FRAME_MAX_LEN - 4096) )
		{
			memcpy( tRtpInfo.m_pFrameBuffer + tRtpInfo.m_nDataLen, pRtpStream + nSkip, nLen - nSkip );
			tRtpInfo.m_nDataLen += nLen - nSkip;
		}
		
	    if( ((ucPh2 & 0x40) >> 6) == 1 )  //end
	    {
	        nRet = RTP_FRAME_OK;
	        if( (pRtpStream[1] & 0x1f) != 1 )
			{
				tRtpInfo.m_bKeyFrame = true;
			}
			else
			{
				tRtpInfo.m_bKeyFrame = false;
			}
	    }
	}
	else if( ( (ucPh1 & 0x1f) > 5 ) && ( (ucPh1 & 0x1f) <= 8 ) )
	{
		nSkip = 0;
	    tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen] = 0x00;
	    tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 1] = 0x00;
	    tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 2] = 0x00;
	    tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 3] = 0x01;
	    tRtpInfo.m_nDataLen += 4;

		if( tRtpInfo.m_nDataLen < (RTP_VIDEO_FRAME_MAX_LEN - 4096) )
		{
	    	memcpy( tRtpInfo.m_pFrameBuffer + tRtpInfo.m_nDataLen, pRtpStream + nSkip, nLen - nSkip );
	    	tRtpInfo.m_nDataLen += nLen;
		}
	}
	else if( ( (ucPh1 & 0x1f) == 1) || ( (ucPh1 & 0x1f) == 5 ) )
	{
		nSkip = 0;
	    tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen] = 0x00;
	    tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 1] = 0x00;
	    tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 2] = 0x00;
	    tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 3] = 0x01;
	    tRtpInfo.m_nDataLen += 4;

	    memcpy( tRtpInfo.m_pFrameBuffer + tRtpInfo.m_nDataLen, pRtpStream + nSkip, nLen - nSkip );
	    tRtpInfo.m_nDataLen += nLen;
		//p frame
	    tRtpInfo.m_bKeyFrame = false;
	    nRet = RTP_FRAME_OK;
	}
	else if( (ucPh1 & 0x1f) == 9 )
	{
		//not dispose
	}
	else
	{
		RTP_LOGWRN( " this h264 stream head = %x ( can't parse ), nLen = %d", ucPh1, nLen );
	}
	
	return nRet;
}

int CRtpPayloadH264::RtpPacket( unsigned char* pRtpStream, int &nLen, RTP_STREAM_INFO & tRtpInfo )
{
	if( ( NULL == pRtpStream ) || (tRtpInfo.m_pFrameBuffer == NULL) || ( tRtpInfo.m_nDataLen <= 0 ))
	{
		RTP_LOGWRN( "h264 RtpPacket paramter error pRtpStream = %p, tRtpInfo.m_pFrameBuffer = %p, tRtpInfo.m_nDataLen = %d", pRtpStream, tRtpInfo.m_pFrameBuffer, tRtpInfo.m_nDataLen );
		return RTP_FAILURE;
	}
	//RTP_LOGPRI( "this h264 RtpPacket len = %d", tRtpInfo.m_nDataLen );

	if( 1 == tRtpInfo.m_nStreamFlag )
	{
		//start
		m_H264Header[0] = (tRtpInfo.m_pFrameBuffer[4] & 0xE0) + 28;
		m_H264Header[1] = (1 << 7) | ( tRtpInfo.m_pFrameBuffer[4] & 0x1f );
		pRtpStream[0] = m_H264Header[0];
		pRtpStream[1] = m_H264Header[1];

		memcpy( (pRtpStream + 2), (tRtpInfo.m_pFrameBuffer + 5), (tRtpInfo.m_nDataLen - 5) );

		nLen = tRtpInfo.m_nDataLen + 2 - 5;
	}
	else if( 2 == tRtpInfo.m_nStreamFlag )
	{
		//end
		m_H264Header[1] &= 0x1f;
		m_H264Header[1] |= 1 << 6;
		pRtpStream[0] = m_H264Header[0];
		pRtpStream[1] = m_H264Header[1];

		memcpy( (pRtpStream + 2), tRtpInfo.m_pFrameBuffer, tRtpInfo.m_nDataLen );

		nLen = tRtpInfo.m_nDataLen + 2;
	}
	else if( 0 == tRtpInfo.m_nStreamFlag )
	{
		//body
		m_H264Header[1] &= 0x1f;
		pRtpStream[0] = m_H264Header[0];
		pRtpStream[1] = m_H264Header[1];
		
		memcpy( (pRtpStream + 2), tRtpInfo.m_pFrameBuffer, tRtpInfo.m_nDataLen );
		nLen = tRtpInfo.m_nDataLen + 2;
	}
	else if( 3 == tRtpInfo.m_nStreamFlag )
	{
		memcpy( pRtpStream, (tRtpInfo.m_pFrameBuffer + 4), (tRtpInfo.m_nDataLen - 4) );
		nLen = tRtpInfo.m_nDataLen -4;
	}
	else
	{
		return RTP_FAILURE;
	}

	return RTP_SUCCESS;
}
