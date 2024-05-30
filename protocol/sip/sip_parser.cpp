/*********************************************************************************************
*
*
*
*
*
*********************************************************************************************/

#include "sip_std.h"
#include "sip_common.h"
#include "sip_headers_parser.h"
#include "sip_parser.h"




int sip_handout_message( char *hname , char *hvalue , sip_message_t *sip_message )
{
	int ret ;

	if(  0 == strncmp( hname , "Via" , strlen(hname) ) )
	{
		ret = sip_message_parse_via( hvalue , &(sip_message->via) );
	}else if(  0 == strncmp( hname , "Max-Forwards" , strlen(hname) ) )
	{
		ret = sip_message_parse_max_forwards( hvalue , &(sip_message->max_forwards) );
	}else if(  0 == strncmp( hname , "To" , strlen(hname) ) )
	{
		ret = sip_message_parse_to( hvalue , &(sip_message->to) );
	}else if(  0 == strncmp( hname , "From" , strlen(hname) ) )
	{
		ret = sip_message_parse_from( hvalue , &(sip_message->from) );
	}else if(  0 == strncmp( hname , "Call-ID" , strlen(hname) ) )
	{
		ret = sip_message_parse_callid( hvalue , &(sip_message->callid) );
	}else if(  0 == strncmp( hname , "CSeq" , strlen(hname) ) )
	{
		ret = sip_message_parse_cseq( hvalue , &(sip_message->cseq) );
	}else if(  0 == strncmp( hname , "Contact" , strlen(hname) ) )
	{
		ret = sip_message_parse_contact( hvalue , &(sip_message->contact) );
	}else if(  0 == strncmp( hname , "Content-Type" , strlen(hname) ) )
	{
		ret = sip_message_parse_content_type( hvalue , &(sip_message->content_type) );
	}else if(  0 == strncmp( hname , "Content-Length" , strlen(hname) ) )
	{
		ret = sip_message_parse_content_length( hvalue , &(sip_message->content_length) );
	}else if(	0 == strncmp( hname , "Expires" , strlen(hname)) )
	{
		ret = sip_message_parse_expires( hvalue, &(sip_message->expires) );
	}else if(	0 == strncmp( hname , "Date" , strlen(hname)) )
	{
		ret = sip_message_parse_date( hvalue, &(sip_message->date) );
	}else if(	0 == strncmp( hname , "WWW-Authenticate" , strlen(hname)) )
	{
		ret = sip_message_parse_www_authenticate( hvalue, &(sip_message->www_authenticate) );
	}else if(	0 == strncmp( hname , "Authorization" , strlen(hname)) )
	{
		ret = sip_message_parse_authorization( hvalue, &(sip_message->authorization) );
	}else if(	0 == strncmp( hname , "Subject" , strlen(hname)) )
	{
		ret = sip_message_parse_subject( hvalue, &(sip_message->subject) );
	}else if(	0 == strncmp( hname , "User-Agent" , strlen(hname)) )
	{
		ret = sip_message_parse_user_agent( hvalue, &(sip_message->user_agent) );
	}else if(	0 == strncmp( hname , "Event" , strlen(hname)) )
	{
		ret = sip_message_parse_event( hvalue, &(sip_message->event) );
	}else if(	0 == strncmp( hname , "Subscription-State" , strlen(hname)) )
	{
		ret = sip_message_parse_subscription_state( hvalue, &(sip_message->subscription_state) );
	}else{
		return SIP_NO_HEADER ;
	}

	return ret ;
}

