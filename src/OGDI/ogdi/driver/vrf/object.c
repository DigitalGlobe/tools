/******************************************************************************
 *
 * Component: OGDI VRF Driver
 * Purpose: Implementation of vrf Server getObject* functions
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
 * $Log: object.c,v $
 * Revision 1.12  2016/07/06 09:00:39  erouault
 * VRF: implement ecs_SetErrorShouldStop() logic in the various _getNext.... methods
 *
 * Revision 1.11  2016/07/04 17:03:12  erouault
 * Error handling: Add a ecs_SetErrorShouldStop() function that can be
 *     used internally when the code is able to recover from an error. The user
 *     may decide if he wants to be resilient on errors by defining OGDI_STOP_ON_ERROR=NO
 *     as environment variable (the default being YES: stop on error).
 *     Add a ecs_SetReportErrorFunction() method to install a custom callback that
 *     will be called when OGDI_STOP_ON_ERROR=YES so that the user code is still
 *     aware of errors that occured. If not defined, the error will be logged in stderr.
 *
 * Revision 1.10  2016/07/04 14:34:40  erouault
 * VPF: _getNextObject / _getObject functions: validate the value of the tile_id to avoid a potential out-of-bounds read. Fix crash on dqyarea@dqy layer of DNC17/H1708311
 *
 * Revision 1.9  2007/05/09 20:46:28  cbalint
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
 * Revision 1.8  2004/10/19 14:17:03  warmerda
 * primList leak fixed in vrf driver
 *
 * Revision 1.7  2001/08/16 20:40:34  warmerda
 * applied VITD fixes - merge primitive lines into a feature
 *
 * Revision 1.6  2001/06/21 20:30:15  warmerda
 * added ECS_CVSID
 *
 * Revision 1.5  2001/06/13 17:33:59  warmerda
 * upgraded source headers
 *
 */

#include "ecs.h"
#include "vrf.h"

ECS_CVSID("$Id: object.c,v 1.12 2016/07/06 09:00:39 erouault Exp $");

/* 
********************************************************************

FUNCTION_INFORMATION

NAME 
    _getTileAndPrimId

DESCRIPTION 
    With a object id, this function return the feature id
    of the object, and the tile id and primitive id related
    to it.
END_DESCRIPTION

PARAMETERS 

    INPUT 
    ecs_Server *s: Server info given by OGDI API 
    ecs_Layer *l: The current layer
    int32 object_id: The object id
    OUTPUT
    int32 *feature_id: The feature id
    short *tile_id: The tile id of the primitive object
    int32 *prim_id: The primitive id related to the feature

END_PARAMETERS

END_FUNCTION_INFORMATION

PSEUDO-CODE

Set tile_id, feature_id and prim_id to -1.

If the layer is not tiled (the primitives are in tiles)
Begin

    Set tile_id to 1

End

If the primitive id at the position object_id is negative (not
calculated yet) 
Begin
    If a join table exist (the joinTableName is valid)
    Begin

       // 1:n relation, get the feature id in the join table

       Get the row object id in the join table

       If a join table feature id row don't exist in the join table
       Begin
          Set the feature table with object id (relation 1:1).
       End
       Else
       Begin
	  Get the position of the feature attribute in the join
	  table (with the join table feature id name). Read the
	  content of the join table at this position and set
	  feature_id.
       End

       If the tile_id is -1
       Begin
	  
          Get the position of the tile id attribute in the
	  join table. Read the content of the join table at
	  this position and set tile_id.

       End

       Get the position of the prim id attribute in the join
       table (with featureTablePrimIdName). Read the content
       of the join table at this position and set prim_id.
       
       Free the row in the join table.

    End
    Else
    Begin

        // 1:1 relation without a join table.

        Set the feature id with object id.

        Get the row object id in the feature table.

        If the tile_id is -1
        Begin

            Get the position of the tile id attribute in the feature
            table. Read the content of the feature table at this
            position and set tile_id.

        End

        Get the position of the prim id attribute in the feature table
        (with featureTablePrimIdName). Read the content of the
        feature table at this position and set prim_id.

        Free the row in the feature table.

    End

    Set the index at the position object_id with the feature_id, the
    tile_id and the prim_id.

End

******************************************************************** 
*/

void
_getTileAndPrimId(s,l,object_id,feature_id,tile_id,prim_id)
     ecs_Server *s;
     ecs_Layer *l;
     int32 object_id;
     int32 *feature_id;
     short *tile_id;
     int32 *prim_id;
{
  int32 count;
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  row_type row,join_row;
  int pos;

  (void) s;

  *tile_id = -1;
  *prim_id = -1;
  *feature_id = -1;

  if (!lpriv->isTiled) {
    *tile_id = 1;
  }

  if (lpriv->index[object_id].prim_id == -1) {
    if ((lpriv->joinTableName != NULL) && 
	(*tile_id != -1 || (table_pos("TILE_ID",lpriv->joinTable) != -1)) &&
	(table_pos(lpriv->featureTablePrimIdName,lpriv->joinTable) != -1)) {
      join_row = get_row(object_id+1, lpriv->joinTable);
      
      if (lpriv->joinTableFeatureIdName == NULL) {
	*feature_id = object_id+1;
      } else {
	pos = table_pos(lpriv->joinTableFeatureIdName,lpriv->joinTable);
	if (pos != -1) {
	  get_table_element(pos, join_row, lpriv->joinTable, feature_id, &count);
	} else {
	  return;
	}
      }

      if (*tile_id != 1) {
	pos = table_pos("TILE_ID",lpriv->joinTable);
	if (pos != -1) {
	  /* DAP TR326 */
	  if (lpriv->joinTable.nrows <= 0) {
	    *tile_id = -2;
	    *prim_id = -1;
	    return;
	  } else {
	    get_table_element(pos,join_row, lpriv->joinTable, tile_id, &count);
	  }
	} else {
	  return;
	}
      }

      pos = table_pos(lpriv->featureTablePrimIdName,lpriv->joinTable);
      if (pos != -1) {
	get_table_element(pos,join_row, lpriv->joinTable, prim_id, &count);
      } else {
	*feature_id = -1;
	*tile_id = -1;
	return;	  
      }

      free_row(join_row,lpriv->joinTable);
    } else {
      row = get_row(object_id+1, lpriv->featureTable);
      *feature_id = object_id+1;
      if (*tile_id != 1) {
        pos = table_pos("TILE_ID",lpriv->featureTable);
        if (pos != -1) {
          get_table_element(pos, row, lpriv->featureTable, tile_id, &count);
        } else {
          free_row(row, lpriv->featureTable);
          return;
        }
      }
      pos = table_pos(lpriv->featureTablePrimIdName,lpriv->featureTable);
      if (pos != -1) {
        get_table_element(pos, row, lpriv->featureTable, prim_id, &count);
      } else {
        free_row(row, lpriv->featureTable);
        return;
      }
      free_row(row, lpriv->featureTable);
    }
    lpriv->index[object_id].feature_id = *feature_id;
    lpriv->index[object_id].tile_id = *tile_id;
    lpriv->index[object_id].prim_id = *prim_id;
  } else {
    *feature_id = lpriv->index[object_id].feature_id;
    *tile_id = lpriv->index[object_id].tile_id;
    *prim_id = lpriv->index[object_id].prim_id;
  }
}

