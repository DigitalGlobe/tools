/**********************************************************************
 * $Id: gdal_serv.c,v 1.1 2001/05/04 03:13:35 warmerda Exp $
 *
 * Project:  GDAL OGDI Server
 * Purpose:  Implements dynamic entry points into the driver. 
 * Author:   Frank Warmerda, warmerda@home.com
 *
 **********************************************************************
 * Copyright (c) 2000, Frank Warmerdam
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 **********************************************************************
 * 
 * $Log: gdal_serv.c,v $
 * Revision 1.1  2001/05/04 03:13:35  warmerda
 * New
 *
 * Revision 1.2  2000/08/28 20:21:47  warmerda
 * minimally operational
 *
 * Revision 1.1  2000/08/23 19:40:17  warmerda
 * New
 *
 */

#include "gdal_serv.h"

static void _releaseAllLayers(ecs_Server *s);

/************************************************************************/
/*                          dyn_CreateServer()                          */
/************************************************************************/

ecs_Result *dyn_CreateServer(ecs_Server *s, char *Request)

{
    ServerPrivateData *spriv;
    int               nPixels, nLines;
    OGRSpatialReferenceH   hSRS;
    char                   *pszWKT;

    (void) Request;

/* -------------------------------------------------------------------- */
/*      Initialize GDAL Bridge                                          */
/* -------------------------------------------------------------------- */
    if( !GDALBridgeInitialize( ".." ) )
    {
        ecs_SetError(&(s->result), 1, 
                     "Unable to initialize GDAL Bridge." );
        return &(s->result);		
    }

    GDALAllRegister();

/* -------------------------------------------------------------------- */
/*      Create server private information.                              */
/* -------------------------------------------------------------------- */
    spriv = s->priv = (ServerPrivateData *) malloc(sizeof(ServerPrivateData));
    if (s->priv == NULL) {

        ecs_SetError(&(s->result), 1, 
                     "Could not connect to the skeleton driver, "
                     "not enough memory");
        return &(s->result);		
    }

/* -------------------------------------------------------------------- */
/*      Open the file.  We should eventually capture the real error     */
/*      message.                                                        */
/* -------------------------------------------------------------------- */
    spriv->hDS = GDALOpen( s->pathname, GA_ReadOnly );
    if( spriv->hDS == NULL )
    {
        /* Don't forget to unallocate the previous priv */
        free(s->priv);

        ecs_SetError(&(s->result), 1, 
                     "GDALOpen() open failed for given path.");
        return &(s->result);		    
    }

/* -------------------------------------------------------------------- */
/*      Establish the global bounds.  We will assume an unrotated       */
/*      frame of reference for now, but this should be checked          */
/*      later.                                                          */
/*                                                                      */
/*      We will treat the bottom left corner as the origin for raw      */
/*      rasters in order to maintain a compatible orientation to        */
/*      georeferenced rasters.                                          */
/* -------------------------------------------------------------------- */
    nPixels = GDALGetRasterXSize( spriv->hDS );
    nLines  = GDALGetRasterYSize( spriv->hDS );
    if( GDALGetGeoTransform( spriv->hDS, spriv->adfGeoTransform ) != CE_None 
        || (spriv->adfGeoTransform[0] == 0.0
            && spriv->adfGeoTransform[1] == 1.0
            && spriv->adfGeoTransform[2] == 0.0
            && spriv->adfGeoTransform[3] == 0.0
            && spriv->adfGeoTransform[4] == 0.0
            && spriv->adfGeoTransform[5] == 1.0 ) )
    {
        spriv->adfGeoTransform[0] = 0.0;
        spriv->adfGeoTransform[1] = 1.0;
        spriv->adfGeoTransform[2] = 0.0;
        spriv->adfGeoTransform[3] = 0.0;
        spriv->adfGeoTransform[4] = 0.0;
        spriv->adfGeoTransform[5] = -1.0;
    }

    s->globalRegion.north = spriv->adfGeoTransform[3];
    s->globalRegion.south = 
        spriv->adfGeoTransform[3] + nLines * spriv->adfGeoTransform[5];
    s->globalRegion.east = 
        spriv->adfGeoTransform[0] + nPixels * spriv->adfGeoTransform[1];
    s->globalRegion.west = spriv->adfGeoTransform[0];
    s->globalRegion.ns_res = 
        (s->globalRegion.north - s->globalRegion.south) / nLines;
    s->globalRegion.ew_res = 
        (s->globalRegion.east - s->globalRegion.west) / nPixels;

/* -------------------------------------------------------------------- */
/*      Establish the projection                                        */
/* -------------------------------------------------------------------- */
    pszWKT = (char *) GDALGetProjectionRef(spriv->hDS);
    
    spriv->pszProjection = NULL;

    hSRS = OSRNewSpatialReference(NULL);
    if( OSRImportFromWkt( hSRS, &pszWKT ) != OGRERR_NONE
        || OSRExportToProj4( hSRS, &(spriv->pszProjection)) != OGRERR_NONE )
    {
        /* notdef: what should we used for "ungeoreferenced" datasets? */
        spriv->pszProjection = strdup("+proj=utm +ellps=clrk66 +zone=13");
    }
    OSRDestroySpatialReference( hSRS );

    ecs_SetSuccess(&(s->result));
    return &(s->result);
}

