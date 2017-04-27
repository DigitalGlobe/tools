/******************************************************************************
 *
 * Component: OGDI RPF Driver
 * Purpose: Implementation of dyn_* api for RPF driver.
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
 * $Log: rpf.c,v $
 * Revision 1.14  2016/07/08 10:22:55  erouault
 * Fix various compilation problems on Windows (contributed by Jerome Siot)
 *
 * Revision 1.13  2007/02/24 16:58:17  cbalint
 *     Clear olso tilestruct while freeing memory in dyn_freelayerpriv()
 *     glibc mtrace() reports zero malloc problems now.
 *     Thanks to djdejohn for the report !
 *  Modified Files:
 *  	ChangeLog ogdi/driver/rpf/rpf.c
 *
 * Revision 1.12  2004/03/26 22:33:41  warmerda
 * Fixed computation of nbfeature in dyn_SelectRegion() to avoid rounding
 * error issues.  As per Bug 924250.
 *
 * Revision 1.11  2001/08/27 14:56:29  warmerda
 * RPF rasterinfo width fixed
 *
 * Revision 1.10  2001/08/16 13:06:52  warmerda
 * ensure that only Matrix and Image are allowed by RPF SelectLayer
 *
 * Revision 1.9  2001/06/13 17:17:40  warmerda
 * fixed capabilities to match 6.2 spec
 *
 * Revision 1.8  2001/05/30 19:02:51  warmerda
 * return real per-layer bounds in capabilities
 *
 * Revision 1.7  2001/04/16 21:32:05  warmerda
 * added capabilities support
 *
 * Revision 1.6  2001/04/12 19:22:46  warmerda
 * applied DND support Image type support
 *
 */

#include "rpf.h"
#include "datadict.h"

ECS_CVSID("$Id: rpf.c,v 1.14 2016/07/08 10:22:55 erouault Exp $");

int colorintensity[6] = {0,63,105,147,189,255};


/* layer oriented functions are keeped in data structure to simplify code */

LayerMethod rpf_layerMethod[11] = {  
  /* 0 */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Area */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Line */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Point */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Matrix */	{ NULL, NULL, dyn_rewindRasterLayer, dyn_getNextObjectRaster, dyn_getObjectRaster, dyn_getObjectIdRaster },
  /* Image */	{ NULL, NULL, dyn_rewindImageLayer, dyn_getNextObjectImage, dyn_getObjectImage, dyn_getObjectIdImage },
  /* Text */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Edge */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Face */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Node */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Ring */	{ NULL, NULL, NULL, NULL, NULL, NULL }
};

/* ----------------------------------------------------------------------
 *  _dyn_CreateServer: 
 *     
 *   Creation of a new RPF server
 * ----------------------------------------------------------------------
 */


