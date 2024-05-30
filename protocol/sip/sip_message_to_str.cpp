/*********************************************************************************************
*
*
*
*
*
*********************************************************************************************/
#include "sip_std.h"
#include "sip_message_to_str.h"
#include "sip_common.h"
//to function
/*
--> "philip" <sip:philip@sinonet.net.cn>;tag=1234-5678
--><sip:philip@sinonet.net.cn>;tag=1234-5678
*/
int sip_message_to_to_str( sip_to_t *to , char **dest )
{
	char *tmp;
	int len;
	if(to == NULL || to->url == NULL)
	{
		return SIP_BADPARAMETER;
	}
	len = strlen(to->url) + 2;
	if(to->name != NULL)
		len += strlen(to->name) + 3;
	if(to->tag != NULL)
		len += strlen(to->tag) + 5;
	
	*dest = (char *)sip_malloc(len+1);
	if(*dest == NULL)
	{
		return SIP_NOMEM;
	}
	tmp = *dest;
	
	if(to->name != NULL)
	{
		sip_str_append(&tmp,"\"");
		sip_str_append(&tmp,to->name);
		sip_str_append(&tmp,"\" ");
	}
	
	sip_str_append(&tmp,"<");
	sip_str_append(&tmp,to->url);
	sip_str_append(&tmp,">");

	if(to->tag != NULL)
	{	
		sip_str_append(&tmp,";tag=");
		sip_str_append(&tmp,to->tag);
	}

	return SIP_SUCCESS;
}

//from function
int sip_message_from_to_str( sip_from_t *from , char **dest )
{
    return sip_message_to_to_str((sip_to_t *) from, dest);
}

//contact function
int sip_message_contact_to_str(sip_contact_t *contact , char **dest )
{
	return sip_message_to_to_str((sip_to_t *) contact, dest);
}


//via funciton
/*Via: SIP/2.0/UDP 10.6.10.101:5060;branch=z9hG4bK791-13184-10901*/
int sip_message_via_to_str( sip_via_t *via , char **dest )
{
	char *tmp;
	char port[6];
	int len;
	if(via == NULL || via->ip_addr == NULL )
	{
		return SIP_BADPARAMETER;
	}
	sprintf(port,"%d",via->port);
	len = strlen(via->ip_addr)+strlen(port)+strlen("SIP/2.0/")+50;
	if(via->branch != NULL)
		len += strlen(via->branch)+8;
	*dest = (char *)sip_malloc(len+1);
	if(*dest == NULL)
	{
		return SIP_NOMEM;
	}
	tmp = *dest;
	sip_str_append(&tmp,"SIP/2.0/");
    if( via->net_type == TYPE_TCP )
	    sip_str_append( &tmp , "TCP" );
	else
        sip_str_append( &tmp , "UDP" );

	sip_str_append(&tmp," ");

	sip_str_append(&tmp,via->ip_addr);
	sip_str_append(&tmp,":");
	sip_str_append(&tmp,port);

	sip_str_append(&tmp,";rport" );
	if( via->rport != 0 )
	{
		sprintf(port,"%d",via->rport);
		sip_str_append(&tmp,port);
	}
	//sip_str_append(&tmp,";");
	if(via->branch != NULL)
	{
		sip_str_append(&tmp,";branch=");
		sip_str_append(&tmp,via->branch);
	}
	return SIP_SUCCESS;
	
}
int sip_message_vias_to_str(sip_via_t *vias , char **dest)
{
	sip_via_t *via_list;
	char *tmp,*tmp_via,port[6];
	int len = 0;
	if(vias == NULL)
	{
		return SIP_BADPARAMETER;
	}
	via_list = vias;
	while(via_list != NULL)
	{
		if( via_list->ip_addr == NULL )
		{
			return SIP_BADPARAMETER;
		}
		sprintf(port,"%d",via_list->port);
		len += strlen("Via: ")+strlen("SIP/2.0/")+3+strlen(via_list->ip_addr)+strlen(port)+2+2;
		if(via_list->branch != NULL)
			len += strlen(via_list->branch)+8;
		via_list = via_list->next;
	}
	*dest = (char *)sip_malloc(len+1);
	if(*dest == NULL)
	{
		return SIP_NOMEM;
	}
	tmp = *dest;
	via_list = vias;
	while(via_list != NULL)
	{	
		if(SIP_SUCCESS != sip_message_via_to_str(via_list,&tmp_via))
		{
			sip_free(*dest);
			*dest = NULL;
			return SIP_NOMEM;
		}
		sip_str_append(&tmp,"Via: ");
		sip_str_append(&tmp,tmp_via);
		sip_free(tmp_via);
		sip_str_append(&tmp,CRLF);
		via_list = via_list->next;
	}
	return SIP_SUCCESS;
}


