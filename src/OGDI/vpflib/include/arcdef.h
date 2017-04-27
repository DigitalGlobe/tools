#ifndef H_ARC_DEF
#define H_ARC_DEF

/************************************************/
/* Header file for ARC projection software. 		 */
/* Author: Jeff Buck (JB)								 */
/* Company: Spatial Data Sciences (SDS)			 */
/************************************************/

#include	<ctype.h>

/*
 * #include <malloc.h>
 */
#include	<math.h>
#include	<stdio.h>
#include 	<stdlib.h>
#include	<string.h>

#define PI	3.14159265358979323
#define RADIAN(x)	(x * (PI / 180.))
#define DEGREES(x)	(x * (180. / PI))
#define ROUND(x)	((int32)(x + .5))

#define ARC_FILE "arc.dat"	/* name of file containing ARC parameters	 */

/*
 * File offsets used by read_arc_parms()
 */
#define SCALE_OFFSET			1760	/* file offset to scale
						 * string				 */
#define LAMBDA_OFFSET		1793	/* file offset to origin longitude
					 * string	 */
#define PHI_OFFSET			1804	/* file offset to origin
						 * latitude string	 */
#define TILE_SIZE_OFFSET	1909	/* file offset to tile size string			 */
#define ROW1_OFFSET			1879	/* file offset to row upper
						 * left	string	 */
#define COL1_OFFSET			1897	/* file offset to column
						 * upper left string */
#define ROW2_OFFSET			1891	/* file offset to row lower
						 * right string	 */
#define COL2_OFFSET			1885	/* file offset to column
						 * lowerright string */
#define ZONE_OFFSET			1769	/* file offset to image zone
						 * number			 */

/*
 * Length of strings to be read in by read_arc_parms()
 */
#define SCALE_LEN				9	/* length of scale
							 * string						 */
#define LAMBDA_LEN			11	/* length of longitude string					 */
#define PHI_LEN				10	/* length of latitude string					 */
#define TILE_SIZE_LEN	   6	/* length of tile size string					 */
#define ROW_LEN				6	/* length of row string							 */
#define COL_LEN				6	/* length of column string						 */
#define ZONE_LEN				2	/* length of zone string						 */

/*
 * Error codes returned by arc()
 */
#define NO_ERROR				0
#define BAD_ZONE				34
#define BAD_SCALE				35
#define BAD_TILE_SIZE		36

/*
 * Data structures used by ARC routines
 */
typedef struct lpoint
{
    int32            row;
    int32            col;
}               LPOINT;

typedef struct latlon
{
    double          lat;
    double          lon;
}               LATLON;

typedef struct tile_info
{
    int             tile_row;	/* row number of the tile in the image					 */
    int             tile_col;	/* column number of the tile in the image				 */
    int             row;	/* coordinate's row number relative to the
				 * tile		 */
    int             col;	/* coordinate's column number relative to the
				 * tile	 */
}               TILE_INFO;


#if 0
typedef struct arc_parms
{
    int32            scale;	/* scale of the map									 */
    LATLON          origin;	/* lat/lon of map origin (upper left corner)	 */
    LPOINT          ul;		/* image upper left row and column				 */
    LPOINT          lr;		/* image lower right row and column				 */
    int             tile_size;	/* size of a tile in pixels						 */
    int             zone;	/* zone number of upper-left of image			 */
}               ARC_PARMS;
#endif


extern double   fmod();
extern double   fabs();
#endif				/* H_ARC_DEF */
