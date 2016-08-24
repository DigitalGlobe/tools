#ifndef H_DTCC_F

#define H_DTCC_F

#ifndef H_COORD_DEF
#include "coord_d.h"
#endif
#ifndef H_COORD_FUNC
#include "coord_f.h"
#endif


/***************************************************************
@   check_for_ups();
****************************************************************

*/
#if XVT_CC_PROTO
BOOLEAN
check_for_ups(double sphi, int32 *izone, double *y, double *x, char *mgrs,
               int32 *iarea, double degrad);
#else
BOOLEAN
check_for_ups();
#endif


/***************************************************************
@   check_for_zone_and_100kms_error();
****************************************************************

*/
#if XVT_CC_PROTO
BOOLEAN
check_for_zone_and_100kms_error(int iset, char *string);
#else
BOOLEAN
check_for_zone_and_100kms_error();
#endif


/***************************************************************
@   coord_2_gp_radian();
****************************************************************

*/
#if XVT_CC_PROTO
ERRSTATUS
coord_2_gp_radian(POINT_UNITS *user_pt, POINT_DD *radian_pt,
                  COORD_SYS *params, char *err_message);
#else
ERRSTATUS
coord_2_gp_radian();
#endif


/***************************************************************
@   gp_radian_2_coord();
****************************************************************

*/
#if XVT_CC_PROTO
ERRSTATUS
gp_radian_2_coord(POINT_UNITS *user_pt, POINT_DD *radian_pt,
                  COORD_SYS *params, char *err_message);
#else
ERRSTATUS
gp_radian_2_coord();
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
#endif /* H_DTCC_F */
