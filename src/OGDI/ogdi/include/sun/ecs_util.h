#ifndef ECS_UTIL
#define ECS_UTIL 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
#include <math.h>
#include <memory.h>
#include <ctype.h>

#ifdef _WINDOWS
#include <windows.h>
#include <winsock.h>
#else
#include <dlfcn.h>
#endif

#include "projects.h"
#include "ecs.h"

#ifdef _SCO
#include <sys/fs/nfs/time.h>
#endif

/***********************************************************************/

/* Global definitions */

#undef _ANSI_ARGS_
#undef CONST
#if ((defined(__STDC__) || defined(SABER)) && !defined(NO_PROTOTYPE)) || defined(__cplusplus)
#   define _USING_PROTOTYPES_ 1
#   define _ANSI_ARGS_(x)	x
#   define CONST const
#   ifdef __cplusplus
#       define VARARGS(first) (first, ...)
#   else
#       define VARARGS(first) ()
#   endif
#else
#   define _ANSI_ARGS_(x)	()
#   define CONST
#endif

#ifdef __cplusplus
#   define EXTERN extern "C"
#else
#   define EXTERN extern
#endif

/*
 * Macro to use instead of "void" for arguments that must have
 * type "void *" in ANSI C;  maps them to type "char *" in
 * non-ANSI systems.
 */

#ifndef VOID
#   ifdef __STDC__
#       define VOID void
#   else
#       define VOID char
#   endif
#endif

/*
 * Miscellaneous declarations (to allow Tcl to be used stand-alone,
 * without the rest of Sprite).
 */

#ifndef NULL
#define NULL 0
#endif

#define PROJ_UNKNOWN "unknown"
#define PROJ_LONGLAT "+proj=longlat"

#define ECSGETJ(s,i1,j1) (int)(((s->rasterconversion.coef.coef_val[0]*j1 + s->rasterconversion.coef.coef_val[1]*i1 + s->rasterconversion.coef.coef_val[6]) / (s->rasterconversion.coef.coef_val[4]*j1 + s->rasterconversion.coef.coef_val[5]*i1 + 1)) + 0.5)
#define ECSGETI(s,i1,j1) (int)(((s->rasterconversion.coef.coef_val[2]*j1 + s->rasterconversion.coef.coef_val[3]*i1 + s->rasterconversion.coef.coef_val[7]) / (s->rasterconversion.coef.coef_val[4]*j1 + s->rasterconversion.coef.coef_val[5]*i1 + 1)) + 0.5)

#ifndef PI
#define PI 3.14159265359
#endif

/***********************************************************************/

/* server.c declarations */

#define ECSMAXLAYER 64

typedef ecs_Result *dynfunc();

typedef struct {
  ecs_LayerSelection sel;
  int index;
  int nbfeature;
  void *priv;
} ecs_Layer;

typedef struct {
  /* Specific information for dynamic part of the server */

  void *priv;

  ecs_Layer layer[ECSMAXLAYER];
  int nblayer;
  int currentLayer;

  /* dynamic library handle */
  
  void *handle;

  /* Regions of the server */

  ecs_Region currentRegion;
  ecs_Region globalRegion;

  /* Server projection */

  char *projection;

  /* Extracted information from URL */

  char *hostname;
  char *server_type;
  char *pathname;

  /* Indicate if the server is a remote server or not */

  int isRemote;

  /* RasterConversion structure */
  
  ecs_RasterConversion rasterconversion;

  /* Structure returned to client */

  ecs_Result result;
  
  /* pointers to functions */

  dynfunc *createserver;
  dynfunc *destroyserver;
  dynfunc *selectlayer;
  dynfunc *releaselayer;

  dynfunc *closelayer;

  dynfunc *selectregion;
  dynfunc *getdictionary;
  dynfunc *getattrformat;
  dynfunc *getnextobject;
  dynfunc *getrasterinfo;
  dynfunc *getobject;
  dynfunc *getobjectid;
  dynfunc *updatedictionary;
  dynfunc *getserverprojection;
  dynfunc *getglobalbound;
  dynfunc *setserverlanguage;
  dynfunc *setserverprojection;
  dynfunc *setrasterconversion;
} ecs_Server;


