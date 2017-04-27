/******************************************************************************
 *
 * Component: OGDI DTED Driver
 * Purpose: Implementation of DTED Server getObject* functions
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
 * Revision 1.8  2016/06/28 14:32:45  erouault
 * Fix all warnings about unused variables raised by GCC 4.8
 *
 * Revision 1.7  2007/02/12 15:52:57  cbalint
 *
 *    Preliminary cleanup.
 *    Get rif of unitialized variables, and unused ones.
 *
 * Revision 1.6  2001/04/19 05:04:59  warmerda
 * move pos_y to middle of row, add errors for unsupported funcs
 *
 * Revision 1.5  2001/04/10 14:29:43  warmerda
 * Upgraded with changes from DND (hand applied to avoid losing bug fixes).
 * Patch also includes change to exclude zero elevations when computing
 * mincat/maxcat.
 * New style headers also applied.
 *
 */

#include "ecs.h"
#include "dted.h"

ECS_CVSID("$Id: object.c,v 1.8 2016/06/28 14:32:45 erouault Exp $");

/*
 *  --------------------------------------------------------------------------
 *  _get*Object*Raster: 
 *   
 *      a set of functions to acheive Line objects retrieval
 *  --------------------------------------------------------------------------
 */


void _getNextObjectRaster(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  ServerPrivateData *spriv = s->priv;
  ecs_Coordinate start, end;
  double pos_y;
  /*double record_y;*/

  if (l->index >= l->nbfeature) {
    ecs_SetError(&(s->result),2,"End of selection");
    return;
  }

  /* get geographic position of current region's matrix point l->index */
  pos_y = s->currentRegion.north - (l->index+0.5) * s->currentRegion.ns_res;  

  /* get the corresponding point from the global region's matrix */
  /*record_y = (int) ((s->globalRegion.north-pos_y) / s->globalRegion.ns_res);*/
  
  start.x=s->currentRegion.west;
  end.x=s->currentRegion.east;
  start.y=pos_y;
  end.y=pos_y;
  
  if (!ecs_TileGetLine(s, &(spriv->t), &start, &end)) {
    ecs_SetError(&(s->result),1,"Unable to retrieve a line.");
    return;
  }
  if (l->sel.F == Image) {
    s->result.res.ecs_ResultUnion_u.dob.geom.family = Image;    
  }
  
  l->index++;
  ecs_SetSuccess(&(s->result));

}

void 
_getObjectRaster(s,l,id)
     ecs_Server *s;
     ecs_Layer *l;
     char *id;
{
  (void) l;
  (void) id;
  ecs_SetError(&(s->result),1,
               "GetObject(id) not supported for DTED.");
}

void 
_getObjectIdRaster(s,l,coord)
     ecs_Server *s;
     ecs_Layer *l;
     ecs_Coordinate *coord;
{
  (void) l;
  (void) coord;

  ecs_SetError(&(s->result),1,
               "GetObjectIdFromCoord() not supported for DTED.");
}	

void _rewindRasterLayer(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  ServerPrivateData *spriv = s->priv;
  ecs_TileClearBuffer(&(spriv->t));
  l->index=0;
}


