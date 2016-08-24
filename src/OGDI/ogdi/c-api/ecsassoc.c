/******************************************************************************
 *
 * Component: OGDI Core C API
 * Purpose: Code encapsulating memory allocation, and association of the
 *          structure ecs_Result. 
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
 * $Log: ecsassoc.c,v $
 * Revision 1.6  2016/07/06 08:59:46  erouault
 * ecs_SetError(): display error message on stderr if memory allocation fails
 *
 * Revision 1.5  2016/07/04 17:33:49  erouault
 * Also export ecs_ShouldStopOnError and ecs_SetErrorShouldStop on Windows
 *
 * Revision 1.4  2016/07/04 17:03:12  erouault
 * Error handling: Add a ecs_SetErrorShouldStop() function that can be
 *     used internally when the code is able to recover from an error. The user
 *     may decide if he wants to be resilient on errors by defining OGDI_STOP_ON_ERROR=NO
 *     as environment variable (the default being YES: stop on error).
 *     Add a ecs_SetReportErrorFunction() method to install a custom callback that
 *     will be called when OGDI_STOP_ON_ERROR=YES so that the user code is still
 *     aware of errors that occured. If not defined, the error will be logged in stderr.
 *
 * Revision 1.3  2016/06/28 14:32:45  erouault
 * Fix all warnings about unused variables raised by GCC 4.8
 *
 * Revision 1.2  2001/04/09 15:04:34  warmerda
 * applied new source headers
 *
 */

#include "ecs.h"

ECS_CVSID("$Id: ecsassoc.c,v 1.6 2016/07/06 08:59:46 erouault Exp $");

char memory_error[] = "not enough memory";

#define ALLOCNULLSTRING(p) \
if (p==NULL) { \
    if ((p = malloc(1)) != NULL) { \
	 p[0] = '\0'; \
    } \
} 

/*
   ----------------------------------------------------------------------

   ecs_assoc: The ecsassoc library encapsulate memory allocation and
   association of the structure ecs_Result. This structure is use
   to send complex information to client without major concern from
   the server.

   Each of these method work exactly the same way. They receive an 
   information to add in the ecs_Result and the information is add
   in ecs_Result. If an error occur during this operation, the functions
   will return an error code (FALSE). Otherwise, the function end the
   operation and return TRUE. It is important to handle the error code
   in the calling code, a quick return must be done in these cases. There
   is no need from the calling function to call SetError if an error append
   during association, an error code is automatically set and ready
   to be sent to client.

   ----------------------------------------------------------------------
   */

/*************************************************/

/*
   ----------------------------------------------------------------------

   ecs_SetError

   Set an error code and a message into ecs_Result. This function will
   not reinitialize the ecs_Result structure.

   IN
       int errorcode: Error code to assign to ecs_Result. Usually 1.
                      0 mean a success message.
       char *error_message: The error message to assign to ecs_Result.
       Could be a null value.

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will only update the contain of this structure.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_SetError (r,errorcode,error_message)
     ecs_Result *r;
     int errorcode;
     char *error_message;
{
  r->error = errorcode;
  r->res.type = SimpleError;
  if (r->message != NULL)
    free(r->message);
  if (error_message == NULL) {
    r->message = NULL;
  } else {
    r->message = (char *) malloc(strlen(error_message)+1);
    if (r->message == NULL)
    {
      fprintf(stderr, "Not enough memory in ecs_SetError(): %s\n", error_message); 
      return FALSE;
    }
    strcpy(r->message,error_message);
  }

  ecs_AdjustResult(r);

  return TRUE;
}

/************************************************************************/
/*                     ecs_ShouldStopOnError()                          */
/************************************************************************/

int ecs_ShouldStopOnError()
{
    const char* pszStopOnError = getenv("OGDI_STOP_ON_ERROR");
    if( pszStopOnError == NULL )
        return TRUE;
    if( strcmp(pszStopOnError, "yes") == 0 ||
        strcmp(pszStopOnError, "YES") == 0 )
    {
         return TRUE;
    }
    else if( strcmp(pszStopOnError, "no") == 0 ||
             strcmp(pszStopOnError, "NO") == 0 )
    {
         return FALSE;
    }
    else
    {
         fprintf(stderr, "Unhandled value for OGDI_STOP_ON_ERROR = %s. "
                         "Considering it is YES\n",
                         pszStopOnError);
         return TRUE;
    }
}

/************************************************************************/
/*                     ecs_ShouldStopOnError()                            */
/************************************************************************/

static int ecs_DefaultReportError(int errorcode, const char *error_message)
{
    fprintf(stderr, "Error %d: %s\n", errorcode, error_message);
    return FALSE; /* go on */
}

/************************************************************************/
/*                   ecs_SetReportErrorFunction()                       */
/************************************************************************/

static ReportErrorType pfnReportError = ecs_DefaultReportError;

/* Installs a custom error handler and returns the previous one */
ReportErrorType ecs_SetReportErrorFunction(ReportErrorType pfn)
{
    ReportErrorType oldErrorFunc = pfnReportError;
    pfnReportError = pfn;
    return oldErrorFunc;
}

/************************************************************************/
/*                       ecs_SetErrorShouldStop()                       */
/************************************************************************/

/* ecs_SetErrorShouldStop() is a variant of ecs_SetError() that can be used */
/* when the error can be recovered. By default it will be considered as non */
/* recoverable, and thus return TRUE, and regular ecs_SetError() will be used. */
/* But the user may have  installed its own error handler with */
/* ecs_SetReportErrorFunction() and decide of the appropriate behaviour */
int ecs_SetErrorShouldStop (r,errorcode,error_message)
     ecs_Result *r;
     int errorcode;
     char *error_message;
{
  if( ecs_ShouldStopOnError() )
  {
      ecs_SetError(r,errorcode,error_message);
      return TRUE;
  }
  else
  {
      return pfnReportError(errorcode,error_message);
  }
}

