/******************************************************************************
 *
 * Component: OGDI Tcl Binding
 * Purpose: Contains all function bindings.
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
 * $Log: ecs_tcl.c,v $
 * Revision 1.6  2007/02/12 16:04:21  cbalint
 *    More warrning fixes in tcl and odbc plugins.
 *
 * Revision 1.5  2001/04/09 15:04:35  warmerda
 * applied new source headers
 *
 */

#include "ecs.h"
#include "tcl.h"

char *ecstcl_message[] = {
  /*  0 */ "wrong number of args: should be ",
  /*  1 */ "URLdescriptor",
  /*  2 */ "URL unknown:",
  /*  3 */ "Family",
  /*  4 */ "Request",
  /*  5 */ "Error: The family appears to be invalid.  Must be one of \"Area, Line, Point, Matrix, Image, Text, Edge, Face, Node, Ring\".",
  /*  6 */ "TclArrayVariable",
  /*  7 */ "Region",
  /*  8 */ "Error: The region is invalid.  It should be in the form {N S E W NS_resolution EW_resolution} where N>S and E>W.",
  /*  9 */ "Error: This doesn't appear to be a valid itcl_class dictionary",
  /* 10 */ "ID",
  /* 11 */ "Projection",
  /* 12 */ "Error: unable to get list of URLs",
  /* 13 */ "TclCallbackProc",
  /* 14 */ "Callback procedure removed.",
  /* 15 */ "LanguageNumber",
  /* 16 */ "This was an unknown return type",
  /* 17 */ "Cannot set the tcl array variable",
  /* 18 */ "DictionaryString",
  /* 19 */ "CompressType",
  /* 20 */ "CompressVersion",
  /* 21 */ "CompressLevel",
  /* 22 */ "CompressBlksize",
  /* 23 */ "CacheSize",
};

struct ecs_UserData {
  int ClientID;
  char *ObjID;
  char *proc;
  char *tclvar;
};

typedef struct ecs_UserData ecs_UserData;

int ecs_SetClientProjectionList        _ANSI_ARGS_((Tcl_Interp *interp,int ClientID, char *list));
int ecs_SetGeoRegionList               _ANSI_ARGS_((Tcl_Interp *interp,ecs_Region *gr,char *list));
int ecs_CreateClientCmd                _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp,int argc, const char **argv));
int ecs_DestroyClientCmd               _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_SelectLayerCmd                 _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_ReleaseLayerCmd                _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_SelectRegionCmd                _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_SelectMaskCmd                  _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_UnSelectMaskCmd                _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_GetDictionaryCmd               _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_GetAttributesFormatCmd         _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_GetNextObjectCmd               _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_GetRasterInfoCmd               _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_GetObjectCmd                   _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_GetObjectIdFromCoordCmd        _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_UpdateDictionaryCmd            _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_GetServerProjectionCmd         _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_GetGlobalBoundCmd              _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_SetClientProjectionCmd         _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_SetServerProjectionCmd         _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_GetURLListCmd                  _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_AssignTclFunctionCmd           _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_SetCacheCmd                    _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_LoadCacheCmd                   _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_ReleaseCacheCmd                _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_SetServerLanguageCmd           _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
int ecs_SetCompressionCmd              _ANSI_ARGS_((ClientData clientData, Tcl_Interp *interp, int argc, const char **argv));
static int _interpEcsResult            _ANSI_ARGS_((Tcl_Interp *interp, ecs_Result *result, ecs_UserData  *userdata));
static int _interpObject               _ANSI_ARGS_((Tcl_Interp *interp, ecs_Result *result, char *buffer, ecs_UserData *userdata));
static int _interpArea                 _ANSI_ARGS_((Tcl_Interp *interp, ecs_Area *area, char *buffer));
static int _interpMatrix               _ANSI_ARGS_((Tcl_Interp *interp, ecs_Matrix *matrix, char *buffer));
static int _interpImage                _ANSI_ARGS_((Tcl_Interp *interp, ecs_Image *image, char *buffer));
static int _interpRegion               _ANSI_ARGS_((Tcl_Interp *interp, ecs_Region *region, char *buffer));
static int _interpRasterInfo           _ANSI_ARGS_((Tcl_Interp *interp, ecs_RasterInfo *raster, char *buffer));
static int _interpCoord                _ANSI_ARGS_((Tcl_Interp *interp, ecs_Coordinate *coord, char *buffer));
static int _interpCategory             _ANSI_ARGS_((Tcl_Interp *interp, ecs_Category *category, char *buffer));
static int _interpObjAttribute         _ANSI_ARGS_((Tcl_Interp *interp, ecs_ObjAttribute *oa, char *buffer));
static int _interpObjAttributeFormat   _ANSI_ARGS_((Tcl_Interp *interp, ecs_ObjAttributeFormat *oaf, char *buffer, ecs_UserData *userdata));
static int _GetLayer                   _ANSI_ARGS_((Tcl_Interp *interp, char *family, char *request, ecs_LayerSelection *layer));
static int _getObjectFromTclProc       _ANSI_ARGS_((Tcl_Interp *interp, char *proc, char *ObjID, char *tclvar, int ClientID));
int _getAttributesFromTCLProc   _ANSI_ARGS_((Tcl_Interp *interp, char *proc, char *tclvar, int ClientID));

int 
Ecs_Init(interp)
Tcl_Interp *interp;		/* Interpreter to add extra commands */
{
  Tcl_CreateCommand(interp, "ecs_CreateClient", 
		    ecs_CreateClientCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_DestroyClient", 
		    ecs_DestroyClientCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_SelectLayer", 
		    ecs_SelectLayerCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_ReleaseLayer", 
		    ecs_ReleaseLayerCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_SelectRegion", 
		    ecs_SelectRegionCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_SelectMask", 
		    ecs_SelectMaskCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_UnSelectMask", 
		    ecs_UnSelectMaskCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_GetDictionary", 
		    ecs_GetDictionaryCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_GetNextObject", 
		    ecs_GetNextObjectCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_GetAttributesFormat", 
		    ecs_GetAttributesFormatCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_GetRasterInfo", 
		    ecs_GetRasterInfoCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_GetObject", 
		    ecs_GetObjectCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_GetObjectIdFromCoord", 
		    ecs_GetObjectIdFromCoordCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_SetClientProjection", 
		    ecs_SetClientProjectionCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_SetServerProjection", 
		    ecs_SetServerProjectionCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_UpdateDictionary", 
		    ecs_UpdateDictionaryCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_GetServerProjection", 
		    ecs_GetServerProjectionCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_GetURLList", 
		    ecs_GetURLListCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_GetGlobalBound", 
		    ecs_GetGlobalBoundCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_AssignTclAttributeCallback", 
		    ecs_AssignTclFunctionCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_LoadCache", 
		    ecs_LoadCacheCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_ReleaseCache", 
		    ecs_ReleaseCacheCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_SetCache", 
		    ecs_SetCacheCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_SetServerLanguage", 
		    ecs_SetServerLanguageCmd, (ClientData) 0, NULL);
  Tcl_CreateCommand(interp, "ecs_SetCompression", 
		    ecs_SetCompressionCmd, (ClientData) 0, NULL);

/*  globalinterp = interp; */

  return TCL_OK;
}

int Ecs_tcl_Init(Tcl_Interp *interp )
{
    return Ecs_Init( interp );
}