/* returns a raw category value. */
int _getRawValue(ecs_Server *s, ecs_TileStructure *t, int xtile,
		 int ytile, int xpixel, int ypixel, int *cat) {

  char dtedfilename[512];
  unsigned char number[2];
  int32 filepos;
  unsigned char pos1;
  unsigned char pos2;
  int x,y;
  LayerPrivateData *lpriv = (LayerPrivateData *) s->layer[s->currentLayer].priv;
  ServerPrivateData *spriv= (ServerPrivateData *) s->priv;

  x=xpixel;
  y=spriv->ewdir[xtile].nsfile[ytile].rows-ypixel;
  
  /* check if we're off the map */
  if (x < 0 || y < 0 || y >= spriv->ewdir[xtile].nsfile[ytile].rows || x >= spriv->ewdir[xtile].nsfile[ytile].columns) {
    *cat=t->offValue;
    return TRUE;
  }

  /* check if tile appears in database */

  if (! spriv->ewdir[xtile].nsfile[ytile].used) {
    *cat = t->offValue;
    return TRUE;
  }
  
  /* open file if necessary, close old file */
  if ((spriv->lastTile.none==0) || spriv->lastTile.x != xtile 
				   || spriv->lastTile.y != ytile) { 
    if (spriv->lastTile.none) {
      fclose(spriv->ewdir[spriv->lastTile.x].nsfile[spriv->lastTile.y].filehandle);
    }
    /*(  if (spriv->ewdir[xtile].nsfile[ytile].filehandle == -1) */ 

    strcpy(dtedfilename,spriv->pathname);
    strcat(dtedfilename,"/");
    strcat(dtedfilename,spriv->ewdir[xtile].name);
    strcat(dtedfilename,"/");
    strcat(dtedfilename,spriv->ewdir[xtile].nsfile[ytile].name);
    
#ifdef _WINDOWS
    spriv->ewdir[xtile].nsfile[ytile].filehandle = fopen(dtedfilename, "rb");
#else
    spriv->ewdir[xtile].nsfile[ytile].filehandle = fopen(dtedfilename, "r");
#endif

    if (!spriv->ewdir[xtile].nsfile[ytile].filehandle) {
      return FALSE;
    }
      
    if (!_read_dted(s,xtile,ytile))
      return FALSE;
    
    spriv->lastTile.none=1;
    spriv->lastTile.x=xtile;
    spriv->lastTile.y=ytile;

  } /* end open file */

    if (lpriv->isInRam) {
      /*       filepos = (x)*(12+lpriv->rows*2);*/
      filepos = (x)*(12+spriv->ewdir[xtile].nsfile[ytile].rows*2);
      if (filepos < 0)
	filepos = 0;
      filepos += 8 + y*2;

      pos1 = lpriv->matrixbuffer[filepos];
      pos2 = lpriv->matrixbuffer[filepos+1];
      if(pos1 & 128) {
	/*
	  pos1 &= 127;
	  *cat = -((pos1<<8) + pos2);
	  */
	*cat = 0;
      } else {
	*cat = (pos1<<8) + pos2;
      }
    } else {
      /* ZZZ */
      /*       filepos = (x)*(12+lpriv->rows*2);*/

      filepos = (x)*(12+spriv->ewdir[xtile].nsfile[ytile].rows*2);
      if (filepos < 0)
	filepos = 0;
      filepos += 8 + y*2 + spriv->firstcoordfilepos;

      fseek(spriv->ewdir[xtile].nsfile[ytile].filehandle, filepos, SEEK_SET);

      if (fread(&number,1,2,spriv->ewdir[xtile].nsfile[ytile].filehandle) < 2) {
	return FALSE;
      }

      if(number[0] & 128) {
	/*
	number[0] &= 127;
	*cat = (256*number[0] + number[1]);
	*/
	*cat = 0;
      } else {
	*cat = 256*number[0] + number[1];
      }
    }
  return TRUE;
}

/* returns TRUE/FALSE, puts category in "cat" */
/* scales category to be between 1-216 */
/* pre: expects t->height to be set to the tileheight */
/* if no tile, sets to "0" */

int _calcPosValue(ecs_Server *s, ecs_TileStructure *t, int xtile,
		  int ytile, int xpixel, int ypixel, int *cat ) {
  ServerPrivateData *spriv=s->priv;
  LayerPrivateData *lpriv = (LayerPrivateData *)s->layer[s->currentLayer].priv;
  ecs_Region tile;
  double tile_width;
  double tile_height;

  if (xtile < 0 || xtile >= spriv->xtiles ||
      ytile < 0 || ytile >= spriv->ytiles) {
    *cat = t->offValue;
    return TRUE;
  }

  if (! spriv->ewdir[xtile].nsfile[ytile].used) {
    *cat = t->offValue;
    return TRUE;
  }
  tile_width=(s->globalRegion.east-s->globalRegion.west) / spriv->xtiles;
  tile_height=(s->globalRegion.north-s->globalRegion.south) / spriv->ytiles;

  /* the boundaries of the logical tile (as the tiling library sees it) */
  tile.east=s->globalRegion.west+tile_width * (xtile+1);
  tile.west=s->globalRegion.west+tile_width * xtile;

  tile.north=s->globalRegion.north-tile_height * (ytile);
  tile.south=s->globalRegion.north-tile_height * (ytile + 1);
  
  /* transform if the data Region is different than the tile region. */
  xpixel= (int) (xpixel-(spriv->ewdir[xtile].nsfile[ytile].region.west-tile.west)/
			 spriv->ewdir[xtile].nsfile[ytile].region.ew_res);
  ypixel= (int) (ypixel-(tile.north -spriv->ewdir[xtile].nsfile[ytile].region.north)/
			 spriv->ewdir[xtile].nsfile[ytile].region.ns_res);

  _getRawValue(s,t,xtile,ytile,xpixel,ypixel,cat);

  if (lpriv->family == Matrix) {
    /* catch stragglers */
    if (*cat < spriv->mincat) {
      *cat = spriv->mincat;
    }
    if (*cat > spriv->maxcat) {
      *cat=spriv->maxcat;
    }
    *cat = *cat - spriv->mincat;  /* ensure non-negative */
    
    if (spriv->maxcat-spriv->mincat > 215) {
      *cat = *cat * 215/(spriv->maxcat-spriv->mincat) +1;
    }
    if (*cat > 216) { 
      *cat = 216;
    }
  } 
  return TRUE;
}