/*
   ----------------------------------------------------------------------

   ecs_Success

   Set an success code and flush the error message previously set. 
   This function will not reinitialize the ecs_Result structure.

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will only update the contain of this structure.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_SetSuccess (r)
     ecs_Result *r;
{
  r->error = 0;
  if (r->message != NULL)
    free(r->message);
  r->message = NULL;

  ecs_AdjustResult(r);

  return TRUE;
}



/*
   ----------------------------------------------------------------------

   ecs_AdjustResult

   This function is there to correct a deficiency of RPC about data 
   strings. If a string in ecs_Result is NULL, the software will crash. 

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will only update the contain of this structure.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_AdjustResult (r)
     ecs_Result *r;
{
  int val,i;

  ALLOCNULLSTRING(r->message);

  switch(r->res.type) {
  case Object: 
    {
      ALLOCNULLSTRING(ECSRESULT(r).dob.Id);
      ALLOCNULLSTRING(ECSRESULT(r).dob.attr);
      if (ECSGEOMTYPE(r) == Text) {
	ALLOCNULLSTRING(ECSGEOM(r).text.desc);
      }
    };
    break;
  case objAttributeFormat: 
    {
      val = ECSRESULT(r).oaf.oa.oa_len;
      if (ECSRESULT(r).oaf.oa.oa_val != NULL) {
	for(i=0;i<val;i++) {
	  ALLOCNULLSTRING(ECSRESULT(r).oaf.oa.oa_val[i].name);
	}
      }
    };
    break;
  case RasterInfo: 
    {
      val = ECSRESULT(r).ri.cat.cat_len;
      if (ECSRESULT(r).ri.cat.cat_val != NULL) {
	for(i=0;i<val;i++) {
	  ALLOCNULLSTRING(ECSRESULT(r).ri.cat.cat_val[i].label);
	}
      }
    };
    break;
  case AText: 
    {
      ALLOCNULLSTRING(ECSRESULT(r).s);
    };
    break;
  default:
    break;
  };

  return TRUE;
}

/*
   ----------------------------------------------------------------------

   ecs_SetObjectId

   Set the attribute Id in object. Before calling this function,
   the caller must call ecs_SetGeomText, ecs_SetGeomPoint, ecs_SetGeomLine,
   ecs_SetGeomArea, ecs_SetGeomMatrix or ecs_SetGeomImage to initialize the
   geographic object.

   IN
       char *id: Identifier of the object. Must be different than NULL.

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will only update the contain of this structure.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_SetObjectId (r,id)
     ecs_Result *r;
     char *id;
{
  if (r->res.type == Object) {
    if (r->res.ecs_ResultUnion_u.dob.Id != NULL)
      free(r->res.ecs_ResultUnion_u.dob.Id);
    r->res.ecs_ResultUnion_u.dob.Id = (char *) malloc(strlen(id)+1);
    if (r->res.ecs_ResultUnion_u.dob.Id == NULL) {
      ecs_SetError(r,1,memory_error);
      return FALSE;
    }
    strcpy(r->res.ecs_ResultUnion_u.dob.Id,id);
  }
  
  return TRUE;
}

/*
   ----------------------------------------------------------------------

   ecs_SetObjectAttr

   Set the attribute attr in object. Before calling this function,
   the caller must call ecs_SetGeomText, ecs_SetGeomPoint, ecs_SetGeomLine,
   ecs_SetGeomArea, ecs_SetGeomMatrix or ecs_SetGeomImage to initialize the
   geographic object.

   IN
       char *attr: This string contain the attribute to set. This
       argument must be different than NULL.

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will only update the contain of this structure.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_SetObjectAttr (r,attr)
     ecs_Result *r;
     char *attr;
{
  if (r->res.type == Object) {
    if (r->res.ecs_ResultUnion_u.dob.attr != NULL)
      free(r->res.ecs_ResultUnion_u.dob.attr);
    r->res.ecs_ResultUnion_u.dob.attr = (char *) malloc(strlen(attr)+1);
    if (r->res.ecs_ResultUnion_u.dob.attr == NULL) {
      ecs_SetError(r,1,memory_error);
      return FALSE;
    }
    strcpy(r->res.ecs_ResultUnion_u.dob.attr,attr);
  }
  
  return TRUE;
}

/* 
   ----------------------------------------------------------------------

   ecs_SetGeoRegion

   Set the geographic region. This function will make a clean up
   of ecs_Result and perform his operation.

   IN
       double north, south, east, west, ns_res, ew_res: Geographic
       region to set.

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will clean up the structure and put his information into it.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_SetGeoRegion (r,north,south,east,west,ns_res,ew_res)
     ecs_Result *r;
     double north;
     double south;
     double east;
     double west;
     double ns_res;
     double ew_res;
{
  ecs_CleanUp(r);
  ECSRESULTTYPE(r) = GeoRegion;
  ECSRESULT(r).gr.north = north;
  ECSRESULT(r).gr.south = south;
  ECSRESULT(r).gr.east = east;
  ECSRESULT(r).gr.west = west;
  ECSRESULT(r).gr.ns_res = ns_res;
  ECSRESULT(r).gr.ew_res = ew_res;

  return TRUE;
}

/* 
   ----------------------------------------------------------------------

   ecs_SetText

   Set the Atext item with a string. This function will make a clean up
   of ecs_Result and perform his operation.

   IN
       char *text: Text to set in the structure

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will clean up the structure and put his information into it.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_SetText (r,text)
     ecs_Result *r;
     char *text;
{
  ecs_CleanUp(r);

  ECSRESULTTYPE(r) = AText;
  ECSRESULT(r).s = (char *) malloc(strlen(text)+1);
  if (ECSRESULT(r).s == NULL) {
    ecs_SetError(r,1,memory_error);
    return FALSE;
  }
  strcpy(ECSRESULT(r).s, text);
  return TRUE;
}

/* 
   ----------------------------------------------------------------------

   ecs_AddText

   Concatenate a text to Atext item. The function ecs_SetText must
   be call before.

   IN
       char *text: Text to concatanate

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will only update the contain of this structure.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_AddText (r,text)
     ecs_Result *r;
     char *text;
{
  char *temp;
  int code;

  temp = ECSRESULT(r).s;
  code = TRUE;
  if ((ECSRESULTTYPE(r) == AText) &&
      (temp != NULL)) {
    ECSRESULT(r).s = NULL;
    ECSRESULT(r).s = (char *) malloc(strlen(text)+strlen(temp)+1);
    if (ECSRESULT(r).s == NULL) {
      ECSRESULT(r).s = temp;
      ecs_SetError(r,1,memory_error);
      return FALSE;
    }
    strcpy(ECSRESULT(r).s, temp);
    strcat(ECSRESULT(r).s, text);
    free(temp);
  } else {
    code = ecs_SetText(r,text);
  }

  return code;
}

/* 
   ----------------------------------------------------------------------

   Set the RasterInfo attribute. This function will make a clean up
   of ecs_Result and perform his operation. 

   IN
       int width, height: Width and height of the raster

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will clean up the structure and put his information into it.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_SetRasterInfo (r,width,height)
     ecs_Result *r;
     int width;
     int height;
{
  ecs_CleanUp(r);

  ECSRESULTTYPE(r) = RasterInfo;

  /* It is impossible to have maxcat < mincat. In this
     case, ecs_AddRasterInfoCategory will set both values
     with the first value. */
  ECSRESULT(r).ri.mincat = 1;
  ECSRESULT(r).ri.maxcat = 0;
  ECSRESULT(r).ri.width = width;
  ECSRESULT(r).ri.height = height;
  ECSRESULT(r).ri.cat.cat_len = 0;
  ECSRESULT(r).ri.cat.cat_val = NULL;
  return TRUE;
}

/*************************************************/

