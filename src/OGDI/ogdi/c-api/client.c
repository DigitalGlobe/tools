/******************************************************************************
 *
 * Component: OGDI Core C API
 * Purpose: Control dispatch of locals client. Also control cache management 
 *          and projection changes.
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
 * $Log: client.c,v $
 * Revision 1.17  2016/06/28 14:32:45  erouault
 * Fix all warnings about unused variables raised by GCC 4.8
 *
 * Revision 1.16  2008/05/28 01:34:30  cbalint
 *    * Convert this file to UTF-8
 *
 * Revision 1.15  2007/02/19 19:31:33  cbalint
 *    Reimplement Matrix algebra under an opensource compatible license.
 *  Modified Files:
 *  	LICENSE ogdi/c-api/client.c ogdi/c-api/makefile
 *  Added Files:
 *  	ogdi/c-api/matrix.c ogdi/c-api/matrix.h
 *
 * Revision 1.14  2007/02/12 15:52:57  cbalint
 *
 *    Preliminary cleanup.
 *    Get rif of unitialized variables, and unused ones.
 *
 * Revision 1.13  2004/02/19 06:20:09  warmerda
 * ecs_CleanUp() cln_dummy_result in ecs_DestroyServer()
 *
 * Revision 1.12  2003/08/27 05:26:36  warmerda
 * Call ecs_SplitURL(NULL) in cln_DestroyClient() to free static resources.
 * This makes use of memory checkers easier even though this wasn't a *real*
 * memory leak.
 *
 * Revision 1.11  2001/10/01 19:51:13  warmerda
 * fixed bug in cln_CalcCtlPoints() with 1 pixel regions
 *
 * Revision 1.10  2001/08/16 15:34:23  warmerda
 * fixed roundoff bug with width/height in cln_ConvRegion
 *
 * Revision 1.9  2001/05/04 17:59:37  warmerda
 * clear autoCache in cln_SelectRegion
 *
 * Revision 1.8  2001/04/12 18:14:16  warmerda
 * added/finished capabilities support
 *
 * Revision 1.7  2001/04/12 05:31:23  warmerda
 * added init/free support for capabilities fields in ecs_Client
 *
 * Revision 1.6  2001/04/09 15:04:34  warmerda
 * applied new source headers
 *
 */

#include "ecs.h"
#include "matrix.h"
#include <assert.h>

ECS_CVSID("$Id: client.c,v 1.17 2016/06/28 14:32:45 erouault Exp $");

/* 
   Definitions specific to c_interface 
   */

static int multiblock = 0;

ecs_Result cln_dummy_result;

ecs_Client *soc[MAXCLIENT] = {
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};


char *cln_messages[] = {
  "success",
  "no more memory",
  "client not found",
  "unable to set the client projection",
  "unable to set the server projection",
  "maximum number of clients reached",
  "cache not found",
  "ecs_Result do not contain an object",
  "unable to change the projection of this point",
  "unable to split this projection",
  "unable to convert the actual region",
  "unable to convert the global region",
  "Invalid layer type for a cache",
  "Invalid region returned by the server",
  "unable to execute the command, the OGDI is blocked"
};

char cln_empty_string[1] = "";

/*
 *----------------------------------------------------------------------
 *
 * cln_FreeClient: Free the client structure
 *
 * IN
 *     ecs_Client *cln:  Client to be freed
 *   
 * MOD: Bruno Savard, INFOMAR INC., bsavard@infomar.com, 1998/09/21
 * Description: Free the cahche using the cln_FreeCache() function
 *              instead of freeing only the cache autocache pointer
 *              that causes memory leaks.
 *              Addition of NULL pointer assignment after the free.
 *   
 *----------------------------------------------------------------------
 */

void cln_FreeClient(cln)
     ecs_Client **cln;
{
  if ((*cln) != NULL) {
    if ((*cln)->url != NULL) {
      free((*cln)->url);
      (*cln)->url = NULL;
    }
    if ((*cln)->cache != NULL) {
      cln_FreeCache((*cln)->cache);
      (*cln)->cache = NULL;
    }
    if ((*cln)->autoCache != NULL)
    {
      cln_FreeCache((*cln)->autoCache);
      (*cln)->autoCache = NULL;
    }
    if ((*cln)->tclprocname != NULL)
    {
      free((*cln)->tclprocname);
      (*cln)->tclprocname = NULL;
    }
    if ((*cln)->mask != NULL) {
       if ((*cln)->mask->c.c_val != NULL) {
         free((*cln)->mask->c.c_val);
         (*cln)->mask->c.c_val = NULL;
       }
      free((*cln)->mask);
      (*cln)->mask = NULL;
    }
    if ((*cln)->target != NULL)
    {
      pj_free((*cln)->target);
      (*cln)->target = NULL;
    }
    if ((*cln)->target_proj != NULL)
    {
      free((*cln)->target_proj);
      (*cln)->target_proj = NULL;
    }
    if ((*cln)->source != NULL)
    {
      pj_free((*cln)->source);
      (*cln)->source = NULL;
    }
    if ((*cln)->dthandle != NULL && ((*cln)->nad_close != NULL)) {
      ((*cln)->nad_close)((*cln)->privdatuminfo);
      ecs_CloseDynamicLib((*cln)->dthandle);
      (*cln)->privdatuminfo = NULL;
      (*cln)->nad_close = NULL;
      (*cln)->dthandle = NULL;
    }      

    if( (*cln)->global_extensions != NULL )
    {
        int	i;

        for( i = 0; (*cln)->global_extensions[i] != NULL; i++ )
            free( (*cln)->global_extensions[i] );
        free( (*cln)->global_extensions );
        (*cln)->global_extensions = NULL;
    }

    if( (*cln)->layer_cap_count > 0 )
    {
        int	lindex;

        for( lindex = 0; lindex < (*cln)->layer_cap_count; lindex++ )
        {
            ecs_LayerCapabilities *layer;
            int			  i;

            layer = (*cln)->layer_cap[lindex];

            if( layer->name != NULL )
                free( layer->name );
            if( layer->title != NULL )
                free( layer->title );
            if( layer->srs != NULL )
                free( layer->srs );

            for( i = 0; 
                 layer->parents != NULL && layer->parents[i] != NULL; 
                 i++ )
            {
                free( layer->parents[i] );
            }
            if( layer->parents != NULL )
                free( layer->parents );

            for( i = 0; 
                 layer->extensions != NULL && layer->extensions[i] != NULL; 
                 i++ )
            {
                free( layer->extensions[i] );
            }
            if( layer->extensions != NULL )
                free( layer->extensions );

            if( layer->qe_prefix != NULL )
                free( layer->qe_prefix );
            if( layer->qe_suffix != NULL )
                free( layer->qe_suffix );
            if( layer->qe_format != NULL )
                free( layer->qe_format );
            if( layer->qe_description != NULL )
                free( layer->qe_description );

            free( layer );
        }

        free( (*cln)->layer_cap );

        (*cln)->layer_cap = NULL;
        (*cln)->layer_cap_count = 0;
    }

    free((*cln));
    (*cln) = NULL;
  }
}

/*
 *----------------------------------------------------------------------
 *
 * cln_AllocClient: Add a client in the table
 *
 * IN 
 *	URL: Url of the client
 * OUT
 *	error_code: pointer to an error code. This code is the position
 *	in the cln_messages.
 *
 * Results:
 *	int: ClientID to the new client.
 *	If negative, an error occur in the operation.
 *
 *----------------------------------------------------------------------
 */

int cln_AllocClient(URL, error_code)
     char *URL;
     int *error_code;
{
  ecs_Client *cln;
  int i,ClientID;

  *error_code = 0;

  /*
   * Found the new client position.
   */

  ClientID = -1;
  for(i=0;i<MAXCLIENT;i++) {
    if (soc[i] == NULL) {	
      ClientID = i;
      break;
    }
  }	
  if (ClientID == -1) {
    /* no avalable client space, return a error message */
    *error_code = 5;
    return ClientID;
  }

  /* 
   * Client allocation 
   */

  cln = (ecs_Client *) calloc(1,sizeof(ecs_Client));
  if (cln != NULL) {
    cln->url = (char *) malloc(strlen(URL)+1);
    if (cln->url != NULL)
      strcpy(cln->url,URL);
    cln->cache = NULL;
    cln->autoCache = NULL;
    cln->selectCache = NULL;
    cln->tclprocname = NULL;
    cln->target = NULL;
    cln->source = NULL;
    cln->target_proj = NULL;
    cln->isSourceLL = FALSE;
    cln->isTargetLL = FALSE;
    cln->isProjEqual = FALSE;
    cln->currentSelectionFamily = 0;
    cln->isCurrentRegionSet = FALSE;
    cln->targetdatum = nodatum;
    cln->sourcedatum = nodatum;
    strcpy(cln->datumtable,"");
    cln->dthandle = NULL;
    cln->nad_init = NULL;
    cln->nad_forward = NULL;
    cln->nad_reverse = NULL;
    cln->nad_close = NULL;
    cln->mask = NULL;
    strcpy( cln->server_version_str, "3.0" );
    cln->server_version = 3000;
  }

  if ((cln == NULL) || (cln->url == NULL)) {
    cln_FreeClient(&(cln));
    /* no memory space left, return a error message */
    *error_code = 1;
    return -1;
  }
  soc[ClientID] = cln;

  return ClientID;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_BroadCloseLayers: For each URL, call the svr_CloseLayer command.
 *
 *----------------------------------------------------------------------
 */

void cln_BroadCloseLayers()
{
  int i;
  for(i=0;i<MAXCLIENT;i++) {
    if (soc[i] != NULL) {	
      svr_CloseLayer(&(soc[i]->s));
    }
  }	
}

/*
 *----------------------------------------------------------------------
 * Client creation. Will register this new client into
 * a global structure (soc).
 *
 * IN
 *	URL:  This string is used to create a new server
 *
 * OUT
 *	ecs_Result: Operation result
 *	ReturnedID: ClientID of the new client
 *
 *----------------------------------------------------------------------
 */

ecs_Result *cln_CreateClient(ReturnedID,URL)
     int *ReturnedID;
     char *URL;
{
  ecs_Client *cln;
  int i;
  ecs_Result *res;
  int error_code;
  char *projection;
  
  if (multiblock != 0) {
    res = &cln_dummy_result;
    ecs_SetError(res,1,cln_messages[14]);
    return res;
  }
  
  /* 
   * Check actual clients is there is a client with the same URL.
   * If it's the case, return this ClientID. 
   */

  if ((i = cln_GetClientIdFromURL(URL)) >= 0) {
    *ReturnedID = i;

    /* 
       Because the CreateClient could be call more than one time for a client,
       it is possible the projection change. Specially for the unknown projections.
       */

    cln = soc[*ReturnedID];
    res = svr_GetServerProjection(&(cln->s));
    if ((res->error == 0) && (res->res.type == AText)) {
      projection = (char *) malloc(strlen(res->res.ecs_ResultUnion_u.s)+1);
      if (projection != NULL) {
	strcpy(projection,res->res.ecs_ResultUnion_u.s);
	res = cln_SetClientProjection(*ReturnedID,projection);
	free(projection);
      } else {
	res = &cln_dummy_result;
	ecs_SetError(res,1,cln_messages[1]);
	cln_FreeClient(&cln);
	soc[*ReturnedID] = NULL;
	*ReturnedID = -1;
      }
    } else {
      return res;
    }

    ecs_SetSuccess(&cln_dummy_result);
    return &cln_dummy_result;
  } 

  if (((*ReturnedID) = cln_AllocClient(URL,&error_code)) < 0) {
    res = &cln_dummy_result;
    ecs_SetError(res,1,cln_messages[error_code]);
  } else {    
    cln = soc[*ReturnedID];
    
    /*
     * Call the server creation. If an error occur, destroy
     * the client.
     */
    
    res = svr_CreateServer(&(cln->s),URL,1);
    if (res->error == 1) {
      cln_FreeClient(&cln);
      soc[*ReturnedID] = NULL;
      *ReturnedID = -1;
    } else {
      /* 
       * The CreateServer is a success. Now get the source projection
       * and set the target projection with this projection as a
       * default value. 
       */
      
      res = svr_GetServerProjection(&(cln->s));
      if ((res->error == 0) && (res->res.type == AText)) {
	projection = (char *) malloc(strlen(res->res.ecs_ResultUnion_u.s)+1);
	if (projection != NULL) {
	  strcpy(projection,res->res.ecs_ResultUnion_u.s);
	  res = cln_SetClientProjection(*ReturnedID,projection);
	  free(projection);
	} else {
	  res = &cln_dummy_result;
	  ecs_SetError(res,1,cln_messages[1]);
	  cln_FreeClient(&cln);
	  soc[*ReturnedID] = NULL;
	  *ReturnedID = -1;
	}
      }
    }
  }

  return res;
}

/*
 *----------------------------------------------------------------------
 * Client destruction. Will destroy the client and free
 * is location into soc.
 *
 * IN
 *	ClientID:   Id of the client in soc
 *
 * OUT
 *	ecs_Result: Operation result
 *    
 *----------------------------------------------------------------------
 */

ecs_Result *cln_DestroyClient(ClientID)
     int ClientID;
{
  ecs_Client *cln;
  ecs_Result *msg;

  if (multiblock != 0) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[14]);
    return msg;
  }
  
  cln = soc[ClientID];
  if (cln == NULL) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[2]);
    return msg;
  }
  
  msg = svr_DestroyServer(&(cln->s));
  cln_FreeClient(&cln);
  soc[ClientID] = NULL;

  /* free regex resources for spliturl */
  ecs_SplitURL( NULL, NULL, NULL, NULL );
  ecs_CleanUp( &cln_dummy_result );

  return msg;
}

/*
 *----------------------------------------------------------------------
 * cln_SelectLayer: Select a layer of object in the database.
 * If the layer is present in a local cache, the selection
 * will be there.
 *
 * IN
 *	int ClientID:   Id of the client in soc
 *	ecs_LayerSelection *ls: Layer information structure
 *
 * OUT
 *	return ecs_Result: Operation result
 *
 *----------------------------------------------------------------------
 */


ecs_Result *cln_SelectLayer(ClientID, ls)
     int ClientID;
     ecs_LayerSelection *ls;
{
  ecs_Cache *CachePtr;  
  register ecs_Client *cln;
  ecs_Result *res;
  char *error_message;
  ecs_CtlPoints *cpts;
  ecs_Region region;
  int regionset = FALSE;
  int code;

  if (multiblock != 0) {
    res = &cln_dummy_result;
    ecs_SetError(res,1,cln_messages[14]);
    return res;
  }
  
  cln = soc[ClientID];

  if (cln == NULL) {
    res = &cln_dummy_result;
    ecs_SetError(res,1,cln_messages[2]);
    return res;
  }

  /* 
   * Found the cache related to this request 
   */

  CachePtr = cln_FoundCache(ClientID, ls);
  if (CachePtr != NULL) {
    cln->selectCache = CachePtr;
    res = &cln_dummy_result;
    CachePtr->currentpos = 0;
    ecs_SetSuccess(res);
  } else {
    cln->selectCache = NULL;
    res = svr_SelectLayer(&(cln->s),ls);
    cln->currentSelectionFamily = ls->F;
    if (res->error == 0 && res->res.type == GeoRegion) {
      region.north = res->res.ecs_ResultUnion_u.gr.north;
      region.south = res->res.ecs_ResultUnion_u.gr.south;
      region.east = res->res.ecs_ResultUnion_u.gr.east;
      region.west = res->res.ecs_ResultUnion_u.gr.west;
      region.ns_res = res->res.ecs_ResultUnion_u.gr.ns_res;
      region.ew_res = res->res.ecs_ResultUnion_u.gr.ew_res;
      regionset = TRUE;
    }
  }  

  if ((res->error == 0) && ((cln->currentSelectionFamily == Matrix) || 
			    (cln->currentSelectionFamily == Image))) {
    cpts = NULL;
    if (!cln_SetRasterConversion(ClientID,&cpts,nn,projective,&error_message)) {
      res = &cln_dummy_result;
      ecs_SetError(res,1,error_message);
      return res;
    }
  }

  if (cln->autoCache != NULL) {
    cln_FreeCache(cln->autoCache);
    cln->autoCache = NULL;
  }

  if (regionset == TRUE) {
    if (region.north != region.south &&
	region.west != region.east &&
	region.ew_res != 0.0 &&
	region.ns_res != 0.0) {
      code = cln_ConvRegion(ClientID,&region,ECS_STOT);
      if (code != 0) {
	res = &cln_dummy_result;
	ecs_SetError(res,1,cln_messages[code]);
	return res;
      }
    } else {
      res = &cln_dummy_result;
      ecs_SetError(res,1,cln_messages[13]);
      return res;
    }
  
    ecs_SetGeoRegion(res,region.north, region.south, 
		     region.east, region.west, region.ns_res, 
		     region.ew_res);
    ecs_SetSuccess(res);	
  }
  
  return res;
}

