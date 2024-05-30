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
#include "sip_message.h"
#include "get_random.h"


int sip_message_parse_uri(char *value,sip_uri_t **uri)
{
	char *start_pos, *end_pos;
	
	if(value == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(SIP_SUCCESS != sip_message_uri_init(uri))
	{
		return SIP_NOMEM;
	}
	start_pos = value;
	if( SIP_SUCCESS != sip_find_next_string(start_pos , &end_pos , "sip:"))
	{
		return SIP_SYNTAXERROR ;
	}
	start_pos += strlen("sip:");
	if( SIP_SUCCESS == sip_find_next_character(start_pos,&end_pos,'@'))
	{
		(*uri)->client_id = (char *)sip_malloc(end_pos - start_pos + 1);
		sip_str_cpy((*uri)->client_id,end_pos - start_pos + 1,start_pos,end_pos - start_pos);
		//printf("%s\n",(*uri)->client_id);

		start_pos = end_pos += 1;
	}
	if(SIP_SUCCESS != sip_str_find_end(start_pos,&end_pos))
	{
		return SIP_SYNTAXERROR;
	}
	(*uri)->host = (char *)sip_malloc(end_pos - start_pos + 1);
	sip_str_cpy((*uri)->host,end_pos - start_pos + 1,start_pos,end_pos - start_pos);
	//printf("%s\n",(*uri)->host);

	return SIP_SUCCESS;
}


/**************to*************/ 

/*
example : 
	-->	sip:philip@sinonet.net.cn;tag=1234-5678
	-->	philip <sip:philip@sinonet.net.cn>;tag=1234-5678
	-->	"philip" <sip:philip@sinonet.net.cn>;tag=1234-5678
	--><sip:philip@sinonet.net.cn>;tag=1234-5678

*/
int sip_message_parse_to( char *value , sip_to_t **to )
{
	char *start_pos , *end_pos ;
    char *url_start_pos,*url_end_pos;

	if(value == NULL)
	{
		return SIP_NOMEM;
	}
	//init sip_to
	if(SIP_SUCCESS != sip_message_to_init( to ))
	{
		return  SIP_NOMEM;
	}

	start_pos = value ;
	while( *start_pos == SIP_BLANK ) start_pos ++ ;//skip all space

	if( SIP_SUCCESS != sip_find_next_string(start_pos , &end_pos , "sip:"))
	{
		return SIP_SYNTAXERROR ;
	}
	//find url ,sip url must exist
	url_end_pos = url_start_pos = end_pos;
	while((*url_end_pos != '>') && (*url_end_pos != ';') && (url_end_pos[0] != SIP_CR && url_end_pos[1] != SIP_LF))
	{
		url_end_pos ++;
	}
	(*to)->url = (char *)sip_malloc( url_end_pos-url_start_pos+1) ;
	if((*to)->url == NULL)
	{
		return SIP_NOMEM;
	}
	sip_str_cpy( (*to)->url , url_end_pos-url_start_pos+1, url_start_pos ,url_end_pos-url_start_pos );
	
	//find host name
	if( end_pos - start_pos > 1 )
	{
		char key;
		if( *start_pos == '"' )
		{
			key = '"';
			start_pos ++;
		}
		else
			key = ' ';
    
		if( SIP_SUCCESS == sip_find_next_character( start_pos , &end_pos , key))
		{
			if( end_pos - start_pos > 1 )
			{
				(*to)->name = (char *)sip_malloc( end_pos - start_pos + 1) ;
				if((*to)->name == NULL)
				{
					return SIP_NOMEM;
				}
				sip_str_cpy((*to)->name , end_pos - start_pos + 1 , start_pos, end_pos-start_pos);
			}
		}
	}
	
	//find tag 
	if( SIP_SUCCESS == sip_find_next_string(start_pos , &end_pos , "tag=" ))
	{
		start_pos = end_pos += strlen( "tag=" );
		if( SIP_SUCCESS == sip_find_next_crlf(start_pos , &end_pos ))
		{
			if( end_pos - start_pos > 2 )
			{
				(*to)->tag = (char *)sip_malloc( end_pos-start_pos-1) ;
				if((*to)->tag == NULL)
				{
					return SIP_NOMEM;
				}
				sip_str_cpy( (*to)->tag , end_pos-start_pos-1, start_pos , end_pos-start_pos-2); 
			}
		}
	}

	return SIP_SUCCESS ;
}

/**************from*************/ 
int sip_message_parse_from( char *value , sip_from_t **from )
{
	return sip_message_parse_to( value , (sip_to_t **)from ) ;
}


/**************contact*************/

int sip_message_parse_contact( char *value , sip_contact_t **contact )
{
	return sip_message_parse_to( value , (sip_to_t **)contact ) ;
}

/**************via*************/
/*
Via: SIP/2.0/UDP 10.6.10.101:5060;branch=z9hG4bK791-13184-10901
Via: SIP/2.0/UDP 10.6.10.101:5060;received=10.6.10.101;branch=z9hG4bK791-13184-10901
Via: SIP/2.0/UDP 10.6.10.1:5061;received=10.6.10.1;branch=z9hG4bK*1-1-57-15-BE*mnCR3uHmgg.1;cmcc-did=1-1-57-15-BE
*/

int sip_message_parse_via( char *value , sip_via_t **via)
{
	char *start_pos , *end_pos , *portnum;
	sip_via_t **via_list  ;
	char *head = value;
	if(value == NULL)
	{
		return SIP_BADPARAMETER;
	}
	
	if( *via != NULL)
	{	
		if((*via)->next != NULL)
		{
			if((*via)->next->next != NULL)
			{
				if((*via)->next->next->next != NULL)
				{
					return SIP_SYNTAXERROR;
				}
				via_list = &((*via)->next->next->next);
			}else
			{
				via_list = &((*via)->next->next);
			}
		}else
		{
			via_list = &((*via)->next);
		}
	
	}else
	{
		via_list = via ;
	}
	if(SIP_SUCCESS != sip_message_via_init(via_list))
	{
		return SIP_NOMEM;
	}
	start_pos = value ;
	//skip all blank
	while( *start_pos == SIP_BLANK ) start_pos ++ ;

	//find net type
	if( SIP_SUCCESS == sip_find_next_string(start_pos , &end_pos , "SIP/2.0/UDP"))
	{
		(*via_list)->net_type = TYPE_UDP;
		start_pos = end_pos += strlen("SIP/2.0/UDP");
	}
	else if( SIP_SUCCESS == sip_find_next_string(start_pos , &end_pos , "SIP/2.0/TCP"))
	{
		(*via_list)->net_type = TYPE_TCP;
		start_pos = end_pos += strlen("SIP/2.0/TCP");
	}
	else
	{
		return SIP_SYNTAXERROR;
	} 
	//find ip addr
	while( *start_pos == SIP_BLANK ) start_pos ++ ;
	if( SIP_SUCCESS == sip_find_next_character(start_pos,&end_pos,':'))
	{
		(*via_list)->ip_addr = (char *)sip_malloc( end_pos - start_pos + 1) ;
		if((*via_list)->ip_addr == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*via_list)->ip_addr,end_pos - start_pos + 1,start_pos,end_pos - start_pos);
		start_pos = end_pos += 1;
	}else if( SIP_SUCCESS == sip_find_next_crlf(start_pos , &end_pos ) )
    {
        (*via_list)->ip_addr = (char *)sip_malloc( end_pos - start_pos - 1) ;
		if((*via_list)->ip_addr == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*via_list)->ip_addr,end_pos - start_pos - 1,start_pos,end_pos - start_pos-2);
        start_pos = end_pos += 1;
        return SIP_SUCCESS ;
    }else
	{
		return SIP_SYNTAXERROR;
	} 
	//find port
	if(SIP_SUCCESS == sip_find_next_character(start_pos,&end_pos,';'))
	{
		portnum = (char *)sip_malloc( end_pos - start_pos + 1) ;
		if(portnum == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy(portnum,end_pos - start_pos + 1,start_pos,end_pos - start_pos);
		sscanf(portnum,"%d",&(*via_list)->port);
		sip_free(portnum);
	}
	else
	{
		if(SIP_SUCCESS == sip_find_next_crlf(start_pos , &end_pos ))
		{
			portnum = (char *)sip_malloc( end_pos - start_pos - 1) ;
			if(portnum == NULL)
			{
				return SIP_NOMEM;
			}
			sip_str_cpy(portnum,end_pos - start_pos - 1,start_pos,end_pos - start_pos-2);
			sscanf(portnum,"%d",&(*via_list)->port);
			sip_free(portnum);
		}
		else
		{
			return SIP_SYNTAXERROR;
		}
	} 

	
	//find rport
	if(SIP_SUCCESS == sip_find_next_string(head,&end_pos,"rport"))
	{
		start_pos = end_pos += strlen("rport");
		
		if('=' != *start_pos )
		{
			(*via_list)->rport = 0 ;
		}else{
			start_pos ++ ;
			sscanf(start_pos,"%d",&(*via_list)->rport);
		}
	}
	//find branch
	if(SIP_SUCCESS == sip_find_next_string(head,&end_pos,"branch="))
	{
		start_pos = end_pos += strlen("branch=");
		
		if(SIP_SUCCESS != sip_find_next_string(start_pos,&end_pos,"z9hG4bK"))
		{
			return SIP_SYNTAXERROR;
		}
		if(start_pos != end_pos)
		{
			return SIP_SYNTAXERROR;
		}
		if( SIP_SUCCESS == sip_find_next_character(start_pos,&end_pos,';'))
		{
			(*via_list)->branch = (char *)sip_malloc( end_pos - start_pos + 1) ;
			if((*via_list)->branch == NULL)
			{
				return SIP_NOMEM;
			}
			sip_str_cpy((*via_list)->branch,end_pos - start_pos + 1,start_pos,end_pos - start_pos);
		}
		else if( SIP_SUCCESS == sip_find_next_crlf(start_pos , &end_pos ))
		{
			(*via_list)->branch = (char *)sip_malloc( end_pos-start_pos-1) ;
			if((*via_list)->branch == NULL)
			{
				return SIP_NOMEM;
			}
			sip_str_cpy( (*via_list)->branch , end_pos-start_pos-1 , start_pos , end_pos-start_pos-2); 
		}
	}
	return SIP_SUCCESS ;
}

