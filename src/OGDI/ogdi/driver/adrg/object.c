/******************************************************************************
 *
 * Component: OGDI ADRG Driver
 * Purpose: Implementation of ADRG getObject* functions.
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
 * Revision 1.7  2016/06/28 14:32:45  erouault
 * Fix all warnings about unused variables raised by GCC 4.8
 *
 * Revision 1.6  2007/02/12 16:09:06  cbalint
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
 * Revision 1.5  2001/06/22 16:37:50  warmerda
 * added Image support, upgraded headers
 *
 */

#include "ecs.h"
#include "adrg.h"

ECS_CVSID("$Id: object.c,v 1.7 2016/06/28 14:32:45 erouault Exp $");

/*
 *  --------------------------------------------------------------------------
 *  _LoadADRGTiles
 *   
 *      Load tiles in memory if it's not too big. The projection
 *      must be longlat. These tiles is a kind of buffer for a row
 *      of tiles.
 *  --------------------------------------------------------------------------
 */

void _LoadADRGTiles(s,l,UseOverview)
     ecs_Server *s;
     ecs_Layer *l;
     int *UseOverview;
{
  register ServerPrivateData *spriv = s->priv;
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  int i1,j1,i2,j2,prev_i,prev_j,i,count,tile,tile_physique;
  double y,prev_y;
  
  /* Found the first and the last request point in adrg matrix */
  
  y = s->currentRegion.north - l->index*s->currentRegion.ns_res;
  prev_y = s->currentRegion.north - (l->index-1)*s->currentRegion.ns_res;
  
  _calPosWithCoord(s,l,s->currentRegion.west,y,&i1,&j1,FALSE);
  _calPosWithCoord(s,l,s->currentRegion.east,y,&i2,&j2,FALSE);
  _calPosWithCoord(s,l,s->currentRegion.east,prev_y,&prev_i,&prev_j,FALSE);

  /* Found the first and last tile */

  i1 = i1/128;
  i2 = i2/128;
  j1 = j1/128;
  j2 = j2/128;
  prev_j = prev_j/128;
  
  /* Update the tile database */

  count = 0;
  if ((lpriv->buffertile == NULL) || (prev_j != j1) || (l->index == 0)) {

    if ((i2-i1) > MAXADRGTILES) {
      *UseOverview = TRUE;
      _calPosWithCoord(s,l,s->currentRegion.west,y,&i1,&j1,TRUE);
      _calPosWithCoord(s,l,s->currentRegion.east,y,&i2,&j2,TRUE);
      _calPosWithCoord(s,l,s->currentRegion.east,prev_y,&prev_i,&prev_j,TRUE);
      
      /* Found the first and last tile */
      
      i1 = i1/128;
      i2 = i2/128;
      j1 = j1/128;
      j2 = j2/128;
      prev_j = prev_j/128;

      if ((spriv->overview.buffertile == NULL) || (prev_j != j1) || (l->index == 0)) {

	if (spriv->overview.buffertile != NULL) {
	  free(spriv->overview.buffertile);
	  spriv->overview.buffertile = NULL;
	}
	
	if ((i2-i1) > MAXADRGTILES) {
	  return;
	}
	
	if (s->rasterconversion.isProjEqual == FALSE) {
	  return;
	}
	
	spriv->overview.firsttile = i1;
	spriv->overview.buffertile = malloc((i2-i1+1)*sizeof(tile_mem));
	
	for(i=i1;i<=i2;i++) {
	  tile = (j1 * spriv->overview.coltiles) + i;
	  if ((tile < 0) || 
	      (tile > (spriv->overview.coltiles*spriv->overview.rowtiles)))
	    tile_physique = 0;
	  else 
	    tile_physique = spriv->overview.tilelist[tile];
	  
	  if (tile_physique != 0) {
	    fseek(spriv->overview.imgfile,(spriv->overview.firstposition + (tile_physique-1)*49152) - 1,SEEK_SET);
	    ogdi_fread(spriv->overview.buffertile[count].data,49152,1,spriv->overview.imgfile);
	    spriv->overview.buffertile[count].isActive = TRUE;
	  } else {
	    spriv->overview.buffertile[count].isActive = FALSE;
	  }
	  
	  count++;
	}
	
      }
      return;
    } 

    if (lpriv->buffertile != NULL) {
      free(lpriv->buffertile);
      lpriv->buffertile = NULL;
    }

    *UseOverview = FALSE;

    if (s->rasterconversion.isProjEqual == FALSE) {
      if ((i2-i1) > MAXADRGTILES)
	*UseOverview = TRUE;
      return;
    }

    if ((lpriv->zonenumber == 9) ||
	(lpriv->zonenumber == 18)) {
      return;
    }

    lpriv->firsttile = i1;
    lpriv->buffertile = malloc((i2-i1+1)*sizeof(tile_mem));

    for(i=i1;i<=i2;i++) {
      tile = (j1 * lpriv->coltiles) + i;
      if ((tile < 0) || (tile > (lpriv->coltiles*lpriv->rowtiles)))
	tile_physique = 0;
      else 
	tile_physique = lpriv->tilelist[tile];

      if (tile_physique != 0) {
	fseek(lpriv->imgfile,(lpriv->firstposition + (tile_physique-1)*49152) - 1,SEEK_SET);
	ogdi_fread(lpriv->buffertile[count].data,49152,1,lpriv->imgfile);
	lpriv->buffertile[count].isActive = TRUE;
      } else {
	lpriv->buffertile[count].isActive = FALSE;
      }

      count++;
    }
  }
}


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
  int i,i2,j2;
  char buffer[128];
  static int UseOverview;

  int totalcol;
  /*int totalrow;*/
  int value;
  double pos;

  if (l->index >= l->nbfeature) {
    ecs_SetError(&(s->result),2,"End of selection");
    return;
  }

  _LoadADRGTiles(s,l,&UseOverview);

  totalcol = (int) ((s->currentRegion.east - s->currentRegion.west)/s->currentRegion.ew_res);
  /*totalrow = (int) ((s->currentRegion.north - s->currentRegion.south)/s->currentRegion.ns_res);*/
  ecs_SetGeomMatrix(&(s->result),totalcol);

  if (s->rasterconversion.isProjEqual) {
    for (i=0; i<totalcol; i++) {
      value = _calcPosValue(s,l,i,l->index,UseOverview);
      ECS_SETGEOMMATRIXVALUE((&(s->result)),i,value);
    }
  } else {
    for (i=0; i<totalcol; i++) {
      i2 = ECSGETI(s,((double) l->index),((double)i));
      j2 = ECSGETJ(s,((double) l->index),((double)i));
      value = _calcPosValue(s,l,j2,i2,UseOverview);
      
      ECS_SETGEOMMATRIXVALUE((&(s->result)),i,value);
    }
  }
  

  sprintf(buffer,"%d",l->index);
  if (!ecs_SetObjectId(&(s->result),buffer)) {
    return;
  }

  pos = s->currentRegion.north - l->index*s->currentRegion.ns_res;
  ECS_SETGEOMBOUNDINGBOX((&(s->result)),s->currentRegion.west,
			 pos+s->currentRegion.ns_res,
			 s->currentRegion.east,pos)


  l->index++;
  ecs_SetSuccess(&(s->result));
}

