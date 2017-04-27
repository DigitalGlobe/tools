#ifndef H_ELLIPS_DEF

#define H_ELLIPS_DEF

#ifndef INCL_XVTH
#include "xvt.h"
#endif


/***************************************************************
@    WGS 84 ellipsoid parameters
****************************************************************
*/
#define WGS84_A            6378137.0
#define WGS84_F            1.0 / 298.257223563
#define WGS84_E_SQ         2.0 * WGS84_F - WGS84_F * WGS84_F
#define WGS84_ELLIPS_INDEX 21
/*
Description: Semi-major axis, flattening, eccentricity squared,
and index in the ellipsoids array
*/

/***************************************************************
@    Max Constants
****************************************************************
*/
#define MAX_ELLIPS_NAME_SIZE       60
#define MAX_ELLIPS_RECORD_SIZE     100
#define MAX_NO_OF_ELLIPSOIDS       30
/*
Description: First three constants are used to declare strings
for storing datum country\area, datum name, and ellipsoid name.  Next
two are for declaring strings to read in each line of datum.dat and
ellips.dat.  Last two are used to declare the arrays which store the
information in the datum and ellipsoid input files
*/

/* API */
/* TEMP: move #define and IPC to a more suitable location */

#define SIZE_FILENAME SZ_FNAME

/***************************************************************
@  IPC
***************************************************************/
typedef struct
{
   char	 client_name[SIZE_FILENAME];
   short server_id;
} IPC;
/*
Description: Structure for storing identifiers used in interprocess communication between a client program 
and a server program.
*/

/***************************************************************
@  ELLIPS_PARAMS
***************************************************************/
typedef struct
{
   char		name[MAX_ELLIPS_NAME_SIZE];
   double	a_in_meters;   /* semi-major axis in meters */
   double	f;             /* flattening */
   double	e_sq;          /* eccentricity squared */
   char		code[3] ;      /* alphanumeric code identifier */
   IPC      ipc;  /* info for inter-process, client-server communication */ 
   BOOLEAN	valid;
} ELLIPS_PARAMS;
/*
Description: Structure for storing one record, containing the parameters for one ellipsoid, from the 
ellipsoids file, "xellips.dat". 
*/

#if 0
/***************************************************************
@  ELLIPS_TYPE
****************************************************************
*/
typedef struct
{
   char    name[MAX_ELLIPS_NAME_SIZE];
   double  a_in_meters;
   double  f;
   double  e_sq;
   char	   index[3] ;
   BOOLEAN valid;
} ELLIPS_TYPE;
/*
Description: Structure for storing one record from the ellipsoids
file.  Fields are the ellipsoid name, semi-major axis, flattening,
and eccentricity squared.
*/
#endif

/***************************************************************
@  ELLIPS_FILE_INFO
****************************************************************
*/
typedef struct
{
    ELLIPS_PARAMS ellipsoids[MAX_NO_OF_ELLIPSOIDS];
    int         ellipsoids_read;
    int         input_ellipsoid; /*new--replaces ellips_selected */
    int         output_ellipsoid;/* new */
    BOOLEAN     need_user_info;
}  ELLIPS_FILE_INFO;
/*
Description: Structure for storing all pre-defined ellipsoids parameters
from ellipsoids file, as well as total number of ellipsoids read, index of
current local ellipsoid, and flag indicating whether parameters have been
entered for the user-specified ellipsoid. ellipsoid_selected is the
same value as datum_info->datums[datum_info->datum_selected].ellips_num.
*/


#endif /* H_ELLIPS_DEF */




