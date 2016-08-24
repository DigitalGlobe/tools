
#ifndef __VPFPRIM_H__
#define __VPFPRIM_H__ 1

#ifndef __COORGEOM_H__
#include "coorgeom.h"
#endif
#ifndef __SET_H__
#include "set.h"
#endif
#ifndef _VPF_TABLE_H_
#include "vpftable.h"
#endif
#ifndef __VPF_H__
#include "vpf.h"
#endif

/* VPF edge record internal structure */
typedef struct {
   int32 id;
   int32 start_node;
   int32 end_node;
   int32 right_face;
   int32 left_face;
   int32 right_edge;
   int32 left_edge;
   char dir;
   int32 npts;
   double_coordinate_type *coords;
   /* For coordinate arrays too large for memory (DOS support) */
   FILE *fp;
   int32 startpos, pos, current_coordinate;
   char coord_type;
   int32 (*projfunc)();
} edge_rec_type;


/* "static" part of the edge record (non-variable) */
typedef struct {
   int32 id;
   int32 start_node;
   int32 end_node;
   int32 right_face;
   int32 left_face;
   int32 right_edge;
   int32 left_edge;
   char dir;
   int32 npts;
} edge_static_type;
 
 
/* VPF face record structure */
typedef struct {
   int32 id;
   int32 ring;
} face_rec_type;
 
 
/* VPF ring record structure */
typedef struct {
   int32 id;
   int32 face;
   int32 edge;
} ring_rec_type;
 
 
/* VPF entity node record internal structure */
typedef struct {
   int32 id;
   int32 face;
   int32 first_edge;
   double x;
   double y;
} node_rec_type;
 
 
/* VPF text record internal structure */
typedef struct {
   int32 id;
   char  *text;
   double x;
   double y;
} text_rec_type;
 

#ifdef PROTO
   edge_rec_type create_edge_rec( row_type row, vpf_table_type edge_table,
			       int32 (*projfunc)() );
   edge_rec_type read_edge( int32 id,
                         vpf_table_type edge_table,
			 int32 (*projfunc)() );
   edge_rec_type read_next_edge( vpf_table_type edge_table,
			      int32 (*projfunc)() );
   double_coordinate_type first_edge_coordinate( edge_rec_type *edge_rec );
   double_coordinate_type next_edge_coordinate( edge_rec_type *edge_rec );
   double_coordinate_type get_edge_coordinate( int32 n,
                                            edge_rec_type *edge_rec );
   face_rec_type read_face( int32 id,
                         vpf_table_type face_table );
   face_rec_type read_next_face( vpf_table_type face_table );
   ring_rec_type read_ring( int32 id,
                         vpf_table_type ring_table );
   ring_rec_type read_next_ring( vpf_table_type ring_table );
   node_rec_type read_node( int32 id,
                         vpf_table_type node_table,
			 int32 (*projfunc)() );
   node_rec_type read_next_node( vpf_table_type node_table, int32 (*projfunc)() );
   text_rec_type read_text( int32 id,
                         vpf_table_type text_table,
			 int32 (*projfunc)() );
   text_rec_type read_next_text( vpf_table_type text_table,
			      int32 (*projfunc)() );
   set_type bounding_select( char *brpath, extent_type extent, 
                          vpf_projection_type invproj );
   vpf_table_type open_bounding_rect( char *covpath, char *tiledir,
				   int32 pclass );
   extent_type read_bounding_rect( int32 row, vpf_table_type brtable,
				int32 (*projfunc)() );
   extent_type read_next_bounding_rect( vpf_table_type brtable,
				     int32 (*projfunc)() );
#else
   edge_rec_type create_edge_rec ();
   edge_rec_type read_edge ();
   edge_rec_type read_next_edge ();
   double_coordinate_type first_edge_coordinate ();
   double_coordinate_type next_edge_coordinate ();
   double_coordinate_type get_edge_coordinate ();
   face_rec_type read_face ();
   face_rec_type read_next_face ();
   ring_rec_type read_ring ();
   ring_rec_type read_next_ring ();
   node_rec_type read_node ();
   node_rec_type read_next_node ();
   text_rec_type read_text ();
   text_rec_type read_next_text ();
   set_type bounding_select ();
   vpf_table_type open_bounding_rect ();
   extent_type read_bounding_rect ();
   extent_type read_next_bounding_rect ();
#endif /* If PROTO */

#endif
