
/*****************************************************************************/
/* STRFUNC                                                                   */
/*                                                                           */
/*   Purpose:                                                                */
/*     This module contains functions some additional character string       */
/*     handling functions beyond the standard C string libraries.            */
/*     The strings passed in to each of these functions is actually          */
/*     modified and an identical string is returned.                         */
/*   Contents:                                                               */
/*       strupr                                                              */
/*       strlwr                                                              */
/*       strreverse                                                          */
/*       leftjust                                                            */
/*       rightjust                                                           */
/*       justify                                                             */
/*       Mstrcmpi                                                            */
/*       Mstrncmpi                                                           */
/*****************************************************************************/
#ifndef INCL_XVTH
#include <xvt.h>
#endif 

#ifdef _UNIX  
#include <strings.h>
#endif

#ifndef __STRFUNC_H__
#include "strfunc.h"
#endif

#ifndef _MACHINE_
#include "machine.h"
#endif


/*****************************************************************************/
/* strupr                                                                    */
/*                                                                           */
/*   Purpose:                                                                */
/*   This function changes all lowercase characters in a string to           */
/*  uppercase.                                                               */
/*                                                                           */
/*   Parameters:                                                             */
/*     string     <inout>  == (char *) string to be made uppercase.          */
/*     strupr     <output> == (char *) pointer to string.                    */
/*                                                                           */
/*****************************************************************************/
#ifdef PROTO
   char *strupr (char *string)
#else
   char *strupr (string)
      char *string;
#endif

   {
   size_t i;

   if (!string)
      return string;

   for (i=0; i<strlen (string); i++)
      string[i] = (char)toupper (string[i]);
   return string;
   }

/*****************************************************************************/
/* strlwr                                                                    */
/*                                                                           */
/*   Purpose:                                                                */
/*   This function changes all lowercase characters in a string to           */
/*  uppercase.                                                               */
/*                                                                           */
/*   Parameters:                                                             */
/*     string     <inout>  == (char *) string to be made uppercase.          */
/*     strupr     <output> == (char *) pointer to string.                    */
/*                                                                           */
/*****************************************************************************/

#ifdef PROTO
   char *strlwr (char *string)
#else
   char *strlwr (string)
      char *string;
#endif

   {
   size_t i;

   if (!string)
      return string;

   for (i=0; i<strlen (string); i++)
      string[i] = (char)tolower (string[i]);
   return string;
   }


/*************************************************************************
*
*N  strreverse
*
*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
*
*   Purpose:
*P
*     This function reverses the characters in a string.
*E
*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
*
*   Parameters:
*A
*     string     <inout>  == (char *) string to be reversed.
*     strreverse <output> == (char *) pointer to string.
*E
*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
*
*   History:
*H
*     Brian Glover      April 1992     Rewrite of non-ANSI function
*E
*************************************************************************/
#ifdef PROTO
   char *strreverse (char *str)
#else
   char *strreverse (str)
      char *str;
#endif

{
  size_t i, len;
  char *copy;

  len = strlen (str);
  copy = (char*)xvt_malloc (sizeof(char) * (len+1));
  strcpy (copy, str);

  for (i=0; i<len; i++)
    str[i] = copy[(len-1)-i];
  xvt_free (copy);

  return str;
}

/*****************************************************************************/
/*  leftjust                                                                 */
/*                                                                           */
/*   Purpose:                                                                */
/*     This function left justifies a string by removing all leading         */
/*     whitespace.                                                           */
/*                                                                           */
/*   Parameters:                                                             */
/*     string     <inout>  == (char *) string to be left justified.          */
/*     leftjust   <output> == (char *) pointer to string.                    */
/*****************************************************************************/

#ifdef PROTO
   char *leftjust (char *str)
#else
   char *leftjust (str)
      char *str;