/*
 *  --------------------------------------------------------------------------
 *  _getPrimList()
 *   
 *      Build list of primitives joined to the same feature.
 *	This function assumes that all the primitives for a given feature
 *      will occur together in the join table.  While this appears to be
 *      true of test datasets, it might not be true in general.  Eventually
 *      an efficient for relating a feature id with it's list of primitives
 *      should be build, and maintained over the access to this join table
 *      if this is to be safe.
 *
 *      Note the object_id passed to this function is supposed to be the
 *      row number of the first primitive in the join table when using
 *      join tables and merging features.  
 *  --------------------------------------------------------------------------
 */

void _getPrimList( ecs_Server *s, 
                   ecs_Layer *l, 
                   int32 object_id,
                   int32 *feature_id,
                   short *tile_id,
                   int32 *primCount,
                   int32 **primList,
                   int32 *next_index )

{
    LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
    int32	edg_id;
    int		maxCount = 0;

    /*
      Get the first primitive for this feature.
      */
    
    _getTileAndPrimId(s,l,object_id,feature_id,tile_id,&edg_id);
    object_id++;

    /*
      If we aren't operating in merged format, just return this primiitive
      */
    maxCount = 1;
    *primCount = 1;
    *primList = (int32 *) malloc(sizeof(int32) * maxCount);
    (*primList)[0] = edg_id;

    if( !lpriv->mergeFeatures )
    {
        *next_index = object_id;
        return;
    }
    
    /*
      Collect all other primitives with the same line id.  Note we are
      incrementing the global index value.
      */

    while( object_id < lpriv->joinTable.nrows )
    {
        int32	this_feature_id;
        short	this_tile_id;
        
        _getTileAndPrimId(s,l,object_id,
                          &this_feature_id,&this_tile_id,&edg_id);

        if( this_feature_id != *feature_id )
            break;

        /*
          This primitive matches our feature_id, add to the list.
          */

        if( *primCount == maxCount )
        {
            int32* newPrimList;
            maxCount += 100;
            newPrimList = (int32*) realloc(*primList, sizeof(int32) * maxCount);
            if( newPrimList == NULL )
            {
                /* Should probably error out loudly */
                free( *primList );
                *primList = NULL;
                *primCount = 0;
                object_id++;
                break;
            }
            *primList = newPrimList;
        }

        (*primList)[*primCount] = edg_id;
        (*primCount)++;
        
        object_id++;
    }

    *next_index = object_id;
}

/*
 *  --------------------------------------------------------------------------
 *  _getPrimListByFeatureId()
 *   
 *      Build list of primitives joined to the same feature based on the
 *      feature id as a key.  This can be kind of slow since the join
 *      table is scanned linearly. 
 *  --------------------------------------------------------------------------
 */

static void
_getPrimListByFeatureId( ecs_Server *s, 
                         ecs_Layer *l, 
                         int32 object_id, /* this should be feature id */
                         short *tile_id,
                         int32 *primCount,
                         int32 **primList,
                         int32 *next_index )

{
    register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
    int		edgeCount, edgeId;
    
    if( lpriv->mergeFeatures )
        edgeCount = lpriv->joinTable.nrows;
    else
        edgeCount = l->nbfeature;

    for( edgeId = 0; edgeId < edgeCount; edgeId++ )
    {
        int32	prim_id, this_feature_id;
        
        _getTileAndPrimId( s, l, edgeId, &this_feature_id, tile_id, &prim_id );

        if( object_id == this_feature_id )
        {
            _getPrimList( s, l, edgeId, &this_feature_id, tile_id,
                          primCount, primList, next_index );
            return;
        }
    }

    *primCount = 0;
    *primList = NULL;
}

/*
 *  --------------------------------------------------------------------------
 *  _get*Object*Area: 
 *   
 *      a set of functions to acheive Area objects retrieval
 *  --------------------------------------------------------------------------
 */

void 
_getNextObjectArea(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  char buffer[256];
  register ServerPrivateData *spriv = (ServerPrivateData *) s->priv;
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  short tile_id;
  int32 area_id;
  int32 fac_id;
  int found = 0;
  char *temp;
  double xmin, xmax, ymin, ymax;

retry:
  while(!found && l->index < l->nbfeature) {    
    _getTileAndPrimId(s,l,l->index,&area_id,&tile_id, &fac_id);

    /* 
       Check the case of the polygon with all the database polygon as
       island.  The case only appear with DCW
       */
    
    if ((l->index == 0) && (spriv->isDCW == TRUE)) {
      l->index++;
      continue;
    }

    if (set_member(area_id,lpriv->feature_rows)) {
      if (tile_id == -1) {
	if( ecs_SetErrorShouldStop(&(s->result), 1, "The VRF tiles are badly defined") )
          return;
        l->index++;
        continue;
      }

      if (tile_id == -2) {
	if( ecs_SetErrorShouldStop(&(s->result), 1, "The join table is empty") )
          return;
        l->index++;
        continue;
      }
      
      if( lpriv->isTiled && (tile_id < 1 || tile_id > spriv->nbTile) )
      {
	/* Happens with dqyarea@dqy(*) coverage of DNC17/H1708311 */
	char szErrorMsg[128];
	sprintf(szErrorMsg, "Object index=%d references incorrect tile_id=%d (nbTile=%d)",
	    l->index, tile_id, spriv->nbTile);
	if( ecs_SetErrorShouldStop(&(s->result), 1, szErrorMsg) )
	    return;
        l->index++;
        continue;
      }
      else
      if (lpriv->isTiled == 0 || spriv->tile[tile_id-1].isSelected) {
	_selectTileArea(s,l,tile_id);
	if (!vrf_get_area_mbr(l,fac_id,&xmin,&ymin,&xmax,&ymax)) {
	  if( ecs_SetErrorShouldStop(&(s->result), 1, "VRF table mbr not open") )
            return;
          l->index++;
          continue;
	}
	if (!vrf_IsOutsideRegion(ymax,ymin,xmax,xmin,
			      &(s->currentRegion))) {
	  found = 1;
	  break;
	}
      } 
    }
    l->index++;
  }

  /* if a feature is found, get the feature info */

  if (found) {
    if (!vrf_get_area_feature(s,l,fac_id)) 
    {
        if( !ecs_ShouldStopOnError() )
        {
            char* message= strdup(s->result.message);
            int should_stop;
            ecs_CleanUp(&(s->result));
            should_stop = ecs_SetErrorShouldStop(&(s->result),1,message);
            free(message);
            if( !should_stop )
            {
                found = 0;
                l->index++;
                goto retry;
            }
        }
        return;
    }
    l->index++;
  } else {
    ecs_SetError(&(s->result),2,"End of selection");
    return;
  }

  /* Add the identifier to the object */

  sprintf(buffer,"%d",(int) area_id);
  ecs_SetObjectId(&(s->result),buffer);

  /* Add the attributes to the object */

  temp =vrf_get_ObjAttributes(lpriv->featureTable, area_id);
  if (temp != NULL)
    ecs_SetObjectAttr(&(s->result),temp);
  else 
    ecs_SetObjectAttr(&(s->result),"");

  /* Add the bounding box to the object */

  ECS_SETGEOMBOUNDINGBOX((&(s->result)),xmin,ymin,xmax,ymax);

  ecs_SetSuccess(&(s->result));
}

