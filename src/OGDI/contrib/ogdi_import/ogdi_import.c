/******************************************************************************
 * $Id: ogdi_import.c,v 1.13 2007/02/12 15:52:57 cbalint Exp $
 *
 * Project:  OGDI Contributed Clients
 * Purpose:  Simple console import to shapefile/raw raster.
 * Author:   Frank Warmerdam <warmerdam@pobox.com>
 *
 ******************************************************************************
 * Copyright (c) 2000, Frank Warmerdam <warmerdam@pobox.com>
 *
 * Permission to use, copy, modify and distribute this software and
 * its documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies, that
 * both the copyright notice and this permission notice appear in
 * supporting documentation, and that the name of the author not be used 
 * in advertising or publicity pertaining to distribution of the software 
 * without specific, written prior permission.   The author makes no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 ******************************************************************************
 *
 * $Log: ogdi_import.c,v $
 * Revision 1.13  2007/02/12 15:52:57  cbalint
 *
 *    Preliminary cleanup.
 *    Get rif of unitialized variables, and unused ones.
 *
 * Revision 1.12  2002/02/21 15:54:17  warmerda
 * fixed bug with declaration of dst
 *
 * Revision 1.11  2002/02/08 21:22:58  warmerda
 * fixed serious bug importing floating point fields of unknown precision
 *
 * Revision 1.10  2001/12/11 18:34:18  warmerda
 * fixed region handling if GetLayerRegion() fails - default to global bounds.
 *
 * Revision 1.9  2001/11/14 04:55:47  warmerda
 * added -no-dict option
 *
 * Revision 1.8  2001/10/09 22:48:27  warmerda
 * preliminary support for setting output projection
 *
 * Revision 1.7  2001/10/02 02:06:25  warmerda
 * various bug fixes related to region setting
 *
 * Revision 1.6  2001/08/16 13:44:52  warmerda
 * fixed roundoff bug xsize/ysize calc from region
 *
 * Revision 1.5  2001/07/17 19:03:42  warmerda
 * Added support for exporting lines, text and areas.
 *
 * Revision 1.4  2001/06/22 16:41:42  warmerda
 * Enabled RGBA image support.
 *
 * Revision 1.3  2001/05/30 19:13:06  warmerda
 * utilize layer capabitilies if available to set region and resolution
 *
 * Revision 1.2  2000/11/23 19:15:57  warmerda
 * added header block
 *
 */

#include "ecs.h"
#include "shapefil.h"
#include <assert.h>
#include "projects.h"

#ifndef PJ_VERSION
#define projPJ PJ*
#define projUV UV
#endif

static int	ClientID = -1;
static int      bNoDict = FALSE;

#ifndef MAX
#  define MIN(a,b)      ((a<b) ? a : b)
#  define MAX(a,b)      ((a>b) ? a : b)
#endif

/************************************************************************/
/*                             CheckError()                             */
/************************************************************************/

static int CheckError( ecs_Result * result )

{
    if( ECSERROR( result ) ) {
        printf( "ECSERROR: %s\n", result->message );
        return TRUE;
    }
    else
        return FALSE;
}

/************************************************************************/
/*                             AccessURL()                              */
/************************************************************************/

static int AccessURL( char * url, ecs_Region * region )

{
    ecs_Result *result;

/* -------------------------------------------------------------------- */
/*      Close old client if there is one active.                        */
/* -------------------------------------------------------------------- */
    if( ClientID != -1 ) {
        result = cln_DestroyClient(ClientID);
        if( CheckError( result ) )
            return( FALSE );
        ClientID = -1;
    }
            
/* -------------------------------------------------------------------- */
/*      Open new URL/client.                                            */
/* -------------------------------------------------------------------- */
    result = cln_CreateClient(&ClientID,url);
    if( CheckError( result ) )
        return( FALSE );

/* -------------------------------------------------------------------- */
/*      print the global bounds.                                        */
/* -------------------------------------------------------------------- */
    result = cln_GetGlobalBound(ClientID);
    if( CheckError( result ) )
        return( FALSE );

    *region = ECSREGION(result);

/* -------------------------------------------------------------------- */
/*      Print the Dictionary (update).                                  */
/* -------------------------------------------------------------------- */
    result = cln_UpdateDictionary( ClientID, "" );
    if( CheckError( result ) )
        return( FALSE );

    if( !bNoDict )
        printf( "UpdateDictionary = \n%s\n", ECSTEXT(result) );
  
/* -------------------------------------------------------------------- */
/*      Print the projection.                                           */
/* -------------------------------------------------------------------- */
    result = cln_GetServerProjection( ClientID );
    if( CheckError( result ) )
        return( FALSE );

    printf( "Projection = `%s'\n", ECSTEXT(result) );
  
    return( TRUE );
}

