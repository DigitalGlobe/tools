/******************************************************************************
 * $Id: ogdi_info.c,v 1.14 2004/10/26 19:45:53 warmerda Exp $
 *
 * Project:  OGDI Contributed Clients
 * Purpose:  Simple console query program for testing OGDI.
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
 * $Log: ogdi_info.c,v $
 * Revision 1.14  2004/10/26 19:45:53  warmerda
 * Fixed last fix.
 *
 * Revision 1.13  2004/10/26 19:37:52  warmerda
 * Close old clientid if multiple -u's met.
 * Report errors on illegal family specifications.
 *
 * Revision 1.12  2004/02/18 21:27:13  warmerda
 * Use ecs_CleanUp() to recover result memory
 *
 * Revision 1.11  2001/12/14 21:22:02  warmerda
 * dont reproject to latlong if already latlong
 *
 * Revision 1.10  2001/08/17 00:37:15  warmerda
 * updated formatting of capabilities bounds
 *
 * Revision 1.9  2001/07/17 19:08:01  warmerda
 * Fixed reporting of Text layers.
 *
 * Revision 1.8  2001/04/19 05:28:50  warmerda
 * improve region bounds reporting
 *
 * Revision 1.7  2001/04/12 19:26:08  warmerda
 * added RGB image support
 *
 * Revision 1.6  2001/04/12 18:12:42  warmerda
 * Added lots of capabilities related to capabilities
 *
 * Revision 1.5  2001/04/10 14:41:47  warmerda
 * Added support for reporting Image values.
 *
 * Revision 1.4  2001/04/09 16:06:53  warmerda
 * added -no-proj support
 *
 * Revision 1.3  2001/02/19 04:46:13  warmerda
 * use projUV, not UV
 *
 * Revision 1.2  2000/11/23 19:10:36  warmerda
 * added copyright header
 *
 */

#include "ecs.h"
#include "projects.h"

static int	ClientID = -1;
static int      bNoDict = FALSE;
static int      bNoProj = FALSE;
static int      nSampleFrequency = 1;

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
/*                          DumpCapabilities()                          */
/************************************************************************/

static void DumpCapabilities()

{
    const ecs_LayerCapabilities *layer;
    int			   layer_index;
    ecs_Result		  *result;

    result = cln_GetVersion( ClientID );
    if( CheckError( result ) )
        return;

    printf( "Version: `%s'\n", ECSTEXT(result) );

    for( layer_index = 0; 
         (layer = cln_GetLayerCapabilities(ClientID,layer_index)) != NULL;
         layer_index++ )
    {
        printf( "Layer %d: %s\n", layer_index, layer->name );

        if( layer->title )
            printf( "  Title: %s\n", layer->title );

        if( layer->srs )
            printf( "  SRS: %s\n", layer->srs );

        printf( "  Families: " );
        if( layer->families[Area] )
            printf( "Area " );
        if( layer->families[Point] )
            printf( "Point " );
        if( layer->families[Line] )
            printf( "Line " );
        if( layer->families[Text] )
            printf( "Text " );
        if( layer->families[Image] )
            printf( "Image " );
        if( layer->families[Matrix] )
            printf( "Matrix " );
        printf( "\n" );

        printf( "  Bounds: n=%.4f, s=%.4f, nsres=%g\n"
                "          e=%.4f, w=%.4f, ewres=%g\n", 
                layer->srs_north, 
                layer->srs_south, 
                layer->srs_nsres, 
                layer->srs_east, 
                layer->srs_west, 
                layer->srs_ewres );
        if( layer->ll_bounds_set )
        {
            printf( "  LLBounds: n=%.14f, s=%.14f\n"
                    "            e=%.14f, w=%.14f\n", 
                    layer->ll_north, 
                    layer->ll_south, 
                    layer->ll_east, 
                    layer->ll_west );
        }

        if( layer->extensions != NULL )
        {
            int		i;

            printf( "  Extensions:" );

            for( i = 0; layer->extensions[i] != NULL; i++ )
                printf( " %s", layer->extensions[i] );
            
            printf( "\n" );
        }

        if( layer->query_expression_set )
        {
            printf( "  Query Expression:\n" );
            if( layer->qe_prefix != NULL )
                printf( "    prefix=\"%s\"\n", layer->qe_prefix );
            if( layer->qe_suffix != NULL )
                printf( "    suffix=\"%s\"\n", layer->qe_suffix );
            if( layer->qe_format != NULL )
                printf( "    format=\"%s\"\n", layer->qe_format );
            if( layer->qe_description != NULL )
                printf( "    description=\"%s\"\n", layer->qe_description );
        }
    }
}