/*************************************/

void 
_getObjectArea(s,l,id)
     ecs_Server *s;
     ecs_Layer *l;
     char *id;
{
  ServerPrivateData *spriv = (ServerPrivateData *) s->priv;
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  int object_id;
  int32 area_id;
  short tile_id;
  int32 fac_id;
  double xmin, xmax, ymin, ymax;
  char *temp;

  object_id = atoi(id);

  if (object_id > l->nbfeature || object_id < 0) {
    ecs_SetError(&(s->result),1,"Invalid area id");
    return;
  }

  _getTileAndPrimId(s,l,object_id,&area_id,&tile_id, &fac_id);
  if (tile_id == -1) {
    ecs_SetError(&(s->result), 1, "The VRF tiles are badly defined");
    return;
  }
  if (tile_id == -2) {
    ecs_SetError(&(s->result), 1, "The join table is empty");
    return;
  }

  if( lpriv->isTiled && (tile_id < 1 || tile_id > spriv->nbTile) )
  {
    char szErrorMsg[128];
    sprintf(szErrorMsg, "Object index=%d references incorrect tile_id=%d (nbTile=%d)",
            l->index, tile_id, spriv->nbTile);
    if( ecs_SetErrorShouldStop(&(s->result), 1, szErrorMsg) )
      return;
  }

  _selectTileArea(s,l,tile_id);

  if (!vrf_get_area_feature(s,l,fac_id))
    return;

  /* Add the identifier to the object */

  ecs_SetObjectId(&(s->result),id);

  if (vrf_get_area_mbr(l,fac_id,&xmin,&ymin,&xmax,&ymax)) {
    ECS_SETGEOMBOUNDINGBOX((&(s->result)),xmin,ymin,xmax,ymax);
  } else {
    ecs_SetError(&(s->result), 1, "VRF table mbr not open");
    return;
  }

  /* Add the attributes to the object */

  temp =vrf_get_ObjAttributes(lpriv->featureTable, area_id);
  if (temp != NULL)
    ecs_SetObjectAttr(&(s->result),temp);
  else 
    ecs_SetObjectAttr(&(s->result),"");

  ecs_SetSuccess(&(s->result));
}

/*************************************/

void 
_getObjectIdArea(s,l,coord)
     ecs_Server *s;
     ecs_Layer *l;
     ecs_Coordinate *coord;
{
  char buffer[256];
  register ServerPrivateData *spriv = (ServerPrivateData *) s->priv;
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  short tile_id;
  int32 fac_id;
  int feature_id;
  int32 area_id;
  double xmin, xmax, ymin, ymax;
  int32 index;
  double distance,result;

  distance = HUGE_VAL;
  feature_id = -1;

  for(index = 0; index < l->nbfeature; index++) {
    _getTileAndPrimId(s,l,index,&area_id,&tile_id, &fac_id);
    if (set_member(area_id,lpriv->feature_rows)) {
      if (tile_id == -1) {
	ecs_SetError(&(s->result), 1, "The VRF tiles are badly defined");
	return;
      }
      if (tile_id == -2) {
	ecs_SetError(&(s->result), 1, "The join table is empty");
	return;
      }

      if( lpriv->isTiled && (tile_id < 1 || tile_id > spriv->nbTile) )
      {
        char szErrorMsg[128];
        sprintf(szErrorMsg, "Object index=%d references incorrect tile_id=%d (nbTile=%d)",
                l->index, tile_id, spriv->nbTile);
        if( ecs_SetErrorShouldStop(&(s->result), 1, szErrorMsg) )
          return;
      }
      else
      if (!(lpriv->isTiled) || 
	  ((coord->x > spriv->tile[tile_id-1].xmin) && 
	   (coord->x < spriv->tile[tile_id-1].xmax) && 
	   (coord->y > spriv->tile[tile_id-1].ymin) && 
	   (coord->y < spriv->tile[tile_id-1].ymax))) {
	
	_selectTileArea(s,l,tile_id);
	if (!vrf_get_area_mbr(l,fac_id,&xmin,&ymin,&xmax,&ymax)) {
	  ecs_SetError(&(s->result), 1, "VRF table mbr not open");
	  return;
	}
	if ((coord->x>xmin) && (coord->x<xmax) && 
	    (coord->y>ymin) && (coord->y<ymax)) {
	  if (!vrf_get_area_feature(s,l,fac_id)) 
	    return;
	  
	  result = ecs_DistanceObjectWithTolerance((&(ECSOBJECT((&(s->result))))),
				      coord->x, coord->y);
	  if (result < distance) {
	    distance = result;
	    feature_id = index;
	  }
	}
      }
    }
  }

  if (feature_id < 0) {
    ecs_SetError(&(s->result),1,"Can't find any area at this location");
    return;
  }

  sprintf(buffer,"%d",feature_id);
  ecs_SetText(&(s->result),buffer);
  ecs_SetSuccess(&(s->result));
}

/*************************************/