void 
_getObjectRaster(s,l,id)
     ecs_Server *s;
     ecs_Layer *l;
     char *id;
{
  int i,i2,j2;
  char buffer[128];
  int totalcol;
  /*int totalrow;*/
  int value;
  int index;
  double pos;

  index = atoi(id);

  if (index >= l->nbfeature) {
    ecs_SetError(&(s->result),2,"Bad index value");
    return;
  }

  totalcol = (int) ((s->currentRegion.east - s->currentRegion.west)/s->currentRegion.ew_res);
  /*totalrow = (int) ((s->currentRegion.north - s->currentRegion.south)/s->currentRegion.ns_res);*/
  ecs_SetGeomMatrix(&(s->result),totalcol);

  if (s->rasterconversion.isProjEqual) {
    for (i=0; i<totalcol; i++) {
      value = _calcPosValue(s,l,i,index,FALSE);
      ECS_SETGEOMMATRIXVALUE((&(s->result)),i,value);
    }
  } else {
    for (i=0; i<totalcol; i++) {
      i2 = ECSGETI(s,((double) index),((double)i));
      j2 = ECSGETJ(s,((double) index),((double)i));
      value = _calcPosValue(s,l,j2,i2,FALSE);
      ECS_SETGEOMMATRIXVALUE((&(s->result)),i,value);
    }
  }

  sprintf(buffer,"%d",index);
  if (!ecs_SetObjectId(&(s->result),buffer)) {
    return;
  }

  pos = s->currentRegion.north - index*s->currentRegion.ns_res;
  ECS_SETGEOMBOUNDINGBOX((&(s->result)),s->currentRegion.west,
			 pos+s->currentRegion.ns_res,
			 s->currentRegion.east,pos)

  ecs_SetSuccess(&(s->result));
}

