/*
 * =====================================================================================
 *
 *	Filename:  PubMd5.h
 *
 *	Description: 
					
				
 *	Version:  1.0
 *	Created:  06/01/2018
 *
 *	Author:  jgg
 *	Organization:  ICHINAE(BEIJING) TECH.CO.,LTD. 
 *
 * =====================================================================================
*/

#ifndef __PUB_MD5_H__
#define __PUB_MD5_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MD5Contest{
    unsigned int state[4];
    unsigned int count[2];
    unsigned char buffer[64];
}PUB_MD5_CTX;

void pub_md5_init( PUB_MD5_CTX * context );
void pub_md5_update( PUB_MD5_CTX * context, const unsigned char* input, unsigned int inputLen);
char* pub_md5_end( PUB_MD5_CTX * ctx, char* buf );
void pub_md5( const unsigned char* input, unsigned int inputLen, char* out);

#ifdef __cplusplus
};
#endif


#endif

