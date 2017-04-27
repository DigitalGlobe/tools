#ifndef H_UNITS_F
#define H_UNITS_F

#ifndef INCL_XVTH
#include "xvt.h"
#endif
#ifndef H_MUSE1
#include "muse1.h"
#endif
#ifndef H_UNITS_D
#include "units_d.h"
#endif

#if 0				/* JLL */
************
@area_conv_from[5] ()
****************************************************************
Area conversion functions from user units to internal units
* /

#if XVT_CC_PROTO

#if XVT_CC == XVT_CC_THINK
extern ERRSTATUS MUSE_API(*area_conv_from[5]) (...);
#else
extern ERRSTATUS MUSE_API(*area_conv_from[5]) ();
#endif

#else
extern ERRSTATUS MUSE_API(*area_conv_from[5]) ();
#endif

/*
 * Description: This is the declaration of an array of pointers to functions.
 * The user unit is represented by a set of enumerated constants for this
 * unit.  This constant is used as the index into the array of pointers to
 * functions, so as to call the particular conversion function.
 * 
 * Ex: GFLOAT area_in_sq_feet, area_in_sq_meters; status =
 * (*area_conv_from[SQ_FEET]) (area_in_sq_feet, &area_in_sq_meters);
 */

/***************************************************************
@     (*area_conv_to[5])()
****************************************************************
Area conversion functions from internal units to user units
*/

#if XVT_CC_PROTO

#if XVT_CC == XVT_CC_THINK
extern ERRSTATUS MUSE_API(*area_conv_to[5]) (...);
#else
extern ERRSTATUS MUSE_API(*area_conv_to[5]) ();
#endif

#else
extern ERRSTATUS MUSE_API(*area_conv_to[5]) ();
#endif

/*
 * Description: This is the declaration of an array of pointers to functions.
 * The user unit is represented by a set of enumerated constants for this
 * unit.  This constant is used as the index into the array of pointers to
 * functions, so as to call the particular conversion function.
 * 
 * Ex: GFLOAT area_in_sq_feet, area_in_sq_meters; status =
 * (*area_conv_to[SQ_FEET]) (area_in_sq_meters, &area_in_sq_feet);
 * 
 */

/***************************************************************
@     (*volume_conv_from[5])()
****************************************************************
Volume conversion functions from user units to internal units
*/

#if XVT_CC_PROTO

#if XVT_CC == XVT_CC_THINK
extern ERRSTATUS MUSE_API(*volume_conv_from[5]) (...);
#else
extern ERRSTATUS MUSE_API(*volume_conv_from[5]) ();
#endif

#else
extern ERRSTATUS MUSE_API(*volume_conv_from[5]) ();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@     (*volume_conv_to[5])()
****************************************************************
Volume conversion functions from internal units to user units
*/

#if XVT_CC_PROTO

#if XVT_CC == XVT_CC_THINK
extern ERRSTATUS MUSE_API(*volume_conv_to[5]) (...);
#else
extern ERRSTATUS MUSE_API(*volume_conv_to[5]) ();
#endif

#else
extern ERRSTATUS MUSE_API(*volume_conv_to[5]) ();
#endif

/*
 * Description:
 * 
 */
#endif

/***************************************************************
@     ver_conv_from()
****************************************************************
Converts from user vertical distance units to internal (meters)
*/
ERRSTATUS       MUSE_API

#if XVT_CC_PROTO
                ver_conv_from(short user_unit_index, double *mtr_1, double mtr_2);
#else
                ver_conv_from();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@     ver_conv_to()
****************************************************************
Converts from internal vertical distance to user units
*/
ERRSTATUS       MUSE_API

#if XVT_CC_PROTO
ver_conv_to(short user_unit_index, double mtr_1, double *mtr_2);
#else
ver_conv_to();
#endif

/*
 * Description:
 * 
 */


/***************************************************************
@     hor_conv_from()
****************************************************************
Horizonal conversion functions from user units to internal units
*/
ERRSTATUS       MUSE_API

#if XVT_CC_PROTO
hor_conv_from(short user_unit_index, double *hor_1, double hor_2);
#else
hor_conv_from();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@     hor_conv_to()
****************************************************************
Horizonal conversion functions from internal units to user units
*/
ERRSTATUS       MUSE_API

#if XVT_CC_PROTO
hor_conv_to(short user_unit_index, double hor_1, double *hor_2);
#else
hor_conv_to();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@     scale_conv_from()
****************************************************************
Scale conversion functions from user units to internal units
*/
ERRSTATUS       MUSE_API

#if XVT_CC_PROTO
                scale_conv_from(short user_unit_index, double *recip, double user_unit);
#else
scale_conv_from();
#endif

/*
 * Description: This is the declaration of an array of pointers to functions.
 * The user unit is represented by set of enumerated constants for this unit.
 * This constant is used as the index into the array of pointers to
 * functions, so as to call the particular conversion function.
 * 
 * Ex: GFLOAT scale_in_per_deg, scale_reciprocal; status =
 * (*scale_conf_from[SCALE_IN_PER_DEG]) (scale_in_per_deg,
 * &scale_reciprocal);
 */


