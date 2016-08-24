#ifndef H_MAPDOC_D
#define H_MAPDOC_D 1

#ifndef H_MUSE1
#include "muse1.h"
#endif
#ifndef H_MGM_D
#include "mgm_d.h"
#endif
#ifndef H_PHIGS_D
#include "phigs_d.h"
#endif
#ifndef H_UNIT_DEF
#include "unit_d.h"
#endif
#ifndef H_RASTER_D
#include "raster_d.h"
#endif
/*
 *
 @#Defines
 */
/***************************************************************
@    SZ_*
****************************************************************
SZ_* is size of structure "*" in the map data file.

*/
#define SZ_PRODUCTS 3072
#define SZ_GRAT_DATA 512
#define SZ_BASEMAP 512
#define SZ_RASTER 1024
#define SZ_UNITS 512
#define SZ_MGM 512
#define SZ_PALETTE 2048
#define SZ_LUT 2048
#define SZ_MAP_DOC 512
#define SZ_VECTOR 512
#define SZ_VEC_DATA 256
#define SZ_SQL_DATA 2048


/***************************************************************
@    Magic Numbers
****************************************************************
Record structure type and version
*/
#define MAGIC_MGM       10000000L
#define MAGIC_UNITS     20000000L
#define MAGIC_LUT       30000000L
#define MAGIC_PALETTE   40000000L
#define MAGIC_PRODUCTS  50000001L
#define MAGIC_VIEW      60000000L
#define MAGIC_BASEMAP   70000000L
#define MAGIC_RASTER    80000000L
#define MAGIC_MAP_DOC   90080892L
#define MAGIC_DEFAULTS 100000000L
#define MAGIC_VECTOR   110000000L
#define MAGIC_VEC_DATA  120000001L
#define MAGIC_GRAT_DATA 130000001L
#define MAGIC_SQL_DATA 14000000L

/*
 * Description: These magic numbers are placed into the first 4 bytes of
 * their respective structures to give the programmer an easy way to verify
 * that a structure is valid. They may also be used as structure version
 * identifiers for structures saved in files.
 */

/***************************************************************
@    MAX_PRODUCTS
****************************************************************
Limits the number of open data products
*/
#define MAX_PRODUCTS 10

/*
 * Description:
 */

/***************************************************************
@    STRUCTURE ID'S
****************************************************************
*/
#define BASEMAP_STRUCT 1
#define GRAT_STRUCT   2

/*
 * Description:
 */

/***************************************************************
@    TOTAL_OVERLAYS
****************************************************************
Limit on number of overlays
*/
#define TOTAL_OVERLAYS 8

/*
 * Description:
 */

/***************************************************************
@    __V_HDR__
****************************************************************
A vector data header structure
*/

#ifndef __V_HDR__
#define __V_HDR__
#define DATA_HDR_EXTENT (X) ((X)->extent)
#define DATA_HDR_LEFT    (x) EXTENT_LEFT  (DATA_HDR_EXTENT(x))
#define DATA_HDR_RIGHT    (x) EXTENT_RIGHT (DATA_HDR_EXTENT(x))
#define DATA_HDR_TOP    (x) EXTENT_TOP     (DATA_HDR_EXTENT(x))
#define DATA_HDR_BOTTOM (x) EXTENT_BOTTOM(DATA_HDR_EXTENT(x))

#define EXTENT_LEFT (x)    ((x)->left)
#define EXTENT_RIGHT (x)   ((x)->right)
#define EXTENT_BOTTOM (x)  ((x)->bottom)
#define EXTENT_TOP (x)       ((x)->top)

#define V_HDR_DATA_HDR (x)  ((x)->v_hdr)
#define V_HDR_TOP      (X)  DATA_HDR_TOP    (V_HDR_DATA_HDR(x))
#define V_HDR_BOTTOM   (x)  DATA_HDR_BOTTOM (V_HDR_DATA_HDR(x))
#define V_HDR_LEFT     (x)  DATA_HDR_LEFT   (V_HDR_DATA_HDR(x))
#define V_HDR_RIGHT    (x)  DATA_HDR_RIGHT  (V_HDR_DATA_HDR(x))
#endif

/*
 * Description:
 */

/***************************************************************
@    VER_NUM
****************************************************************
Current saved file revision number
*/
#define VER_NUM = 0x01;

/*
 * Description:
 */

/*
 * @Enumerated Data Types
 */

