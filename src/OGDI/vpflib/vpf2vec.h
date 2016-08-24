/**********************************************************************/
/*   VPF2VEC.H                                                         */
/*      Dan Maddux, DMASC/WGEA, 12 March 1993                         */
/**********************************************************************/
#ifndef VPF2VEC_H
#define VPF2VEC_H

#ifndef __VPFVIEW_H__
#include "vpfview.h"
#endif
#ifndef __VPFRELAT_H__
#include "vpfrelat.h"
#endif
#ifndef PARAMS_H
#include "params.h"
#endif
#ifndef H_VEC_D
#include "vec_d.h"
#endif

/* Copied from grpim.h */
#ifndef NoLine
#define NoLine -1
#endif

#define PATHSIZE 255

#if 0
typedef struct
   {
   char *description;
   int32 value;
   double relem;
   } DATA;
typedef struct
   {
   double x;
   double y;
   } DATA_AREA;
#endif

/* Global Declarations */
vpf_projection_type NOPROJ = {DDS, 0.0, 0.0, 0.0, 0.0, 0, 0.0, 0.0,
                              NULL, NULL, "Decimal Degrees     "};



/* Prototype Definitions */
#ifdef PROTO
   void vpf2vec (USER_PARAMS *params, VEC *vec);
   set_type query_table (char *expression, vpf_table_type table);
   int32 get_features (VEC*, view_type*, map_environment_type*);
   set_type get_selected_tile_primitives (char *covpath, fcrel_type fcrel, int32 primclass,
                                          set_type feature_rows, map_environment_type *mapenv,
					                           int32 tile, char *tiledir, int32 *status);
   set_type primitives_within_extent (extent_type mapextent, char *covpath, char *tiledir,
                                      int32 primclass, int32 numprims);
   set_type get_tile_set (char*, extent_type, BOOLEAN*);
   void clean_up (view_type);
   set_type get_fit_primitives (char *covpath, int32 primclass, set_type fset,
			        set_type sprims, int32 tile, int32 fca_id,
				int32 numprims, BOOLEAN *stop_processing);
   set_type get_prim_set (char *covpath, int32 primclass, char *expression,
			  vpf_table_type ft, set_type sprims,
                          int32 tile, int32 numprims);

#else
   void vpf2vec ();
   set_type query_table ();
   int32 get_features ();
   set_type get_selected_tile_primitives ();
   set_type primitives_within_extent ();
   set_type get_tile_set ();
   void clean_up ();
   set_type get_fit_primitives ();
   set_type get_prim_set ();
#endif

#endif /* VPF2VEC_H */
