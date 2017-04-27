/* I_STAT.H */

#ifndef i_stat_h
#define i_stat_h

#include "xvt.h"

extern BOOLEAN    iCreateStatus   XVT_CC_ARGS((char *,char *));
extern BOOLEAN    iUpdateStatus   XVT_CC_ARGS((char *,char *));
extern void       iDestroyStatus  XVT_CC_ARGS((void));
extern BOOLEAN    iCheckCancel    XVT_CC_ARGS((void));

#endif /* i_stat_h */
