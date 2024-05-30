/*******************************************************************************************
*		
*		
*		
*		
*		
*******************************************************************************************/


#ifndef __SIP_COMMON_H__
#define __SIP_COMMON_H__

#define SIP_CR		(13)
#define SIP_LF		(10)
#define SIP_COLON	(58)
#define SIP_BLANK	(32)
#include "sip_std.h"
#ifdef __cplusplus
extern "C" {
#endif
//string function
int sip_find_next_crlf( char *start_pos , char **end_pos ) ;

int sip_find_next_character( char *start_pos , char **end_pos ,const char ch) ;

int sip_find_next_string( char *start_pos , char **end_pos ,const char *str ) ;

int sip_str_cpy( char *dest , int max ,const char *source , int num ) ;

int sip_str_append(char **dest, const char *source);

int sip_find_quotes(char *pos,char **start_quote,char **end_quote);

int sip_str_find_end( char *start_pos , char **end_pos ) ;
//memory funtion
void *sip_malloc(size_t size) ;

void *sip_realloc(void *ptr, size_t size) ;

void sip_free(void *ptr) ; 


//delay function
 void sip_msleep( int msec ) ;

#ifdef __cplusplus
}
#endif

#endif