ecs_Result *svr_CreateServer _ANSI_ARGS_((ecs_Server *s, char *Request, int isLocal));
ecs_Result *svr_DestroyServer _ANSI_ARGS_((ecs_Server *s));
ecs_Result *svr_SelectLayer _ANSI_ARGS_((ecs_Server *s, ecs_LayerSelection *ls));
ecs_Result *svr_ReleaseLayer _ANSI_ARGS_((ecs_Server *s, ecs_LayerSelection *ls));
void svr_BroadCloseLayers _ANSI_ARGS_((ecs_Server *s));
void svr_CloseLayer _ANSI_ARGS_((ecs_Server *s));
ecs_Result *svr_SelectRegion _ANSI_ARGS_((ecs_Server *s, ecs_Region *gr));
ecs_Result *svr_GetDictionary _ANSI_ARGS_((ecs_Server *s));
ecs_Result *svr_GetAttributesFormat _ANSI_ARGS_((ecs_Server *s));
ecs_Result *svr_GetNextObject _ANSI_ARGS_((ecs_Server *s));
ecs_Result *svr_GetRasterInfo _ANSI_ARGS_((ecs_Server *s));
ecs_Result *svr_GetObject _ANSI_ARGS_((ecs_Server *s, char *Id));
ecs_Result *svr_GetObjectIdFromCoord _ANSI_ARGS_((ecs_Server *s, ecs_Coordinate *coord));
ecs_Result *svr_UpdateDictionary _ANSI_ARGS_((ecs_Server *s, char *info));
ecs_Result *svr_GetServerProjection _ANSI_ARGS_((ecs_Server *s));
ecs_Result *svr_GetGlobalBound _ANSI_ARGS_((ecs_Server *s));
ecs_Result *svr_SetServerLanguage _ANSI_ARGS_((ecs_Server *s, u_int language));
ecs_Result *svr_SetServerProjection _ANSI_ARGS_((ecs_Server *s, char *projection));
ecs_Result *svr_SetRasterConversion _ANSI_ARGS_((ecs_Server *s,
						 ecs_RasterConversion *rc));

/* These functions will be called by dynamic servers */

int ecs_SetLayer  _ANSI_ARGS_((ecs_Server *s, ecs_LayerSelection *sel));
int ecs_GetLayer  _ANSI_ARGS_((ecs_Server *s, ecs_LayerSelection *sel));
void ecs_FreeLayer _ANSI_ARGS_((ecs_Server *s, int layer));
int ecs_RemoveDir _ANSI_ARGS_((char *path));

/***********************************************************************/

/* ecs_dyna.c declarations */

void *ecs_OpenDynamicLib _ANSI_ARGS_((char *libname));
void *ecs_GetDynamicLibFunction _ANSI_ARGS_((void *handle,char *functionname));
void ecs_CloseDynamicLib _ANSI_ARGS_((void *handle));

/***********************************************************************/

/* ecsregex.c declarations */

#define NSUBEXP  50
typedef struct ecs_regexp {
	char *startp[NSUBEXP];
	char *endp[NSUBEXP];
	char regstart;		/* Internal use only. */
	char reganch;		/* Internal use only. */
	char *regmust;		/* Internal use only. */
	int regmlen;		/* Internal use only. */
	char program[1];	/* Unwarranted chumminess with compiler. */
} ecs_regexp;

ecs_regexp *EcsRegComp _ANSI_ARGS_((char *exp));
int EcsRegExec _ANSI_ARGS_((ecs_regexp *prog, char *string, char *start));
void EcsRegError _ANSI_ARGS_((char *msg));
char *EcsGetRegError _ANSI_ARGS_((void));

/***********************************************************************/

/* ecsdist.c declarations */


