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
 * $Log: ecssplit.c,v $
 * Revision 1.3  2003/08/27 05:27:21  warmerda
 * Modified ecs_SplitURL() so that calling with a NULL url indicates it
 * should free the resources associated with the static regular expressions.
 * This makes memory leak debugging with OGDI more convenient.
 *
 * Revision 1.2  2001/04/09 15:04:34  warmerda
 * applied new source headers
 *
 */

#include "ecs.h"

ECS_CVSID("$Id: ecssplit.c,v 1.3 2003/08/27 05:27:21 warmerda Exp $");

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

/* 
   -------------------------------------------------
  
   ecs_GetRegex: Allocate and set a string with the
   values contained in the regex.

   IN: 
      ecs_regexp *reg: A pointer to ecs_regexp structure
         (initialize in ecs_SplitURL).
      int index: Index of the table contained in ecs_regexp
   OUT:
      char **chaine: String returned value
      return int: Error code
          TRUE: success
          FALSE: failure

   -------------------------------------------------
   */

int ecs_GetRegex(reg,index,chaine)
     ecs_regexp *reg;
     int index;
     char **chaine;
{
  int chaine_len;

  chaine_len = reg->endp[index] - reg->startp[index] ;
  *chaine = (char *) malloc(chaine_len+1);
  if (*chaine ==NULL)
    return FALSE;
  strncpy(*chaine,reg->startp[index],chaine_len);
  (*chaine)[chaine_len] = '\0';
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
     char *url;
     char **machine;
     char **server;
     char **path;
{
  static int compiled = 0;
  static ecs_regexp *local,*remote;
  int msg;

  if( url == NULL ) { /* Cleanup */
      if( compiled ) {
          compiled = 0;
          free( local );
          free( remote );
          local = NULL;
          remote = NULL;
      }
      return FALSE;
  }

  if (!compiled) {
    remote = EcsRegComp("gltp://([0-9a-zA-Z\\.\\-]+)/([0-9a-zA-Z\\.]+)(.*)");
    local = EcsRegComp("gltp:/([0-9a-zA-Z\\.]+)(.*)");
    compiled = 1;
  }

  *machine = NULL;
  *server = NULL;
  *path = NULL;

  if (strncmp(url,"gltp://",7) != 0) {
    if (EcsRegExec(local,url,NULL) == 0)
      return FALSE;
    if (((msg = ecs_GetRegex(local,1,server)) == FALSE) ||
	((msg = ecs_GetRegex(local,2,path)) == FALSE)) {
      ecs_freeSplitURL(machine,server,path);
      return msg;
    }
  } else {
    if (EcsRegExec(remote,url,NULL) == 0)
      return FALSE;
    if (((msg = ecs_GetRegex(remote,1,machine)) == FALSE) ||
	((msg = ecs_GetRegex(remote,2,server)) == FALSE) ||
 	((msg = ecs_GetRegex(remote,3,path)) == FALSE)) {
      ecs_freeSplitURL(machine,server,path);
      return msg;
    }
  }
							 
  return TRUE;
}