ecs_Result *dyn_CreateServer(s,Request)
     ecs_Server *s;
     char *Request;
{
  register ServerPrivateData *spriv = s->priv = (void *) malloc(sizeof(ServerPrivateData));

  (void) Request;

  if (spriv == NULL) {
    ecs_SetError(&(s->result),1,"Not enough memory to allocate server private data");
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

  tprintf("dyn_CreateServer 1\n");

  /* check the path and see if the location is valid */

  if (!(dyn_verifyLocation(s))) {
    free(spriv->pathname);
    free(s->priv);
    return &(s->result);		
  }

  tprintf("dyn_CreateServer 2\n");

  /* initialize the driver globalRegion */

  if (!(dyn_initRegionWithDefault(s))) {
    free(spriv->pathname);
    free(s->priv);
    return &(s->result);
  }


  tprintf("dyn_CreateServer 3\n");

  /* initialize layer private data */

  s->nblayer = 0; 

  ecs_SetSuccess(&(s->result));
  return &(s->result);
}

/* ----------------------------------------------------------------------
 *  _dyn_DestroyServer: 
 *     
 *   Destruction of this RPF server
 * ----------------------------------------------------------------------
 */

ecs_Result *dyn_DestroyServer(s)
     ecs_Server *s;
{
  register ServerPrivateData *spriv = s->priv;

  /* Release all layer */
  
  dyn_releaseAllLayers(s);

  if(spriv != NULL) {
    if (spriv->pathname != NULL) {
      free(spriv->pathname);
    }
    if (spriv->toc != (Toc_file *)NULL) {
      free_toc(spriv->toc);
    }

    free(spriv);
  }
  
  ecs_SetSuccess(&(s->result));
  return &(s->result);

}

void dyn_freelayerpriv(lpriv)
     LayerPrivateData *lpriv;
{
  if (lpriv != NULL) {
    lpriv->entry = NULL;
    if (lpriv->buffertile != NULL) {
      free(lpriv->buffertile);
      lpriv->buffertile = NULL;
    }
    
#ifdef notdef
   /* FIXME? ecs_TileDeleteAllLines does not exist */
   /* empty tilestruct */
   ecs_TileDeleteAllLines (&(lpriv->tilestruct));
#endif
    
    if (lpriv->ff != NULL) 
      free(lpriv->ff);
    if (lpriv->rgb_pal != NULL) 
      free(lpriv->rgb_pal);
    if (lpriv->rpf_table != NULL) 
      free(lpriv->rpf_table);
    free(lpriv);
  }
  lpriv = NULL;
}

/*
*******************************************************************

FUNCTION_INFORMATION

NAME

     dyn_SelectLayer

DESCRIPTION

     Select a layer and prepare it to be read in the rpf database.

END_DESCRIPTION

PRECONDITIONS

     dyn_CreateServer must have been previously called.

END_PRECONDITIONS

POSTCONDITIONS

     The layer is selected and ready to be read with the commands
     dyn_GetNextObject, dyn_GetRasterInfo and dyn_GetObject.

END_POSTCONDITIONS

PARAMETERS

     INPUT
     ecs_Server *s: The driver information
     ecs_LayerSelection *sel: The layer selection information

END_PARAMETERS

RETURN_VALUE

     ecs_Result *: The result structure common to all OGDI calls.

END_FUNCTION_INFORMATION

PSEUDOCODE

     Check if a layer exist with ecs_GetLayer. If a layer exist
     Begin

          The current layer is now the layer returned by ecs_GetLayer
          The index of this layer is set to 0
          Return a success message

     End

     Set a new layer in the layer structure with ecs_SetLayer.

     Allocate the mamory needed to hold private info about this new layer.

     Prepare the new allocated structure with default values.

     Prepare the layer for selection with _prepare_rpflayer. If an error
     occur, free the memory of this layer, call ecs_FreeLayer and return
     an error message.

     Set the current layer to the new layer

     Set the index of this layer to 0

     Return a success message

END_PSEUDOCODE

*******************************************************************
*/

ecs_Result *dyn_SelectLayer(s,sel)
     ecs_Server *s;
     ecs_LayerSelection *sel;
{
  int layer;
  register LayerPrivateData *lpriv;
  ecs_Region region;

  /* Disallow any families but Image and Matrix */
  if( sel->F != Matrix && sel->F != Image )
  {
      ecs_SetError(&(s->result),1,
               "RPF driver only supports Matrix and Image in SelectLayer.");
      return &(s->result);
  }

  /* first, try to find an existing layer with same request and family */

  if ((layer = ecs_GetLayer(s,sel)) != -1) {

    /* if it already exists than assign currentLayer and set index to 0 to force rewind */

    s->currentLayer = layer;
    s->layer[layer].index = 0;
    lpriv = (LayerPrivateData *) s->layer[layer].priv;
    region.north = lpriv->entry->nw_lat;
    region.south = lpriv->entry->sw_lat;
    region.east = lpriv->entry->ne_long;
    region.west = lpriv->entry->nw_long;
    region.ns_res = (region.north - region.south) / (1536*lpriv->entry->vert_frames);
    region.ew_res = (region.east - region.west) / (1536*lpriv->entry->horiz_frames);
    
    ecs_SetGeoRegion(&(s->result),region.north, region.south, 
		     region.east, region.west, region.ns_res, 
		     region.ew_res);
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
  lpriv->entry = NULL;
  lpriv->buffertile = NULL;
  lpriv->ff = NULL;
  lpriv->rgb_pal = NULL;
  lpriv->rpf_table = NULL;
  lpriv->cct = NULL;
  lpriv->tile_row = -1;
  lpriv->tile_col = -1;
  lpriv->isActive = FALSE;
  lpriv->isColor = TRUE;

  /*
    Prepare the layer for selection with _prepare_rpflayer. If an error
    occur, free the memory of this layer, call ecs_FreeLayer and return
    an error message.
    */

  if (!(dyn_prepare_rpflayer(s,&(s->layer[layer])))) {
    dyn_freelayerpriv(lpriv);
    ecs_FreeLayer(s,layer);
    /* The error message is already set */
    return &(s->result);	
  }

  s->currentLayer = layer;
  s->layer[layer].nbfeature = (int) ((s->currentRegion.north - s->currentRegion.south)/s->currentRegion.ns_res);
  s->layer[layer].index = 0;

  region.north = lpriv->entry->nw_lat;
  region.south = lpriv->entry->sw_lat;
  region.east = lpriv->entry->ne_long;
  region.west = lpriv->entry->nw_long;
  region.ns_res = (region.north - region.south) / (1536*lpriv->entry->vert_frames);
  region.ew_res = (region.east - region.west) / (1536*lpriv->entry->horiz_frames);

  ecs_SetGeoRegion(&(s->result),region.north, region.south, 
		   region.east, region.west, region.ns_res, 
		   region.ew_res);
  ecs_SetSuccess(&(s->result));
  return &(s->result);
}

/*
*******************************************************************

FUNCTION_INFORMATION

NAME

     dyn_ReleaseLayer

DESCRIPTION

     Release a given layer

END_DESCRIPTION

PRECONDITIONS

     dyn_CreateServer must have been previously called.

END_PRECONDITIONS

POSTCONDITIONS

     If a layer with the selection information sel exist. It will
     be remove from the memory and from the layer list.

END_POSTCONDITIONS

PARAMETERS

     INPUT
     ecs_Server *s: The driver information
     ecs_LayerSelection *sel: The layer selection information

END_PARAMETERS

RETURN_VALUE

     ecs_Result *: The result structure common to all OGDI calls.

END_FUNCTION_INFORMATION

PSEUDOCODE

     Check if a layer exist with ecs_GetLayer. If the layer don't exist
     Begin

          Return an error message to indicate the layer don't exist.

     End

     Free the layer private data in the layer previously selected.

     Call ecs_FreeLayer to free the layer from the layer list.

     If the currentlayer was this layer, set the currentlayer to -1.

     Return a success message

END_PSEUDOCODE

*******************************************************************
*/

ecs_Result *dyn_ReleaseLayer(s,sel)
     ecs_Server *s;
     ecs_LayerSelection *sel;
{
  int layer;
  char buffer[128];
  register LayerPrivateData *lpriv;

  /* first, try to find an existing layer with same request and family */

  if ((layer = ecs_GetLayer(s,sel)) == -1) {
    sprintf(buffer,"Invalid layer %s",sel->Select);
    ecs_SetError(&(s->result),1,buffer);
    return &(s->result);
  }
  lpriv = (LayerPrivateData *) s->layer[layer].priv;
  
  if (lpriv != NULL) {
    dyn_freelayerpriv(lpriv);    
    ecs_FreeLayer(s,layer);
    
    if (s->currentLayer == layer) {
      s->currentLayer = -1;		/* just in case released layer was selected */
    }
  }

  ecs_SetSuccess(&(s->result));
  return &(s->result);
}

/* deselect all layer */

void
dyn_releaseAllLayers(s)
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
    s->layer[s->currentLayer].index = 0;
    s->layer[s->currentLayer].nbfeature = 
        (int) ((s->currentRegion.north - s->currentRegion.south)
               / s->currentRegion.ns_res + 0.5);
  }

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

/*
*******************************************************************

FUNCTION_INFORMATION

NAME

     dyn_GetDictionary

DESCRIPTION

     Get a itcl applet to Grassland able to read 
     dyn_UpdateDictionary results.

END_DESCRIPTION

PRECONDITIONS

     dyn_CreateServer must have been previously called.

END_PRECONDITIONS

POSTCONDITIONS

     No post conditions

END_POSTCONDITIONS

PARAMETERS

     INPUT
     ecs_Server *s: The driver information

END_PARAMETERS

RETURN_VALUE

     ecs_Result *: The result structure common to all OGDI calls.

END_FUNCTION_INFORMATION

PSEUDOCODE

     Return the contain of the struct in datadict.h with SetText.

     Return a success message

END_PSEUDOCODE

*******************************************************************
*/

ecs_Result *dyn_GetDictionary(s)
     ecs_Server *s;
{
  ecs_SetText(&(s->result),datadict);
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

  if (s->layer[s->currentLayer].index == 0) {
    (rpf_layerMethod[s->layer[s->currentLayer].sel.F].rewind)(s,&(s->layer[s->currentLayer]));
  }
  
  (rpf_layerMethod[s->layer[s->currentLayer].sel.F].getNextObject)(s,&(s->layer[s->currentLayer]));
    
  return &(s->result);
    
}

/*
*******************************************************************

FUNCTION_INFORMATION

NAME

     dyn_GetRasterInfo

DESCRIPTION

     Return the raster information for the current layer and set a
     category table for a 6x6x6 color cube for matrix informations and
     a default category table for images.

END_DESCRIPTION

PRECONDITIONS

     dyn_CreateServer must have been previously called. A SelectLayer
     must have been called successfully previously.

END_PRECONDITIONS

POSTCONDITIONS

     No post conditions

END_POSTCONDITIONS

PARAMETERS

     INPUT
     ecs_Server *s: The driver information

END_PARAMETERS

RETURN_VALUE

     ecs_Result *: The result structure common to all OGDI calls.

END_FUNCTION_INFORMATION

PSEUDOCODE

     Get the current private layer information

     If the layer is matricial
     Begin

        Call ecs_SetRasterInfo with the width and the height of the
        layer.  To calculate that, multiply the number of tiles
        horizontally and vertically by 1536.
   
        For each value i between 0 and 5
        Begin
   
             For each value j between 0 and 5
             Begin
   
                  For each value k between 0 and 5
                  Begin
   
                       Add a category with ecs_AddRasterInfoCategory
                       with a color r=i*43, g=j*43 and b=k*43. The
                       buffer is a empty string.
   
                  End
   
             End
   
        End
   
     End

     Else if the layer is an image
     Begin

        Call ecs_SetRasterInfo. Set the width with 1 (RGB image) and
        height to 0.

	Add a category with ecs_AddRasterInfoCategory with a white
	color. The buffer is a string "No data".
        
     End

     Return a success message

END_PSEUDOCODE

*******************************************************************
*/

ecs_Result *dyn_GetRasterInfo(s)
     ecs_Server *s;
{
  register unsigned int i,j,k;
  char buffer[2];
  register LayerPrivateData *lpriv;
  int count, rows, columns;
  ecs_Region region = s->currentRegion;

  strcpy(buffer,"");
  lpriv = (LayerPrivateData *) s->layer[s->currentLayer].priv;

  /* Put table contain in RasterInfo here */
  
  rows = (int) floor((region.north-region.south)/region.ns_res + 0.5);
  columns = (int) floor((region.east-region.west)/region.ew_res + 0.5);

  if (s->layer[s->currentLayer].sel.F == Matrix) {
    ecs_SetRasterInfo(&(s->result),columns,rows);
    count = 1;
    if (lpriv->isColor == TRUE) {
      for(i=0;i<6;i++) {
        for(j=0;j<6;j++) {
	  for(k=0;k<6;k++) {
	    ecs_AddRasterInfoCategory(&(s->result),count,
                                      colorintensity[i],
                                      colorintensity[j],
                                      colorintensity[k],buffer,0);
            count++;
          }
        }
      }
    } else {
        for(i=1;i<255;i++) {
            ecs_AddRasterInfoCategory(&(s->result),i,i,i,i,buffer,0);
        }
    }
  } else {
    ecs_SetRasterInfo(&(s->result),1,0);
    ecs_AddRasterInfoCategory(&(s->result),1, 255, 255, 255,"No data",0);
  }

  ecs_SetSuccess(&(s->result));
  return &(s->result);
}

/* ----------------------------------------------------------------------
   _initCatTable
      
       Inititalize category informations

  ----------------------------------------------------------------------
 */

int dyn_initCatTable(s,layer)
     ecs_Server *s;
     ecs_Layer *layer;
{
  register LayerPrivateData *lpriv;

  (void) s;
  
  lpriv = (LayerPrivateData *) layer->priv;

  lpriv->mincat = 0+13;
  lpriv->maxcat = lpriv->n_pal_cols-1+13;

  return TRUE;
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
  if (rpf_layerMethod[s->layer[s->currentLayer].sel.F].getObject != NULL) 
    (rpf_layerMethod[s->layer[s->currentLayer].sel.F].getObject)(s,&(s->layer[s->currentLayer]),Id);
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
  if (rpf_layerMethod[s->layer[s->currentLayer].sel.F].getObjectIdFromCoord != NULL) 
    (rpf_layerMethod[s->layer[s->currentLayer].sel.F].getObjectIdFromCoord)(s,&(s->layer[s->currentLayer]),coord);
  else {
    ecs_SetError(&(s->result),1,"Can't get objectid from coordinate for this type of layer");
  }
  return &(s->result);
}

/*
*******************************************************************

FUNCTION_INFORMATION

NAME

     dyn_UpdateDictionary

DESCRIPTION

     Return a list of entries in the format of the SelectLayer request.

END_DESCRIPTION

PRECONDITIONS

     dyn_CreateServer must have been previously called.

END_PRECONDITIONS

POSTCONDITIONS

     No post conditions

END_POSTCONDITIONS

PARAMETERS

     INPUT
     ecs_Server *s: The driver information
     char *info: The info structure. Not used in this structure

END_PARAMETERS

RETURN_VALUE

     ecs_Result *: The result structure common to all OGDI calls.

END_FUNCTION_INFORMATION

PSEUDOCODE

     Set an empty string with ecs_Settext

     For each valid entry (invalid_geographics to false) in spriv->toc
     Begin

          Construct with the strings scale, zone, type and producer in
          Toc_entry a selection string in the format scale@zone@type@producer.
          Take care to remove all the empty zones. Keep a space in the end.

          Add this string in ecs_Result with ecs_AddText. 

     End

END_PSEUDOCODE

*******************************************************************
*/

ecs_Result *dyn_UpdateDictionary(s,info)
     ecs_Server *s;
     char *info;
{
    char buffer[50],result[50];
    register ServerPrivateData *spriv = s->priv;
    int i,j,k;
    Toc_file *toc;

    toc = spriv->toc;

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
                    "      </Operations>\n" );

        for (i=0; i<(int)toc->num_boundaries; i++)
        {
            Toc_entry	*entry = toc->entries + i;

            if (entry->invalid_geographics == 1L)
                continue;
            
            sprintf(buffer,"%s@%s@%s@%s@%d",
                    entry->scale,
                    entry->zone, entry->type,
                    entry->producer,
                    entry->boundary_id);

            /* Remove the spaces */
            k = 0;
            for(j=0;j<(int) strlen(buffer);j++) {
                if (buffer[j] != ' ') {
                    result[k] = buffer[j];
                    k++;
                }
            }
            result[k] = '\0';
     
            ecs_AddText(&(s->result),
                        "      <FeatureType>\n" );
            sprintf( line, "         <Name>%s</Name>\n", result );
            ecs_AddText(&(s->result), line );

            
            
            sprintf( line, "         <SRS>PROJ4:%s</SRS>\n", PROJ_LONGLAT );
            ecs_AddText(&(s->result),line);

            sprintf(line, 
                    "         <LatLonBoundingBox minx=\"%.9f\"  miny=\"%.9f\"\n"
                    "                            maxx=\"%.9f\"  maxy=\"%.9f\" />\n",
                    entry->nw_long, entry->se_lat, 
                    entry->se_long, entry->nw_lat );
            
            ecs_AddText(&(s->result),line);
            
            sprintf(line, 
                    "         <BoundingBox minx=\"%.9f\" miny=\"%.9f\"\n"
                    "                      maxx=\"%.9f\" maxy=\"%.9f\"\n"
                    "                      resx=\"%.9f\" resy=\"%.9f\" />\n",
                    entry->nw_long, entry->se_lat, 
                    entry->se_long, entry->nw_lat,
                    entry->horiz_interval, entry->vert_interval );
            ecs_AddText(&(s->result),line);
            
            ecs_AddText(&(s->result),
                        "         <Family>Matrix</Family>\n"
                        "         <Family>Image</Family>\n"
                        "      </FeatureType>\n" );
        }

        ecs_AddText(&(s->result),
                    "   </FeatureTypeList>\n" 
                    "</OGDI_Capabilities>\n" );
        ecs_SetSuccess(&(s->result));
    }
    else if( strcmp(info,"") == 0 )
    {
        ecs_SetText(&(s->result)," "); 

        for (i=0; i<(int)toc->num_boundaries; i++)
        {
            if (toc->entries[i].invalid_geographics == 1L)
                continue;

            sprintf(buffer,"%s@%s@%s@%s@%d",toc->entries[i].scale,
                    toc->entries[i].zone,toc->entries[i].type,
                    toc->entries[i].producer,toc->entries[i].boundary_id);

            /* Remove the spaces */
            k = 0;
            for(j=0;j<(int) strlen(buffer);j++) {
                if (buffer[j] != ' ') {
                    result[k] = buffer[j];
                    k++;
                }
            }
            result[k] = '\0';
     
            if (!(ecs_AddText(&(s->result),result))) {
                return &(s->result);
            }
     
            if (!(ecs_AddText(&(s->result)," "))) {
                return &(s->result);
            }
        }
  
        ecs_SetSuccess(&(s->result));
    }
    else
    {
        char emsg[129];

        sprintf( emsg, "RPF driver UpdateDictionary(%s) unsupported.", info );
      
        ecs_SetError(&(s->result), 1, emsg );
    }

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


