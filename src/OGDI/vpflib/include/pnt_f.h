

#ifndef H_PNT_F

#define H_PNT_F

#ifndef H_COORD_DEF
#include "coord_d.h"
#endif

#ifndef H_VALIDFUN
#include "valid_f.h"
#endif


/***************************************************************
@   check_for_ltr_x_error()
****************************************************************
Check for invalid zones when latitude band is X
*/
#if XVT_CC_PROTO
BOOLEAN
check_for_ltr_x_error(char *mgrs);
#else
BOOLEAN
check_for_ltr_x_error();
#endif
/*
Description: In MGRS, zones 32, 34, and 36 do not exist in latitude band X.
If user has entered one of these zones
followed by an X, return an error message in the MGRS string.
*/

/***************************************************************
@     check_for_zone_and_100kms_error()
****************************************************************
Check for incompatibility between the MGRS zone and the first
letter of the 100,000-meter square designation
*/
#if XVT_CC_PROTO
BOOLEAN check_for_zone_and_100kms_error(int iset, char *string);
#else
BOOLEAN check_for_zone_and_100kms_error();
#endif
/*
Description: An MGRS zone belongs to one of six zone sets, and the
set determines which letters are valid for the first character of the
100,000-meter square.  This function returns TRUE if incompatiibility
is found. See Appendix B of DMA TM 8358.1.
*/


/***************************************************************
@     check_for_zone_and_100kms_error()
****************************************************************
Check for incompatibility between the MGRS zone and the first
letter of the 100,000-meter square designation
*/
#if XVT_CC_PROTO
BOOLEAN check_for_zone_and_100kms_error(int iset, char *string);
#else
BOOLEAN check_for_zone_and_100kms_error();
#endif
/*
Description: An MGRS zone belongs to one of six zone sets, and the
set determines which letters are valid for the first character of the
100,000-meter square.  This function returns TRUE if incompatiibility
is found. See Appendix B of DMA TM 8358.1.
*/


/***************************************************************
@   check_integer()
****************************************************************
Check that edit field contains a valid integer with no more
than the given number of digits
*/
#if XVT_CC_PROTO
BOOLEAN check_integer(short length, char *string, BOOLEAN negs_ok,
                      int *caret_pos);
#else
BOOLEAN check_integer();
#endif
/*
Description:  Checks each character in a string and
returns TRUE if a non-digit or too many digits are found.  This
method is used for dialog box fields because E_CHAR events are not
sent or are not portable.
*/


/***************************************************************
@   check_real()
****************************************************************
Check that edit field contains a valid real number with no
more than the given total number of digits and the given number
of digits to the right of the decimal place
*/
#if XVT_CC_PROTO
BOOLEAN check_real(short width, short precision, char *string,
                   int *caret_pos);
#else
BOOLEAN check_real();
#endif
/*
Description: checks each character in a string and
returns TRUE if a non-digit or too many digits are found.  This
method is used for dialog box fields because E_CHAR events are not
sent or are not portable.
*/

/***************************************************************
@   get_input_pt();
****************************************************************
*/
#if XVT_CC_PROTO
ERRSTATUS
get_input_pt( COORD_TYPE *coord_type, POINT_TYPE *point, int *pt_task_id,
	      char *client_name );
#else
ERRSTATUS
get_input_pt( );
#endif


/***************************************************************
@   notify_inv_UTM_easting();
****************************************************************
Displays error message for out-of-range easting value
*/
#if XVT_CC_PROTO
BOOLEAN
notify_inv_UTM_easting(BOOLEAN *flag, WINDOW win, int control_id);
#else
BOOLEAN
notify_inv_UTM_easting();
#endif
/*
Description: In UTM point-entry screen, displays message and returns focus
to the easting field if field is blank or value is not in the range
100000 to 900000.
*/

/***************************************************************
@   notify_inv_lat_band()
****************************************************************
Displays error message for blank latitude band field
*/
#if XVT_CC_PROTO
BOOLEAN
notify_inv_lat_band(BOOLEAN *flag, WINDOW win, int control_id);
#else
BOOLEAN
notify_inv_lat_band();
#endif
/*
Description: In MGRS point-entry screen, displays message and returns focus
to the latitude band field if it is blank
*/


/***************************************************************
@notify_inv_lat_deg()
****************************************************************
Displays error message for out-of-range GP latitude value
*/
#if XVT_CC_PROTO
BOOLEAN
notify_inv_lat_deg(BOOLEAN *flag, WINDOW win, int control_id, double maxval);
#else
BOOLEAN
notify_inv_lat_deg();
#endif
/*
Description: In point-entry screen, displays message and returns focus to
the latitude degrees field if value > 90.
*/

/***************************************************************
@notify_inv_lon_deg()
****************************************************************
Displays error message for out-of-range GP longitude value
*/
#if XVT_CC_PROTO
BOOLEAN
notify_inv_lon_deg(BOOLEAN *flag, WINDOW win, int control_id, double maxval);
#else
BOOLEAN
notify_inv_lon_deg();
#endif
/*
Description: In point-entry screen, displays message and returns focus to
the longitude degrees field if value > 180.
*/


