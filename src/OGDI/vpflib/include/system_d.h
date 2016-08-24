/* SYSTEM_D.H */

#ifndef H_SYSTEM_D
#define H_SYSTEM_D

#include <stdio.h>

#ifndef INCL_XVTH
#include "xvt.h"		/* to define the conditional compilation
				 * constants */
#endif				/* INCL_XVTH */

#ifndef H_MUSE1
#include "muse1.h"
#endif
#ifndef _MACHINE_
#include "machine.h"
#endif


/***************************************************************
@    DATA_TYPE
****************************************************************
Type of data contained in a RASTER object
*/
typedef enum
{
    COLOR_INDIRECT,		/* color index numbers */
    COLOR_RGB,			/* rgb values */
    DATA,			/* non-color data values */
    GRAY_SCALE			/* gray scale values */
}
                DATA_TYPE;

/*
 * Description: COLOR_INDIRECT rasters contain a matrix of color-map indices.
 * COLOR_RGB rasters contain an RGB true-color image. DATA rasters contain
 * data which must be rendered before being displayed.
 */


/***************************************************************
@    NUM_TYPE
****************************************************************
Types of numbers
*/
typedef enum
{
    ONEBIT,			/* binary data (0 or 1) */
    UINT_4BIT,			/* unsigned 4 bit data (0 -> 15) */
    SINT_8BIT,			/* signed 8 bit data (-128 -> 127) */
    UINT_8BIT,			/* unsigned 8 bit data (0 -> 255) */
    SINT_16BIT,			/* signed 16 bit (-32768 -> 32767) */
    UINT_16BIT,			/* unsigned 16 bit ( 0 -> 64K) */
    DTED_16BIT,			/* Univac signed magnitude */
    RGB_24BIT,			/* RGB (0 -> 255) interleaved by pixel */
    RGB_48BIT,			/* RGB (0 -> 65535) interleaved by pixel */
    SINT_32BIT,			/* signed 32 bit */
    UINT_32BIT,			/* unsigned 32 bit */
    IEEE_32BIT,			/* same as float */
    IEEE_64BIT			/* same as double */
}
                NUM_TYPE;



/****************************************************************/
/* THINK C for the Macintosh II running MAC OS          */
/****************************************************************/

#if XVT_OS == XVT_OS_MAC
/* Standard THINK C libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define DTED_EXT ".DT1;1"
#define PAL_OFFSET 0
#endif				/* if XVT_OS == XVT_OS_MAC */


/****************************************************************/
/* TURBO C for the IBM PC running MS-DOS             */
/****************************************************************/

#if XVT_OS == XVT_OS_WIN
/* Standard TURBO C header files */

#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <fcntl.h>
#include <process.h>

/* Other useful macros */
#define TRUE    1
#define FALSE   0
#define DLongSwap(a,b)

#define DTED_EXT ".DT1"
#define PAL_OFFSET 0
#endif				/* if XVT_OS == XVT_OS_WIN */


/****************************************************************/
/* Sun Workstation                        */
/****************************************************************/

#if XVT_OS_IS_SUNOS
/* Standard SUN C headers */
#include <stdio.h>
#include <fcntl.h>
#include <math.h>

/* Other useful SUN macros and typedefs */
#define TRUE      1
#define FALSE     0
#define SEEK_SET  0
#define SEEK_CUR  1
#define SEEK_END  2
#define O_BINARY  0

/* typedef unsigned short size_t; */

#define DTED_EXT ".dt1"
#define PAL_OFFSET 38
#endif				

/****************************************************************/
/* SGI Workstation                        */
/****************************************************************/

#if XVT_OS == XVT_OS_IRIX
/* Standard IRIX C headers */
#include <stdio.h>
#include <fcntl.h>
#include <math.h>

/* Other useful SGI macros and typedefs */
#define TRUE      1
#define FALSE     0
#define SEEK_SET  0
#define SEEK_CUR  1
#define SEEK_END  2
#define O_BINARY  0

