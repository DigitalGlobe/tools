/******************************************************************************
 *
 * Component: OGDI Core C API
 * Purpose: Control to dynamic geographic database driver.
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
 * $Log: server.c,v $
 * Revision 1.12  2016/07/06 08:59:20  erouault
 * fix memory leaks in error code paths of svr_CreateServer()
 *
 * Revision 1.11  2016/06/28 14:32:45  erouault
 * Fix all warnings about unused variables raised by GCC 4.8
 *
 * Revision 1.10  2016/06/27 20:05:12  erouault
 * Grow some buffers in VRF driver (patch by Craig Bruce)
 *
 * Revision 1.9  2007/02/12 21:01:48  cbalint
 *      Fix win32 target. It build and works now. (tested with VC6)
 *
 * Revision 1.8  2007/02/12 16:09:06  cbalint
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
 * Revision 1.7  2001/04/09 15:04:34  warmerda
 * applied new source headers
 *
 */

#include "ecs.h"
#ifdef _WINDOWS
#include <string.h>
#include <io.h>
#include <direct.h>
#include <stdio.h>
#else
#include <stdio.h>
#endif

#include <ogdi_macro.h>

ECS_CVSID("$Id: server.c,v 1.12 2016/07/06 08:59:20 erouault Exp $");

ecs_Result svr_dummy_result;

/* MOD: Bruno Savard, INFOMAR INC., bsavard@infomar.com, 1998/09/21 */
/* Description: Remove unused grass driver specific stuff.          */
/*              Addition of declaration of private functions used   */
/*              in this module to avoid some warnings.              */
static ecs_Result *GetOneNextObjectAttributes(ecs_Server *s,ecs_Result *msg,int *isSelected);
static ecs_Result *GetOneNextObject(ecs_Server *s);
static ecs_Result *GetRasterInfoAttributes(ecs_Server *s,ecs_Result *msg);
static ecs_Result *GetObjectAttributes(ecs_Server *s,ecs_Result *msg);

/*#define TESTOGDIINTERFACE 1*/

#ifdef TESTOGDIINTERFACE
  FILE *testfile = NULL;
#endif

/* Error string list */

char *svr_messages[] = {
  /* 0  */  "",
	    "not able to understand this URL",
	    "not able to open more than one local server GRASS",
	    "not able to open the dynamic library",
	    "not able to open the function dyn_CreateServer in the dynamic library",
  /* 5  */  "not enough memory",
	    "SelectLayer not present in dynamic library",
	    "ReleaseLayer not present in dynamic library",
	    "SelectRegion not present in dynamic library",
	    "GetDictionary not present in dynamic library",
  /* 10 */  "GetAttributesFormat not present in dynamic library",
	    "GetNextObject not present in dynamic library",
	    "GetRasterInfo not present in dynamic library",
	    "GetObject not present in dynamic library",
	    "GetObjectIdFromCoord not present in dynamic library",
  /* 15 */  "UpdateDictionary not present in dynamic library",
	    "GetServerProjection not present in dynamic library",
	    "GetGlobalBound not present in dynamic library",
	    "SetServerLanguage not present in dynamic library",
	    "Can't use GetAttributesFormat, no layer selected",
  /* 20 */  "Can't use GetNextObject, no layer selected",
	    "Can't use GetRasterInfo, no layer selected",
	    "Can't use GetRasterInfo, the layer selected is not a raster",
	    "Can't use GetObject, no layer selected",
	    "Can't use GetObjectIdFromCoord, no layer selected",
  /* 25 */  "This point is outside the current region",
	    "The file specified in OGDILINK don't exist",
	    "One of the attributes specified in the list is not define",
	    "Could not link the attribute driver",
	    "Invalid attribute driver dll",
  /* 30 */  "Invalid SQL request",
	    "SetCompression not present in dynamic library",
	    "OGDILINK is not in the correct format"
};

/*
   --------------------------------------------------------
   Server creation. Will create a new server

   IN
       s:    Pointer to ecs_Server structure (given by caller)
       url:  This string is used to create a new server
       isLocal: Indicate if server is used as a remote server
       or a local server.

   OUT
       ecs_Result: Operation result
     
  MOD: Bruno Savard, INFOMAR INC., bsavard@infomar.com, 1998/09/21
  Description: Remove unused grass driver specific stuff.
               Addition of an error handling message to indicate
               that the remote driver cannot be found instead of
               reporting in that case the s->servertype cannot
               be found.
               
   --------------------------------------------------------
   */

ecs_Result *svr_CreateServer(s,url,isLocal)
     ecs_Server *s;
     char *url;
     int isLocal;
{
  ecs_Result *res, *msg;
  char buffer[128];

#ifdef TESTOGDIINTERFACE
  if (testfile == NULL) {
    testfile = fopen("testinterface.txt","a");
  }
  if (testfile != NULL) {
    fprintf(testfile,"svr_CreateServer %s %d\n",url,isLocal);
    fclose(testfile);
    testfile = NULL;
  }  
#endif

  /* Initialize server arguments */

  s->priv = NULL;
  s->nblayer = 0;
  s->layer = NULL;
  s->layer_tablesize = 0;
  s->currentLayer = -1;
  s->handle = NULL;
  s->projection = NULL;
  s->createserver = NULL;
  s->destroyserver = NULL;
  s->selectlayer = NULL;
  s->releaselayer = NULL;
  s->closelayer = NULL;
  s->selectregion = NULL;
  s->getdictionary = NULL;
  s->getattrformat = NULL;
  s->getnextobject = NULL;
  s->getrasterinfo = NULL;
  s->getobject = NULL;
  s->getobjectid = NULL;
  s->updatedictionary =NULL;
  s->getserverprojection = NULL;
  s->getglobalbound = NULL;
  s->setserverlanguage = NULL;
  s->setserverprojection = NULL;
  s->setrasterconversion = NULL;
  s->isRemote = FALSE;
  s->localClient = isLocal;
  s->AttributeListQty = 0;
  s->AttributeList = NULL;
  s->compression.ctype = isLocal ? ECS_COMPRESS_NONE : ECS_COMPRESS_ZLIB;
  s->compression.cversion = isLocal ? 0 : ECS_ZLIB_VERSION;
  s->compression.clevel = isLocal ? 0 : ECS_ZLIB_LEVEL_DEFAULT;
  s->compression.cblksize = isLocal ? 0 : ECS_ZLIB_BLKSIZE_DEFAULT;
  s->compression.cfullsize = 0;
  s->compression.cachesize = ECS_CACHE_DEFAULT;

  /* initialise raster conversion */

  s->rasterconversion.coef.coef_len = 0;
  s->rasterconversion.coef.coef_val = NULL;

  /* Extract information from url */

  s->url = malloc(strlen(url)+1);
  if (s->url == NULL) {
    res = &svr_dummy_result;
    ecs_SetError(res,1,svr_messages[5]);
    return res;    
  }
  strcpy(s->url,url);

  if (!ecs_SplitURL(url,&(s->hostname),&(s->server_type),&(s->pathname))) {
    res = &svr_dummy_result;
    ecs_SetError(res,1,svr_messages[1]);
    return res;
  }

  /* Open the dynamic library. If the server is remote, do no open 
     the server remote again. */

  if (isLocal == 0) {
    /* is a remote server */
    s->handle = ecs_OpenDynamicLib(s->server_type);
  } else {
    /* is a local server */
    if ((s->hostname) != NULL) {
      s->handle = ecs_OpenDynamicLib("remote");
      if (s->handle == NULL) {
        res = &svr_dummy_result;
        sprintf(buffer,"Could not find the dynamic library \"remote\"");
        ecs_SetError(res,1,buffer);
        return res;
      }
      s->isRemote = TRUE;
    } else {
      s->handle = ecs_OpenDynamicLib(s->server_type);
    }
  }
  /* if the driver is not found, then look for the script driver */
  if (s->handle == NULL) {
    s->handle = ecs_OpenDynamicLib("script");
  }

  /* if the script driver is not found, then return error message */
  if (s->handle == NULL) {
    sprintf(buffer,"Could not find the dynamic library \"%s\"",s->server_type);
    svr_DestroyServer(s);

    res = &svr_dummy_result;
    ecs_SetError(res,1,buffer);

    return res;
  }

  s->createserver = (dynfunc *) ecs_GetDynamicLibFunction(s->handle,"dyn_CreateServer");
  if (s->createserver == NULL) {
    svr_DestroyServer(s);

    res = &svr_dummy_result;
    ecs_SetError(res,1,svr_messages[4]);
    return res;
  }
  s->destroyserver = (dynfunc *) ecs_GetDynamicLibFunction(s->handle,"dyn_DestroyServer");
  s->selectlayer = (dynfunc *) ecs_GetDynamicLibFunction(s->handle,"dyn_SelectLayer");
  s->releaselayer = (dynfunc *) ecs_GetDynamicLibFunction(s->handle,"dyn_ReleaseLayer");
  s->closelayer = (dynfunc *) ecs_GetDynamicLibFunction(s->handle,"dyn_CloseLayer");
  s->selectregion = (dynfunc *) ecs_GetDynamicLibFunction(s->handle,"dyn_SelectRegion");
  s->getdictionary = (dynfunc *) ecs_GetDynamicLibFunction(s->handle,"dyn_GetDictionary");
  s->getattrformat = (dynfunc *) ecs_GetDynamicLibFunction(s->handle,"dyn_GetAttributesFormat");
  s->getnextobject =  (dynfunc *) ecs_GetDynamicLibFunction(s->handle,"dyn_GetNextObject");
  s->getrasterinfo =  (dynfunc *) ecs_GetDynamicLibFunction(s->handle,"dyn_GetRasterInfo");
  s->getobject =  (dynfunc *) ecs_GetDynamicLibFunction(s->handle,"dyn_GetObject");
  s->getobjectid =  (dynfunc *) ecs_GetDynamicLibFunction(s->handle,"dyn_GetObjectIdFromCoord");
  s->updatedictionary =  (dynfunc *) ecs_GetDynamicLibFunction(s->handle,"dyn_UpdateDictionary");
  s->getserverprojection =  (dynfunc *) ecs_GetDynamicLibFunction(s->handle,"dyn_GetServerProjection");
  s->getglobalbound =  (dynfunc *) ecs_GetDynamicLibFunction(s->handle,"dyn_GetGlobalBound");
  s->setserverlanguage =  (dynfunc *) ecs_GetDynamicLibFunction(s->handle,"dyn_SetServerLanguage");
  s->setserverprojection =  (dynfunc *) ecs_GetDynamicLibFunction(s->handle,"dyn_SetServerProjection");
  s->setrasterconversion =  (dynfunc *) ecs_GetDynamicLibFunction(s->handle,"dyn_SetRasterConversion");
  s->setcompression =  (dynfunc *) ecs_GetDynamicLibFunction(s->handle,"dyn_SetCompression");

  /* Initialise ecs_Result in "s" */

  ecs_ResultInit(&(s->result));

  /* Call the dynamic function dyn_CreateServer */

  res = s->createserver(s,url); 
  if (res == NULL) {
    res = &svr_dummy_result;
    sprintf(buffer,"A memory error occured when creating the server for the URL \"%s\"", url);
    ecs_SetError(res,1,buffer);
    return res;
    }
  if (res->error) {
    char* saved_message = res->message;
    res->message = NULL;
    svr_DestroyServer(s);
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,saved_message);
    free(saved_message);

/*    if (s->isRemote || (s->hostname == NULL))
      ecs_CloseDynamicLib(s->handle);
*/

    /*ecs_freeSplitURL(&(s->hostname),&(s->server_type),&(s->pathname));*/
    return msg;
  }

  ecs_GetLateralDBConnectionCtrlFile(s);

  return res;
}

/*
   --------------------------------------------------------
   Server destruction. Will destroy the server and unlink
   the program with the dynamic library

   IN
       s:    Pointer to ecs_Server structure (given by caller)

   OUT
       ecs_Result: Operation result

   MOD: Bruno Savard, INFOMAR INC., bsavard@infomar.com, 1998/09/21
   Description: Remove unused grass driver specific stuff
     
   --------------------------------------------------------
   */

