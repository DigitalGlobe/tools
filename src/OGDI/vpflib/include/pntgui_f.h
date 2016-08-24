#ifndef H_PNTGUI_F
#define H_PNTGUI_F

#ifndef H_MUSE1
#include "muse1.h"
#endif
#ifndef INCL_XVTH
#include "xvt.h"
#endif

/***************************************************************
@    pnt_call_back
****************************************************************

*/
#if XVT_CC_PROTO
extern void pnt_call_back(int task_id);
#else
extern void pnt_call_back();
#endif
/*
Description:

*/

/***************************************************************
@    send_pnt
****************************************************************

*/
#if XVT_CC_PROTO
ERRSTATUS send_pnt(void *point);
#else
ERRSTATUS send_pnt();
#endif
/*
Description:

*/

/***************************************************************
@    dmpnt_call_back
****************************************************************

*/
#if XVT_CC_PROTO
extern int dmpnt_call_back(int task_id);
#else
extern int dmpnt_call_back();
#endif
/*
Description:

*/

/***************************************************************
@    dmspnt_call_back
****************************************************************

*/
#if XVT_CC_PROTO
extern int dmspnt_call_back(int task_id);
#else
extern int dmspnt_call_back();
#endif
/*
Description:

*/

/***************************************************************
@    georfpnt_call_back
****************************************************************

*/
#if XVT_CC_PROTO
extern int georfpnt_call_back(int task_id);
#else
extern int georfpnt_call_back();
#endif
/*
Description:

*/

/***************************************************************
@    mgrspnt_call_back
****************************************************************

*/
#if XVT_CC_PROTO
extern int mgrspnt_call_back(int task_id);
#else
extern int mgrspnt_call_back();
#endif
/*
Description:

*/

/***************************************************************
@    otherpnt_call_back
****************************************************************

*/
#if XVT_CC_PROTO
extern int otherpnt_call_back(int task_id);
#else
extern int otherpnt_call_back();
#endif
/*
Description:

*/

/***************************************************************
@    utmpnt_call_back
****************************************************************

*/
#if XVT_CC_PROTO
extern int utmpnt_call_back(int task_id);
#else
extern int utmpnt_call_back();
#endif
/*
Description:

*/

/***************************************************************
@    upspnt_call_back
****************************************************************

*/
#if XVT_CC_PROTO
extern int upspnt_call_back(int task_id);
#else
extern int upspnt_call_back();
#endif
/*
Description:

*/

/***************************************************************
@    do_menu_item_quit
****************************************************************
do_menu_item_quit
*/
#if XVT_CC_PROTO
void do_menu_item_quit(WINDOW xdWindow);
#else
void do_menu_item_quit();
#endif
/*
Description:

*/


/***************************************************************
@    do_e_close
****************************************************************
do_e_close
*/
#if XVT_CC_PROTO
void
do_e_close( void );
#else
void
do_e_close();
#endif


/***************************************************************
@    do_menu_item_enter_point
****************************************************************
do_menu_item_enter_point
*/
#if XVT_CC_PROTO
void do_menu_item_enter_point(WINDOW xdWindow);
#else
void do_menu_item_test_enter_point();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_e_create
****************************************************************
do_d101_e_create
*/
#if XVT_CC_PROTO
void do_d101_e_create(WINDOW xdWindow);
#else
void do_d101_e_create();
#endif
/*
Description:

*/

/***************************************************************
@    do_d101_lost_focus
****************************************************************
do_d101_lost_focus
*/
#if XVT_CC_PROTO
void do_d101_lost_focus(WINDOW xdWindow);
#else
void do_d101_lost_focus();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_cancel
****************************************************************
do_d101_cancel
*/
#if XVT_CC_PROTO
void do_d101_cancel(WINDOW xdWindow);
#else
void do_d101_cancel();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_pb_cancel
****************************************************************
do_d101_cancel
*/
#if XVT_CC_PROTO
void do_d101_pb_cancel(WINDOW xdWindow);
#else
void do_d101_pb_cancel();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_pb_done
****************************************************************
do_d101_pb_done
*/
#if XVT_CC_PROTO
void do_d101_pb_done(WINDOW xdWindow);
#else
void do_d101_pb_done();
#endif
/*
Description:

*/


