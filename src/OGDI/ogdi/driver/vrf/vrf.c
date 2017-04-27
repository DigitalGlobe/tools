/******************************************************************************
 *
 * Component: OGDI VRF Driver
 * Purpose: Implementation of the dyn_* entry points to the VRF driver.
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
 * $Log: vrf.c,v $
 * Revision 1.21  2016/07/06 09:01:30  erouault
 * VRF: fix memory leaks in error code paths of dyn_SelectLayer()
 *
 * Revision 1.20  2007/05/09 21:29:50  cbalint
 * fix is to increase of 1 the size of smallint (5 -> 6)
 *
 * Revision 1.19  2007/05/09 20:46:28  cbalint
 * From: Even Rouault <even.rouault@mines-paris.org>
 * Date: Friday 21:14:18
 *
 *         * fix filename case sensitivy problems (for Unix-like systems).
 *
 *         * fix incorrect use of sprintf in vrf_GetMetadata.
 *
 *         * report wgs84 instead of nad83, not sure whether that is true
 *         for all VPF products, but at least it's correct for VMAP products
 *         that *must* be WGS84. A better fix would be to read the VPF table
 *         that contains this information.
 *
 *         * fix a few minor memory leaks and memory usage issues.
 *
 *         * enable XMIN, YMIN, XMAX and YMAX columns to be of type double
 *         in EBR and FBR files (for read the VMAP2i 'MIG2i000' product).
 *
 *         * add .pjt and .tjt as possible extensions for join tables
 *         (VMAP2i 'MIG2i000' product).
 *
 *         * fix duplicated layers report (VMAP2i 'MIG2i000' product).
 *
 *         * handle 'L' (Latin1) type for text files (GEOCAPI 'MIGxxx' products).
 *
 *         * optionnaly, convert text to UTF-8 when environment variable
 *         CONVERT_OGDI_TXT_TO_UTF8 is defined. This part is not portable
 *         on Windows I guess (only tested on Linux) and maybe too specific.
 *
 *         * enable reading of VPF products without table indexes file
 *         (GEOCAPI 'MIG013' and 'MIG016' products). VPF norm says that when
 *         there is a variable length field in one table, an index should exist,
 *         but some test products don't follow this. The approach here is to read
 *         the whole table content and build the index in memory.
 *
 *  Modified Files:
 *  	ChangeLog ogdi/driver/vrf/feature.c ogdi/driver/vrf/object.c
 *  	ogdi/driver/vrf/utils.c ogdi/driver/vrf/vrf.c
 *  	ogdi/driver/vrf/vrfswq.c vpflib/musedir.c vpflib/strfunc.c
 *  	vpflib/vpfbrows.c vpflib/vpfprop.c vpflib/vpfquery.c
 *  	vpflib/vpfread.c vpflib/vpftable.c
 *
 * Revision 1.18  2007/02/12 21:01:48  cbalint
 *      Fix win32 target. It build and works now. (tested with VC6)
 *
 * Revision 1.17  2007/02/12 15:52:57  cbalint
 *
 *    Preliminary cleanup.
 *    Get rif of unitialized variables, and unused ones.
 *
 * Revision 1.16  2006/05/05 19:10:54  warmerda
 * VRF fix when doing feature merging with index sizing (ie. with DNC 13 dataset)
 *
 * Revision 1.15  2004/02/19 06:56:43  warmerda
 * fixed serious bug in releaseAllLayers() with multiple layers
 *
 * Revision 1.14  2004/02/19 05:59:22  warmerda
 * Removed temporary debug messages.
 *
 * Revision 1.13  2004/02/18 21:49:18  warmerda
 * Fixed typo in last fix.
 *
 * Revision 1.12  2004/02/18 21:33:18  warmerda
 * free regex memory
 *
 * Revision 1.11  2001/08/16 20:40:34  warmerda
 * applied VITD fixes - merge primitive lines into a feature
 *
 * Revision 1.10  2001/06/29 19:16:30  warmerda
 * fixed memory leak if FCS not found
 *
 * Revision 1.9  2001/06/21 20:30:15  warmerda
 * added ECS_CVSID
 *
 * Revision 1.8  2001/06/20 21:49:31  warmerda
 * added improved query support (swq)
 *
 * Revision 1.7  2001/06/13 17:33:59  warmerda
 * upgraded source headers
 *
 */

