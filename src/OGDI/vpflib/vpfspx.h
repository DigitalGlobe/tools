#ifndef __VPFSPX_H__

#define __VPFSPX_H__

#ifndef __SET_H__
#include "set.h"
#endif

/* Function Definitions */
#ifdef PROTO
   set_type spatial_index_search( char *spxname, float x1, float y1,
	                                float x2, float y2 );
#else
	 set_type spatial_index_search ();
#endif

#endif /* ifdef __VPFSPX_H__ */
