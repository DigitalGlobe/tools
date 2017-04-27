/*********************************************************************

  CSOURCE_INFORMATION
  
  NAME
     skeleton.h

  DESCRIPTION
     Data structure and prototype definition for the skeleton driver
  END_DESCRIPTION

  MOD: Bruno Savard, INFOMAR INC., bsavard@infomar.com, 1998/09/21
  Description: Removal of th unused include "glutil.h"

  END_CSOURCE_INFORMATION

  Copyright (C) 1995 Logiciels et Applications Scientifiques (L.A.S.) Inc
  Permission to use, copy, modify and distribute this software and
  its documentation for any purpose and without fee is hereby granted,
  provided that the above copyright notice appear in all copies, that
  both the copyright notice and this permission notice appear in
  supporting documentation, and that the name of L.A.S. Inc not be used 
  in advertising or publicity pertaining to distribution of the software 
  without specific, written prior permission. L.A.S. Inc. makes no
  representations about the suitability of this software for any purpose.
  It is provided "as is" without express or implied warranty.
  
  ********************************************************************/

#ifndef SKELETON_H
#define SKELETON_H


/*********************************************************************

  MODULE_INFORMATION

  NAME
     Skeleton driver

  DESCRIPTION
     This driver is an example to help developers to write new OGDI
     drivers.  It is part of the Open Geospatial Datastore Interface
     (OGDI).  This driver communicates dummy data to the OGDI API.
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
     skeleton.h
     skeleton.c
     object.c
     open.c
     utils.c
  END_C_SOURCES

  END_MODULE_INFORMATION

  ****************************************************************/

#include "ecs.h"


/*********************************************************************

  STRUCTURE_INFORMATION
  
  NAME
     LayerPrivateData

  DESCRIPTION
     LayerPrivateData holds all the layer information private
     data. The main idea of this structure is to give the driver
     programmer a place where to keep specific driver information
     related to a layer selection. If the layer selection of a driver
     needs to open a file, this will be the ideal placeholder.
  END_DESCRIPTION

  ATTRIBUTES
     ecs_Region matrixregion: For matrix layers, contain the region
                              occupied by the matrix.
     int matrixwidth: For matrix layers, the width of the matrix
     int matrixheight: For matrix layers, the height of the matrix
  END_ATTRIBUTES

  END_STRUCTURE_INFORMATION

  ********************************************************************/

typedef struct {
  ecs_Region matrixregion;
  int matrixwidth;
  int matrixheight;
  int offsetx;
  int offsety;
} LayerPrivateData;

/*********************************************************************

  STRUCTURE_INFORMATION
  
  NAME
     ServerPrivateData

  DESCRIPTION
     ServerPrivateData holds all the driver private data. The idea of
     this structure is to have a placeholder for the driver specific
     information.  For example, if the driver is a database interface,
     the database information and its location will be kept in this
     structure.
  END_DESCRIPTION

  ATTRIBUTES
     int globaldummy: Dummy information. It's here only to show the 
                      initialization of a value during dyn_CreateServer 
		      operation.
  END_ATTRIBUTES

  END_STRUCTURE_INFORMATION

  ********************************************************************/

typedef struct {
  int globaldummy;
} ServerPrivateData;

/* open.c prototypes */

int _openAreaLayer _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l));
void _closeAreaLayer _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l));
void _rewindAreaLayer _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l));
int _openLineLayer _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l));
void _closeLineLayer _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l));
void _rewindLineLayer _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l));
int _openPointLayer _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l));
void _closePointLayer _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l));
void _rewindPointLayer _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l));
int _openMatrixLayer _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l));
void _closeMatrixLayer _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l));
void _rewindMatrixLayer _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l));
int _openTextLayer _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l));
void _closeTextLayer _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l));
void _rewindTextLayer _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l));

/* object.c prototypes */

void _getNextObjectArea _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l));
void _getObjectArea _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l, char *id));
void _getObjectIdArea _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l,ecs_Coordinate *coord));
void _getNextObjectLine _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l));
void _getObjectLine _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l, char *id));
void _getObjectIdLine _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l,ecs_Coordinate *coord));
void _getNextObjectPoint _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l));
void _getObjectPoint _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l, char *id));
void _getObjectIdPoint _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l,ecs_Coordinate *coord));
void _getNextObjectText _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l));
void _getObjectText _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l, char *id));
void _getObjectIdText _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l,ecs_Coordinate *coord));
void _getNextObjectMatrix _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l));
void _getObjectMatrix _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l, char *id));
void _getObjectIdMatrix _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l,ecs_Coordinate *coord));
int _calcPosValue _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l,int i,int j));
int _getValueFromCoord _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l,int pix_c,int pix_r));

/* 
   utils.c prototype 

   Usually, all the utility functions are hold here. In our case, the
   skeleton don't contain any specific functionnality. The utility
   functions are internal functions for general purpose.
   */

/* layer structure */

typedef int layerfunc();
typedef void layervoidfunc();

/*********************************************************************

  STRUCTURE_INFORMATION
  
  NAME
     LayerMethod

  DESCRIPTION
     LayerMethod holds all the pointers to geographical access functions.
  END_DESCRIPTION

  ATTRIBUTES
     layerfunc *open: Pointer to a open function
     layervoidfunc *close: Pointer to a close function
     layervoidfunc *rewind: Pointer to a rewind function
     layervoidfunc *getNextObject: Pointer to a GetNextObject function
     layervoidfunc *getObject: Pointer to a GetObject function
     layervoidfunc *getObjectIdFromCoord: Pointer to a GetObjectIdFromCoord function
  END_ATTRIBUTES

  END_STRUCTURE_INFORMATION

  ********************************************************************/

typedef struct {
  layerfunc	*open;
  layervoidfunc	*close;
  layervoidfunc	*rewind;
  layervoidfunc	*getNextObject;
  layervoidfunc	*getObject;
  layervoidfunc	*getObjectIdFromCoord;	
} LayerMethod;

/* 
   Here is the "Database" definition itself. Skeleton holds global
   variables to contain data information of the database. The
   definitions are in object.c.
   */

typedef struct {
  double x,y;
} point_data;

typedef struct {
  int id;
  int arealistlength;
  point_data arealist[15];
  int islandlistlength;
  point_data islandlist[15];
  double north;
  double south;
  double east;
  double west;
} dbareatype;

typedef struct {
  int id;
  int linelistlength;
  point_data linelist[15];
  double north;
  double south;
  double east;
  double west;
} dblinetype;

typedef struct {
  int id;
  point_data geopoint;
} dbpointtype;

typedef struct {
  int id;
  point_data geopoint;
} dbtexttype;

#endif