#include "ecs.h"
#include "vrf.h"
#include "datadict.h"

ECS_CVSID("$Id: vrf.c,v 1.21 2016/07/06 09:01:30 erouault Exp $");

/* layer oriented functions are keeped in data structure to simplify code */

LayerMethod layerMethod[11] = {
  /* 0 */      	{ NULL, NULL, NULL,NULL },
		/* Area */	{ _getNextObjectArea, _getObjectArea, _getObjectIdArea, _selectTileArea },
		/* Line */	{ _getNextObjectLine, _getObjectLine, _getObjectIdLine, _selectTileLine },
		/* Point */	{ _getNextObjectPoint, _getObjectPoint, _getObjectIdPoint, _selectTilePoint },
		/* Matrix */	{ NULL, NULL, NULL, NULL },
		/* Image */	{ NULL, NULL, NULL, NULL },
		/* Text */	{ _getNextObjectText, _getObjectText, _getObjectIdText, _selectTileText },
		/* Edge */	{ NULL, NULL, NULL, NULL },
		/* Face */	{ NULL, NULL, NULL, NULL },
		/* Node */	{ NULL, NULL, NULL, NULL },
		/* Ring */	{ NULL, NULL, NULL, NULL }
};

/* ----------------------------------------------------------------------
 *  _dyn_CreateServer: 
 *     
 *   Creation of a new GRASS server
 * ----------------------------------------------------------------------
 */


ecs_Result *dyn_CreateServer(s,Request)
     ecs_Server *s;
     char *Request;
{
  char buffer[256];
  char *ptr;
  int i;
  register ServerPrivateData *spriv; 

  (void) Request;

#ifdef VRF_DEBUG
  printf("dyn_CreateServer\n");
#endif

  spriv = s->priv = (void *) calloc(1,sizeof(ServerPrivateData));
  if (s->priv == NULL) {
    ecs_SetError(&(s->result), 1, "Could not create VRF server, not enough memory");
    return &(s->result);		
  }

  spriv->nbTile = 1;
  spriv->tile = NULL;
  spriv->isTiled = 0;
  spriv->isMetaLoad = FALSE;

  /* Check if s->pathname is valid */

  if (strlen(s->pathname) == 0) {
    ecs_SetError(&(s->result), 1, "Could not create VRF server, invalid URL");
    return &(s->result);		
  }

  /* extract full library pathname from s->pathname */

  if (s->pathname[2] == ':') { /* if it contain something like /e:/cdrom/dcw, strip first slash */
    strcpy(spriv->library,&(s->pathname[1]));
  } else {
    strcpy(spriv->library,s->pathname);
  }

  /* extract full database pathname and library short name from s->pathname */

  for(i = strlen(spriv->library) - 1; spriv->library[i] != '/'; --i);
  strncpy(spriv->database,spriv->library,i);	
  spriv->database[i] = 0;
  strcpy(spriv->libname,&(spriv->library[i+1]));

  /* verify if this is really a VRF database */

  if (!vrf_verifyCATFile(s)) {
    return &(s->result);		
  }

  /* Is it a DCW database (in the path) */

  spriv->isDCW = FALSE;
  for (i=0;i<(int) (strlen(s->pathname)-3);i++) {
    ptr = &(s->pathname[i]);
    if (strnicmp(ptr,"dcw",3) == 0) {
      spriv->isDCW = TRUE;
      break;
    }
  }

  /* open schema files */

  sprintf(buffer,"%s/lat",spriv->database);
  if (muse_access(buffer,0) != 0) {
    sprintf(buffer,"%s/LAT",spriv->database);
  }
#ifdef TESTOPENTABLE
  printf("open spriv->latTable:%s\n",buffer);
#endif

  spriv->latTable = vpf_open_table(buffer, disk, "rb", NULL);

  if (spriv->latTable.fp == NULL) {
    ecs_SetError(&(s->result),1,"Unable to open the LAT table");
    return &(s->result);
  }

  /* initialize the server globalRegion with the one found in the LAT file */

  if (!vrf_initRegionWithDefault(s)) {
    return &(s->result);
  }

  /* read all tile reference */

  if (!vrf_initTiling(s)) {
    return &(s->result);
  }

  s->nblayer = 0; /* no layer selected so far */
  s->currentLayer = -1;

  ecs_SetSuccess(&(s->result));
  return &(s->result);
}

