/******************************************************************************
 *
 * Component: OGDI Core C API
 * Purpose: Raster tile caching implementation.
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
 * $Log: ecstile.c,v $
 * Revision 1.5  2007/02/12 15:52:57  cbalint
 *
 *    Preliminary cleanup.
 *    Get rif of unitialized variables, and unused ones.
 *
 * Revision 1.4  2001/04/19 05:09:17  warmerda
 * fixed round off errors in calculation of t->linelength, and placed
 * coord.x/y at center of pixel instead of the corner.
 *
 * Revision 1.3  2001/04/09 15:04:34  warmerda
 * applied new source headers
 *
 */

#include "ecs.h"

ECS_CVSID("$Id: ecstile.c,v 1.5 2007/02/12 15:52:57 cbalint Exp $");

/*
   *******************************************************************
   

   FUNCTION_INFORMATION

        NAME
	   ecs_TileInitialize

        DESCRIPTION
	     -allows driver to describe the tile structure
        END_DESCRIPTION

        POSTCONDITIONS
             -tile structure allocated in ecs_Server, or error
	     -t->index == -1
        END_POSTCONDITIONS

        PARAMETERS
	    INPUT
	    ecs_Server *server:               the server structure.
	    ecs_Region *region:               the region of the database
	    int xtiles:                       the number of tiles along x axis	
	    int ytiles:                       the number of tiles along y axis
	    int callbackFunc():               the callback for i,j values
	    int tileDimFunc():           the callback for tile dimension.
                                              (pass NULL if none)
        END_PARAMETERS

        RETURN_VALUE
                <
		returns TRUE or FALSE if successful or not
                >
        PSEUDOCODE
		1) initialize the tile layout.
		2) ecs_TileStructure allocated, added to ecs_Result.
	        3) callback, closeTile procedures are registered.
	END_PSEUDOCODE

	
   END_FUNCTION_INFORMATION

   *******************************************************************
   
*/

/*
*******************************************************************

FUNCTION_INFORMATION

NAME

    ecs_TileInitialize

DESCRIPTION

    allows driver to describe the tile structure

END_DESCRIPTION

POSTCONDITIONS

    -tile structure allocated in ecs_Server, or error
    -t->index == -1

END_POSTCONDITIONS

PARAMETERS
INPUT

    ecs_Server *s: the server structure.
    ecs_TileStructure *t: The pointer to tile information.
    ecs_Region *region: the region of the database
    int xtiles: the number of tiles along x axis 
    int ytiles: the number of tiles along y axis
    int tilewidth: The tile width
    int tileheight: The tile height
    int callbackFunc(): the callback for i,j values
    int tileDimFunc(): the callback for tile dimension.
    (pass NULL if none)

END_PARAMETERS

RETURN_VALUE

    returns TRUE or FALSE if successful or not

END_FUNCTION_INFORMATION

PSEUDOCODE

    1) initialize the tile layout.
    2) ecs_TileStructure allocated, added to ecs_Result.
    3) callback, closeTile procedures are registered.

END_PSEUDOCODE

*******************************************************************
*/

int ecs_TileInitialize (s, t, region, xtiles, ytiles, tilewidth, tileheight, callbackFunc, tileDimFunc) 
    ecs_Server *s;
    ecs_TileStructure *t;
    ecs_Region *region;
    int xtiles;
    int ytiles;
    int tilewidth;
    int tileheight;
    tile_func *callbackFunc;
    tile_func *tileDimFunc;
{
  /*  static dirty=0;

  if (dirty) {
    ecs_TileDeleteAllLines(t);
  }
  dirty=1;
  */

  (void) s;

  t->width=tilewidth;
  t->height=tileheight;

  t->nb_lines=0;  
  t->index = -1;
  t->offValue=0;
  t->uninitializedValue=-1;
  t->currentTile.none=1;
  t->callback=callbackFunc;
  t->tileDimCallback=tileDimFunc;
  t->linelength= -1;
  t->region.north=region->north;
  t->region.south=region->south;
  t->region.east=region->east;
  t->region.west=region->west;
  t->region.ew_res=region->ew_res;
  t->region.ns_res=region->ns_res;
  t->regionwidth = (int) ((t->region.east - t->region.west) / t->region.ew_res);
  t->regionheight = (int) ((t->region.north - t->region.south) / t->region.ns_res);
  t->linebuffer=NULL;
  t->xtiles=xtiles;
  t->ytiles=ytiles;
  
  return TRUE;
}

