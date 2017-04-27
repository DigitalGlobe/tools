/******************************************************************************
 *
 * Component: OGDI gltpd Server
 * Purpose: Server function wrappers.
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
 * $Log: ecs_sif.c,v $
 * Revision 1.3  2001/04/09 15:04:35  warmerda
 * applied new source headers
 *
 */

#include "ecs.h"

ECS_CVSID("$Id: ecs_sif.c,v 1.3 2001/04/09 15:04:35 warmerda Exp $");

ecs_Server *svr_handle;

ecs_Result ecs_dummy_sif;

/* In this real gltpd server, this is a stub function */
ecs_Result *
createproxyserver_1_svc(args,rqstp)
     ecs_ProxyCreateServer *args;
     struct svc_req *rqstp;
{
    (void) args;
    (void) rqstp;

    return NULL;
}     

ecs_Result *
createserver_1_svc(args,rqstp)
     char **args;
     struct svc_req *rqstp;
{
    (void) rqstp;

    svr_handle = (ecs_Server *) malloc(sizeof(ecs_Server));
    if (svr_handle == NULL) {
        ecs_ResultInit(&ecs_dummy_sif);
        ecs_SetError(&ecs_dummy_sif,1,"no more memory");
        return &ecs_dummy_sif;
    }
    return svr_CreateServer (svr_handle, *args, 0);
}

ecs_Result *
destroyserver_1_svc(args,rqstp)
     void *args;
     struct svc_req *rqstp;
{
    ecs_Result *msg;

    (void) args;
    (void) rqstp;

    msg = svr_DestroyServer(svr_handle);
    return msg;
}

ecs_Result *selectlayer_1_svc(args,rqstp)
     ecs_LayerSelection *args;
     struct svc_req *rqstp;
{
    (void) rqstp;

    return svr_SelectLayer(svr_handle, args);  
}

ecs_Result *releaselayer_1_svc(args,rqstp)
     ecs_LayerSelection *args;
     struct svc_req *rqstp;
{
    (void) rqstp;

    return svr_ReleaseLayer(svr_handle, args);  
}

ecs_Result *selectregion_1_svc(args,rqstp)
     ecs_Region *args;
     struct svc_req *rqstp;
{
    (void) rqstp;

    return svr_SelectRegion(svr_handle, args);
}

ecs_Result *getdictionnary_1_svc(args,rqstp)
     void *args;
     struct svc_req *rqstp;
{
    (void) rqstp;
    (void) args;

    return svr_GetDictionary(svr_handle);
}

ecs_Result *getattributeformat_1_svc(args,rqstp)
     void *args;
     struct svc_req *rqstp;
{
    (void) rqstp;
    (void) args;

    return svr_GetAttributesFormat(svr_handle);
}

ecs_Result *getnextobject_1_svc(args,rqstp)
     void *args;
     struct svc_req *rqstp;
{
    (void) rqstp;
    (void) args;

    return svr_GetNextObject(svr_handle);
}

ecs_Result *getrasterinfo_1_svc(args,rqstp)
     void *args;
     struct svc_req *rqstp;
{
    (void) rqstp;
    (void) args;

    return svr_GetRasterInfo(svr_handle);
}

ecs_Result *getobject_1_svc(args,rqstp)
     char **args;
     struct svc_req *rqstp;
{
    (void) rqstp;

    return svr_GetObject(svr_handle, *args);
}

ecs_Result *getobjectidfromcoord_1_svc(args,rqstp)
     ecs_Coordinate *args;
     struct svc_req *rqstp;
{
    (void) rqstp;

    return svr_GetObjectIdFromCoord(svr_handle, args);
}

ecs_Result *updatedictionary_1_svc(args,rqstp)
     char **args;
     struct svc_req *rqstp;
{
    (void) rqstp;

    return svr_UpdateDictionary(svr_handle,*args);
}

ecs_Result *getserverprojection_1_svc(args,rqstp)
     void *args;
     struct svc_req *rqstp;
{
    (void) rqstp;
    (void) args;

    return svr_GetServerProjection(svr_handle);  
}

ecs_Result *getglobalbound_1_svc(args,rqstp)
     void *args;
     struct svc_req *rqstp;
{
    (void) rqstp;
    (void) args;

    return svr_GetGlobalBound(svr_handle);
}

ecs_Result *setserverlanguage_1_svc(args,rqstp)
     u_int *args;
     struct svc_req *rqstp;
{
    (void) rqstp;

    return svr_SetServerLanguage(svr_handle, *args);
}

ecs_Result * setserverprojection_1_svc(args,rqstp)
     char **args;
     struct svc_req *rqstp;
{
    (void) rqstp;

    return svr_SetServerProjection(svr_handle, *args);
}

ecs_Result * setrasterconversion_1_svc(args,rqstp)
     ecs_RasterConversion *args;
     struct svc_req *rqstp;
{
    (void) rqstp;

    return svr_SetRasterConversion(svr_handle,args);
}

ecs_Result *setcompression_1_svc(compression,rqstp)
     ecs_Compression *compression;
     struct svc_req *rqstp;
{
    (void) rqstp;

    if ((compression->ctype != ECS_COMPRESS_NONE) &&
        (compression->ctype != ECS_COMPRESS_ZLIB)) {
        ecs_SetError(&ecs_dummy_sif,1,"Compression format not supported");
        return &ecs_dummy_sif;
    }
    if (compression->cversion != 0) {
        ecs_SetError(&ecs_dummy_sif,1,"Compression version not supported");
        return &ecs_dummy_sif;
    }
    svr_handle->compression.ctype = compression->ctype;
    svr_handle->compression.cversion = compression->cversion;

    if (compression->cachesize == 0) {
        svr_handle->compression.cachesize = ECS_CACHE_DEFAULT;
    } else if (compression->cachesize > ECS_CACHE_MAX) {
        svr_handle->compression.cachesize = ECS_CACHE_MAX;
    } else {
        svr_handle->compression.cachesize = compression->cachesize;
    }

    if (compression->ctype == ECS_COMPRESS_ZLIB) {
        if (compression->clevel == 0) {
            svr_handle->compression.clevel = ECS_ZLIB_LEVEL_DEFAULT;
        } else if (compression->clevel > 9) {
            svr_handle->compression.clevel = 9;
        } else {
            svr_handle->compression.clevel = compression->clevel;
        }
        if (compression->cblksize > ECS_ZLIB_BLKSIZE_MAX) {
            svr_handle->compression.cblksize = ECS_ZLIB_BLKSIZE_MAX;
        } else if (compression->cblksize == 0) {
            svr_handle->compression.cblksize = ECS_ZLIB_BLKSIZE_DEFAULT;
        } else {
            svr_handle->compression.cblksize = compression->cblksize;
        }
    } else {
        svr_handle->compression.clevel = 0;
        svr_handle->compression.cblksize = 0;
    }
    svr_handle->compression.cfullsize = 0;

    ecs_SetSuccess(&ecs_dummy_sif);
    return &ecs_dummy_sif;
}
