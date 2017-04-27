/******************************************************************************
 *
 * Component: OGDI RPF Driver
 * Purpose: Implementation of RPF getObject functions.
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
 * Revision 1.3  2001/04/12 19:22:46  warmerda
 * applied DND support Image type support
 *
 */

#include "ecs.h"
#include "rpf.h"

ECS_CVSID("$Id: object.c,v 1.3 2001/04/12 19:22:46 warmerda Exp $");

/*
*******************************************************************

FUNCTION_INFORMATION

NAME

    _getNextObjectRaster

DESCRIPTION

    Pick a line in the line buffer and return it to the OGDI.

END_DESCRIPTION

PRECONDITIONS

    A dyn_SelectLayer must have been call previously succesfully.

END_PRECONDITIONS

POSTCONDITIONS

    l->index is incremented of 1. 

END_POSTCONDITIONS

PARAMETERS

    INPUT

        ecs_Server *s: The driver information
        ecs_Layer *l: The current layer

END_PARAMETERS

END_FUNCTION_INFORMATION

PSEUDOCODE

    If the line index l->index is bigger than the number of lines (l->nbfeature)
    Begin

        Return the error code 2 end of selection

    End

    Calculate the extremities of the lines in the current projection:

        Point1:
        x: s->currentRegion.west
        y: s->currentRegion.north - l->index * s->currentRegion.ns_res

        Point2:
        x: s->currentRegion.east
        y: s->currentRegion.north - l->index * s->currentRegion.ns_res

    Call ecs_GetTileLine with these two points. If an error occur,
    return the error "Unable to retreive the line".

    Increment l->index

    Set a success message

END_PSEUDOCODE

*******************************************************************
*/

void dyn_getNextObjectRaster(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  LayerPrivateData *lpriv = l->priv;
  ecs_Coordinate start, end;
  double pos_y;

  if (l->index >= l->nbfeature) {
    ecs_SetError(&(s->result),2,"End of selection");
    return;
  }

  /* get geographic position of current region's matrix point l->index */
  pos_y = s->currentRegion.north - l->index * s->currentRegion.ns_res;  
  
  start.x=s->currentRegion.west;
  end.x=s->currentRegion.east;
  start.y=pos_y;
  end.y=pos_y;
  
  if (!ecs_TileGetLine(s, &(lpriv->tilestruct), &start, &end)) {
    ecs_SetError(&(s->result),1,"Unable to retrieve a line.");
    return;
  }
  
  l->index++;
  ecs_SetSuccess(&(s->result));
}

void 
dyn_getObjectRaster(s,l,id)
     ecs_Server *s;
     ecs_Layer *l;
     char *id;
{
  (void) l;
  (void) id;
  ecs_SetError(&(s->result),1,"The GetObject function is not implemented for this adapter");
}

void 
dyn_getObjectIdRaster(s,l,coord)
     ecs_Server *s;
     ecs_Layer *l;
     ecs_Coordinate *coord;
{
  (void) l;
  (void) coord;
  ecs_SetError(&(s->result),1,"The GetObjectIdFromCoord function is not implemented for this adapter");
}	

/*
*******************************************************************

FUNCTION_INFORMATION

NAME

    _rewindRasterLayer

DESCRIPTION

    Prepare a new selection.

END_DESCRIPTION

PRECONDITIONS

    A dyn_SelectLayer must have been call previously succesfully.

END_PRECONDITIONS

POSTCONDITIONS

    The layer is reinitialized and ready to be used

END_POSTCONDITIONS

PARAMETERS

    INPUT

        ecs_Server *s: The driver information
        ecs_Layer *l: The current layer

END_PARAMETERS

END_FUNCTION_INFORMATION

PSEUDOCODE

    Call ecs_TileClearBuffer

    Set l->index to 0.

END_PSEUDOCODE

*******************************************************************
*/

void dyn_rewindRasterLayer(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  LayerPrivateData *lpriv = l->priv;

  (void) s;

  ecs_TileClearBuffer(&(lpriv->tilestruct));
  l->index=0;
}