/************************************************************************/
/*                         dyn_DestroyServer()                          */
/************************************************************************/

ecs_Result *dyn_DestroyServer(s)
     ecs_Server *s;
{
    ServerPrivateData *spriv = s->priv;

    /* 
       Release all layers selection.
    */
  
    _releaseAllLayers(s);
  
    /* 
       Release spriv
    */

    if (spriv != NULL) {
        free( spriv->pszProjection );
        if( spriv->hDS != NULL )
            GDALClose( spriv->hDS );
        free(spriv);
    }

    ecs_SetSuccess(&(s->result));
    return &(s->result);
}

/************************************************************************/
/*                          dyn_SelectLayer()                           */
/************************************************************************/

ecs_Result *dyn_SelectLayer(ecs_Server *s, ecs_LayerSelection *sel)

{
    int layer;
    LayerPrivateData *lpriv;
    ServerPrivateData *spriv = (ServerPrivateData *) s->priv;

    /* 
       First, try to find an existing layer with same request and family
       using ecs_GetLayer.
    */

    if ((layer = ecs_GetLayer(s,sel)) != -1) {
    
        /*
          If it already exists than assign currentLayer and set index to 0
          to force a rewind.
        */
    
        s->currentLayer = layer;
        s->layer[layer].index = 0;
        ecs_SetSuccess(&(s->result));
        return &(s->result);
    }

    /* 
       Is the layer name valid? 
    */

    if( strncmp(sel->Select,"band_",5) != 0 
        || atoi(sel->Select+5) < 1 
        || atoi(sel->Select+5) > GDALGetRasterCount(spriv->hDS) )
    {
        ecs_SetError(&(s->result),1,
                     "Illegal layer identifier.");
        return &(s->result);	
    }

    /* 
       It did not exist so we try to create it with ecs_SetLayer. Don't
       forget to set the current layer to this new layer.
    */

    if ((layer = ecs_SetLayer(s,sel)) == -1) {
        return &(s->result);
    }
    s->currentLayer = layer;
	
    /* 
       Allocate memory to hold private information about this new
       layer. 
    */

    s->layer[layer].priv = (void *) malloc(sizeof(LayerPrivateData));
    if (s->layer[layer].priv == NULL) {
        ecs_FreeLayer(s,layer);
        ecs_SetError(&(s->result),1,
                     "Not enough memory to allocate layer private data");
        return &(s->result);	
    }

    lpriv = (LayerPrivateData *) s->layer[layer].priv;

    lpriv->nBand = atoi(sel->Select+5);
    lpriv->hBand = GDALGetRasterBand( spriv->hDS, lpriv->nBand );
    lpriv->nOGDIImageType = 0;

    lpriv->dfMatrixScale = 1.0;
    lpriv->dfMatrixOffset = 0.0;

    if( sel->F == Image )
    {
        switch( GDALGetRasterDataType( lpriv->hBand ) )
        {
          case GDT_Byte:
            lpriv->nOGDIImageType = 2;
            lpriv->nGDALImageType = GDT_Byte;
            break;

          case GDT_UInt16:
            lpriv->nOGDIImageType = 3;
            lpriv->nGDALImageType = GDT_UInt16;
            break;

          case GDT_Int16:
            lpriv->nOGDIImageType = 4;
            lpriv->nGDALImageType = GDT_Int16;
            break;

          default:
            lpriv->nOGDIImageType = 5;
            lpriv->nGDALImageType = GDT_Int32;
            break;
        }
    }

    s->layer[layer].index = 0;

    return &(s->result);
}