/*
   *******************************************************************
   

   FUNCTION_INFORMATION

        NAME
	   ecs_TileGetLine

        DESCRIPTION
	     -allows driver to retrieve a line.
        END_DESCRIPTION

	PRECONDITIONS:
	    -callback must have been registered
	    -the length of a line must have been put into TileData
        POSTCONDITIONS
            tile structure allocated in ecs_Server, or error
        END_POSTCONDITIONS

        PARAMETERS
	    INPUT
	    ecs_TileStructure *server:     the tile structure.
	    ecs_Coordinate *start;  the start geog coordinate
	    ecs_Coordinate *end;    the ending geog coordinate
        END_PARAMETERS

        RETURN_VALUE
                <
		returns TRUE or FALSE if successful or not
                >
        PSEUDOCODE
	    1) determines the last point (if any) that has been set by 
	       previous calls to ecs_TileGetLine (the part which is sitting in the
	       buffer)	
	    2) examines remainder of the line to determine the tiles it crosses.
	       2a) for each point in the matrix,
	           2a1) if it is outside the tiles, add the "no_data" value.
	           2a2) else call the callback for each point in the line for 
	                each of these tiles, 
		   2a3) read the rest of the tile into the buffer for future lines.
	    3) goto (2) for each tile that intersects with the line until the easternmost 
	       point is reached. 
	    4) pad extra values with offValue
	    5) return an ecs_Result with a line (or error)
	
	END_PSEUDOCODE

	
   END_FUNCTION_INFORMATION

   *******************************************************************
*/

