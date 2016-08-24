#ifndef H_COORD_DEF

#define H_COORD_DEF

#ifndef INCL_XVTH
#include "xvt.h"
#endif
#ifndef H_MUSE1
#include "muse1.h"
#endif
#ifndef H_DATUM_DEF
#include "datum_d.h"
#endif
/*#include "math.h"*/

#define SZ_COORD_SYS 651   /* not used */
/***************************************************************
@    Misc. Constants
****************************************************************
*/
#define NORTH_HEMI                     0
#define SOUTH_HEMI                     1
#define EAST_HEMI                      0
#define WEST_HEMI                      1
#define USER_DEFINED                   0
#define GET_USER_POINT                 20000
#define MAX_COORD_NAME_SIZE            60
#define MAX_COORD_RECORD_SIZE          315
#define MAX_NO_OF_COORD_TYPES          50
#define MAX_COORD_UNIT_NAME_SIZE       /*10*/  20 /* 02/22/94 */
#define MAX_COORD_PROJ_NAME_SIZE       30
#define PNT_LABEL_LEN                  13

/***************************************************************
@    MISCELLANEOUS DEFINES
****************************************************************
*/
#define MUSE_ERROR(n) xvt_error(n)
#define MUSE_LOG_CHECK  /* enables check for negative log argument */


/***************************************************************
@    COORD_ENUM
****************************************************************
*/
typedef enum
{
    COORD_GP,
    COORD_UTM,
    COORD_UPS,
    COORD_MGRS,
    COORD_XYZ,
    COORD_AE,
    COORD_AKRSO,
    COORD_GEOREF,
    COORD_LAM,
    COORD_MADL,
    COORD_MER,
    COORD_MRSO,
    COORD_NZMG,
    COORD_OBM,
    COORD_TM
} COORD_ENUM;
/*
Description: User's choice for type of point coordinates.
*/

/***************************************************************
@    PROJ_ENUM
****************************************************************
*/
typedef enum
{
	UNIT_METERS,
	UNIT_FEET,
	UNIT_YARDS,
	UNIT_INMS,
	UNIT_MMMS,
        UNIT_PIXMS
} PROJ_ENUM;
/*
Description: User's choice for type of point coordinates.
*/

/***************************************************************
@    GP_ENUM
****************************************************************
*/
typedef enum
{
	UNIT_RAD,
        UNIT_GRAD,
	UNIT_DD,
	UNIT_DM,
	UNIT_DMS
} GP_ENUM;
/*
Description: User's choice for type of point coordinates.
*/

/***************************************************************
@    UNIT_ENUM
****************************************************************
*/

typedef struct
{
	BOOLEAN 	gp ;
	GP_ENUM		gp_enum ;
	PROJ_ENUM 	proj_enum;
	BOOLEAN         uses_paris_merid;
} UNIT_ENUM;
/*
Description: User's choice for type of point coordinates.
*/

/***************************************************************
@   USER_CONTROL    
****************************************************************
*/
typedef enum
{
    USER_DEFINABLE,
    INTERNALLY_DEFINED,
    NOT_APPLICABLE
} USER_CONTROL;
/*
Description: Control dialog interface for each instance
*/

/***************************************************************
@  DATUM_REF
****************************************************************
*/
typedef struct
{
   char index[6];
   USER_CONTROL user_control;
} DATUM_REF;
/*
Description:
*/

/***************************************************************
@  ELLIPS_REF
****************************************************************
*/
typedef struct
{
   char index[3];
   USER_CONTROL user_control;
} ELLIPS_REF;
/*
Description:
*/

/***************************************************************
@  COORD_UNIT
****************************************************************
*/
typedef struct
{
   UNIT_ENUM unit_enum;
   char    name[MAX_COORD_UNIT_NAME_SIZE];
   USER_CONTROL user_control;
} COORD_UNIT;
/*
Description:
*/

/***************************************************************
@  FN
****************************************************************
*/
typedef struct
{
   double   value;
   USER_CONTROL  user_control;
   BOOLEAN valid;
} FN;
/*
Description:
*/

/***************************************************************
@  FE
****************************************************************
*/
typedef struct
{
   double  value;
   USER_CONTROL user_control;
   BOOLEAN valid;
} FE;
/*
Description:
*/

/***************************************************************
@  COORD_PROJ
****************************************************************
*/
typedef struct
{
   COORD_ENUM   coord_enum;
   char		name[MAX_COORD_PROJ_NAME_SIZE];
   USER_CONTROL user_control;
} COORD_PROJ;
/*
Description:
*/