/*
*******************************************************************

FUNCTION_INFORMATION

NAME

    _PointCallBack

DESCRIPTION

    This callback function will return the value of a given point
    in a given tile.

END_DESCRIPTION

PRECONDITIONS

    A dyn_SelectLayer must have been call previously succesfully.

END_PRECONDITIONS

POSTCONDITIONS

    The tile may have change if the tile requested was not the
    same than the previous one.

END_POSTCONDITIONS

PARAMETERS

    INPUT

        ecs_Server *s: The driver information
        ecs_TileStructure *t: The tile structure
        int xtile: The tile column in the tile grid
        int ytile: The tile line in the tile grid
        int xpixel: The x position of the point inside the current tile
        int ypixel: The y position of the point inside the current tile

    OUTPUT

        int *cat: The return value.

END_PARAMETERS

RETURN_VALUE

    int: Indicate a success or an error message.

END_FUNCTION_INFORMATION

PSEUDOCODE

    Call _rpf_readtile for the tile xtile,ytile.

    If lpriv->isActive is FALSE, return 0 for cat.

    Calculate the pixel position: (xpixel,lpriv->columns-ypixel)

    If the point position is inside the region of the raster,
    Greater to 0 but smaller than the number of columns or lines
    Begin

        Calculate the subtile position in the current tile
        tile x = pixel position x / 256
        tile y = pixel position y / 256
        physical tile position = tile y * 6 + tile x

        Calculate the rows and column position of the pixel in this tile
        tile row = pixel position x - tile x * 256
        tile column = pixel position y - tile y* 256

        Get the value of the current point in this tile
        Value = (lpriv->buffertile[physical tile position].data[tile row * 256 + tile column]+13

        The return value cat receive the equivalent value of Value
        cat = lpriv->equi_cat[Value]

    End

    Else return a 0 value for cat.

END_PSEUDOCODE

*******************************************************************
*/

int dyn_PointCallBack(s,t,xtile,ytile,xpixel,ypixel,cat)

    ecs_Server *s;
    ecs_TileStructure *t;
    int xtile;
    int ytile;
    int xpixel;
    int ypixel;
    int *cat;

{
  ecs_Layer *l = (ecs_Layer *) &(s->layer[s->currentLayer]);
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  int pix_c,pix_r;
  register int tile,tile_x,tile_y;
  register int tile_r,tile_c;
  register unsigned int Val,PosVal;
  register double rapport;

  (void) t;

  rapport = s->currentRegion.ns_res/lpriv->entry->vert_interval;
  if (rapport > 10.0) {
    if (lpriv->entry->frames[ytile][xtile].exists == FALSE)
      *cat = 0;
    else {
      if (xpixel < 100 || xpixel > 1436 ||
	  ypixel < 100 || ypixel > 1436)
	*cat = ((lpriv->entry->boundary_id+1)*4)%216;
      else
	*cat = 0;
    }
    return TRUE;
  } 

  if (!dyn_read_rpftile(s,l,xtile,ytile)) {
    *cat = 0;
    return TRUE;
  }

  if (lpriv->isActive == FALSE) {
    *cat = 0;
    return TRUE;
  }
  
  pix_c = xpixel;
  pix_r = ypixel;
  
  if ((pix_c>=0) && (pix_c<lpriv->columns) &&
      (pix_r>=0) && (pix_r<lpriv->rows))
    {

      /* Compute subframe numbers for pix_c,pix_r */
	
      tile_x = pix_c/256;
      tile_y = pix_r/256;
      tile = tile_y*6+tile_x;
      
      /* Si cette tile physique est 0, c'est une tile vide */
      
      tile_r = pix_r - tile_y * 256;
      tile_c = pix_c - tile_x * 256;
      
      PosVal = tile_r*256 + tile_c;

      Val = (lpriv->buffertile[tile].data[PosVal]);
      *cat = lpriv->equi_cat[Val];
    }	  
  else
    *cat = 0;
  
return TRUE;
}

/*******************************************************************

FUNCTION_INFORMATION

NAME

    _DimCallBack

DESCRIPTION

    This callback function will return the resolution of the tile
    for the geographic point in it.

END_DESCRIPTION

PRECONDITIONS

    A dyn_SelectLayer must have been call previously succesfully.

END_PRECONDITIONS

PARAMETERS

    INPUT

        ecs_Server *s: The driver information
        ecs_TileStructure *t: The tile structure
        double x: The x of the geographic position
        double y: The y of the geographic position

    OUTPUT

        int *width: Width of the tile at the position x
        int *height: Height of the tile at the position y

END_PARAMETERS

RETURN_VALUE

    int: Indicate a success or an error message.

END_FUNCTION_INFORMATION

PSEUDOCODE

    Set 1536 to *width and *height

    return TRUE

END_PSEUDOCODE

*******************************************************************
*/

