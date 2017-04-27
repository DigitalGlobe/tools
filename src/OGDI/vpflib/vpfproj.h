
#ifndef __VPFPROJ_H__
#define __VPFPROJ_H__ 1

#ifndef __COORGEOM_H__
#include "coorgeom.h"
#endif
#ifndef __VPF_H__
#include "vpf.h"
#endif

#ifdef PROTO
   vpf_projection_type set_vpf_projection_parms (vpf_projection_code code,
                                                         extent_type extent);
   void set_vpf_forward_projection( vpf_projection_type proj );
   void set_vpf_inverse_projection( vpf_projection_type proj );
   vpf_projection_type get_vpf_forward_projection();
   vpf_projection_type get_vpf_inverse_projection();
   double central_meridian( double minlon, double maxlon );
   int32 plate_carree_fwd( double *x, double *y );
   int32 plate_carree_inv( double *x, double *y );
   int32 albers_equal_area_fwd( double *x, double *y );
   int32 albers_equal_area_inv( double *x, double *y );
   int32 azimuthal_equal_area_fwd( double *x, double *y );
   int32 azimuthal_equal_area_inv( double *x, double *y );
   int32 azimuthal_equal_distance_fwd( double *x, double *y );
   int32 azimuthal_equal_distance_inv( double *x, double *y );
   int32 gnomonic_fwd( double *x, double *y );
   int32 gnomonic_inv( double *x, double *y );
   int32 lambert_conformal_conic_fwd( double *x, double *y );
   int32 lambert_conformal_conic_inv( double *x, double *y );
   int32 lambert_equal_area_fwd( double *x, double *y );
   int32 lambert_equal_area_inv( double *x, double *y );
   int32 mercator_fwd( double *x, double *y );
   int32 mercator_inv( double *x, double *y );
   int32 oblique_mercator_fwd( double *x, double *y );
   int32 oblique_mercator_inv( double *x, double *y );
   int32 orthographic_fwd( double *x, double *y );
   int32 orthographic_inv( double *x, double *y );
   int32 polar_stereographic_fwd( double *x, double *y );
   int32 polar_stereographic_inv( double *x, double *y );
   int32 transverse_mercator_fwd( double *x, double *y );
   int32 transverse_mercator_inv( double *x, double *y );
   int32 UTM_fwd( double *x, double *y );
   int32 UTM_inv( double *x, double *y );
   int32 forward_project( double *x, double *y );
   int32 inverse_project( double *x, double *y );
   void set_vpf_coordinate_projection( vpf_projection_type proj );
#else
   vpf_projection_type set_vpf_projection_parms ();
   void set_vpf_forward_projection();
   void set_vpf_inverse_projection();
   vpf_projection_type get_vpf_forward_projection();
   vpf_projection_type get_vpf_inverse_projection();
   double central_meridian();
   int32 plate_carree_fwd();
   int32 plate_carree_inv();
   int32 albers_equal_area_fwd();
   int32 albers_equal_area_inv();
   int32 azimuthal_equal_area_fwd();
   int32 azimuthal_equal_area_inv();
   int32 azimuthal_equal_distance_fwd();
   int32 azimuthal_equal_distance_inv();
   int32 gnomonic_fwd();
   int32 gnomonic_inv();
   int32 lambert_conformal_conic_fwd();
   int32 lambert_conformal_conic_inv();
   int32 lambert_equal_area_fwd();
   int32 lambert_equal_area_inv();
   int32 mercator_fwd();
   int32 mercator_inv();
   int32 oblique_mercator_fwd();
   int32 oblique_mercator_inv();
   int32 orthographic_fwd();
   int32 orthographic_inv();
   int32 polar_stereographic_fwd();
   int32 polar_stereographic_inv();
   int32 transverse_mercator_fwd();
   int32 transverse_mercator_inv();
   int32 UTM_fwd();
   int32 UTM_inv();
   int32 forward_project();
   int32 inverse_project();
   void set_vpf_coordinate_projection();
#endif
#endif
