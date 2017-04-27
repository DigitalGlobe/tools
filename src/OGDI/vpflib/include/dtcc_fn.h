#ifndef H_DTCC_FN
#define H_DTCC_FN

/*#include "fusion_d.h"*/

#ifndef H_MGM_FUNC
#include "mgm_f.h"
#endif
#ifndef H_ARC_F
#include "arc_f.h"
#endif
#ifndef H_TS_F
#include "ts_f.h"
#endif

#if XVT_CC_PROTO
ERRSTATUS MUSE_API polar_geo_to_cr(double *geo, double *cr, double *geo0, double brv);
ERRSTATUS MUSE_API polar_cr_to_geo(double *cr, double *geo, double *geo0, double brv);
#else
ERRSTATUS MUSE_API polar_geo_to_cr();
ERRSTATUS MUSE_API polar_cr_to_geo();
#endif

/***************************************************************
@    bit_to_geo()
****************************************************************
Converts a bit map coordinate to geographic
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
bit_to_geo(MGM * mgm,
	   GFLOAT bitx, GFLOAT bity,
	   GFLOAT * londd, GFLOAT * latdd);
#else
ERRSTATUS MUSE_API bit_to_geo();
#endif

/*
 * Description: Using the map geometry in the first argument, the bitx and
 * bity are inverse map projected to decimal degrees (last 2 args).  Bitmap
 * coordinates are the indices into the raster basemap files and images.
 */

/***************************************************************
@    bit_to_win()
****************************************************************
Converts a bitmap coordinate to a window coordinate.
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
bit_to_win(WINDOW xvt_win,
	   GFLOAT bitx, GFLOAT bity,
	   GFLOAT * winx, GFLOAT * winy);
#else
ERRSTATUS MUSE_API bit_to_win();
#endif

/*
 * Description: Bitmap coordinates are indices into the raster objects. Their
 * origin is at the lower left. Window coordinates are returned in mouse
 * event records. Their origin is at the upper left.
 */

/***************************************************************
@    cot()
****************************************************************
cotangent function
*/

#if XVT_CC_PROTO
double          cot(double a);
#else
double          cot();
#endif

/*
 * Description:
 * 
 */
/***************************************************************
@    geo_to_bit()
****************************************************************
Convert the geographic coordinate to a bit map or raster coord.
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
geo_to_bit(MGM * mgm,
	   GFLOAT londd, GFLOAT latdd,
	   GFLOAT * bitx, GFLOAT * bity);
#else
ERRSTATUS MUSE_API geo_to_bit();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    geo_to_user_str()
****************************************************************
Convert the geographic coordinate to user formatted string
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API geo_to_user_str(short geo_units, POINT_DD * value, char *output);
#else
ERRSTATUS MUSE_API geo_to_user_str();
#endif

#if XVT_CC_PROTO
ERRSTATUS MUSE_API user_str_to_geo(short geo_units, POINT_DD * value, char *output);
#else
ERRSTATUS MUSE_API user_str_to_geo();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    geo_to_win()
****************************************************************
Convert the geographic coordinate to a window coordinate
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
geo_to_win(MGM * mgm, WINDOW xvt_win,
	   GFLOAT londd, GFLOAT latdd,
	   GFLOAT * winx, GFLOAT * winy);
#else
ERRSTATUS MUSE_API geo_to_win();
#endif

/*
 * Description:
 * 
 */


/***************************************************************
@    geo_to_view()
****************************************************************
Convert the geographic coordinate to a view coordinate
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
geo_to_view(MGM * mgm, WINDOW xvt_win,
	    GFLOAT londd, GFLOAT latdd,
	    GFLOAT * viewx, GFLOAT * viewy);
#else
ERRSTATUS MUSE_API geo_to_view();
#endif

/*
 * Description: The input geographic coordinate is in decimal degrees. The
 * output is a view coordinate which is a window coordinate which may have
 * been zoomed and scrolled.
 */


/***********************************************************
@       get_user_pt()
************************************************************
Invoke a dialog box that allows entry of a map point in user units
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API get_user_pt
		(
				 MAP_POINT * map_point,
				 GEOGRAPHIC user_units
);
#else
ERRSTATUS MUSE_API get_user_pt();
#endif

/***************************************************************
@    lamber_config()
****************************************************************
Configure the Lambert Conformal Conic projection
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API lamber_config(MGM * mgm);
#else
ERRSTATUS MUSE_API lamber_config();
#endif

/*
 * Description: Performs the necessary calculations to fit the requested
 * geo-area into the image bitmap.
 * 
 */

