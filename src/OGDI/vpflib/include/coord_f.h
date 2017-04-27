#ifndef H_COORD_FUNC

#define H_COORD_FUNC

#ifndef INCL_XVTH
#include "xvt.h"
#endif
#ifndef H_COORD_DEF
#include "coord_d.h"
#endif

#if XVT_CC_PROTO
void coord_sys_construct ( FILE *file , COORD_SYS **pointer );
#else
void coord_sys_construct ();
#endif


/*SR 08/07 changed argument types for geographic units to points */
/***************************************************************
@    dd_from_dd()
****************************************************************
Decimal degrees from decimal degrees
*/
#if XVT_CC_PROTO
ERRSTATUS dd_from_dd(
POINT_DD *dd1, POINT_UNITS *user_unit);
#else
ERRSTATUS dd_from_dd();
#endif
/*
Description:

*/

/***************************************************************
@    dd_from_dms()
****************************************************************
Decimal degrees from degrees, minutes, and seconds
*/
#if XVT_CC_PROTO
ERRSTATUS dd_from_dms(
  POINT_DD *dd, POINT_UNITS *user_unit);
#else
ERRSTATUS dd_from_dms();
#endif
/*
Description:

*/

/***************************************************************
@    dd_from_georef()
****************************************************************
Decmial degrees from GEOREF point
*/
#if XVT_CC_PROTO
ERRSTATUS dd_from_georef(
  POINT_DD *dd, POINT_UNITS *user_unit);
#else
ERRSTATUS dd_from_georef();
#endif
/*
Description:

*/

/***************************************************************
@    dd_from_mgrs()
****************************************************************
Decimal degrees from MGRS
*/
#if XVT_CC_PROTO
ERRSTATUS dd_from_mgrs(
  POINT_DD *dd, POINT_UNITS *user_unit);
#else
ERRSTATUS dd_from_mgrs();
#endif
/*
Description:

*/

/***************************************************************
@    dd_from_min()
****************************************************************
Decimal degrees from minutes
*/
#if XVT_CC_PROTO
ERRSTATUS dd_from_min(
  POINT_DD *dd, POINT_UNITS *user_unit);
#else
ERRSTATUS dd_from_min();
#endif
/*
Description:

*/

/***************************************************************
@    dd_from_sec()
****************************************************************
Decimal degrees from seconds
*/
#if XVT_CC_PROTO
ERRSTATUS dd_from_sec(
  POINT_DD *dd, POINT_UNITS *user_unit);
#else
ERRSTATUS dd_from_sec();
#endif
/*
Description:

*/

/***************************************************************
@    dd_from_utm()
****************************************************************
Decimal degrees from UTM
*/
#if XVT_CC_PROTO
ERRSTATUS dd_from_utm(
  POINT_DD *dd, POINT_UNITS *user_unit);
#else
ERRSTATUS dd_from_utm();
#endif
/*
Description:

*/

/***************************************************************
@    dd_to_dd()
****************************************************************
Decimal degrees to decimal degrees
*/
#if XVT_CC_PROTO
ERRSTATUS dd_to_dd(
  POINT_DD *dd, POINT_UNITS *user_unit);
#else
ERRSTATUS dd_to_dd();
#endif
/*
Description:

*/

/***************************************************************
@	dd_to_dms();
****************************************************************
Decimal degrees to degrees, minutes, and seconds
*/
#if XVT_CC_PROTO
ERRSTATUS dd_to_dms(
  POINT_DD *dd, POINT_UNITS *user_unit);
#else
ERRSTATUS dd_to_dms();
#endif
/*
Description:

*/

/***************************************************************
@    dd_to_georef()
****************************************************************
Decimal degree to GEOREF point
*/
#if XVT_CC_PROTO
ERRSTATUS dd_to_georef(
  POINT_DD *dd, POINT_UNITS *user_unit);
#else
ERRSTATUS dd_to_georef();
#endif
/*
Description:

*/


