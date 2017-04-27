/******************************************************************************
 *
 * Component: OGDI DTED Driver
 * Purpose: External (dyn_*) entry points for DTED driver.
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
 * $Log: dted.c,v $
 * Revision 1.10  2001/06/13 17:17:40  warmerda
 * fixed capabilities to match 6.2 spec
 *
 * Revision 1.9  2001/04/19 05:04:12  warmerda
 * fixed roundoff issues with computing nbfeature
 *
 * Revision 1.8  2001/04/10 16:18:28  warmerda
 * added ogdi_server_capabilities, and ogdi_capabilities support
 *
 * Revision 1.7  2001/04/10 14:29:43  warmerda
 * Upgraded with changes from DND (hand applied to avoid losing bug fixes).
 * Patch also includes change to exclude zero elevations when computing
 * mincat/maxcat.
 * New style headers also applied.
 *
 */

#include "dted.h"
#include "datadict.h"

ECS_CVSID("$Id: dted.c,v 1.10 2001/06/13 17:17:40 warmerda Exp $");

/* layer oriented functions are keeped in data structure to simplify code */

LayerMethod dted_layerMethod[11] = {  
  /* 0 */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Area */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Line */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Point */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Matrix */	{ NULL, NULL, _rewindRasterLayer, _getNextObjectRaster, _getObjectRaster, _getObjectIdRaster },
  /* Image */	{ NULL, NULL, _rewindRasterLayer, _getNextObjectRaster, _getObjectRaster, _getObjectIdRaster },
  /* Text */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Edge */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Face */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Node */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Ring */	{ NULL, NULL, NULL, NULL, NULL, NULL }
};

/* ----------------------------------------------------------------------
 *  _dyn_CreateServer: 
 *     
 *   Creation of a new S server
 * ----------------------------------------------------------------------
 */


ecs_Result *dyn_CreateServer(s,Request)
     ecs_Server *s;
     char *Request;
{
  ServerPrivateData *spriv = s->priv =
      (void *) calloc(1,sizeof(ServerPrivateData));

  (void) Request;
  
  if (spriv == NULL) {
    ecs_SetError(&(s->result),1,
                 "Not enough memory to allocate server private data");
    return &(s->result);
  }

  spriv->pathname = (char *) malloc(strlen(s->pathname)+1);
  if (spriv->pathname == NULL) {
    free(s->priv);
    ecs_SetError(&(s->result),1,"Not enough memory");
    return &(s->result);
  }  

  if (s->pathname[2] == ':') {
    strcpy(spriv->pathname,s->pathname+1);
  } else {
    strcpy(spriv->pathname,s->pathname);
  }

  /* check the path and see if the location is valid */

  if (!_verifyLocation(s)) {
    free(spriv->pathname);
    free(s->priv);
    return &(s->result);		
  }

  /* initialize the driver globalRegion */
  /*
  if (!_initRegionWithDefault(s)) {
    free(spriv->pathname);
    free(s->priv);
    return &(s->result);
  }
  */
  /* initialize with the info from the database */

  /* set the last-opened tile to "none" */
  spriv->lastTile.x=spriv->lastTile.y=-1;
  spriv->lastTile.none=0;
  
  if (!_readValuesFromDirList(s)) {
    free(spriv->pathname);
    free(s->priv);
    return &(s->result);
  }

  if (! _readDMED(s)) {
    ecs_SetError(&(s->result), 1, "Unable to reconstruct missing DMED file.");
    free(spriv->pathname);
    free(s->priv);
    return &(s->result);        
  }

  /* 1,1 are placeholders; the width is calculated from _getResolution */
  if (!ecs_TileInitialize( s, &(spriv->t), &(s->globalRegion), spriv->xtiles,
                           spriv->ytiles, 1, 1, _calcPosValue, _getTileDim)) { 
    ecs_SetError(&(s->result), 1, "Unable to retrieve tile structure.");
    free(spriv->pathname);
    free(s->priv);
    return &(s->result);    
 }

  if (! _sample_tiles(s,&(spriv->t))) {
    ecs_SetError(&(s->result), 1, "Unable to sample data to set colour table.");
    free(spriv->pathname);
    free(s->priv);
    return &(s->result);    
  }

  s->nblayer = 0; 

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
  ServerPrivateData *spriv = s->priv;
  int               i;

  /* Release all layer */
  
  _releaseAllLayers(s);

  if(spriv != NULL) {
    ecs_TileClearBuffer(&(spriv->t));
    if (spriv->pathname != NULL) {
      free(spriv->pathname);
    }
    for( i=0; i < spriv->xtiles; i++ )
    {
        if( spriv->ewdir != NULL && spriv->ewdir[i].nsfile != NULL )
            free( spriv->ewdir[i].nsfile );
    }
    if( spriv->ewdir != NULL )
        free( spriv->ewdir );
        
    free(spriv);
  }
  
  ecs_SetSuccess(&(s->result));
  return &(s->result);

}

