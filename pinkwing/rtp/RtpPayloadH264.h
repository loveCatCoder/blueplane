/*
 * =====================================================================================
 *
 *	Filename:  RtpPayloadH264.h
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
#ifndef __RTP_PAYLOAD_H264_H__
#define __RTP_PAYLOAD_H264_H__

#include "RtpPrivate.h"
#include "RtpPayload.h"
#include "RtpPayloadFactory.h"

class CRtpPayloadH264: public CRtpPayload
{
public:
	CRtpPayloadH264( );
	virtual ~CRtpPayloadH264( );	
public:
	virtual int RtpUnpack( unsigned char* pRtpStream, int nLen, RTP_STREAM_INFO & tRtpInfo );
	virtual int RtpPacket( unsigned char* pRtpStream, int &nLen, RTP_STREAM_INFO & tRtpInfo );
private:
	int m_nMediaType;
	char m_H264Header[2];
};


RTP_PAYLOAD_REGISTER( CRtpPayloadH264 );

#endif