/***************************************************************
@    do_d101_ed_lat_changed
****************************************************************
do_d101_ed_lat_changed
*/
#if XVT_CC_PROTO
void do_d101_ed_lat_changed(WINDOW xdWindow);
#else
void do_d101_ed_lat_changed();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_pb_ed_lat_changed
****************************************************************
do_d101_pb_ed_lat_changed
*/
#if XVT_CC_PROTO
void do_d101_pb_ed_lat_changed(WINDOW xdWindow);
#else
void do_d101_pb_ed_lat_changed();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_latd_changed
****************************************************************
do_d101_pb_ed_latd_changed
*/
#if XVT_CC_PROTO
void do_d101_ed_latd_changed(WINDOW xdWindow);
#else
void do_d101_ed_latd_changed();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_latm_changed
****************************************************************
do_d101_pb_ed_latm_changed
*/
#if XVT_CC_PROTO
void do_d101_ed_latm_changed(WINDOW xdWindow);
#else
void do_d101_ed_latm_changed();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_lats_changed
****************************************************************
do_d101_pb_ed_lats_changed
*/
#if XVT_CC_PROTO
void do_d101_ed_lats_changed(WINDOW xdWindow);
#else
void do_d101_ed_lats_changed();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_lon_changed
****************************************************************
do_d101_ed_lon_changed
*/
#if XVT_CC_PROTO
void do_d101_ed_lon_changed(WINDOW xdWindow);
#else
void do_d101_ed_lon_changed();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_lond_changed
****************************************************************
do_d101_pb_ed_lond_changed
*/
#if XVT_CC_PROTO
void do_d101_ed_lond_changed(WINDOW xdWindow);
#else
void do_d101_ed_lond_changed();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_lonm_changed
****************************************************************
do_d101_pb_ed_lonm_changed
*/
#if XVT_CC_PROTO
void do_d101_ed_lonm_changed(WINDOW xdWindow);
#else
void do_d101_ed_lonm_changed();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_lons_changed
****************************************************************
do_d101_pb_ed_lons_changed
*/
#if XVT_CC_PROTO
void do_d101_ed_lons_changed(WINDOW xdWindow);
#else
void do_d101_ed_lons_changed();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_rb_n
****************************************************************
do_d101_rb_n
*/
#if XVT_CC_PROTO
void do_d101_rb_n(WINDOW xdWindow);
#else
void do_d101_rb_n();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_rb_s
****************************************************************
do_d101_rb_s
*/
#if XVT_CC_PROTO
void do_d101_rb_s(WINDOW xdWindow);
#else
void do_d101_rb_s();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_rb_e
****************************************************************
do_d101_rb_e
*/
#if XVT_CC_PROTO
void do_d101_rb_e(WINDOW xdWindow);
#else
void do_d101_rb_e();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_rb_w
****************************************************************
do_d101_rb_w
*/
#if XVT_CC_PROTO
void do_d101_rb_w(WINDOW xdWindow);
#else
void do_d101_rb_w();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_lat_focus_active
****************************************************************
do_d101_ed_lat_focus_active
*/
#if XVT_CC_PROTO
void do_d101_ed_lat_focus_active(WINDOW xdWindow);
#else
void do_d101_ed_lat_focus_active();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_latd_focus_active
****************************************************************
do_d101_ed_lat_focus_active
*/
#if XVT_CC_PROTO
void do_d101_ed_latd_focus_active(WINDOW xdWindow);
#else
void do_d101_ed_latd_focus_active();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_latm_focus_active
****************************************************************
do_d101_ed_latm_focus_active
*/
#if XVT_CC_PROTO
void do_d101_ed_latm_focus_active(WINDOW xdWindow);
#else
void do_d101_ed_latm_focus_active();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_lats_focus_active
****************************************************************
do_d101_ed_lats_focus_active
*/
#if XVT_CC_PROTO
void do_d101_ed_lats_focus_active(WINDOW xdWindow);
#else
void do_d101_ed_lats_focus_active();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_lon_focus_active
****************************************************************
do_d101_ed_lon_focus_active
*/
#if XVT_CC_PROTO
void do_d101_ed_lon_focus_active(WINDOW xdWindow);
#else
void do_d101_ed_lon_focus_active();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_lond_focus_active
****************************************************************
do_d101_ed_lon_focus_active
*/
#if XVT_CC_PROTO
void do_d101_ed_lond_focus_active(WINDOW xdWindow);
#else
void do_d101_ed_lond_focus_active();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_lonm_focus_active
****************************************************************
do_d101_ed_lonm_focus_active
*/
#if XVT_CC_PROTO
void do_d101_ed_lonm_focus_active(WINDOW xdWindow);
#else
void do_d101_ed_lonm_focus_active();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_lons_focus_active
****************************************************************
do_d101_ed_lons_focus_active
*/
#if XVT_CC_PROTO
void do_d101_ed_lons_focus_active(WINDOW xdWindow);
#else
void do_d101_ed_lons_focus_active();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_lonmin_changed
****************************************************************
do_d101_ed_lonmin_changed
*/
#if XVT_CC_PROTO
void do_d101_ed_lonmin_changed(WINDOW xdWindow);
#else
void do_d101_ed_lonmin_changed();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_latmin_changed
****************************************************************
do_d101_ed_latmin_changed
*/
#if XVT_CC_PROTO
void do_d101_ed_latmin_changed(WINDOW xdWindow);
#else
void do_d101_ed_latmin_changed();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_latmin_focus_active
****************************************************************
do_d101_ed_latmin_focus_active
*/
#if XVT_CC_PROTO
void do_d101_ed_latmin_focus_active(WINDOW xdWindow);
#else
void do_d101_ed_latmin_focus_active();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_lonmin_focus_active
****************************************************************
do_d101_ed_lonmin_focus_active
*/
#if XVT_CC_PROTO
void do_d101_ed_lonmin_focus_active(WINDOW xdWindow);
#else
void do_d101_ed_lonmin_focus_active();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_quad15_changed
****************************************************************
do_d101_ed_quad15_changed
*/
#if XVT_CC_PROTO
void do_d101_ed_quad15_changed(WINDOW xdWindow);
#else
void do_d101_ed_quad15_changed();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_quad1_changed
****************************************************************
do_d101_ed_quad1_changed
*/
#if XVT_CC_PROTO
void do_d101_ed_quad1_changed(WINDOW xdWindow);
#else
void do_d101_ed_quad1_changed();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_quad1_focus_active
****************************************************************
do_d101_ed_quad1_focus_active
*/
#if XVT_CC_PROTO
void do_d101_ed_quad1_focus_active(WINDOW xdWindow);
#else
void do_d101_ed_quad1_focus_active();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_quad15_focus_active
****************************************************************
do_d101_ed_quad15_focus_active
*/
#if XVT_CC_PROTO
void do_d101_ed_quad15_focus_active(WINDOW xdWindow);
#else
void do_d101_ed_quad15_focus_active();
#endif
/*
Description:

*/