/*
 *----------------------------------------------------------------------
 * cln_ReleaseLayer: Release a layer of object in the database.
 * If the layer is actually in this local cache, this selection will be
 * unset.
 *
 * IN
 *	int ClientID:   Id of the client in soc
 *	ecs_LayerSelection *ls: Layer information structure
 *
 * OUT
 *	return ecs_Result: Operation result
 *
 *----------------------------------------------------------------------
 */

ecs_Result *cln_ReleaseLayer(ClientID, ls)
     int ClientID;
     ecs_LayerSelection *ls;
{
  register ecs_Client *cln;
  ecs_Result *res;

  if (multiblock != 0) {
    res = &cln_dummy_result;
    ecs_SetError(res,1,cln_messages[14]);
    return res;
  }
  
  cln = soc[ClientID];

  if (cln == NULL) {
    res = &cln_dummy_result;
    ecs_SetError(res,1,cln_messages[2]);
    return res;
  }

  /* 
   * check is the actual cache is this request
   */

  if ((cln->selectCache != NULL) &&
      (cln->selectCache->coverage.Select != NULL) &&
      (strcmp(cln->selectCache->coverage.Select,ls->Select) == 0) &&
      (cln->selectCache->coverage.F == ls->F)) {
    cln->selectCache = NULL;
  }

  if (cln->autoCache != NULL) {
    cln_FreeCache(cln->autoCache);
    cln->autoCache = NULL;
  }

  res = svr_ReleaseLayer(&(cln->s),ls);
  return res;
}


/*
 *----------------------------------------------------------------------
 *
 * cln_SelectRegion: Select the geographic region on the server.
 *
 * IN 
 *	int ClientID: Client identifier
 *	ecs_Region *gr: Geographic region
 *      
 * OUT
 *	return ecs_Result: Operation result
 *
 *----------------------------------------------------------------------
 */

ecs_Result *cln_SelectRegion(ClientID, gr)
     int ClientID;
     ecs_Region *gr;
{
  ecs_Result *msg;
  ecs_Region tmpgr;
  register ecs_Client *cln;
  char *error_message;
  ecs_CtlPoints *cpts;

  if (multiblock != 0) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[14]);
    return msg;
  }
  
  cln = soc[ClientID];
  if (cln == NULL) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[2]);
    return msg;
  }

  tmpgr.north = gr->north;
  tmpgr.south = gr->south;
  tmpgr.east = gr->east;
  tmpgr.west = gr->west;
  tmpgr.ns_res = gr->ns_res;
  tmpgr.ew_res = gr->ew_res;

  cln->currentRegion.north = gr->north;
  cln->currentRegion.south = gr->south;
  cln->currentRegion.east = gr->east;
  cln->currentRegion.west = gr->west;
  cln->currentRegion.ns_res = gr->ns_res;
  cln->currentRegion.ew_res = gr->ew_res;

  cln_ConvRegion(ClientID,&(tmpgr),ECS_TTOS);
  cln->isCurrentRegionSet = TRUE;

  msg = svr_SelectRegion(&(cln->s),&tmpgr);

  if (cln->autoCache != NULL) {
    cln_FreeCache(cln->autoCache);
    cln->autoCache = NULL;
  }

  if ((msg->error == 0) && ((cln->currentSelectionFamily == Matrix) || 
			    (cln->currentSelectionFamily == Image))) {
    cpts = NULL;
    if (!cln_SetRasterConversion(ClientID,&cpts,nn,projective,&error_message)) {
      msg = &cln_dummy_result;
      ecs_SetError(msg,1,error_message);
      return msg;
    }
  }
  
  return msg;
}


/*
 *----------------------------------------------------------------------
 *
 * cln_SelectMask --
 * 
 *	Set a polygon mask for object retreival.
 * 
 * PARAMETERS
 *    INPUT
 *	int ClientID: Client's identifier
 *      ecs_FeatureRing *mask: The mask itself
 *      int isInclusive: A boolean that indicate if the selected objects will be completely inside (TRUE) or only partially inside (FALSE) the polygon
 * 
 * RETURN_VALUE
 *    ecs_Result *: A structure with the result information
 * 
 * PSEUDO_CODE
 * 
 * 1. Set the attributes in ecs_Client with the polygon and the flag
 *    set in the parameters of the function.
 * 
 * 2. Return a success message
 *
 *----------------------------------------------------------------------
 */

ecs_Result *cln_SelectMask(ClientID, mask, isInclusive)
     int ClientID;
     ecs_FeatureRing *mask;
     int isInclusive;
{
  ecs_Result *msg;
  register ecs_Client *cln;
  int i;

  (void) isInclusive;

  if (multiblock != 0) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[14]);
    return msg;
  }
  
  cln = soc[ClientID];
  if (cln == NULL) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[2]);
    return msg;
  }

  if (cln->mask != NULL) {
    if (cln->mask->c.c_val != NULL)
      free(cln->mask->c.c_val);
    free(cln->mask);
    cln->mask = NULL;
  }
  
  cln->mask = (ecs_FeatureRing *) malloc(sizeof(ecs_FeatureRing));
  if (cln->mask == NULL) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[1]);
    return msg;
  }

  cln->mask->centroid.x = mask->centroid.x;
  cln->mask->centroid.y = mask->centroid.y;
  cln->mask->c.c_len = mask->c.c_len;
  
  cln->mask->c.c_val = (ecs_Coordinate *) malloc(sizeof(ecs_Coordinate)*mask->c.c_len);
  if (cln->mask->c.c_val == NULL) {
    free(cln->mask);
    cln->mask = NULL;
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[1]);
    return msg;
  }

  cln->maskregion.west = cln->maskregion.east = mask->c.c_val[0].x;
  cln->maskregion.north = cln->maskregion.south = mask->c.c_val[0].y;
  for (i=0;i<(int) mask->c.c_len;i++) {
    cln->mask->c.c_val[i].x = mask->c.c_val[i].x;
    cln->mask->c.c_val[i].y = mask->c.c_val[i].y;

    if (cln->maskregion.west > mask->c.c_val[i].x)
      cln->maskregion.west = mask->c.c_val[i].x;
    if (cln->maskregion.east < mask->c.c_val[i].x)
      cln->maskregion.east = mask->c.c_val[i].x;
    if (cln->maskregion.south > mask->c.c_val[i].y)
      cln->maskregion.south = mask->c.c_val[i].y;
    if (cln->maskregion.north < mask->c.c_val[i].y)
      cln->maskregion.north = mask->c.c_val[i].y;
  }

  msg = &cln_dummy_result;
  ecs_SetText(msg,"");
  ecs_SetSuccess(msg);
  return msg;
}


/*
 *----------------------------------------------------------------------
 *
 * cln_UnSelectMask --
 * 
 *	Unselect a previously selected mask
 * 
 * PARAMETERS
 *    INPUT
 *	int ClientID: Client's identifier
 * 
 * RETURN_VALUE
 *    ecs_Result *: A structure with the result information
 * 
 * PSEUDO_CODE
 * 
 * 1. Free the memory of the mask in ecs_Client
 *
 *----------------------------------------------------------------------
 */

ecs_Result *cln_UnSelectMask(ClientID)
     int ClientID;
{
  ecs_Result *msg;
  register ecs_Client *cln;

  if (multiblock != 0) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[14]);
    return msg;
  }
  
  cln = soc[ClientID];
  if (cln == NULL) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[2]);
    return msg;
  }

  if (cln->mask != NULL) {
    if (cln->mask->c.c_val != NULL)
      free(cln->mask->c.c_val);
    free(cln->mask);
    cln->mask = NULL;
  }
  
  msg = &cln_dummy_result;
  ecs_SetText(msg,"");
  ecs_SetSuccess(msg);
  return msg;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_GetDictionary: Get the dictionary from the server
 *
 * IN 
 *	int ClientID: Client identifier
 *      
 * OUT
 *	return ecs_Result: Operation result
 *
 *----------------------------------------------------------------------
 */


ecs_Result *cln_GetDictionary(ClientID)
     int ClientID;
{
  register ecs_Client *cln;
  ecs_Result *msg;

  if (multiblock != 0) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[14]);
    return msg;
  }
  
  cln = soc[ClientID];
  if (cln == NULL) {
    ecs_SetError(&cln_dummy_result,1,cln_messages[2]);
    return &cln_dummy_result;
  }

  msg = svr_GetDictionary(&(cln->s));
  return msg;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_GetAttributesFormat: Get the attribute format of a given layer
 *
 * IN 
 *	int ClientID: Client identifier
 *      
 * OUT
 *	return ecs_Result: Operation result
 *
 *----------------------------------------------------------------------
 */

ecs_Result *cln_GetAttributesFormat(ClientID)
     int ClientID;
{
  register ecs_Client *cln;
  ecs_Result *msg;

  if (multiblock != 0) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[14]);
    return msg;
  }
  
  cln = soc[ClientID];
  if (cln == NULL) {
    ecs_SetError(&cln_dummy_result,1,cln_messages[2]);
    return &cln_dummy_result;
  }

  msg = svr_GetAttributesFormat(&(cln->s));
  return msg;
}


/*
 *----------------------------------------------------------------------
 *
 * cln_GetNextObject: Get the next selected object in the server.
 *
 * IN 
 *	int ClientID: Client identifier
 *      
 * OUT
 *	return ecs_Result: Operation result
 *
 *----------------------------------------------------------------------
 */


ecs_Result *cln_GetNextObject(ClientID)
     int ClientID;
{
  ecs_Result *msg, *obj, *temp;
  register ecs_Client *cln;
  int code;
  int pos;
  ecs_Cache *cache;
  int n;

  if (multiblock != 0) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[14]);
    return msg;
  }
  
  cln = soc[ClientID];
  if (cln == NULL) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[2]);
    return msg;
  }

  /*
   * Extract the object
   */

  if (cln->selectCache) {
    cache = cln->selectCache;
  } else if (cln->autoCache) {
    cache = cln->autoCache;
  } else {
    cache = NULL;
  }

  msg = NULL;
  if (cache) {
    pos = cache->currentpos - cache->startpos;
    if (pos >= 0 && pos < cache->size) {
      msg = cache->o[pos];
      if (msg->res.type == Object) {
	code = cln_ChangeProjection(ClientID, &(msg->res.ecs_ResultUnion_u.dob));
	if (code > 0) {
	  msg = &cln_dummy_result;
	  ecs_SetError(msg,1,cln_messages[code]);
	}
      }

      while (!(cln_IsGeoObjectInsideMask(ClientID,msg)) && !(msg->error)) {
	cache->currentpos++;
	pos = cache->currentpos - cache->startpos;
	if (pos >= 0 && pos < cache->size-1) {
	  msg = cache->o[pos];
	  if (msg->res.type == Object) {
	    code = cln_ChangeProjection(ClientID, &(msg->res.ecs_ResultUnion_u.dob));
	    if (code > 0) {
	      msg = &cln_dummy_result;
	      ecs_SetError(msg,1,cln_messages[code]);
	    }
	  }
	} else {
	  msg = &cln_dummy_result;
	  ecs_SetSuccess(msg);
	  break;
	}
      }

      cache->currentpos++;
      if (msg->error) {
	ecs_SetError(&cln_dummy_result,msg->error,msg->message);
	msg = &cln_dummy_result;
	cln_FreeCache(cln->autoCache);
	cln->autoCache = NULL;
	return msg;
      }
    } else if (cln->selectCache) {
      /* Not allowed outside the bounds of the cache with explicit caches */
      msg = &cln_dummy_result;
      ecs_SetError(msg,2,"End of selection in cache");
      return msg;
    } else {
      cln_FreeCache(cln->autoCache);
      cln->autoCache = NULL;
    }
  }

  if (msg == NULL) {
    msg = svr_GetNextObject(&(cln->s));
    if (msg->res.type == Object) {
      code = cln_ChangeProjection(ClientID, &(msg->res.ecs_ResultUnion_u.dob));
      if (code > 0) {
	msg = &cln_dummy_result;
	ecs_SetError(msg,1,cln_messages[code]);
      }
    }

    /* 
     *  Apply the filter of the mask if not MultiResult 
     */

    while ((msg->res.type != MultiResult) && !(msg->error) && !(cln_IsGeoObjectInsideMask(ClientID,msg))) {
      msg = svr_GetNextObject(&(cln->s));
      if (msg->res.type == Object) {
	code = cln_ChangeProjection(ClientID, &(msg->res.ecs_ResultUnion_u.dob));
	if (code > 0) {
	  msg = &cln_dummy_result;
	  ecs_SetError(msg,1,cln_messages[code]);
	}
      }
    }

    /*
     * If the result contains multiple pieces, we need to break
     * the response into separate result pieces.
     */

    if (msg->res.type == MultiResult) {
      n = msg->res.ecs_ResultUnion_u.results.results_len;
      if (msg->error) {
	n++;
      }
      cache = cln_NewCache(n);
      if(cache == NULL) {
	msg = &cln_dummy_result;
	ecs_SetError(msg, 1, cln_messages[1]);
	return msg;
      }

      for (n = 0; n < (int) msg->res.ecs_ResultUnion_u.results.results_len; n++) {
	code = ecs_CopyResultFromUnion(&msg->res.ecs_ResultUnion_u.results.results_val[n],&obj);
	if (code == FALSE) {
	  cln_FreeCache(cache);

	  msg = &cln_dummy_result;
	  ecs_SetError(msg, 1, cln_messages[1]);
	  return msg;
	}
	cache->o[cache->size++] = obj;
      }
      if (msg->error) {
	obj = malloc(sizeof(ecs_Result));
	obj->message = NULL;
	ecs_SetError(obj,msg->error,msg->message);
	cache->o[cache->size++] = obj;
      }
      cln->autoCache = cache;
      temp = cln_GetNextObject(ClientID);
      return temp;
    }
  }
  return msg;
}

/*
  ----------------------------------------------------------------------
 
  cln_IsGeoObjectInsideMask
  
  Indicate if the object is inside the mask. If the object
  is not valid or the mask don't exist, return a success message (the
  program will handle the error but we must give him the chance to do
  it). Only vector is handle by this function, but it could be modified
  in the future to clip rasters.
  
  PARAMETERS
  INPUT
 	 int ClientID: Client's identifier
	 ecs_Result *obj: The geographic object
	 
  RETURN_VALUE
         int : The boolean indication if the object is outside or inside the mask
  
  PSEUDO_CODE
  
 
  ----------------------------------------------------------------------
  */