/************************************************************************/
/*                           GetLayerRegion()                           */
/*                                                                      */
/*      Fetch the region and resolution for the named layer if          */
/*      possible, from the capabilities.                                */
/************************************************************************/

static void GetLayerRegion( const char *layerName, ecs_Region *region )

{
    const ecs_LayerCapabilities *layer;
    int			   layer_index;

    for( layer_index = 0; 
         (layer = cln_GetLayerCapabilities(ClientID,layer_index)) != NULL;
         layer_index++ )
    {
        if( strcmp(layer->name,layerName) == 0 )
        {
            region->north = layer->srs_north;
            region->south = layer->srs_south;
            region->east = layer->srs_east;
            region->west = layer->srs_west;
            region->ew_res = layer->srs_ewres;
            region->ns_res = layer->srs_nsres;
            return;
        }
    }
}

/************************************************************************/
/*                            ImportVectors()                           */
/************************************************************************/

static void ImportVectors( ecs_Region *region, const char * layer,
                           const char * out_file,
                           ecs_Family featureType )

{
    ecs_Result *result;
    ecs_LayerSelection selectionLayer;
    SHPHandle   hShape = NULL;
    DBFHandle   hDBF;
    char	filename[1024];
    ecs_ObjAttributeFormat *oaf;
    int		i, field_count, iText = -1, nFld;

/* -------------------------------------------------------------------- */
/*      Select a region ... this should be overridable from the         */
/*      command line.                                                   */
/* -------------------------------------------------------------------- */
    result = cln_SelectRegion(ClientID,region);
    if( CheckError( result ) )
        return;

/* -------------------------------------------------------------------- */
/*      Define the layer to select.  For now we only support lines.     */
/* -------------------------------------------------------------------- */
    selectionLayer.Select = (char *) layer;
    selectionLayer.F = featureType;
    
    result = cln_SelectLayer(ClientID,&selectionLayer);
    
    if( CheckError(result) )
        return;

/* -------------------------------------------------------------------- */
/*      Create the shapefile to write the lines to.                     */
/* -------------------------------------------------------------------- */
    sprintf( filename, "%s.shp", out_file );
    if( featureType == Line )
        hShape = SHPCreate( filename, SHPT_ARC );
    else if( featureType == Point || featureType == Text )
        hShape = SHPCreate( filename, SHPT_POINT );
    else if( featureType == Area )
        hShape = SHPCreate( filename, SHPT_POLYGON );

    if( hShape == NULL )
    {
        fprintf( stderr, "Unable to create shapefile dataset %s\n", filename );
        return;
    }

    sprintf( filename, "%s.dbf", out_file );
    hDBF = DBFCreate( filename );

    if( hDBF == NULL )
    {
        fprintf( stderr, "Unable to create DBF file %s\n", filename );
        return;
    }

/* -------------------------------------------------------------------- */
/*      Setup DBF schema.                                               */
/* -------------------------------------------------------------------- */
    result = cln_GetAttributesFormat( ClientID );
    if( CheckError( result ) )
        return;
        
    oaf = &(ECSRESULT(result).oaf);
    field_count = oaf->oa.oa_len;
    for( i = 0; i < field_count; i++ )
    {
        switch( oaf->oa.oa_val[i].type )
        {
          case Char:
          case Varchar:
          case Longvarchar:
            if( oaf->oa.oa_val[i].lenght > 0 )
                nFld = DBFAddField( hDBF, oaf->oa.oa_val[i].name,
                                    FTString, oaf->oa.oa_val[i].lenght, 0 );
            else
                nFld = DBFAddField( hDBF, oaf->oa.oa_val[i].name,
                                    FTString, 64, 0 );
            break;

          case Decimal:
          case Smallint:
          case Integer:
            if( oaf->oa.oa_val[i].lenght > 0 )
                nFld = DBFAddField( hDBF, oaf->oa.oa_val[i].name,
                                    FTDouble, oaf->oa.oa_val[i].lenght, 0 );
            else 
                nFld = DBFAddField( hDBF, oaf->oa.oa_val[i].name,
                                    FTString, 11, 0 );
            break;

          case Numeric:
          case Real:
          case Float:
          case Double:
            if( oaf->oa.oa_val[i].lenght > 0 )
                nFld = DBFAddField( hDBF, oaf->oa.oa_val[i].name,
                                    FTDouble,
                                    oaf->oa.oa_val[i].lenght,
                                    oaf->oa.oa_val[i].precision );
            else
                nFld = DBFAddField( hDBF, oaf->oa.oa_val[i].name,
                                    FTDouble, 18, 7 );
            break;

          default:
            nFld = 0;
            fprintf( stderr, "Field %s of unrecognised type %d dropped.\n", 
                     oaf->oa.oa_val[i].name, oaf->oa.oa_val[i].type );
            break;
        }

        if( nFld == -1 )
        {
            fprintf( stderr, 
                     "Field %s was not created due to a failure in DBFAddField()\n",
                     oaf->oa.oa_val[i].name );
                     
        }
    }

    if( featureType == Text )
    {
        iText = DBFAddField( hDBF, "text", FTString, 64, 0 );
    }    

/* -------------------------------------------------------------------- */
/*      Process objects.                                                */
/* -------------------------------------------------------------------- */
    result = cln_GetNextObject(ClientID);
    while (ECSSUCCESS(result))
    {
        char	*pszList;
        int	iField, iShape=0;
        
/* -------------------------------------------------------------------- */
/*      Write shapefile geometry                                        */
/* -------------------------------------------------------------------- */
        
        if( featureType == Area ) {
            ecs_Area	*area = &(ECSGEOM(result).area);
            double	*x, *y;
            int		*parts, nPoints, iRing;
            SHPObject   *psShape;

            nPoints = 0;
            for( iRing = 0; iRing < (int) area->ring.ring_len; iRing++ )
            {
                ecs_FeatureRing	*ring = area->ring.ring_val + iRing;

                nPoints += ring->c.c_len;
            }
            
            parts = (int *) malloc(sizeof(int) * area->ring.ring_len);
            x = (double *) malloc(sizeof(double) * nPoints);
            y = (double *) malloc(sizeof(double) * nPoints);

            nPoints = 0;
            for( iRing = 0; iRing < (int) area->ring.ring_len; iRing++ )
            {
                ecs_FeatureRing	*ring = area->ring.ring_val + iRing;

                parts[iRing] = nPoints;
                for( i = 0; i < (int) ring->c.c_len; i++ )
                {
                    x[nPoints] = ring->c.c_val[i].x;
                    y[nPoints] = ring->c.c_val[i].y;
                    nPoints++;
                }
            }
            
            psShape = SHPCreateObject( SHPT_POLYGON, -1, area->ring.ring_len,
                                       parts, NULL, nPoints, x, y, NULL, NULL);

            free( x );
            free( y );
            free( parts );
            
            iShape = SHPWriteObject( hShape, -1, psShape );

            SHPDestroyObject( psShape );
        }

        else if( featureType == Line ) {
            ecs_Line	*line = &(ECSGEOM(result).line);
            double	*x, *y;
            SHPObject   *psShape;

            x = (double *) malloc(sizeof(double) * line->c.c_len);
            y = (double *) malloc(sizeof(double) * line->c.c_len);

            for( i=0; i < (int) line->c.c_len; i++ ) {
                x[i] = line->c.c_val[i].x;
                y[i] = line->c.c_val[i].y;
            }

            psShape = SHPCreateSimpleObject( SHPT_ARC, line->c.c_len,
                                             x, y, NULL );

            free( x );
            free( y );
            
            iShape = SHPWriteObject( hShape, -1, psShape );

            SHPDestroyObject( psShape );
        }

        else if( featureType == Point ) {
            ecs_Point	*point = &(ECSGEOM(result).point);
            SHPObject   *psShape;

            psShape = SHPCreateSimpleObject( SHPT_POINT, 1, 
                                             &(point->c.x), &(point->c.y), 
                                             NULL );

            iShape = SHPWriteObject( hShape, -1, psShape );

            SHPDestroyObject( psShape );
        }

        else if( featureType == Text ) {
            ecs_Text	*point = &(ECSGEOM(result).text);
            SHPObject   *psShape;

            psShape = SHPCreateSimpleObject( SHPT_POINT, 1, 
                                             &(point->c.x), &(point->c.y), 
                                             NULL );

            iShape = SHPWriteObject( hShape, -1, psShape );

            SHPDestroyObject( psShape );

            DBFWriteStringAttribute( hDBF, iShape, iText, point->desc );
        }

/* -------------------------------------------------------------------- */
/*      Write attributes to dbf file.                                   */
/* -------------------------------------------------------------------- */
        pszList = ECSOBJECTATTR(result);
        
        for( iField = 0; iField < field_count; iField++ )
        {
            char	*pszFieldStart;
            int		nNameLen;
            char	chSavedChar;
            
            /* parse out the next attribute value */
            if( !ecs_FindElement( pszList, &pszFieldStart, &pszList,
                                  &nNameLen, NULL ) )
            {
                nNameLen = 0;
                pszFieldStart = pszList;
            }

            /* Skip any trailing white space (for string constants). */

            if( nNameLen > 0 && pszFieldStart[nNameLen-1] == ' ' )
                nNameLen--;

	    /* zero terminate the single field value, but save the           */
            /* character we overwrote, so we can restore it when done.       */
            
            chSavedChar = pszFieldStart[nNameLen];
            pszFieldStart[nNameLen] = '\0';

            switch( DBFGetFieldInfo( hDBF, iField, NULL, NULL, NULL ) )
            {
              case FTString:
                DBFWriteStringAttribute( hDBF, iShape, iField, pszFieldStart );
                break;

              case FTInteger:
                DBFWriteIntegerAttribute( hDBF, iShape, iField,
                                          atoi(pszFieldStart) );
                break;

              case FTDouble:
                DBFWriteDoubleAttribute( hDBF, iShape, iField,
                                         atof(pszFieldStart) );
                break;

              default:
		break;
            }

            pszFieldStart[nNameLen] = chSavedChar;
        }
        
/* -------------------------------------------------------------------- */
/*      Read next object                                                */
/* -------------------------------------------------------------------- */
        result = cln_GetNextObject(ClientID);
    }

    SHPClose( hShape );
    DBFClose( hDBF );
}

