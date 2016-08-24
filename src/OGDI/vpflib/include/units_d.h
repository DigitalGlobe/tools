#ifndef H_UNITS_D
#define H_UNITS_D

#ifndef H_MUSE1
#include "muse1.h"
#endif
#ifndef H_DTCC_D
#include "dtcc_d.h"
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
    char            list[20][30];	/* Change: 20 strings instead of 10 */
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
#endif				/* H_UNITS_D */
