/*********************************************************************

  CSOURCE_INFORMATION
  
  NAME
     skeleton.c

  DESCRIPTION
     Implementation of the skeleton driver
  END_DESCRIPTION

  MOD: Bruno Savard, INFOMAR INC., bsavard@infomar.com, 1998/09/21
  Procedures changed: dyn_CreateServer()
                      dyn_ReleaseLayer()

  END_CSOURCE_INFORMATION

  Copyright (C) 1995 Logiciels et Applications Scientifiques (L.A.S.) Inc
  Permission to use, copy, modify and distribute this software and
  its documentation for any purpose and without fee is hereby granted,
  provided that the above copyright notice appear in all copies, that
  both the copyright notice and this permission notice appear in
  supporting documentation, and that the name of L.A.S. Inc not be used 
  in advertising or publicity pertaining to distribution of the software 
  without specific, written prior permission. L.A.S. Inc. makes no
  representations about the suitability of this software for any purpose.
  It is provided "as is" without express or implied warranty.

  ********************************************************************/

#include "skeleton.h"
#include "datadict.h"

/*
  Goal:
  
  This driver is an example that helps developers to write new OGDI drivers.

  Description:

  The driver is divided in two parts: the usual driver part described
  here and the "invisible part" that handles global operations
  (server.c). That must be seen as an object oriented relationship
  between the server.c and the driver. The server.c is the base class
  where all common operations, common checks and calls to the driver
  functions are done.  The driver must be seen as an object that
  inherits from this base class.  Because we are working in standard
  C, this is not totally "Object oriented".  The functions are seen by
  the OGDI as pointers but the ecs_Server structure must be seen as
  the base class attributes. The ecs_Server structure contains many
  attributes the driver programmer needs to know. Here is the list of
  the attributes inside ecs_Server that needed to be used and
  initialized by the driver.

  void *priv: The private geographic information of the geographic driver 
  (spriv).
  int currentLayer: The current layer in use in the driver
  ecs_Region currentRegion: The current region of the geographic driver
  ecs_Region globalRegion: The global region of the geographic driver
  char *projection: The projection string for in case the projection 
  is undefined in the driver. 
  ecs_Result result: Returned structure to the OGDI user

  The following attributes are handled by server.c. They must not be
  modified by the driver.

  char *hostname: The hostname extracted from the URL
  char *server_type: The server type extracted from the URL
  char *pathname: The path name extracted from the URL
  ecs_RasterConversion rasterconversion: Used to convert rasters in the driver
  ecs_Layer *layer: The table of the layer in use in the driver
  int nblayer: Quantity of layers in layer.

  As you notice, everything is handled by this structure. However,
  this is global information and most of the drivers need to keep more
  information.  For this reason, there is a private structure in the
  ecs_Server (priv).  This is simply a pointer of the private
  information for the driver. There is an example in skeleton.h
  (ServerPrivateData).

  Also, the URL is composed of three parts, the hostname, the driver
  type and the pathname. The pathname is probably the most important
  because it contains all the information a driver programmer really
  needs. To facilitate the operation, the URL was preprocessed and
  this part is already in the pathname attribute of the ecs_Server
  structure.

  The layers
  ----------

  Each time a request is passed to the SelectLayer, a structure is
  created in memory which is the Layer structure. A layer contains all
  the necessary information to handle a set of geographic data,
  whatever the type. To handle a layer and its information, the OGDI
  provides three important functions:

  ecs_SetLayer: Create a layer in the driver and return its number
  ecs_GetLayer: Check if a layer exists and return its number
  ecs_FreeLayer: Remove a layer from the set of layers.

  The layers are contained in the "layer" attribute of the ecs_Server
  structure. We also know the number of layers open and the current
  layer number, which is the last layer called by
  ecs_SelectLayer. Here are the attributes available in ecs_Layer
  useful for the driver programmer:

  ecs_LayerSelection sel: Layer Selection Information
  int index: For GetNextObject, the current object extracted
  int nbfeature: The number of features in a layer. Optional.
  void *priv: The private geographic information of the geographic driver 
  for a geographic layer.

  As you notice, there is an equivalent structure pointer to handle
  information specific to a driver for a particular layer. There is an
  example of this in the skeleton.h (LayerPrivateData).
  */

static void _releaseAllLayers _ANSI_ARGS_((ecs_Server *s));

