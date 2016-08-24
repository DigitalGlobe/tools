#ifndef H_COLOR_F

#define H_COLOR_F

#ifndef H_COLOR_D
#include "color_d.h"
#endif
#ifndef H_MAPDOC_D
#include "mapdoc_d.h"
#endif

/***************************************************************
@    add_color_bias()
****************************************************************
Adds PAL_OFFSET to the raster bitmap.
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API add_color_bias(GHANDLE bitmap);
#else
ERRSTATUS MUSE_API add_color_bias();
#endif

/*
 * Description:
 */

/***************************************************************
@    color_dist()
****************************************************************
Computes distance between two RGB colors.
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API color_dist(RGB x, RGB y, int32 *distance);
#else
ERRSTATUS MUSE_API color_dist();
#endif

/*
 * Description: A simple cartesian 3D distance is computed.
 */

/***************************************************************
@    decode_palette()
****************************************************************
Convert palette structure to local binary
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
decode_palette(
	       unsigned char *buffer,
	       PALETTE * palette);
#else
ERRSTATUS MUSE_API decode_palette();
#endif

/*
 * Description: The palette structure information in the character buffer
 * (binary portable Intel format) is placed into the palette structure in
 * local binary.  Used by the constructor functions while loading in a map
 * document.
 */

/***************************************************************
@    decode_lut()
****************************************************************
Convert lut structure to local binary
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
decode_lut(
	   unsigned char *record,
	   LUT * lut);
#else
ERRSTATUS MUSE_API decode_lut();
#endif

/*
 * Description: The lut structure information in the character buffer (binary
 * portable Intel format) is placed into the lut structure in local binary.
 * Used by the constructor functions while loading in a map document.
 */

/***************************************************************
@    encode_lut()
****************************************************************
Convert lut structure external form
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
encode_lut(
	   unsigned char *buffer,
	   LUT * lut);
#else
ERRSTATUS MUSE_API encode_lut();
#endif

/*
 * Description: The lut structure is converted to the external binary
 * portable (Intel) format. Used by the destructor functions while preparing
 * to store a map document.
 */

/***************************************************************
@    encode_palette()
****************************************************************
Convert palette structure external form
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
encode_palette(
	       unsigned char *buffer,
	       PALETTE * palette);
#else
ERRSTATUS MUSE_API encode_palette();
#endif

/*
 * Description: The palette structure is converted to the external binary
 * portable (Intel) format. Used by the destructor functions while preparing
 * to store a map document.
 */

/***************************************************************
@    hsv2rgb()
****************************************************************
Invoke a user interface dialog or window
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
hsv2rgb(GFLOAT hue, GFLOAT sat, GFLOAT val,
	GFLOAT * r, GFLOAT * g, GFLOAT * b);
#else
ERRSTATUS MUSE_API hsv2rgb();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    lut_construct()
****************************************************************
Create a look-up-table.
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
lut_construct(
	      FILE * file,
	      PALETTE * pal,
	      LUT ** lut);
#else
ERRSTATUS MUSE_API lut_construct();
#endif

/*
 * Description: If the file argument is not NULL, the look_up_table will be
 * read in from the file.  Otherwise a default lut will be created. Lut's
 * define the relationship between data values and the colors used to display
 * them on the map.
 */

/***************************************************************
@    lut_destruct()
****************************************************************
Frees up the lut memory.
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
lut_destruct(
	     FILE * file,
	     BOOLEAN destruct,
	     LUT ** lut);
#else
ERRSTATUS MUSE_API lut_destruct();
#endif

/*
 * Description: If the file argument is not NULL the lut is written into the
 * file using fwrite.  If the destruct argument is true the lut is removed
 * from memory.
 */

/***************************************************************
@    lut_lookup()
****************************************************************
Look up a data value in a look-up-table
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
lut_lookup(
	   LUT * lut,
	   PALETTE * pal,
	   GFLOAT data,
	   GFLOAT delta,
	   RENDER_INFO * render_info,
	   USHORT * index);	/* int32 *color */
#else
ERRSTATUS MUSE_API lut_lookup();
#endif

/*
 * Description: Looks up the data value and delta in the lut and returns the
 * color index.
 * 
 */

#if 0
/***************************************************************
@    lut2pal()
****************************************************************
Generates a palette from a look-up-table
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
lut2pal(
	LUT * lut,
	COLOR_PREF color_pref,
	PALETTE * pal);
#else
ERRSTATUS MUSE_API lut2pal();
#endif

/*
 * Description: The color information contained in the lut in the form of
 * hue, saturation, and value is used to generate the palette.
 */
#endif

/***************************************************************
@    palette_construct()
****************************************************************
Construct a color palette.
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
palette_construct(
		  FILE * file,
		  PALETTE ** palette);
#else
ERRSTATUS MUSE_API palette_construct();
#endif

/*
 * Description: If the file argument is not NULL the palette is loaded from
 * the file.  Otherwise, a default palette is created.
 */

/***************************************************************
@    palette_destruct()
****************************************************************
Destroy the color palette
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
palette_destruct(
		 FILE * file,
		 BOOLEAN destruct,
		 PALETTE ** palette);
#else
ERRSTATUS MUSE_API palette_destruct();
#endif

/*
 * Description: If the file argument is not NULL, the palette is read from
 * the file, otherwise a default palette is created.
 */

/***************************************************************
@    palette_rgbcube_construct()
****************************************************************
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API palette_rgbcube_construct(PALETTE ** pointer);
#else
ERRSTATUS MUSE_API palette_rgbcube_construct();
#endif

/*
 * Description:
 */

/***************************************************************
@    pal_nearest_color()
****************************************************************
Gets a palette color from an RGB color
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
pal_nearest_color(
		  PALETTE * palette,
		  PALETTE_USAGE palette_usage,
		  RGB rgb,
		  USHORT * color_index);
#else
ERRSTATUS MUSE_API pal_nearest_color();
#endif

/*
 * Description: Maps the input RGB color into the palette.  The method used
 * depends on the palette type.
 * 
 */

/***************************************************************
@    pal_rgb_bins_construct()
****************************************************************
Construct a palette of RGB bins
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
pal_gen(
	PALETTE * palette);
#else
ERRSTATUS MUSE_API pal_gen();
#endif

/*
 * Description: Constructs a palette as a 3D matrix of color with red, green,
 * and blue on the 3 axes.  The palette structure must have been initialized
 * with the desired PAL_TYPE and the bin counts for red, green, and blue. The
 * total number of bins must not exceed 256.
 */

/***************************************************************
@    remove_color_bias()
****************************************************************
Removes PAL_OFFSET from the raster bitmap.
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API remove_color_bias(GHANDLE bitmap);
#else
ERRSTATUS MUSE_API remove_color_bias();
#endif

/*
 * Description:
 */

/***************************************************************
@    rgb2uvw()
****************************************************************
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API rgb2uvw(int32 r, int32 g, int32 b, int32 *u, int32 *v, int32 *w);
#else
ERRSTATUS MUSE_API rgb2uvw();
#endif

/*
 * Description:
 */

/***************************************************************
@    uvw2rgb()
****************************************************************
Sets windows and cursors during a map update.
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API uvw2rgb(int32 u, int32 v, int32 w, int32 *r, int32 *g, int32 *b);
#else
ERRSTATUS MUSE_API uvw2rgb();
#endif

/*
 * Description:
 * 
 */

#endif /* H_COLOR_F */
