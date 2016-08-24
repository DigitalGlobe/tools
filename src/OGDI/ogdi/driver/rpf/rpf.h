/******************************************************************************
 *
 * Component: OGDI RPF Driver
 * Purpose: Declarations of RPF driver structures and functions.
 * 
 ******************************************************************************
 * Copyright (C) 1995 Logiciels et Applications Scientifiques (L.A.S.) Inc
 * Permission to use, copy, modify and distribute this software and
 * its documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies, that
 * both the copyright notice and this permission notice appear in
 * supporting documentation, and that the name of L.A.S. Inc not be used 
 * in advertising or publicity pertaining to distribution of the software 
 * without specific, written prior permission. L.A.S. Inc. makes no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 ******************************************************************************
 *
 * $Log: rpf.h,v $
 * Revision 1.7  2007/02/12 21:01:48  cbalint
 *      Fix win32 target. It build and works now. (tested with VC6)
 *
 * Revision 1.6  2007/02/12 16:09:06  cbalint
 *   *  Add hook macros for all GNU systems, hook fread,fwrite,read,fgets.
 *   *  Handle errors in those macro, if there are any.
 *   *  Fix some includes for GNU systems.
 *   *  Reduce remaining warnings, now we got zero warnings with GCC.
 *
 *  Modified Files:
 *  	config/unix.mak contrib/ogdi_import/dbfopen.c
 *  	contrib/ogdi_import/shapefil.h contrib/ogdi_import/shpopen.c
 *  	ogdi/c-api/ecs_xdr.c ogdi/c-api/ecsinfo.c ogdi/c-api/server.c
 *  	ogdi/datum_driver/canada/nadconv.c ogdi/driver/adrg/adrg.c
 *  	ogdi/driver/adrg/adrg.h ogdi/driver/adrg/object.c
 *  	ogdi/driver/adrg/utils.c ogdi/driver/rpf/rpf.h
 *  	ogdi/driver/rpf/utils.c ogdi/gltpd/asyncsvr.c
 *  	ogdi/glutil/iofile.c vpflib/vpfprim.c vpflib/vpfspx.c
 *  	vpflib/vpftable.c vpflib/vpftidx.c vpflib/xvt.h
 *
 * Revision 1.5  2001/04/12 19:22:46  warmerda
 * applied DND support Image type support
 *
 */

#ifndef RPF_H
#define RPF_H 1

#include "ecs.h"
#include <sys/stat.h>
#ifdef _WINDOWS
#include <direct.h>
#include "compat/dirent.h"
#else
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#endif
#ifdef _WINDOWS
#include <io.h>
#endif
#include <fcntl.h>
#include <string.h>

#include <ogdi_macro.h>


#ifndef max
#define max(x,y) ((x > y) ? x : y)
#endif

#define ushort         unsigned short
#define   uint         unsigned int
/* typedef's conflict with /usr/include/sys/types.h
typedef unsigned short ushort;
typedef unsigned long  uint;
*/
typedef unsigned char  bool;
typedef unsigned char  uchar;
typedef unsigned char* ucharp;
typedef char           ascii;
typedef float          real4;
typedef double         real8;

#ifndef BOOLEAN
#define BOOLEAN short
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

#define   BOOL_TRUE        0xFF
#define   BOOL_FALSE       0

#define   LOC_BOUNDARIES   3
#define   LOC_FRAMES       4

#define   LOC_COVERAGE     6
#define   LOC_COMPRESSION  8
#define   LOC_CLUT         9
#define   LOC_IMAGE       10

