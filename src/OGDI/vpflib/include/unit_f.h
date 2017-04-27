

#ifndef H_UNIT_FUNC
#define H_UNIT_FUNC

#ifndef INCL_XVTH
#include "xvt.h"
#endif

#ifndef H_CC1_F
#include "cc1_f.h"
#endif

#include "math.h"

/****************************************************************
@    aemtgp()
*****************************************************************
Azimuthal Equidistant to Geodetic
*/
#if  XVT_CC_PROTO
void
aemtgp(double a, double recf, double ophi, double olam, double fn, double fe,
       double *sphi, double *slam, double y, double x);
#else
void
aemtgp();
#endif

/****************************************************************
@    aeorig()
*****************************************************************
Validate system origin parameters for Azimuthal Equidistant to/from Geodetic
*/

#if XVT_CC_PROTO
ERRSTATUS
aeorig( int32 itype, COORD_TYPE *coord_type );
#else
ERRSTATUS
aeorig();
#endif

/****************************************************************
@    convergence_and_scale_factor()
*****************************************************************
Calls the appropriate function to calculate convergence and
scale factor for the current coordinate system  
*/
ERRSTATUS
#if XVT_CC_PROTO
convergence_and_scale_factor(POINT_UNITS *user_pt, POINT_DD *radian_pt,
                             COORD_SYS *coord_sys,
			     COORD_DMS *convergence, double *scale_factor);
#else
convergence_and_scale_factor( );
#endif

/****************************************************************
@    deg_to_degmin()
*****************************************************************
Convert decimal degrees to degrees and minutes
*/
#if XVT_CC_PROTO
void deg_to_degmin( double dd, short *deg, double *mn);
#else
void deg_to_degmin();
#endif
/****************************************************************
@    deg_to_degminsec()
*****************************************************************
Convert decimal degrees to degrees, minutes, and seconds
*/
#if XVT_CC_PROTO
void deg_to_degminsec( double dd, short *deg, short *mn, double *sec);
#else
void deg_to_degminsec();
#endif
/*
Description:
*/

/***************************************************************
@     geo_conv_from()
****************************************************************
Geo. conversion functions from user units to internal units
*/

#if XVT_CC_PROTO
ERRSTATUS       MUSE_API
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



/****************************************************************
@    gptaem()
*****************************************************************
Geodetic to Azimuthal Equidistant
*/
#if XVT_CC_PROTO
void
gptaem( double a, double recf, double ophi, double olam, double fn, double fe,
        double sphi, double slam, double *y, double *x);
#else
void
gptaem();
#endif
/*
Description:
*/


/****************************************************************
@    gpgref()
*****************************************************************
Geodetic to GEOREF
*/
#if XVT_CC_PROTO
void
gpgref(char *georef, /*int P1,*/ double *sphi, double *slam, int32 ndec);
#else
void
gpgref();
#endif
/*
Description:
*/

/***************************************************************
@     ftnblkd()
****************************************************************
Initialize block data
*/
#if XVT_CC_PROTO
void ftnblkd(void);
#else
void ftnblkd();
#endif
/*
Description:
*/

/****************************************************************
@    gptlam()
*****************************************************************
GP to LAMBERT
*/
#if XVT_CC_PROTO
void
gptlam(double a, double recf, double ophi, double olam, double fn,
       double fe, double ok, double sphi, double slam, double *y,
       double *x);
#else
void
gptlam();
#endif

/****************************************************************
@    gptmer()
*****************************************************************
GP to MERCATOR
*/
#if XVT_CC_PROTO
ERRSTATUS
gptmer(double a, double recf, double ophi, double olam, double fn,
            double fe, double ok, double sphi, double slam, double *y,
            double *x);
#else
ERRSTATUS
gptmer();
#endif
/*
Description:
*/

/****************************************************************
@    gptmgr()
*****************************************************************
GP to MGRS and UTM grids
*/
#if XVT_CC_PROTO
void gptmgr(
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
    /*int32 *isph,*/char *isph,
    int32 *igrpin);