/* ----------------------------------------------------------------------
 *  _dyn_DestroyServer: 
 *     
 *   Destruction of this GRASS server
 * ----------------------------------------------------------------------
 */

ecs_Result *dyn_DestroyServer(s)
    ecs_Server *s;
{
  register ServerPrivateData *spriv = s->priv;

#ifdef VRF_DEBUG
  printf("dyn_DestroyServer\n");
#endif

  /* Release all layer */
  
  vrf_releaseAllLayers(s);
  
  /* if dynamic mapset than remove directory contents */

#ifdef TESTOPENTABLE
  printf("close: spriv->catTable\n");
  printf("close: spriv->latTable\n");
#endif

  vpf_close_table(&(spriv->catTable));
  vpf_close_table(&(spriv->latTable));

  /* DAP  6/19/97 */
  if (spriv->tile != NULL) {
    int     iTile;

    for( iTile=0; iTile < spriv->nbTile; iTile++ )
    {
        if( spriv->tile[iTile].path != NULL )
            free( spriv->tile[iTile].path );
    }
    free(spriv->tile);
    spriv->tile = NULL;
  }

  s->currentLayer = -1;
  s->nblayer = 0; /* no layer selected so far */

  free(spriv);

  vrf_freePathRegex();
  vrf_free_ObjAttributeBuffer();

  ecs_SetSuccess(&(s->result));

  return &(s->result);

}

/* 
********************************************************************

FUNCTION_INFORMATION

NAME 
    dyn_SelectLayer

DESCRIPTION 
    Select a VRF layer
END_DESCRIPTION

PARAMETERS 
    INPUT 
    ecs_Server *s: Server info given by OGDI API 
END_PARAMETERS

END_FUNCTION_INFORMATION

PSEUDO-CODE

Check if a layer exist with ecs_GetLayer. If a layer exist 
Begin

    Close the current layer table if it exist
    The current layer is now the layer returned by ecs_GetLayer
    The index of this layer is set to 0
    Return a success message.

End

Close the current layer table if it exist

Set a new layer in the layer structure with ecs_SetLayer. 

Allocate the memory needed to hold private infor about the new layer

Decompose sel into library, coverage, feature class and 
expression with vrf_parsePath.

Obtain the feature and primitive table file name from the 
FCS file with vrf_getFileNameFromFcs

Open the feature table. If the feature table don`t open correctly,
return an error message.

If a joint table exist for this feature table, open it. If the
join table don't open correctly, return an error message.

Process the expression and create a table of selected features.

Set the current layer with the current layer.

Set the index of the current layer to 0 to force the object extract
to start from the first object.

Calculate the number of objects. If there is a join table, use the
length of the join table. Else, use the lenght of the feature table.

Define the current tile id to -1 to indicate to the driver there is
no tile already defined.

Allocate and initialize the VPF index with the number of objects.
Initialize the contain of this table with -1 for each argument.

Return a success message

******************************************************************** 
*/