/*
 *----------------------------------------------------------------------
 *
 * ecs_Init --
 *
 *	Basically an alias for Ecs_Init.  The load command in the
 *	Sun Tcl distribution wants the initialization routine to
 *	begin with a capital letter.  Oblige it without breaking
 *	the old initialization routine.
 *
 * Results:
 *	A standard TCL result
 *
 *----------------------------------------------------------------------
 */

int 
ecs_Init(interp)
Tcl_Interp *interp;		/* Interpreter to add extra commands */
{
    return Ecs_Init(interp);
}

/********************/

/*
   ecs_CreateClientCmd
   arg1: URL

   Cette fonction cree le client et etablit une connection entre ce client
   et un serveur identifie par URL. Cette fonction s'occupe de la creation
   du client et lui assigne une projection geographique.
   */

int ecs_CreateClientCmd(clientData,interp,argc,argv)
ClientData clientData;
Tcl_Interp *interp;
int argc;
const char **argv;
{
  int ClientID;
  ecs_Result *result;

  if (argc != 2) {
    Tcl_AppendResult(interp, ecstcl_message[0],"\"",
		     argv[0], " ",ecstcl_message[1], "\"", (char *) NULL);
    return(TCL_ERROR);
  }

  /* Creation du client */

  result = cln_CreateClient(&ClientID,(char *)argv[1]);
  if (result->error > 0) {
    _interpEcsResult(interp,result,NULL);
    return TCL_ERROR;
  }
 
  Tcl_AppendResult(interp, argv[1], (char *) NULL);
  return TCL_OK;
}


/********************/

/*
   ecs_DestroyClientCmd
   arg1: URL

   Detruit un client et disconnecte le disconnecte du serveur correspondant
   */

int ecs_DestroyClientCmd(clientData,interp,argc,argv)
ClientData clientData;
Tcl_Interp *interp;
int argc;
const char **argv;
{
  int ClientID;

  if (argc != 2) {
    Tcl_AppendResult(interp, ecstcl_message[0],"\"",
		     argv[0], " ",ecstcl_message[1], "\"", (char *) NULL);
    return(TCL_ERROR);
  }
  
  if ((ClientID = cln_GetClientIdFromURL((char *)argv[1])) < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
  }

  return _interpEcsResult(interp,cln_DestroyClient(ClientID), NULL);

}

/*
  _GetLayer
  arg1: interp
  arg2: Family
  arg3: Request
  arg4: Layer

  creates an ecs_Layer from the arguments
*/

static int _GetLayer(interp,family,request,layer)
     Tcl_Interp *interp;
     char *family;
     char *request;
     ecs_LayerSelection *layer;
{
  char c;
  size_t length;

  c = family[0];
  length = strlen(family);

  layer->Select=request;  
/*  if ((c == 'M') && (strncmp(family, "Meta", length) == 0)) {
    layer->F = Meta;
    return TCL_OK;
  }*/
  if ((c == 'L') && (strncmp(family, "Line", length) == 0)) {
    layer->F = Line;
    return TCL_OK;
  }
  if ((c == 'P') && (strncmp(family, "Point", length) == 0)) {
    layer->F = Point;
    return TCL_OK;
  }
  if ((c == 'A') && (strncmp(family, "Area", length) == 0)) {
    layer->F = Area;
    return TCL_OK;
  }
  if ((c == 'T') && (strncmp(family, "Text", length) == 0)) {
    layer->F = Text;
    return TCL_OK;
  }
  if ((c == 'M') && (strncmp(family, "Matrix", length) == 0)) {
    layer->F = Matrix;
    return TCL_OK;
  }
  if ((c == 'I') && (strncmp(family, "Image", length) == 0)) {
    layer->F = Image;
    return TCL_OK;
  }
  if ((c == 'E') && (strncmp(family, "Edge", length) == 0)) {
    layer->F = Edge;
    return TCL_OK;
  }
  if ((c == 'F') && (strncmp(family, "Face", length) == 0)) {
    layer->F = Face;
    return TCL_OK;
  }
  if ((c == 'N') && (strncmp(family, "Node", length) == 0)) {
    layer->F = Node;
    return TCL_OK;
  }
  if ((c == 'R') && (strncmp(family, "Ring", length) == 0)) {
    layer->F = Ring;
    return TCL_OK;
  }
  Tcl_AppendResult(interp, ecstcl_message[5],(char *) NULL);
  layer= NULL;
  return TCL_ERROR;
}

/********************/




/* 
   ecs_SelectLayerCmd
   arg 1: URL
   arg 2: Family
   arg 3-fin: Request

   Defini le coverage actuel pour un client donne. Ce coverage
   sera considere par toutes les autres fonctions.
   */

int ecs_SelectLayerCmd(clientData,interp,argc,argv)
ClientData clientData;
Tcl_Interp *interp;
int argc;
const char **argv;
{
  ecs_LayerSelection layer;
  int ClientID;

  if (argc != 4) {
    Tcl_AppendResult(interp, ecstcl_message[0],"\"",
		     argv[0], " ",ecstcl_message[1]," ", ecstcl_message[3], " ",
		     ecstcl_message[4], "\"", (char *) NULL);
/*    Tcl_AppendResult(interp, ""wrong # args: should be \"",
                     argv[0], " URLdescriptor Family Request\"", 0);*/
    return(TCL_ERROR);
  }

  ClientID = cln_GetClientIdFromURL((char *)argv[1]);
  if (ClientID < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
  }

  if ((_GetLayer(interp, (char *)argv[2],(char *)argv[3],&layer)) != TCL_OK) {
    /* GetLayer appends its own error message */
    return TCL_ERROR;
  }

  return _interpEcsResult(interp,cln_SelectLayer(ClientID,&layer), NULL);

}

/********************/


/* 
   ecs_ReleaseLayerCmd
   arg 1: URL
   arg 2: Family
   arg 3-fin: Request

   */

int ecs_ReleaseLayerCmd(clientData,interp,argc,argv)
     ClientData clientData;
     Tcl_Interp *interp;
     int argc;
     const char **argv;
{
  int ClientID;
  ecs_LayerSelection layer;

  if (argc != 4) {
    Tcl_AppendResult(interp, ecstcl_message[0],"\"",
		     argv[0], " ",ecstcl_message[1]," ", ecstcl_message[3], " ",
		     ecstcl_message[4], "\"", (char *) NULL);
/*    Tcl_AppendResult(interp, "wrong # args: should be \"",
                     argv[0], " URLdescriptor Family Request ...\"", 0); */
    return(TCL_ERROR);
  }

  if ((ClientID = cln_GetClientIdFromURL((char *)argv[1])) < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
  }

  if ((_GetLayer(interp, (char *)argv[2],(char *)argv[3], &layer)) != TCL_OK) {
    /* GetLayer returns its own error msg to the interp */
    return TCL_ERROR;
  }

  return _interpEcsResult(interp,cln_ReleaseLayer(ClientID,&layer), NULL);

}

/********************/

/*
   ecs_SelectRegionCmd
   arg 1: URL
   arg 2: {Region}

   Met a jour la region geographique courante. Tous les objects geographiques
   seront contenu dans cette region.
   */

