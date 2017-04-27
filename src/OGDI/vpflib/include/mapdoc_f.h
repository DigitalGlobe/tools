#ifndef H_MAPDOC_F

#define H_MAPDOC_F

#ifndef H_VECTOR
#include "vector.h"
#endif

/***************************************************************
@    basemap_construct()
 ****************************************************************
Constructs the basemap
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
basemap_construct(
		  FILE * file,
		  BASEMAP ** basemap);
#else
ERRSTATUS MUSE_API basemap_construct();
#endif

/*
 * Description: Constructs a basemap object.  IF file is not NULL the basemap
 * is read in from the file. /***************************************************************
 * @    basemap_destruct() ***************************************************************
 * 
 * Destroys the basemap structure
 */

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
basemap_destruct(
		 FILE * file,
		 BOOLEAN destruct,
		 BASEMAP ** basemap);
#else
ERRSTATUS MUSE_API basemap_destruct();
#endif

/*
 * Description: If file is not NULL the basemap is written to the file. If
 * destruct is TRUE the basemap memory is released.
 */

/***************************************************************
@    basemap_new_bitmap()
****************************************************************
Get a new bit_map raster
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
basemap_new_bitmap(int32 width,
		   int32 height,
		   int32 bits_per_pixel,
		   RASTER ** bit_map,
           DEFAULTS *defaults);
#else
ERRSTATUS MUSE_API basemap_new_bitmap();
#endif

/*
 * Description: If the bit_map already exists, it is deleted. A new bit_map
 * object of type RASTER is created according to the specified dimensions.
 */

/***************************************************************
@    basemap_match_geometry()
****************************************************************
Sets up the map geometry to match the basemap geometry
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API basemap_match_geometry(BASEMAP * basemap, MGM * mgm);
#else
ERRSTATUS MUSE_API basemap_match_geometry();
#endif

/*
 * Description: The map geometry object mgm is adjusted to the basemap's
 * projection and geographic extent, so that vector overlays, and the cursor
 * readout will be correct.
 * 
 * 
/*************************************************************** @
 * basemap_setup() ***************************************************************
 * 
 * Sets up the basemap
 */

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
basemap_setup(
	      BASEMAP * basemap,
	      MGM * mgm);
#else
ERRSTATUS MUSE_API basemap_setup();
#endif

/*
 * Description: Sets up the basemap by calling any needed product
 * constructors and setup functions.
 * 
 */

/***************************************************************
@    decode_basemap()
****************************************************************
Convert basemap structure to local binary
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
decode_basemap(
	       unsigned char *buffer,
	       BASEMAP * basemap);
#else
ERRSTATUS MUSE_API decode_basemap();
#endif

/*
 * Description: The basemap structure information in the character buffer
 * (binary portable Intel format) is placed into the basemap structure in
 * local binary.  Used by the constructor functions while loading in a map
 * document.
 */

/***************************************************************
@    decode_grat()
****************************************************************
Convert grat structure to local binary
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API decode_grat(unsigned char *buffer, GRAT_DATA * grat, int32 version);
#else
ERRSTATUS MUSE_API decode_grat();
#endif

/*
 * Description: The grat structure information in the character buffer
 * (binary portable Intel format) is placed into the grat structure in local
 * binary.  Used by the constructor functions while loading in a map
 * document.
 */

/***************************************************************
@    decode_mapdoc()
****************************************************************
Convert map_doc structure to local binary
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
decode_mapdoc(
	      unsigned char *buffer,
	      MAP_DOC * map_doc);
#else
ERRSTATUS MUSE_API decode_mapdoc();
#endif

/*
 * Description: The map_doc structure information in the character buffer
 * (binary portable Intel format) is placed into the map_doc structure in
 * local binary.  Used by the constructor functions while loading in a map
 * document.
 */
/***************************************************************
@    decode_mgm()
****************************************************************
Convert mgm structure to local binary
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
decode_mgm(
	   unsigned char *buffer,
	   MGM * mgm);
#else
ERRSTATUS MUSE_API decode_mgm();
#endif

/*
 * Description: The mgm structure information in the character buffer (binary
 * portable Intel format) is placed into the mgm structure in local binary.
 * Used by the constructor functions while loading in a map document.
 */

/***************************************************************
@    decode_products()
****************************************************************
Convert products structure to local binary
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
decode_products(
		unsigned char *buffer,
		PRODUCTS * products);
#else
ERRSTATUS MUSE_API decode_products();
#endif

/*
 * Description: The products structure information in the character buffer
 * (binary portable Intel format) is placed into the products structure in
 * local binary.  Used by the constructor functions while loading in a map
 * document.
 */

