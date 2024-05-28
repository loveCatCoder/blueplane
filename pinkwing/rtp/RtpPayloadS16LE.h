/*
 * =====================================================================================
 *
 *	Filename:  RtpPayloadS16LE.h
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
#ifndef __RTP_PAYLOAD_S16LE_H__
#define __RTP_PAYLOAD_S16LE_H__

#include "RtpPrivate.h"
#include "RtpPayload.h"
#include "RtpPayloadFactory.h"

class CRtpPayloadS16LE: public CRtpPayload
{
public:
	CRtpPayloadS16LE( );
	virtual ~CRtpPayloadS16LE( );	
public:
	virtual int RtpUnpack( unsigned char* pRtpStream, int nLen, RTP_STREAM_INFO & tRtpInfo );
	virtual int RtpPacket( unsigned char* pRtpStream, int &nLen, RTP_STREAM_INFO & tRtpInfo );
private:
	int m_nMediaType;
};


RTP_PAYLOAD_REGISTER( CRtpPayloadS16LE );

#endif
