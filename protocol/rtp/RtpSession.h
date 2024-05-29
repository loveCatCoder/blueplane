/*
 * =====================================================================================
 *
 *	Filename:  RtpSession.h
 *
 *	Description: 			
 *	Version:  1.0
 *	Created: 2019-04-16
 *
 *	Author:  wty
 *	Organization:  ICHINAE(BEIJING) TECH.CO.,LTD. 
 *
 * =====================================================================================
*/
#ifndef __RTP_SESSION_H__
#define __RTP_SESSION_H__

#include "RtpPrivate.h"
#include "RtpPayload.h"

typedef int ( *RtpOneFrameReady )( RTP_STREAM_INFO &tRtpStreamInfo, void *pUserData );

#define RTP_PCMA	8
#define RTP_PCMU	0



class CRtpSession
{
public:
	CRtpSession( );
	virtual ~CRtpSession( );
public:
	int InitRtpSession( RTP_SESSION_INFO& tRtpSessionInfo );
	int DeinitRtpSession( );
	int SetFrameReadyCallBack( RtpOneFrameReady funFrameReadyCallback , void *pUserData );
	int ParsePackage( char* pRtpBuf, int nLen, RTP_STREAM_INFO &tRtpStreamInfo );
	int CreatePackage( char* pRtpBuf, int *pnLen, RTP_STREAM_INFO &tRtpStreamInfo, RTP_EXTENSION_INFO &RtpExtensionInfo );
	
	static unsigned short ParsePackageNum( char* pRtpBuf );

	int RtpRecvDispose( unsigned short SeqNum, unsigned int nTimeStamp );
	int RtpSendDispose( unsigned int nTimeStamp , int nRtpDataLen );

	unsigned int GetRtpPackage( ){ return m_nPackageTotal; };
	int GetExtendSeq( ){ return ((m_nExtendNum<<16) + m_nSeqNum); };
	int GetJitter( ){ return m_nJitter; };
	unsigned int GetRtpTimeStamp( ){ return m_nRtpTimeStamp; };
	unsigned int GetRtpSendPackageNum( ) { return m_nRtpPackageNum; };
	unsigned int GetRtpSendPackageByte( ) { return m_nRtpPackageByte; };
private:
	int CreateRtpPayLoad( RTP_SESSION_INFO& tRtpSessionInfo );
	int ParseRtpExHeader( char* pBuf , int nRtpExHeaderLen, FRAME_TIME_STAMP &tTimeStamp );
private:
	CRtpPayload * m_pRtpPayload;
	RTP_SESSION_INFO m_tRtpSessionInfo;
	int m_nSSRC;
	char RtpHeaderExVersion;
	char RtpHeaderExName[128];
	unsigned short m_nRtpCseq;

	//callback 
	RtpOneFrameReady m_funFrameReadyCallback ;
	void *m_pFrameReadyUserData ;

	//recv
	unsigned int m_nPackageTotal;
	unsigned int m_nExtendNum;
	unsigned int m_nSeqNum;
	unsigned int m_nLastTimeStamp;
	unsigned int m_nArriveTime;
	unsigned int m_nLastArriveTime;
	int m_nJitter;
	//send
	unsigned int m_nRtpTimeStamp;
	unsigned int m_nRtpPackageNum;
	unsigned int m_nRtpPackageByte;
};

#endif

