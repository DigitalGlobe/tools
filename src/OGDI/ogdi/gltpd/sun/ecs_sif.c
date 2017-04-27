#include "ecs.h"

ecs_Server *mptr;

ecs_Result ecs_dummy_sif;

ecs_Result *createserver_1(args,rqstp)
     char **args;
     struct svc_req *rqstp;
{
  mptr = (ecs_Server *) malloc(sizeof(ecs_Server));
  if (mptr == NULL) {
    ecs_ResultInit(&ecs_dummy_sif);
    ecs_SetError(&ecs_dummy_sif,1,"no more memory");
    return &ecs_dummy_sif;
  }

  return svr_CreateServer (mptr, *args, 0);
}

ecs_Result *destroyserver_1(args,rqstp)
     void *args;
     struct svc_req *rqstp;
{
  ecs_Result *msg;

  msg = svr_DestroyServer(mptr);
  free(mptr);
  mptr = NULL;
  return msg;
}

ecs_Result *selectlayer_1(args,rqstp)
     ecs_LayerSelection *args;
     struct svc_req *rqstp;
{
  return svr_SelectLayer(mptr, args);  
}

ecs_Result *releaselayer_1(args,rqstp)
     ecs_LayerSelection *args;
     struct svc_req *rqstp;
{
  return svr_ReleaseLayer(mptr, args);  
}

ecs_Result *selectregion_1(args,rqstp)
     ecs_Region *args;
     struct svc_req *rqstp;
{
  return svr_SelectRegion(mptr, args);
}

ecs_Result *getdictionnary_1(args,rqstp)
     void *args;
     struct svc_req *rqstp;
{
  return svr_GetDictionary(mptr);
}

ecs_Result *getattributeformat_1(args,rqstp)
     void *args;
     struct svc_req *rqstp;
{
  return svr_GetAttributesFormat(mptr);
}

ecs_Result *getnextobject_1(args,rqstp)
     void *args;
     struct svc_req *rqstp;
{
  return svr_GetNextObject(mptr);
}

ecs_Result *getrasterinfo_1(args,rqstp)
     void *args;
     struct svc_req *rqstp;
{
  return svr_GetRasterInfo(mptr);
}

ecs_Result *getobject_1(args,rqstp)
     char **args;
     struct svc_req *rqstp;
{
  return svr_GetObject(mptr, *args);
}

ecs_Result *getobjectidfromcoord_1(args,rqstp)
     ecs_Coordinate *args;
     struct svc_req *rqstp;
{
  return svr_GetObjectIdFromCoord(mptr, args);
}

ecs_Result *updatedictionary_1(args,rqstp)
     char **args;
     struct svc_req *rqstp;
{
  return svr_UpdateDictionary(mptr,*args);
}

ecs_Result *getserverprojection_1(args,rqstp)
     void *args;
     struct svc_req *rqstp;
{
  return svr_GetServerProjection(mptr);  
}

ecs_Result *getglobalbound_1(args,rqstp)
     void *args;
     struct svc_req *rqstp;
{
  return svr_GetGlobalBound(mptr);
}

ecs_Result *setserverlanguage_1(args,rqstp)
     u_int *args;
     struct svc_req *rqstp;
{
  return svr_SetServerLanguage(mptr, *args);
}

ecs_Result * setserverprojection_1(args,rqstp)
     char **args;
     struct svc_req *rqstp;
{
  return svr_SetServerProjection(mptr, *args);
}

ecs_Result * setrasterconversion_1(args,rqstp)
     ecs_RasterConversion *args;
     struct svc_req *rqstp;
{
  return svr_SetRasterConversion(mptr,args);
}














