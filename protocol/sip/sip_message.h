/*********************************************************************************************
*
*
*
*
*
*********************************************************************************************/

#ifndef __SIP_MESSAGE_H__
#define __SIP_MESSAGE_H__


#ifdef __cplusplus
extern "C" {
#endif



#define SIP_BODY_SIZE	(2048)
#define SIP_HEADERS_SIZE (1024)

#define TYPE_UDP 1
#define TYPE_TCP 2


#define MESSAGE_PU_CREATE   1

typedef struct _tag_sip_via{
	int net_type ;		//udp or tcp
	char *ip_addr ;		//where do we send to 
	int port ;			//and which port
	int rport ;
	char *branch ;	
	struct _tag_sip_via  *next;
}sip_via_t ;


typedef struct _tag_sip_callid{
	char *number ;
	char *host ;		//maybe nouse
}sip_callid_t ;

//add gyl 20121107
typedef struct _tag_sip_uri{
	char *client_id;
	char *host;
}sip_uri_t ;
//add end
typedef struct _tag_sip_from{
	char *name ;
	char *url ;
	char *tag ;
}sip_from_t ;

typedef sip_from_t sip_to_t ;
typedef sip_from_t sip_contact_t ;

typedef struct _tag_sip_cseq{
	char *method ;
	char *number ;
}sip_cseq_t ;

typedef struct _tag_content_type{
	char *type ;
	char *sub_type ;
	char *other ;		//maybe nouse
}sip_content_type_t ;

typedef struct _tag_authorization{
	char *auth_type;
	char *username;
	char *realm;
	char *nonce;
	char *uri;
	char *response;
	char *cnonce;
	char *algorithm;
	char *opaque;
	char *qop;
	char *nonce_count;
}sip_authorization_t;

typedef struct _tag_www_authenticate{
	char *auth_type;
	char *realm;
	char *domain;
	char *nonce;
	char *opaque;
    char *stale;
	char *algorithm;
	char *qop;
}sip_www_authenticate_t;

//add 20121109
typedef struct _tag_time{
	int sec;
	int min;
	int hour;
	int day;
	int mon;
	int year;
}sip_time_t;
//add end

typedef struct _tag_subject{
	char *sender_id ;
	char *sender_seq ;
	char *recver_id ;
	char *recver_seq ;
}sip_subject_t ;

//sip message body
typedef struct _tag_sip_message{
	char *sip_version ;					//sip version  "SIP/2.0"
	char *sip_method ;					//method
	char *req_uri;          
	
	char *status_code;                  //result code     //add  gyl 20121024
	char *reason_phase;
	
	sip_via_t			*via ;			//header via 
	sip_from_t			*from ;			
	sip_to_t			*to ;			
	sip_contact_t		*contact ;
	sip_content_type_t	*content_type ;
	sip_callid_t		*callid ;
	sip_cseq_t			*cseq ;
	sip_authorization_t *authorization;
	sip_www_authenticate_t *www_authenticate;
	sip_subject_t 		*subject ;

	char *max_forwards ;
	char *content_length ;
	char *expires;
	char *date;
	char *user_agent;
	char *event;
	char *subscription_state;	
	
	char	*sip_body ;					//xml or sdp
	char	*message  ;					//store string message

    int     origin ;
}sip_message_t ;
//add 20121107
int sip_message_uri_init(sip_uri_t **uri);
int sip_message_uri_free(sip_uri_t *uri);
//add end
int sip_message_to_init( sip_to_t **to ) ;
int sip_message_to_free( sip_to_t *to ) ;

int sip_message_from_init( sip_from_t **from ) ;
int sip_message_from_free( sip_from_t *from ) ;

int sip_message_contact_init( sip_contact_t **contact ) ;
int sip_message_contact_free( sip_contact_t *contact ) ;

int sip_message_via_init( sip_via_t **via );
int sip_message_via_free( sip_via_t *via );
int sip_message_vias_free(sip_via_t *vias);

int sip_message_cseq_init( sip_cseq_t **cseq );
int sip_message_cseq_free( sip_cseq_t *cseq );

int sip_message_content_type_init( sip_content_type_t **content_type );
int sip_message_content_type_free( sip_content_type_t *content_type );

int sip_message_callid_init( sip_callid_t **callid );
int sip_message_callid_free( sip_callid_t *callid );

int sip_message_www_authenticate_init( sip_www_authenticate_t **www_authenticate );
int sip_message_www_authenticate_free( sip_www_authenticate_t *www_authenticate );

int sip_message_authorization_init( sip_authorization_t **authorization );
int sip_message_authorization_free( sip_authorization_t *authorization );

int sip_message_subject_init( sip_subject_t **subject ) ;
int sip_message_subject_free( sip_subject_t *subject ) ;


int sip_message_init( sip_message_t **sip_message ) ;

int sip_message_destory( sip_message_t *sip_message ) ;

#ifdef __cplusplus
}
#endif


#endif