/***************************************************************
@    lamber_map()
****************************************************************
Forward Lambert Conformal Conic projection
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
lamber_map(
	   MGM * mgm,
	   GFLOAT londd,
	   GFLOAT latdd,
	   GFLOAT * x,
	   GFLOAT * y);
#else
ERRSTATUS MUSE_API lamber_map();
#endif

/*
 * Description: Converts the geo-coordinate into a map image coordinate.
 * 
 */

/***************************************************************
@    lamber_unmap()
****************************************************************
Reverse Lambert Conformal Conic projection

*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
lamber_unmap(
	     MGM * mgm,
	     GFLOAT * londd,
	     GFLOAT * latdd,
	     GFLOAT x,
	     GFLOAT y);
#else
ERRSTATUS MUSE_API lamber_unmap();
#endif

/*
 * Description: Converts the map image coordinate into a geo-coordinate.
 * 
 */

/***************************************************************
@    linear_config()
****************************************************************
Configure the linear map projction
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API linear_config(MGM * mgm);
#else
ERRSTATUS MUSE_API linear_config();
#endif

/*
 * Description: Performs the necessary calculations to fit the requested
 * geo-area into the image bitmap.
 * 
 */

/***************************************************************
@    linear_map()
****************************************************************
Forward linear projection
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
linear_map(
	   MGM * mgm,
	   GFLOAT londd,
	   GFLOAT latdd,
	   GFLOAT * x,
	   GFLOAT * y);
#else
ERRSTATUS MUSE_API linear_map();
#endif

/*
 * Description: Converts the geo-coordinate into a map image coordinate.
 * 
 */

/***************************************************************
@    lon180()
****************************************************************
Convert longitude to +-180
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API lon180(double *longitude);
#else
ERRSTATUS MUSE_API lon180();
#endif

/*
 * Description: Longitude in decimal degrees is converted to the range -180
 * to +180.
 * 
 */

/***************************************************************
@    lon360()
****************************************************************
Convert longitude to 0-360
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API lon360(double *longitude);
#else
ERRSTATUS MUSE_API lon360();
#endif

/*
 * Description: Longitude in decimal degrees is converted to the range 0 to
 * 360.
 * 
 */

/***************************************************************
@    linear_unmap()
****************************************************************
Reverse linear projection
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
linear_unmap(
	     MGM * mgm,
	     GFLOAT * londd,
	     GFLOAT * latdd,
	     GFLOAT x,
	     GFLOAT y);
#else
ERRSTATUS MUSE_API linear_unmap();
#endif

/*
 * Description: Converts the map image coordinate into a geo-coordinate.
 * 
 */

/***************************************************************
@    map()
****************************************************************
Map the geographic coordinate into a bit map and PHIGS coord
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
map(
    MGM * mgm,
    GFLOAT londd,
    GFLOAT latdd,
    GFLOAT * x,
    GFLOAT * y);
#else
ERRSTATUS MUSE_API map();
#endif

/*
 * Description: Converts the geo-coordinate into a map image coordinate.
 * 
 */
/***************************************************************
@    mercat_config()
****************************************************************
Configure Mercator projection
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API mercat_config(MGM * mgm);
#else
ERRSTATUS MUSE_API mercat_config();
#endif

/*
 * Description: Performs the necessary calculations to fit the requested
 * geo-area into the image bitmap.
 * 
 */

/***************************************************************
@    mercat_map()
****************************************************************
Forward Mercator projection
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
mercat_map(
	   MGM * mgm,
	   GFLOAT londd,
	   GFLOAT latdd,
	   GFLOAT * x,
	   GFLOAT * y);
#else
ERRSTATUS MUSE_API mercat_map();
#endif

/*
 * Description: Converts the geo-coordinate into a map image coordinate.
 * 
 */

/***************************************************************
@    mercat_unmap()
****************************************************************
Reverse Mercator projection
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
mercat_unmap(
	     MGM * mgm,
	     GFLOAT * londd,
	     GFLOAT * latdd,
	     GFLOAT x,
	     GFLOAT y);
#else
ERRSTATUS MUSE_API mercat_unmap();
#endif

/*
 * Description: Converts the map image coordinate into a geo-coordinate.
 * 
 */

/***************************************************************
@    mgm_adjust()
****************************************************************
Adjust the map geometry
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
mgm_adjust(
	   MGM * mgm);
#else
ERRSTATUS MUSE_API mgm_adjust();
#endif

/*
 * Description: Adjusts mgm parameters according to the CONFIG_PREF.
 * 
 */

/***************************************************************
@    mgm_config()
****************************************************************
Configure an MGM object.
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API mgm_config(MGM * mgm);
#else
ERRSTATUS MUSE_API mgm_config();
#endif

/*
 * Description:
 * 
 */


