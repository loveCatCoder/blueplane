/*
  The sip library implements the Session Initiation Protocol (SIP -rfc3261-)
  Copyright (C) 2001,2002,2003,2004,2005,2006,2007 Aymeric MOIZARD jack@atosc.org

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#ifndef _sip_PORT_H_
#define _sip_PORT_H_

/* Include necessary headers for sip */

#include <stdio.h>
#include <string.h>
#include "sip_common.h"
#include "sip_const.h"
#include "sip_list.h"
#ifdef __cplusplus
extern "C" {
#endif

#ifdef ENABLE_TRACE
#define sip_TRACE(P) P
#else
#define sip_TRACE(P) do {} while (0)
#endif

typedef enum _trace_level {
		TRACE_LEVEL0 = 0,
#define sip_FATAL    TRACE_LEVEL0
		TRACE_LEVEL1 = 1,
#define sip_BUG      TRACE_LEVEL1
		TRACE_LEVEL2 = 2,
#define sip_ERROR    TRACE_LEVEL2
		TRACE_LEVEL3 = 3,
#define sip_WARNING  TRACE_LEVEL3
		TRACE_LEVEL4 = 4,
#define sip_INFO1    TRACE_LEVEL4
		TRACE_LEVEL5 = 5,
#define sip_INFO2    TRACE_LEVEL5
		TRACE_LEVEL6 = 6,
#define sip_INFO3    TRACE_LEVEL6
		TRACE_LEVEL7 = 7,
#define sip_INFO4    TRACE_LEVEL7
		END_TRACE_LEVEL = 8
	} sip_trace_level_t;

//void *sip_malloc(size_t size);
//void *sip_realloc(void *, size_t size);
//void sip_free(void *);

char *sip_strncpy(char *dest, const char *src, size_t length);
char *sip_strdup(const char *ch);
char *sip_port_str_append(char *dst, const char *src);
char *sip_strn_append(char *dst, const char *src, size_t len);

int __sip_set_next_token(char **dest, char *buf, int end_separator,
							  char **next);
char *__sip_sdp_append_string(char *string, size_t size,
                                char *cur, const char *string_sip_to_append);
int sip_trace(char *fi, int li, sip_trace_level_t level, FILE * f,
				   char *chfr, ...);


#define sip_SUCCESS               0
#define sip_UNDEFINED_ERROR      -1
#define sip_BADPARAMETER         -2
#define sip_WRONG_STATE          -3
#define sip_NOMEM                -4
#define sip_SYNTAXERROR          -5
#define sip_NOTFOUND             -6
#define sip_API_NOT_INITIALIZED  -7
#define sip_NO_NETWORK           -10
#define sip_PORT_BUSY            -11
#define sip_UNKNOWN_HOST         -12
#define sip_DISK_FULL            -30
#define sip_NO_RIGHTS            -31
#define sip_FILE_NOT_EXIST       -32
#define sip_TIMEOUT              -50
#define sip_TOOMUCHCALL          -51
#define sip_WRONG_FORMAT         -52
#define sip_NOCOMMONCODEC        -53


#ifdef __cplusplus
}
#endif
#endif