ecs_Result *svr_DestroyServer(s)
     ecs_Server *s;
{
  ecs_Result *msg;
  int i;

  ecs_CleanUp(&(s->result));

#ifdef TESTOGDIINTERFACE
  if (testfile == NULL) {
    testfile = fopen("testinterface.txt","a");
  }
  if (testfile != NULL) {
    fprintf(testfile,"svr_DestroyServer %s\n",s->url);
    fclose(testfile);
    testfile = NULL;
  }
#endif

  if (s->handle != NULL && s->destroyserver != NULL) {
    msg = s->destroyserver(s);
    ecs_CleanUp(msg);
  }

  if (s->url != NULL)
    free (s->url);
  if (s->projection != NULL)
    free(s->projection);
  if (s->hostname != NULL)
    free(s->hostname);
  if (s->server_type != NULL)
    free(s->server_type);
  if (s->pathname != NULL)
    free(s->pathname);

  msg = &svr_dummy_result;
  ecs_SetSuccess(msg);

  s->priv = NULL;
  s->url = NULL;
  s->projection = NULL;
  s->hostname = NULL;
  s->server_type = NULL;
  s->pathname = NULL;
  s->createserver = NULL;
  s->destroyserver = NULL;
  s->selectlayer = NULL;
  s->releaselayer = NULL;
  s->closelayer = NULL;
  s->selectregion = NULL;
  s->getdictionary = NULL;
  s->getattrformat = NULL;
  s->getnextobject = NULL;
  s->getrasterinfo = NULL;
  s->getobject = NULL;
  s->getobjectid = NULL;
  s->updatedictionary = NULL;
  s->getserverprojection = NULL;
  s->getglobalbound = NULL;
  s->setserverlanguage = NULL;
  s->setserverprojection = NULL;
  s->setrasterconversion = NULL;
  s->setcompression = NULL;

  if (s->AttributeList != NULL) {
    for(i=0;i<s->AttributeListQty;i++) {
      free(s->AttributeList[i].url);
      free(s->AttributeList[i].layer);
      free(s->AttributeList[i].DriverType);
      free(s->AttributeList[i].InformationSource);
      free(s->AttributeList[i].UserDescription);
      free(s->AttributeList[i].AutorizationDescription);
      free(s->AttributeList[i].SelectionRequest);
    }
    free(s->AttributeList);
  }
  s->AttributeListQty = 0;
  s->AttributeList = NULL;

  if (s->rasterconversion.coef.coef_val != NULL)
    free(s->rasterconversion.coef.coef_val);
  s->rasterconversion.coef.coef_val = NULL;
  
  if (s->layer != NULL) {
    free(s->layer);
    s->layer = NULL;
  }

  if (s->isRemote || (s->hostname == NULL))
/*    ecs_CloseDynamicLib(s->handle);*/

  s->handle = NULL;

  return msg;
}

/*
 *----------------------------------------------------------------------
 * FUNCTION_INFORMATION
 *
 * NAME
 *    svr_SelectLayer
 *
 * DESCRIPTION
 *    Make the selection of a layer in a driver.
 * END_DESCRIPTION
 *
 * PARAMETERS
 *    INPUT
 *       ecs_Server *s: Server object attributes
 *       ecs_LayerSelection *ls: Layer selection
 * END_PARAMETERS
 *
 * RETURN_VALUE
 *    ecs_Result *: A structure with the result information
 *
 * END_FUNCTION_INFORMATION
 *
 * PSEUDO_CODE
 *
 * 1. Clean up the result structure
 *
 * 2. If the handle is valid to the driver and the function selectlayer
 * exist. 
 * Begin
 *
 *    2.1. Call SetAttributeLinkWithRequest with the LayerSelection
 *
 *    2.2. Call SelectLayer in the geographic driver. If an error occur,
 *    return the error message.
 *
 *    2.3. Found the position of the layer in the layer table currentLayer
 *    variable
 *
 *    2.4. Call SetAttributeQuery with this layer
 *
 * End
 *
 * 3. Else return a error message indicating the function don't exist.
 *
 * 4. return the message returned by SelectLayer
 *
 *----------------------------------------------------------------------
 */
 
ecs_Result *svr_SelectLayer(s,ls)
     ecs_Server *s;
     ecs_LayerSelection *ls;
{
  ecs_Result *msg;
  char *error;
  ecs_Region region = {0,0,0,0,0,0};
  int regionset = FALSE;

  ecs_CleanUp(&(s->result));

#ifdef TESTOGDIINTERFACE
  if (testfile == NULL) {
    testfile = fopen("testinterface.txt","a");
  }
  if (testfile != NULL) {
    fprintf(testfile,"svr_SelectLayer %s %s %d\n",s->url,ls->Select,ls->F);
    fclose(testfile);
    testfile = NULL;
  }
#endif

  if (s->handle != NULL && s->selectlayer != NULL) {
    ecs_SetAttributeLinkWithRequest(s,ls->Select,ls->F);
    ecs_UnstackRequest(s,&(ls->Select));
    msg = s->selectlayer(s,ls);
    
    if (ECSSUCCESS(msg) && s->currentLayer >= 0) {
      if (msg->res.type == GeoRegion) {
	region.north = msg->res.ecs_ResultUnion_u.gr.north;
	region.south = msg->res.ecs_ResultUnion_u.gr.south;
	region.east = msg->res.ecs_ResultUnion_u.gr.east;
	region.west = msg->res.ecs_ResultUnion_u.gr.west;
	region.ns_res = msg->res.ecs_ResultUnion_u.gr.ns_res;
	region.ew_res = msg->res.ecs_ResultUnion_u.gr.ew_res;
	regionset = TRUE;
      }

      if (ecs_SetAttributeQuery(s,&(s->layer[s->currentLayer]),&error)!=0) {
	msg = s->releaselayer(s,ls);
	msg = &svr_dummy_result;
	ecs_SetError(msg,1,error);
      } else {
	msg = &svr_dummy_result;

	if (regionset == TRUE) {
	  ecs_SetGeoRegion(msg,region.north, region.south, 
			   region.east, region.west, region.ns_res, 
			   region.ew_res);
	}
	ecs_SetSuccess(msg);	
      }
    }
  } else {
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,svr_messages[6]);
  }

  return msg;
}

/*
 *----------------------------------------------------------------------
 *
 * FUNCTION_INFORMATION
 *
 * NAME
 *    svr_ReleaseLayer
 *
 * DESCRIPTION
 *    Release the link to a layer
 * END_DESCRIPTION
 *
 * PARAMETERS
 *    INPUT
 *       ecs_Server *s: Server object attributes
 *       ecs_LayerSelection *ls: Layer selection
 * END_PARAMETERS
 *
 * RETURN_VALUE
 *    ecs_Result *: A structure with the result information
 *
 * END_FUNCTION_INFORMATION
 *
 * PSEUDO_CODE
 *
 * 1. Clean up the result structure
 *
 * 2. If the handle is valid to the driver and the function releaselayer
 * exist. 
 * Begin
 *
 *    2.1. Found the position of the layer in the layer table with the 
 *    LayerSelection information.
 *
 *    2.2. Call ReleaseAttributeQuery with this layer
 *
 *    2.3. Call ReleaseLayer in the geographic driver. 
 *
 *    2.2. Call ReleaseAttributeLinkWithRequest with the LayerSelection
 *
 * End
 *
 * 3. Else return a error message indicating the function don't exist.
 *
 * 4. return the message returned by ReleaseLayer
 *
 *----------------------------------------------------------------------
 */

ecs_Result *svr_ReleaseLayer(s,ls)
     ecs_Server *s;
     ecs_LayerSelection *ls;
{
  ecs_Result *msg;
  int i;
  char *error;
  char *temp;
  ecs_Family F;

  ecs_CleanUp(&(s->result));

#ifdef TESTOGDIINTERFACE
  if (testfile == NULL) {
    testfile = fopen("testinterface.txt","a");
  }
  if (testfile != NULL) {
    fprintf(testfile,"svr_ReleaseLayer %s %s %d\n",s->url,ls->Select,ls->F);
    fclose(testfile);
    testfile = NULL;
  }
#endif

  if (s->handle != NULL && s->releaselayer != NULL) {
    for(i=0;i<s->nblayer;i++) {
      if (strcmp(ls->Select,s->layer[i].sel.Select) == 0 &&
	  ls->F == s->layer[i].sel.F) {
	if (ecs_ReleaseAttributeQuery(s,&(s->layer[i]),&error)!=0) {
	  msg = &svr_dummy_result;
	  ecs_SetError(msg,1,error);
	  return msg;
	}
	break;
      }
    }
    temp = malloc(strlen(ls->Select)+1);
    if (temp == NULL) {
      msg = &svr_dummy_result;
      ecs_SetError(msg,1,svr_messages[5]);
      return msg;
    }
    strcpy(temp,ls->Select);
    F = ls->F;
    ecs_UnstackRequest(s,&(ls->Select));
    msg = s->releaselayer(s,ls);
    ecs_RemoveAttributeLinkWithRequest(s,temp,F);
    free(temp);
  } else { 
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,svr_messages[7]);
  }

  return msg;
}

/****************************************************************/

void svr_BroadCloseLayers(s)
     ecs_Server *s;
{
  void *handle;
  dynfunc *func;

  if (!(s->isRemote)) {
    handle = ecs_OpenDynamicLib("ecs");
    if (handle!=NULL) {
      func =  (dynfunc *) ecs_GetDynamicLibFunction(handle,"cln_BroadCloseLayers");
      func();
      ecs_CloseDynamicLib(func);
    }
  } else {
    s->closelayer(s);
  }
}

/****************************************************************/

void svr_CloseLayer(s)
     ecs_Server *s;
{
  if (s->handle != NULL && s->closelayer != NULL) {
    s->closelayer(s);
  } 
}

/****************************************************************/

ecs_Result *svr_SelectRegion(s,gr)
     ecs_Server *s;
     ecs_Region *gr;
{
  ecs_Result *msg;

#ifdef TESTOGDIINTERFACE
  if (testfile == NULL) {
    testfile = fopen("testinterface.txt","a");
  }
  if (testfile != NULL) {
    fprintf(testfile,"svr_SelectRegion %s %f %f %f %f %f %f\n",s->url,gr->north,gr->south,gr->east,gr->west,gr->ns_res, gr->ew_res);
    fclose(testfile);
    testfile = NULL;
  }
#endif

  ecs_CleanUp(&(s->result));

  if (s->handle != NULL && s->selectregion != NULL) {
    msg = s->selectregion(s,gr);
  } else {
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,svr_messages[8]);
  }

  return msg;
}

/****************************************************************/

ecs_Result *svr_GetDictionary(s)
     ecs_Server *s;
{
  ecs_Result *msg;

#ifdef TESTOGDIINTERFACE
  if (testfile == NULL) {
    testfile = fopen("testinterface.txt","a");
  }
  if (testfile != NULL) {
    fprintf(testfile,"svr_GetDictionary %s\n",s->url);
    fclose(testfile);
    testfile = NULL;
  }
#endif

  ecs_CleanUp(&(s->result));

  if (s->handle != NULL && s->getdictionary != NULL) {
    msg = s->getdictionary(s);
  } else {
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,svr_messages[9]);
  }

  return msg;
}

/*
 *----------------------------------------------------------------------
 * 
 * FUNCTION_INFORMATION
 *
 * NAME
 *    svr_GetAttributesFormat
 *
 * DESCRIPTION
 *    Get the attribute format list of all the possibles attributes
 *    of a geographic object.
 * END_DESCRIPTION
 *
 * PARAMETERS
 *    INPUT
 *       ecs_Server *s: Server object attributes
 * END_PARAMETERS
 *
 * RETURN_VALUE
 *    ecs_Result *: A structure with the result information
 *
 * END_FUNCTION_INFORMATION
 *
 * PSEUDO_CODE
 *
 * 1. Clean up the result structure
 *
 * 2. If the handle is valid to the driver and the function GetAttributeFormat
 *  exist. 
 * Begin
 *
 *    2.1. If it's not a remote driver and the currentlayer is selected
 *    Begin
 *
 *       2.1.1. Call GetAttributeFormat in the geographical driver
 *
 *	 2.1.2. If the AttributeDriverHandle is not null
 *	 Begin
 *	    
 *	    2.1.2.1. Call GetColumnInfo to retreave the 
 *	    attribute driver attribute info.
 *	 
 *	    2.1.2.2. If no error append during GetColumnInfo, merge the
 *	    attribute driver attribute info with the elements in 
 *	    the result of GetAttributeFormat
 *
 *	 End
 *
 *    End
 *    
 *    2.2. Else return an error indicating that the layer is not selected.
 *
 * End
 *
 * 3. Else return a error message indicating the function don't exist.
 *
 * 4. return the message returned by GetAttributeFormat
 *----------------------------------------------------------------------
 */