/************************************************************************/
/*                            ImportMatrix()                            */
/************************************************************************/

static void ImportMatrix( ecs_Region *region, const char * layer,
                          const char * out_file )

{
    FILE	*fp_aux, *fp_raw;
    char	filename[256];
    int		i, xsize, ysize;
    ecs_Result *result;
    ecs_LayerSelection selectionLayer;

/* -------------------------------------------------------------------- */
/*      Select a region ... this should be overridable from the         */
/*      command line.                                                   */
/* -------------------------------------------------------------------- */
    result = cln_SelectRegion(ClientID,region);
    if( CheckError( result ) )
        return;

    xsize = (int) ((region->east - region->west) / region->ew_res + 0.5);
    ysize = (int) ((region->north - region->south) / region->ns_res + 0.5);

/* -------------------------------------------------------------------- */
/*      Define the layer to select.                                     */
/* -------------------------------------------------------------------- */
    selectionLayer.Select = (char *) layer;
    selectionLayer.F = Matrix;
    
    result = cln_SelectLayer(ClientID,&selectionLayer);
    
    if( CheckError( result ) )
        return;

/* -------------------------------------------------------------------- */
/*	Write .aux file header.						*/
/* -------------------------------------------------------------------- */ 
    sprintf( filename, "%s.aux", out_file );
    fp_aux = fopen( filename, "wt" );

    sprintf( filename, "%s.raw", out_file );

    fprintf( fp_aux, "AuxilaryTarget: %s\n", filename );

/* -------------------------------------------------------------------- */
/*      Get the raster information.                                     */
/* -------------------------------------------------------------------- */
    result = cln_GetRasterInfo( ClientID );
    if( CheckError( result ) )
        return;

    fprintf( fp_aux, "RawDefinition: %d %d 1\n", xsize, ysize );

    fprintf( fp_aux, "ChanDefinition-1: 8U 0 1 %d Swapped\n", xsize );

    for( i = 0; i < (int) ECSRASTERINFO(result).cat.cat_len; i++ ) {
        fprintf( fp_aux, "METADATA_IMG_1_Class_%d_Name: %s\n",
                 (int) ECSRASTERINFO(result).mincat + i, 
                 ECSRASTERINFO(result).cat.cat_val[i].label );
        fprintf( fp_aux, "METADATA_IMG_1_Class_%d_Color: (RGB:%d %d %d)\n",
                 (int) ECSRASTERINFO(result).mincat + i, 
                 ECSRASTERINFO(result).cat.cat_val[i].r,
                 ECSRASTERINFO(result).cat.cat_val[i].g,
                 ECSRASTERINFO(result).cat.cat_val[i].b );
    }

/* -------------------------------------------------------------------- */
/*      Attach the projection information.                              */
/* -------------------------------------------------------------------- */
    fprintf( fp_aux, "MapUnits: METER\n" );
    fprintf( fp_aux, "UpLeftX: %24.15E\n", region->west );
    fprintf( fp_aux, "UpLeftY: %25.15E\n", region->north );
    fprintf( fp_aux, "LoRightX: %25.15E\n", region->east );
    fprintf( fp_aux, "LoRightY: %25.15E\n", region->south );

    fclose( fp_aux );

/* -------------------------------------------------------------------- */
/*      open the raw data file.                                         */
/* -------------------------------------------------------------------- */
    fp_raw = fopen( filename, "wb" );
    
    result = cln_GetNextObject(ClientID);
    while (ECSSUCCESS(result)) {
        
        static int report_flag = 0;
        
        if( (int) ECSOBJECT(result).geom.ecs_Geometry_u.matrix.x.x_len
            != xsize && report_flag == 0 ) {

            report_flag = 1;
            printf( "Got %d pixels, instead of expected %d pixels!\n",
                    (int) ECSOBJECT(result).geom.ecs_Geometry_u.matrix.x.x_len,
                    xsize );
        }
            
        for( i = 0; i < xsize; i++ )
        {
            fprintf( fp_raw, "%c", ECSRASTER(result)[i] );
        }

        result = cln_GetNextObject(ClientID);
    }

    CheckError( result );

    fclose( fp_raw );
}