int cln_IsGeoObjectInsideMask(ClientID, obj)
     int ClientID;
     ecs_Result *obj;
{
  register ecs_Client *cln;

  cln = soc[ClientID];
  if (cln == NULL)
    return TRUE;

  if (obj->res.type != Object)
    return TRUE;
  if (obj->res.ecs_ResultUnion_u.dob.geom.family != Line &&
      obj->res.ecs_ResultUnion_u.dob.geom.family != Area &&
      obj->res.ecs_ResultUnion_u.dob.geom.family != Point &&
      obj->res.ecs_ResultUnion_u.dob.geom.family != Text)
    return TRUE;

  /*
    If the mask don't exist, the object is always valid
    */

  if (cln->mask == NULL)
    return TRUE;

  /*
    Check the bounding box of the object, if it's outside the extent of the mask,
    always return FALSE
    */

  if (!((cln->maskregion.north > obj->res.ecs_ResultUnion_u.dob.ymin) &&
	(cln->maskregion.south < obj->res.ecs_ResultUnion_u.dob.ymax) &&
	(cln->maskregion.east  > obj->res.ecs_ResultUnion_u.dob.xmin) &&
	(cln->maskregion.west  < obj->res.ecs_ResultUnion_u.dob.xmax))) {
    return FALSE;
  }

  /*
    Knowing the object is inside the extent of the mask is not enough. Check each corner
    of the extent of the object. If one of the corner is inside the mask and cln->isMaskInclusive
    is FALSE, return TRUE. If the four corner is inside the mask and cln->isMaskInclusive is TRUE,
    return TRUE. Else, return FALSE.
    */

  if (ecs_IsPointInPolygon(cln->mask->c.c_len,cln->mask->c.c_val,
			   obj->res.ecs_ResultUnion_u.dob.xmin,
			   obj->res.ecs_ResultUnion_u.dob.ymin)) {
    if (!cln->isMaskInclusive)
      return TRUE;
  }

  if (ecs_IsPointInPolygon(cln->mask->c.c_len,cln->mask->c.c_val,
			   obj->res.ecs_ResultUnion_u.dob.xmin,
			   obj->res.ecs_ResultUnion_u.dob.ymax)) {
    if (!cln->isMaskInclusive)
      return TRUE;
  }

  if (ecs_IsPointInPolygon(cln->mask->c.c_len,cln->mask->c.c_val,
			   obj->res.ecs_ResultUnion_u.dob.xmax,
			   obj->res.ecs_ResultUnion_u.dob.ymin)) {
    if (!cln->isMaskInclusive)
      return TRUE;
  }

  if (ecs_IsPointInPolygon(cln->mask->c.c_len,cln->mask->c.c_val,
			   obj->res.ecs_ResultUnion_u.dob.xmax,
			   obj->res.ecs_ResultUnion_u.dob.ymax)) {
    return TRUE;
  }

  return FALSE;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_GetRasterInfo: Get the raster information for a given selected layer.
 *
 * IN 
 *	int ClientID: Client identifier
 *      
 * OUT
 *	return ecs_Result: Operation result
 *
 *----------------------------------------------------------------------
 */

ecs_Result *cln_GetRasterInfo(ClientID)
     int ClientID;
{
  register ecs_Client *cln;
  ecs_Result *msg;

  if (multiblock != 0) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[14]);
    return msg;
  }
  
  cln = soc[ClientID];
  if (cln == NULL) {
    ecs_SetError(&cln_dummy_result,1,cln_messages[2]);
    return &cln_dummy_result;
  }

  msg = svr_GetRasterInfo(&(cln->s));
  return msg;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_GetObject: Get an object in the server with an id.
 *
 * IN 
 *	int ClientID: Client identifier
 *	char *id: Identifiant of object
 *
 * OUT
 *	return ecs_Result: Operation result
 *
 *----------------------------------------------------------------------
 */


ecs_Result *cln_GetObject(ClientID,id)
     int ClientID;
     char *id;
{
  ecs_Result *msg;
  register ecs_Client *cln;
  int code;

  if (multiblock != 0) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[14]);
    return msg;
  }
  

  cln = soc[ClientID];
  if (cln == NULL) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[2]);
    return msg;
  }

  msg = svr_GetObject(&(cln->s),id);
  if (msg->res.type == Object) {
    code = cln_ChangeProjection(ClientID, &(msg->res.ecs_ResultUnion_u.dob));
    if (code > 0) {
      msg = &cln_dummy_result;
      ecs_SetError(msg,1,cln_messages[code]);
    }
  }

  return msg;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_GetObjectIdFromCoord: Get the id of the nearest object of a point 
 * in the selected layer.
 *
 * IN 
 *	int ClientID: Client identifier
 *	ecs_Coordinate *coord: coordinate
 *
 * OUT
 *	return ecs_Result: Operation result
 *
 *----------------------------------------------------------------------
 */

ecs_Result *cln_GetObjectIdFromCoord(ClientID, coord)
     int ClientID;
     ecs_Coordinate *coord;
{
  double dx,dy;
  int position;
  double distance;
  double mindist;
  int error_code;
  register ecs_Client *cln;
  ecs_Result *msg;
  char *id;

  if (multiblock != 0) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[14]);
    return msg;
  }
  

  cln = soc[ClientID];
  if (cln == NULL) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[2]);
    return msg;
  }

  dx = coord->x;
  dy = coord->y;
  if ((error_code = cln_ConvTtoS(ClientID, &dx, &dy)) > 0) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[error_code]);
    return msg;    
  }
  coord->x = dx;
  coord->y = dy;

  if (cln->selectCache == NULL) {
    msg = svr_GetObjectIdFromCoord(&(cln->s),coord);
  } else {
    /* 
     * If a cache exist for this layer, select directly the
     * object in this cache. 
     */

    switch(cln->selectCache->coverage.F) {
    case Line:
    case Area:
    case Text:
    case Point:
      position = 0;
      distance = ecs_DistanceObject(&(cln->selectCache->o[position]->res.ecs_ResultUnion_u.dob),dx,dy);
      if (distance>=0.0) 
	mindist = distance; 
      else 
	mindist = HUGE_VAL;
      cln->selectCache->currentpos = 0;
      for(position=1;position<cln->selectCache->size;position++) {
	distance = ecs_DistanceObject(&(cln->selectCache->o[position]->res.ecs_ResultUnion_u.dob),dx,dy);
	if (mindist>distance && distance>=0.0) {
	  mindist=distance;
	  cln->selectCache->currentpos = position;
	}
      }
      id = cln->selectCache->o[cln->selectCache->currentpos]->res.ecs_ResultUnion_u.dob.Id;
      msg = &cln_dummy_result;
      ecs_SetText(msg,id);
      break;
    default:
      msg = svr_GetObjectIdFromCoord(&(cln->s),coord);
      break;
    }
  }
  return msg;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_UpdateDictionary: Get the update dictionary information from the server
 *
 * IN 
 *	int ClientID: Client identifier
 *      
 * OUT
 *	return ecs_Result: Operation result
 *
 *----------------------------------------------------------------------
 */

ecs_Result *cln_UpdateDictionary(ClientID, info)
     int ClientID;
     char *info;
{
  register ecs_Client *cln;
  ecs_Result *msg;

  if (multiblock != 0) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[14]);
    return msg;
  }
  

  if (info == NULL) {
    info = cln_empty_string;
  }

  cln = soc[ClientID];
  if (cln == NULL) {
    ecs_SetError(&cln_dummy_result,1,cln_messages[2]);
    return &cln_dummy_result;
  }
  msg = svr_UpdateDictionary(&(cln->s),info);
  return msg;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_GetGlobalBound: Get the global rectangle of the server.
 *
 * IN 
 *	ClientID: Client identifier
 *      
 * OUT
 *	return ecs_Result: Operation result
 *
 *----------------------------------------------------------------------
 */

ecs_Result *cln_GetGlobalBound(ClientID)
     int ClientID;
{
  ecs_Result *res;
  register ecs_Client *cln;
  int code;

  if (multiblock != 0) {
    res = &cln_dummy_result;
    ecs_SetError(res,1,cln_messages[14]);
    return res;
  }
  
  cln = soc[ClientID];
  if (cln == NULL) {
    res = &cln_dummy_result;
    ecs_SetError(res,1,cln_messages[2]);
    return res;
  }

  res = svr_GetGlobalBound(&(cln->s));
  if (res->error == 0) {
    if (res->res.type == GeoRegion &&
	res->res.ecs_ResultUnion_u.gr.north != res->res.ecs_ResultUnion_u.gr.south &&
	res->res.ecs_ResultUnion_u.gr.west != res->res.ecs_ResultUnion_u.gr.east &&
	res->res.ecs_ResultUnion_u.gr.ew_res != 0.0 &&
	res->res.ecs_ResultUnion_u.gr.ns_res != 0.0) {
      code = cln_ConvRegion(ClientID,&(res->res.ecs_ResultUnion_u.gr),ECS_STOT);
      if (code != 0) {
	res = &cln_dummy_result;
	ecs_SetError(res,1,cln_messages[code]);
      }
    } else {
      res = &cln_dummy_result;
      ecs_SetError(res,1,cln_messages[13]);
    }
  }
  
  return res;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_SetServerLanguage: Set the server language
 *
 * IN 
 *	int ClientID: Client identifier
 *	u_int language: Server language applied
 *      
 * OUT
 *	return ecs_Result: Operation result
 *
 *----------------------------------------------------------------------
 */

ecs_Result *cln_SetServerLanguage(ClientID,language)
     int ClientID;
     u_int language;
{
  register ecs_Client *cln;
  ecs_Result *msg;

  if (multiblock != 0) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[14]);
    return msg;
  }
  
  cln = soc[ClientID];
  if (cln == NULL) {
    ecs_SetError(&cln_dummy_result,1,cln_messages[2]);
    return &cln_dummy_result;
  }
  msg = svr_SetServerLanguage(&(cln->s),language);
  return msg;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_SetServerProjection: Set the server projection
 *
 * IN 
 *	int ClientID: Client identifier
 *	char *projection: Server projection applied
 *      
 * OUT
 *	return ecs_Result: Operation result
 *
 *----------------------------------------------------------------------
 */

ecs_Result *cln_SetServerProjection(ClientID,projection)
     int ClientID;
     char *projection;
{
  register ecs_Client *cln;
  char *error_message;
  ecs_CtlPoints *cpts;
  ecs_Result *res;
  PJ *tempproj;

  if (multiblock != 0) {
    res = &cln_dummy_result;
    ecs_SetError(res,1,cln_messages[14]);
    return res;
  }
  
  if (projection != NULL) {
    while(projection[0]==' ')
      projection++;
  }

  cln = soc[ClientID];
  if (cln == NULL) {
    ecs_SetError(&cln_dummy_result,1,cln_messages[2]);
    return &cln_dummy_result;
  }

  /* Check if the projection is valid */

  if (strncmp(projection,PROJ_LONGLAT,13) != 0) {
    tempproj = (PJ *) cln_ProjInit(projection);
    if(tempproj == NULL) {
      res = &cln_dummy_result;
      ecs_SetError(res,1,"This projection string is invalid");
      return res;
    }
    pj_free(tempproj);
  }

  res = svr_SetServerProjection(&(cln->s),projection);
  if ((res->error == 0) && ((cln->currentSelectionFamily == Matrix) || 
			    (cln->currentSelectionFamily == Image))) {
    cpts = NULL;
    if (!cln_SetRasterConversion(ClientID,&cpts,nn,projective,&error_message)) {
      res = &cln_dummy_result;
      ecs_SetError(res,1,error_message);
      return res;
    }
  }


  return res;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_GetServerProjection: Get the server projection.
 *
 * IN
 *	ClientID: Client's identifier
 *  
 * OUT
 *	return ecs_Result: Operation result
 *
 *----------------------------------------------------------------------
 */

ecs_Result *cln_GetServerProjection(ClientID)
     int ClientID;
{
  register ecs_Client *cln;
  ecs_Result *msg;

  if (multiblock != 0) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[14]);
    return msg;
  }
  
  cln = soc[ClientID];

  if (cln == NULL) {
    ecs_SetError(&cln_dummy_result,1,cln_messages[2]);
    return &cln_dummy_result;
  }

  msg = svr_GetServerProjection(&(cln->s));
  return msg;
}

/*
  ----------------------------------------------------------------------
  
   cln_SetClientProjection --
   
  	Set the target (client) projection. In return, if the operation
  	is a success, it will return the current region transformed in
  	the new projection.
   
   PARAMETERS
      INPUT
  	int ClientID: Client's identifier
  	char *projection: Projection string
   
   RETURN_VALUE
      ecs_Result *: A structure with the result information
   
   PSEUDO_CODE
   
   1. Verify if the client id is valid. If not, return an error
   message
   
   2. Keep the old projection initiator and the different flags of the
   client structure related to the projection and the nad.  If a
   problem appear in the current function, the function will be able
   to get back to is original stage.
   
   3. Change the client region into the projection of the source. This
   way, when the projection will be completely set, we will be able to
   know the region of the current region in the new projection.
   
   4. Set the isProjEqual (the indicator of projection to FALSE).
   Totalitarian algorithm.
   
   5. Check if target (client) projection is "unknown". If it's the
   case, set the isProjEqual to true and return a success message.
   
   6. Check if the target (client) projection is "longlat". If it's
   the case
   Begin
      6.1. Set isTargetLL to TRUE
   End
   7. Else
   Begin
      7.1. Set isTargetLL to FALSE

      7.2. Call cln_ProjInit to set the cln->target. If an error
      occur, go to 99.
   End
   
   8. Get the driver projection from the source. If an error occur,
   return an error.
   
   9. Check if the source (driver) projection is "longlat". If it's
   the case
   Begin
      9.1. Set isSourceLL to TRUE
   End
   10. Else
   Begin
      10.1. Set isSourceLL to FALSE

      10.2. Call cln_ProjInit to set the cln->source. If an error
      occur, go to 99.
   End
   
   11. If a link to a datum converter already exist, close the old one
   but keep the old converter information in memory if a problem
   appear.
   
   12. Datum initialization. Check the string of the target projection
   and the source projection and extract the nad item descriptor
   (nad27 or nad83).
   
   13. If the nads are differents 
   Begin
      13.1. Extract the table indicator from the target projection.
   
      13.2. If the table indicator don't exist, use the default one.
      
      13.3. Open a link to the nad converter driver named after the
      table indicator and call nad_init. If an error occur, return an
      error message and goto 99.
   End
   
   14. Check if an azimuth is defined in the projection definition. If
   it's the case, get the value and set target_azimuth with the
   value. Take care to apply a modulo of 360 to this value.

   15. Change the current client region previously calculated in the
   driver projection to the new projection. If the current region is
   not set, use the global region. If an error occur, goto 99.
   
   16. Free the old source and target
   
   17. Return the region calculated in 15.
   
   99. Free the new target and source and set the projection
   attributes to the value kept in 2. Restore also the old datum
   environment.  Return an error message.

   MOD: Bruno Savard, INFOMAR INC., bsavard@infomar.com, 1998/09/21
   Description:  The result of ecsGetDynamicLibFunction() has been cast
                 to type (dtfunc *).  The original cas to (void *)
                 seems incorrect.
  
  ----------------------------------------------------------------------
  */

