#include "ecs.h"

#ifdef _SCO
#include <rpc/rpc.h>
#include <sys/fs/nfs/time.h>
#endif

#ifndef _WINDOWS
#  include <unistd.h>
#endif

struct ecs_Remote {
  CLIENT *handle;
  ecs_Result *result;
};
typedef struct ecs_Remote ecs_Remote;

#define CONTIMEOUT 60
#define DEFTIMEOUT 900
#define MAXCONRETRY 1

#define DISPATCHER 1

#ifdef _WINDOWS
int rpc_IsInit = 0;
#endif

/*
   ------------------------------------------------------------------------
   */

static struct timeval DISPATCH_TIMEOUT = { 20, 0 };


u_long dispatch_1(clnt)
     CLIENT *clnt;
{
  static u_long clnt_res;

  memset((char *)&clnt_res, 0, sizeof (clnt_res));
  if (clnt_call(clnt, DISPATCHER,
		(xdrproc_t) xdr_void, (caddr_t) NULL,
		(xdrproc_t) xdr_u_long, (caddr_t) &clnt_res,
		DISPATCH_TIMEOUT) != RPC_SUCCESS) {
    clnt_res = 0;
  }
  return clnt_res;
}



/*
   ------------------------------------------------------------------------
   */

ecs_Result *dyn_CreateServer(s,Request)
     ecs_Server *s;
     char *Request;
{
  ecs_Remote *rc;
  u_long csid;
  struct timeval timeOut;
  int counter;
  ecs_ProxyCreateServer proxy_req;
  char *proxyhost = getenv("GLTPPROXYHOST");

#ifdef _WINDOWS
  if (!rpc_IsInit) {
    rpc_nt_init();
    rpc_IsInit = 1;
  }
#endif  

  /* 
     Initialisation of ecs_Remote in s->priv 
     */

  s->priv = (void *) malloc(sizeof(ecs_Remote));
  if (s->priv == NULL) {
    ecs_SetError(&(s->result),1,"No enough memory");
    return &(s->result);
  }

  rc = (ecs_Remote *) s->priv;
  rc->handle = NULL;
  rc->result = NULL;

  /* 
     Ask to dispatcher what is the number of the new server 
     */

  /* Try to open the dispatcher */

  for(counter = 0; counter < MAXCONRETRY; counter++) {
    rc->handle = clnt_create(proxyhost ? proxyhost : s->hostname,
			     proxyhost ? ECSPROXYPROG : ECSPROG, 
			     ECSVERS, "tcp");
    if (rc->handle != 0) {
      break;
    }
  }
  
  if (rc->handle == 0) {
    ecs_SetError(&(s->result),1,"Unable to connect to dispatcher gltpd");
    return &(s->result);
  }  

#ifdef _WINDOWS
    Sleep(1000);
#else
    sleep(1);
#endif

  /* Request to dispatcher */

  csid = dispatch_1(rc->handle);  

  if (csid == 0) {
    ecs_SetError(&(s->result),1,"Not answer from the dispatcher");
    return &(s->result);    
  }

  clnt_destroy(rc->handle);

#ifdef _WINDOWS
    Sleep(1000);
#else
    sleep(1);
#endif

  /* Try to connect to server with the number returned by dispatch_1 */

  for(counter = 0; counter < MAXCONRETRY; counter++) {
    rc->handle = clnt_create(proxyhost ? proxyhost : s->hostname,
			     csid, ECSVERS, "tcp");
    if (rc->handle != 0) {
      break;
    }
  }

#ifdef _WINDOWS
    Sleep(1000);
#else
    sleep(1);
#endif

  if (rc->handle == 0) {
    ecs_SetError(&(s->result),1,"Unable to connect to server number given by dispatcher");
    return &(s->result);
  }  

  /* Initialize the timeout of the create client */

  timeOut.tv_sec = CONTIMEOUT;
  timeOut.tv_usec = 0;
  clnt_control(rc->handle, CLSET_TIMEOUT, (char *) &timeOut);

  /* Call the server creation method */

  if (proxyhost) {
    proxy_req.server_name = s->hostname;
    proxy_req.server_url = Request;
    rc->result = (ecs_Result *) (createproxyserver_1(&proxy_req,rc->handle));
  } else {
    rc->result = (ecs_Result *) (createserver_1(&Request,rc->handle));
  }

  /* Initialize default timeout */

  timeOut.tv_sec = DEFTIMEOUT;
  timeOut.tv_usec = 0;
  clnt_control(rc->handle, CLSET_TIMEOUT, (char *) &timeOut);

  if (rc->result == NULL) {
    ecs_SetError(&(s->result),1,"No answer from server when CreateServer is called");
    return &(s->result);
  } else {
    return rc->result;
  } 
  ecs_SetSuccess(&(s->result));
  return &(s->result);
}