/************************************************************************/
/*                              DecToDMS()                              */
/************************************************************************/

const char * DecToDMS( double dfAngle )

{
    static char	szDMS1[128];
    static char	szDMS2[128];
    static int	nRotator = 1;
    int		nDeg, nMin;
    double	dfRemainder;
    

    nDeg = (int) dfAngle;

    dfRemainder = (fabs(dfAngle) - fabs(nDeg));

    nMin = (int) floor(dfRemainder*60.0);
    dfRemainder = dfRemainder - nMin / 60.0;

    if( nRotator == 1 )
    {
        nRotator = 2;
        sprintf( szDMS1, "%4dd%2d'%7.4f\"", nDeg, nMin, dfRemainder*3600.0 );
        return( szDMS1 );
    }
    else
    {
        nRotator = 1;
        sprintf( szDMS2, "%4dd%2d'%7.4f\"", nDeg, nMin, dfRemainder*3600.0 );
        return( szDMS2 );
    }
}

/************************************************************************/
/*                          DumpGlobalRegion()                          */
/************************************************************************/

static int DumpGlobalRegion( ecs_Region * region, PJ * proj_defn )

{
    ecs_Result	*result;
    ecs_Region  tmpRegion;
    projUV     proj_pnt;

    if( region == NULL )
        region = &tmpRegion;

/* -------------------------------------------------------------------- */
/*      print the global bounds.                                        */
/* -------------------------------------------------------------------- */
    result = cln_GetGlobalBound(ClientID);
    if( CheckError( result ) )
        return( FALSE );

    printf( "Bounds\n" );
    printf( "north = %f\n", ECSREGION(result).north );
    printf( "south = %f\n", ECSREGION(result).south );
    printf( "east = %f\n", ECSREGION(result).east );
    printf( "west = %f\n", ECSREGION(result).west );
    printf( "ns_res = %.9f\n", ECSREGION(result).ns_res );
    printf( "ew_res = %.9f\n", ECSREGION(result).ew_res );

    *region = ECSREGION(result);

/* -------------------------------------------------------------------- */
/*	Print the corner coordinates in lat/long.			*/
/* -------------------------------------------------------------------- */
#ifndef _WINDOWS    
    if( proj_defn != NULL )
    {
        printf( "Lat/Long Corners\n" );
        
        proj_pnt.v = region->north;
        proj_pnt.u = region->west;
        proj_pnt = pj_inv(proj_pnt, proj_defn);
        printf( "Upper Left:  (%s,%s) (%g,%g)\n",
                DecToDMS(proj_pnt.u/DEG_TO_RAD),
                DecToDMS(proj_pnt.v/DEG_TO_RAD),
                proj_pnt.u/DEG_TO_RAD, proj_pnt.v/DEG_TO_RAD );
        
        proj_pnt.v = region->north;
        proj_pnt.u = region->east;
        proj_pnt = pj_inv(proj_pnt, proj_defn);
        printf( "Upper Right: (%s,%s) (%g,%g)\n",
                DecToDMS(proj_pnt.u/DEG_TO_RAD),
                DecToDMS(proj_pnt.v/DEG_TO_RAD),
                proj_pnt.u/DEG_TO_RAD, proj_pnt.v/DEG_TO_RAD );
        
        proj_pnt.v = region->south;
        proj_pnt.u = region->west;
        proj_pnt = pj_inv(proj_pnt, proj_defn);
        printf( "Lower Left:  (%s,%s) (%g,%g)\n",
                DecToDMS(proj_pnt.u/DEG_TO_RAD),
                DecToDMS(proj_pnt.v/DEG_TO_RAD),
                proj_pnt.u/DEG_TO_RAD, proj_pnt.v/DEG_TO_RAD );
        
        proj_pnt.v = region->south;
        proj_pnt.u = region->east;
        proj_pnt = pj_inv(proj_pnt, proj_defn);
        printf( "Lower Right: (%s,%s) (%g,%g)\n",
                DecToDMS(proj_pnt.u/DEG_TO_RAD),
                DecToDMS(proj_pnt.v/DEG_TO_RAD),
                proj_pnt.u/DEG_TO_RAD, proj_pnt.v/DEG_TO_RAD );
    }
#endif
    
    return TRUE;
}

