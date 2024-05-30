/*
 * =====================================================================================
 *
 *	Filename:  Pubbase64.h
 *
 *	Description:  
 *
 *	Version:  1.0
 *	Created:  11/26/2014
 *
 *	Author:  philip 
 *	Organization:  FIAMM(BEIJING) TECH.CO.,LTD. 
 *
 * =====================================================================================
*/



#ifndef __20190412_PUB_BASE64_H__
#define __20190412_PUB_BASE64_H__


#ifdef __cplusplus
extern "C" {
#endif


int base64encode(const unsigned  char *lpString, unsigned  char *lpBuffer, int sLen  );
int base64decode(unsigned char *lpString, const unsigned  char *lpSrc, int sLen );


#ifdef __cplusplus
};
#endif


#endif




