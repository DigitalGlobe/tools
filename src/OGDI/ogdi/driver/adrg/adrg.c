/******************************************************************************
 *
 * Component: OGDI ADRG Driver
 * Purpose: External (dyn_*) entry points for ADRG driver.
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
 * $Log: adrg.c,v $
 * Revision 1.10  2016/06/27 22:01:46  erouault
 * Fix memory leak in ADRG driver
 *
 * Revision 1.9  2007/02/12 16:09:06  cbalint
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
 * Revision 1.8  2003/08/27 04:50:01  warmerda
 * Fixed _releaseAllLayers() to release the layers starting with the
 * last till the first since it appears that ecs_FreeLayer() is unexpectedly
 * reducing the layer count, and shuffling layers down in the ecs_Server
 * layer list.   Found while investigating leaking described in bug 795612.
 *
 * Revision 1.7  2001/06/25 19:46:10  warmerda
 * Made cleanup safer if verifyLocation() fails.
 *
 * Revision 1.6  2001/06/23 14:06:31  warmerda
 * added capabilities support, cache layer list when opening datastore
 *
 * Revision 1.5  2001/06/22 16:37:50  warmerda
 * added Image support, upgraded headers
 *
 */

#include "adrg.h"
#include "datadict.h"
#include <assert.h>

ECS_CVSID("$Id: adrg.c,v 1.10 2016/06/27 22:01:46 erouault Exp $");

static void	_releaseAllLayers _ANSI_ARGS_((ecs_Server *s));
int colorintensity[6] = {0,63,105,147,189,255};

/* layer oriented functions are keeped in data structure to simplify code */

LayerMethod adrg_layerMethod[11] = {  
  /* 0 */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Area */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Line */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Point */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Matrix */	{ NULL, NULL, _rewindRasterLayer, _getNextObjectRaster, _getObjectRaster, _getObjectIdRaster },
  /* Image */	{ NULL, NULL, _rewindImageLayer, _getNextObjectImage, _getObjectImage, _getObjectIdImage },
  /* Text */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Edge */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Face */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Node */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Ring */	{ NULL, NULL, NULL, NULL, NULL, NULL }
};

/* ----------------------------------------------------------------------
 *  _dyn_CreateServer: 
 *     
 *   Creation of a new ADRG server
 * ----------------------------------------------------------------------
 */