int ecs_SelectRegionCmd(clientData,interp,argc,argv)
ClientData clientData;
Tcl_Interp *interp;
int argc;
const char **argv;
{
  int j;
  int ClientID;
  ecs_Region GR;

  if (argc != 3) {
    Tcl_AppendResult(interp, ecstcl_message[0],"\"",
		     argv[0], " ",ecstcl_message[1], " ",
		     ecstcl_message[7], "\"", (char *) NULL);
/*    Tcl_AppendResult(interp, "wrong # args: should be \"",
                     argv[0], " URLdescriptor Region\"", 0); */
    return(TCL_ERROR);
  }

  if ((j=ecs_SetGeoRegionList(interp,&GR,(char *)argv[2])) == ECS_FAILURE) {
    /* region is invalid */
    Tcl_AppendResult(interp,ecstcl_message[8], (char *) NULL);
    return TCL_ERROR;
  }

  if ((ClientID = cln_GetClientIdFromURL((char *)argv[1])) < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
  }
  
  return _interpEcsResult(interp,cln_SelectRegion(ClientID,&GR), NULL);

/*  Tcl_AppendResult(interp, argv[2]," is selected for ", argv[1], (char *) NULL); */
}

/********************/

int ecs_SelectMaskCmd(clientData,interp,argc,argv)
     ClientData clientData;
     Tcl_Interp *interp;
     int argc;
     const char **argv;
{
  int largc;
  const char **largv;
  int pargc;
  const char **pargv;
  ecs_FeatureRing poly;
  int i;
  int isInclusive;
  int ClientID;

  if (argc != 4) {
    Tcl_AppendResult(interp,
	   "ecs_SelectMask url {isMaskInclusive (0 or 1)} {list of points {X Y} } ", 
		     (char *) NULL);
    return TCL_ERROR;
  }

  if ((ClientID = cln_GetClientIdFromURL((char *)argv[1])) < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
  }

  if (Tcl_GetInt(interp, argv[2], &isInclusive) != TCL_OK) {
    return TCL_ERROR;
  }
    
  /* 
     Convert the points into a ecs_FeatureRing 
     */

  Tcl_ResetResult(interp);
  if ((Tcl_SplitList(interp,argv[3],&largc,&largv) != TCL_OK) ||
      (largc < 0)) {
    return TCL_ERROR;
  }

  poly.c.c_len = largc;
  poly.c.c_val = malloc(sizeof(ecs_Coordinate)*largc);
  if (poly.c.c_val == NULL) {
    ckfree((char *)largv);
    return TCL_ERROR;
  }

  for (i=0;i<largc;i++) {
    Tcl_ResetResult(interp);
    if ((Tcl_SplitList(interp,largv[i],&pargc,&pargv) != TCL_OK) ||
	(pargc < 0)) {
      ckfree((char *)largv);
      free(poly.c.c_val);
      return TCL_ERROR;
    }

    if (Tcl_GetDouble(interp,pargv[0], &(poly.c.c_val[i].x)) != TCL_OK) {
      ckfree((char *)largv);
      ckfree((char *)pargv);
      free(poly.c.c_val);
      return TCL_ERROR;
    }

    if (Tcl_GetDouble(interp,pargv[1], &(poly.c.c_val[i].y)) != TCL_OK) {
      ckfree((char *)largv);
      ckfree((char *)pargv);
      free(poly.c.c_val);
      return TCL_ERROR;
    }

    ckfree((char *)pargv);
  }

  ckfree((char *)largv);

  return _interpEcsResult(interp,cln_SelectMask(ClientID,&poly,isInclusive), NULL);
}


/********************/

int ecs_UnSelectMaskCmd(clientData,interp,argc,argv)
     ClientData clientData;
     Tcl_Interp *interp;
     int argc;
     const char **argv;
{
  int ClientID;

  if (argc != 2) {
    Tcl_AppendResult(interp,
		     "ecs_UnSelectMask url",
		     (char *) NULL);
    return TCL_ERROR;
  }

  if ((ClientID = cln_GetClientIdFromURL((char *)argv[1])) < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
  }

  return _interpEcsResult(interp,cln_UnSelectMask(ClientID), NULL);
}


/********************/

/*
   ecs_GetDictionaryCmd
   arg 1: URL

   Retourne un objet ITCL qui decrit le contenu de la base de donnees
   du serveur.
   */


int ecs_GetDictionaryCmd(clientData,interp,argc,argv)
ClientData clientData;
Tcl_Interp *interp;
int argc;
const char **argv;
{
  ecs_Result *result;
  int ClientID;
  char *dictionary;
  char class[129];
  int class_len;
  Tcl_RegExp itclclass;
  const char *startp, *endp;

  /*  if (!compiled) {
    itclclass = TclRegComp("itcl_class ([A-Za-z_]+)"); */
  itclclass = Tcl_RegExpCompile(interp,"itcl_class ([A-Za-z_]+)");  
    /*    compiled = 1;
  }*/

  if (argc != 2) {
    Tcl_AppendResult(interp, ecstcl_message[0],"\"",
		     argv[0], " ",ecstcl_message[1], "\"", (char *) NULL);

/*    Tcl_AppendResult(interp, "wrong # args: should be \"",
                     argv[0], " URLdescriptor\"", 0); */
    return(TCL_ERROR);
  }

  if ((ClientID = cln_GetClientIdFromURL((char *)argv[1])) < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
  }

  result = cln_GetDictionary(ClientID);
  if (result->error > 0) {
    _interpEcsResult(interp,result,NULL);
    return TCL_ERROR;
  }

/* if (TclRegExec(itclclass,dictionary,NULL) == 0) { */
  dictionary=result->res.ecs_ResultUnion_u.s;
  if (Tcl_RegExpExec(interp,itclclass,dictionary,NULL) <= 0) {
    Tcl_ResetResult(interp);
    Tcl_AppendResult(interp,ecstcl_message[9], (char *) NULL);
    return TCL_ERROR;
  }

  Tcl_RegExpRange(itclclass,0,&startp,&endp);
  class_len = endp - startp;
  if (class_len > 127) {class_len=127;}
/*  strncpy(class,itclclass->startp[1],class_len); */
  strncpy(class,startp,class_len);
  class[class_len] = '\0';
  
  Tcl_AppendElement(interp,class);
  Tcl_AppendElement(interp,dictionary);
  return TCL_OK;
}


/********************/

/*
   ecs_GetAttributesFormatCmd
   arg 1: URL

   Retourne le format des attributs d'un coverage quelconque.
   */

int ecs_GetAttributesFormatCmd(clientData,interp,argc,argv)
ClientData clientData;
Tcl_Interp *interp;
int argc;
const char **argv;
{
  ecs_Result *result;
  ecs_UserData userdata;
  int ClientID;
  
  if (argc != 2) {
    Tcl_AppendResult(interp, ecstcl_message[0],"\"",
		     argv[0], " ",ecstcl_message[1], "\"", (char *) NULL);

/*    Tcl_AppendResult(interp, "wrong # args: should be \"",
                     argv[0], " URLdescriptor\"", 0); */
    return(TCL_ERROR);
  }

  if ((ClientID = cln_GetClientIdFromURL((char *)argv[1])) < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
  }
  userdata.ClientID=ClientID;
  userdata.ObjID=NULL;
  userdata.proc=cln_GetTclProc(ClientID);
  userdata.tclvar=NULL;

  result=cln_GetAttributesFormat(ClientID);
  /* todo: move this to ecsResult */

  return(_interpEcsResult(interp,result,&userdata));
}

/********************/

/*
   ecs_GetNextObjectCmd
   arg 1: URL
   arg 2: tclArrayVariable
   */

