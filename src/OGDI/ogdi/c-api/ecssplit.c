/******************************************************************************
 *
 * Component: OGDI Core C API
 * Purpose: High level URL splitting functions.
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
 * $Log$
 * Revision 1.3  2003-08-27 05:27:21  warmerda
 * Modified ecs_SplitURL() so that calling with a NULL url indicates it
 * should free the resources associated with the static regular expressions.
 * This makes memory leak debugging with OGDI more convenient.
 *
 * Revision 1.2  2001/04/09 15:04:34  warmerda
 * applied new source headers
 *
 */

#include "ecs.h"
#include <string.h>

ECS_CVSID("$Id$");

/* 
   -------------------------------------------------
  
   ecs_freeSplitURL: deallocate all the string used in 
   SplitURL operation.

   IN/OUT: 
      type: 
      machine:
      path:    the three string to deallocate

   -------------------------------------------------
   */

void ecs_freeSplitURL(type,machine,path)
     char **type;
     char **machine;
     char **path;
{
  if (*type != NULL) {
    free(*type);
    *type = NULL;
  }
  if (*machine != NULL) {
    free(*machine);
    *machine = NULL;
  }
  if (*path != NULL) {
    free(*path);
    *path = NULL;
  }
}

static int parse_server_path(const char* url, char **server, char **path)
{
    int i;
    for( i = 0; url[i]; i++ )
    {
        if ( !( (url[i] >= '0' && url[i] <= '9') ||
                (url[i] >= 'a' && url[i] <= 'z') ||
                (url[i] >= 'A' && url[i] <= 'Z') ||
                url[i] == '.' ) )
        {
            break;
        }
    }
    if( i == 0 )
        return FALSE;

    *server = malloc( i + 1 );
    memcpy(*server, url, i );
    (*server)[i] = 0;

    *path = malloc( strlen(url + i) + 1 );
    strcpy(*path, url + i);
    return TRUE;
}

/* 
   -------------------------------------------------
  
   ecs_SplitURL: Extract information from the URL
   and return it in the arguments.

   IN: 
      char *url: This string contain the URL

   OUT:
      machine: Machine addresses contain in the URL. If NULL,
               the server is local
      server:  The server type of the DLL to load
      path:    The string used by the dynamic database library to
               set the database server. Specific to each kind of
	       server.
      return int: A error message
          TRUE: success
          FALSE: failure

   -------------------------------------------------
   */

int ecs_SplitURL(url,machine,server,path)
     const char *url;
     char **machine;
     char **server;
     char **path;
{
  if( url == NULL ) { /* Cleanup */
      return FALSE;
  }

  *machine = NULL;
  *server = NULL;
  *path = NULL;

  if(strncmp(url,"gltp://",7) == 0) {
    const char* slash;
    url += 7;
    slash = strchr(url, '/');
    if( !slash ) {
        return FALSE;
    }
    *machine = malloc(slash - url + 1);
    memcpy(*machine, url, slash - url);
    (*machine)[slash - url] = 0;

    return parse_server_path(slash + 1, server, path);
  } else if (strncmp(url,"gltp:/",6) == 0) {
    return parse_server_path(url + 6, server, path);
  } else {
      return FALSE;
  }
}