double ecs_DistanceObject _ANSI_ARGS_((ecs_Object *obj, double X, double Y));
double ecs_DistanceMBR _ANSI_ARGS_((double xl, double yl, double xu, double yu, double dx, double dy));
double ecs_DistanceSegment _ANSI_ARGS_((double xl, double yl, double xu, double yu, double dx, double dy));

/***********************************************************************/

/* ecsassoc.c declarations */

/* WARNING: These define do more than one operation.
   Please, don't consider these as functions.
   */


/*
  ----------------------------------------------------------------
  ECSRESULTTYPE: This macro indicate the object type of ecs_Result
  ----------------------------------------------------------------
  */

#define ECSRESULTTYPE(result) result->res.type

/*
  ----------------------------------------------------------------
  ECSRESULTTYPE: This macro access the union in ecs_Result and 
  facilitate ecs_Result structures access.
  ----------------------------------------------------------------
  */

#define ECSRESULT(result) result->res.ecs_ResultUnion_u

/*
  ----------------------------------------------------------------
  ECSRESULTTYPE: This macro indicate the geographical object type 
  contain in ecs_Result
  ----------------------------------------------------------------
  */

#define ECSGEOMTYPE(result) result->res.ecs_ResultUnion_u.dob.geom.family

/*
  ----------------------------------------------------------------
  ECSGEOM: This macro access the union contain in the geographical
  object in ecs_Result.
  ----------------------------------------------------------------
  */

#define ECSGEOM(result) result->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u

/*
  ----------------------------------------------------------------
  ECSAREARING: This macro access the area ring in the geographic
  object of type Area
  ----------------------------------------------------------------
  */

#define ECSAREARING(result,pos) result->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.area.ring.ring_val[pos]

/*
  ----------------------------------------------------------------
  ECSERROR: Indicate if ecs_Result contain an error code
  ----------------------------------------------------------------
  */

#define ECSERROR(r) (r->error != 0)

/*
  ----------------------------------------------------------------
  ECSSUCCESS: Indicate if ecs_Result contain an success code
  ----------------------------------------------------------------
  */

#define ECSSUCCESS(r) (r->error == 0)

/*
  ----------------------------------------------------------------
  ECSEOF: Indicate if ecs_Result contain EOF message. Mainly
  use in cln_GetNextObject function.
  ----------------------------------------------------------------
  */

#define ECSEOF(r) (r->error == 2)

/*
  ----------------------------------------------------------------
  ECSPROJ: Indicate if ecs_Result contain PROJ error message. Mainly
  use in cln_GetNextObject function.
  ----------------------------------------------------------------
  */

#define ECSPROJ(r) (r->error == 3)

/*
  ----------------------------------------------------------------
  ECSMESSAGE: Return the error message contain in ecs_Result (an
  string).
  ----------------------------------------------------------------
  */

#define ECSMESSAGE(r) r->message

/*
  ----------------------------------------------------------------
  ECSREGION: Return the geographical region contain in ecs_Result
  (if it's the case). The structure returned is a ecs_Region.
  ----------------------------------------------------------------
  */

#define ECSREGION(r) ECSRESULT(r).gr

/*
  ----------------------------------------------------------------
  ECSTEXT: Return the text string contain in ecs_Result (if 
  it's the case). The structure returned is a string.
  ----------------------------------------------------------------
  */

#define ECSTEXT(r) ECSRESULT(r).s

/*
  ----------------------------------------------------------------
  ECSRASTERINFO: Return the raster information contain in ecs_Result
  (if it's the case). The structure return is a ecs_RasterInfo.
  ----------------------------------------------------------------
  */

#define ECSRASTERINFO(r) ECSRESULT(r).ri

/*
  ----------------------------------------------------------------
  ECSRASTERINFONB: Return the number of categories in the
  ecs_RasterInfo structure contain in ecs_Result.
  ----------------------------------------------------------------
  */

#define ECSRASTERINFONB(r) ECSRASTERINFO(r).cat.cat_len

