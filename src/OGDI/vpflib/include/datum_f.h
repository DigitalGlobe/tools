

#ifndef H_DATUM_F
#define H_DATUM_F


#include "stdlib.h"

#if XVT_OS == XVT_OS_WIN
#include "float.h"
#endif

#ifndef H_MUSE1
#include "muse1.h"
#endif

/***************************************************************
@   datum_transform();
****************************************************************
Driver for calling various datum transformation routines
*/

#if XVT_CC_PROTO
ERRSTATUS
datum_transform(POINT_DD *from_gp_radian_pt, POINT_DD *to_gp_radian_pt,
                POINT_XYZ *error,
				DATUM_TYPE *loc_datum, ELLIPS_PARAMS *loc_ellips,
                int transform_dir);
#else
ERRSTATUS
datum_transform();
#endif

/* Prototypes formerly in madfunc.h */
/***************************************************************
@   molerror();
****************************************************************
Calculates transformation uncertainty
*/
#if XVT_CC_PROTO
void molerror( POINT_DD *pt, TRANSFORM_PARAMS *params, POINT_XYZ *error);
#else
void molerror();
#endif
/*
Description: Based on the translation sigmas and the coordinates
of the transformation point, calculates uncertainty of latitude, longitude
and height in meters. The error equations are derived by geometric
projection of x, y and z vectors to latitude, longitude and height.
*/


/***************************************************************
@   molod();
****************************************************************
Performs Molodensky datum transformation
*/
#if XVT_CC_PROTO
void molod( POINT_DD *from_pt, POINT_DD *delta,
            TRANSFORM_PARAMS *params);
#else
void molod();
#endif
/*
Description: Performs Molodensky datum transformation in all cases except for WGS72.
See DMA TR 8350.2.
*/



/***************************************************************
@   wgs72();
****************************************************************
Performs datum transformation for the special case when the local
datum is WGS 1972.
*/
#if XVT_CC_PROTO
void wgs72( POINT_DD *from_pt, POINT_DD *delta,
            TRANSFORM_PARAMS *params);
#else
void wgs72();
#endif
/*
Description: Transforms WGS 72 coordinates to WGS 84 and the reverse.
*/

#endif