int ecs_GetNextObjectCmd(clientData,interp,argc,argv)
ClientData clientData;
Tcl_Interp *interp;
int argc;
const char **argv;
{
  int ClientID;
  ecs_Result *result;
  ecs_UserData userdata;

  if (argc != 3) {
    Tcl_AppendResult(interp, ecstcl_message[0],"\"",
		     argv[0], " ",ecstcl_message[1], " ", ecstcl_message[6], 
		     "\"", (char *) NULL);

/*    Tcl_AppendResult(interp, "wrong # args: should be \"",
                     argv[0], " URLdescriptor tclArrayVariable\"", 0); */
    return(TCL_ERROR);
  }

  if ((ClientID = cln_GetClientIdFromURL((char *)argv[1])) < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
  }
  userdata.ClientID=ClientID;
  userdata.tclvar=(char *)argv[2];
  result = cln_GetNextObject(ClientID);    
  return (_interpEcsResult(interp, result, &userdata));

}

/********************/
/*
   ecs_GetRasterInfoCmd
   arg 1: URL

   */

int ecs_GetRasterInfoCmd(clientData,interp,argc,argv)
ClientData clientData;
Tcl_Interp *interp;
int argc;
const char **argv;
{
  int ClientID;
  ecs_Result *result;

  if (argc != 2) {
    Tcl_AppendResult(interp, ecstcl_message[0],"\"",
		     argv[0], " ",ecstcl_message[1], "\"", (char *) NULL);

/*     Tcl_AppendResult(interp, "wrong # args: should be \"",
                     argv[0], " URLdescriptor\"", 0); */
    return(TCL_ERROR);
  }

  if ((ClientID = cln_GetClientIdFromURL((char *)argv[1])) < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
  }

  result= cln_GetRasterInfo(ClientID);

  return(_interpEcsResult(interp,result,NULL));    

}

/*******************/

static int _getObjectFromTclProc(interp, proc, ObjID, tclvar, ClientID) 
     Tcl_Interp *interp;
     char *proc;
     char *ObjID;
     char *tclvar;
     int ClientID;
{
  char buffer[256];
  sprintf(buffer,"%s %d %s %s", proc, ClientID, ObjID, tclvar);
  return (Tcl_Eval(interp, buffer));
}

/*******************/

static int _getAttributesFromTclProc(interp, proc, tclvar, ClientID) 
     Tcl_Interp *interp;
     char *proc;
     char *tclvar;
     int ClientID;
{
  char buffer[256];
  sprintf(buffer,"%s %d %s{} {}", proc, ClientID, tclvar);
  return (Tcl_Eval(interp, buffer));
}


/********************/

/*
   ecs_GetObjectCmd
   arg 1: URL
   arg 2: 
   */

int ecs_GetObjectCmd(clientData,interp,argc,argv)
ClientData clientData;
Tcl_Interp *interp;
int argc;
const char **argv;
{
  int ClientID;
  ecs_Result *result;
  ecs_UserData userdata;
  
  if (argc != 4) {
    Tcl_AppendResult(interp, ecstcl_message[0],"\"",
		     argv[0], " ",ecstcl_message[1], " ", ecstcl_message[10], " ", 
		     ecstcl_message[6], "\"", (char *) NULL);
/*    Tcl_AppendResult(interp, "wrong # args: should be \"",
                     argv[0], " URLdescriptor ID tclArrayVariable\"", 0); */
    return(TCL_ERROR);
  }

  if ((ClientID = cln_GetClientIdFromURL((char *)argv[1])) < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
  }

  result = cln_GetObject(ClientID,(char *)argv[2]);
  /* if there is an error in GetObject, return */

  userdata.tclvar=(char *)argv[3];
  userdata.ClientID=ClientID;
  userdata.ObjID=(char *)argv[2];

  return (_interpEcsResult(interp, result, &userdata));
    

}

/********************/

/*
   ecs_GetObjectIdFromCoordCmd
   arg 1: URL
   arg 2: X
   arg 3: Y

   Retourne l'identifiant de l'objet se trouve le plus pres
   du point X,Y dans le coverage courant, peu importe le
   type de l'objet.
   */

int ecs_GetObjectIdFromCoordCmd(clientData,interp,argc,argv)
ClientData clientData;
Tcl_Interp *interp;
int argc;
const char **argv;
{
  int ClientID;
  ecs_Coordinate coordinate;

  if (argc != 4) {
        Tcl_AppendResult(interp, ecstcl_message[0],"\"",
		     argv[0], " ",ecstcl_message[1], " X Y\"", (char *) NULL);

/*    Tcl_AppendResult(interp, "wrong # args: should be \"",
                     argv[0], " URLdescriptor X Y\"", 0); */
    return(TCL_ERROR);
  }

  if ((ClientID = cln_GetClientIdFromURL((char *)argv[1])) < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
  }
  
  coordinate.x=atof(argv[2]);
  coordinate.y=atof(argv[3]);

  return _interpEcsResult(interp, cln_GetObjectIdFromCoord(ClientID,&coordinate),NULL);

}

/********************/

/*
   ecs_UpdateDictionaryCmd
   arg 1: URL
   arg 2: ?dictionary String?
   Retourne une liste de toutes les coverages geographiques
   disponibles du cote du serveur. Le dictionnaire de donnees
   retourne par GetDictionary est en mesure de lire ces
   informations.
   */

int ecs_UpdateDictionaryCmd(clientData,interp,argc,argv)
ClientData clientData;
Tcl_Interp *interp;
int argc;
const char **argv;
{
  int ClientID;

  if (argc < 2 || argc > 3)  {
    Tcl_AppendResult(interp, ecstcl_message[0],"\"",
		     argv[0], " ",ecstcl_message[1], " ?", 
		     ecstcl_message[18], "?\"", (char *) NULL);

/*     Tcl_AppendResult(interp, "wrong # args: should be \"",
                     argv[0], " URLdescriptor dictionaryString\"", 0); */
    return(TCL_ERROR);
  }

  if ((ClientID = cln_GetClientIdFromURL((char *)argv[1])) < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
  }
  if (argc == 2) {
    return _interpEcsResult(interp,cln_UpdateDictionary(ClientID,NULL),NULL);    
  } else {
    return _interpEcsResult(interp,cln_UpdateDictionary(ClientID,(char *)argv[2]),NULL);    
  }
}


/********************/

/*
   ecs_GetServerProjectionCmd
   arg 1: URL

   Retourne la projection geographique d'un serveur donne.
   */

int ecs_GetServerProjectionCmd(clientData,interp,argc,argv)
ClientData clientData;
Tcl_Interp *interp;
int argc;
const char **argv;
{
  int ClientID;

  if (argc != 2) {
    Tcl_AppendResult(interp, ecstcl_message[0],"\"",
		     argv[0], " ",ecstcl_message[1], "\"", (char *) NULL);

/*     Tcl_AppendResult(interp, "wrong # args: should be \"",
                     argv[0], " URLdescriptor\"", 0); */
    return(TCL_ERROR);
  }
  if ((ClientID = cln_GetClientIdFromURL((char *)argv[1])) < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
  }

  return _interpEcsResult(interp, cln_GetServerProjection(ClientID),NULL);

}

/********************/

/*
   ecs_GetGlobalBoundCmd
   arg 1: URL

   Retourne la region geographique globale des donnees contenu par
   le serveur.
   */