int ecs_TileGetLine(s,t,start,end)
     ecs_Server *s;
     ecs_TileStructure *t;
     ecs_Coordinate *start;
     ecs_Coordinate *end;
{
  int count, tmp, cat;
  register int y, i, j, firsttime, pix_x, pix_y;
  ecs_TileBufferLine *tbuf;
  ecs_Coordinate coord;  
  ecs_TileID tile_id;
  ecs_Layer *l;
  double ew_res, ns_res;
  register int offsetx,offsety;
  register double ratio_x,ratio_y;
  int posidres;
  
  offsetx = (int) ((s->currentRegion.west - t->region.west)/t->region.ew_res);
  offsety = (int) ((t->region.north - s->currentRegion.north)/t->region.ns_res);

  ratio_x = s->currentRegion.ew_res/t->region.ew_res;
  ratio_y = s->currentRegion.ns_res/t->region.ns_res;
  
  l=&(s->layer[s->currentLayer]);
  /*  get the id of this line */
  if (start->x >= end->x) {
    ecs_SetError(&(s->result), 1, "Coordinates are invalid");
    ecs_TileDeleteAllLines(t);
    return FALSE;    
  }
  
  /* calculate linelength */  
  if (t->linelength<0) {
    
    t->linelength = (int) (((end->x-start->x) / s->currentRegion.ew_res)+0.5);
    
  } else {
    tmp=(int) (((end->x - start->x) / s->currentRegion.ew_res)+0.5);

    /* check if resolution has changed */
    if (tmp!=t->linelength) {
      ecs_TileDeleteAllLines(t);
    };
    t->linelength=tmp;
  }
  
  /* initialize the ecs_result */
  ecs_SetGeomMatrix(&(s->result), t->linelength);
  
  /* if the line is not the first in the buffer, it isn't there, so
     it must be created. */
  
  /* the y value of this point */
  /* y=(int) ((s->currentRegion.north- start->y) / s->currentRegion.ns_res); */
  
  y=l->index;

  /* if the first line of the buffer isn't the current line, add it to the buffer */
  if (t->index != y) {
    ecs_TileAddLine(t,t->linelength, y, &tbuf);
  }
  
  firsttime=1;
  
  /* scan the first line of the buffer for uninitialized pixels. */
  for (count=0; count< t->linelength; count++) {
    if (t->linebuffer->linebuffer[count]==t->uninitializedValue) {
      if (s->rasterconversion.isProjEqual) {
	i=y;
	j=count;
      } else {
	i = ECSGETI(s,((double) y),((double) count));
	j = ECSGETJ(s,((double) y),((double) count));
      }
      
      if (t->tileDimCallback!=NULL) {
	coord.x=s->currentRegion.west+(j+0.5)*s->currentRegion.ew_res;    
	coord.y=s->currentRegion.north-(i+0.5)*s->currentRegion.ns_res;
	t->tileDimCallback(s, t, coord.x, coord.y, &(t->width), &(t->height));
	ew_res=1.0 / (double) t->width;
	ns_res=1.0 / (double) t->height;
	pix_x = (int) ((coord.x - t->region.west) / ew_res);
	pix_y = (int) ((t->region.north-coord.y) / ns_res);
	posidres = ecs_GetTileId(s, t, &coord, &tile_id);
      } else {
	pix_x = ((int) (j*ratio_x))+offsetx;
	pix_y = ((int) (i*ratio_y))+offsety;
	posidres = ecs_GetTileIdFromPos(s, t, pix_x, pix_y, &tile_id);
      }
      
      if (posidres) {
	if (!firsttime) {
	  /* if the tile_id changes, try to read the next line in the old tile */
	  if (! ecs_TileCompare(&(t->currentTile),&tile_id)) {
	    /* read the rest of the tile */
	    ecs_TileFill(s, t, y, &(t->currentTile));
	  }
	}
	firsttime=0;
	ecs_SetTile(&(t->currentTile), &tile_id);
	
	if (tile_id.x < 0 || tile_id.x >= t->xtiles ||
	    tile_id.y < 0 || tile_id.y >= t->ytiles) {
	  ECS_SETGEOMMATRIXVALUE((&(s->result)),count,t->offValue); 
	} else {
	  if (t->callback(s, t, tile_id.x, tile_id.y, pix_x % t->width, pix_y % t->height, &cat)) {
	    ECS_SETGEOMMATRIXVALUE((&(s->result)),count,cat); 
	  } else { 
	    ecs_TileDeleteAllLines(t);
	    printf("can't read pixel (%d,%d) in tile (%d,%d)\n", pix_x, pix_y, tile_id.x, tile_id.y);
	    ecs_SetError(&(s->result),1,"Unable to read matrix value");
	    return FALSE;
	  }
	}
      } else { /* out of bounds */
	ECS_SETGEOMMATRIXVALUE((&(s->result)),count,t->offValue); 
      }
    } else {
      ECS_SETGEOMMATRIXVALUE((&(s->result)),count,t->linebuffer->linebuffer[count]); 
    }
  }
  
  ecs_TileDeleteLine(t);
  ecs_SetSuccess(&(s->result));
  return TRUE;
}

/*
*******************************************************************

FUNCTION_INFORMATION

NAME

    ecs_ClearTileBuffer

DESCRIPTION

    Public function called by the driver during Select Layer

END_DESCRIPTION

PRECONDITIONS

    ecs_TileInitialize must have been called first.

END_PRECONDITIONS

PARAMETERS

    INPUT
    ecs_TileStructure *t;

END_PARAMETERS

RETURN_VALUE

    none

END_FUNCTION_INFORMATION

PSEUDOCODE

    1) call the destroyalllines procedure

END_PSEUDOCODE

*******************************************************************
*/

void ecs_TileClearBuffer(ecs_TileStructure *t) {
  
  ecs_TileDeleteAllLines(t);
  return;
}