int dyn_DimCallBack(s,t,x,y,width,height)
    ecs_Server *s;
    ecs_TileStructure *t;
    double x;
    double y;
    int *width;
    int *height;
{
  (void) s;
  (void) t;
  (void) x;
  (void) y;

  *width = 1536;
  *height = 1536;
  return TRUE;
}


/*
*******************************************************************

FUNCTION_INFORMATION

NAME

    _getNextObjectImage

DESCRIPTION

    Pick a line in the line buffer and return it to the OGDI.

END_DESCRIPTION

PRECONDITIONS

    A dyn_SelectLayer must have been call previously succesfully.

END_PRECONDITIONS

POSTCONDITIONS

    l->index is incremented of 1. 

END_POSTCONDITIONS

PARAMETERS

    INPUT

        ecs_Server *s: The driver information
        ecs_Layer *l: The current layer

END_PARAMETERS

END_FUNCTION_INFORMATION

PSEUDOCODE

    If the line index l->index is bigger than the number of lines (l->nbfeature)
    Begin

        Return the error code 2 end of selection

    End

    Calculate the extremities of the lines in the current projection:

        Point1:
        x: s->currentRegion.west
        y: s->currentRegion.north - l->index * s->currentRegion.ns_res

        Point2:
        x: s->currentRegion.east
        y: s->currentRegion.north - l->index * s->currentRegion.ns_res

    Call ecs_GetTileLine with these two points. If an error occur,
    return the error "Unable to retreive the line".

    Increment l->index

    Set a success message

END_PSEUDOCODE

*******************************************************************
*/

void dyn_getNextObjectImage(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  LayerPrivateData *lpriv = l->priv;
  ecs_Coordinate start, end;
  double pos_y;

  if (l->index >= l->nbfeature) {
    ecs_SetError(&(s->result),2,"End of selection");
    return;
  }

  /* get geographic position of current region's matrix point l->index */
  pos_y = s->currentRegion.north - l->index * s->currentRegion.ns_res;  
  
  start.x=s->currentRegion.west;
  end.x=s->currentRegion.east;
  start.y=pos_y;
  end.y=pos_y;
  
  if (!ecs_TileGetLine(s, &(lpriv->tilestruct), &start, &end)) {
    ecs_SetError(&(s->result),1,"Unable to retrieve a line.");
    return;
  }

  s->result.res.ecs_ResultUnion_u.dob.geom.family = Image;
  
  l->index++;
  ecs_SetSuccess(&(s->result));
}

void 
dyn_getObjectImage(s,l,id)
     ecs_Server *s;
     ecs_Layer *l;
     char *id;
{
  ecs_SetError(&(s->result),1,"The GetObject function is not implemented for this adapter");
}

void 
dyn_getObjectIdImage(s,l,coord)
     ecs_Server *s;
     ecs_Layer *l;
     ecs_Coordinate *coord;
{
  ecs_SetError(&(s->result),1,"The GetObjectIdFromCoord function is not implemented for this adapter");
}	

/*
*******************************************************************

FUNCTION_INFORMATION

NAME

    _rewindImageLayer

DESCRIPTION

    Prepare a new selection.

END_DESCRIPTION

PRECONDITIONS

    A dyn_SelectLayer must have been call previously succesfully.

END_PRECONDITIONS

POSTCONDITIONS

    The layer is reinitialized and ready to be used

END_POSTCONDITIONS

PARAMETERS

    INPUT

        ecs_Server *s: The driver information
        ecs_Layer *l: The current layer

END_PARAMETERS

END_FUNCTION_INFORMATION

PSEUDOCODE

    Call ecs_TileClearBuffer

    Set l->index to 0.

END_PSEUDOCODE

*******************************************************************
*/

void dyn_rewindImageLayer(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  LayerPrivateData *lpriv = l->priv;
  ecs_TileClearBuffer(&(lpriv->tilestruct));
  l->index=0;
}