/*
  ----------------------------------------------------------------
  ECSRASTERINFOCAT: Return the category number "c" contain in
  ecs_RasterInfo in ecs_Result. The structure returned is a
  ecs_Category.
  ----------------------------------------------------------------
  */

#define ECSRASTERINFOCAT(r,c) ECSRASTERINFO(r).cat.cat_val[c]

/*
  ----------------------------------------------------------------
  ECSOBJECT: Return the geographic object contain in ecs_Result
  (if it's the case). The structure return is a ecs_Object.
  ----------------------------------------------------------------
  */

#define ECSOBJECT(r) ECSRESULT(r).dob

/*
  ----------------------------------------------------------------
  ECSOBJECTID: Return the "Id" attribute contain the ecs_Object structure
  contain in ecs_Result.
  ----------------------------------------------------------------
  */

#define ECSOBJECTID(r) ECSOBJECT(r).Id

/*
  ----------------------------------------------------------------
  ECSOBJECTATTR: Return the "attr" attribute contain the ecs_Object structure
  contain in ecs_Result.
  ----------------------------------------------------------------
  */

#define ECSOBJECTATTR(r) ECSOBJECT(r).attr

/*
  ----------------------------------------------------------------
  ECSRASTER: Return the raster line table contain in ecs_Object.
  ----------------------------------------------------------------
  */

#define ECSRASTER(r) ECSOBJECT(r).geom.ecs_Geometry_u.matrix.x.x_val

/*
  ----------------------------------------------------------------
  ECS_SETGEOMBOUNDINGBOX: This macro will put in the geographical
  object contain in "result" the bounding rectangle (lxmin, lymin,
  lxmax, lymax).
  ----------------------------------------------------------------
  */

#define ECS_SETGEOMBOUNDINGBOX(result,lxmin,lymin,lxmax,lymax) { \
    if (result->res.type==Object) { \
	result->res.ecs_ResultUnion_u.dob.xmin=lxmin; \
	result->res.ecs_ResultUnion_u.dob.ymin=lymin; \
	result->res.ecs_ResultUnion_u.dob.xmax=lxmax; \
	result->res.ecs_ResultUnion_u.dob.ymax=lymax; \
    } \
}

/*
  ----------------------------------------------------------------
  ECSGEOMLINECOORD: This macro put a point (lx,ly) at the position
  "position" in a line geographical object. To entirely set the
  line object, ecs_SetGeomLine must be call first and for each point,
  ECS_SETGEOMLINECOORD must be call.
  ----------------------------------------------------------------
  */

#define ECS_SETGEOMLINECOORD(result,position,lx,ly) \
result->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.line.c.c_val[position].x = lx; \
result->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.line.c.c_val[position].y = ly;

/*
  ----------------------------------------------------------------
  ECSGEOMAREACOORD: This macro put a point (lx,ly) at the position
  "position" of the ring "ringpos" in a area geographical object. To 
  entirely set the area object, ecs_SetGeomArea must be call first 
  and for each ring, ecs_SetGeomAreaRing must be call. In each of 
  this ring, ECS_SETGEOMAREACOORD must be call define the polygons 
  points.
  ----------------------------------------------------------------
  */

#define ECS_SETGEOMAREACOORD(result,ringpos,position,lx,ly) \
result->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.area.ring.ring_val[ringpos].c.c_val[position].x = lx; \
result->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.area.ring.ring_val[ringpos].c.c_val[position].y = ly;

/*
  ----------------------------------------------------------------
  ECSGEOMMATRIXCOORD: This macro put a value at the position 
  "position" in a matrix geographical object. To entirely set the 
  matrix object, ecs_SetGeomMatrix must be call first and for each 
  column, ECS_SETGEOMMATRIXCOORD must be call.
  ----------------------------------------------------------------
  */

#define ECS_SETGEOMMATRIXVALUE(result,lpos,lval) \
result->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.matrix.x.x_val[lpos] = lval