int ecs_GetGlobalBoundCmd(clientData,interp,argc,argv)
ClientData clientData;
Tcl_Interp *interp;
int argc;
const char **argv;
{
  int ClientID;

  if (argc != 2) {
    Tcl_AppendResult(interp, ecstcl_message[0],"\"",
		     argv[0], " ",ecstcl_message[1], "\"", (char *) NULL);
/*    Tcl_AppendResult(interp, "wrong # args: should be \"",
                     argv[0], " URLdescriptor\"", 0); */
    return(TCL_ERROR);
  }

  if ((ClientID = cln_GetClientIdFromURL((char *)argv[1])) < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
   }

  return _interpEcsResult(interp,cln_GetGlobalBound(ClientID),NULL);
  return TCL_OK;
}

/********************/

/*
   ecs_SetClientProjectionCmd
   arg 1: URL
   arg 2-fin: Projection

   Met a jour la projection geographique du client.
   */

int ecs_SetClientProjectionCmd(clientData,interp,argc,argv)
ClientData clientData;
Tcl_Interp *interp;
int argc;
const char **argv;
{
  int ClientID;
  ecs_Result *result;

  if (argc != 3) {
    Tcl_AppendResult(interp, ecstcl_message[0],"\"",
		     argv[0], " ",ecstcl_message[1], " ", 
		     ecstcl_message[11],"\"", (char *) NULL);    
/*    Tcl_AppendResult(interp, "wrong # args: should be \"",
                     argv[0], " URLdescriptor Projection\"", 0); */
    return(TCL_ERROR);
  }

  if ((ClientID = cln_GetClientIdFromURL((char *)argv[1])) < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
  }

  result = cln_SetClientProjection(ClientID,(char *)argv[2]);
  _interpEcsResult(interp,result,NULL);    
  if(result->error > 0) {
    return TCL_ERROR;
  }

  return TCL_OK;

}

/********************/

/*
   ecs_SetServerProjectionCmd
   arg 1: URL
   arg 2-fin: Projection

   Met a jour la projection geographique du client.
   */

int ecs_SetServerProjectionCmd(clientData,interp,argc,argv)
ClientData clientData;
Tcl_Interp *interp;
int argc;
const char **argv;
{
  int ClientID;

  if (argc != 3) {
    Tcl_AppendResult(interp, ecstcl_message[0],"\"",
		     argv[0], " ",ecstcl_message[1], " ", 
		     ecstcl_message[11],"\"", (char *) NULL);    
/*    Tcl_AppendResult(interp, "wrong # args: should be \"",
                     argv[0], " URLdescriptor Projection\"", 0); */
    return(TCL_ERROR);
  }

  if ((ClientID = cln_GetClientIdFromURL((char *)argv[1])) < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
  }

  return _interpEcsResult(interp, cln_SetServerProjection(ClientID,(char *)argv[2]),NULL);

  /*
  Tcl_AppendResult(interp, "projection ",argv[2]," is selected for ", argv[1], (
char *) NULL);
  */

}

/********************/

/*
   ecs_GetURLListCmd

   Retourne la liste des URL valides
   */

int ecs_GetURLListCmd(clientData,interp,argc,argv)
ClientData clientData;
Tcl_Interp *interp;
int argc;
const char **argv;
{
  int j;
  char *Liste;

  j = cln_GetURLList(&Liste);

  if (!j) {
    Tcl_AppendResult(interp,ecstcl_message[12], (char *) NULL);
    return TCL_ERROR;
  }

  Tcl_AppendResult(interp,Liste, (char *) NULL);

  return TCL_OK;
}

/********************/

/*
   ecs_AssignTclFunctionCmd
   arg 1: URL
   arg 2: tcl command

   Assigne une fonction TCL pour gerer les attributs pour un coverage
   donne.
   */

int ecs_AssignTclFunctionCmd(clientData,interp,argc,argv)
ClientData clientData;
Tcl_Interp *interp;
int argc;
const char **argv;
{ 
  int ClientID;

  if (argc < 2) {
    Tcl_AppendResult(interp, ecstcl_message[0],"\"",
		     argv[0], " ",ecstcl_message[1], " ?", 
		     ecstcl_message[13],"?\"", (char *) NULL);    

/*    Tcl_AppendResult(interp, "wrong # args: should be \"",
                     argv[0], " URLdescriptor tclcommand \" or \"",
                     argv[0], " URLdescriptor\"", 0); */
    return(TCL_ERROR);
  }

  if ((ClientID = cln_GetClientIdFromURL((char *)argv[1])) < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
  }

  if (argc == 2) {
    cln_SetTclProc(ClientID, NULL);
    Tcl_AppendResult(interp,ecstcl_message[14], (char *) NULL);
    return TCL_OK;
  } else {
    cln_SetTclProc(ClientID,(char *)argv[2]);
    Tcl_AppendResult(interp, argv[2], (char *) NULL);
    return TCL_OK;
  }

  return TCL_OK;

}

/********************/
/*
   ecs_SetCacheCmd
   arg 1: URL
   arg 2: {Region}

   Met a jour la region que seront occupes par les caches.
   */

int ecs_SetCacheCmd(clientData,interp,argc,argv)
     ClientData clientData;
     Tcl_Interp *interp;
     int argc;
     const char **argv;
{
  int j;
  int ClientID;
  ecs_Region GR;
  char *string;

  if (argc != 3) {
    Tcl_AppendResult(interp, ecstcl_message[0],"\"",
		     argv[0], " ",ecstcl_message[1], " ",
		     ecstcl_message[7], "\"", (char *) NULL);

/*    Tcl_AppendResult(interp, "wrong # args: should be \"",
                     argv[0],
                     " URLdescriptor Region\"", 0); */
    return(TCL_ERROR);
  }

  if ((ClientID = cln_GetClientIdFromURL((char *)argv[1])) < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
  }

  if ((j = ecs_SetGeoRegionList(interp,&GR,(char *)argv[2])) != ECS_SUCCESS) {
    Tcl_AppendResult(interp,ecstcl_message[8], (char *) NULL);
    return TCL_ERROR;
  }

  if (!cln_SetRegionCaches(ClientID,&GR,&string)) {
    Tcl_AppendResult(interp,string, (char *) NULL);
    /*"Error: ecs_SetCache failed. ", (char *) NULL); */
    return TCL_ERROR;
  }
  Tcl_AppendResult(interp,argv[2], (char *) NULL);
  return TCL_OK;

  }

/********************/

/*
   ecs_LoadCacheCmd
   arg 1: URL
   arg 2: Family
   arg 3-fin: Request

   Charge les objets geographiques d'une region quelconque
   dans une cache pour accelerer l'acces aux objets geographiques.
   */

int ecs_LoadCacheCmd(clientData,interp,argc,argv)
ClientData clientData;
Tcl_Interp *interp;
int argc;
const char **argv;
{
  int ClientID;
  int j;
  ecs_LayerSelection layer;
  char *errorMsg;

  if (argc != 4) {
    Tcl_AppendResult(interp, ecstcl_message[0],"\"",
		     argv[0], " ",ecstcl_message[1], " ", ecstcl_message[3], " ",
		     ecstcl_message[4], "\"", (char *) NULL);
/*    Tcl_AppendResult(interp, "wrong # args: should be \"",
                     argv[0], " URLdescriptor Family Request\"", 0); */
    return(TCL_ERROR);
  }

  if ((ClientID = cln_GetClientIdFromURL((char *)argv[1])) < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
  }

  if ((_GetLayer(interp,(char *)argv[2],(char *)argv[3],&layer)) != TCL_OK) {
    return TCL_ERROR;
  }

  j = cln_LoadCache(ClientID,&layer,&errorMsg);

  if (!j) {
    Tcl_AppendResult(interp,errorMsg, (char *) NULL);
    return TCL_ERROR;
  }

  Tcl_AppendResult(interp,argv[3], (char *) NULL);

  return TCL_OK;
}