#else
void gptmgr();
#endif
/*
Description:
*/


/****************************************************************
@    gptobm()
*****************************************************************
GP to Oblique Mercator Projection
*/
#if XVT_CC_PROTO
ERRSTATUS
gptobm(int32 *isolut, int32 kodor, double sphi, double slam,
            double *u, double *v, double *y, double *x,
            double e, double es, double fn, double fe,
            double ogam, double olam, double oaz,
            double ta, double tb, double te);
#else
ERRSTATUS
gptobm();
#endif

/****************************************************************
@    gptoyx()
*****************************************************************
GP to TRANSVERSE MERCATOR map projection
*/
#if XVT_CC_PROTO
void
gptoyx(double a, double recf, double ophi, double olam, double fn, double fe,
       double ok, double sphi, double slam, double *y, double *x);
#else
void gptoyx();
#endif
/*
Description:
*/

/***************************************************************
@    gptups()
****************************************************************
GP to UPS map projection
*/
#if XVT_CC_PROTO
void gptups(
    double a,
    double recf,
    double sphi,
    double slam,
    double *y,
    double *x);
#else
void gptups();
#endif
/*
Description:
*/

/******************************************************************
@    gptutm()
*******************************************************************
GP to UTM grid
*/
#if XVT_CC_PROTO
void gptutm(
    double a,
    double recf,
    double sphi,
    double slam,
    int32 *izone,
    double *y,
    double *x,
    int32 ifixz);
#else
void gptutm();
#endif
/*
Description:
*/

/****************************************************************
@    gpgref()
*****************************************************************
GEOREF to GP
*/
#if XVT_CC_PROTO
void
grefgp(char *georef, /*int P1,*/ double *sphi, double *slam, int32 *ierr);
#else
void
grefgp();
#endif
/*
Description:
*/

/****************************************************************
@    iset_from_zone_str()
*****************************************************************
GEOREF to GP
*/
#if XVT_CC_PROTO
short
iset_from_zone_str(char *string);
#else
short
iset_from_zone_str();
#endif
/*
Description:
*/

/*******************************************************************
@    lamorg()
********************************************************************
VALIDATION OF INPUT ORIGIN PARAMETERS AND CONVERSION TO RADIANS, AND
DERIVATION OF OTHER ORIGIN PARAMETERS FOR LAMBERT TO GP AND GP TO LAMBERT
*/
#if XVT_CC_PROTO
ERRSTATUS
lamorg(double a, double recf, COORD_TYPE *coord_type);
#else
ERRSTATUS 
lamorg();
#endif

/******************************************************************
@    lamtgp()
*******************************************************************
LAMBERT to GP
*/
#if XVT_CC_PROTO
ERRSTATUS
lamtgp(double a, double recf, double ophi, double olam, double fn,
       double fe, double ok, double *sphi, double *slam, double y,
       double x);
#else
ERRSTATUS
lamtgp();
#endif

/*******************************************************************
@    merorg()
********************************************************************
VALIDATION OF ORIGIN PARAMETERS, CONVERSION OF ORIGIN TO RADIANS, AND
SCALE FACTOR CALCULATION FOR MERCATOR TO GP AND GP TO MERCATOR
*/
#if XVT_CC_PROTO
ERRSTATUS
merorg(double a, double recf, COORD_TYPE *coord_type );
#else
ERRSTATUS
merorg();
#endif


/*******************************************************************
@    mertgp()
********************************************************************
Mercator to GP
*/
#if  XVT_CC_PROTO
void
mertgp(double a, double recf, double ophi, double olam, double fn,
       double fe, double ok, double *sphi, double *slam, double y, double x);
#else
void
mertgp();
#endif

/*******************************************************************
@    milref()
********************************************************************
GP to MGRS
*/
#if XVT_CC_PROTO
void milref(
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
    /*int32 *isph,*/ char *isph,
    int32 *igrpin);
#else
void milref();
#endif
/*
Description:
*/

