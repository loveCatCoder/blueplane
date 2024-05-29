/*
 * =====================================================================================
 *
 *	Filename:  RtpPayload.cpp
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
#include "RtpPayload.h"

CRtpPayload::CRtpPayload( )
{
	
}

CRtpPayload::~CRtpPayload( )
{
	
}

int CRtpPayload::Init( RTP_SESSION_INFO& tRtpSessionInfo )
{
	m_tRtpSessionInfo = tRtpSessionInfo;
	return RTP_SUCCESS;
}

int CRtpPayload::Deinit( )
{
	return RTP_SUCCESS;
}