/************************************************************************/
/*                          DumpRasterObject()                          */
/************************************************************************/

static void DumpRasterObject( ecs_Result * result, ecs_Family featureType,
                              int nDataType )

{
    int		i, xsize;
    unsigned char *raw = (unsigned char *) ECSRASTER(result);

    xsize = ECSOBJECT(result).geom.ecs_Geometry_u.matrix.x.x_len;

    printf("Object ID:%s (%d pixels)\n",ECSOBJECTID(result), xsize);

    for( i = 0; i < xsize; i += nSampleFrequency )
    {
        if( nSampleFrequency != 1 )
            printf( "%d:", i );

        if( featureType == Matrix )
        {
            printf( "%d ", ECSRASTER(result)[i] );
        }
        else if( featureType == Image )
        {
            if( nDataType == 1 )
            {
                if( raw[i*4+3] != 0 )
                    printf( "%d,%d,%d ", raw[i*4], raw[i*4+1], raw[i*4+2] );
                else
                    printf( "<null> " );
            }
            else if( nDataType == 2 )
                printf( "%d ", raw[i] );
            else if( nDataType == 3 )
                printf( "%d ", ((unsigned short *) ECSRASTER(result))[i] );
            else if( nDataType == 4 )
                printf( "%d ", ((short *) ECSRASTER(result))[i] );
            else if( nDataType == 5 )
                printf( "%d ", ((int *) ECSRASTER(result))[i] );
            else
                printf( "?? " );
        }
    }

    printf( "\n" );
}

/************************************************************************/
/*                          DumpVectorObject()                          */
/************************************************************************/

static void DumpVectorObject( ecs_Result * result, ecs_Family featureType )

{
    printf("Object ID:%s\n",ECSOBJECTID(result));
    printf("Object Attributes:%s\n",ECSOBJECTATTR(result));
        
    if( featureType == Text ) {
        printf( "%d: (%f , %f) Text=%s\n", 0, 
                ECSGEOM(result).text.c.x,
                ECSGEOM(result).text.c.y,
                ECSGEOM(result).text.desc );
    }
    else if( featureType == Point ) {
        printf( "%d: (%f , %f)\n", 0, 
                ECSGEOM(result).point.c.x,
                ECSGEOM(result).point.c.y );
    }
    else if( featureType == Line ) {
        int		i;
            
        for( i=0; 
             i < (int) ECSGEOM(result).line.c.c_len; 
             i += nSampleFrequency ) {
            printf("%d: (%f , %f)\n", i,
                   ECSGEOM(result).line.c.c_val[i].x,
                   ECSGEOM(result).line.c.c_val[i].y );
        }
    }
    else if( featureType == Area ) {
        int		i, iRing;

        for( iRing=0;
             iRing < (int) ECSGEOM(result).area.ring.ring_len;
             iRing++ ) {
            ecs_FeatureRing	*ring;
                
            printf( "Ring %d\n", iRing );

            ring = ECSGEOM(result).area.ring.ring_val + iRing;

            for( i=0; i < (int) ring->c.c_len; i += nSampleFrequency ) {
                printf("%d: (%f , %f)\n", i,
                       ring->c.c_val[i].x,
                       ring->c.c_val[i].y );
            }
            printf( "\n" );
        }
    }
    printf( "\n" );
}

/************************************************************************/
/*                             AccessURL()                              */
/************************************************************************/

static int AccessURL( char * url, ecs_Region * region )

{
    ecs_Result *result;
    PJ	       *proj_defn = NULL;

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
/*      Print the projection.                                           */
/* -------------------------------------------------------------------- */
    result = cln_GetServerProjection( ClientID );
    if( CheckError( result ) )
        return( FALSE );

    printf( "Projection = `%s'\n", ECSTEXT(result) );

#ifndef _WINDOWS    
    if( !bNoProj 
        && strstr(ECSTEXT(result),"latlong") == NULL
        && strstr(ECSTEXT(result),"longlat") == NULL )
        proj_defn = cln_ProjInit( ECSTEXT(result) );
#endif

/* -------------------------------------------------------------------- */
/*      Dump the global region.                                         */
/* -------------------------------------------------------------------- */
    DumpGlobalRegion( region, proj_defn );

/* -------------------------------------------------------------------- */
/*      Print the Dictionary (update).                                  */
/* -------------------------------------------------------------------- */
    result = cln_UpdateDictionary( ClientID, "" );
    if( CheckError( result ) )
        return( FALSE );

    if( !bNoDict )
        printf( "UpdateDictionary = \n%s\n", ECSTEXT(result) );
  
    return( TRUE );
}