ecs_Result *dyn_SelectLayer(s,sel)
     ecs_Server *s;
     ecs_LayerSelection *sel;
{
  char buffer[256];
  int layer,i, index_size;
  register ServerPrivateData *spriv = s->priv;
  register LayerPrivateData *lpriv;

#ifdef VRF_DEBUG
  printf("dyn_SelectLayer\n");
#endif

  /* first, try to find an existing layer with same request and family */

  if ((layer = ecs_GetLayer(s,sel)) != -1) {

  if (s->currentLayer != -1)
    _closeLayerTable(s,&(s->layer[s->currentLayer]));
#if 0
  /* Close the join table for the previous layer */

  lpriv = (LayerPrivateData *) s->layer[s->currentLayer].priv;
  if (lpriv->joinTableName != NULL) {

#ifdef TESTOPENTABLE
      printf("close lpriv->joinTable\n");
#endif

      if (lpriv->joinTable.fp == NULL) 
	vpf_close_table(&(lpriv->joinTable));
  }
#endif
    
  /* if it already exists than assign currentLayer and set index to 0 to force rewind */
    
  s->currentLayer = layer;
  s->layer[layer].index = 0;
  lpriv = (LayerPrivateData *) s->layer[layer].priv;
#if 0
  /* Open the current join table */

  if (lpriv->joinTableName != NULL) {
    sprintf(buffer,"%s/%s/%s",spriv->library,lpriv->coverage,lpriv->joinTableName);
    if (muse_access(buffer,0) == 0 ) {
#ifdef TESTOPENTABLE
	printf("open lpriv->joinTable:%s\n",buffer);
#endif
	lpriv->joinTable = vpf_open_table(buffer,disk,"rb",NULL);
	if (lpriv->joinTable.fp == NULL) {
      ecs_SetError(&(s->result),1,"Unable to open the join table");
      vpf_close_table(lpriv->featureTable);
      free(s->layer[layer].priv);
      ecs_FreeLayer(s,layer);
      return &(s->result);
    }
    }
  }
#endif
  ecs_SetSuccess(&(s->result));
  return &(s->result);
  }

  if (s->currentLayer != -1)
    _closeLayerTable(s,&(s->layer[s->currentLayer]));

  /* it did not exists so we are going to try to create it */

  if ((layer = ecs_SetLayer(s,sel)) == -1) {
    return &(s->result);
  }
	
  /* allocate memory to hold private info about this new layer */

  if ((s->layer[layer].priv = (void *) malloc(sizeof(LayerPrivateData))) == NULL) {
    ecs_SetError(&(s->result),1,"Not enough memory to allocate layer private data");
    return &(s->result);	
  }
  lpriv = (LayerPrivateData *) s->layer[layer].priv;
  lpriv->index = NULL;
  lpriv->coverage = NULL;
  lpriv->fclass = NULL;
  lpriv->expression = NULL;
  lpriv->featureTableName = NULL;
  lpriv->featureTablePrimIdName = NULL;
  lpriv->joinTableName = NULL;
  lpriv->joinTableForeignKeyName = NULL;
  lpriv->joinTableFeatureIdName = NULL;
  lpriv->primitiveTableName = NULL;
  lpriv->isTiled = spriv->isTiled;

  /* decompose sel into library, coverage, feature class and expression */

  if (!vrf_parsePath(s,lpriv,sel))  {
    free(s->layer[layer].priv);
    ecs_FreeLayer(s,layer);
    return &(s->result);	
  }

  if (stricmp(lpriv->coverage,"tileref") == 0 ||
      stricmp(lpriv->coverage,"gazette") == 0 ||
      stricmp(lpriv->coverage,"libref") == 0) {
    lpriv->isTiled = 0;
  }

  /* obtain the feature and primitive table file name from the FCS file */

  if (!vrf_getFileNameFromFcs(s,lpriv)) {
    free( lpriv->coverage );
    free( lpriv->fclass );
    free( lpriv->expression );
    free( lpriv->featureTableName );
    free( lpriv->featureTablePrimIdName );
    free( lpriv->joinTableName );
    free( lpriv->joinTableForeignKeyName );
    free( lpriv->joinTableFeatureIdName );
    free( lpriv->primitiveTableName );
    free(s->layer[layer].priv);
    ecs_FreeLayer(s,layer);   
    return &(s->result);
  }

  /* open layer */

  sprintf(buffer,"%s/%s/%s",spriv->library,lpriv->coverage,lpriv->featureTableName);
  if (muse_access(buffer,0) == 0 ) {
#ifdef TESTOPENTABLE
    printf("open spriv->featureTable:%s\n",buffer);
#endif

    lpriv->featureTable = vpf_open_table(buffer,disk,"rb",NULL);
    if (lpriv->featureTable.fp == NULL) {
      ecs_SetError(&(s->result),1,"Unable to open the feature table");
      free(s->layer[layer].priv);
      ecs_FreeLayer(s,layer);   
      return &(s->result);
    }

    if (lpriv->joinTableName != NULL) {
      sprintf(buffer,"%s/%s/%s",spriv->library,lpriv->coverage,lpriv->joinTableName);
      if (muse_access(buffer,0) == 0 ) {
#ifdef TESTOPENTABLE
	printf("open lpriv->joinTable:%s\n",buffer);
#endif
	lpriv->joinTable = vpf_open_table(buffer,disk,"rb",NULL);
	if (lpriv->joinTable.fp == NULL) {
	  ecs_SetError(&(s->result),1,"Unable to open the join table");
	  vpf_close_table(lpriv->featureTable);
	  free(s->layer[layer].priv);
	  ecs_FreeLayer(s,layer);   
	  return &(s->result);
	}
      }

      /*
	Check if Tile_ID is defined in the join table for tiled datasets, if
        not, it's an attribute join.

        There is no apparent way of identifying whether this is an
        attribute join or not for non-tiled datasets such as VITD which don't
        have TILE_ID, but do have feature joins so we assume in this case that
        it is a feature join.
	*/
      if (table_pos("TILE_ID",lpriv->joinTable) == -1
          && lpriv->isTiled ) {
#ifdef TESTOPENTABLE
      printf("close lpriv->joinTable:%s\n", lpriv->joinTableName);
#endif
	vpf_close_table(&(lpriv->joinTable));
	free(lpriv->joinTableName);
	lpriv->joinTableName = NULL;
      }
    }

  } else {
    free( lpriv->coverage );
    free( lpriv->fclass );
    free( lpriv->expression );
    free( lpriv->featureTableName );
    free( lpriv->featureTablePrimIdName );
    free( lpriv->joinTableName );
    free( lpriv->joinTableForeignKeyName );
    free( lpriv->joinTableFeatureIdName );
    free( lpriv->primitiveTableName );
    free(s->layer[layer].priv);
    ecs_FreeLayer(s,layer);
    ecs_SetError(&(s->result),1,"Can't open this feature class");
    return &(s->result);		
  }


  lpriv->feature_rows = query_table2(lpriv->expression,lpriv->featureTable);

  /* process result */

  s->currentLayer = layer;
  s->layer[layer].index = 0;

  lpriv->mergeFeatures = FALSE;
#ifdef VRF_LINE_JOIN_HACK
  if( s->layer[layer].sel.F == Line && lpriv->joinTableName != NULL )
      lpriv->mergeFeatures = TRUE;
#endif  

  if (lpriv->joinTableName != NULL && !lpriv->mergeFeatures)
    s->layer[layer].nbfeature = lpriv->joinTable.nrows;
  else
    s->layer[layer].nbfeature = lpriv->featureTable.nrows;

  lpriv->current_tileid = -1;

  if( lpriv->joinTableName != NULL )
      index_size = lpriv->joinTable.nrows + 1;
  else
      index_size = lpriv->featureTable.nrows + 1;

  lpriv->index = (VRFIndex *) malloc(sizeof(VRFIndex) * index_size);

  for (i=0; i < index_size; ++i) {
    lpriv->index[i].prim_id = -1;
  }

  ecs_SetSuccess(&(s->result));
  return &(s->result);
}