int sip_message_parse_headers( char *value , sip_message_t *sip_message  )
{

	char *start_header , *end_header ;
	char *hname , *hvalue ;
	int ret = SIP_SUCCESS ;
	unsigned int len = 0;
	if(value == NULL || sip_message == NULL)
	{
		return SIP_BADPARAMETER;
	}
	
	start_header = value ;
	//find  header name 
	
	while(( *start_header != SIP_COLON  ) && ( len < strlen(value) ) )
	{
		if( start_header[0] == SIP_CR  && start_header[1] == SIP_LF )
		{
			return SIP_SYNTAXERROR ;
		}
		len++;
		start_header++ ;
	}
	if( start_header == value)   //no header name
	{
		return SIP_SYNTAXERROR ;
	}
	end_header = start_header - 1;
	while(*end_header == SIP_BLANK) end_header --;//skip space between  hname and ':'
	if( end_header == value)   //no header name
	{
		return SIP_SYNTAXERROR ;
	}
	hname = (char *)sip_malloc(HEADER_NAME_SIZE) ;
	if(hname == NULL)
	{
		return SIP_NOMEM;
	}
	sip_str_cpy( hname , HEADER_NAME_SIZE , value , end_header - value + 1);

	//find  header value
	start_header ++ ;
	while( *start_header == SIP_BLANK ) start_header ++ ;//skip space between ':' and hvalue
	if( SIP_SUCCESS != sip_find_next_crlf( start_header , &end_header ) )
	{
		sip_free( hname );
		return SIP_SYNTAXERROR ;
	}
	//add  20121024
	if(start_header == (end_header - 2))//no header value
	{
		sip_free( hname );
		return SIP_SYNTAXERROR;
	}
	//add  end 
	hvalue = ( char * )sip_malloc( HEADER_VALUE_SIZE );
	if(hvalue == NULL)
	{
		sip_free( hname );
		return SIP_NOMEM;
	}
	sip_str_cpy( hvalue , HEADER_VALUE_SIZE , start_header , end_header - start_header);//with CRLF
	ret = sip_handout_message(hname , hvalue , sip_message);

    //change it 
    if( ret == SIP_NO_HEADER )
        ret = SIP_SUCCESS ;

    sip_free( hvalue );
	sip_free( hname );

	return ret ;
}

/*
requst_line:
REGISTER sip:js.qly SIP/2.0
OPTIONS sip:js.qly SIP/2.0
INVITE  sip: 010001010500020109@js.qly SIP/2.0

BYE sip: 010001010500020109@js.qly SIP/2.0
ACK sip:js.qly SIP/2.0

status_line:
SIP/2.0 301 Moved Permanently
SIP/2.0 401 Unauthorized
SIP/2.0 200 OK

*/