/***************************************************************
@    mgm_construct()
****************************************************************
Allocates space for a map MGM object.
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
mgm_construct(
	      FILE * file,
	      MGM ** mgm,
	      DEFAULTS * defaults);
#else
ERRSTATUS MUSE_API mgm_construct();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    mgm_defaults()
****************************************************************
Assigns default parameters for the map geometry.
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
mgm_defaults(
	     MGM * mgm, DEFAULTS * defaults);
#else
ERRSTATUS MUSE_API mgm_defaults();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    mgm_destruct()
****************************************************************
Destroys the MGM object.
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
mgm_destruct(
	     FILE * file,
	     BOOLEAN destruct,
	     MGM ** mgm);
#else
ERRSTATUS MUSE_API mgm_destruct();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    mgm_fit()
****************************************************************
Fit the geographic area to the bit map image
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
mgm_fit(
	MGM * mgm);
#else
ERRSTATUS MUSE_API mgm_fit();
#endif

/*
 * Description: Performs the necessary calculations to fit the requested
 * geo-area into an arbitrary output coordinate range.
 * 
 */

/***************************************************************
@    map_it()
****************************************************************
Performs map projection, both forward and reverse
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
map_it(MGM * mgm, PROJ_DIR forward_or_reverse,
       GFLOAT * londd, GFLOAT * latdd,
       GFLOAT * phigs_x, GFLOAT * phigs_y);
#else
ERRSTATUS MUSE_API map_it();
#endif

/*
 * Description: The map geometry object mgm is used project in the direction
 * given by forward_or_reverse.  Note that forward_or_reverse determine
 * whether the GP is computed based on the xy or vice versa, so bad things
 * will happen with no error messages if this function is misused.
 */

/***************************************************************
@    mgm_is_ok()
****************************************************************
Check the MGM structure for DMA legal usage
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API mgm_is_ok(MGM * mgm, char *what_is_wrong);
#else
ERRSTATUS MUSE_API mgm_is_ok();
#endif

/*
 * Description: If the mgm object is valid according to the DMA rules of map
 * projection usage, STAT_SUCCESS is returned.  If the mgm object is not
 * legal, then STAT_INV_PROJ is returned and the string argument
 * what_is_wrong contains a description of the problem.
 */

/***************************************************************
@    mgm_setup()
****************************************************************
Setup the map geometry
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API mgm_set
		(
				 int proj,
				 PROJECTION * projection
);
#else
ERRSTATUS MUSE_API mgm_set();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    mgm_setup()
****************************************************************
Setup the map geometry
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
mgm_setup(
	  MGM * mgm);
#else
ERRSTATUS MUSE_API mgm_setup();
#endif

/*
 * Description: Configures the map geometry object according to the mode
 * specified by MGM->CONFIG_PREF
 * 
 */

/***************************************************************
@    mgm_subclass()
****************************************************************
Sub-class the map geometry object to a particular projection
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
mgm_subclass(
	     MGM * mgm);
#else
ERRSTATUS MUSE_API mgm_subclass();
#endif

/*
 * Description: Assigns the generic map, unmap, and mgm_config pointers to a
 * specific projection.
 * 
 */


/***************************************************************
@    stereo_config()
****************************************************************
Configure the stereographic map projection
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
stereo_config(
	      MGM * mgm);
#else
ERRSTATUS MUSE_API stereo_config();
#endif

/*
 * Description: Performs the necessary calculations to fit the requested
 * geo-area into the image bitmap.
 * 
 */

/***************************************************************
@    transm_config()
****************************************************************
Configure transverse Mercator projection
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
transm_config(
	      MGM * mgm);
#else
ERRSTATUS MUSE_API transm_config();
#endif

/*
 * Description: Performs the necessary calculations to fit the requested
 * geo-area into the image bitmap.
 * 
 */

/***************************************************************
@    transm_map()
****************************************************************
Forward transverse Mercator projection
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
transm_map(
	   MGM * mgm,
	   GFLOAT londd,
	   GFLOAT latdd,
	   GFLOAT * x,
	   GFLOAT * y);
#else
ERRSTATUS MUSE_API transm_map();
#endif

/*
 * Description: Converts the geo-coordinate into a map image coordinate.
 * 
 */

/***************************************************************
@    transm_unmap()
****************************************************************
Reverse transverse Mercator projection
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
transm_unmap(
	     MGM * mgm,
	     GFLOAT * londd,
	     GFLOAT * latdd,
	     GFLOAT x,
	     GFLOAT y);
#else
ERRSTATUS MUSE_API transm_unmap();
#endif

/*
 * Description: Converts the map image coordinate into a geo-coordinate.
 * 
 */

