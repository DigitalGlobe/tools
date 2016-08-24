/******************************************************************************
 *
 * Component: OGDI VRF Driver
 * Purpose: Data structure and prototype definition for the VRF driver
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
 * $Log: vrf.h,v $
 * Revision 1.10  2004/04/04 04:33:01  warmerda
 * added vrf_free_ObjAttributeBuffer
 *
 * Revision 1.9  2004/02/18 21:33:18  warmerda
 * free regex memory
 *
 * Revision 1.8  2001/08/16 21:02:37  warmerda
 * Removed MAXSEGS and MAXRINGS fixed limits
 *
 * Revision 1.7  2001/08/16 20:40:34  warmerda
 * applied VITD fixes - merge primitive lines into a feature
 *
 * Revision 1.6  2001/06/20 21:49:31  warmerda
 * added improved query support (swq)
 *
 * Revision 1.5  2001/06/13 17:33:59  warmerda
 * upgraded source headers
 *
 */

/*********************************************************************

  MODULE_INFORMATION

  NAME
     VRF driver

  DESCRIPTION
     This driver access VRF database. It is part a of the Open Geospatial
     Datastore Interface (OGDI). This driver communicate VRF data to
     the OGDI API.
  END_DESCRIPTION
     
  EXPORTED_FUNCTIONS
     dyn_CreateServer
     dyn_DestroyServer
     dyn_SelectLayer
     dyn_ReleaseLayer
     dyn_SelectRegion
     dyn_GetDictionary
     dyn_GetAttributesFormat
     dyn_GetNextObject
     dyn_GetObject
     dyn_GetObjectIdFromCoord
     dyn_UpdateDictionary
     dyn_GetServerProjection
     dyn_GetGlobalBound
     dyn_SetServerLanguage
  END_EXPORTED_FUNCTIONS

  C_SOURCES
     vrf.c
     vrf.h
     object.c
     feature.c
     open.c
     utils.c
  END_C_SOURCES

  END_MODULE_INFORMATION

  ****************************************************************/

/* #define TESTOPENTABLE 1 */

#include <sys/stat.h>
#ifdef _WINDOWS
#include <direct.h>
#include "compat/dirent.h"
#else
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#endif

#include "glutil.h"
#include "vpfview.h"
#include "vpfquery.h"
#include "vpfprim.h"
#include "musedir.h"
#include "strfunc.h"



/* ----------------------------------------
 * Define VRF_LINE_JOIN_HACK to enable
 * merging of line features based on the
 * join table. 
 * ----------------------------------------
 */

#define VRF_LINE_JOIN_HACK

/* ----------------------------------------
 * Definition of VRF specific structures
 * ----------------------------------------
 */

#ifndef max
#define max(x,y) ((x > y) ? x : y)
#endif


/* private data for a VPF Line layer  */

typedef struct {

  vpf_table_type edgeTable;
  vpf_table_type mbrTable;

} VRFLine;



/* private data for a VPF Area layer  */

typedef struct {

  vpf_table_type faceTable;
  vpf_table_type mbrTable;
  vpf_table_type ringTable;
  vpf_table_type edgeTable;
  
} VRFArea;



/* private data for a VPF Point layer  */

typedef struct {

  vpf_table_type primTable;

} VRFPoint;



/* private data for a VPF text layer  */

typedef struct {

  vpf_table_type textTable;

} VRFText;

typedef struct {
  char *path;		/* directory where the tiled info sits	*/	
  float xmin;		/* geographic extents of this tile */
  float xmax;
  float ymin;
  float ymax;
  int isSelected;		/* is within the current geographic region or not */ 
} VRFTile;


/*********************************************************************

STRUCTURE_INFORMATION

NAME 
    VRFIndex

DESCRIPTION 
    Description of one cell of the index table
END_DESCRIPTION

ATTRIBUTES 

    int32 feature_id: The identifier of the feature identifiant
    short tile_id: The tile id of the primitive data
    int32 prim_id: The identifier of the primitive identifiant

END_ATTRIBUTES

END_STRUCTURE_INFORMATION

********************************************************************/ 

typedef struct {
  int32 feature_id;
  short tile_id;
  int32 prim_id;
} VRFIndex;