/*
  ----------------------------------------------------------------
  ECSGEOMIMAGECOORD: This macro put a value at the position 
  "position" in a image geographical object. To entirely set the 
  image object, ecs_SetGeomImage must be call first and for each 
  column, ECS_SETGEOMIMAGECOORD must be call.
  ----------------------------------------------------------------
  */

#define ECS_SETGEOMIMAGEVALUE(result,lpos,lval) \
result->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.image.x.x_val[lpos] = lval

/********************************/

int ecs_SetError _ANSI_ARGS_((ecs_Result *r,
			      int errorcode, char *error_message));
int ecs_SetSuccess _ANSI_ARGS_((ecs_Result *r));
int ecs_AdjustResult _ANSI_ARGS_((ecs_Result *r));
int ecs_SetGeoRegion _ANSI_ARGS_((ecs_Result *r,
				  double north, double south, double east,
				  double west, double nsres, double ewres));
int ecs_SetText _ANSI_ARGS_((ecs_Result *r,char *text));
int ecs_AddText _ANSI_ARGS_((ecs_Result *r,char *text));
int ecs_SetRasterInfo _ANSI_ARGS_((ecs_Result *r, int width, int height));
int ecs_AddRasterInfoCategory _ANSI_ARGS_((ecs_Result *r, long no_cat,
					   unsigned int red, 
					   unsigned int green,
					   unsigned int blue, char *label,
					   unsigned long qty));
int ecs_SetObjAttributeFormat _ANSI_ARGS_((ecs_Result *r));
int ecs_AddAttributeFormat _ANSI_ARGS_((ecs_Result *r, char *name,
					ecs_AttributeFormat type,
					int lenght, int precision,
					int nullable));
int ecs_SetGeomPoint _ANSI_ARGS_((ecs_Result *r, double x, double y));
int ecs_SetGeomText _ANSI_ARGS_((ecs_Result *r, double x, 
				 double y, char *desc));
int ecs_SetGeomLine _ANSI_ARGS_((ecs_Result *r, unsigned int lenght));
int ecs_SetGeomArea _ANSI_ARGS_((ecs_Result *r, unsigned int lenght));
int ecs_SetGeomAreaRing _ANSI_ARGS_((ecs_Result *r, int position, 
				     unsigned int length,
				     double centroid_x, double centroid_y));
int ecs_SetGeomMatrix _ANSI_ARGS_((ecs_Result *r, int size));
int ecs_SetGeomMatrixWithArray _ANSI_ARGS_((ecs_Result *r, int size, unsigned int *array));
int ecs_SetGeomImage _ANSI_ARGS_((ecs_Result *r, int size));
int ecs_SetGeomImageWithArray _ANSI_ARGS_((ecs_Result *r, int size, unsigned int *array));
int ecs_SetObjectId _ANSI_ARGS_((ecs_Result *r,char *id));
int ecs_SetObjectAttr _ANSI_ARGS_((ecs_Result *r,char *attr));
int ecs_CleanUp _ANSI_ARGS_((ecs_Result *r));
int ecs_CleanUpObject _ANSI_ARGS_((ecs_Result *r));
int ecs_ResultInit _ANSI_ARGS_((ecs_Result *r));
int ecs_CalcObjectMBR _ANSI_ARGS_((ecs_Server *r, ecs_Result *e));

/***********************************************************************/

/* ecs_split.c declarations */

void ecs_freeSplitURL _ANSI_ARGS_((char **type,char **machine,char **path));
int ecs_GetRegex _ANSI_ARGS_((ecs_regexp *reg,int index,char **chaine));
int ecs_SplitURL _ANSI_ARGS_((char *url,char **machine,char **server,char **path));


/***********************************************************************/

/* ecs_list.c declarations */

char ecs_Backslash _ANSI_ARGS_((char *src, int *readPtr));
int ecs_FindElement _ANSI_ARGS_((register char *list,char **elementPtr, char **nextPtr, int *sizePtr, int *bracePtr));
void ecs_CopyAndCollapse _ANSI_ARGS_((int count,register char *src,register char *dst));
int ecs_SplitList _ANSI_ARGS_((char *list,int *argcPtr,char ***argvPtr));

