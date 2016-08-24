/* Vpfdproj dummy routines */

#ifndef __VPFPROJ_H__
#include "vpfproj.h"
#endif
#ifndef NOREF
#define NOREF(a) a = a
#endif

#ifdef PROTO
   int32 plate_carree_fwd (double *x, double *y)
#else
   int32 plate_carree_fwd (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return(0);
   }

#ifdef PROTO
   int32 plate_carree_inv (double *x, double *y)
#else
   int32 plate_carree_inv (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return(0);
   }

#ifdef PROTO
   void set_vpf_forward_projection (vpf_projection_type proj)
#else
void set_vpf_forward_projection (proj)
vpf_projection_type proj;
#endif

   {
   NOREF (proj);
   return;
   }

#ifdef PROTO
   void set_vpf_inverse_projection (vpf_projection_type proj)
#else
   void set_vpf_inverse_projection (proj)
      vpf_projection_type proj;
#endif

   {
   NOREF (proj);
   return;
   }

#ifdef PROTO
   vpf_projection_type set_vpf_projection_parms (vpf_projection_code code,
                                                 extent_type extent)
#else
   vpf_projection_type set_vpf_projection_parms (code, extent)
      vpf_projection_code code;
      extent_type extent;
#endif

   {
   vpf_projection_type dummy;
   NOREF (code);
   NOREF (extent);
   memset(&dummy, 0, sizeof(dummy));
   return (dummy);
   }

#ifdef PROTO
   vpf_projection_type get_vpf_forward_projection()
#else
   vpf_projection_type get_vpf_forward_projection ()
#endif

   {
   vpf_projection_type dummy;
   memset(&dummy, 0, sizeof(dummy));
   return (dummy);
   }

#ifdef PROTO
   vpf_projection_type get_vpf_inverse_projection()
#else
   vpf_projection_type get_vpf_inverse_projection ()
#endif

   {
   vpf_projection_type dummy;
   memset(&dummy, 0, sizeof(dummy));
   return (dummy);
   }

#ifdef PROTO
   int32 albers_equal_area_fwd (double *x, double *y)
#else
   int32 albers_equal_area_fwd (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 albers_equal_area_inv( double *x, double *y )
#else
   int32 albers_equal_area_inv (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 azimuthal_equal_area_fwd( double *x, double *y )
#else
   int32 azimuthal_equal_area_fwd (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 azimuthal_equal_area_inv (double *x, double *y )
#else
   int32 azimuthal_equal_area_inv (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 azimuthal_equal_distance_fwd (double *x, double *y )
#else
   int32 azimuthal_equal_distance_fwd (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 azimuthal_equal_distance_inv (double *x, double *y )
#else
   int32 azimuthal_equal_distance_inv (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 gnomonic_fwd (double *x, double *y )
#else
   int32 gnomonic_fwd (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 gnomonic_inv (double *x, double *y )
#else
   int32 gnomonic_inv (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 lambert_conformal_conic_fwd ( double *x, double *y )
#else
   int32 lambert_conformal_conic_fwd (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 lambert_conformal_conic_inv (double *x, double *y )
#else
   int32 lambert_conformal_conic_inv (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 lambert_equal_area_fwd( double *x, double *y )
#else
   int32 lambert_equal_area_fwd (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 lambert_equal_area_inv (double *x, double *y )
#else
   int32 lambert_equal_area_inv (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 mercator_fwd( double *x, double *y )
#else
   int32 mercator_fwd (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 mercator_inv (double *x, double *y )
#else
   int32 mercator_inv (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 oblique_mercator_fwd( double *x, double *y )
#else
   int32 oblique_mercator_fwd (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 oblique_mercator_inv ( double *x, double *y )
#else
   int32 oblique_mercator_inv (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 orthographic_fwd ( double *x, double *y )
#else
   int32 orthographic_fwd (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 orthographic_inv ( double *x, double *y )
#else
   int32 orthographic_inv (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 polar_stereographic_fwd (double *x, double *y)
#else
   int32 polar_stereographic_fwd (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 polar_stereographic_inv( double *x, double *y )
#else
   int32 polar_stereographic_inv (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 transverse_mercator_fwd( double *x, double *y )
#else
   int32 transverse_mercator_fwd (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 transverse_mercator_inv ( double *x, double *y )
#else
   int32 transverse_mercataor_inv (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 UTM_fwd( double *x, double *y )
#else
   int32 UTM_fwd (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 UTM_inv( double *x, double *y )
#else
   int32 UTM_inv (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 forward_project( double *x, double *y )
#else
   int32 forward_project (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }

#ifdef PROTO
   int32 inverse_project( double *x, double *y )
#else
   int32 inverse_project (x, y)
      double *x, *y;
#endif

   {
   NOREF (x);
   NOREF (y);
   return (0);
   }


#ifdef PROTO
   void set_vpf_coordinate_projection( vpf_projection_type proj )
#else
   void set_vpf_coordinate_projection (proj)
      vpf_projection_type proj;
#endif
   {
   NOREF (proj);
   return;
   }