/***************************************************************
@    dd_to_mgrs()
****************************************************************
Decimal degree to MGRS
*/
#if XVT_CC_PROTO
ERRSTATUS dd_to_mgrs(
  POINT_DD *dd, POINT_UNITS *user_unit);
#else
ERRSTATUS dd_to_mgrs();
#endif
/*
Description:

*/

/***************************************************************
@    dd_to_min()
****************************************************************
Decimal degrees to minutes
*/
#if XVT_CC_PROTO
ERRSTATUS dd_to_min(
  POINT_DD *dd, POINT_UNITS *user_unit);
#else
ERRSTATUS dd_to_min();
#endif
/*
Description:

*/

/***************************************************************
@    dd_to_sec()
****************************************************************
Decimal degrees to seconds
*/
#if XVT_CC_PROTO
ERRSTATUS dd_to_sec(
  POINT_DD *dd, POINT_UNITS *user_unit);
#else
ERRSTATUS dd_to_sec();
#endif
/*
Description:

*/


/***************************************************************
@    dd_to_utm()
****************************************************************
Decimal degrees to UTM
*/
#if XVT_CC_PROTO
ERRSTATUS dd_to_utm(
  POINT_DD *dd, POINT_UNITS *user_unit);
#else
ERRSTATUS dd_to_utm();
#endif
/*
Description:

*/


/***************************************************************
@    hm_from_as()
****************************************************************
Horizontal meters from arc seconds
*/
#if XVT_CC_PROTO
ERRSTATUS hm_from_as(
  GFLOAT *hm, GFLOAT as);
#else
ERRSTATUS hm_from_as();
#endif
/*
Description:

*/

/***************************************************************
@    hm_from_hf()
****************************************************************
Horizontal meters from horizontal feet
*/
#if XVT_CC_PROTO
ERRSTATUS hm_from_hf(
  GFLOAT *hm, GFLOAT hf);
#else
ERRSTATUS hm_from_hf();
#endif
/*
Description:

*/

/***************************************************************
@    hm_from_hkm()
****************************************************************
Horizontal meters from horizontal kilometers
*/
#if XVT_CC_PROTO
ERRSTATUS hm_from_hkm(
  GFLOAT *hm, GFLOAT hkm);
#else
ERRSTATUS hm_from_hkm();
#endif
/*
Description:

*/

/***************************************************************
@    hm_from_hl()
****************************************************************
Horizontal meters from horizontal leagues
*/
#if XVT_CC_PROTO
ERRSTATUS hm_from_hl(
  GFLOAT *hm, GFLOAT hl);
#else
ERRSTATUS hm_from_hl();
#endif
/*
Description:

*/

/***************************************************************
@    hm_from_hm()
****************************************************************
Horizontal meters from horizontal meters
*/
#if XVT_CC_PROTO
ERRSTATUS hm_from_hm(
  GFLOAT *hm1, GFLOAT hm2);
#else
ERRSTATUS hm_from_hm();
#endif
/*
Description:

*/

/***************************************************************
@    hm_from_hnm()
****************************************************************
Horizontal meters from horizontal nautical miles
*/
#if XVT_CC_PROTO
ERRSTATUS hm_from_hnm(
  GFLOAT *hm, GFLOAT hnm);
#else
ERRSTATUS hm_from_hnm();
#endif
/*
Description:

*/

/***************************************************************
@    hm_from_hsm()
****************************************************************
Horizontal meters from horizontal statute miles
*/
#if XVT_CC_PROTO
ERRSTATUS hm_from_hsm(
  GFLOAT *hm, GFLOAT hsm);
#else
ERRSTATUS hm_from_hsm();
#endif
/*
Description:

*/

/***************************************************************
@    hm_from_hy()
****************************************************************
Horizontal meters from horizontal yards
*/
#if XVT_CC_PROTO
ERRSTATUS hm_from_hy(
  GFLOAT *hm, GFLOAT hy);
#else
ERRSTATUS hm_from_hy();
#endif
/*
Description:

*/