/*
   *******************************************************************
   

   FUNCTION_INFORMATION

        NAME
	    ecs_TileFill

        DESCRIPTION
	    fill the buffer with data from a single tile.
        END_DESCRIPTION

	PRECONDITIONS
	    -there is at least one line in the buffer already.
	    -the "index" position in the matrix has already been
	    filled.
	END_PRECONDITIONS

        PARAMETERS
	    INPUT
	    ecs_TileStructure *s:        the server structure.
	    ecs_TileID *tile_id:  the tile_id of the tile to fill.
	    int index:            the y position in the matrix
        END_PARAMETERS

        RETURN_VALUE
                <
		returns TRUE or FALSE if successful or not
                >
        PSEUDOCODE
	  1) start at line index+2, position "last" (i.e. start from
	  the first unfilled pixel on the next line)
	  2) If the first pixel is in another tile, return.  (This means
	  that either the tile border has a negative slope (\) and the
	  left tile hasn't been filled, or that the tile has been completed).
	  3) Read the line until the end of the tile, then recurse.
	END_PSEUDOCODE

   END_FUNCTION_INFORMATION

   *******************************************************************
   
*/

int ecs_TileFill(ecs_Server *s, ecs_TileStructure *t, int index, ecs_TileID *current_tile_id) {

  register int i,j, bufptr, pix_x, pix_y;
  int cat,count;
  short firsttime=1;
  ecs_Coordinate coord;
  ecs_TileBufferLine *tbuf;
  ecs_TileID tile_id;
  double ew_res, ns_res;
  register int offsetx,offsety;
  register double ratio_x,ratio_y;
  int posidres;

  bufptr=index+1; /* the number of the next line in the buffer */

  offsetx = (int) ((s->currentRegion.west - t->region.west)/t->region.ew_res);
  offsety = (int) ((t->region.north - s->currentRegion.north)/t->region.ns_res);
  ratio_x = s->currentRegion.ew_res/t->region.ew_res;
  ratio_y = s->currentRegion.ns_res/t->region.ns_res;

  /* if the line isn't in the buffer, add it. */
  if (! ecs_TileFindBuffer(t, bufptr, &tbuf)) {
    if (! ecs_TileAddLine(t,t->linelength, bufptr, &tbuf)) {
      return FALSE;
    }
  }

  /*   return 1; */

  for (count=tbuf->last+1; count< t->linelength; count++) {
    if (tbuf->linebuffer[count]==t->uninitializedValue) {
      
      if (s->rasterconversion.isProjEqual) {
	
	j = count;
	i = bufptr;
	
      } else { /* if s->rasterconversion */
	
	i = ECSGETI(s,((double) bufptr),((double) count));
	j = ECSGETJ(s,((double) bufptr),((double) count));
	
      }

      if (t->tileDimCallback!=NULL) {
	coord.x=s->currentRegion.west+j*s->currentRegion.ew_res;    
	coord.y=s->currentRegion.north-i*s->currentRegion.ns_res;
	t->tileDimCallback(s, t, coord.x, coord.y, &(t->width), &(t->height));
	ew_res=1.0 / (double) t->width;
	ns_res=1.0 / (double) t->height;
	pix_x = (int) ((coord.x - t->region.west) / ew_res);
	pix_y = (int) ((t->region.north-coord.y) / ns_res);
	posidres = ecs_GetTileId(s, t, &coord, &tile_id);
      } else {
	pix_x = ((int) (j*ratio_x))+offsetx;
	pix_y = ((int) (i*ratio_y))+offsety;
	posidres = ecs_GetTileIdFromPos(s, t, pix_x, pix_y, &tile_id);
      }

      if (posidres) {
	
	/* if the tile_id changes, try to read the next line in the same tile,
	   unless this is the first pixel in the line. */
	if (! ecs_TileCompare(current_tile_id,&tile_id)) {
	  if (firsttime) {
	    /* the first value is not in the tile, so return. */
	    return TRUE;
	  } 
	  /* we're done this line, so read the rest of the tile recursively */
	  return (ecs_TileFill(s, t, bufptr, current_tile_id));
	  	  
	} /* ecs_TileCompare */
	firsttime=0; /* i.e. we've found a valid pixel */
	
	/* ecs_SetTile(&(t->currentTile), &tile_id); */
	
	/* i and j are BOGUS values for testing */

	if (tile_id.x < 0 || tile_id.x >= t->xtiles ||
	    tile_id.y < 0 || tile_id.y >= t->ytiles) {
	  tbuf->linebuffer[++(tbuf->last)]=t->offValue;
	} else {
	  if (t->callback(s, t, tile_id.x, tile_id.y, pix_x % t->width, pix_y %t->height, &cat)) {
	    tbuf->linebuffer[++(tbuf->last)]=cat;
	  } else { 
	    ecs_TileDeleteAllLines(t);
	    ecs_SetError(&(s->result),1,"Unable to read matrix value");
	    return FALSE;
	  }
	}
      } else { /* out of bounds */
	tbuf->linebuffer[++(tbuf->last)]=t->offValue;
      }
    } else {   /* t->linebuffer... is uninitialized */
#if TILE_DEBUG      
      printf("**** point redone. ***********\n");
#endif
    }
  }
  return TRUE;

}

