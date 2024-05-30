/******************************************************************************
 *
 *
 *
 *
 *
 *
 * ***************************************************************************/
#include "sip_std.h"
#include "sip_md5.h"
#include "get_random.h" 
#include <pthread.h>


#define NONCE_LEN 32


char g_str_tag[RANDOM_LENGTH]  ;
char g_str_call_id[RANDOM_LENGTH] ;
char g_str_branch[RANDOM_LENGTH] ;
char g_str_nonce[RANDOM_LENGTH] ;

volatile unsigned int  g_cseq = 1 ;
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;  

int get_new_random( )
{
	struct timeval time_value;

	memset(&time_value, 0, sizeof(struct timeval));
	if(gettimeofday(&time_value, NULL) != 0){
		printf("Get time error !\n");
		return GET_UNDEFINED_ERROR;
	}

	srand( time_value.tv_usec );
	return rand() ;
}

int get_new_cseq( )
{
	pthread_mutex_lock(&g_mutex); 

    int cseq = g_cseq += 2 ; //for register , we should do this
    cseq = g_cseq++;
    pthread_mutex_unlock(&g_mutex);
    return  cseq;
}


std::string get_new_branch(  )
{
    char str_branch[RANDOM_LENGTH] = {0};
	sprintf( str_branch , "z9hG4bK%010u" , ((get_new_random())<<4)/3 );
	return str_branch ;
}

std::string get_new_call_id( )
{
	char str_call_id[RANDOM_LENGTH] = {0};
	sprintf( str_call_id , "%010u" , (get_new_random()>>2)/5);
	return str_call_id ;
}

std::string get_new_tag(   )
{
	char str_tag[RANDOM_LENGTH] = {0};
    sprintf( str_tag , "%010u" , (unsigned int)get_new_random() );
	return str_tag;
}


void CvtHex( HASH Bin,  HASHHEX Hex)
{
	unsigned short i;

	for (i = 0; i < HASHLEN; i++) {
		unsigned char j;
		j = (Bin[i] >> 4) & 0xf;
		if (j <= 9)
			Hex[i * 2] = (j + '0');
		else
			Hex[i * 2] = (j + 'a' - 10);
		j = Bin[i] & 0xf;
		if (j <= 9)
			Hex[i * 2 + 1] = (j + '0');
		else
			Hex[i * 2 + 1] = (j + 'a' - 10);
	};
	Hex[HASHHEXLEN] = '\0';
}

void get_md5(const char *method , const char *username , const char *uri , 
        const char *realm , const char *nonce ,const char *password , char *out )
{
    sip_MD5_CTX context;	
    HASH digest1 , digest2; 
    HASHHEX str_result1 , str_result2;	

    sip_MD5Init (&context);	
    sip_MD5Update( &context , (unsigned char *)username, strlen(username) );
    sip_MD5Update(&context, (unsigned char *) ":", 1);
    sip_MD5Update( &context , (unsigned char *)realm, strlen(realm) ); 
    sip_MD5Update(&context, (unsigned char *) ":", 1);
    sip_MD5Update( &context , (unsigned char *)password, strlen(password) );
    sip_MD5Final ((unsigned char *)digest1, &context); 
    CvtHex(  digest1 , str_result1	);	
    sip_MD5Init (&context);
    sip_MD5Update( &context , (unsigned char *)method, strlen(method) );
    sip_MD5Update(&context, (unsigned char *) ":", 1); 
    sip_MD5Update( &context , (unsigned char *)uri, strlen(uri) );
    sip_MD5Final ((unsigned char *)digest2, &context); 
    CvtHex(  digest2 , str_result2	);	
    sip_MD5Init (&context);	
    sip_MD5Update( &context , (unsigned char *)str_result1, 32 );	
    sip_MD5Update(&context, (unsigned char *) ":", 1);    
    sip_MD5Update( &context , (unsigned char *)nonce, strlen(nonce) ); 
    sip_MD5Update(&context, (unsigned char *) ":", 1); 
    sip_MD5Update( &context , (unsigned char *)str_result2, 32 );	
    sip_MD5Final ((unsigned char *)digest2, &context);
    CvtHex(  digest2 , str_result2	);	
    strcpy( out , str_result2 ) ;
}

std::string get_new_nonce( )
{

	int i = 0;

	char str_nonce[RANDOM_LENGTH] = {0};
	srand((unsigned)time(NULL));
	for (i = 0; i < NONCE_LEN - 1; i++)
    {
    	int flag = 0;
        flag = rand() % 3;
        switch (flag)
        {
            case 0:  
                str_nonce[i] = 'A' + rand() % 26;
                break;
            case 1:
                str_nonce[i] = 'a' + rand() % 26;
                break;
            case 2:
                str_nonce[i] = '0' + rand() % 10;
                break;
            default:
                str_nonce[i] = '=';
                break;
        }
    }
    str_nonce[NONCE_LEN - 1] = '\0';
	return str_nonce;
}


