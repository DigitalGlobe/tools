/* VPF_BROWSE.H */
#ifndef vpf_browse_h
#define vpf_browse_h

#include "xvt.h"
#include "vpftable.h"

#ifndef BOOLEAN
typedef short BOOLEAN;
#endif

/* Browse Data Structure Definition */
typedef struct
   {
   char *path;
   BOOLEAN hdr_data;
   BOOLEAN field_desc;
   BOOLEAN field_labels;
   BOOLEAN cancel;
   int32    nr_fields;
   int32    start_record;
   int32    end_record;
   int32    string_length;
   } BROWSE;

/* Prototype Declaration */
#ifdef PROTO
   void     vpf_browse (BROWSE*);
   SLIST    table_to_slist (BROWSE*);
   SLIST    idx_to_str (BROWSE*, SLIST);
   SLIST    si_to_str  (BROWSE*, SLIST);
   SLIST    ti_to_str  (BROWSE*, SLIST);
   BOOLEAN  add_string (SLIST, char*, char*);
   BOOLEAN  str2list   (SLIST, char*);
#else
   void     vpf_browse ();
   SLIST    table_to_slist ();
   SLIST    idx_to_str ();
   SLIST    si_to_str  ();
   SLIST    ti_to_str  ();
   BOOLEAN  add_string ();
   BOOLEAN  str2list   ();
#endif

#endif