ecs_Result *dyn_CreateServer(s,Request)
     ecs_Server *s;
     char *Request;
{
  register ServerPrivateData *spriv = s->priv = 
      (void *) calloc(sizeof(ServerPrivateData),1);
  struct dirent *structure;
  DIR *dirlist;
  char *c;
  char buffer[125];
  char cc,sc[3];

  if (spriv == NULL) {
    ecs_SetError(&(s->result),1,"Not enough memory to allocate server private data");
    return &(s->result);
  }

  spriv->imgdir = (char *) malloc(strlen(s->pathname)+1);
  if (spriv->imgdir == NULL) {
    free(s->priv);
    ecs_SetError(&(s->result),1,"Not enough memory");
    return &(s->result);
  }  

  spriv->layer_count = 0;
  spriv->layer_list = (char **) malloc(sizeof(char *) * 1);

  if (s->pathname[2] == ':') {
    strcpy(spriv->imgdir,s->pathname+1);
  } else {
    strcpy(spriv->imgdir,s->pathname);
  }

  /* Search for the .GEN file and set spriv->genfilename
     with this value. */

  dirlist = opendir(spriv->imgdir);
  if (dirlist==NULL) {
    free(spriv->imgdir);
    free(s->priv);
    ecs_SetError(&(s->result),1,"Unable to see the ADRG directory");
    return &(s->result);
  }

  structure = (struct dirent *) readdir(dirlist);

  while (structure != NULL) {
    if (!((strcmp(structure->d_name,".") == 0) || 
	  (strcmp(structure->d_name,"..") == 0))) {
      c = structure->d_name;
      while((c[0]!='.') && (c[0]!='\0'))
	c++;

      if ((strcmp(".GEN",c) == 0) || (strcmp(".gen",c) == 0)) {
	spriv->genfilename = (char *) malloc(strlen(spriv->imgdir)+
					     strlen(structure->d_name)+2);
	if (spriv->genfilename==NULL) {
	  free(spriv->imgdir);
	  free(s->priv);
	  ecs_SetError(&(s->result),1,"Not enough memory");
	  return &(s->result);
	}
	
	strcpy(spriv->genfilename,spriv->imgdir);
	strcat(spriv->genfilename,"/");
	strcat(spriv->genfilename,structure->d_name);

      } else if( (strcmp(".IMG",c) == 0) || (strcmp(".img",c) == 0)) {
          spriv->layer_list = (char **) 
              realloc(spriv->layer_list,
                      sizeof(char *)*(spriv->layer_count+1));
          if( spriv->layer_list == NULL )
          {
              ecs_SetError(&(s->result),1,"Not enough memory");
              return &(s->result);
          }
          
          spriv->layer_list[spriv->layer_count++] = 
              strdup( structure->d_name );
      }
    }

    structure = (struct dirent *) readdir(dirlist);       
  }

  closedir(dirlist);


  /* check the .GEN file and see if the location is valid */

  if (!_verifyLocation(s)) {
    if( spriv->imgdir )
        free(spriv->imgdir);
    if( spriv->genfilename )
        free(spriv->genfilename);
    free(s->priv);
    return &(s->result);		
  }

  /* initialize the driver globalRegion */

  if (!_initRegionWithDefault(s)) {
    free(spriv->imgdir);
    free(spriv->genfilename);
    free(s->priv);
    return &(s->result);
  }

  /* Read overview and open the corresponding file */

  if (!_read_overview(s)) {
    free(spriv->imgdir);
    free(spriv->genfilename);
    free(s->priv);
    return &(s->result);
  }

  /* Open the adrg IMG file */

  strcpy(buffer,spriv->imgdir);
  strcat(buffer,"/");
  strcat(buffer,spriv->overview.imgfilename);

  spriv->overview.imgfile = fopen(buffer,"rb");
  if (spriv->overview.imgfile == NULL) {
    strcpy(buffer,spriv->imgdir);
    strcat(buffer,"/");
    loc_strlwr(spriv->overview.imgfilename);
    strcat(buffer,spriv->overview.imgfilename);    
    spriv->overview.imgfile = fopen(buffer,"rb");
    if (spriv->overview.imgfile == NULL) {
      strcpy(buffer,spriv->imgdir);
      strcat(buffer,"/");
      loc_strupr(spriv->overview.imgfilename);
      strcat(buffer,spriv->overview.imgfilename);    
      spriv->overview.imgfile = fopen(buffer,"rb");
      if (spriv->overview.imgfile == NULL) {
	ecs_SetError(&(s->result),1,"Unable to open the adrg .IMG file ");
	if (spriv->overview.tilelist != NULL) {
	  free(spriv->overview.tilelist);
	}
	free(spriv->imgdir);
	free(spriv->genfilename);
	free(s->priv);
	return &(s->result);
      }
    }
  }
  spriv->overview.firstposition = 1;
  cc = getc(spriv->overview.imgfile);
  while(!feof(spriv->overview.imgfile)) {
    if (cc==(char) 30) {
      ogdi_fread(sc,3,1,spriv->overview.imgfile);
      spriv->overview.firstposition+=3;
      if (strncmp(sc,"IMG",3) == 0) {
	spriv->overview.firstposition+=4;
	fseek(spriv->overview.imgfile,3,SEEK_CUR);
	cc = getc(spriv->overview.imgfile);
	while(cc==' ') {
	  spriv->overview.firstposition++;
	  cc = getc(spriv->overview.imgfile);
	}
	spriv->overview.firstposition++;
	break;
      }
    }

    spriv->overview.firstposition++;
    cc = getc(spriv->overview.imgfile);
  }

  /* initialize layer private data */

  s->nblayer = 0; 

  ecs_SetSuccess(&(s->result));
  return &(s->result);
}


/* ----------------------------------------------------------------------
 *  _dyn_DestroyServer: 
 *     
 *   Destruction of this ADRG server
 * ----------------------------------------------------------------------
 */

ecs_Result *dyn_DestroyServer(s)
     ecs_Server *s;
{
  register ServerPrivateData *spriv = s->priv;

  /* Release all layer */
  
  _releaseAllLayers(s);

  if(spriv != NULL) {
    int  i;
    if (spriv->imgdir != NULL) {
      free(spriv->imgdir);
    }
    if (spriv->genfilename != NULL) {
      free(spriv->genfilename);
    }
    if (spriv->overview.tilelist != NULL) {
      free(spriv->overview.tilelist);
    }
    if (spriv->overview.imgfile != NULL) {
      fclose(spriv->overview.imgfile);
    }

    for( i = 0; i < spriv->layer_count; i++ )
        free( spriv->layer_list[i] );
    if( spriv->layer_list != NULL )
        free( spriv->layer_list );
    
    free(spriv);
  }
  
  ecs_SetSuccess(&(s->result));
  return &(s->result);

}

