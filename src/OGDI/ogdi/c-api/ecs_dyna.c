/******************************************************************************
 *
 * Component: OGDI Core C API
 * Purpose: Functions for loading functions from DLLs / .so files.  
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
 * $Log: ecs_dyna.c,v $
 * Revision 1.6  2016/07/11 09:15:53  erouault
 * fix ecs_OpenDynamicLib on 64 bit Windows (OGDI #72)
 *
 * Revision 1.5  2007/02/12 18:06:31  cbalint
 *         Hide plugins from system libs path.
 *         Release versioning using sonames.
 *
 * Revision 1.4  2004/02/18 21:50:21  warmerda
 * Added debug statement #ifdefed out.
 *
 * Revision 1.3  2001/04/09 15:04:34  warmerda
 * applied new source headers
 *
 */

#include "ecs.h"

ECS_CVSID("$Id: ecs_dyna.c,v 1.6 2016/07/11 09:15:53 erouault Exp $");

#if !defined(MISSING_DLFCN_H)
#include <dlfcn.h>
#endif


/* 
   -------------------------------------------------
  
   ecs_OpenDynamicLib: Open a dynamic link in a system

   IN: 
      libname: A string with the dynamic library file name.
      The dynamic library file could be without file extension.
   OUT: 
      void *: A handle to a new link to a dynamic link. If NULL,
      the dynamic link failed.

   -------------------------------------------------
   */

void *ecs_OpenDynamicLib(libname)
     char *libname;
{
  char *temp;
#ifdef _WINDOWS
  static HINSTANCE handle;

  handle = LoadLibrary(libname);
  if (handle == NULL) {
    /* Try with the .dll extension */

    if ((temp = (char *) malloc(strlen(libname)+5)) == NULL)
      return NULL;
    strcpy(temp,libname);
    strcat(temp,".dll");
        
    handle = LoadLibrary(temp);
    free(temp);
    if (handle == NULL) {
      return NULL;
    } else {
      return (void *) handle;
    }
  } else {
    return (void *) handle;
  }
#else
#if !defined(MODULES_PATH)
#define MODULES_PATH "/usr/lib/ogdi/"
#endif
  void *handle;

  handle = dlopen(libname,RTLD_LAZY);
  if (handle != NULL)
    return handle;

  if ((temp = (char *) malloc(strlen(MODULES_PATH)+strlen(libname)+1)) == NULL)
    return NULL;
  sprintf(temp,MODULES_PATH "%s",libname);
  handle = dlopen(temp,RTLD_LAZY);
  free(temp);
  if (handle != NULL)
    return handle;

  if ((temp = (char *) malloc(strlen(MODULES_PATH)+strlen(libname)+7)) == NULL)
    return NULL;
  sprintf(temp,MODULES_PATH "lib%s.so",libname);
  handle = dlopen(temp,RTLD_LAZY);
  free(temp);

  if (handle == NULL) {
    /* Try with the .so extension */

    if ((temp = (char *) malloc(strlen(libname)+7)) == NULL)
      return NULL;
    strcpy(temp,"lib");
    strcat(temp,libname);
    strcat(temp,".so");

    handle = dlopen(temp,RTLD_LAZY);
#ifdef notdef
    printf( "dlopen(%s) error: %s\n", temp, dlerror() );
#endif
    free(temp);
  }
  return handle;

#endif

}

/* 
   -------------------------------------------------
   
   ecs_GetDynamicLibFunction: Initialise a pointer to
   a function in the dynamic library.

   IN: 
      handle      : A link to dynamic library initialized by
                    ecs_OpenDynamicLib
      functionname: A string with the function name

   OUT: 
      void *: A pointer to the function. If NULL, an error
      append during this operation.

   -------------------------------------------------
   */

void *ecs_GetDynamicLibFunction(handle,functionname)
     void *handle;
     char *functionname;
{
  void *function;
#ifndef _WINDOWS
  char *temp;
#endif

  /* If the handle is NULL, return NULL */
  if (handle == NULL)
    return NULL;

#ifdef _WINDOWS
  function = (void *) GetProcAddress((HINSTANCE) handle, functionname);
#else

  function = (void *) dlsym(handle, functionname);
  if (function == NULL) {
    /* try with an underscore before the function name */
    temp = (char *) malloc(strlen(functionname)+2);
    if (temp == NULL)
      return NULL;
    strcpy(temp,"_");
    strcat(temp,functionname);
    function = (void *) dlsym(handle, functionname);
    free(temp);
  }
#endif

  return (void *) function;
}

/* 
   -------------------------------------------------
   
   ecs_CloseDynamicLib: Open a dynamic link

   IN: 
      handle      : A link to dynamic library initialized by
                    ecs_OpenDynamicLib

   -------------------------------------------------
   */

void ecs_CloseDynamicLib(handle)
     void *handle;
{
  /* If the handle is NULL, end this operation */
  if (handle == NULL)
    return;

#ifdef _WINDOWS
  FreeLibrary((HINSTANCE) handle);
#else
  dlclose(handle);
#endif

  handle = NULL;
  return;
}