#define LOC_HEADER_SECTION                   128
#define LOC_LOCATION_SECTION                 129
#define LOC_COVERAGE_SECTION                 130
#define LOC_COMPRESSION_SECTION              131
#define LOC_COMPRESSION_LOOKUP_SUBSECTION    132
#define LOC_COMPRESSION_PARAMETER_SUBSECTION 133
#define LOC_COLORGRAY_SECTION_SUBHEADER      134
#define LOC_COLORMAP_SUBSECTION              135
#define LOC_IMAGE_DESCR_SUBHEADER            136
#define LOC_IMAGE_DISPLAY_PARAM_SUBHEADER    137
#define LOC_MASK_SUBSECTION                  138
#define LOC_COLOR_CONVERTOR_SUBSECTION       139
#define LOC_SPATIAL_DATA_SUBSECTION          140
#define LOC_ATTRIBUTE_SECTION_SUBHEADER      141
#define LOC_ATTRIBUTE_SUBSECTION             142
#define LOC_EXPLICIT_AREAL_TABLE             143
#define LOC_RELATED_IMAGE_SECTION_SUBHEADER  144
#define LOC_RELATED_IMAGE_SUBSECTION         145
#define LOC_REPLACE_UPDATE_SECTION_SUBHEADER 146
#define LOC_REPLACE_UPDATE_TABLE             147
#define LOC_BOUNDARY_SECTION_SUBHEADER       148
#define LOC_BOUNDARY_RECTANGLE_TABLE         149
#define LOC_FRAME_FILE_INDEX_SUBHEADER       150
#define LOC_FRAME_FILE_INDEX_SUBSECTION      151
#define LOC_COLOR_TABLE_SECTION_SUBHEADER    152
#define LOC_COLOR_TABLE_INDEX_RECORD         153

typedef enum _rpf_type
{
   RPF_CADRG,
   RPF_CIB,
   RPF_CDTED
} Rpf_type;

typedef struct header
{
  bool     endian;
  ushort   hdr_sec_len;       /* header section length */
  ascii    filename[12];
  uchar    new;
  ascii    standard_num[15];
  ascii    standard_date[8];
  ascii    classification;
  ascii    country[2];
  ascii    release[2];
  uint     loc_sec_phys_loc;  /* location section physical location */
  uint     loc_sec_log_loc;   /* location section logical location */
  uint     cadrg_or_cib;
  Rpf_type rpf_type;
  int     NITF_hdr_len;
} Header;


typedef struct location
{
  ushort   id;
  uint     length;       /* section/component length */
  uint     phys_index;   /* section/component physical location */
  uint     log_index;    /* section/component logical location */
} Location;

#define  CADRG_COLORS      216

typedef struct rgb
{
  unsigned char      r, g, b, alpha;
} Rgb;

#define   RGPF_TOC   "a.toc"

typedef struct boundary
{
  ascii       type[8];
  ascii       compression[6];
  ascii       scale[18];
  ascii       zone[2];
  ascii       producer[6];
  real8      nw_lat, nw_long;
  real8      sw_lat, sw_long;
  real8      ne_lat, ne_long;
  real8      se_lat, se_long;
  real8      vert_interval, horiz_interval;
  real8      vert_resolution, horiz_resolution;
  uint       vert_frames, horiz_frames;
} Boundary;


typedef struct frame_index
{
  ushort     boundary_id;
  ushort     frame_row, frame_col;
  ascii      dir_number[3];
  ascii      filename[12];
  ascii      location[6];
} Frame_index;

typedef struct frame_entry
{
  int     exists;
  ushort   frame_row;
  ushort   frame_col;
  char     *directory;
  char     filename[16];
  char     georef[7];
} Frame_entry;


typedef struct toc_entry
{
  real8       nw_lat, nw_long;
  real8       sw_lat, sw_long;
  real8       ne_lat, ne_long;
  real8       se_lat, se_long;
  real8       vert_interval, horiz_interval;
  real8       vert_resolution, horiz_resolution;
  int        horiz_frames, vert_frames;
  Frame_entry **frames;
  ushort      boundary_id;
  ascii       type[8];
  ascii       compression[6];
  ascii       scale[18];
  ascii       zone[2];
  ascii       producer[6];
  ascii       *title;
  int        invalid_geographics;
} Toc_entry;