/************************************************************************/
/*                              IdSearch()                              */
/************************************************************************/

static void IdSearch( char * layer, ecs_Family featureType,
                      const char * id )

{
    ecs_LayerSelection selectionLayer;
    ecs_Result *result;
    
/* -------------------------------------------------------------------- */
/*      Define the layer to select.                                     */
/* -------------------------------------------------------------------- */
    selectionLayer.Select = (char *) layer;
    selectionLayer.F = featureType;
    
    result = cln_SelectLayer(ClientID,&selectionLayer);
    
    if( CheckError( result ) )
        return;

/* -------------------------------------------------------------------- */
/*      Search for the coordinate.                                      */
/* -------------------------------------------------------------------- */
    result = cln_GetObject( ClientID, (char *) id );
    if( CheckError( result ) )
        return;

    if( featureType == Matrix || featureType == Image )
    {
        DumpRasterObject( result, featureType, 5 );
    }
    else
    {
        DumpVectorObject( result, featureType );
    }
}

/************************************************************************/
/*                            CoordSearch()                             */
/************************************************************************/

static void CoordSearch( char * layer, ecs_Family featureType,
                         double x, double y )

{
    ecs_LayerSelection selectionLayer;
    ecs_Result *result;
    ecs_Coordinate	coord;
    
/* -------------------------------------------------------------------- */
/*      Define the layer to select.                                     */
/* -------------------------------------------------------------------- */
    selectionLayer.Select = (char *) layer;
    selectionLayer.F = featureType;
    
    result = cln_SelectLayer(ClientID,&selectionLayer);
    
    if( CheckError( result ) )
        return;

/* -------------------------------------------------------------------- */
/*      Search for the coordinate.                                      */
/* -------------------------------------------------------------------- */
    coord.x = x;
    coord.y = y;
    result = cln_GetObjectIdFromCoord( ClientID, &coord );
    if( CheckError( result ) )
        return;

    printf( "cln_GetObjectIdFromCoord(%f,%f) = `%s'\n",
            x, y, ECSTEXT(result) );
}

/************************************************************************/
/*                              DumpDict()                              */
/************************************************************************/

static void DumpDict( const char * pszDictName )

{
    ecs_Result *result;

    result = cln_UpdateDictionary( ClientID, (char *) pszDictName );
    if( CheckError( result ) )
        return;

    printf( "UpdateDictionary(%s) = \n%s\n", pszDictName, ECSTEXT(result) );
}

/************************************************************************/
/*                             DumpLayer()                              */
/************************************************************************/

static void DumpLayer( const char * options, ecs_Region * region,
                       char * layer, ecs_Family featureType )