/***************************************************************
@    PRODUCTS bit flag masks
****************************************************************
*/
#define OVERLAYS_NEED_REDRAW_MASK       0x0002
#define OVERLAYS_USE_TIMER_MASK         0x0003
/*
 * Description:
 */

/***************************************************************
@    BASEMAP_TYPE
****************************************************************
Type of basemap products
*/
typedef enum
{
    BM_NONE,
    BM_SURFACE,
    BM_GEOIMAGE,
    BM_RGB_IMAGE,
    BM_INDIRECT_IMAGE,
    BM_DATA
}
                BASEMAP_TYPE;

/*
 * Description: DTED and DBDB5 are BM_SURFACE products. ADRG and CAC are
 * BM_GEOIMAGE products.
 */

/*
 * Description:
 * 
 */

/***************************************************************
@    OVERLAY_TYPE
****************************************************************
Types of overlays
*/
typedef enum
{
    GRAT,
    VECT,
    SQL,
    AUTOMATE,
    NONE
}
                OVERLAY_TYPE;

/*
 * Description:
 * 
 */

/***************************************************************
@    PRODUCT
****************************************************************
Basemap products
*/
typedef enum
{
    BASE_NONE,
    BASE_ADRG,
    BASE_ADRI,
    BASE_CAC,
    BASE_CADRG,
    BASE_CIB,
    BASE_CRG,
    BASE_DTED,
    BASE_DBDB5,
    BASE_DATA,
    BASE_OTHER,
    BASE_ASRP,
    BASE_USRP,
    BASE_SRG

}
                PRODUCT;

/*
 * Description:
 */


/***************************************************************
@    BASEMAP_PRODUCT
****************************************************************
Info about a basemap product
*/
typedef struct
{
    char            name[30];
    BASEMAP_TYPE    type;
    char            prompt[48];
    BOOLEAN         warpable;
    FILE_SPEC       file_spec;
}               BASEMAP_PRODUCT;

/*
 * Description:
 */

/***************************************************************
@    Grat_Data bit flag masks
****************************************************************
*/
#define GRAT_VALID_MASK              0x0001
#define GRAT_NEEDS_REDRAW_MASK       0x0002

/*
 * Description: GRAT_DATA contains a long variable "bit_flags" whose bits can
 * be set to show the current status of the structure.  Currently, bit 0 is
 * set to show the structure contains valid data, and bit 1 is set if the
 * grat overlay needs to be redrawn.
 */


/***************************************************************
@    MARKER
****************************************************************
Marker types for PHIGS
*/
#if 0
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
}
#endif
typedef Pmarker      MARKER;

/*
 * Description:
 */

/***************************************************************
@    GRAT_DATA
****************************************************************
Structure to define a graticule
*/
typedef struct
{
    int32            color;
    int32            magic;	/* SR 10/02 */
    int32            bit_flags;
    BOOLEAN         contains_valid_data;
    BOOLEAN         border_enable;
    BOOLEAN         border_major_enable;
    GFLOAT          border_major_lat;
    GEOGRAPHIC      border_major_lat_unit;
    GFLOAT          border_major_lon;
    GEOGRAPHIC      border_major_lon_unit;
    int32            border_major_color;

    BOOLEAN         border_minor_enable;
    GFLOAT          border_minor_lat;
    GEOGRAPHIC      border_minor_lat_unit;
    GFLOAT          border_minor_lon;
    GEOGRAPHIC      border_minor_lon_unit;
    int32            border_minor_color;

    BOOLEAN         border_labels_enable;
    GFLOAT          border_labels_lat;
    GEOGRAPHIC      border_labels_lat_unit;
    GFLOAT          border_labels_lon;
    GEOGRAPHIC      border_labels_lon_unit;
    int32            border_labels_color;

    BOOLEAN         grat_enable;
    BOOLEAN         grat_major_enable;
    MARKER          grat_major;
    BYTE            grat_major_ascii_code;
    GFLOAT          grat_major_lat;
    GEOGRAPHIC      grat_major_lat_unit;
    GFLOAT          grat_major_lon;
    GEOGRAPHIC      grat_major_lon_unit;
    int32            grat_major_color;

    BOOLEAN         grat_minor_enable;
    MARKER          grat_minor;
    BYTE            grat_minor_ascii_code;
    GFLOAT          grat_minor_lat;
    GEOGRAPHIC      grat_minor_lat_unit;
    GFLOAT          grat_minor_lon;
    GEOGRAPHIC      grat_minor_lon_unit;
    int32            grat_minor_color;
}               GRAT_DATA;