/* typedef unsigned short size_t; */

#define DTED_EXT ".dt1"
#define PAL_OFFSET 38
#endif	    

/****************************************************************/
/* HP Workstation                        */
/****************************************************************/

#if XVT_OS == XVT_OS_HPUX
/* Standard HP headers */
#include <stdio.h>
#include <fcntl.h>
#include <math.h>

/* Other useful HP  macros and typedefs */
#define TRUE      1
#define FALSE     0
#define SEEK_SET  0
#define SEEK_CUR  1
#define SEEK_END  2
#define O_BINARY  0

#define DTED_EXT ".dt1"
#define PAL_OFFSET 38
#endif	

/****************************************************************/
/* SCO Workstation                        */
/****************************************************************/

#if XVT_OS == XVT_OS_SCOUNIX
/* Standard SCO headers */
#include <stdio.h>
#include <fcntl.h>
#include <math.h>

/* Other useful SCO  macros and typedefs */
#define TRUE      1
#define FALSE     0
#define SEEK_SET  0
#define SEEK_CUR  1
#define SEEK_END  2
#define O_BINARY  0

#define DTED_EXT ".dt1"
#define PAL_OFFSET 38

typedef unsigned int size_t;
#endif	

/****************************************************/
/* SPECIFY FILE TYPES                               */
/****************************************************/

#define FT_LUT	                "lut"
#define FT_MAP			"map"
#define FT_PAL			"pal"
#define FT_RAS			"ras"
#define FT_RAS_ADRG		"adr"
#define FT_RAS_ADRI             "adi"
#define FT_RAS_CAC		"cac"
#define FT_RAS_CIB		"cib"
#define FT_RAS_CADRG	        "cad"
#define FT_RAS_CRG		"crg"
#define FT_RAS_DATA		"ima"
#define FT_RAS_DBDB5	        "dbd"
#define FT_RAS_ASRP	        "asr"
#define FT_RAS_USRP	        "usr"
#define FT_RAS_SRG	        "srg"
#define FT_DTC		        "dtc"
#define FT_TXT		        "txt"
#define FT_RAS_DTED		"dte"
#define FT_VEC		        "vec"
#define	FT_UNKNOWN		"\0"

/****************************************************/
/* PROCESSOR BYTE ORDER DEPENDENCIES                */
/****************************************************/

#if  (XVT_OS == XVT_OS_MAC) || (XVT_OS_IS_SUNOS) || (XVT_OS == XVT_OS_IRIX)
/* Motorola, Sparc, or other BigEndian processors */

#define BigEndianToShortArray(byteCount,shortPtr)
#define BigEndianToLongArray(byteCount,longPtr)

#if XVT_CC_PROTO
void            LittleEndianToLongArray(int32 byteCount, int32 *longPtr);
void            LittleEndianToShortArray(int32 byteCount, short *shortPtr);
void            SignedMagnitudeToShortArray(int32 byteCount, short *shortPtr);
#else
void            LittleEndianToLongArray();
void            LittleEndianToShortArray();
void            SignedMagnitudeToShortArray();
#endif

#endif	

#if (XVT_OS == XVT_OS_WIN) ||  (XVT_OS == XVT_OS_SCOUNIX) ||  (XVT_OS == XVT_OS_HPUX)
/* Intel, or LittleEndian processor */

#define LittleEndianToLongArray(s,d)
#define LittleEndianToShortArray(s,d)
#define BigEndianToShortArray(s, d)  swab((char *)(d), (char *)(d), (int)(s))
#define TwosComplimentToShortArray(s,d)

#if XVT_CC_PROTO
void            BigEndianToLongArray(int32 size, int32 *data);
void            SignedMagnitudeToShortArray(int32 size, short *data);
#else
void            BigEndianToLongArray();
void            SignedMagnitudeToShortArray();
#endif

