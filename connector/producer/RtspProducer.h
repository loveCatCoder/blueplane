

#ifndef _RTSP_PRODUCER_H_
#define _RTSP_PRODUCER_H_


#include "include/private.h"
#include "protocol/rtsp/RtspClient.h"
#include "protocol/rtp/RtpSession.h"
#include "protocol/rtcp/RtcpSession.h"


class RtspProducer
{
private:
    /* data */
public:
    RtspProducer(asio::io_context& io);
    ~RtspProducer();

    int Init();
    int Destroy();

    //Frame Callback
	int FrameReadyProcess( RTS_FRAME_READY_INFO &tFrameReadyInfo );
    static int RtpFrameReadyProcess( RTP_STREAM_INFO &tRtpStreamInfo, void *pUserData );

    //Rtsp Send Message
    int StartStream();
    int SendOptionsRequest();
    int SendDescribeRequest();
    int SendVideoSetupRequest();
    int SendPlayRequest();
    int SendTeardownRequest();
    //Receive tcp rtsp rtp message
    int ReceiveTcpMessage();
    int ReceiveRtspMessage();
    int ReceiveRtpMessage();

    int SetRouterFrameCallback(std::function<void(FRAME_INFO)> cb);
    int SetReportStatusCallback(std::function<int(std::string id,E_NODE_STATUS eStatus)> cb){m_statusCallback = cb;return SUCCESS;}

    int CreateVideoRtcpPackage( E_RTCP_PACKET_TYPE PackageType, unsigned char* pBuf, int* pnLen );
private:
    int ReceiveFrameHeader();
    int ReceiveFrameBody(int len);
private:
    CRtspClient *m_rtspSession = nullptr;
    CRtpSession *m_rtpVideoSession = nullptr;
    CRtcpSession *m_rtcpVideoSession = nullptr;
    RTP_STREAM_INFO m_videoRtpStreamInfo;
    asio::ip::tcp::socket m_socket;
    char m_recvBuf[2048] = {0};
    int m_recvLen = 0;
    std::string m_currStatus = "teardown";
    std::function<void(FRAME_INFO)> m_routerFrameCallback;
    std::function<int(std::string id,E_NODE_STATUS eStatus)> m_statusCallback;
};


#endif