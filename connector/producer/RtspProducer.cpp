
#include "producer/RtspProducer.h"





RtspProducer::RtspProducer(EventLoop& loop)
{
	muduo::net::InetAddress peerAddr("10.0.21.42", 554);
	std::string name = "test";
	m_socket = std::make_shared<TcpSocket>(&loop,peerAddr,"test");
}

RtspProducer::~RtspProducer()
{
	if(m_rtpVideoSession != nullptr)
	{
		m_rtpVideoSession->DeinitRtpSession();
		delete m_rtpVideoSession;
		m_rtpVideoSession = nullptr;
	}
	if(m_rtspSession != nullptr)
	{
		m_rtspSession->DeinitRtspClinet();
		delete m_rtspSession;
		m_rtspSession = nullptr;

	}
	if(m_rtcpVideoSession!= nullptr)
	{
		m_rtcpVideoSession->DeinitRtcpSession();
		delete m_rtcpVideoSession;
		m_rtcpVideoSession = nullptr;
	}
}	

int RtspProducer::FrameReadyProcess( RTS_FRAME_READY_INFO &tFrameReadyInfo )
{
	log_debug("mediaType:%d,buffer:%x_%x_%x_%x_%x,len:%d,timestamp:%d,keyframe:%d",tFrameReadyInfo.m_nMediaType,tFrameReadyInfo.m_pFrame[0],tFrameReadyInfo.m_pFrame[1],tFrameReadyInfo.m_pFrame[2],tFrameReadyInfo.m_pFrame[3],tFrameReadyInfo.m_pFrame[4],tFrameReadyInfo.m_nLength,tFrameReadyInfo.m_nTimeStamp,tFrameReadyInfo.m_bKeyFrame);
	FRAME_INFO frame;
	frame.buffer = (char *)tFrameReadyInfo.m_pFrame;
	frame.len = tFrameReadyInfo.m_nLength;
	frame.key = tFrameReadyInfo.m_bKeyFrame;
	if(m_routerFrameCallback)
	{
		m_routerFrameCallback(frame);
	}
	return SUCCESS;
}

int RtspProducer::RtpFrameReadyProcess( RTP_STREAM_INFO &tRtpStreamInfo, void *pUserData )
{
	if( NULL == pUserData )
	{
		return FAILURE;
	}

	RtspProducer * producer = ( RtspProducer* )pUserData;

	RTS_FRAME_READY_INFO tFrameReadyInfo;
	tFrameReadyInfo.m_nMediaType = tRtpStreamInfo.m_nMediaType;
	tFrameReadyInfo.m_pFrame = tRtpStreamInfo.m_pFrameBuffer;
	tFrameReadyInfo.m_nLength = tRtpStreamInfo.m_nDataLen;
	tFrameReadyInfo.m_nTimeStamp = tRtpStreamInfo.m_nTimeStamp;
	tFrameReadyInfo.m_bKeyFrame = tRtpStreamInfo.m_bKeyFrame;
	
	producer->FrameReadyProcess( tFrameReadyInfo );

	return SUCCESS;
}


int RtspProducer::Init()
{
    if( m_rtspSession == NULL )
	{
		m_rtspSession = new CRtspClient( );
		if( m_rtspSession == NULL )
		{
			log_debug( "CRtspRtsChannel create CRtspClient error" );

		}
	}
	int nRet = m_rtspSession->InitRtspClient( "rtsp://10.0.21.42:554", "admin", "admin123" );
	if( nRet != SUCCESS )
	{
		log_debug( " CRtspRtsChannel InitRtspClient error");

	}


	//init rtp session
	if( m_rtpVideoSession == NULL )
	{
		m_rtpVideoSession = new CRtpSession( );
	}
	if( m_rtpVideoSession == NULL )
	{
		log_warn( "CRtspRtsChannel create m_pRtpVideoSession error");
		return FAILURE;
	}
	m_rtpVideoSession->SetFrameReadyCallBack(RtspProducer::RtpFrameReadyProcess ,(void*) this);

		//init rtcp
	if( m_rtcpVideoSession == NULL )
	{
		m_rtcpVideoSession = new CRtcpSession( );
	}	
	if( m_rtcpVideoSession == NULL )
	{
		log_warn( "CRtspRtsChannel create m_pRtcpVideoSession error" );
		return FAILURE;

	}
	m_rtcpVideoSession->InitRtcpSession( m_rtpVideoSession );
	
	m_videoRtpStreamInfo.m_nBufferLen = RTP_VIDEO_FRAME_MAX_LEN;
	m_videoRtpStreamInfo.m_pFrameBuffer = (unsigned char *)malloc(m_videoRtpStreamInfo.m_nBufferLen);

    log_debug( "CRtspRtsChannel create CRtspClient success" );
    return SUCCESS;
}


