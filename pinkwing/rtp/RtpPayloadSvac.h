/*
 * =====================================================================================
 *
 *	Filename:  RtpPayloadSvac.h
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
#ifndef __RTP_PAYLOAD_SVAC_H__
#define __RTP_PAYLOAD_SVAC_H__

#include "RtpPrivate.h"
#include "RtpPayload.h"
#include "RtpPayloadFactory.h"

class CRtpPayloadSvac: public CRtpPayload
{
public:
	CRtpPayloadSvac( );
	virtual ~CRtpPayloadSvac( );	
public:
	virtual int RtpUnpack( unsigned char* pRtpStream, int nLen, RTP_STREAM_INFO & tRtpInfo );
	virtual int RtpPacket( unsigned char* pRtpStream, int &nLen, RTP_STREAM_INFO & tRtpInfo );
private:
	
};


RTP_PAYLOAD_REGISTER( CRtpPayloadSvac );

#endif

