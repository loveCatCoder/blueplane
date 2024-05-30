/*********************************************************************************************
*
*
*
*
*
*********************************************************************************************/

#ifndef __SIP_PARSER_H__
#define __SIP_PARSER_H__

#include "sip_message.h"
#ifdef __cplusplus
extern "C" {
#endif
#define COMMON_BUFFER_SIZE	(2048)
#define HEADER_NAME_SIZE		(20)
#define HEADER_VALUE_SIZE	(300)

int sip_message_parser( char *strmsg , sip_message_t **sip_message  ) ;

int sip_message_parse_startline( char *value , sip_message_t *sip_message );

#ifdef __cplusplus
}
#endif
#endif

