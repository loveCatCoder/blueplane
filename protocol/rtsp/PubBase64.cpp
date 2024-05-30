/*
 * =====================================================================================
 *
 *	Filename:  Pubbase64.c
 *
 *	Description:  
 *
 *	Version:  1.0
 *	Created:  11/26/2014
 *
 *	Author:  philip 
 *	Organization:  FIAMM(BEIJING) TECH.CO.,LTD. 
 *
 * =====================================================================================
*/


#include <stdio.h>	
#include <string.h>
#include "PubBase64.h"

const char STANDRAD_BASE_CODE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int base64encode(const unsigned char *lpString,unsigned  char *lpBuffer, int sLen ) 
{	
	int vLen = 0;
	const char *basecode = STANDRAD_BASE_CODE ;
	char addflag = '=' ;

	while(sLen > 0)		
	{	
		*lpBuffer++ = basecode[(lpString[0] >> 2 ) & 0x3F];	
		if(sLen > 2)	
		{	
			*lpBuffer++ = basecode[((lpString[0] & 3) << 4) | (lpString[1] >> 4)];
			*lpBuffer++ = basecode[((lpString[1] & 0xF) << 2) | (lpString[2] >> 6)];
			*lpBuffer++ = basecode[lpString[2] & 0x3F];
		}else
		{	
			switch(sLen)	
			{	
				case 1:
					*lpBuffer ++ = basecode[(lpString[0] & 3) << 4 ];
					*lpBuffer ++ = addflag;
					*lpBuffer ++ = addflag;
					break;
				case 2:
					*lpBuffer ++ = basecode[((lpString[0] & 3) << 4) | (lpString[1] >> 4)];
					*lpBuffer ++ = basecode[((lpString[1] & 0x0F) << 2) | (lpString[2] >> 6)];
					*lpBuffer ++ = addflag;
					break;
			}
		}
		lpString += 3;
		sLen -= 3;
		vLen +=4;
	}
	*lpBuffer = 0;
	return vLen;
}

inline char getchar_index(char c) 
{	
	if((c >= 'A') && (c <= 'Z'))
	{	
		return c - 'A';
	}else if((c >= 'a') && (c <= 'z'))
	{	
		return c - 'a' + 26;
	}else if((c >= '0') && (c <= '9'))
	{	
		return c - '0' + 52;
	}else if(c == '+' )
	{	
		return 62;
	}else if(c == '/' )
	{	
		return 63;
	}else if(c == '=' )
	{	
		return 0;
	}
	return 0;
}

int base64decode(unsigned  char *lpString, const unsigned char *lpSrc, int sLen )  
{	static char lpCode[4];
	int vLen = 0;
	
	if(sLen % 4)		
	{	
		lpString[0] = '\0';
		return -1;
	}
	while(sLen > 2)	
	{	
		lpCode[0] = getchar_index(lpSrc[0]);
		lpCode[1] = getchar_index(lpSrc[1]);
		lpCode[2] = getchar_index(lpSrc[2]);
		lpCode[3] = getchar_index(lpSrc[3]);

		*lpString++ = ((lpCode[0] << 2)&0xfc)|((lpCode[1] >> 4)&0x0f);
		*lpString++ = ((lpCode[1] << 4)&0xf0)|((lpCode[2] >> 2)&0x3f);
		*lpString++ = ((lpCode[2] << 6)&0xc0)|((lpCode[3]));

		vLen += 3 ;
		sLen -= 4;
		lpSrc += 4;
	}
	return vLen;
}