/***********************************************************************/



/* dynamic library server declarations */

ecs_Result *dyn_CreateServer _ANSI_ARGS_((ecs_Server *s, char *Request));
ecs_Result *dyn_DestroyServer _ANSI_ARGS_((ecs_Server *s));
ecs_Result *dyn_SelectLayer _ANSI_ARGS_((ecs_Server *s, ecs_LayerSelection *ls));
ecs_Result *dyn_ReleaseLayer _ANSI_ARGS_((ecs_Server *s, ecs_LayerSelection *ls));
ecs_Result *dyn_SelectRegion _ANSI_ARGS_((ecs_Server *s, ecs_Region *gr));
ecs_Result *dyn_GetDictionary _ANSI_ARGS_((ecs_Server *s));
ecs_Result *dyn_GetAttributesFormat _ANSI_ARGS_((ecs_Server *s));
ecs_Result *dyn_GetNextObject _ANSI_ARGS_((ecs_Server *s));
ecs_Result *dyn_GetRasterInfo _ANSI_ARGS_((ecs_Server *s));
ecs_Result *dyn_GetObject _ANSI_ARGS_((ecs_Server *s, char *Id));
ecs_Result *dyn_GetObjectIdFromCoord _ANSI_ARGS_((ecs_Server *s, ecs_Coordinate *coord));
ecs_Result *dyn_UpdateDictionary _ANSI_ARGS_((ecs_Server *s, char *info));
ecs_Result *dyn_GetServerProjection _ANSI_ARGS_((ecs_Server *s));
ecs_Result *dyn_GetGlobalBound _ANSI_ARGS_((ecs_Server *s));
ecs_Result *dyn_SetServerLanguage _ANSI_ARGS_((ecs_Server *s, u_int language));
ecs_Result *dyn_SetServerProjection _ANSI_ARGS_((ecs_Server *s, char *projection));
ecs_Result *dyn_SetRasterConversion _ANSI_ARGS_((ecs_Server *s,
						 ecs_RasterConversion *rc));


/***********************************************************************/


/*
 * client.h --
 *
 * Control dispatch of locals client. Also control cache management and
 * projection changes.
 *
 * Copyright (c) 1995 Logiciels et Applications Scientifiques (L.A.S.) Inc.
 * Il est strictement interdit de publier ou de devoiler le contenu de ce
 * programme sans avoir prealablement obtenu la permission de L.A.S. Inc.
 */

#define CACHEINITSIZE 100
#define COMPARETOLERANCE 0.005    /* For projection conversion, tolerance factor during projection compare */

#define ECS_TTOS 0  /* define the conversion direction (target to source) or (source to target) */
#define ECS_STOT 1


typedef struct ecs_CtlPoint {
  double e1,n1,e2,n2;
  double errorx,errory;
} ecs_CtlPoint;


typedef struct ecs_CtlPoints {
  int nbpts;
  ecs_CtlPoint *pts;
} ecs_CtlPoints;

 
typedef struct ecs_Cache {
  ecs_LayerSelection coverage;   /* coverage descriptor */

  int size;         /* logical cache size */
  int currentpos;   /* current position in cache for getnextobject */
  int allocatedSize;   /* physical size of cache */  
  ecs_Result **o;   /* the cache itself */

  struct ecs_Cache *next; /* linked list successor, NULL if end of list */
  struct ecs_Cache *previous; /* linked list predecessor */
} ecs_Cache;


