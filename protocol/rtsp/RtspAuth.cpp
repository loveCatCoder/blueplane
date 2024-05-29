/*
 * =====================================================================================
 *
 *	Filename:  RtspAuth.h
 *
 *	Description: 			
 *	Version:  1.0
 *	Created: 2019-04-18
 *
 *	Author:  wty
 *	Organization:  ICHINAE(BEIJING) TECH.CO.,LTD. 
 *
 * =====================================================================================
*/
#include "include/private.h"
#include "RtspAuth.h"
#include "common/PubBase64.h"
#include "common/PubMd5.h"


CRtspAuth::CRtspAuth( const char* strUser, const char* strPassword )
{
	m_strUserName = strUser;
	m_strPassword = strPassword;
	m_nAuthType = -1;
}

CRtspAuth::~CRtspAuth( )
{
	
}

int CRtspAuth::ParseAuthData( const char* pMsg )
{
	if( NULL == pMsg )
	{
		return FAILURE;
	}

	//check "WWW-Authenticate"
	int nRet = CheckStringAttributeExist( pMsg, "www-authenticate" );
	if( FAILURE == nRet )
	{
		return FAILURE;
	}

	//check auth type
	if( CheckStringAttributeExist( pMsg, "digest" ) == SUCCESS )
	{
		m_nAuthType = AUTH_DIGEST;
		char strBuf[256];
		if( GetStringAttribute( pMsg, "realm", strBuf ) == SUCCESS )
		{
			m_strRealm = strBuf;
		}
		if( GetStringAttribute( pMsg, "nonce", strBuf ) == SUCCESS )
		{
			m_strNonce = strBuf;
		}		
	}
	else if( CheckStringAttributeExist( pMsg, "basic" ) == SUCCESS )
	{
		m_nAuthType = AUTH_BASIC;
	}
	else
	{
		return FAILURE;
	}
	
	return SUCCESS;
}

int CRtspAuth::ComputeDigestRequest( char* pDigest, const char* strUserName, const char* strPassword, const char* strCmd, const char* strUrl, const char* strRealm, const char* strNonce )
{
	if( (NULL == pDigest) || (NULL == strUserName) || (NULL == strPassword) || (NULL == strCmd) || (NULL == strUrl) || (NULL == strRealm) || (NULL == strNonce))
	{
		return FAILURE;
	}
	char Ha1Buf[33] = {0};
	char Ha2Buf[33] = {0};
	unsigned char * pHa1Data = NULL, *pHa2Data = NULL , *pDigestData = NULL;
	unsigned int nHa1DataLen = 0, nHa2DataLen = 0, nDigestDataLen = 0;

	nHa1DataLen = strlen( strUserName ) + 1 + strlen( strRealm ) + 1 + strlen( strPassword );
	pHa1Data = ( unsigned char * )malloc(nHa1DataLen + 1);
	snprintf((char*)pHa1Data, (nHa1DataLen + 1), "%s:%s:%s", strUserName, strRealm, strPassword );
	pub_md5( pHa1Data, nHa1DataLen, Ha1Buf );
	free( pHa1Data );
	pHa1Data = NULL;

	nHa2DataLen = strlen(strCmd) + 1 + strlen(strUrl);
	pHa2Data = ( unsigned char * )malloc( nHa2DataLen + 1 ) ;
	snprintf((char*)pHa2Data, (nHa2DataLen + 1),"%s:%s", strCmd, strUrl);
	pub_md5( pHa2Data, nHa2DataLen, Ha2Buf );
	free( pHa2Data );
	pHa2Data = NULL;

	nDigestDataLen = 32 + 1 + strlen( strNonce ) + 1 + 32;
	pDigestData = ( unsigned char * )malloc( nDigestDataLen + 1 ) ;
	snprintf( (char*)pDigestData, (nDigestDataLen + 1), "%s:%s:%s", Ha1Buf, strNonce, Ha2Buf );
	pub_md5( pDigestData, nDigestDataLen, pDigest );
	free( pDigestData );
	pDigestData = NULL;

	return SUCCESS;
}