/********************/
/********************/

/*
   ecs_ReleaseCacheCmd
   arg 1: URL
   arg 2: Family
   arg 3-fin: Request

   Detruit une cache
   */

int ecs_ReleaseCacheCmd(clientData,interp,argc,argv)
ClientData clientData;
Tcl_Interp *interp;
int argc;
const char **argv;
{
  int ClientID;
  int j;
  ecs_LayerSelection layer;
  char *errorMsg;

  if (argc != 4) {
    Tcl_AppendResult(interp, ecstcl_message[0],"\"",
		     argv[0], " ",ecstcl_message[1], " ", ecstcl_message[3], " ",
		     ecstcl_message[4], "\"", (char *) NULL);
/*    Tcl_AppendResult(interp, "wrong # args: should be \"",
                     argv[0], " URLdescriptor Family Request\"", 0); */
    return(TCL_ERROR);
  }

  if ((ClientID = cln_GetClientIdFromURL((char *)argv[1])) < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
  }

  if ((_GetLayer(interp,(char *)argv[2],(char *)argv[3],&layer)) != TCL_OK) {
    return TCL_ERROR;
  }

  j = cln_ReleaseCache(ClientID,&layer,&errorMsg);

  if (!j) {
    Tcl_AppendResult(interp,errorMsg, (char *) NULL);
    return TCL_ERROR;
  }

  return TCL_OK;
}

int ecs_SetServerLanguageCmd(clientData,interp,argc,argv)
ClientData clientData;
Tcl_Interp *interp;
int argc;
const char **argv;
{
  int ClientID, num;

  if (argc != 3) {
    Tcl_AppendResult(interp, ecstcl_message[0],"\"",
		     argv[0], " ",ecstcl_message[1], " ", 
		     ecstcl_message[15], "\"", (char *) NULL);

/*    Tcl_AppendResult(interp, "wrong # args: should be \"",
                     argv[0], " URLdescriptor LanguageNumber\"", 0); */
    return(TCL_ERROR);
  }

  if (Tcl_GetInt(interp, argv[2], &num) != TCL_OK) {
    return TCL_ERROR;
  }

  if ((ClientID = cln_GetClientIdFromURL((char *)argv[1])) < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
  }

  return _interpEcsResult(interp, cln_SetServerLanguage(ClientID, num),NULL);

}

/*
 *----------------------------------------------------------------------
 * ecs_SetCompressionCmd -- 
 *
 *	Sets the compression parameters in the server.  Parameters are:
 *	type:    0 = none, 1 = zlib; no others supported right now 
 *	version: 0 is only version supported right now
 *	level:   1-9 where 1 is fastest, 9 compresses best.  If 0, gets
 *		 the server's default compression level.
 *	blksize: Size of the blocks that are compressed at a time.  If 0,
 *		 gets the server's default compression blocksize
 *
 * Results:
 *	Standard Tcl result
 *----------------------------------------------------------------------
 */

int ecs_SetCompressionCmd(clientData,interp,argc,argv)
ClientData clientData;
Tcl_Interp *interp;
int argc;
const char **argv;
{
  int ClientID;
  ecs_Compression c;

  if (argc != 7) {
    Tcl_AppendResult(interp, ecstcl_message[0],"\"",
		     argv[0], " ",ecstcl_message[1], " ", 
		     ecstcl_message[19], " ", ecstcl_message[20], " ",
		     ecstcl_message[21], " ", ecstcl_message[22], " ",
		     ecstcl_message[23], "\"", (char *) NULL);

/*    Tcl_AppendResult(interp, "wrong # args: should be \"",
                     argv[0], " URLdescriptor type version level blksize cachesize\"", 0); */
    return(TCL_ERROR);
  }

  if (Tcl_GetInt(interp, argv[2],(int *) &c.ctype) != TCL_OK) {
    return TCL_ERROR;
  }
  if (Tcl_GetInt(interp, argv[3],(int *) &c.cversion) != TCL_OK) {
    return TCL_ERROR;
  }
  if (Tcl_GetInt(interp, argv[4],(int *) &c.clevel) != TCL_OK) {
    return TCL_ERROR;
  }
  if (Tcl_GetInt(interp, argv[5],(int *) &c.cblksize) != TCL_OK) {
    return TCL_ERROR;
  }
  if (Tcl_GetInt(interp, argv[6],(int *) &c.cachesize) != TCL_OK) {
    return TCL_ERROR;
  }
  c.cfullsize = 0;

  if ((ClientID = cln_GetClientIdFromURL((char *)argv[1])) < 0) {
    /* url unknown */
    Tcl_AppendResult(interp,ecstcl_message[2], " ", argv[1], (char *) NULL);
    return TCL_ERROR;
  }

  return _interpEcsResult(interp, cln_SetCompression(ClientID, &c),NULL);
}


/********************/
/********************/

int ecs_SetGeoRegionList(interp,gr,liste)
     Tcl_Interp *interp;
     ecs_Region *gr;
     char *liste;
{
  int argc;
  const char **argv;
  int code = ECS_FAILURE;

  if (Tcl_SplitList(interp,liste,&argc,&argv) != TCL_OK) {
    return ECS_FAILURE;
  }

  if (argc == 6) {
    gr->north = atof(argv[0]);
    gr->south = atof(argv[1]);
    gr->east = atof(argv[2]);
    gr->west = atof(argv[3]);
    gr->ns_res = atof(argv[4]);
    gr->ew_res = atof(argv[5]);

    if ((gr->north>gr->south) && (gr->east>gr->west) &&
        (gr->ns_res > 0) && (gr->ew_res > 0)) {
      if (((gr->north-gr->south)> gr->ns_res) &&
          ((gr->east-gr->west)> gr->ew_res))
        code = ECS_SUCCESS;
    }
  }

  free((char *) argv);
  return code;

}


/********************/

/*
   _interpEcsResult
   arg 1: interp
   arg 2: result
   arg 3: userdata
   Convert ecs_Result into a string and give it to the interpreter given 
   by interp.  "userdata" is a ptr to a structure of miscellany that can be passed in.
   (if it is null, then there is no data.)
*/
static int _interpEcsResult(interp, result, userdata) 
     Tcl_Interp *interp;
     ecs_Result *result;
     ecs_UserData *userdata;
{

  char buffer[200];

  Tcl_ResetResult(interp);
  /* check for error */
  if (result->error > 0) {
    Tcl_AppendResult(interp,result->message,(char*) NULL);
      /* also set errorCode here */
    sprintf(buffer,"%d",result->error);
    Tcl_SetErrorCode(interp,"ECS",buffer,(char*) NULL); 
    return TCL_ERROR;
  }
  
  /* switch on ecs_ResultUnion */

  switch (result->res.type) {
  case Object:
    _interpObject(interp,result,buffer,userdata);
    break;
  case GeoRegion: 
    _interpRegion(interp, &(result->res.ecs_ResultUnion_u.gr),buffer);
    break;
  case objAttributeFormat: 
    _interpObjAttributeFormat(interp, &(result->res.ecs_ResultUnion_u.oaf),buffer,userdata);
    break;
  case RasterInfo:
    _interpRasterInfo(interp, &(result->res.ecs_ResultUnion_u.ri),buffer);
    break;
  case AText:
    Tcl_AppendResult(interp,result->res.ecs_ResultUnion_u.s, (char *) NULL );
    break;
  default:
    Tcl_AppendResult(interp, "OK", (char *) NULL );
    break;
  }

/*  Tcl_AppendResult(interp, tmp, (char *) NULL ); */

  return TCL_OK;
}