void 
_getObjectIdRaster(s,l,coord)
     ecs_Server *s;
     ecs_Layer *l;
     ecs_Coordinate *coord;
{

  ecs_SetSuccess(&(s->result));
}	

void _rewindRasterLayer(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
}


int _calcPosValue(s,l,i,j,UseOverview)
     ecs_Server *s;
     ecs_Layer *l;
     int i;
     int j;
     int UseOverview;
{
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  register LayerPrivateData *ptrlpriv;
  register ServerPrivateData *spriv = s->priv;
  double pos_x, pos_y;
  int pix_c,pix_r;
  register int value,tile,tile_physique,tile_x,tile_y;
  register int tile_r,tile_c;
  register unsigned int Red,Green,Blue,PosRed;

  if (UseOverview == TRUE) {
    ptrlpriv = &(spriv->overview);
  } else {
    ptrlpriv = lpriv;
  }

  pos_x = s->currentRegion.west + i*s->currentRegion.ew_res;
  pos_y = s->currentRegion.north - j*s->currentRegion.ns_res;

  _calPosWithCoord(s,l,pos_x,pos_y,&pix_c,&pix_r,UseOverview);

  if ((pix_c>=0) && (pix_c<ptrlpriv->columns) &&
      (pix_r>=0) && (pix_r<ptrlpriv->rows)) {

    /* Trouver la tile correspondante a pix_c,pix_r */
    
    tile_x = pix_c/128;
    tile_y = pix_r/128;
    tile = (tile_y * ptrlpriv->coltiles) + tile_x;

    /* Trouver a quel numero de tile correspond cette tile */

    if ((tile < 0) || (tile > (ptrlpriv->coltiles*ptrlpriv->rowtiles)))
      tile_physique = 0;
    else 
      tile_physique = ptrlpriv->tilelist[tile];
    
    /* Si cette tile physique est 0, c'est une tile vide */
    
    if (tile_physique == 0) {
      value = 0;
    } else {
      tile_r = pix_r - tile_y * 128;
      tile_c = pix_c - tile_x * 128;

      if (ptrlpriv->buffertile != NULL) {
	tile_physique = tile_x - ptrlpriv->firsttile;

	if (ptrlpriv->buffertile[tile_physique].isActive == TRUE) {
	  PosRed = tile_r*128 + tile_c;
	  Red = (ptrlpriv->buffertile[tile_physique].data[PosRed]) / 43;
	  Green = (ptrlpriv->buffertile[tile_physique].data[PosRed+16384]) / 43;
	  Blue = (ptrlpriv->buffertile[tile_physique].data[PosRed+32768]) / 43;
	} else {
	  value = 0;
	  return value;
	}
      } else {
	if ((tile_physique--) < 0)
	  tile_physique = 0;
	
	PosRed = ptrlpriv->firstposition + tile_physique*49152 + tile_r*128 + tile_c;
	fseek(ptrlpriv->imgfile,PosRed-1,SEEK_SET);
	Red = ((unsigned int) getc(ptrlpriv->imgfile)) / 43;
	fseek(ptrlpriv->imgfile,16383,SEEK_CUR);
	Green = ((unsigned int) getc(ptrlpriv->imgfile)) / 43;
	fseek(ptrlpriv->imgfile,16383,SEEK_CUR);
	Blue = ((unsigned int) getc(ptrlpriv->imgfile)) / 43;
	
      }
      value = Red*36 + Green*6 + Blue + 1;
    }
  } else {
    value = 0;
  }
  
  return value;
}

/*
   --------------------------------------------------------------------

   Given an equilateral coordinate, calculate the position in the raster 

   --------------------------------------------------------------------
   */