/*
   ------------------------------------------------------------------------
   */

ecs_Result *dyn_DestroyServer(s)
     ecs_Server *s;
{
  ecs_Remote *rc;
  struct timeval timeOut;

  rc = (ecs_Remote *) s->priv;
  if (rc == NULL) {
    ecs_SetError(&(s->result),1,"Server not initialized");
    return &(s->result);   
  }

  /* Free old rc->result */

  if (rc->result != NULL) {
    xdr_free((xdrproc_t) xdr_ecs_Result,(char *) rc->result);
    rc->result = NULL;
  }

  /* Initialize the timeout of the create client */

  timeOut.tv_sec = CONTIMEOUT;
  timeOut.tv_usec = 0;
  clnt_control(rc->handle, CLSET_TIMEOUT, (char *) &timeOut);

  rc->result = (ecs_Result *) (destroyserver_1(NULL,rc->handle));  
  clnt_destroy(rc->handle);

  if (rc->result == NULL) {
    ecs_SetError(&(s->result),
		 1,"No answer from server when DestroyServer is called. The server is possibly orphan.");
  } else {
    xdr_free((xdrproc_t) xdr_ecs_Result,(char *) rc->result);
    ecs_SetSuccess(&(s->result));
  } 
  free(rc);
  rc = NULL;


  return &(s->result);
}

/*
   ------------------------------------------------------------------------
   */

ecs_Result *dyn_SelectLayer(s,ls)
     ecs_Server *s;
     ecs_LayerSelection *ls;
{
  ecs_Remote *rc;
  int layer;

  rc = (ecs_Remote *) s->priv;
  if (rc == NULL) {
    ecs_SetError(&(s->result),1,"Server not initialized");
    return &(s->result);   
  }

  if ((layer = ecs_GetLayer(s,ls)) == -1) {
    /* it did not exists so we are going to try to create it */    
    if ((layer = ecs_SetLayer(s,ls)) == -1) {
      return &(s->result);
    }
  }
       
  /* Free old rc->result */

  if (rc->result != NULL) {
    xdr_free((xdrproc_t) xdr_ecs_Result,(char *) rc->result);
    rc->result = NULL;
  }

  rc->result = (ecs_Result *) (selectlayer_1(ls,rc->handle));  

  if (rc->result == NULL) {
    ecs_SetError(&(s->result),
		 1,"No answer from server when selectlayer is called.");
    return &(s->result);
  } 

  s->currentLayer = layer;
  s->layer[layer].index = 0;

  return rc->result;
}

/*
   ------------------------------------------------------------------------
   */

ecs_Result *dyn_ReleaseLayer(s,ls)
     ecs_Server *s;
     ecs_LayerSelection *ls;
{
  ecs_Remote *rc;
  int layer;
  char buffer[128];

  rc = (ecs_Remote *) s->priv;
  if (rc == NULL) {
    ecs_SetError(&(s->result),1,"Server not initialized");
    return &(s->result);   
  }

  if ((layer = ecs_GetLayer(s,ls)) == -1) {
    sprintf(buffer,"Invalid layer %s",ls->Select);
    ecs_SetError(&(s->result),1,buffer);
    return &(s->result);
  }
  ecs_FreeLayer(s,layer);

  /* Free old rc->result */

  if (rc->result != NULL) {
    xdr_free((xdrproc_t) xdr_ecs_Result,(char *) rc->result);
    rc->result = NULL;
  }

  rc->result = (ecs_Result *) (releaselayer_1(ls,rc->handle));  

  if (rc->result == NULL) {
    ecs_SetError(&(s->result),
		 1,"No answer from server when releaselayer is called.");
    return &(s->result);
  } 

  if (s->currentLayer == layer) {
    s->currentLayer = -1;
  }

  return rc->result;
}

