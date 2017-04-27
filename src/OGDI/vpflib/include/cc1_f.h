
#ifndef H_CC1_F
#define H_CC1_F

#ifndef H_UNIT_DEF
#include "unit_d.h"
#endif
#ifndef H_COORD_DEF
#include "coord_d.h"
#endif


#if XVT_CC_PROTO
ERRSTATUS
coord_2_gp_radian(POINT_UNITS *user_pt, POINT_DD *radian_pt,
		  COORD_SYS *params, char *err_message);
#else
ERRSTATUS
coord_2_gp_radian();
#endif


#if XVT_CC_PROTO
ERRSTATUS
gp_radian_2_coord(POINT_UNITS *user_pt, POINT_DD *radian_pt,
		  COORD_SYS *params, char *err_message);
#else
ERRSTATUS
gp_radian_2_coord();
#endif

#if XVT_CC_PROTO
ERRSTATUS
input_pt_2_output_pt( COORD_SYS *in_sys, COORD_SYS *out_sys,
		      POINT_UNITS *in_pt, DTCC_OUTPUT *out_info );
#else
ERRSTATUS
input_pt_2_output_pt( );
#endif

#if XVT_CC_PROTO
void
convert_pt_to_user_str(COORD_SYS *coord_in, POINT_UNITS *upoint, char *output);
#else
void
convert_pt_to_user_str();
#endif

#if XVT_CC_PROTO
void
conv_lon_to_180_scale(double *rad_lon);
#else
void
convert_pt_to_user_str();
#endif

#endif