/* 
   ----------------------------------------------------------------------

   ecs_AddRasterInfoCategory

   Add a raster info category. Must be called after ecs_SetRasterInfo

   IN
       long no_cat: Category number
       unsigned int red,green,blue: Color of the category
       char *label: Label of the category
       unsigned long qty: Statistical informations about the 
                          raster. (How many points)

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will only update the contain of this structure.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_AddRasterInfoCategory (r,no_cat,red,green,blue,label,qty)
     ecs_Result *r;
     long no_cat;
     unsigned int red;
     unsigned int green;
     unsigned int blue;
     char *label;
     unsigned long qty;
{
  ecs_Category *ptr;

  /* Allocate a new category */
  ECSRESULT(r).ri.cat.cat_val = 
    (ecs_Category *) realloc(ECSRESULT(r).ri.cat.cat_val,
			     sizeof(ecs_Category)*(ECSRESULT(r).ri.cat.cat_len + 1));
  if (ECSRESULT(r).ri.cat.cat_val == NULL) {
    ecs_SetError(r,1,memory_error);
    return FALSE;
  }
  ECSRESULT(r).ri.cat.cat_len++;
  ptr = &(ECSRESULT(r).ri.cat.cat_val[ECSRESULT(r).ri.cat.cat_len - 1]);

  /* Update mincat and maxcat with no_cat */
  if (ECSRESULT(r).ri.mincat > ECSRESULT(r).ri.maxcat) {
    ECSRESULT(r).ri.mincat = no_cat;
    ECSRESULT(r).ri.maxcat = no_cat;
  } else {
    if (ECSRESULT(r).ri.mincat > no_cat)
      ECSRESULT(r).ri.mincat = no_cat;
    if (ECSRESULT(r).ri.maxcat < no_cat)
      ECSRESULT(r).ri.maxcat = no_cat;
  }

  ptr->no_cat = no_cat;
  ptr->r = red;
  ptr->g = green;
  ptr->b = blue;
  ptr->label = (char *) malloc(strlen(label)+1);
  if (ptr->label == NULL) {
    ecs_SetError(r,1,memory_error);
    return FALSE;
  }
  strcpy(ptr->label,label);
  ptr->qty = qty;

  return TRUE;
}

/* 
   ----------------------------------------------------------------------

   ecs_SetObjAttributeFormat

   Set the objAttributeFormat attribute. This function will make a clean up
   of ecs_Result and perform his operation.

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will clean up the structure and put his information into it.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_SetObjAttributeFormat (r)
     ecs_Result *r;
{
  ecs_CleanUp(r);

  ECSRESULTTYPE(r) = objAttributeFormat;
  ECSRESULT(r).oaf.oa.oa_len = 0;
  ECSRESULT(r).oaf.oa.oa_val = NULL;

  return TRUE;
}

/* 
   ----------------------------------------------------------------------

   ecs_AddAttributeFormat

   Add an attribute format to objAttributeFormat attribute

   IN
       char *name: Name of the attribute
       ecs_AttributeFormat type: 
       int lenght              :   Attribute format info
       int precision           :   (ODBC)
       int nullable            :  

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will only update the contain of this structure.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_AddAttributeFormat (r,name,type,lenght,precision,nullable)
     ecs_Result *r;
     char *name;     
     ecs_AttributeFormat type;
     int lenght;
     int precision;
     int nullable;
{
  ecs_ObjAttribute *ptr;

  /* Allocate a new attribute */
  ECSRESULT(r).oaf.oa.oa_val = 
    (ecs_ObjAttribute *) realloc(ECSRESULT(r).oaf.oa.oa_val,
				 sizeof(ecs_ObjAttribute)*(ECSRESULT(r).oaf.oa.oa_len + 1));
  if (ECSRESULT(r).oaf.oa.oa_val == NULL) {
    ecs_SetError(r,1,memory_error);
    return FALSE;
  }
  ECSRESULT(r).oaf.oa.oa_len++;
  ptr = &(ECSRESULT(r).oaf.oa.oa_val[ECSRESULT(r).oaf.oa.oa_len - 1]);

  ptr->name = (char *) malloc(strlen(name)+1);
  if (ptr->name == NULL) {
    ecs_SetError(r,1,memory_error);
    return FALSE;
  }
  strcpy(ptr->name,name);
  ptr->type = type;
  ptr->lenght = lenght;
  ptr->precision = precision;
  ptr->nullable = nullable;

  return TRUE;
}

/* 
   ----------------------------------------------------------------------

   ecs_SetGeomPoint

   Set a point geographical object. This function will make a clean up
   of ecs_Result and perform his operation.

   IN
       double x,y : Point to assign

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will clean up the structure and put his information into it.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_SetGeomPoint (r,x,y)
     ecs_Result *r;
     double x;
     double y;
{
  ecs_CleanUp(r);

  ECSRESULTTYPE(r) = Object;
  ECSGEOMTYPE(r) = Point;
  r->res.ecs_ResultUnion_u.dob.Id = NULL;
  r->res.ecs_ResultUnion_u.dob.attr = NULL;
  
  ECSGEOM(r).point.c.x = x;
  ECSGEOM(r).point.c.y = y;
  r->res.ecs_ResultUnion_u.dob.xmin = 0.0;
  r->res.ecs_ResultUnion_u.dob.ymin = 0.0;
  r->res.ecs_ResultUnion_u.dob.xmax = 0.0;
  r->res.ecs_ResultUnion_u.dob.ymax = 0.0;

  return TRUE;
}

/* 
   ----------------------------------------------------------------------

   ecs_SetGeomText

   Set a text geographical object. This function will make a clean up
   of ecs_Result and perform his operation.

   IN
       double x,y : Position of the text
       char *desc:  Description string

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will only update the contain of this structure.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_SetGeomText (r,x,y,desc)
     ecs_Result *r;
     double x;
     double y;
     char *desc;
{
  ecs_CleanUp(r);

  ECSRESULTTYPE(r) = Object;
  ECSGEOMTYPE(r) = Text;
  r->res.ecs_ResultUnion_u.dob.Id = NULL;
  r->res.ecs_ResultUnion_u.dob.attr = NULL;
  
  ECSGEOM(r).text.c.x = x;
  ECSGEOM(r).text.c.y = y;
  r->res.ecs_ResultUnion_u.dob.xmin = 0.0;
  r->res.ecs_ResultUnion_u.dob.ymin = 0.0;
  r->res.ecs_ResultUnion_u.dob.xmax = 0.0;
  r->res.ecs_ResultUnion_u.dob.ymax = 0.0;
  if (desc != NULL) {
    ECSGEOM(r).text.desc = (char *) malloc(strlen(desc)+1);
    if (ECSGEOM(r).text.desc == NULL) {
      ecs_SetError(r,1,memory_error);
      return FALSE;
    }
    strcpy(ECSGEOM(r).text.desc,desc);
  } else {
    ECSGEOM(r).text.desc = NULL;
  }
  return TRUE;
}

/* 
   ----------------------------------------------------------------------

   ecs_SetGeomLine

   Set a line geographical object. This function will make a clean up
   of ecs_Result and perform his operation.

   IN
       unsigned int lenght : Number of points in Polyline

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will clean up the structure and put his information into it.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.


   ----------------------------------------------------------------------
   */