#endif

   {
   register char * eol;
   const char *post_white;

   if (!str)
      return str;

   post_white = str + strspn(str, " \t\n\b");
   if( post_white != str )
   {
       memmove( str, post_white, strlen(post_white)+1 );
   }

   if ((eol = strchr (str, '\n')) != NULL)
      *eol = 0;

   return str;
   }


/*****************************************************************************/
/*  rightjust                                                                */
/*                                                                           */
/*   Purpose:                                                                */
/*     This function right justifies a string by removing all trailing       */
/*     whitespace.                                                           */
/*                                                                           */
/*   Parameters:                                                             */
/*     string     <inout>  == (char *) string to be right justified.         */
/*     rightjust  <output> == (char *) pointer to string.                    */
/*****************************************************************************/

#ifdef PROTO
   char *rightjust (char *str)
#else
   char *rightjust (str)
      char *str;
#endif

   {
   size_t len, i;

   len = strlen (str);
   if (len == 0)
     return str;

   i = len - 1;
   while ((i>0) && ((str[i]==0) || (str[i]==' '))) i--;
      if (i < (len-1))
         str[i+1] = '\0';
   for (i=0; i<strlen (str); i++)
      {
      if (str[i]=='\n')        /* Newline char */
         str[i] = '\0';
      else if (str[i] == '\t') /* Tab char */
         str[i] = '\0';
      else if (str[i] == '\b') /* Backspace char */
         str[i] = '\0';
      }
   return str;
   }
 
#ifdef _MAC 
int strncmpi (char *str1, char *str2, size_t len)
   { 

   char *string1, *string2;
   int retvalue;

   string1 = (char*)xvt_zmalloc (len + 1L);
   string2 = (char*)xvt_zmalloc (len + 1L);
   strncpy(string1, str1, len);
   strncpy(string2, str2, len);
   retvalue = strcmpi ((char*)string1, (char*)string2);
   xvt_free (string1);
   xvt_free (string2);
   return (retvalue);
   }
#endif

/*****************************************************************************/
/*  justify                                                                  */
/*                                                                           */
/*   Purpose:                                                                */
/*     This function justifies a string by removing all leading and trailing */
/*     whitespace.                                                           */
/*                                                                           */
/*   Parameters:                                                             */
/*     string     <inout>  == (char*) string to be left justified.           */
/*     justify    <output> == (char*) pointer to string.                     */
/*****************************************************************************/

#ifdef PROTO
   char *justify (char *str)
#else
   char *justify (str)
      char *str;
#endif

   {
   str = leftjust (str);
   str = rightjust (str);
   return (str);
   }


/*****************************************************************************/
/*  Mstrcmpi                                                                 */
/*    MUSE platform independent form of a case insensitive string compare    */
/*****************************************************************************/
#ifdef PROTO
   int Mstrcmpi (const char* string1, const char *string2)
#else
   int Mstrcmpi (string1, string2)
      const char *string1;
      const char *string2;
#endif

   {
   int value;

#ifdef _MAC
   value = strcmpi ((char*)string1, (char*)string2);
#elif defined(_HPUX_SOURCE) || defined(_UNIX)
   value = strcasecmp (string1, string2);
#else
   value = stricmp (string1, string2);
#endif

   return (value);
   }


/*****************************************************************************/
/*  Mstrncmpi                                                                */
/*    MUSE platform independent form of a function to compare a portion of   */
/* two strings without regard to case.                                       */
/*****************************************************************************/
#ifdef PROTO
   int Mstrncmpi (const char* string1, const char *string2, size_t size)
#else
   int Mstrncmpi (string1, string2, size)
      const char *string1;
      const char *string2;
      size_t size;
#endif

   {
   int value;

#ifdef _MAC
   value = strncmpi ((char*)string1, (char*)string2, size);
#elif defined(_WINDOWS)
   value = strnicmp (string1, string2, size);
#else
   value = strncasecmp (string1, string2, size);
#endif

   return (value);
   }
