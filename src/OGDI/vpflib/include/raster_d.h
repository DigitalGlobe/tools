#ifndef H_RASTER_D
#define H_RASTER_D

#ifndef H_COLOR_D
#include "color_d.h"
#endif


/*
 * Description: These are the number types that can be stored in RASTER's.
 */

/***************************************************************
@    DATA_MATRIX
****************************************************************
A structure for matrix data
*/
typedef struct
{
    int32            width;	/* Num of columns */
    int32            height;	/* Num of rows */
    int32            bits_per_pixel;	/* Num bits for each matrix value */
    int32            bytes_per_row;	/* All rows must be word(16 bit)
					 * alligned */
    NUM_TYPE        num_type;	/* Numeric type and size */
    DATA_TYPE       data_type;	/* Data or color type */
    GHANDLE         ghandle;	/* Global handle defined by XVT */
}               DATA_MATRIX;

/*
 * Description: To allow portable big arrays, the global heap is used. See
 * the XVT User's Guide, Chap 15, Memory Allocation. If you are not using XVT
 * you may need to redefine GHANDLE to void *.
 */

/*
 * Description:
 */

/***************************************************************
@    ACCURACY
****************************************************************
*/
typedef struct
{
    GFLOAT          rel_hor;	/* relative horizontal accuracy */
    GFLOAT          abs_hor;	/* absolute horizontal accuracy */
    GFLOAT          rel_ver;	/* relative data value accuracy */
    GFLOAT          abs_ver;	/* absolute data value accuracy */
}               ACCURACY;

/*
 * Description:
 */

/***************************************************************
@    DATE
****************************************************************
Date structure
*/
typedef struct
{
    char            year[3];
    char            month[3];
    char            day[3];
    char            hour[3];
    char            minute[3];
    char            second[3];
}               DATE;

/*
 * Description:
 */

/***************************************************************
@    DATA_HDR STRUCTURE
****************************************************************
A header structure for all extracted data
*/
typedef struct
{
    char            source[30];	/* Source of the data                  */
    GEOGRAPHIC      h_units;	/* Horizonal units of measurement      */
    VER_DISTANCE    v_units;	/* Vertical units of measurement       */
    GFLOAT          x_res;	/* X resoultion of the data in h_units */
    GFLOAT          y_res;	/* Y Resolution of the data in h_units */
    PROJECTION      projection;	/* Projection name of the data         */
    POINT_DD        center;	/* Center of the data in geographics   */
    EXTENT          extent;	/* Bounding geocoordinates of the data */
    DATUM           datum;	/* Horizonal and vertical datum        */
    ACCURACY        accuracy;	/* Accuricy of the data                */
    DATE            date;	/* Date the data was created/extracted */
    GFLOAT          zoom;	/* 0->1 for downsampling, >1 for
				 * interpolation */
    GFLOAT          data_min;	/* minimum data value */
    GFLOAT          data_max;	/* maximum data value */
}               DATA_HDR;

/*
 * Description:
 */

/***************************************************************
@    RASTER
****************************************************************
Contains a grid of image or other data
*/
typedef struct
{
    int32            magic;
    DATA_HDR        data_hdr;
    DATA_MATRIX     data_matrix;
    LUT            *lut;	/* NULL unless data_type is DATA */
    PALETTE        *palette;	/* NULL unless data_type is COLOR_INDIRECT */
}               RASTER;

/*
 * Description: The functions raster_construct() ans raster_destruct are used
 * to create and destroy these, as well as read and write them from files.
 */

/***************************************************************
@    RENDER_INFO
****************************************************************
Info to render a DATA raster into a COLOR_INDIRECT raster
*/
typedef struct
{
    GFLOAT          shade_threshold;
    GFLOAT          precision;
    int32            sunx;
    int32            suny;
    PALETTE_USAGE   palette_usage;
}               RENDER_INFO;

/*
 * Description: The shade_threshold is the data differencebetween the data
 * value and the one located at relative position sunx, suny within the data
 * raster that is needed to move the color a shade lighter or darker
 * (depending on sign).  The precision can be increased to make the rendering
 * faster.  If the the previous data value and delta are within precision of
 * the last data value and delta, the last color is used.
 */

/***************************************************************
@    MATRIX
****************************************************************
A data matrix of any kind
*/
typedef struct
{
    int32            width;	/* of data matrix                        */
    int32            height;	/* of data matrix                        */
    GHANDLE         gh;
    short pHMEM     data;	/* Row major, bottom to top (short data) */
}               MATRIX;

/*
 * Description: A major component of the RASTER structure
 */
#endif				/* H_RASTER_D */