int ecs_SetGeomLine (r,lenght)
     ecs_Result *r;
     unsigned int lenght;
{
  ecs_CleanUp(r);
  ECSRESULTTYPE(r) = Object;
  ECSGEOMTYPE(r) = Line;
  r->res.ecs_ResultUnion_u.dob.Id = NULL;
  r->res.ecs_ResultUnion_u.dob.attr = NULL;
  r->res.ecs_ResultUnion_u.dob.xmin = 0.0;
  r->res.ecs_ResultUnion_u.dob.ymin = 0.0;
  r->res.ecs_ResultUnion_u.dob.xmax = 0.0;
  r->res.ecs_ResultUnion_u.dob.ymax = 0.0;
  
  ECSGEOM(r).line.c.c_len = lenght;
  ECSGEOM(r).line.c.c_val = (ecs_Coordinate *) malloc(sizeof(ecs_Coordinate)*lenght);
  if (ECSGEOM(r).line.c.c_val == NULL) {
    ecs_SetError(r,1,memory_error);
    return FALSE;
  }

  return TRUE;
}

/* 
   ----------------------------------------------------------------------

   ecs_SetGeomArea

   Set a area geographical object. This function will make a clean up
   of ecs_Result and perform his operation.

   IN
       unsigned int lenght : Number of rings in this area

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will clean up the structure and put his information into it.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_SetGeomArea (r,lenght)
     ecs_Result *r;
     unsigned int lenght;
{
  ecs_CleanUp(r);
  ECSRESULTTYPE(r) = Object;
  ECSGEOMTYPE(r) = Area;
  r->res.ecs_ResultUnion_u.dob.Id = NULL;
  r->res.ecs_ResultUnion_u.dob.attr = NULL;
  r->res.ecs_ResultUnion_u.dob.xmin = 0.0;
  r->res.ecs_ResultUnion_u.dob.ymin = 0.0;
  r->res.ecs_ResultUnion_u.dob.xmax = 0.0;
  r->res.ecs_ResultUnion_u.dob.ymax = 0.0;  

  ECSGEOM(r).area.ring.ring_len = lenght;
  ECSGEOM(r).area.ring.ring_val = (ecs_FeatureRing *) malloc(sizeof(ecs_FeatureRing)*lenght);
  if (ECSGEOM(r).line.c.c_val == NULL) {
    ecs_SetError(r,1,memory_error);
    return FALSE;
  }

  return TRUE;
}

/*************************************************/

/* 
   ----------------------------------------------------------------------

   ecs_SetGeomAreaRing

   Set a ring into an area object.

   IN
       int position: Position in ring table
       unsigned int lenght: Number of points in the ring
       double centroid_x: 
       double centroid_y: Centroid of the ring      

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will only update the contain of this structure.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_SetGeomAreaRing (r,position,lenght,centroid_x,centroid_y)
     ecs_Result *r;
     int position;
     unsigned int lenght;
     double centroid_x;
     double centroid_y;
{
  ecs_FeatureRing *ptr;

  ptr = &(ECSGEOM(r).area.ring.ring_val[position]);

  ptr->centroid.x = centroid_x;
  ptr->centroid.y = centroid_y;

  ptr->c.c_len = lenght;
  ptr->c.c_val = (ecs_Coordinate *) malloc(sizeof(ecs_Coordinate)*lenght);
  if (ptr->c.c_val == NULL) {
    ecs_SetError(r,1,memory_error);
    return FALSE;
  }

  return TRUE;
}

/* 
   ----------------------------------------------------------------------

   ecs_SetGeomMatrixWithArray

   Set a matrix geographical object with a array of unsigned int already
   allocated. This function will make a clean up of ecs_Result and perform 
   his operation.

   IN
       int size: Size of array
       unsigned int *array: Table of values

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will clean up the structure and put his information into it.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_SetGeomMatrixWithArray (r, size, array)
     ecs_Result *r;
     int size;
     unsigned int *array;
{
  ecs_CleanUp(r);
  ECSRESULTTYPE(r) = Object;
  ECSGEOMTYPE(r) = Matrix;
  r->res.ecs_ResultUnion_u.dob.Id = NULL;
  r->res.ecs_ResultUnion_u.dob.attr = NULL;
  ECSGEOM(r).matrix.x.x_len = size;
  ECSGEOM(r).matrix.x.x_val = (u_int *) malloc(sizeof(u_int)*size);
  if (ECSGEOM(r).matrix.x.x_val == NULL) {
    ecs_SetError(r,1,memory_error);
    return FALSE;
  }
  memcpy(ECSGEOM(r).matrix.x.x_val,array,sizeof(u_int)*size);
  r->res.ecs_ResultUnion_u.dob.xmin = 0.0;
  r->res.ecs_ResultUnion_u.dob.ymin = 0.0;
  r->res.ecs_ResultUnion_u.dob.xmax = 0.0;
  r->res.ecs_ResultUnion_u.dob.ymax = 0.0;

  return TRUE;
}

/* 
   ----------------------------------------------------------------------

   ecs_SetGeomMatrix

   Set a matrix geographical object. This function will make a clean up
   of ecs_Result and perform his operation.

   IN
       int size: Number of colums in this raster row

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will clean up the structure and put his information into it.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_SetGeomMatrix (r, size)
     ecs_Result *r;
     int size;
{
  ecs_CleanUp(r);
  ECSRESULTTYPE(r) = Object;
  ECSGEOMTYPE(r) = Matrix;
  r->res.ecs_ResultUnion_u.dob.Id = NULL;
  r->res.ecs_ResultUnion_u.dob.attr = NULL;
  ECSGEOM(r).matrix.x.x_len = size;
  ECSGEOM(r).matrix.x.x_val = (u_int *) malloc(sizeof(u_int)*size);
  if (ECSGEOM(r).matrix.x.x_val == NULL) {
    ecs_SetError(r,1,memory_error);
    return FALSE;
  }
  r->res.ecs_ResultUnion_u.dob.xmin = 0.0;
  r->res.ecs_ResultUnion_u.dob.ymin = 0.0;
  r->res.ecs_ResultUnion_u.dob.xmax = 0.0;
  r->res.ecs_ResultUnion_u.dob.ymax = 0.0;

  return TRUE;
}

/* 
   ----------------------------------------------------------------------

   ecs_SetGeomImageWithArray

   Set a image geographical object with a array of unsigned int already
   allocated. This function will make a clean up of ecs_Result and 
   perform his operation.

   IN
       int size: Size of array
       unsigned int *array: Table of values

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will clean up the structure and put his information into it.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_SetGeomImageWithArray (r, size, array)
     ecs_Result *r;
     int size;
     unsigned int *array;
{
  ecs_CleanUp(r);
  ECSRESULTTYPE(r) = Object;
  ECSGEOMTYPE(r) = Image;
  r->res.ecs_ResultUnion_u.dob.Id = NULL;
  r->res.ecs_ResultUnion_u.dob.attr = NULL;
  ECSGEOM(r).image.x.x_len = size;
  ECSGEOM(r).image.x.x_val = (u_int *) malloc(sizeof(u_int)*size);
  if (ECSGEOM(r).image.x.x_val == NULL) {
    ecs_SetError(r,1,memory_error);
    return FALSE;
  }
  memcpy(ECSGEOM(r).image.x.x_val,array,sizeof(u_int)*size);
  r->res.ecs_ResultUnion_u.dob.xmin = 0.0;
  r->res.ecs_ResultUnion_u.dob.ymin = 0.0;
  r->res.ecs_ResultUnion_u.dob.xmax = 0.0;
  r->res.ecs_ResultUnion_u.dob.ymax = 0.0;

  return TRUE;
}

/* 
   ----------------------------------------------------------------------

   ecs_SetGeomImage

   Set a image geographical object. This function will make a clean up
   of ecs_Result and perform his operation.

   IN
       int size: Number of colums in this raster row

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will clean up the structure and put his information into it.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_SetGeomImage(r, size)
     ecs_Result *r;
     int size;
{
  ecs_CleanUp(r);
  ECSRESULTTYPE(r) = Object;
  ECSGEOMTYPE(r) = Image;
  r->res.ecs_ResultUnion_u.dob.Id = NULL;
  r->res.ecs_ResultUnion_u.dob.attr = NULL;
  ECSGEOM(r).image.x.x_len = size;
  ECSGEOM(r).image.x.x_val = (u_int *) malloc(sizeof(u_int)*size);
  if (ECSGEOM(r).image.x.x_val == NULL) {
    ecs_SetError(r,1,memory_error);
    return FALSE;
  }
  r->res.ecs_ResultUnion_u.dob.xmin = 0.0;
  r->res.ecs_ResultUnion_u.dob.ymin = 0.0;
  r->res.ecs_ResultUnion_u.dob.xmax = 0.0;
  r->res.ecs_ResultUnion_u.dob.ymax = 0.0;

  return TRUE;
}

/*
   ----------------------------------------------------------------------

   ecs_CleanUp

   Will make a complete clean up and reinitialisation of ecs_Result.

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will clean up the structure and put his information into it.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */


int ecs_CleanUp (r)
     ecs_Result *r;
{
  r->error = 0;

  if (r->message != NULL)
    free(r->message);
  r->message = NULL;

  return ecs_CleanUpResultUnion (&r->res);
}

/*
   ----------------------------------------------------------------------

   ecs_CleanUpResultUnion

   Will make a complete clean up of the ResultUnion.

   IN/OUT
       ecs_ResultUnion *r: A pointer to a structure already defined. This
       method will clean up the structure and put his information into it.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */


int ecs_CleanUpResultUnion (r)
     ecs_ResultUnion *r;
{
  int val,i;

  switch (r->type) {
  case Object: 
    {
      ecs_CleanUpObject(&r->ecs_ResultUnion_u.dob);
    };
    break;
  case objAttributeFormat: 
    {
      val = r->ecs_ResultUnion_u.oaf.oa.oa_len;
      if (r->ecs_ResultUnion_u.oaf.oa.oa_val != NULL) {
	for(i=0;i<val;i++) {
	  if (r->ecs_ResultUnion_u.oaf.oa.oa_val[i].name != NULL)
	    free(r->ecs_ResultUnion_u.oaf.oa.oa_val[i].name);
	  r->ecs_ResultUnion_u.oaf.oa.oa_val[i].name = NULL;
	}
	free(r->ecs_ResultUnion_u.oaf.oa.oa_val);
      }
      r->ecs_ResultUnion_u.oaf.oa.oa_val = NULL;
    };
    break;
  case RasterInfo: 
    {
      val = r->ecs_ResultUnion_u.ri.cat.cat_len;
      if (r->ecs_ResultUnion_u.ri.cat.cat_val != NULL) {
	for(i=0;i<val;i++) {
	  if (r->ecs_ResultUnion_u.ri.cat.cat_val[i].label != NULL)
	    free(r->ecs_ResultUnion_u.ri.cat.cat_val[i].label);
	  r->ecs_ResultUnion_u.ri.cat.cat_val[i].label = NULL;
	}
	free(r->ecs_ResultUnion_u.ri.cat.cat_val);
      }
      r->ecs_ResultUnion_u.ri.cat.cat_val = NULL;     
    };
    break;
  case AText: 
    {
      if (r->ecs_ResultUnion_u.s != NULL)
	free(r->ecs_ResultUnion_u.s);
      r->ecs_ResultUnion_u.s = NULL;
    };
    break;
  case MultiResult:
    for (i = 0; i < (int) r->ecs_ResultUnion_u.results.results_len; i++) {
      ecs_CleanUpResultUnion(&r->ecs_ResultUnion_u.results.results_val[i]);
    }
    free(r->ecs_ResultUnion_u.results.results_val);
    break;
  default:
    break;
  };
  r->type = 0;

  return TRUE;
}

/*
   ----------------------------------------------------------------------

   ecs_CleanUpObject

   Will make a complete clean up and reinitialisation of ecs_Object in
   ecs_Result.

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will clean up the structure and put his information into it.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_CleanUpObject (obj)
     ecs_Object *obj;
{
  int val,i;

  if (obj->Id != NULL)
    free(obj->Id);
  obj->Id = NULL;

  if (obj->attr != NULL)
    free(obj->attr);
  obj->attr = NULL;

  switch (obj->geom.family) {
  case Area:
    {
      val = obj->geom.ecs_Geometry_u.area.ring.ring_len;
      if (obj->geom.ecs_Geometry_u.area.ring.ring_val != NULL) {
	for(i=0;i<val;i++) {
	  if (obj->geom.ecs_Geometry_u.area.ring.ring_val[i].c.c_val != NULL)
	    free(obj->geom.ecs_Geometry_u.area.ring.ring_val[i].c.c_val);
	  obj->geom.ecs_Geometry_u.area.ring.ring_val[i].c.c_val = NULL;
	}
	free(obj->geom.ecs_Geometry_u.area.ring.ring_val);
      }
      obj->geom.ecs_Geometry_u.area.ring.ring_val = NULL;
    };
    break;
  case Line:
    {
      if (obj->geom.ecs_Geometry_u.line.c.c_val != NULL)
	free(obj->geom.ecs_Geometry_u.line.c.c_val);
      obj->geom.ecs_Geometry_u.line.c.c_val = NULL;
    };
    break;
  case Matrix:
    {
      if (obj->geom.ecs_Geometry_u.matrix.x.x_val != NULL)
	free(obj->geom.ecs_Geometry_u.matrix.x.x_val);
      obj->geom.ecs_Geometry_u.matrix.x.x_val = NULL;
    };
    break;
  case Image:
    {
      if (obj->geom.ecs_Geometry_u.image.x.x_val != NULL)
	free(obj->geom.ecs_Geometry_u.image.x.x_val);
      obj->geom.ecs_Geometry_u.image.x.x_val = NULL;
    };
    break;
  case Text:
    {
      if (obj->geom.ecs_Geometry_u.text.desc != NULL)
	free(obj->geom.ecs_Geometry_u.text.desc);
      obj->geom.ecs_Geometry_u.text.desc = NULL;
    };
    break;
  default:
    break;
  };

  return TRUE;
}

/*************************************************/