int RtspProducer::Destroy()
{
	if(m_currStatus != "teardown")
	{
		SendTeardownRequest();
	}
    return SUCCESS;
}


int RtspProducer::StartStream()
{
    
	m_socket->SetReadCallback([this](const TcpSocketPtr& ptr,
                            Buffer* buffer,
                            muduo::Timestamp time){
		int len = buffer->readAll(m_recvBuf.data()+m_recvLen);
		m_recvLen += len;
		log_debug("recv len:%d,sum len:%d",len,m_recvLen);
		ReceiveTcpMessage();
	});
	m_socket->AsyncConnect([this](const TcpSocketPtr&,int err){
	  if (err == 0)
      {
		SendOptionsRequest();
      }
	  else
	  {
		log_error("rtsp options send:%d",err);
	  }
	});
	return SUCCESS;
}

int RtspProducer::SendOptionsRequest()
{
	int nLen = 0;
	int nRet = SUCCESS;
	char SendBuf[2048] = {0};
	int nSendCount = 0;
	
	if( m_rtspSession == NULL )
	{
		return FAILURE;
	}
	
	nLen = m_rtspSession->CreateOptionRequest( SendBuf, 2048 );
	log_info( "rtsp options send:%s", SendBuf );

	m_socket->Send(SendBuf,strlen(SendBuf));
	m_currStatus = "option";
	return SUCCESS;
}

int RtspProducer::SendDescribeRequest()
{
	int nLen = 0;
	int nRet = SUCCESS;
	char SendBuf[2048] = {0};
	int nSendCount = 0;
	
	if( m_rtspSession == NULL )
	{
		return FAILURE;
	}
	
	nLen = m_rtspSession->CreateDescribeRequest( SendBuf, 2048 );
	log_info( "rtsp options send:%s", SendBuf );
	m_socket->Send(SendBuf,strlen(SendBuf));
	m_currStatus = "describe";
	return SUCCESS;
}

int RtspProducer::SendVideoSetupRequest()
{
	int nLen = 0;
	int nRet = SUCCESS;
	char SendBuf[2048] = {0};
	if( m_rtspSession == NULL )
	{
		return FAILURE;
	}

	nLen = m_rtspSession->CreateSetupRequest( SendBuf, 2048, VIDEO_TRACK, 2,0,1 );
	log_info( "rtsp video setup send:%s",SendBuf );
	m_socket->Send(SendBuf,strlen(SendBuf));
	m_currStatus = "setup";
	return SUCCESS;
}

int RtspProducer::SendPlayRequest()
{
	int nLen = 0;
	int nRet = SUCCESS;
	char SendBuf[2048] = {0};
	int nSendCount = 0;
	
	if( m_rtspSession == NULL )
	{
		return FAILURE;
	}
	
	nLen = m_rtspSession->CreatePlayRequest( SendBuf, 2048 ,0,0 );
	log_info( "rtsp options send:%s", SendBuf );
	m_socket->Send(SendBuf,strlen(SendBuf));
	m_currStatus = "play";
	return SUCCESS;
}

int RtspProducer::SendTeardownRequest()
{
	int nLen = 0;
	int nRet = SUCCESS;
	char SendBuf[2048] = {0};
	int nSendCount = 0;
	
	if( m_rtspSession == NULL )
	{
		return FAILURE;
	}
	
	nLen = m_rtspSession->CreateTeardownRequest( SendBuf, 2048 );
	log_info( "rtsp options send:%s", SendBuf );
	m_socket->Send(SendBuf,strlen(SendBuf));
	m_currStatus = "teardown";
	return SUCCESS;
}

int RtspProducer::ReceiveTcpMessage()
{
start:
	if(m_recvBuf[0] == 0x24)
	{
		if(m_recvLen <4)
			return 0;
		int len = 0 ;
		len = ((m_recvBuf[2]&0xff)<<8) +(m_recvBuf[3]&0xff);
		log_debug("rtp len:%d,sum len:%d",len,m_recvLen);
		if(m_recvLen >= (len + 4) )
		{
			ReceiveRtpMessage(len);
			std::rotate(m_recvBuf.begin(),m_recvBuf.begin()+len+4,m_recvBuf.end());
			m_recvLen -=4;
			m_recvLen -=len;
			log_debug("rtp len:%d,sum len:%d",len,m_recvLen);
			if((m_recvLen != 0)&&(m_recvBuf[0] == 0x24))
				goto start;
		}
	}
	else if(m_recvLen != 0)
	{
		log_debug("ReceiveRtspMessage:%s",m_recvBuf.data());
		ReceiveRtspMessage();
	}
	return 0;
}

