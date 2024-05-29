/*
 * =====================================================================================
 *
 *	Filename:  RtpPayloadPcm.h
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
#ifndef __RTP_PAYLOAD_PCM_H__
#define __RTP_PAYLOAD_PCM_H__

#include "RtpPrivate.h"
#include "RtpPayload.h"
#include "RtpPayloadFactory.h"

class CRtpPayloadPcm: public CRtpPayload
{
public:
	CRtpPayloadPcm( );
	virtual ~CRtpPayloadPcm( );	
public:
	virtual int RtpUnpack( unsigned char* pRtpStream, int nLen, RTP_STREAM_INFO & tRtpInfo );
	virtual int RtpPacket( unsigned char* pRtpStream, int &nLen, RTP_STREAM_INFO & tRtpInfo );
private:
	int m_nMediaType;
};


RTP_PAYLOAD_REGISTER( CRtpPayloadPcm );

#endif