/*
   ----------------------------------------------------------------------

   ecs_ResultInit

   First initialisation of ecs_Result. Perform when the structure
   is create to initialise it.

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will only update the contain of this structure.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_ResultInit (r)
     ecs_Result *r;
{
  r->error = 0;
  r->message = NULL;
  r->res.type = 0;

  return TRUE;

}

/* 
   ----------------------------------------------------------------------

   ecs_CalcObjectMBR

   Given a ecs_Result with a ecs_Object already define.
   Found the bounding rectangle of this object and 
   assign it in ecs_Result.

   IN
       ecs_Server *s: Server structure contain some complement information
       about region.

   IN/OUT
       ecs_Result *r: A pointer to a structure already defined. This
       method will only update the contain of this structure.

   OUT
       return int: A error code. If a problem occur in the method,
       "r" will be update to infor the user about the problem and
       will return a FALSE value. The default return value is TRUE.

   ----------------------------------------------------------------------
   */

int ecs_CalcObjectMBR (s,obj)
     ecs_Server *s;
     ecs_Object *obj;
{
  register unsigned int i,j,len;
  register ecs_Coordinate *coord;

  switch(obj->geom.family) {
  case Point:
    obj->xmin = obj->geom.ecs_Geometry_u.point.c.x;
    obj->ymin = obj->geom.ecs_Geometry_u.point.c.y;
    obj->xmax = obj->geom.ecs_Geometry_u.point.c.x;
    obj->ymax = obj->geom.ecs_Geometry_u.point.c.y;
    break;
  case Text:
    obj->xmin = obj->geom.ecs_Geometry_u.text.c.x;
    obj->ymin = obj->geom.ecs_Geometry_u.text.c.y;
    obj->xmax = obj->geom.ecs_Geometry_u.text.c.x;
    obj->ymax = obj->geom.ecs_Geometry_u.text.c.y;
    break;
  case Line:
    len = obj->geom.ecs_Geometry_u.line.c.c_len;
    if (len > 0) {
      obj->xmin = obj->geom.ecs_Geometry_u.line.c.c_val[0].x;
      obj->ymin = obj->geom.ecs_Geometry_u.line.c.c_val[0].y;
      obj->xmax = obj->geom.ecs_Geometry_u.line.c.c_val[0].x;
      obj->ymax = obj->geom.ecs_Geometry_u.line.c.c_val[0].y;
    }
    for(i=1;i<len;i++) {
      coord = &(obj->geom.ecs_Geometry_u.line.c.c_val[i]);
      if (obj->xmin > coord->x)
	obj->xmin = coord->x;
      if (obj->ymin > coord->y)
	obj->ymin = coord->y;
      if (obj->xmax < coord->x)
	obj->xmax = coord->x;
      if (obj->ymax < coord->y)
	obj->ymax = coord->y;
    }
    break;
  case Area:
    if ((obj->geom.ecs_Geometry_u.area.ring.ring_len > 0) && 
	(obj->geom.ecs_Geometry_u.area.ring.ring_val[0].c.c_len > 0)) {
      obj->xmin = obj->geom.ecs_Geometry_u.area.ring.ring_val[0].c.c_val[0].x;
      obj->ymin = obj->geom.ecs_Geometry_u.area.ring.ring_val[0].c.c_val[0].y;
      obj->xmax = obj->geom.ecs_Geometry_u.area.ring.ring_val[0].c.c_val[0].x;
      obj->ymax = obj->geom.ecs_Geometry_u.area.ring.ring_val[0].c.c_val[0].y;
    }
    for(i=0;i<obj->geom.ecs_Geometry_u.area.ring.ring_len;i++) {
      for(j=0;j<obj->geom.ecs_Geometry_u.area.ring.ring_val[i].c.c_len;j++) {
	coord = &(obj->geom.ecs_Geometry_u.area.ring.ring_val[i].c.c_val[j]);
	if (obj->xmin > coord->x)
	  obj->xmin = coord->x;
	if (obj->ymin > coord->y)
	  obj->ymin = coord->y;
	if (obj->xmax < coord->x)
	  obj->xmax = coord->x;
	if (obj->ymax < coord->y)
	  obj->ymax = coord->y;
      }
    }
    break;
  case Matrix:
  case Image:
    obj->xmin = s->currentRegion.west;
    obj->ymin = s->currentRegion.south;
    obj->xmax = s->currentRegion.east;
    obj->ymax = s->currentRegion.north;
    break;
  default:
    break;
  }
  return TRUE;
}


/*
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

   ecs_CopyResult: Copy an object contain in a ecs_Result structure.

   IN
       ecs_Result *source: Original object
   OUT
       ecs_Result *copy: Copied object
       return int: Error message returned.
                    TRUE: Success
		    FALSE: Failure

   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
   */

int ecs_CopyResult(source,copy)
     ecs_Result *source;
     ecs_Result **copy;
{
  return ecs_CopyResultFromUnion(&source->res,copy);
}


/*
 *----------------------------------------------------------------------
 * ecs_CopyResultFromUnion --
 *
 *	Creates an ecs_Result structure from an ecs_ResultUnion
 *	structure.
 *
 * IN
 *	ecs_ResultUnion *source: Original object piece
 * OUT
 *	ecs_Result **copy:	 Copied object piece with wrapping header
 *
 * Results:
 *	TRUE if successful.
 *	FALSE if unsuccessful.
 *----------------------------------------------------------------------
 */
int
ecs_CopyResultFromUnion(source,copy)
     ecs_ResultUnion *source;
     ecs_Result **copy;
{
  int returncode;
  ecs_Result *result;

  returncode = TRUE;
  if (source->type == Object) {

    /* Allocate the new object */

    if ((result = (ecs_Result *) malloc(sizeof(ecs_Result))) == NULL) {
      returncode = FALSE;
    } 

    /* Prepare the object structure */

    result->error = 0;
    result->message = NULL;

    returncode = ecs_CopyResultUnionWork(source,&result->res);
    if (returncode == FALSE) {
      free(result);
      result = NULL;
    }
  } else {
    returncode = FALSE;
    result = NULL;
  }

  *copy = result;
  return returncode;
}

/*
 *----------------------------------------------------------------------
 * ecs_CopyResultUnion --
 *
 *	Creates an ecs_Result structure from an ecs_ResultUnion
 *	structure.
 *
 * IN
 *	ecs_ResultUnion *source: Original object piece
 * OUT
 *	ecs_Result **copy:	 Copied object piece with wrapping header
 *
 * Results:
 *	TRUE if successful.
 *	FALSE if unsuccessful.
 *----------------------------------------------------------------------
 */