ecs_Result *svr_GetAttributesFormat(s)
     ecs_Server *s;
{
  int columns_qty;
  ecs_ObjAttribute *attr;
  char *error;
  ecs_Result *msg;
  int i;

#ifdef TESTOGDIINTERFACE
  if (testfile == NULL) {
    testfile = fopen("testinterface.txt","a");
  }
  if (testfile != NULL) {
    fprintf(testfile,"svr_GetAttributesFormat %s\n",s->url);
    fclose(testfile);
    testfile = NULL;
  }
#endif

  ecs_CleanUp(&(s->result));

  if (s->handle != NULL && s->getattrformat != NULL) {
    if ((!(s->isRemote)) &&
	(s->currentLayer == -1)) {
      msg = &svr_dummy_result;
      ecs_SetError(msg,1,svr_messages[19]);
    } else {
      msg = s->getattrformat(s);
      if (ECSSUCCESS(msg) && s->currentLayer >= 0 && s->layer[s->currentLayer].AttributeDriverHandle != NULL) {
	if ((s->layer[s->currentLayer].GetColumnsInfoFuncPtr)(s,&(s->layer[s->currentLayer]),&columns_qty,&attr,&error) == 0) {
	  for(i=0;i<columns_qty;i++) {
	    ecs_AddAttributeFormat(msg,attr[i].name,attr[i].type,attr[i].lenght,attr[i].precision,attr[i].nullable);
	  }	  
	  ecs_SetSuccess(msg);
	} else {
	  msg = &svr_dummy_result;
	  ecs_SetError(msg,1,error);	  
	}
      }
    }
  } else {
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,svr_messages[10]);
  }

  return msg;
}

/*
 *----------------------------------------------------------------------
 *
 * FUNCTION_INFORMATION
 *
 * NAME
 *    GetOneNextObjectAttributes
 *
 * DESCRIPTION
 *    When using an attribute driver, use it to get the attributes
 * END_DESCRIPTION
 *
 * PARAMETERS
 *    INPUT
 *       ecs_Server *s: Server object attributes
 *	 ecs_Result *msg: The result up until now.
 *    OUTPUT
 *       int *isSelected: Was this object selected
 *       return ecs_Result
 *
 * END_PARAMETERS
 *
 * RETURN_VALUE
 *    ecs_Result *: A structure with the result information
 *
 * END_FUNCTION_INFORMATION
 *
 * PSEUDO_CODE
 *
 * 1. Set a tables of characters containing the attributes
 *  with ecs_SetBindListForVector.
 * 2. Call SelectAttributes with this table.
 * 3. If there is an error in SelectAttributes, return.
 * 4. Call IsSelected in the attribute driver
 * 5. If the result of IsSelected is TRUE
 * Begin
 *    5.1. Set the selected variable to TRUE
 *    5.2. Call GetAttributes in the attribute driver
 *    5.3. Concatenate this string with the attributes
 *    of the object (don't forget the space between both strings
 * End
 *
 *----------------------------------------------------------------------
 */

static ecs_Result *
GetOneNextObjectAttributes(s, msg, isSelected)
     ecs_Server *s;
     ecs_Result *msg;
     int *isSelected;
{
  int attribute_qty;
  char **attribute_list;
  char *error;
  short objSelected;
  char *temp,*attributes;

  *isSelected = FALSE;
  attribute_qty = s->layer[s->currentLayer].SelectionAttributeListQty;

  /* 
   * Set the table of characters 
   */

  if (ecs_SetBindListForVector(s,&(s->layer[s->currentLayer]),msg,&attribute_list,&error) != 0) {

    /* 
       The bind don't work for this object. It's usually because the
       object is incomplete.
       */

    *isSelected = FALSE;
    return msg;
  }

  /* 
   * Select the attributes 
   */

  if ((s->layer[s->currentLayer].SelectAttributesFuncPtr)(s,&(s->layer[s->currentLayer]),attribute_qty,attribute_list,&error) != 0) {		
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,error);	  
    return msg;
  }

  /*
   * Check if the object, accordingly to the selection, is selected
   */

  if ((s->layer[s->currentLayer].IsSelectedFuncPtr)(s,&(s->layer[s->currentLayer]),&objSelected,&error) != 0) {		
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,error);	  
    return msg;
  }

  if (! objSelected) {
    return msg;
  }

  /* 
   * If the object is selected, get the new attributes of the object 
   */

  *isSelected = TRUE;
  if ((s->layer[s->currentLayer].GetSelectedAttributesFuncPtr)(s,&(s->layer[s->currentLayer]),&attributes,&error) != 0) {
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,error);	  
    return msg;
  }

  temp = malloc(strlen(attributes)+strlen(ECSOBJECTATTR(msg))+2);
  if (temp != NULL) {
    strcpy(temp,ECSOBJECTATTR(msg));
    strcat(temp," ");
    strcat(temp,attributes);
    ecs_SetObjectAttr(msg,temp);
    free(temp);
  }
  return msg;
}

/*
 *----------------------------------------------------------------------
 *
 * FUNCTION_INFORMATION
 *
 * NAME
 *    GetOneNextObject
 *
 * DESCRIPTION
 *    Extract the geographical object from the driver like from a stack
 * END_DESCRIPTION
 *
 * PARAMETERS
 *    INPUT
 *       ecs_Server *s: Server object attributes
 * END_PARAMETERS
 *
 * RETURN_VALUE
 *    ecs_Result *: A structure with the result information
 *
 * END_FUNCTION_INFORMATION
 *
 * PSEUDO_CODE
 *
 * 1. Clean up the result structure and set the "selected" variable to FALSE
 * Begin
 *    1.1. While selected is FALSE
 *    Begin
 *	 1.1.1. Call GetNextObject in the geographical driver
 *	 1.1.2. If the AttributeDriverHandle and there is no error 
 *        in GetNextObject and the family is a vector (Area, Point, 
 *        Text or Line)
 *       Begin
 *	    1.1.2.1. Call GetOneNextObjectAttributes
 *	 End
 *	 1.1.3. Else, Set the selected variable to TRUE
 *    End
 * End
 * 2. Else return an error indicating that the layer is not selected.
 * 3. return the message
 *
 *----------------------------------------------------------------------
 */
static ecs_Result *
GetOneNextObject(s)
     ecs_Server *s;
{
  ecs_Result *msg;
  int isSelected;

  ecs_CleanUp(&(s->result));
  isSelected = FALSE;

  while (!isSelected) {
    msg = s->getnextobject(s);
    if (!ECSSUCCESS(msg)) {
      return msg;
    }
    if (s->currentLayer >= 0 && s->layer[s->currentLayer].AttributeDriverHandle != NULL &&
	(s->layer[s->currentLayer].sel.F == Area || s->layer[s->currentLayer].sel.F == Line || 
	 s->layer[s->currentLayer].sel.F == Point || s->layer[s->currentLayer].sel.F == Text)) {
      msg = GetOneNextObjectAttributes(s, msg, &isSelected);
    } else {
      isSelected = TRUE;
    }
  } 

  if ((!(s->isRemote)) &&
      (msg->res.type == Object) && 
      (msg->res.ecs_ResultUnion_u.dob.xmin == 0.0) &&
      (msg->res.ecs_ResultUnion_u.dob.ymin == 0.0) &&
      (msg->res.ecs_ResultUnion_u.dob.xmax == 0.0) &&
      (msg->res.ecs_ResultUnion_u.dob.ymax == 0.0)) {
    ecs_CalcObjectMBR(s,&msg->res.ecs_ResultUnion_u.dob);
  }

  return msg;
}

/*
 *----------------------------------------------------------------------
 *
 * FUNCTION_INFORMATION
 *
 * NAME
 *    svr_GetNextObject
 *
 * DESCRIPTION
 *    Extract the geographical object from the driver like from a stack
 * END_DESCRIPTION
 *
 * PARAMETERS
 *    INPUT
 *       ecs_Server *s: Server object attributes
 * END_PARAMETERS
 *
 * RETURN_VALUE
 *    ecs_Result *: A structure with the result information
 *
 * END_FUNCTION_INFORMATION
 *
 * PSEUDO_CODE
 *
 * 1. If the handle to the driver is invalid or the function GetNextObject
 *    does not exist, return an error.
 * 2. If it's not a remote driver and the currentlayer isn't selected,
 *    return an error.
 * 3. If no caching is occurring, just return the next object.
 * 4. If caching is occurring, grab an object at a time to fill up
 *    the array of objects.
 *
 *----------------------------------------------------------------------
 */

ecs_Result *svr_GetNextObject(s)
     ecs_Server *s;
{
  ecs_Result *msg;
  ecs_ResultUnion *array, *ru;
  int count;
  int result;
  int num;

#ifdef TESTOGDIINTERFACE
  if (testfile == NULL) {
    testfile = fopen("testinterface.txt","a");
  }
  if (testfile != NULL) {
    fprintf(testfile,"svr_GetNextObject %s\n",s->url);
    fclose(testfile);
    testfile = NULL;
  }
#endif

  if (s->handle == NULL || s->getnextobject == NULL) {
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,svr_messages[11]);
    return msg;
  }

  if (!s->isRemote && (s->currentLayer == -1)) {
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,svr_messages[20]);
    return msg;
  }

  num = s->compression.cachesize;

  /* Do not grab multiple objects where the client is local */
  if (s->localClient || num == 1) {
    return GetOneNextObject(s);
  }

  array = (ecs_ResultUnion *) malloc(sizeof(ecs_ResultUnion) * num);
  if (array == NULL) {
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,svr_messages[5]);
    return msg;
  }

  ru = array;
  count = 0;

  do {
    msg = GetOneNextObject(s);
    if (msg->error != 0) {
      break;
    }
    result = ecs_CopyResultUnionWork(&msg->res, ru);
    if (result == FALSE) {
      /*
       * Let ecs_Cleanup() take care of this
       */
      s->result.res.type = MultiResult;
      s->result.res.ecs_ResultUnion_u.results.results_len = count;
      s->result.res.ecs_ResultUnion_u.results.results_val = array;
      msg = &svr_dummy_result;
      ecs_SetError(msg,1,svr_messages[5]);
      return msg;
    }
    count++;
    ru++;
  } while (count < num && msg->error == 0);

  if (count > 0) {
    s->result.res.ecs_ResultUnion_u.results.results_len = count;
    s->result.res.ecs_ResultUnion_u.results.results_val = array;
    s->result.res.type = MultiResult;
  }

  return msg;
}


/*
 *----------------------------------------------------------------------
 *
 * FUNCTION_INFORMATION
 *
 * NAME
 *    GetRasterInfoAttributes --
 *
 * DESCRIPTION
 *    When using an AttributeDriver, fixup the results.
 * END_DESCRIPTION
 *
 * PARAMETERS
 *    INPUT
 *       ecs_Server *s: Server object attributes
 * END_PARAMETERS
 *
 * RETURN_VALUE
 *    ecs_Result *: A structure with the result information
 *
 * END_FUNCTION_INFORMATION
 *
 * PSEUDO_CODE
 *
 * Begin
 *    1. For each category in the object
 *    Begin
 *	 1.1. Set a tables of characters containing 
 *	 the id or the category number or category descriptor
 *	 to bind describe in the list of selection attribute 
 *	 (SelectionAttributeList in the ecs_Layer structure)
 *	 1.2. Call SelectAttributes with this table.
 *	 1.3. If there is no error in SelectAttributes
 *	 Begin
 *	    1.3.1. Call IsSelected in the attribute driver
 *	    1.3.2. If the result of IsSelected is TRUE
 *	    Begin
 *	       1.3.2.1. Call GetAttributes in the attribute driver
 *	       1.3.2.2. Concatenate this string with the attributes
 *	       of the object (don't forget the space between both strings
 *	    End
 *	    1.3.3. Else
 *	    Begin
 *	       1.3.3.1. Remove the category from the list.
 *	    End
 *	 End
 *    End
 * End
 *
 *----------------------------------------------------------------------
 */
