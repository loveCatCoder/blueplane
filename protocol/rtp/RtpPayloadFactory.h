/*
 * =====================================================================================
 *
 *	Filename:  RtpPayloadFactory.h
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
#ifndef __RTP_PAYLOAD_FACTORY_H__
#define __RTP_PAYLOAD_FACTORY_H__

#include "RtpPrivate.h"

typedef void* (*RtpPayloadObjectCreator)(void);

class CRtpPayloadFactory
{
public:
	CRtpPayloadFactory( );
	virtual ~CRtpPayloadFactory( );
public:
	int RegistClass( const std::string &strClassName, RtpPayloadObjectCreator pCreator );
	void* GreateObject( const std::string &strClassName );
	static CRtpPayloadFactory* GetInstance( );
private:
	std::map<std::string, RtpPayloadObjectCreator> m_MapRtpPayloadClass;
};

//register action
class RtpPayloadRegisterAction
{
public:
	RtpPayloadRegisterAction( const std::string & strClassName, RtpPayloadObjectCreator pCreator )
	{
		CRtpPayloadFactory::GetInstance( )->RegistClass( strClassName, pCreator );
	}
};


//rtp payload register
#define RTP_PAYLOAD_REGISTER( RTP_PAYLOAD_CLASS_NAME ) \
    RTP_PAYLOAD_CLASS_NAME* Creator##RTP_PAYLOAD_CLASS_NAME( ){ \
		return new RTP_PAYLOAD_CLASS_NAME;\
    } \
    RtpPayloadRegisterAction gRtpPayloadRegisterAction##RTP_PAYLOAD_CLASS_NAME(#RTP_PAYLOAD_CLASS_NAME, (RtpPayloadObjectCreator)Creator##RTP_PAYLOAD_CLASS_NAME)


#endif
