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
#include "sip_message_clone.h"
//sizeof
//to function
int sip_message_clone_to( sip_to_t *to , sip_to_t **dest_to ) 
{
	if(to == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(to->url == NULL)
	{
		return SIP_BADPARAMETER;
	}
	
	if(SIP_SUCCESS != sip_message_to_init(dest_to))
	{
		return SIP_NOMEM;
	}
	//url
	(*dest_to)->url = (char *)sip_malloc(strlen(to->url )+1);
	if((*dest_to)->url  == NULL)
	{
		return SIP_NOMEM;
	}
	sip_str_cpy((*dest_to)->url ,strlen(to->url )+1,to->url ,strlen(to->url));
	//name
	if(to->name != NULL)
	{
		(*dest_to)->name = (char *)sip_malloc(strlen(to->name)+1);
		if((*dest_to)->name == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*dest_to)->name,strlen(to->name)+1,to->name,strlen(to->name));
	}
	//tag
	if(to->tag != NULL)
	{
		(*dest_to)->tag = (char *)sip_malloc(strlen(to->tag )+1);
		if((*dest_to)->tag  == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*dest_to)->tag,strlen(to->tag )+1,to->tag ,strlen(to->tag));
	}

	return SIP_SUCCESS;
}

//from function
int sip_message_clone_from( sip_from_t *from , sip_from_t **dest_from ) 
{
	return sip_message_clone_to( (sip_to_t *)from , (sip_to_t **)dest_from ) ;
}

//contact function
int sip_message_clone_contact( sip_contact_t *contact , sip_contact_t **dest_contact ) 
{
	return sip_message_clone_to( (sip_to_t *)contact , (sip_to_t **)dest_contact) ;
}