void _calPosWithCoord(s,l,pos_x,pos_y,i,j,UseOverview)
     ecs_Server *s;
     ecs_Layer *l;
     double pos_x;
     double pos_y;
     int *i;
     int *j;
     int UseOverview;
{
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  register LayerPrivateData *ptrlpriv;
  register ServerPrivateData *spriv = s->priv;
  register long pix_c,pix_r;
  double x,y,x0,y0;
  double diff;

  if (UseOverview == TRUE) {
    ptrlpriv = &(spriv->overview);
  } else {
    ptrlpriv = lpriv;
  }

  /* The ARC zone is important in the x,y calculation. */

  if (lpriv->zonenumber == 9) {
    /* North pole case */

    x0 = (((double) ptrlpriv->BRV)/360.0) * (90.0 - ptrlpriv->PSO) * sin(ptrlpriv->LSO * PI / 180.0);
    y0 = (-1.0 * ((double) ptrlpriv->BRV)/360.0) * (90.0 - ptrlpriv->PSO) * cos(ptrlpriv->LSO * PI / 180.0);
    x = (((double) ptrlpriv->BRV)/360.0) * (90.0 - pos_x) * sin(pos_y * PI / 180.0);
    y = (-1.0 * ((double) ptrlpriv->BRV)/360.0) * (90.0 - pos_x) * cos(pos_y * PI / 180.0);

    pix_r = (int) (y0 - y);
    pix_c = (int) (x - x0);

  } else if (lpriv->zonenumber == 18) {
    /* South pole case */

    x0 = (((double) ptrlpriv->BRV)/360.0) * (90.0 + ptrlpriv->PSO) * sin(ptrlpriv->LSO * PI / 180.0);
    y0 = (((double) ptrlpriv->BRV)/360.0) * (90.0 + ptrlpriv->PSO) * cos(ptrlpriv->LSO * PI / 180.0);
    x = (((double) ptrlpriv->BRV)/360.0) * (90.0 + pos_x) * sin(pos_y * PI / 180.0);
    y = (((double) ptrlpriv->BRV)/360.0) * (90.0 + pos_x) * cos(pos_y * PI / 180.0);

    pix_r = (int) (y0 - y);
    pix_c = (int) (x - x0);

  } else {
    /* Non-polar zones */

    diff = pos_x - ptrlpriv->LSO;
    
    pix_r = (int) ((ptrlpriv->PSO - pos_y) * (ptrlpriv->BRV/360));
    pix_c = (int) (diff * (ptrlpriv->ARV/360));
  }

  *i = pix_c;
  *j = pix_r;

  return;
}



/*
 *  --------------------------------------------------------------------------
 *  _get*Object*Image: 
 *   
 *      a set of functions to acheive Line objects retrieval
 *  --------------------------------------------------------------------------
 */

void _getNextObjectImage(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  int i,i2,j2;
  char buffer[128];
  static int UseOverview;

  int totalcol;
  /*int totalrow;*/
  int value;
  double pos;

  if (l->index >= l->nbfeature) {
    ecs_SetError(&(s->result),2,"End of selection");
    return;
  }

  _LoadADRGTiles(s,l,&UseOverview);

  totalcol = (int) ((s->currentRegion.east - s->currentRegion.west)/s->currentRegion.ew_res);
  /*totalrow = (int) ((s->currentRegion.north - s->currentRegion.south)/s->currentRegion.ns_res);*/
  ecs_SetGeomImage(&(s->result),totalcol);

  if (s->rasterconversion.isProjEqual) {
    for (i=0; i<totalcol; i++) {
      value = _calcImagePosValue(s,l,i,l->index,UseOverview);
      ECS_SETGEOMIMAGEVALUE(&(s->result),i,value);
    }
  } else {
    for (i=0; i<totalcol; i++) {
      i2 = ECSGETI(s,((double) l->index),((double)i));
      j2 = ECSGETJ(s,((double) l->index),((double)i));
      value = _calcImagePosValue(s,l,j2,i2,UseOverview);
      
      ECS_SETGEOMIMAGEVALUE((&(s->result)),i,value);
    }
  }
  

  sprintf(buffer,"%d",l->index);
  if (!ecs_SetObjectId(&(s->result),buffer)) {
    return;
  }

  pos = s->currentRegion.north - l->index*s->currentRegion.ns_res;
  ECS_SETGEOMBOUNDINGBOX((&(s->result)),s->currentRegion.west,
			 pos+s->currentRegion.ns_res,
			 s->currentRegion.east,pos)


  l->index++;
  ecs_SetSuccess(&(s->result));
}

