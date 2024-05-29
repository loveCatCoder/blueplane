/*
 * =====================================================================================
 *
 *	Filename:  RtpPayloadPcm.cpp
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
#include "RtpPayloadPcm.h"

CRtpPayloadPcm::CRtpPayloadPcm( )
{
	m_nMediaType = RTP_AUDIO_FRAME;
}

CRtpPayloadPcm::~CRtpPayloadPcm( )
{
	
}

int CRtpPayloadPcm::RtpUnpack( unsigned char* pRtpStream, int nLen, RTP_STREAM_INFO & tRtpInfo )
{
	if( (pRtpStream == NULL) || (nLen <= 0) )
    {
        RTP_LOGERR("PCMA parse parameter error ");
        return RTP_FAILURE;
    }

	tRtpInfo.m_nMediaType = m_nMediaType;
	
    memcpy( tRtpInfo.m_pFrameBuffer + tRtpInfo.m_nDataLen, pRtpStream, nLen );
    tRtpInfo.m_nDataLen += nLen;

    return RTP_FRAME_OK;
}

int CRtpPayloadPcm::RtpPacket( unsigned char* pRtpStream, int &nLen, RTP_STREAM_INFO & tRtpInfo )
{
	return RTP_SUCCESS;
}
