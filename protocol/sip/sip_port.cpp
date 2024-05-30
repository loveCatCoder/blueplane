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


#include <stdlib.h>
#include "sip_port.h"


/* __sip_set_next_token:
   dest is the place where the value will be allocated
   buf is the string where the value is searched
   end_separator is the character that MUST be found at the end of the value
   next is the final location of the separator + 1

   the element MUST be found before any "\r" "\n" "\0" and
   end_separator

   return -1 on error
   return 1 on success
*/
int __sip_set_next_token(char **dest, char *buf, int end_separator, char **next)
{
	char *sep;					/* separator */

	*next = NULL;

	sep = buf;
	while ((*sep != end_separator) && (*sep != '\0') && (*sep != '\r')
		   && (*sep != '\n'))
		sep++;
	if ((*sep == '\r') || (*sep == '\n')) {	/* we should continue normally only if this is the separator asked! */
		//if (*sep != end_separator)
		//	return sip_UNDEFINED_ERROR;
	}
	if (*sep == '\0')
		return sip_UNDEFINED_ERROR;	/* value must not end with this separator! */
	//if (sep == buf)
	//	return sip_UNDEFINED_ERROR;	/* empty value (or several space!) */

	*dest = (char *)sip_malloc(sep - (buf) + 1);
	if (*dest == NULL)
		return sip_NOMEM;
	sip_strncpy(*dest, buf, sep - buf);

	*next = sep + 1;			/* return the position right after the separator */
	return sip_SUCCESS;
}

/* append string_sip_to_append to string at position cur
   size is the current allocated size of the element
*/
char *__sip_sdp_append_string(char *string, size_t size, char *cur,
							   const char *string_sip_to_append)
{
	size_t length = strlen(string_sip_to_append);

	if (cur - string + length > size) {
		size_t length2;

		length2 = cur - string;
		string = (char *)sip_realloc(string, size + length + 10);
		cur = string + length2;	/* the initial allocation may have changed! */
	}
	sip_strncpy(cur, string_sip_to_append, length);
	return cur + strlen(cur);
}