typedef struct toc_file
{
  Header     head;
  Toc_entry *entries;
  uint       num_boundaries;
} Toc_file;

typedef struct coverage
{
  real8     nw_lat, nw_long;
  real8     sw_lat, sw_long;
  real8     ne_lat, ne_long;
  real8     se_lat, se_long;
  real8     vert_resolution, horiz_resolution;
  real8     vert_interval, horiz_interval;
} Coverage;


typedef struct compression
{
  ushort   algorithm;
  ushort   noff_recs;
  ushort   nparm_off_recs;
  ushort   tables;
  ushort   table_length;
  ushort   parameters;
  ushort   para_length;
} Compression;


typedef struct lookup_table
{
  ushort   id;
  uint     records;
  ushort   values;
  ushort   bit_length;
  uint     phys_offset;
} Lookup_table;

typedef struct comp_param
{
  ushort id;
  uint rec_off;
} Comp_parm;


typedef struct color_table
{
  uchar     colormaps;
  ushort    color_length;
  uint      table_index;
  ascii     filename[12];
} Color_table;


typedef struct color_offset
{
  ushort   color_id;
  uint     records;
  uchar    length;
  ushort   hist_length;
  uint     offset_color_index;
  uint     offset_histogram_index;
} Color_offset;


typedef struct rpf_image
{
  ushort   spectral_groups;
  ushort   subframe_tables;
  ushort   spectral_tables;
  ushort   spectral_lines;
  ushort   horiz_subframes, vert_subframes;
  uint     output_cols, output_rows;
} RPF_image;


typedef struct attribute
{
  ascii    curr_date[8];
  ascii    prod_date[8];
  ascii    sig_date[8];
  ascii    series_code[2];
  ascii    map_code[8];
  ascii    old_datum_code[4];
  ascii    edition_id[8];
  ascii    proj_code[2];
  real4    parameters[4];
  ushort   vert_datum_code;
  ushort   horiz_datum_code;
  uint     vert_abs_acc;
  ushort   vert_abs_units;
  uint     horiz_abs_acc;
  ushort   horiz_abs_units;
  uint     vert_rel_acc;
  ushort   vert_rel_units;
  uint     horiz_rel_acc;
  ushort   horiz_rel_units;
  ushort   ellips_code;
  ascii    sound_datum_code;
  ushort   nav_code;
  short    grid_code;
  real4    east_mag_chg;
  ushort   east_mag_units;
  real4    west_mag_chg;
  ushort   west_mag_units;
  real4    grid_north;
  ushort   grid_north_units;
  short    max_elev;
  ushort   max_elev_units;
  real8    max_elev_lat;
  real8    max_elev_lon;
} Attribute;


typedef struct replace
{
  ascii    new_filename[12];
  ascii    old_filename[12];
  ascii    repupd_status;
} Replace;


typedef struct display1
{
  bool      all_subframes;
  bool      no_transparent;
  ushort    subframe_seq_length;
  ushort    transparent_seq_length;
  uint      image_rows;
  uint      image_codes;
  uchar     code_length;
  uint      transparent_value;
} Display1;

typedef struct frame_file
{
  Frame_entry  entry;
  Header       head;
  Coverage     cover;
  Compression  compr;
  uint         loc_lut_shdr;
  Lookup_table lut[4];
  uint         comp_parm_shdr;
  Comp_parm    *comp;
  uint         indices[6][6];
  uint         loc_data;
  RPF_image    img;
  Attribute    att;
  Replace      repupd;
  bool         all_subframes;
  int         NITF_hdr_len;
} Frame_file;

extern void check_swap _ANSI_ARGS_((unsigned char little_endian));
extern void swap       _ANSI_ARGS_((unsigned char *ptr, size_t count));