/***************************************************************
@    encode_basemap()
****************************************************************
Convert basemap structure to external form
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
encode_basemap(
	       unsigned char *buffer,
	       BASEMAP * basemap);
#else
ERRSTATUS MUSE_API encode_basemap();
#endif

/*
 * Description: The basemap structure is converted to the external binary
 * portable (Intel) format. Used by the destructor functions while preparing
 * to store a map document.
 */

/***************************************************************
@    encode_grat()
****************************************************************
Convert grat structure to external form
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API encode_grat(unsigned char *buffer, GRAT_DATA * grat);
#else
ERRSTATUS MUSE_API encode_grat();
#endif

/*
 * Description: The grat structure is converted to the external binary
 * portable (Intel) format. Used by the destructor functions while preparing
 * to store a map document.
 */

/***************************************************************
@    encode_mapdoc()
****************************************************************
Convert map_doc structure external form
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
encode_mapdoc(
	      unsigned char *buffer,
	      MAP_DOC * map_doc);
#else
ERRSTATUS MUSE_API encode_mapdoc();
#endif

/*
 * Description: The map_doc structure is converted to the external binary
 * portable (Intel) format. Used by the destructor functions while preparing
 * to store a map document.
 */

/***************************************************************
@    encode_mgm()
****************************************************************
Convert mgm structure external form
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
encode_mgm(
	   unsigned char *buffer,
	   MGM * mgm);
#else
ERRSTATUS MUSE_API encode_mgm();
#endif

/*
 * Description: The mgm structure is converted to the external binary
 * portable (Intel) format. Used by the destructor functions while preparing
 * to store a map document.
 */

/***************************************************************
@    encode_products()
****************************************************************
Convert products structure external form
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
encode_products(
		unsigned char *buffer,
		PRODUCTS * products);
#else
ERRSTATUS MUSE_API encode_products();
#endif

/*
 * Description: The products structure is converted to the external binary
 * portable (Intel) format. Used by the destructor functions while preparing
 * to store a map document.
 */

/***************************************************************
@    grat()
****************************************************************
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API grat(WINDOW map_win, short count, MGM * mgm);
#else
ERRSTATUS MUSE_API grat();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    grat_construct()
****************************************************************
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API grat_construct(FILE * file, GRAT_DATA ** pointer, int32 version);
#else
ERRSTATUS MUSE_API grat_construct();
#endif

/*
 * Description:
 */

/***************************************************************
@    grat_destruct()
****************************************************************
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API grat_destruct(FILE * file, BOOLEAN destruct, GRAT_DATA ** grat);
#else
ERRSTATUS MUSE_API grat_destruct();
#endif

/*
 * Description:
 * 
 */


/***************************************************************
@    map_doc_construct()
****************************************************************
Create a map document
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API map_doc_construct(
    FILE * file,
    MAP_DOC ** pointer,
    BYTE_ORDER * byte_order,
    DEFAULTS *defaults);
#else
ERRSTATUS MUSE_API map_doc_construct();
#endif

/*
 * Description: Calls the constructors for the sub-structures to allocate a
 * new  map document.  If the file argument is not NULL, the map document is
 * read in from the file.
 */
/***************************************************************
@    map_doc_destruct()
****************************************************************
Destroy the map document
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
map_doc_destruct(
		 FILE * file,
		 BOOLEAN destruct,
		 MAP_DOC ** map_doc,
		 BYTE_ORDER * bo);
#else
ERRSTATUS MUSE_API map_doc_destruct();
#endif

/*
 * Description: If the file argument is not NULL, the map document is written
 * to the file. If the destruct argument is TRUE, the map document is removed
 * from memory.
 * 
 */

/***************************************************************
@    products_construct()
****************************************************************
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API products_construct(FILE * file, PRODUCTS ** pointer, BYTE_ORDER * bo);
#else
ERRSTATUS MUSE_API products_construct();
#endif

/*
 * Description:
 */

/***************************************************************
@    products_destruct()
****************************************************************
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API products_destruct
                (
		                 FILE * file,
		                 BOOLEAN destruct,
		                 PRODUCTS ** products,
		                 BYTE_ORDER * bo
);
#else
ERRSTATUS MUSE_API products_destruct();
#endif

/*
 * Description:
 */

#endif /* H_MAPDOC_F */