/************************************************************************/
/*                          dyn_ReleaseLayer()                          */
/************************************************************************/

ecs_Result *dyn_ReleaseLayer(ecs_Server *s, ecs_LayerSelection *sel)

{
  int layer;
  char buffer[200];

  /* 
     First, try to find an existing layer with same request and family. 
     */

  if ((layer = ecs_GetLayer(s,sel)) == -1) {
    sprintf(buffer,"Invalid layer %s",sel->Select);
    ecs_SetError(&(s->result),1,buffer);
    return &(s->result);
  }

  /* 
     Free the content of lpriv.
     */

  free( s->layer[layer].priv );

  /* 
     Free the layer.
     */

  ecs_FreeLayer(s,layer);

  if (s->currentLayer == layer) {
    s->currentLayer = -1;   /* just in case released layer was selected */
  }

  ecs_SetSuccess(&(s->result));
  return &(s->result);
}

/************************************************************************/
/*                         _releaseAllLayers()                          */
/************************************************************************/

static void _releaseAllLayers(ecs_Server *s)

{
    int i;

    for (i = 0; i < s->nblayer; ++i)
        dyn_ReleaseLayer(s,&(s->layer[i].sel));
}

/************************************************************************/
/*                          dyn_SelectRegion()                          */
/************************************************************************/

ecs_Result *dyn_SelectRegion(ecs_Server *s,ecs_Region *gr)

{
    s->currentRegion.north = gr->north;
    s->currentRegion.south = gr->south;
    s->currentRegion.east = gr->east;			
    s->currentRegion.west = gr->west;
    s->currentRegion.ns_res = gr->ns_res;
    s->currentRegion.ew_res = gr->ew_res;
  
    /* 
       Reset currentLayer index to 0 to force a rewind.
    */
  
    if (s->currentLayer != -1) {
        s->layer[s->currentLayer].index = 0;
    }
  
    ecs_SetSuccess(&(s->result));
    return &(s->result);	
}

/************************************************************************/
/*                         dyn_GetDictionary()                          */
/************************************************************************/

ecs_Result *dyn_GetDictionary(ecs_Server *s)

{
    if (ecs_SetText(&(s->result),"")) {
        ecs_SetSuccess(&(s->result));
    }
    return &(s->result);
}

/************************************************************************/
/*                         dyn_GetNextObject()                          */
/************************************************************************/

ecs_Result *dyn_GetNextObject(ecs_Server *s)