/***************************************************************
@    COORD_DMS
****************************************************************
A geo-coordinate in deg, min, sec
*/
typedef struct
{
    short           d;		/* degrees */
    short           m;		/* minutes */
    GFLOAT          s;		/* seconds */
}               COORD_DMS;

/*
 * Description:
 */
/***************************************************************
@    POINT_DMS
****************************************************************
A point id deg, min, sec
*/
typedef struct
{
    COORD_DMS       lon;
    COORD_DMS       lat;
}               POINT_DMS;

/*
 * Description:
 */

/***************************************************************
@  ORIGIN_LAT
****************************************************************
*/
typedef struct
{
   COORD_DMS  point;
   USER_CONTROL user_control;
   BOOLEAN valid;
} ORIGIN_LAT;
/*
Description:
*/

/***************************************************************
@  ORIGIN_LON
****************************************************************
*/
typedef struct
{
   COORD_DMS point;
   USER_CONTROL user_control;
   BOOLEAN valid;
} ORIGIN_LON;
/*
Description:
*/

/***************************************************************
@  SP1
****************************************************************
*/
typedef struct
{
   COORD_DMS    point;
   USER_CONTROL user_control;
   BOOLEAN valid;
} SP1;
/*
Description:
*/

/***************************************************************
@  SP2
****************************************************************
*/
typedef struct
{
   COORD_DMS    point;
   USER_CONTROL user_control;
   BOOLEAN valid;
} SP2;
/*
Description:
*/

/***************************************************************
@  CENTRAL_AZIMUTH
****************************************************************
*/
typedef struct
{
   COORD_DMS    point;
   USER_CONTROL user_control;
   BOOLEAN valid;
} CENTRAL_AZIMUTH;
/*
Description:
*/

/***************************************************************
@  CENTRAL_SCALE
****************************************************************
*/
typedef struct
{
   double  value;
   USER_CONTROL user_control;
   BOOLEAN valid;
} CENTRAL_SCALE;
/*
Description:
*/

/***************************************************************
@  EXTENTS
****************************************************************
*/
typedef struct
{
   double  value;
   USER_CONTROL user_control;
   BOOLEAN valid;
} EXTENTS;
/*
Description:
*/

/***************************************************************
@  SCALE_RECIP
 *****************************************************************/
typedef struct
{
   double  value;
   USER_CONTROL user_control;
   BOOLEAN valid;
} SCALE_RECIP;
/*
Description:
*/

/***************************************************************
@  SCREEN_PPI
 *****************************************************************/

typedef struct
{
   double  value;
   USER_CONTROL user_control;
   BOOLEAN valid;
}  SCREEN_PPI;
/*
Description:
*/

/***************************************************************
@  COORD_EXTENT
 *****************************************************************/

typedef struct
{
   double  value;
   USER_CONTROL user_control;
   BOOLEAN valid;
}  COORD_EXTENT;
/*
Description:
*/

/***************************************************************
@    COORD_DM
****************************************************************
A geo-coordinate in deg, min, sec
*/
typedef struct
{
    short           d;		/* degrees */
    GFLOAT          m;		/* minutes */
}               COORD_DM;

/*
 * Description:
 */


/***************************************************************
@    POINT_DD
****************************************************************
A Point in decimal degrees
*/
typedef struct
{
    GFLOAT          lon;
    GFLOAT          lat;
    GFLOAT          height;
}               POINT_DD;

/*
 * Description:
 */

/***************************************************************
@    POINT_XY
****************************************************************
Any point POINT_DD structure
*/

typedef struct
{
 	GFLOAT 		x;
	GFLOAT		y;
} 		POINT_XY;

/*
 * Description:
 */

/***************************************************************
@    POINT_XYZ
****************************************************************
A Point in decimal degrees
*/
typedef struct
{
    GFLOAT          x;
    GFLOAT          y;
    GFLOAT	    z;
}               POINT_XYZ;

/*
 * Description:
 */

/***************************************************************
@    POINT_DM
****************************************************************
A geo-coordinate in deg, min
*/
typedef struct
{
    COORD_DM     lon;		
    COORD_DM     lat;
    GFLOAT       height;
}               POINT_DM;


/***************************************************************
@    POINT_GEOREF
****************************************************************
A point in GEOREF
*/
typedef struct
{
    char            quad_15deg[3];
    char            quad_1deg[3];
    int32            lonminutes;
    int32            latminutes;
    char            georef[20]; /* string representation of above */
    GFLOAT          height;
}               POINT_GEOREF;