/***************************************************************
@    unmap()
****************************************************************

*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
unmap(
      MGM * mgm,
      GFLOAT * londd,
      GFLOAT * latdd,
      GFLOAT x,
      GFLOAT y);
#else
ERRSTATUS MUSE_API unmap();
#endif

/*
 * Description: Converts the display coordinate to the equivalent geographic
 * coordinate.
 * 
 */

/********************************************************************
@    upnset()
*********************************************************************
Set MGRS info for northern polar zone
*/

#if XVT_CC_PROTO
void 
upnset(
       int32 n,
       int32 *ltrlow,
       int32 *ltrhi,
       double *feltr,
       double *fnltr,
       int32 *ltrhy);
#else
void            upnset();
#endif

/*
 * Description:
 * 
 */


/********************************************************************
@    upsset()
*********************************************************************
Set MGRS info for southern polar zone
*/

#if XVT_CC_PROTO
void 
upsset(
       int32 n,
       int32 *ltrlow,
       int32 *ltrhi,
       double *feltr,
       double *fnltr,
       int32 *ltrhy);
#else
void            upsset();
#endif

/*
 * Description:
 * 
 */


/********************************************************************
@    utmlim()
*********************************************************************
UTM lim
*/

#if XVT_CC_PROTO
void 
utmlim(
       int32 *n,
       double sphi,
       int32 izone,
       double *spsou,
       double *spnor,
       double *sleast,
       double *slwest);
#else
void            utmlim();
#endif

/*
 * Description:
 * 
 */



/********************************************************************
@    shiftr
*********************************************************************
Shiftr
*/

#if XVT_CC_PROTO
void 
shiftr(
       char *mgrs,
       int P1,
       int32 n,
       int32 *nchar);
#else
void            shift();
#endif

/*
 * Description:
 * 
 */


/********************************************************************
@    dmsh
*********************************************************************
Radians to degrees, minutes, seconds (????)
*/

#if XVT_CC_PROTO
void 
dmsh(
     int32 khem,
     double radian,
     int32 *id,
     int32 *im,
     double *rs,
     char *ah,
     int P1);
#else
void            dmsh();
#endif

/*
 * Description:
 * 
 */




/*********************************************************************
@    utmtgp
**********************************************************************
Universal Transverse Mercator to GP
*/

#if XVT_CC_PROTO
void 
utmtgp(
       double a,
       double recf,
       double *sphi,
       double *slam,
       int32 izone,
       double *y,
       double x);
#else
void            utmtgp();
#endif

/*
 * Description:
 * 
 */



/*********************************************************************
@    fifdint
**********************************************************************
Source code from the Promula.Fortran library
*/

#if XVT_CC_PROTO
double 
fifdint(
	double a);
#else
double          fifdint();
#endif

/*
 * Description:
 * 
 */


/********************************************************************
@    fifdnint
*********************************************************************
Source code from the Promula.Fortran library
*/

#if XVT_CC_PROTO
double 
fifdnint(
	 double a);
#else
double          fifdnint();
#endif

/*
 * Description:
 * 
 */


/**********************************************************************
@    fifichar
***********************************************************************
Source code from the Promula.Fortran library
*/

#if XVT_CC_PROTO
int 
fifichar(
	 unsigned char *c1);
#else
int             fifichar();
#endif

/*
 * Description:
 * 
 */


/********************************************************************
@    fifidint
*********************************************************************
Source code from the Promula.Fortran library
*/

#if XVT_CC_PROTO
int32 
fifidint(
	 double a);
#else
int32            fifidint();
#endif

/*
 * Description:
 * 
 */


/*********************************************************************
@    fifmod
**********************************************************************
Source code from the Promula.Fortran library
*/

#if XVT_CC_PROTO
int32 
fifmod(
       int32 num,
       int32 dem);
#else
int32            fifmod();
#endif

/*
 * Description:
 * 
 */


/*********************************************************************
@    fifnint
**********************************************************************
Source code from the Promula.Fortran library
*/

#if XVT_CC_PROTO
int32 
fifnint(
	double a);
#else
int32            fifnint();
#endif

/*
 * Description:
 * 
 */


/**********************************************************************
@    ftncms
***********************************************************************
Source code from the Promula.Fortran library
*/

#if XVT_CC_PROTO
int 
ftncms(
       char *s1,
       int n1,
       char *s2,
       int n2);
#else
int             ftncms();
#endif

/*
 * Description:
 * 
 */


/**********************************************************************
@    ftnsac
***********************************************************************
Source code from the Promula.Fortran library
*/

#if XVT_CC_PROTO
void 
ftnsac(
       char *s1,
       int n1,
       char *s2,
       int n2);
#else
void            ftnsac();
#endif

/*
 * Description:
 * 
 */
#endif                          /* H_DTCC_FN */
