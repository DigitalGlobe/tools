#ifndef H_VEC_D
#define H_VEC_D

#ifndef H_MUSE1
#include "muse1.h"
#endif
#ifndef H_VIEW_DEF
#include "view_d.h"
#endif
#ifndef H_PHIGS_D
#include "phigs_d.h"
#endif

#ifndef __VPF_H__
/* VPF feature types */
typedef enum { LINE=1, AREA, ANNO, POINTS, COMPLEX=6 } vpf_feature_type;
#endif


#define MAGIC_VEC2_DATA 120000002L


#define MAXPOINTS 16000
#define MAXLINES  16000
#define MAXAREAS  16000
#define MAXTEXT   16000
#define MAXRINGS  5000
#define MAXSEGS   5000
#define MAXCOORDS 5000
#define MAXSTRING   80

#define SZ_VEC           60
#define SZ_VEC_EXTENTS   32
#define SZ_VEC_LINES      4
#define SZ_VEC_TEXT       4
#define SZ_VEC_POINTS     4
#define SZ_VEC_AREAS      4
#define SZ_VEC_SYMBOLOGY 16
#define SZ_VEC_INFO       8
#define SZ_SYMB_LINES    20
#define SZ_SYMB_TEXT     16
#define SZ_SYMB_POINTS   16
#define SZ_SYMB_AREAS    10
#define SZ_COORDINATE    16

#define SZ_LONG           4
#define SZ_DOUBLE         8


/***************************************************************
@    FEATURE_EDIT_FLAGS
****************************************************************
A set of flags used in feature editing.
*/
typedef struct
{
    BOOLEAN selected; /*TRUE if selected*/
    BOOLEAN deleted;  /*TRUE if flagged for deletion*/ 
    BOOLEAN modified; /*TRUE if recently modified*/
} FEATURE_EDIT_FLAGS;
/*
Description:

*/

/***************************************************************
@    POINT_SYMB
****************************************************************
Point Symbology structure for display of markers.
*/
typedef struct
{
    Pint    marker_color;
    Pint    marker_type;
    Pfloat  marker_scale;
} POINT_SYMB;
/*
Description:

*/

/***************************************************************
@    LINE_SYMB
****************************************************************
Line Symbology structure for display of lines.
*/
typedef struct
{
    Pint        line_color;
    Pint        line_style;
    Pfloat      line_width;
	POINT_SYMB *point_symb;
} LINE_SYMB;
/*
Description:

*/

/***************************************************************
@    TEXT_SYMB
****************************************************************
Text Symbology structure for display of text.
*/
typedef struct
{
    Pint        text_color;
    Pfloat      text_scale;
    POINT_SYMB *point_symb;
} TEXT_SYMB;
/*
Description:

*/

/***************************************************************
@    AREA_SYMB
****************************************************************
Area Symbology structure for display of areas.
*/
typedef struct
{
    Pint        area_color;
    Pint_style  area_style;
	LINE_SYMB  *line_symb;
} AREA_SYMB;
/*
Description:

*/

/***************************************************************
@   COORDINATE
****************************************************************
Geospatial coordinate pair.
*/
typedef struct
   {
   float x;
   float y;
   } COORDINATE;
/*
Description:

*/

/***************************************************************
@   SEGMENT
****************************************************************
Line/edge segment.
*/
typedef struct
   {
   int32       id;
   int32       nr_coords;
   COORDINATE *coords;
   } SEGMENT;
/*
Description:

*/

/***************************************************************
@    POINT_FEATURE
****************************************************************
Point Feature structure.
*/
typedef struct
   {
   int32       id;
   COORDINATE *coord;
   POINT_SYMB *point_symb;
   FEATURE_EDIT_FLAGS flags;
   } POINT_FEATURE;
/*
Description:

*/

/***************************************************************
@    LINE_FEATURE
****************************************************************
Line Feature structure.
*/
typedef struct
   {
   int32    id;
   int32    nr_segs;
   SEGMENT **segs;
   LINE_SYMB *line_symb;
   FEATURE_EDIT_FLAGS flags;
   } LINE_FEATURE;
/*
Description:

*/

/***************************************************************
@    RING
****************************************************************
Ring structure.
*/
typedef struct
   {
   int32    id;
   int32    nr_segs;
   SEGMENT **segs;
   } RING;
/*
Description:

*/

/***************************************************************
@    AREA_FEATURE
****************************************************************
Area Feature structure.
*/
typedef struct
   {
   int32  id;
   int32  nr_rings;
   RING  **rings;
   AREA_SYMB *area_symb;
   FEATURE_EDIT_FLAGS flags;
   } AREA_FEATURE;
/*
Description:

*/

/***************************************************************
@    TEXT_FEATURE
****************************************************************
Text Feature structure.
*/
typedef struct
   {
   int32  id;
   char  *string;
   int32  nr_coords;
   COORDINATE  *coords;
   TEXT_SYMB   *text_symb;
   FEATURE_EDIT_FLAGS flags;
   } TEXT_FEATURE;
/*
Description:

*/

/****************************************************************************
@    VEC_EXTENT
*****************************************************************************
Spatial extents of the data-set.
*/
typedef struct
{
    double xmin;
    double ymin;
    double xmax;
    double ymax;
} VEC_EXTENT;
/*
Description:

*/

/****************************************************************************
@    VEC_INFO
*****************************************************************************
Spatial extents of the data-set.
*/
typedef struct
{
    char *type;
    char *info;
} VEC_INFO;
/*
Description:

*/

/***************************************************************
@    SYMBOLOGY
****************************************************************
Symbology structure for displaying of VECs.
*/
typedef struct
{
	LINE_SYMB    *line_symb;
    TEXT_SYMB    *text_symb;
    POINT_SYMB   *point_symb;
    AREA_SYMB    *area_symb;
} SYMBOLOGY;
/*
Description:

*/

/***********************************************************************
@  VEC
************************************************************************
A MUSE structure to hold vector overlay data.
*/
typedef struct
   {
   int32      magic;
   vpf_feature_type feature_type;
   int32      nr_features;
   int32      nr_lines;
   int32      nr_text;
   int32      nr_points;
   int32      nr_areas;
   VEC_EXTENT       *extents;
   VEC_INFO         *info;
   LINE_FEATURE     **lines;
   TEXT_FEATURE     **text;
   POINT_FEATURE    **points;
   AREA_FEATURE     **areas;
   SYMBOLOGY        *symbology;
   } VEC;
/*
Description:

*/

#endif /* H_VEC_DEF */
