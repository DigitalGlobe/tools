/* VPFSELEC.H - SELECT VPF FEATURES */

#ifndef __VPFSELEC_H__

#define __VPFSELEC_H__ 1

#ifndef __SET_H__
#include "set.h"
#endif
#ifndef __VPFVIEW_H__
#include "vpfview.h"
#endif
#ifndef __VPFRELAT_H__
#include "vpfrelat.h"
#endif

#ifdef PROTO
   set_type get_fit_tile_primitives (char *covpath, int32 primclass, char *expression,
                                  vpf_table_type feature_table, int32 tile, int32 fca_id,
				                      int32 numprims, int32 *status);
#else
   set_type get_fit_tile_primitives ();
#endif

#endif
