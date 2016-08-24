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
 * $Log: feature.c,v $
 * Revision 1.20  2016/07/06 09:00:14  erouault
 * add heuristics in vrf_get_ring_coords() to detect cycling topology of edges that lead to endless looping and eventually crashes. Be robust to memory allocation failures in various places, and properly cleanup allocated structures when returning
 *
 * Revision 1.19  2016/06/28 14:32:45  erouault
 * Fix all warnings about unused variables raised by GCC 4.8
 *
 * Revision 1.18  2016/06/27 20:05:12  erouault
 * Grow some buffers in VRF driver (patch by Craig Bruce)
 *
 * Revision 1.17  2009/05/08 04:15:50  warmerda
 * fixed count type in VRF driver for 64bit systems (#2787502)
 *
 * Revision 1.16  2007/05/09 20:46:28  cbalint
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
 * Revision 1.15  2007/02/12 15:52:57  cbalint
 *
 *    Preliminary cleanup.
 *    Get rif of unitialized variables, and unused ones.
 *
 * Revision 1.14  2004/10/26 20:29:43  warmerda
 * Removed hack that was dropping some inner rings from polygons unnecessarily.
 * The hack appears to be to deal with some problem of inner rings duplicating
 * outer rings in browse products, but I don't know how to check the original
 * case.  See bug report 692844.
 *
 * Revision 1.13  2004/04/04 04:33:01  warmerda
 * added vrf_free_ObjAttributeBuffer
 *
 * Revision 1.12  2004/02/19 05:46:28  warmerda
 * fixed memory leak of edge coords with dangles
 *
 * Revision 1.11  2003/05/22 17:04:05  warmerda
 * Removed debug statement.
 *
 * Revision 1.10  2003/05/22 16:58:01  warmerda
 * Several fixes related to reading VITD area geometries properly even if
 * the datasets face information seems to be corrupt.  See bug:
 * http://sf.net/tracker/index.php?func=detail&aid=741854&group_id=11181&atid=111181
 *
 * Revision 1.9  2003/05/21 18:50:19  warmerda
 * verify that table_pos(COORDINATE) succeeds in point/line feature read
 *
 * Revision 1.8  2001/08/16 21:02:37  warmerda
 * Removed MAXSEGS and MAXRINGS fixed limits
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
#include <assert.h>

ECS_CVSID("$Id: feature.c,v 1.20 2016/07/06 09:00:14 erouault Exp $");

vpf_projection_type NOPROJ = {DDS, 0.0, 0.0, 0.0, 0.0, 0, 0.0, 0.0,
                              NULL, NULL, "Decimal Degrees     "};


/*********************************************************************
  vrf_merge_line_prim()

  Local service routine for vrf_get_merged_line_feature, which applies
  the algorithm to merge a single new line segment into an existing
  aggregated line.

  IN
     ecs_Server *s: ecs_Server structure
     ecs_Layer *layer: Layer information structure
     int primCount,primList: Primitive ID list

  OUT
     return int: Error code. True if the function execute correctly,
     false else.
  
  ********************************************************************/

static int vrf_merge_line_prim( int *vertCount, double * vertX, double *vertY,
                                ecs_Line * line )

{
    int		insertFlag = FALSE, reverseFlag = FALSE, i, insertStart;
    int		line_vert = line->c.c_len;

    /*
      Figure out the end points that match, if any, so we know how to
      organize
      */
    
    if( vertX[0] == line->c.c_val[0].x
        && vertY[0] == line->c.c_val[0].y )
    {
        insertFlag = TRUE;
        reverseFlag = TRUE;
    }
    else if( vertX[*vertCount - 1] == line->c.c_val[0].x
             && vertY[*vertCount - 1] == line->c.c_val[0].y )
    {
        /* append to end, no reverse */
    }
    else if( vertX[*vertCount - 1] == line->c.c_val[line_vert-1].x
             && vertY[*vertCount - 1] == line->c.c_val[line_vert-1].y )
    {
        reverseFlag = TRUE;
    }
    else if( vertX[0] == line->c.c_val[line_vert-1].x
             && vertY[0] == line->c.c_val[line_vert-1].y )
    {
        insertFlag = TRUE;
    }
    else
    {
        /* there is no coincident end points ... give up */
        return FALSE;
    }
    
    /*
      If we are inserting the new primitive in front of the existing
      vertices, then we will have to push the existing ones down ...
      */

    if( insertFlag )
    {
        for( i = *vertCount - 1; i >= 0; i-- )
        {
            vertX[i + line_vert - 1] = vertX[i];
            vertY[i + line_vert - 1] = vertY[i];
        }
    }

    /*
      Insert the new primitives vertices
      */

    if( insertFlag )
        insertStart = 0;
    else
        insertStart = *vertCount - 1;

    for( i = 0; i < line_vert; i++ )
    {
        if( reverseFlag )
        {
            vertX[insertStart + i] = line->c.c_val[line_vert - i - 1].x;
            vertY[insertStart + i] = line->c.c_val[line_vert - i - 1].y;
        }
        else
        {
            vertX[insertStart + i] = line->c.c_val[i].x;
            vertY[insertStart + i] = line->c.c_val[i].y;
        }
    }

    *vertCount += (line_vert - 1);
    
    return TRUE;
}

/*********************************************************************
  vrf_get_merged_line_feature

  Fill the ecs_Result with the merged coordinates of the passed primitive list.

  IN
     ecs_Server *s: ecs_Server structure
     ecs_Layer *layer: Layer information structure
     int primCount,primList: Primitive ID list

  OUT
     return int: Error code. True if the function execute correctly,
     false else.
  
  ********************************************************************/

int vrf_get_merged_line_feature (s, layer, primCount, primList)
     ecs_Server *s;
     ecs_Layer *layer;
     int primCount;
     int32 *primList;
{
    int		iPrim;
    ecs_Result	*primResults;
    double      *vertX, *vertY;
    int		vertCount, maxVertCount, i, *primConsumed, work_done;
    int		primsRemaining;
    ecs_Line	*line;

    /*
      simple case, no merging of primitives.
      */
    
    if( primCount == 1 )
    {
        return vrf_get_line_feature( s, layer, primList[0],
                                     &(s->result) );
    }

    /*
      Collect geometry for each of the primitives.
      */
    primResults = (ecs_Result *) calloc(sizeof(ecs_Result),primCount);
    maxVertCount = 0;
    
    for( iPrim = 0; iPrim < primCount; iPrim++ )
    {
        if( !vrf_get_line_feature( s, layer, primList[iPrim],
                                   primResults+iPrim ) )
        {
            for( ; iPrim >=0; iPrim-- )
                ecs_CleanUp( primResults + iPrim );

            free( primResults );
            ecs_SetError(&(s->result), 1,"Error in vrf_get_merged_line_feature");
            return FALSE;
        }

        maxVertCount += ECSGEOM((primResults+iPrim)).line.c.c_len;
    }

    /*
     * Initialize our aggregate feature with the first primitive.
     */
    
    vertX = (double *) malloc(sizeof(double) * maxVertCount);
    vertY = (double *) malloc(sizeof(double) * maxVertCount);
    primConsumed = (int *) calloc(sizeof(int),primCount);

    line = &(ECSGEOM((primResults+0)).line);
    vertCount = line->c.c_len;
    for( i = 0; i < (int) line->c.c_len; i++ )
    {
        vertX[i] = line->c.c_val[i].x;
        vertY[i] = line->c.c_val[i].y;
    }

    /*
     * Merge in new features one at a time.  If we make a pass through all
     * the unmerged features without being able to merge another one
     * at either end we give up, abandoning any remaining primitives.
     */

    primsRemaining = primCount - 1;
    work_done = TRUE;
    while( work_done && primsRemaining > 0 )
    {
        work_done = FALSE;
        
        for( iPrim = 1; iPrim < primCount; iPrim++ )
        {
            line = &(ECSGEOM((primResults+iPrim)).line);

            if( primConsumed[iPrim] )
                continue;

            if( vrf_merge_line_prim( &vertCount, vertX, vertY, line ) )
            {
                work_done = TRUE;
                primConsumed[iPrim] = TRUE;
                primsRemaining--;
            }
        }
    }

    /*
      Build returned line structure.
      */
    if (!ecs_SetGeomLine(&(s->result), vertCount))
        return FALSE; 
    
    for( i = 0; i < vertCount; i++ )
    {
        ECS_SETGEOMLINECOORD((&(s->result)), i, vertX[i], vertY[i]);
    }
    
    /*
      Cleanup datastructures.
      */

    free( vertX );
    free( vertY );
    free( primConsumed );

    for( iPrim = 0; iPrim < primCount; iPrim++ )
        ecs_CleanUp( primResults + iPrim );

    free( primResults );

    return TRUE;
}

/*********************************************************************
  vrf_get_line_feature

  Fill the ecs_Result with the vrf information directly extract
  from the table. 

  IN
     ecs_Server *s: ecs_Server structure
     ecs_Layer *layer: Layer information structure
     int prim_id: Primitive ID

  OUT
     return int: Error code. True if the function execute correctly,
     false else.
  
  ********************************************************************/

int vrf_get_line_feature (s, layer, prim_id, result)
     ecs_Server *s;
     ecs_Layer *layer;
     int prim_id;
     ecs_Result *result;
{
  int32 pos, count;
  row_type row;
  int i;
  coordinate_type *ptr1=NULL;
  tri_coordinate_type *ptr2=NULL;
  double_coordinate_type *ptr3=NULL;
  double_tri_coordinate_type *ptr4=NULL;
  register LayerPrivateData *lpriv = (LayerPrivateData *) layer->priv;

  /* 
     -----------------------------------------------------------

     Check the tables to see if they are open

     -----------------------------------------------------------
     */  

  if (!vrf_checkLayerTables(s,layer)) {
    return FALSE;
  }

  /* 
     -----------------------------------------------------------

     Extract table informations from the ecs_Server structure "s".

     -----------------------------------------------------------
     */  

  row = read_row (prim_id, lpriv->l.line.edgeTable);
  if (row == NULL) {
    ecs_SetError(result, 1,"Unable to extract the edge");
    return FALSE;
  }
  pos = table_pos ("COORDINATES", lpriv->l.line.edgeTable);

  if( pos == -1 )
  {
      ecs_SetError(result, 2, "No COORDINATE column");
      free_row(row,lpriv->l.line.edgeTable);
      return FALSE;
  }
  
  /* 
     -----------------------------------------------------------

     Get the coordinates in the table 

     -----------------------------------------------------------
     */

  switch (lpriv->l.line.edgeTable.header[pos].type) {
  case 'C': 
    ptr1 = get_table_element (pos, row, lpriv->l.line.edgeTable, NULL, &count);
    break;
  case 'Z':
    ptr2 = get_table_element (pos, row, lpriv->l.line.edgeTable, NULL, &count);
    break;
  case 'B':
    ptr3 = get_table_element (pos, row, lpriv->l.line.edgeTable, NULL, &count);
    break;
  case 'Y':
    ptr4 = get_table_element (pos, row, lpriv->l.line.edgeTable, NULL, &count);
    break;
  default:
    ecs_SetError(result, 2, "Undefined VRF table type");
    free_row(row,lpriv->l.line.edgeTable);
    return FALSE;
  }

  free_row(row,lpriv->l.line.edgeTable);

  /* 
     -----------------------------------------------------------

     Initialize line structure 

     -----------------------------------------------------------
     */

  if (!ecs_SetGeomLine(result, count))
    return FALSE; 

  /* 
     -----------------------------------------------------------

     Fill the table line structure and free the old structure ptr

     -----------------------------------------------------------
     */

  switch (lpriv->l.line.edgeTable.header[pos].type) {
  case 'C': 
    {
      if ((count == 1) && (ptr1 == (coordinate_type*)NULL)) {
	ecs_SetError(result, 2, "Only one coordinate found for a line");
        xvt_free ((char*)ptr1);
        return FALSE;
      } else if( ptr1 == NULL ) {
        ecs_SetError(result, 1, "ptr1 == NULL");
        return FALSE;
      } else {
	for (i=0; i<count; i++) {
         ECS_SETGEOMLINECOORD((result),i,
			       ((double) ptr1[i].x),
			       ((double) ptr1[i].y))
	}
	if (ptr1)
	  xvt_free ((char*)ptr1);
	break;
      }
    }
  case 'Z':
    {
      if ((count == 1) && (ptr2 == (tri_coordinate_type*)NULL)) {
	ecs_SetError(result, 2, "Only one coordinate found for a line");
        xvt_free ((char*)ptr2);
        return FALSE;
      } else if( ptr2 == NULL ) {
        ecs_SetError(result, 1, "ptr2 == NULL");
        return FALSE;
      } else {
	for (i=0; i<count; i++) {
	  ECS_SETGEOMLINECOORD((result),i,((double) ptr2[i].x),((double) ptr2[i].y))
	}
	if (ptr2)
	  xvt_free ((char*)ptr2);
	break;
      }
    }
  case 'B':
    {
      if ((count == 1) && (ptr3 == (double_coordinate_type*)NULL)) {
	ecs_SetError(result, 2, "Only one coordinate found for a line");
        xvt_free ((char*)ptr3);
        return FALSE;
      } else if( ptr3 == NULL ) {
        ecs_SetError(result, 1, "ptr3 == NULL");
        return FALSE;
      } else {
	for (i=0; i<count; i++) {
	  ECS_SETGEOMLINECOORD((result),i,((double) ptr3[i].x),((double) ptr3[i].y))
	}
      }
      if (ptr3)
	xvt_free ((char*)ptr3);
      break;
    }
  case 'Y':
    {
      if ((count == 1) && (ptr4 == (double_tri_coordinate_type*)NULL)) {
	ecs_SetError(result, 2, "Only one coordinate found for a line");
        xvt_free ((char*)ptr4);
        return FALSE;
      } else if( ptr4 == NULL ) {
        ecs_SetError(result, 1, "ptr4 == NULL");
        return FALSE;
      } else {
	for (i=0; i<count; i++) {
	  ECS_SETGEOMLINECOORD((result),i,((double) ptr4[i].x),((double) ptr4[i].y))
	}
      }
      if (ptr4)
	xvt_free ((char*)ptr4);
      break;
    }    
  default:
    break;
  } /* switch type */
  return TRUE;
}


static int vrf_get_mbr (table, prim_id, xmin, ymin, xmax, ymax)
     vpf_table_type table;
     int32 prim_id;
     double *xmin;
     double *ymin;
     double *xmax;
     double *ymax;
{
  int32 count;
  float temp = 0.0f;
  row_type row;

  *xmin = 0;
  *ymin = 0;
  *xmax = 0;
  *ymax = 0;

  if (table.fp == NULL) {
    return FALSE;
  }

  row = read_row (prim_id, table);
  if( !row )
    return FALSE;

  /* The 'DBVMAP2I' VMAP2i product has FBR tables with columns of type double instead of float */
  /* so we must check for the type */
  if (table.header[table_pos("XMIN",table)].type == 'F')
  {
    get_table_element (table_pos("XMIN",table), row, table, &temp, &count);
    *xmin = (double) temp;
    get_table_element (table_pos("XMAX",table), row, table, &temp, &count);
    *xmax = (double) temp;
    get_table_element (table_pos("YMIN",table), row, table, &temp, &count);
    *ymin = (double) temp;
    get_table_element (table_pos("YMAX",table), row, table, &temp, &count);
    *ymax = (double) temp;
  }
  else
  {
    get_table_element (table_pos("XMIN",table), row, table, xmin, &count);
    get_table_element (table_pos("XMAX",table), row, table, xmax, &count);
    get_table_element (table_pos("YMIN",table), row, table, ymin, &count);
    get_table_element (table_pos("YMAX",table), row, table, ymax, &count);
  }

  free_row(row,table);

  return TRUE;
}

/*********************************************************************
  vrf_get_line_mbr

  Get the related mbr of a primitive line

  IN
     ecs_Server *s: ecs_Server structure
     ecs_Layer *layer: Layer information structure
     int prim_id: Primitive ID

  OUT
     return int: Error code. True if the function execute correctly,
     false else.
     double *xmin, *xmax, *ymin, *ymax: Returned bounding box
  
  ********************************************************************/

int vrf_get_line_mbr (layer, prim_id, xmin, ymin, xmax, ymax)
     ecs_Layer *layer;
     int32 prim_id;
     double *xmin;
     double *ymin;
     double *xmax;
     double *ymax;
{
  LayerPrivateData *lpriv = (LayerPrivateData *) layer->priv;
  return vrf_get_mbr(lpriv->l.line.mbrTable, prim_id, xmin, ymin, xmax, ymax);
}

/*********************************************************************
  vrf_get_lines_mbr

  Get the related mbr of a list of primitive lines

  IN
     ecs_Layer *layer: Layer information structure
     int primCount: primitive count
     int primList: list of Primitive IDs

  OUT
     return int: Error code. True if the function execute correctly,
     false else.
     double *xmin, *xmax, *ymin, *ymax: Returned bounding box
  
  ********************************************************************/

int vrf_get_lines_mbr (layer, primCount, primList, xmin, ymin, xmax, ymax)
     ecs_Layer *layer;
     int32 primCount;
     int32 *primList;
     double *xmin;
     double *ymin;
     double *xmax;
     double *ymax;
{
    int		i;

    if( !vrf_get_line_mbr( layer, primList[0], xmin, ymin, xmax, ymax ) )
        return FALSE;

    for( i = 1; i < primCount; i++ )
    {
        double	x2min, x2max, y2min, y2max;

        if( !vrf_get_line_mbr( layer, primList[i],
                               &x2min, &y2min, &x2max, &y2max ) )
            return FALSE;

        if( x2min < *xmin )
            *xmin = x2min;
        if( y2min < *ymin )
            *ymin = y2min;
        if( x2max > *xmax )
            *xmax = x2max;
        if( y2max > *ymax )
            *ymax = y2max;
    }

    return TRUE;
}

/*********************************************************************
  GET_TEXT_FEATURE                                                   
  ********************************************************************/

int vrf_get_text_feature (s, layer, prim_id)
     ecs_Server *s;
     ecs_Layer *layer;
     int prim_id;
{
  row_type row;			/* Row type in the text primitive table  */
  vpf_table_type table;         /* VRF table type format		 */
  int32 pos; 		       	/* Position in the text primitive table  */
  int32 count;		       	/* Number of caracters that were read    */
  double x,y;		       	/* coordinates of the text 	      	 */
  int code;		        /* success or failure of the operation   */
  LayerPrivateData *PrivData;   /* Private information on the layer      */
  char * desc;
  
  /* 
     -----------------------------------------------------------

     Check the tables to see if they are open

     -----------------------------------------------------------
     */  

  if (!vrf_checkLayerTables(s,layer)) {
    return FALSE;
  }

  PrivData = (LayerPrivateData *) layer->priv; /* casting the private data for a VPF Point layer from */
  table = PrivData->l.text.textTable;	       /* our interest here is the primitive table 	      */
  row = read_row (prim_id, table);	       /* Read the prim_id row from the text primitive table */
  if( row == NULL )
  {
    ecs_SetError(&(s->result), 1, "Unable to get row");
    return FALSE;
  }

  pos = table_pos ("STRING", table);	       /* find the position in the primitive table */
  desc = (char *) get_table_element (pos, row, table, NULL, &count); /* get the text string   */
  
  pos = table_pos ("SHAPE_LINE", table);
  /* get the text coordinate, code will receive the result of th 0 = problem, 1 = success */
  if ((code = vrf_get_xy (table, row, pos, &x, &y)) == TRUE) {
    code = ecs_SetGeomText(&(s->result),x,y,desc); 
  } else {
    ecs_SetError(&(s->result), 1, "Unable to get coordinates");    
  }

  free_row(row,PrivData->l.text.textTable);  
  xvt_free(desc);
  /* here all the information needed is known in result (ecs_Result) that is in s (ecs_Server) */
  return code;

}

/*********************************************************************
  GET_POINT_FEATURE
  Derived from draw_point_row  [vpfdraw.c]                       
  ********************************************************************/

int vrf_get_point_feature (s, layer, prim_id)
     ecs_Server *s;
     ecs_Layer *layer;
     int prim_id;
{
  row_type row;			/* Row type in the point primitive table */
  vpf_table_type table;         /* VRF table type format		 */
  int32 pos; 		        /* Position in the point primitive table */
  double x,y;			/* Coordinates of the point 		 */
  int code;			/* Success or failure of the operation   */
  LayerPrivateData *PrivData;   /* Private information on the layer      */

  /* 
     -----------------------------------------------------------

     Check the tables to see if they are open

     -----------------------------------------------------------
     */  

  if (!vrf_checkLayerTables(s,layer)) {
    return FALSE;
  }
  
  PrivData = (LayerPrivateData *) layer->priv; /* casting the private data for a VPF Point layer from */
  table = PrivData->l.point.primTable;	       /* our interest here is the primitive table 	      */
  row = read_row (prim_id, table);	       /* Read the prim_id row from the point primitive table */
  pos = table_pos ("COORDINATE", table);       /* find the position in the primitive table */
  /* get the point coordinate, code will receive the result of th 0 = problem, 1 = success */
  if ( pos != -1 && (code = vrf_get_xy (table, row, pos, &x, &y)) == TRUE) {
    code = ecs_SetGeomPoint(&(s->result),x,y); 
  } else {
    ecs_SetError(&(s->result), 1, "Unable to get coordinates");
    code = FALSE;
  }
  free_row(row,PrivData->l.point.primTable);  
  /* here all the information needed is known in result (ecs_Result) that is in s (ecs_Server) */
  return code;
}

/*********************************************************************
  GET_AREA_FEATURE                                                  
  Derived from outline_face  [vpfdraw.c]                          
  ********************************************************************/

int vrf_get_area_feature (s, layer, prim_id)
     ecs_Server *s;
     ecs_Layer *layer;
     int prim_id;
{                                    
  int32 n=0;
  int code,i,j,k,qty;
  face_rec_type face_rec;
  ring_rec_type ring_rec;
  vpf_table_type facetable, ringtable, edgetable;
  AREA_FEATURE area;
  double x,y;
#ifdef notdef
  int firstlength;
#endif
  int max_rings;
    
  /* 
     -----------------------------------------------------------

     Check the tables to see if they are open

     -----------------------------------------------------------
     */  

  if (!vrf_checkLayerTables(s,layer)) {
    return FALSE;
  }

  facetable = ((LayerPrivateData *) layer->priv)->l.area.faceTable;
  ringtable = ((LayerPrivateData *) layer->priv)->l.area.ringTable;
  edgetable = ((LayerPrivateData *) layer->priv)->l.area.edgeTable;
  face_rec = read_face (prim_id, facetable);
  ring_rec = read_ring (face_rec.ring, ringtable);
  
  /* 
     Allocate space to store addresses of all the ring structures 
     */
  max_rings = 5;
  area.rings = (RING**)xvt_zmalloc (max_rings * sizeof (RING*));
  if (area.rings == NULL) {
    ecs_SetError(&(s->result), 2, "No enough memory");
    return FALSE;
  }

  /* 
     Get the outer ring coords 
     */
  area.rings[n] = (RING*)xvt_zmalloc (sizeof (RING));
  if (area.rings[n] == NULL) {
    ecs_SetError(&(s->result), 2, "No enough memory");
    xvt_free ((char*)area.rings);
    return FALSE;
  }
  
  area.rings[n]->id = n+1;

  if (!vrf_get_ring_coords (s,area.rings[n], prim_id, ring_rec.edge, edgetable)) {
    xvt_free((char*)area.rings[0]);
    xvt_free ((char*)area.rings);
    return FALSE;
  }
#ifdef notdef
  firstlength = area.rings[n]->nr_segs;
#endif
  n++;
  
  /* 
     Get the coords for any inner rings that exist 
     */
  while (ring_rec.face == prim_id) {
    ring_rec = read_next_ring (ringtable);
    
    if (feof (ringtable.fp))
      break;

    /*
    ** The Browse Case: It is possible the last island cover the same
    ** region than the first one.
    **
    ** NFW/2004: The following logic seems unreasonably broad and has for 
    ** certain been causing some island polygons (such as for the island at
    ** 14.85E, 60.55N in inwatera@hydro(*) of the eurasia VMAP0 dataset) to 
    ** disappear without reason.  There may be a case where this logic should
    ** apply, but without detail on how to reproduce the original issue, I am
    ** just removing the logic completely. 
    **
    ** See ogdi.sf.net bug tracker bug: 692844
    */
#ifdef notdef
    if (n>=2 && ring_rec.face != prim_id && area.rings[n-1]->nr_segs == firstlength) {
        n--;
        break;
    }
#endif

    if (ring_rec.face == prim_id) {
      if( n == max_rings )
      {
          RING** newrings;
          max_rings *= 2;
          newrings = (RING **) xvt_realloc(area.rings, 
                                             sizeof(RING *) * max_rings);
          if( newrings == NULL )
          {
            for(i=0;i<n;i++) {
              for(j=0;j<area.rings[i]->nr_segs;j++) {
                xvt_free((char*) area.rings[i]->segs[j]->coords);
                xvt_free((char*) area.rings[i]->segs[j]);
              }
              xvt_free((char*)area.rings[i]->segs);
              xvt_free((char*)area.rings[i]);
            }
            xvt_free ((char*)area.rings);
            ecs_SetError(&(s->result), 2, "No enough memory");
            return FALSE;
          }
          area.rings = newrings;
      }

      area.rings[n] = (RING*)xvt_zmalloc (sizeof (RING));
      if (area.rings[n] == NULL) {
	for(i=0;i<n;i++) {
	  for(j=0;j<area.rings[i]->nr_segs;j++) {
	    xvt_free((char*) area.rings[i]->segs[j]->coords);
	    xvt_free((char*) area.rings[i]->segs[j]);
	  }
	  xvt_free((char*)area.rings[i]->segs);
	  xvt_free((char*)area.rings[i]);
	}
	xvt_free ((char*)area.rings);
	ecs_SetError(&(s->result), 2, "No enough memory");
	return FALSE;
      }
        
      area.rings[n]->id = n+1;
      
      if (!vrf_get_ring_coords (s,area.rings[n], prim_id, ring_rec.edge, edgetable)) {
	for(i=0;i<n;i++) {
	  for(j=0;j<area.rings[i]->nr_segs;j++) {
	    xvt_free((char*) area.rings[i]->segs[j]->coords);
	    xvt_free((char*) area.rings[i]->segs[j]);
	  }
	  xvt_free((char*)area.rings[i]->segs);
	  xvt_free((char*)area.rings[i]);
	}
	xvt_free((char*)area.rings[n]);
	xvt_free ((char*)area.rings);
	ecs_SetError(&(s->result), 2, "No enough memory");
	return FALSE;
      }

      n++;                        
    }
  }
  area.nr_rings = n;
  assert( n <= max_rings );
  
  /* 
     Extract all coordinates from area and put them in a ecs_Area 
     */

  code = TRUE;
  if ((code = ecs_SetGeomArea(&(s->result), area.nr_rings))) {
    for(i=0;i<area.nr_rings;i++) {

      if (!code)
	break;

      /*
	For all the ring segments, calculate the total number of points
	*/

      qty = 0;
      for(j=0;j<area.rings[i]->nr_segs;j++)
	qty += area.rings[i]->segs[j]->nr_coords;
      
      /*
	Initialise the ring and add all the coordinates
	*/

      if( (code = ecs_SetGeomAreaRing(&(s->result), i, qty, 0.0, 0.0)) ) { 
	qty = 0;
	for(j=0;j<area.rings[i]->nr_segs;j++) {
	  for(k=0; k<area.rings[i]->segs[j]->nr_coords;k++) {
	    x = (double) area.rings[i]->segs[j]->coords[k].x;
	    y = (double) area.rings[i]->segs[j]->coords[k].y;
	    ECS_SETGEOMAREACOORD((&(s->result)), i, qty, x, y);
	    qty++;
	  }
	}
      }
    }
  }
    
  for(i=0;i < area.nr_rings;i++) {
    for(j=0;j<area.rings[i]->nr_segs;j++) {
      xvt_free((char*) area.rings[i]->segs[j]->coords);
      xvt_free((char*) area.rings[i]->segs[j]);
    }
    xvt_free((char*)area.rings[i]->segs);
    xvt_free((char*)area.rings[i]);
  }
  xvt_free ((char*)area.rings);
  
  return code;
} 
   
/*********************************************************************
  GET_RING_COORDS                                                   
  Derived from outline_face_ring  [vpfdraw.c]                   
  ********************************************************************/

int vrf_get_ring_coords (s,ring, face_id, start_edge, edgetable)
     ecs_Server *s;
     RING *ring;
     int32 face_id, start_edge;
     vpf_table_type edgetable;
{
  edge_rec_type edge_rec;
  int32 next_edge, prevnode, i, n=0;
  boolean done=FALSE;
  vpf_projection_type proj;
  double_coordinate_type  dcoord;
  SEGMENT **temp;
  long eqlface1=0L;
#ifdef notdef
  long eqlface2=0L;
#endif
  /*long eqlnpts;*/
  long eqlleft_edge=0L, eqlright_edge=0L;
  long maxsegs;
  char buffer[120];
  char start_dir = '+';

  maxsegs = 5;
  proj = NOPROJ;

  edge_rec = read_edge (start_edge, edgetable, proj.inverse_proj);
  if (edge_rec.npts == 0) {
    sprintf(buffer,"Unable to read the edge %d in the face %d",
            (int) start_edge, (int) face_id);
    ecs_SetError(&(s->result), 1,buffer);
    return FALSE;
  }
  edge_rec.dir = '+';
  prevnode = edge_rec.start_node;
  
  if (edge_rec.start_node == edge_rec.end_node)
    done = TRUE;
  next_edge = vrf_next_face_edge (&edge_rec, &prevnode, face_id);
  if ((edge_rec.right_face == face_id) && (edge_rec.left_face == face_id))
    {
      eqlface1 = 1L;
      /*eqlnpts = edge_rec.npts;*/
      eqlleft_edge = edge_rec.left_edge;
      eqlright_edge = edge_rec.right_edge;
      start_dir = edge_rec.dir;
    }
  else
    eqlface1 = 0L;
   
  /* Allocate plenty of space for array of segment addresses */
  ring->segs = (SEGMENT**)xvt_zmalloc (maxsegs * sizeof (SEGMENT*));
  if( ring->segs == NULL )
  {
    if (edge_rec.coords)
      xvt_free ((char*)edge_rec.coords);
    sprintf(buffer,"Unable to allocate memory in vrf_get_ring_coords() for face %d",
            (int) face_id);
    ecs_SetError(&(s->result), 1,buffer);
    return FALSE;
  }

  /* Load the first segment of the ring */
  ring->segs[n] = (SEGMENT*)xvt_zmalloc (sizeof (SEGMENT));
  if( ring->segs[n] == NULL )
  {
    xvt_free((char*)ring->segs);
    ring->segs = NULL;
    if (edge_rec.coords)
      xvt_free ((char*)edge_rec.coords);
    sprintf(buffer,"Unable to allocate memory in vrf_get_ring_coords() for face %d",
            (int) face_id);
    ecs_SetError(&(s->result), 1,buffer);
    return FALSE;
  }
  ring->segs[n]->nr_coords = edge_rec.npts;
  ring->segs[n]->id = n+1;

  /* Allocate space for the coordinates of the first segment */
  ring->segs[n]->coords = (COORDINATE*)xvt_zmalloc ((size_t)ring->segs[n]->nr_coords * sizeof (COORDINATE));
  if( ring->segs[n]->coords == NULL )
  {
    xvt_free((char*)ring->segs[n]);
    xvt_free((char*)ring->segs);
    ring->segs = NULL;
    if (edge_rec.coords)
      xvt_free ((char*)edge_rec.coords);
    sprintf(buffer,"Unable to allocate memory in vrf_get_ring_coords() for face %d",
            (int) face_id);
    ecs_SetError(&(s->result), 1,buffer);
    return FALSE;
  }
                                                
  /* If the direction is - load in reverse order */
  if (edge_rec.dir == '-')
    {
      for (i=(edge_rec.npts-1); i>=0; i--)
	{
	  dcoord = next_edge_coordinate (&edge_rec);
	  ring->segs[n]->coords[i].x = (float)dcoord.x;
	  ring->segs[n]->coords[i].y = (float)dcoord.y;
	}
      } else {
	for (i=0; i<edge_rec.npts; i++) {
	  dcoord = next_edge_coordinate (&edge_rec);
	  ring->segs[n]->coords[i].x = (float)dcoord.x;
	  ring->segs[n]->coords[i].y = (float)dcoord.y;
	}
    }

  n++;
  if (edge_rec.coords)
    xvt_free ((char*)edge_rec.coords);

  while (!done)
    {
      /* This is a temptative way to detect cycles in the chaining of edges: */
      /* There is no reason that a sane ring might follow each edges more than */
      /* twice */
      if( n > edgetable.nrows * 2 )
      {
        sprintf(buffer,"Cycle detected in the edges of face %d",
               (int) face_id);
        for( --n;  n >= 0; --n )
        {
            xvt_free((char*) ring->segs[n]->coords);
            xvt_free((char*) ring->segs[n]);
        }
        xvt_free(ring->segs);
        ring->segs = NULL;
        ecs_SetError(&(s->result), 1,buffer);
        return FALSE;
      }

      if (next_edge < 0)
	{
	  done = TRUE;
	}

      if (next_edge == 0)
	{
	  done = TRUE;
	}

      if (next_edge == start_edge && !eqlface1)
	{
	  done = TRUE;
	  continue;
	}

      if (next_edge == start_edge && eqlface1 &&
	  eqlleft_edge == 0L && eqlright_edge == 0L)
	{
	  done = TRUE;
	}

      if (!done)
	{
	  edge_rec = read_edge( next_edge, edgetable, (long)proj.inverse_proj);
	  if (edge_rec.npts == 0) {
            sprintf(buffer,"Unable to read the edge %d in the face %d, segment %d",
                    (int) next_edge, (int) face_id, n);
            for( --n;  n >= 0; --n )
            {
                xvt_free((char*) ring->segs[n]->coords);
                xvt_free((char*) ring->segs[n]);
            }
            xvt_free(ring->segs);
            ring->segs = NULL;
            ecs_SetError(&(s->result), 1,buffer);
            return FALSE;
	  }

	  next_edge = vrf_next_face_edge( &edge_rec, &prevnode, face_id );
#ifdef notdef
	  if ((edge_rec.right_face == face_id) && (edge_rec.left_face ==face_id))
            eqlface2 = 1L;
	  else
            eqlface2 = 0L;
#endif
          /* 
           * This is to catch cases where there would appear to be a dangle
           * (so we set eqlface1), but when we go to repeat the start edge
           * we find we are going the same direction as the first time.  
           * This occurs with some VITD dataset as per bug 741854 on
           * http://ogdi.sf.net/
           */
#ifndef SKIP_BUG_741854_FIX
          if( edge_rec.id == start_edge && edge_rec.dir == start_dir )
          {
              if (edge_rec.coords)
                  xvt_free ((char*)edge_rec.coords);
              edge_rec.coords = NULL;
              done = TRUE;
              continue;
          }
#endif
	  /* Allocate space for the next segment */
	  if (eqlface1 && edge_rec.id == eqlleft_edge)
            eqlleft_edge = 0L;
	  if (eqlface1 && edge_rec.id == eqlright_edge)
            eqlright_edge = 0L;

          if( n == maxsegs )
          {
              SEGMENT** newsegs;
              maxsegs *= 2;
              newsegs = (SEGMENT**)
                  xvt_realloc(ring->segs, maxsegs * sizeof (SEGMENT*));
              if( newsegs == NULL )
              {
                  sprintf(buffer,"Line %d: Memory allocation failure for segment %d in the face %d",
                          __LINE__, n, (int) face_id);
                  for( --n;  n >= 0; --n )
                  {
                      xvt_free((char*) ring->segs[n]->coords);
                      xvt_free((char*) ring->segs[n]);
                  }
                  xvt_free(ring->segs);
                  ring->segs = NULL;
                  if (edge_rec.coords)
                    xvt_free ((char*)edge_rec.coords);
                  ecs_SetError(&(s->result), 1,buffer);
                  return FALSE;
              }
              ring->segs = newsegs;
          }

	  ring->segs[n] = (SEGMENT*)xvt_zmalloc (sizeof (SEGMENT));
	  if( ring->segs[n] == NULL )
	  {
              sprintf(buffer,"Line %d: Memory allocation failure for segment %d in the face %d",
                          __LINE__, n, (int) face_id);
	      for( ;  n >= 0; --n )
	      {
		  xvt_free((char*) ring->segs[n]->coords);
		  xvt_free((char*) ring->segs[n]);
	      }
	      xvt_free(ring->segs);
              ring->segs = NULL;
              if (edge_rec.coords)
                xvt_free ((char*)edge_rec.coords);
              ecs_SetError(&(s->result), 1,buffer);
	      return FALSE;
	  }
	  ring->segs[n]->nr_coords = edge_rec.npts;
	  ring->segs[n]->id = n+1;
         
	  /* Allocate space for the segment coordinates */
	  ring->segs[n]->coords = (COORDINATE*)xvt_zmalloc ((size_t)ring->segs[n]->nr_coords * sizeof (COORDINATE));
          if( ring->segs[n]->coords == NULL )
          {
              sprintf(buffer,"Line %d: Memory allocation failure for segment %d in the face %d",
                          __LINE__, n, (int) face_id);
              for( ;  n >= 0; --n )
              {
                  xvt_free((char*) ring->segs[n]->coords);
                  xvt_free((char*) ring->segs[n]);
              }
              xvt_free(ring->segs);
              ring->segs = NULL;
              if (edge_rec.coords)
                xvt_free ((char*)edge_rec.coords);
              ecs_SetError(&(s->result), 1,buffer);
              return FALSE;
          }

	  /* If the direction is - load in reverse order */
	  if (edge_rec.dir == '-')
            {
	      for (i=(edge_rec.npts-1); i>=0; i--)
		{
		  dcoord = next_edge_coordinate (&edge_rec);
		  ring->segs[n]->coords[i].x = (float)dcoord.x;
		  ring->segs[n]->coords[i].y = (float)dcoord.y;
		}
            }
	  else
            {
	      for (i=0; i<edge_rec.npts; i++)
		{
		  dcoord = next_edge_coordinate (&edge_rec);
		  ring->segs[n]->coords[i].x = (float)dcoord.x;
		  ring->segs[n]->coords[i].y = (float)dcoord.y;
		}
            }
	  n++;
	  if (edge_rec.coords)
            xvt_free ((char*)edge_rec.coords);
          edge_rec.coords = NULL;

	} /* if (!done) */
    } /* while */              
  ring->nr_segs = n;
  assert( ring->nr_segs <= maxsegs );

  /* Realloc the segs array to free unused memory */
  temp = (SEGMENT**)xvt_realloc(ring->segs, ring->nr_segs * sizeof (SEGMENT*));
  if( temp )
      ring->segs = temp;

  return TRUE;
} 

/*********************************************************************
  NEXT_FACE_EDGE                                                   
  Derived from next_face_edge   [vpfdraw.c]                     
  ********************************************************************/
int32 vrf_next_face_edge (edge_rec, prevnode, face_id)
     edge_rec_type *edge_rec;
     int32 *prevnode, face_id;
{
  int32 next;

  if ((edge_rec->right_face == face_id) && 
      (edge_rec->left_face == face_id)) {
    /* 
       Dangle - go the opposite dir to continue aint32 the boundary 
       */
    if (*prevnode == edge_rec->start_node) {
      edge_rec->dir = '+';
      next = edge_rec->right_edge;
      *prevnode = edge_rec->end_node;
    } else if (*prevnode == edge_rec->end_node) {
      edge_rec->dir = '-';
      next = edge_rec->left_edge;
      *prevnode = edge_rec->start_node;
    } else {
      next = -1;
    }
  } else if (edge_rec->right_face == face_id) {
    /* 
       The face is on the right - take the right forward edge 
       */
    next = edge_rec->right_edge;
    edge_rec->dir = '+';
    *prevnode = edge_rec->end_node;
  } else if (edge_rec->left_face == face_id) {
    /* 
       The face is on the left - take the left forward edge 
       */
    next = edge_rec->left_edge;
    edge_rec->dir = '-';
    *prevnode = edge_rec->start_node;
  }
  /*
   * I think we only end up here if the face information is wrong for some
   * reason.  I have this problem with most layers in some VITD datasets
   * 04KOREA (Edition 1) VITD data.  In this case we fall back to establishing
   * the correction edge direction based on the start and end node.
   * 
   * See bug 741854 on http://ogdi.sf.net/
   */
  else {
    if (*prevnode == edge_rec->start_node) {
      edge_rec->dir = '+';
      next = edge_rec->right_edge;
      *prevnode = edge_rec->end_node;
    } else if (*prevnode == edge_rec->end_node) {
      edge_rec->dir = '-';
      next = edge_rec->left_edge;
      *prevnode = edge_rec->start_node;
    } else {
      next = -1;
    }
  }

  return next;
}  

/*********************************************************************
  vrf_get_area_mbr

  Get the related mbr of a primitive face

  IN
     ecs_Server *s: ecs_Server structure
     ecs_Layer *layer: Layer information structure
     int prim_id: Primitive ID

  OUT
     return int: Error code. True if the function execute correctly,
     false else.
     double *xmin, *xmax, *ymin, *ymax: Returned bounding box
  
  ********************************************************************/

int vrf_get_area_mbr (layer, prim_id, xmin, ymin, xmax, ymax)
     ecs_Layer *layer;
     int32 prim_id;
     double *xmin;
     double *ymin;
     double *xmax;
     double *ymax;
{
  LayerPrivateData *lpriv = (LayerPrivateData *) layer->priv;
  return vrf_get_mbr(lpriv->l.area.mbrTable, prim_id, xmin, ymin, xmax, ymax);
}


/****************************************************************************

  vrf_get_xy

  Extract from the database the point contain at the "pos" column of
  the table "table" at the row "row". If the structure found is a list
  of coordinate, only the first one will be returned.

  IN
     vpf_table_type table :   Table of primitives (already open)
     row_type       row   :   Table row in "table"
     long           pos   :   Column position in "table" for "COORDINATE"

  OUT
     double *x,*y         :   Point extract from structure
     return int           :   This flag indicate the success or the failure
                              of the function.

  ***************************************************************************/

int vrf_get_xy (table, row, pos, x, y)
     vpf_table_type table;
     row_type       row;
     int32           pos;
     double         *x;
     double         *y;
{
  int32 count = 0;
  coordinate_type temp1, *ptr1;
  tri_coordinate_type temp2, *ptr2;
  double_coordinate_type temp3, *ptr3;
  double_tri_coordinate_type temp4, *ptr4;

  switch (table.header[pos].type) {
  case 'C': 
    {
      ptr1 = get_table_element (pos, row, table, &temp1, &count);
      
      if ((count == 1) && (ptr1 == (coordinate_type*)NULL)) {
	*x = (double) temp1.x;
	*y = (double) temp1.y;
      } else if( ptr1 == NULL ) {
        return FALSE;
      } else {
	*x = (double) ptr1->x;
	*y = (double) ptr1->y;
	if (ptr1)
	  free(ptr1);
      }
      break;
    }
  case 'Z':
    {
      ptr2 = get_table_element (pos, row, table, &temp2, &count);
      if ((count == 1) && (ptr2 == (tri_coordinate_type*)NULL)) {
	*x = temp2.x;
	*y = temp2.y;
      } else if( ptr2 == NULL ) {
        return FALSE;
      } else {
	*x = (double) ptr2[0].x;
	*y = (double) ptr2[0].y;
	if (ptr2)
	  xvt_free ((char*)ptr2);
      }
      break;
    }
  case 'B':
    {
      ptr3 = get_table_element (pos, row, table, &temp3, &count);
      if ((count == 1) && (ptr3 == (double_coordinate_type*)NULL)) {
	*x = (double) temp3.x;
	*y = (double) temp3.y;
      } else if( ptr3 == NULL ) {
        return FALSE;
      } else {
	*x = (double) ptr3[0].x;
	*y = (double) ptr3[0].y;
	if (ptr3)
	  xvt_free ((char*)ptr3);
      }
      break;
    }
  case 'Y':
    {
      ptr4 = get_table_element (pos, row, table, &temp4, &count);
      if ((count == 1) && (ptr4 == (double_tri_coordinate_type*)NULL)) {
	*x = (double) temp4.x;
	*y = (double) temp4.y;
      } else if( ptr4 == NULL ) {
        return FALSE;
      } else {
	*x = (double) ptr4[0].x;
	*y = (double) ptr4[0].y;
	if (ptr4)
	  xvt_free ((char*)ptr4);
      }    
      break;
    }
  default:
    break;
  } /* switch type */
  return TRUE;
}



/****************************************************************************

  vrf_get_ObjAttributes

  Get the attributes from the feature table and generate a string with
  it.

  IN
     vpf_table_type table :   Table of primitives (already open)
     int32 row_pos         :   Row position in table

  OUT
     return char *: The returned string. If NULL, the operation
     was unsuccessul.

  ***************************************************************************/

static char *returnString = NULL;

char *vrf_get_ObjAttributes(table, row_pos)
     vpf_table_type table;
     int32 row_pos;
{
  int i;
  char buffer[255];
  row_type row;
  int32 lenght;
  char temp1, *ptr1;
  float temp2;
  double temp3;
  short int temp4;
  int temp5;
  date_type temp6;
  int32 count;

  if (returnString != NULL) {
    free(returnString);
    returnString = NULL;
  }

  row = read_row(row_pos,table);

  lenght = 4;
  returnString = (char *) malloc(lenght);
  strcpy(returnString,"");

  for(i = 0; i < table.nfields; ++i) {
    switch(table.header[i].type) {
    case 'T':
    case 'L':
      ptr1 = get_table_element (i, row, table, &temp1, &count);
      if ((count == 1) && (ptr1 == (char *) NULL)) {
	lenght += 6;
	returnString = (char *) realloc(returnString,lenght);
	if (returnString == NULL) {
	  free_row(row,table);
	  return NULL;
	}
	sprintf(buffer,"%c",temp1);
	strcat(returnString,"{ ");
	strcat(returnString,buffer);
	strcat(returnString," } ");
      } else {
	lenght += count + 6;
	returnString = (char *) realloc(returnString,lenght);
	if (returnString == NULL) {
	  free_row(row,table);
	  free(ptr1);
	  return NULL;
	}
	strcat(returnString,"{ ");
	strcat(returnString,ptr1);
	strcat(returnString," } ");
	free(ptr1);
      }    
      break;
      /*added 5-28-97 case "D"  */
    case 'D':  /* Date */
      ptr1 = get_table_element (i, row, table, &temp6, &count);
      if ((count == 1) && (ptr1 == (char *) NULL)) {
	lenght += 5 + sizeof(date_type);  /*Changed from += 6 to += 5 + sizeof(date_type)*/
	returnString = (char *) realloc(returnString,lenght);
	if (returnString == NULL) {
	  free_row(row,table);
	  return NULL;
	}
	sprintf(buffer,"%20s",temp6);	/*dap Changed %c to %20s*/
	strcat(returnString,"{ ");
	strcat(returnString,buffer);
	strcat(returnString," } ");
      } else {
	/* Changed from += count + 6 to += 5 + (count * sizeof(date_type))*/	
	lenght += 5 + (count * sizeof(date_type));  
	returnString = (char *) realloc(returnString,lenght);
	if (returnString == NULL) {
	  free_row(row,table);
	  free(ptr1);
	  return NULL;
	}
	strcat(returnString,"{ ");
	strcat(returnString,ptr1);
	strcat(returnString," } ");
	free(ptr1);
      }    
      break;
   case 'F':
      get_table_element (i, row, table, &temp2, &count);
      sprintf(buffer,"%f",temp2);
      lenght += strlen(buffer) + 2;
      returnString = (char *) realloc(returnString,lenght);
      if (returnString == NULL) {
	free_row(row,table);
	return NULL;
      }
      strcat(returnString,buffer);
      strcat(returnString," ");
      break;
    case 'R':
      get_table_element (i, row, table, &temp3, &count);
      sprintf(buffer,"%f",temp3);
      lenght += strlen(buffer) + 2;
      returnString = (char *) realloc(returnString,lenght);
      if (returnString == NULL) {
	free_row(row,table);
	return NULL;
      }
      strcat(returnString,buffer);
      strcat(returnString," ");
      break;
    case 'S':
      get_table_element (i, row, table, &temp4, &count);
      sprintf(buffer,"%d",temp4);
      lenght += strlen(buffer) + 2;
      returnString = (char *) realloc(returnString,lenght);
      if (returnString == NULL) {
	free_row(row,table);
	return NULL;
      }
      strcat(returnString,buffer);
      strcat(returnString," ");
      break;
    case 'I':
      get_table_element (i, row, table, &temp5, &count);
      sprintf(buffer,"%d",temp5);
      lenght += strlen(buffer) + 2;
      returnString = (char *) realloc(returnString,lenght);
      if (returnString == NULL) {
	free_row(row,table);
	return NULL;
      }
      strcat(returnString,buffer);
      strcat(returnString," ");
      break;
    }
  }

  free_row(row,table);
  return returnString;
}

void vrf_free_ObjAttributeBuffer()

{
    if( returnString != NULL )
    {
        free( returnString );
        returnString = NULL;
    }
}

int vrf_checkLayerTables(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  register LayerPrivateData *lpriv;

  lpriv = (LayerPrivateData *) l->priv;
  switch(l->sel.F) {
  case Area:
    if (lpriv->l.area.faceTable.fp == NULL) {
      ecs_SetError(&(s->result), 1, "VRF table fac not open");
      return FALSE;
    }
    if (lpriv->l.area.mbrTable.fp == NULL) {
      ecs_SetError(&(s->result), 1, "VRF table mbr not open");
      return FALSE;
    }
    if (lpriv->l.area.ringTable.fp == NULL) {
      ecs_SetError(&(s->result), 1, "VRF table rng not open");
      return FALSE;
    }
    if (lpriv->l.area.edgeTable.fp == NULL) {
      ecs_SetError(&(s->result), 1, "VRF table edg not open");
      return FALSE;
    }

    break;
  case Line:
    if (lpriv->l.line.mbrTable.fp == NULL) {
      ecs_SetError(&(s->result), 1, "VRF table mbr not open");
      return FALSE;
    }
    if (lpriv->l.line.edgeTable.fp == NULL) {
      ecs_SetError(&(s->result), 1, "VRF table edg not open");
      return FALSE;
    }
    break;
  case Point:
    if (lpriv->l.point.primTable.fp == NULL) {
      ecs_SetError(&(s->result), 1, "VRF table end or cnd not open");
      return FALSE;
    }
    break;
  case Text:
    if (lpriv->l.text.textTable.fp == NULL) {
      ecs_SetError(&(s->result), 1, "VRF table txt not open");
      return FALSE;
    }
    break;
  default:
    return FALSE;
  }

  return TRUE;
}