static ecs_Result *
GetRasterInfoAttributes(s, msg)
     ecs_Server *s;
     ecs_Result *msg;
{
  int attribute_qty,i;
  ecs_Category *ptr;
  char **attribute_list;
  char *error;
  char *temp;
  short objSelected;
  char *attributes;

  attribute_qty = s->layer[s->currentLayer].SelectionAttributeListQty;
#if 0 /* XXX: Note from Gordon.  This next line doesn't compile as written */
  ecs_SetRasterInfo(&(svr_dummy_result),msg->res.ecs_ResultUnion_u.ri.width,msg->res.ecs_ResultUnion_u.ri.height);
#endif
  for(i=0; i < (int) ECSRASTERINFONB(msg); i++) {
        
    ptr = &(ECSRASTERINFOCAT(msg,i));

    /* 
     * Set the table of characters 
     */
	    
    if (ecs_SetBindListForMatrix(s,&(s->layer[s->currentLayer]),ptr,&attribute_list,&error) != 0) {
      ecs_SetError(&svr_dummy_result,1,error);
    } else {

      /* 
       * Select the attributes 
       */
	      
      if ((s->layer[s->currentLayer].SelectAttributesFuncPtr)(s,&(s->layer[s->currentLayer]),attribute_qty,attribute_list,&error) != 0) {
	ecs_SetError(&svr_dummy_result,1,error);	  
      } else {

	/*
	 * Check if the object, accordingly to the selection, is
	 * selected
	 */

	if ((s->layer[s->currentLayer].IsSelectedFuncPtr)(s,&(s->layer[s->currentLayer]),&objSelected,&error) != 0) {		
	  ecs_SetError(&svr_dummy_result,1,error);	  
	} else {
	  if (objSelected) {

	    /* 
	     * If the category is selected, get the new attributes
	     * of the category descriptor. Else,
	     * the category is removed from the list.
	     */

	    if ((s->layer[s->currentLayer].GetSelectedAttributesFuncPtr)(s,&(s->layer[s->currentLayer]),&attributes,&error) != 0) {
	      ecs_SetError(&svr_dummy_result,1,error);	  
	    } else {
	      temp = malloc(strlen(attributes)+strlen(ptr->label)+2);
	      if (temp != NULL) {
		strcpy(temp,ptr->label);
		strcat(temp," ");
		strcat(temp,attributes);
		ecs_AddRasterInfoCategory(&(svr_dummy_result),ptr->no_cat,ptr->r,ptr->g,ptr->b,temp,ptr->qty);
		free(temp);
	      }
	    }
	  }
	}
      }      
    }
  }	  
  msg = &svr_dummy_result;
  return msg;
}

/*
 *----------------------------------------------------------------------
 *
 * FUNCTION_INFORMATION
 *
 * NAME
 *    ecs_GetRasterInfo
 *
 * DESCRIPTION
 *    Get the current selected raster information
 * END_DESCRIPTION
 *
 * PARAMETERS
 *    INPUT
 *       ecs_Server *s: Server object attributes
 * END_PARAMETERS
 *
 * RETURN_VALUE
 *    ecs_Result *: A structure with the result information
 *
 * END_FUNCTION_INFORMATION
 *
 * PSEUDO_CODE
 *
 * 1. Clean up the result structure
 * 2. If the handle is valid to the driver and the function GetRasterInfo
 * exist. 
 * Begin
 *    2.1. If it's not a remote driver and the currentlayer is selected
 *    Begin
 *       2.1.1. Call GetRasterInfo in the geographical driver
 *	 2.1.2. If the AttributeDriverHandle is not null
 *	 Begin
 *	    2.1.2.1. Call GetRasterInfoAttributes
 *	 End
 *    End
 *    2.2. Else return an error indicating that the layer is not selected.
 * End
 * 3. Else return a error message indicating the function don't exist.
 * 4. return the message returned by GetAttributeFormat
 *
 *----------------------------------------------------------------------
 */

ecs_Result *svr_GetRasterInfo(s)
     ecs_Server *s;
{
  ecs_Result *msg;

#ifdef TESTOGDIINTERFACE
  if (testfile == NULL) {
    testfile = fopen("testinterface.txt","a");
  }
  if (testfile != NULL) {
    fprintf(testfile,"svr_GetRasterInfo %s\n",s->url);
    fclose(testfile);
    testfile = NULL;
  }
#endif

  ecs_CleanUp(&(s->result));

  if (s->handle != NULL && s->getrasterinfo != NULL) {
    if ((!(s->isRemote)) &&
	(s->currentLayer == -1)) {
      msg = &svr_dummy_result;
      ecs_SetError(msg,1,svr_messages[21]);
    } else {
      if ((s->isRemote) ||
	  (s->layer[s->currentLayer].sel.F == Matrix) ||
	  (s->layer[s->currentLayer].sel.F == Image)) {
	msg = s->getrasterinfo(s);
	if (s->currentLayer >= 0 && ECSSUCCESS(msg) && s->layer[s->currentLayer].AttributeDriverHandle != NULL) {
	  msg = GetRasterInfoAttributes(s, msg);
	}
      } else {
	msg = &svr_dummy_result;
	ecs_SetError(msg,1,svr_messages[22]);
      }
    }
  } else {
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,svr_messages[12]);
  }

  return msg;
}

/*
 *----------------------------------------------------------------------
 *
 * FUNCTION_INFORMATION
 *
 * NAME
 *    GetObjectAttributes
 *
 * DESCRIPTION
 *    When using a attribute driver, get the object attributes
 * END_DESCRIPTION
 *
 * PARAMETERS
 *    INPUT
 *       ecs_Server *s: Server object attributes
 *	 ecs_Result *msg: The result to be used if no matching attributes
 *			  are found
 * END_PARAMETERS
 *
 *  RETURN_VALUE
 *    ecs_Result *: A structure with the result information
 *
 * END_FUNCTION_INFORMATION
 *
 * PSEUDO_CODE
 *
 *  1. Set a tables of characters containing the attributes
 *  to bind describe in the list of selection attribute 
 *  (SelectionAttributeList in the ecs_Layer structure)
 *  2. Call SelectAttributes with this table.
 *  3. If there is an error in SelectAttributes, return
 *  4. Call GetAttributes in the attribute driver
 *  5. Concatenate this string with the attributes
 *  of the object (don't forget the space between both strings
 *
 *----------------------------------------------------------------------
 */

static ecs_Result *
GetObjectAttributes(s, msg)
     ecs_Server *s;
     ecs_Result *msg;
{
  int attribute_qty;
  char **attribute_list;
  char *error;
  short objSelected;
  char *temp,*attributes;

  attribute_qty = s->layer[s->currentLayer].SelectionAttributeListQty;

  /* 
   * Set the table of characters 
   */
	
  if (ecs_SetBindListForVector(s,&(s->layer[s->currentLayer]),msg,&attribute_list,&error) != 0) {
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,error);
    return msg;
  }

  /* 
   * Select the attributes 
   */
	  
  if ((s->layer[s->currentLayer].SelectAttributesFuncPtr)(s,&(s->layer[s->currentLayer]),attribute_qty,attribute_list,&error) != 0) {		
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,error);
    return msg;
  }
	    
  /*
   * Check if the object, accordingly to the selection, is selected
   */

  if ((s->layer[s->currentLayer].IsSelectedFuncPtr)(s,&(s->layer[s->currentLayer]),&objSelected,&error) != 0) {		
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,error);
    return msg;
  }

  if (! objSelected) {
    return msg;
  }

  /* 
   * If the object is selected, get the new attributes of the object 
   */

  if ((s->layer[s->currentLayer].GetSelectedAttributesFuncPtr)(s,&(s->layer[s->currentLayer]),&attributes,&error) != 0) {
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,error);
    return msg;
  }

  temp = malloc(strlen(attributes)+strlen(ECSOBJECTATTR(msg))+2);
  if (temp != NULL) {
    strcpy(temp,ECSOBJECTATTR(msg));
    strcat(temp," ");
    strcat(temp,attributes);
    ecs_SetObjectAttr(msg,temp);
    free(temp);
  }
  return msg;
}

/*
 *----------------------------------------------------------------------
 *
 * FUNCTION_INFORMATION
 *
 * NAME
 *    svr_GetObject
 *
 * DESCRIPTION
 *    Get an geographic object knowing the 
 * END_DESCRIPTION
 *
 * PARAMETERS
 *    INPUT
 *       ecs_Server *s: Server object attributes
 *	 char *Id: The id of the object wanted
 * END_PARAMETERS
 *
 *  RETURN_VALUE
 *    ecs_Result *: A structure with the result information
 *
 * END_FUNCTION_INFORMATION
 *
 * PSEUDO_CODE
 *
 * 1. Clean up the result structure and set the "selected" variable to FALSE
 * 2. If the handle is valid to the driver and the function GetObject
 * exist. 
 * Begin
 *    2.1. If it's not a remote driver and the currentlayer is selected
 *    Begin
 *       2.1.1. Call GetObject in the geographical driver with Id
 *	 2.1.2. If the AttributeDriverHandle and there is no error 
 *	 in GetObject and the family is a vector (Area, Point, Text,Line)
 *	 Begin
 *	    2.1.2.1 Call GetObjectAttributes
 *	 End
 *    End
 *    2.2. Else return an error indicating that the layer is not selected.
 * End
 * 3. Else return a error message indicating the function don't exist.
 * 4. return the message
 *
 *----------------------------------------------------------------------
 */

ecs_Result *svr_GetObject(s,Id)
     ecs_Server *s;
     char *Id;
{
  ecs_Result *msg;
  ecs_Family family;

#ifdef TESTOGDIINTERFACE
  if (testfile == NULL) {
    testfile = fopen("testinterface.txt","a");
  }
  if (testfile != NULL) {
    fprintf(testfile,"svr_GetObject %s %s\n",s->url,Id);
    fclose(testfile);
    testfile = NULL;
  }
#endif

  ecs_CleanUp(&(s->result));

  if (s->handle != NULL && s->getobject != NULL) {
    if ((!(s->isRemote)) &&
	(s->currentLayer == -1)) {
      msg = &svr_dummy_result;
      ecs_SetError(msg,1,svr_messages[23]);
    } else {
      msg = s->getobject(s,Id);
      family = s->layer[s->currentLayer].sel.F;
      if (s->currentLayer >= 0 && ECSSUCCESS(msg) && s->layer[s->currentLayer].AttributeDriverHandle != NULL &&
	  (family == Area || family == Line || family == Point || family == Text)) {
	msg = GetObjectAttributes(s,msg);
      }
    }
  } else {
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,svr_messages[13]);
  }

  if (!(s->isRemote)) {
    if (msg->res.type == Object) {
      ecs_Object *obj = &msg->res.ecs_ResultUnion_u.dob;
      if ((obj->xmin == 0.0) && (obj->ymin == 0.0) &&
	  (obj->xmax == 0.0) && (obj->ymax == 0.0)) {
	ecs_CalcObjectMBR(s,&msg->res.ecs_ResultUnion_u.dob);
      }
    }
  }

  return msg;
}

/****************************************************************/

ecs_Result *svr_GetObjectIdFromCoord(s,coord)
     ecs_Server *s;
     ecs_Coordinate *coord;
{
  ecs_Result *msg;

#ifdef TESTOGDIINTERFACE
  if (testfile == NULL) {
    testfile = fopen("testinterface.txt","a");
  }
  if (testfile != NULL) {
    fprintf(testfile,"svr_GetObjectIdFromCoord %s %f %f\n",s->url,coord->x,coord->y);
    fclose(testfile);
    testfile = NULL;
  }
#endif

  ecs_CleanUp(&(s->result));

  /* Check if the coordinate is inside the current region */

  if ((s->currentRegion.north < coord->y) ||
      (s->currentRegion.south > coord->y) ||
      (s->currentRegion.east < coord->x) ||
      (s->currentRegion.west > coord->x)) {
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,svr_messages[25]);
  } else {
    
    /* Call the getobjectid of the current driver */
    
    if (s->handle != NULL && s->getobjectid != NULL) {
      if ((!(s->isRemote)) &&
	  (s->currentLayer == -1)) {
	msg = &svr_dummy_result;
	ecs_SetError(msg,1,svr_messages[24]);
      } else {
	/* Set the tolerance accordingly to the current region */

	ecs_SetTolerance(&(s->currentRegion));

	msg = s->getobjectid(s,coord);
      }
    } else {
      msg = &svr_dummy_result;
      ecs_SetError(msg,1,svr_messages[14]);
    }
  }
  
  return msg;
}

/****************************************************************/

ecs_Result *svr_UpdateDictionary(s,info)
     ecs_Server *s;
     char *info;
{
  ecs_Result *msg;

#ifdef TESTOGDIINTERFACE
  if (testfile == NULL) {
    testfile = fopen("testinterface.txt","a");
  }
  if (testfile != NULL) {
    fprintf(testfile,"svr_UpdateDictionary %s\n",s->url);
    fclose(testfile);
    testfile = NULL;
  }
#endif

  ecs_CleanUp(&(s->result));

  if (s->handle != NULL && s->updatedictionary != NULL) {
    msg = s->updatedictionary(s,info);
  } else {
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,svr_messages[15]);
  }

  return msg;
}

/****************************************************************/

ecs_Result *svr_GetServerProjection(s)
     ecs_Server *s;
{
  ecs_Result *msg;

#ifdef TESTOGDIINTERFACE
  if (testfile == NULL) {
    testfile = fopen("testinterface.txt","a");
  }
  if (testfile != NULL) {
    fprintf(testfile,"svr_GetServerProjection %s\n",s->url);
    fclose(testfile);
    testfile = NULL;
  }
#endif


  ecs_CleanUp(&(s->result));

  if (s->handle != NULL && s->getserverprojection != NULL) {
    msg = s->getserverprojection(s);
  } else {
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,svr_messages[16]);
  }

  return msg;
}

