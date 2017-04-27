#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <signal.h>

#ifdef __GNU_LIBRARY__
#include <wait.h>
#endif

#include <ogdi_macro.h>

extern  void G_warning (char *msg);

#define XVT_OS NONE
#ifdef unix
#define XVT_OS_WIN NULL
#endif

#define MAXLONG LONG_MAX
#define MAXSHORT SHRT_MAX
#ifdef _WINDOWS
#define MAXFLOAT DBL_MAX
#endif
#define DIR_SEPARATOR '\\'
#define OS_SEPARATOR '\\'
#define OS_SEPARATOR_STRING "\\"
#define SZ_FNAME 256

/* typedef void * GHANDLE; */
#define GHANDLE void *
#define xvt_zmalloc(n) calloc(n,1)
#define xvt_free(a) free(a)
#define xvt_note printf
#define xvt_malloc(a) malloc(a)
#define xvt_realloc(m,s) realloc(m,s)
#define gmemset memset


#define xvt_fatal printf
#define gunlock
#define gfree(a) free(a)
#define xvt_error printf
#define XVT_CC_ARGS(p) p
 
#define BOOLEAN unsigned char
/* typedef unsigned char BOOLEAN; */

#define UNUSED_RET(x) do { if(x) {} } while(0)