void _freelayerpriv(lpriv)
     LayerPrivateData *lpriv;
{
  if (lpriv != NULL) {
    if (lpriv->tilelist != NULL)
      free(lpriv->tilelist);
    if (lpriv->buffertile != NULL)
      free(lpriv->buffertile);
    if (lpriv->imgfile != NULL)
      fclose(lpriv->imgfile);
    
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
  register LayerPrivateData *lpriv;
  register ServerPrivateData *spriv = s->priv;
  char c,sc[3];
  char buffer[128];
  
  /* first, try to find an existing layer with same request and family */

  if ((layer = ecs_GetLayer(s,sel)) != -1) {

    /* if it already exists than assign currentLayer and set index to 0 to force rewind */

    s->currentLayer = layer;
    s->layer[layer].index = 0;
    lpriv = (LayerPrivateData *) s->layer[layer].priv;
    ecs_SetGeoRegion(&(s->result),lpriv->region.north, lpriv->region.south, 
		     lpriv->region.east, lpriv->region.west, 
		     lpriv->region.ns_res, lpriv->region.ew_res);

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
  lpriv->tilelist = NULL;
  lpriv->buffertile = NULL;

  strcpy(lpriv->imgfilename,sel->Select);
  
  /* Extract the layer information from the GEN file */

  if (!_read_adrg(s,&(s->layer[layer]))) {
    _freelayerpriv(lpriv);
    ecs_FreeLayer(s,layer);
    return &(s->result);
  }

  /* Open the adrg IMG file */

  strcpy(buffer,spriv->imgdir);
  strcat(buffer,"/");
  strcat(buffer,lpriv->imgfilename);
  lpriv->imgfile = fopen(buffer,"rb");

  if (lpriv->imgfile == NULL) {

    strcpy(buffer,spriv->imgdir);
    strcat(buffer,"/");
    loc_strlwr(lpriv->imgfilename);
    strcat(buffer,lpriv->imgfilename);
    lpriv->imgfile = fopen(buffer,"rb");

    if (lpriv->imgfile == NULL) {
      strcpy(buffer,spriv->imgdir);
      strcat(buffer,"/");
      loc_strupr(lpriv->imgfilename);
      strcat(buffer,lpriv->imgfilename);
      lpriv->imgfile = fopen(buffer,"rb");

      if (lpriv->imgfile == NULL) {
	_freelayerpriv(lpriv);
	ecs_FreeLayer(s,layer);
	ecs_SetError(&(s->result),1,"Unable to open the adrg .IMG file ");
	return &(s->result);
      }
    }
  }      
  
  lpriv->firstposition = 1;
  c = getc(lpriv->imgfile);
  while(!feof(lpriv->imgfile)) {
    if (c==(char) 30) {
      ogdi_fread(sc,3,1,lpriv->imgfile);
      lpriv->firstposition+=3;
      if (strncmp(sc,"IMG",3) == 0) {
	lpriv->firstposition+=4;
	fseek(lpriv->imgfile,3,SEEK_CUR);
	c = getc(lpriv->imgfile);
	while(c==' ') {
	  lpriv->firstposition++;
	  c = getc(lpriv->imgfile);
	}
	lpriv->firstposition++;
	break;
      }
    }

    lpriv->firstposition++;
    c = getc(lpriv->imgfile);
  }

  s->currentLayer = layer;
  s->layer[layer].nbfeature = (int) ((s->currentRegion.north - s->currentRegion.south)/s->currentRegion.ns_res);


  ecs_SetGeoRegion(&(s->result),lpriv->region.north, lpriv->region.south, 
		   lpriv->region.east, lpriv->region.west, 
		   lpriv->region.ns_res, lpriv->region.ew_res);
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

  ecs_SetSuccess(&(s->result));
  return &(s->result);
}

/* deselect all layer */

static void
_releaseAllLayers(s)
     ecs_Server *s;
{
  int i;

  for (i = s->nblayer-1; i >= 0; i-- )
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
    s->layer[s->currentLayer].nbfeature = (int) ((s->currentRegion.north - s->currentRegion.south)/s->currentRegion.ns_res);
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
 *  dyn_GetNextObject: 
 *     
 *      return the next object for the current layer
 * ----------------------------------------------------------------------
 */

ecs_Result *dyn_GetNextObject(s)
     ecs_Server *s;
{

  if (s->layer[s->currentLayer].index == 0) {
    (adrg_layerMethod[s->layer[s->currentLayer].sel.F].rewind)(s,&(s->layer[s->currentLayer]));
  }
  
  (adrg_layerMethod[s->layer[s->currentLayer].sel.F].getNextObject)(s,&(s->layer[s->currentLayer]));
    
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
  register unsigned int i,j,k;
  char buffer[2];
  register LayerPrivateData *lpriv;
  int count;

  strcpy(buffer,"");
  lpriv = (LayerPrivateData *) s->layer[s->currentLayer].priv;

  /* Put table contain in RasterInfo here */

  if (s->layer[s->currentLayer].sel.F == Matrix) {
    ecs_SetRasterInfo(&(s->result),lpriv->columns,lpriv->rows);
    count = 1;
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
    ecs_SetRasterInfo(&(s->result),1,0);
    ecs_AddRasterInfoCategory(&(s->result),1, 255, 255, 255,"No data",0);
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
  if (adrg_layerMethod[s->layer[s->currentLayer].sel.F].getObject != NULL) 
    (adrg_layerMethod[s->layer[s->currentLayer].sel.F].getObject)(s,&(s->layer[s->currentLayer]),Id);
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
  if (adrg_layerMethod[s->layer[s->currentLayer].sel.F].getObjectIdFromCoord != NULL) 
    (adrg_layerMethod[s->layer[s->currentLayer].sel.F].getObjectIdFromCoord)(s,&(s->layer[s->currentLayer]),coord);
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
    register ServerPrivateData *spriv = s->priv;
    int	   i;

/* -------------------------------------------------------------------- */
/*      Reduced capabilities without layers.                            */
/* -------------------------------------------------------------------- */
    if( strcmp(info,"ogdi_server_capabilities") == 0 )
    {
        ecs_AddText(&(s->result),
                    "<?xml version=\"1.0\" ?>\n"
                    "<OGDI_Capabilities version=\"3.1\">\n"
                    "</OGDI_Capabilities>\n" );
        ecs_SetSuccess(&(s->result));
    }

/* -------------------------------------------------------------------- */
/*      Full capabilities.                                              */
/* -------------------------------------------------------------------- */
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

        for (i=0; i < spriv->layer_count; i++ )
        {
            ecs_Layer	dummy_layer;
            LayerPrivateData *lpriv;
            
            dummy_layer.priv = (void *) calloc(sizeof(LayerPrivateData),1);

            lpriv = (LayerPrivateData *) dummy_layer.priv;
            lpriv->tilelist = NULL;
            lpriv->buffertile = NULL;

            strcpy(lpriv->imgfilename,spriv->layer_list[i]);
  
            /* Extract the layer information from the GEN file */
            
            if (!_read_adrg(s,&dummy_layer)) 
            {
                _freelayerpriv(lpriv);
                continue;
            }

            /* Format the XML info */

            ecs_AddText(&(s->result),
                        "      <FeatureType>\n" );
            sprintf( line, "         <Name>%s</Name>\n", 
                     spriv->layer_list[i] );
            ecs_AddText(&(s->result), line );
            
            sprintf( line, "         <SRS>PROJ4:%s</SRS>\n", PROJ_LONGLAT );
            ecs_AddText(&(s->result),line);
            sprintf(line, 
                    "         <LatLonBoundingBox minx=\"%.9f\"  miny=\"%.9f\"\n"
                    "                            maxx=\"%.9f\"  maxy=\"%.9f\" />\n",
                    lpriv->region.west, 
                    lpriv->region.south,
                    lpriv->region.east,
                    lpriv->region.north );
            ecs_AddText(&(s->result),line);

            sprintf(line, 
                    "         <BoundingBox minx=\"%.9f\" miny=\"%.9f\"\n"
                    "                      maxx=\"%.9f\" maxy=\"%.9f\"\n"
                    "                      resx=\"%.9f\" resy=\"%.9f\" />\n",
                    lpriv->region.west, 
                    lpriv->region.south,
                    lpriv->region.east,
                    lpriv->region.north,
                    lpriv->region.ew_res, 
                    lpriv->region.ns_res );
            ecs_AddText(&(s->result),line);

            ecs_AddText(&(s->result),
                        "         <Family>Matrix</Family>\n"
                        "         <Family>Image</Family>\n"
                        "      </FeatureType>\n" );

            _freelayerpriv(lpriv);
        }

        ecs_AddText(&(s->result),
                    "   </FeatureTypeList>\n" 
                    "</OGDI_Capabilities>\n" );
        ecs_SetSuccess(&(s->result));
    }

/* -------------------------------------------------------------------- */
/*      Old style return result.                                        */
/* -------------------------------------------------------------------- */
    else 
    {
        ecs_SetText(&(s->result)," "); 
        for( i = 0; i < spriv->layer_count; i++ )
        {
            ecs_AddText( &(s->result), spriv->layer_list[i] );
            ecs_AddText( &(s->result), " " );
        }
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
		   s->globalRegion.east, s->globalRegion.west, 
		   s->globalRegion.ns_res, s->globalRegion.ew_res);
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
  ecs_SetSuccess(&(s->result));
  return &(s->result);
}




char *loc_strlwr (string)
     char *string;
{
  size_t i;
  
  if (!string)
    return string;
  
  for (i=0; i<strlen (string); i++)
    string[i] = (char)tolower (string[i]);
  return string;
}

char *loc_strupr (string)
     char *string;
{
  size_t i;
  
  if (!string)
    return string;
  
  for (i=0; i<strlen (string); i++)
    string[i] = (char)toupper (string[i]);
  return string;
}

