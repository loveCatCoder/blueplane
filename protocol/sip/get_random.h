/******************************************************************************
 *
 *
 *
 *
 *
 *
 * ***************************************************************************/
#ifndef __SIP_GET_H__
#define __SIP_GET_H__

#include <iostream>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif
#define RANDOM_LENGTH   512
#define HEADER_LENGTH   512
#define HASHHEXLEN 32
#define HASHLEN 16
typedef char HASHHEX[HASHHEXLEN + 1];
typedef char HASH[HASHLEN];

#define GET_SUCCESS               0
#define GET_UNDEFINED_ERROR      -1

int  get_new_cseq();

std::string get_new_branch(  );

std::string get_new_call_id( ) ;

std::string get_new_tag(  ) ;

void get_md5(const char *method , const char *username ,const char *uri ,const char *realm ,const char *nonce ,
        const char *password , char *out ) ;
std::string get_new_nonce( );

#ifdef __cplusplus
}
#endif
#endif