/*
   *******************************************************************
   

   FUNCTION_INFORMATION

        NAME
	    ecs_GetTileId

        DESCRIPTION
	    Determine which tile a pair of matrix coords fit in.
        END_DESCRIPTION

	PRECONDITIONS:
	    ecs_TileStructure is initialized
	END_PRECONDITIONS

        PARAMETERS
	    INPUT
	    ecs_TileStructure *s:     the server structure.
	    int x_pixel:       the pixel values of the point
	    int y_pixel:
	    
        END_PARAMETERS

        RETURN_VALUE
                <
		returns TRUE or FALSE if successful or not
                >
        PSEUDOCODE

	
   END_FUNCTION_INFORMATION

   *******************************************************************
   
*/

int ecs_GetTileId(ecs_Server *s, ecs_TileStructure *t, ecs_Coordinate *coord, ecs_TileID *tile_id) {

  (void) s;

  if (coord->x < t->region.west || coord->x > t->region.east ||
      coord->y < t->region.south || coord->y > t->region.north) {
    tile_id->none=1;
    return FALSE;    
  }

  tile_id->x=(int) ((coord->x-t->region.west) / (t->region.east 
	      - t->region.west) * t->xtiles);
  tile_id->y=(int) ((t->region.north - coord->y) / (t->region.north
	      - t->region.south) * t->ytiles);
  tile_id->none=0;

  return TRUE;

}

/*
   *******************************************************************
   

   FUNCTION_INFORMATION

        NAME
	    ecs_GetTileIdFromPos

        DESCRIPTION
	    Determine which tile a pair of matrix coords fit in.
        END_DESCRIPTION

	PRECONDITIONS:
	    ecs_TileStructure is initialized
	END_PRECONDITIONS

        PARAMETERS
	    INPUT
	    ecs_TileStructure *s:     the server structure.
	    int x_pixel:       the pixel values of the point
	    int y_pixel:
	    
        END_PARAMETERS

        RETURN_VALUE
                <
		returns TRUE or FALSE if successful or not
                >
        PSEUDOCODE

	
   END_FUNCTION_INFORMATION

   *******************************************************************
   
*/

int ecs_GetTileIdFromPos(ecs_Server *s, ecs_TileStructure *t, int x, int y, ecs_TileID *tile_id) {

  (void) s;

  if (x < 0 || x > t->regionwidth ||
      y < 0 || y > t->regionheight) {
    tile_id->none=1;
    return FALSE;    
  }

  tile_id->x=(int) ((x * t->xtiles) / t->regionwidth);
  tile_id->y=(int) ((y * t->ytiles) / t->regionheight);
  tile_id->none=0;

  return TRUE;

}

