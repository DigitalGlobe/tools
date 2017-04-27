#ifndef __STRFUNC_H__
#define __STRFUNC_H__

#include "stdio.h"
#include <string.h>

#define VPF_SEPARATOR '\\'
#define VPF_SEPARATOR_STRING "\\"

#ifdef _MSDOS
#define OS_SEPARATOR '\\'
#define OS_SEPARATOR_STRING "\\"
#define DIR_SEPARATOR '\\'
#endif

#ifdef _UNIX
#define OS_SEPARATOR '/'
#define OS_SEPARATOR_STRING "/"
#define DIR_SEPARATOR '/'
#endif

#ifdef _MAC
#define OS_SEPARATOR ':'
#define OS_SEPARATOR_STRING ":"
#define DIR_SEPARATOR ':'
#endif



/* Functions: */
#ifdef PROTO
   char *strupr( char *str);
   char *strlwr( char *str);
   char *leftjust( char *str);
   char *rightjust( char *str);
   char *justify (char *str);
   int Mstrcmpi (const char *string1, const char *string2);
   int Mstrncmpi (const char *string1, const char *string2, size_t size);
#else

   char *strupr ();
   char *strlwr ();
   char *leftjust ();
   char *rightjust ();
   char *justify ();
   int Mstrcmpi ();
   int Mstrncmpi ();
#endif

#endif /* ifdef __STRFUNC_H__ */
