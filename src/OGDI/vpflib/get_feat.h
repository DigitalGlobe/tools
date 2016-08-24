/* GET_FEATURE.H */

#ifndef GET_FEATURE_H
#define GET_FEATURE_H

#ifndef H_VEC_D
#include "../vec_d.h"
#endif

/*  Prototype Definitions */
#ifdef PROTO
   void get_point_feature (POINT_FEATURE*, row_type, vpf_table_type);
   void get_text_feature (TEXT_FEATURE*, row_type, vpf_table_type);
   void get_line_feature (LINE_FEATURE*, row_type, vpf_table_type);
   void get_area_feature (AREA_FEATURE*, int32, vpf_table_type, 
                                 vpf_table_type, vpf_table_type);
   void get_ring_coords (RING*, int32, int32, vpf_table_type);
   int32 next_face_edge (edge_rec_type*, int32*, int32);
   COORDINATE *get_xy (vpf_table_type, row_type, int32, int32*);
#else
   void get_point_feature ();
   void get_text_feature ();
   void get_line_feature ();
   void get_area_feature ();
   void get_ring_coords ();
   int32 next_face_edge ();
   COORDINATE *get_xy ();
#endif

#endif /* GET_FEATURE_H */