/*
   *******************************************************************
   

   FUNCTION_INFORMATION

        NAME
	   ecs_TileFindBuffer

        DESCRIPTION
	    private function which finds a line in the buffer if it exists,
	    and returns a pointer to it.
        END_DESCRIPTION

	PRECONDITIONS:
	    ecs_TileStructure structure has been initialized.
	END_PRECONDITIONS

        POSTCONDITIONS
        END_POSTCONDITIONS

        PARAMETERS
	    INPUT
	    ecs_TileStructure *tile:     the server structure.
	    int bufptr                   the index of the line in the buffer
	    OUTPUT
	    tbuf                         pointer to the new line buffer.
        END_PARAMETERS

        RETURN_VALUE
                <
		returns TRUE or FALSE if successful or not
                >
        PSEUDOCODE
	    1) determine if the line is in the list.
	    2) if no, return FALSE.
	    3) set tbuf to point to the buffer line, return TRUE

	END_PSEUDOCODE
   END_FUNCTION_INFORMATION

   *******************************************************************
*/

int ecs_TileFindBuffer(ecs_TileStructure *t, int bufptr, ecs_TileBufferLine **tbuf) {
  ecs_TileBufferLine *tptr;

  if ( t->nb_lines <= 0) {
    tbuf=NULL;
    return FALSE;
  }

  if (bufptr < t->index || bufptr > t->index + t->nb_lines -1) {
    tbuf=NULL;
    return FALSE;
  }

  tptr=t->linebuffer;
  while (tptr!=NULL) {
    if (tptr->index==bufptr) {
      *tbuf=tptr;
      return TRUE;
    }
    tptr=tptr->next;
  }
#ifdef TILE_DEBUG  
  printf("   can't find buffer line %d; it's supposed to be there.\n",bufptr);
#endif
  tbuf=NULL;
  return FALSE;
}


/*
   *******************************************************************
   

   FUNCTION_INFORMATION

        NAME
	   ecs_TileAddLine

        DESCRIPTION
	    private function which adds a buffered line to the ecs_TileStructure structure
        END_DESCRIPTION

	PRECONDITIONS:
	    ecs_TileStructure structure has been initialized.	
	END_PRECONDITIONS

        POSTCONDITIONS
            extra line has been mallocked.  All values are set to the "uninitialized"
	    value.
        END_POSTCONDITIONS

        PARAMETERS
	    INPUT
	    ecs_TileStructure *tile:     the server structure.
	    int length                   the length of the buffer.
	    int index                    the index of the line.
	    OUTPUT
	    tbuf                         pointer to the new line buffer.
        END_PARAMETERS

        RETURN_VALUE
                <
		returns TRUE or FALSE if successful or not
                >
        PSEUDOCODE
	    1) malloc a line; set next pointers accordingly.
	    2) place line in ecs_tile
	    3) set last=-1, index to the y matrix (map pixel) value.
	    4) set all values to "uninitialized"
	END_PSEUDOCODE
   END_FUNCTION_INFORMATION

   *******************************************************************
   
*/

int ecs_TileAddLine(ecs_TileStructure *t, int length, int index, ecs_TileBufferLine **tbuf) {
  /* malloc buffer */
  register int i;
  ecs_TileBufferLine *tmp, *last=NULL;

#if 0
  printf("+++adding a line %d to buffer\n",index);
#endif
  /* insert new line at tail of list (FIFO)*/

  tmp=t->linebuffer;

  /* find the last buffer line */
  while (tmp != NULL) {
    last=tmp;
    tmp=tmp->next;
  }

  /* allocate the buffer */
  tmp=(ecs_TileBufferLine *) malloc (sizeof (ecs_TileBufferLine));
  if (!tmp) {
    return FALSE;
  }
  if (t->linebuffer==NULL) {
    t->linebuffer=tmp;
    t->index=index;
  } else {
    last->next=tmp;
  }

  /* allocate the space for the pixels */
  tmp->linebuffer=(int *) malloc (sizeof (int) * length);
  if (!tmp->linebuffer) {
    return FALSE;
  }

  tmp->next=NULL;
  tmp->index=index;
  tmp->last=-1;

  t->nb_lines++;

  /* set all values to "uninitialized" */
  for (i=0; i<length; i++) {
    tmp->linebuffer[i]=t->uninitializedValue;
  }
  *tbuf=tmp;
  return TRUE;

}

