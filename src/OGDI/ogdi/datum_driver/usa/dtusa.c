/*********************************************************************

  CSOURCE_INFORMATION
  
  NAME
     dtusa.c

  DESCRIPTION
     Module used to convert geographics point from nad27 to nad83. Interface
     between nadconv module and the c_interface.
  END_DESCRIPTION

  END_CSOURCE_INFORMATION

  Copyright (C) 1997 Logiciels et Applications Scientifiques (L.A.S.) Inc
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

#include "ecs.h"
#include "projects.h"

typedef struct {
  struct CTABLE *dtptr;
  int count;
  char *tablename;
} datuminfo;

int tableqty = 7;
datuminfo datumtable[] = {{NULL,0,"conus"},{NULL,0,"alaska"},{NULL,0,"hawaii"},{NULL,0,"prvi"},
			  {NULL,0,"stgeorge"},{NULL,0,"stlrnc"},{NULL,0,"stpaul"}};

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_nad_init

   DESCRIPTION
      Prepare the nad converter to convert points in a table of the USA.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         char *table: The table name
      IN/OUT
         void **privdata: Private data pointer
   END_PARAMETERS

   RETURN_VALUE
      int : Contain the result of the operation. Success if TRUE,
      failure if FALSE.

   END_FUNCTION_INFORMATION

   PSEUDO_CODE

   1. Search in datumtable the corresponding position of the datum table.
   If it's not there, return FALSE. Set privdata to this position.

   2. Increment count in privdata

   3. If count in privdata is 1 and dtptr is NULL then
   Begin
      3.1. Check the OGDIDATUM environment variable. If it's not there,
      decrement count in privdata and return FALSE

      3.2. Get the OGDIDATUM and concatenate the table name to form a table
      file directory. 

      3.3. Call nad_init in the library with the file path found in 3.2.
      Get the returned code.

      3.4. If the code is an error, decrement count in privdata and return FALSE.
   End

   4. Return TRUE

   ********************************************************************
   */