int
ecs_CopyResultUnion(source,copy)
     ecs_ResultUnion *source;
     ecs_ResultUnion **copy;
{
  int returncode;
  ecs_ResultUnion *result;

  returncode = TRUE;
  if (source->type == Object) {

    /* Allocate the new object */

    result = (ecs_ResultUnion *) malloc(sizeof(ecs_ResultUnion));
    if (result == NULL) {
      returncode = FALSE;
    } else {
      returncode = ecs_CopyResultUnionWork(source, result);
      if (returncode == FALSE) {
	free(result);
	result = NULL;
      }
    }
  } else {
    returncode = FALSE;
    result = NULL;
  }

  *copy = result;
  return returncode;
}

/*
 *----------------------------------------------------------------------
 * ecs_CopyResultUnionWork --
 *
 *	Sets one ecs_ResultUnion to be exactly like the other.
 *	Allocates any substructure that are needed.
 *
 * IN
 *	ecs_ResultUnion *source: Original object piece
 * OUT
 *	ecs_ResultUnion *dest:	 Copied object piece
 *
 * Results:
 *	TRUE if successful.
 *	FALSE if unsuccessful.
 *----------------------------------------------------------------------
 */
int
ecs_CopyResultUnionWork(source,copy)
     ecs_ResultUnion *source;
     ecs_ResultUnion *copy;
{
  int returncode;
  ecs_Object *obj;
  ecs_Object *newobj;

  returncode = TRUE;
  if (source->type == Object) {
    copy->type = Object;
    obj = &(source->ecs_ResultUnion_u.dob);

    newobj = &copy->ecs_ResultUnion_u.dob;

    /* Allocate in the new object Id and attr */

    if ((returncode == TRUE) && (obj->Id != NULL) && ((newobj->Id = (char *) malloc(strlen(obj->Id)+1)) == NULL)) {
      returncode = 1;
    }     

    if ((returncode == TRUE) && (obj->attr != NULL) && ((newobj->attr = (char *) malloc(strlen(obj->attr)+1)) == NULL)) {
      returncode = FALSE;
    } 
    
    /* Fill the new ecs_Object structure with obj values */

    if (obj->Id != NULL) {
      strcpy(newobj->Id,obj->Id);
    } else {
      newobj->Id = NULL;
    }
    if (obj->attr != NULL) {
      strcpy(newobj->attr,obj->attr);
    } else {
      newobj->attr = NULL;
    }
    newobj->xmin = obj->xmin;
    newobj->ymin = obj->ymin;
    newobj->xmax = obj->xmax;
    newobj->ymax = obj->ymax;

    /* Call ecs_CopyGeometry to fill the geom argument */

    returncode = ecs_CopyGeometry(obj,newobj);

    if (returncode == FALSE) {
      ecs_FreeObject(newobj);
    }
  } else {
    returncode = FALSE;
  }

  return returncode;
}

/*
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

   ecs_CopyObject: Copy an object contain in a ecs_Result structure.

   IN
       ecs_Object *obj: Original object
   OUT
       ecs_Object *copy: Copied object preallocated

       return int: Error message returned.
                    TRUE: Success
		    FALSE: Failure

   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
   */

int ecs_CopyObject(obj,newobj)
     ecs_Object *obj;
     ecs_Object *newobj;
{
  int returncode;

  returncode = TRUE;

  /* Allocate in the new object Id and attr */
  
  if ((returncode == TRUE) && (obj->Id != NULL) && ((newobj->Id = (char *) malloc(strlen(obj->Id)+1)) == NULL)) {
    returncode = FALSE;
  }     

  if ((returncode == TRUE) && (obj->attr != NULL) && ((newobj->attr = (char *) malloc(strlen(obj->attr)+1)) == NULL)) {
    returncode = FALSE;
  } 
  
  /* Fill the new ecs_Object structure with obj values */
  
  if (obj->Id != NULL) {
    strcpy(newobj->Id,obj->Id);
  } else {
    newobj->Id = NULL;
  }
  if (obj->attr != NULL) {
    strcpy(newobj->attr,obj->attr);
  } else {
    newobj->attr = NULL;
  }
  newobj->xmin = obj->xmin;
  newobj->ymin = obj->ymin;
  newobj->xmax = obj->xmax;
  newobj->ymax = obj->ymax;
  
  /* Call ecs_CopyGeometry to fill the geom argument */
  
  returncode = ecs_CopyGeometry(obj,newobj);
  
  if (returncode == FALSE) {
    ecs_FreeObject(newobj);
  }
  
  return returncode;
}

/*
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

   ecs_CopyGeometry: Copy an object geometry into a new object

   IN
       ecs_Object *source: Original object
   IN/OUT
       ecs_Object *copy: Copied object
   OUT
       return int: Error message returned.
                    TRUE: Success
		    FALSE: Failure

   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
   */

int ecs_CopyGeometry(source,copy)
     ecs_Object *source;
     ecs_Object *copy;
{
  int returncode;

  returncode = TRUE;
  copy->geom.family = source->geom.family; 
  switch(copy->geom.family) {
  case Area:
    returncode = ecs_CopyArea(&(source->geom.ecs_Geometry_u.area),
			      &(copy->geom.ecs_Geometry_u.area));
    break;
  case Line:
    returncode = ecs_CopyLine(&(source->geom.ecs_Geometry_u.line),
			      &(copy->geom.ecs_Geometry_u.line));
    break;
  case Point:
    returncode = ecs_CopyPoint(&(source->geom.ecs_Geometry_u.point),
			      &(copy->geom.ecs_Geometry_u.point));
    break;
  case Text:
    returncode = ecs_CopyText(&(source->geom.ecs_Geometry_u.text),
			      &(copy->geom.ecs_Geometry_u.text));
    break;
  case Matrix:
    returncode = ecs_CopyMatrix(&(source->geom.ecs_Geometry_u.matrix),
				&(copy->geom.ecs_Geometry_u.matrix));
    break;
  case Image:
    returncode = ecs_CopyImage(&(source->geom.ecs_Geometry_u.image),
			       &(copy->geom.ecs_Geometry_u.image));
    break;
  default:
    break;
  }

  return returncode;
}

/*
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

   ecs_CopyArea: Copy the area from an original object to a copy

   IN
       ecs_Area *source: Pointer to the source area
   IN/OUT
       ecs_Area *copy: Pointer to the copy area
   OUT
       return int: Error message returned.
                    TRUE: Success
		    FALSE: Failure

   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
   */