/***************************************************************
@    hm_to_as()
****************************************************************
Horizontal meters to arc seconds
*/
#if XVT_CC_PROTO
ERRSTATUS hm_to_as(
  GFLOAT hm, GFLOAT *as);
#else
ERRSTATUS hm_to_as();
#endif
/*
Description:

*/

/***************************************************************
@    hm_to_hf()
****************************************************************
Horizontal meters to horizontal feet
*/
#if XVT_CC_PROTO
ERRSTATUS hm_to_hf(
  GFLOAT hm, GFLOAT *hf);
#else
ERRSTATUS hm_to_hf();
#endif
/*
Description:

*/

/***************************************************************
@    hm_to_hkm()
****************************************************************
Horizontal meters to horizontal kilometers
*/
#if XVT_CC_PROTO
ERRSTATUS hm_to_hkm(
  GFLOAT hm, GFLOAT *hkm);
#else
ERRSTATUS hm_to_hkm();
#endif
/*
Description:

*/

/***************************************************************
@    hm_to_hl()
****************************************************************
Horizontal meters to horizontal leagues
*/
#if XVT_CC_PROTO
ERRSTATUS hm_to_hl(
  GFLOAT hm, GFLOAT *hl);
#else
ERRSTATUS hm_to_hl();
#endif
/*
Description:

*/

/***************************************************************
@    hm_to_hm()
****************************************************************
Horizontal meters to horizontal meters
*/
#if XVT_CC_PROTO
ERRSTATUS hm_to_hm(
  GFLOAT hm1, GFLOAT *hm2);
#else
ERRSTATUS hm_to_hm();
#endif
/*
Description:

*/

/***************************************************************
@    hm_to_hnm()
****************************************************************
Horizontal meters to horizontal nautical miles
*/
#if XVT_CC_PROTO
ERRSTATUS hm_to_hnm(
  GFLOAT hm, GFLOAT *hnm);
#else
ERRSTATUS hm_to_hnm();
#endif
/*
Description:

*/

/***************************************************************
@    hm_to_hsm()
****************************************************************
Horizontal meters to horizontal statute miles
*/
#if XVT_CC_PROTO
ERRSTATUS hm_to_hsm(
  GFLOAT hm, GFLOAT *hsm);
#else
ERRSTATUS hm_to_hsm();
#endif
/*
Description:

*/

/***************************************************************
@   invoke_point_dd
****************************************************************

*/
#if XVT_CC_PROTO
WINDOW
invoke_point_dd(int32 data, WINDOW parent_win, RCT *rct);
#else
WINDOW
invoke_point_dd();
#endif
/*
Description:
*/

/***************************************************************
@   invoke_point_dm
****************************************************************

*/
#if XVT_CC_PROTO
WINDOW
invoke_point_dm(int32 data, WINDOW parent_win, RCT *rct);
#else
WINDOW
invoke_point_dm();
#endif
/*
Description:
*/

/***************************************************************
@   invoke_point_dms
****************************************************************

*/
#if XVT_CC_PROTO
WINDOW
invoke_point_dms(int32 data, WINDOW parent_win, RCT *rct);
#else
WINDOW
invoke_point_dms();
#endif
/*
Description:
*/

/***************************************************************
@   invoke_point_mgrs
****************************************************************

*/
#if XVT_CC_PROTO
WINDOW
invoke_point_mgrs(int32 data, WINDOW parent_win, RCT *rct);
#else
WINDOW
invoke_point_mgrs();
#endif
/*
Description:
*/

/***************************************************************
@   invoke_point_utm
****************************************************************

*/
#if XVT_CC_PROTO
WINDOW
invoke_point_utm(int32 data, WINDOW parent_win, RCT *rct);
#else
WINDOW
invoke_point_utm();
#endif
/*
Description:
*/

/***************************************************************
@    hm_to_hy()
****************************************************************
Horizontal meters to horizontal yards
*/
#if XVT_CC_PROTO
ERRSTATUS hm_to_hy(
  GFLOAT hm, GFLOAT *hy);
#else
ERRSTATUS hm_to_hy();
#endif
/*
Description:

*/