/* ----------------------------------------------------------------------
 *  dyn_ReleaseLayer: 
 *     
 *      deselect a layer
 * ----------------------------------------------------------------------
 */

ecs_Result *dyn_ReleaseLayer(s,sel)
     ecs_Server *s;
     ecs_LayerSelection *sel;
{
  int layer;
  char buffer[128];
  register LayerPrivateData *lpriv;

#ifdef VRF_DEBUG
  printf("dyn_ReleaseLayer\n");
#endif

  /* first, try to find an existing layer with same request and family */

  if ((layer = ecs_GetLayer(s,sel)) == -1) {
    sprintf(buffer,"Invalid layer %s",sel->Select);
    ecs_SetError(&(s->result),1,buffer);
    return &(s->result);
  }

  lpriv = (LayerPrivateData *) s->layer[layer].priv;

  if (s->currentLayer != -1) {
    _closeLayerTable(s,&(s->layer[s->currentLayer]));
    s->currentLayer = -1;
  }
	
#ifdef TESTOPENTABLE
  printf("close: spriv->featureTable\n");
  printf("close: spriv->fcsTable\n");
#endif

  if (lpriv->joinTableName != NULL) {
#ifdef TESTOPENTABLE
    printf("close lpriv->joinTable:%s\n", lpriv->joinTableName);
#endif
    vpf_close_table(&(lpriv->joinTable));
  }
  vpf_close_table(&(lpriv->featureTable));
  vpf_close_table(&(lpriv->fcsTable));

  set_nuke(&(lpriv->feature_rows));

  if (lpriv->joinTableFeatureIdName) {
    free(lpriv->joinTableFeatureIdName);
    lpriv->joinTableFeatureIdName = NULL;
  }
  if (lpriv->joinTableForeignKeyName) {
    free(lpriv->joinTableForeignKeyName);
    lpriv->joinTableForeignKeyName = NULL;
  }
  if (lpriv->joinTableName) {
    free(lpriv->joinTableName);
    lpriv->joinTableName = NULL;
  }
  free(lpriv->coverage);
  free(lpriv->fclass);
  free(lpriv->expression);
  free(lpriv->featureTableName);
  free(lpriv->primitiveTableName);
  free(lpriv->featureTablePrimIdName);
  free(lpriv->index);
  free(lpriv);

  ecs_FreeLayer(s,layer);

  ecs_SetSuccess(&(s->result));
  return &(s->result);
}