/*
 * Description:
 */


/***************************************************************
@    POINT_MGRS
****************************************************************
A point in Military Grid reference System
This is struct is to be determined
*/
typedef struct
{
    char            mgrs[30];
    GFLOAT          height;
}               POINT_MGRS;

/*
 * Description:
 */

/***************************************************************
@    POINT_UTM
****************************************************************
A point in Universal Transverse Mercator projection
*/
typedef struct
{
    int             zone;
    GFLOAT          northing;
    GFLOAT          easting;
    GFLOAT          height;
}               POINT_UTM;

/*
 * Description:
 */

/***************************************************************
@    POLAR_PARAMS
****************************************************************
Parameters for the ADRG Polar projection
*/
typedef struct
{
    GFLOAT          lat0;
    GFLOAT          lon0;
    GFLOAT          brv;
}               POLAR_PARAMS;

/*
 * Description:
 */

/***************************************************************
@    POINT_UNITS
****************************************************************
Union of available types of point-oriented user units.
*/
typedef union
{
    POINT_DD        gpoint;  /* changed from MUSE structure */
    POINT_DM        dm;      /* added to MUSE structure */
    POINT_DMS       dms;
    POINT_UTM       utm;
    POINT_MGRS      mgrs;
    POINT_GEOREF    georef;
    POINT_XYZ       otherpnt;
}               POINT_UNITS;
/***
Description: Used in the conversion functions for geographic units.
***/

/***************************************************************
@    POINT_TYPE
****************************************************************
Union of available types of point-oriented user units.
*/
typedef struct
{
    char            promptstr[40];  /*needed by Fusion */
    POINT_UNITS     point;
    BOOLEAN	    valid;
    char	    label[PNT_LABEL_LEN];
    int             gp_enum;
    double          north_extent, south_extent, east_extent, west_extent;
}               POINT_TYPE;
/***
Description: Used in the conversion functions for geographic units.
***/


/***************************************************************
@  GEO_EXTENT
 *****************************************************************/

typedef struct
{
   POINT_UNITS  point;
   USER_CONTROL user_control;
   BOOLEAN valid;
}  GEO_EXTENT;
/*
Description:
*/

/***************************************************************
@  COORD_PREF
 *****************************************************************/

typedef enum
{
	OFFSET_AND_SCALE,
	CENTER_POINT_SCALE_IMAGE_EXTENT,
	GEO_EXTENT_SCALE,
	GEO_AND_IMAGE_EXTENT
}  COORD_PREF;
/*
Description:
*/

/***************************************************************
@  COORD_TYPE
****************************************************************/

typedef struct
{
/*
	 THESE VALUES ARE RETRIEVED FROM THE COORDSYS.DAT
   	 FILE.
*/
   char              name[MAX_COORD_NAME_SIZE];
   BOOLEAN           valid;
   BOOLEAN           uses_ellipsoid;
   DATUM_REF         datum_ref;
   ELLIPS_REF        ellips_ref;
   COORD_UNIT        coord_unit;
   FN                fn;
   FE                fe;
   COORD_PROJ        coord_proj;
   ORIGIN_LAT        origin_lat;
   ORIGIN_LON        origin_lon;
   SP1               sp1;
   SP2               sp2;
   CENTRAL_AZIMUTH   central_azimuth;
   CENTRAL_SCALE     central_scale;
   EXTENTS	     north,south,east,west;
   SCALE_RECIP       scale_reciprocal;
/*
	THESE VALUES ARE RETRIEVED FROM THE CLIENT
	CALLING THIS PROGRAM.
*/
   SCREEN_PPI	     screen_ppi;
   COORD_EXTENT      height,width;
   GEO_EXTENT        geo_upper_right,geo_lower_left,geo_center;
   COORD_PREF	     coord_pref;
/*      THESE VALUES ARE DERIVED BY THE "ORIGIN FUNCTIONS" OF THE
        COORDINATE TYPES (EG TMORIG.C)
*/
   double            ophi, olam;    /* origin lat and lon in radians */
   double            b, e, es;      /* additional ellipsoid parameters */
   double            ogam;          /* origin meridian convergence angle in radians */
   double            oaz;           /* central azimuth in radians */
   double            ta, tb, te;    /* projection constants for Oblique Mercator */
   int32              method, kodor; /* setup codes for Oblique Mercator */
} COORD_TYPE;
/*
Description: Structure for storing one record from the ellipsoids
file.  Fields are the ellipsoid name, semi-major axis, flattening,
and eccentricity squared.
*/


