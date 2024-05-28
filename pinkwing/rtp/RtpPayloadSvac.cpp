/*
 * =====================================================================================
 *
 *	Filename:  RtpPayloadSvac.cpp
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
#include "RtpPayloadSvac.h"

CRtpPayloadSvac::CRtpPayloadSvac( )
{

}

CRtpPayloadSvac::~CRtpPayloadSvac( )
{
	
}

int CRtpPayloadSvac::RtpUnpack( unsigned char* pRtpStream, int nLen, RTP_STREAM_INFO & tRtpInfo )
{
	return RTP_SUCCESS;
}

int CRtpPayloadSvac::RtpPacket( unsigned char* pRtpStream, int &nLen, RTP_STREAM_INFO & tRtpInfo )
{
	return RTP_SUCCESS;
}