/* deselect all layer */

void
vrf_releaseAllLayers(s)
     ecs_Server *s;
{
    int i;

    /* count down since nblayer will change as we go */
    for (i = s->nblayer-1; i >= 0; i--)
        dyn_ReleaseLayer(s,&(s->layer[i].sel));
}

/* ----------------------------------------------------------------------
 *  dyn_SelectRegion: 
 *     
 *      selection current geographic region.
 * ----------------------------------------------------------------------
 */


ecs_Result *dyn_SelectRegion(s,gr)
     ecs_Server *s;
     ecs_Region *gr;
{
  register ServerPrivateData *spriv = (ServerPrivateData *) s->priv;
  int i;	

#ifdef VRF_DEBUG
  printf("dyn_SelectRegion\n");
#endif
  
  s->currentRegion.north = gr->north;
  s->currentRegion.south = gr->south;
  s->currentRegion.east = gr->east;			
  s->currentRegion.west = gr->west;
  s->currentRegion.ns_res = gr->ns_res;
  s->currentRegion.ew_res = gr->ew_res;
  
  /* reset currentLayer index to 0 to force rewind */
  
  if (s->currentLayer != -1) {
    s->layer[s->currentLayer].index = 0;
  }

#ifdef VRF_DEBUG
  printf("nbtile: %d\n",spriv->nbTile);
#endif
  
  
  for(i = 0; i < spriv->nbTile; ++i) {
    if (spriv->isTiled) {
      if (vrf_IsOutsideRegion(spriv->tile[i].ymax,
			      spriv->tile[i].ymin,
			      spriv->tile[i].xmax,
			      spriv->tile[i].xmin,
			      &(s->currentRegion))) {
	spriv->tile[i].isSelected = 0;
      } else {
	spriv->tile[i].isSelected = 1;
      }
    }
  }
  
  ecs_SetSuccess(&(s->result));
  return &(s->result);	
}

