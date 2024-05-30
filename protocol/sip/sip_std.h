/*********************************************************************************************
*
*
*
*
*
*********************************************************************************************/


#ifndef __SIP_STD_H__
#define __SIP_STD_H__

//#define VSIP_IPNC_DEVICE_SD18  1

//#define SIP_PTZ_DEBUG

////INCLUDE HEADER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/timeb.h>
#include <pthread.h>
#include <linux/sockios.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LOCAL_SIP_PORT      5070

#define DOMAIN_ENABLE       0

#define GB28181_PROGRAM_NAME		"GB28181"

#define gb_inflog_wrapper(format , ...) 	
#define gb_wrnlog_wrapper(format , ...) 	
#define gb_errlog_wrapper(format , ...) 	


#ifdef	DEBUG	
#define gb_dbglog_wrapper(...) \
	do \
	{ \
		time_t rawtime; \
		struct tm * ptm; \
		time(&rawtime); \
		ptm = localtime(&rawtime); \
		fprintf(stderr, "\033[%dm%02d%02d-%02d:%02d:%02d.%03ld|[D] |%s|%s|%d| ",37, ptm->tm_mon, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec , 0 , __FILE__ ,  __FUNCTION__ , __LINE__ );	\
		fprintf(stderr, __VA_ARGS__); \
		fprintf(stderr, "\033[0m\n"); \
	} \
	while(0);

#else
#define gb_dbglog_wrapper(format ,...)	
#endif




#define SIP_SUCCESS               0
#define SIP_UNDEFINED_ERROR      -1
#define SIP_BADPARAMETER         -2
#define SIP_WRONG_STATE          -3
#define SIP_NOMEM                -4
#define SIP_SYNTAXERROR          -5
#define SIP_NOTFOUND             -6
#define SIP_NODATA				 -7
#define SIP_NO_HEADER            -8

#define SIP_NO_NETWORK           -10


#ifdef __cplusplus
}
#endif
#endif