/* 
   Layer oriented functions are kept in data structure to simplify the code 
   */

LayerMethod layerMethod[11] = {
  /* 0 */      	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Area */	{ _openAreaLayer, _closeAreaLayer, _rewindAreaLayer, _getNextObjectArea, _getObjectArea, _getObjectIdArea },
  /* Line */	{ _openLineLayer, _closeLineLayer, _rewindLineLayer, _getNextObjectLine, _getObjectLine, _getObjectIdLine },
  /* Point */	{ _openPointLayer, _closePointLayer, _rewindPointLayer, _getNextObjectPoint, _getObjectPoint, _getObjectIdPoint },
  /* Matrix */	{ _openMatrixLayer, _closeMatrixLayer, _rewindMatrixLayer, _getNextObjectMatrix, _getObjectMatrix, _getObjectIdMatrix },
  /* Image */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Text */	{ _openTextLayer, _closeTextLayer, _rewindTextLayer, _getNextObjectText, _getObjectText, _getObjectIdText },
  /* Edge */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Face */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Node */	{ NULL, NULL, NULL, NULL, NULL, NULL },
  /* Ring */	{ NULL, NULL, NULL, NULL, NULL, NULL }
};

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_CreateServer

   DESCRIPTION
      This function prepares a new skeleton driver interface to a
      database.  When this operation is completed, the user will be
      able to perform other operations with the other functions of
      this driver.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Driver info given by the OGDI API
	 char *Request: An complete URL string
   END_PARAMETERS

   RETURN_VALUE
      ecs_Result* : Standard return value to OGDI. This is a very complex 
                    structure that handles all the different information
		    that could be returned to the application.

   MOD: Bruno Savard, INFOMAR INC., bsavard@infomar.com, 1998/09/21
   Description:  Change the east bounding value from 600000 to 608000.
                 The previous value did not enclose all the objects.

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