/**************cseq*************/
int sip_message_parse_cseq( char *value , sip_cseq_t **cseq )
{
	char *start_pos , *end_pos;

	if(value == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(SIP_SUCCESS != sip_message_cseq_init(cseq))
	{
		return SIP_NOMEM;
	}

	start_pos = value ;
	//skip all blank
	while( *start_pos == SIP_BLANK ) start_pos ++ ;
	
   	//find number
	if(SIP_SUCCESS == sip_find_next_character(start_pos,&end_pos,' '))
	{
		(*cseq)->number = (char *)sip_malloc(end_pos - start_pos + 1);
		if((*cseq)->number == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*cseq)->number,end_pos - start_pos + 1,start_pos,end_pos - start_pos);
		start_pos = end_pos;
	}
	//find method
	while( *start_pos == SIP_BLANK ) start_pos ++ ;
	if(SIP_SUCCESS == sip_find_next_crlf(start_pos , &end_pos ))
	{
		(*cseq)->method = (char *)sip_malloc(end_pos - start_pos-1);
		if((*cseq)->method  == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*cseq)->method,end_pos - start_pos - 1,start_pos,end_pos - start_pos - 2);
	}
	return SIP_SUCCESS ;
}



/**************content_length*************/
int sip_message_parse_content_length( char *value , char **content_length )
{
	char *start_pos, *end_pos;

	start_pos = value;
	if(value == NULL)
	{
		return SIP_BADPARAMETER;
	}
	while( *start_pos == SIP_BLANK) start_pos ++;
	if(SIP_SUCCESS == sip_find_next_crlf(start_pos, &end_pos))
	{
		*content_length = (char *)sip_malloc(end_pos - start_pos - 1);
		if(*content_length == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy(*content_length,end_pos - start_pos - 1,start_pos,end_pos - start_pos - 2);
	}
	return SIP_SUCCESS ;
}



/**************content_type*************/
/*Content-Type: application/xml*/
int sip_message_parse_content_type( char *value , sip_content_type_t **content_type )
{
	char *start_pos,*end_pos;

	if(value == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(SIP_SUCCESS != sip_message_content_type_init(content_type))
	{
		return SIP_BADPARAMETER;
	}
	start_pos  = value;
	while( *start_pos == SIP_BLANK) start_pos ++;

	//find type
	if(SIP_SUCCESS == sip_find_next_character(start_pos,&end_pos,'/'))
	{
		(*content_type)->type = (char *)sip_malloc(end_pos - start_pos + 1);
		if((*content_type)->type == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*content_type)->type,end_pos - start_pos + 1,start_pos,end_pos - start_pos);
		start_pos = end_pos += 1;
	}
	//find sub_type
	if(SIP_SUCCESS == sip_find_next_character(start_pos,&end_pos,';'))
	{
		(*content_type)->sub_type = (char *)sip_malloc(end_pos - start_pos + 1);
		if((*content_type)->sub_type == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*content_type)->sub_type,end_pos - start_pos + 1,start_pos,end_pos - start_pos);

		//find others
		start_pos = end_pos += 1;
		if(SIP_SUCCESS == sip_find_next_crlf(start_pos,&end_pos))
		{
			(*content_type)->other = (char *)sip_malloc(end_pos - start_pos - 1);
			if((*content_type)->other == NULL)
			{
				return SIP_NOMEM;
			}
			sip_str_cpy((*content_type)->other,end_pos - start_pos - 1,start_pos,end_pos - start_pos - 2);
		}
	}
	else
	{
		if(SIP_SUCCESS == sip_find_next_crlf(start_pos,&end_pos))
		{
			(*content_type)->sub_type = (char *)sip_malloc(end_pos - start_pos - 1);
			if((*content_type)->sub_type == NULL)
			{
				return SIP_NOMEM;
			}
			sip_str_cpy((*content_type)->sub_type,end_pos - start_pos - 1,start_pos,end_pos - start_pos - 2);
		}
	}
	return SIP_SUCCESS ;
}


