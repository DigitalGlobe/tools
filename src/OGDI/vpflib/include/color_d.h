#ifndef H_COLOR_D
#define H_COLOR_D

#ifndef INCL_XVTH
#include "xvt.h"
#endif
#ifndef H_MUSE1
#include "muse1.h"
#endif
/***************************************************************
@    RGB_BITS
****************************************************************
The default number of bits used to store RGB color values
*/
#define RGB_BITS 16

/*
 * Description: RGB values usually are stored as 16 bit unsigned shorts: (0 -
 * 65545).
 */

/***************************************************************
@    RGB_MAX
****************************************************************
The maximum value used to store RGB.
*/
#define RGB_MAX 65535L

/*
 * Description: RGB values are stored as 16 bit unsigned shorts: (0 - 65545).
 */

/***************************************************************
@    MAX_COLORS
****************************************************************
Miximum number of colors allowed
*/
#define MAX_COLORS 256

/*
 * Description: MUSE uses 8 bit indirect color which allows 256 colors. Some
 * of these colors are reserved by the windowing system and some are reserved
 * by MUSE-PHIGS.  Only 216 colors are available for use in raster displays.
 */

/***************************************************************
@    MAX_LUT
****************************************************************
Number of look-up-table ranges
*/
#define MAX_LUT 6

/*
 * Description: Since MAX_LUT is used to dimension the LUT data structure,
 * changing it may cause problems with existing LUT files and maps.
 */

/**************************************************************
@    COLOR_PREF
****************************************************************
Preference for reconciling lut and palette
*/
typedef enum
{
    ALL_PURPOSE,		/* Both image and terrain use default */
    FAVOR_IMAGE,		/* Lut is mapped to image palette */
    FAVOR_TERRAIN		/* Lut generates the palette */
}
                COLOR_PREF;

/*
 * Description: If all map windows use ALL_PURPOSE, the map windows will all
 * use the same default palette and will not compete for the system palette.
 * The FAVOR_IMAGE setting will result in the terrain look-up-table colors
 * being mapped to the closest existing palette color.  The FAVOR_TERRAIN
 * setting will generate a new palette to match the lut.
 */

/***************************************************************
@    PAL_TYPE
****************************************************************
Types of palettes
*/

typedef enum
{
    PAL_UNKNOWN,		/* non-algorithmic palette- use min dist */
    PAL_BIT_BINS,		/* #bits for R,G, and B */
    PAL_NUM_BINS,		/* #bins for R, G, and B */
    PAL_GREY			/* equal R, G, B */
}
                PAL_TYPE;

/*
 * Description: PAL_TYPE indicates the algorithm by which it was constructed
 * and thus how it can be used.  PAL_UNKNOWN palettes have an unknown origin.
 * Colors are classified into unknown palettes using minimum distance.
 * PAL_BIT_BINS and PAL_NUM_BINS palettes are constructed as a 3D matrix of
 * color with red, green, and blue on the three axes. Colors may be
 * classified into the PAL_BIT_BINS palettes using fast bit operations.
 * Colors may be classified into the PAL_NUM_BINS palettes using slightly
 * slower integer math.
 */

/***************************************************************
@    PALETTE_USAGE
****************************************************************
Controls use of gray and rgb palette sections
*/
typedef enum
{
    PAL_BOTH,
    PAL_RGB_ONLY,
    PAL_GRAY_ONLY,
    PAL_NATIVE
}
                PALETTE_USAGE;
/***************************************************************
@    RGB24
****************************************************************
Structure to contain 24 bit RGB pixel
*/
typedef struct
{
    BYTE            r;
    BYTE            g;
    BYTE            b;
}               RGB24;

/*
 * Description: The red, green, and blue values are stored as BYTE (unsigned
 * char) with a range of 0-255.
 */

/***************************************************************
@    RGB
****************************************************************
Structure to contain 48 bit RGB data
*/
typedef struct
{
    unsigned short  r;
    unsigned short  g;
    unsigned short  b;
}               RGB;

/*
 * Description: The red, green, and blue vbalues range from 0-65535.
 */

/***************************************************************
@    PALETTE
****************************************************************
Structure for a color palette
*/
typedef struct
{
    int32            magic;	/* structure id */
    PAL_TYPE        type;	/* how it was generated and used */
    RGB             bins;	/* # bins if type is PAL_RGB_BINS */
    USHORT          first_color;/* index of first valid palette color */
    USHORT          num_colors;	/* # of valid colors */
    RGB             rgb[MAX_COLORS];	/* RGBs range 0 - 65535 (16bit) */
    FILE_SPEC       palette_file;	/* where it is stored */
}               PALETTE;

/*
 * Description: These objects may be created and read using
 * palette_construct(). They may be destroyed and written using
 * palette-destruct().
 */

/***************************************************************
@    SHADE
****************************************************************
A structure containing a color saturation and value
*/
typedef struct
{
    GFLOAT          sat;	/* 0.0 - 1.0 range */
    GFLOAT          val;	/* 0.0 - 1.0 range */
}               SHADE;

/*
 * Description:
 */

/***************************************************************
@    LUT_DESC
****************************************************************
A look-up-table descriptor
*/
typedef struct
{
    int32            num_hues;	/* number of color bands */
    int32            num_shades;	/* number of shades of each color */
    GFLOAT          data_range[2];
    GFLOAT          hue_range[2];
    SHADE           shade_range[3];
}               LUT_DESC;

/*
 * Description:
 */

/***************************************************************
@    LUT
****************************************************************
A look-up-table
*/
typedef struct
{
    int32            magic;	/* LUT version id */
    LUT_DESC        lut_desc[MAX_LUT];	/* The lut descriptors */
    /* USHORT          map[MAX_COLORS]; *//* map to nearest pal color */
    BOOLEAN         stretch;	/* Fit Colors to LUT range */
    FILE_SPEC       lut_file;	/* File name */
}               LUT;

/*
 * Description: Each LUT_DESCriptor defines the data, hue, saturation, and
 * value ranges for a data range.  Lut2pal() generates a palette from a lut.
 * Lut_lookup() looks a data value up in the lut to determine its color.
 */

typedef struct
{
    int             lr, tb, nh, ns;
}               global_sq_size;
#endif				/* H_COLOR_D */
