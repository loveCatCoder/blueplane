/*********************************************************************************************
*
*
*
*
*
*********************************************************************************************/

#ifndef __SIP_HEADERS_PARSER_H__
#define __SIP_HEADERS_PARSER_H__

#include "sip_message.h"
#ifdef __cplusplus
extern "C" {
#endif
//add  gyl 20121107
//url
int sip_message_parse_uri(char *value, sip_uri_t **url);
//add end
//to function
int sip_message_parse_to( char *value , sip_to_t **to ) ;
int sip_message_set_to( char *value , sip_to_t **to ) ;

//from function
int sip_message_parse_from( char *value , sip_from_t **from ) ;
int sip_message_set_from( char *value , sip_from_t **from ) ;

//contact function
int sip_message_parse_contact( char *value , sip_contact_t **contact ) ;
int sip_message_set_contact( char *value , sip_contact_t **contact ) ;


//via funciton
int sip_message_parse_via( char *value , sip_via_t **via ) ;
int sip_message_set_via( char *value , sip_via_t **via ) ;


//cseq function
int sip_message_parse_cseq( char *value , sip_cseq_t **cseq ) ;
int sip_message_set_cseq( char *value , sip_cseq_t **cseq ) ;


//content length funciton
int sip_message_parse_content_length( char *value , char **content_length ) ;
int sip_message_set_content_length( char *value , char **content_length ) ;


//content type function
int sip_message_parse_content_type( char *value , sip_content_type_t **content_type ) ;
int sip_message_set_content_type( char *value , sip_content_type_t **content_type ) ;


//callid function
int sip_message_parse_callid( char *value , sip_callid_t **sip_callid ) ;
int sip_message_set_callid( char *value , sip_callid_t **sip_callid ) ;


//max forwards function
int sip_message_parse_max_forwards(  char *value , char **max_forwards  ) ;
int sip_message_set_max_forwards(  char *value , char **max_forwards  ) ;

//expires function
int sip_message_parse_expires(  char *value , char **expires  ) ;
int sip_message_set_expires(  char *value , char **expires  ) ;

//date function
int sip_message_parse_date(  char *value , char **date  ) ;
int sip_message_set_date(  char *value , char **date  ) ;
int sip_message_get_time(char *date,sip_time_t *time);

//www_authenticate function
int sip_message_parse_www_authenticate(  char *value , sip_www_authenticate_t **www_authenticate ) ;
int sip_message_set_www_authenticate(  char *value , sip_www_authenticate_t **www_authenticate ) ;

//subject
int sip_message_parse_subject(  char *value  , sip_subject_t **subject ) ;
int sip_message_set_subject(  char *value  , sip_subject_t **subject ) ;

//user agent funciton
int sip_message_parse_user_agent( char *value , char **user_agent ) ;
int sip_message_set_user_agent( char *value , char **user_agent ) ;

//user agent funciton
int sip_message_parse_event( char *value , char **event ) ;
int sip_message_set_event( char *value , char **event ) ;

//subscription_state funciton
int sip_message_parse_subscription_state( char *value , char **subscription_state ) ;
int sip_message_set_subscription_state( char *value , char **subscription_state ) ;
//authorization function
int sip_message_parse_authorization(  char *value , sip_authorization_t **authorization ) ;
int sip_message_set_authorization(  char *value , sip_authorization_t **authorization ) ;

//add 20121107
int sip_message_create_authorization(sip_message_t *sip_message,sip_authorization_t **authorization, 
	char * password, char * server_id, char * server_ip, int server_port);
//add end

#ifdef __cplusplus
}
#endif
#endif


