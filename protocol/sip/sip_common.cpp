/*******************************************************************************************
*		
*		
*		
*		
*			
*******************************************************************************************/
#include "sip_std.h"
#include "sip_common.h"



//string function
int sip_find_next_crlf( char *start_pos , char **end_pos )
{
	char *soh = start_pos ;

	if( start_pos == NULL  )
		return SIP_BADPARAMETER ;
	while(soh[0] != 0 )
	{
		if(  soh[0] == SIP_CR && soh[1] == SIP_LF )
			goto find_end ;
		soh ++ ;
	}
	return SIP_NOTFOUND ;
	
find_end :
	soh += 2 ;
	*end_pos = soh ;
	return SIP_SUCCESS ;
}


int sip_find_next_character( char *start_pos , char **end_pos ,const char ch)
{
/*
	//start_pos must = find character
	if( start_pos == NULL )//modify
	{
		return SIP_BADPARAMETER ;
	}
	start_pos ++ ;

	while( *start_pos != ch && start_pos != 0) start_pos ++ ;
    
	if( start_pos == 0 )
		return SIP_NOTFOUND ;
	else
		*end_pos = start_pos ;
*/
	if( start_pos == NULL)
		return SIP_BADPARAMETER ;
	*end_pos = strchr( start_pos , ch );
	if( *end_pos == NULL )
		return SIP_NOTFOUND ;
	return SIP_SUCCESS ;
}


int sip_find_next_string( char *start_pos , char **end_pos ,const char *str )
{
	if( start_pos == NULL || str == NULL  )
		return SIP_BADPARAMETER ;
	*end_pos = strstr( start_pos , str );
	if( *end_pos == NULL )
		return SIP_NOTFOUND ;
	return SIP_SUCCESS ;
}
//add  20121026
int sip_find_quotes(char *pos,char **start_quote,char **end_quote)
{
	if( pos == NULL)
		return SIP_BADPARAMETER;
	*start_quote = strchr( pos, '"');
	//printf("1_1\n");
	if( *start_quote == NULL )
		return SIP_NOTFOUND;
	//printf("1_2\n");
	*end_quote = strchr( (*start_quote + 1), '"');
	//printf("1_3\n");
	if( *end_quote == NULL)
		return SIP_NOTFOUND;
	return SIP_SUCCESS;
}
//add  end
int sip_str_find_end( char *start_pos , char **end_pos )
{
	char *soh = start_pos ;

	if( start_pos == NULL  )
		return SIP_BADPARAMETER ;
	
	if( soh == 0 ) return SIP_NODATA ;

	while( *soh != 0 ) soh ++ ;

	*end_pos = soh ;

	return SIP_SUCCESS ;
}

int sip_str_cpy( char *dest , int max , const char *source , int num )
{
	if( dest == NULL || source == NULL )
		return SIP_BADPARAMETER ;
	memset( dest , 0 , max );
	if(max > num)
	{
		strncpy( dest , source , num ) ;
		dest[num] = '\0';
	}
	else if(max > 1)
	{
		strncpy( dest , source , max ) ;
		dest[max - 1] = '\0';
	}

	//add 20121022
	//add end
	return SIP_SUCCESS ;
}

int sip_str_append(char **dest,const char *source)
{
	char *tmp;
	if( dest == NULL || source == NULL )
		return SIP_BADPARAMETER ;
	tmp = *dest;
	memset( tmp , 0 , strlen(source));
	strncpy(tmp,source,strlen(source));
	tmp += strlen(source);
	*tmp = '\0';
	*dest = tmp;
	return SIP_SUCCESS;
}

//memory function

void *sip_malloc(size_t size)
{
	void *ptr = malloc(size);

	if (ptr != NULL)
		memset(ptr, 0, size);
	return ptr;
}

void *sip_realloc(void *ptr, size_t size)
{
	return realloc(ptr, size);
}

void sip_free(void *ptr)
{
	if (ptr == NULL)
		return;
	free(ptr);
}

//time control
 void sip_msleep( int msec )
{
	struct timeval g_delay;

	g_delay.tv_sec = 0;
	g_delay.tv_usec = msec * 1000; 
	select(0, NULL,NULL, NULL, &g_delay);

}