/*
 * Description:
 */

/***************************************************************
@    OVERLAYS
****************************************************************
The list of overlays
*/
typedef struct
{
    OVERLAY_TYPE    type;
    char            name[64];
    void           *product_pointer;
}               OVERLAYS;

/*
 * Description:
 */


/***************************************************************
@    BASEMAP
****************************************************************
Struct to contain the basemap information
*/
typedef struct BASEMAP
{
    int32            magic;
    PRODUCT         product;	/* which product */
    RASTER         *area;	/* product dependent */
    RASTER         *bit_map;	/* plotable image */
    RENDER_INFO     render_info;/* NULL unless its a DATA product */
    GFLOAT          zoom;	/* 0.0<zoom<1.0=downsampling,
				 * zoom>1.0=replication */
    BASEMAP_TYPE    type;       /* Product type */
    BOOLEAN         needs_refresh;	/* true if needs any work */
    BOOLEAN         needs_redraw;	/* true if it needs redrawing */
    BOOLEAN         needs_reload;	/* true if it needs to be reloaded */
    BOOLEAN         warp;	/* true ifit supports projection warp */
    FILE_SPEC       file_spec;	/* where in the file system */
}               BASEMAP;
/*
 * Description:
 */

/***************************************************************
@    PRODUCTS
****************************************************************
The list of products
*/
typedef struct
{
    int32            magic;
    int32            bit_flags;
    short           count;
    short           order;
    short           current;
    OVERLAYS        overlays[MAX_PRODUCTS];
    BASEMAP        *basemap;
}               PRODUCTS;

/*
 * Description:
 */


/***************************************************************
@    MAPDOC
****************************************************************
The map document structure
*/
typedef struct
{
    int32            magic;	/* for programmer's sanity check */
    MGM            *mgm;	/* the map geometry */
    UNITS          *units;	/* the selected display units */
    LUT            *lut;	/* the look-up-table */
    PALETTE        *palette;	/* the palette */
    PRODUCTS       *products;	/* the list of products displayed */
    VIEW           *view;	/* the viewing parameters, scrool, zoom */
    /* items below here are not written into the map files */
    BOOLEAN         needs_update;	/* TRUE if display is not current */
    BOOLEAN         needs_save;	/* TRUE if file is not current */
    BOOLEAN         auto_update;/* TRUE for automatic map updates */
    CURSOR          map_cursor; /* map window cursor type */
    FILE_SPEC       file_spec;	/* the map document's file */
}               MAP_DOC;

/*
 * Description: Contains all the information associated with a map document.
 * mapdoc_construct() and mapdoc_destruct() are used to create and destroy
 * and also to read and write.
 */

/***************************************************************
@    TEXT
****************************************************************
A text structure
*/
typedef struct
{
    uint32   length;
    char           *string;
}               TEXT;

/*
 * Description: These are returned by the product meta-data constructors.
 */
/***************************************************************
@    USER_LEVEL
****************************************************************
Levels of user experience.
*/
typedef enum
{
    NOVICE,
    EXPERIENCED,
    AUTOMATIC
}
		USER_LEVEL;

/*
 * Description:
 */
/***************************************************************
@    DEFAULTS
****************************************************************
The structure of program defaults
*/
typedef struct
{
    int32            magic;      /* magic number to id structure */
    DATA_MATRIX     bitmap_matrix;      /* default bit_map size and type */
    GFLOAT          center_lon; /* default map center, DD */
    GFLOAT          center_lat; /* default map center, DD */
    int red;
    int green;
    int blue;
    COLOR_PREF      color_pref; /* default color_pref */
    USER_LEVEL      user_level; /* NOVICE, EXPERIENCED, EXPERT */
    char            map_file_dir[SZ_FNAME];     /* default map directory SR */
    BOOLEAN         needs_save; /* true if needs to be saved */
    BOOLEAN         auto_update;/* if FALSE, the user has to push the Update
				 * Map button */
    UNITS           units;      /* user units */
    FILE_SPEC       map_file_spec;      /* default map file */
    FILE_SPEC       def_file_spec;      /* where the defaults file is stored */
    CURSOR      map_cursor; /* map windows cursor shape */
}               DEFAULTS;

/*
 * Description:
 */



#endif				/* ifndef H_MAPDOC_D */