//cseq function
/*1 REGISTER*/
int sip_message_cseq_to_str( sip_cseq_t *cseq , char **dest )
{
 	char *tmp;
	int len;
	if(cseq == NULL || cseq->method == NULL || cseq->number == NULL)
	{
		return SIP_BADPARAMETER;
	}
	len = strlen(cseq->number)+strlen(cseq->method)+1;
	*dest = (char *)sip_malloc(len+1);
	if(*dest == NULL)
	{
		return SIP_NOMEM;
	}
	tmp = *dest;

	sip_str_append(&tmp,cseq->number);
	sip_str_append(&tmp," ");
	sip_str_append(&tmp,cseq->method);

	return SIP_SUCCESS;
}


//content length funciton
int sip_message_content_length_to_str( char *content_length, char **dest )
{
	char *tmp;
	int len;

	if(content_length == NULL)
	{
		return SIP_BADPARAMETER;
	}
	len = strlen(content_length);
	*dest = (char *)sip_malloc(len+1);
	if(*dest == NULL)
	{
		return SIP_NOMEM;
	}
	tmp = *dest;
	sip_str_append(&tmp,content_length);

	return SIP_SUCCESS;
}

//content type function
/*application/sdp*/
int sip_message_content_type_to_str( sip_content_type_t *content_type, char **dest )
{
	char *tmp;
	int len;

	if(content_type == NULL || content_type->type == NULL || content_type->sub_type == NULL)
	{
		return SIP_BADPARAMETER;
	}
	len = strlen(content_type->type)+strlen(content_type->sub_type)+1;
	if(content_type->other != NULL)
	{
		len += strlen(content_type->other)+1;
	}

	*dest = (char *)sip_malloc(len+1);
	if(*dest == NULL)
	{
		return SIP_NOMEM;
	}
	tmp = *dest;
	sip_str_append(&tmp,content_type->type);
	sip_str_append(&tmp,"/");
	sip_str_append(&tmp,content_type->sub_type);

	if(content_type->other != NULL)
	{
		sip_str_append(&tmp,";");
		sip_str_append(&tmp,content_type->other);
	}

	return SIP_SUCCESS;
}

//callid function
int sip_message_callid_to_str( sip_callid_t *callid, char **dest )
{
	char *tmp;
	int len;

	if(callid == NULL || callid->number == NULL)
	{
		return SIP_BADPARAMETER;
	}
	len = strlen(callid->number);
	if(callid->host != NULL)
		len += strlen(callid->host)+1;
	*dest = (char *)sip_malloc(len+1);
	if(*dest == NULL)
	{
		return SIP_NOMEM;
	}
	tmp = *dest;
	sip_str_append(&tmp,callid->number);
	if(callid->host != NULL)
	{
		sip_str_append(&tmp,"@");
		sip_str_append(&tmp,callid->host);
	}
	return SIP_SUCCESS;
}


