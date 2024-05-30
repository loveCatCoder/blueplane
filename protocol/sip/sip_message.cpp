/*********************************************************************************************
*
*
*
*
*
*********************************************************************************************/

#include "sip_std.h"
#include "sip_common.h"
#include "sip_message.h"
//#include "memwatch.h"
//add  20121107
int sip_message_uri_init(sip_uri_t **uri)
{
	*uri = (sip_uri_t *)sip_malloc(sizeof(sip_uri_t));
	if(*uri == NULL)
		return SIP_NOMEM;
	return SIP_SUCCESS;
}
int sip_message_uri_free(sip_uri_t *uri)
{
	if(uri == NULL)
		return SIP_BADPARAMETER;
	sip_free(uri->host);
	uri->host = NULL;
	sip_free(uri->client_id);
	uri->client_id = NULL;
	free(uri);
	
	return SIP_SUCCESS;
}

//add end

int sip_message_to_init( sip_to_t **to )
{
	*to = ( sip_to_t *)sip_malloc( sizeof( sip_to_t ) ) ;
	if( *to == NULL )
		return SIP_NOMEM ;
	return SIP_SUCCESS ;
}

int sip_message_to_free( sip_to_t *to )
{
	if( to  == NULL )
		return SIP_BADPARAMETER ;

	sip_free( to->url ) ;
	to->url = NULL;
	sip_free( to->name ) ;
	to->name = NULL;
	sip_free( to->tag ) ;
	to->tag = NULL;
	free( to ) ;
	return SIP_SUCCESS ;
}

int sip_message_from_init( sip_from_t **from )
{
	return sip_message_to_init( (sip_to_t**)from ) ;
}

int sip_message_from_free( sip_from_t *from )
{
	return sip_message_to_free( (sip_to_t*)from );
}

int sip_message_contact_init( sip_contact_t **contact )
{
	return sip_message_to_init( (sip_to_t**)contact ) ;
}

int sip_message_contact_free( sip_contact_t *contact )
{
	return sip_message_to_free( (sip_to_t*)contact );
}

//20121022
int sip_message_via_init( sip_via_t **via )
{
	*via = ( sip_via_t *)sip_malloc( sizeof( sip_via_t ) ) ;
	if( *via == NULL )
		return SIP_NOMEM ;
	(*via)->next = NULL;
	return SIP_SUCCESS ;
}
int sip_message_via_free( sip_via_t *via )
{
	if( via  == NULL )
		return SIP_BADPARAMETER ;
	sip_free( via->ip_addr) ;
	via->ip_addr = NULL;
	sip_free( via->branch);
	via->branch = NULL;
		
	free( via ) ;
	return SIP_SUCCESS ;
}
//end
int sip_message_vias_free(sip_via_t *vias)
{
	sip_via_t *via_list,*next_via;
	if( vias  == NULL )
		return SIP_BADPARAMETER ;	
	via_list = vias;
	next_via = vias->next;
	while( next_via != NULL)
	{
		sip_message_via_free(via_list);
		
		via_list = next_via;
		next_via = next_via->next;
	}
	sip_message_via_free(via_list);

	return SIP_SUCCESS;
}
//20121022
int sip_message_cseq_init( sip_cseq_t **cseq )
{
	*cseq= ( sip_cseq_t *)sip_malloc( sizeof( sip_cseq_t ) ) ;
	if( *cseq == NULL )
		return SIP_NOMEM ;
	return SIP_SUCCESS ;
}

int sip_message_cseq_free( sip_cseq_t *cseq )
{
	if( cseq  == NULL )
		return SIP_BADPARAMETER ;
	
	sip_free( cseq->method) ;
	cseq->method = NULL;
	sip_free( cseq->number) ;
	cseq->number = NULL;
	free( cseq ) ;
	return SIP_SUCCESS ;
}
//end

//20121022
int sip_message_content_type_init( sip_content_type_t **content_type )
{
	*content_type= ( sip_content_type_t *)sip_malloc( sizeof( sip_content_type_t ) ) ;
	if( *content_type == NULL )
		return SIP_NOMEM ;
	return SIP_SUCCESS ;
}

int sip_message_content_type_free( sip_content_type_t *content_type )
{
	if( content_type  == NULL )
		return SIP_BADPARAMETER ;
	sip_free( content_type->type) ;
	content_type->type = NULL;
	sip_free( content_type->sub_type) ;
	content_type->sub_type = NULL;
	sip_free( content_type->other) ;
	content_type->other = NULL;
	free( content_type ) ;
	
	return SIP_SUCCESS ;
}
//end

//20121022
int sip_message_callid_init( sip_callid_t **callid )
{
	*callid= ( sip_callid_t *)sip_malloc( sizeof( sip_callid_t ) ) ;
	if( *callid == NULL )
		return SIP_NOMEM ;
	return SIP_SUCCESS ;
}

int sip_message_callid_free( sip_callid_t *callid )
{
	if( callid  == NULL )
		return SIP_BADPARAMETER ;
	
	sip_free( callid->number) ;
	callid->number = NULL;
	sip_free( callid->host) ;
	callid->host = NULL;
	
	free( callid ) ;
	return SIP_SUCCESS ;
}
//end

int sip_message_subject_init( sip_subject_t **subject )
{
	*subject= ( sip_subject_t *)sip_malloc( sizeof( sip_subject_t ) ) ;
	if( *subject == NULL )
		return SIP_NOMEM ;
	return SIP_SUCCESS ;
}

int sip_message_subject_free( sip_subject_t *subject )
{
	if( subject  == NULL )
		return SIP_BADPARAMETER ;
	
	sip_free( subject->sender_id ) ;
	subject->sender_id = NULL ;
	sip_free( subject->sender_seq ) ;
	subject->sender_seq = NULL ;
	sip_free( subject->recver_id ) ;
	subject->recver_id = NULL ;
	sip_free( subject->recver_seq ) ;
	subject->recver_seq = NULL ;
	
	free( subject ) ;
	return SIP_SUCCESS ;
}