int ecs_CopyArea(source,copy)
     ecs_Area *source;
     ecs_Area *copy; 
{
  int i,j;

  copy->ring.ring_len = source->ring.ring_len;
  if (source->ring.ring_val != NULL) {
    /* Allocate ring table */

    copy->ring.ring_val = (ecs_FeatureRing *) malloc(sizeof(ecs_FeatureRing)*source->ring.ring_len);
    if (copy->ring.ring_val == NULL) {
      return FALSE;
    }

    /* Copy ring table containt */

    for(i=0;i<(int) source->ring.ring_len;i++) {
      copy->ring.ring_val[i].centroid.x = source->ring.ring_val[i].centroid.x;
      copy->ring.ring_val[i].centroid.y = source->ring.ring_val[i].centroid.y;
      copy->ring.ring_val[i].c.c_len = source->ring.ring_val[i].c.c_len;
      if (source->ring.ring_val[i].c.c_val != NULL) {
	/* Allocate c table */

	copy->ring.ring_val[i].c.c_val = (ecs_Coordinate *) malloc(sizeof(ecs_Coordinate)*source->ring.ring_val[i].c.c_len);
	if (copy->ring.ring_val[i].c.c_val == NULL) {
	  return FALSE;
	}

	/* Copy the c table contain */

	for(j=0;j<(int) source->ring.ring_val[i].c.c_len;j++) {
	  copy->ring.ring_val[i].c.c_val[j].x = source->ring.ring_val[i].c.c_val[j].x;
	  copy->ring.ring_val[i].c.c_val[j].y = source->ring.ring_val[i].c.c_val[j].y;
	}

      } else {
	copy->ring.ring_val[i].c.c_val = NULL;
      }
    }
  } else {
    copy->ring.ring_val = NULL;
  }

  return TRUE;
}

/*
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

   ecs_CopyLine: Copy the Line from an original object to a copy

   IN
       ecs_Line *source: Pointer to the source Line
   IN/OUT
       ecs_Line *copy: Pointer to the copy Line
   OUT
       return int: Error message returned.
                    TRUE: Success
		    FALSE: Failure

   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
   */

int ecs_CopyLine(source,copy)
     ecs_Line *source; 
     ecs_Line *copy;
{
  int i;

  copy->c.c_len = source->c.c_len;
  if (source->c.c_val != NULL) {
    /* Allocate c table */

    copy->c.c_val = (ecs_Coordinate *) malloc(sizeof(ecs_Coordinate)*source->c.c_len);
    if (copy->c.c_val == NULL) {
      return FALSE;
    }

    /* Copy c table containt */

    for(i=0;i<(int) source->c.c_len;i++) {
      copy->c.c_val[i].x = source->c.c_val[i].x;
      copy->c.c_val[i].y = source->c.c_val[i].y;
    }
  } else {
    copy->c.c_val = NULL;
  }

  return TRUE;
}

/*
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

   ecs_CopyPoint: Copy the point from an original object to a copy

   IN
       ecs_Point *source: Pointer to the source point
   IN/OUT
       ecs_Point *copy: Pointer to the copy point
   OUT
       return int: Error message returned.
                    TRUE: Success
		    FALSE: Failure

   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
   */

int ecs_CopyPoint(source,copy)
     ecs_Point *source; 
     ecs_Point *copy; 
{
  copy->c.x = source->c.x;
  copy->c.y = source->c.y;

  return TRUE;
}

/*
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

   ecs_CopyText: Copy the text from an original object to a copy

   IN
       ecs_Text *source: Pointer to the source text
   IN/OUT
       ecs_Text *copy: Pointer to the copy text
   OUT
       return int: Error message returned.
                    TRUE: Success
		    FALSE: Failure

   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
   */

int ecs_CopyText(source,copy)
     ecs_Text *source; 
     ecs_Text *copy; 
{
  copy->c.x = source->c.x;
  copy->c.y = source->c.y;

  if (source->desc != NULL) {
    copy->desc = (char *) malloc(strlen(source->desc)+1);
    if (copy->desc == NULL) {
      return FALSE;
    }
    strcpy(copy->desc,source->desc);
  } else {
    copy->desc = NULL;
  }

  return TRUE;  
}

/*
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

   ecs_CopyMatrix: Copy the matrix from an original object to a copy

   IN
       ecs_Matrix *source: Pointer to the source matrix
   IN/OUT
       ecs_Matrix *copy: Pointer to the copy matrix
   OUT
       return int: Error message returned.
                    TRUE: Success
		    FALSE: Failure

   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
   */

int ecs_CopyMatrix(source,copy)
     ecs_Matrix *source; 
     ecs_Matrix *copy; 
{
  int i;

  copy->x.x_len = source->x.x_len;
  if (source->x.x_val != NULL) {
    /* Allocate x table */

    copy->x.x_val = (u_int *) malloc(sizeof(ecs_Coordinate)*source->x.x_len);
    if (copy->x.x_val == NULL) {
      return FALSE;
    }

    /* Copy x table containt */

    for(i=0;i<(int) source->x.x_len;i++) {
      copy->x.x_val[i] = source->x.x_val[i];
    }
  } else {
    copy->x.x_val = NULL;
  }

  return TRUE;
}

/*
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

   ecs_CopyImage: Copy the image from an original object to a copy

   IN
       ecs_Image *source: Pointer to the source image
   IN/OUT
       ecs_Image *copy: Pointer to the copy image
   OUT
       return int: Error message returned.
                    TRUE: Success
		    FALSE: Failure

   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
   */

int ecs_CopyImage(source,copy)
     ecs_Image *source; 
     ecs_Image *copy; 
{
  int i;

  copy->x.x_len = source->x.x_len;
  if (source->x.x_val != NULL) {
    /* Allocate x table */

    copy->x.x_val = (u_int *) malloc(sizeof(ecs_Coordinate)*source->x.x_len);
    if (copy->x.x_val == NULL) {
      return FALSE;
    }

    /* Copy x table containt */

    for(i=0;i<(int) source->x.x_len;i++) {
      copy->x.x_val[i] = source->x.x_val[i];
    }
  } else {
    copy->x.x_val = NULL;
  }

  return TRUE;
}

/*
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

   ecs_FreeObject: Free an object

   IN
       ecs_Object *obj: object to be free

   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
   */

void ecs_FreeObject(obj)
     ecs_Object *obj;
{
  int i;

  if (obj != NULL) {
    if (obj->Id != NULL)
      free(obj->Id);
    if (obj->attr != NULL)
      free(obj->attr);
    switch(obj->geom.family) {
    case Area:
      if (obj->geom.ecs_Geometry_u.area.ring.ring_val != NULL) {
	for(i=0;i<(int) obj->geom.ecs_Geometry_u.area.ring.ring_len;i++) {
	  if (obj->geom.ecs_Geometry_u.area.ring.ring_val[i].c.c_val != NULL)
	    free(obj->geom.ecs_Geometry_u.area.ring.ring_val[i].c.c_val);
	}
	free(obj->geom.ecs_Geometry_u.area.ring.ring_val);
      }
      break;
    case Line:
      if (obj->geom.ecs_Geometry_u.line.c.c_val != NULL)
	free(obj->geom.ecs_Geometry_u.line.c.c_val);
      break;
    case Text:
      if (obj->geom.ecs_Geometry_u.text.desc != NULL)
	free(obj->geom.ecs_Geometry_u.text.desc);
      break;
    case Matrix:
      if (obj->geom.ecs_Geometry_u.matrix.x.x_val != NULL)
	free(obj->geom.ecs_Geometry_u.matrix.x.x_val);
      break;
    case Image:
      if (obj->geom.ecs_Geometry_u.image.x.x_val != NULL)
	free(obj->geom.ecs_Geometry_u.image.x.x_val);
      break;
    default:
      break;
    }
  }
  return;
}