/********************/

/*
   _interpObjAttributeFormat
   arg 1: interp
   arg 2: oaf
   arg 3: a pointer to some space
   arg 4: the user's data
   given an ecs_ObjAttributeFormat, appends whatever is in
   it into interp in the form:
   {<_interpObjAttribute1> <_interpObjAttribute2> ...}
*/

static int _interpObjAttributeFormat(interp, oaf, buffer, userdata)
     Tcl_Interp *interp;
     ecs_ObjAttributeFormat *oaf;
     char *buffer;
     ecs_UserData *userdata;
{
  u_int i;

  if (userdata != NULL && userdata->proc != NULL) {
    return(_getAttributesFromTclProc(interp, userdata->proc, userdata->tclvar, userdata->ClientID));
  }

  for(i=0; i<oaf->oa.oa_len; i++) {
    _interpObjAttribute(interp, &(oaf->oa.oa_val[i]),buffer);
  }
  return TCL_OK;
}

/********************/

/*
   _interpObjAttribute
   arg 1: interp
   arg 2: oa
   arg 3: a pointer to some space
   given an ecs_ObjAttribute, appends whatever is in
   it into interp in the form:
   {name type length precision nullable}
*/

static int _interpObjAttribute(interp, oa, buffer) 
     Tcl_Interp *interp;
     ecs_ObjAttribute *oa;
     char* buffer;
{
   Tcl_AppendResult(interp,"{ ",(char *) NULL);
  Tcl_AppendResult(interp, "\"", oa->name, "\"", (char *) NULL);
  switch (oa->type) {
  case Char:
    Tcl_AppendElement(interp, "CHAR");
    break;
  case Varchar:
    Tcl_AppendElement(interp, "VARCHAR");
    break;
  case Longvarchar:
    Tcl_AppendElement(interp, "LONGVARCHAR");    
    break;    
  case Decimal:
    Tcl_AppendElement(interp, "DECIMAL");    
    break;    
  case Numeric:
    Tcl_AppendElement(interp, "NUMERIC");    
    break;
  case Smallint:
    Tcl_AppendElement(interp, "SMALLINT");    
    break;
  case Integer:
    Tcl_AppendElement(interp, "INTEGER");    
    break;
  case Real:
    Tcl_AppendElement(interp, "REAL");    
    break;
  case Float:
    Tcl_AppendElement(interp, "FLOAT");    
    break;
  case Double:
    Tcl_AppendElement(interp, "DOUBLE");    
    break;
  default:
    Tcl_AppendElement(interp, "UNKNOWNTYPE");        
    break;
  }
  sprintf(buffer," %d %d %d } ",
	  oa->lenght,
	  oa->precision,
	  oa->nullable
	  );
  Tcl_AppendResult(interp,buffer,(char *) NULL);
  
  return TCL_OK;
}

/********************/

/*
   _interpRasterInfo
   arg 1: interp
   arg 2: raster
   arg 3: a pointer to some space
   given an ecs_RasterInfo, appends whatever is in
   it into interp in the form:
   {mincat maxcat} {<_interpCategory1> <_interpCategory2> ...}

*/

static int _interpRasterInfo (interp, raster, buffer) 
     Tcl_Interp *interp;
     ecs_RasterInfo *raster;
     char *buffer;
{
  u_int i;
  sprintf(buffer,"{%ld %ld %d %d}",
	  raster->mincat,
	  raster->maxcat,
	  raster->width,
	  raster->height
	  );
  Tcl_AppendResult(interp, buffer, " { ", (char *) NULL);
    
  for (i=0; i< raster->cat.cat_len; i++) {
    _interpCategory(interp,&(raster->cat.cat_val[i]),buffer);
  }
  Tcl_AppendResult(interp, " }", (char *) NULL);

  return TCL_OK;
  }

/********************/

/*
   _interpCategory
   arg 1: interp
   arg 2: category
   arg 3: a pointer to some space
   given an ecs_Category, appends whatever is in
   it into interp in the form:
   {no_cat r g b label qty}

*/

static int _interpCategory(interp, category, buffer)
     Tcl_Interp *interp;
     ecs_Category *category;
     char *buffer;
{
  Tcl_AppendResult(interp,"{",(char*) NULL);
  sprintf(buffer, "%ld %u %u %u",
	  category->no_cat,
	  category->r,
	  category->g,
	  category->b
	  );
  Tcl_AppendResult(interp, buffer, (char *) NULL);
  Tcl_AppendElement(interp, category->label);
  sprintf(buffer," %lu",category->qty); 
  Tcl_AppendResult(interp, buffer, (char *) NULL);

  Tcl_AppendResult(interp," } ",(char*) NULL);

  return TCL_OK;
}

/********************/
/********************/

/*
   _interpObject
   arg 1: interp
   arg 2: result
   arg 3: a pointer to some space
   Convert ecs_Result, into a tcl string of the form:
   {id attr {xmin ymin xmax ymax} type { <type relative code> }}
   { type Id {<type relative data>} {xmin ymin xmax ymax} }
   and adds lines of attributes into a tcl array whose name is in "tclvar"
*/