{
    ServerPrivateData *spriv = s->priv;
    LayerPrivateData *lpriv = (LayerPrivateData *) 
        				s->layer[s->currentLayer].priv;
    double	dfULX, dfULY, dfLRX, dfLRY, dfRatio;
    int         nXOff, nYOff, nXSize, nYSize;
    int         nOutOffset, nFullSize, nOutSize, i;
    int         nRasterXSize = GDALGetRasterXSize(spriv->hDS);
    int         nRasterYSize = GDALGetRasterYSize(spriv->hDS);

/* -------------------------------------------------------------------- */
/*      Compute desired region in "georeferenced" coordinates.          */
/* -------------------------------------------------------------------- */
    dfULX = s->currentRegion.west;
    dfULY = s->currentRegion.north 
        - s->currentRegion.ns_res * s->layer[s->currentLayer].index;
    dfLRX = s->currentRegion.east;
    dfLRY = s->currentRegion.north 
        - s->currentRegion.ns_res * (s->layer[s->currentLayer].index+1);

    if( (dfULY+dfLRY)*0.5 < s->currentRegion.south )
    {
        ecs_SetError( &(s->result), 2, "End of selection" );
        return &(s->result);
    }

/* -------------------------------------------------------------------- */
/*      Convert into pixel coordinates but don't integerize yet.        */
/* -------------------------------------------------------------------- */
    dfULX = (dfULX - spriv->adfGeoTransform[0]) / spriv->adfGeoTransform[1];
    dfLRX = (dfLRX - spriv->adfGeoTransform[0]) / spriv->adfGeoTransform[1];
    dfULY = (dfULY - spriv->adfGeoTransform[3]) / spriv->adfGeoTransform[5];
    dfLRY = (dfLRY - spriv->adfGeoTransform[3]) / spriv->adfGeoTransform[5];

/* -------------------------------------------------------------------- */
/*      Convert into a raster window, but without clamping to the       */
/*      actual raster bounds.                                           */
/* -------------------------------------------------------------------- */
    nXOff = (int) floor(dfULX+0.5);
    nYOff = (int) floor(dfULY+0.5);
    nXSize = (int) floor(dfLRX+0.5) - nXOff;
    nYSize = (int) floor(dfLRY+0.5) - nYOff;
    
    nXSize = MAX(1,nXSize);
    nYSize = MAX(1,nYSize);

/* -------------------------------------------------------------------- */
/*      Is this entirely on the raster?  If not compute the reduced     */
/*      window, and the region on the current scanline buffer to set    */
/*      from it.                                                        */
/* -------------------------------------------------------------------- */
    nFullSize = (int) floor((s->currentRegion.east - s->currentRegion.west)
                           / s->currentRegion.ew_res + 0.1);

    nOutOffset = 0;
    nOutSize = nFullSize;
    dfRatio = nFullSize / (double) nXSize;

    if( nXOff < 0 )
    {
        nOutOffset = (int) floor((-nXOff) * dfRatio + 0.5);
        nOutSize -= nOutOffset;
        nXSize += nXOff;
        nXOff = 0;
    }

    if( nXOff + nXSize > nRasterXSize )
    {
        int nNewXSize = nRasterXSize - nXOff;
        
        nOutSize = nOutSize - (nXSize - nNewXSize) * dfRatio;
        nXSize = nNewXSize;
    }

    if( nYOff < 0 )
    {
        nYSize += nYOff;
        nYOff = 0;
    }

    nYSize = MAX(1,nYSize);
    if( nYOff + nYSize > nRasterYSize )
    {
        nYSize = nRasterYSize - nYOff;
    }
   
/* -------------------------------------------------------------------- */
/*      Setup the buffer, setting to zero initially - for Matrix        */
/*      results.                                                        */
/* -------------------------------------------------------------------- */
    if( s->layer[s->currentLayer].sel.F == Matrix )
    {
        void	*pData;

        ecs_SetGeomMatrix( &(s->result), nFullSize );
        pData = 
       s->result.res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.matrix.x.x_val;

        memset( pData, 0, 4 * nFullSize );

        if( nXSize > 0 && nYSize > 0 )
        {
            GDALRasterIO( lpriv->hBand, GF_Read, nXOff, nYOff, nXSize, nYSize, 
                          ((float *) pData) + nOutOffset, 
                          nOutSize, 1, GDT_Float32, 0, 0 );

            for( i = nOutOffset; i < nOutOffset+nOutSize; i++ )
            {
                ((GUInt32 *) pData)[i] = (int) 
                    (((float *) pData)[i] * lpriv->dfMatrixScale 
                     + lpriv->dfMatrixOffset);
            }
        }
    }

/* -------------------------------------------------------------------- */
/*      Setup the buffer, setting to zero initially - for Matrix        */
/*      results.                                                        */
/* -------------------------------------------------------------------- */
    else if( s->layer[s->currentLayer].sel.F == Image )
    {
        GByte 	*pabyData;
        int	nBytesPerWord;

        nBytesPerWord = GDALGetDataTypeSize(lpriv->nGDALImageType)/8;

        ecs_SetGeomImage( &(s->result), nFullSize );
        pabyData = (GByte *) 
       s->result.res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.matrix.x.x_val;

        memset( pabyData, 0, 4 * nFullSize );

        if( nXSize > 0 && nYSize > 0 )
        {
            GDALRasterIO( lpriv->hBand, GF_Read, nXOff, nYOff, nXSize, nYSize, 
                          pabyData + nOutOffset * nBytesPerWord,
                          nOutSize, 1, lpriv->nGDALImageType, 0, 0 );
        }
    }

/* -------------------------------------------------------------------- */
/*      Increment current line indicator.                               */
/* -------------------------------------------------------------------- */
    s->layer[s->currentLayer].index += 1;

    ecs_SetSuccess(&(s->result));

    return &(s->result);
}