int
sip_trace(char *fi, int li, sip_trace_level_t level, FILE * f, char *chfr, ...)
{
//#ifdef ENABLE_TRACE
#if 0
	va_list ap;
	int relative_time = 0;

#if (defined(WIN32)  && !defined(_WIN32_WCE)) || defined(__linux)
	static struct timeval start = { 0, 0 };
	struct timeval now;

	if (start.tv_sec == 0 && start.tv_usec == 0) {
		__sip_port_gettimeofday(&start, NULL);
	}
	__sip_port_gettimeofday(&now, NULL);

	relative_time = 1000 * (now.tv_sec - start.tv_sec);
	if (now.tv_usec - start.tv_usec > 0)
		relative_time = relative_time + ((now.tv_usec - start.tv_usec) / 1000);
	else
		relative_time = relative_time - 1 + ((now.tv_usec - start.tv_usec) / 1000);
#endif

#if !defined(WIN32) && !defined(SYSTEM_LOGGER_ENABLED)
	if (logfile == NULL && use_syslog == 0 && trace_func == NULL) {	/* user did not initialize logger.. */
		return 1;
	}
#endif

	if (tracing_table[level] == LOG_FALSE)
		return sip_SUCCESS;

	if (f == NULL && trace_func == NULL)
		f = logfile;

	VA_START(ap, chfr);

#if  defined(__VXWORKS_OS__) || defined(__rtems__)
	/* vxworks can't have a local file */
	f = stdout;
#endif

#ifdef ANDROID
	if (1) {
		int lev;

		switch(level){
	case sip_INFO3:	lev = ANDROID_LOG_DEBUG;	break;
	case sip_INFO4:	lev = ANDROID_LOG_DEBUG;	break;
	case sip_INFO2:	lev = ANDROID_LOG_INFO;	break;
	case sip_INFO1:	lev = ANDROID_LOG_INFO;	break;
	case sip_WARNING:	lev = ANDROID_LOG_WARN;	break;
	case sip_ERROR:	lev = ANDROID_LOG_ERROR;	break;
	case sip_BUG:	lev = ANDROID_LOG_FATAL;	break;
	case sip_FATAL:	lev = ANDROID_LOG_FATAL;	break;
	default:		lev = ANDROID_LOG_DEFAULT;	break;
		}
		__android_log_vprint(lev, "sip2", chfr, ap);
	}
#else
	if (f && use_syslog == 0) {
		if (level == sip_FATAL)
			fprintf(f, "| FATAL | %i <%s: %i> ", relative_time, fi, li);
		else if (level == sip_BUG)
			fprintf(f, "|  BUG  | %i <%s: %i> ", relative_time, fi, li);
		else if (level == sip_ERROR)
			fprintf(f, "| ERROR | %i <%s: %i> ", relative_time, fi, li);
		else if (level == sip_WARNING)
			fprintf(f, "|WARNING| %i <%s: %i> ", relative_time, fi, li);
		else if (level == sip_INFO1)
			fprintf(f, "| INFO1 | %i <%s: %i> ", relative_time, fi, li);
		else if (level == sip_INFO2)
			fprintf(f, "| INFO2 | %i <%s: %i> ", relative_time, fi, li);
		else if (level == sip_INFO3)
			fprintf(f, "| INFO3 | %i <%s: %i> ", relative_time, fi, li);
		else if (level == sip_INFO4)
			fprintf(f, "| INFO4 | %i <%s: %i> ", relative_time, fi, li);

		vfprintf(f, chfr, ap);

		fflush(f);
	} else if (trace_func) {
		trace_func(fi, li, level, chfr, ap);
	}
#endif
#if defined (HAVE_SYSLOG_H) && !defined(__arc__)
	else if (use_syslog == 1) {
		char buffer[512];
		int in = 0;

		memset(buffer, 0, sizeof(buffer));
		if (level == sip_FATAL)
			in = snprintf(buffer, 511, "| FATAL | <%s: %i> ", fi, li);
		else if (level == sip_BUG)
			in = snprintf(buffer, 511, "|  BUG  | <%s: %i> ", fi, li);
		else if (level == sip_ERROR)
			in = snprintf(buffer, 511, "| ERROR | <%s: %i> ", fi, li);
		else if (level == sip_WARNING)
			in = snprintf(buffer, 511, "|WARNING| <%s: %i> ", fi, li);
		else if (level == sip_INFO1)
			in = snprintf(buffer, 511, "| INFO1 | <%s: %i> ", fi, li);
		else if (level == sip_INFO2)
			in = snprintf(buffer, 511, "| INFO2 | <%s: %i> ", fi, li);
		else if (level == sip_INFO3)
			in = snprintf(buffer, 511, "| INFO3 | <%s: %i> ", fi, li);
		else if (level == sip_INFO4)
			in = snprintf(buffer, 511, "| INFO4 | <%s: %i> ", fi, li);

		vsnprintf(buffer + in, 511 - in, chfr, ap);
		if (level == sip_FATAL)
			syslog(LOG_ERR, "%s", buffer);
		else if (level == sip_BUG)
			syslog(LOG_ERR, "%s", buffer);
		else if (level == sip_ERROR)
			syslog(LOG_ERR, "%s", buffer);
		else if (level == sip_WARNING)
			syslog(LOG_WARNING, "%s", buffer);
		else if (level == sip_INFO1)
			syslog(LOG_INFO, "%s", buffer);
		else if (level == sip_INFO2)
			syslog(LOG_INFO, "%s", buffer);
		else if (level == sip_INFO3)
			syslog(LOG_DEBUG, "%s", buffer);
		else if (level == sip_INFO4)
			syslog(LOG_DEBUG, "%s", buffer);
	}
#endif
#ifdef SYSTEM_LOGGER_ENABLED
	else {
		char buffer[512];
		int in = 0;
#ifdef DISPLAY_TIME
		int relative_time;
#endif

		memset(buffer, 0, sizeof(buffer));
		if (level == sip_FATAL)
			in = _snprintf(buffer, 511, "| FATAL | %i <%s: %i> ", relative_time,
						   fi, li);
		else if (level == sip_BUG)
			in = _snprintf(buffer, 511, "|  BUG  | %i <%s: %i> ", relative_time,
						   fi, li);
		else if (level == sip_ERROR)
			in = _snprintf(buffer, 511, "| ERROR | %i <%s: %i> ", relative_time,
						   fi, li);
		else if (level == sip_WARNING)
			in = _snprintf(buffer, 511, "|WARNING| %i <%s: %i> ", relative_time,
						   fi, li);
		else if (level == sip_INFO1)
			in = _snprintf(buffer, 511, "| INFO1 | %i <%s: %i> ", relative_time,
						   fi, li);
		else if (level == sip_INFO2)
			in = _snprintf(buffer, 511, "| INFO2 | %i <%s: %i> ", relative_time,
						   fi, li);
		else if (level == sip_INFO3)
			in = _snprintf(buffer, 511, "| INFO3 | %i <%s: %i> ", relative_time,
						   fi, li);
		else if (level == sip_INFO4)
			in = _snprintf(buffer, 511, "| INFO4 | %i <%s: %i> ", relative_time,
						   fi, li);

		_vsnprintf(buffer + in, 511 - in, chfr, ap);
#ifdef UNICODE
		{
			WCHAR wUnicode[1024];
			MultiByteToWideChar(CP_UTF8, 0, buffer, -1, wUnicode,
								1024);
			OutputDebugString(wUnicode);
		}
#else
		OutputDebugString(buffer);
#endif
	}
#endif

	va_end(ap);
#endif
	return sip_SUCCESS;
}




char *sip_port_str_append(char *dst, const char *src)
{
	while (*src != '\0') {
		*dst = *src;
		src++;
		dst++;
	}
	*dst = '\0';
	return dst;
}

/* ---For better performance---
   Same as above, only this time we know the length */
char *sip_strn_append(char *dst, const char *src, size_t len)
{
	memmove((void *) dst, (void *) src, len);
	dst += len;
	*dst = '\0';
	return dst;
}


char *sip_strncpy(char *dest, const char *src, size_t length)
{
	strncpy(dest, src, length);
	dest[length] = '\0';
	return dest;
}

char *sip_strdup(const char *ch)
{
	char *copy;
	size_t length;

	if (ch == NULL)
		return NULL;
	length = strlen(ch);
	copy = (char *) sip_malloc(length + 1);
	if (copy == NULL)
		return NULL;
	sip_strncpy(copy, ch, length);
	return copy;
}
/*
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
}*/