typedef struct {
  char *url;              /* Client URL, mostly used to see if a client already exist */

  ecs_Cache *cache;       /* linked list of all cached coverage */
  ecs_Cache *selectCache; /* A pointer to the cache selected by SelectLayer */
  ecs_Region cacheRegion; /* mbr region of cache */
  ecs_Region currentRegion; /* mbr of current region */
  ecs_Family currentSelectionFamily; /* Current layer selection type */

  char *tclprocname;      /* attribute callback procedure for tcl */

  PJ *target;             /* source and target projection descriptors */
  PJ *source;
  int isSourceLL;         /* flags to avoid unnecessary computation */
  int isTargetLL;
  int isProjEqual;
  int isCurrentRegionSet;

  ecs_Server s;

} ecs_Client;

#define MAXCLIENT 8

/* Functions used for allocation and deallocation. */

void        cln_FreeClient           _ANSI_ARGS_((ecs_Client **c));
int         cln_AllocClient          _ANSI_ARGS_((char *URL,int *error));

/* API functions */

ecs_Result *cln_CreateClient         _ANSI_ARGS_((int *ClientID,char *url));
ecs_Result *cln_DestroyClient        _ANSI_ARGS_((int ClientID));
ecs_Result *cln_SelectLayer          _ANSI_ARGS_((int ClientID, ecs_LayerSelection *ls));
ecs_Result *cln_ReleaseLayer         _ANSI_ARGS_((int ClientID, ecs_LayerSelection *ls));
void cln_BroadCloseLayers();
ecs_Result *cln_SelectRegion         _ANSI_ARGS_((int ClientID, ecs_Region *gr));
ecs_Result *cln_GetDictionary        _ANSI_ARGS_((int ClientID));
ecs_Result *cln_GetAttributesFormat  _ANSI_ARGS_((int ClientID));
ecs_Result *cln_GetNextObject        _ANSI_ARGS_((int ClientID));
ecs_Result *cln_GetRasterInfo        _ANSI_ARGS_((int ClientID));
ecs_Result *cln_GetObject            _ANSI_ARGS_((int ClientID, char *Id));
ecs_Result *cln_GetObjectIdFromCoord _ANSI_ARGS_((int ClientID, ecs_Coordinate *coord));
ecs_Result *cln_UpdateDictionary     _ANSI_ARGS_((int ClientID, char *info));
ecs_Result *cln_GetGlobalBound       _ANSI_ARGS_((int ClientID));
ecs_Result *cln_SetServerLanguage    _ANSI_ARGS_((int ClientID, u_int language));
ecs_Result *cln_GetServerProjection  _ANSI_ARGS_((int ClientID));
ecs_Result *cln_SetServerProjection  _ANSI_ARGS_((int ClientID, char *projection));
ecs_Result *cln_SetClientProjection  _ANSI_ARGS_((int ClientID, char *projection));
void cln_SetTclProc                  _ANSI_ARGS_((int ClientID, char *tclproc));
char *cln_GetTclProc                 _ANSI_ARGS_((int ClientID));

/* Projection conversion functions */

PJ *cln_ProjInit                     _ANSI_ARGS_((char *d));
int cln_CompareProjections           _ANSI_ARGS_((int ClientID));
int cln_UpdateMaxRegion              _ANSI_ARGS_((int ClientID, double x, double y, ecs_Region *gr, int sens, int first));
int cln_ConvRegion                   _ANSI_ARGS_((int ClientID, ecs_Region *gr, int sens));
int cln_ConvTtoS                     _ANSI_ARGS_((int ClientID, double *X, double *Y));
int cln_ConvStoT                     _ANSI_ARGS_((int ClientID, double *X, double *Y));
int cln_ChangeProjection             _ANSI_ARGS_((int ClientID, ecs_Object *obj));
int cln_ChangeProjectionArea         _ANSI_ARGS_((int ClientID, ecs_Area *obj));
int cln_ChangeProjectionLine         _ANSI_ARGS_((int ClientID, ecs_Line *obj));
int cln_ChangeProjectionPoint        _ANSI_ARGS_((int ClientID, ecs_Point *obj));
int cln_ChangeProjectionMatrix       _ANSI_ARGS_((int ClientID, ecs_Matrix *obj));
int cln_ChangeProjectionImage        _ANSI_ARGS_((int ClientID, ecs_Image *obj));
int cln_ChangeProjectionText         _ANSI_ARGS_((int ClientID, ecs_Text *obj));
int cln_PointValid                   _ANSI_ARGS_((int ClientID, double x, double y));