/*
   ------------------------------------------------------------------------
   */

ecs_Result *dyn_SelectRegion(s,gr)
     ecs_Server *s;
     ecs_Region *gr;
{
  ecs_Remote *rc;

  rc = (ecs_Remote *) s->priv;
  if (rc == NULL) {
    ecs_SetError(&(s->result),1,"Server not initialized");
    return &(s->result);   
  }

  /* Free old rc->result */

  if (rc->result != NULL) {
    xdr_free((xdrproc_t) xdr_ecs_Result,(char *) rc->result);
    rc->result = NULL;
  }

  s->currentRegion.north = gr->north;
  s->currentRegion.south = gr->south;
  s->currentRegion.east = gr->east;
  s->currentRegion.west = gr->west;
  s->currentRegion.ns_res = gr->ns_res;
  s->currentRegion.ew_res = gr->ew_res;

  rc->result = (ecs_Result *) (selectregion_1(gr,rc->handle));  

  if (rc->result == NULL) {
    ecs_SetError(&(s->result),
		 1,"No answer from server when selectregion is called.");
    return &(s->result);
  } 

  return rc->result;
}

/*
   ------------------------------------------------------------------------
   */

ecs_Result *dyn_GetDictionary(s)
     ecs_Server *s;
{
  ecs_Remote *rc;

  rc = (ecs_Remote *) s->priv;
  if (rc == NULL) {
    ecs_SetError(&(s->result),1,"Server not initialized");
    return &(s->result);   
  }

  /* Free old rc->result */

  if (rc->result != NULL) {
    xdr_free((xdrproc_t) xdr_ecs_Result,(char *) rc->result);
    rc->result = NULL;
  }

  rc->result = (ecs_Result *) (getdictionnary_1(NULL,rc->handle));  

  if (rc->result == NULL) {
    ecs_SetError(&(s->result),
		 1,"No answer from server when getdictionary is called.");
    return &(s->result);
  } 

  return rc->result;
}

/*
   ------------------------------------------------------------------------
   */

ecs_Result *dyn_GetNextObject(s)
     ecs_Server *s;
{
  ecs_Remote *rc;

  rc = (ecs_Remote *) s->priv;
  if (rc == NULL) {
    ecs_SetError(&(s->result),1,"Server not initialized");
    return &(s->result);   
  }

  /* Free old rc->result */

  if (rc->result != NULL) {
    xdr_free((xdrproc_t) xdr_ecs_Result,(char *) rc->result);
    rc->result = NULL;
  }

  rc->result = (ecs_Result *) (getnextobject_1(NULL,rc->handle));  

  if (rc->result == NULL) {
    ecs_SetError(&(s->result),
		 1,"No answer from server when getnextobject is called.");
    return &(s->result);
  } 

  return rc->result;
}

/*
   ------------------------------------------------------------------------
   */

ecs_Result *dyn_GetObject(s,Id)
     ecs_Server *s;
     char *Id;
{
  ecs_Remote *rc;

  rc = (ecs_Remote *) s->priv;
  if (rc == NULL) {
    ecs_SetError(&(s->result),1,"Server not initialized");
    return &(s->result);   
  }

  /* Free old rc->result */

  if (rc->result != NULL) {
    xdr_free((xdrproc_t) xdr_ecs_Result,(char *) rc->result);
    rc->result = NULL;
  }

  rc->result = (ecs_Result *) (getobject_1(&Id,rc->handle));  

  if (rc->result == NULL) {
    ecs_SetError(&(s->result),
		 1,"No answer from server when getobject is called.");
    return &(s->result);
  } 

  return rc->result;
}

/*
   ------------------------------------------------------------------------
   */