/*********************************************************************

STRUCTURE_INFORMATION

NAME 
LayerPrivateData

DESCRIPTION 
Layer private data description.
END_DESCRIPTION

ATTRIBUTES 
    vpf_table_type featureTable: The feature table
    vpf_table_type joinTable: The join table
    set_type feature_rows: A set over the feature table showing the selection accordingly to the expression.
    int current_tileid: Indicate which tile is currently selected.
    VRFIndex *index: The index of feature to primitives relations.
    char *coverage: The current coverage of the layer
    char *fclass: The feature class of the coverage
    char *expression: The selection expression over the features.
    vpf_table_type fcsTable: The FCS table.
    char *featureTableName: The feature table name.
    char *featureTablePrimIdName: The feature table PrimId attribute name.
    char *joinTableName: The join table name.
    char *joinTableForeignKeyName: The join table foreign key name
    char *joinTableFeatureIdName: The join table FeatureId attribute name.
    char *primitiveTableName: The primitive table name.

    union { 

        VRFArea area; 
        VRFLine line; 
        VRFPoint point; 
        VRFText text; 

    } l; : This union contain the specialization for each data type.

END_ATTRIBUTES

END_STRUCTURE_INFORMATION

********************************************************************/ 

typedef struct {

  vpf_table_type featureTable;
  vpf_table_type joinTable;
  set_type feature_rows;	/* the set of all selected features according to expression */
  int current_tileid;	/* usefull to know when to reset the primitive tables: only when a new tile is encountered */
	
  VRFIndex *index;

  char *coverage;		/* the result of the sel.Request parsing */
  char *fclass;
  char *expression;

  vpf_table_type fcsTable;

  char *featureTableName;
  char *featureTablePrimIdName;
  char *joinTableName;
  char *joinTableForeignKeyName;
  char *joinTableFeatureIdName;
  char *primitiveTableName;
  int isTiled;		/* is this a tiled layer ? */ 
  int mergeFeatures;    /* merge primitives into features based on join table*/

  union {			/* specifics to each feature type */

    VRFArea area;
    VRFLine line;
    VRFPoint point;
    VRFText text; 

  } l;

} LayerPrivateData;


typedef struct {

  char database[256];		/* fullpath to database and library are usefull when opening table */
  char library[256];
  char libname[32];		/* the short name of the library (last part of the path) */
  char metadatastring[250000]; /*transfert to updatedictionnary of the metadata strings*/
  vpf_table_type catTable;	/* support files are opened at client creation and remain opened until client destruction */
  vpf_table_type latTable;
  vpf_table_type dhtTable;
  vpf_table_type lhtTable;
  vpf_table_type grtTable;
  vpf_table_type dqtTable;
  vpf_table_type fcsTable;
  vpf_table_type fcaTable;
	
  int isTiled;		/* is this a tiled library ? */ 
  VRFTile *tile;		/* tile info, if any, for this library */
  int nbTile;		/* number of tile, if tiled */
  int isDCW;            /* Is it a DCW database? */

  int isMetaLoad;       /* Indicate if the metainfo is load */

} ServerPrivateData;

	/* layer oriented method are keeped into a single data structure to simplify the code */

typedef void layerfunc();
typedef void layerobfunc();
typedef void layercoordfunc();


typedef struct {
  layerfunc	*getNextObject;
  layerobfunc	*getObject;
  layercoordfunc	*getObjectIdFromCoord;	
  layerfunc	*selectTile;
} LayerMethod;



/* VPF feature types used */

#define MAGIC_VEC2_DATA 120000002L


typedef struct
{
  float x;
  float y;
} COORDINATE;


typedef struct
{
  int32       id;
  int32       nr_coords;
  COORDINATE *coords;
} SEGMENT;


typedef struct
{
  int32    id;
  int32    nr_segs;
  SEGMENT **segs;
} RING;


typedef struct
{
  int32  id;
  int32  nr_rings;
  RING  **rings;
} AREA_FEATURE;


/* private functions prototype */

/* vrf.c */

void vrf_releaseAllLayers _ANSI_ARGS_((ecs_Server *s));

/* utils.c */