/************************************************************************/
/*                           dyn_GetObject()                            */
/************************************************************************/

ecs_Result *dyn_GetObject(ecs_Server *s, char *Id)

{
    int		nOldIndex;

    nOldIndex = s->layer[s->currentLayer].index;
    s->layer[s->currentLayer].index = atoi(Id);

    dyn_GetNextObject( s );

    s->layer[s->currentLayer].index = nOldIndex;

    return &(s->result);
}

/************************************************************************/
/*                      dyn_GetObjectIdFromCoord()                      */
/************************************************************************/

ecs_Result *dyn_GetObjectIdFromCoord( ecs_Server *s,
                                      ecs_Coordinate *coord)

{
    return &(s->result);
}

/************************************************************************/
/*                        dyn_UpdateDictionary()                        */
/************************************************************************/

ecs_Result *dyn_UpdateDictionary(ecs_Server *s, char *arg)

{
    ServerPrivateData *spriv = s->priv;
    int               i;

    /* Make sure an empty list is returned in all cases */ 

    ecs_SetText(&(s->result),""); 

    if( strcmp(arg,"ogdi_server_capabilities") == 0 )
    {
        ecs_AddText(&(s->result),
                    "<?xml version=\"1.0\" ?>\n"
                    "<OGDI_Capabilities version=\"3.1\">\n"
                    "</OGDI_Capabilities>\n" );
    }

    else if( strcmp(arg,"ogdi_capabilities") == 0 )
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

        for( i = 0; i < GDALGetRasterCount(spriv->hDS); i++ )
        {
            ecs_AddText(&(s->result),
                        "      <FeatureType>\n" );

            sprintf( line, "         <Name>band_%d</Name>\n", i+1 );
            ecs_AddText(&(s->result),line);

            sprintf( line, "         <SRS>PROJ4:%s</SRS>\n", 
                     spriv->pszProjection );
            ecs_AddText(&(s->result),line);

            sprintf(line, 
                    "         <SRSBoundingBox minx=\"%.9f\"  miny=\"%.9f\"\n"
                    "                         maxx=\"%.9f\"  maxy=\"%.9f\"\n"
                    "                         x_res=\"%.9f\" y_res=\"%.9f\" />\n",
                    s->globalRegion.west, s->globalRegion.south, 
                    s->globalRegion.east, s->globalRegion.north,
                    s->globalRegion.ew_res, s->globalRegion.ns_res );
            ecs_AddText(&(s->result),line);
      
            ecs_AddText(&(s->result),
                        "         <Family>Matrix</Family>\n"
                        "         <Family>Image</Family>\n" 
                        "      </FeatureType>\n" );
        }
        ecs_AddText(&(s->result),
                    "   </FeatureTypeList>\n" 
                    "</OGDI_Capabilities>\n" );
    }

    ecs_SetSuccess(&(s->result));

    return &(s->result);
}

/************************************************************************/
/*                      dyn_GetServerProjection()                       */
/************************************************************************/