void
_selectTileArea(s,l,tile_id)
     ecs_Server *s;
     ecs_Layer *l;
     int tile_id;
{
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  register ServerPrivateData *spriv = (ServerPrivateData *) s->priv;
  char buffer[256];

  if (lpriv->isTiled) {
    if (lpriv->current_tileid != tile_id) {
      if (lpriv->current_tileid != -1) {
	/* fermeture des tables ouvertes precedemment */

#ifdef TESTOPENTABLE
	printf("close lpriv->l.area.faceTable\n");
	printf("close lpriv->l.area.edgeTable\n");
	printf("close lpriv->l.area.ringTable\n");
	printf("close lpriv->l.area.mbrTable\n");
#endif
  
	vpf_close_table(&(lpriv->l.area.faceTable));
	vpf_close_table(&(lpriv->l.area.ringTable));
	vpf_close_table(&(lpriv->l.area.edgeTable));
	vpf_close_table(&(lpriv->l.area.mbrTable));
      }
      /* ouverture des tables de primitives dans la bonne tuile */

#ifdef TESTOPENTABLE
      printf("open lpriv->l.area.faceTable\n");
      printf("open lpriv->l.area.edgeTable\n");
      printf("open lpriv->l.area.ringTable\n");
      printf("open lpriv->l.area.mbrTable\n");
#endif
  
      if (tile_id != 0) {
	sprintf(buffer,"%s/%s/%s/fac",spriv->library,lpriv->coverage,
		spriv->tile[tile_id-1].path);
	if (muse_access(buffer,0) != 0 ) {
	  sprintf(buffer,"%s/%s/%s/FAC",spriv->library,lpriv->coverage,
		  spriv->tile[tile_id-1].path);
	}
	lpriv->l.area.faceTable = vpf_open_table(buffer, disk, "rb", NULL);
	sprintf(buffer,"%s/%s/%s/edg",spriv->library,lpriv->coverage,
		spriv->tile[tile_id-1].path);
	if (muse_access(buffer,0) != 0 ) {
	  sprintf(buffer,"%s/%s/%s/EDG",spriv->library,lpriv->coverage,
		  spriv->tile[tile_id-1].path);
	}
	lpriv->l.area.edgeTable = vpf_open_table(buffer, disk, "rb", NULL);
	sprintf(buffer,"%s/%s/%s/rng",spriv->library,lpriv->coverage,
		spriv->tile[tile_id-1].path);
	if (muse_access(buffer,0) != 0 ) {
	  sprintf(buffer,"%s/%s/%s/RNG",spriv->library,lpriv->coverage,
		  spriv->tile[tile_id-1].path);
	}
	lpriv->l.area.ringTable = vpf_open_table(buffer, disk, "rb", NULL);
	sprintf(buffer,"%s/%s/%s/fbr",spriv->library,lpriv->coverage,
		spriv->tile[tile_id-1].path);
	if (muse_access(buffer,0) != 0 ) {
	  sprintf(buffer,"%s/%s/%s/FBR",spriv->library,lpriv->coverage,
		  spriv->tile[tile_id-1].path);
	}
	lpriv->l.area.mbrTable = vpf_open_table(buffer, disk, "rb", NULL);
      } else {
	sprintf(buffer,"%s/%s/%s",spriv->library,lpriv->coverage,lpriv->primitiveTableName);
	lpriv->l.area.faceTable = vpf_open_table(buffer, disk, "rb", NULL);
	sprintf(buffer,"%s/%s/edg",spriv->library,lpriv->coverage);
	if (muse_access(buffer,0) != 0 ) {
	  sprintf(buffer,"%s/%s/EDG",spriv->library,lpriv->coverage);
	}
	lpriv->l.area.edgeTable = vpf_open_table(buffer, disk, "rb", NULL);
	sprintf(buffer,"%s/%s/rng",spriv->library,lpriv->coverage);
	if (muse_access(buffer,0) != 0 ) {
	  sprintf(buffer,"%s/%s/RNG",spriv->library,lpriv->coverage);
	}
	lpriv->l.area.ringTable = vpf_open_table(buffer, disk, "rb", NULL);
	sprintf(buffer,"%s/%s/fbr",spriv->library,lpriv->coverage);
	if (muse_access(buffer,0) != 0 ) {
	  sprintf(buffer,"%s/%s/FBR",spriv->library,lpriv->coverage);
	}
	lpriv->l.area.mbrTable = vpf_open_table(buffer, disk, "rb", NULL);
      }

      lpriv->current_tileid = tile_id;
    }
  } else {
    if (lpriv->current_tileid == -1) {

      /* ouverture des tables de primitives non-tuilees */

#ifdef TESTOPENTABLE
      printf("open lpriv->l.area.faceTable\n");
      printf("open lpriv->l.area.edgeTable\n");
      printf("open lpriv->l.area.ringTable\n");
      printf("open lpriv->l.area.mbrTable\n");
#endif

      sprintf(buffer,"%s/%s/%s",spriv->library,lpriv->coverage,lpriv->primitiveTableName);
      lpriv->l.area.faceTable = vpf_open_table(buffer, disk, "rb", NULL);
      sprintf(buffer,"%s/%s/edg",spriv->library,lpriv->coverage);
      if (muse_access(buffer,0) != 0 ) {
	sprintf(buffer,"%s/%s/EDG",spriv->library,lpriv->coverage);
      }
      lpriv->l.area.edgeTable = vpf_open_table(buffer, disk, "rb", NULL);
      sprintf(buffer,"%s/%s/rng",spriv->library,lpriv->coverage);
      if (muse_access(buffer,0) != 0 ) {
	sprintf(buffer,"%s/%s/RNG",spriv->library,lpriv->coverage);
      }
      lpriv->l.area.ringTable = vpf_open_table(buffer, disk, "rb", NULL);
      sprintf(buffer,"%s/%s/fbr",spriv->library,lpriv->coverage);
      if (muse_access(buffer,0) != 0 ) {
	sprintf(buffer,"%s/%s/FBR",spriv->library,lpriv->coverage);
      }
      lpriv->l.area.mbrTable = vpf_open_table(buffer, disk, "rb", NULL);

      lpriv->current_tileid = 1;			
    }	
  }

}

/*
 *  --------------------------------------------------------------------------
 *  _get*Object*Line: 
 *   
 *      a set of functions to acheive Line objects retrieval
 *  --------------------------------------------------------------------------
 */

void 
_getNextObjectLine(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  char buffer[256];
  register ServerPrivateData *spriv = (ServerPrivateData *) s->priv;
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  short tile_id;
  int32 line_id;
  int found = 0;
  char *temp;
  double xmin, xmax, ymin, ymax;
  int	edgeCount;
  int32	*primList = NULL, primCount = 0;

  if( lpriv->mergeFeatures )
      edgeCount = lpriv->joinTable.nrows;
  else
      edgeCount = l->nbfeature;

retry:
  while(!found && l->index < edgeCount) {

    if( primList != NULL )
    {
        free( primList );
        primList = NULL;
    }

    _getPrimList( s, l, l->index, &line_id, &tile_id, &primCount, &primList,
                  (int32 *) &(l->index));
    
    if (set_member(line_id,lpriv->feature_rows)) {      
      if (tile_id == -1) {
        free( primList );
        primList = NULL;
	if( ecs_SetErrorShouldStop(&(s->result), 1, "The VRF tiles are badly defined") )
          return;
        continue;
      }
      if (tile_id == -2) {
        free( primList );
        primList = NULL;
	if( ecs_SetErrorShouldStop(&(s->result), 1, "The join table is empty") )
          return;
        continue;
      }

      if( lpriv->isTiled && (tile_id < 1 || tile_id > spriv->nbTile) )
      {
        char szErrorMsg[128];
        free( primList );
        primList = NULL;
        sprintf(szErrorMsg, "Object index=%d references incorrect tile_id=%d (nbTile=%d)",
                l->index, tile_id, spriv->nbTile);
        if( ecs_SetErrorShouldStop(&(s->result), 1, szErrorMsg) )
          return;
      }
      else if (lpriv->isTiled == 0 || spriv->tile[tile_id-1].isSelected) {

	_selectTileLine(s,l,tile_id);
	if (!vrf_get_lines_mbr(l,primCount,primList,&xmin,&ymin,&xmax,&ymax)) {
          free( primList );
          primList = NULL;
	  if( ecs_SetErrorShouldStop(&(s->result),1,"Unable to open mbr") )
            return;
          continue;
	}
	if (!vrf_IsOutsideRegion(ymax,ymin,xmax,xmin,
			      &(s->currentRegion))) { 
	  found = 1;
	  break;
	} 
      }
    }
  }

  /* if a feature is found, get the feature info */

  if (found) {
    if( !vrf_get_merged_line_feature(s,l,primCount,primList) )
    {
        free( primList );
        primList = NULL;
        if( !ecs_ShouldStopOnError() )
        {
            char* message= strdup(s->result.message);
            int should_stop;
            ecs_CleanUp(&(s->result));
            should_stop = ecs_SetErrorShouldStop(&(s->result),1,message);
            free(message);
            if( !should_stop )
            {
                found = 0;
                goto retry;
            }
        }
        return;
    }

  } else {
    free( primList );
    ecs_SetError(&(s->result),2,"End of selection");
    return;
  }

  free( primList );

  /* Add the identifier to the object */

  sprintf(buffer,"%d", (int) line_id);
  ecs_SetObjectId(&(s->result),buffer);

  /* Add the bounding box to the object */

  ECS_SETGEOMBOUNDINGBOX((&(s->result)),xmin,ymin,xmax,ymax);
  
  /* Add the attributes to the object */
  
  temp =vrf_get_ObjAttributes(lpriv->featureTable, line_id);
  if (temp != NULL)
    ecs_SetObjectAttr(&(s->result),temp);
  else 
    ecs_SetObjectAttr(&(s->result),"");

  ecs_SetSuccess(&(s->result));
}

