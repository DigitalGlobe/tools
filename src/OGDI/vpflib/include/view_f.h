/*
@Viewfunc
*/
#ifndef H_VIEW_FUNC
#define H_VIEW_FUNC

#ifndef H_MUSEDEF
/*JLL#include "musedef.h"*/
#endif

#if XVT_OS == XVT_OS_WIN
#ifndef __WINDOWS_H
#include "windows.h"
#endif
#endif

/***************************************************************
@    view_close();
****************************************************************
Close map view.
*/
#if XVT_CC_PROTO
ERRSTATUS view_close(WINDOW xvt_win);
#else /* !XVT_CC_PROTO */
ERRSTATUS view_close();
#endif /* XVT_CC_PROTO */
/*
Description:

*/

/***************************************************************
@    view_construct()
****************************************************************
Allocates memory for VIEW structure.
*/
#if XVT_CC_PROTO
ERRSTATUS view_construct(void);
#else
ERRSTATUS view_construct();
#endif
/*
Description:

*/

/***************************************************************
@    view_create();
****************************************************************
Creates map view of screen.
*/
#if XVT_CC_PROTO
#if XVTWS == WINWS
ERRSTATUS view_create(WINDOW xvt_win, HDC MSmem, HBITMAP MSbitmap);
#elif XVTWS == MTFWS
#include <X11/Xlib.h>
ERRSTATUS view_create(WINDOW xvt_win, XImage* image, BYTE pHMEM data, Pixmap pix);
#else /* XVTWS == MACWS */
ERRSTATUS view_create(WINDOW xvt_win);
#endif
#else /* !XVT_CC_PROTO */
ERRSTATUS view_create();
#endif /* XVT_CC_PROTO */
/*
Description:

*/

/***************************************************************
@    view_destruct()
****************************************************************
Deallocates memory for VIEW structure.
*/
#if XVT_CC_PROTO
ERRSTATUS view_destruct(void);
#else
ERRSTATUS view_destruct();
#endif
/*
Description:

*/

/***************************************************************
@    view_get_color()
****************************************************************
Returns a valid color to phigs.
*/
#if XVT_CC_PROTO
void view_get_color(WINDOW xvt_win, int index, int type);
#else
void view_get_color();
#endif
/*
Description:

*/

/***************************************************************
@    view_color_dist()
****************************************************************
Returns a dist color.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API view_color_dist(RGB x, RGB y, int32 *distance);
#else
ERRSTATUS MUSE_API view_color_dist();
#endif
/*
Description:

*/

/***************************************************************
@    view_get_cur()
****************************************************************
Gets current view associated with the window.
*/
#if XVT_CC_PROTO
ERRSTATUS MUSE_API view_get_cur(WINDOW xvt_win, VIEW **view);
#else
ERRSTATUS MUSE_API view_get_cur();
#endif
/*
Description:

*/

/***************************************************************
@    view_new_win()
****************************************************************
Set a new view window.
*/
#if XVT_CC_PROTO
ERRSTATUS view_new_win(WINDOW xvt_win);
#else
ERRSTATUS view_new_win();
#endif
/*
Description:

*/

/***************************************************************
@    view_open();
****************************************************************
Opens map view to write into.
*/
#if XVT_CC_PROTO
ERRSTATUS view_open(WINDOW xvt_win);
#else /* !XVT_CC_PROTO */
ERRSTATUS view_open();
#endif /* XVT_CC_PROTO */
/*
Description:

*/

/***************************************************************
@    view_put_palette()
****************************************************************
Puts the palette out to the native window manager
*/
#if XVT_CC_PROTO
ERRSTATUS view_put_palette(WINDOW xvt_win, PALETTE *palette);
#else
ERRSTATUS view_put_palette();
#endif
/*
Description:

*/

/***************************************************************
@    view_reset()
****************************************************************
Resets map view to original pix map.
*/
#if XVT_CC_PROTO
ERRSTATUS view_reset(WINDOW xvt_win);
#else /* !XVT_CC_PROTO */
ERRSTATUS view_reset();
#endif /* XVT_CC_PROTO */
/*
Description:

*/

/***************************************************************
@    view_scale();
****************************************************************
Opens map view to write into.
*/
#if XVT_CC_PROTO
ERRSTATUS view_scale(WINDOW xvt_win);
#else
ERRSTATUS view_scale();
#endif
/*
Description:

*/
/***************************************************************
@    view_update()
****************************************************************
Refreshes map view
*/
#if XVT_CC_PROTO
ERRSTATUS view_update(WINDOW xvt_win, BOOLEAN force_resample);
#else /* !XVT_CC_PROTO */
ERRSTATUS view_update();
#endif /* XVT_CC_PROTO */
/*
Description:

*
/
/***************************************************************
@    view_remove_bitmap()
****************************************************************
Remove bitmap from view list.
*/
#if XVT_CC_PROTO
ERRSTATUS view_remove_bitmap(WINDOW xvt_win);
#else /* !XVT_CC_PROTO */
ERRSTATUS view_remove_bitmap();
#endif /* XVT_CC_PROTO */
/*
Description:

*/

/***************************************************************
@    view_remove_win()
****************************************************************
Remove window from view list.
*/
#if XVT_CC_PROTO
ERRSTATUS view_remove_win(WINDOW xvt_win);
#else /* !XVT_CC_PROTO */
ERRSTATUS view_remove_win();
#endif /* XVT_CC_PROTO */
/*
Description:

*/

/***************************************************************
@    view_nearest_color()
****************************************************************

*/
#if XVT_CC_PROTO
ERRSTATUS 
view_nearest_color(PALETTE * pal, PALETTE_USAGE palette_usage, RGB rgb, USHORT * pal_color);
#else
ERRSTATUS 
view_nearest_color();
#endif

#if XVT_CC_PROTO
ERRSTATUS 
view_get_list(Pint_list * ws_id_list);
#else
ERRSTATUS 
view_get_list();
#endif

#endif /* #ifndef H_VIEW_FUNC */