ecs_Result *dyn_UpdateDictionary(s,info)
     ecs_Server *s;
     char *info;
{
  ecs_Remote *rc;

  rc = (ecs_Remote *) s->priv;
  if (rc == NULL) {
    ecs_SetError(&(s->result),1,"Server not initialized");
    return &(s->result);   
  }

  /* Free old rc->result */

  if (rc->result != NULL) {
    xdr_free((xdrproc_t) xdr_ecs_Result,(char *) rc->result);
    rc->result = NULL;
  }

  rc->result = (ecs_Result *) (updatedictionary_1(&info,rc->handle));  

  if (rc->result == NULL) {
    ecs_SetError(&(s->result),
		 1,"No answer from server when updatedictionary is called.");
    return &(s->result);
  } 

  return rc->result;
}

/*
   ------------------------------------------------------------------------
   */

ecs_Result *dyn_GetServerProjection(s)
     ecs_Server *s;
{
  ecs_Remote *rc;

  rc = (ecs_Remote *) s->priv;
  if (rc == NULL) {
    ecs_SetError(&(s->result),1,"Server not initialized");
    return &(s->result);   
  }

  /* Free old rc->result */

  if (rc->result != NULL) {
    xdr_free((xdrproc_t) xdr_ecs_Result,(char *) rc->result);
    rc->result = NULL;
  }

  rc->result = (ecs_Result *) (getserverprojection_1(NULL,rc->handle));  

  if (rc->result == NULL) {
    ecs_SetError(&(s->result),
		 1,"No answer from server when getserverprojection is called.");
    return &(s->result);
  } 

  return rc->result;

}

/*
   ------------------------------------------------------------------------
   */

ecs_Result *dyn_SetServerLanguage(s,language)
     ecs_Server *s;
     u_int language;
{
  ecs_Remote *rc;

  rc = (ecs_Remote *) s->priv;
  if (rc == NULL) {
    ecs_SetError(&(s->result),1,"Server not initialized");
    return &(s->result);   
  }

  /* Free old rc->result */

  if (rc->result != NULL) {
    xdr_free((xdrproc_t) xdr_ecs_Result,(char *) rc->result);
    rc->result = NULL;
  }

  rc->result = (ecs_Result *) (setserverlanguage_1(&language,rc->handle));  

  if (rc->result == NULL) {
    ecs_SetError(&(s->result),
		 1,"No answer from server when setserverlanguage is called.");
    return &(s->result);
  } 

  return rc->result;
}

/*
   ------------------------------------------------------------------------
   */

ecs_Result *dyn_SetServerProjection(s,projection)
     ecs_Server *s;
     char *projection;
{
  ecs_Remote *rc;

  rc = (ecs_Remote *) s->priv;
  if (rc == NULL) {
    ecs_SetError(&(s->result),1,"Server not initialized");
    return &(s->result);   
  }

  /* Free old rc->result */

  if (rc->result != NULL) {
    xdr_free((xdrproc_t) xdr_ecs_Result,(char *) rc->result);
    rc->result = NULL;
  }

  rc->result = (ecs_Result *) (setserverprojection_1(&projection,rc->handle));  

  if (rc->result == NULL) {
    ecs_SetError(&(s->result),
		 1,"No answer from server when setserverprojection is called.");
    return &(s->result);
  } 

  return rc->result;
}

/*
   ------------------------------------------------------------------------
   */

ecs_Result *dyn_GetObjectIdFromCoord(s,c)
     ecs_Server *s;
     ecs_Coordinate *c;
{
  ecs_Remote *rc;

  rc = (ecs_Remote *) s->priv;
  if (rc == NULL) {
    ecs_SetError(&(s->result),1,"Server not initialized");
    return &(s->result);   
  }

  /* Free old rc->result */

  if (rc->result != NULL) {
    xdr_free((xdrproc_t) xdr_ecs_Result,(char *) rc->result);
    rc->result = NULL;
  }

  rc->result = (ecs_Result *) (getobjectidfromcoord_1(c,rc->handle));  

  if (rc->result == NULL) {
    ecs_SetError(&(s->result),
		 1,"No answer from server when GetObjectIdFromCoord is called.");
    return &(s->result);
  } 

  return rc->result;

}

/*
   ------------------------------------------------------------------------
   */