/************************************************************************/
/*                            ImportImage()                             */
/************************************************************************/

static void ImportImage( ecs_Region *region, const char * layer,
                         const char * out_file )

{
    FILE	*fp_aux, *fp_raw;
    char	filename[256];
    int		i, xsize, ysize, width_code;
    ecs_Result *result;
    ecs_LayerSelection selectionLayer;

/* -------------------------------------------------------------------- */
/*      Select a region ... this should be overridable from the         */
/*      command line.                                                   */
/* -------------------------------------------------------------------- */
    result = cln_SelectRegion(ClientID,region);
    if( CheckError( result ) )
        return;

    xsize = (int) ((region->east - region->west) / region->ew_res + 0.5);
    ysize = (int) ((region->north - region->south) / region->ns_res + 0.5);

/* -------------------------------------------------------------------- */
/*      Define the layer to select.                                     */
/* -------------------------------------------------------------------- */
    selectionLayer.Select = (char *) layer;
    selectionLayer.F = Image;
    
    result = cln_SelectLayer(ClientID,&selectionLayer);
    
    if( CheckError( result ) )
        return;

/* -------------------------------------------------------------------- */
/*	Write .aux file header.						*/
/* -------------------------------------------------------------------- */ 
    sprintf( filename, "%s.aux", out_file );
    fp_aux = fopen( filename, "wt" );

    sprintf( filename, "%s.raw", out_file );

    fprintf( fp_aux, "AuxilaryTarget: %s\n", filename );

/* -------------------------------------------------------------------- */
/*      Get the raster information.                                     */
/* -------------------------------------------------------------------- */
    result = cln_GetRasterInfo( ClientID );
    if( CheckError( result ) )
        return;

    width_code = ECSRASTERINFO(result).width;
    if( width_code == 1 /* RGBA */ ) {
        fprintf( fp_aux, "RawDefinition: %d %d 4\n", xsize, ysize );
        fprintf( fp_aux, "ChanDefinition-1: 8U 0 4 %d\n", xsize*4 );
        fprintf( fp_aux, "ChanDefinition-2: 8U 1 4 %d\n", xsize*4 );
        fprintf( fp_aux, "ChanDefinition-3: 8U 2 4 %d\n", xsize*4 );
        fprintf( fp_aux, "ChanDefinition-4: 8U 3 4 %d\n", xsize*4 );
    } else if( width_code == 5 /* int32 */ ) {
        fprintf( fp_aux, "RawDefinition: %d %d 1\n", xsize, ysize );
        fprintf( fp_aux, "ChanDefinition-1: 16U 0 2 %d Swapped\n", xsize*2 );
    }
    else {
        printf( "Unsupported Image raster type %d.\n",
                ECSRASTERINFO(result).width );
        assert( FALSE );
        return;
    }

/* -------------------------------------------------------------------- */
/*      Attach the projection information.                              */
/* -------------------------------------------------------------------- */
    fprintf( fp_aux, "MapUnits: METER\n" );
    fprintf( fp_aux, "UpLeftX: %24.15E\n", region->west );
    fprintf( fp_aux, "UpLeftY: %25.15E\n", region->north );
    fprintf( fp_aux, "LoRightX: %25.15E\n", region->east );
    fprintf( fp_aux, "LoRightY: %25.15E\n", region->south );

    fclose( fp_aux );

/* -------------------------------------------------------------------- */
/*      open the raw data file.                                         */
/* -------------------------------------------------------------------- */
    fp_raw = fopen( filename, "wb" );
    
    result = cln_GetNextObject(ClientID);
    while (ECSSUCCESS(result)) {
        
        static int report_flag = 0;
        
        if( (int) ECSOBJECT(result).geom.ecs_Geometry_u.matrix.x.x_len
            != xsize && report_flag == 0 ) {

            report_flag = 1;
            printf( "Got %d pixels, instead of expected %d pixels!\n",
                    (int) ECSOBJECT(result).geom.ecs_Geometry_u.matrix.x.x_len,
                    xsize );
        }
            
        for( i = 0; i < xsize; i++ ) {
            if( width_code == 5 ) 
            {
                fprintf( fp_raw, "%c%c",
                         (ECSRASTER(result)[i] & 0xff),
                         (ECSRASTER(result)[i] >> 8) & 0xff );
            } 
            else if( width_code == 1 ) 
            {
                unsigned char red, green, blue, trans;
                
                ecs_GetRGBFromPixel( ECSRASTER(result)[i],
                                     &trans, &red, &green, &blue );
                fprintf( fp_raw, "%c%c%c%c",
                         red, green, blue, trans );
            }
        }

        result = cln_GetNextObject(ClientID);
    }

    fclose( fp_raw );
}

