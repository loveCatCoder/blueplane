/*
 * =====================================================================================
 *
 *	Filename:  RtpPrivate.h
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
#ifndef __RTP_PRIVATE_H__
#define __RTP_PRIVATE_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <error.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/time.h>

#include <map>

#include "include/private.h"



//return 
#define RTP_SUCCESS       0
#define RTP_FAILURE       -1
#define RTP_FRAME_OK      1

//media type
#define RTP_VIDEO_FRAME 1
#define RTP_AUDIO_FRAME 2

//logs
#define RTP_LOGDBG   log_debug
#define RTP_LOGPRI   log_info
#define RTP_LOGWRN   log_warn
#define RTP_LOGERR   log_error

#define RTP_EXTENSION_INFO_LEN 5

#define RTPHEADER_EX_COMPANY "IChinaE"
#define RTPHEADER_EX_VERSION 1

#define RTP_VIDEO_FRAME_MAX_LEN 2*1024*1024
#define RTP_AUDIO_FRAME_MAX_LEN 4*1024


struct FRAME_TIME_STAMP
{
public:
	FRAME_TIME_STAMP( )
	{
		m_nYear = 0;
		m_nMonth = 0;
		m_nDay = 0;
		m_nTimeStamp = 0;
		m_bUsable = false;
	}
	int          m_nYear;
	int          m_nMonth;
	int          m_nDay;
	unsigned int m_nTimeStamp;
	bool         m_bUsable;
};

struct RTP_STREAM_INFO
{
public:
	RTP_STREAM_INFO( )
	{
		m_nBufferLen = 0;
		m_pFrameBuffer = NULL;
		m_nDataLen = 0;
		m_nTimeStamp = 0;
		m_nMediaType = 0;
		m_nMediaEncodeFormat = 0;
		m_nStreamFlag = 0;
		m_bKeyFrame = false;
	}
	int m_nBufferLen;
	unsigned char *m_pFrameBuffer; //frame
	int m_nDataLen;
	unsigned int m_nTimeStamp;
	int m_nMediaType;
	int m_nMediaEncodeFormat;
	int m_nStreamFlag;
	bool m_bKeyFrame;

	FRAME_TIME_STAMP m_PrivateTimeStamp;
};


struct RTP_EXTENSION_INFO
{
public:
	RTP_EXTENSION_INFO( )
	{
		m_nEnableRtpExtension = 0;
		m_nDateInfo = 0;
		m_nTimeInfo = 0;
		m_cVersion = 0;
	}
	int m_nEnableRtpExtension;
	char m_strName[8];
	unsigned int m_nDateInfo;
	unsigned int m_nTimeInfo;
	unsigned char m_cVersion;
};

struct RTP_SESSION_INFO
{
public:
	RTP_SESSION_INFO( )
	{
		m_bSupportAudio = true;
		m_cPayload= 255;
	}
	bool 			m_bSupportAudio;
	unsigned char 	m_cPayload;
};


#endif