//max forwards function
int sip_message_max_forwards_to_str( char *max_forwards, char **dest )
{
	char *tmp;
	int len;

	if(max_forwards == NULL)
	{
		return SIP_BADPARAMETER;
	}
	len = strlen(max_forwards);
	*dest = (char *)sip_malloc(len+1);
	if(*dest == NULL)
	{
		return SIP_NOMEM;
	}
	tmp = *dest;
	sip_str_append(&tmp,max_forwards);

	return SIP_SUCCESS;
}

//expires function
int sip_message_expires_to_str( char *expires, char **dest )
{
	char *tmp;
	int len;
   	if(expires == NULL)
   	{
   		return SIP_BADPARAMETER;
   	}
	len = strlen(expires);
	*dest = (char *)sip_malloc(len+1);
	if(*dest == NULL)
	{
		return SIP_NOMEM;
	}
	tmp = *dest;
	sip_str_append(&tmp,expires);

	return SIP_SUCCESS;
}

//date function
int sip_message_date_to_str( char *date , char **dest )
{
	char *tmp;
	int len;

	if(date == NULL)
	{
		return SIP_BADPARAMETER;
	}
	len = strlen(date);
	*dest = (char *)sip_malloc(len+1);
	if(*dest == NULL)
	{
		return SIP_NOMEM;
	}
	tmp = *dest;
	sip_str_append(&tmp,date);

	return SIP_SUCCESS;
}

//www_authenticate function
/*
Digest realm="cmcc.com",domain="vss.cmcc.com",nonce="0f8afc8ddcbcf03fae686f85dbc3a703",algorithm=MD5,qop="auth"

*/
int sip_message_www_authenticate_to_str( sip_www_authenticate_t *www_authenticate, char **dest )
{
	char *tmp;
	int len;

	if(www_authenticate == NULL || www_authenticate->auth_type == NULL)
	{
		return SIP_BADPARAMETER;
	}
	len = strlen(www_authenticate->auth_type);
	if(www_authenticate->realm != NULL)
		len += strlen(www_authenticate->realm)+7+2;
	if(www_authenticate->domain != NULL)
		len += strlen(www_authenticate->domain)+9+2;
	if(www_authenticate->nonce != NULL)
		len += strlen(www_authenticate->nonce)+8+2;
	if(www_authenticate->opaque != NULL)
		len += strlen(www_authenticate->opaque)+9+2;
	if(www_authenticate->stale != NULL)
		len += strlen(www_authenticate->stale)+8;
	if(www_authenticate->algorithm != NULL)
		len += strlen(www_authenticate->algorithm)+12;
	if(www_authenticate->qop != NULL)
		len += strlen(www_authenticate->qop)+6;
	
	*dest = (char *)sip_malloc(len+1);
	if(*dest == NULL)
	{
		return SIP_NOMEM;
	}
	tmp = *dest;

	sip_str_append(&tmp,www_authenticate->auth_type);
	
	if(www_authenticate->realm != NULL)
	{
		sip_str_append(&tmp," realm=");
		sip_str_append(&tmp,"\"");
		sip_str_append(&tmp,www_authenticate->realm);
		sip_str_append(&tmp,"\"");
	}
	if(www_authenticate->domain != NULL)
	{
		sip_str_append(&tmp,", domain=");
		sip_str_append(&tmp,"\"");
		sip_str_append(&tmp,www_authenticate->domain);
		sip_str_append(&tmp,"\"");
	}
		
	if(www_authenticate->nonce != NULL)
	{
		sip_str_append(&tmp,", nonce=");
		sip_str_append(&tmp,"\"");
		sip_str_append(&tmp,www_authenticate->nonce);
		sip_str_append(&tmp,"\"");
	}
		
	if(www_authenticate->opaque != NULL)
	{
		sip_str_append(&tmp,", opaque=");
		sip_str_append(&tmp,"\"");
		sip_str_append(&tmp,www_authenticate->opaque);
		sip_str_append(&tmp,"\"");
	}
		
	if(www_authenticate->stale != NULL)
	{
		sip_str_append(&tmp,", stale=");
		sip_str_append(&tmp,www_authenticate->stale);
	}
		
	if(www_authenticate->algorithm != NULL)
	{
		sip_str_append(&tmp,", algorithm=");
		sip_str_append(&tmp,www_authenticate->algorithm);
	}
		
	if(www_authenticate->qop != NULL)
	{
		sip_str_append(&tmp,", qop=");
		sip_str_append(&tmp,www_authenticate->qop);
	}
	
	return SIP_SUCCESS;
}

