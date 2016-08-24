#ifndef H_PRINT_FUNC

#define H_PRINT_FUNC

#ifndef INCL_XVTH
#include "xvt.h"
#endif

void
#if XVT_CC_PROTO
get_printer_size(PRINT_RCD *pr, int32 *height, int32 *width);
#else
get_printer_size();
#endif

#if XVT_CC_PROTO
BOOLEAN
print_path( void );
#else
BOOLEAN
print_path();
#endif

#endif

