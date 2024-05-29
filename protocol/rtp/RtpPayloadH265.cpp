/*
 * =====================================================================================
 *
 *	Filename:  RtpPayloadH265.cpp
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
#include "RtpPayloadH265.h"

CRtpPayloadH265::CRtpPayloadH265( )
{
	m_nMediaType = RTP_VIDEO_FRAME;
}

CRtpPayloadH265::~CRtpPayloadH265( )
{
	
}

int CRtpPayloadH265::RtpUnpack( unsigned char* pRtpStream, int nLen, RTP_STREAM_INFO & tRtpInfo )
{
	if( ( pRtpStream == NULL ) || ( nLen <= 0 ) )
	{
		RTP_LOGWRN("h265 parse parameter error , nLen = %d", nLen);
		return RTP_FAILURE;
	}

	tRtpInfo.m_nMediaType = m_nMediaType;

	unsigned char ucPh1 = pRtpStream[0];
	unsigned char ucPh2 = pRtpStream[1];
	unsigned char ucPh3 = pRtpStream[2];
	int nSkip = 0;
	int nRet = RTP_SUCCESS;

	if( ((ucPh1 & 0x7f) >> 1) == 49 )
	{
		nSkip = 3;
		if( ((ucPh3 & 0x80) >> 7) == 1 )  //start
		{
			tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen] = 0x00;
			tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 1] = 0x00;
			tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 2] = 0x00;
			tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 3] = 0x01;

			
			tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 4] = (ucPh1 & 0x81)|((ucPh3 & 0x3f)<<1);
			tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 5] = ucPh2;
			//RTP_LOGPRI(" h265 head = 0x%2x 0x%2x ",tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 4],tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 5]);
			tRtpInfo.m_nDataLen += 6;
			
		}

		memcpy( tRtpInfo.m_pFrameBuffer + tRtpInfo.m_nDataLen , pRtpStream + nSkip , nLen - nSkip );
		tRtpInfo.m_nDataLen += nLen - nSkip;
		
		if( ((ucPh3 & 0x40) >> 6) == 1 )  //end
		{
			nRet = RTP_FRAME_OK;
			if( (ucPh3 & 0x3f) != 1 )
			{
				tRtpInfo.m_bKeyFrame = true;
				//RTP_LOGERR("I frame");
			}
			else
			{
				tRtpInfo.m_bKeyFrame = false;
				//RTP_LOGERR("p frame");
			}

		}
	}
	else if( ( ((ucPh1 & 0x7f) >> 1) == 32 ) || ( ((ucPh1 & 0x7f) >> 1) == 33 ) || ( ((ucPh1 & 0x7f) >> 1) == 34 ) )
	{
		//RTP_LOGPRI("frame head = 0x%x 0x%x", ucPh1, ucPh2);
		nSkip = 0;
		tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen] = 0x00;
		tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 1] = 0x00;
		tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 2] = 0x00;
		tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 3] = 0x01;
		tRtpInfo.m_nDataLen += 4;

		memcpy( tRtpInfo.m_pFrameBuffer + tRtpInfo.m_nDataLen, pRtpStream + nSkip , nLen - nSkip );
		tRtpInfo.m_nDataLen += nLen;
	}
	else if( ((ucPh1 & 0x7f) >> 1) == 1 )
	{
		//RTP_LOGPRI("frame head = 0x%2x 0x%2x", ucPh1, ucPh2);
		nSkip = 0;
		tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen] = 0x00;
		tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 1] = 0x00;
		tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 2] = 0x00;
		tRtpInfo.m_pFrameBuffer[tRtpInfo.m_nDataLen + 3] = 0x01;
		tRtpInfo.m_nDataLen += 4;

		memcpy( tRtpInfo.m_pFrameBuffer + tRtpInfo.m_nDataLen, pRtpStream + nSkip , nLen - nSkip );
		tRtpInfo.m_nDataLen += nLen;
		//p frame
		tRtpInfo.m_bKeyFrame = false;
		nRet = RTP_FRAME_OK;
	}
	else
	{
		RTP_LOGWRN( " this h265 stream head = %x ( can't parse ), nLen = %d", ucPh1, nLen );
	}
	
	return nRet;
}

int CRtpPayloadH265::RtpPacket( unsigned char* pRtpStream, int &nLen, RTP_STREAM_INFO & tRtpInfo )
{
	return RTP_SUCCESS;
}

