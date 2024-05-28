/*
 * =====================================================================================
 *
 *	Filename:  RtpPayloadPs.h
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
#ifndef __RTP_PAYLOAD_PS_H__
#define __RTP_PAYLOAD_PS_H__

#include "RtpPrivate.h"
#include "RtpPayload.h"
#include "RtpPayloadFactory.h"

class CRtpPayloadPs: public CRtpPayload
{
public:
	CRtpPayloadPs( );
	virtual ~CRtpPayloadPs( );	
public:
	virtual int Init( RTP_SESSION_INFO& tRtpSessionInfo );
	virtual int Deinit( );
	virtual int RtpUnpack( unsigned char* pRtpStream, int nLen, RTP_STREAM_INFO & tRtpInfo );
	virtual int RtpPacket( unsigned char* pRtpStream, int &nLen, RTP_STREAM_INFO & tRtpInfo );
private:
	int CheckFrameOk( unsigned char* pStream, int nStreamLen );
	int ParsePsHeader( unsigned char* pStream, int nStreamLen );
	int ParsePsHeaderBA( unsigned char* pStream );
	int ParsePsHeaderBB( unsigned char* pStream );
	int ParsePsHeaderBC( unsigned char* pStream );
	int ParsePesData( unsigned char* pStream, int nLen );
	int ParsePesDateLen( unsigned char* pStream );
	int ParsePesHeader( unsigned char* pStream );
	
private:
	unsigned char* m_pStream;
	int m_nStreamLen;
	int m_nCurrentPesLen;
	
	bool m_nPsKeyFrame;
	int m_nPsFrameType;
};


RTP_PAYLOAD_REGISTER( CRtpPayloadPs );

#endif