/* figures out what the "global" resolution is for this coordinate */
/* x, y : point in the tile */

int _getTileDim(ecs_Server *s, ecs_TileStructure *t, double x, double y, 
                int *width, int  *height) {
  int ns_int, ew_int=0;  /* interval value in seconds*/
  ServerPrivateData *spriv= s->priv;

  (void) t;
  (void) x;

  if (y < 0)
    y=-y;
  if (y <= 50.0) {
    ew_int=1;
  } else if ((50.0 < y) && (y <= 70.0)) {
    ew_int=2;
  } else if ((70.0 < y) && (y <= 75.0)) {
    ew_int=3;	
  } else if ((75.0 < y) && (y <= 80.0)) {
    ew_int=4;
  } else if (y > 80.0) {
    ew_int=6;
  }

  switch (spriv->level) {
  case 0: 
    ns_int= 30;
    ew_int= ew_int * 30;
    break;
  case 1: 
    ns_int= 3;
    ew_int*= 3;
    break;
  case 2:
    /* ns_int=1 */
    ns_int= 1;
    ew_int*= 1;
    break;
  default:
    return FALSE;
    break;
  }
  *width=3600/ew_int+1;
  *height=3600/ns_int+1;
  return TRUE;
}

/* ========================================== 
  Test procedures to be used without an lpriv value 
   */

int _sample_getRawValue(ecs_Server *s, ecs_TileStructure *t, int xtile,
		 int ytile, int xpixel, int ypixel, int *cat) {
  char *dtedfilename;
  unsigned char number[2];
  int32 filepos;

  int x,y;
  ServerPrivateData *spriv= (ServerPrivateData *) s->priv;
  static int32 firstcoordfilepos;

  x=xpixel;
  y=t->height-ypixel;
  
  /* check if tile appears in database */

  if (! spriv->ewdir[xtile].nsfile[ytile].used) {
    *cat = t->offValue;
    return TRUE;
  }
  
  /* open file if necessary, close old file */
  if ((spriv->lastTile.none==0) || spriv->lastTile.x != xtile 
				   || spriv->lastTile.y != ytile) { 
    if (spriv->lastTile.none) {
      fclose(spriv->ewdir[spriv->lastTile.x].nsfile[spriv->lastTile.y].filehandle);
    }
    /*(  if (spriv->ewdir[xtile].nsfile[ytile].filehandle == -1) */ 

    dtedfilename = (char *) malloc(strlen(spriv->pathname)+strlen(spriv->ewdir[xtile].name)+strlen(spriv->ewdir[xtile].nsfile[ytile].name)+3);
    if (dtedfilename == NULL) {
      return FALSE;
    }    
    strcpy(dtedfilename,spriv->pathname);
    strcat(dtedfilename,"/");
    strcat(dtedfilename,spriv->ewdir[xtile].name);
    strcat(dtedfilename,"/");
    strcat(dtedfilename,spriv->ewdir[xtile].nsfile[ytile].name);
    
#ifdef _WINDOWS
    spriv->ewdir[xtile].nsfile[ytile].filehandle = fopen(dtedfilename, "rb");
#else
    spriv->ewdir[xtile].nsfile[ytile].filehandle = fopen(dtedfilename, "r");
#endif
    free(dtedfilename);
    if (!spriv->ewdir[xtile].nsfile[ytile].filehandle) {
      return FALSE;
    }
      
    if (!_sample_read_dted(s,xtile,ytile, &firstcoordfilepos, spriv->ewdir[xtile].nsfile[ytile].filehandle))
      return FALSE;
    
    spriv->lastTile.none=1;
    spriv->lastTile.x=xtile;
    spriv->lastTile.y=ytile;

  } /* end open file */

  /*       filepos = (x)*(12+lpriv->rows*2);*/
  filepos = (x)*(12+spriv->ewdir[xtile].nsfile[ytile].rows*2);

  if (filepos < 0)
    filepos = 0;
  filepos += 8 + y*2 + firstcoordfilepos;
  
  fseek(spriv->ewdir[xtile].nsfile[ytile].filehandle, filepos, SEEK_SET);

  if (fread(&number, 1,2,spriv->ewdir[xtile].nsfile[ytile].filehandle) < 2) {
    fclose(spriv->ewdir[xtile].nsfile[ytile].filehandle);
    return FALSE;
  }
  
  if(number[0] & 128) {
    *cat = 0;
  } else {
    *cat = 256*number[0] + number[1];
  }


  return TRUE;
}