ecs_Result *dyn_GetServerProjection(ecs_Server *s)

{
    ServerPrivateData *spriv=s->priv;

    ecs_SetText(&(s->result), spriv->pszProjection);
  
    ecs_SetSuccess(&(s->result));
    return &(s->result);
}

/************************************************************************/
/*                         dyn_GetGlobalBound()                         */
/************************************************************************/

ecs_Result *dyn_GetGlobalBound(ecs_Server *s)

{
    ecs_SetGeoRegion(&(s->result),
                     s->globalRegion.north, 
                     s->globalRegion.south, 
                     s->globalRegion.east, 
                     s->globalRegion.west, 
                     s->globalRegion.ns_res, 
                     s->globalRegion.ew_res);
    ecs_SetSuccess(&(s->result));
    return &(s->result);
}

/************************************************************************/
/*                       dyn_SetServerLanguage()                        */
/************************************************************************/

ecs_Result *dyn_SetServerLanguage( ecs_Server *s, u_int language)

{
    (void) language;

    ecs_SetSuccess(&(s->result));
    return &(s->result);
}


/************************************************************************/
/*                         dyn_SetCompression()                         */
/************************************************************************/

ecs_Result *dyn_SetCompression(ecs_Server *s, ecs_Compression *compression)

{
    (void) compression;

    ecs_SetSuccess(&(s->result));
    return &(s->result);
}

/************************************************************************/
/*                         dyn_GetRasterInfo()                          */
/************************************************************************/

ecs_Result *dyn_GetRasterInfo(ecs_Server *s)

{
    ServerPrivateData *spriv = s->priv;
    LayerPrivateData *lpriv;

    lpriv = (LayerPrivateData *) s->layer[s->currentLayer].priv;

/* -------------------------------------------------------------------- */
/*      Handle Matrix                                                   */
/* -------------------------------------------------------------------- */
    if( s->layer[s->currentLayer].sel.F == Matrix )
    {
        int     i;
        char    szName[64];
        GDALColorTableH hCT;

        ecs_SetRasterInfo(&(s->result),
                          GDALGetRasterXSize( spriv->hDS ),
                          GDALGetRasterYSize( spriv->hDS ) );

        hCT = GDALGetRasterColorTable( lpriv->hBand );
        if( hCT != NULL )
        {
            for( i = 0; i < GDALGetColorEntryCount(hCT); i++ ) {
                GDALColorEntry  sColor;

                GDALGetColorEntryAsRGB(hCT, i, &sColor);
                sprintf(szName,"%d",i);

                if( sColor.c4 > 0 ) 
                    ecs_AddRasterInfoCategory(&(s->result), 
                                              i+1, 
                                              sColor.c1, 
                                              sColor.c2, 
                                              sColor.c3, 
                                              szName, 0 );
            }
        }
        else
        {
            for( i = 1; i < 255; i++ ) {
                sprintf(szName,"%d-%d",
                 (int) ((i / lpriv->dfMatrixScale) + lpriv->dfMatrixOffset),
                 (int) (((i+1)/lpriv->dfMatrixScale)+lpriv->dfMatrixOffset-1));

                ecs_AddRasterInfoCategory(&(s->result), i, i, i, i, szName, 0);
            }
        }

        ecs_SetSuccess(&(s->result));
    }

/* -------------------------------------------------------------------- */
/*      Handle Image                                                    */
/* -------------------------------------------------------------------- */
    else if( s->layer[s->currentLayer].sel.F == Image )
    {
        ecs_SetRasterInfo(&(s->result), lpriv->nOGDIImageType, 0 );

        ecs_AddRasterInfoCategory(&(s->result),1, 255, 255, 255,"No data",0);

        s->result.res.ecs_ResultUnion_u.ri.mincat = 0;
        s->result.res.ecs_ResultUnion_u.ri.maxcat = 255;

        ecs_SetSuccess(&(s->result));
    }

    else
    {
        ecs_SetError(&(s->result), 1, 
                     "The current layer is not a Matrix or Image");
    }

    return &(s->result);
}
