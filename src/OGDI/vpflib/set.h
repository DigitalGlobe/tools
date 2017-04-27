/* =====================================================================

   Environmental Systems Research Institute (ESRI) Applications Programming

       Project:                 VPF display software
       Original Coding:         Barry Micheals  November 1990  PC version
       Modifications:           David Flinn     October 1991   UNIX

   ================================================================= */

#ifndef __SET_H__
#define __SET_H__

#include "xvt.h"

#include "machine.h"

/* A set is represented as an array of characters with each character */
/* holding 8 bits of the set. */
typedef struct
   {
   int32     size;
   char /*huge*/ *buf;
   GHANDLE  buf_handle;
   } set_type;


/* Functions: */
#ifdef PROTO
   set_type set_init (int32 n);
   int  set_empty (set_type set);
   void set_insert (int32 element, set_type set);
   void set_delete (int32 element, set_type set);
   int  set_member (int32  element, set_type set);
   int32 set_min (set_type set);
   int32 set_max (set_type set);
   int32 num_in_set (set_type set);
   void set_on (set_type set);
   void set_off (set_type set);
   int  set_equal (set_type a, set_type b);
   void set_assign (set_type *a, set_type b);
   set_type set_union (set_type a, set_type b);
   set_type set_intersection (set_type a, set_type b);
   set_type set_difference (set_type a, set_type b);
   void set_nuke (set_type *set);
#else
   set_type set_init ();
   int  set_empty ();
   void set_insert ();
   void set_delete ();
   int  set_member ();
   int32 set_min ();
   int32 set_max ();
   int32 num_in_set ();
   void set_on ();
   void set_off ();
   int  set_equal ();
   void set_assign ();
   set_type set_union ();
   set_type set_intersection ();
   set_type set_difference ();
   void set_nuke ();
#endif /* If PROTO */

#endif /* ifdef __SET_H__ */