/************************************************************************/
/*                          ParseProjection()                           */
/************************************************************************/

projPJ ParseProjection( const char *projection )

{
    char      wproj[1024];
    char      *proj_parms[30];
    int       parm_count = 0;
    projPJ    pj;

/* -------------------------------------------------------------------- */
/*      Parse into tokens.                                              */
/* -------------------------------------------------------------------- */
    strcpy( wproj, projection );
    proj_parms[parm_count] = strtok( wproj, " " );
    while( proj_parms[parm_count] != NULL )
        proj_parms[++parm_count] = strtok( NULL, " " );

/* -------------------------------------------------------------------- */
/*      Call pj_init()                                                  */
/* -------------------------------------------------------------------- */
    pj = pj_init( parm_count, proj_parms );

    if( pj == NULL )
    {
        printf( "Failed to initialize projection from: %s\n", projection );
    }

    return pj;
}

/************************************************************************/
/*                          RecomputeRegion()                           */
/*                                                                      */
/*      Translate the source region into the indicate output            */
/*      coordinate system.                                              */
/************************************************************************/

static int RecomputeRegion( const char * output_projection,
                            ecs_Region *region )

{
    ecs_Result *result;
    projPJ      src;
    projPJ      dst;
    projUV      corners[4];
    ecs_Region  out_region = {0,0,0,0,0,0};
    int         iCorner, src_xsize, src_ysize, max_dim;
    
/* -------------------------------------------------------------------- */
/*      Fetch source projection.                                        */
/* -------------------------------------------------------------------- */
    result = cln_GetServerProjection( ClientID );
    if( CheckError( result ) )
        return( FALSE );

    src = ParseProjection( ECSTEXT(result) );

/* -------------------------------------------------------------------- */
/*      Parse destination projection.                                   */
/* -------------------------------------------------------------------- */
    dst = ParseProjection( output_projection );

/* -------------------------------------------------------------------- */
/*      Setup input coordinates of corners.                             */
/* -------------------------------------------------------------------- */
    corners[0].u = region->west;
    corners[0].v = region->north;
    corners[1].u = region->east;
    corners[1].v = region->north;
    corners[2].u = region->east;
    corners[2].v = region->south;
    corners[3].u = region->west;
    corners[3].v = region->south;

/* -------------------------------------------------------------------- */
/*      Reproject, and establish new bounding region.                   */
/* -------------------------------------------------------------------- */
    for( iCorner = 0; iCorner < 4; iCorner++ )
    {
        /* Translate source to radians */
        
        if( src != NULL )
        {
            corners[iCorner] = pj_inv( corners[iCorner], src );
        }
        else
        {
            corners[iCorner].u *= DEG_TO_RAD;
            corners[iCorner].v *= DEG_TO_RAD;
        }

        /* Translate into destination projection or back to degrees */

        if( dst != NULL )
        {
            corners[iCorner] = pj_fwd( corners[iCorner], dst );
        }
        else
        {
            corners[iCorner].u *= DEG_TO_RAD;
            corners[iCorner].v *= DEG_TO_RAD;
        }

        /* Grow region to hold result */

        if( iCorner == 0 )
        {
            out_region.north = out_region.south = corners[iCorner].v;
            out_region.east = out_region.west = corners[iCorner].u;
        }
        else
        {
            out_region.north = MAX(out_region.north,corners[iCorner].v);
            out_region.south = MIN(out_region.south,corners[iCorner].v);
            out_region.east = MAX(out_region.east,corners[iCorner].u);
            out_region.west = MIN(out_region.west,corners[iCorner].u);
        }
    }

/* -------------------------------------------------------------------- */
/*      Establish output resolution.                                    */
/* -------------------------------------------------------------------- */
    src_xsize = (int) ceil((region->east - region->west) / region->ew_res);
    src_ysize = (int) ceil((region->north - region->south) / region->ns_res);

    max_dim = MAX(src_xsize,src_ysize);
    if( out_region.north - out_region.south >
        out_region.east - out_region.west )
    {
        out_region.ns_res = (out_region.north - out_region.south) / max_dim;
        out_region.ew_res = out_region.ns_res;
    }
    else
    {
        out_region.ew_res = (out_region.east - out_region.west) / max_dim;
        out_region.ns_res = out_region.ew_res;
    }

/* -------------------------------------------------------------------- */
/*      Grow region by one pixel to help with weird round off issues.   */
/* -------------------------------------------------------------------- */
    out_region.north += out_region.ns_res;
    out_region.south -= out_region.ns_res;
    out_region.east += out_region.ew_res;
    out_region.west -= out_region.ew_res;

    *region = out_region;

    printf( "out_region.north = %f\n", out_region.north );
    printf( "out_region.south = %f\n", out_region.south );
    printf( "out_region.east = %f\n", out_region.east );
    printf( "out_region.west = %f\n", out_region.west );
    printf( "out_region.ns_res = %f\n", out_region.ns_res );
    printf( "out_region.ew_res = %f\n", out_region.ew_res );

    return TRUE;
}