/* Matrix conversion functions */

int cln_CalcCtlPoint _ANSI_ARGS_((int ClientID, ecs_Region *server_region,
				  int SI, int SJ, ecs_CtlPoint *pt));
int cln_CalcCtlPoints _ANSI_ARGS_((int ClientID, ecs_CtlPoints **pts,
				   char **error_message));
int cln_SetRasterConversion          _ANSI_ARGS_((int ClientID,
						  ecs_CtlPoints **pts,
						  ecs_Resampling resampling,
						  ecs_Transformation trans,
						  char **error_message));
						  

/* URL manipulation */

int cln_GetClientIdFromURL           _ANSI_ARGS_((char *url));
int cln_GetURLList                   _ANSI_ARGS_((char **urllist));

/* Cache functions */

int cln_SetRegionCaches              _ANSI_ARGS_((int ClientID, ecs_Region *GR, char **error_message));
int cln_LoadCache                    _ANSI_ARGS_((int ClientID, ecs_LayerSelection *ls, char **error_message));
int cln_ReleaseCache                 _ANSI_ARGS_((int ClientID, ecs_LayerSelection *ls, char **error_message));
void cln_FreeCache                   _ANSI_ARGS_((ecs_Cache *cache));
ecs_Cache *cln_FoundCache            _ANSI_ARGS_((int ClientID, ecs_LayerSelection *ls));
int cln_CopyObject                   _ANSI_ARGS_((ecs_Result *source, ecs_Result **copy, char **error_message));
int cln_CopyGeometry                 _ANSI_ARGS_((ecs_Object *source, ecs_Object *copy, char **error_message));
int cln_CopyArea                     _ANSI_ARGS_((ecs_Area *source, ecs_Area *copy, char **error_message));
int cln_CopyLine                     _ANSI_ARGS_((ecs_Line *source, ecs_Line *copy, char **error_message));
int cln_CopyPoint                    _ANSI_ARGS_((ecs_Point *source, ecs_Point *copy, char **error_message));
int cln_CopyText                     _ANSI_ARGS_((ecs_Text *source, ecs_Text *copy, char **error_message));
int cln_CopyMatrix                   _ANSI_ARGS_((ecs_Matrix *source, ecs_Matrix *copy, char **error_message));
int cln_CopyImage                    _ANSI_ARGS_((ecs_Image *source, ecs_Image *copy, char **error_message));
void cln_FreeObject                  _ANSI_ARGS_((ecs_Object *obj));

#endif /* ECS_UTIL */


/***********************************************************************/


/*
 * ecsgeo --
 *
 * Make geometric calculations
 *
 * Copyright (c) 1995 Logiciels et Applications Scientifiques (L.A.S.) Inc.
 * Il est strictement interdit de publier ou de devoiler le contenu de ce
 * programme sans avoir prealablement obtenu la permission de L.A.S. Inc.
 */

void ecs_begin_ellipsoid_polygon_area _ANSI_ARGS_((double a, double e2));
double ecs_Q _ANSI_ARGS_((double x));
double ecs_Qbar _ANSI_ARGS_((double x));
double ecs_planimetric_polygon_area _ANSI_ARGS_((int n,ecs_Coordinate *coord));
double ecs_ellipsoid_polygon_area _ANSI_ARGS_((int n,ecs_Coordinate *coord));
double ecs_geodesic_distance _ANSI_ARGS_((double lon1, double lat1, double lon2, double lat2));
double ecs_distance_meters _ANSI_ARGS_((char *projection, double X1, double Y1, double X2, double Y2));
int ecs_CalculateCentroid _ANSI_ARGS_((int nb_segment, ecs_Coordinate *coord,ecs_Coordinate *centroid));