/* ----------------------------------------------------------------------
 *  dyn_GetDictionary: 
 *     
 *      return the itcl_class object 
 * ----------------------------------------------------------------------
 */

ecs_Result *dyn_GetDictionary(s)
     ecs_Server *s;
{

  ecs_SetText(&(s->result),datadict);
  ecs_SetSuccess(&(s->result));
 
  return &(s->result);
}

/* ----------------------------------------------------------------------
 *  dyn_GetAttributesFormat: 
 *     
 *      return the attribute format of the currently selected layer
 * ----------------------------------------------------------------------
 */



ecs_Result *dyn_GetAttributesFormat(s)
     ecs_Server *s;
{
  int i;
  register LayerPrivateData *lpriv;
  ecs_AttributeFormat type=0;
  int length=0;
  int precision=0;

  lpriv = (LayerPrivateData *) s->layer[s->currentLayer].priv;

  ecs_SetObjAttributeFormat(&(s->result));

  /* the code to retreive attribute format of a specific layer is inserted here */

  for(i = 0; i < lpriv->featureTable.nfields; ++i) {

    switch(lpriv->featureTable.header[i].type) {

    case 'T':
    case 'L':
      length = lpriv->featureTable.header[i].count;
      if (length == -1) {
	type = Varchar;
	length = 0;
      }	
      else
	type = Char;
      precision = 0;
      break;

    case 'F':
      type = Float;
      length = 15;
      precision = 6;
      break;

    case 'R':
      type = Double;
      length = 25;
      precision = 12;
      break;

    case 'D':
      type = Char;
      length = 20;
      precision = 0;
      break;      

    case 'S':
      type = Smallint;
      length = 6;
      precision = 0;
      break;

    case 'I':
      type = Integer;
      length = 10;
      precision = 0;
      break;

    }

    ecs_AddAttributeFormat(&(s->result), lpriv->featureTable.header[i].name, type, length, precision, 0);
  }	

	
  ecs_SetSuccess(&(s->result));

  return &(s->result);
}

/* ----------------------------------------------------------------------
 *  dyn_GetNextObject: 
 *     
 *      return the next object for the current layer
 * ----------------------------------------------------------------------
 */

ecs_Result *dyn_GetNextObject(s)
     ecs_Server *s;
{
  (layerMethod[s->layer[s->currentLayer].sel.F].getNextObject)(s,&(s->layer[s->currentLayer]));
  return &(s->result);

}


/* ----------------------------------------------------------------------
 *  dyn_GetObject: 
 *     
 *      return an object for the current layer
 * ----------------------------------------------------------------------
 */

ecs_Result *dyn_GetObject(s,Id)
     ecs_Server *s;
     char *Id;
{

  (layerMethod[s->layer[s->currentLayer].sel.F].getObject)(s,&(s->layer[s->currentLayer]),Id);
  return &(s->result);
}

/* ----------------------------------------------------------------------
 *  dyn_GetObjectIdFromCoord: 
 *     
 *      return the object id sitting at (or near) to a coordinate 
 * ----------------------------------------------------------------------
 */

ecs_Result *dyn_GetObjectIdFromCoord(s,coord)
     ecs_Server *s;
     ecs_Coordinate *coord;
{
  (layerMethod[s->layer[s->currentLayer].sel.F].getObjectIdFromCoord)(s,&(s->layer[s->currentLayer]),coord);
  return &(s->result);
}

/* ----------------------------------------------------------------------
 *  _dyn_UpdateDictionary: 
 *     
 *   Return the content of this location data dictionary in a Tcl List
 * ----------------------------------------------------------------------
 */