/***************************************************************
@notify_inv_lat_min_sec()
****************************************************************
Displays error message for out-of-range latitude minutes or seconds value
*/
#if XVT_CC_PROTO
BOOLEAN
notify_inv_lat_min_sec(BOOLEAN *flag, WINDOW win, int control_id,
                       int deg_control_id);
#else
BOOLEAN
notify_inv_lat_min_sec();
#endif
/*
Description: In GP point-entry screen, displays message and returns focus to
a field with an out-of-range value for minutes or seconds;
*/


/***************************************************************
@notify_inv_lon_min_sec()
****************************************************************
Displays error message for out-of-range longitude minutes or seconds value
*/
#if XVT_CC_PROTO
BOOLEAN
notify_inv_lon_min_sec(BOOLEAN *flag, WINDOW win, int control_id,
                       int deg_control_id);
#else
BOOLEAN
notify_inv_lon_min_sec();
#endif
/*
Description: In GP point-entry screen, displays message and returns focus to
a field with an out-of-range value for minutes or seconds;
*/

/***************************************************************
@   notify_inv_MGRS_zone()
****************************************************************
Displays error message for out-of-range zone value
*/
#if XVT_CC_PROTO
BOOLEAN
notify_inv_MGRS_zone(BOOLEAN *flag, WINDOW win, int control_id);
#else
BOOLEAN
notify_inv_MGRS_zone();
#endif
/*
Description: In MGRS point-entry screen, displays error message and
returns focus to field if zone is invalid
*/


/***************************************************************
@   notify_inv_UTM_northing()
****************************************************************
Displays error message for out-of-range northing value
*/
#if XVT_CC_PROTO
BOOLEAN
notify_inv_UTM_northing(BOOLEAN *flag, WINDOW win, int control_id);
#else
BOOLEAN
notify_inv_UTM_northing();
#endif
/*
Description: In UTM point-entry screen, displays error message and
returns focus to field if northing > 10000000
*/


/***************************************************************
@   notify_inv_UPS_northing()
****************************************************************
Displays error message for out-of-range northing value
*/
#if XVT_CC_PROTO
BOOLEAN
notify_inv_UPS_northing(BOOLEAN *flag, WINDOW win, int control_id);
#else
BOOLEAN
notify_inv_UPS_northing();
#endif
/*
Description: In UPS point-entry screen, displays error message and
returns focus to field if northing is bad.
*/


/***************************************************************
@   notify_inv_UPS_easting()
****************************************************************
Displays error message for out-of-range easting value
*/
#if XVT_CC_PROTO
BOOLEAN
notify_inv_UPS_easting(BOOLEAN *flag, WINDOW win, int control_id);
#else
BOOLEAN
notify_inv_UPS_easting();
#endif
/*
Description: In UPS point-entry screen, displays error message and
returns focus to field if easting is bad.
*/


/***************************************************************
@   notify_inv_UTM_zone()
****************************************************************
Displays error message for out-of-range zone value
*/
#if XVT_CC_PROTO
BOOLEAN
notify_inv_UTM_zone(BOOLEAN *flag, WINDOW win, int control_id);
#else
BOOLEAN
notify_inv_UTM_zone();
#endif
/*
Description: In UTM and MGRS point-entry screen, displays
message and returns focus to field with an out-of-range value for zone;
ie value < 1 or > 60.
*/


/***************************************************************
@   notify_inv_100kms()
****************************************************************
Displays error message for invalid 100,000-meter square value
*/
#if XVT_CC_PROTO
BOOLEAN
notify_inv_100kms(BOOLEAN *flag, WINDOW win, int control_id);
#else
BOOLEAN
notify_inv_100kms();
#endif
/*
Description: In MGRS point-entry screen, displays message and
returns focus to 100,000-meter square field if value has less than
two characters.
*/

/***************************************************************
@   notify_invalid_dm_fields()
****************************************************************
Displays error messages for all invalid DM edit fields
*/
#if XVT_CC_PROTO
BOOLEAN
notify_invalid_dm_fields(WINDOW xdWindow);
#else
BOOLEAN
notify_invalid_dm_fields();
#endif
/*
Description: Calls individual notify functions for each of the
GP DM point-entry edit controls
*/

/***************************************************************
@   notify_invalid_dms_fields()
****************************************************************
Displays error messages for all invalid DMS edit fields
*/
#if XVT_CC_PROTO
BOOLEAN
notify_invalid_dms_fields(WINDOW win);
#else
BOOLEAN
notify_invalid_dms_fields();
#endif
/*
Description: Calls individual notify functions for each of the
GP DMS point-entry edit controls
*/


/***************************************************************
@   notify_inv_x()
****************************************************************
Displays error message for invalid value in grid easting edit field
*/
#if XVT_CC_PROTO
BOOLEAN
notify_inv_x(BOOLEAN *flag, WINDOW win, int control_id, double east_extent,
             double west_extent);
#else
BOOLEAN
notify_inv_x();
#endif

/***************************************************************
@   notify_inv_y()
****************************************************************
Displays error message for invalid value in grid easting edit field
*/
#if XVT_CC_PROTO
BOOLEAN
notify_inv_y(BOOLEAN *flag, WINDOW win, int control_id, double north_extent,
             double south_extent);
#else
BOOLEAN
notify_inv_y();
#endif


#endif /* H_PNT_F */