/***************************************************************
@     scale_conv_to()
****************************************************************
Scale conversion functions from internal units to user units
*/
ERRSTATUS       MUSE_API

#if XVT_CC_PROTO
                scale_conv_to(short user_unit_index, double recip, double *user_unit);
#else
scale_conv_to();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    cubm_from_cubf()
****************************************************************
Cubic meters from cubic feet
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
cubm_from_cubf(
	       double *meter, double feet);
#else
ERRSTATUS MUSE_API cubm_from_cubf();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    cubm_from_cubkm()
****************************************************************
Cubic meters from cubic kilometers
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
cubm_from_cubkm(
		double *meter, double kmeter);
#else
ERRSTATUS MUSE_API cubm_from_cubkm();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    cubm_from_cubm()
****************************************************************
Cubic meters from cubic meters
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
cubm_from_cubm(
	       double *meter1, double meter2);
#else
ERRSTATUS MUSE_API cubm_from_cubm();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    cubm_from_cubnm()
****************************************************************
Cubic meters from cubic nautical miles
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
cubm_from_cubnm(
		double *meter, double nmile);
#else
ERRSTATUS MUSE_API cubm_from_cubnm();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    cubm_from_cubsm()
****************************************************************
Cubic meters from cubic statute miles
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
cubm_from_cubsm(
		double *meter, double stmile);
#else
ERRSTATUS MUSE_API cubm_from_cubsm();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    cubm_to_cubf()
****************************************************************
Cubic meters to cubic feet
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
cubm_to_cubf(
	     double meter, double *feet);
#else
ERRSTATUS MUSE_API cubm_to_cubf();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    cubm_to_cubkm()
****************************************************************
Cubic meters to cubic kilometers
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
cubm_to_cubkm(
	      double meter, double *kmeter);
#else
ERRSTATUS MUSE_API cubm_to_cubkm();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    cubm_to_cubm()
****************************************************************
Cubic meters to cubic meters
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
cubm_to_cubm(
	     double meter1, double *meter2);
#else
ERRSTATUS MUSE_API cubm_to_cubm();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    cubm_to_cubnm()
****************************************************************
Cubic meters to cubic nautical miles
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
cubm_to_cubnm(
	      double meter, double *nmile);
#else
ERRSTATUS MUSE_API cubm_to_cubnm();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    cubm_to_cubsm()
****************************************************************
Cubic meters to cubic statute miles
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
cubm_to_cubsm(
	      double meter, double *stmile);
#else
ERRSTATUS MUSE_API cubm_to_cubsm();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    decode_units()
****************************************************************
Convert units structure to local binary
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
decode_units(
	     unsigned char *buffer,
	     UNITS * units);
#else
ERRSTATUS MUSE_API decode_units();
#endif

/*
 * Description: The units structure information in the character buffer
 * (binary portable Intel format) is placed into the units structure in local
 * binary.  Used by the constructor functions while loading in a map
 * document.
 */

/***************************************************************
@    encode_units()
****************************************************************
Convert units structure external form
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
encode_units(
	     unsigned char *buffer,
	     UNITS * units);
#else
ERRSTATUS MUSE_API encode_units();
#endif

/*
 * Description: The units structure is converted to the external binary
 * portable (Intel) format. Used by the destructor functions while preparing
 * to store a map document.
 */

/***************************************************************
@     geo_conv_from()
****************************************************************
Geo. conversion functions from user units to internal units
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
geo_conv_from(short tag, POINT_DD * dd, POINT_UNITS * user_unit);
#else
ERRSTATUS MUSE_API geo_conv_from();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@     geo_conv_to()
****************************************************************
Geo. conversion functions from internal units to user units
*/
ERRSTATUS       MUSE_API

#if XVT_CC_PROTO
geo_conv_to(short tag, POINT_DD * dd, POINT_UNITS * user_unit);
#else
geo_conv_to();
#endif

/*
 * Description:
 * 
 */


/***************************************************************
@     ftnblkd()
****************************************************************
Initialize block data
*/

#if XVT_CC_PROTO
void            ftnblkd(void);
#else
void            ftnblkd();
#endif

/*
 * Description:
 * 
 */



/****************************************************************
@    gptmgr()
*****************************************************************
GP to MGRS and UTM grids
*/

#if XVT_CC_PROTO
void 
gptmgr(
       double *a,
       double *recf,
       double *sphi,
       double *slam,
       int32 *izone,
       double *y,
       double *x,
       char *mgrs,
       int P1,
       char *azone,
       int P2,
       int32 *isph,
       int32 *igrpin);
#else
void            gptmgr();
#endif

/*
 * Description:
 * 
 */




/******************************************************************
@    gptutm()
*******************************************************************
GP to UTM grid
*/

