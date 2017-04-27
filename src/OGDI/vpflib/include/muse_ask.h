#include "xvt.h"
#if (XVT_CC == XVT_CC_SUNB)
#include <sys/varargs.h>
#else
#include <stdarg.h>
#endif

#ifndef MUSE_ASK
#define MUSE_ASK 103
#endif
#ifndef M_RESP_DEFAULT
#define M_RESP_DEFAULT 1
#endif
#ifndef M_RESP_2
#define M_RESP_2 2
#endif
#ifndef M_RESP_3
#define M_RESP_3 3
#endif
#ifndef M_RESP_4
#define M_RESP_4 4
#endif
#ifndef M_RESP_5
#define M_RESP_5 5
#endif
#ifndef M_RESP_6
#define M_RESP_6 6
#endif
#ifndef M_RESP_7
#define M_RESP_7 7
#endif
#ifndef M_RESP_8
#define M_RESP_8 8
#endif
#ifndef M_RESP_9
#define M_RESP_9 9
#endif
#ifndef M_RESP_10
#define M_RESP_10 10
#endif
#ifndef M_MSG
#define M_MSG 11
#endif

#if XVTWS == PMWS
#ifndef URL_SRC_WIDTH
#define URL_SRC_WIDTH   6
#endif
#ifndef URL_SRC_HEIGHT
#define URL_SRC_HEIGHT  16
#endif
#else
#ifndef URL_SRC_WIDTH
#define URL_SRC_WIDTH   8
#endif
#ifndef URL_SRC_HEIGHT
#define URL_SRC_HEIGHT  16
#endif
#endif

#if XVTWS == WINWS || XVTWS == PMWS
#define UNITS U_SEMICHARS
#undef  URL_DEST_WIDTH
#define URL_DEST_WIDTH  4
#undef  URL_DEST_HEIGHT
#define URL_DEST_HEIGHT 8
#elif XVTWS == WMWS
#define UNITS U_CHARS
#undef  URL_DEST_WIDTH
#define URL_DEST_WIDTH  1
#undef  URL_DEST_HEIGHT
#define URL_DEST_HEIGHT 1
#else
#define UNITS U_PIXELS
#ifndef URL_DEST_WIDTH
#define URL_DEST_WIDTH  URL_SRC_WIDTH
#endif
#ifndef URL_DEST_HEIGHT
#define URL_DEST_HEIGHT URL_SRC_HEIGHT
#endif
#endif

/* define utility macros */

#define X_OFF(x)  ((x)*URL_DEST_WIDTH)/URL_SRC_WIDTH
#define Y_OFF(y)  ((y)*URL_DEST_HEIGHT)/URL_SRC_HEIGHT
#define WIDTH(w)  ((w)*URL_DEST_WIDTH)/URL_SRC_WIDTH
#define HEIGHT(h) ((h)*URL_DEST_HEIGHT)/URL_SRC_HEIGHT

/* get control window in dialog */

#define CTL_WIN(id) get_ctl_window(win,(id))

STATICFCN int32 dlg_eh XVT_CC_ARGS((WINDOW win, EVENT *ep));
STATICFCN void do_control XVT_CC_ARGS((WINDOW win, short id, CONTROL_INFO *cip));

#if (XVT_CC == XVT_CC_SUNB)
extern int32    muse_ask  XVT_CC_ARGS((char* msgbuf, int num_buttons, va_list arg_ptr ));
#else
extern int32    muse_ask  XVT_CC_ARGS((char* msgbuf, int num_buttons, ... ));
#endif