int RtspProducer::ReceiveRtspMessage()
{
	if(m_currStatus == "option")
	{
		int nRet = m_rtspSession->ParseOptionReply( m_recvBuf.data() );
		if(nRet == RTSP_TYPE_UNAUTHORIZED)
		{
			SendOptionsRequest();
		}
		else if(nRet != SUCCESS )
		{
			log_debug("ParseOptionReply:%d",nRet);
		}
		else
		{
			SendDescribeRequest();
		}
	}
	else if(m_currStatus == "describe")
	{
		int nRet = m_rtspSession->ParseDescribeReply( m_recvBuf.data() );
		if(nRet == RTSP_TYPE_UNAUTHORIZED)
		{
			log_debug("ParseDescribeReply : RTSP_TYPE_UNAUTHORIZED");
			SendDescribeRequest();
		}
		else if(nRet != SUCCESS )
		{
			log_debug("ParseDescribeReply:%d",nRet);
		}
		else
		{
			RTP_SESSION_INFO tRtpSessionInfo;
			tRtpSessionInfo.m_cPayload = 1;
			m_rtpVideoSession->InitRtpSession( tRtpSessionInfo );
			SendVideoSetupRequest();
		}
	}
	else if(m_currStatus == "setup")
	{
		int rtpPort =0;
		int rtcpPort = 1;
		int nRet = m_rtspSession->ParseSetupReply( m_recvBuf.data() ,&rtpPort,&rtcpPort);
		if(nRet != SUCCESS )
		{
			log_debug("ParseSetupReply:%d",nRet);
		}
		else
		{
			SendPlayRequest();
		}
	}
	else if(m_currStatus == "play")
	{
		int seqVideo ,seqAudio = 0;
		unsigned int rtpTimeVideo,rtpTimeAudio = 0;
		int nRet = m_rtspSession->ParsePlayReply( m_recvBuf.data(),&seqVideo,&rtpTimeVideo,&seqAudio,&rtpTimeAudio );
		if(nRet != SUCCESS )
		{
			log_debug("ParsePlayReply:%d",nRet);
		}
		else
		{
			//SendPlayRequest();
			//ReceiveFrameHeader();
		}
	}
	else if(m_currStatus == "teardown")
	{
		int nRet = m_rtspSession->ParseTeardownReply( m_recvBuf.data() );
		if(nRet != SUCCESS )
		{
			log_debug("ParseTeardownReply:%d",nRet);
		}
	}
	m_recvLen = 0;
	m_recvBuf.fill(0);
	return SUCCESS;
}

int RtspProducer::ReceiveRtpMessage(int len)
{
	if(m_recvBuf[0] == 0x24&&m_recvBuf[1] == 0x00)
	{
		int nPackageNum = CRtpSession::ParsePackageNum( (char*)(m_recvBuf.data()+4) );
		log_debug("nPackageNum :%d",nPackageNum);
		m_rtpVideoSession->ParsePackage( (char*)(m_recvBuf.data()+4), len, m_videoRtpStreamInfo );
	}
	else if(m_recvBuf[0] == 0x24&&m_recvBuf[1] == 0x01)
	{
		m_rtcpVideoSession->SetRecvTime( );
		int nRtcpLen = m_rtcpVideoSession->GetPackageLen( (const char*)m_recvBuf.data() );
		m_rtcpVideoSession->ParseRtcpPackage( (const char*)m_recvBuf.data() );
		// if( nRtcpLen < length +4 )
		// {
		// 	m_rtcpVideoSession->ParseRtcpPackage( (const char*)(m_recvBuf.data() + nRtcpLen) );
		// }

		unsigned char SendBuf[2048];
		int nSendLen = 0;
		int nRet = CreateVideoRtcpPackage( RTCP_SDES, SendBuf, &nSendLen );
		

		SendBuf[0] = 0x24;
		SendBuf[1] = 1;
		int TempLen = nSendLen - 4;
		TempLen = ntohs(TempLen);
		memcpy( SendBuf + 2, &TempLen, 2);
		m_socket->Send(SendBuf,nSendLen);
	}
	return 0;
}