/*
   *******************************************************************
   

   FUNCTION_INFORMATION

        NAME
	   ecs_TileDeleteLine

        DESCRIPTION
	    removes a line from the buffer
        END_DESCRIPTION

	PRECONDITIONS:
	    The t->linebuffer *must* have been initialized before,
	    at least so it points to NULL.
	END_PRECONDITIONS

        POSTCONDITIONS
            deletes top line from the buffer.
	    decreases t->nb_lines by 1
        END_POSTCONDITIONS

        PARAMETERS
	    INPUT
	    ecs_TileStructure *tile:     the server structure.
	    int length              the length of the buffer.
	    
        END_PARAMETERS

        RETURN_VALUE
  	    returns TRUE if a line was deleted.  Else return FALSE.
        PSEUDOCODE
	    1) remove the top line from the buffer and free memory.  
	    If none, return "FALSE"
	END_PSEUDOCODE
	
   END_FUNCTION_INFORMATION

   *******************************************************************
   
*/

int ecs_TileDeleteLine(ecs_TileStructure *t) {
  ecs_TileBufferLine *tmp;

  /* free buffer if any */
  if (t->nb_lines==0) {
    return FALSE;
  }
#if 0
  fprintf(stdout, "---deleting line %d from buffer\n", t->index);
#endif
  t->nb_lines--;
  tmp=t->linebuffer->next;
  if (tmp != NULL) {
    t->index=tmp->index;
  } else {
    t->index = -1;
  }
  free(t->linebuffer->linebuffer);
  free(t->linebuffer);
  t->linebuffer=tmp;

  return TRUE;
}

/*
   *******************************************************************
   

   FUNCTION_INFORMATION

        NAME
	   ecs_TileDeleteAllLines

        DESCRIPTION
	    removes all lines from the buffer
        END_DESCRIPTION

	PRECONDITIONS:
	    none
	END_PRECONDITIONS

	POSTCONDITIONS:
	    t->nb_lines is 0
	END_POSTCONDITIONS

        PARAMETERS
	    INPUT
	    ecs_TileStructure *tile: the tile structure.
	    
        END_PARAMETERS

        RETURN_VALUE
	    none
        PSEUDOCODE
	    1) Keep calling ecs_TileDeleteLine until it returns FALSE.
	END_PSEUDOCODE
	
   END_FUNCTION_INFORMATION

   *******************************************************************
   
*/
	

void ecs_TileDeleteAllLines (ecs_TileStructure *t) {
  /*   fprintf(stdout, "removing buffer\n");  */
  while (ecs_TileDeleteLine(t)) {};
  t->nb_lines=0;
  
  return;
}

/*
   *******************************************************************
   

   FUNCTION_INFORMATION

        NAME
	   ecs_TileFind

        DESCRIPTION
	   returns true if within current tiles
        END_DESCRIPTION

	PRECONDITIONS:
	    none
	END_PRECONDITIONS

        PARAMETERS
	    INPUT	    
	    ecs_Server *s:         the server structure
	    ecs_Coordinate *coord: the coordinate to locate
	    OUTPUT
	    ecs_TileId *tile:       the tiling structure.
        END_PARAMETERS

        RETURN_VALUE
	    none
        PSEUDOCODE
	    1) Keep calling ecs_TileDeleteLine until it returns FALSE.
	END_PSEUDOCODE
	
   END_FUNCTION_INFORMATION

   *******************************************************************
   
*/


int ecs_TileCompare (ecs_TileID *id1, ecs_TileID *id2) {
  
  if (id1->none || id2->none) 
    return FALSE;

  if (id1->x==id2->x && id1->y==id2->y)
    return TRUE;
  return FALSE;
}

int ecs_SetTile (ecs_TileID *destination, ecs_TileID *source) {
  /* determine which tile x and y are in */
  
  destination->none=source->none;
  destination->x=source->x;
  destination->y=source->y;
  return TRUE;
}


