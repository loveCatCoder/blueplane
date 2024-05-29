
#ifndef _ONION_PRIVATE_H_
#define _ONION_PRIVATE_H_







#include <time.h>


#include <string>
#include <vector>
#include <map>
#include <memory>
#include <list>
#include <functional>
#include <cstdint>

#include <arpa/inet.h>

#include <zlog/src/zlog.h>
#include <asio/include/asio.hpp>

#define project_name "onion"



#define SUCCESS 0
#define FAILURE -1


#define log_debug   dzlog_debug
#define log_info    dzlog_info
#define log_warn    dzlog_warn
#define log_error   dzlog_error


#define UDP_MAX_RTP_LEN 1400

struct STRING_INFO
{
	char *strBuf;
	int nLength;
};


typedef enum
{
	VIDEO_TRACK = 1,
	AUDIO_TRACK = 2,
}E_TRACK_TYPE;


typedef enum
{
	NODE_CLOSE = 1,
}E_NODE_STATUS;


namespace http{
    using namespace std;
    struct response{
        string status;
        int statusCode;
        string proto;
        map<string,string> header;
        int contentLength;
    };

    struct request{
        string Method;
        string url;
        string proto;
        map<string,string> header;
        string host;
        int port;
        int contentLength;
    };

};

enum MEDIA_VIDEO_TYPE{
    VIDEO_DATA =1,
    AUDIO_DATA,
};
enum PECKET_PAYLOAD_FORMAT
{
	PAYLOAD_FORMAT_NULL = 0,
	PAYLOAD_FORMAT_H264 = 1,
	PAYLOAD_FORMAT_SVAC = 2,
	PAYLOAD_FORMAT_H265 = 3,

	PAYLOAD_FORMAT_PCMA = 101,
	PAYLOAD_FORMAT_PCMU = 102,
	PAYLOAD_FORMAT_S16_LE_8000 = 103 ,
	PAYLOAD_FORMAT_S16_LE_16000 = 104 ,

	PAYLOAD_FORMAT_PS = 201,
};


struct RTS_FRAME_READY_INFO
{
public:
	RTS_FRAME_READY_INFO( )
	{
		m_pFrame = NULL;
		m_nMediaType = 0;
		m_nMediaEncodeFormat = 0;
		m_nLength = 0;
		m_nTimeStamp = 0;
		m_bKeyFrame = false;

		m_bNeedReadScache = false;
		m_nPos = 0;
		m_nScacheIndex = 0;
	}
	unsigned char* 	m_pFrame;
	int 			m_nMediaType;
	int 			m_nMediaEncodeFormat;
	int 			m_nLength;
	
	unsigned int 	m_nTimeStamp;
	bool 			m_bKeyFrame;

	bool 			m_bNeedReadScache;
	int 			m_nPos;
	int 			m_nScacheIndex;
};

//stream info
struct STREAM_INFO
{
	STREAM_INFO( )
	{
		m_nVideoFormat = 0;
		m_nWidth = 0;
		m_nHeight = 0;
		m_nFrameRate = 0;
		m_bAudioEnable = false;
		m_nAudioFormat = 0;
		m_nAudioChannel = 0;
		m_nSamplingRate = 0;
		m_nSamplingBit = 0;
		
	}
	int 			m_nVideoFormat; //video code 
	int 			m_nWidth;       //video width
	int 			m_nHeight;      //video height
	int 			m_nFrameRate;   //video frame rate
	
	bool 			m_bAudioEnable; //audio enable
	int 			m_nAudioFormat; //audio vode
	int 			m_nAudioChannel;//audio channels
	int 			m_nSamplingRate;//audio sampling rate (8K,16K..)
	int 			m_nSamplingBit; //audio sampling bit (8bit,16bit..)

	std::string 	m_strBase64Sps; //video sps base64 code
	std::string 	m_strBase64Pps; //video pps base64 code
};

/*
	network link type
*/
typedef enum 
{
	LINK_TYPE_UDP = 1,
	LINK_TYPE_TCP = 2, //PASSIVE
	LINK_TYPE_TCP_ACTIVE = 3
}E_LINK_TYPE ;


/*
	packet payload format
*/
typedef enum
{
	ENCODE_NULL = 0,
	ENCODE_H264 = 1,
	ENCODE_SVAC = 2,
	ENCODE_H265 = 3,

	ENCODE_PCMA = 101,
	ENCODE_PCMU = 102,
	ENCODE_S16_LE_8000 = 103,
	ENCODE_S16_LE_16000 = 104,
}E_ENCODE_TYPE;




struct FRAME_INFO
{
	char *buffer = nullptr;
	int len = 0;
	int encode=0;
	bool key = false;
};


#endif