void _freelayerpriv(lpriv)
     LayerPrivateData *lpriv;
{
  if (lpriv != NULL) {
    if (lpriv->ewdir != NULL)
      free(lpriv->ewdir);
    
    if (lpriv->matrixbuffer != NULL)
      free(lpriv->matrixbuffer);
    free(lpriv);
  }
  lpriv = NULL;
}

/* ----------------------------------------------------------------------
 *  _dyn_SelectLayer: 
 *     
 *      Select or reselect a layer
 * ----------------------------------------------------------------------
 */

ecs_Result *dyn_SelectLayer(s,sel)
     ecs_Server *s;
     ecs_LayerSelection *sel;
{
  int layer;
  /*   char *ewdir,*nsfile; */
  LayerPrivateData *lpriv;
  /*  char *dtedfilename; */
  ServerPrivateData *spriv = s->priv;

  if (sel->F != Matrix && sel->F != Image) {
    ecs_SetError(&(s->result),1,"Invalid layer type");
    return &(s->result);
  }

  /* first, try to find an existing layer with same request and family */
  if (spriv->lastTile.none) {
    fclose(spriv->ewdir[spriv->lastTile.x].nsfile[spriv->lastTile.y].filehandle);
    spriv->lastTile.x=spriv->lastTile.y=-1;
    spriv->lastTile.none=0;
  }

  if ((layer = ecs_GetLayer(s,sel)) != -1) {

    /* if it already exists than assign currentLayer and set index to 0 to force rewind */

    s->currentLayer = layer;
    _rewindRasterLayer(s,&(s->layer[layer]));
    ecs_SetGeoRegion(&(s->result),s->globalRegion.north, s->globalRegion.south, 
		     s->globalRegion.east, s->globalRegion.west, s->globalRegion.ns_res, 
		     s->globalRegion.ew_res);
    ecs_SetSuccess(&(s->result));
    return &(s->result);
  }

  /* it did not exists so we are going to try to create it */

  if ((layer = ecs_SetLayer(s,sel)) == -1) {
    return &(s->result);
  }
	
  /* allocate memory to hold private info about this new layer */

  if ((s->layer[layer].priv = (void *) malloc(sizeof(LayerPrivateData))) == NULL) {
    ecs_SetError(&(s->result),1,"Not enough memory to allocate layer private data");
    ecs_FreeLayer(s,layer);
    return &(s->result);	
  }
  lpriv = (LayerPrivateData *) s->layer[layer].priv;
  lpriv->ewdir = NULL;
  lpriv->nsfile = NULL;
  lpriv->matrixbuffer = NULL;
  lpriv->family = sel->F;

  if (!_parse_request(s,sel->Select,&(lpriv->isInRam))) {
    _freelayerpriv(layer);
    ecs_FreeLayer(s,layer);
    return &(s->result);
  }

  s->currentLayer = layer;
  s->layer[layer].nbfeature = (int) 
      ((s->currentRegion.north - s->currentRegion.south)
       /s->currentRegion.ns_res + 0.5);

  ecs_SetGeoRegion(&(s->result),s->globalRegion.north, s->globalRegion.south, 
		   s->globalRegion.east, s->globalRegion.west, 
                   s->globalRegion.ns_res, s->globalRegion.ew_res);

  ecs_SetSuccess(&(s->result));
  return &(s->result);
}


/* ----------------------------------------------------------------------
 *  dyn_GetAttributesFormat: 
 *     
 *      return the attribute format of the currently selected layer
 * ----------------------------------------------------------------------
 */

