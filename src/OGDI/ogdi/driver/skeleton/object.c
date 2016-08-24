/*********************************************************************

  CSOURCE_INFORMATION
  
  NAME
     object.c

  DESCRIPTION&
     Implementation of skeleton driver getObject* functions
  END_DESCRIPTION

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

/*
  Database of area. 
  */

int dbareaqty = 2;
dbareatype dbarea[2] = {{1,
			 9,
			 {{598924.242,4921629.735},{594678.030,4920190.341},{594138.258,4917851.326},{595937.500,4915224.432},{599140.152,4915692.235},{601119.318,4916987.689},{601227.273,4920082.386},{600543.561,4920946.023},{598924.242,4921629.735}},
			 6,
			 {{597736.742,4919434.659},{596369.318,4918535.038},{596441.288,4917491.477},{597772.727,4917203.598},{599140.152,4919038.826},{597736.742,4919434.659}},
			 4921629.735,
			 4915224.432,
			 601227.273,
			 594138.258},
			{2,
			 7,
			 {{605653.409,4920586.174},{602918.561,4919074.811},{602486.742,4916771.780},{604142.045,4915548.295},{606517.045,4916591.856},{607956.439,4919722.538},{605653.409,4920586.174}},
			 5,
			 {{605185.606,4918499.053},{603314.394,4917635.417},{603494.318,4916843.750},{605149.621,4917023.674},{605185.606,4918499.053}},
			 4920586.174,
			 4915548.295,
			 605185.606,
			 603314.394}
};

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _getNextObjectArea

   DESCRIPTION
      This function retrieves the Area data objects in a sequential
      access.
      
      In this example, areas of the spearfish database are used and
      each area contains two polygons, the main polygon and other as
      an island. In the current layer, the geographical objects are
      related to an index. If the current position in the index is
      greater than the number of objects in it, then the function will
      indicate that the selection is completed. The attributes will be
      dummy values set in the format defined in GetAttributesFormat.

      The objects are filtered in a way that prevents them to be
      returned if the object is outside the current region. A filter
      could also be applied in the drivers with a filter expression
      like VRF.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void _getNextObjectArea(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  int i;
  char buffer[3];

  /*
    Go to the next valid index position.
    */

  while (dbarea[l->index].north < s->currentRegion.south || 
	 dbarea[l->index].south > s->currentRegion.north ||
	 dbarea[l->index].east < s->currentRegion.west ||
	 dbarea[l->index].west > s->currentRegion.east) {
    l->index++;
    if (l->index >= l->nbfeature) {
      break;
    }
  }

  /*
    If the index is superior to the number of geographical objects in
    the database, the function returns an error message with an error
    code of "2" that will simply indicate the end of the selection.
    Code 1 is for a real error message.
    */

  if (l->index >= l->nbfeature) {
    ecs_SetError(&(s->result),2,"End of selection");
    return;
  }

  /*
    Extract the area at the position l->index. It's a valid polygon.
    */

  /*
    ecs_SetGeomArea define a area in the OGDI. It indicate in the
    ecs_Result structure the quantity of rings presents in the
    area. In this case, the number of rings is 2, the main ring and
    the island.
    */

  ecs_SetGeomArea(&(s->result),2);

  /*
    Define the first ring (ring number 0). When a ring is define, the
    number of points and the centroid must be known. In that case, the
    centroid is undefined (0,0).  Once the ecs_SetGeomAreaRing is
    called, the points are added one by one in the ring with
    ECS_SETGEOMAREACOORD.
    */
   
  ecs_SetGeomAreaRing(&(s->result),0,
		      dbarea[l->index].arealistlength,0.0,0.0);

  for(i=0;i<dbarea[l->index].arealistlength;i++) {
    ECS_SETGEOMAREACOORD((&(s->result)), 0, i, 
			 dbarea[l->index].arealist[i].x,
			 dbarea[l->index].arealist[i].y);
  }

  /*
    Define the second ring (ring number 1). Work like the ring 0.
    */

  ecs_SetGeomAreaRing(&(s->result),1,
		      dbarea[l->index].islandlistlength,0.0,0.0);

  for(i=0;i<dbarea[l->index].islandlistlength;i++) {
    ECS_SETGEOMAREACOORD((&(s->result)), 1, i, 
			 dbarea[l->index].islandlist[i].x,
			 dbarea[l->index].islandlist[i].y);
  }

  /*
    Define the id of the object. Could be any value define for the
    object in the database but it must be unique. The dyn_GetObject
    and dyn_GetObjectIdFromCoord use this identifier to handle the
    objects.
    */

  sprintf(buffer,"%d",l->index);
  ecs_SetObjectId(&(s->result),buffer);

  /*
    Define the bounding box of the object.
    */

  ECS_SETGEOMBOUNDINGBOX((&(s->result)),
			 dbarea[l->index].west,
			 dbarea[l->index].south,
			 dbarea[l->index].east,
			 dbarea[l->index].north);

  /*
    Set the attribute list. It's a string list with a format describe
    by dyn_GetAttributesFormat.
    */

  ecs_SetObjectAttr(&(s->result),"{test d'attributs} 1 10.0");

  l->index++;

  ecs_SetSuccess((&(s->result)));
  return;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _getObjectArea

   DESCRIPTION
      This function retrieves an Area data object with a dynamic
      access.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
	 char *id: A string with the object id number
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void 
_getObjectArea(s,l,id)
     ecs_Server *s;
     ecs_Layer *l;
     char *id;
{ 
  int index;
  int i;
  char buffer[3];

  index = atoi(id);

  if (index < 0 || index >= l->nbfeature) {
    ecs_SetError(&(s->result),1,"Invalid area id");
    return;
  }

  /*
    Extract the area at the position l->index. It's a valid polygon.
    */

  /*
    ecs_SetGeomArea define a area in the OGDI. It indicate in the
    ecs_Result structure the quantity of rings presents in the
    area. In this case, the number of rings is 2, the main ring and
    the island.
    */

  ecs_SetGeomArea(&(s->result),2);
   
  /*
    Define the first ring (ring number 0). When a ring is define, the
    number of points and the centroid must be known. In that case, the
    centroid is undefined (0,0).  Once the ecs_SetGeomAreaRing is
    called, the points are added one by one in the ring with
    ECS_SETGEOMAREACOORD.
    */

  ecs_SetGeomAreaRing(&(s->result),0,
		      dbarea[index].arealistlength,0.0,0.0);

  for(i=0;i<dbarea[index].arealistlength;i++) {
    ECS_SETGEOMAREACOORD((&(s->result)), 0, i, 
			 dbarea[index].arealist[i].x,
			 dbarea[index].arealist[i].y);
  }

  /*
    Define the second ring (ring number 1). Work like the ring 0.
    */

  ecs_SetGeomAreaRing(&(s->result),1,
		      dbarea[index].islandlistlength,0.0,0.0);

  for(i=0;i<dbarea[index].islandlistlength;i++) {
    ECS_SETGEOMAREACOORD((&(s->result)), 1, i, 
			 dbarea[index].islandlist[i].x,
			 dbarea[index].islandlist[i].y);
  }

  /*
    Define the id of the object. Could be any value define for the
    object in the database but it must be unique. The dyn_GetObject
    and dyn_GetObjectIdFromCoord use this identifier to handle the
    objects.
    */

  sprintf(buffer,"%d",index);
  ecs_SetObjectId(&(s->result),buffer);

  /*
    Define the bounding box of the object.
    */

  ECS_SETGEOMBOUNDINGBOX((&(s->result)),
			 dbarea[index].west,
			 dbarea[index].south,
			 dbarea[index].east,
			 dbarea[index].north);


  /*
    Set the attribute list. It's a string list with a format describe
    by dyn_GetAttributesFormat.
    */

  ecs_SetObjectAttr(&(s->result),"{test d'attributs} 1 10.0");

  ecs_SetSuccess((&(s->result)));
  return;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _getObjectIdArea

   DESCRIPTION
      This function retrieves the object id of the nearest area from a
      set of coordinates. Usually, these coordinates are defined by
      the user of the OGDI in order to extract with it a geographical
      object.

      The way the algorithm works in this example is simply by
      checking which point of the area are the nearest from a set of
      coordinates. The identifier of the polygon where the point
      belong is returned.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information.
	 ecs_Coordinate *coord: Object coordinates.
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void _getObjectIdArea(s,l,coord)
     ecs_Server *s;
     ecs_Layer *l;
     ecs_Coordinate *coord;
{
  double distance=0,calcdistance=0,pointdistance;
  int firstobj;
  int index,position;
  char buffer[60];
  int i;

  firstobj = TRUE;
  index = 0;
  
  position = -1;
      
  while (index <= l->nbfeature) {
    
    /* 
       Calculate the shortest distance between the selected set of
       coordinates and the area at the "index" position.
       */
    
    for(i=0;i<dbarea[index].arealistlength;i++) {
      pointdistance = ((dbarea[index].arealist[i].x - coord->x)*(dbarea[index].arealist[i].x - coord->x)+
		       (dbarea[index].arealist[i].y - coord->y)*(dbarea[index].arealist[i].y - coord->y));
      if (i==0)
	calcdistance = pointdistance;
      else
	if (calcdistance > pointdistance)
	  calcdistance = pointdistance;
    }
    
    if (firstobj) {
      distance = calcdistance;
      position = index;
      firstobj = FALSE;
    } else {
      if (calcdistance < distance) {
	distance = calcdistance;
	position = index;
      }
    }
    index++;
  }
  
  if (position < 0) {
    ecs_SetError(&(s->result),2,"No polygons found");
  } else {
    sprintf(buffer,"%d",position);
    if(ecs_SetText(&(s->result),buffer)) {
      ecs_SetSuccess(&(s->result));
    }
  }
}


/*
  Database of lines
  */

int dblineqty = 4;
dblinetype dbline[4] = {{1,
			 9,
			 {{598924.242,4921629.735},{594678.030,4920190.341},{594138.258,4917851.326},{595937.500,4915224.432},{599140.152,4915692.235},{601119.318,4916987.689},{601227.273,4920082.386},{600543.561,4920946.023},{598924.242,4921629.735}},
			 4921629.735,
			 4915224.432,
			 601227.273,
			 594138.258},
			{2,
			 6,
			 {{597736.742,4919434.659},{596369.318,4918535.038},{596441.288,4917491.477},{597772.727,4917203.598},{599140.152,4919038.826},{597736.742,4919434.659}},
			 4919434.659,
			 4917203.598,
			 599140.152,
			 596369.318},
			{3,
			 7,
			 {{605653.409,4920586.174},{602918.561,4919074.811},{602486.742,4916771.780},{604142.045,4915548.295},{606517.045,4916591.856},{607956.439,4919722.538},{605653.409,4920586.174}},
			 4920586.174,
			 4915548.295,
			 605185.606,
			 603314.394},
			{4,
			 5,
			 {{605185.606,4918499.053},{603314.394,4917635.417},{603494.318,4916843.750},{605149.621,4917023.674},{605185.606,4918499.053}},
			 4920586.174,
			 4915548.295,
			 605185.606,
			 603314.394}};

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _getNextObjectLine

   DESCRIPTION
      This function retrieves the Line data objects in a sequential
      access.
      
      In the current layer, the geographical objects are related to an
      index. If the current position in the index is greater than the
      number of objects in it, then the function will indicate that
      the selection is completed. The attributes will be dummy values
      set in the format defined in GetAttributesFormat.

      The objects are filtered in a way that prevents them to be
      returned if the object is outside the current region. A filter
      could also be applied in the drivers with a filter expression
      like VRF.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection informations.
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void _getNextObjectLine(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  int i;
  char buffer[3];

  /*
    Go to the next valid index position.
    */

  while (dbline[l->index].north < s->currentRegion.south || 
	 dbline[l->index].south > s->currentRegion.north ||
	 dbline[l->index].east < s->currentRegion.west ||
	 dbline[l->index].west > s->currentRegion.east) {
    l->index++;
    if (l->index >= l->nbfeature) {
      break;
    }
  }

  /*
    If the index is superior to the number of geographical objects in
    the database, the function returns an error message with an error
    code of "2" that will simply indicate the end of the selection.
    Code 1 is for a real error message.
    */

  if (l->index >= l->nbfeature) {
    ecs_SetError(&(s->result),2,"End of selection");
    return;
  }

  /*
    Extract the line at the position l->index. It's
    a valid polygon.
    */

  ecs_SetGeomLine(&(s->result),dbline[l->index].linelistlength);
   
  for(i=0;i<dbline[l->index].linelistlength;i++) {
    ECS_SETGEOMLINECOORD((&(s->result)), i, 
			 dbline[l->index].linelist[i].x,
			 dbline[l->index].linelist[i].y);
  }

  /*
    Define the id of the object. Could be any value define for the
    object in the database but it must be unique and the dyn_GetObject
    and dyn_GetObjectIdFromCoord use this identifier to handle the
    objects.
    */

  sprintf(buffer,"%d",l->index);
  ecs_SetObjectId(&(s->result),buffer);

  /*
    Define the bounding box of the object.
    */

  ECS_SETGEOMBOUNDINGBOX((&(s->result)),
			 dbline[l->index].west,
			 dbline[l->index].south,
			 dbline[l->index].east,
			 dbline[l->index].north);

  /*
    Set the attribute list. It's a string list with a format describe
    by dyn_GetAttributesFormat.
    */

  ecs_SetObjectAttr(&(s->result),"{test d'attributs} 1 10.0");

  l->index++;

  ecs_SetSuccess((&(s->result)));
  return;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _getObjectLine

   DESCRIPTION
      This function retrieves the Line data objects with a dynamic
      access.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection informations.
	 char *id: A string with the object id number
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void 
_getObjectLine(s,l,id)
     ecs_Server *s;
     ecs_Layer *l;
     char *id;
{
  int index;
  int i;
  char buffer[3];

  index = atoi(id);

  if (index < 0 || index >= l->nbfeature) {
    ecs_SetError(&(s->result),1,"Invalid line id");
    return;
  }

  /*
    Extract the line at the position index. It's a valid polygon.
    */

  ecs_SetGeomLine(&(s->result),dbline[index].linelistlength);
   
  for(i=0;i<dbline[index].linelistlength;i++) {
    ECS_SETGEOMLINECOORD((&(s->result)), i, 
			 dbline[index].linelist[i].x,
			 dbline[index].linelist[i].y);
  }

  sprintf(buffer,"%d",index);
  ecs_SetObjectId(&(s->result),buffer);

  ECS_SETGEOMBOUNDINGBOX((&(s->result)),
			 dbline[index].west,
			 dbline[index].south,
			 dbline[index].east,
			 dbline[index].north);

  ecs_SetObjectAttr(&(s->result),"{test d'attributs} 1 10.0");

  index++;

  ecs_SetSuccess((&(s->result)));
  return;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _getObjectIdLine

   DESCRIPTION
      This function retrieves the object id of the nearest polyline of
      a set of coordinates.

      The way the algorithm works in this example is simply by
      checking which points of the polyline are the nearest from of
      set of coordinates. The identifier of the polyline where the
      point belong is returned,
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
	 ecs_Coordinate *coord: Object coordinate
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void 
_getObjectIdLine(s,l,coord)
     ecs_Server *s;
     ecs_Layer *l;
     ecs_Coordinate *coord;
{
  double distance=0,calcdistance=0,pointdistance;
  int firstobj;
  int index,position;
  char buffer[60];
  int i;

  firstobj = TRUE;
  index = 0;
  
  position = -1;
      
  while (index <= l->nbfeature) {
    
    /* Calculate the shortest distance between the set of coordinates and
       the area at the "index" position */
    
    for(i=0;i<dbline[index].linelistlength;i++) {
      pointdistance = ((dbline[index].linelist[i].x - coord->x)*(dbline[index].linelist[i].x - coord->x)+
		       (dbline[index].linelist[i].y - coord->y)*(dbline[index].linelist[i].y - coord->y));
      if (i==0)
	calcdistance = pointdistance;
      else
	if (calcdistance > pointdistance)
	  calcdistance = pointdistance;
    }
    
    if (firstobj) {
      distance = calcdistance;
      position = index;
      firstobj = FALSE;
    } else {
      if (calcdistance < distance) {
	distance = calcdistance;
	position = index;
      }
    }
    index++;
  }
  
  if (position < 0) {
    ecs_SetError(&(s->result),2,"No line found");
  } else {
    sprintf(buffer,"%d",position);
    if(ecs_SetText(&(s->result),buffer)) {
      ecs_SetSuccess(&(s->result));
    }
  }
}

/*
  Database of points
  */

int dbpointqty = 9;
dbpointtype dbpoint[9] = {{1,
			  {598924.242,4921629.735}},
			 {2,
			  {594678.030,4920190.341}},
			 {3,
			  {594138.258,4917851.326}},
			 {4,
			  {595937.500,4915224.432}},
			 {5,
			  {599140.152,4915692.235}},
			 {6,
			  {601119.318,4916987.689}},
			 {7,
			  {601227.273,4920082.386}},
			 {8,
			  {600543.561,4920946.023}},
			 {9,
			  {598924.242,4921629.735}}};


/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _getNextObjectPoint

   DESCRIPTION
      This function retrieves the Point data objects in a sequential
      access.
      
      In the current layer, the geographical objects are related to an
      index. If the current position in the index is greater than the
      number of objects in it, then the function will indicate that
      the selection is completed. The attributes will be dummy values
      set in the format defined in GetAttributesFormat.

      An object is filtered in a way that prevents them to be returned
      if the object is outside the current region. A filter could also
      be apply in the drivers with a filter expression like VRF.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by OGDI API
	 ecs_Layer *l: Layer selection infos
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void 
_getNextObjectPoint(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  char buffer[3];

  /*
    Go to the next valid index position.
    */

  while (dbpoint[l->index].geopoint.y < s->currentRegion.south || 
	 dbpoint[l->index].geopoint.y > s->currentRegion.north ||
	 dbpoint[l->index].geopoint.x < s->currentRegion.west ||
	 dbpoint[l->index].geopoint.x > s->currentRegion.east) {
    l->index++;
    if (l->index >= l->nbfeature) {
      break;
    }
  }

  /*
    If the index is superior to the number of geographical objects in
    the database, the function returns an error message with an error
    code of "2" that will simply indicate the end of the selection.
    Code 1 is for a real error message.
    */

  if (l->index >= l->nbfeature) {
    ecs_SetError(&(s->result),2,"End of selection");
    return;
  }

  /*
    Extract the line at the position l->index. It's
    a valid polygon.
    */

  ecs_SetGeomPoint(&(s->result),dbpoint[l->index].geopoint.x,dbpoint[l->index].geopoint.y);
   
  sprintf(buffer,"%d",l->index);
  ecs_SetObjectId(&(s->result),buffer);

  ECS_SETGEOMBOUNDINGBOX((&(s->result)),
			 dbpoint[l->index].geopoint.x,
			 dbpoint[l->index].geopoint.y,
			 dbpoint[l->index].geopoint.x,
			 dbpoint[l->index].geopoint.y);

  ecs_SetObjectAttr(&(s->result),"{test d'attributs} 1 10.0");

  l->index++;

  ecs_SetSuccess((&(s->result)));
  return;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _getObjectPoint

   DESCRIPTION
      This function retrieves the Point data objects with a dynamic 
      access.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by OGDI API
	 ecs_Layer *l: Layer selection infos
	 char *id: A string with the object id number
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */


void 
_getObjectPoint(s,l,id)
     ecs_Server *s;
     ecs_Layer *l;
     char *id;
{
  int index;
  char buffer[3];

  index = atoi(id);

  if (index < 0 || index >= l->nbfeature) {
    ecs_SetError(&(s->result),1,"Invalid point id");
    return;
  }

  ecs_SetGeomPoint(&(s->result),dbpoint[index].geopoint.x,dbpoint[index].geopoint.y);
   
  sprintf(buffer,"%d",index);
  ecs_SetObjectId(&(s->result),buffer);

  ECS_SETGEOMBOUNDINGBOX((&(s->result)),
			 dbpoint[index].geopoint.x,
			 dbpoint[index].geopoint.y,
			 dbpoint[index].geopoint.x,
			 dbpoint[index].geopoint.y);

  ecs_SetObjectAttr(&(s->result),"{test d'attributs} 1 10.0");

  ecs_SetSuccess((&(s->result)));
  return;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _getObjectIdPoint

   DESCRIPTION
      This function retrieves the object id of the nearest point
      object from a set of coordinates
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by OGDI API
	 ecs_Layer *l: Layer selection infos
	 ecs_Coordinate *coord: Object coordinate
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void 
_getObjectIdPoint(s,l,coord)
     ecs_Server *s;
     ecs_Layer *l;
     ecs_Coordinate *coord;
{
  double distance,calcdistance;
  int firstobj;
  int index,position;
  char buffer[60];

  firstobj = TRUE;
  index = 0;
  distance = 0.0;
  
  position = -1;
      
  while (index <= l->nbfeature) {

    /* 
       Calculate the shortest distance between the set of given
       coordinates and a point at the "index" position.
       */
    
    calcdistance = ((dbpoint[index].geopoint.x - coord->x)*(dbpoint[index].geopoint.x - coord->x)+
		    (dbpoint[index].geopoint.y - coord->y)*(dbpoint[index].geopoint.y - coord->y));
    
    if (firstobj) {
      distance = calcdistance;
      position = index;
      firstobj = FALSE;
    } else {
      if (calcdistance < distance) {
	distance = calcdistance;
	position = index;
      }
    }
    index++;
  }
  
  if (position < 0) {
    ecs_SetError(&(s->result),2,"No point found");
  } else {
    sprintf(buffer,"%d",position);
    if(ecs_SetText(&(s->result),buffer)) {
      ecs_SetSuccess(&(s->result));
    }
  }
}

/*
  Text point database.
  */

int dbtextqty = 9;
dbtexttype dbtext[9] = {{1,
			  {598924.242,4921629.735}},
			 {2,
			  {594678.030,4920190.341}},
			 {3,
			  {594138.258,4917851.326}},
			 {4,
			  {595937.500,4915224.432}},
			 {5,
			  {599140.152,4915692.235}},
			 {6,
			  {601119.318,4916987.689}},
			 {7,
			  {601227.273,4920082.386}},
			 {8,
			  {600543.561,4920946.023}},
			 {9,
			  {598924.242,4921629.735}}};

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _getNextObjectText

   DESCRIPTION
      This function retrieves the Text data objects in a sequential
      access.
      
      In the current layer, the geographical objects are related to an
      index. If the current position in the index is greater than the
      number of objects in it, then the function will indicate that
      the selection is completed. The attributes will be dummy values
      set in the format defined in GetAttributesFormat.

      The objects are filtered in a way that prevent them to be
      returned if an object is outside the current region. A filter
      could also be applied in the drivers with a filter expression
      like VRF.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void 
_getNextObjectText(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  char buffer[3];

  /*
    Go to the next valid index position.
    */

  while (dbtext[l->index].geopoint.y < s->currentRegion.south || 
	 dbtext[l->index].geopoint.y > s->currentRegion.north ||
	 dbtext[l->index].geopoint.x < s->currentRegion.west ||
	 dbtext[l->index].geopoint.x > s->currentRegion.east) {
    l->index++;
    if (l->index >= l->nbfeature) {
      break;
    }
  }

  /*
    If the index is superior to the number of geographical objects in
    the database, the function returns an error message with an error
    code of "2" that will simply indicate the end of the selection.
    Code 1 is for a real error message.
    */

  if (l->index >= l->nbfeature) {
    ecs_SetError(&(s->result),2,"End of selection");
    return;
  }

  /*
    Extract the line at the position l->index. It's
    a valid polygon.
    */

  sprintf(buffer,"%d",l->index);

  ecs_SetGeomText(&(s->result),dbtext[l->index].geopoint.x,dbtext[l->index].geopoint.y,buffer);
   
  ecs_SetObjectId(&(s->result),buffer);

  ECS_SETGEOMBOUNDINGBOX((&(s->result)),
			 dbtext[l->index].geopoint.x,
			 dbtext[l->index].geopoint.y,
			 dbtext[l->index].geopoint.x,
			 dbtext[l->index].geopoint.y);

  ecs_SetObjectAttr(&(s->result),"{test d'attributs} 1 10.0");

  l->index++;

  ecs_SetSuccess((&(s->result)));
  return;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _getObjectText

   DESCRIPTION
      This function retrieves the Text data objects with a dynamic access.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
	 char *id: A string with the object id number
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void 
_getObjectText(s,l,id)
     ecs_Server *s;
     ecs_Layer *l;
     char *id;
{
  int index;
  char buffer[3];

  index = atoi(id);

  if (index < 0 || index >= l->nbfeature) {
    ecs_SetError(&(s->result),1,"Invalid text id");
    return;
  }

  sprintf(buffer,"%d",index);

  ecs_SetGeomText(&(s->result),dbtext[index].geopoint.x,dbtext[index].geopoint.y,buffer);

  ecs_SetObjectId(&(s->result),buffer);
   
  ECS_SETGEOMBOUNDINGBOX((&(s->result)),
			 dbtext[index].geopoint.x,
			 dbtext[index].geopoint.y,
			 dbtext[index].geopoint.x,
			 dbtext[index].geopoint.y);

  ecs_SetObjectAttr(&(s->result),"{test d'attributs} 1 10.0");

  ecs_SetSuccess((&(s->result)));
  return;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _getObjectIdText

   DESCRIPTION
      This function retrieves the object id of the nearest text point
      of a set of coordinates.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
	 ecs_Coordinate *coord: Object coordinate
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void 
_getObjectIdText(s,l,coord)
     ecs_Server *s;
     ecs_Layer *l;
     ecs_Coordinate *coord;
{
  double distance,calcdistance;
  int firstobj;
  int index,position;
  char buffer[60];

  firstobj = TRUE;
  index = 0;
  distance = 0.0;
  
  position = -1;
      
  while (index <= l->nbfeature) {

    /* Calculate the shortest distance between the coordinate and
       the point at the position "index" */
    
    calcdistance = ((dbtext[index].geopoint.x - coord->x)*(dbtext[index].geopoint.x - coord->x)+
		    (dbtext[index].geopoint.y - coord->y)*(dbtext[index].geopoint.y - coord->y));
    
    if (firstobj) {
      distance = calcdistance;
      position = index;
      firstobj = FALSE;
    } else {
      if (calcdistance < distance) {
	distance = calcdistance;
	position = index;
      }
    }
    index++;
  }
  
  if (position < 0) {
    ecs_SetError(&(s->result),2,"No text found");
  } else {
    sprintf(buffer,"%d",position);
    if(ecs_SetText(&(s->result),buffer)) {
      ecs_SetSuccess(&(s->result));
    }
  }
}

/*
  The matrix extraction

      The matrix extraction with OGDI is different than
      usual. Actually, these functions do not extract a matrix but a
      geographic region matrix. The objective is to fit the matrix
      data selected in this layer into the matrix of the current
      region.

      There are two matrices to handle during this operation. The
      current region matrix that should be considered as a "view" of a
      given region and the matrix region itself. Both regions are
      positionned inside a geospatial view. To fill the current region
      matrix, the value related to each pixel is extracted point by
      point. With a given pixel in the current region matrix, it is
      easy to calculate its geographic position. Once this geographic
      position is found, a resampling is carried out using a
      nearest-neighbor algorithm to extract the value related to this
      position in the matrix region.

      We could do more, suppose your current region is actually a
      region where the projection was previously changed. In that
      case, the OGDI contain a RasterConversion module that will
      automatically set variables in ecs_Server. This is simply a
      point converter, for a given point in the current projection
      matrix, the module indicate which point in the projected matrix
      must be selected. All we need to do is to get the points
      returned by these functions and get the value related to it as
      described before.

      Here is another point about how a raster layer, once transformed
      and resampled, is returned to the OGDI. This is done row by row
      in the current region. The values contained in these rows must
      be non-negative values.

      In the current example, all the functionality about how to use
      the projections, the regions and the requests of the OGDI are
      encapsulated inside the _getNextObjectMatrix and
      _getObjectMatrix. All the driver programmer needs to do is to
      modify the code inside _getValueFromCoord. What this function
      does is to get value at the position i,j in the matrix layer
      (the data itself). It is important that the region of this
      matrix fits exactly the matrix region itself. It is also
      important that the matrix resolution be correctly calculated
      during the layer initialization.

      In our example, the matrix contains four rectangular zone. The
      first zone covers the first quarter of the total number of rows
      and will be of category 1. The second zone cover the second
      quarter of the total number of the matrix rows and will be of
      category 2. The third zone covers the third quarter from the 3/4
      of the total matrix rows and go to the last row. There is a gap
      placed between the half matrix row to the 3/4 to illuatrate the
      value 0. The 0 value is a non-existing category, all 0 values
      are considered as empty pixels, and simply indicates those
      pixels are not members of the layer matrix.
  */


/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _getNextObjectMatrix

   DESCRIPTION
      Extract a row from the position of l->index and return it to
      OGDI.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void 
_getNextObjectMatrix(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  int i,i2,j2;
  char buffer[128];
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  int totalcol,totalrow;
  int value;
  double pos;

  totalcol = (int) ((s->currentRegion.east - s->currentRegion.west)/s->currentRegion.ew_res);
  totalrow = (int) ((s->currentRegion.north - s->currentRegion.south)/s->currentRegion.ns_res);

  lpriv->offsetx = (int) ((s->currentRegion.west - lpriv->matrixregion.west)/lpriv->matrixregion.ew_res);
  lpriv->offsety = (int) ((lpriv->matrixregion.north - s->currentRegion.north)/lpriv->matrixregion.ns_res);

  if (l->index >= totalrow) {
    ecs_SetError(&(s->result),2,"End of selection");
    return;
  }

  ecs_SetGeomMatrix(&(s->result),totalcol);

  if (s->rasterconversion.isProjEqual) {
    for (i=0; i<totalcol; i++) {
      value = _calcPosValue(s,l,i,l->index);
      ECS_SETGEOMMATRIXVALUE((&(s->result)),i,value);
    }
  } else {
    for (i=0; i<totalcol; i++) {
      i2 = ECSGETI(s,((double) l->index),((double)i));
      j2 = ECSGETJ(s,((double) l->index),((double)i));
      value = _calcPosValue(s,l,j2,i2);
      
      ECS_SETGEOMMATRIXVALUE((&(s->result)),i,value);
    }
  }
  
  sprintf(buffer,"%d",l->index);
  if (!ecs_SetObjectId(&(s->result),buffer)) {
    return;
  }

  pos = s->currentRegion.north - l->index*s->currentRegion.ns_res;
  ECS_SETGEOMBOUNDINGBOX((&(s->result)),s->currentRegion.west,
			 pos+s->currentRegion.ns_res,
			 s->currentRegion.east,pos)


  l->index++;
  ecs_SetSuccess(&(s->result));
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _getObjectMatrix

   DESCRIPTION
      This function retrieves the Matrix data row for the position in
      "id".
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
	 char *id: A string with the object id number
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */


void 
_getObjectMatrix(s,l,id)
     ecs_Server *s;
     ecs_Layer *l;
     char *id;
{
  int index;
  int i,i2,j2;
  char buffer[128];
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  int totalcol,totalrow;
  int value;
  double pos;

  index = atoi(id);

  totalcol = (int) ((s->currentRegion.east - s->currentRegion.west)/s->currentRegion.ew_res);
  totalrow = (int) ((s->currentRegion.north - s->currentRegion.south)/s->currentRegion.ns_res);

  lpriv->offsetx = (int) ((s->currentRegion.west - lpriv->matrixregion.west)/lpriv->matrixregion.ew_res);
  lpriv->offsety = (int) ((lpriv->matrixregion.north - s->currentRegion.north)/lpriv->matrixregion.ns_res);

  if (index < 0 || index >= totalrow) {
    ecs_SetError(&(s->result),1,"Invalid matrix line id");
    return;
  }

  ecs_SetGeomMatrix(&(s->result),totalcol);

  if (s->rasterconversion.isProjEqual) {
    for (i=0; i<totalcol; i++) {
      value = _calcPosValue(s,l,i,index);
      ECS_SETGEOMMATRIXVALUE((&(s->result)),i,value);
    }
  } else {
    for (i=0; i<totalcol; i++) {
      i2 = ECSGETI(s,((double) index),((double)i));
      j2 = ECSGETJ(s,((double) index),((double)i));
      value = _calcPosValue(s,l,j2,i2);
      
      ECS_SETGEOMMATRIXVALUE((&(s->result)),i,value);
    }
  }
  
  sprintf(buffer,"%d",index);
  if (!ecs_SetObjectId(&(s->result),buffer)) {
    return;
  }

  pos = s->currentRegion.north - index*s->currentRegion.ns_res;
  ECS_SETGEOMBOUNDINGBOX((&(s->result)),s->currentRegion.west,
			 pos+s->currentRegion.ns_res,
			 s->currentRegion.east,pos)


  ecs_SetSuccess(&(s->result));
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _getObjectIdMatrix

   DESCRIPTION
      This function retrieves the pixel value from a set of
      coordinates.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
	 ecs_Coordinate *coord: Object coordinate
   END_PARAMETERS

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

void 
_getObjectIdMatrix(s,l,coord)
     ecs_Server *s;
     ecs_Layer *l;
     ecs_Coordinate *coord;
{
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  char buffer[128];
  int pix_c,pix_r;
  int value;

  pix_c = (int) ((coord->x - lpriv->matrixregion.west) / lpriv->matrixregion.ew_res);
  pix_r = (int) ((coord->y - lpriv->matrixregion.south) / lpriv->matrixregion.ns_res);
  value = 0;
  if (pix_c >= 0 && pix_c < lpriv->matrixwidth && 
      pix_r >= 0 && pix_r < lpriv->matrixheight) {

    value = _getValueFromCoord(s,l,pix_c,pix_r);
  }
    
  sprintf(buffer,"%d",value);
  if(ecs_SetText(&(s->result),buffer)) {
    ecs_SetSuccess(&(s->result));
  }    
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _calcPosValue

   DESCRIPTION
      Get the value of a position i,j in the current region matrix.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
	 int i: Row in the matrix
	 int j: Column in the matrix
   END_PARAMETERS

   RETURN_VALUE
      int : The category value found by this function

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

int _calcPosValue(s,l,i,j)
     ecs_Server *s;
     ecs_Layer *l;
     int i;
     int j;
{
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  int pix_c,pix_r;
  register int value;

  /*
    Calculate the geographic position in the current region matrix for
    the point i,j.
   */

  /*
  pos_x = s->currentRegion.west + i*s->currentRegion.ew_res;
  pos_y = s->currentRegion.north - j*s->currentRegion.ns_res;


    With geographic region calculated, found the nearest pixel
    position where fall this coordinate.


  pix_c = (int) ((pos_x - lpriv->matrixregion.west) / lpriv->matrixregion.ew_res);
  pix_r = (int) ((pos_y - lpriv->matrixregion.south) / lpriv->matrixregion.ns_res);
    */

  pix_c = ((int) (i*s->currentRegion.ew_res/lpriv->matrixregion.ew_res))+lpriv->offsetx;
  pix_r = ((int) (j*s->currentRegion.ns_res/lpriv->matrixregion.ns_res))+lpriv->offsety;

  /*
    Get the value at this pixel position with a special care
    to check if the point fall inside the matrix.
    */

  if ((pix_c>=0) && (pix_c<lpriv->matrixwidth) &&
      (pix_r>=0) && (pix_r<lpriv->matrixheight)) {
    value = _getValueFromCoord(s,l,pix_c,pix_r);
  } else {
    value = 0;
  }
  
  return value;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      _getValueFromCoord

   DESCRIPTION
      Get the value for a position pix_c,pix_r in the current layer
      matrix.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server info given by the OGDI API
	 ecs_Layer *l: Layer selection information
	 int i: Row in the matrix
	 int j: Column in the matrix
   END_PARAMETERS

   RETURN_VALUE
      int : The category value found by this function

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

int _getValueFromCoord(s,l,pix_c,pix_r)
     ecs_Server *s;
     ecs_Layer *l;
     int pix_c;
     int pix_r;
{
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  int value;

  (void) pix_c;

  if (pix_r < ((double) (lpriv->matrixheight/4.0)))
    value = 1;
  else if (pix_r < ((double) (lpriv->matrixheight/2.0)))
    value = 2;
  else if (pix_r < ((double) (3.0*lpriv->matrixheight/4.0)))
    value = 0;
  else value = 3;

  return value;
}