/*
*******************************************************************

FUNCTION_INFORMATION

NAME

    _PointCallBack

DESCRIPTION

    This callback function will return the value of a given point
    in a given tile.

END_DESCRIPTION

PRECONDITIONS

    A dyn_SelectLayer must have been call previously succesfully.

END_PRECONDITIONS

POSTCONDITIONS

    The tile may have change if the tile requested was not the
    same than the previous one.

END_POSTCONDITIONS

PARAMETERS

    INPUT

        ecs_Server *s: The driver information
        ecs_TileStructure *t: The tile structure
        int xtile: The tile column in the tile grid
        int ytile: The tile line in the tile grid
        int xpixel: The x position of the point inside the current tile
        int ypixel: The y position of the point inside the current tile

    OUTPUT

        int *cat: The return value.

END_PARAMETERS

RETURN_VALUE

    int: Indicate a success or an error message.

END_FUNCTION_INFORMATION

PSEUDOCODE

    Call _rpf_readtile for the tile xtile,ytile.

    If lpriv->isActive is FALSE, return 0 for cat.

    Calculate the pixel position: (xpixel,lpriv->columns-ypixel)

    If the point position is inside the region of the Image,
    Greater to 0 but smaller than the number of columns or lines
    Begin

        Calculate the subtile position in the current tile
        tile x = pixel position x / 256
        tile y = pixel position y / 256
        physical tile position = tile y * 6 + tile x

        Calculate the rows and column position of the pixel in this tile
        tile row = pixel position x - tile x * 256
        tile column = pixel position y - tile y* 256

        Get the value of the current point in this tile
        Value = (lpriv->buffertile[physical tile position].data[tile row * 256 + tile column]+13

        The return value cat receive the equivalent value of Value
        cat = lpriv->equi_cat[Value]

    End

    Else return a 0 value for cat.

END_PSEUDOCODE

*******************************************************************
*/

int dyn_ImagePointCallBack(s,t,xtile,ytile,xpixel,ypixel,cat)

    ecs_Server *s;
    ecs_TileStructure *t;
    int xtile;
    int ytile;
    int xpixel;
    int ypixel;
    int *cat;

{
  ecs_Layer *l = (ecs_Layer *) &(s->layer[s->currentLayer]);
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  int pix_c,pix_r;
  register int tile,tile_x,tile_y;
  register int tile_r,tile_c;
  register unsigned int Val,PosVal;
  register double rapport;
  
  rapport = s->currentRegion.ns_res/lpriv->entry->vert_interval;
  if (rapport > 10.0) {
    if (lpriv->entry->frames[ytile][xtile].exists == FALSE) {
      *cat = ecs_GetPixelFromRGB(0,0,0,0);
    } else {
      if (xpixel < 100 || xpixel > 1436 ||
	  ypixel < 100 || ypixel > 1436) {
	*cat = ecs_GetPixelFromRGB(1,0,255,0);
      } else {
	*cat = ecs_GetPixelFromRGB(0,0,0,0);
      }
      return TRUE;
    } 
  }    

  if (!dyn_read_rpftile(s,l,xtile,ytile)) {
    *cat = ecs_GetPixelFromRGB(0,0,0,0);
    return TRUE;
  }
  
  if (lpriv->isActive == FALSE) {
    *cat = ecs_GetPixelFromRGB(0,0,0,0);
    return TRUE;
  }
  
  pix_c = xpixel;
  pix_r = ypixel;
  
  if ((pix_c>=0) && (pix_c<lpriv->columns) &&
      (pix_r>=0) && (pix_r<lpriv->rows)) {
    
    /* Compute subframe numbers for pix_c,pix_r */
    
    tile_x = pix_c/256;
    tile_y = pix_r/256;
    tile = tile_y*6+tile_x;
    
    /* Si cette tile physique est 0, c'est une tile vide */
    
    tile_r = pix_r - tile_y * 256;
    tile_c = pix_c - tile_x * 256;
    
    PosVal = tile_r*256 + tile_c;
    
    Val = (lpriv->buffertile[tile].data[PosVal]);
    *cat = ecs_GetPixelFromRGB(1,lpriv->rgb_pal[Val].r,
				lpriv->rgb_pal[Val].g,
				lpriv->rgb_pal[Val].b);
  } else {
    *cat = ecs_GetPixelFromRGB(0,0,0,0);
  }
  return TRUE;
}
