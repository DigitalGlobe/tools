#ifndef H_PHIGS_D
#define H_PHIGS_D

#ifndef H_MUSE1
#include "muse1.h"
#endif
#ifndef H_VIEW_DEF
#include "view_d.h"
#endif

/***************************************************************
@PHIGS #Defines
****************************************************************
*/


/***************************************************************
@    PHIGS Line Primitives
****************************************************************
PHIGS polyline definitions.
*/
#define PLINE_SOLID      1
#define PLINE_DASH       2
#define PLINE_DOT        3
#define PLINE_DOT_DASH   4
/*
Description:
Definitions for PHIGS line styles.
*/

/***************************************************************
@    PHIGS Marker Primitives
****************************************************************
PHIGS polymarker definitions.
*/
#define PMARKER_DOT      1
#define PMARKER_PLUS     2
#define PMARKER_ASTERICK 3
#define PMARKER_CIRCLE   4
#define PMARKER_CROSS    5
/*
Description:
Definitions for PHIGS marker styles.
*/

/***************************************************************
@    PHIGS Color Primitive
****************************************************************
PHIGS color definitions.
*/
#define BLACK            0
#define WHITE            1
#define RED              2
#define GREEN            3
#define BLUE             4
#define YELLOW           5
#define CYAN             6
#define MAGENTA          7
/*
Description:
Definitions for PHIGS color primitives for lines,
markers and text.
*/

/***************************************************************
@    PHIGS Primitives
****************************************************************
PHIGS primitive definitions.
*/
#define PLINE            0
#define PMARKER          1
#define PTEXT            2
#define PEDGE            3
#define PAREA            4
/*
Description:
*/

/***************************************************************
@    Pconnid
****************************************************************
A PHIGS connection id.
*/
typedef char* Pconnid;
/*
Description:

*/

/***************************************************************
@    Pfloat
****************************************************************
A PHIGS float.
*/
typedef double Pfloat;
/*
Description:

*/

/***************************************************************
@    Pint
****************************************************************
A PHIGS integer.
*/
typedef int32  Pint;
/*
Description:

*/

/**************************************************************
@    Pmatrix
****************************************************************
A PHIGS matrix. 
*/
typedef Pfloat Pmatrix[3][3];
/*
Description:

*/

/***************************************************************
@    PMARKER
****************************************************************
Marker types for PHIGS
*/
typedef enum
{
    DOT,
    PLUS,
    ASTERICK,
    CIRCLE,
    CROSS,
    SOLID_LINE,
    DASH_LINE,
    DOT_LINE
} Pmarker;

/*
 * Description:
 */

/***************************************************************
@    Pedge_flag
****************************************************************
The PHIGS edge flag.
*/
typedef enum
{
    PEDGE_ON,
    PEDGE_OFF
} Pedge_flag;
/*
Description:
Display fill area edges on or off.
*/

/***************************************************************
@    Pint_size
****************************************************************
A PHIGS size structure.
*/
typedef struct
{
    Pint size_x;
    Pint size_y;
} Pint_size;
/*
Description:

*/

/***************************************************************
@    Pint_style
****************************************************************
The PHIGS interior style structure.
*/
typedef enum
{
    PSTYLE_HOLLOW,
    PSTYLE_SOLID,
    PSTYLE_PAT,
    PSTYLE_HATCH,
    PSTYLE_EMPTY
} Pint_style;
/*
Description:
The current PHIGS update state.
*/

/***************************************************************
@    Pint_list
****************************************************************
A PHIGS integer list structure.
*/
typedef struct
{
    Pint num_ints;
    Pint *ints;
} Pint_list;
/*
Description:

*/

/***************************************************************
@    Plimit
****************************************************************
A PHIGS bounding limit structure.
*/
typedef struct
{
    Pfloat x_min;
    Pfloat x_max;
    Pfloat y_min;
    Pfloat y_max;
} Plimit;
/*
Description:

*/

/***************************************************************
@    Ppat_rep
****************************************************************
A PHIGS pattern representative structure.
*/
typedef struct
{
    Pint_size dims;
    unsigned char pHMEM colr_array;
    Pint pix_per_byte;
} Ppat_rep;
/*
Description:
A structure containing a stretchable image.
*/

/***************************************************************
@    Ppoint
****************************************************************
A PHIGS point structure.
*/
typedef struct
{
    Pfloat x;
    Pfloat y;
} Ppoint;
/*
Description:

*/

/***************************************************************
@    Ppoint_list
****************************************************************
A list of PHIGS points list structure.
*/
typedef struct
{
    Pint   num_points;
#ifndef _WINDOWS
    Ppoint *points;
#else
	Ppoint *points;
#endif
} Ppoint_list;
/*
Description:

*/

/***************************************************************
@    Ppoint_list_list
****************************************************************
A list of PHIGS point list structures.
*/
typedef struct
{
    Pint   num_point_lists;
    Ppoint_list *point_lists;
} Ppoint_list_list;
/*
Description:
Used for displaying fill area sets.
*/

/***************************************************************
@    Prect
****************************************************************
A PHIGS rectangle structure.
*/
typedef struct
{
    Ppoint p;
    Ppoint q;
} Prect;
/*
Description:

*/

/***************************************************************
@    Pregen_flag
****************************************************************
A PHIGS regeneration flag enumerater.
*/
typedef enum
{
    PFLAG_PERFORM,
    PFLAG_POSTPONE
} Pregen_flag;
/*
Description:
The PGLAG_POSTPONE value will defer updates to the PHIGS
workstation.
*/

/***************************************************************
@    Pupd_st
****************************************************************
The PHIGS update state structure.
*/
typedef enum
{
    PUPD_NOT_PEND,
    PUPD_PEND
} Pupd_st;
/*
Description:
The current PHIGS update state.
*/

/***************************************************************
@    Pvec
****************************************************************
A PHIGS vector position structure.
*/
typedef struct
{
    Pfloat delta_x;
    Pfloat delta_y;
} Pvec;
/*
Description:
A PHIGS relative position.
*/

#endif /* H_PHIGS_D */