extern Toc_entry *parse_toc       _ANSI_ARGS_((ecs_Server *s, char *, Header *, uint *));
extern int       parse_locations _ANSI_ARGS_((ecs_Server *s, FILE *, Location *, int));
extern int       parse_clut      _ANSI_ARGS_((ecs_Server *s, Frame_file *, char *, Rgb *,int,uint *, int, uint *, uchar *));
extern int       parse_frame     _ANSI_ARGS_((ecs_Server *s, Frame_file *, char *));

extern BOOLEAN    get_comp_lut    _ANSI_ARGS_((ecs_Server *s, Frame_file *file, char *, uchar *table, uint *cct, int ReducedColorTable));
extern BOOLEAN    get_rpf_image_tile _ANSI_ARGS_((ecs_Server *s, Frame_file *, char *, int tno, unsigned char *table, uchar *tile, int decompress, uchar blackpixel));
extern void       free_toc        _ANSI_ARGS_((Toc_file *toc));

/* Categories table */

typedef struct {
  unsigned int r, g, b;
  int count;
} CatTable;

typedef struct {
  int isActive;
  unsigned char data[65536]; /* 256*256 Sub frames */
} tile_mem;

/*
********************************************************************

STRUCTURE_INFORMATION

NAME

     LayerPrivateData

DESCRIPTION

     structure for storing informations need by a OGDI layer in RPF

END_DESCRIPTION

ATTRIBUTES

     Toc_entry *entry: Pointer to the table of content for this layer
     int tile_row: Current tile row in the tile table of this layer
     int tile_col: Current tile col in the tile table of this layer
     int isActive: Indicate if the layer information contain a tile or not.
     int rows: Number of rows of the current tile
     int columns: Number of columns of the current tile
     unsigned int mincat: Minimum number of categories of the current tile
     unsigned int maxcat: Maximum number of categories of the current tile
     int firstcoordfilepos: First position in the file of the current tile
     Frame_file *ff: Header file information for the current tile
     Rgb *rgb_pal: Table of colors
     int equi_cat[255]: Table of correspondance between each value in the rgb_pal and the 6x6x6 color table.
     uint n_pal_cols: Number of colors in the rgb_pal table
     uchar *rpf_table: The rpf table
     uchar blackpixel: The blackpixel value
     uint *cct: The cct
     int boundary_num: Boundary number
     int firsttile: Number of the first tile
     tile_mem *buffertile: Buffer tile list (each tile contain a 6x6 subtiles)
     ecs_TileStructure tilestruct: The tile structure used by the tile handler library
     unsigned int isColor: Indicate if the table is in color or not
END_ATTRIBUTES

END_STRUCTURE_INFORMATION

********************************************************************
*/

typedef struct {

  Toc_entry *entry;
  int tile_row;
  int tile_col;
  int isActive;
  int rows;
  int columns;
  unsigned int mincat;
  unsigned int maxcat;
  int firstcoordfilepos;
  Frame_file *ff;
  Rgb *rgb_pal;
  int equi_cat[255];
  uint n_pal_cols;
  uchar *rpf_table;
  uchar blackpixel;
  uint *cct;
  int boundary_num;
  int firsttile;
  tile_mem *buffertile;
  ecs_TileStructure tilestruct;
  unsigned int isColor;
  
} LayerPrivateData;

/*
********************************************************************

STRUCTURE_INFORMATION

NAME

     ServerPrivateData

DESCRIPTION

     structure for storing global informations for the driver rpf

END_DESCRIPTION

ATTRIBUTES

     char *pathname: The directory path where the database is located in the hard-disk.
     Toc_file *toc: The table of content of the rpf database.

END_ATTRIBUTES

END_STRUCTURE_INFORMATION

********************************************************************
*/

typedef struct {

     char *pathname;
     Toc_file *toc;

} ServerPrivateData;

/* private functions prototype */