static int _interpObject(interp,result,buffer,userdata) 
     Tcl_Interp *interp;
     ecs_Result *result;
     char *buffer;
     ecs_UserData *userdata;
{
  u_int i;
  
  /* switch on the ecs_Geometry family type */
  switch (result->res.ecs_ResultUnion_u.dob.geom.family) {
 
  case Area: /* Area */
    Tcl_AppendResult (interp, "Area ", (char *) NULL);
    Tcl_AppendElement(interp, result->res.ecs_ResultUnion_u.dob.Id);
    _interpArea(interp, &(result->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.area), buffer);
    break;

  case Line: /* Line */
    Tcl_AppendResult (interp, "Line ", (char *) NULL);
    Tcl_AppendElement(interp, result->res.ecs_ResultUnion_u.dob.Id);
    Tcl_AppendResult (interp, " { ", (char *) NULL);
    for (i=0; i<result->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.line.c.c_len; i++) {
      _interpCoord (interp,&(result->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.line.c.c_val[i]),buffer);
    }
    Tcl_AppendResult (interp, " } ", (char *) NULL);
    break;

  case Point: /* Point */
    Tcl_AppendResult (interp, "Point ", (char *) NULL);
    Tcl_AppendElement(interp, result->res.ecs_ResultUnion_u.dob.Id);
    _interpCoord(interp, &(result->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.point.c), buffer);
    break;

  case Matrix: /* Matrix */
    Tcl_AppendResult (interp, "Matrix ", (char *) NULL);
    Tcl_AppendElement(interp, result->res.ecs_ResultUnion_u.dob.Id);
    _interpMatrix(interp, &(result->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.matrix), buffer);
    break;

  case Image: /* Image */
    Tcl_AppendResult (interp, "Image ", (char *) NULL);
    Tcl_AppendElement(interp, result->res.ecs_ResultUnion_u.dob.Id);
    _interpImage(interp, &(result->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.image), buffer);
    break;

  case Text: /* Text */
    Tcl_AppendResult (interp, "Text ", (char *) NULL);
    Tcl_AppendElement(interp, result->res.ecs_ResultUnion_u.dob.Id);
    Tcl_AppendElement (interp, result->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.text.desc);
    _interpCoord (interp, &(result->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.text.c), buffer);
    break;

  case Edge: /* Edge */
    Tcl_AppendResult (interp, "Edge ", (char *) NULL);
    Tcl_AppendElement(interp, result->res.ecs_ResultUnion_u.dob.Id);
    break;

  case Face: /* Face */
    Tcl_AppendResult (interp, "Face ", (char *) NULL);
    Tcl_AppendElement(interp, result->res.ecs_ResultUnion_u.dob.Id);
    break;

  case Node: /* Node */
    Tcl_AppendResult (interp, "Node ", (char *) NULL);
    Tcl_AppendElement(interp, result->res.ecs_ResultUnion_u.dob.Id);
    break;

  case Ring: /* Ring */
    Tcl_AppendResult (interp, "Ring ", (char *) NULL);
    Tcl_AppendElement(interp, result->res.ecs_ResultUnion_u.dob.Id);
    break;

  default:
    Tcl_AppendResult(interp, ecstcl_message[16], (char *) NULL );
    break;    
  }

  /* the bounding box */
  sprintf(buffer,"%lf %lf %lf %lf ",
	  result->res.ecs_ResultUnion_u.dob.xmin,
	  result->res.ecs_ResultUnion_u.dob.ymin,
	  result->res.ecs_ResultUnion_u.dob.xmax,
	  result->res.ecs_ResultUnion_u.dob.ymax
	);
  Tcl_AppendElement(interp, buffer);

  /* Set the array - if tcl proc exists for this client, call it, else use returned attributes. */

  if ((userdata->proc=cln_GetTclProc(userdata->ClientID)) != NULL) {
    return(_getObjectFromTclProc(interp, userdata->proc, userdata->ObjID, userdata->tclvar, userdata->ClientID));
  } else {  
    Tcl_UnsetVar(interp,userdata->tclvar, TCL_GLOBAL_ONLY);
    if (Tcl_SetVar2(interp,userdata->tclvar, "0", result->res.ecs_ResultUnion_u.dob.attr, 
		    TCL_GLOBAL_ONLY) == NULL) {
      Tcl_ResetResult(interp);
/*      sprintf(buffer, "Cannot set tcl array variable ""\"%s\".", userdata->tclvar); */
      Tcl_AppendResult(interp, ecstcl_message[17], " ", userdata->tclvar,".", (char *) NULL);
      return TCL_ERROR;
    };
  }

  return TCL_OK;
}

/********************/

/*
   _interpRegion
   arg 1: interp
   arg 2: region
   arg 3: a pointer to some space
   Convert ecs_Region into a tcl string of the form:
   {north south east west ns_res ew_res}
   
*/

static int _interpRegion(interp, region, buffer)
     Tcl_Interp *interp;
     ecs_Region *region;
     char *buffer;
{
    sprintf(buffer,"%lf %lf %lf %lf %lf %lf", 
	    region->north,
	    region->south,
	    region->east,
	    region->west,
	    region->ns_res,
	    region->ew_res
	    );
    Tcl_AppendResult(interp, buffer, (char *) NULL);
    return TCL_OK;
}

/********************/

/*
   _interpCoordinate
   arg 1: interp
   arg 2: coordinate
   arg 3: a pointer to some space
   Convert ecs_Coordinate into a tcl string of the form:
   {x y} and add to interpreter
   
*/

static int _interpCoord(interp, coord, buffer)
     Tcl_Interp *interp;
     ecs_Coordinate *coord;
     char *buffer;
{
  sprintf(buffer," {%lf %lf} ",	
	  coord->x,
	  coord->y
	  );
  Tcl_AppendResult(interp,buffer, (char *) NULL);
  return TCL_OK;
}

/********************/

/*
   _interpMatrix
   arg 1: interp
   arg 2: matrix
   arg 3: a pointer to some space
   Convert ecs_matrix into a tcl string of the form:
    {width height} {<interpRegion>} { x1 x2 ... xn } 
   
*/

static int _interpMatrix(interp, matrix, buffer)
     Tcl_Interp *interp;
     ecs_Matrix *matrix;
     char *buffer;
{
  u_int i;
/*  sprintf(buffer,"%d %d"
	  matrix->width,
	  matrix->height 
	  );
  Tcl_AppendElement(interp, buffer);
  _interpRegion(interp, &(matrix->blockbound), buffer);
 */  

  Tcl_AppendResult(interp," {", (char*) NULL); 
  for (i=0; i<matrix->x.x_len; i++) {
    sprintf(buffer,"%u ", matrix->x.x_val[i]); 
    Tcl_AppendResult(interp,buffer, (char *) NULL); 
  }
  Tcl_AppendResult(interp,"} ",(char*) NULL); 

  return TCL_OK;
}

/********************/

/*
   _interpImage
   arg 1: interp
   arg 2: image
   arg 3: a pointer to some space
   Convert ecs_image into a tcl string of the form:
    {width height} {<interpRegion>} { x1 x2 ... xn } 
   
*/

static int _interpImage(interp, image, buffer)
     Tcl_Interp *interp;
     ecs_Image *image;
     char *buffer;
{
  u_int i;
/*  sprintf(buffer,"%d %d",
	  image->width,
	  image->height
	  );
  Tcl_AppendElement(interp, buffer);
  _interpRegion(interp, &(image->blockbound), buffer);
*/  
  Tcl_AppendResult(interp," {", (char*) NULL); 
  for (i=0; i<image->x.x_len; i++) {
    sprintf(buffer,"%u ", image->x.x_val[i]); 
    Tcl_AppendResult(interp,buffer, (char *) NULL); 
  }
  Tcl_AppendResult(interp,"} ",(char*) NULL); 

  return TCL_OK;

}


/********************/

/*
   _interpArea
   arg 1: interp
   arg 2: area
   arg 3: a pointer to some space
   Convert ecs_Area into a tcl string of the form:

   { {centroid0.x centroid0.y} {{x0 y0} {x1 y1} ...}
     {centroid1.x centroid1.y} {{x0 y0} {x1 y1} ...}
       ...       
   }

*/

static int _interpArea(interp, area, buffer)
     Tcl_Interp *interp;
     ecs_Area *area;
     char *buffer;
{     
  u_int i,j;
 
  /* FeatureRings */
  Tcl_AppendResult (interp, " {", (char *) NULL);

  for (i=0; i < area->ring.ring_len; i++) {  
    Tcl_AppendResult (interp, " {", (char *) NULL);    
    sprintf(buffer," {%lf %lf} ",area->ring.ring_val[i].centroid.x, area->ring.ring_val[i].centroid.y);
    Tcl_AppendResult (interp, buffer, (char *) NULL);
    
    /* Coordinates */
    Tcl_AppendResult (interp, " {", (char *) NULL);
    
    for (j=0; j < area->ring.ring_val[i].c.c_len; j++) {  
      _interpCoord(interp,&(area->ring.ring_val[i].c.c_val[j]),buffer);
    };
    
    Tcl_AppendResult (interp, "} } ", (char *) NULL);

  };

  Tcl_AppendResult (interp, "} ", (char *) NULL);
  return TCL_OK;
}