/***************************************************************
@    do_d101_ed_zone_changed
****************************************************************
do_d101_ed_zone_changed
*/
#if XVT_CC_PROTO
void do_d101_ed_zone_changed(WINDOW xdWindow);
#else
void do_d101_ed_zone_changed();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_zone_focus_active
****************************************************************
do_d101_ed_zone_focus_active
*/
#if XVT_CC_PROTO
void do_d101_ed_zone_focus_active(WINDOW xdWindow);
#else
void do_d101_ed_zone_focus_active();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_lb_changed
****************************************************************
do_d101_ed_lb_changed
*/
#if XVT_CC_PROTO
void do_d101_ed_lb_changed(WINDOW xdWindow);
#else
void do_d101_ed_lb_changed();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_lb_focus_active
****************************************************************
do_d101_ed_lb_focus_active
*/
#if XVT_CC_PROTO
void do_d101_ed_lb_focus_active(WINDOW xdWindow);
#else
void do_d101_ed_lb_focus_active();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_sq_changed
****************************************************************
do_d101_ed_sq_changed
*/
#if XVT_CC_PROTO
void do_d101_ed_sq_changed(WINDOW xdWindow);
#else
void do_d101_ed_sq_changed();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_sq_focus_active
****************************************************************
do_d101_ed_sq_focus_active
*/
#if XVT_CC_PROTO
void do_d101_ed_sq_focus_active(WINDOW xdWindow);
#else
void do_d101_ed_sq_focus_active();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_easting_changed
****************************************************************
do_d101_ed_easting_changed
*/
#if XVT_CC_PROTO
void do_d101_ed_easting_changed(WINDOW xdWindow);
#else
void do_d101_ed_easting_changed();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_easting_focus_active
****************************************************************
do_d101_ed_easting_focus_active
*/
#if XVT_CC_PROTO
void do_d101_ed_easting_focus_active(WINDOW xdWindow);
#else
void do_d101_ed_easting_focus_active();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_northing_changed
****************************************************************
do_d101_ed_northing_changed
*/
#if XVT_CC_PROTO
void do_d101_ed_northing_changed(WINDOW xdWindow);
#else
void do_d101_ed_northing_changed();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_northing_focus_active
****************************************************************
do_d101_ed_northing_focus_active
*/
#if XVT_CC_PROTO
void do_d101_ed_northing_focus_active(WINDOW xdWindow);
#else
void do_d101_ed_northing_focus_active();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_x_changed
****************************************************************
do_d101_ed_x_changed
*/
#if XVT_CC_PROTO
void do_d101_ed_x_changed(WINDOW xdWindow);
#else
void do_d101_ed_x_changed();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_x_focus_active
****************************************************************
do_d101_ed_x_focus_active
*/
#if XVT_CC_PROTO
void do_d101_ed_x_focus_active(WINDOW xdWindow);
#else
void do_d101_ed_x_focus_active();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_y_changed
****************************************************************
do_d101_ed_y_changed
*/
#if XVT_CC_PROTO
void do_d101_ed_y_changed(WINDOW xdWindow);
#else
void do_d101_ed_y_changed();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_y_focus_active
****************************************************************
do_d101_ed_y_focus_active
*/
#if XVT_CC_PROTO
void do_d101_ed_y_focus_active(WINDOW xdWindow);
#else
void do_d101_ed_y_focus_active();
#endif
/*
Description:

*/
/*template*/
#if 0
/***************************************************************
@    do_d101_ed_zzz_changed
****************************************************************
do_d101_ed_zzz_changed
*/
#if XVT_CC_PROTO
void do_d101_ed_zzz_changed(WINDOW xdWindow);
#else
void do_d101_ed_zzz_changed();
#endif
/*
Description:

*/
/***************************************************************
@    do_d101_ed_zzz_focus_active
****************************************************************
do_d101_ed_zzz_focus_active
*/
#if XVT_CC_PROTO
void do_d101_ed_zzz_focus_active(WINDOW xdWindow);
#else
void do_d101_ed_zzz_focus_active();
#endif
/*
Description:

*/
#endif

#endif /*H_PNTGUI_F*/