void vrf_freePathRegex();
int  vrf_parsePath _ANSI_ARGS_((ecs_Server *s,LayerPrivateData *lpriv,ecs_LayerSelection *sel));
int  vrf_parsePathValue _ANSI_ARGS_((ecs_Server *s,char *request,char **fclass, char **coverage,char **expression));
int  vrf_getFileNameFromFcs _ANSI_ARGS_((ecs_Server *s, LayerPrivateData *lpriv));
int  vrf_verifyCATFile _ANSI_ARGS_((ecs_Server *s));
int  vrf_initRegionWithDefault _ANSI_ARGS_((ecs_Server *s));
int  vrf_GetMetadata _ANSI_ARGS_((ecs_Server *s));
int  vrf_initTiling _ANSI_ARGS_((ecs_Server *s));
int  vrf_IsOutsideRegion _ANSI_ARGS_((double n, double s, double e,double w, ecs_Region *region));
void vrf_AllFClass _ANSI_ARGS_((ecs_Server *s, char *coverage));
int vrf_feature_class_dictionary _ANSI_ARGS_((ecs_Server *s,char *request));
int vrf_build_capabilities( ecs_Server *s, const char *request );

/* feature.c */

int  vrf_get_xy _ANSI_ARGS_((vpf_table_type table, row_type row,int32 pos, double *x,double *y));
int  vrf_get_point_feature _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, int prim_id));
int  vrf_get_line_feature _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer,
                                       int prim_id, ecs_Result *result ));
int  vrf_get_merged_line_feature _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer,
                                              int primCount, int32 *primList));
int  vrf_get_text_feature _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, int prim_id));
int  vrf_get_area_feature _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, int prim_id));
int  vrf_get_line_mbr _ANSI_ARGS_((ecs_Layer *layer,int32 prim_id,double *xmin,double *ymin,double *xmax,double *ymax));
int  vrf_get_lines_mbr _ANSI_ARGS_((ecs_Layer *layer,
                                    int32 primCount, int32 *primList,
                                    double *xmin,double *ymin,
                                    double *xmax,double *ymax));
int  vrf_get_area_mbr _ANSI_ARGS_((ecs_Layer *layer,int32 prim_id,double *xmin,double *ymin,double *xmax,double *ymax));
int  vrf_get_ring_coords _ANSI_ARGS_((ecs_Server *s, RING *ring, int32 face_id, 
				 int32 start_edge,vpf_table_type edgetable));
int32 vrf_next_face_edge _ANSI_ARGS_((edge_rec_type *edge_rec,int32 *prevnode, int32 face_id));
char *vrf_get_ObjAttributes _ANSI_ARGS_((vpf_table_type table,int32 row_pos));
void vrf_free_ObjAttributeBuffer();
int  vrf_checkLayerTables _ANSI_ARGS_((ecs_Server *s,ecs_Layer *l));

/* vrfswq.c */

set_type query_table2( char *expression, vpf_table_type table );

/* layer oriented method definition */

/* open.c */

void		_openLineLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_openAreaLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_openPointLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_openTextLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));

void		_closeLineLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_closeAreaLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_closePointLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_closeTextLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));

void		_rewindLineLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_rewindAreaLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_rewindPointLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_rewindTextLayer _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));


/* object.c */

void            _getTileAndPrimId _ANSI_ARGS_((ecs_Server *s,ecs_Layer *l,
					       int32 object_id,int32 *feature_id,
					       short *tile_id,int32 *prim_id));
void            _getPrimList _ANSI_ARGS_((ecs_Server *s,ecs_Layer *l,
                                          int32 object_id,
                                          int32 *feature_id, short *tile_id,
                                          int32 *primCount, int32 **primList,
                                          int32 *next_object_id));

void		_getNextObjectLine _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_getNextObjectArea _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_getNextObjectPoint _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));
void		_getNextObjectText _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer));

void		_getObjectLine _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, char *objectId));
void		_getObjectArea _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, char *objectId));
void		_getObjectPoint _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, char *objectId));
void		_getObjectText _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, char *objectId));

void		_getObjectIdLine _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, ecs_Coordinate *coord));
void		_getObjectIdArea _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, ecs_Coordinate *coord));
void		_getObjectIdPoint _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, ecs_Coordinate *coord));
void		_getObjectIdText _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, ecs_Coordinate *coord));

void		_selectTileLine _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, int tile_id));
void		_selectTileArea _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, int tile_id));
void		_selectTilePoint _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, int tile_id));
void		_selectTileText _ANSI_ARGS_((ecs_Server *s,ecs_Layer *layer, int tile_id));

void            _closeLayerTable _ANSI_ARGS_((ecs_Server *s,ecs_Layer *l));



