#ifndef H_RASTER_F

#define H_RASTER_F

/***************************************************************
@    decode_raster_hdr()
****************************************************************
Convert raster_hdr structure to local binary
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
decode_raster_hdr(
		  unsigned char *buffer,
		  RASTER * raster);
#else
ERRSTATUS MUSE_API decode_raster_hdr();
#endif

/*
 * Description: The raster structure information in the character buffer
 * (binary portable Intel format) is placed into the raster structure in
 * local binary.  Used by the constructor functions while loading in a map
 * document.
 */

/***************************************************************
@    encode_raster_hdr()
****************************************************************
Convert raster_hdr structure external form
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
encode_raster_hdr(
		  unsigned char *buffer,
		  RASTER * raster);
#else
ERRSTATUS MUSE_API encode_raster_hdr();
#endif

/*
 * Description: The raster structure is converted to the external binary
 * portable (Intel) format. Used by the destructor functions while preparing
 * to store a map document.
 */

/***************************************************************
@    is_raster_ok()
****************************************************************
Check for a good raster magic number.
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API is_raster_ok(FILE * file);
#else
ERRSTATUS MUSE_API is_raster_ok();
#endif

/*
 * Description: Check to see if there a
 * good raster by checking the raster number.
 */

/***************************************************************
@    raster_construct()
****************************************************************
Construct a RASTER object
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
raster_construct(
		 FILE * file,
		 RASTER ** pointer);
#else
ERRSTATUS MUSE_API raster_construct();
#endif

/*
 * Description: If the file argument is not NULL, the raster is read in from
 * the file, otherwise a default raster is created.
 */

/***************************************************************
@    raster_construct_data()
****************************************************************
Allocates memory for the raster data
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
raster_construct_data(
		      RASTER * raster);
#else
ERRSTATUS MUSE_API raster_construct_data();
#endif

/*
 * Description: Once the raster has been created (using raster-construct())
 * and the width, height, and bits_per_pixel initialized, this function may
 * be called to allocate global heap space for the data.
 */

/***************************************************************
@    raster_copy()
****************************************************************
Copy an object of type RASTER
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
raster_copy(
	    RASTER * in,
	    RASTER * out);
#else
ERRSTATUS MUSE_API raster_copy();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    raster_data_to_indirect()
****************************************************************
Convert a raster containing non-image data into a color image
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
raster_data_to_indirect(
			RASTER * in,
			RASTER * out,
			RENDER_INFO * render_info,
			LUT * default_lut,
			PALETTE * default_palette);
#else
ERRSTATUS MUSE_API raster_data_to_indirect();
#endif

/*
 * Description: Pseudocoloring using a look-up-table is used to produce the
 * image raster.
 * 
 */

/***************************************************************
@    raster_destruct()
****************************************************************
Destroy the object of type RASTER
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
raster_destruct(
		FILE * file,
		BOOLEAN destruct,
		RASTER ** pointer);
#else
ERRSTATUS MUSE_API raster_destruct();
#endif

/*
 * Description: If the file argument is not NULL, the raster is written to
 * the file.  If destruct is TRUE the object of type RASTER is removed from
 * memory.
 * 
 */

/***************************************************************
@    raster_draw()
****************************************************************
Draw the (drawable) raster to the map window
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
raster_draw(
	    RASTER * raster, WINDOW map_window);
#else
ERRSTATUS MUSE_API raster_draw();
#endif

/*
 * Description: The raster must be in drawable form.  To be drawable, a
 * raster must contain data of type COLOR_INDIRECT. Rraster_xform() is used
 * to convert the various data rasters to a drawable form.
 * 
 */

/***************************************************************
@    raster_indirect_to_indirect()
****************************************************************
Convert the indirect raster to a new palette
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
raster_indirect_to_indirect(
			    RASTER * in,
			    RASTER * out,
			    PALETTE * new_palette,
			    RENDER_INFO * render_info);
#else
ERRSTATUS MUSE_API raster_indirect_to_indirect();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    raster_indirect_to_rgb()
****************************************************************
Convert the indirect raster containing an RGB raster
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
raster_indirect_to_rgb(
		       RASTER * in,
		       RASTER * out);
#else
ERRSTATUS MUSE_API raster_indirect_to_rgb();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    raster_rgb_to_indirect()
****************************************************************
Convert the raster containing an RGB image to a color map image
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
raster_rgb_to_indirect(
		       RASTER * in,
		       RASTER * out,
		       PALETTE * default_palette,
		       RENDER_INFO * render_info);
#else
ERRSTATUS MUSE_API raster_rgb_to_indirect();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    raster_setup()
****************************************************************
Setup the object of type RASTER
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
raster_setup(
	     BASEMAP * basemap,
	     MGM * mgm);
#else
ERRSTATUS MUSE_API raster_setup();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    raster_xform()
****************************************************************
Convert a raster to a displayable image form
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API raster_xform
                (
		                 RASTER * in,
		                 RASTER * out,
		                 RENDER_INFO * render_info,
		                 LUT * default_lut,
		                 PALETTE * default_palette
);
#else
ERRSTATUS MUSE_API raster_xform();
#endif

/*
 * Description: Rasters containing non-image data are rendered.  RGB images
 * rasters are converted to color map rasters.
 */

#endif /* H_RASTER_F */