/**************callid*************/

/*Call-ID: 001E90A980A1-20101009120010111-3422@010001010500020109*/
int sip_message_parse_callid( char *value , sip_callid_t **callid )
{
	char *start_pos,*end_pos;
	if(value == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(SIP_SUCCESS != sip_message_callid_init(callid))
	{
		return SIP_BADPARAMETER;
	}
	start_pos = value;
	while( *start_pos == SIP_BLANK) start_pos ++;
	if(SIP_SUCCESS == sip_find_next_character(start_pos,&end_pos,'@'))
	{
		(*callid)->number = (char *)sip_malloc(end_pos - start_pos + 1);
		if((*callid)->number == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*callid)->number,end_pos - start_pos + 1,start_pos,end_pos - start_pos);
		start_pos = end_pos += 1;
		if(SIP_SUCCESS == sip_find_next_crlf(start_pos,&end_pos))
		{
			(*callid)->host = (char *)sip_malloc(end_pos - start_pos-1);
			if((*callid)->host == NULL)
			{
				return SIP_NOMEM;
			}
			sip_str_cpy((*callid)->host,end_pos - start_pos-1,start_pos,end_pos - start_pos - 2);
		}
		else
		{
			return SIP_SYNTAXERROR;
		}
	}
	else
	{
		if(SIP_SUCCESS == sip_find_next_crlf(start_pos,&end_pos))
		{
			(*callid)->number= (char *)sip_malloc(end_pos - start_pos-1);
			if((*callid)->number == NULL)
			{
				return SIP_NOMEM;
			}
			sip_str_cpy((*callid)->number,end_pos - start_pos-1,start_pos,end_pos - start_pos - 2);
		}
		else
		{
			return SIP_SYNTAXERROR;
		}
	}
	return SIP_SUCCESS ;
}