int sip_message_parse_startline( char *value , sip_message_t *sip_message )
{
    char *start_line, *end_line;

	if(value == NULL || sip_message == NULL)
	{
		return SIP_BADPARAMETER;
	}
	start_line = value;
	if(SIP_SUCCESS != sip_find_next_string(start_line,&end_line,"SIP/2.0"))
	{
		return SIP_SYNTAXERROR;
	}
	sip_message->sip_version  = (char*)sip_malloc(strlen("SIP/2.0") + 1);
	if(sip_message->sip_version  == NULL)
	{
		return SIP_NOMEM;
	}
	sip_str_cpy(sip_message->sip_version ,strlen("SIP/2.0") + 1,"SIP/2.0",strlen("SIP/2.0"));
	//printf("%s\n",sip_message->sip_version);
	
	if(end_line > start_line)//requst_line
	{
		if(SIP_SUCCESS != sip_find_next_character(start_line,&end_line,' '))
		{
			return SIP_SYNTAXERROR;
		}
		sip_message->sip_method = (char*)sip_malloc(end_line - start_line + 1);
		if(sip_message->sip_method == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy(sip_message->sip_method,end_line - start_line + 1,start_line,end_line - start_line);
		//printf("%s\n",sip_message->sip_method);
		
		start_line = end_line + 1;
		if(SIP_SUCCESS != sip_find_next_character(start_line,&end_line,' '))
		{
			return SIP_SYNTAXERROR;
		}
		sip_message->req_uri = (char*)sip_malloc(end_line - start_line + 1);
		if(sip_message->req_uri == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy(sip_message->req_uri,end_line - start_line + 1,start_line,end_line - start_line);
		
	}
	else  //status_line
	{
		start_line = end_line += strlen("SIP/2.0");
		if(*start_line != SIP_BLANK) //must be a space
		{
			return SIP_SYNTAXERROR;
		}
		start_line ++;
		if(SIP_SUCCESS == sip_find_next_character(start_line,&end_line,' '))
		{
			if((end_line - start_line) != 3)//must be size of 3
			{
				return SIP_SYNTAXERROR;
			}
			sip_message->status_code= (char*)sip_malloc(end_line - start_line + 1);
			if(sip_message->status_code == NULL)
			{
				return SIP_NOMEM;
			}
			sip_str_cpy(sip_message->status_code,end_line - start_line + 1,start_line,end_line - start_line);
		}
		else
		{
			return SIP_SYNTAXERROR;
		}
		start_line = end_line + 1;
		if(SIP_SUCCESS == sip_find_next_crlf(start_line,&end_line))
		{
			sip_message->reason_phase = (char*)sip_malloc(end_line - start_line - 1);
			if(sip_message->reason_phase == NULL)
			{
				return SIP_NOMEM;
			}			
			sip_str_cpy(sip_message->reason_phase,end_line - start_line - 1,start_line,end_line - start_line - 2);
		}
		else
		{
			return SIP_SYNTAXERROR;
		}
	}
	return SIP_SUCCESS ;
}

//body
int sip_message_parse_body( char *value , sip_message_t *sip_message )
{
	int len;
	len = strlen(value);
	sip_message->sip_body = (char *)sip_malloc(len+1);
	sip_str_cpy(sip_message->sip_body,len+1,value,len);
	/*callback  */
	return SIP_SUCCESS ;
}


int sip_message_parser( char *strmsg , sip_message_t **sip_message  )
{
	char *start_header , *end_header ;
	char *c_buffer ;

	if( strmsg == NULL )
		return SIP_BADPARAMETER ;
	if(*sip_message == NULL)
		return SIP_BADPARAMETER;
	
	start_header = strmsg ;
	c_buffer = ( char * )malloc( COMMON_BUFFER_SIZE );
	if(c_buffer == NULL)
	{
		return SIP_NOMEM;
	}
	//get sip message startline
	if( SIP_SUCCESS != sip_find_next_crlf( start_header , &end_header ) )
	{
		free( c_buffer ) ;
		return SIP_SYNTAXERROR ;
	}
	sip_str_cpy( c_buffer , COMMON_BUFFER_SIZE , start_header , end_header-start_header );
	if(SIP_SUCCESS != sip_message_parse_startline( c_buffer , *sip_message ) )
	{
		free( c_buffer ) ;
		return SIP_UNDEFINED_ERROR;
	}
	//get sip message headers and body
	for( ; ;  )
	{
		start_header = end_header ;
		if( SIP_SUCCESS != sip_find_next_crlf( start_header , &end_header ) )
		{
			free(c_buffer) ;
			return SIP_SYNTAXERROR ;
		}

		if((end_header - start_header - 2)>(HEADER_NAME_SIZE + HEADER_VALUE_SIZE))//some header may have no CRLF,some have
		{
			free(c_buffer) ;
			return SIP_SYNTAXERROR ;			
		}

		sip_str_cpy( c_buffer , COMMON_BUFFER_SIZE , start_header , end_header-start_header );
		if((NULL == strchr(c_buffer,SIP_COLON)) || (NULL == strchr(c_buffer,SIP_CR)) || (NULL == strchr(c_buffer,SIP_LF)))
		{
			free(c_buffer);
			return SIP_UNDEFINED_ERROR;
		}
		if(SIP_SUCCESS != sip_message_parse_headers( c_buffer , *sip_message ))
		{
			free(c_buffer);
			return SIP_UNDEFINED_ERROR;
		}
		//check if end
		if( end_header[0] == SIP_CR && end_header[1] == SIP_LF)
		{
			start_header = end_header+2 ;
			if( SIP_SUCCESS == sip_str_find_end( start_header , &end_header ) )//body
			{
				char *pbody = (char *)sip_malloc(end_header-start_header+10);
				if(NULL == pbody)
				{
					free(  c_buffer ) ;
					return SIP_NOMEM;
				}
				sip_str_cpy( pbody , end_header-start_header+10 , start_header , end_header-start_header );
				sip_message_parse_body( pbody , *sip_message ) ;
				free(  pbody ) ;
				break ;
			}
		}

	}
	free(  c_buffer ) ;	
	return SIP_SUCCESS ;
}



