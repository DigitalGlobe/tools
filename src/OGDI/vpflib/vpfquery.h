#ifndef __VPFQUERY_H__
#define __VPFQUERY_H__

#ifndef __VPFVIEW_H__
#include "vpfview.h"
#endif
#ifndef __SET_H__
#include "set.h"
#endif
#ifndef __LIKLIST_H__
#include "linklist.h"
#endif

/* Function Definitions */
#ifdef PROTO
   set_type query_table (char *expression, vpf_table_type table );
   linked_list_type parse_expression (char *expression, vpf_table_type table);
#else
   set_type query_table ();
   linked_list_type parse_expression ();
#endif

#endif /* ifdef __VPFQUERY_H__ */