/****************************************************************/

ecs_Result *svr_GetGlobalBound(s)
     ecs_Server *s;
{
  ecs_Result *msg;

#ifdef TESTOGDIINTERFACE
  if (testfile == NULL) {
    testfile = fopen("testinterface.txt","a");
  }
  if (testfile != NULL) {
    fprintf(testfile,"svr_GetGlobalBound %s\n",s->url);
    fclose(testfile);
    testfile = NULL;
  }
#endif

  ecs_CleanUp(&(s->result));

  if (s->handle != NULL && s->getglobalbound != NULL) {
    msg = s->getglobalbound(s);
  } else {
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,svr_messages[17]);
  }

  return msg;
}

/****************************************************************/

ecs_Result *svr_SetServerLanguage(s,language)
     ecs_Server *s;
     u_int language;
{
  ecs_Result *msg;

  ecs_CleanUp(&(s->result));

  if (s->handle != NULL && s->setserverlanguage != NULL) {
    msg = s->setserverlanguage(s,language);
  } else {
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,svr_messages[18]);
  }

  return msg;
}
     

/****************************************************************/

ecs_Result *svr_SetServerProjection(s,projection)
     ecs_Server *s;
     char *projection;
{
  ecs_Result *msg;

#ifdef TESTOGDIINTERFACE
  if (testfile == NULL) {
    testfile = fopen("testinterface.txt","a");
  }
  if (testfile != NULL) {
    fprintf(testfile,"svr_SetServerProjection %s %s\n",s->url,projection);
    fclose(testfile);
    testfile = NULL;
  }
#endif

  ecs_CleanUp(&(s->result));
  
  if (s->handle != NULL && s->setserverprojection != NULL) {
    msg = s->setserverprojection(s,projection);
  } else {
    msg = &svr_dummy_result;

    if (s->projection != NULL) {
      free(s->projection);
    }
    s->projection = (char *) malloc(strlen(projection)+1);
    if (s->projection == NULL) {
      ecs_SetError(msg,1,svr_messages[5]);    
    } else {
      strcpy(s->projection,projection);
      ecs_SetSuccess(msg);
    }
  }
  return msg;
}
     

/****************************************************************/

ecs_Result *svr_SetRasterConversion(s,rc)
     ecs_Server *s;
     ecs_RasterConversion *rc;
{
  ecs_Result *msg;
  int i;

#ifdef TESTOGDIINTERFACE
  if (testfile == NULL) {
    testfile = fopen("testinterface.txt","a");
  }
  if (testfile != NULL) {
    fprintf(testfile,"svr_SetRasterConversion %s\n",s->url);
    fclose(testfile);
    testfile = NULL;
  }
#endif

  ecs_CleanUp(&(s->result));
  
  if (s->handle != NULL && s->setrasterconversion != NULL) {
    msg = s->setrasterconversion(s,rc);
  } else {
    msg = &svr_dummy_result;

    if (s->rasterconversion.coef.coef_val != NULL)
      free(s->rasterconversion.coef.coef_val);
    s->rasterconversion.coef.coef_val = NULL;
    
    s->rasterconversion.coef.coef_len = rc->coef.coef_len;
    s->rasterconversion.coef.coef_val = (double *) malloc(sizeof(double)*rc->coef.coef_len);
    if (s->rasterconversion.coef.coef_val == NULL) {
      ecs_SetError(msg,1,svr_messages[5]);    
      return msg;
    } else {
      for (i=0;(int) i< (int) rc->coef.coef_len;i++) {
	s->rasterconversion.coef.coef_val[i] = rc->coef.coef_val[i];
      }
    }
    s->rasterconversion.isProjEqual = rc->isProjEqual;
    s->rasterconversion.r_method = rc->r_method;
    s->rasterconversion.t_method = rc->t_method;
  }

  ecs_SetSuccess(msg);
  return msg;
}
     
/*
   --------------------------------------------------------
   ecs_SetLayer: Add a new layer to layer list in ecs_Server.

   IN
       s:    Pointer to ecs_Server structure (given by caller)
       sel:  Layer selection structure

   OUT
       return int: Layer position in table. If an error occur
       during allocation, a negative value is return.
     
   --------------------------------------------------------
   */

int ecs_SetLayer(s,sel)
     ecs_Server *s;
     ecs_LayerSelection *sel;
{
  ecs_Layer *temp;

  if ((s->nblayer + 1) >= s->layer_tablesize) {
    temp = malloc(sizeof(ecs_Layer)*(s->layer_tablesize+OGDILAYERINC));
    if (temp == NULL) {
      ecs_SetError(&(s->result),1,"Not enough memory to allocate layer private data");
      return -1;
    }
    if (s->layer != NULL) {
      memcpy(temp,s->layer,(sizeof(ecs_Layer)*s->layer_tablesize));
      free(s->layer);
    }
    s->layer = temp;
    s->layer_tablesize += OGDILAYERINC;
  }

  s->layer[s->nblayer].sel.Select = (char *) malloc(strlen(sel->Select)+1);
  if (s->layer[s->nblayer].sel.Select == NULL) {
    ecs_SetError(&(s->result),1,"Not enough memory to allocate layer private data");
    return -1;
  }

  strcpy(s->layer[s->nblayer].sel.Select,sel->Select);
  s->layer[s->nblayer].sel.F = sel->F;
  s->layer[s->nblayer].index = 0;
  s->layer[s->nblayer].nbfeature = 0;
  s->layer[s->nblayer].priv = 0;
  s->layer[s->nblayer].AttributeDriverLinkPtr = NULL;
  s->layer[s->nblayer].attribute_priv = NULL;
  s->layer[s->nblayer].InitializeDBLinkFuncPtr = NULL;
  s->layer[s->nblayer].DeinitializeDBLinkFuncPtr = NULL;
  s->layer[s->nblayer].GetColumnsInfoFuncPtr = NULL;
  s->layer[s->nblayer].SelectAttributesFuncPtr = NULL;
  s->layer[s->nblayer].IsSelectedFuncPtr = NULL;
  s->layer[s->nblayer].GetSelectedAttributesFuncPtr = NULL;
  s->layer[s->nblayer].AttributeDriverHandle = NULL;
  s->layer[s->nblayer].SelectionAttributeListQty = 0;
  s->layer[s->nblayer].SelectionAttributeList = NULL;
  s->layer[s->nblayer].AttrRequest = NULL;
  
  (s->nblayer)++;
  return (s->nblayer)-1;
}

/*
   --------------------------------------------------------
   ecs_GetLayer: Found a layer in layer attribute of ecs_Server
   for a certain selection.

   IN
       s:    Pointer to ecs_Server structure (given by caller)
       sel:  Layer selection structure

   OUT
       return int: Layer position in table. If the layer don't exist,
       this value will be negative.
     
   --------------------------------------------------------
   */

int ecs_GetLayer(s,sel)
     ecs_Server *s;
     ecs_LayerSelection *sel;
{
  int i;
  
  for(i=0;i<s->nblayer;i++) {
    if ((strcmp(s->layer[i].sel.Select,sel->Select) == 0) &&
	(s->layer[i].sel.F == sel->F)) {
      return i;
    }
  }

  return -1;
}

/*
   --------------------------------------------------------
   ecs_FreeLayer: Free a certain layer

   IN
       s:    Pointer to ecs_Server structure (given by caller)
       layer: Layer position
     
   --------------------------------------------------------
   */

void ecs_FreeLayer(s,layer)
     ecs_Server *s;
     int layer;
{
  int i;

  /* Check if the current layer is the one to destroy */
  if (s->currentLayer == layer)
    s->currentLayer = -1;

  if (s->layer[layer].sel.Select != NULL)
    free(s->layer[layer].sel.Select);
  if (s->layer[layer].attribute_priv != NULL)
    free(s->layer[layer].attribute_priv);
  s->layer[layer].SelectionAttributeListQty = 0;
  if (s->layer[layer].SelectionAttributeList != NULL)
    free(s->layer[layer].SelectionAttributeList);
  if (s->layer[layer].AttrRequest != NULL)
    free(s->layer[layer].AttrRequest);  

  /* repositionate all the following layers */

  for(i=layer;i<(s->nblayer)-1;i++) {
    s->layer[i].sel.Select = s->layer[i+1].sel.Select;
    s->layer[i].sel.F      = s->layer[i+1].sel.F;
    s->layer[i].index      = s->layer[i+1].index;
    s->layer[i].nbfeature  = s->layer[i+1].nbfeature;
    s->layer[i].priv       = s->layer[i+1].priv;
    s->layer[i].AttributeDriverLinkPtr = s->layer[i+1].AttributeDriverLinkPtr;
    s->layer[i].attribute_priv = s->layer[i+1].attribute_priv;
    s->layer[i].InitializeDBLinkFuncPtr = s->layer[i+1].InitializeDBLinkFuncPtr;
    s->layer[i].DeinitializeDBLinkFuncPtr = s->layer[i+1].DeinitializeDBLinkFuncPtr;
    s->layer[i].GetColumnsInfoFuncPtr = s->layer[i+1].GetColumnsInfoFuncPtr;
    s->layer[i].SelectAttributesFuncPtr = s->layer[i+1].SelectAttributesFuncPtr;
    s->layer[i].IsSelectedFuncPtr = s->layer[i+1].IsSelectedFuncPtr;
    s->layer[i].GetSelectedAttributesFuncPtr = s->layer[i+1].GetSelectedAttributesFuncPtr;
    s->layer[i].AttributeDriverHandle = s->layer[i+1].AttributeDriverHandle;
    s->layer[i].SelectionAttributeListQty = s->layer[i+1].SelectionAttributeListQty;
    s->layer[i].SelectionAttributeList = s->layer[i+1].SelectionAttributeList;
    s->layer[i].AttrRequest = s->layer[i+1].AttrRequest;
  }
  (s->nblayer)--;


  if (s->nblayer < (s->layer_tablesize - OGDILAYERINC)) {
    s->layer_tablesize -= OGDILAYERINC;
    if (s->nblayer <= 0) {
      free(s->layer);
      s->layer = NULL;
    } else {
      ecs_Layer* newlayer = realloc(s->layer,sizeof(ecs_Layer)*s->layer_tablesize);
      if( newlayer != NULL )
      {
        s->layer = newlayer;
      }
    }
  }

  return;
}

/****************************************************************/

ecs_Result *svr_SetCompression(s,compression)
     ecs_Server *s;
     ecs_Compression *compression;
{
  ecs_Result *msg;

#ifdef TESTOGDIINTERFACE
  if (testfile == NULL) {
    testfile = fopen("testinterface.txt","a");
  }
  if (testfile != NULL) {
    fprintf(testfile,"svr_SetCompression %s\n",s->url);
    fclose(testfile);
    testfile = NULL;
  }
#endif

  ecs_CleanUp(&(s->result));

  if (s->handle != NULL && s->setcompression != NULL) {
    msg = s->setcompression(s,compression);
  } else {
    msg = &svr_dummy_result;
    ecs_SetError(msg,1,svr_messages[31]);
  }

  return msg;
}
     

/*
 *----------------------------------------------------------------------
 * ecs_RemoveDir
 *
 *	Empty a directory (delete all *.* files) and remove the directory 
 *
 * Results:
 *	A int (1 = done, 0 = error
 *
 *----------------------------------------------------------------------
 */