/********************************************************************
@    nzmggp()
*********************************************************************
Convert New Zealand Map Grid coordinates to GP radians
*/
#if  XVT_CC_PROTO
void
nzmggp(double a, double ophi, double olam, double fn, double fe,
       double *sphi, double *slam, double y, double x);
#else
void
nzmggp();
#endif


/********************************************************************
@    obmorg()
*********************************************************************
VALIDATION OF USER-ENTERED ORIGIN PARAMETERS, CONVERSION TO RADIANS, AND
DERIVATION OF ADDITIONAL ORIGIN PARAMETERS FOR OBLIQUE MERCATOR TO GP AND
GP TO OBLIQUE MERCATOR
*/
#if XVT_CC_PROTO
ERRSTATUS
obmorg(  double a, double recf, COORD_TYPE *coord_type, int32 itype );
#else
ERRSTATUS
obmorg();
#endif


/********************************************************************
@    obmtgp()
*********************************************************************
Convert from Oblique Mercator to GP
*/
#if XVT_CC_PROTO
void obmtgp(int32 method, int32 kodor, double *sphi, double *slam,
            double *u, double *v, double *y, double *x,
            double es, double fn, double fe,
            double ogam, double olam, double oaz, 
            double ta, double tb, double te);
#else
void obmtgp();
#endif