ecs_Result *
dyn_GetAttributesFormat(s)
     ecs_Server *s;
{
  ecs_SetObjAttributeFormat(&(s->result));

  ecs_AddAttributeFormat(&(s->result),"category",Integer,5,0,0);
  ecs_AddAttributeFormat(&(s->result),"label",Char,80,0,0);	

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
  ServerPrivateData *spriv=s->priv;
  LayerPrivateData *lpriv;

  /* first, try to find an existing layer with same request and family */

  if ((layer = ecs_GetLayer(s,sel)) == -1) {
    sprintf(buffer,"Invalid layer %s",sel->Select);
    ecs_SetError(&(s->result),1,buffer);
    return &(s->result);
  }
  lpriv = (LayerPrivateData *) s->layer[layer].priv;
  
  if (lpriv != NULL) {
    _freelayerpriv(lpriv);    
    ecs_FreeLayer(s,layer);
    
    if (s->currentLayer == layer) {
      s->currentLayer = -1;		/* just in case released layer was selected */
    }
  }
  if (spriv->lastTile.none) {
    fclose(spriv->ewdir[spriv->lastTile.x].nsfile[spriv->lastTile.y].filehandle);
    spriv->lastTile.x=spriv->lastTile.y=-1;
    spriv->lastTile.none=0;
  
  }
  ecs_SetSuccess(&(s->result));
  return &(s->result);
}

/* deselect all layer */