#if XVT_CC_PROTO
void 
gptutm(
       double a,
       double recf,
       double sphi,
       double slam,
       int32 *izone,
       double *y,
       double *x,
       int32 ifixz);
#else
void            gptutm();
#endif

/*
 * Description:
 * 
 */


/*******************************************************************
@    milref()
********************************************************************
GP to MGRS
*/

#if XVT_CC_PROTO
void 
milref(
       int32 *iarea,
       double *a,
       double *recf,
       double *sphi,
       double slam,
       int32 *izone,
       double y,
       double x,
       char *mgrs,
       int P2,
       char *azone,
       int P1,
       int32 *isph,
       int32 *igrpin);
#else
void            milref();
#endif

/*
 * Description:
 * 
 */


/***************************************************************
@    pdmstodd()
****************************************************************
*/

#if XVT_CC_PROTO
void            pdmstodd
                (
		                 float pdms,
		                 float *dd
);
#else
void            pdmstodd();
#endif

/*
 * Description:
 */

/********************************************************************
@    savmgr()
*********************************************************************
Save MGRS
*/

#if XVT_CC_PROTO
void 
savmgr(
       char *mgrs,
       int P1,
       int32 nchar,
       double xltr,
       double yltr,
       int32 ltrnum[],
       double spsou,
       double spnor,
       double sleast,
       double slwest,
       double ylow,
       double yslow,
       int32 iarea,
       int32 izone,
       int32 ltrlow,
       int32 ltrhi,
       int32 ltrhy,
       double fnltr,
       double feltr,
       char *azone,
       int P2);
#else
void            savmgr();
#endif

/*
 * Description:
 * 
 */




/***************************************************************
@     scale_to_user_str
****************************************************************
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API scale_to_user_str
                (
		                 UNITS * units,
		                 GFLOAT scale_reciprocal,
		                 char *output
);
#else
ERRSTATUS MUSE_API scale_to_user_str();
#endif

/*
 * Description:
 * 
 */
/***************************************************************
@    sqm_from_sqf()
****************************************************************
Square meters from square feet
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
sqm_from_sqf(
	     GFLOAT * sqm, GFLOAT sqf);
#else
ERRSTATUS MUSE_API sqm_from_sqf();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    sqm_from_sqkm()
****************************************************************
Square meters from square kilometers
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
sqm_from_sqkm(
	      GFLOAT * sqm, GFLOAT sqkm);
#else
ERRSTATUS MUSE_API sqm_from_sqkm();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    sqm_from_sqm()
****************************************************************
Square meters from square meters
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
sqm_from_sqm(
	     GFLOAT * sqm1, GFLOAT sqm2);
#else
ERRSTATUS MUSE_API sqm_from_sqm();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    sqm_from_sqnm()
****************************************************************
Square meters from square nautical miles
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
sqm_from_sqnm(
	      GFLOAT * sqm, GFLOAT sqnm);
#else
ERRSTATUS MUSE_API sqm_from_sqnm();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    sqm_from_sqsm()
****************************************************************
Square meters from square statue miles
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
sqm_from_sqsm(
	      GFLOAT * sqm, GFLOAT sqnm);
#else
ERRSTATUS MUSE_API sqm_from_sqsm();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    sqm_to_sqf()
****************************************************************
Square meters to square feet
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
sqm_to_sqf(
	   GFLOAT sqm, GFLOAT * sqf);
#else
ERRSTATUS MUSE_API sqm_to_sqf();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    sqm_to_sqkm()
****************************************************************
Square meters to square kilometers
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
sqm_to_sqkm(
	    GFLOAT sqm, GFLOAT * sqkm);
#else
ERRSTATUS MUSE_API sqm_to_sqkm();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    sqm_to_sqm()
****************************************************************
Square meters to square meters
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
sqm_to_sqm(
	   GFLOAT sqm1, GFLOAT * sqm2);
#else
ERRSTATUS MUSE_API sqm_to_sqm();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    sqm_to_sqnm()
****************************************************************
Square meters to square nautical miles
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
sqm_to_sqnm(
	    GFLOAT sqm, GFLOAT * sqnm);
#else
ERRSTATUS MUSE_API sqm_to_sqnm();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    sqm_to_sqsm()
****************************************************************
Square meters to square statute miles
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
sqm_to_sqsm(
	    GFLOAT sqm, GFLOAT * sqsm);
#else
ERRSTATUS MUSE_API sqm_to_sqsm();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    units_construct()
****************************************************************
Construct the units object
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
units_construct(
		FILE * file,
		UNITS ** pointer);
#else
ERRSTATUS MUSE_API units_construct();
#endif

/*
 * Description: The units object contains a list of the user selected map
 * units.
 */

/***************************************************************
@    units_destruct()
****************************************************************
Destroy the object of type UNITS
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
units_destruct(
	       FILE * file,
	       BOOLEAN destruct,
	       UNITS ** pointer);
#else
ERRSTATUS MUSE_API units_destruct();
#endif

/*
 * Description:
 * 
 */
#endif				/* H_UNITS_F */
