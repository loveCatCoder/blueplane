/**************************************************************************
*
*
*
*
*
*************************************************************************/

#ifndef __SIP_TYPE_H__
#define __SIP_TYPE_H__

#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

#define SIP_SUCCESS      0
#define SIP_EFAIL		(-1)


#ifndef TRUE

typedef short   sipBool;                 ///< Boolean type
#define TRUE		((sipBool) 1)        ///< Boolean value : TRUE
#define FALSE		((sipBool) 0)        ///< Boolean value : FALSE

#endif

/* unsigned quantities */
typedef unsigned long long sipUint64;      ///< Unsigned 64-bit integer
typedef unsigned int sipUint32;            ///< Unsigned 32-bit integer
typedef unsigned short sipUint16;          ///< Unsigned 16-bit integer
typedef unsigned char sipUint8;            ///< Unsigned  8-bit integer

/* signed quantities */
typedef long long sipInt64;               ///< Signed 64-bit integer
typedef int sipInt32;                     ///< Signed 32-bit integer
typedef short sipInt16;                   ///< Signed 16-bit integer
typedef char sipInt8;                     ///< Signed  8-bit integer


#ifdef __cplusplus
}
#endif
#endif