/***************************************************************
@  COORD_FILE_INFO
****************************************************************
*/
typedef struct
{
    COORD_TYPE  coord[MAX_NO_OF_COORD_TYPES];
    int         coord_read;
}  COORD_FILE_INFO;
/*
Description: Structure for storing all pre-defined ellipsoids parameters
from ellipsoids file, as well as total number of ellipsoids read, index of
current local ellipsoid, and flag indicating whether parameters have been
entered for the user-specified ellipsoid. ellipsoid_selected is the
same value as datum_info->datums[datum_info->datum_selected].ellips_num.
*/


/***************************************************************
@    ARCPOINT
****************************************************************
ARC map projection parameters
*/

typedef struct
{
    int32            row;
    int32            col;
}               ARCPOINT;
/*
 * Description:
 */

/***************************************************************
@    ASPECT
****************************************************************
Aspect of a map projection
*/
typedef struct
{
    GFLOAT          plon;
    GFLOAT          plat;
    GFLOAT          plat1;
    GFLOAT          plat2;
}               ASPECT;

/*
 * Description: Plon and plat are the points of tangency of the developable
 * surface.  Plat1 and Plat2 are the standard parallels for the Lambert
 * Conformal Conic.
 */



/***************************************************************
@   DIR_ENUM 
****************************************************************
Transformation direction
*/
typedef enum
{
    TO_WGS84,
    FROM_WGS84
}
         DIR_ENUM;
/*
 * Description:
 */


/***************************************************************
@    GEOGRAPHIC
****************************************************************
Geographic coordinates are stored internally in DD.
*/
typedef enum
{
    DD,
    DMS,
    GEOREF,
    MGRS,
    MIN,
    SEC,
    UTM
}
                GEOGRAPHIC;

/*
 * Description:
 */

/***************************************************************
@  GRID_PARAMS
****************************************************************
*/
typedef struct
{
    double a;
    double b;
    double e;
    double es;
    double fe;
    double fn;
    double oaz;
    double ogam;
    double ok;
    double olam;
    double slam;
    double ta;
    double tb;
    double te;
} GRID_PARAMS;
/*
 * Description:
 */

/***************************************************************
@    HOR_DATUM
****************************************************************
Horizontal datums
*/
typedef enum
{
    NDEFINED,
    HD_WGS84,
    HD_WGS72,
    HD_NAD27,
    HD_EUD50
}
                HOR_DATUM;

/*
 * Description:
 */

/***************************************************************
@    LAMBER_PARAMS
****************************************************************
Lambert Conformal Conic map projection parameters
*/
typedef struct
{
    GFLOAT          n;
    GFLOAT          F;
    GFLOAT          rho0;
}               LAMBER_PARAMS;

/*
 * Description:
 */

/***************************************************************
@    LINEAR_PARAMS
****************************************************************
Linear (Platt-Carree) map projection parameters
*/
typedef struct
{
    GFLOAT          dummy;
}               LINEAR_PARAMS;

/*
 * Description:
 */

/***************************************************************
@    MERCAT_PARAMS
****************************************************************
Mercator map projection parameters
*/
typedef struct
{
    GFLOAT          dummy;
}               MERCAT_PARAMS;

/*
 * Description:
 */


/***************************************************************
@    DTCC_OUTPUT
****************************************************************
Data returned by main coordinate conversion and datum transformation
routine
*/
typedef struct
{
     POINT_UNITS    output_pt;
     POINT_DD       out_gp_rad_pt;
     POINT_XYZ      trans_error;
     char           err_message[80];
}    DTCC_OUTPUT;
/*
 * Description:
 */

/***************************************************************
@    PROJ_DIR
****************************************************************
Map projection direction
*/
typedef enum
{
    PROJ_FORWARD,
    PROJ_REVERSE
}
                PROJ_DIR;

/*
 * Description: PROJ_FORWARD tells map_it() to convert from grographic
 * coordinates to PHIGS graphics coordinates. PROJ_REVERSE tells map_it() to
 * convert from PHIGS graphics coordinates back to geographic coordinates.
 */

/***************************************************************
@    PROJ_NAME_LEN
****************************************************************
Limit on length of a projection name
*/
#define PROJ_NAME_LEN 30

/*
 * Description:
 */


