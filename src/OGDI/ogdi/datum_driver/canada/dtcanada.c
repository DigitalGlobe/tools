/*********************************************************************

  CSOURCE_INFORMATION
  
  NAME
     dtcanada.c

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
#include "nadconv.h"

NAD_DATA *dtptr = NULL;
int nad_count = 0;

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_nad_init

   DESCRIPTION
      Prepare the nad converter to convert points in Canada.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         char *table: The table name (not used)
      IN/OUT
         void **privdata: Private data pointer
   END_PARAMETERS

   RETURN_VALUE
      int : Contain the result of the operation. Success if TRUE,
      failure if FALSE.

   END_FUNCTION_INFORMATION

   PSEUDO_CODE

   1. Increment nad_count. If nad_count value is 1
   Begin
      1.1. Check the OGDIDATUM environment variable. If it's not there,
      return FALSE
      
      1.2. Get the OGDIDATUM and concatenate NTV2_0.GSB to form a file path
      
      1.3. Call NAD_init in the library with the file path found in 2. and
      the arguments "NAD27" and "NAD83". Get the returned code.
      
      1.4. If the code is an error, return FALSE.

      1.5. Set dtptr to the value retuned by NAD_init
   End

   2. Set privdata to dtptr

   3. Return TRUE

   ********************************************************************
   */

int dyn_nad_init(privdata, table) 
     void **privdata;
     char *table;
{
  char *pathfile;

  (void) table;

  *privdata = NULL;
  nad_count++;
  
  if (nad_count == 1) {
    if (getenv("OGDIDATUM") == NULL) {
      nad_count--;
      return FALSE;
    }

    pathfile = malloc(strlen(getenv("OGDIDATUM"))+12);
    if (pathfile == NULL) {
      nad_count--;
      return FALSE;
    }
    strcpy(pathfile,getenv("OGDIDATUM"));
    strcat(pathfile,"/NTV2_0.GSB");  

    dtptr = NAD_Init(pathfile,"NAD27","NAD83");
    if (dtptr == NULL) {
      nad_count--;
      return FALSE;
    }
    free(pathfile);

  } else {
    if (dtptr == NULL) {
      nad_count=1;
      return FALSE;
    }
  }
   
  *privdata = (void *) dtptr;
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

   1. Decrement count

   2. If count is lower or equal to zero
   Begin
      2.1. Close the pointer dtptr with NAD_Close and set it to NULL   
   End

   3. Return TRUE

   ********************************************************************
   */

int dyn_nad_close(privdata)
     void *privdata;
{
  (void) privdata;
  nad_count--;
  if (nad_count <= 0) {
    NAD_Close(dtptr);
    dtptr = NULL;
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

   2. Call NAD_Forward with temp_x and temp_y

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
  double temp_x,temp_y;

  (void) privdata;

  if (dtptr == NULL)
    return TRUE;

  temp_x = *x * -3600.0;
  temp_y = *y * 3600.0;

  if (NAD_Forward(dtptr,&temp_x,&temp_y) == NAD_OK) {
    *x = temp_x / -3600.0;
    *y = temp_y / 3600.0;
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

   2. Call NAD_Reverse with temp_x and temp_y

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
  double temp_x,temp_y;

  (void) privdata;

  if (dtptr == NULL)
    return TRUE;

  temp_x = *x * -3600.0;
  temp_y = *y * 3600.0;

  if (NAD_Reverse(dtptr,&temp_x,&temp_y) == NAD_OK) {
    *x = temp_x / -3600.0;
    *y = temp_y / 3600.0;
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
  switch (fdwReason) {
  case DLL_PROCESS_ATTACH:
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
    break;
  case DLL_PROCESS_DETACH:
    nad_count=1;
    dyn_nad_close((void *) dtptr);
  break;
  }
  return TRUE;
}
#endif
