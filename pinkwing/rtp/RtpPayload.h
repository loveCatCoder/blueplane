/*
 * =====================================================================================
 *
 *	Filename:  RtpPayload.h
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
#ifndef __RTP_PAYLOAD_H__
#define __RTP_PAYLOAD_H__

#include "RtpPrivate.h"

class CRtpPayload
{
public:
	CRtpPayload( );
	virtual ~CRtpPayload( );	
public:
	virtual int Init( RTP_SESSION_INFO& tRtpSessionInfo );
	virtual int Deinit( );
	virtual int RtpUnpack( unsigned char* pRtpStream, int nLen, RTP_STREAM_INFO & tRtpInfo ) = 0;
	virtual int RtpPacket( unsigned char* pRtpStream, int &nLen, RTP_STREAM_INFO & tRtpInfo ) = 0;
public:
	RTP_SESSION_INFO m_tRtpSessionInfo;
};

#endif
