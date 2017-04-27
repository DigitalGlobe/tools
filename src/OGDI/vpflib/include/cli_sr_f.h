

#ifndef H_CLI_SR_F

#define H_CLI_SR_F

#ifndef INCL_XVTH
#include "xvt.h"
#endif

extern WINDOW dlg_wait_eh XVT_CC_ARGS((WINDOW xdWindow, EVENT *xdEvent));


/***************************************************************
@    dlg_wait_open()
****************************************************************
Invoke dialog that displays wait message while client is waiting for
information form server
*/
#if XVT_CC_PROTO
void 
dlg_wait_open(WINDOW xdWindow, char *child_str, char *parent_str);
#else
void
dlg_wait_open();
#endif
/*
Description:
*/


/***************************************************************
@    dlg_wait_close()
****************************************************************
Close dialog that displays wait message while client is waiting for
information form server
*/
#if XVT_CC_PROTO
void 
dlg_wait_close( WINDOW xdWindow );
#else
void
dlg_wait_close();
#endif
/*
Description:
*/

/***************************************************************
@     do_menu_item_quit()
****************************************************************
*/
#if XVT_CC_PROTO
void
do_menu_item_quit(WINDOW xdWindow);
#else
void
do_menu_item_quit();
#endif
/*
Description:
*/


#endif  /* H_CLI_SR_F */