int dyn_nad_init(privdata, table) 
     void **privdata;
     char *table;
{
  int i;
  datuminfo *ptr;
  char *pathfile;

  ptr = NULL;
  for (i=0;i<tableqty;i++) {
    if (strcmp(table,datumtable[i].tablename) == 0) {
      ptr = &(datumtable[i]);
      break;
    }
  }
  *privdata = (void *) ptr;
  if (ptr == NULL) 
    return FALSE;
  ptr->count++;

  if (ptr->count == 1 && ptr->dtptr == NULL) {
    if (getenv("OGDIDATUM") == NULL) {
      ptr->count--;
      return FALSE;
    }

    pathfile = malloc(strlen(getenv("OGDIDATUM"))+12);
    if (pathfile == NULL) {
      ptr->count--;
      return FALSE;
    }
    strcpy(pathfile,getenv("OGDIDATUM"));
    strcat(pathfile,"/");  
    strcat(pathfile,table);  

#if PJ_VERSION >= 480
    ptr->dtptr = nad_init(pj_get_default_ctx(), pathfile);
#else
    ptr->dtptr = nad_init(pathfile);
#endif
    if (ptr->dtptr == NULL) {
      ptr->count--;
      return FALSE;
    }
    free(pathfile);
  }

  return TRUE;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_nad_close

   DESCRIPTION
      Free the allocated structure dtptr
   END_DESCRIPTION

   PARAMETERS
      INPUT
         void *privdata: The private data of the driver
   END_PARAMETERS

   RETURN_VALUE
      int : Contain the result of the operation. Success if TRUE,
      failure if FALSE.

   END_FUNCTION_INFORMATION

   PSEUDO_CODE

   1. Decrement count in privdata

   2. If count is lower or equal to zero
   Begin
      2.1. Close the pointer dtptr in privdata with NAD_Close and set it to NULL   
   End

   3. Return TRUE

   ********************************************************************
   */

int dyn_nad_close(privdata)
     void *privdata;
{
  datuminfo *ptr = (datuminfo *) privdata;

  if (ptr == NULL) return TRUE;

  ptr->count--;
  if (ptr->count<=0) {
    nad_free(ptr->dtptr);
    ptr->dtptr = NULL;
  }
  return TRUE;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_nad_forward

   DESCRIPTION
      Convert a point in the NAD27 datum to NAD83. If an error occur,
      don't make any convertion.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         void *privdata: The private data of the driver
      IN/OUT
         double *x: lattitude of the geographical coordinate
         double *y: longitude of the geographical coordinate
   END_PARAMETERS

   RETURN_VALUE
      int : Contain the result of the operation. Success if TRUE,
      failure if FALSE. In this case, it's always success.

   END_FUNCTION_INFORMATION

   PSEUDO_CODE

   1. Copy the x and y coordinates in temp_x and temp_y

   2. Call nad_cvt with temp_x and temp_y and inverse to 0

   3. If the previous operation is a success, put in x and y
   the contain of temp_x and temp_y.

   4. Return TRUE

   ********************************************************************
   */

int dyn_nad_forward(privdata,x,y)
     void *privdata;
     double *x;
     double *y;
{
  datuminfo *ptr = (datuminfo *) privdata;
  projUV val,val1;

  if (ptr == NULL) return TRUE;

  if (ptr->dtptr == NULL)
    return TRUE;

  val.u = *x * DEG_TO_RAD;
  val.v = *y * DEG_TO_RAD;

  val1 = nad_cvt(val,0,ptr->dtptr);

  if (val1.u != HUGE_VAL && val1.v != HUGE_VAL) {
    *x = val1.u * RAD_TO_DEG;
    *y = val1.v * RAD_TO_DEG;
  }

  return TRUE;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_nad_reverse

   DESCRIPTION
      Convert a point in the NAD83 datum to NAD27. If an error occur,
      don't make any convertion.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         void *privdata: The private data of the driver
      IN/OUT
         double *x: lattitude of the geographical coordinate
         double *y: longitude of the geographical coordinate
   END_PARAMETERS

   RETURN_VALUE
      int : Contain the result of the operation. Success if TRUE,
      failure if FALSE. In this case, it's always success.

   END_FUNCTION_INFORMATION

   PSEUDO_CODE

   1. Copy the x and y coordinates in temp_x and temp_y

   2. Call nad_cvt with temp_x and temp_y and inverse to 1

   3. If the previous operation is a success, put in x and y
   the contain of temp_x and temp_y.

   4. Return TRUE

   ********************************************************************
   */

int dyn_nad_reverse(privdata,x,y)
     void *privdata;
     double *x;
     double *y;
{
  datuminfo *ptr = (datuminfo *) privdata;
  projUV val,val1;

  if (ptr == NULL) return TRUE;

  if (ptr->dtptr == NULL)
    return TRUE;

  val.u = *x * DEG_TO_RAD;
  val.v = *y * DEG_TO_RAD;

  val1 = nad_cvt(val,1,ptr->dtptr);

  if (val1.u != HUGE_VAL && val1.v != HUGE_VAL) {
    *x = val1.u * RAD_TO_DEG;
    *y = val1.v * RAD_TO_DEG;
  }

  return TRUE;
}


#ifdef _WINDOWS
/*
 * When the DLL is being unloaded, make sure to destroy all of
 * the clients that remain open.
 */
BOOL WINAPI 
DllMain( HINSTANCE  hinstDLL,   // handle of DLL module 
         DWORD  fdwReason,      // reason for calling function 
         LPVOID  lpvReserved)
{
  int i;

  switch (fdwReason) {
  case DLL_PROCESS_ATTACH:
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
    break;
  case DLL_PROCESS_DETACH:
    for (i=0;i<tableqty;i++) {
      if (datumtable[i].dtptr != NULL) {
	nad_free(datumtable[i].dtptr);
	datumtable[i].dtptr=NULL;
	datumtable[i].count=0;
      }
    }
  break;
  }
  return TRUE;
}
#endif
