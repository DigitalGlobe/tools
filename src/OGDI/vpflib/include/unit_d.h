#ifndef H_UNIT_DEF

#define H_UNIT_DEF

#ifndef H_COORD_DEF
#include "coord_d.h"
#endif

/***************************************************************
@    UNIT_LIST
****************************************************************
List of available units of measure.
*/
typedef struct
{
    char            label[30];
    short           count;
    short           setting;
    char            list[20][30]; /*Change: 20 strings instead of 10 */
    char            help_topic[49];
}               UNIT_LIST;

/*
 * Description:
 */


/***************************************************************
@    UNITS_LIST
****************************************************************
The list of lists of available units
*/
typedef struct
{
    char            label[30];
    short           count;
    short           setting;
    char            list[7][15];
    UNIT_LIST       units[8];
}               UNITS_LIST;

/*
 * Description:
 */

/***************************************************************
@    AREA
****************************************************************
Area is stored internally in SQ_METERS
*/
typedef enum
{
    SQ_METERS,
    SQ_FEET,
    SQ_KM,
    SQ_NM,
    SQ_SM
}
                UAREA;

/*
 * Description:
 * 
 */


/***************************************************************
@    VOLUME
****************************************************************
Volume is stored internally in CU_METERS
*/
typedef enum
{
    CU_METERS,
    CU_FT,
    CU_KM,
    CU_NM,
    CU_SM
}
                VOLUME;

/***************************************************************
@    HOR_DISTANCE
****************************************************************
Horizontal distance is stored internally in meters.
*/
typedef enum
{
    HOR_METERS,
    HOR_FEET,
    HOR_KILOMETERS,
    HOR_LEAGUES,
    HOR_NAUTICAL_MILES,
    HOR_STATUTE_MILES,
    HOR_SEC_OF_ARC,
    HOR_YARDS
}
                HOR_DISTANCE;

/*
 * Description:
 * 
 */

/***************************************************************
@    SCALE
****************************************************************
Scale is stored internally as SCALE_RECIPROCAL.
*/
typedef enum
{
    SCALE_RECIPROCAL,
    SCALE_IN_PER_DEG
}
                SCALE;

/*
 * Description:
 */

/***************************************************************
@    VER_DISTANCE
****************************************************************
Vertical distance is stored internally in meters.
*/
typedef enum
{
    VER_METERS,
    VER_FATHOMS,
    VER_FEET
}
                VER_DISTANCE;

/*
 * Description:
 */


/***************************************************************
@    UNITS
****************************************************************
Structure to contain the users units selections
*/
typedef struct
{
    int32            magic;
    SCALE           scale;
    HOR_DISTANCE    hor_distance;
    VER_DISTANCE    ver_distance;
    UAREA            area;
    VOLUME          volume;
    GEOGRAPHIC      geographic;
}               UNITS;

#endif /* H_UNIT_DEF */