//subject Subject: xxxxxxxxx0:0,xxxxxxxxxx1:1
int sip_message_parse_subject(  char *value  , sip_subject_t **subject )
{
	char *start_pos, *end_pos;
	if(value == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(SIP_SUCCESS != sip_message_subject_init(subject))
	{
		return SIP_BADPARAMETER;
	}
	start_pos = value;
	while( *start_pos == SIP_BLANK) start_pos ++;

	if(SIP_SUCCESS == sip_find_next_character( start_pos , &end_pos , ':' ) )
	{
		(*subject)->sender_id = (char *)sip_malloc(end_pos - start_pos + 1);
		sip_str_cpy((*subject)->sender_id,end_pos - start_pos+1,start_pos,end_pos - start_pos);
		start_pos = end_pos += 1;
	}else{
		return SIP_SYNTAXERROR;
	}
	if(SIP_SUCCESS == sip_find_next_character( start_pos , &end_pos , ',' ) )
	{
		(*subject)->sender_seq = (char *)sip_malloc(end_pos - start_pos + 1);
		sip_str_cpy((*subject)->sender_seq,end_pos - start_pos+1,start_pos,end_pos - start_pos);
		start_pos = end_pos += 1;
	}else{
		return SIP_SYNTAXERROR;
	}
	if(SIP_SUCCESS == sip_find_next_character( start_pos , &end_pos , ':' ) )
	{
		(*subject)->recver_id = (char *)sip_malloc(end_pos - start_pos + 1);
		sip_str_cpy((*subject)->recver_id,end_pos - start_pos+1,start_pos,end_pos - start_pos);
		start_pos = end_pos += 1;
	}else{
		return SIP_SYNTAXERROR;
	}
	if(SIP_SUCCESS == sip_find_next_crlf( start_pos , &end_pos ) )
	{
		(*subject)->recver_seq = (char *)sip_malloc(end_pos - start_pos + 1);
		sip_str_cpy((*subject)->recver_seq,end_pos - start_pos-1,start_pos,end_pos - start_pos-2);
		//start_pos = end_pos += 1;
	}else{
		return SIP_SYNTAXERROR;
	}
	return SIP_SUCCESS ;
}

/**************user_agent*************/
int sip_message_parse_user_agent( char *value , char **user_agent )
{
	char *start_pos, *end_pos;

	start_pos = value;
	if(value == NULL)
	{
		return SIP_BADPARAMETER;
	}
	while( *start_pos == SIP_BLANK) start_pos ++;
	if(SIP_SUCCESS == sip_find_next_crlf(start_pos, &end_pos))
	{
		*user_agent = (char *)sip_malloc(end_pos - start_pos - 1);
		if(*user_agent == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy(*user_agent,end_pos - start_pos - 1,start_pos,end_pos - start_pos - 2);
	}
	return SIP_SUCCESS ;
}
/**************user_agent*************/
int sip_message_parse_event( char *value , char **event )
{
	char *start_pos, *end_pos;

	start_pos = value;
	if(value == NULL)
	{
		return SIP_BADPARAMETER;
	}
	while( *start_pos == SIP_BLANK) start_pos ++;
	if(SIP_SUCCESS == sip_find_next_crlf(start_pos, &end_pos))
	{
		*event = (char *)sip_malloc(end_pos - start_pos - 1);
		if(*event == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy(*event,end_pos - start_pos - 1,start_pos,end_pos - start_pos - 2);
	}
	return SIP_SUCCESS ;
}
/**************subscription_state*************/
int sip_message_parse_subscription_state( char *value , char **subscription_state )
{
	char *start_pos, *end_pos;

	start_pos = value;
	if(value == NULL)
	{
		return SIP_BADPARAMETER;
	}
	while( *start_pos == SIP_BLANK) start_pos ++;
	if(SIP_SUCCESS == sip_find_next_crlf(start_pos, &end_pos))
	{
		*subscription_state = (char *)sip_malloc(end_pos - start_pos - 1);
		if(*subscription_state == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy(*subscription_state,end_pos - start_pos - 1,start_pos,end_pos - start_pos - 2);
	}
	return SIP_SUCCESS ;
}


/**************max_forwards*************/
int sip_message_parse_max_forwards(  char *value , char **max_forwards  )
{
	char *start_pos, *end_pos;
	if(value == NULL)
	{
		return SIP_BADPARAMETER;
	}
	start_pos = value;
	while( *start_pos == SIP_BLANK) start_pos ++;
	if(SIP_SUCCESS == sip_find_next_crlf(start_pos, &end_pos))
	{
		*max_forwards = (char *)sip_malloc(end_pos - start_pos - 1);
		if(*max_forwards == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy(*max_forwards,end_pos - start_pos - 1,start_pos,end_pos - start_pos - 2);
	}
	return SIP_SUCCESS ;
}

/**************expires*********************/
int sip_message_parse_expires(  char *value , char **expires  )
{	
	char *start_pos, *end_pos;
	if(value == NULL)
	{
		return SIP_BADPARAMETER;
	}	
	start_pos = value;
	while( *start_pos == SIP_BLANK) start_pos ++;
	if(SIP_SUCCESS == sip_find_next_crlf(start_pos, &end_pos))
	{
		*expires = (char *)sip_malloc(end_pos - start_pos - 1);
		if(*expires == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy(*expires,end_pos - start_pos - 1,start_pos,end_pos - start_pos - 2);
	}
	return SIP_SUCCESS ;
}

/*************Date***********************/
int sip_message_parse_date(  char *value ,char **date  )
{
	char *start_pos, *end_pos;
	if(value == NULL)
	{
		return SIP_BADPARAMETER;
	}
	start_pos = value;
	while( *start_pos == SIP_BLANK) start_pos ++;
	if(SIP_SUCCESS == sip_find_next_crlf(start_pos, &end_pos))
	{
		*date = (char *)sip_malloc(end_pos - start_pos - 1);
		if(*date == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy(*date,end_pos - start_pos - 1,start_pos,end_pos - start_pos - 2);
	}
	return SIP_SUCCESS ;
}
//Tue, 26 Mar 2002 17:17:00 GMT

int sip_message_get_time(char *date,sip_time_t *time)
{
	char  * start_pos, *end_pos ;
    int m = 0 ;
	char buf[5];
	const char *Month[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	if(date == NULL || time == NULL)
	{
		return SIP_BADPARAMETER;
	}
	start_pos = date;
	if(SIP_SUCCESS != sip_find_next_character(start_pos,&end_pos,','))
	{
		return SIP_SYNTAXERROR;
	}
	//day
	start_pos = end_pos += 2;
	if(SIP_SUCCESS != sip_find_next_character(start_pos,&end_pos,' '))
	{
		return SIP_SYNTAXERROR;
	}
	if((end_pos - start_pos) != 2)
	{
		return SIP_SYNTAXERROR;
	}
	sip_str_cpy(buf,3,start_pos,2);
	sscanf(buf,"%d",&(time->day));
	//mon
	start_pos = end_pos += 1;
	if(SIP_SUCCESS != sip_find_next_character(start_pos,&end_pos,' '))
	{
		return SIP_SYNTAXERROR;
	}
	if((end_pos - start_pos) != 3)
	{
		return SIP_SYNTAXERROR;
	}
	sip_str_cpy(buf,4,start_pos,3);
	for(m = 0; m < 12; m ++)
	{
		if(strcmp(Month[m],buf) == 0)
			break;
	}
	time->mon = m+1;

	//year
	start_pos = end_pos += 1;
	if(SIP_SUCCESS != sip_find_next_character(start_pos,&end_pos,' '))
	{
		return SIP_SYNTAXERROR;
	}
	if((end_pos - start_pos) != 4)
	{
		return SIP_SYNTAXERROR;
	}
	sip_str_cpy(buf,5,start_pos,4);
	sscanf(buf,"%d",&(time->year));

	//hour
	start_pos = end_pos += 1;
	if(SIP_SUCCESS != sip_find_next_character(start_pos,&end_pos,':'))
	{
		return SIP_SYNTAXERROR;
	}
	if((end_pos - start_pos) != 2)
	{
		return SIP_SYNTAXERROR;
	}
	sip_str_cpy(buf,3,start_pos,2);
	sscanf(buf,"%d",&(time->hour));
	//min
	start_pos = end_pos += 1;
	if(SIP_SUCCESS != sip_find_next_character(start_pos,&end_pos,':'))
	{
		return SIP_SYNTAXERROR;
	}
	if((end_pos - start_pos) != 2)
	{
		return SIP_SYNTAXERROR;
	}
	sip_str_cpy(buf,3,start_pos,2);
	sscanf(buf,"%d",&(time->min));
	//sec
	start_pos = end_pos += 1;
	if(SIP_SUCCESS != sip_find_next_character(start_pos,&end_pos,' '))
	{
		return SIP_SYNTAXERROR;
	}
	if((end_pos - start_pos) != 2)
	{
		return SIP_SYNTAXERROR;
	}
	sip_str_cpy(buf,3,start_pos,2);
	sscanf(buf,"%d",&(time->sec));
	
	return SIP_SUCCESS;
}
/***************www_authenticate****************/
/*
WWW-Authenticate: Digest realm="cmcc.com",domain="vss.cmcc.com",nonce="0f8afc8ddcbcf03fae686f85dbc3a703",algorithm=MD5,qop="auth"

*/

static int sip_message_find_quotevalue(char *start_pos,char **start_quote,char **end_quote, const char *param)
{
    char *end_pos, *token_pos;
	if(SIP_SUCCESS != sip_find_next_string(start_pos,&end_pos,param))
	{
		return SIP_NOTFOUND;
	}
	end_pos += strlen(param);
	if(SIP_SUCCESS != sip_find_quotes(end_pos,start_quote,end_quote))
	{
		return SIP_SYNTAXERROR;
	}
	if( SIP_SUCCESS == sip_find_next_character(end_pos,&token_pos,','))
	{
		if( token_pos < *end_quote)
		{
			return SIP_SYNTAXERROR;
		}
	}
	/*
	if(SIP_SUCCESS == sip_find_next_character(end_pos,&token_pos,'='))
	{	
		if( token_pos < *end_quote)
		{
			return SIP_SYNTAXERROR;
		}
	}*/
	return SIP_SUCCESS;
}

static int sip_mesage_find_value(char *start_pos,char **start_value,char **end_value, const char *param)
{
	char *end_pos,*token_pos;
	if(SIP_SUCCESS != sip_find_next_string(start_pos,&end_pos,param))
	{
		return SIP_NOTFOUND;
	}
	*start_value = end_pos += strlen(param);
	
	if( SIP_SUCCESS != sip_find_next_character(end_pos,&token_pos,','))
	{
		if( SIP_SUCCESS == sip_find_next_crlf(end_pos,&token_pos))
		{
			token_pos = token_pos - 2;
		}
		else
		{
			return SIP_SYNTAXERROR;
		}
    }
	*end_value = token_pos - 1;
	if(SIP_SUCCESS == sip_find_next_character(end_pos,&token_pos,'='))
	{	
		if( token_pos < *end_value )
		{
			return SIP_SYNTAXERROR;
		}
	}
	return SIP_SUCCESS;
}

int sip_message_parse_www_authenticate(  char *value , sip_www_authenticate_t **www_authenticate )
{
	char *start_pos, *end_pos;
	char *start_quote, *end_quote;
	char *start_value, *end_value;

	if(value == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(SIP_SUCCESS != sip_message_www_authenticate_init(www_authenticate))
	{
		return SIP_NOMEM;
	}
	start_pos = value;
	while( *start_pos == SIP_BLANK) start_pos ++;
	if(SIP_SUCCESS == sip_find_next_character(start_pos,&end_pos,' '))
	{
		(*www_authenticate)->auth_type = (char *)sip_malloc(end_pos - start_pos + 1);
		if((*www_authenticate)->auth_type == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*www_authenticate)->auth_type,end_pos - start_pos + 1,start_pos,end_pos - start_pos);
	}
	start_pos = end_pos +=1;

	if(SIP_SUCCESS == sip_message_find_quotevalue(start_pos,&start_quote,&end_quote,"realm="))
	{
		(*www_authenticate)->realm = (char *)sip_malloc(end_quote - start_quote );
		if((*www_authenticate)->realm == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*www_authenticate)->realm,end_quote - start_quote ,start_quote+1,end_quote - start_quote - 1);
	}
	
	if(SIP_SUCCESS == sip_message_find_quotevalue(start_pos,&start_quote,&end_quote,"domain="))
	{
		(*www_authenticate)->domain = (char *)sip_malloc(end_quote - start_quote );
		if((*www_authenticate)->domain == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*www_authenticate)->domain,end_quote - start_quote ,start_quote+1,end_quote - start_quote - 1);
	}

	if(SIP_SUCCESS == sip_message_find_quotevalue(start_pos,&start_quote,&end_quote,"nonce="))
	{
		(*www_authenticate)->nonce = (char *)sip_malloc(end_quote - start_quote );
		if((*www_authenticate)->nonce == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*www_authenticate)->nonce,end_quote - start_quote ,start_quote+1,end_quote - start_quote - 1);
	}

	if(SIP_SUCCESS == sip_message_find_quotevalue(start_pos,&start_quote,&end_quote,"opaque="))
	{
		(*www_authenticate)->opaque = (char *)sip_malloc(end_quote - start_quote );
		if((*www_authenticate)->opaque == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*www_authenticate)->opaque,end_quote - start_quote ,start_quote+1,end_quote - start_quote - 1);
	}
	
	if(SIP_SUCCESS == sip_message_find_quotevalue(start_pos,&start_quote,&end_quote,"qop="))
	{
		(*www_authenticate)->qop = (char *)sip_malloc(end_quote - start_quote );
		if((*www_authenticate)->qop == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*www_authenticate)->qop,end_quote - start_quote ,start_quote+1,end_quote - start_quote - 1);
	}

	if(SIP_SUCCESS  == sip_mesage_find_value(start_pos,&start_value,&end_value,"stale="))
	{
		(*www_authenticate)->stale = (char *)sip_malloc(end_value - start_value + 2);
		if((*www_authenticate)->stale == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*www_authenticate)->stale,end_value - start_value + 2,start_value,end_value - start_value + 1);
	}
	
	if(SIP_SUCCESS	== sip_mesage_find_value(start_pos,&start_value,&end_value,"algorithm="))
	{
		(*www_authenticate)->algorithm = (char *)sip_malloc(end_value - start_value + 2);
		if((*www_authenticate)->algorithm == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*www_authenticate)->algorithm,end_value - start_value + 2,start_value,end_value - start_value + 1);
	}

	return SIP_SUCCESS;
}


/***********authenticate***********/
//authorization function

/*
Authorization: Digest username="010001010500020109@js.qly", realm="js.qly", nonce="DiNgix1LO0zuvI1skkZfgQAAAAARaxERzJzmsGp8FpY", uri="sip:js.qly", response="5614bd1170f6ea16e50f00988aa362a9", cnonce="179661552", algorithm=MD5, opaque="", qop=auth, nc=00000001

*/
int sip_message_parse_authorization(  char *value , sip_authorization_t **authorization ) 
{
	char *start_pos, *end_pos;
	char *start_quote, *end_quote;
	char *start_value, *end_value;

	if(value == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(SIP_SUCCESS != sip_message_authorization_init(authorization))
	{
		return SIP_NOMEM;
	}
	start_pos = value;
	while( *start_pos == SIP_BLANK) start_pos ++;
	if(SIP_SUCCESS == sip_find_next_character(start_pos,&end_pos,' '))
	{
		(*authorization)->auth_type = (char *)sip_malloc(end_pos - start_pos + 1);
		if((*authorization)->auth_type == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*authorization)->auth_type,end_pos - start_pos + 1,start_pos,end_pos - start_pos);
		//printf("%s\n",(*authorization)->auth_type);
	}
	start_pos = end_pos +=1;

	
	if(SIP_SUCCESS == sip_message_find_quotevalue(start_pos,&start_quote,&end_quote,"username="))
	{
		(*authorization)->username = (char *)sip_malloc(end_quote - start_quote );
		if((*authorization)->username == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*authorization)->username,end_quote - start_quote ,start_quote+1,end_quote - start_quote - 1);
		//printf("username : %s\n",(*authorization)->username);
	}
	if(SIP_SUCCESS == sip_message_find_quotevalue(start_pos,&start_quote,&end_quote,"realm="))
	{
		(*authorization)->realm = (char *)sip_malloc(end_quote - start_quote );
		if((*authorization)->realm == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*authorization)->realm,end_quote - start_quote ,start_quote+1,end_quote - start_quote - 1);
		//printf("realm : %s\n",(*authorization)->realm);
	}
	
	if(SIP_SUCCESS == sip_message_find_quotevalue(start_pos,&start_quote,&end_quote,"nonce="))
	{
		(*authorization)->nonce = (char *)sip_malloc(end_quote - start_quote );
		if((*authorization)->nonce == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*authorization)->nonce,end_quote - start_quote ,start_quote+1,end_quote - start_quote - 1);
		//printf("nonce : %s\n",(*authorization)->nonce);
	}
	if(SIP_SUCCESS == sip_message_find_quotevalue(start_pos,&start_quote,&end_quote,"uri="))
	{
		(*authorization)->uri = (char *)sip_malloc(end_quote - start_quote );
		if((*authorization)->uri == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*authorization)->uri,end_quote - start_quote ,start_quote+1,end_quote - start_quote - 1);
		//printf("uri : %s\n",(*authorization)->uri);
	}
	if(SIP_SUCCESS == sip_message_find_quotevalue(start_pos,&start_quote,&end_quote,"response="))
	{
		(*authorization)->response = (char *)sip_malloc(end_quote - start_quote );
		if((*authorization)->response == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*authorization)->response,end_quote - start_quote ,start_quote+1,end_quote - start_quote - 1);
		//printf("response : %s\n",(*authorization)->response);
	}
	if(SIP_SUCCESS == sip_message_find_quotevalue(start_pos,&start_quote,&end_quote,"cnonce="))
	{
		(*authorization)->cnonce = (char *)sip_malloc(end_quote - start_quote );
		if((*authorization)->cnonce == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*authorization)->cnonce,end_quote - start_quote ,start_quote+1,end_quote - start_quote - 1);
		//printf("cnonce : %s\n",(*authorization)->cnonce);
	}
	if(SIP_SUCCESS == sip_message_find_quotevalue(start_pos,&start_quote,&end_quote,"opaque="))
	{
		(*authorization)->opaque = (char *)sip_malloc(end_quote - start_quote );
		if((*authorization)->opaque == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*authorization)->opaque,end_quote - start_quote ,start_quote+1,end_quote - start_quote - 1);
		//printf("opaque : %s\n",(*authorization)->opaque);
	}
	
	if(SIP_SUCCESS	== sip_mesage_find_value(start_pos,&start_value,&end_value,"qop="))
	{
		(*authorization)->qop = (char *)sip_malloc(end_value - start_value + 2);
		if((*authorization)->qop == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*authorization)->qop,end_value - start_value + 2,start_value,end_value - start_value + 1);
		//printf("qop : %s\n",(*authorization)->qop);
	}
		if(SIP_SUCCESS	== sip_mesage_find_value(start_pos,&start_value,&end_value,"nc="))
	{
		(*authorization)->nonce_count = (char *)sip_malloc(end_value - start_value + 2);
		if((*authorization)->nonce_count == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*authorization)->nonce_count,end_value - start_value + 2,start_value,end_value - start_value + 1);
		//printf("nonce_count : %s\n",(*authorization)->nonce_count);
	}
	if(SIP_SUCCESS	== sip_mesage_find_value(start_pos,&start_value,&end_value,"algorithm="))
	{
		(*authorization)->algorithm = (char *)sip_malloc(end_value - start_value + 2);
		if((*authorization)->algorithm == NULL)
		{
			return SIP_NOMEM;
		}
		sip_str_cpy((*authorization)->algorithm,end_value - start_value + 2,start_value,end_value - start_value + 1);
		//printf("algorithm : %s\n",(*authorization)->algorithm);
	}
	return SIP_SUCCESS;
}

