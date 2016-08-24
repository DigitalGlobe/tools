/*
@Viewstruct
*/
#ifndef H_VIEW_DEF
#define H_VIEW_DEF

#ifndef _WINDOWS
#ifndef INCL_XVTH
#include "xvt.h"
#endif

/***************************************************************
@    VIEW
****************************************************************
The structure of bitmap viewing parmeters
*/
typedef struct
{
    int32        magic;
    int  bitmap_xsize;
    int  bitmap_ysize;
    int  bitmap_xmin;
    int  bitmap_ymin;
    int  bitmap_xmax;
    int  bitmap_ymax;
    int  scroll_x;
    int  scroll_y;
    int  status;
    double zoomx;
    double zoomy;
    void *winstuff;
    BOOLEAN new_palette; /* has a palette been made? */
} VIEW;
/*
Description:
bit_maps may contain 1, 4, 8, or 24 bits per pixel.
The bit_map begins at the upper left corner of the image.
The order is row-major, left-to-right, then top-to-bottom.
Each row is 0 padded (if necessary) so that the next row
    begins on an even (2 byte) word.
The bit_map is useable directly on Macintosh and MS Windows.
*/

#ifdef _WINDOWS
/***************************************************************
@    WINVIEW
****************************************************************
A window list and corresponding view list.
*/
typedef struct
{   WINDOW list[100];
    VIEW view[100];
} WINVIEW;
#endif
/*
Description:

*/
#endif /* _WINDOWS */
#endif /* #ifndef H_VIEW_DEF */