int RtspProducer::SetRouterFrameCallback(std::function<void(FRAME_INFO)> cb)
{
	if(cb)
	{
		log_debug("set router frame callback is success");
		m_routerFrameCallback = cb;
	}
	return SUCCESS;
}

int RtspProducer::CreateVideoRtcpPackage(E_RTCP_PACKET_TYPE PackageType, unsigned char* pBuf, int* pnLen)
{
	int nLen = 0;
	int nRet = 0;
	int nPacketLen = 0;

	nLen = 4;
	nRet = m_rtcpVideoSession->CreateRtcpPackage( RTCP_RR, pBuf + nLen, &nPacketLen );
	if( nRet == FAILURE)
	{
		return FAILURE;
	}
	nLen += nPacketLen;
	nRet = m_rtcpVideoSession->CreateRtcpPackage( PackageType, pBuf + nLen, &nPacketLen );
	if( nRet == FAILURE)
	{
		return FAILURE;
	}
	nLen += nPacketLen;
		

	*pnLen = nLen;
	return nRet;
}




int RtspProducer::ReceiveFrameHeader()
{
	m_recvBuf.fill(0);
	// asio::async_read(m_socket, asio::buffer(m_recvBuf, 4),[this](std::error_code ec, std::size_t length /*length*/){
	// 	if (!ec )
	// 	{
	// 		//24_ 0_ 0_28
	// 		int len = 0;
	// 		len = ((m_recvBuf[2]&0xff)<<8) +(m_recvBuf[3]&0xff);
	// 		//log_debug("rtsp header  %2x_%2x_%2x_%2x,%ld_%d",m_recvBuf[0],m_recvBuf[1],m_recvBuf[2],m_recvBuf[3],length,len);
			
	// 		ReceiveFrameBody(len);
	// 	}
	// 	else
	// 	{
	// 		log_error("tcp connection error :%s",ec.message().c_str());
	// 		if(length == 0)
	// 		{
	// 			Destroy();
	// 			if(m_statusCallback)
	// 			{
	// 				m_statusCallback("rtsp_producer",E_NODE_STATUS::NODE_CLOSE);
	// 			}
	// 		}
	// 	}
	// });
	return 0;
}
int RtspProducer::ReceiveFrameBody(int len)
{
	return 0;
}
#if 0
int RtspProducer::ReceiveFrameBody(int len)
{
	//m_recvBuf.fill(0);
	asio::async_read(m_socket, asio::buffer(m_recvBuf+4, len),[len,this](std::error_code ec, std::size_t length){
		if (!ec )
		{
			if(m_recvBuf[0] == 0x24&&m_recvBuf[1] == 0x00)
			{
				int nPackageNum = CRtpSession::ParsePackageNum( (char*)(m_recvBuf+4) );
				//log_debug("nPackageNum :%d",nPackageNum);
				m_rtpVideoSession->ParsePackage( (char*)(m_recvBuf+4), len, m_videoRtpStreamInfo );
			}
			else if(m_recvBuf[0] == 0x24&&m_recvBuf[1] == 0x01)
			{
				m_rtcpVideoSession->SetRecvTime( );
				int nRtcpLen = m_rtcpVideoSession->GetPackageLen( (const char*)m_recvBuf );
				m_rtcpVideoSession->ParseRtcpPackage( (const char*)m_recvBuf );
				if( nRtcpLen < length +4 )
				{
					m_rtcpVideoSession->ParseRtcpPackage( (const char*)(m_recvBuf + nRtcpLen) );
				}

				unsigned char SendBuf[2048];
				int nSendLen = 0;
				int nRet = CreateVideoRtcpPackage( RTCP_SDES, SendBuf, &nSendLen );
				
	
				SendBuf[0] = 0x24;
				SendBuf[1] = 1;
				int TempLen = nSendLen - 4;
				TempLen = ntohs(TempLen);
				memcpy( SendBuf + 2, &TempLen, 2);

				asio::async_write(m_socket, asio::buffer(SendBuf,nSendLen), [this](const asio::error_code& err, size_t len) {
					if(err)
					{
						log_error("rtsp teardown send:%s",err.message().c_str());
					}
				});
				
			}
			
			ReceiveFrameHeader();
		}
		else
		{
			log_error("tcp connection error :%s",ec.message().c_str());
			if(length == 0)
			{
				Destroy();
				if(m_statusCallback)
				{
					m_statusCallback("rtsp_producer",E_NODE_STATUS::NODE_CLOSE);
				}
			}
		}
	});
	return 0;
}
#endif