#ifndef H_MUSED_FUNC
#define H_MUSED_FUNC

#ifndef H_MUSEDEF
#include "musedef.h"
#endif

#ifndef H_MUSED_DEF
#include "museddef.h"
#endif

#if  XVT_OS != XVT_OS_MAC

#if XVT_CC_PROTO
ERRSTATUS       muse_dll_link(char *parent_program, char *dll_filename, FARPROC * function_ptr);
ERRSTATUS       muse_dll_unlink(void);
ERRSTATUS       muse_dll_error(char *user_message);
#else
ERRSTATUS       muse_dll_link();
ERRSTATUS       muse_dll_unlink();
ERRSTATUS       muse_dll_error();
#endif

#endif

#endif