int sip_message_create_authorization(sip_message_t *sip_message,sip_authorization_t **authorization, 
	char * password, char * server_id, char * server_ip, int server_port)
{	
	int len;
	sip_uri_t *uri;
	char MD5[33];
	const char *alg = "MD5";
	
	if(sip_message == NULL || sip_message->from == NULL || sip_message->www_authenticate == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(sip_message->from->url == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(sip_message->www_authenticate->auth_type == NULL || sip_message->www_authenticate->realm == NULL
		|| sip_message->www_authenticate->nonce == NULL)
	{
		return SIP_BADPARAMETER;
	}
	if(SIP_SUCCESS != sip_message_authorization_init(authorization))
	{
		return SIP_NOMEM;
	}
	
	if(SIP_SUCCESS != sip_message_parse_uri(sip_message->from->url,&uri))
	{
		sip_message_uri_free(uri);
		return SIP_UNDEFINED_ERROR;
	}
	//auth_type
	len = strlen(sip_message->www_authenticate->auth_type);
	(*authorization)->auth_type = (char *)sip_malloc(len + 1);
	if((*authorization)->auth_type == NULL)
	{
		sip_message_uri_free(uri);
		return SIP_NOMEM;
	}
	sip_str_cpy((*authorization)->auth_type,len + 1,sip_message->www_authenticate->auth_type,len);
	//printf("%s\n",(*authorization)->auth_type);

	
	//usename
	len = strlen(uri->client_id);
	(*authorization)->username = (char *)sip_malloc(len + 1);
	if((*authorization)->username == NULL)
	{
		sip_message_uri_free(uri);
		return SIP_NOMEM;
	}
	sip_str_cpy((*authorization)->username,len + 1,uri->client_id,len);
	//printf("%s\n",(*authorization)->username);

	
	//realm
	len = strlen(sip_message->www_authenticate->realm);
	(*authorization)->realm = (char *)sip_malloc(len + 1);
	if((*authorization)->realm == NULL)
	{
		sip_message_uri_free(uri);
		return SIP_NOMEM;
	}
	sip_str_cpy((*authorization)->realm,len + 1,sip_message->www_authenticate->realm,len);
	//printf("%s\n",(*authorization)->realm);

	
	//nonce
	len = strlen(sip_message->www_authenticate->nonce);
	(*authorization)->nonce = (char *)sip_malloc(len + 1);
	if((*authorization)->nonce == NULL)
	{
		sip_message_uri_free(uri);
		return SIP_NOMEM;
	}
	sip_str_cpy((*authorization)->nonce,len + 1,sip_message->www_authenticate->nonce,len);
	//printf("%s\n",(*authorization)->nonce);

	
	//uri
	len = strlen("sip:");
	(*authorization)->uri = (char *)sip_malloc(len + 200);
	if((*authorization)->uri == NULL)
	{
		sip_message_uri_free(uri);
		return SIP_NOMEM;
	}
	sprintf((*authorization)->uri,"sip:%s@%s:%d" , server_id , server_ip , server_port );

	//response
	get_md5("REGISTER" , uri->client_id , (*authorization)->uri , sip_message->www_authenticate->realm ,
							sip_message->www_authenticate->nonce , password , MD5);
	len = strlen(MD5);
	(*authorization)->response = (char *)sip_malloc(len + 1);
	if((*authorization)->response == NULL)
	{
		sip_message_uri_free(uri);
		return SIP_NOMEM;
	}
	sip_str_cpy((*authorization)->response,len+1,MD5,len);
	//printf("%s\n",(*authorization)->response);

	
	//cnonce
	//len =  strlen(cn);
	/*(*authorization)->cnonce = (char *)sip_malloc(len + 1);
	if((*authorization)->cnonce == NULL)
	{
		sip_message_uri_free(uri);
		return SIP_NOMEM;
	}
	//sip_str_cpy((*authorization)->cnonce,len + 1,cn,len);
	//printf("%s\n",(*authorization)->cnonce);
	*/
	//alg
	len = strlen(alg);
	(*authorization)->algorithm = (char *)sip_malloc(len + 1);
	if((*authorization)->algorithm == NULL)
	{
		sip_message_uri_free(uri);
		return SIP_NOMEM;
	}
	sip_str_cpy((*authorization)->algorithm,len + 1,alg,len);
	//opaque
	/*if(sip_message->www_authenticate->opaque != NULL)
	{
		len = strlen(sip_message->www_authenticate->opaque);
		(*authorization)->opaque = (char *)sip_malloc(len + 1);
		if((*authorization)->opaque == NULL)
		{
			sip_message_uri_free(uri);
			return SIP_NOMEM;
		}
		sip_str_cpy((*authorization)->opaque,len + 1,sip_message->www_authenticate->opaque,len);
	}
	else
	{
		(*authorization)->opaque = (char *)sip_malloc(1);
		if((*authorization)->opaque == NULL)
		{
			sip_message_uri_free(uri);
			return SIP_NOMEM;
		}
		sip_str_cpy((*authorization)->opaque,1,"",0);
	}
	//printf("%s\n",(*authorization)->opaque);
	//qop,only support auth
	len = strlen("auth");
	(*authorization)->qop = (char *)sip_malloc(len + 1);
	if((*authorization)->qop == NULL)
	{
		sip_message_uri_free(uri);
		return SIP_NOMEM;
	}
	sip_str_cpy((*authorization)->qop,len + 1,"auth",len);
	//printf("%s\n",(*authorization)->qop);
	*/
	sip_message_uri_free(uri);
	return SIP_SUCCESS;
}

//add end