int CRtspAuth::ComputeBasicRequest( char* pBasic, const char* strUserName, const char* strPassword )
{
	if( (NULL == pBasic) || (NULL == strUserName) || (NULL == strPassword) )
	{
		return FAILURE;
	}

	char TempBuf[256] = {0};
	int nLen = snprintf( TempBuf, 256,"%s:%s", strUserName, strPassword );

	base64encode( (unsigned char*)TempBuf, (unsigned char*)pBasic, nLen );
	return SUCCESS;
}

int CRtspAuth::GetAuth( char* pBuf, int nBufLen, const char* strCmd, const char* strUrl )
{
	if( NULL == pBuf )
	{
		return FAILURE;
	}

	char tempBuf[128] = {0};
	if( AUTH_DIGEST == m_nAuthType )
	{
		ComputeDigestRequest( tempBuf, m_strUserName.c_str( ), m_strPassword.c_str( ), strCmd, strUrl, m_strRealm.c_str( ), m_strNonce.c_str( ) );
		snprintf( pBuf, nBufLen, "Digest username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", response=\"%s\"", 
					m_strUserName.c_str( ), m_strRealm.c_str( ), m_strNonce.c_str( ), strUrl, tempBuf );
	}
	else if( AUTH_BASIC == m_nAuthType )
	{
		ComputeBasicRequest( tempBuf, m_strUserName.c_str( ), m_strPassword.c_str( ) );
		snprintf( pBuf, nBufLen, "Basic %s", tempBuf );
	}
	else
	{
		return FAILURE;
	}
	
	return SUCCESS;
}

void CRtspAuth::AllCharToLower( char *pBuf ) 
{ 
	int i=0;
	while( pBuf[i] != 0 ) 
	{	
		if((pBuf[i] >= 'A')&&(pBuf[i] <= 'Z'))
		{
			pBuf[i] +=32;
		}
		i++;
	}
}

int CRtspAuth::GetStringAttribute( const char* pMsg, const char *strAttr, char* pValue )
{
	if( (NULL == pMsg) || (NULL == strAttr) || (NULL == pValue) )
	{
		return FAILURE;
	}

	char *strTemp = strdup( pMsg );
	if( NULL == strTemp )
	{
		return FAILURE;
	}
	AllCharToLower( strTemp );
	
	const char* pEndPos = NULL;
	const char* pStratPos = strstr( strTemp, strAttr );
	if( NULL == pStratPos )
	{
		free( strTemp );
		return FAILURE;
	}
	pStratPos += strlen(strAttr);
	pStratPos = pMsg + (pStratPos - strTemp);

	//skip blank
	while( (*pStratPos  == ' ') || (*pStratPos  == '=') || (*pStratPos  == '"'))
	{
		pStratPos++;
	}
	//skip value
	pEndPos = pStratPos;
	while( (*pEndPos  != '\r') && (*pEndPos  != '\n') && ( *pEndPos  != '"' ) )
	{
		pEndPos++;
	}

	memcpy( pValue, pStratPos, pEndPos-pStratPos );
	pValue[pEndPos-pStratPos] = 0 ;
	free( strTemp );
	return SUCCESS;
}

int CRtspAuth::CheckStringAttributeExist( const char* pMsg, const char *strAttr )
{
	if( (NULL == pMsg) || (NULL == strAttr) )
	{
		return FAILURE;
	}
	char *strTemp = strdup( pMsg );
	if( NULL == strTemp )
	{
		return FAILURE;
	}
	AllCharToLower( strTemp );
	const char* pStratPos = strstr( strTemp, strAttr );
	if( NULL == pStratPos )
	{
		free( strTemp );
		return FAILURE;
	}

	free( strTemp );
	return SUCCESS;
}