/***************************************************************
@    mgrs_to_dd()
****************************************************************
Converts Military Grid Reference System to Decimal Deg.
*/
#if XVT_CC_PROTO
ERRSTATUS mgrs_to_dd(POINT_MGRS mgrs, POINT_DD dd);
#else
ERRSTATUS mgrs_to_dd();
#endif
/*
Description:

*/


/***************************************************************
@    sr_from_sipd()
****************************************************************
Scale reciprocal from scale inches per degree
*/
#if XVT_CC_PROTO
ERRSTATUS sr_from_sipd(
  GFLOAT *sr, GFLOAT sipd);
#else
ERRSTATUS sr_from_sipd();
#endif
/*
Description:

*/

/***************************************************************
@    sr_from_sr()
****************************************************************
Scale reciprocal from scale reciprocal
*/
#if XVT_CC_PROTO
ERRSTATUS sr_from_sr(
  GFLOAT *sr1, GFLOAT sr2);
#else
ERRSTATUS sr_from_sr();
#endif
/*
Description:

*/

/***************************************************************
@    sr_to_sipd()
****************************************************************
Scale reciprocal to scale inches per degree
*/
#if XVT_CC_PROTO
ERRSTATUS sr_to_sipd(
  double sr, double *user_unit);
#else
ERRSTATUS sr_to_sipd();
#endif
/*
Description:

*/

/***************************************************************
@    sr_to_sr()
****************************************************************
Scale reciprocal to scale reciprocal
*/
#if XVT_CC_PROTO
ERRSTATUS sr_to_sr(
  double sr1, double *user_unit);
#else
ERRSTATUS sr_to_sr();
#endif
/*
Description:

*/

/***************************************************************
@    utm_to_dd()
****************************************************************
Converts Universal Transverse Mercator to Decimal Deg.
*/
#if XVT_CC_PROTO
ERRSTATUS utm_to_dd(POINT_UTM utm, POINT_DD dd);
#else
ERRSTATUS utm_to_dd();
#endif
/*
Description:

*/


/***************************************************************
@    vm_from_vf()
****************************************************************
Vertical meters from vertical fathoms
*/
#if XVT_CC_PROTO
ERRSTATUS vm_from_vf(
  GFLOAT *vm, GFLOAT vf);
#else
ERRSTATUS vm_from_vf();
#endif
/*
Description:

*/

/***************************************************************
@    vm_from_vft()
****************************************************************
Vertical meters from vertical feet
*/
#if XVT_CC_PROTO
ERRSTATUS vm_from_vft(
  GFLOAT *vm, GFLOAT vft);
#else
ERRSTATUS vm_from_vft();
#endif
/*
Description:

*/

/***************************************************************
@    vm_from_vm()
****************************************************************
Vertical meters from vertical meters
*/
#if XVT_CC_PROTO
ERRSTATUS vm_from_vm(
  GFLOAT *vm1, GFLOAT vm2);
#else
ERRSTATUS vm_from_vm();
#endif
/*
Description:

*/

/***************************************************************
@    vm_to_vf()
****************************************************************
Vertical meters to vertical fathoms
*/
#if XVT_CC_PROTO
ERRSTATUS vm_to_vf(
  GFLOAT vm, GFLOAT *user_unit);
#else
ERRSTATUS vm_to_vf();
#endif
/*
Description:

*/

/***************************************************************
@    vm_to_vft()
****************************************************************
Vertical meters to vertical feet
*/
#if XVT_CC_PROTO
ERRSTATUS vm_to_vft(
  GFLOAT vm, GFLOAT *user_unit);
#else
ERRSTATUS vm_to_vft();
#endif
/*
Description:

*/

/***************************************************************
@    vm_to_vm()
****************************************************************
Vertical meters to vertical meters
*/
#if XVT_CC_PROTO
ERRSTATUS vm_to_vm(
  GFLOAT vm1, GFLOAT *user_unit);
#else
ERRSTATUS vm_to_vm();
#endif
/*
Description:

*/

#endif    /* H_COORD */