//authorization
/*
Digest username="010001010500020109@js.qly", realm="js.qly", once="DiNgix1LO0zuvI1skkZfgQAAAAARaxERzJzmsGp8FpY=", uri="sip:js.qly", response="5614bd1170f6ea16e50f00988aa362a9", cnonce="179661552", algorithm=MD5, opaque="", qop=auth, nc=00000001

*/
int sip_message_authorization_to_str( sip_authorization_t *authorization, char **dest )
{
	char *tmp;
	int len;
	if(authorization == NULL || authorization->auth_type == NULL)
	{
		return SIP_BADPARAMETER;
	}
	len =  strlen(authorization->auth_type);
	if(authorization->username != NULL)
		len += strlen(authorization->username)+10+2;
	if(authorization->realm != NULL)
		len += strlen(authorization->realm)+8+2;
	if(authorization->nonce != NULL)
		len += strlen(authorization->nonce)+8+2;
	if(authorization->uri != NULL)
		len += strlen(authorization->uri)+6+2;
	if(authorization->response != NULL)
		len += strlen(authorization->response)+11+2;
	if(authorization->cnonce != NULL)
		len += strlen(authorization->cnonce)+9+2;
	if(authorization->algorithm != NULL)
		len += strlen(authorization->algorithm)+12;
	if(authorization->opaque != NULL)
		len += strlen(authorization->opaque)+9+2;
	if(authorization->qop != NULL)
		len += strlen(authorization->qop)+6;
	if(authorization->nonce_count != NULL)
		len += strlen(authorization->nonce_count)+5;
	
	*dest = (char *)sip_malloc(len+1);
	if(*dest == NULL)
	{
		return SIP_NOMEM;
	}
	tmp = *dest;

	sip_str_append(&tmp,authorization->auth_type);

	if(authorization->username != NULL)
	{
		sip_str_append(&tmp," username=");
		sip_str_append(&tmp,"\"");
		sip_str_append(&tmp,authorization->username);
		sip_str_append(&tmp,"\"");
	}
	
	if(authorization->realm != NULL)
	{
		sip_str_append(&tmp,", realm=");
		sip_str_append(&tmp,"\"");
		sip_str_append(&tmp,authorization->realm);
		sip_str_append(&tmp,"\"");
	}
	if(authorization->nonce != NULL)
	{
		sip_str_append(&tmp,", nonce=");
		sip_str_append(&tmp,"\"");
		sip_str_append(&tmp,authorization->nonce);
		sip_str_append(&tmp,"\"");
	}
	if(authorization->uri != NULL)
	{
		sip_str_append(&tmp,", uri=");
		sip_str_append(&tmp,"\"");
		sip_str_append(&tmp,authorization->uri);
		sip_str_append(&tmp,"\"");
	}
	if(authorization->response != NULL)
	{
		sip_str_append(&tmp,", response=");
		sip_str_append(&tmp,"\"");
		sip_str_append(&tmp,authorization->response);
		sip_str_append(&tmp,"\"");
	}
	if(authorization->cnonce != NULL)
	{
		sip_str_append(&tmp,", cnonce=");
		sip_str_append(&tmp,"\"");
		sip_str_append(&tmp,authorization->cnonce);
		sip_str_append(&tmp,"\"");
	}
	if(authorization->algorithm != NULL)
	{
		sip_str_append(&tmp,", algorithm=");
		sip_str_append(&tmp,authorization->algorithm);
	}
	if(authorization->opaque != NULL)
	{
		sip_str_append(&tmp,", opaque=");
		sip_str_append(&tmp,"\"");
		sip_str_append(&tmp,authorization->opaque);
		sip_str_append(&tmp,"\"");
	}
	if(authorization->qop != NULL)
	{
		sip_str_append(&tmp,", qop=");
		sip_str_append(&tmp,authorization->qop);
	}
	if(authorization->nonce_count != NULL)
	{
		sip_str_append(&tmp,", nc=");
		sip_str_append(&tmp,authorization->nonce_count);
	}
		
	return SIP_SUCCESS;
}