/********************************************************************
@    savmgr()
*********************************************************************
Save MGRS
*/
#if XVT_CC_PROTO
void savmgr(
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
void savmgr();
#endif
/*
Description:
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


/*******************************************************************
@    tmorig()
********************************************************************
VALIDATION OF ORIGIN PARAMETERS FOR TRANSVERSE MERCATOR TO GP AND
GP TO TRANSVERSE MERCATOR AND CONVERSION OF ORIGIN TO RADIANS
*/
#if XVT_CC_PROTO
ERRSTATUS
tmorig( COORD_TYPE *coord_type );
#else
ERRSTATUS
tmorig();
#endif

/***************************************************************
@    units_construct()
****************************************************************
Construct the units object
*/

#if XVT_CC_PROTO
ERRSTATUS       MUSE_API
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
ERRSTATUS       MUSE_API
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

 /***************************************************************
@     ver_conv_from()
****************************************************************
Converts from user vertical distance units to internal (meters)
*/
ERRSTATUS       MUSE_API

#if XVT_CC_PROTO
ver_conv_from(short user_unit_index, double *mtr_1, double mtr_2
);
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
@    yxtogp()
****************************************************************
Transverse Mercator map projection to GP
*/
#if XVT_CC_PROTO
void
yxtogp(double a, double recf, double ophi, double olam, double fn, double fe,
       double ok, double *sphi, double *slam, double y, double x);
#else
void
yxtogp();
#endif
/*
Description:
*/


/***************************************************************
@    unflow()
****************************************************************
Check for underflow
*/
#if XVT_CC_PROTO
void unflow(
    double *value,
    int32 kode);
#else
void unflow();
#endif
/*
Description:
*/



/********************************************************************
@    upnset()
*********************************************************************
Set MGRS info for northern polar zone
*/
#if XVT_CC_PROTO
void upnset(
    int32 n,
    int32 *ltrlow,
    int32 *ltrhi,
    double *feltr,
    double *fnltr,
    int32 *ltrhy);
#else
void upnset();
#endif
/*
Description:

*/


/********************************************************************
@    upsset()
*********************************************************************
Set MGRS info for southern polar zone
*/
#if XVT_CC_PROTO
void upsset(
    int32 n,
    int32 *ltrlow,
    int32 *ltrhi,
    double *feltr,
    double *fnltr,
    int32 *ltrhy);
#else
void upsset();
#endif
/*
Description:

*/



/***************************************************************
@    upstgp()
****************************************************************
UPS map projection to geog posn
*/
#if XVT_CC_PROTO
void upstgp(
    double a,
    double recf,
    double *sphi,
    double *slam,
    double *y,
    double x);
#else
void upstgp();
#endif
/*
Description:

*/


/********************************************************************
@    utmlim()
*********************************************************************
UTM lim
*/
#if XVT_CC_PROTO
void utmlim(
    int32 *n,
    double sphi,
    int32 izone,
    double *spsou,
    double *spnor,
    double *sleast,
    double *slwest);
#else
void utmlim();
#endif
/*
Description:

*/


/*********************************************************************
@    utmset
**********************************************************************
UTM set
*/
#if XVT_CC_PROTO
void utmset(
    int32 izone,
    int32 *ltrlow,
    int32 *ltrhi,
    double *fnltr,
    char *isph,
    int32 igrpin);
#else
void utmset();
#endif
/*
Description:

*/


/********************************************************************
@    mgrtgp
*********************************************************************
MGRS to GP
*/
#if XVT_CC_PROTO
void mgrtgp(
    char *mgrs,
    int P1,
    char *azone,
    int P2,
    /*int32 *isph,*/ char *isph,
    int32 *igrpin,
    double *a,
    double *recf,
    double *sphi,
    double *slam,
    int32 *izone,
    double *y,
    double *x,
    int32 *ierr);
#else
void mgrtgp();
#endif
/*
Description:

*/


/********************************************************************
@    radian
*********************************************************************
Radian
*/
#if XVT_CC_PROTO
void
radian(double *rad, double rd, double rm, double rs);
#else
void
radian();
#endif
/*
Description:

*/


/********************************************************************
@    radian_and_dd
*********************************************************************
Radian
*/
#if XVT_CC_PROTO
void
radian_and_dd(double *rad, double *rd, double rm, double rs);
#else
void
radian_and_dd();
#endif
/*
Description:

*/


/********************************************************************
@    shiftr
*********************************************************************
Shiftr
*/
#if XVT_CC_PROTO
void shiftr(
    char *mgrs,
    int P1,
    int32 n,
    int32 *nchar);
#else
void shiftr();
#endif
/*
Description:

*/


/********************************************************************
@    dmsh
*********************************************************************
Radians to degrees, minutes, seconds (????)
*/
#if XVT_CC_PROTO
void dmsh(
    int32 khem,
    double radian,
    int32 *id,
    int32 *im,
    double *rs,
    char *ah,
    int P1);
#else
void dmsh();
#endif
/*
Description:

*/




/*********************************************************************
@    utmtgp
**********************************************************************
Universal Transverse Mercator to GP
*/
#if XVT_CC_PROTO
void utmtgp(
    double a,
    double recf,
    double *sphi,
    double *slam,
    int32 izone,
    double *y,
    double x);
#else
void utmtgp();
#endif
/*
Description:

*/


/**********************************************************************
@    yxtmgr
***********************************************************************
UTM to MGRS and UTM
*/
#if XVT_CC_PROTO
void yxtmgr(
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
    /*int32 *isph,*/char *isph,
    int32 *igrpin,
    int32 *ierr);
#else
void yxtmgr();
#endif
/*
Description:

*/

#if XVT_CC_PROTO
ERRSTATUS
horiz_conv_from_user_units ( short tag , double *hor_1 , double hor_2,
                             double scale_recip, double screen_PPI );
#else
ERRSTATUS
horiz_conv_from_user_units ( );
#endif

#if XVT_CC_PROTO
ERRSTATUS
horiz_conv_to_user_units ( short tag , double hor_1 , double *hor_2,
                           double scale_recip, double screen_PPI );
#else
ERRSTATUS
horiz_conv_to_user_units ( );
#endif

/* Prototypes for functions related to Promula.Fortran library */

typedef struct
{
    double cr;
    double ci;
}  DCOMPLEX;


/*********************************************************************
@    dpxadd
**********************************************************************
Returns the double complex sum of two double complex arguments
*/
#if XVT_CC_PROTO
DCOMPLEX
dpxadd( DCOMPLEX a, DCOMPLEX b);
#else
DCOMPLEX
dpxadd();
#endif

/*********************************************************************
@    dpxdbl
**********************************************************************
Converts a double precision number to a double complex whose real part is the
double precsion argument, and whose imaginary part is zero.
*/
#if XVT_CC_PROTO
DCOMPLEX
dpxdbl( double dbl );
#else
DCOMPLEX
dpxdbl();
#endif

/*********************************************************************
@    dpxdiv
**********************************************************************
Returns the double complex quotient of two double complex arguments.
Based on Cdiv() in Numerical Recipes in C by Press, et alii.
*/
#if XVT_CC_PROTO
DCOMPLEX
dpxdiv( DCOMPLEX a, DCOMPLEX b);
#else
DCOMPLEX
dpxdiv();
#endif

/*********************************************************************
@    dpxdpx
**********************************************************************
Converts the two double arguments to a double complex number, which is returned
*/
#if XVT_CC_PROTO
DCOMPLEX
dpxdpx( double d1, double d2);
#else
DCOMPLEX
dpxdpx( );
#endif

/*********************************************************************
@    dpxima
**********************************************************************
Returns the imaginary part of the double complex argument
*/
#if XVT_CC_PROTO
double
dpxima( DCOMPLEX a );
#else
double
dpxima( );
#endif

/*********************************************************************
@    dpxlongpow
**********************************************************************
Raises the double complex argument to the long argument power and returns the
double complex result
*/
/***DO: Switch to formula using polar coordinates if this isn't accurate enough */
#if XVT_CC_PROTO
DCOMPLEX
dpxlongpow( DCOMPLEX a, int32 b);
#else
DCOMPLEX
dpxlongpow();
#endif

/*********************************************************************
@    dpxmul
**********************************************************************
Returns the double complex product of two double complex arguments.
*/
#if XVT_CC_PROTO
DCOMPLEX
dpxmul( DCOMPLEX a, DCOMPLEX b);
#else
DCOMPLEX
dpxmul();
#endif

/*********************************************************************
@    dpxreal
**********************************************************************
Returns the real part of the double complex argument
*/
#if XVT_CC_PROTO
double
dpxreal( DCOMPLEX a );
#else
double
dpxreal( );
#endif


/*********************************************************************
@    fifdint
**********************************************************************
Source code from the Promula.Fortran library
*/
#if XVT_CC_PROTO
double fifdint(
    double a);
#else
double fifdint();
#endif
/*
Description:

*/


/********************************************************************
@    fifdnint
*********************************************************************
Source code from the Promula.Fortran library
*/
#if XVT_CC_PROTO
double fifdnint(
    double a);
#else
double fifdnint();
#endif
/*
Description:
*/

/**********************************************************************
@    fifdsign
***********************************************************************
Source code from the Promula.Fortran library
*/
#if XVT_CC_PROTO
double
fifdsign( double mag, double sgn );
#else
double
fifdsign( );
#endif



/**********************************************************************
@    fifichar
***********************************************************************
Source code from the Promula.Fortran library
*/
#if XVT_CC_PROTO
int fifichar(
    unsigned char* c1);
#else
int fifichar();
#endif
/*
Description:

*/


/********************************************************************
@    fifidint
*********************************************************************
Source code from the Promula.Fortran library
*/
#if XVT_CC_PROTO
int32 fifidint(
    double a);
#else
int32 fifidint();
#endif
/*
Description:

*/


/*********************************************************************
@    fifmod
**********************************************************************
Source code from the Promula.Fortran library
*/
#if XVT_CC_PROTO
int32 fifmod(
    int32 num,
    int32 dem);
#else
int32 fifmod();
#endif
/*
Description:

*/


/*********************************************************************
@    fifnint
**********************************************************************
Source code from the Promula.Fortran library
*/
#if XVT_CC_PROTO
int32 fifnint(
    double a);
#else
int32 fifnint();
#endif
/*
Description:

*/


/**********************************************************************
@    ftncms
***********************************************************************
Source code from the Promula.Fortran library
*/
#if XVT_CC_PROTO
int ftncms(
    char* s1,
    int n1,
    char* s2,
    int n2);
#else
int ftncms();
#endif
/*
Description:

*/


/**********************************************************************
@    ftnsac
***********************************************************************
Source code from the Promula.Fortran library
*/
#if XVT_CC_PROTO
void ftnsac(
    char* s1,
    int n1,
    char* s2,
    int n2);
#else
void ftnsac();
#endif
/*
Description:

*/

/**********************************************************************
@    dd_to_dm_formatted
***********************************************************************
*/
void
#if XVT_CC_PROTO
dd_to_dm_formatted( double dd, short *deg, double *mn);
#else
dd_to_dm_formatted( );
#endif



/**********************************************************************
@    dd_to_dms_formatted
***********************************************************************
*/

void
#if XVT_CC_PROTO
dd_to_dms_formatted( double dd, short *deg, short *mn, double *sec);
#else
dd_to_dms_formatted( );
#endif



/**********************************************************************
@    hor_m_from_m
***********************************************************************
*/

ERRSTATUS
#if XVT_CC_PROTO
hor_m_from_m(double *hor_mtr1, double hor_mtr2);
#else
hor_m_from_m( );
#endif

ERRSTATUS
#if XVT_CC_PROTO
hor_m_from_ft(double *hor_meter, double hor_feet);
#else
hor_m_from_ft( );
#endif


/**********************************************************************
@    hor_m_from_yd
***********************************************************************
*/


ERRSTATUS
#if XVT_CC_PROTO
hor_m_from_yd(double *hor_meter, double hor_yard);
#else
hor_m_from_yd( );
#endif


/**********************************************************************
@    hor_m_from_in_at_map_scale
***********************************************************************
*/

ERRSTATUS
#if XVT_CC_PROTO
hor_m_from_in_at_map_scale(double *hor_meter, double hor_in, double scale_recip);
#else
hor_m_from_in_at_map_scale( );
#endif




/**********************************************************************
@    hor_m_from_mm_at_map_scale
***********************************************************************
*/

ERRSTATUS
#if XVT_CC_PROTO
hor_m_from_mm_at_map_scale(double *hor_meter, double hor_mm, double scale_recip);
#else
hor_m_from_mm_at_map_scale( );
#endif


/**********************************************************************
@    hor_m_from_pixels
***********************************************************************
*/

ERRSTATUS
#if XVT_CC_PROTO
hor_m_from_pixels ( double *hor_meter , double hor_pix, double scale_recip,
		    double screen_PPI );
#else
hor_m_from_pixels (   );
#endif


/**********************************************************************
@    hor_m_to_m
***********************************************************************
*/

ERRSTATUS
#if XVT_CC_PROTO
hor_m_to_m(double hor_mtr1, double *hor_mtr2);
#else
hor_m_to_m( );
#endif




/**********************************************************************
@    hor_m_to_ft
***********************************************************************
*/

ERRSTATUS
#if XVT_CC_PROTO
hor_m_to_ft(double hor_meter, double *hor_feet);
#else
hor_m_to_ft( );
#endif



/**********************************************************************
@    hor_m_to_yd
***********************************************************************
*/

ERRSTATUS
#if XVT_CC_PROTO
hor_m_to_yd(double hor_meter, double *hor_yard);
#else
hor_m_to_yd( );
#endif



/**********************************************************************
@    hor_m_to_in_at_map_scale
***********************************************************************
*/

ERRSTATUS
#if XVT_CC_PROTO
hor_m_to_in_at_map_scale  ( double hor_meter, double *hor_in, double scale_recip );
#else
hor_m_to_in_at_map_scale( );
#endif



/**********************************************************************
@    hor_m_to_mm_at_map_scale
***********************************************************************
*/

ERRSTATUS
#if XVT_CC_PROTO
hor_m_to_mm_at_map_scale( double hor_meter, double *hor_mm, double scale_recip );
#else
hor_m_to_mm_at_map_scale( );
#endif




/**********************************************************************
@    hor_m_to_pixels
***********************************************************************
*/

ERRSTATUS
#if XVT_CC_PROTO
hor_m_to_pixels ( double hor_meter , double *hor_pix, double scale_recip,
		  double screen_PPI );
#else
hor_m_to_pixels (  );
#endif



/**********************************************************************
@    madlgp
***********************************************************************
*/

#if  XVT_CC_PROTO
ERRSTATUS
madlgp(double a,double recf,double *sphi,double *slam,double y,double x,
       double *ygs,double *xgs,double ophi,double ophis,double olam,
       double fn,double fe,double ok,double al,double Const,double r,
       double ca, double cb, double cc, double cd);
#else
ERRSTATUS
madlgp( );
#endif


/**********************************************************************
@    madlor
***********************************************************************
*/


#if XVT_CC_PROTO
ERRSTATUS
madlor(COORD_TYPE *coord_type,
       double a,double recf,double *parism, double *ophi, double *ophis,
       double *olam, /*double *fn,double *fe,double *ok, */
       double *al,double *Const,
       double *r,double *ca, double *cb, double *cc, double *cd);
#else
ERRSTATUS
madlor( );
#endif



/**********************************************************************
@    convert_user_str_to_pt
***********************************************************************
*/

#if XVT_CC_PROTO
ERRSTATUS
convert_user_str_to_pt(COORD_SYS *coord_in, POINT_UNITS *upoint, char *pntstr);
#else
ERRSTATUS
convert_user_str_to_pt( );
#endif


/**********************************************************************
@    gplam()
***********************************************************************
*/

void
#if XVT_CC_PROTO
gpclam(double a, double recf, double ophi, double olam, double ok,
            double sphi, double slam, double *conv, double *sk);
#else
gpclam();
#endif

/**********************************************************************
@    gpcmer()
***********************************************************************
*/

void
#if XVT_CC_PROTO
gpcmer(double a, double recf, double ok, double sphi, double *conv,
       double *sk);
#else
gpcmer();
#endif

/**********************************************************************
@    gpcnzm()
***********************************************************************
*/

void
#if XVT_CC_PROTO
gpcnzm(double a, double recf, double ophi, double olam,
	    double sphi, double slam, double *conv, double *sk);
#else
gpcnzm();
#endif


/**********************************************************************
@    gpcobm()
***********************************************************************
*/

void
#if XVT_CC_PROTO
gpcobm(double a, int32 kodor, double sphi, double slam, double u,
       double *conv, double *sk, double olam, double e, double es,
       double ogam, double oaz, double ta, double tb, double te);
#else
gpcobm();
#endif

/**********************************************************************
@    gpconv_tm()
***********************************************************************
*/

void
#if XVT_CC_PROTO
gpconv_tm(double a, double recf, double olam, double ok, double sphi,
       double slam, double *conv, double *sk);
#else
gpconv_tm();
#endif

/**********************************************************************
@    gpconv_ups()
***********************************************************************
*/

void
#if XVT_CC_PROTO
gpconv_ups(double a, double recf, double sphi, double slam,
       double *conv, double *sk);
#else
gpconv_ups();
#endif

/**********************************************************************
@    gpconv_utm()
***********************************************************************
*/

void
#if XVT_CC_PROTO
gpconv_utm( double a, double recf, double sphi, double slam,
	     int32 izone, double *conv, double *sk);
#else
gpconv_utm();
#endif


/**********************************************************************
@    gpmadl
***********************************************************************
*/

#if  XVT_CC_PROTO
ERRSTATUS
gpmadl(double a,double recf,double sphi,double slam,
       double *y,double *x,double *ygs,double *xgs,
       double ophis,double olam,double fn,double fe,double ok,double al,
       double Const,double r,double ca,double cb);
#else
ERRSTATUS
gpmadl( );
#endif


/**********************************************************************
@    gpnzmg
***********************************************************************
*/
#if XVT_CC_PROTO
void gpnzmg(double a, double ophi, double olam, double fn, double fe,
            double sphi, double slam, double *y, double *x);
#else
void gpnzmg( );
#endif



/**********************************************************************
@    nzmgor
***********************************************************************
*/
void
#if XVT_CC_PROTO
nzmgor( COORD_TYPE *coord_type );
#else
nzmgor( );
#endif



#endif


