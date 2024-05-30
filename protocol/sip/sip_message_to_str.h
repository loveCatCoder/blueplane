#ifndef __SIP_MESSAGE_TO_STR_H__
#define __SIP_MESSAGE_TO_STR_H__

#include "sip_message.h"
#ifdef __cplusplus
extern "C" {
#endif
#define CRLF "\r\n"

//to function
int sip_message_to_to_str( sip_to_t *to , char **dest );

//from function
int sip_message_from_to_str( sip_from_t *from , char **dest );

//contact function
int sip_message_contact_to_str(sip_contact_t *contact , char **dest );


//via funciton
int sip_message_via_to_str( sip_via_t *via , char **dest );
int sip_message_vias_to_str(sip_via_t *vias , char **dest);

//cseq function
int sip_message_cseq_to_str( sip_cseq_t *cseq , char **dest );


//content length funciton
int sip_message_content_length_to_str( char *content_length, char **dest );

//content type function
int sip_message_content_type_to_str( sip_content_type_t *content_type, char **dest );

//callid function
int sip_message_callid_to_str( sip_callid_t *callid, char **dest );


//max forwards function
int sip_message_max_forwards_to_str( char *max_forwards, char **dest );

//expires function
int sip_message_expires_to_str( char *expires, char **dest );

//date function
int sip_message_date_to_str( char *date , char **dest );

//www_authenticate function
int sip_message_www_authenticate_to_str( sip_www_authenticate_t *www_authenticate, char **dest );

//authorization function
int sip_message_authorization_to_str( sip_authorization_t *authorization, char **dest );
//subject
int sip_message_subject_to_str(sip_subject_t *subject,char **dest);

//user_agent funciton
int sip_message_user_agent_to_str( char *user_agent, char **dest );
//user_agent funciton
int sip_message_event_to_str( char *event, char **dest );
//subscription_state funciton
int sip_message_subscription_state_to_str( char *subscription_state, char **dest );
//startline 
int sip_message_startline_to_str(sip_message_t *sip_message,char **dest);
//headers
int sip_message_headers_to_str(sip_message_t *sip_message,char **dest);
//body
int sip_message_body_to_str(sip_message_t *sip_message,char **dest);
//sip message
int sip_message_to_str(sip_message_t *sip_message);


#ifdef __cplusplus
}
#endif
#endif