/***************************************************************
@    PROJECTION
****************************************************************
Map projections
*/
typedef enum
{
    PROJ_UNDEFINED,
    PROJ_LAMBERT1,
    PROJ_LAMBERT2,
    PROJ_LINEAR,
    PROJ_MERCATOR,
    PROJ_UPS,
    PROJ_UTM,
    PROJ_GRAPH,
    PROJ_POLAR,
    PROJ_ARC,
    PROJ_TS
}
                PROJECTION;

/*
 * Projections to be added in the future: PROJ_BONNE, PROJ_MOLLWEIDE,
 * PROJ_ORTHOGRAPHIC, PROJ_STEREOGRAPHIC, PROJ_TRANSVERSE_MERCATOR,
 * 
 * Description:
 */

/***************************************************************
@    STEREO_PARAMS
****************************************************************
Stereographic map projection parameters
*/
typedef struct
{
    GFLOAT          dummy;
}               STEREO_PARAMS;

/*
 * Description:
 */


/***************************************************************
@   TRANSFORM_PARAMS 
****************************************************************
*/
typedef struct
{
   char    from_ellips[3]; /* isph in FORTRAN; nnfr in NT MADTRAN */
   double  from_a;      
   double  from_f;     
   double  from_e_sq;   
   double  da;          
   double  df;         
   int     dx;        
   int     dy;       
   int     dz;      
   int     ex;     
   int     ey;    
   int     ez;
} TRANSFORM_PARAMS;
/*
Description: Structure used to store all parameters needed by
the transformation algorithm.  
*/


/***************************************************************
@    TRANSM_PARAMS
****************************************************************
Transverse Mercator map projection parameters
*/
typedef struct
{
    int32            izone;
}               TRANSM_PARAMS;
/*
 * Description:
 */

/***************************************************************
@    TS_PARAMS
****************************************************************
Tessellated Spheroid map projection parameters
*/
typedef struct
{
    short           ts_zone;
    short           ts_scale;
}               TS_PARAMS;

/*
 * Description:
 */


/***************************************************************
@    UNITS_TYPE
****************************************************************
List of available types of units.
*/
typedef union
{
    GFLOAT          dd2;
    COORD_DMS       dms;
    POINT_UTM       utm;
    POINT_MGRS      mgrs;
    POINT_GEOREF    georef;
    GFLOAT          mn;
    GFLOAT          sec;
}               UNITS_TYPE;

/*
 * Description:
 */

/***************************************************************
@    VER_DATUM
****************************************************************
*/
typedef enum
{
    UDEFINED,
    VD_ELIPSOID,
    VD_MSL
}
                VER_DATUM;

/*
 * Description:
 */

/***************************************************************
@    ARC_PARMS
****************************************************************
*/
typedef struct
{
    int32            scale;
    POINT_DD          origin;
    ARCPOINT        ul;
    ARCPOINT        lr;
    int             tile_size;
    int             zone;
}               ARC_PARMS;
/*
 * Description:
 */

/***************************************************************
@    DATUM
****************************************************************
Structure to contain the horizontal and vertical datums
*/
typedef struct
{
    HOR_DATUM       hor_datum;
    VER_DATUM       ver_datum;
}               DATUM;

/*
 * Description:
 */


/***************************************************************
@    MAP_POINT
****************************************************************
Data needed when getting a new user point
*/
typedef struct
{
    char            prompt_str[80];
    /* char *user_pt_str[40]; */
    POINT_UNITS     user_pt;
    POINT_DD          dd_pt;
}               MAP_POINT;

/*
 * Description: Data that is either needed by the dialog box that prompts
 * user to enter a new data point, or is returned by the box.
 */

/***************************************************************
@    PARAMS
****************************************************************
The union of projection-specific parameters
*/
typedef union
{
    LINEAR_PARAMS   linear_params;
    LAMBER_PARAMS   lamber_params;
    MERCAT_PARAMS   mercat_params;
    TRANSM_PARAMS   transm_params;
    ARC_PARMS       arc_params;
    TS_PARAMS       ts_params;
    POLAR_PARAMS    polar_params;
}               PARAMS;

/*
 * Description:
 */


/***************************************************************
@  COORD_SYS
****************************************************************
*/
typedef struct
{
    COORD_TYPE          coord; 
    DATUM_TYPE          datum;
    ELLIPS_PARAMS       ellips;
    BOOLEAN             valid;
} COORD_SYS ;
/*
 * Description:
 */


#endif    /* H_COORD */