/************************************************************************/
/*                                main()                                */
/************************************************************************/

int main( int argc, char ** argv )
{
    ecs_Family	featureType = Point;
    char *layer = "";
    static ecs_Region	reg, *region;
    ecs_Result *result;
    int		i, set_region = FALSE, set_res = FALSE;
    char	*out_file = "ogdi_out";
    const char *output_projection = NULL;

    if( argc == 1 )
    {
        printf("Usage: ogdi_import [-no-dict] -u url -f family\n");
        printf("          [-op output_projection]\n" );
        printf("          [-r north south east west] [-res ns_res ew_res]\n" );
        printf("          [-o filename]\n" );
        printf("          -l layername [more_opts -l layername]\n" );
        exit( 1 );
    }

/* -------------------------------------------------------------------- */
/*      Handle commandline arguments.                                   */
/* -------------------------------------------------------------------- */
    for( i = 1; i < argc; i++ ) {

        if( i == argc - 1 ) {
            /* skip ... the rest require arguments.  */
        }
        else if( strcmp(argv[i],"-u") == 0 ) {
            AccessURL( argv[++i], &reg );
            if( region == NULL )
                region = &reg;
        }
        else if( strcmp(argv[i], "-no-dict") == 0 ) {
            bNoDict = TRUE;
        }
        else if( strcmp(argv[i],"-o") == 0 ) {
            out_file = argv[++i];
        }
        else if( strcmp(argv[i],"-op") == 0 ) {
            output_projection = argv[++i];
            RecomputeRegion( output_projection, region );
            set_res = TRUE;
            set_region = TRUE;
            result = cln_SetClientProjection( ClientID, 
                                              (char *) output_projection );
            if( CheckError( result ) )
                break;
        }
        else if( strcmp(argv[i], "-l") == 0 ) {
            layer = argv[++i];

            if( !set_region )
            {
                ecs_Region   reg2 = *region;

                GetLayerRegion( layer, &reg2 );
                if( set_res )
                {
                    reg2.ew_res = region->ew_res;
                    reg2.ns_res = region->ns_res;
                }
                *region = reg2;
            }

            if( featureType == Matrix ) {
                ImportMatrix( region, layer, out_file );
            }
            else if( featureType == Image ) {
                ImportImage( region, layer, out_file );
            }
            else {
                ImportVectors( region, layer, out_file, featureType );
            }
        }
        else if( strcmp(argv[i], "-f") == 0 ) {
            if( strcmp(argv[i+1],"Point") == 0 )
                featureType = Point;
            else if( strcmp(argv[i+1],"Line") == 0 )
                featureType = Line;
            else if( strcmp(argv[i+1],"Area") == 0 )
                featureType = Area;
            else if( strcmp(argv[i+1],"Text") == 0 )
                featureType = Text;
            else if( strcmp(argv[i+1],"Matrix") == 0 )
                featureType = Matrix;
            else if( strcmp(argv[i+1],"Image") == 0 )
                featureType = Image;
            i++;
        }
        else if( strcmp(argv[i], "-r") == 0 && i < argc - 4 ) {

            region = &reg;
            reg.north = atof(argv[++i]);
            reg.south = atof(argv[++i]);
            reg.east = atof(argv[++i]);
            reg.west = atof(argv[++i]);

            set_region = TRUE;
        }
        else if( strcmp(argv[i], "-res") == 0 && i < argc - 2 ) {

            reg = *region;
            region = &reg;
            
            reg.ns_res = atof(argv[++i]);
            reg.ew_res = atof(argv[++i]);

            set_res = TRUE;
        }
    }

/* -------------------------------------------------------------------- */
/*      Close old client if there is one active.                        */
/* -------------------------------------------------------------------- */
    if( ClientID != -1 ) {
        result = cln_DestroyClient(ClientID);
        if( CheckError( result ) )
            return( FALSE );
        ClientID = -1;
    }

    return 0;
}