int sip_message_www_authenticate_init( sip_www_authenticate_t **www_authenticate )
{
	*www_authenticate = ( sip_www_authenticate_t *)sip_malloc( sizeof( sip_www_authenticate_t ) ) ;
	if( *www_authenticate == NULL )
		return SIP_NOMEM ;
	return SIP_SUCCESS ;
}

int sip_message_www_authenticate_free( sip_www_authenticate_t *www_authenticate )
{
	if( www_authenticate  == NULL )
		return SIP_BADPARAMETER ;

	sip_free( www_authenticate->algorithm) ;
	www_authenticate->algorithm = NULL;
	sip_free( www_authenticate->auth_type);
	www_authenticate->auth_type = NULL;
	sip_free( www_authenticate->domain);
	www_authenticate->domain = NULL;
	sip_free( www_authenticate->nonce);
	www_authenticate->nonce = NULL;
	sip_free( www_authenticate->opaque);
	www_authenticate->opaque = NULL;
	sip_free( www_authenticate->qop);
	www_authenticate->qop = NULL;
	sip_free( www_authenticate->realm);
	www_authenticate->realm = NULL;
	sip_free( www_authenticate->stale);
	www_authenticate->stale = NULL;
	
	free( www_authenticate  ) ;
	return SIP_SUCCESS ;
}

int sip_message_authorization_init( sip_authorization_t **authorization )
{
	*authorization = ( sip_authorization_t *)sip_malloc( sizeof( sip_authorization_t ) ) ;
	if( *authorization == NULL )
		return SIP_NOMEM ;
	return SIP_SUCCESS ;
}
int sip_message_authorization_free( sip_authorization_t *authorization )
{
	if( authorization  == NULL )
		return SIP_BADPARAMETER ;

	sip_free( authorization->algorithm);
	authorization->algorithm = NULL;
	sip_free( authorization->cnonce);
	authorization->cnonce = NULL;
	sip_free( authorization->nonce);
	authorization->nonce = NULL;
	sip_free( authorization->nonce_count);
	authorization->nonce_count = NULL;
	sip_free( authorization->opaque);
	authorization->opaque = NULL;
	sip_free( authorization->qop);
	authorization->qop = NULL;
	sip_free( authorization->realm);
	authorization->realm = NULL;
	sip_free( authorization->response);
	authorization->response = NULL;
	sip_free( authorization->uri);
	authorization->uri = NULL;
	sip_free( authorization->username ) ;
	authorization->username = NULL;
	sip_free( authorization->auth_type);
	authorization->auth_type = NULL;
	free( authorization  );
	return SIP_SUCCESS ;
}

int sip_message_init( sip_message_t **sip_message )
{
	if( sip_message == NULL )
		return SIP_NOMEM ;
	*sip_message = (sip_message_t *)sip_malloc( sizeof(sip_message_t) ) ;
	if(NULL == *sip_message)
		return SIP_NOMEM ;
	memset(*sip_message,0,sizeof(sip_message_t));
	//(*sip_message)->message = ( char * )sip_malloc(  SIP_BODY_SIZE ) ;
	//if( (*sip_message)->message == NULL )
	//{
	//	sip_free( *sip_message ) ;
	//	return SIP_NOMEM ;
	//}
	//memset((*sip_message)->message, 0,SIP_BODY_SIZE);
	return SIP_SUCCESS ;
}



int sip_message_destory( sip_message_t *sip_message )
{
	sip_message_to_free(sip_message->to);
	sip_message->to = NULL;
	sip_message_from_free( sip_message->from);
	sip_message->from = NULL;
	sip_message_contact_free( sip_message->contact);
	sip_message->contact = NULL;
	sip_message_vias_free( sip_message->via);
	sip_message->via = NULL;
	sip_message_cseq_free( sip_message->cseq);
	sip_message->cseq = NULL;
	sip_message_content_type_free( sip_message->content_type);
	sip_message->content_type = NULL;
	sip_message_callid_free( sip_message->callid);
	sip_message->callid = NULL;
	sip_message_www_authenticate_free( sip_message->www_authenticate);
	sip_message->www_authenticate = NULL;
	sip_message_authorization_free( sip_message->authorization);
	sip_message->authorization = NULL;
	sip_message_subject_free( sip_message->subject);
	sip_message->subject = NULL;

	sip_free(sip_message->max_forwards);
	sip_message->max_forwards = NULL;
	sip_free(sip_message->content_length);
	sip_message->content_length = NULL;
	sip_free(sip_message->expires);
	sip_message->expires = NULL;	
	sip_free(sip_message->date);
	sip_message->date = NULL;
	sip_free(sip_message->reason_phase);
	sip_message->reason_phase = NULL;
	sip_free(sip_message->status_code);
	sip_message->status_code = NULL;
	sip_free(sip_message->req_uri);
	sip_message->req_uri = NULL;
	sip_free(sip_message->sip_method);
	sip_message->sip_method = NULL;
	sip_free(sip_message->sip_version);
	sip_message->sip_version = NULL;
	sip_free(sip_message->user_agent);
	sip_message->user_agent = NULL;
	sip_free(sip_message->event);
	sip_message->event = NULL;
	sip_free(sip_message->subscription_state);
	sip_message->subscription_state = NULL;
	
	sip_free(sip_message->sip_body);
	sip_message->sip_body = NULL;
	
	sip_free( sip_message->message );
	sip_message->message = NULL;
	
	sip_free( sip_message ) ;
	//sip_message = NULL;
	return SIP_SUCCESS ;
}