ecs_Result *dyn_UpdateDictionary(s,arg)
     ecs_Server *s;
     char *arg;
{
    int i,count;
    row_type row;
    char *coverage;
    char *description;
    register ServerPrivateData *spriv = (ServerPrivateData *) s->priv;

    /* Get all the metadata and store them in a string*/

    if (spriv->isMetaLoad == FALSE) {
        if (!vrf_GetMetadata(s)) {
            return &(s->result);
        } 
        spriv->isMetaLoad = TRUE;
    }

    if ((arg == NULL) || (strcmp(arg,"") == 0)) 
    {
	 
        ecs_SetText(&(s->result)," ");
        ecs_AddText(&(s->result),spriv->metadatastring);
    } 
    else if (strcmp(arg,"ogdi_capabilities") == 0 
             || strcmp(arg,"ogdi_server_capabilities") == 0 )
    {
        if( !vrf_build_capabilities( s, arg ) )
            return &(s->result);
    }
    else if ((strncmp(arg,"cat_list",8)) == 0) 
    {
        ecs_SetText(&(s->result)," ");
        for (i = 1; i <= spriv->catTable.nrows; ++i) 
        {
				
      
            row = get_row(i, spriv->catTable);
            coverage = justify( (char *) get_table_element(1, row, spriv->catTable, NULL, &count));
            description = justify( (char *) get_table_element(2, row, spriv->catTable, NULL, &count));
 
            free_row(row, spriv->catTable);
            /*	      if (strcmp(coverage,"libref") != 0 && strcmp(coverage,"tileref") != 0) {*/
            ecs_AddText(&(s->result),"{ ");
            ecs_AddText(&(s->result),coverage);	
            ecs_AddText(&(s->result)," {");
            ecs_AddText(&(s->result),description);
            ecs_AddText(&(s->result),"}");
            vrf_AllFClass(s,coverage);	
            ecs_AddText(&(s->result),"} ");
            /*	      }*/
            free(coverage);
            free(description); 
        }	
    } else {	
        if (!vrf_feature_class_dictionary(s,arg))
            return &(s->result);	 
    }  
 
    ecs_SetSuccess(&(s->result));
    return &(s->result); 

}



/* ----------------------------------------------------------------------
 *  _dyn_GetServerProjection: 
 *     
 *   Return this server cartographic projection
 * ----------------------------------------------------------------------
 */

ecs_Result *dyn_GetServerProjection(s)
     ecs_Server *s;
{
  ecs_SetText(&(s->result), "+proj=longlat +datum=wgs84");
  ecs_SetSuccess(&(s->result));
  return &(s->result);
}


/* ----------------------------------------------------------------------
 *  _dyn_GetGlobalBound: 
 *     
 *   Return this server global bounding region
 * ----------------------------------------------------------------------
 */


ecs_Result *dyn_GetGlobalBound(s)
     ecs_Server *s;
{
  ecs_SetGeoRegion(&(s->result),s->globalRegion.north, s->globalRegion.south, 
		   s->globalRegion.east, s->globalRegion.west, s->globalRegion.ns_res, 
		   s->globalRegion.ew_res);
  ecs_SetSuccess(&(s->result));
  return &(s->result);
}

/* ----------------------------------------------------------------------
 *  _dyn_SetServerLanguage: 
 *     
 *   Set this server lnaguage for error message; not yet implemented
 * ----------------------------------------------------------------------
 */

ecs_Result *dyn_SetServerLanguage(s,language)
     ecs_Server *s;
     u_int language;
{
  (void) language;
  ecs_SetSuccess(&(s->result));
  return &(s->result);
}

/* ----------------------------------------------------------------------
 *  _dyn_SetCompression: 
 *     
 *   No compression used in local databases.
 * ----------------------------------------------------------------------
 */

ecs_Result *dyn_SetCompression(s,compression)
     ecs_Server *s;
     ecs_Compression *compression;
{
  (void) compression;
  ecs_SetSuccess(&(s->result));
  return &(s->result);
}
