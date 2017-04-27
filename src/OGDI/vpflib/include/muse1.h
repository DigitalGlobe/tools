#ifndef H_MUSE1

#define H_MUSE1


/***************************************************************
@    MUSE_API
****************************************************************
Define the parameter _API at the compilers command line
to have all API functions compiled for use in Windows DLLs.
*/
#ifdef _API
#define MUSE_API FAR _export
#else
#define MUSE_API
#endif

/***************************************************************
@    Misc. Constants
****************************************************************
*/
#define ANIOTA                   1.0E-4

/***************************************************************
@    BYTE
****************************************************************
*/
#ifdef BYTE
#undef BYTE
#endif
typedef unsigned char BYTE;
/*
Description:
*/

/***************************************************************
@    ERRSTATUS
****************************************************************
*/
typedef short ERRSTATUS;
/*
Description:
*/

/***************************************************************
@    GFLOAT
****************************************************************
*/
typedef double GFLOAT;
/*
Description:
*/

/***************************************************************
@    pHMEM
****************************************************************
pointer to High MEMory
*/
#ifndef _WINDOWS
#if 0
#    if XVT_OS == XVT_OS_WIN
#        define pHMEM _huge * 
#    else
#    endif
#endif
#        define pHMEM *
#else
#    define pHMEM *
#endif
/*
Description:
PC's require programming backflips in order to use the
installed hardware memory.  A pHMEM is the pointer you
get when you lock down an item on the global heap.  See
XVT functions galloc, glock, gfree.
*/

/***************************************************************
@    USHORT
****************************************************************
*/
typedef unsigned short USHORT;
/*
Description:
*/
/*
 * @    Multiples of Pi
 */
#define PXPI                  3.14159265358979323846
#define PX2_PI                6.28318530717958647693
#define PXPI_OVER_2           1.57079632679489661923
#define PXDEGREES_TO_RADIANS  0.0174532925199432957692

/*
 * Description:
 */

/***************************************************************
@    NULL
****************************************************************
A long 0
*/
#ifndef NULL
#define NULL 0L
#endif
/*
Description:
*/
/*
#ifndef BOOLEAN
typedef int   BOOLEAN;
#endif
*/
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif



/***************************************************************
@    EXTENT
****************************************************************
The bounds of something
*/
typedef struct
{
    GFLOAT          left;	/* Leftmost extent of the dataset in DD   */
    GFLOAT          right;	/* Rightmost extent of the dataset in DD  */
    GFLOAT          bottom;	/* Bottommost extent of the dataset in DD */
    GFLOAT          top;	/* Topmost extent of the dataset in DD    */
}               EXTENT;

/*
 * Description:
 */




#endif /* H_MUSE1 */