void
_releaseAllLayers(s)
     ecs_Server *s;
{
  int i;

  for (i = 0; i < s->nblayer; ++i)
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
	
  s->currentRegion.north = gr->north;
  s->currentRegion.south = gr->south;
  s->currentRegion.east = gr->east;			
  s->currentRegion.west = gr->west;
  s->currentRegion.ns_res = gr->ns_res;
  s->currentRegion.ew_res = gr->ew_res;

  /* reset currentLayer index to 0 to force rewind */

  if (s->currentLayer != -1) {
    _rewindRasterLayer(s,&(s->layer[s->currentLayer]));
    s->layer[s->currentLayer].nbfeature = (int) ((s->currentRegion.north - s->currentRegion.south)/s->currentRegion.ns_res + 0.5);
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
  if (ecs_SetText(&(s->result),datadict)) {
    ecs_SetSuccess(&(s->result));
  }
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

  if (s->layer[s->currentLayer].index == 0) {
    (dted_layerMethod[s->layer[s->currentLayer].sel.F].rewind)(s,&(s->layer[s->currentLayer]));
  }
  
  (dted_layerMethod[s->layer[s->currentLayer].sel.F].getNextObject)(s,&(s->layer[s->currentLayer]));
    
    return &(s->result);
    
}

/* ----------------------------------------------------------------------
 *  dyn_GetRasterInfo: 
 *     
 *      return raster layer meta information
 * ----------------------------------------------------------------------
 */

ecs_Result *dyn_GetRasterInfo(s)
     ecs_Server *s;
{
    int k;
    int intensity;
    char buffer[256];
    LayerPrivateData *lpriv;
    int limit1,limit2;
    int max, min, range;
    double m,b;

    ServerPrivateData *spriv = s->priv;
    lpriv = (LayerPrivateData *) s->layer[s->currentLayer].priv;

    if (lpriv->family == Matrix) {
        /* rules for re-categorization:
           1) if there are more than 216 categories, rescale to 1-216.
           2) else recategorize to 1 to (maxcat-mincat) + 1.
        */

        range=spriv->maxcat-spriv->mincat;
        min=1;
        if (range<216) {
            max=range+1;
        } else {
            max=216;
        }
        limit1 = (int) (((max - min) / 3.0) + min);
        limit2 = (int) ((2*(max - min) / 3.0) + min);

        /* Put table contain in RasterInfo here */

        ecs_SetRasterInfo(&(s->result),100, 100);
        for (k=min;k<=max;k++) {
            if (spriv->maxcat-spriv->mincat<216) {
                sprintf(buffer, "%d", k+spriv->mincat);
            } else {
                sprintf(buffer,"%d",(k-1)*(spriv->maxcat-spriv->mincat)/215 + spriv->mincat);
            }
            m = 242.0 / (limit1 - min);

            if (k < limit1) {
                b = 255-m*limit1;
                intensity = (int) (m*((double) k)+b);
                if (intensity >= 255)
                    intensity = 255;
                if (intensity <= 13)
                    intensity = 13;
                ecs_AddRasterInfoCategory(&(s->result),k,0,0,intensity,buffer,0);
            } else {
                if (k > limit2) {
                    b = 255-m*max;
                    intensity = (int) (m*((double) k)+b);
                    if (intensity >= 255)
                        intensity = 255;
                    if (intensity <= 13)
                        intensity = 13;
                    ecs_AddRasterInfoCategory(&(s->result),k,intensity,0,0,buffer,0);
                } else {
                    b = 255-m*limit2;
                    intensity = (int) (m*((double) k)+b);
                    if (intensity >= 255)
                        intensity = 255;
                    if (intensity <= 13)
                        intensity = 13;
                    ecs_AddRasterInfoCategory(&(s->result),k,0,intensity,0,buffer,0);
                }
            }
        }
    } else {
        ecs_SetRasterInfo(&(s->result),5,0);
        ecs_AddRasterInfoCategory(&(s->result),1, 255, 255, 255,"No data",0);
        s->result.res.ecs_ResultUnion_u.ri.mincat = spriv->mincat;
        s->result.res.ecs_ResultUnion_u.ri.maxcat = spriv->maxcat;
    }
    ecs_SetSuccess(&(s->result));
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
  if (dted_layerMethod[s->layer[s->currentLayer].sel.F].getObject != NULL) 
    (dted_layerMethod[s->layer[s->currentLayer].sel.F].getObject)(s,&(s->layer[s->currentLayer]),Id);
  else {
    ecs_SetError(&(s->result),1,"Can't get object for this type of layer");
  }
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
  if (dted_layerMethod[s->layer[s->currentLayer].sel.F].getObjectIdFromCoord != NULL) 
    (dted_layerMethod[s->layer[s->currentLayer].sel.F].getObjectIdFromCoord)(s,&(s->layer[s->currentLayer]),coord);
  else {
    ecs_SetError(&(s->result),1,"Can't get objectid from coordinate for this type of layer");
  }
  return &(s->result);
}

/* ----------------------------------------------------------------------
 *  _dyn_UpdateDictionary: 
 *     
 *   Return the content of this location data dictionary in a Tcl List
 * ----------------------------------------------------------------------
 */


ecs_Result *dyn_UpdateDictionary(s,info)
     ecs_Server *s;
     char *info;
{
  ServerPrivateData *spriv = s->priv;

  if( strcmp(info,"ogdi_server_capabilities") == 0 )
  {
      ecs_AddText(&(s->result),
                  "<?xml version=\"1.0\" ?>\n"
                  "<OGDI_Capabilities version=\"3.1\">\n"
                  "</OGDI_Capabilities>\n" );
      ecs_SetSuccess(&(s->result));
  }

  else if( strcmp(info,"ogdi_capabilities") == 0 )
  {
      char		line[256];

      ecs_AddText(&(s->result),
                  "<?xml version=\"1.0\" ?>\n"
                  "<OGDI_Capabilities version=\"3.1\">\n" );
      ecs_AddText(&(s->result),
                  "   <FeatureTypeList>\n"
                  "      <Operations>\n"
                  "         <Query/>\n"
                  "      </Operations>\n"
                  "      <FeatureType>\n" );

      sprintf( line, "         <Name>%s(RAM)</Name>\n", spriv->layername);
      ecs_AddText(&(s->result),line);

      sprintf( line, "         <SRS>PROJ4:%s</SRS>\n", PROJ_LONGLAT );
      ecs_AddText(&(s->result),line);

      sprintf(line, 
              "         <LatLonBoundingBox minx=\"%.9f\"  miny=\"%.9f\"\n"
              "                            maxx=\"%.9f\"  maxy=\"%.9f\" />\n",
              s->globalRegion.west, s->globalRegion.south, 
              s->globalRegion.east, s->globalRegion.north );

      ecs_AddText(&(s->result),line);
      
      sprintf(line, 
              "         <BoundingBox minx=\"%.9f\"  miny=\"%.9f\"\n"
              "                      maxx=\"%.9f\"  maxy=\"%.9f\"\n"
              "                      resx=\"%.9f\"  resy=\"%.9f\" />\n",
              s->globalRegion.west, s->globalRegion.south, 
              s->globalRegion.east, s->globalRegion.north,
              s->globalRegion.ew_res, s->globalRegion.ns_res );
      ecs_AddText(&(s->result),line);
      
      ecs_AddText(&(s->result),
                  "         <Family>Matrix</Family>\n" ); 
      ecs_AddText(&(s->result),
                  "         <Family>Image</Family>\n" ); 
      ecs_AddText(&(s->result),
                  "      </FeatureType>\n"
                  "   </FeatureTypeList>\n" 
                  "</OGDI_Capabilities>\n" );
      ecs_SetSuccess(&(s->result));
  }

  else if( strcmp(info,"") == 0 )
  {
      char buffer[64];

      strcpy(buffer,spriv->layername);
      ecs_AddText(&(s->result),buffer);
      ecs_SetSuccess(&(s->result));
  }
  else
  {
      char emsg[129];

      sprintf( emsg, "DTED driver UpdateDictionary(%s) unsupported.", info );
      
      ecs_SetError(&(s->result), 1, emsg );
  }

  return (&(s->result));
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
  ecs_SetText(&(s->result), PROJ_LONGLAT);
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