/*************************************/

void 
_getObjectLine(s,l,id)
     ecs_Server *s;
     ecs_Layer *l;
     char *id;
{

  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  int object_id;
  short tile_id;
  double xmin, xmax, ymin, ymax;
  char *temp;
  int32 primCount, *primList, next_object;

  object_id = atoi(id);

  _getPrimListByFeatureId( s, l, object_id, &tile_id,
                           &primCount, &primList, &next_object );
  if (primCount == 0) {
    ecs_SetError(&(s->result), 1,
                 "No primitives identified for this feature.");
    return;
  }
  if (tile_id == -1) {
    ecs_SetError(&(s->result), 1, "The VRF tiles are badly defined");
    return;
  }
  if (tile_id == -2) {
    ecs_SetError(&(s->result), 1, "The join table is empty");
    return;
  }
	
  _selectTileLine(s,l,tile_id);

  if (!vrf_get_merged_line_feature(s,l,primCount,primList))
    return;

  /* Add the identifier to the object */

  ecs_SetObjectId(&(s->result),id);

  if (vrf_get_lines_mbr(l,primCount,primList,&xmin,&ymin,&xmax,&ymax)) {
    ECS_SETGEOMBOUNDINGBOX((&(s->result)),xmin,ymin,xmax,ymax);
  } else {
    free( primList );
    ecs_SetError(&(s->result), 1, "VRF table mbr not open");
    return;
  }

  free( primList );

  /* Add the attributes to the object */

  temp =vrf_get_ObjAttributes(lpriv->featureTable, object_id);
  if (temp != NULL)
    ecs_SetObjectAttr(&(s->result),temp);
  else 
    ecs_SetObjectAttr(&(s->result),"");

  ecs_SetSuccess(&(s->result));
}

/*************************************/

void 
_getObjectIdLine(s,l,coord)
     ecs_Server *s;
     ecs_Layer *l;
     ecs_Coordinate *coord;
{
  char buffer[256];
  register ServerPrivateData *spriv = (ServerPrivateData *) s->priv;
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  short tile_id;
  int feature_id;
  int32 line_id, primCount, *primList;
  double xmin, xmax, ymin, ymax;
  int32 index, edgeCount;
  double distance,result;

  distance = HUGE_VAL;
  feature_id = -1;

  if( lpriv->mergeFeatures )
      edgeCount = lpriv->joinTable.nrows;
  else
      edgeCount = l->nbfeature;

  index = 0;
  while( index < edgeCount ) {
    _getPrimList( s, l, index, &line_id, &tile_id, &primCount, &primList,
                  &index);
    if (set_member(line_id,lpriv->feature_rows)) {
      if (tile_id == -1) {
	ecs_SetError(&(s->result), 1, "The VRF tiles are badly defined");
	return;
      }
      if (tile_id == -2) {
	ecs_SetError(&(s->result), 1, "The join table is empty");
	return;
      }    

      if (!(lpriv->isTiled) || 
	  ((coord->x > spriv->tile[tile_id-1].xmin) && 
	   (coord->x < spriv->tile[tile_id-1].xmax) && 
	   (coord->y > spriv->tile[tile_id-1].ymin) && 
	   (coord->y < spriv->tile[tile_id-1].ymax))) {
	
	_selectTileLine(s,l,tile_id);
	if (!vrf_get_lines_mbr(l,primCount,primList,&xmin,&ymin,&xmax,&ymax)) {
	  ecs_SetError(&(s->result), 1, "VRF table mbr not open");
	  return;
	}
	if ((coord->x>xmin) && (coord->x<xmax) && 
	    (coord->y>ymin) && (coord->y<ymax)) {
	  if (!vrf_get_merged_line_feature(s,l,primCount,primList))
	    return;
	  
	  result = ecs_DistanceObjectWithTolerance((&(ECSOBJECT((&(s->result))))),
				      coord->x, coord->y);
	  if (result < distance) {
	    distance = result;
	    feature_id = line_id;
	  }
	}
      }
    }
  }

  if (feature_id < 0) {
    ecs_SetError(&(s->result),1,"Can't find any line at this location");
    return;
  }

  sprintf(buffer,"%d",feature_id);
  ecs_SetText(&(s->result),buffer);
  ecs_SetSuccess(&(s->result));
}

/*************************************/

void
_selectTileLine(s,l,tile_id)
     ecs_Server *s;
     ecs_Layer *l;
     int tile_id;
{
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  register ServerPrivateData *spriv = (ServerPrivateData *) s->priv;
  char buffer[256];

  if (lpriv->isTiled) {
    if (lpriv->current_tileid != tile_id) {
      if (lpriv->current_tileid != -1) {
	/* fermeture des tables ouvertes precedemment */

#ifdef TESTOPENTABLE
	printf("close lpriv->l.line.mbrTable\n");
	printf("close lpriv->l.line.edgeTable\n");
#endif

	vpf_close_table(&(lpriv->l.line.edgeTable));
	vpf_close_table(&(lpriv->l.line.mbrTable));
      }

      /* 
	 ouverture des tables de primitives dans la bonne tuile 
	 */

#ifdef TESTOPENTABLE
      printf("open lpriv->l.line.edgeTable\n");
      printf("open lpriv->l.line.mbrTable\n");
#endif

      if (tile_id != 0) {
	sprintf(buffer,"%s/%s/%s/%s",spriv->library,lpriv->coverage,
		spriv->tile[tile_id-1].path,lpriv->primitiveTableName);
	lpriv->l.line.edgeTable = vpf_open_table(buffer, disk, "rb", NULL);
	sprintf(buffer,"%s/%s/%s/ebr",spriv->library,lpriv->coverage,
		spriv->tile[tile_id-1].path);
	if (muse_access(buffer,0) != 0 ) {
	  sprintf(buffer,"%s/%s/%s/EBR",spriv->library,lpriv->coverage,
		  spriv->tile[tile_id-1].path);
	}
	lpriv->l.line.mbrTable = vpf_open_table(buffer, disk, "rb", NULL);
      } else {
	sprintf(buffer,"%s/%s/%s",spriv->library,lpriv->coverage,lpriv->primitiveTableName);
	lpriv->l.line.edgeTable = vpf_open_table(buffer, disk, "rb", NULL);
	sprintf(buffer,"%s/%s/ebr",spriv->library,lpriv->coverage);
	if (muse_access(buffer,0) != 0 ) {
	  sprintf(buffer,"%s/%s/EBR",spriv->library,lpriv->coverage);
	}
	lpriv->l.line.mbrTable = vpf_open_table(buffer, disk, "rb", NULL);
      }
	
      lpriv->current_tileid = tile_id;
    }
  } else {
    if (lpriv->current_tileid == -1) {

      /* 
	 ouverture des tables de primitives non-tuilees 
	 */

#ifdef TESTOPENTABLE
      printf("open lpriv->l.line.edgeTable\n");
      printf("open lpriv->l.line.mbrTable\n");
#endif

      sprintf(buffer,"%s/%s/%s",spriv->library,lpriv->coverage,lpriv->primitiveTableName);
      lpriv->l.line.edgeTable = vpf_open_table(buffer, disk, "rb", NULL);
      sprintf(buffer,"%s/%s/ebr",spriv->library,lpriv->coverage);
      if (muse_access(buffer,0) != 0 ) {
	sprintf(buffer,"%s/%s/EBR",spriv->library,lpriv->coverage);
      }
      lpriv->l.line.mbrTable = vpf_open_table(buffer, disk, "rb", NULL);

      lpriv->current_tileid = 1;			
    }	
  }
}