{
    int nObjectCount = 0, i;
    ecs_Result *result;
    ecs_Region selectionRegion;
    ecs_LayerSelection selectionLayer;
    ecs_ObjAttributeFormat *oaf;

    (void) options;

/* -------------------------------------------------------------------- */
/*      Select a region ... this should be overridable from the         */
/*      command line.                                                   */
/* -------------------------------------------------------------------- */
    if( region != NULL ) {
        selectionRegion = *region;
        result = cln_SelectRegion(ClientID,&selectionRegion);
        if( CheckError( result ) )
            return;
    }

/* -------------------------------------------------------------------- */
/*      Define the layer to select.                                     */
/* -------------------------------------------------------------------- */
    selectionLayer.Select = (char *) layer;
    selectionLayer.F = featureType;
    
    result = cln_SelectLayer(ClientID,&selectionLayer);
    
    if( CheckError( result ) )
        return;

    DumpGlobalRegion( NULL, NULL );

/* -------------------------------------------------------------------- */
/*      Dump the attribute definitions.                                 */
/* -------------------------------------------------------------------- */
    if( featureType != Matrix && featureType != Image ) {
        int	nSampleCounter = 0;

        result = cln_GetAttributesFormat( ClientID );
        if( CheckError( result ) )
            return;
        
        oaf = &(ECSRESULT(result).oaf);
        for( i = 0; i < (int) oaf->oa.oa_len; i++ )
        {
            printf( "Field %d: %s ", i, oaf->oa.oa_val[i].name );
            
            if( oaf->oa.oa_val[i].type == Char )
                printf( " (Char)" );
            else if( oaf->oa.oa_val[i].type == Varchar )
                printf( " (Varchar)" );
            else if( oaf->oa.oa_val[i].type == Longvarchar )
                printf( " (Longvarchar)" );
            else if( oaf->oa.oa_val[i].type == Decimal )
                printf( " (Decimal)" );
            else if( oaf->oa.oa_val[i].type == Numeric )
                printf( " (Numeric)" );
            else if( oaf->oa.oa_val[i].type == Smallint )
                printf( " (Smallint)" );
            else if( oaf->oa.oa_val[i].type == Integer )
                printf( " (Integer)" );
            else if( oaf->oa.oa_val[i].type == Real )
                printf( " (Real)" );
            else if( oaf->oa.oa_val[i].type == Float )
                printf( " (Float)" );
            else if( oaf->oa.oa_val[i].type == Double )
                printf( " (Double)" );
            else
                printf( " (unknown)" );
            
            printf( " %d.%d",
                    oaf->oa.oa_val[i].lenght,
                    oaf->oa.oa_val[i].precision );
            
            if( oaf->oa.oa_val[i].nullable )
                printf( " nullable" );
            printf( "\n" );
        }

/* -------------------------------------------------------------------- */
/*      Process all shapes.                                             */
/* -------------------------------------------------------------------- */
        result = cln_GetNextObject(ClientID);
        while (ECSSUCCESS(result)) {

            if( ++nSampleCounter >= nSampleFrequency )
            {
                DumpVectorObject( result, featureType );
                nSampleCounter = 0;
            }
            
            nObjectCount++;
            result = cln_GetNextObject(ClientID);
        }
    }

/* -------------------------------------------------------------------- */
/*      Report on matrix information.                                   */
/* -------------------------------------------------------------------- */
    if( featureType == Matrix || featureType == Image ) {
        int		i;
        int             nSampleCounter = 0;
        int		nDataType = 5;
        
        printf( "RasterInfo:\n" );

        result = cln_GetRasterInfo( ClientID );
        if( CheckError( result ) )
            return;

        printf( "mincat = %ld\n", ECSRASTERINFO(result).mincat );
        printf( "maxcat = %ld\n", ECSRASTERINFO(result).maxcat );
        
        printf( "width = %d", ECSRASTERINFO(result).width );
        if( featureType == Image )
        {
            nDataType = ECSRASTERINFO(result).width;
            if( ECSRASTERINFO(result).width == 1 )
                printf( " (RGB)" );
            else if( ECSRASTERINFO(result).width == 2 )
                printf( " (8U)" );
            else if( ECSRASTERINFO(result).width == 3 )
                printf( " (16U)" );
            else if( ECSRASTERINFO(result).width == 4 )
                printf( " (16S)" );
            else if( ECSRASTERINFO(result).width == 5 )
                printf( " (32S)" );
            else
                printf( " (unknown data type)") ;

        }
        printf( "\nheight = %d\n", ECSRASTERINFO(result).height );
        
        printf( "cat_len = %d\n", ECSRASTERINFO(result).cat.cat_len );

        for( i = 0; i < (int) ECSRASTERINFO(result).cat.cat_len; i++ ) {
            printf( "%d: no=%ld, rgb=%d,%d,%d  %s\n",
                    i,
                    ECSRASTERINFO(result).cat.cat_val[i].no_cat,
                    ECSRASTERINFO(result).cat.cat_val[i].r,
                    ECSRASTERINFO(result).cat.cat_val[i].g,
                    ECSRASTERINFO(result).cat.cat_val[i].b,
                    ECSRASTERINFO(result).cat.cat_val[i].label );
        }
/* -------------------------------------------------------------------- */
/*      Process all lines.                                              */
/* -------------------------------------------------------------------- */
        result = cln_GetNextObject(ClientID);
        while (ECSSUCCESS(result)) {

            if( ++nSampleCounter >= nSampleFrequency )
            {
                DumpRasterObject( result, featureType, nDataType );
                nSampleCounter = 0;
            }

            nObjectCount++;
            result = cln_GetNextObject(ClientID);
        }
    }
  
/* -------------------------------------------------------------------- */
/*      Release the layer, and client and cleanup.                      */
/* -------------------------------------------------------------------- */
/*    result = cln_ReleaseLayer(ClientID,&selectionLayer); */

    printf( "Object Count = %d\n", nObjectCount );
}