int ecs_RemoveDir(path)
     char *path;
{
#ifdef _WINDOWS     		  
  struct _finddata_t c_file;
  long hfile;
  char current_dir[_MAX_PATH];
      
  if (_getcwd(current_dir,_MAX_PATH) == NULL) {
    return 0;
  }
  
  if (_chdir(path)) {
    return 0;
  }
  
  if ((hfile = _findfirst("*.*", &c_file)) == -1L) {
    return 0;
  } else {
    do {
      _unlink(c_file.name);
    }
    while(_findnext(hfile, &c_file) == 0);
    _findclose(hfile);
  }   
  
  
  _chdir(current_dir);
  
  return (int) RemoveDirectory(path);
#else
  char buffer[256];
    
  sprintf(buffer,"rm -r %s",path);
  ogdi_system(buffer);
  return 1;
#endif
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      ecs_GetLateralDBConnection

   DESCRIPTION
      Put the contain of the Attribute Connection file in the Attribute
      List.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server object attributes
   END_PARAMETERS

   RETURN_VALUE
      int : An boolean code. 0 if the operation is a success. a code if the operation failed.

   END_FUNCTION_INFORMATION

   PSEUDO_CODE

   1. Get the value of the environment variable OGDILINK with
   the command getenv. If it's don't exist, return 0, no file
   is found but no error encounter.

   2. Open the file specified in OGDILINK. If the file don't
   open, return an error code indicating the OGDILINK is not
   a valid file (code 26).

   3. While we are not in the end of the file
   Begin

      3.1. Read the nine next line and set with each of them the
      following structure attributes: url, layer, family, DriverType,
      InformationSource, UserDescription, AutorizationDescription,
      and SelectionRequest. The last line don't contain usefull
      information.

      3.2. Call AddAttributeLink

   End

   4. close the file

   ********************************************************************
   */

int ecs_GetLateralDBConnectionCtrlFile(s)
     ecs_Server *s;
{
  char *gl = NULL;
  FILE *attr;
  char *url = NULL;
  char *layer = NULL;
  ecs_Family family = 0;
  char *drivertype = NULL;
  char *informationSource = NULL;
  char *user = NULL;
  char *passwrd = NULL;
  char *request = NULL;
  int count;
  int code = 0;
  char chaine[200];
  int size,i;

  gl = getenv("OGDILINK");
  if (gl == NULL) 
    return 0;

  attr = fopen(gl,"rb");
  if (attr == NULL) {
    return 26;
  }

  count = 0;

  while ( fgets(chaine,200,attr) != NULL ) {
    size = strlen(chaine)+1;
    for(i=0;i<size-1;i++) {
      if (chaine[i] == '\n' || chaine[i] == '\r') {
	chaine[i] = '\0';
	size = i+1;
	break;
      }
    }
    count++;
    switch(count) {
    case 1: 
      url = malloc(size);
      if (url == NULL)
	goto GetLateralDBConnectionCtrlFileError;
      strcpy(url,chaine);
      break;
    case 2:
      layer = malloc(size);
      if (layer == NULL)
	goto GetLateralDBConnectionCtrlFileError;
      strcpy(layer,chaine);
      break;
    case 3:
      family = 0;
      if (strcmp(chaine,"Area") == 0 || strcmp(chaine,"AREA") == 0)
	family = Area;
      else
	if (strcmp(chaine,"Line") == 0 || strcmp(chaine,"LINE") == 0)
	  family = Line;
	else
	  if (strcmp(chaine,"Point") == 0 || strcmp(chaine,"POINT") == 0)
	    family = Point;
	  else
	    if (strcmp(chaine,"Text") == 0 || strcmp(chaine,"TEXT") == 0)
	      family = Text;
	    else
	      if (strcmp(chaine,"Matrix") == 0 || strcmp(chaine,"MATRIX") == 0)
		family = Matrix;
	      else
		if (strcmp(chaine,"Image") == 0 || strcmp(chaine,"IMAGE") == 0)
		  family = Image;
		else {
		  code = 32;
		  goto GetLateralDBConnectionCtrlFileError;
		}
      if (family == 0) {
	goto GetLateralDBConnectionCtrlFileError;
      }
      break;
    case 4:
      drivertype = malloc(size);
      if (drivertype == NULL)
	goto GetLateralDBConnectionCtrlFileError;
      strcpy(drivertype,chaine);
      break;
    case 5:
      informationSource = malloc(size);
      if (informationSource == NULL)
	goto GetLateralDBConnectionCtrlFileError;
      strcpy(informationSource,chaine);
      break;
    case 6:
      user = malloc(size);
      if (user == NULL)
	goto GetLateralDBConnectionCtrlFileError;
      strcpy(user,chaine);
      break;
    case 7:
      passwrd = malloc(size);
      if (passwrd == NULL)
	goto GetLateralDBConnectionCtrlFileError;
      strcpy(passwrd,chaine);
      break;
    case 8:
      request = malloc(size);
      if (request == NULL)
	goto GetLateralDBConnectionCtrlFileError;
      strcpy(request,chaine);

      ecs_AddAttributeLink(s,url,layer,family,drivertype,informationSource,user,passwrd,request);

      if (url != NULL) free(url);
      if (layer != NULL) free(layer);
      if (drivertype != NULL) free(drivertype);
      if (informationSource != NULL) free(informationSource);
      if (user != NULL) free(user);
      if (passwrd != NULL) free(passwrd);
      if (request != NULL) free(request);	

      url = NULL;
      layer = NULL;
      drivertype = NULL;
      informationSource = NULL;
      user = NULL;
      passwrd = NULL;
      request = NULL;
      
      break;
    case 9:
      count = 0;
    }
  }

  fclose(attr);
  
  return 0;

GetLateralDBConnectionCtrlFileError:
  if (url != NULL) free(url);
  if (layer != NULL) free(layer);
  if (drivertype != NULL) free(drivertype);
  if (informationSource != NULL) free(informationSource);
  if (user != NULL) free(user);
  if (passwrd != NULL) free(passwrd);
  if (request != NULL) free(request);
  if (code == 0)
    return 5;
  else return code;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      ecs_SetAttributeLinkWithRequest

   DESCRIPTION
      Check a request and if it contain a attribute link descriptor,
      add it in the attribute link list. It place it in the beginning
      of the list. When the SetAttributeQuery will be call, the search
      in the list will lead to this item an the layer will be link with
      it. If more than one link is define in the request, use the last
      one.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server object attributes
	 char *request: The request to use in creation
	 Family family: The family of the geographic object
   END_PARAMETERS

   RETURN_VALUE
      int : An boolean code. 0 if the operation is a success. a code if the operation failed.

   END_FUNCTION_INFORMATION

   PSEUDO_CODE

   1. Extract from the request the link information (if it there are some)
   with ecs_ExtractRequestInformation. If there is no request information
   about a attribute driver link, return 0.

   2. Call AddAttributeLink with these values

   ********************************************************************
   */

int ecs_SetAttributeLinkWithRequest(s,request,family)
     ecs_Server *s;
     char *request;
     ecs_Family family;
{
  char *ExtractRequest;
  char *DriverType;
  char *InformationSource;
  char *UserDescription;
  char *AutorizationDescription;
  char *SelectionRequest;

  if (ecs_ExtractRequestInformation(request,&ExtractRequest,&DriverType,&InformationSource,
				    &UserDescription,&AutorizationDescription,&SelectionRequest) == 0) {
    ecs_AddAttributeLink(s,s->url,ExtractRequest,family,DriverType,
			 InformationSource,UserDescription,AutorizationDescription,SelectionRequest);    

    free(ExtractRequest);
    free(DriverType);
    free(InformationSource); 
    free(UserDescription);
    free(AutorizationDescription);
    free(SelectionRequest);
  }

  return 0;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      ecs_RemoveAttributeLinkWithRequest

   DESCRIPTION
      Remove the item specified in the request from the AttributeLink list.
      All the link to this attribute link must be remove before. 
      If more than one link is define in the request, use the fist one.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server object attributes
	 char *request: The request to use in creation
	 ecs_Family family: The family of the geographic object
   END_PARAMETERS

   RETURN_VALUE
      int : An boolean code. 0 if the operation is a success. a code if the operation failed.

   END_FUNCTION_INFORMATION

   PSEUDO_CODE

   1. Extract from the request the link information (if it there are some)
   with ecs_ExtractRequestInformation. If there is no request information
   about a attribute driver link, return 0.

   2. Search in the list to found a position with the same information.
   If you found none, return 0.

   3. Destroy this position and shift the list in a way to fill the gap.

   4. Return 0

   ********************************************************************
   */

int ecs_RemoveAttributeLinkWithRequest(s,request,family)
     ecs_Server *s;
     char *request;
     ecs_Family family;
{
  char *ExtractRequest;
  char *DriverType;
  char *InformationSource;
  char *UserDescription;
  char *AutorizationDescription;
  char *SelectionRequest;
  int i,j;

  if (ecs_ExtractRequestInformation(request,&ExtractRequest,&DriverType,&InformationSource,
				    &UserDescription,&AutorizationDescription,&SelectionRequest) == 0) {
    for(i=0;i<s->AttributeListQty;i++) {
      if (strcmp(s->url,s->AttributeList[i].url) == 0 &&
	  strcmp(ExtractRequest,s->AttributeList[i].layer) == 0 &&
	  family == s->AttributeList[i].family &&
	  strcmp(DriverType,s->AttributeList[i].DriverType) == 0 &&
	  strcmp(InformationSource,s->AttributeList[i].InformationSource) == 0 &&
	  strcmp(UserDescription,s->AttributeList[i].UserDescription) == 0 &&
	  strcmp(AutorizationDescription,s->AttributeList[i].AutorizationDescription) == 0 &&
	  strcmp(SelectionRequest,s->AttributeList[i].SelectionRequest) == 0) {

	free(s->AttributeList[i].url);
	free(s->AttributeList[i].layer);
	free(s->AttributeList[i].DriverType);
	free(s->AttributeList[i].InformationSource);
	free(s->AttributeList[i].UserDescription);
	free(s->AttributeList[i].AutorizationDescription);
	free(s->AttributeList[i].SelectionRequest);

	for(j=i;j<s->AttributeListQty-1;j++) {
	  s->AttributeList[j].url = s->AttributeList[j+1].url;
	  s->AttributeList[j].layer = s->AttributeList[j+1].layer;
	  s->AttributeList[j].family = s->AttributeList[j+1].family;
	  s->AttributeList[j].DriverType = s->AttributeList[j+1].DriverType;
	  s->AttributeList[j].InformationSource = s->AttributeList[j+1].InformationSource;
	  s->AttributeList[j].UserDescription = s->AttributeList[j+1].UserDescription;
	  s->AttributeList[j].AutorizationDescription = s->AttributeList[j+1].AutorizationDescription;
	  s->AttributeList[j].SelectionRequest = s->AttributeList[j+1].SelectionRequest;
	}

	(s->AttributeListQty)--;
	
	break;
      }
    }
  }

  free(ExtractRequest);
  free(DriverType);
  free(InformationSource); 
  free(UserDescription);
  free(AutorizationDescription);
  free(SelectionRequest);
  return 0;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      ecs_AddAttributeLink

   DESCRIPTION
      Add in the attribute link list an attribute link.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server object attributes
	 char *url: URL
	 char *layer: Layer
	 Family family: Family
	 char *drivertype: Driver type
	 char *infosource: Information source
	 char *userdesc: User description
	 char *autorization: Autorization description
	 char *request: Selection request

   END_PARAMETERS

   RETURN_VALUE
      int : An boolean code. 0 if the operation is a success. a code if the operation failed.

   END_FUNCTION_INFORMATION

   PSEUDO_CODE

   1. Initialize and add to the last position of attribute list a link
   2. Return 0

   ********************************************************************
   */

int ecs_AddAttributeLink(s,url,layer,family,drivertype,infosource,userdesc,autorization,request)
     ecs_Server *s;
     char *url;
     char *layer;
     ecs_Family family;
     char *drivertype;
     char *infosource;
     char *userdesc;
     char *autorization;
     char *request;
{
  ecs_AttributeLink *ptr;
  
  s->AttributeList = realloc(s->AttributeList,(s->AttributeListQty+1)*sizeof(ecs_AttributeLink));
  if (s->AttributeList == NULL) {
    return 5;
  }

  ptr = &(s->AttributeList[s->AttributeListQty]);

  ptr->url = NULL;
  ptr->layer = NULL;
  ptr->DriverType = NULL;
  ptr->InformationSource = NULL;
  ptr->UserDescription = NULL;
  ptr->AutorizationDescription = NULL;
  ptr->SelectionRequest = NULL;

  if ((ptr->url = malloc(strlen(url)+1)) == NULL) 
    goto AddAttributeLinkError;
  if ((ptr->layer = malloc(strlen(layer)+1)) == NULL)
    goto AddAttributeLinkError;
  if ((ptr->DriverType = malloc(strlen(drivertype)+1)) == NULL) 
    goto AddAttributeLinkError;
  if ((ptr->InformationSource = malloc(strlen(infosource)+1)) == NULL) 
    goto AddAttributeLinkError;  
  if ((ptr->UserDescription = malloc(strlen(userdesc)+1)) == NULL) 
    goto AddAttributeLinkError;  
  if ((ptr->AutorizationDescription = malloc(strlen(autorization)+1)) == NULL) 
    goto AddAttributeLinkError;
  if ((ptr->SelectionRequest = malloc(strlen(request)+1)) == NULL) 
    goto AddAttributeLinkError;

  strcpy(ptr->url,url);
  strcpy(ptr->layer,layer);
  ptr->family = family;
  strcpy(ptr->DriverType,drivertype);
  strcpy(ptr->InformationSource,infosource);
  strcpy(ptr->UserDescription,userdesc);
  strcpy(ptr->AutorizationDescription,autorization);
  strcpy(ptr->SelectionRequest,request);

  (s->AttributeListQty)++;

  return 0;
  
AddAttributeLinkError: 
  if (ptr->url != NULL)
    free(ptr->url);
  if (ptr->layer != NULL)
    free(ptr->layer);
  if (ptr->DriverType != NULL)
    free(ptr->DriverType);
  if (ptr->InformationSource != NULL)
    free(ptr->InformationSource);
  if (ptr->UserDescription != NULL)
    free(ptr->UserDescription);
  if (ptr->AutorizationDescription != NULL)
    free(ptr->AutorizationDescription);
  if (ptr->SelectionRequest != NULL)
    free(ptr->SelectionRequest);
  return 5;
}


/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      ecs_ExtractRequestInformation

   DESCRIPTION
      Extract the attribute link information if this information exist.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         char *request: Request string
      OUTPUT
         char **ExtractRequest: Substring of the request without the request information 
         char **DriverType: Driver type
	 char **InformationSource: Information source
	 char **UserDescription: User description
	 char **AutorizationDescription: Autorization description
	 char **SelectionRequest: Selection Request (usually a SQL request)
   END_PARAMETERS

   RETURN_VALUE
      int : An boolean code. 0 if the operation is a success. a code if the operation failed.

   END_FUNCTION_INFORMATION

   PSEUDO_CODE

   1. Check for the '&' character. If it's it's not in the string,
   then there is no link define. Set all the output to null and return 1.

   2. Replace all the /SPACE/ string in the request by a real space

   3. Positionnate a cursor to the sixth last '&' in request

   4. Extract the string between this '&' and the next one. Put the
   string in DriverType. Set the cursor to the next '&'.

   5. Extract the string between this '&' and the next one. Put the
   string in UserDescription. Set the cursor to the next '&'.
   
   6. Extract the string between this '&' and the next one. Put the
   string in AutorizationDescription. Set the cursor to the next '&'.
   
   7. Extract the string between this '&' and the next one. Put the
   string in SelectionRequest. 
   
   8. Return 0

   ********************************************************************
   */

int ecs_ExtractRequestInformation(request,ExtractRequest,DriverType,InformationSource,UserDescription,AutorizationDescription,SelectionRequest)
     char *request;
     char **ExtractRequest;
     char **DriverType;
     char **InformationSource;
     char **UserDescription;
     char **AutorizationDescription;
     char **SelectionRequest;
{
  int count,i;
  char *temp,*ptr;
  char character[2];
  int candlist[13];

  *ExtractRequest = NULL;
  *DriverType = NULL;
  *InformationSource = NULL;
  *UserDescription = NULL;
  *AutorizationDescription = NULL;
  *SelectionRequest = NULL;

  /* Convert /SPACE/ to ' ' */

  temp = malloc(strlen(request)+4);
  if (temp == NULL) 
    return 5;

  strcpy(temp,"");
  for(i=0,ptr = request;i<(int) strlen(request);i++,ptr++) {
    if(strncmp(ptr,"/SPACE/",7) == 0) {
      strcat(temp," ");
      ptr+=6;
      i+=6;
    } else {
      character[0] = ptr[0];
      character[1] = '\0';
      strcat(temp,character);
    }
  }

  /* Count the quantity of & in request and keep the position of each of it */
  count = 0;
  for(i=0;i<(int) strlen(temp);i++) {
    if (temp[i] == '&') {
      candlist[count] = i;
      count++;
    }
  }

  if ( !(count == 6 || count == 11) ) {
    free(temp);
    return 1;
  }

  /*
    Extract the string between the fifth lastest '&' and the next one. Put the
    string in DriverType. 
    */

  *ExtractRequest = malloc(candlist[count-6]+2);
  *DriverType = malloc(candlist[count-5]-candlist[count-6]+1);
  *InformationSource = malloc(candlist[count-4]-candlist[count-5]+1);
  *UserDescription = malloc(candlist[count-3]-candlist[count-4]+1);
  *AutorizationDescription = malloc(candlist[count-2]-candlist[count-3]+1);
  *SelectionRequest = malloc(candlist[count-1]-candlist[count-2]+1);
  
  if (*ExtractRequest == NULL || *DriverType == NULL ||
      *InformationSource == NULL || *UserDescription == NULL ||
      *AutorizationDescription == NULL || *SelectionRequest == NULL) {
    if (*ExtractRequest != NULL) free(*ExtractRequest);
    if (*DriverType != NULL) free(*DriverType);
    if (*InformationSource != NULL) free(*InformationSource);
    if (*UserDescription != NULL) free(*UserDescription);
    if (*AutorizationDescription != NULL) free(*AutorizationDescription);
    if (*SelectionRequest != NULL) free(*SelectionRequest);
    if (temp != NULL) free(temp);
    return 5;
  }
  
  if (count == 6) {
    strncpy(*ExtractRequest,temp,candlist[count-6]);
    (*ExtractRequest)[candlist[count-6]] = '\0';
    ptr = &(temp[candlist[count-6]+1]);
  } else {
    strncpy(*ExtractRequest,temp,candlist[count-6]+1);
    (*ExtractRequest)[candlist[count-6]+1] = '\0';
    ptr = &(temp[candlist[count-6]+1]);
  }

  strncpy(*DriverType,ptr,candlist[count-5]-candlist[count-6]-1);
  (*DriverType)[candlist[count-5]-candlist[count-6]-1] = '\0';
  ptr = &(temp[candlist[count-5]+1]);

  strncpy(*InformationSource,ptr,candlist[count-4]-candlist[count-5]-1);
  (*InformationSource)[candlist[count-4]-candlist[count-5]-1] = '\0';
  ptr = &(temp[candlist[count-4]+1]);

  strncpy(*UserDescription,ptr,candlist[count-3]-candlist[count-4]-1);
  (*UserDescription)[candlist[count-3]-candlist[count-4]-1] = '\0';
  ptr = &(temp[candlist[count-3]+1]);

  strncpy(*AutorizationDescription,ptr,candlist[count-2]-candlist[count-3]-1);
  (*AutorizationDescription)[candlist[count-2]-candlist[count-3]-1] = '\0';
  ptr = &(temp[candlist[count-2]+1]);

  strncpy(*SelectionRequest,ptr,candlist[count-1]-candlist[count-2]-1);
  (*SelectionRequest)[candlist[count-1]-candlist[count-2]-1] = '\0';

  return 0;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      ecs_SetAttributeQuery

   DESCRIPTION
      Scan the attribute link list to see if a link is possible
      with an attribute driver. If it found one, it link it to the
      layer and initialize the link to the attribute driver.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server object attributes
	 ecs_Layer *l: Layer object attributes
      OUTPUT
	 char **error: Error message (if it's the case)
   END_PARAMETERS

   RETURN_VALUE
      int : An boolean code. 0 if the operation is a success. a code if the operation failed.

   END_FUNCTION_INFORMATION

   PSEUDO_CODE

   1. Search in the AttributeLink list a link for with the url, layer 
   and family of this query. If there is none, return 0. 

   2. Load the dll specified in DriverType and put the handle in AttributeDriverHandle.
   If it's not possible, return the code to indicate an error in the connection of the
   dll.

   3. Create the links to the functions of the attribute driver and put it in the 
   pointers in the layer information. If one of the links is not possible, return the
   code to indicate this is impossible to open a link.

   4. Replace the elements of the SQL request between interrogation marks with
   only one interrogation marks. These elements are attributes to be replaced during
   selection. Check if these attributes exist really with GetAttributesFormat and
   simply set a list of the attributes positions in the layer for later retreaval.
   The positions simply indicate the position in the attributes string of the
   attributes to be sent in SelectAttributes in the attribute driver.

   5. Call InitializeDBLink to initialize the attribute link. If it's not possible,
   return the error code from this command.

   6. Return 0

   MOD: Bruno Savard, INFOMAR INC., bsavard@infomar.com, 1998/09/21
   Description: The result of ecs_GetDynamicLibFunction() has been cast
                to type (attrfunc *).  The original cast to (void *)
                seems incorrect.

   ********************************************************************
   */

int ecs_SetAttributeQuery(s,l,error)
     ecs_Server *s;
     ecs_Layer *l;
     char **error;
{
  int i,j,count,k,pos;
  ecs_AttributeLink *link;
  char **ptr,*request;
  ecs_Result *res = NULL;
  char buffer[100];
  int isAttributes;
  char *temp,*ptr1;
  char character[2];
  int code;

  if (l->AttributeDriverHandle != NULL)
    return 0;

  temp = malloc(strlen(l->sel.Select)+1);
  if (temp == NULL) {
    *error = svr_messages[5];
    return 1;    
  }
  j=0;
  strcpy(temp,"");
  for(i=0,ptr1 = l->sel.Select;i<(int) strlen(l->sel.Select);i++,ptr1++) {
    if(strncmp(ptr1,"/SPACE/",7) == 0) {
      strcat(temp," ");
      ptr1+=6;
      i+=6;
    } else {
      character[0] = ptr1[0];
      character[1] = '\0';
      strcat(temp,character);
    }
  }

  /*
    1. Search in the AttributeLink list a link for with the url, layer 
    and family of this query. If there is none, return 0. 
    */

  link = NULL;
  for(i=(s->AttributeListQty-1);i>=0;i--) {
    if (strcmp(s->url,s->AttributeList[i].url) == 0 &&
	strcmp(temp,s->AttributeList[i].layer) == 0 &&
        l->sel.F == s->AttributeList[i].family) {
      link = &(s->AttributeList[i]);
      break;
    }
  }
  free(temp);

  if (link == NULL) {
    return 0;
  }
  
  l->AttributeDriverLinkPtr = link;

  /*
    2. Load the dll specified in DriverType and put the handle in AttributeDriverHandle.
    If it's not possible, return the code to indicate an error in the connection of the
    dll.
    */

  l->AttributeDriverHandle = ecs_OpenDynamicLib(link->DriverType);
  if (l->AttributeDriverHandle == NULL) {
    *error = svr_messages[28];
    return 1;
  }

  /*
    3. Create the links to the functions of the attribute driver and put it in the 
    pointers in the layer information. If one of the links is not possible, return the
    code to indicate this is impossible to open a link.
    */
 
/**MOD START**/
  l->InitializeDBLinkFuncPtr = (attrfunc *) ecs_GetDynamicLibFunction(l->AttributeDriverHandle,"dyn_InitializeDBLink");
  l->DeinitializeDBLinkFuncPtr = (attrfunc *) ecs_GetDynamicLibFunction(l->AttributeDriverHandle,"dyn_DeinitializeDBLink");
  l->GetColumnsInfoFuncPtr = (attrfunc *) ecs_GetDynamicLibFunction(l->AttributeDriverHandle,"dyn_GetColumnsInfo");
  l->SelectAttributesFuncPtr = (attrfunc *) ecs_GetDynamicLibFunction(l->AttributeDriverHandle,"dyn_SelectAttributes");
  l->IsSelectedFuncPtr = (attrfunc *) ecs_GetDynamicLibFunction(l->AttributeDriverHandle,"dyn_IsSelected");
  l->GetSelectedAttributesFuncPtr = (attrfunc *) ecs_GetDynamicLibFunction(l->AttributeDriverHandle,"dyn_GetSelectedAttributes");
/**MOD END**/

  if (l->InitializeDBLinkFuncPtr == NULL ||
      l->DeinitializeDBLinkFuncPtr == NULL ||      
      l->GetColumnsInfoFuncPtr == NULL ||
      l->SelectAttributesFuncPtr == NULL ||
      l->IsSelectedFuncPtr == NULL ||
      l->GetSelectedAttributesFuncPtr == NULL) {
    ecs_CloseDynamicLib(l->AttributeDriverHandle);    
    *error = svr_messages[28];
    return 1;    
  }

  /*
    4. Replace the elements of the SQL request between interrogation marks with
    only one interrogation marks. These elements are attributes to be replaced during
    selection. Check if these attributes exist really with GetAttributesFormat and
    simply set a list of the attributes positions in the layer for later retreaval.
    The positions simply indicate the position in the attributes string of the
    attributes to be sent in SelectAttributes in the attribute driver.
    */
    
  request = l->AttributeDriverLinkPtr->SelectionRequest;
  count = 0;
  for(i=0;i<(int) strlen(request);i++) {
    if (request[i] == '?')
      count++;
  }
  ptr = malloc((count+1)*sizeof(char *));
  if (ptr == NULL) {
    ecs_CloseDynamicLib(l->AttributeDriverHandle);    
    *error = svr_messages[28];
    return 1;    
  }

  k=0;
  for(i=0;i<(int) strlen(request);i++) {
    if (request[i] == '?') {
      ptr[k] = &(request[i]);
      k++;
    }
  }

  if (count%2 == 1) {
    ecs_CloseDynamicLib(l->AttributeDriverHandle);    
    free(ptr);
    *error = svr_messages[29];
    return 1;    
  }

  l->SelectionAttributeListQty = count/2;
  l->SelectionAttributeList = malloc(sizeof(int)*l->SelectionAttributeListQty);
  if (l->SelectionAttributeList == NULL) {
    free(ptr);
    ecs_CloseDynamicLib(l->AttributeDriverHandle);    
    *error = svr_messages[5];
    return 1;    
  }

  l->AttrRequest = malloc(strlen(request)+2);
  if (l->AttrRequest == NULL) {
    free(ptr);
    ecs_CloseDynamicLib(l->AttributeDriverHandle);    
    *error = svr_messages[5];
    return 1;    
  }

  /* Fill the selection attribute list */

  isAttributes = FALSE;
  if (s->handle != NULL && s->getattrformat != NULL) {
    if (!((!(s->isRemote)) &&
	(s->currentLayer == -1))) {
      res = s->getattrformat(s);
      if (ECSSUCCESS(res)) {
	isAttributes=TRUE;
      }      
    }
  } 

  if (count > 0) {
    pos = (int) (ptr[0]-request);
    strncpy(l->AttrRequest,request,pos);
    l->AttrRequest[pos]='\0';
    
    for(i=0;i<count;i+=2) {
      /* 
	 Extract the element pointed in i 
	 */
      
      strncpy(buffer,ptr[i]+1,ptr[i+1]-ptr[i]-1);
      buffer[ptr[i+1]-ptr[i]-1] = '\0';
      
      if (strncmp(buffer,"OGDI.ID",7) == 0)
	l->SelectionAttributeList[i/2] = OGDIID;
      else if (strncmp(buffer,"OGDI.DESC",9) == 0)
	l->SelectionAttributeList[i/2] = OGDIDESC;
      else if (strncmp(buffer,"OGDI.CAT",8) == 0)
	l->SelectionAttributeList[i/2] = OGDICAT;
      else if (!isAttributes) {
	free(ptr);
	ecs_CloseDynamicLib(l->AttributeDriverHandle);    
	*error = svr_messages[29];
	return 1;
      } else {
	l->SelectionAttributeList[i/2] = -1;
	for(j=0;j<(int) ECSRESULT(res).oaf.oa.oa_len;j++) {
	  if(strcmp(buffer,ECSRESULT(res).oaf.oa.oa_val[j].name) == 0) {
	    l->SelectionAttributeList[i/2] = j;
	    break;
	  }
	}
	if (l->SelectionAttributeList[i/2] == -1) {
	  free(ptr);
	  ecs_CloseDynamicLib(l->AttributeDriverHandle);    
	  *error = svr_messages[27];
	  return 1;
	}
      }
      strcat(l->AttrRequest,"?");

      
      if(i+2<count) {
	strncat(l->AttrRequest,ptr[i+1],ptr[i+2]-ptr[i+1]);
      }
    }
    strcat(l->AttrRequest,ptr[count-1]+1);
    
    /*
      5. Call InitializeDBLink to initialize the attribute link. If it's not possible,
      return the error code from this command.
      */
    
    free(ptr);
  } else {
    strcpy(l->AttrRequest,request);
  }
  
  code = (l->InitializeDBLinkFuncPtr)(s,l,error);
  return code;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      ecs_ReleaseAttributeQuery

   DESCRIPTION
      Release the link to the attribute driver.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server object attributes
	 ecs_Layer *l: Layer object attributes
      OUTPUT
	 char **error: Error message (if it's the case)
   END_PARAMETERS

   RETURN_VALUE
      int : An boolean code. 0 if the operation is a success. a code if the operation failed.

   END_FUNCTION_INFORMATION

   PSEUDO_CODE

   1. Call DeinitializeDBLink to deinitialize the attribute link.

   2. Unload the attribute driver dll.

   3. Set AttributeDriverHandle to NULL.

   ********************************************************************
   */

int ecs_ReleaseAttributeQuery(s,l,error)
     ecs_Server *s;
     ecs_Layer *l;
     char **error;
{
  if (l->AttributeDriverHandle != NULL) {
    (l->DeinitializeDBLinkFuncPtr)(s,l,error);
    ecs_CloseDynamicLib(l->AttributeDriverHandle);
    l->AttributeDriverHandle = NULL;
  }
  return 0;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      ecs_UnstackRequest

   DESCRIPTION
      Remove the attribute link information from the request string
      (if it's the case). If there is more than one of these links,
      remove the last one.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server object attributes
      IN/OUT
	 char **request: Request
   END_PARAMETERS

   RETURN_VALUE
      int : An boolean code. 0 if the operation is a success. a code if the operation failed.

   END_FUNCTION_INFORMATION

   PSEUDO_CODE

   1. Check if there is a '&' in the request. If not, return 0.

   2. Found the position of the sixth last position of & in request
   and put a end of string there.

   3. Return 0

   ********************************************************************
   */

int ecs_UnstackRequest(s,request)
     ecs_Server *s;
     char **request;
{
  int count,i;
  int candlist[13];

  (void) s;

  /* Count the quantity of & in request and keep the position of each of it */
  count = 0;
  for(i=0;i<(int) strlen(*request);i++) {
    if ((*request)[i] == '&') {
      candlist[count] = i;
      count++;
    }
  }

  if ( !(count == 6 || count == 11) ) {
    return 1;
  }

  if (count == 6) {
    (*request)[candlist[count-6]] = '\0';
  } else {
    (*request)[candlist[count-6]+1] = '\0';
  }

  return 0;  
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      ecs_SetBindListForVector

   DESCRIPTION
      With the information retrieved from the geographic object vector,
      initialize a list of bind.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server object attributes
	 ecs_Layer *l: Layer info
	 ecs_Result *msg: The geographic object
      OUTPUT
         char **BindList: The result bind list
	 char **error: Error message (if it's the case)
   END_PARAMETERS

   RETURN_VALUE
      int : An boolean code. 0 if the operation is a success. a code if the operation failed.

   END_FUNCTION_INFORMATION

   PSEUDO_CODE

   1. Allocate the attribute list with the bind information
   2. If the allocation in 1. is a success
   Begin
      2.1. Initialize the attribute list table with nulls
      2.2. Split the list of attributes in the object
      2.3. If the split work correctly
      Begin
         2.3.1. For each attribute i in the attribute list
	 Begin
	    2.3.1.1. If the attribute is suppose to be the object id
	    Begin
	       2.3.1.1.1. Initialize the position i in the attribute list
	       with the id. If an error occur, the code is 5 (not enough
	       memory) and go to 99.
	    End
	    2.3.1.2. Else
	    Begin
	      2.3.1.2.1. Check if the attribute is in a impossible position
	      (less then 0 or greater than the quantity of attributes
	      in the list). If it's the case, the code is 27 (One of the 
	      attributes specified in the list is not define) and go to 99.
	      2.3.1.2.2. Initialize the position i in the attribute list
	      with the attribute index. If an error occur, the code 
	      is 5 (not enough memory) and go to 99.
	    End
	 End
      End
      2.4. Else
      Begin
        The code is 5 and go to 99.
      End
   End
   3. Else
   Begin
     The code is 5 and go to 99.
   End

   99. ErrorCase, Free all the memory and return the error message
   related to a code.

   ********************************************************************
   */

int ecs_SetBindListForVector(s,l,msg,BindList,error)
     ecs_Server *s;
     ecs_Layer *l;
     ecs_Result *msg;
     char ***BindList;
     char **error;
{
  int attribute_qty,i;
  int code;
  char **attribute_list;
  int argc;
  char **argv;

  (void) s;

  attribute_qty = l->SelectionAttributeListQty;

  code = 0;
  attribute_list = (char **) malloc(sizeof(char **)*(attribute_qty+1));
  if (attribute_list != NULL) {
    for(i=0;i>attribute_qty;i++) 
      attribute_list[i] = NULL;
    if (ecs_SplitList(ECSOBJECT(msg).attr,&argc,&argv)) {
      for(i=0;i<attribute_qty;i++) {
	if (l->SelectionAttributeList[i] == OGDIID) {
	  attribute_list[i] = malloc(strlen(ECSOBJECT(msg).Id)+1);
	  if (attribute_list[i] == NULL) {
	    code = 5;
	    goto SetBindListForVectorError;
	  }
	  strcpy(attribute_list[i],ECSOBJECT(msg).Id);
	} else {
	  if (l->SelectionAttributeList[i] > argc ||
	      l->SelectionAttributeList[i] < 0 ||
	      argc <= 0) {
	    code = 27;
	    goto SetBindListForVectorError;
	  }
	  attribute_list[i] = malloc(strlen(argv[l->SelectionAttributeList[i]])+1);
	  if (attribute_list[i] == NULL) {
	    code = 5;
	    goto SetBindListForVectorError;
	  }
	  strcpy(attribute_list[i],argv[l->SelectionAttributeList[i]]);
	}
      }
    } else {
      code = 5;
      goto SetBindListForVectorError;
    }
  } else {
    code = 5;
    goto SetBindListForVectorError;
  }

  *BindList = attribute_list;
  *error = NULL;
  free(argv);
  return 0;

SetBindListForVectorError:
  for(i=0;i>attribute_qty;i++) 
    if (attribute_list[i] != NULL)
      free(attribute_list[i]);
  free(argv);
  free(attribute_list);
  *error = svr_messages[code];
  return 1;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      ecs_SetBindListForMatrix

   DESCRIPTION
      With the information retrieved from a category, contruct
      the bind list information for a matrix.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server object attributes
	 ecs_Layer *l: Layer info
	 ecs_Category *ptr: The geographic object
      OUTPUT
         char **BindList: The result bind list
	 char **error: Error message (if it's the case)
   END_PARAMETERS

   RETURN_VALUE
      int : An boolean code. 0 if the operation is a success. a code if the operation failed.

   END_FUNCTION_INFORMATION

   PSEUDO_CODE

   1. Allocate the attribute list with the bind information
   2. If the allocation in 1. is a success
   Begin
      2.1. Initialize the attribute list table with nulls
      2.2. For each attribute i in the attribute list
      Begin
         2.2.1. If the attribute is suppose to be the category number
	 Begin
	    2.2.1.1. Initialize the position i in the attribute list
	    with the category number. If an error occur, the code is 5 (not enough
	    memory) and go to 99.
	 End
	 2.2.2. Else
	 Begin
  	    2.2.2.1. If the attribute is suppose to be the category description
	    Begin
	       2.2.2.1.1. Initialize the position i in the attribute list
		  with the category description. If an error occur, the code is 5 (not enough
		  memory) and go to 99.
	    End
	    2.2.2.2. Else
	    Begin
	       The code is 27 and go to 99.
	    End
	 End
      End
   End
   3. Else
   Begin
     The code is 5 and go to 99.
   End

   99. ErrorCase, Free all the memory and return the error message
   related to a code.

   ********************************************************************
   */

int ecs_SetBindListForMatrix(s,l,ptr,BindList,error)
     ecs_Server *s;
     ecs_Layer *l;
     ecs_Category *ptr;
     char ***BindList;
     char **error;
{
  int attribute_qty,i;
  int code;
  char temp[100];
  char **attribute_list;

  (void) s;

  attribute_qty = l->SelectionAttributeListQty;

  code = 0;
  attribute_list = (char **) malloc(sizeof(char **)*(attribute_qty+1));
  if (attribute_list != NULL) {
    for(i=0;i>attribute_qty;i++) 
      attribute_list[i] = NULL;
    for(i=0;i<attribute_qty;i++) {
      switch(l->SelectionAttributeList[i]) {
      case OGDICAT:
	sprintf(temp,"%ld",ptr->no_cat);
	attribute_list[i] = malloc(strlen(temp)+1);
	if (attribute_list[i] == NULL) {
	  code = 5;
	  goto SetBindListForMatrixError;
	}
	strcpy(attribute_list[i],temp);
	break;
      case OGDIDESC:
	attribute_list[i] = malloc(strlen(ptr->label)+1);
	if (attribute_list[i] == NULL) {
	  code = 5;
	  goto SetBindListForMatrixError;
	}
	strcpy(attribute_list[i],ptr->label);
	break;
      default:
	code = 27;
	goto SetBindListForMatrixError;	
      }
    }
  } else {
    code = 5;
    goto SetBindListForMatrixError;
  }

  *BindList = attribute_list;
  *error = NULL;
  return 0;

SetBindListForMatrixError:
  for(i=0;i>attribute_qty;i++) 
    if (attribute_list[i] != NULL)
      free(attribute_list[i]);
  free(attribute_list);
  *error = svr_messages[5];
  return code;
}