ecs_Result *cln_SetClientProjection(ClientID,projection)
     int ClientID;
     char *projection;
{
  ecs_Result *res;
  register ecs_Client *cln;
  char *server_proj;
  PJ *oldtarget,*oldsource;
  ecs_Region oldreg;
  int oldisprojequal;
  int oldisTargetLL;
  int oldisSourceLL;
  int code = 0;
  int result;
  char olddatumtable[10],datumtable[10];
  char filename[15];
  ecs_Datum oldtargetdatum=0;
  ecs_Datum oldsourcedatum=0;
  char *ptr,*ptr1=NULL;
  char *temp;
  int tempint;

  if (multiblock != 0) {
    res = &cln_dummy_result;
    ecs_SetError(res,1,cln_messages[14]);
    return res;
  }
  
  cln = soc[ClientID];
  if (cln == NULL) {
    res = &cln_dummy_result;
    ecs_SetError(res,1,cln_messages[2]);
    return res;
  }

  if (projection != NULL) {
    while(projection[0]==' ')
      projection++;
  }

  /* If the old projection string is the same, don't change anything */

  if (cln->target_proj != NULL && strcmp(cln->target_proj,projection) == 0) {
    res = &cln_dummy_result;
    ecs_SetSuccess(res);
    return res;    
  }

  /* keep the old values */

  oldisprojequal = cln->isProjEqual;
  oldisTargetLL= cln->isTargetLL;
  oldisSourceLL= cln->isSourceLL;
  oldtarget = cln->target;
  oldsource = cln->source;

  /* Change the current client region to the projection of the driver */

  if (cln->isCurrentRegionSet == TRUE) {
    oldreg.north = cln->currentRegion.north;
    oldreg.south = cln->currentRegion.south;
    oldreg.east = cln->currentRegion.east;
    oldreg.west = cln->currentRegion.west;
    oldreg.ns_res = cln->currentRegion.ns_res;
    oldreg.ew_res = cln->currentRegion.ew_res;
    result = cln_ConvRegion(ClientID,&(oldreg),ECS_TTOS);
    if (result != 0) {
      code = 10;
      goto setprojerror;
    }
  }


  /* 
   * Strategy: 
   * Keep the old projection until the new projection is correctly set. 
   * If a problem appear, nothing will change.
   */

  cln->isProjEqual = FALSE;

  /*
   * Set the client projection
   */

  cln->target = NULL;

  /* Check if client projection is "unknown" */

  if (strncmp(projection,PROJ_UNKNOWN,7) == 0) {
    cln->isProjEqual = TRUE;
    res = &cln_dummy_result;
    ecs_SetGeoRegion(res,cln->currentRegion.north, 
		     cln->currentRegion.south, 
		     cln->currentRegion.east,
		     cln->currentRegion.west, 
		     cln->currentRegion.ns_res, 
		     cln->currentRegion.ew_res);  
    ecs_SetSuccess(res);
    return res;    
  }

  /* Check the projection if not unknown */

  if (strncmp(projection,PROJ_LONGLAT,13) != 0) {
    cln->isTargetLL = FALSE;
    cln->target = (PJ *) cln_ProjInit(projection);
    if(cln->target == NULL) {
      goto setprojerror;
    }
  } else {  
    cln->isTargetLL = TRUE;
  }

  /*
   * Get the server projection
   */

  res = cln_GetServerProjection(ClientID);
  if ((res->res.type == AText) || (res->res.ecs_ResultUnion_u.s != NULL)) {
    server_proj = res->res.ecs_ResultUnion_u.s;
    while(server_proj[0]==' ')
      server_proj++;
  } else {
    return res;
  }

  /*
   * Set the server projection
   */

  cln->source = NULL;

  /* Check if server projection is "unknown" */

  if (strncmp(server_proj,PROJ_UNKNOWN,7) == 0) {
    cln->isProjEqual = TRUE;
    res = &cln_dummy_result;
    ecs_SetGeoRegion(res,cln->currentRegion.north, 
		     cln->currentRegion.south, 
		     cln->currentRegion.east,
		     cln->currentRegion.west, 
		     cln->currentRegion.ns_res, 
		     cln->currentRegion.ew_res);  
    ecs_SetSuccess(res);
    return res;    
  }

  /* Check the projection if not unknown */

  if (strncmp(server_proj,PROJ_LONGLAT,13) != 0) {
    cln->isSourceLL = FALSE;
    cln->source = (PJ *) cln_ProjInit(server_proj);
    if(cln->source == NULL) {
      goto setprojerror;
    }

  } else {  
    cln->isSourceLL = TRUE;
  }

  /*
   * 11. If a link to a datum converter already exist, close the old one but
   * keep the old converter in memory if a problem appear.
   */

  if (cln->dthandle != NULL && (cln->nad_close != NULL)) {
    (cln->nad_close)(cln->privdatuminfo);
    ecs_CloseDynamicLib(cln->dthandle);
  }      
  strcpy(olddatumtable,cln->datumtable);
  oldtargetdatum = cln->targetdatum;
  oldsourcedatum = cln->sourcedatum;
  strcpy(cln->datumtable,"");
  cln->dthandle = NULL;

  /*
   * 12. Datum initialization. Check the string of the target projection
   * and the source projection and extract the nad item descriptor (nad27
   * or nad83).
   */

  cln->targetdatum = cln_GetDatumInfo(projection);
  cln->sourcedatum = cln_GetDatumInfo(server_proj);

  if ((cln->targetdatum == nad27 && cln->sourcedatum == nad83) ||
      (cln->targetdatum == nad83 && cln->sourcedatum == nad27)) {
    /*
     * 13.1. Extract the table indicator from the target projection. 
     */
    
    strcpy(datumtable,"conus");
    ptr = projection;
    while(ptr[0] != '\0') {
#ifdef _WINDOWS
      if (_strnicmp(ptr,"+datumconv",10) == 0) 
#else
	if (strncasecmp(ptr,"+datumconv",10) == 0)
#endif
	  {
	    ptr+=11;
	    if (ptr != '\0') {
	      ptr1 = ptr;
	      while(ptr1[0] != '\0' && ptr1[0] != ' ') 
		ptr1++;
	    }
	    if ((int) (ptr1-ptr) < 10) {
	      strncpy(datumtable,ptr,(int) (ptr1-ptr));
	      datumtable[(int) (ptr1-ptr)] = '\0';
	    } 
	  }
      ptr++;
    }    

#ifdef _WINDOWS
    if (_stricmp(datumtable,"canada") == 0)
#else
      if (strncasecmp(datumtable,"canada",10) == 0)
#endif
	{
	  strcpy(filename,"dtcanada");
	} else {
	  strcpy(filename,"dtusa");
	}
    
    cln->dthandle = ecs_OpenDynamicLib(filename);
    if (cln->dthandle != NULL) {
/**MOD START**/
      cln->nad_init = (dtfunc *) ecs_GetDynamicLibFunction(cln->dthandle,"dyn_nad_init");
      cln->nad_forward = (dtfunc *) ecs_GetDynamicLibFunction(cln->dthandle,"dyn_nad_forward");
      cln->nad_reverse = (dtfunc *) ecs_GetDynamicLibFunction(cln->dthandle,"dyn_nad_reverse");
      cln->nad_close = (dtfunc *) ecs_GetDynamicLibFunction(cln->dthandle,"dyn_nad_close");
/**MOD END**/

      if (cln->nad_init == NULL || cln->nad_forward == NULL ||
	  cln->nad_reverse == NULL || cln->nad_close == NULL) {
      } else {
	if ((cln->nad_init)(&(cln->privdatuminfo),datumtable) != TRUE) {
	}
      }
    } 
  }

  /*
    Check if in the target there is a azimuth defined
    */

  ptr = projection;
  cln->target_azimuth = 0.0;
  while(ptr[0] != '\0') {
#ifdef _WINDOWS
    if (_strnicmp(ptr,"+azimuth",8) == 0) 
#else
      if (strncasecmp(ptr,"+azimuth",8) == 0)
#endif
	{
	  ptr+=9;
	  if (ptr != '\0') {
	    sscanf(ptr,"%d",&tempint);
	    cln->target_azimuth = (double) tempint;
	    cln->target_azimuth = cln->target_azimuth*PI/180.0;
	    cln->sinazimuth = sin(cln->target_azimuth);
	    cln->cosazimuth = cos(cln->target_azimuth);
	  }
	}
    ptr++;
  }

  /*
   * Compare both projections and see if they are equals
   */

  cln->isProjEqual = cln_CompareProjections(ClientID);

  /*
   * Get the global bound and put it on currentRegion
   */

  /*
   * Change the current client region previously calculated 
   * in the driver projection to the new projection
   */

  if (cln->isCurrentRegionSet == TRUE) {
    if (cln_ConvRegion(ClientID,&oldreg,ECS_STOT) == 0) {
      cln->currentRegion.north  = oldreg.north;
      cln->currentRegion.south  = oldreg.south;
      cln->currentRegion.east   = oldreg.east;
      cln->currentRegion.west   = oldreg.west;
      cln->currentRegion.ns_res = oldreg.ns_res;
      cln->currentRegion.ew_res = oldreg.ew_res;  
      res = cln_SelectRegion(ClientID,&(cln->currentRegion));
    } else {
      code = 10;
      goto setprojerror;
    }
  } else {
    res = cln_GetGlobalBound(ClientID);
    if ((res->error == 0) && (res->res.type == GeoRegion)) {
      cln->currentRegion.north  = ECSRESULT(res).gr.north;
      cln->currentRegion.south  = ECSRESULT(res).gr.south;
      cln->currentRegion.east   = ECSRESULT(res).gr.east;
      cln->currentRegion.west   = ECSRESULT(res).gr.west;
      cln->currentRegion.ns_res = ECSRESULT(res).gr.ns_res;
      cln->currentRegion.ew_res = ECSRESULT(res).gr.ew_res;
      
      res = cln_SelectRegion(ClientID,&(cln->currentRegion));
    } else {
      code = 11;
      goto setprojerror;
    }
  }

  if (cln->mask != NULL) {
    if (cln->mask->c.c_val != NULL)
      free(cln->mask->c.c_val);
    free(cln->mask);
    cln->mask = NULL;
  }

  temp = malloc(strlen(projection)+1);
  if (temp == NULL) {
    code = 2;
    goto setprojerror;
  }
  if (cln->target_proj != NULL)
    free(cln->target_proj);
  cln->target_proj = temp;
  strcpy(cln->target_proj,projection);

  if (oldtarget != NULL) 
    pj_free(oldtarget);
  if (oldsource != NULL) 
    pj_free(oldsource);

  res = &cln_dummy_result;
  ecs_SetGeoRegion(res,cln->currentRegion.north, 
		   cln->currentRegion.south, 
		   cln->currentRegion.east,
		   cln->currentRegion.west, 
		   cln->currentRegion.ns_res, 
		   cln->currentRegion.ew_res);  

  ecs_SetSuccess(res);
  return res;

setprojerror:

  if (cln->target != NULL) 
    pj_free(cln->target);      
  if (cln->source != NULL) 
    pj_free(cln->source);      
  cln->isProjEqual = oldisprojequal;      
  cln->isTargetLL  = oldisTargetLL;
  cln->isSourceLL  = oldisSourceLL;
  cln->target      = oldtarget;
  cln->source      = oldsource;

  /*
   * Datum reinitiator
   */

  if (strlen(olddatumtable) != 0) {
#ifdef _WINDOWS
    if (_stricmp(olddatumtable,"canada") == 0)
#else
      if (strncasecmp(olddatumtable,"canada",10) == 0)
#endif
	{
	  strcpy(filename,"dtcanada");
	} else {
	  strcpy(filename,"dtusa");
	}
    

    cln->dthandle = ecs_OpenDynamicLib(filename);

    if (cln->dthandle != NULL) {
      cln->dthandle = NULL;
    } else {
      cln->nad_init = (void *) ecs_GetDynamicLibFunction(cln->dthandle,"dyn_nad_init");
      cln->nad_forward = (void *) ecs_GetDynamicLibFunction(cln->dthandle,"dyn_nad_forward");
      cln->nad_reverse = (void *) ecs_GetDynamicLibFunction(cln->dthandle,"dyn_nad_reverse");
      cln->nad_close = (void *) ecs_GetDynamicLibFunction(cln->dthandle,"dyn_nad_close");
      
      if (cln->nad_init == NULL || cln->nad_forward == NULL ||
	  cln->nad_reverse == NULL || cln->nad_close == NULL) {
	ecs_CloseDynamicLib(cln->dthandle);
	cln->dthandle = NULL;
      } else {
	if ((cln->nad_init)(&(cln->privdatuminfo),olddatumtable) != TRUE) {
	  ecs_CloseDynamicLib(cln->dthandle);
	  cln->dthandle = NULL;	  
	}
      }
    }
  } else {
    cln->dthandle = NULL;
  }
  cln->targetdatum = oldtargetdatum;
  cln->sourcedatum = oldsourcedatum;
  
  res = &cln_dummy_result;
  if (code == 0)
    ecs_SetError(res,1,cln_messages[4]);
  else
    ecs_SetError(res,1,cln_messages[code]);

  return res;  
}

/*
 *----------------------------------------------------------------------
 *
 *  FUNCTION_INFORMATION
 *  
 *  NAME
 *     cln_GetDatumInfo
 *
 *  DESCRIPTION
 *     Get the datum information from the datum projection.
 *
 *  END_DESCRIPTION
 *
 *  PARAMETERS
 *     INPUT
 *        char *projection: The projection string
 *  END_PARAMETERS
 *
 *  RETURN_VALUE
 *     ecs_Datum: The returned information. Could be nodatum,
 *     nad27 or nad83.
 *
 *  END_FUNCTION_INFORMATION
 *
 *   PSEUDO_CODE
 *
 *  1. Set ptr to the beginning of the string projection
 *  
 *  2. While ptr is not the end of the string
 *  Begin
 *     2.1. Compare the first 12 characters of ptr and "+datum=nad27"
 *     without the case sensitivity. If they are the same, return
 *     nad27.
 *
 *     2.2. Compare the first 12 characters of ptr and "+datum=nad83"
 *     without the case sensitivity. If they are the same, return
 *     nad83.
 *
 *     2.3. Increment ptr
 *  End
 *
 *  3. Return nodatum
 *
 *----------------------------------------------------------------------
 */

