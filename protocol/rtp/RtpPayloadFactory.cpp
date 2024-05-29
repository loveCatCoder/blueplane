/*
 * =====================================================================================
 *
 *	Filename:  RtpPayloadFactory.cpp
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
#include "RtpPayloadFactory.h"

CRtpPayloadFactory::CRtpPayloadFactory( )
{
	
}

CRtpPayloadFactory::~CRtpPayloadFactory( )
{

}

int CRtpPayloadFactory::RegistClass( const std::string &strClassName, RtpPayloadObjectCreator pCreator )
{
	m_MapRtpPayloadClass[strClassName] = pCreator;
	return 0;
}

void* CRtpPayloadFactory::GreateObject( const std::string &strClassName )
{
	auto iter = m_MapRtpPayloadClass.find( strClassName );
	if( iter == m_MapRtpPayloadClass.end() )
	{
		return NULL;
	}
	return iter->second( );
}

CRtpPayloadFactory* CRtpPayloadFactory::GetInstance( )
{
	static CRtpPayloadFactory* pRtpPayloadFactroy = NULL;

	if ( NULL == pRtpPayloadFactroy )
	{
		pRtpPayloadFactroy = new CRtpPayloadFactory( );
	}
	return pRtpPayloadFactroy;
}