ecs_Result *dyn_CreateServer(s,Request)
     ecs_Server *s;
     char *Request;
{
  register ServerPrivateData *spriv;

  (void) Request;

  /* 
     This code creates the driver private structure. The main purpose
     of this structure is to keep the global information related to
     specific aspects of the geographic information accessed by this
     driver. In this case, the only information initialized is the
     attribute globaldummy. This attribute will be handled like a very
     complex structure.
     */

  spriv = s->priv = (ServerPrivateData *) malloc(sizeof(ServerPrivateData));
  if (s->priv == NULL) {

    ecs_SetError(&(s->result), 1, 
		 "Could not connect to the skeleton driver, not enough memory");
    return &(s->result);		
  }

  spriv->globaldummy = 1;

  /* 
     Extract information from Request. Each OGDI driver gets a
     specific URL that contains necessary information to access the
     geographic data. Most of the time, the URL contains the file path
     to a directory, a file or a database. However, this URL must be
     verified here in order to prevent errors. For more information
     about the format of the URL, please check the current OGDI
     documentation.

     In order to facilitate the operation, the specific information of
     this driver is already extracted from the URL and placed in the
     variable s->pathname.

     For the skeleton driver, the URL will be in the following format
     gltp:/skeleton/dummyinfo.  That mean that s->pathname already
     contains "dummyinfo"; if not, the driver must return an error
     message.
     */
 
  if (strstr(s->pathname,"dummyinfo") == NULL) {
    /* Don't forget to unallocate the previous priv */
    free(s->priv);

    ecs_SetError(&(s->result), 1, 
		 "Incorrect URL format for the skeleton driver.");
    return &(s->result);		    
  }

  /*
     Check the database itself. The first operation a programmer
     should do is to check if the database is in the right
     format. Then, the programmer can perform the index creation to
     the different layers and a database connection.
     */

  /* 
     Extracting the bounding rectangle is a difficult process for most
     of the drivers. The global region could be used as the default
     region but usually, it outlines the full area that contains all
     the geographic information in the database. For the sake of the
     demonstration, the area of Spearfish, South Dakota, will be used
     in the following.

     This region is represented in the UTM projection zone 13. The
     ns_res and the ew_res parameters of the region are the
     north-south and east-west resolution of each pixel. They are used
     to calculate the width and the height of the matrix during its
     extraction. Here, let's take the default size of the matrix to be
     100x100.
     */

  s->globalRegion.north = 4928000;
  s->globalRegion.south = 4914000;
/**MOD START**/
  s->globalRegion.east = 608000;
/**MOD END**/
  s->globalRegion.west = 589000;
  s->globalRegion.ns_res = (s->globalRegion.north - s->globalRegion.south)/100.0;
  s->globalRegion.ew_res = (s->globalRegion.east - s->globalRegion.west)/100.0;

  /*
    Its very important to call ecs_SetSuccess before leaving. This
    operation prepares the returned function with a success
    message. This message simply indicates the correct completion of
    the operation.
    */

  ecs_SetSuccess(&(s->result));
  return &(s->result);
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_DestroyServer

   DESCRIPTION
      Deallocate an existing skeleton driver interface to a database.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
   END_PARAMETERS

   RETURN_VALUE
      ecs_Result* : Standard returned value to OGDI 

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

ecs_Result *dyn_DestroyServer(s)
     ecs_Server *s;
{
  register ServerPrivateData *spriv = s->priv;

  /* 
     Release all layers selection.
     */
  
  _releaseAllLayers(s);
  
  /* 
     Release spriv
     */

  if (spriv != NULL) {
    spriv->globaldummy = 0;    
    free(spriv);
  }

  /*
    It is very important to call ecs_SetSuccess before leaving. This
    operation prepares the returned function with a success
    message. This message simply indicates the correct completion of
    the operation.
    */

  ecs_SetSuccess(&(s->result));
  return &(s->result);
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_SelectLayer

   DESCRIPTION
      Make a layer selection and prepare the driver to receive
      commands related to this layer. A layer is a set of geographic
      objects of a certain type (Line, Area, Matrix, Point, Text,
      etc.) This set could vary from one kind of database to another
      but the role remains the same, i.e. define and prepare a layer
      to perform different operations.

      The selection request is formed by a string and a family. The
      family could be Area, Line, Text, Point, Matrix, etc. The string
      is a description of what to select. For example, VRF contains a
      string of the form FEATURE_NAME@COVERAGE(REQUEST).  That defines
      a specific feature name (ex: roads), a coverage type (ex:
      transportation) and a request which is the operation to perform
      to the feature table (ex: TYPE==double_lane). This string is
      different from one driver to another.

      Ex: roads@transportation(roadtype == doubleline)

      At the end of this operation, if everything goes right, the
      driver will have access to a new LayerPrivateData in memory.  It
      will remain in memory until a ecs_ReleaseLayer is applied to
      this layer. You could open as many layers as you want. The only
      limit is the memory and some system specific limitations (nbr of
      files on DOS).

      For this example, the family information will be used to select
      from various types of selection. However, the string must
      contain "layername" or this operation will return an error
      message.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_LayerSelection *sel: Selection information
   END_PARAMETERS

   RETURN_VALUE
      ecs_Result* : Standard returned value to OGDI 

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

ecs_Result *dyn_SelectLayer(s,sel)
     ecs_Server *s;
     ecs_LayerSelection *sel;
{
  int layer;
  char buffer[100];

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

    /* 
       The operation failed, destroy the layer from the memory.
       */

    ecs_FreeLayer(s,layer);
    ecs_SetError(&(s->result),1,"Not enough memory to allocate layer private data");
    return &(s->result);	
  }

  /* 
     At this point, you could prepare the information related to the
     layer itself. All these operations are encapsulated in the "open"
     attribute function in the layerMethod structure and could vary
     from one family of geographic objects to another.
     */

  if ((layerMethod[s->layer[layer].sel.F].open) == NULL) {
    dyn_ReleaseLayer(s,sel);
    ecs_SetError(&(s->result),1,"Unable to open this layer");
  } else {
    if ((layerMethod[s->layer[layer].sel.F].open)(s,&(s->layer[layer]))) {
      ecs_SetSuccess(&(s->result));
    } else {

      /*
	The dyn_ReleaseLayer will change the content of s->result.
	Don't forget to keep in a buffer the error message before the
	call.
	*/

      if (s->result.message != NULL)
	strcpy(buffer,s->result.message);
      dyn_ReleaseLayer(s,sel);
      ecs_SetError(&(s->result),1,buffer);
    }
  }

  return &(s->result);
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_ReleaseLayer

   DESCRIPTION
      This command will remove all information about a previously
      select layer from the memory.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_LayerSelection *sel: Selection information
   END_PARAMETERS

   RETURN_VALUE
      ecs_Result* : Standard returned value to OGDI 

   MOD: Bruno Savard, INFOMAR INC., bsavard@infomar.com, 1998/09/21
   Description:  Addition of code to free the private layer structure
                 to avoid memory leaks.
                 Addition of information on the purpose of the "close"
                 attribute function.
                 

   END_FUNCTION_INFORMATION
  

   ********************************************************************
   */

ecs_Result *dyn_ReleaseLayer(s,sel)
     ecs_Server *s;
     ecs_LayerSelection *sel;
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

  /* Close all related things to this layer and free, if any, all
     allocated memory that the lpriv structure could contains.
     All these operations are encapsulated in the "close"
     attribute function in the layerMethod structure and could vary
     from one family of geographic objects to another.
     */
  if ((layerMethod[s->layer[s->currentLayer].sel.F].close) != NULL)
  {  
     (layerMethod[s->layer[s->currentLayer].sel.F].close)(s,&(s->layer[s->currentLayer]));
  }

  /* 
     Free the private layer structure pointer.
     */
  if (s->layer[layer].priv != NULL)
  {
     free( s->layer[layer].priv );
     s->layer[layer].priv = NULL;
  }

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

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _releaseAllLayers

   DESCRIPTION
      This local command will whipe out of memory all the active
      layers in this driver
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

static void
_releaseAllLayers(s)
     ecs_Server *s;
{
  int i;

  for (i = 0; i < s->nblayer; ++i)
    dyn_ReleaseLayer(s,&(s->layer[i].sel));
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_SelectRegion

   DESCRIPTION
      This command will change the currently used geographic region.
      The default value of this region is the global region.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Region *gr: Geographical region
   END_PARAMETERS

   RETURN_VALUE
      ecs_Result* : Standard returned value to OGDI 

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

ecs_Result *dyn_SelectRegion(s,gr)
     ecs_Server *s;
     ecs_Region *gr;
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

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_GetDictionary

   DESCRIPTION
      Return the itcl_class object related to this driver.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
   END_PARAMETERS

   RETURN_VALUE
      ecs_Result* : Standard returned value to OGDI 

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

ecs_Result *dyn_GetDictionary(s)
     ecs_Server *s;
{
  if (ecs_SetText(&(s->result),datadict)) {
    ecs_SetSuccess(&(s->result));
  }
  return &(s->result);
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_GetAttributesFormat

   DESCRIPTION
      Return the attribute format of the currently selected layer.

      Each vector object contains a string attribute called attr.
      This string contains the list of attribute values related to the
      geographic object. This function indicates the complete format
      and description of these attributes.  The format of this
      description is very similar to the ODBC format.

      In this example, the attributes format will describe various
      dummy information. These attributes will be described in the
      geographic objects.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
   END_PARAMETERS

   RETURN_VALUE
      ecs_Result* : Standard returned value to OGDI 

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

ecs_Result *dyn_GetAttributesFormat(s)
     ecs_Server *s;
{
  if (s->layer[s->currentLayer].sel.F == Matrix) {
    if (!ecs_SetObjAttributeFormat(&(s->result)))
      return &(s->result);
    ecs_AddAttributeFormat(&(s->result),"category",Integer,5,0,0);
    ecs_AddAttributeFormat(&(s->result),"label",Char,80,0,0);	
  } else {

    if (!ecs_SetObjAttributeFormat(&(s->result)))
      return &(s->result);
    
    /*
      The first attribute is a string of variable length.
      */
    
    if(!ecs_AddAttributeFormat(&(s->result), "Variable string name", Varchar, 0, 0, 0))
    return &(s->result);
    
    /*
      The second attribute is an integer with a maximum length of 10.
      */
    
    if(!ecs_AddAttributeFormat(&(s->result), "Integer name", Integer, 10, 0, 0))
      return &(s->result);
    
    /*
      The third attribute is a float number with a maximum length of
      15 and a precision of 6.
      */
    
    if(!ecs_AddAttributeFormat(&(s->result), "Float name", Float, 15, 6, 0))
      return &(s->result);
    
  }
  ecs_SetSuccess(&(s->result));
  return &(s->result);
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_GetNextObject

   DESCRIPTION
      Return the next object for the current layer. In order to do
      that, an increment (l->index) is used.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
   END_PARAMETERS

   RETURN_VALUE
      ecs_Result* : Standard returned value to OGDI 

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

ecs_Result *dyn_GetNextObject(s)
     ecs_Server *s;
{
  if (layerMethod[s->layer[s->currentLayer].sel.F].getNextObject != NULL) {
    (layerMethod[s->layer[s->currentLayer].sel.F].getNextObject)(s,&(s->layer[s->currentLayer]));
  } else {
    ecs_SetError(&(s->result), 1, "GetNextObject is not implemented for this family");
  }
  return &(s->result);

}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_GetObject

   DESCRIPTION
      Return a requested object for the current layer.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 char *Id: This string contains the object number
   END_PARAMETERS

   RETURN_VALUE
      ecs_Result* : Standard returned value to OGDI 

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

ecs_Result *dyn_GetObject(s,Id)
     ecs_Server *s;
     char *Id;
{
  if (layerMethod[s->layer[s->currentLayer].sel.F].getObject != NULL) {
    (layerMethod[s->layer[s->currentLayer].sel.F].getObject)(s,&(s->layer[s->currentLayer]),Id);
  } else {
    ecs_SetError(&(s->result), 1, "GetObject is not implemented for this family");
  }
  return &(s->result);
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_GetObjectIdFromCoord

   DESCRIPTION
      Return the object id sitting at (or near) a coordinate. Each
      object in the database contain an Id. This function will return
      the id of the nearest object in the current layer to a given set
      of coordinates. Depending of the family type, that id could be a
      vector object id for vector layers or a category for matrix
      layers. For example, if we select a layer of polylines, this
      function will return to the user the nearest polyline from the
      set of coordinates.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Coordinate *coord: Geographical coordinate
   END_PARAMETERS

   RETURN_VALUE
      ecs_Result* : Standard returned value to OGDI 

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

ecs_Result *dyn_GetObjectIdFromCoord(s,coord)
     ecs_Server *s;
     ecs_Coordinate *coord;
{
  if (layerMethod[s->layer[s->currentLayer].sel.F].getObjectIdFromCoord != NULL) {
    (layerMethod[s->layer[s->currentLayer].sel.F].getObjectIdFromCoord)(s,&(s->layer[s->currentLayer]),coord);
  } else {
    ecs_SetError(&(s->result), 1, "GetObjectIdFromCoord is not implemented for this family");
  }
  return &(s->result);
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_UpdateDictionary

   DESCRIPTION
      Return the content of this database data dictionary in a Tcl
      List.

      The content of the database data dictionary could be different
      from one driver to another. That could be a simple list of file
      names to a list of mapsets where each mapset is described by a
      list of data types that is described by a list of datasets.

      Theorically, each driver returns the database data dictionary in
      a different format than the other. However, this is the role of
      the applet (dyn_GetDictionary) to handle this list and to show
      the user the content of this database.

      For those who want to get specific information related to a
      particular part of the database, another argument is
      available. If this argument is empty, the dyn_UpdateDictionary
      should work as usual.
      
      In this example, a set of information lists will be returned.
      The first list will be the list of Matrix layers, the second is
      a list of Area layers, the third is a list of Line layers, the
      forth is a list of Point layers and the last is a list of Text
      layers.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 char *arg: A complementary information. Not used.
   END_PARAMETERS

   RETURN_VALUE
      ecs_Result* : Standard returned value to OGDI 

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

ecs_Result *dyn_UpdateDictionary(s,arg)
     ecs_Server *s;
     char *arg;
{
  (void) arg;
    
  /* Make sure an empty list is returned in all cases */ 

  ecs_SetText(&(s->result),""); 

  /* 
     Matrix list
     */
 
  ecs_AddText(&(s->result), "{ layername dummydatamatrix } ");

  /* 
     Area list
     */
 
  ecs_AddText(&(s->result), "{ layername dummydataarea } ");

  /* 
     Line list
     */
 
  ecs_AddText(&(s->result), "{ layername dummydataline } ");

  /* 
     Point list
     */
 
  ecs_AddText(&(s->result), "{ layername dummydatapoint } ");

  /* 
     Text list
     */
 
  ecs_AddText(&(s->result), "{ layername dummydatatext } ");


  ecs_SetSuccess(&(s->result));
  return &(s->result);
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_GetServerProjection

   DESCRIPTION
      Return the driver cartographic projection. It is important to
      say that the content of this database remains always constant
      for all the geographical information of this database. That
      means the driver can handle one geographic projection at the
      time for a given database and this could not be changed during a
      session. It also means that database must be in a uniform
      projection.
      
      The format of the projection is based on the USGS PROJ.4
      system. The most important information is given in the OGDI
      documentation. For more details, go to the following FTP site:

      ftp://kai.er.usgs.gov/pub/proj.4

      For this example, the projection is UTM, zone 13.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
   END_PARAMETERS

   RETURN_VALUE
      ecs_Result* : Standard returned value to OGDI 

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

ecs_Result *dyn_GetServerProjection(s)
     ecs_Server *s;
{
  ecs_SetText(&(s->result), "+proj=utm +ellps=clrk66 +zone=13");
  
  ecs_SetSuccess(&(s->result));
  return &(s->result);
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_GetGlobalBound

   DESCRIPTION
      Return the database global bounding rectangle.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
   END_PARAMETERS

   RETURN_VALUE
      ecs_Result* : Standard returned value to OGDI 

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

ecs_Result *dyn_GetGlobalBound(s)
     ecs_Server *s;
{
  ecs_SetGeoRegion(&(s->result),s->globalRegion.north, s->globalRegion.south, 
		   s->globalRegion.east, s->globalRegion.west, s->globalRegion.ns_res, 
		   s->globalRegion.ew_res);
  ecs_SetSuccess(&(s->result));
  return &(s->result);
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_SetServerLanguage

   DESCRIPTION
      Set this server language for error message; not yet implemented.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 u_int language: The Microsoft language number
   END_PARAMETERS

   RETURN_VALUE
      ecs_Result* : Standard returned value to OGDI 

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

ecs_Result *dyn_SetServerLanguage(s,language)
     ecs_Server *s;
     u_int language;
{
  (void) language;

  ecs_SetSuccess(&(s->result));
  return &(s->result);
}


/* ----------------------------------------------------------------------
 *  _dyn_SetCompression: 
 *     
 *   No compression is used in local databases.
 * ----------------------------------------------------------------------
 */

ecs_Result *dyn_SetCompression(s,compression)
     ecs_Server *s;
     ecs_Compression *compression;
{
  (void) compression;
  ecs_SetSuccess(&(s->result));
  return &(s->result);
}

/*
*******************************************************************

FUNCTION_INFORMATION

NAME

     dyn_GetRasterInfo

DESCRIPTION

     Return the raster information for the current layer and set a
     category table. The category table elements contain a color
     description, a description of the category and a category number.

     For this example, only three colors will be defined, and only if
     the current layer is a matrix. The matrix will have a size of
     100x100.

END_DESCRIPTION

PRECONDITIONS

     dyn_CreateServer must have been previously called. A SelectLayer
     must have been successfully called previously.

END_PRECONDITIONS

POSTCONDITIONS

     No post conditions

END_POSTCONDITIONS

PARAMETERS

     INPUT
     ecs_Server *s: The driver information

END_PARAMETERS

RETURN_VALUE

     ecs_Result *: The result structure common to all OGDI calls.

END_FUNCTION_INFORMATION

*******************************************************************
*/

ecs_Result *dyn_GetRasterInfo(s)
     ecs_Server *s;
{
  register LayerPrivateData *lpriv;

  if (s->layer[s->currentLayer].sel.F != Matrix) {
    ecs_SetError(&(s->result), 1, "The current layer is not a Matrix");
  }

  lpriv = (LayerPrivateData *) s->layer[s->currentLayer].priv;

  /* 
     Put the table content in RasterInfo here 
     */

  ecs_SetRasterInfo(&(s->result),lpriv->matrixwidth,lpriv->matrixheight);

  /* 
     Add a category called red for the first category 
     */

  ecs_AddRasterInfoCategory(&(s->result),1,255,0,0,"Red",0);

  /* 
     Add a category called green for the second category 
     */

  ecs_AddRasterInfoCategory(&(s->result),2,0,255,0,"Green",0);

  /* 
     Add a category called blue for the third category 
     */

  ecs_AddRasterInfoCategory(&(s->result),3,0,0,255,"Blue",0);

  ecs_SetSuccess(&(s->result));
  return &(s->result);
}
