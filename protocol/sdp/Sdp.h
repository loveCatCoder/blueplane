/*
 * =====================================================================================
 *
 *	Filename:  Sdp.h
 *
 *	Description: 			
 *	Version:  1.0
 *	Created: 2019-04-11
 *
 *	Author:  wty
 *	Organization:  ICHINAE(BEIJING) TECH.CO.,LTD. 
 *
 * =====================================================================================
*/
#ifndef __SDP_H__
#define __SDP_H__
#include "include/private.h"
#include "sip/sdp_message.h"


//media info , video and audio
struct SDP_MEDIA_INFO
{
public:
        SDP_MEDIA_INFO( )
        {
                m_nMediaFormat = 0;
                m_nBandWidth = 0;
                m_nPacketizationMode = 0;
                m_nProfileLevelId = 0;
        }
        std::string                m_strMediaType;
        std::string                m_strMediaProtocol;
        int                                m_nMediaFormat;
        std::string                m_strNetType;
        std::string                m_strAddrType;
        std::string                m_strAddr;
        std::string                m_strBandWidthType;
        int                                m_nBandWidth;
        
        std::string                m_strPayload;
        int                                m_nPacketizationMode;
        int                                m_nProfileLevelId;
        std::string                m_strSPS;
        std::string                m_strPPS;
        std::string                m_strTrack;
};


class CSdpSession
{
public:
	CSdpSession( );
	virtual ~CSdpSession( );
public:
	int SdpMessageParse( const char* strSdp );
	SDP_MEDIA_INFO* GetVideoMediaInfo( ){ return m_pVideoMediaInfo;};
	SDP_MEDIA_INFO* GetAudioMediaInfo( ){ return m_pAudioMediaInfo;};
private:
	int ParseMedia( sdp_message_t * pSdpMsg, SDP_MEDIA_INFO * pMediaInfo,  int nMediaPos );
	int ParseRtpmap( sdp_message_t * pSdpMsg, SDP_MEDIA_INFO * pMediaInfo, int nMediaPos, int nPos );
	int ParseFmtp( sdp_message_t * pSdpMsg, SDP_MEDIA_INFO * pMediaInfo, int nMediaPos, int nPos );
	int ParseControl( sdp_message_t * pSdpMsg, SDP_MEDIA_INFO * pMediaInfo, int nMediaPos, int nPos );
private:
	SDP_MEDIA_INFO * m_pVideoMediaInfo;
	SDP_MEDIA_INFO * m_pAudioMediaInfo;
};



#endif

