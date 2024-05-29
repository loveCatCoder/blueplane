/*
 * =====================================================================================
 *
 *	Filename:  RtpPayloadH265.h
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
#ifndef __RTP_PAYLOAD_H265_H__
#define __RTP_PAYLOAD_H265_H__

#include "RtpPrivate.h"
#include "RtpPayload.h"
#include "RtpPayloadFactory.h"

class CRtpPayloadH265: public CRtpPayload
{
public:
	CRtpPayloadH265( );
	virtual ~CRtpPayloadH265( );	
public:
	virtual int RtpUnpack( unsigned char* pRtpStream, int nLen, RTP_STREAM_INFO & tRtpInfo );
	virtual int RtpPacket( unsigned char* pRtpStream, int &nLen, RTP_STREAM_INFO & tRtpInfo );
private:
	int m_nMediaType;
};


RTP_PAYLOAD_REGISTER( CRtpPayloadH265 );

#endif