/*
 *  --------------------------------------------------------------------------
 *  _get*Object*Point: 
 *   
 *      a set of functions to acheive Point objects retrieval
 *  --------------------------------------------------------------------------
 */

void 
_getNextObjectPoint(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  char buffer[256];
  register ServerPrivateData *spriv = (ServerPrivateData *) s->priv;
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  short tile_id;
  int32 point_id;
  int32 fpoint_id;
  int found = 0;
  char *temp;
  
  while(!found && l->index < l->nbfeature) {
    _getTileAndPrimId(s,l,l->index,&fpoint_id,&tile_id, &point_id);
    if (set_member(fpoint_id,lpriv->feature_rows)) {
      if (tile_id == -1) {
	if( ecs_SetErrorShouldStop(&(s->result), 1, "The VRF tiles are badly defined"))
	    return;
        l->index++;
        continue;
      }
      if (tile_id == -2) {
	if( !ecs_SetErrorShouldStop(&(s->result), 1, "The join table is empty") )
	    return;
        l->index++;
        continue;
      }

      if( lpriv->isTiled && (tile_id < 1 || tile_id > spriv->nbTile) )
      {
	char szErrorMsg[128];
	sprintf(szErrorMsg, "Object index=%d references incorrect tile_id=%d (nbTile=%d)",
	    l->index, tile_id, spriv->nbTile);
	if( ecs_SetErrorShouldStop(&(s->result), 1, szErrorMsg) )
	    return;
        l->index++;
        continue;
      }
      else
      if (lpriv->isTiled == 0 || spriv->tile[tile_id-1].isSelected) {

	_selectTilePoint(s,l,tile_id);
	if (!vrf_get_point_feature(s,l,point_id)) 
        {
          if( !ecs_ShouldStopOnError() )
          {
              char* message= strdup(s->result.message);
              int should_stop;
              ecs_CleanUp(&(s->result));
              should_stop = ecs_SetErrorShouldStop(&(s->result),1,message);
              free(message);
              if( should_stop )
                  return;
              else
              {
                  l->index++;
                  continue;
              }
          }
          else
          {
              return;
          }
        }
	if ((ECSGEOM((&(s->result))).point.c.x>s->currentRegion.west) && 
	    (ECSGEOM((&(s->result))).point.c.x<s->currentRegion.east) &&
	    (ECSGEOM((&(s->result))).point.c.y>s->currentRegion.south) && 
	    (ECSGEOM((&(s->result))).point.c.y<s->currentRegion.north)) {
	  found = 1;
	  break;
	}
      }
    }
    l->index++;
  }

  /* if a feature is found, get the feature info */
  
  if (found) {
    l->index++;
  } else {
    ecs_SetError(&(s->result),2,"End of selection");
    return;
  }

  /* Add the identifier to the object */

  sprintf(buffer,"%d",(int) point_id+1);
  ecs_SetObjectId(&(s->result),buffer);

  /* Add the bounding box to the object */

  ECS_SETGEOMBOUNDINGBOX((&(s->result)),
			 ECSGEOM((&(s->result))).point.c.x,
			 ECSGEOM((&(s->result))).point.c.y,
			 ECSGEOM((&(s->result))).point.c.x,
			 ECSGEOM((&(s->result))).point.c.y);

  /* Add the attributes to the object */

  temp =vrf_get_ObjAttributes(lpriv->featureTable, fpoint_id);
  if (temp != NULL)
    ecs_SetObjectAttr(&(s->result),temp);
  else 
    ecs_SetObjectAttr(&(s->result),"");

  ecs_SetSuccess(&(s->result));
}

/*************************************/

void 
_getObjectPoint(s,l,id)
     ecs_Server *s;
     ecs_Layer *l;
     char *id;
{
  ServerPrivateData *spriv = (ServerPrivateData *) s->priv;
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  int object_id;
  int32 point_id;
  short tile_id;
  int32 prim_id;
  char *temp;

  object_id = atoi(id);

  if (object_id > l->nbfeature || object_id < 0) {
    ecs_SetError(&(s->result),1,"Invalid point id");
    return;
  }

  _getTileAndPrimId(s,l,object_id,&point_id,&tile_id, &prim_id);
  if (tile_id == -1) {
    ecs_SetError(&(s->result), 1, "The VRF tiles are badly defined");
    return;
  }
  if (tile_id == -2) {
    ecs_SetError(&(s->result), 1, "The join table is empty");
    return;
  }

  if( lpriv->isTiled && (tile_id < 1 || tile_id > spriv->nbTile) )
  {
    char szErrorMsg[128];
    sprintf(szErrorMsg, "Object index=%d references incorrect tile_id=%d (nbTile=%d)",
            l->index, tile_id, spriv->nbTile);
    if( ecs_SetErrorShouldStop(&(s->result), 1, szErrorMsg) )
      return;
  }

  _selectTilePoint(s,l,tile_id);

  if (!vrf_get_point_feature(s,l,prim_id)) 
    return;

  /* Add the identifier to the object */

  ecs_SetObjectId(&(s->result),id);

  /* Add the attributes to the object */

  temp =vrf_get_ObjAttributes(lpriv->featureTable, point_id);
  if (temp != NULL)
    ecs_SetObjectAttr(&(s->result),temp);
  else 
    ecs_SetObjectAttr(&(s->result),"");

  ecs_SetSuccess(&(s->result));
}

/*************************************/

void 
_getObjectIdPoint(s,l,coord)
     ecs_Server *s;
     ecs_Layer *l;
     ecs_Coordinate *coord;
{
  char buffer[256];
  register ServerPrivateData *spriv = (ServerPrivateData *) s->priv;
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  short tile_id;
  int32 prim_id;
  int32 point_id;
  int feature_id;
  int32 index;
  double distance,result;

  distance = HUGE_VAL;
  feature_id = -1;

  for(index = 0; index < l->nbfeature; index++) {
    _getTileAndPrimId(s,l,index,&point_id,&tile_id, &prim_id);
    if (set_member(point_id,lpriv->feature_rows)) {
      if (tile_id == -1) {
	ecs_SetError(&(s->result), 1, "The VRF tiles are badly defined");
	return;
      }
      if (tile_id == -2) {
	ecs_SetError(&(s->result), 1, "The join table is empty");
	return;
      }      

      if( lpriv->isTiled && (tile_id < 1 || tile_id > spriv->nbTile) )
      {
        char szErrorMsg[128];
        sprintf(szErrorMsg, "Object index=%d references incorrect tile_id=%d (nbTile=%d)",
                l->index, tile_id, spriv->nbTile);
        if( ecs_SetErrorShouldStop(&(s->result), 1, szErrorMsg) )
          return;
      }
      else
      if (!(lpriv->isTiled) || 
	  ((coord->x > spriv->tile[tile_id-1].xmin) && 
	   (coord->x < spriv->tile[tile_id-1].xmax) && 
	   (coord->y > spriv->tile[tile_id-1].ymin) && 
	   (coord->y < spriv->tile[tile_id-1].ymax))) {
	
	_selectTilePoint(s,l,tile_id);
	if (!vrf_get_point_feature(s,l,prim_id)) 
	  return;
	
	result = ecs_DistanceObjectWithTolerance((&(ECSOBJECT((&(s->result))))),
				    coord->x, coord->y);
	if (result < distance) {
	  distance = result;
	  feature_id = index;
	}
      }
    }
  }

  if (feature_id < 0) {
    ecs_SetError(&(s->result),1,"Can't find any point at this location");
    return;
  }

  sprintf(buffer,"%d",feature_id);
  ecs_SetText(&(s->result),buffer);
  ecs_SetSuccess(&(s->result));
}

