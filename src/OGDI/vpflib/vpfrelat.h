
#ifndef __VPFRELAT_H__
#define __VPFRELAT_H__ 1

#ifndef __LINKLIST_H__
#include "linklist.h"
#endif
#ifndef _VPFTABLE_H_
#include "vpftable.h"
#endif
#ifndef __VPFTIDX_H__
#include "vpftidx.h"
#endif

typedef struct {
   char table1[40];
   char key1[40];
   char table2[40];
   char key2[40];
} vpf_relate_struct;


typedef struct {
   int32 nchain;
   vpf_table_type *table;
   linked_list_type relate_list;
} feature_class_relate_type, fcrel_type;


#ifdef PROTO
   int32 num_relate_paths( char *start_table, char *fcname, vpf_table_type fcs );
   linked_list_type fcs_relate_list( char *fcname, char *start_table,
				  char *end_table, vpf_table_type fcs,
				  int32 npath );
   int32 related_row( void *keyval, vpf_table_type table, char *keyfield,
							       int32 tile_id );
   linked_list_type related_rows( void *keyval, vpf_table_type table,
					       char *keyfield, int32 tile_id );
   fcrel_type select_feature_class_relate( char *covpath, char *fcname,
					char *start_table, char *end_table,
					int32 npath );
   int32 fc_row_number( row_type row, fcrel_type fcrel, int32 tile );
   linked_list_type fc_row_numbers( row_type row, fcrel_type fcrel,
				 int32 tile );
   void deselect_feature_class_relate( fcrel_type *fcrel );
#else
   int32 num_relate_paths ();
   linked_list_type fcs_relate_list ();
   int32 related_row ();
   linked_list_type related_rows ();
   fcrel_type select_feature_class_relate ();
   int32 fc_row_number ();
   linked_list_type fc_row_numbers ();
   void deselect_feature_class_relate ();
#endif  /* If PROTO */

#endif /* VPFRELAT_H */
