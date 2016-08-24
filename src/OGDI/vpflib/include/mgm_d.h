#ifndef H_MGM_D
#define H_MGM_D

#ifndef H_DTCC_D
#include "dtcc_d.h"
#endif

/***************************************************************
@    CONFIG_PREF
****************************************************************
Ways of configuring map geometry
*/
typedef enum
{
    CONFIG_GEO_EXTENT_AND_DEV_EXTENT,
    CONFIG_CENTER_AND_SCALE,
    CONFIG_CENTER_AND_POST_SPACING
}
                CONFIG_PREF;

/*
 * Description: The map geometry module supports these preferences when
 * mgm_setup is called.
 */

/***************************************************************
@    MGM
****************************************************************
The map geometry structure
*/
typedef struct MGM
{
    int32            magic;
    PROJECTION      projection;	/* the objects projection */
    HOR_DATUM       hor_datum;	/* an enum type */
    VER_DATUM       ver_datum;	/* an enum type */

    CONFIG_PREF     config_pref;/* how to resolve inconsistent params */
    POINT_DD        map_center;	/* geocoord of map center, DD */
    GFLOAT          scale_reciprocal;	/* 250000 for scale of 1:250,000 */
    GFLOAT          pixel_size_on_screen;	/* meters */
    GFLOAT          pixel_size_on_ground;	/* meters */
    GFLOAT          pixel_size_lon;	/* size in decimal degrees */
    GFLOAT          pixel_size_lat;	/* size in decimal degrees */
    EXTENT          dev_extent;	/* bounding device rectangle */
    GFLOAT          dev_ymin;	/* smallest y on map */
    GFLOAT          dev_ymax;	/* largest y on map */
    int32            xpixels;	/* width in pixels */
    int32            ypixels;	/* height in pixels */
    int32            bit_map_depth;	/* bits per pixel 1, 4, 8, or 24 */
    EXTENT          geo_extent;	/* bounding geographic rectangle, DD */
    GFLOAT          pixasp;	/* aspect ratio of pixel, (Y/X) */
    GFLOAT          horizontal_accuracy;	/* of most inaccurate
						 * product, meters */
    GFLOAT          vertical_accuracy;	/* of most inaccurate product, meters */
    BOOLEAN         user_aspect;/* If true user aspect used, else default */
    ASPECT          aspect;	/* aspect of projection */
    PARAMS          params;	/* projection dependent params */
    BOOLEAN         valid;	/* FALSE indicates an invalid projection
				 * object */

    GFLOAT          sx;		/* empirical x scale */
    GFLOAT          sy;		/* empirical y scale */
    GFLOAT          tx;		/* empirical x shift */
    GFLOAT          ty;		/* empirical y shift */
    BOOLEAN         is_locked;	/* true when locked to basemap */
}               MGM;

/*
 * Description: The sx,sy, tx, ty items fit the projection space to the
 * bitmap space.
 */
#endif				/* H_MGM_D */