/*************************************/

void
_selectTilePoint(s,l,tile_id)
     ecs_Server *s;
     ecs_Layer *l;
     int tile_id;
{
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  register ServerPrivateData *spriv = (ServerPrivateData *) s->priv;
  char buffer[256];

  if (lpriv->isTiled) {
    if (lpriv->current_tileid != tile_id) {
      if (lpriv->current_tileid != -1) {
#ifdef TESTOPENTABLE
	printf("close lpriv->l.point.primTable\n");
#endif

	/* fermeture des tables ouvertes precedemment */
	vpf_close_table(&(lpriv->l.point.primTable));
      }
      /* ouverture des tables de primitives dans la bonne tuile */
#ifdef TESTOPENTABLE
      printf("open lpriv->l.point.primTable\n");
#endif

      if (tile_id != 0) {
	sprintf(buffer,"%s/%s/%s/%s",spriv->library,lpriv->coverage,
		spriv->tile[tile_id-1].path,lpriv->primitiveTableName);
	lpriv->l.point.primTable = vpf_open_table(buffer, disk, "rb", NULL);
      } else {
	sprintf(buffer,"%s/%s/%s",spriv->library,lpriv->coverage,
		lpriv->primitiveTableName);
	lpriv->l.point.primTable = vpf_open_table(buffer, disk, "rb", NULL);
      }
 
      lpriv->current_tileid = tile_id;
    }
  } else {
    if (lpriv->current_tileid == -1) {

      /* ouverture des tables de primitives non-tuilees */
#ifdef TESTOPENTABLE
      printf("open lpriv->l.point.primTable\n");
#endif

      sprintf(buffer,"%s/%s/%s",spriv->library,
	      lpriv->coverage,lpriv->primitiveTableName);
      lpriv->l.point.primTable = vpf_open_table(buffer, disk, "rb", NULL);

      lpriv->current_tileid = 1;			
    }	
  }
}

/*************************************/

/*
 *  --------------------------------------------------------------------------
 *  _get*Object*Text: 
 *   
 *      a set of functions to acheive Text objects retrieval
 *  --------------------------------------------------------------------------
 */

void 
_getNextObjectText(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  char buffer[256];
  register ServerPrivateData *spriv = (ServerPrivateData *) s->priv;
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  short tile_id;
  int32 text_id;
  int32 prim_id;
  int found = 0;
  char *temp;
  
  while(!found && l->index < l->nbfeature) {
    _getTileAndPrimId(s,l,l->index,&text_id,&tile_id, &prim_id);
    if (set_member(text_id,lpriv->feature_rows)) {
      if (tile_id == -1) {
	if( ecs_SetErrorShouldStop(&(s->result), 1, "The VRF tiles are badly defined") )
          return;
        l->index ++;
        continue;
      }
      if (tile_id == -2) {
	if( ecs_SetErrorShouldStop(&(s->result), 1, "The join table is empty") )
          return;
        l->index ++;
        continue;
      }

      if( lpriv->isTiled && (tile_id < 1 || tile_id > spriv->nbTile) )
      {
        char szErrorMsg[128];
        sprintf(szErrorMsg, "Object index=%d references incorrect tile_id=%d (nbTile=%d)",
                l->index, tile_id, spriv->nbTile);
        if( ecs_SetErrorShouldStop(&(s->result), 1, szErrorMsg) )
          return;
        l->index ++;
        continue;
      }
      else
      if (lpriv->isTiled == 0 || spriv->tile[tile_id-1].isSelected) {

	_selectTileText(s,l,tile_id);
	if (!vrf_get_text_feature(s,l,prim_id)) 
        {
          if( !ecs_ShouldStopOnError() )
          {
              char* message= strdup(s->result.message);
              int should_stop;
              ecs_CleanUp(&(s->result));
              should_stop = ecs_SetErrorShouldStop(&(s->result),1,message);
              free(message);
              if( should_stop )
                  return;
              else
              {
                  l->index ++;
                  continue;
              }
          }
          else
          {
              return;
          }
        }
	if ((ECSGEOM((&(s->result))).text.c.x>s->currentRegion.west) && 
	    (ECSGEOM((&(s->result))).text.c.x<s->currentRegion.east) &&
	    (ECSGEOM((&(s->result))).text.c.y>s->currentRegion.south) && 
	    (ECSGEOM((&(s->result))).text.c.y<s->currentRegion.north)) {
	  found = 1;
	  break;
	}
      }
    }
    l->index++;
  }

  /* if a feature is found, get the feature info */
  
  if (found) {
    l->index++;
  } else {
    ecs_CleanUp(&(s->result));
    ecs_SetError(&(s->result),2,"End of selection");
    return;
  }

  /* Add the identifier to the object */

  sprintf(buffer,"%d", (int) text_id);
  ecs_SetObjectId(&(s->result),buffer);

  /* Add the bounding box to the object */

  ECS_SETGEOMBOUNDINGBOX((&(s->result)),
			 ECSGEOM((&(s->result))).text.c.x,
			 ECSGEOM((&(s->result))).text.c.y,
			 ECSGEOM((&(s->result))).text.c.x,
			 ECSGEOM((&(s->result))).text.c.y);

  /* Add the attributes to the object */

  temp =vrf_get_ObjAttributes(lpriv->featureTable, text_id);
  if (temp != NULL)
    ecs_SetObjectAttr(&(s->result),temp);
  else 
    ecs_SetObjectAttr(&(s->result),"");

  ecs_SetSuccess(&(s->result));

}

/*************************************/

