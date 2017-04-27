#ifndef H_DATUM_DEF

#define H_DATUM_DEF

#ifndef INCL_XVTH
#include "xvt.h"
#endif

#ifndef H_ELLIPS_DEF
#include "ellips_d.h"
#endif


/***************************************************************
@    Max Constants
****************************************************************
*/
#define MAX_DATUM_AREA_SIZE        55
#define MAX_DATUM_NAME_SIZE        45
#define MAX_DATUM_RECORD_SIZE     250
#define MAX_NO_OF_DATUMS          300
/*
Description: First three constants are used to declare strings
for storing datum country\area, datum name, and ellipsoid name.  Next
two are for declaring strings to read in each line of datum.dat and
ellips.dat.  Last two are used to declare the arrays which store the
information in the datum and ellipsoid input files
*/

/***************************************************************
@  DATUM_TYPE
****************************************************************
*/
typedef struct
{
   char    name[MAX_DATUM_NAME_SIZE];
   char    ellips_num[3];
   int     dx;
   int     dy;
   int     dz;
   char    area[MAX_DATUM_AREA_SIZE];
   int     ex;
   int     ey;
   int     ez;
   int	   n_points;
   char    index[6];
   BOOLEAN valid;
} DATUM_TYPE;
/*
Description: Structure for storing one record from the datums
file.  Fields are datum name, index into ellipsoid array for
the datum's ellipsoid, ellipsoid center translations, country/
area name, and transformation sigmas.
*/

/***************************************************************
@  DATUM_FILE_INFO
****************************************************************
*/
typedef struct
{
    DATUM_TYPE  datums[MAX_NO_OF_DATUMS];
    int         datums_read;
    int         input_datum;    /* new--replaces datum_selected */
    int         output_datum;   /* new */
    BOOLEAN     need_user_info;
}  DATUM_FILE_INFO;
/*
Description: Structure for storing all pre-defined datums parameters
from datums file, as well as total number of datums read, index of
current local datum, and flag indicating whether parameters have been
entered for the user-specified datum.
*/



#endif /* H_DATUM_DEF */