ecs_Datum cln_GetDatumInfo(projection)
     char *projection;
{
  char *ptr;

  if (projection==NULL) return nodatum;

  ptr = projection;
  while(ptr[0]!='\0') {
    
#ifdef _WINDOWS
    if (_strnicmp(ptr,"+datum=nad27",12) == 0) 
#else
      if (strncasecmp(ptr,"+datum=nad27",12) == 0)
#endif
	return nad27;

#ifdef _WINDOWS
    if (_strnicmp(ptr,"+datum=nad83",12) == 0) 
#else
      if (strncasecmp(ptr,"+datum=nad83",12) == 0)
#endif
	return nad83;

    ptr++;
  }

  return nodatum;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_SetTclProc: Set in the client tclprocname with the value of the
 * attribute.
 *
 *  IN
 *	int ClientID: Client's identifier
 *	char *tclproc: TCL procedure name
 *
 *----------------------------------------------------------------------
 */

void cln_SetTclProc(ClientID,tclproc)
     int ClientID;
     char *tclproc;
{
  register ecs_Client *cln;

  cln = soc[ClientID];
  if (cln == NULL) {
    return;
  }

  if (cln->tclprocname != NULL)
    free(cln->tclprocname);
  if (tclproc != NULL) {
    cln->tclprocname = (char *) malloc(strlen(tclproc)+1);
    if (cln->tclprocname != NULL)
      strcpy(cln->tclprocname, tclproc);
  } else {
    cln->tclprocname = NULL;
  }
  return;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_GetTclProc: Return the client tclprocname
 *
 * IN
 *	int ClientID: Client's identifier
 *
 * OUT
 *	return char *: Pointer to tclprocname. If NULL, there is no
 *	tclprocname.
 *
 *----------------------------------------------------------------------
 */

char *cln_GetTclProc(ClientID)
     int ClientID;
{
  register ecs_Client *cln;

  cln = soc[ClientID];
  if (cln == NULL) {
    return NULL;
  }

  return cln->tclprocname;
}

/*
 *----------------------------------------------------------------------
 *
 *  cln_ProjInit: Initialize a projection
 *
 *  IN
 *	char *d: Projection name
 *
 *  OUT
 *	return PJ *: New projection structure. If NULL, the operation
 *	is a failure.
 *
 *----------------------------------------------------------------------
 */

PJ *cln_ProjInit(d)
     char *d;
{
  int i,n;
  char **tableau;
  char *l;
  int longueur;
  char *c;
  PJ *retour;
  
  if ((c = (char *) malloc(strlen(d)+3)) == NULL) {
    return NULL;
  }
  strcpy(c,d);

  n=0;
  l = c;

  longueur = strlen(c);
  if ((tableau = (char **) malloc(sizeof(char *)*longueur)) == NULL) {
    free(c);
    return NULL;
  }
  for(i=0; i<longueur; i++) {
    if (c[i] == ' ') {
      c[i]= '\0';
      tableau[n] = l;
      l = &c[i+1];
      if (strncmp(tableau[n],"+",1) == 0) 
	tableau[n]++;
      n++;
    }
  }
  
  if (strlen(l)>0) {
    tableau[n] = l;
    if (strncmp(tableau[n],"+",1) == 0) 
      tableau[n]++;
    n++;
  }

  retour = (PJ *) pj_init(n, (char **) tableau);
  free(c);
  free(tableau);

  return retour;
}

/*
 *----------------------------------------------------------------------
 *
 *  cln_CompareProjections: Compare target and source projections of a client
 *  and indicate if they are equals.
 *
 *  IN
 *	int ClientID: Client's identifier
 *
 *  OUT
 *	return int: Flag indicate if both projections are equals.
 *
 *----------------------------------------------------------------------
 */

int cln_CompareProjections(ClientID)
     int ClientID;
{
  ecs_Region *gr;
  register ecs_Client *cln;
  ecs_Result *res;

  cln = soc[ClientID];

  if (cln == NULL)
    return FALSE;

  if(cln->isTargetLL == TRUE ||
     cln->isSourceLL == TRUE)
    return FALSE;

  /*
   * Strategy: Extract the default region and compare the four corners in it.
   * If the four corners are equivalent in both projections, the projections
   * will be considered as equivalent.
   */

  res = svr_GetGlobalBound(&(cln->s));
  if ((res->error != 0) || (res->res.type != GeoRegion)) {
    return FALSE;
  }

  gr = &(res->res.ecs_ResultUnion_u.gr);

  if (!cln_PointValid(ClientID,gr->west,gr->south) &&
      !cln_PointValid(ClientID,gr->east,gr->north) &&
      !cln_PointValid(ClientID,gr->west,gr->north) &&
      !cln_PointValid(ClientID,gr->east,gr->south)) {
    return FALSE;
  }
  
  return TRUE;
}


/*
 *----------------------------------------------------------------------
 *
 * cln_UpdateMaxRegion: The function update a geographic region "gr"
 * in a projection "A" and update it with a point (x,y) taken
 * in a projection "B". If "direction" is ECS_TTOS, projection "A" will
 * be the target projection and "B" the source. If "direction" is ECS_TTOS, 
 * projection "A" will be the target projection and "B" the source. 
 *
 * IN
 *	int ClientID : Client's identifier
 *	x,y          : Point to convert
 *	gr           : Geographical region to convert (already allocated)
 *	direction    : Direction of convertion
 *	first        : Indicate if the point (x,y) is the first to convert
 *
 * OUT
 *
 *	return int   : If 0, the operation is a success. Hense,
 *		       an error code is send. This error code correspond
 *		       to a position in cln_messages.
 *
 *----------------------------------------------------------------------
 */

int cln_UpdateMaxRegion(ClientID,x,y,gr,direction,first)
     int ClientID;
     double x;
     double y;
     ecs_Region *gr;
     int direction;
     int first;
{
  int error_code = 0;
  double temp_x = x;
  double temp_y = y;

  if (direction == ECS_STOT) {
    if ((error_code = cln_ConvStoT(ClientID, &temp_x, &temp_y)) != 0) {
      return error_code;
    }
  } else {
    if ((error_code = cln_ConvTtoS(ClientID, &temp_x, &temp_y)) != 0) {
      return error_code;
    }
  }

  if (first==TRUE) {
    gr->north = temp_y;
    gr->south = temp_y;
    gr->east = temp_x;
    gr->west = temp_x;
  } else {
    if (gr->north < temp_y)
      gr->north = temp_y;
    if (gr->south > temp_y)
      gr->south = temp_y;
    if (gr->east < temp_x)
      gr->east = temp_x;
    if (gr->west > temp_x)
      gr->west = temp_x;
  }

  return 0;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_ConvMBR: Convert a geographic region from a projection "A" to 
 * a projection "B". If "direction" is ECS_TTOS, projection "A" will
 * be the target projection and "B" the source. If "direction" is ECS_TTOS, 
 * projection "A" will be the target projection and "B" the source. 
 *
 * IN
 *	int ClientID : Client's identifier
 *	double *xl   : Left of MBR
 *	double *yl   : Bottom of MBR
 *	double *xu   : Right of MBR
 *	double *yu   : Top of MBR
 *	direction    : Direction of convertion
 *
 *  OUT
 *	return int   : If 0, the operation is a success. Hense,
 *		       an error code is send. This error code correspond
 *		       to a position in cln_messages.
 *
 *----------------------------------------------------------------------
 */

int cln_ConvMBR(ClientID,xl,yl,xu,yu,direction)
     int ClientID;
     double *xl;
     double *yl;
     double *xu;
     double *yu;
     int direction;
{
  ecs_Region maxgr;
  double middle_x;
  double fquart_x;
  double squart_x;
  double middle_y;
  double fquart_y;
  double squart_y;
  int ret = 0;
  register ecs_Client *cln;

  cln = soc[ClientID];
  if (cln == NULL)
    return 2;

  middle_x = ((*xl)+(*xu))/2.0;
  middle_y = ((*yu)+(*yl))/2.0;
  fquart_x = (middle_x + (*xl))/2.0;
  squart_x = (middle_x + (*xu))/2.0;
  fquart_y = (middle_y + (*yl))/2.0;
  squart_y = (middle_y + (*yu))/2.0;

  cln_UpdateMaxRegion(ClientID,(*xl),(*yl),&maxgr,direction,TRUE);
  cln_UpdateMaxRegion(ClientID,(*xl),(*yu),&maxgr,direction,FALSE);
  cln_UpdateMaxRegion(ClientID,(*xu),(*yu),&maxgr,direction,FALSE);
  cln_UpdateMaxRegion(ClientID,(*xu),(*yl),&maxgr,direction,FALSE);
  cln_UpdateMaxRegion(ClientID,(*xu),middle_y,&maxgr,direction,FALSE);
  cln_UpdateMaxRegion(ClientID,(*xl),middle_y,&maxgr,direction,FALSE);
  cln_UpdateMaxRegion(ClientID,middle_x,(*yu),&maxgr,direction,FALSE);
  cln_UpdateMaxRegion(ClientID,middle_x,(*yl),&maxgr,direction,FALSE);
  cln_UpdateMaxRegion(ClientID,middle_x,middle_y,&maxgr,direction,FALSE);
  cln_UpdateMaxRegion(ClientID,fquart_x,fquart_y,&maxgr,direction,FALSE);
  cln_UpdateMaxRegion(ClientID,fquart_x,squart_y,&maxgr,direction,FALSE);
  cln_UpdateMaxRegion(ClientID,squart_x,fquart_y,&maxgr,direction,FALSE);
  cln_UpdateMaxRegion(ClientID,squart_x,squart_y,&maxgr,direction,FALSE);
  (*yu) = maxgr.north;
  (*yl) = maxgr.south;
  (*xu) = maxgr.east;
  (*xl) = maxgr.west;
  ret = 0;
  
  return ret;
}     

/*
 *----------------------------------------------------------------------
 *
 * cln_ConvRegion: Convert a geographic region from a projection "A" to 
 * a projection "B". If "direction" is ECS_TTOS, projection "A" will
 * be the target projection and "B" the source. If "direction" is ECS_TTOS, 
 * projection "A" will be the target projection and "B" the source. 
 *
 * IN
 *	int ClientID : Client's identifier
 *	gr           : Geographical region to convert (already allocated)
 *	direction    : Direction of convertion
 *
 *  OUT
 *	return int   : If 0, the operation is a success. Hense,
 *		       an error code is send. This error code correspond
 *		       to a position in cln_messages.
 *
 *----------------------------------------------------------------------
 */

int cln_ConvRegion(ClientID,gr,direction)
     int ClientID;
     ecs_Region *gr;
     int direction;
{
  double width,height;
  int ret = 0;
  int intwidth,intheight;
  register ecs_Client *cln;
  double ox,oy,dx,dy,temp;
  int i;

  cln = soc[ClientID];
  if (cln == NULL)
    return 2;

  width = (gr->east - gr->west) / gr->ew_res;
  height = (gr->north - gr->south) / gr->ns_res;

  intwidth = (int) floor(width+0.5);
  intheight = (int) floor(height+0.5);

  /*
   * Calculate resolution. For that, calculate four points in
   * the middle of the original region. Convert these points
   * and calculate the resolution.
   */

  /* Calculate ew_res */

  ox = gr->west;
  dx = gr->east - 0.00001;
  temp = (ox + dx)/2.0;
  oy = dy = (gr->north + gr->south) / 2.0;

  gr->ew_res = 1.0;
  for(i=0;i<4;i++) {
    if (direction == ECS_STOT) {
      if ((cln_ConvStoT(ClientID, &ox, &oy) == 0) &&
	  (cln_ConvStoT(ClientID, &dx, &dy) == 0)) {
	gr->ew_res = (sqrt(((dx-ox)*(dx-ox)) + ((dy-oy)*(dy-oy))) / 
		      (width/(i+1)));
	break;
      }
    } else {
      if ((cln_ConvTtoS(ClientID, &ox, &oy) == 0) &&
	  (cln_ConvTtoS(ClientID, &dx, &dy) == 0)) {
	gr->ew_res = (sqrt(((dx-ox)*(dx-ox)) + ((dy-oy)*(dy-oy))) / 
		      (width/(i+1)));
	break;
      }
    }
    /* Divide the distance to the central point by 2 */
    
    ox = ox + temp / 2.0;
    dx = dx + temp / 2.0;
  }

  /* Calculate ns_res */

  oy = gr->south;
  dy = gr->north - 0.00001;
  temp = (oy + dy) / 2.0;
  ox = dx = (gr->west + gr->east) / 2.0;

  gr->ns_res = 1.0;
  for(i=0;i<4;i++) {
    if (direction == ECS_STOT) {
      if ((cln_ConvStoT(ClientID, &ox, &oy) == 0) &&
	  (cln_ConvStoT(ClientID, &dx, &dy) == 0)) {
	gr->ns_res = (sqrt(((dx-ox)*(dx-ox)) + ((dy-oy)*(dy-oy))) / 
		      (height/(i+1)));
	break;
      }
    } else {
      if ((cln_ConvTtoS(ClientID, &ox, &oy) == 0) &&
	  (cln_ConvTtoS(ClientID, &dx, &dy) == 0)) {
	gr->ns_res = (sqrt(((dx-ox)*(dx-ox)) + ((dy-oy)*(dy-oy))) / 
		      (height/(i+1)));
	break;
      }
    }
    /* Divide the distance to the central point by 2 */
    
    oy = oy + temp / 2.0;
    dy = dy + temp / 2.0;
  }

  /* Calculate bounding box */

  ret = cln_ConvMBR(ClientID,&(gr->west),&(gr->south),
		    &(gr->east),&(gr->north),direction);

  /* Check the resolution. If the new width and height of the 
     region is 10X smaller than the original one, then simply
     make the calculation with the width and the height. Also
     check if the resolution is negative */

  gr->ew_res = (gr->east - gr->west)/(double) intwidth;
  gr->ns_res = (gr->north - gr->south)/(double) intheight;

  return ret;
}     

/*
  ----------------------------------------------------------------------
  
  
   FUNCTION_INFORMATION
  
   NAME
      cln_ConvTtoS
      
   DESCRIPTION
      Convert a geographical point from the target geographic
      projection (client) to the source projectioné
   END_DESCRIPTION
  
   PARAMETERS
      INPUT
         int ClientID: Client's identifier
      IN/OUT
         double *X: Pointer to X, the x coordinate of the geographic point to convert
         double *Y: Pointer to Y, the y coordinate of the geographic point to convert
  
   END_PARAMETERS
  
   RETURN_VALUE
      int: If 0, the operation is a success. Hense an error code is send. This error code correspond to a position in cln_messages.
  
   END_FUNCTION_INFORMATION
  
   PSEUDO_CODE
  
    1. If the projections are not equal (isProjEqual == FALSE)
    Begin

       1.1. Apply the conter-rotation if the target_azimuth is different of 0.
       The equations are x1 = x*cos(target_azimuth) + y*sin(target_azimuth)
       and y1 = y*cos(target_azimuth) - x*sin(target_azimuth). 
 
       1.2. If the target projection is not longlat
       Begin
           1.2.1. Convert the point using pj_inv. If an error occur,
           return an error.
       End
       1.3. Else
       Begin
           1.3.1. Check the range of the point. If outside, return an error
       End
  
       At this point, the coordinate is in longlat
       
       1.4. Convert the point nad with nad_converter if the link exist.
       
       1.5. If the target projection is not longlat
       Begin
           1.5.1. Convert the point using pj_fwd. If an error occur,
           return an error.
       End
       1.6. Else
       Begin
           1.6.1. Check the range of the point. If outside, return an error
       End
    End
  
  ----------------------------------------------------------------------
 */

int cln_ConvTtoS(ClientID, X, Y)
     int ClientID;
     double *X;
     double *Y;
{
  projUV data1,data2;
  register ecs_Client *cln;
  double temp_x,temp_y,temp1_x,temp1_y;

  cln = soc[ClientID];

  if (cln == NULL)
    return 2;
  
  if (cln->isProjEqual == FALSE) {

    /*
      Apply the conter-rotation
      */
    
    temp_x = *X;
    temp_y = *Y;

    if (cln->target_azimuth != 0.0) {
      temp1_x = ((temp_x * cln->cosazimuth) - 
		 temp_y * cln->sinazimuth);
      temp1_y = ((temp_x * cln->sinazimuth) + 
		 temp_y * cln->cosazimuth);
      temp_x = temp1_x;
      temp_y = temp1_y;
    }

    /*
      Projection change
      */

    if (cln->isTargetLL == FALSE) {
      data1.u = temp_x;
      data1.v = temp_y;
      data2 = pj_inv(data1,cln->target);
      if ((data2.u == HUGE_VAL) || (data2.v == HUGE_VAL)) 
	return 8;
    } else {
      if (temp_y <= -90.0)
	temp_y = -89.9999;
      if (temp_y >= 90.0)
	temp_y = 89.9999;
      if (temp_x <= -180.0)
	temp_x = -179.9999;
      if (temp_x >= 180.0)
	temp_x = 179.9999;
      data2.u = temp_x * DEG_TO_RAD;
      data2.v = temp_y * DEG_TO_RAD;
    }

    if (cln->dthandle != NULL) {
      temp_x = data2.u * RAD_TO_DEG;
      temp_y = data2.v * RAD_TO_DEG;
      if (cln->targetdatum == nad27 && cln->sourcedatum == nad83) {
	(cln->nad_forward)(cln->privdatuminfo,&temp_x,&temp_y);
      }
      if (cln->targetdatum == nad83 && cln->sourcedatum == nad27) {
	(cln->nad_reverse)(cln->privdatuminfo,&temp_x,&temp_y);
      }
      data2.u = temp_x * DEG_TO_RAD;
      data2.v = temp_y * DEG_TO_RAD;
    }

    if (cln->isSourceLL == FALSE) {
      data1 = pj_fwd(data2,cln->source);
      if (data1.u == HUGE_VAL || data1.v == HUGE_VAL) {
	if (data2.v >= 1.5707)
	  data2.v = 1.5707;
	if (data2.v <= -1.5707)
	  data2.v = -1.5707;
	data1 = pj_fwd(data2,cln->source);
	if (data1.u == HUGE_VAL || data1.v == HUGE_VAL) {
	  return 8;
	}
      }
      *X = data1.u;
      *Y = data1.v;
    } else {
      *X = data2.u * RAD_TO_DEG;
      *Y = data2.v * RAD_TO_DEG;
      if (*X < -180.0) 
	*X = -180.0;
      if (*X > 180.0) 
	*X = 180.0;
      if (*Y < -90.0)
	*Y = -90.0;
      if (*Y > 90.0)
	*Y = 90.0;
    }
  }
  return 0;
}


/*
  ----------------------------------------------------------------------
  
   FUNCTION_INFORMATION
  
   NAME
      cln_ConvStoT
      
   DESCRIPTION
      Convert a geographical point from the source geographic
      projection (client) to the target projection.
   END_DESCRIPTION
  
   PARAMETERS
      INPUT
         int ClientID: Client's identifier
      IN/OUT
         double *X: Pointer to X, the x coordinate of the geographic point to convert
         double *Y: Pointer to Y, the y coordinate of the geographic point to convert
  
   END_PARAMETERS
  
    RETURN_VALUE
      int: If 0, the operation is a success. Hense an error code is send. This error code correspond to a position in cln_messages.
  
   END_FUNCTION_INFORMATION
  
   PSEUDO_CODE
  
    1. If the projections are not equal (isProjEqual == FALSE)
    Begin
       1.1. If the source projection is not longlat
       Begin
           1.1.1. Convert the point using pj_inv. If an error occur,
           return an error.
       End
       1.2. Else
       Begin
           1.2.1. Check the range of the point. If outside, return an error
       End
  
       At this point, the coordinate is in longlat
        
       1.3. Convert the point nad with nad_converter if the link exist.
       
       1.4. If the source projection is not longlat
       Begin
           1.4.1. Convert the point using pj_fwd. If an error occur,
           return an error.
       End
       1.5. Else
       Begin
           1.5.1. Check the range of the point. If outside, return an error
       End

       1.6. Apply the rotation if the cln->target_azimuth is different of 0.
       The equations are x1 = x*cos(target_azimuth) - y*sin(target_azimuth)
       and y1 = y*cos(target_azimuth) + x*sin(target_azimuth). Take care also
       of the quadrant where the angle is.
 
    End
  
  ----------------------------------------------------------------------
 */

int cln_ConvStoT(ClientID, X, Y)
     int ClientID;
     double *X;
     double *Y;
{
  projUV data1,data2;
  register ecs_Client *cln;
  double temp_x,temp_y,temp1_x,temp1_y;

  cln = soc[ClientID];
  
  if (cln == NULL)
    return 2;

  if (cln->isProjEqual == FALSE) {

    /*
      Projection change
      */

    if (cln->isSourceLL == FALSE) {
      data1.u = *X;
      data1.v = *Y;
      data2 = pj_inv(data1,cln->source);
      if (data2.u == HUGE_VAL || data2.v == HUGE_VAL) 
	return 8;
    } else {
      if (*Y <= -90.0)
	*Y = -89.9999;
      if (*Y >= 90.0)
	*Y = 89.9999;
      if (*X <= -180.0)
	*X = -179.9999;
      if (*X >= 180.0)
	*X = 179.9999;
      data2.u = *X * DEG_TO_RAD;
      data2.v = *Y * DEG_TO_RAD;
    }

    if (cln->dthandle != NULL) {
      temp_x = data2.u * RAD_TO_DEG;
      temp_y = data2.v * RAD_TO_DEG;
      if (cln->targetdatum == nad27 && cln->sourcedatum == nad83) {
	(cln->nad_reverse)(cln->privdatuminfo,&temp_x,&temp_y);
      }
      if (cln->targetdatum == nad83 && cln->sourcedatum == nad27) {
	(cln->nad_forward)(cln->privdatuminfo,&temp_x,&temp_y);
      }
      data2.u = temp_x * DEG_TO_RAD;
      data2.v = temp_y * DEG_TO_RAD;
    }

    if (cln->isTargetLL == FALSE) {
      data1 = pj_fwd(data2,cln->target);
      if (data1.u == HUGE_VAL || data1.v == HUGE_VAL) {
	if (data2.v >= 1.5707)
	  data2.v = 1.5707;
	if (data2.v <= -1.5707)
	  data2.v = -1.5707;
	data1 = pj_fwd(data2,cln->target);
	if (data1.u == HUGE_VAL || data1.v == HUGE_VAL) {
	  return 8;
	}
      }
      *X = data1.u;
      *Y = data1.v;
    } else {
      *X = data2.u * RAD_TO_DEG;
      *Y = data2.v * RAD_TO_DEG;
      if (*X < -180.0) 
	*X = -180.0;
      if (*X > 180.0) 
	*X = 180.0;
      if (*Y < -90.0)
	*Y = -90.0;
      if (*Y > 90.0)
	*Y = 90.0;
    }

    /*
      Apply the conter-rotation
      */
    
    temp_x = *X;
    temp_y = *Y;

    if (cln->target_azimuth != 0.0) {
      temp1_x = (temp_x * cln->cosazimuth + 
		 temp_y * cln->sinazimuth);
      temp1_y = (temp_y * cln->cosazimuth - 
		 temp_x * cln->sinazimuth);
      *X = temp1_x;
      *Y = temp1_y;
    }


  }
  return 0;
}


/*
 *----------------------------------------------------------------------
 *
 *  cln_ChangeProjection: Change the geographic projection of an object
 *  from source projection to target projection.
 *
 *  IN
 *	int ClientID : Client's identifier
 *	ecs_Object *obj: Pointer to object
 *      
 *  OUT
 *	return int   : error code. If 0, the operation is succes. Else,
 *	an error code correspond to a position in the cln_messages.
 *
 *----------------------------------------------------------------------
 */

int cln_ChangeProjection(ClientID,obj)
     int ClientID;
     ecs_Object *obj;
{ 
  int code;
  register ecs_Client *cln;

  cln = soc[ClientID];

  if (cln->isProjEqual == TRUE)
    return 0;

  /* Change the MBR of the object */

  code = cln_ConvMBR(ClientID,&(obj->xmin),&(obj->ymin),
		     &(obj->xmax),&(obj->ymax),ECS_STOT);
  if (code != 0)
    return code;

  /* Change the geom attribute */

  switch (obj->geom.family) {
  case Area:
    code = cln_ChangeProjectionArea(ClientID,
				    &(obj->geom.ecs_Geometry_u.area));
    break;
  case Line:
    code = cln_ChangeProjectionLine(ClientID,
				    &(obj->geom.ecs_Geometry_u.line));
    break;
  case Point:
    code = cln_ChangeProjectionPoint(ClientID,
				     &(obj->geom.ecs_Geometry_u.point));
    break;
  case Text:
    code = cln_ChangeProjectionText(ClientID,
				    &(obj->geom.ecs_Geometry_u.text));
    break;
  case Matrix:
    code = cln_ChangeProjectionMatrix(ClientID,
				      &(obj->geom.ecs_Geometry_u.matrix));
    break;
  case Image:
    code = cln_ChangeProjectionImage(ClientID,
				     &(obj->geom.ecs_Geometry_u.image));
    break;
  default:
    break;
  }
  return code;
}


/*
 *----------------------------------------------------------------------
 *
 *  cln_ChangeProjectionArea: Change the geographic projection of an area
 *  from source projection to target projection.
 *
 *  IN
 *      int ClientID : Client's identifier
 *      ecs_Area *obj: Pointer to area object
 *      
 *  OUT
 *      return int   : error code. If 0, the operation is success. Else,
 *      an error code correspond to a position in the cln_messages.
 *
 *----------------------------------------------------------------------
 */

int cln_ChangeProjectionArea(ClientID,obj)
     int ClientID;
     ecs_Area *obj;
{ 
  register int i,j;
  int code;

  for(i=0;(int) i<(int) obj->ring.ring_len; i++) {
    if ((code = cln_ConvStoT(ClientID, &(obj->ring.ring_val[i].centroid.x), 
			     &(obj->ring.ring_val[i].centroid.y))) != 0) {
      return code;
    }  

    for(j=0; (int) j< (int) obj->ring.ring_val[i].c.c_len; j++) {
      if ((code = cln_ConvStoT(ClientID, 
			       &(obj->ring.ring_val[i].c.c_val[j].x), 
			       &(obj->ring.ring_val[i].c.c_val[j].y))) != 0) {
	return code;
      }  
    }    
  }

  return 0;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_ChangeProjectionLine: Change the geographic projection of an line
 * from source projection to target projection.
 *
 * IN
 *	int ClientID : Client's identifier
 *	ecs_Line *obj: Pointer to line object
 *      
 * OUT
 *	return int   : error code. If 0, the operation is success. Else,
 *	an error code correspond to a position in the cln_messages.
 *
 *----------------------------------------------------------------------
 */

int cln_ChangeProjectionLine(ClientID,obj)
     int ClientID;
     ecs_Line *obj;
{ 
  register int i;
  int code;

  for(i=0;(int) i<(int) obj->c.c_len; i++) {
    if ((code = cln_ConvStoT(ClientID, 
			     &(obj->c.c_val[i].x), 
			     &(obj->c.c_val[i].y))) != 0) {
      return code;
    }  
  }

  return 0;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_ChangeProjectionPoint: Change the geographic projection of an point
 * from source projection to target projection.
 *
 * IN
 *	int ClientID : Client's identifier
 *	ecs_Point *obj: Pointer to point object
 *      
 *  OUT
 *	return int   : error code. If 0, the operation is success. Else,
 *	an error code correspond to a position in the cln_messages.
 *
 *----------------------------------------------------------------------
 */

int cln_ChangeProjectionPoint(ClientID,obj)
     int ClientID;
     ecs_Point *obj;
{ 
  int code;

  if ((code = cln_ConvStoT(ClientID,&(obj->c.x),&(obj->c.y))) != 0) {
    return code;
  }  

  return 0;
}


/*
 *----------------------------------------------------------------------
 *
 * cln_ChangeProjectionText: Change the geographic projection of an text
 * from source projection to target projection.
 *
 * IN
 *	int ClientID : Client's identifier
 *	ecs_Text *obj: Pointer to text object
 *      
 * OUT
 *	return int   : error code. If 0, the operation is success. Else,
 *	an error code correspond to a position in the cln_messages.
 *
 *----------------------------------------------------------------------
 */

int cln_ChangeProjectionText(ClientID,obj)
     int ClientID;
     ecs_Text *obj;
{ 
  int code;

  if ((code = cln_ConvStoT(ClientID,&(obj->c.x),&(obj->c.y))) != 0) {
    return code;
  }  

  return 0;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_ChangeProjectionMatrix: Change the geographic projection of an matrix
 * from source projection to target projection.
 *
 * IN
 *	int ClientID : Client's identifier
 *	ecs_Matrix *obj: Pointer to matrix object
 *      
 * OUT
 *	return int   : error code. If 0, the operation is success. Else,
 *	an error code correspond to a position in the cln_messages.
 *
 *----------------------------------------------------------------------
 */

int cln_ChangeProjectionMatrix(ClientID,obj)
     int ClientID;
     ecs_Matrix *obj;
{ 
  ClientID = 0;
  obj = NULL;
  return 0;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_ChangeProjectionImage: Change the geographic projection of an image
 * from source projection to target projection.
 *
 * IN
 *	int ClientID : Client's identifier
 *	ecs_Image *obj: Pointer to image object
 *      
 * OUT
 *	return int   : error code. If 0, the operation is success. Else,
 *	an error code correspond to a position in the cln_messages.
 *
 *----------------------------------------------------------------------
 */

int cln_ChangeProjectionImage(ClientID,obj)
     int ClientID;
     ecs_Image *obj;
{ 
  ClientID = 0;
  obj = NULL;
  return 0;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_PointValid: With a point (X,Y), check if this point, when converted
 * from target to source projection, give a equivalent result.
 *
 * IN
 *	int ClientID : Client's identifier
 *	double X: Pointer to X, the x coordinate to convert
 *	double Y: Pointer to Y, the y coordinate to convert
 *      
 * OUT
 *	return int   : If TRUE, this point is equivalent in both projection
 *
 *----------------------------------------------------------------------
 */

int cln_PointValid(ClientID, x, y)
     int ClientID;
     double x;
     double y;
{
  double ox,oy;        /* Original value */
  double cx,cy;        /* Converted value */
  double dx,dy;        /* Difference between points */
  register ecs_Client *cln;

  cln = soc[ClientID];
  if (cln == NULL)
    return FALSE;

  ox = x;
  oy = y;
  cx = x;
  cy = y;
  if (cln_ConvStoT(ClientID, &cx, &cy) != 0) {
    return FALSE;
  }
  dx = ox-cx;
  if (dx<0) 
    dx*=-1;
  if (ox<0)
    ox*=-1;
  dy = oy-cy;
  if (dy<0) 
    dy*=-1;
  if (oy<0)
    oy*=-1;

  if (cln->dthandle == NULL) {
    if (((dx/ox) > COMPARETOLERANCE) || ((dy/oy) > COMPARETOLERANCE)) {
      return FALSE;
    } 
  } else {
    return FALSE;
  }

  return TRUE;
}


/*
 *----------------------------------------------------------------------
 *
 * cln_GetClientIdFromURL: With an URL, return the related ClientID.
 *
 * IN
 *	char *url: Client URL
 *      
 * OUT
 *	return int: Client ID. If >= 0, the operation is a success.
 *			       If < 0 , it failed to found the client
 *
 *----------------------------------------------------------------------
 */

int cln_GetClientIdFromURL(url)
     char *url;
{ 
  int i;

  for(i=0;i<MAXCLIENT;i++) {
    if (soc[i] != NULL) {
      if (strcmp(soc[i]->url,url) == 0) {
	return i;
      }
    }
  }
  return -1;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_GetURLList: Get URL list
 *
 * OUT
 *	char **urllist: Pointer to a string (char *)
 *	return int: error_code flag. TRUE: Success
 *                                   FALSE: Failure
 *
 *----------------------------------------------------------------------
 */

int cln_GetURLList(urllist)
     char **urllist;
{ 
  static char *retstring = NULL;
  int i;
  int lenght;

  *urllist = NULL;

  if (retstring != NULL)
    free(retstring);
  retstring = NULL;
  lenght = 0;

  retstring = (char *) malloc(1);
  if (retstring == NULL)
    return FALSE;
  strcpy(retstring,"");

  for(i=0;i<MAXCLIENT;i++) {
    if (soc[i] != NULL) {

      /*
       * Add a space
       */

      if (lenght>0)
	strcat(retstring," ");

      /* 
       * Concatenate url to retstring 
       */

      lenght += strlen(soc[i]->url) + 2;
      retstring = (char *) realloc(retstring,lenght);
      if (retstring == NULL)
	return FALSE;
      strcat(retstring,soc[i]->url);
    }
  }

  *urllist = retstring;
  return TRUE;
}


/*
 *----------------------------------------------------------------------
 *
 * cln_SetRegionCaches: Set the geographic region occupied
 * by caches.
 *
 * IN
 *	int ClientID: Identifiant of client
 *	ecs_Region *GR: Geographic region in the 
 *
 *  OUT
 *	char **error_message: Pointer to a string with a error message
 *	return int: Error message returned.
 *		    TRUE: Success
 *		    FALSE: Failure
 *
 *----------------------------------------------------------------------
 */

int cln_SetRegionCaches(ClientID, GR,error_message)
     int ClientID;
     ecs_Region *GR;
     char **error_message;
{
  register ecs_Client *cln;

  *error_message = NULL;

  cln = soc[ClientID];
  if (cln == NULL) {
    *error_message = cln_messages[2];
    return FALSE;
  }

  cln->cacheRegion.north = GR->north;
  cln->cacheRegion.south = GR->south;
  cln->cacheRegion.east = GR->east;
  cln->cacheRegion.west = GR->west;
  cln->cacheRegion.ns_res = GR->ns_res;
  cln->cacheRegion.ew_res = GR->ew_res;

  return TRUE;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_LoadCache: Create a new cache and assign to it the
 * objects requested in a layer selection, in the region
 * given by cln->cacheRegion.
 *
 * IN
 *	int ClientID: Identifiant of client
 *	ecs_LayerSelection *ls: Layer selection to assign
 *	to this server
 *
 *  OUT
 *	char **error_message: Error message to return to client.
 *	return int: Error message returned.
 *		    TRUE: Success
 *		    FALSE: Failure
 *
 *----------------------------------------------------------------------
 */

int cln_LoadCache(ClientID, ls, error_message)
     int ClientID;
     ecs_LayerSelection *ls;
     char **error_message;
{
  ecs_Cache *NewCache;
  ecs_Cache *CachePtr;
  ecs_Result *obj;
  register ecs_Client *cln;
  ecs_Result *res;
  int code;
  int n;

  *error_message = NULL;

  cln = soc[ClientID];
  if (cln == NULL) {
    *error_message = cln_messages[2];
    return FALSE;
  }

  /*
   * Check if the type is vectorial. If not, return an error message.
   */

  if (ls->F != Area && ls->F != Line && ls->F != Point && ls->F != Text) {
    *error_message = cln_messages[12];
    return FALSE;    
  }

  /*  
   * Check if the cache already exist in the cache.
   */

  CachePtr = cln->cache;
  while(CachePtr!=NULL) {
    if ((strcmp(CachePtr->coverage.Select,ls->Select) == 0) && 
	(CachePtr->coverage.F == ls->F)) {
      return TRUE;
    }
    CachePtr = CachePtr->next;
  }

  /*
   * If the cache don't exist, create a new one. First start a selection
   * with a certain layer.
   */

  res = cln_SelectLayer(ClientID, ls);
  if (res->error != 0) {
    *error_message = res->message;
    return FALSE;
  }

  res = cln_SelectRegion(ClientID, &(cln->cacheRegion));
  if (res->error != 0) {
    *error_message = res->message;
    return FALSE;
  }

  NewCache = cln_NewCache(CACHEINITSIZE);
  if (NewCache==NULL) {
    *error_message = cln_messages[1];
    return FALSE;
  }

  /* 
   * Allocate coverage 
   */

  NewCache->coverage.Select = (char *) malloc(strlen(ls->Select)+1);
  if (NewCache->coverage.Select == NULL) {
    cln_FreeCache(NewCache);
    *error_message = cln_messages[1];
    return FALSE;    
  }
  strcpy(NewCache->coverage.Select,ls->Select);
  NewCache->coverage.F = ls->F;

  /* 
   * Add to object cache table all the selected objects.
   * MultiResult responses have to be dealt with a little differently.
   * Only a single error is returned for a MultiResult, and that error
   * pertains only to the a request after the last object in the MultiResult
   * array.  All objects in the array were retrieved without an error.
   * When the server encounters an error, it sends all accumulated results
   * so far and sets the error condition.
   */

  res = svr_GetNextObject(&(cln->s));
  while((res->error == 0) ||
	(res->res.type == MultiResult && res->res.ecs_ResultUnion_u.results.results_len > 0)) {

    n = (res->res.type == MultiResult) ? res->res.ecs_ResultUnion_u.results.results_len : 1;
    if (NewCache->size+n >= NewCache->allocatedSize) {
      NewCache->allocatedSize += (CACHEINITSIZE >= n) ? CACHEINITSIZE : n;
      NewCache->o = (ecs_Result **)
	realloc(NewCache->o,sizeof(ecs_Result *)*(NewCache->allocatedSize));
      if(NewCache == NULL) {
	*error_message = cln_messages[1];
	return FALSE;
      }
    }
    if (res->res.type != MultiResult) {
      /* 
       * Make a copy of the object
       */
      code = ecs_CopyResult(res,&obj);
      if (code == FALSE) {
	cln_FreeCache(NewCache);
	return FALSE;    
      }
      NewCache->o[NewCache->size++] = obj;
    } else {
      /*
       * Break MultiResult object apart
       */
      for (n = 0; n < (int) res->res.ecs_ResultUnion_u.results.results_len; n++) {
	code = ecs_CopyResultFromUnion(&res->res,&obj);
	if (code == FALSE) {
	  cln_FreeCache(NewCache);
	  return FALSE;    
	}
	NewCache->o[NewCache->size++] = obj;
      }
      if (res->error) {
	break;
      }
    }
    res = svr_GetNextObject(&(cln->s));
  }
  
  /* Ajouter la nouvelle cache dans la liste */

  if (cln->cache != NULL) {
    cln->cache->previous = NewCache;
    NewCache->next = cln->cache;
    cln->cache = NewCache;
  } else {
    cln->cache = NewCache;
  }

  return TRUE;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_ReleaseCache: Destroy the cache related to a coverage
 *
 * IN
 *	int ClientID: Identifiant of client
 *	ecs_LayerSelection *ls: Layer selection to assign
 *	to this server
 *
 *  OUT
 *	char **error_message: Error message to return to client.
 *	return int: Error message returned.
 *		    TRUE: Success
 *		    FALSE: Failure
 *
 *----------------------------------------------------------------------
 */

int cln_ReleaseCache(ClientID, ls, error_message)
     int ClientID;
     ecs_LayerSelection *ls;
     char **error_message;
{
  ecs_Cache *CachePtr;  
  register ecs_Client *cln;

  *error_message = NULL;

  cln = soc[ClientID];
  if (cln == NULL) {
    *error_message = cln_messages[2];
    return FALSE;
  }

  cln->selectCache = NULL;
  CachePtr = cln->cache;
  while(CachePtr!=NULL) {
    if ((strcmp(CachePtr->coverage.Select,ls->Select) == 0) && 
	CachePtr->coverage.F == ls->F) {
      if (cln->cache == CachePtr) {
	cln->cache = CachePtr->next;
	if (cln->cache != NULL) 
	  cln->cache->previous = NULL;
      } else {
	if(CachePtr->next!=NULL) 
	  CachePtr->next->previous = CachePtr->previous;
	if(CachePtr->previous!=NULL) 
	  CachePtr->previous->next = CachePtr->next;
      }
      cln_FreeCache(CachePtr);
      return TRUE;
    }
    
    CachePtr = CachePtr->next;
  }

  *error_message = cln_messages[6];
  return FALSE;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_NewCache: Allocate a new cache structure
 *
 * OUT
 *	return : NULL if no memory available, a cache otherwise
 *
 *----------------------------------------------------------------------
 */
ecs_Cache *
cln_NewCache(int size)
{
  ecs_Cache *cache;

  cache = (ecs_Cache *) malloc(sizeof(ecs_Cache));
  if (cache==NULL) {
    return NULL;
  }

  cache->coverage.Select = NULL;
  cache->o = NULL;
  cache->startpos = 0;
  cache->currentpos = 0;
  cache->size = 0;
  cache->next = NULL;
  cache->previous = NULL;

  /* 
   * Allocate object cache table 
   */

  cache->allocatedSize = size;
  cache->o = (ecs_Result **) malloc(sizeof(ecs_Result *) * size);
  if (cache->o == NULL) {
    free(cache);
    return NULL;
  }

  return cache;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_FreeCache: Free the cache memory
 *
 * IN
 *	ecs_Cache *Cache: Cache to be free
 *
 *----------------------------------------------------------------------
 */
 
void cln_FreeCache(Cache)
     ecs_Cache *Cache;  
{
  int i;

  if (Cache != NULL) {
    if (Cache->coverage.Select != NULL) 
      free(Cache->coverage.Select);
    if (Cache->o != NULL) {
      for(i=0;i<Cache->size;i++) {
	if (Cache->o[i] != NULL) {
	  if (Cache->o[i]->res.type == Object) {
	    ecs_FreeObject(&(Cache->o[i]->res.ecs_ResultUnion_u.dob));
	  }
	  free(Cache->o[i]);
	  Cache->o[i] = NULL;
	}
      }
      free(Cache->o);
    }
    free(Cache);  
  }
  
  return;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_FoundCache: Search the cache list for a cache who correspond to
 * a layer.
 *
 * IN
 *	int ClientID: Identifiant of client
 *	ecs_LayerSelection *ls: Layer information structure
 *
 *  OUT
 *	return ecs_Cache *: A pointer to a cache. If NULL, the cache was
 *	not found.
 *
 *----------------------------------------------------------------------
 */

ecs_Cache *cln_FoundCache(ClientID, ls)
     int ClientID;
     ecs_LayerSelection *ls;
{
  ecs_Cache *CachePtr,*FoundCache;  
  register ecs_Client *cln;

  cln = soc[ClientID];
  if (cln == NULL)
    return NULL;

  FoundCache = NULL;
  CachePtr = cln->cache;

  while(CachePtr!=NULL) {
    if ((strcmp(CachePtr->coverage.Select,ls->Select) == 0) && 
	(CachePtr->coverage.F == ls->F)) {
      FoundCache = CachePtr;
      break;
    }
    
    CachePtr = CachePtr->next;
  }

  return FoundCache;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_CalcCtlPoint: For a given point in the client region, 
 * calculate the corresponding point in the server region.
 *
 * IN
 *	int ClientID: Identifiant of client
 *	ecs_Region *server_region: Server region
 *	int SI, SJ: Control point in T matrix
 *
 * OUT
 *	ecs_CtlPoint *pt: Control point
 *	return int: Error message returned.
 *		     TRUE: Success
 *		     FALSE: Failure
 *
 *----------------------------------------------------------------------
 */

int cln_CalcCtlPoint(ClientID,server_region,SI,SJ,pt)
     int ClientID;
     ecs_Region *server_region;
     int SI;
     int SJ;
     ecs_CtlPoint *pt;
{
  double X,Y;
  register ecs_Client *cln;
  int error_code;

  cln = soc[ClientID];
  if (cln == NULL) {
    return FALSE;
  }

  /* Found the geographic position of SI,SJ in source */

  X = cln->currentRegion.west + SI * cln->currentRegion.ew_res;
  Y = cln->currentRegion.north - SJ * cln->currentRegion.ns_res;
  
  /* Convert this point in server projection */

  if ((error_code = cln_ConvTtoS(ClientID, &X, &Y)) != 0) {
    return FALSE;        
  }

  /* Found the corresponding position of this point in server region */

  pt->e1 = (double) SI;
  pt->n1 = (double) SJ;
  pt->e2 = (X - server_region->west) / server_region->ew_res;
  pt->n2 = (server_region->north - Y) / server_region->ns_res;
  return TRUE;
}




/*
 *----------------------------------------------------------------------
 *
 * cln_CalcCtlPoints: For a given region, calculate control points
 * for matrix transformation.
 *
 * IN
 *	int ClientID: Identifiant of client
 *
 * OUT
 *	ecs_CalcCtlPoints **pts: new structure with control points
 *	char **error_message: error message
 *	return int: Error message returned.
 *		    TRUE: Success
 *		    FALSE: Failure
 *
 *----------------------------------------------------------------------
 */

int cln_CalcCtlPoints(ClientID,pts,error_message)
     int ClientID;
     ecs_CtlPoints **pts;
     char **error_message;
{
  static ecs_CtlPoints *newpts = NULL;
  register ecs_Client *cln;
  int width, height;
  ecs_Region server_region;

  *error_message = NULL;

  cln = soc[ClientID];
  if (cln == NULL) {
    *error_message = cln_messages[2];
    *pts = NULL;
    return FALSE;
  }

  /* Prepare newpts structure */

  if (newpts != NULL) {
    if (newpts->pts != NULL) {
      free(newpts->pts);
      newpts->pts = NULL;
    }
    free(newpts);
    newpts = NULL;
  }

  newpts = (ecs_CtlPoints *) malloc(sizeof(ecs_CtlPoints));
  if (newpts == NULL) {
    *error_message = cln_messages[1];
    *pts = NULL;
    return FALSE;   
  }

  newpts->nbpts = 9;
  newpts->pts = (ecs_CtlPoint *) malloc(sizeof(ecs_CtlPoint)*9);
  if (newpts->pts == NULL) {
    *error_message = cln_messages[1];
    free(newpts);
    newpts = NULL;
    *pts = NULL;
    return FALSE;   
  }

  /* Calculate server region with client value */
  
  server_region.north = cln->currentRegion.north;
  server_region.south = cln->currentRegion.south;
  server_region.east = cln->currentRegion.east;
  server_region.west = cln->currentRegion.west;
  server_region.ns_res = cln->currentRegion.ns_res;
  server_region.ew_res = cln->currentRegion.ew_res;
  cln_ConvRegion(ClientID,&(server_region),ECS_TTOS);

  /* Calculate raster width and height */

  width = (int) (((cln->currentRegion.east - cln->currentRegion.west) / 
		 cln->currentRegion.ew_res) + 0.5);
  height = (int)(((cln->currentRegion.north - cln->currentRegion.south) / 
		  cln->currentRegion.ns_res) + 0.5);

  if( width < 1 )
      width = 1;
  if( height < 1 )
      height = 1;

  /* Assign points and return result */

  if ((cln_CalcCtlPoint(ClientID, &server_region, 0, 0, &(newpts->pts[0]))) &&
      (cln_CalcCtlPoint(ClientID, &server_region, 0, (height / 2), &(newpts->pts[1]))) &&
      (cln_CalcCtlPoint(ClientID, &server_region, 0, height, &(newpts->pts[2]))) &&
      (cln_CalcCtlPoint(ClientID, &server_region, (width / 2), 0, &(newpts->pts[3]))) &&
      (cln_CalcCtlPoint(ClientID, &server_region, (width / 2), (height / 2), &(newpts->pts[4]))) &&
      (cln_CalcCtlPoint(ClientID, &server_region, (width / 2), height, &(newpts->pts[5]))) &&
      (cln_CalcCtlPoint(ClientID, &server_region, width, 0, &(newpts->pts[6]))) &&
      (cln_CalcCtlPoint(ClientID, &server_region, width, (height / 2), &(newpts->pts[7]))) &&
      (cln_CalcCtlPoint(ClientID, &server_region, width, height, &(newpts->pts[8])))) {
    *pts = newpts;
    return TRUE;
  }

  *error_message = cln_messages[8];
  free(newpts->pts);
  free(newpts);
  newpts = NULL;
  *pts = NULL;
  return FALSE;        
}


/*
 *----------------------------------------------------------------------
 *
 * cln_SetRasterConversion: Set the matrix projection conversion.
 *
 * This program computes the planar projective transformation
 * parameters and the corresponding residuals.
 *  
 * The transformation is done from system  x, y (rect.in) to
 * system X, Y (nonrect.in) obtained from the original image.
 *  
 * For the client server extension, these coordinates become:
 *  
 * x : e1	Coordinates of the Client
 * y : n1
 *  
 * X : e2	Coordinates of the Server
 * Y : n2
 *  
 * IN
 *	int ClientID: Identifiant of client
 *	ecs_Resampling resampling: Resampling value
 *	ecs_Transformation trans: Transformation value
 *
 *
 * IN/OUT
 *	ecs_CalcCtlPoints **pts: new structure with control points.
 *	If this structure is NULL, the procedure cln_CalcCtlPoints
 *	will be called. In the other cases, the calculations will
 *	be done with this argument value. It's return the same structure
 *	with errorx and errory set.
 *
 *  OUT
 *	char **error_message: error message
 *	return int: Error message returned.
 *		    TRUE: Success
 *		    FALSE: Failure
 *
 *----------------------------------------------------------------------
 */

int cln_SetRasterConversion(ClientID,pts,resampling,trans,error_message)
     int ClientID;
     ecs_CtlPoints **pts;
     ecs_Resampling resampling;
     ecs_Transformation trans;
     char **error_message;
{
  register ecs_Client *cln;
  ecs_CtlPoints *localpts;
  ecs_RasterConversion rc;
  ecs_Result *result;
  double  **A, **AtA, **Atw, **w, **d, **res;
  int     i, j, k;
  
  *error_message = NULL;

  cln = soc[ClientID];
  if (cln == NULL) {
    *error_message = cln_messages[2];
    *pts = NULL;
    return FALSE;
  }

  /* prepare the ecs_RasterConversion structure */

  rc.coef.coef_len = 8;
  rc.coef.coef_val = (double *) malloc(sizeof(double)*8);
  if (rc.coef.coef_val == NULL) {
    *error_message = cln_messages[1];
    *pts = NULL;
    return FALSE;
  }

  /* step 1: Calculate control points */

  if (*pts == NULL) {
    localpts = NULL;
    if (!cln_CalcCtlPoints(ClientID,&localpts,error_message)) {
      *pts = NULL;
      return FALSE;
    }
  } else {
    localpts = *pts;
  }

  /* step 2: Generate matrix A and vector W for least-squares adjustment */

  A    = mat_malloc(localpts->nbpts*2, 8);
  AtA  = mat_malloc(8, 8);
  Atw  = mat_malloc(8, 1);
  w    = mat_malloc(localpts->nbpts*2, 1);
  d    = mat_malloc(8, 1);
  res  = mat_malloc(localpts->nbpts, 2);

  if ((A == NULL) || (AtA == NULL) || (Atw == NULL) || (w == NULL) || 
      (d == NULL) || (res == NULL)) {
    *error_message = cln_messages[1];
    *pts = NULL;
    free(rc.coef.coef_val);
    return FALSE;   
  }

  /* Form the design matrix A and vector w */

  for(i=0, k=0; i<localpts->nbpts*2; i+=2, k++) {

    /* Matrix inxy(x,y) or e1[k], n1[k] : corrected coordinates (client) */
    /* Matrix inXY(X,Y) or e2[k], n2[k] : uncorrected coordinates (server) */

    j = i+1;

    /* Design Matrix A */

    A[i][0] = localpts->pts[k].e1;
    A[i][1] = localpts->pts[k].n1;
    A[i][2] = 0;
    A[i][3] = 0;
    A[i][4] = -(localpts->pts[k].e1)*localpts->pts[k].e2;
    A[i][5] = -(localpts->pts[k].n1)*localpts->pts[k].e2;
    A[i][6] = 1;
    A[i][7] = 0;
    
    A[j][0] = 0;
    A[j][1] = 0;
    A[j][2] = localpts->pts[k].e1;
    A[j][3] = localpts->pts[k].n1;
    A[j][4] = -(localpts->pts[k].e1)*localpts->pts[k].n2;
    A[j][5] = -(localpts->pts[k].n1)*localpts->pts[k].n2;
    A[j][6] = 0;
    A[j][7] = 1;
    
    /* Vector w */

    w[i][0] = localpts->pts[k].e2;
    w[j][0] = localpts->pts[k].n2;
  }

  /* step 3: Matrix resolution and parameters generation */

  /* Compute the parameters using a least-squares adjustment */
  /* d = (AtA)-1 * Atw */

  mat_mul_transposed(A,localpts->nbpts*2,8,A,localpts->nbpts*2,8,AtA);
  
  mat_inverse(AtA, 8);
  
  mat_mul_transposed(A,localpts->nbpts*2,8,w,localpts->nbpts*2,1,Atw);
  
  mat_mul_direct(AtA,8,8,Atw,8,1,d);

  /* Compute the residuals */

  for(i=0; i<localpts->nbpts; i++) {
    localpts->pts[i].errorx = ((d[0][0]*localpts->pts[i].e1+d[1][0]*localpts->pts[i].n1+d[6][0])/(d[4][0]*localpts->pts[i].e1+d[5][0]*localpts->pts[i].n1+1)) - localpts->pts[i].e2;
    
    localpts->pts[i].errory = ((d[2][0]*localpts->pts[i].e1+d[3][0]*localpts->pts[i].n1+d[7][0])/(d[4][0]*localpts->pts[i].e1+d[5][0]*localpts->pts[i].n1+1)) - localpts->pts[i].n2;
  }
  
  /* Puts information in ecs_RasterConversion */

  if(cln->isTargetLL == TRUE &&
     cln->isSourceLL == TRUE)
    rc.isProjEqual = TRUE;
  else
    rc.isProjEqual = cln->isProjEqual;
  rc.r_method = resampling;
  rc.t_method = trans;
  rc.coef.coef_len = 8;
  for (i=0; i<8; i++) {
    rc.coef.coef_val[i] = d[i][0];
  }  

  /* Call the server */

  result = svr_SetRasterConversion(&(cln->s),&rc);
  if (result->error != 0) {
    *error_message = result->message;
    *pts = NULL;
    mat_free(A,localpts->nbpts*2);
    mat_free(AtA, 8);
    mat_free(Atw, 8);
    mat_free(w, localpts->nbpts*2);
    mat_free(d, 8);
    mat_free(res, localpts->nbpts);  
    free(rc.coef.coef_val);
    return FALSE;
  }
  mat_free(A,localpts->nbpts*2);
  mat_free(AtA, 8);
  mat_free(Atw, 8);
  mat_free(w, localpts->nbpts*2);
  mat_free(d, 8);
  mat_free(res, localpts->nbpts);  
  free(rc.coef.coef_val);
  *pts = localpts;
  return TRUE;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_SetCompression: Set the server compression parameters
 *
 * IN 
 *	int ClientID: Client identifier
 *	ecs_Compression: Compression parameters
 *      
 * OUT
 *	return ecs_Result: Operation result
 *
 *----------------------------------------------------------------------
 */

ecs_Result *cln_SetCompression(ClientID,compression)
     int ClientID;
     ecs_Compression *compression;
{
  register ecs_Client *cln;
  ecs_Result *msg;

  if (multiblock != 0) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[14]);
    return msg;
  }
  
  cln = soc[ClientID];
  if (cln == NULL) {
    ecs_SetError(&cln_dummy_result,1,cln_messages[2]);
    return &cln_dummy_result;
  }
  msg = svr_SetCompression(&(cln->s),compression);
  return msg;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_LoadCapabilities(): Load and parse capabilities document.
 *
 * IN 
 *	int ClientID: Client identifier
 *	const char *arg: Either "ogdi_capabilities" or 
 *                       "ogdi_server_capabilities".
 *      
 * OUT
 *	return ecs_Result: Operation result (success is ecs_Text or error)
 *
 *----------------------------------------------------------------------
 */

ecs_Result *cln_LoadCapabilities(int ClientID, const char *arg, 
                                 int error_if_unsupported)

{
  ecs_Client *cln;
  ecs_Result *msg, *result;
  char       *cap_doc;

  if (multiblock != 0) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[14]);
    return msg;
  }
  
  cln = soc[ClientID];
  if (cln == NULL) {
    ecs_SetError(&cln_dummy_result,1,cln_messages[2]);
    return &cln_dummy_result;
  }

  /*
   * Don't do anything, if these capabilities already loaded.  Eventually
   * we should allow re-loading of capabilities document in case the 
   * application wants to test error reports, or force a refresh.
   */
  if( (cln->have_server_capabilities 
       && strcmp(arg,"ogdi_server_capabilities") == 0)
      || (cln->have_capabilities 
          && strcmp(arg,"ogdi_capabilities") == 0) )
  {
      msg = &cln_dummy_result;
      ecs_SetText(msg,"");
      ecs_SetSuccess(msg);
      return msg;
  }

  /*
   * Load the requested capabilities document. 
   */
  result = cln_UpdateDictionary(ClientID, (char *) arg);
  if( ECSERROR(result) && error_if_unsupported )
      return result;

  /*
   * Did we get an error, or is this clearly not a capabilities document?  
   * If not, apply a default configuration for pre 3.1 compatibility. 
   */
  if( ECSERROR(result) 
      || strncmp(ECSTEXT(result),"<?xml",5) != 0 
      || strstr(ECSTEXT(result),"OGDI_Capabilities") == NULL )
  {
      if( error_if_unsupported )
      {
          assert( result == &cln_dummy_result );
          if( !ECSERROR(result) )
          {
              char	error[1024];

              sprintf( error, 
                       "Return value of cln_UpdateDictionary(%s) is clearly "
                       "not an OGDI_Capabilities result.", 
                       arg );
              ecs_SetError(&cln_dummy_result,1, error );
          }

          return &cln_dummy_result;
      }

      ecs_SetText(&cln_dummy_result,"");
      ecs_SetSuccess(&cln_dummy_result);
      
      cln->have_server_capabilities = TRUE;
      strcpy( cln->server_version_str, "3.0" );
      cln->server_version = 3000;
      
      cln->have_capabilities = TRUE;

      return &cln_dummy_result;
  }

  /*
   * Make a copy of the document so nothing weird happens as our static
   * result object is modified during parsing.
   */
  cap_doc = _strdup(ECSTEXT(result));
  if (cap_doc == NULL) {
      ecs_SetError(&cln_dummy_result,1,cln_messages[1]);
      return &cln_dummy_result;
  }

  /*
   * Parse it. 
   */
  ecs_SetSuccess( &cln_dummy_result );
  ecs_ParseCapabilities( cln, cap_doc, &cln_dummy_result );

  free( cap_doc );

  return &cln_dummy_result;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_GetVersion(): Return the driver version string.
 *
 * IN 
 *	int ClientID: Client identifier
 *      
 * OUT
 *	return ecs_Result: Operation result (ecs_Text with version string)
 *
 *----------------------------------------------------------------------
 */

ecs_Result *cln_GetVersion(int ClientID)

{
  ecs_Client *cln;
  ecs_Result *msg;

  if (multiblock != 0) {
    msg = &cln_dummy_result;
    ecs_SetError(msg,1,cln_messages[14]);
    return msg;
  }
  
  cln = soc[ClientID];
  if (cln == NULL) {
    ecs_SetError(&cln_dummy_result,1,cln_messages[2]);
    return &cln_dummy_result;
  }

  cln_LoadCapabilities(ClientID, "ogdi_server_capabilities", 0 );

  msg = &cln_dummy_result;
  ecs_SetText(msg,cln->server_version_str);
  ecs_SetSuccess(msg);

  return msg;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_CheckExtension(): Test if requested extension is enabled.
 *
 * IN 
 *	int ClientID: Client identifier
 *      const char *extension: the name of the extension to test for.
 *      const char *layer_name: the name of the layer to test on or NULL.
 *      
 * OUT
 *	return ecs_Result: Operation result (ecs_Text with version string)
 *
 *----------------------------------------------------------------------
 */

int cln_CheckExtension(int ClientID, const char *extension,
                       const char *layer_name )

{
  ecs_Client *cln;
  ecs_Result *result;
  char	**extensions;
  int         i, layer;

  if (multiblock != 0) {
    return FALSE;
  }
  
  cln = soc[ClientID];
  if (cln == NULL) {
    return FALSE;
  }

  if( layer_name == NULL )
      result = cln_LoadCapabilities(ClientID, "ogdi_server_capabilities", 0 );
  else
      result = cln_LoadCapabilities(ClientID, "ogdi_capabilities", 0 );

  if( ECSERROR(result) )
      return FALSE;
  
  /*
   * Test in global extensions. 
   */
  
  extensions = cln->global_extensions;
  for( i = 0; extensions != NULL && extensions[i] != NULL; i++ )
  {
      if( strcmp(extensions[i],extension) == 0 )
          return TRUE;
  }

  if( layer_name == NULL )
      return FALSE;

  /* 
   * Test within layer.
   */
  for( layer = 0; layer < cln->layer_cap_count; layer++ )
  {
      if( strcmp(layer_name, cln->layer_cap[layer]->name) == 0 )
      {
          extensions = cln->layer_cap[layer]->extensions; 
          for( i = 0; extensions != NULL && extensions[i] != NULL; i++ )
          {
              if( strcmp(extensions[i],extension) == 0 )
                  return TRUE;
          }
          
          return FALSE;
      }
  }

  return FALSE;
}

/*
 *----------------------------------------------------------------------
 *
 * cln_GetLayerCapabilities(): Get layer definition.
 *
 * Note that there is no way to find out the number of layers explicitly.
 * Applications are intended to call cln_GetLayerCapabilities() repeatedly
 * with incrementing layer_index values till a NULL is returned.
 *
 * IN 
 *	int ClientID: Client identifier
 *      int layer_index: The zero based index of the layer.
 *      
 * OUT
 *	return ecs_LayerCapabilities of layer or NULL if unavailable.  The
 *             returned pointer is to internal data and should not be 
 *             modified or freed.  It will be invalidated when the client
 *             is destroyed.
 *
 *----------------------------------------------------------------------
 */

const ecs_LayerCapabilities *
cln_GetLayerCapabilities( int ClientID, int layer_index )

{
  ecs_Client *cln;

  if (multiblock != 0) {
    return NULL;
  }
  
  cln = soc[ClientID];
  if (cln == NULL) {
      return NULL;
  }

  if( ECSERROR(cln_LoadCapabilities(ClientID, "ogdi_capabilities", 0 )) )
      return NULL;

  if( layer_index < 0 || layer_index >= cln->layer_cap_count )
      return NULL;
  else
      return cln->layer_cap[layer_index];
}

/*
  cln_BlockOGDI

  Block the ogdi
 */

void cln_BlockOGDI()
{
  multiblock = 1;
}

/*
  cln_UnBlockOGDI

  Block the ogdi
 */

void cln_UnBlockOGDI()
{
  multiblock = 0;
}

#ifdef _WINDOWS
/*
 *----------------------------------------------------------------------
 * DllMain --
 *
 *	When the DLL is being unloaded, make sure to destroy all of
 *	the clients that remain open.
 *
 * OUT
 *	return TRUE
 *----------------------------------------------------------------------
 */
BOOL WINAPI 
DllMain( HINSTANCE  hinstDLL,   // handle of DLL module 
         DWORD  fdwReason,      // reason for calling function 
         LPVOID  lpvReserved)
{
  switch (fdwReason) {
  case DLL_PROCESS_ATTACH:
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
    break;
  case DLL_PROCESS_DETACH:
    {
      int i;
      for(i=0;i<MAXCLIENT;i++) {
	cln_DestroyClient(i);
      }   
    }
  break;
  }
  return TRUE;
}
#endif /* ifdef _WINDOWS */

