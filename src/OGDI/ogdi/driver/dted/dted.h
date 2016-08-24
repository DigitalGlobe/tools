/******************************************************************************
 *
 * Component: OGDI DTED Driver
 * Purpose: Structure and function declarations for DTED Driver.
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
 * $Log: dted.h,v $
 * Revision 1.4  2001/04/10 14:29:43  warmerda
 * Upgraded with changes from DND (hand applied to avoid losing bug fixes).
 * Patch also includes change to exclude zero elevations when computing
 * mincat/maxcat.
 * New style headers also applied.
 *
 */

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
#include <fcntl.h>

#ifdef _WINDOWS
#define strcasecmp(a,b) stricmp(a,b)
#define strncasecmp(a,b,c) strnicmp(a,b,c)
#endif

#ifndef max
#define max(x,y) ((x > y) ? x : y)
#endif

#define HDR_LENGTH 80
#define UHL_LENGTH 80
#define DSI_LENGTH 648
#define ACC_LENGTH 2700

/* Categories table */

typedef struct {
  unsigned int elevation;
  int count;
} CatTable;

/* private data general to all Grass layer */

typedef struct {
  char *ewdir;       /* Directory for east-west coord */
  char *nsfile;      /* DTED tile file in ewdir */

#if 0
  int rows;          /* Number of rows */
  int columns;       /* Number of columns */
#endif

  unsigned char *matrixbuffer; /* dted matrix itself */
  int isInRam;
  ecs_Family family;

} LayerPrivateData;

typedef struct NSFile {
  char name[20];
  short used;  /* 1 if used 0 if unused (i.e. no data for this tile) */
  ecs_Region region;
  int rows;
  int columns;

  int coord; /* the longlat coordinate value : not guaranteed to be there */
  FILE *filehandle;
} NSFile;

typedef struct EWDir {
  char name[20];         /* the name of the directory */
  NSFile *nsfile;       /* array of filenames */  
  unsigned short nb_files;         /* number of files in this dir*/
  int coord; /* the longlat coordinate value : not guaranteed to be there */
} EWDir;

typedef struct {
  int mincat;        /* Minimum and maximum category */
  int maxcat;           
  char layername[16];             /* the layername from DMED */
  char *pathname;
  EWDir *ewdir;               /* 1D array of directory names */

  ecs_TileStructure t;
  int xtiles;        /* the number of horiz tiles */
  int ytiles;        /* the number of vertical tiles */
  ecs_TileID lastTile;
  int level;         /* 0,1,2 depending on the "level" of the DTED file */
  int cat_increment; /* the width of one colour band */
  int firstcoordfilepos;

} ServerPrivateData;

/* private functions prototype */

int             _parse_request _ANSI_ARGS_((ecs_Server *s,char *sel,int *isInRam));
int             _read_dted _ANSI_ARGS_((ecs_Server *s,int xtile, int ytile));
int		_verifyLocation _ANSI_ARGS_((ecs_Server *s));
int		_createDynamicMapset _ANSI_ARGS_((ecs_Server *s));
int		_initRegionWithDefault _ANSI_ARGS_((ecs_Server *s));
char            *subfield _ANSI_ARGS_((char *buffer,int index,int length));
double          parse_coord _ANSI_ARGS_((char *buffer));
int             _initCatTable _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
int             _readDMED _ANSI_ARGS_((ecs_Server *s));
int             _readValuesFromDirList _ANSI_ARGS_((ecs_Server *s));

/* layer oriented method definition */

void		_openRasterLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_closeRasterLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_rewindRasterLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_getNextObjectRaster _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_getObjectRaster _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, char *objectId));
void		_getObjectIdRaster _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, ecs_Coordinate *coord));
int             _calcPosValue _ANSI_ARGS_((ecs_Server *s, ecs_TileStructure *t, int xtile,
		  int ytile, int xpixel, int ypixel, int *cat));
int             _getTileDim _ANSI_ARGS_((ecs_Server *s, ecs_TileStructure *t, double x, double y, int *width, int  *height));

int             _get_level _ANSI_ARGS_((ecs_Server *s, int xtile, int ytile, int *level));
int             _sample_tiles _ANSI_ARGS_((ecs_Server *s, ecs_TileStructure *t));
int             _getRawValue _ANSI_ARGS_((ecs_Server *s, ecs_TileStructure *t, int xtile,
		 int ytile, int xpixel, int ypixel, int *cat));
int             _sample_getRawValue _ANSI_ARGS_((ecs_Server *s, ecs_TileStructure *t, int xtile,
		 int ytile, int xpixel, int ypixel, int *cat));
/* int             _sample_read_dted _ANSI_ARGS_((ecs_Server *s, int xtile, int ytile, int32 *firstcoordfilepos)); */
int             _sample_read_dted(ecs_Server *s, int xtile, int ytile, int32 *firstcoordfilepos, FILE *fileptr);


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

int	_IsOutsideRegion _ANSI_ARGS_((double n, double s, double e,double w, ecs_Region *x));
void	_releaseAllLayers _ANSI_ARGS_((ecs_Server *s));