void 
_getObjectImage(s,l,id)
     ecs_Server *s;
     ecs_Layer *l;
     char *id;
{
  int i,i2,j2;
  char buffer[128];
  int totalcol;
  /*int totalrow;*/
  int value;
  int index;
  double pos;

  index = atoi(id);

  if (index >= l->nbfeature) {
    ecs_SetError(&(s->result),2,"Bad index value");
    return;
  }

  totalcol = (int) ((s->currentRegion.east - s->currentRegion.west)/s->currentRegion.ew_res);
  /*totalrow = (int) ((s->currentRegion.north - s->currentRegion.south)/s->currentRegion.ns_res);*/
  ecs_SetGeomImage(&(s->result),totalcol);

  if (s->rasterconversion.isProjEqual) {
    for (i=0; i<totalcol; i++) {
      value = _calcImagePosValue(s,l,i,index,FALSE);
      ECS_SETGEOMMATRIXVALUE((&(s->result)),i,value);
    }
  } else {
    for (i=0; i<totalcol; i++) {
      i2 = ECSGETI(s,((double) index),((double)i));
      j2 = ECSGETJ(s,((double) index),((double)i));
      value = _calcImagePosValue(s,l,j2,i2,FALSE);
      ECS_SETGEOMIMAGEVALUE((&(s->result)),i,value);
    }
  }

  sprintf(buffer,"%d",index);
  if (!ecs_SetObjectId(&(s->result),buffer)) {
    return;
  }

  pos = s->currentRegion.north - index*s->currentRegion.ns_res;
  ECS_SETGEOMBOUNDINGBOX((&(s->result)),s->currentRegion.west,
			 pos+s->currentRegion.ns_res,
			 s->currentRegion.east,pos)

  ecs_SetSuccess(&(s->result));
}

void 
_getObjectIdImage(s,l,coord)
     ecs_Server *s;
     ecs_Layer *l;
     ecs_Coordinate *coord;
{

  ecs_SetSuccess(&(s->result));
}	

void _rewindImageLayer(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
}


int _calcImagePosValue(s,l,i,j,UseOverview)
     ecs_Server *s;
     ecs_Layer *l;
     int i;
     int j;
     int UseOverview;
{
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  register LayerPrivateData *ptrlpriv;
  register ServerPrivateData *spriv = s->priv;
  double pos_x, pos_y;
  int pix_c,pix_r;
  register int value,tile,tile_physique,tile_x,tile_y;
  register int tile_r,tile_c;
  register unsigned int Red,Green,Blue,PosRed;

  if (UseOverview == TRUE) {
    ptrlpriv = &(spriv->overview);
  } else {
    ptrlpriv = lpriv;
  }

  pos_x = s->currentRegion.west + i*s->currentRegion.ew_res;
  pos_y = s->currentRegion.north - j*s->currentRegion.ns_res;

  _calPosWithCoord(s,l,pos_x,pos_y,&pix_c,&pix_r,UseOverview);

  if ((pix_c>=0) && (pix_c<ptrlpriv->columns) &&
      (pix_r>=0) && (pix_r<ptrlpriv->rows)) {

    /* Trouver la tile correspondante a pix_c,pix_r */
    
    tile_x = pix_c/128;
    tile_y = pix_r/128;
    tile = (tile_y * ptrlpriv->coltiles) + tile_x;

    /* Trouver a quel numero de tile correspond cette tile */

    if ((tile < 0) || (tile > (ptrlpriv->coltiles*ptrlpriv->rowtiles)))
      tile_physique = 0;
    else 
      tile_physique = ptrlpriv->tilelist[tile];
    
    /* Si cette tile physique est 0, c'est une tile vide */
    
    if (tile_physique == 0) {
      value = ecs_GetPixelFromRGB(0,0,0,0);
    } else {
      tile_r = pix_r - tile_y * 128;
      tile_c = pix_c - tile_x * 128;

      if (ptrlpriv->buffertile != NULL) {
	tile_physique = tile_x - ptrlpriv->firsttile;

	if (ptrlpriv->buffertile[tile_physique].isActive == TRUE) {
	  PosRed = tile_r*128 + tile_c;
	  Red = (ptrlpriv->buffertile[tile_physique].data[PosRed]);
	  Green = (ptrlpriv->buffertile[tile_physique].data[PosRed+16384]);
	  Blue = (ptrlpriv->buffertile[tile_physique].data[PosRed+32768]);
	} else {
	  value = ecs_GetPixelFromRGB(0,0,0,0);
	  return value;
	}
      } else {
	if ((tile_physique--) < 0)
	  tile_physique = 0;
	
	PosRed = ptrlpriv->firstposition + tile_physique*49152 + tile_r*128 + tile_c;
	fseek(ptrlpriv->imgfile,PosRed-1,SEEK_SET);
	Red = ((unsigned int) getc(ptrlpriv->imgfile));
	fseek(ptrlpriv->imgfile,16383,SEEK_CUR);
	Green = ((unsigned int) getc(ptrlpriv->imgfile));
	fseek(ptrlpriv->imgfile,16383,SEEK_CUR);
	Blue = ((unsigned int) getc(ptrlpriv->imgfile));
	
      }
      value = ecs_GetPixelFromRGB(1,Red,Green,Blue);
    }
  } else {
    value = ecs_GetPixelFromRGB(0,0,0,0);
  }
  
  return value;
}