int dyn_prepare_rpflayer       _ANSI_ARGS_((ecs_Server *s,ecs_Layer *l));
int dyn_read_rpftile           _ANSI_ARGS_((ecs_Server *s,ecs_Layer *l,int tile_row,int tile_col));
int dyn_verifyLocation         _ANSI_ARGS_((ecs_Server *s));
int dyn_initRegionWithDefault  _ANSI_ARGS_((ecs_Server *s));
void check_swap             _ANSI_ARGS_((unsigned char little_endian));
void swap                   _ANSI_ARGS_((unsigned char *ptr, size_t count));
Toc_entry *parse_toc        _ANSI_ARGS_((ecs_Server *s, char *dir, Header *head, uint *num_boundaries));
int parse_locations        _ANSI_ARGS_((ecs_Server *s, FILE *fin, Location *locs, int count));
int parse_clut             _ANSI_ARGS_((ecs_Server *s,Frame_file *frame,char *filename,
					 Rgb *rgb,int ReducedColorTable,uint *cct,int Nitf_hdr_len,
					 uint *n_cols,uchar *blackpixel));
int parse_frame            _ANSI_ARGS_((ecs_Server *s,Frame_file *file,char *filename));
BOOLEAN get_comp_lut        _ANSI_ARGS_((ecs_Server *s,Frame_file *file,char *filename,
					 uchar *table, uint *cct,int ReducedColorTable));
BOOLEAN get_rpf_image_tile  _ANSI_ARGS_((ecs_Server *s,Frame_file *file,char *filename,int tno,
					 uchar *table,uchar *tile,int decompress,uchar blackpixel));
void free_toc               _ANSI_ARGS_((Toc_file *toc));

/* objects functions */

void dyn_getNextObjectRaster   _ANSI_ARGS_((ecs_Server *s,ecs_Layer *l));
void dyn_rewindRasterLayer     _ANSI_ARGS_((ecs_Server *s,ecs_Layer *l));
int dyn_PointCallBack          _ANSI_ARGS_((ecs_Server *s,ecs_TileStructure *t,int xtile,int ytile,
					 int xpixel,int ypixel,int *cat));
int dyn_ImagePointCallBack          _ANSI_ARGS_((ecs_Server *s,ecs_TileStructure *t,int xtile,int ytile,
						 int xpixel,int ypixel,int *cat));
int dyn_DimCallBack            _ANSI_ARGS_((ecs_Server *s,ecs_TileStructure *t,
					 double x,double y,int *width,int *height));

/* layer oriented method definition */
void		dyn_openRasterLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		dyn_closeRasterLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		dyn_rewindRasterLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		dyn_getNextObjectRaster _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		dyn_getObjectRaster _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, char *objectId));
void		dyn_getObjectIdRaster _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, ecs_Coordinate *coord));
int         _calcPosValue _ANSI_ARGS_((ecs_Server *s,ecs_Layer *l,int i,int j));
void		dyn_openImageLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		dyn_closeImageLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		dyn_rewindImageLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		dyn_getNextObjectImage _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		dyn_getObjectImage _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, char *objectId));
void		dyn_getObjectIdImage _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, ecs_Coordinate *coord));
int             _calcImagePosValue _ANSI_ARGS_((ecs_Server *s,ecs_Layer *l,int i,int j));

/* layer oriented method are keeped into a single data structure to simplify the code */

typedef void layerfunc();
typedef void layerobfunc();
typedef void layercoordfunc();


typedef struct {
  layerfunc	*open;
  layerfunc	*close;
  layerfunc	*rewind;
  layerfunc	*getNextObject;
  layerobfunc	*getObject;
  layercoordfunc	*getObjectIdFromCoord;	
} LayerMethod;

void dyn_calPosWithCoord _ANSI_ARGS_((ecs_Server *s,ecs_Layer *l,double pos_x,double pos_y,int *i,int *j));
int	dyn_IsOutsideRegion _ANSI_ARGS_((double n, double s, double e,double w, ecs_Region *x));
void	dyn_releaseAllLayers _ANSI_ARGS_((ecs_Server *s));

/*
#define tprintf(A) printf(A)
*/

#define tprintf(A)

#endif