void 
_getObjectText(s,l,id)
     ecs_Server *s;
     ecs_Layer *l;
     char *id;
{
  ServerPrivateData *spriv = (ServerPrivateData *) s->priv;
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  int object_id;
  short tile_id;
  int32 prim_id;
  int32 text_id;
  char *temp;

  object_id = atoi(id);

  if (object_id > l->nbfeature || object_id < 0) {
    ecs_SetError(&(s->result),1,"Invalid text id");
    return;
  }

  _getTileAndPrimId(s,l,object_id,&text_id,&tile_id, &prim_id);	
  if (tile_id == -1) {
    ecs_SetError(&(s->result), 1, "The VRF tiles are badly defined");
    return;
  }
  if (tile_id == -2) {
    ecs_SetError(&(s->result), 1, "The join table is empty");
    return;
  }

  if( lpriv->isTiled && (tile_id < 1 || tile_id > spriv->nbTile) )
  {
    char szErrorMsg[128];
    sprintf(szErrorMsg, "Object index=%d references incorrect tile_id=%d (nbTile=%d)",
            l->index, tile_id, spriv->nbTile);
    if( ecs_SetErrorShouldStop(&(s->result), 1, szErrorMsg) )
      return;
  }

  _selectTileText(s,l,tile_id);

  if (!vrf_get_text_feature(s,l,prim_id)) 
    return;

  /* Add the identifier to the object */

  ecs_SetObjectId(&(s->result),id);

  /* Add the attributes to the object */

  temp =vrf_get_ObjAttributes(lpriv->featureTable, text_id);
  if (temp != NULL)
    ecs_SetObjectAttr(&(s->result),temp);
  else 
    ecs_SetObjectAttr(&(s->result),"");

  ecs_SetSuccess(&(s->result));
}

/*************************************/

void 
_getObjectIdText(s,l,coord)
     ecs_Server *s;
     ecs_Layer *l;
     ecs_Coordinate *coord;
{
  char buffer[256];
  register ServerPrivateData *spriv = (ServerPrivateData *) s->priv;
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  short tile_id;
  int32 prim_id;
  int32 text_id;
  int feature_id;
  int32 index;
  double distance,result;

  distance = HUGE_VAL;
  feature_id = -1;

  for(index = 0; index < l->nbfeature; index++) {
    _getTileAndPrimId(s,l,index,&text_id,&tile_id, &prim_id);
    if (set_member(text_id,lpriv->feature_rows)) {
      if (tile_id == -1) {
	ecs_SetError(&(s->result), 1, "The VRF tiles are badly defined");
	return;
      }
      if (tile_id == -2) {
	ecs_SetError(&(s->result), 1, "The join table is empty");
	return;
      }      

      if( lpriv->isTiled && (tile_id < 1 || tile_id > spriv->nbTile) )
      {
        char szErrorMsg[128];
        sprintf(szErrorMsg, "Object index=%d references incorrect tile_id=%d (nbTile=%d)",
                l->index, tile_id, spriv->nbTile);
        if( ecs_SetErrorShouldStop(&(s->result), 1, szErrorMsg) )
          return;
      }
      else
      if (!(lpriv->isTiled) || 
	  ((coord->x > spriv->tile[tile_id-1].xmin) && 
	   (coord->x < spriv->tile[tile_id-1].xmax) && 
	   (coord->y > spriv->tile[tile_id-1].ymin) && 
	   (coord->y < spriv->tile[tile_id-1].ymax))) {
	
	_selectTileText(s,l,tile_id);
	if (!vrf_get_text_feature(s,l,prim_id)) 
	  return;
	
	result = ecs_DistanceObjectWithTolerance((&(ECSOBJECT((&(s->result))))),
				    coord->x, coord->y);
	if (result < distance) {
	  distance = result;
	  feature_id = index;
	}
      }
    }
  }

  if (feature_id < 0) {
    ecs_SetError(&(s->result),1,"Can't find any text at this location");
    return;
  }

  sprintf(buffer,"%d",feature_id);
  ecs_SetText(&(s->result),buffer);
  ecs_SetSuccess(&(s->result));
}

/*************************************/

void
_selectTileText(s,l,tile_id)
     ecs_Server *s;
     ecs_Layer *l;
     int tile_id;
{
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  register ServerPrivateData *spriv = (ServerPrivateData *) s->priv;
  char buffer[256];

  if (lpriv->isTiled) {
    if (lpriv->current_tileid != tile_id) {
      if (lpriv->current_tileid != -1) {
	/* fermeture des tables ouvertes precedemment */
#ifdef TESTOPENTABLE
	printf("close lpriv->l.text.textTable\n");
#endif
	vpf_close_table(&(lpriv->l.text.textTable));
      }
      /* ouverture des tables de primitives dans la bonne tuile */

      if (tile_id != 0) {
	sprintf(buffer,"%s/%s/%s/%s",spriv->library,lpriv->coverage,spriv->tile[tile_id-1].path,lpriv->primitiveTableName);
      } else {
	sprintf(buffer,"%s/%s/txt",spriv->library,lpriv->coverage);
	if (muse_access(buffer,0) != 0 ) {
	  sprintf(buffer,"%s/%s/TXT",spriv->library,lpriv->coverage);
	}
      }
#ifdef TESTOPENTABLE
      printf("open lpriv->l.text.textTable\n");
#endif
      lpriv->l.text.textTable = vpf_open_table(buffer, disk, "rb", NULL);

      lpriv->current_tileid = tile_id;
    }
  } else {
    if (lpriv->current_tileid == -1) {

#ifdef TESTOPENTABLE
      printf("open lpriv->l.text.textTable\n");
#endif
      /* ouverture des tables de primitives non-tuilees */

      sprintf(buffer,"%s/%s/%s",spriv->library,lpriv->coverage,lpriv->primitiveTableName);
      lpriv->l.text.textTable = vpf_open_table(buffer, disk, "rb", NULL);

      lpriv->current_tileid = 1;			
    }	
  }
}	


void _closeLayerTable(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  register LayerPrivateData *lpriv;

  (void) s;

  lpriv = (LayerPrivateData *) l->priv;

  if (lpriv->current_tileid == -1) {
    return;
  }

  switch(l->sel.F) {
  case Area:
#ifdef TESTOPENTABLE
    printf("close: lpriv->l.area.faceTable\n");
    printf("close: lpriv->l.area.mbrTable\n");
    printf("close: lpriv->l.area.ringTable\n");
    printf("close: lpriv->l.area.edgeTable\n");
#endif
    if (&(lpriv->l.area.faceTable) != NULL)
      vpf_close_table(&(lpriv->l.area.faceTable));
    if (&(lpriv->l.area.mbrTable) != NULL)
      vpf_close_table(&(lpriv->l.area.mbrTable));
    if (&(lpriv->l.area.ringTable) != NULL)
      vpf_close_table(&(lpriv->l.area.ringTable));
    if (&(lpriv->l.area.edgeTable) != NULL)
      vpf_close_table(&(lpriv->l.area.edgeTable));

    break;
  case Line:
#ifdef TESTOPENTABLE
    printf("close: lpriv->l.line.edgeTable\n");
    printf("close: lpriv->l.line.mbrTable\n");
#endif
    if (&(lpriv->l.line.edgeTable) != NULL)
      vpf_close_table(&(lpriv->l.line.edgeTable));
    if (&(lpriv->l.line.mbrTable) != NULL)
      vpf_close_table(&(lpriv->l.line.mbrTable));
    break;
  case Point:
#ifdef TESTOPENTABLE
    printf("close: lpriv->l.point.primTable\n");
#endif
    if (&(lpriv->l.point.primTable) != NULL)
      vpf_close_table(&(lpriv->l.point.primTable));
    break;
  case Text:
#ifdef TESTOPENTABLE
    printf("close: lpriv->l.text.textTable\n");
#endif
    if (&(lpriv->l.text.textTable) != NULL)
      vpf_close_table(&(lpriv->l.text.textTable));
    break;
  default:
    return;
  }

  lpriv->current_tileid = -1;

  return;
}


