#ifndef H_BMP
#ifndef INCL_XVTH
#include "xvt.h"
#endif


#if XVT_OS == XVT_OS_WIN3
#ifndef __WINDOWS_H
#include "windows.h"
#endif
#else
typedef short WORD;
typedef int32 DWORD;
#define BI_RGB  0
#define BI_RLE8 1
#define BI_RLE4 2
#endif
typedef char BYTE1;


/* BITMAPFILEHEADER structure */

typedef struct tag_BITMAP_FILEHEADER
   {
   WORD  bfType;               /* The type of bitmap */
   DWORD bfSize;               /* The size in DWORDS of the file */
   WORD  bfReserved1;          /* Reserved zero */
   WORD  bfReserved2;          /* Reserved zero */
   DWORD bfOffBits;            /* The offset in bytes from the 
                                  BITMAPFILEHEADER to the actual bitmap */
   } BITMAP_FILEHEADER;

/* BITMAPINFOHEADER structure */

typedef struct tag_BITMAP_INFOHEADER
   {
   DWORD biSize;               /* The size of the structure */
   DWORD biWidth;              /* The width of the bitmap */
   DWORD biHeight;             /* The height of the bitmap */
   WORD  biPlanes;             /* The number of planes = 1 */
   WORD  biBitCount;           /* The number of bits per pixel 1 4 8 24 */
   DWORD biCompression;        /* The compression type
                                  BI_RGB(0) = not compressed
                                  BI_RLE8(1)= run length encoded 8 bitsperpix
                                  BI_RLE4(2)= run length encoded 4 bitsperpix */
   DWORD biSizeImage;          /* The size in bytes of the image */
   DWORD biXPelsPerMeter;      /* The horiz resolution in pixels per meter */
   DWORD biYPelsPerMeter;      /* The vert resolution in pixels per meter */
   DWORD biClrUsed;            /* The number of color indexes in the LUT 
                                  if = 0 use maximum for bit count
                                  if not = 0  use actual number */
   DWORD biClrImportant;       /* The number of important colors */
   } BITMAP_INFOHEADER;

/* RGBQUAD structure */

typedef struct tag_RGB_QUAD
   {
   BYTE1 rgbBlue;              /* The blue value */
   BYTE1 rgbGreen;             /* The green value */
   BYTE1 rgbRed;               /* The Red value */
   BYTE1 rgbReserved;          /* The Reserved value */
   } RGB_QUAD;

/* BITMAPINFO structure */

typedef struct tag_BITMAP_INFO
   {
   BITMAP_INFOHEADER bmiHeader;
   RGB_QUAD *bmiColors;
   } BITMAP_INFO;

#endif
