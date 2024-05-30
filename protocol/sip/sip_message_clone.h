#ifndef __SIP_HEADERS_CLONE_H__
#define __SIP_HEADERS_CLONE_H__

#include "sip_message.h"
#ifdef __cplusplus
extern "C" {
#endif
//to function
int sip_message_clone_to( sip_to_t *to , sip_to_t **dest_to ) ;

//from function
int sip_message_clone_from( sip_from_t *from , sip_from_t **dest_from ) ;

//contact function
int sip_message_clone_contact( sip_contact_t *contact , sip_contact_t **dest_contact ) ;


//via funciton
int sip_message_clone_via( sip_via_t *via , sip_via_t **dest_via ) ;
int sip_message_clone_vias(sip_via_t *vias , sip_via_t **dest_vias );

//cseq function
int sip_message_clone_cseq( sip_cseq_t *cseq , sip_cseq_t **dest_cseq ) ;


//content length funciton
int sip_message_clone_content_length( char *content_length , char **dest_content_length ) ;

//content type function
int sip_message_clone_content_type( sip_content_type_t *content_type , sip_content_type_t **dest_content_type ) ;

//callid function
int sip_message_clone_callid( sip_callid_t *sip_callid , sip_callid_t **dest_sip_callid ) ;


//max forwards function
int sip_message_clone_max_forwards(  char *max_forwards , char **dest_max_forwards  ) ;

//expires function
int sip_message_clone_expires(  char *expires  , char **dest_expires  ) ;

//date function
int sip_message_clone_date(  char *date, char **dest_date  ) ;

//www_authenticate function
int sip_message_clone_www_authenticate(  sip_www_authenticate_t *www_authenticate , sip_www_authenticate_t **dest_www_authenticate ) ;

//authorization
int sip_message_clone_authorization(  sip_authorization_t *authorization , sip_authorization_t **dest_authorization ) ;

int sip_message_clone(sip_message_t *sip_message,sip_message_t **dest_sip_message);

#ifdef __cplusplus
}
#endif
#endif