//startline   REGISTER sip:js.qly SIP/2.0     SIP/2.0 301 Moved Permanently
int sip_message_startline_to_str(sip_message_t *sip_message,char **dest)
{
	char *tmp;
	if(sip_message == NULL || sip_message->sip_version == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(((sip_message->sip_method != NULL) && (sip_message->status_code != NULL))
		|| ((sip_message->sip_method == NULL) && (sip_message->status_code == NULL)))
	{
		return SIP_BADPARAMETER;
	}
	
	if(sip_message->sip_method != NULL)//request
	{  
		if(sip_message->req_uri == NULL)
		{
			return SIP_BADPARAMETER;
		}
		*dest = (char *)sip_malloc(strlen(sip_message->sip_method)+strlen(sip_message->req_uri)
			+strlen(sip_message->sip_version)+ 5);
		if(*dest == NULL)
		{
			return SIP_NOMEM;
		}
		tmp = *dest;
		sip_str_append(&tmp,sip_message->sip_method);
		sip_str_append(&tmp," ");
		sip_str_append(&tmp,sip_message->req_uri);
		sip_str_append(&tmp," ");
		sip_str_append(&tmp,sip_message->sip_version);
		sip_str_append(&tmp,CRLF);	
	}
	if(sip_message->status_code != NULL)//response
	{	
		if(sip_message->reason_phase == NULL)
		{
			return SIP_BADPARAMETER;
		}
		*dest = (char *)sip_malloc(strlen(sip_message->status_code)+strlen(sip_message->reason_phase)
			+strlen(sip_message->sip_version)+ 5);
		if(*dest == NULL)
		{
			return SIP_NOMEM;
		}
		tmp = *dest;
		sip_str_append(&tmp,sip_message->sip_version);
		sip_str_append(&tmp," ");
		sip_str_append(&tmp,sip_message->status_code);
		sip_str_append(&tmp," ");
		sip_str_append(&tmp,sip_message->reason_phase);
		sip_str_append(&tmp,CRLF);
	}
	return SIP_SUCCESS;
}
//headers
int sip_message_headers_to_str(sip_message_t *sip_message,char **dest)
{
	char *tmp,*header;
  	sip_via_t *via_list;
	
	*dest = (char *)sip_malloc(SIP_HEADERS_SIZE);
	if(*dest == NULL)
	{
		return SIP_NOMEM;
	}
	header = *dest;
	if(sip_message->via != NULL )
	{
		via_list = sip_message->via;
		while(via_list != NULL)
		{
			if(SIP_SUCCESS != sip_message_via_to_str(via_list,&tmp))
			{
				sip_free(*dest);
				*dest = NULL;
				return SIP_UNDEFINED_ERROR;
			}
			sip_str_append(&header,"Via: ");
			sip_str_append(&header,tmp);
		    sip_free(tmp);
			sip_str_append(&header,CRLF);
			via_list = via_list->next;
		}
	}

	if(sip_message->from != NULL )
	{
		if(SIP_SUCCESS != sip_message_from_to_str(sip_message->from,&tmp))
		{
			sip_free(*dest);
			*dest = NULL;
			return SIP_UNDEFINED_ERROR;
		}
		sip_str_append(&header,"From: ");
		sip_str_append(&header,tmp);
	    sip_free(tmp);
		sip_str_append(&header,CRLF);
	}
	if(sip_message->to != NULL )
	{
		if(SIP_SUCCESS != sip_message_to_to_str(sip_message->to,&tmp))
		{
			sip_free(*dest);
			*dest = NULL;
			return SIP_UNDEFINED_ERROR;
		}
		sip_str_append(&header,"To: ");
		sip_str_append(&header,tmp);
	    sip_free(tmp);
		sip_str_append(&header,CRLF);
	}
	if(sip_message->callid != NULL )
	{
		if(SIP_SUCCESS != sip_message_callid_to_str(sip_message->callid,&tmp))
		{
			sip_free(*dest);
			*dest = NULL;
			return SIP_UNDEFINED_ERROR;
		}
		sip_str_append(&header,"Call-ID: ");
		sip_str_append(&header,tmp);
	    sip_free(tmp);
		sip_str_append(&header,CRLF);
	}
	if(sip_message->cseq != NULL )
	{
		if(SIP_SUCCESS != sip_message_cseq_to_str(sip_message->cseq,&tmp))
		{
			sip_free(*dest);
			*dest = NULL;
			return SIP_UNDEFINED_ERROR;
		}
		sip_str_append(&header,"CSeq: ");
		sip_str_append(&header,tmp);
	    sip_free(tmp);
		sip_str_append(&header,CRLF);
	}
	if(sip_message->max_forwards != NULL )
	{
		if(SIP_SUCCESS != sip_message_max_forwards_to_str(sip_message->max_forwards,&tmp))
		{
			sip_free(*dest);
			*dest = NULL;
			return SIP_UNDEFINED_ERROR;
		}
		sip_str_append(&header,"Max-Forwards: ");
		sip_str_append(&header,tmp);
	    sip_free(tmp);
		sip_str_append(&header,CRLF);
	}
	if(sip_message->expires != NULL )
	{
		if(SIP_SUCCESS != sip_message_expires_to_str(sip_message->expires,&tmp))
		{
			sip_free(*dest);
			*dest = NULL;
			return SIP_UNDEFINED_ERROR;
		}
		sip_str_append(&header,"Expires: ");
		sip_str_append(&header,tmp);
	    sip_free(tmp);
		sip_str_append(&header,CRLF);
	}
	if(sip_message->contact != NULL )
	{
		if(SIP_SUCCESS != sip_message_contact_to_str(sip_message->contact,&tmp))
		{
			sip_free(*dest);
			*dest = NULL;
			return SIP_UNDEFINED_ERROR;
		}
		sip_str_append(&header,"Contact: ");
		sip_str_append(&header,tmp);
	    sip_free(tmp);
		sip_str_append(&header,CRLF);
	}
	if(sip_message->date != NULL )
	{
		if(SIP_SUCCESS != sip_message_date_to_str(sip_message->date,&tmp))
		{
			sip_free(*dest);
			*dest = NULL;
			return SIP_UNDEFINED_ERROR;
		}
		sip_str_append(&header,"Date: ");
		sip_str_append(&header,tmp);
	    sip_free(tmp);
		sip_str_append(&header,CRLF);
	}
	if(sip_message->authorization != NULL )
	{
		if(SIP_SUCCESS != sip_message_authorization_to_str(sip_message->authorization,&tmp))
		{
			sip_free(*dest);
			*dest = NULL;
			return SIP_UNDEFINED_ERROR;
		}
		sip_str_append(&header,"Authorization: ");
		sip_str_append(&header,tmp);
	    sip_free(tmp);
		sip_str_append(&header,CRLF);
	}
	if(sip_message->www_authenticate != NULL )
	{
		if(SIP_SUCCESS != sip_message_www_authenticate_to_str(sip_message->www_authenticate,&tmp))
		{
			sip_free(*dest);
			*dest = NULL;
			return SIP_UNDEFINED_ERROR;
		}
		sip_str_append(&header,"WWW-Authenticate: ");
		sip_str_append(&header,tmp);
	    sip_free(tmp);
		sip_str_append(&header,CRLF);
	}
	if(sip_message->content_type != NULL )
	{
		if(SIP_SUCCESS != sip_message_content_type_to_str(sip_message->content_type,&tmp))
		{
			sip_free(*dest);
			*dest = NULL;
			return SIP_UNDEFINED_ERROR;
		}
		sip_str_append(&header,"Content-Type: ");
		sip_str_append(&header,tmp);
	    sip_free(tmp);
		sip_str_append(&header,CRLF);
	}
	if(sip_message->subject != NULL )
	{
		if(SIP_SUCCESS != sip_message_subject_to_str(sip_message->subject,&tmp))
		{
			sip_free(*dest);
			*dest = NULL;
			return SIP_UNDEFINED_ERROR;
		}
		sip_str_append(&header,"Subject: ");
		sip_str_append(&header,tmp);
	    sip_free(tmp);
		sip_str_append(&header,CRLF);
	}
	if(sip_message->user_agent != NULL)
	{
		if(SIP_SUCCESS != sip_message_user_agent_to_str(sip_message->user_agent,&tmp))
		{
			sip_free(*dest);
			*dest = NULL;
			return SIP_UNDEFINED_ERROR;
		}
		sip_str_append(&header,"User-Agent: ");
		sip_str_append(&header,tmp);
	    sip_free(tmp);
		sip_str_append(&header,CRLF);
	}
	if(sip_message->event != NULL)
	{
		if(SIP_SUCCESS != sip_message_event_to_str(sip_message->event,&tmp))
		{
			sip_free(*dest);
			*dest = NULL;
			return SIP_UNDEFINED_ERROR;
		}
		sip_str_append(&header,"Event: ");
		sip_str_append(&header,tmp);
	    sip_free(tmp);
		sip_str_append(&header,CRLF);
	}
	if(sip_message->subscription_state != NULL)
	{
		if(SIP_SUCCESS != sip_message_subscription_state_to_str(sip_message->subscription_state,&tmp))
		{
			sip_free(*dest);
			*dest = NULL;
			return SIP_UNDEFINED_ERROR;
		}
		sip_str_append(&header,"Subscription-State: ");
		sip_str_append(&header,tmp);
	    sip_free(tmp);
		sip_str_append(&header,CRLF);
	}

	if(sip_message->content_length != NULL)
	{
		if(sip_message->content_type == NULL)
		{
			sprintf(sip_message->content_length , "0");
		}
		if(SIP_SUCCESS != sip_message_content_length_to_str(sip_message->content_length,&tmp))
		{
			sip_free(*dest);
			*dest = NULL;
			return SIP_UNDEFINED_ERROR;
		}
		sip_str_append(&header,"Content-Length: ");
		sip_str_append(&header,tmp);
	    sip_free(tmp);
		sip_str_append(&header,CRLF);
	}
	tmp = NULL;
	return SIP_SUCCESS;
}
//body
int sip_message_body_to_str(sip_message_t *sip_message,char **dest)
{
	char *tmp;
	if(sip_message == NULL || sip_message->sip_body == NULL)
	{
		return SIP_BADPARAMETER;
	}
	
	*dest = (char *)sip_malloc(strlen(sip_message->sip_body)+1);
	if(*dest == NULL)
	{
		return SIP_NOMEM;
	}
	tmp = *dest;
	sip_str_append(&tmp,sip_message->sip_body);
	return SIP_SUCCESS;
}
//sip message
int sip_message_to_str(sip_message_t *sip_message)
{
	char *message;
	char *tmp;
	int message_len;
	if(sip_message == NULL)
	{
		return SIP_BADPARAMETER;
	}
	/*if(sip_message->message == NULL)
	{
		return SIP_BADPARAMETER;
	}
	message = sip_message->message;
    memset( message , 0 , SIP_BODY_SIZE );
    */
    sip_free(sip_message->message);
    sip_message->message = (char *)sip_malloc(((NULL != sip_message->sip_body)?strlen(sip_message->sip_body):0) + SIP_BODY_SIZE);
    if(sip_message->message == NULL)
	{
		return SIP_BADPARAMETER;
	}
	message = sip_message->message;
	memset( message , 0 , ((NULL != sip_message->sip_body)?strlen(sip_message->sip_body):0) + SIP_BODY_SIZE);
	//startline
	if(SIP_SUCCESS != sip_message_startline_to_str(sip_message,&tmp))
	{
		return SIP_UNDEFINED_ERROR;
	}
	sip_str_append(&message,tmp);
	sip_free(tmp);
	//headers
	if(SIP_SUCCESS != sip_message_headers_to_str(sip_message,&tmp))
	{
	
		return SIP_UNDEFINED_ERROR;
	}
	sip_str_append(&message,tmp);
	sip_free(tmp);
	
	sip_str_append(&message,CRLF);

	//body,maybe exist
	if(sip_message->sip_body != NULL)
	{
		if(SIP_SUCCESS != sip_message_body_to_str(sip_message,&tmp))
		{
			return SIP_UNDEFINED_ERROR;
		}
		sip_str_append(&message,tmp);
		sip_free(tmp);
	}
	
	message_len = message - sip_message->message;
	sip_message->message[message_len] = '\0';
	return SIP_SUCCESS;
}
//subject
int sip_message_subject_to_str(sip_subject_t *subject,char **dest)
{
	char *tmp;
	int len;

	if(subject == NULL || subject->sender_id == NULL ||subject->sender_seq == NULL
		|| subject->recver_id == NULL ||subject->recver_seq == NULL)
	{
		return SIP_BADPARAMETER;
	}
	len = strlen(subject->sender_id);
	len += strlen(":")+1;
	
	len += strlen(subject->sender_seq)+1;
	len += strlen(",")+1;	
	len += strlen(subject->recver_id)+1;
	len += strlen(":")+1;	
	len += strlen(subject->recver_seq)+1;
	*dest = (char *)sip_malloc(len+1);
	if(*dest == NULL)
	{
		return SIP_NOMEM;
	}
	tmp = *dest;
	sip_str_append(&tmp,subject->sender_id);
	sip_str_append(&tmp,":");
	sip_str_append(&tmp,subject->sender_seq);
	sip_str_append(&tmp,",");
	sip_str_append(&tmp,subject->recver_id);
	sip_str_append(&tmp,":");
	sip_str_append(&tmp,subject->recver_seq);
						
	return SIP_SUCCESS;
}

//user agent funciton
int sip_message_user_agent_to_str( char *user_agent, char **dest )
{
	char *tmp;
	int len;

	if(user_agent == NULL)
	{
		return SIP_BADPARAMETER;
	}
	len = strlen(user_agent);
	*dest = (char *)sip_malloc(len+1);
	if(*dest == NULL)
	{
		return SIP_NOMEM;
	}
	tmp = *dest;
	sip_str_append(&tmp,user_agent);

	return SIP_SUCCESS;
}
//Event funciton
int sip_message_event_to_str( char *event, char **dest )
{
	char *tmp;
	int len;

	if(event == NULL)
	{
		return SIP_BADPARAMETER;
	}
	len = strlen(event);
	*dest = (char *)sip_malloc(len+1);
	if(*dest == NULL)
	{
		return SIP_NOMEM;
	}
	tmp = *dest;
	sip_str_append(&tmp,event);

	return SIP_SUCCESS;
}
//subscription_state funciton
int sip_message_subscription_state_to_str( char *subscription_state, char **dest )
{
	char *tmp;
	int len;

	if(subscription_state == NULL)
	{
		return SIP_BADPARAMETER;
	}
	len = strlen(subscription_state);
	*dest = (char *)sip_malloc(len+1);
	if(*dest == NULL)
	{
		return SIP_NOMEM;
	}
	tmp = *dest;
	sip_str_append(&tmp,subscription_state);

	return SIP_SUCCESS;
}