//via funciton
int sip_message_clone_via( sip_via_t *via , sip_via_t **dest_via ) 
{
	if(via == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if( via->ip_addr == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(SIP_SUCCESS != sip_message_via_init(dest_via))
	{
		return SIP_NOMEM;
	}
	//net_type
	(*dest_via)->net_type = via->net_type;
	//ip_addr
	(*dest_via)->ip_addr = (char *)sip_malloc(strlen(via->ip_addr)+1);
	if((*dest_via)->ip_addr == NULL)
	{
		return SIP_NOMEM;
	}
	sip_str_cpy((*dest_via)->ip_addr,strlen(via->ip_addr)+1,via->ip_addr,strlen(via->ip_addr));
	
	//branch
	if(via->branch != NULL)
	{
		(*dest_via)->branch = (char *)sip_malloc(strlen(via->branch)+1);
		if((*dest_via)->branch == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*dest_via)->branch,strlen(via->branch)+1,via->branch,strlen(via->branch));
	}
	//port
	(*dest_via)->port = via->port;
	(*dest_via)->rport = via->rport;
	return SIP_SUCCESS;
}
int sip_message_clone_vias(sip_via_t *vias , sip_via_t **dest_vias )
{
	sip_via_t *via_list,*tmp_via,*dest_list;
	if(vias == NULL || vias->ip_addr == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(SIP_SUCCESS != sip_message_clone_via(vias,dest_vias))
	{
		return SIP_NOMEM;
	}
	via_list = vias->next;
	dest_list = *dest_vias;
	while(via_list != NULL)
	{
		sip_message_clone_via(via_list,&tmp_via);
		
		dest_list->next = tmp_via;
		dest_list = dest_list->next;
		
		via_list = via_list->next;
		tmp_via = tmp_via->next;
	}
	
	return SIP_SUCCESS;
}

//cseq function
int sip_message_clone_cseq( sip_cseq_t *cseq , sip_cseq_t **dest_cseq ) 
{
	if(cseq == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(cseq->method == NULL || cseq->number == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(SIP_SUCCESS != sip_message_cseq_init(dest_cseq))
	{
		return SIP_NOMEM;
	}
	//method
	(*dest_cseq)->method = (char*)sip_malloc(strlen(cseq->method)+1);
	if((*dest_cseq)->method == NULL)
	{
		return SIP_NOMEM;
	}
	sip_str_cpy((*dest_cseq)->method,strlen(cseq->method)+1,cseq->method,strlen(cseq->method));
	//number
	(*dest_cseq)->number = (char*)sip_malloc(strlen(cseq->number)+1);
	if((*dest_cseq)->number == NULL)
	{
		return SIP_NOMEM;
	}
	sip_str_cpy((*dest_cseq)->number,strlen(cseq->number)+1,cseq->number,strlen(cseq->number));

	return SIP_SUCCESS;
}


//content length funciton
int sip_message_clone_content_length( char *content_length , char **dest_content_length ) 
{
	if(content_length == NULL)
	{
		return SIP_BADPARAMETER;
	}
	*dest_content_length = (char *)sip_malloc(strlen(content_length)+1);
	if(*dest_content_length == NULL)
	{
		return SIP_NOMEM;
	}
	sip_str_cpy(*dest_content_length,strlen(content_length)+1,content_length,strlen(content_length));
	return SIP_SUCCESS;
}

//content type function
int sip_message_clone_content_type( sip_content_type_t *content_type , sip_content_type_t **dest_content_type )
{
	if(content_type == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(content_type->type == NULL || content_type->sub_type == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(SIP_SUCCESS != sip_message_content_type_init(dest_content_type))
	{
		return SIP_NOMEM;
	}
	//type
	(*dest_content_type)->type = (char *)sip_malloc(strlen(content_type->type)+1);
	if((*dest_content_type)->type == NULL)
	{
		return SIP_NOMEM;
	}
	sip_str_cpy((*dest_content_type)->type,strlen(content_type->type)+1,content_type->type,strlen(content_type->type));

	//sub_type
	(*dest_content_type)->sub_type = (char *)sip_malloc(strlen(content_type->sub_type)+1);
	if((*dest_content_type)->sub_type == NULL)
	{
		return SIP_NOMEM;
	}
	sip_str_cpy((*dest_content_type)->sub_type,strlen(content_type->sub_type)+1,content_type->sub_type,strlen(content_type->sub_type));

	//others
	if(content_type->other != NULL)
	{
		
		(*dest_content_type)->other = (char *)sip_malloc(strlen(content_type->other)+1);
		if((*dest_content_type)->other == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*dest_content_type)->other,strlen(content_type->other)+1,content_type->other,strlen(content_type->other));
	}
	return SIP_SUCCESS;
}

//callid function
int sip_message_clone_callid( sip_callid_t *sip_callid , sip_callid_t **dest_sip_callid ) 
{
	if(sip_callid == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if( sip_callid->number == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(SIP_SUCCESS != sip_message_callid_init(dest_sip_callid))
	{
		return SIP_NOMEM;
	}
	//host
	if(sip_callid->host != NULL)
	{
		(*dest_sip_callid)->host = (char *)sip_malloc(strlen(sip_callid->host)+1);
		if((*dest_sip_callid)->host == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*dest_sip_callid)->host,strlen(sip_callid->host)+1,sip_callid->host,strlen(sip_callid->host));
	}
	//number
	(*dest_sip_callid)->number = (char *)sip_malloc(strlen(sip_callid->number)+1);
	if((*dest_sip_callid)->number == NULL)
	{
		return SIP_NOMEM;
	}
	sip_str_cpy((*dest_sip_callid)->number,strlen(sip_callid->number)+1,sip_callid->number,strlen(sip_callid->number));
	
	return SIP_SUCCESS;
}


//max forwards function
int sip_message_clone_max_forwards(  char *max_forwards , char **dest_max_forwards  ) 
{
	if(max_forwards == NULL)
	{
		return SIP_BADPARAMETER;
	}
	*dest_max_forwards = (char *)sip_malloc(strlen(max_forwards)+1);
	if(*dest_max_forwards == NULL)
	{
		return SIP_NOMEM;
	}
	sip_str_cpy(*dest_max_forwards,strlen(max_forwards)+1,max_forwards,strlen(max_forwards));
	return SIP_SUCCESS;
}

//expires function
int sip_message_clone_expires(  char *expires  , char **dest_expires  ) 
{
	if(expires == NULL)
	{
		return SIP_BADPARAMETER;
	}
	*dest_expires = (char *)sip_malloc(strlen(expires)+1);
	if(*dest_expires == NULL)
	{
		return SIP_NOMEM;
	}
	sip_str_cpy(*dest_expires,strlen(expires)+1,expires,strlen(expires));
	
	return SIP_SUCCESS;
}

//date function
int sip_message_clone_date(  char *date, char **dest_date  ) 
{
	if(date == NULL)
	{
		return SIP_BADPARAMETER;
	}
	*dest_date= (char *)sip_malloc(strlen(date)+1);
	if((*dest_date) == NULL)
	{
		return SIP_NOMEM;
	}
	sip_str_cpy(*dest_date,strlen(date)+1,date,strlen(date));
	
	return SIP_SUCCESS;
}

//www_authenticate function
int sip_message_clone_www_authenticate(  sip_www_authenticate_t *www_authenticate , sip_www_authenticate_t **dest_www_authenticate ) 
{
	if(www_authenticate == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(www_authenticate->auth_type == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(SIP_SUCCESS != sip_message_www_authenticate_init(dest_www_authenticate))
	{
		return SIP_NOMEM;
	}
	//auth_type
	(*dest_www_authenticate)->auth_type = (char *)sip_malloc(strlen(www_authenticate->auth_type)+1);
	if((*dest_www_authenticate)->auth_type == NULL)
	{
		return SIP_NOMEM;
	}
	sip_str_cpy((*dest_www_authenticate)->auth_type,strlen(www_authenticate->auth_type)+1,www_authenticate->auth_type,strlen(www_authenticate->auth_type));

	//realm
	if(www_authenticate->realm != NULL)
	{
		(*dest_www_authenticate)->realm = (char *)sip_malloc(strlen(www_authenticate->realm)+1);
		if((*dest_www_authenticate)->realm == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*dest_www_authenticate)->realm,strlen(www_authenticate->realm)+1,www_authenticate->realm,strlen(www_authenticate->realm));
	}
	//domain
	if(www_authenticate->domain!= NULL)
	{
		(*dest_www_authenticate)->domain = (char *)sip_malloc(strlen(www_authenticate->domain)+1);
		if((*dest_www_authenticate)->domain == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*dest_www_authenticate)->domain,strlen(www_authenticate->domain)+1,www_authenticate->domain,strlen(www_authenticate->domain));
	}
	//nonce
	if(www_authenticate->nonce!= NULL)
	{
		(*dest_www_authenticate)->nonce = (char *)sip_malloc(strlen(www_authenticate->nonce)+1);
		if((*dest_www_authenticate)->nonce == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*dest_www_authenticate)->nonce,strlen(www_authenticate->nonce)+1,www_authenticate->nonce,strlen(www_authenticate->nonce));
	}
	//nonce
	if(www_authenticate->opaque != NULL)
	{
		(*dest_www_authenticate)->opaque = (char *)sip_malloc(strlen(www_authenticate->opaque)+1);
		if((*dest_www_authenticate)->opaque == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*dest_www_authenticate)->opaque,strlen(www_authenticate->opaque)+1,www_authenticate->opaque,strlen(www_authenticate->opaque));
	}
	//stale
	if(www_authenticate->stale!= NULL)
	{
		(*dest_www_authenticate)->stale = (char *)sip_malloc(strlen(www_authenticate->stale)+1);
		if((*dest_www_authenticate)->stale == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*dest_www_authenticate)->stale,strlen(www_authenticate->stale)+1,www_authenticate->stale,strlen(www_authenticate->stale));
	}
	//algorithm
	if(www_authenticate->algorithm != NULL)
	{
		(*dest_www_authenticate)->algorithm = (char *)sip_malloc(strlen(www_authenticate->algorithm)+1);
		if((*dest_www_authenticate)->algorithm == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*dest_www_authenticate)->algorithm,strlen(www_authenticate->algorithm)+1,www_authenticate->algorithm,strlen(www_authenticate->algorithm));
	}
	//qop
	if(www_authenticate->qop!= NULL)
	{
		(*dest_www_authenticate)->qop = (char *)sip_malloc(strlen(www_authenticate->qop)+1);
		if((*dest_www_authenticate)->qop == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*dest_www_authenticate)->qop,strlen(www_authenticate->qop)+1,www_authenticate->qop,strlen(www_authenticate->qop));
	}
	return SIP_SUCCESS;
}

int sip_message_clone_authorization(  sip_authorization_t *authorization , sip_authorization_t **dest_authorization ) 
{
	if(authorization == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(authorization->auth_type == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(SIP_SUCCESS != sip_message_authorization_init(dest_authorization))
	{
		return SIP_NOMEM;
	}
	//auth_type
	(*dest_authorization)->auth_type = (char *)sip_malloc(strlen(authorization->auth_type)+1);
	if((*dest_authorization)->auth_type == NULL)
	{
		return SIP_NOMEM;
	}
	sip_str_cpy((*dest_authorization)->auth_type,strlen(authorization->auth_type)+1,authorization->auth_type,strlen(authorization->auth_type));

	//username
	if(authorization->username != NULL)
	{
		(*dest_authorization)->username = (char *)sip_malloc(strlen(authorization->username)+1);
		if((*dest_authorization)->username == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*dest_authorization)->username,strlen(authorization->username)+1,authorization->username,strlen(authorization->username));
	}
	//realm
	if(authorization->realm != NULL)
	{
		(*dest_authorization)->realm = (char *)sip_malloc(strlen(authorization->realm)+1);
		if((*dest_authorization)->realm == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*dest_authorization)->realm,strlen(authorization->realm)+1,authorization->realm,strlen(authorization->realm));
	}
	//nonce
	if(authorization->nonce != NULL)
	{
		(*dest_authorization)->nonce = (char *)sip_malloc(strlen(authorization->nonce)+1);
		if((*dest_authorization)->nonce == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*dest_authorization)->nonce,strlen(authorization->nonce)+1,authorization->nonce,strlen(authorization->nonce));
	}
	//uri
	if(authorization->uri != NULL)
	{
		(*dest_authorization)->uri = (char *)sip_malloc(strlen(authorization->uri)+1);
		if((*dest_authorization)->uri == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*dest_authorization)->uri,strlen(authorization->uri)+1,authorization->uri,strlen(authorization->uri));
	}
	//response
	if(authorization->response!= NULL)
	{
		(*dest_authorization)->response = (char *)sip_malloc(strlen(authorization->response)+1);
		if((*dest_authorization)->response == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*dest_authorization)->response,strlen(authorization->response)+1,authorization->response,strlen(authorization->response));
	}
	//cnonce
	if(authorization->cnonce!= NULL)
	{
		(*dest_authorization)->cnonce = (char *)sip_malloc(strlen(authorization->cnonce)+1);
		if((*dest_authorization)->cnonce == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*dest_authorization)->cnonce,strlen(authorization->cnonce)+1,authorization->cnonce,strlen(authorization->cnonce));
	}
	//algorithm
	if(authorization->algorithm != NULL)
	{
		(*dest_authorization)->algorithm = (char *)sip_malloc(strlen(authorization->algorithm)+1);
		if((*dest_authorization)->algorithm == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*dest_authorization)->algorithm,strlen(authorization->algorithm)+1,authorization->algorithm,strlen(authorization->algorithm));
	}
	//opaque
	if(authorization->opaque != NULL)
	{
		(*dest_authorization)->opaque = (char *)sip_malloc(strlen(authorization->opaque)+1);
		if((*dest_authorization)->opaque == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*dest_authorization)->opaque,strlen(authorization->opaque)+1,authorization->opaque,strlen(authorization->opaque));
	}
	//qop
	if(authorization->qop != NULL)
	{
		(*dest_authorization)->qop = (char *)sip_malloc(strlen(authorization->qop)+1);
		if((*dest_authorization)->qop == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*dest_authorization)->qop,strlen(authorization->qop)+1,authorization->qop,strlen(authorization->qop));
	}
	//nonce_count
	if(authorization->nonce_count!= NULL)
	{
		(*dest_authorization)->nonce_count = (char *)sip_malloc(strlen(authorization->nonce_count)+1);
		if((*dest_authorization)->nonce_count == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*dest_authorization)->nonce_count,strlen(authorization->nonce_count)+1,authorization->nonce_count,strlen(authorization->nonce_count));
	}
	return SIP_SUCCESS;
}

//sip message
/*
int sip_message_clone(sip_message_t *sip_message,sip_message_t **dest_sip_message)
{
	if(sip_message == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(SIP_SUCCESS != sip_message_init(dest_sip_message))
	{
		return SIP_NOMEM;
	}
	
}
*/