/************************************************************************/
/*                               Usage()                                */
/************************************************************************/

static void Usage()

{
    printf(
        "Usage: ogdi_info [-no-dict] [-no-proj] -u url\n"
        "            [-dict arg] [-cap] [-ext name [layer]]\n"
        "            -l layername -f family [-r north south east west]\n" 
        "            [-dl] [-cs easting northing]\n" 
        "            [-id object_id] [-sf sample_frequency]\n" );
    exit( 1 );
}

/************************************************************************/
/*                                main()                                */
/************************************************************************/

int main( int argc, char ** argv )
{
    ecs_Family	featureType = Point;
    char *layer = "label";
    static ecs_Region	reg;
    ecs_Region  *region = NULL;
    ecs_Result *result;
    int		i;

    if( argc == 1 )
        Usage();

/* -------------------------------------------------------------------- */
/*      Handle commandline arguments.                                   */
/* -------------------------------------------------------------------- */
    for( i = 1; i < argc; i++ ) {

        if( strcmp(argv[i],"-dl") == 0 ) {
            DumpLayer( "", region, layer, featureType );
        }
        else if( strcmp(argv[i],"-dr") == 0 ) {
            DumpGlobalRegion( NULL, NULL );
        }
        else if( strcmp(argv[i], "-no-proj") == 0 ) {
            bNoProj = TRUE;
        }
        else if( strcmp(argv[i], "-no-dict") == 0 ) {
            bNoDict = TRUE;
        }
        else if( strcmp(argv[i], "-cap") == 0 ) {
            DumpCapabilities();
        }
        else if( i == argc - 1 ) {
            /* skip ... the rest require arguments.  */
            Usage();
        }
        else if( strcmp(argv[i], "-dict") == 0 ) {
            DumpDict( argv[++i] );
        }
        else if( strcmp(argv[i],"-u") == 0 ) {
            if( ClientID != -1 ) {
                result = cln_DestroyClient(ClientID);
                if( CheckError( result ) )
                    return( FALSE );
                ecs_CleanUp( result );
                ClientID = -1;
            }

            AccessURL( argv[++i], &reg );
            region = &reg;
        }
        else if( strcmp(argv[i], "-l") == 0 ) {
            layer = argv[++i];
        }
        else if( strcmp(argv[i], "-sf") == 0 ) {
            nSampleFrequency = atoi(argv[++i]);
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
            else
                fprintf( stderr, 
                         "-f argument (%s) not recognised (case matters!)\n",
                         argv[i] );
            i++;
        }
        else if( strcmp(argv[i], "-r") == 0 && i < argc - 4 ) {

            region = &reg;
            reg.north = atof(argv[++i]);
            reg.south = atof(argv[++i]);
            reg.east = atof(argv[++i]);
            reg.west = atof(argv[++i]);
        }
        else if( strcmp(argv[i], "-cs") == 0 && i < argc - 2 ) {
            CoordSearch( layer, featureType,
                         atof(argv[i+1]), atof(argv[i+2]) );

            i += 2;
        }
        else if( strcmp(argv[i], "-id") == 0 && i < argc - 1 ) {
            IdSearch( layer, featureType, argv[i+1] );
            i += 1;
        }
        else if( strcmp(argv[i], "-ext") == 0 && i < argc - 1 ) {
            const char	*layer = NULL;

            printf( "Check Extension %s", argv[i+1] );

            if( i < argc - 2 && argv[i+2][0] != '-' )
            {
                layer = argv[i+2];
                printf( " on layer %s", layer );
            }
            
            if( cln_CheckExtension( ClientID, argv[i+1], layer ) )
                printf( ": enabled\n" );
            else
                printf( ": not enabled\n" );

            if( layer != NULL )
                i += 2;
            else
                i += 1;
        }
        else
            Usage();
    }

/* -------------------------------------------------------------------- */
/*      Close old client if there is one active.                        */
/* -------------------------------------------------------------------- */
    if( ClientID != -1 ) {
        result = cln_DestroyClient(ClientID);
        if( CheckError( result ) )
            return( FALSE );
        ecs_CleanUp( result );
        ClientID = -1;
    }

    return 0;
}