#endif	

#if XVT_OS_IS_SUNOS
#include <sys/stdtypes.h>
#endif

#if XVT_OS == XVT_OS_MAC
#define FILE_SEP ':'
#elif XVT_OS == XVT_OS_WIN
#define FILE_SEP '\\'
#elif XVT_OS_ISUNIX || XVT_OS == XVT_OS_IRIX || XVT_OS == XVT_OS_HPUX
#define FILE_SEP '/'
#endif

#ifndef MUSE_DEBUG
#define muse_malloc(a, b) malloc(a)
#define muse_free(a, b) free(a)
#endif

/***************************************************************
@    Sign
****************************************************************
*/
#define Sign(x)        ( (x) < 0.0 ? -1 : 1 )

/*
 * Description:
 */

/***************************************************************
@    DivBy
****************************************************************
A set of bit shifting divide macros
*/
#define DivBy2(i)      ( (i) >>  1 )
#define DivBy4(i)      ( (i) >>  2 )
#define DivBy8(i)      ( (i) >>  3 )

/*
 * Description:
 * 
 */

/***************************************************************
@    Flip
****************************************************************
Byte fliping macros
*/
#define Flip(a)        ( sizeof(a) == 4 ? Flip4(a) : Flip2(a) )

#define Flip2(a)       ( ( ( (a) & 0x0000FF ) <<  8 ) | \
                         ( ( (a) & 0x00FF00 ) >>  8 )      )

#define Flip4(a)       ( ( ( (a) & 0x000000FF ) << 24 ) | \
                         ( ( (a) & 0x0000FF00 ) <<  8 ) | \
                         ( ( (a) & 0x00FF0000 ) >>  8 ) | \
                         ( ( (a) & 0xFF000000 ) >> 24 )      )

/*
 * Description: These macros are used by the "Endian" functions that
 * rearrange the bytes to allow integer data written on Motorola style
 * computers to be read on Intel style computers, and vice versa.
 */

/***************************************************************
@    ERROR CODES
****************************************************************
Error code values
*/

/*
 * Error Code Masks
 */

#define STAT_MASK_SEV       0xE000
#define STAT_MASK_GRP       0x1F00
#define STAT_MASK_COD       0x00FF

/*
 * Severity Codes
 */

#define STAT_SEV_NO_ERR     0x0000
#define STAT_SEV_MESSAGE    0x2000
#define STAT_SEV_WARNING    0x4000
#define STAT_SEV_ERROR      0x6000
#define STAT_SEV_FATAL      0x8000
#define STAT_SEV_CRITICAL   0xA000

/*
 * Group Codes
 */

#define STAT_GRP_NO_ERR     0x0000
#define STAT_GRP_SYSTEM     0x0100
#define STAT_GRP_FILE       0x0200
#define STAT_GRP_MATH       0x0300
#define STAT_GRP_DATA       0x0400
#define STAT_GRP_PARAM      0x0500

/*
 * No Error Group Error Codes
 */
#define STAT_SUCCESS        ( STAT_GRP_NO_ERR   | \
                   STAT_SEV_NO_ERR   | \
                   0x0000                 )
/* Error allocating memory */

/*
 * System Group Error Codes
 */

#define STAT_MEM_ALLOC_ERR  ( STAT_GRP_SYSTEM   | \
                   STAT_SEV_CRITICAL | \
                   0x0001                 )
/* Error allocating memory */
#define STAT_OS_ERROR       ( STAT_GRP_SYSTEM   | \
                   STAT_SEV_CRITICAL | \
                   0x0002                 )
/* An unknown operating system error */
#define STAT_SOFTWARE_BUG   ( STAT_GRP_SYSTEM   | \
                   STAT_SEV_CRITICAL | \
                   0x0003                 )
/* A possible bug in the software was found */
/* Abort condition */
#define STAT_ABORT          ( STAT_GRP_SYSTEM | \
                   STAT_SEV_MESSAGE | \
                   0x0004)

