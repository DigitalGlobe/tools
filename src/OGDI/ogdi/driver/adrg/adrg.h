/******************************************************************************
 *
 * Component: OGDI ADRG Driver
 * Purpose: Data structure and prototype declarations for ADRG driver.
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
 * $Log: adrg.h,v $
 * Revision 1.8  2007/02/12 21:01:48  cbalint
 *      Fix win32 target. It build and works now. (tested with VC6)
 *
 * Revision 1.7  2007/02/12 16:09:06  cbalint
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
 * Revision 1.6  2001/06/23 14:06:31  warmerda
 * added capabilities support, cache layer list when opening datastore
 *
 * Revision 1.5  2001/06/22 16:37:50  warmerda
 * added Image support, upgraded headers
 *
 */

#include "ecs.h"
#include <sys/stat.h>
#ifdef _WINDOWS
#include <direct.h>
#include "compat/dirent.h"
#include <io.h>

#else
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#endif
#include <fcntl.h>

#include <ogdi_macro.h>

#define MAXADRGTILES 26

#ifndef max
#define max(x,y) ((x > y) ? x : y)
#endif

typedef struct {
  int isActive;
  unsigned char data[49152];
} tile_mem;

/* private data general to all Grass layer */

typedef struct {
  char imgname[10];      /* IMG name */
  char imgfilename[14]; /* IMG file name */
  int zonenumber;       /* ARC zone number */
  int rows;             /* Number of rows */
  int columns;          /* Number of columns */
  int rowtiles;         /* Number of 128x128 tiles in a row */
  int coltiles;         /* Number of 128x128 tiles in a column */
  ecs_Region region;    /* Bounding rectangle of the matrix */
  int *tilelist;        /* Tile list of all the tiles positions.
			   This list is (rowtiles*coltiles) long. */
  FILE *imgfile;        /* IMG file itself */
  int ARV,BRV;
  double LSO,PSO;
  int firstposition;    /* First pixel position in file */
  tile_mem *buffertile; /* buffer table */
  int firsttile;        /* Position of the first tile in the buffer table */
} LayerPrivateData;

typedef struct {
  char *genfilename;
  char *imgdir;
  LayerPrivateData overview;

  int  layer_count;
  char **layer_list;

} ServerPrivateData;

/* layer oriented method definition */

void		_openRasterLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_closeRasterLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_rewindRasterLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_getNextObjectRaster _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_getObjectRaster _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, char *objectId));
void		_getObjectIdRaster _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, ecs_Coordinate *coord));
int             _calcPosValue _ANSI_ARGS_((ecs_Server *s,ecs_Layer *l,int i,int j,int UseOverview));
void		_openImageLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_closeImageLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_rewindImageLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_getNextObjectImage _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_getObjectImage _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, char *objectId));
void		_getObjectIdImage _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, ecs_Coordinate *coord));
int             _calcImagePosValue _ANSI_ARGS_((ecs_Server *s,ecs_Layer *l,int i,int j,int UseOverview));
void            _LoadADRGTiles _ANSI_ARGS_((ecs_Server *s,ecs_Layer *l,int *UseOverview));

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

void _calPosWithCoord _ANSI_ARGS_((ecs_Server *s,ecs_Layer *l,double pos_x,double pos_y,int *i,int *j,int UseOverview));

int	_IsOutsideRegion _ANSI_ARGS_((double n, double s, double e,double w, ecs_Region *x));

int _read_adrg _ANSI_ARGS_((ecs_Server *s,ecs_Layer *l));
int _read_overview _ANSI_ARGS_((ecs_Server *s));
char *subfield _ANSI_ARGS_((char *buffer, int index, int length));
double parse_coord_x _ANSI_ARGS_((char *buffer));
double parse_coord_y _ANSI_ARGS_((char *buffer));
int _verifyLocation _ANSI_ARGS_((ecs_Server *s));
int _initRegionWithDefault _ANSI_ARGS_((ecs_Server *s));
char *loc_strupr _ANSI_ARGS_((char *string));
char *loc_strlwr _ANSI_ARGS_((char *string));