ecs_Result *dyn_GetAttributesFormat(s)
     ecs_Server *s;
{
  ecs_Remote *rc;

  rc = (ecs_Remote *) s->priv;
  if (rc == NULL) {
    ecs_SetError(&(s->result),1,"Server not initialized");
    return &(s->result);   
  }

  /* Free old rc->result */

  if (rc->result != NULL) {
    xdr_free((xdrproc_t) xdr_ecs_Result,(char *) rc->result);
    rc->result = NULL;
  }

  rc->result = (ecs_Result *) (getattributeformat_1(NULL,rc->handle));  

  if (rc->result == NULL) {
    ecs_SetError(&(s->result),
		 1,"No answer from server when GetAttributesFormat is called.");
    return &(s->result);
  } 

  return rc->result;
}

/*
   ------------------------------------------------------------------------
   */

ecs_Result *dyn_GetRasterInfo(s)
     ecs_Server *s;
{
  ecs_Remote *rc;

  rc = (ecs_Remote *) s->priv;
  if (rc == NULL) {
    ecs_SetError(&(s->result),1,"Server not initialized");
    return &(s->result);   
  }

  /* Free old rc->result */

  if (rc->result != NULL) {
    xdr_free((xdrproc_t) xdr_ecs_Result,(char *) rc->result);
    rc->result = NULL;
  }

  rc->result = (ecs_Result *) (getrasterinfo_1(NULL,rc->handle));  

  if (rc->result == NULL) {
    ecs_SetError(&(s->result),
		 1,"No answer from server when getrasterinfo is called.");
    return &(s->result);
  } 

  return rc->result;
}

/*
   ------------------------------------------------------------------------
   */

ecs_Result *dyn_GetGlobalBound(s)
     ecs_Server *s;
{
  ecs_Remote *rc;

  rc = (ecs_Remote *) s->priv;
  if (rc == NULL) {
    ecs_SetError(&(s->result),1,"Server not initialized");
    return &(s->result);   
  }

  /* Free old rc->result */

  if (rc->result != NULL) {
    xdr_free((xdrproc_t) xdr_ecs_Result,(char *) rc->result);
    rc->result = NULL;
  }

  rc->result = (ecs_Result *) (getglobalbound_1(NULL,rc->handle));  

  if (rc->result == NULL) {
    ecs_SetError(&(s->result),
		 1,"No answer from server when getglobalbound is called.");
    return &(s->result);
  } 

  return rc->result;

}


/*
   ------------------------------------------------------------------------
   */

ecs_Result *dyn_SetRasterConversion(s,rasterconversion)
     ecs_Server *s;
     ecs_RasterConversion *rasterconversion;
{
  ecs_Remote *rc;

  rc = (ecs_Remote *) s->priv;
  if (rc == NULL) {
    ecs_SetError(&(s->result),1,"Server not initialized");
    return &(s->result);   
  }

  /* Free old rc->result */

  if (rc->result != NULL) {
    xdr_free((xdrproc_t) xdr_ecs_Result,(char *) rc->result);
    rc->result = NULL;
  }

  rc->result = (ecs_Result *) (setrasterconversion_1(rasterconversion,
						     rc->handle));  

  if (rc->result == NULL) {
    ecs_SetError(&(s->result),
		 1,"No answer from server when setrasterconvertion is called.");
    return &(s->result);
  } 

  return rc->result;

}

/*
   ------------------------------------------------------------------------
   */

ecs_Result *dyn_SetCompression(s,compression)
     ecs_Server *s;
     ecs_Compression *compression;
{
  ecs_Remote *rc;

  rc = (ecs_Remote *) s->priv;
  if (rc == NULL) {
    ecs_SetError(&(s->result),1,"Server not initialized");
    return &(s->result);   
  }

  /* Free old rc->result */

  if (rc->result != NULL) {
    xdr_free((xdrproc_t) xdr_ecs_Result,(char *) rc->result);
    rc->result = NULL;
  }

  rc->result = (ecs_Result *) (setcompression_1(compression,rc->handle));  

  if (rc->result == NULL) {
    ecs_SetError(&(s->result),
		 1,"No answer from server when setcompression is called.");
    return &(s->result);
  } 

  return rc->result;
}