/*
 * File Group Error Codes
 */

#define STAT_FILE_CREAT_ERR ( STAT_GRP_FILE     | \
                   STAT_SEV_ERROR    | \
                   0x0001                 )
/* Unable to create file */
#define STAT_FILE_CLOSE_ERR ( STAT_GRP_FILE     | \
                   STAT_SEV_ERROR    | \
                   0x0002                 )
/* Unable to close file */
#define STAT_FILE_NOT_FOUND ( STAT_GRP_FILE     | \
                   STAT_SEV_ERROR    | \
                   0x0003                 )
/* Could not open file */
#define STAT_FILE_READ_ERR  ( STAT_GRP_FILE     | \
                   STAT_SEV_ERROR    | \
                   0x0004                 )
/* Error reading from file */
#define STAT_FILE_TYPE_ERR  ( STAT_GRP_FILE     | \
                   STAT_SEV_ERROR    | \
                   0x0005                 )
/* The specified file is of the wrong type */
#define STAT_FILE_WRITE_ERR ( STAT_GRP_FILE     | \
                   STAT_SEV_ERROR    | \
                   0x0006                 )
/* Error writing to a file */

/*
 * Math Group Error Codes
 */

#define STAT_DIV_BY_ZERO    ( STAT_GRP_MATH     | \
                   STAT_SEV_ERROR    | \
                   0x0001                 )
/* Attempt to divide by zero */

/*
 * Data Group Error Codes
 */

#define STAT_DATA_ERR       ( STAT_GRP_DATA     | \
                   STAT_SEV_FATAL    | \
                   0x0001                 )
/* Data index or corrupt data error */
#define STAT_NO_DATA        ( STAT_GRP_DATA     | \
                   STAT_SEV_WARNING  | \
                   0x0002                 )
/* No data for given location */

/*
 * Parameter Group Error Codes
 */

#define STAT_INV_PROJ       ( STAT_GRP_PARAM    | \
                   STAT_SEV_FATAL    | \
                   0x0001                 )
/* Invalid projection */
#define STAT_BAD_PARM       (                       \
                STAT_GRP_PARAM    | \
                STAT_SEV_ERROR    | \
                0x0002               \
                 )
#define STAT_PARM_OVERFLOW  (                    \
                STAT_GRP_PARAM | \
                STAT_SEV_ERROR | \
                0x0003            \
                 )
#define STAT_BAD_CLR_NAME   ( STAT_GRP_PARAM    | \
                STAT_SEV_FATAL    | \
                0x0004                 )
/* Unknown color name */

/*
 * Description: All MUSE functions return an error status code. The status
 * code STAT_SUCCESS is zero so a boolean test map be performed. The function
 * muse_error() can be called to handle the error code.
 */

/***************************************************************
@    FILE_CREATOR
****************************************************************
Associates data exchange files with FUSION program
*/
#define FILE_CREATOR "MUSE"

/*
 * Description: The data extractor programs should call the XVT function
 * set_file_type() to set the extracted file's creator to this value.
 */

/***************************************************************
@    GetErr
****************************************************************
*/
#define GerErrCode(status)      (status & STAT_MASK_COD)
#define GetErrGroup(status)     (status & STAT_MASK_GRP)
#define GetErrSeverity(status)  (status & STAT_MASK_SEV)

/*
 * Description:
 */


/***************************************************************
@    BITS_PER_BYTE
****************************************************************
All supported systems have an 8 bit byte.
*/
#define BITS_PER_BYTE 8L

/*
 * Description:
 * 
 */
/***************************************************************
@    pHMEM_NULL
****************************************************************
*/
#define pHMEM_NULL ((pvHMEM) 0L)
typedef void pHMEM pvHMEM;

/*
 * Description:
 */
#endif				/* H_SYSTEM_D */
