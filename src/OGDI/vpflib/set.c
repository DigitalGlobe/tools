/*************************************************************************
 *
 *N  Module SET.C
 *   Purpose:
 *     This module contains functions that make up an abstract data type
 *     "set".  The data structures and algorithms herein allow programs
 *     to perform basic manipulations defined in the mathematics of set
 *     theory.  These operations are fundamental to relational database
 *     theory, as well.
 *
 *     Sets are initialized with a user-defined size, and elements in
 *     the set may be accessed from 0 up to and including the given
 *     set size.  All sets passed into functions in this module are
 *     expected to have been initialized with set_init().
 *   Parameters:
 *    N/A
 *   External Variables:
 *    None
 *   Functions Called:
 *    set_type set_init( int32 n );
 *    int  set_empty( set_type set );
 *    void set_insert( int32 element, set_type set );
 *    void set_delete( int32 element, set_type set );
 *    int set_member( int32 element, set_type set );
 *    int32 set_min( set_type set );
 *    int32 set_max( set_type set );
 *    int  num_in_set( set_type set );
 *    void set_on( set_type set );
 *    void set_off( set_type set );
 *    int  set_equal( set_type a, set_type b );
 *    void set_assign( set_type *a, set_type b );
 *    set_type set_union( set_type a, set_type b );
 *    set_type set_intersection( set_type a, set_type b );
 *    set_type set_difference( set_type a, set_type b );
 *    void set_nuke( set_type *set );
 *************************************************************************/

#ifndef INCL_XVTH
#include "xvt.h"
#endif
#ifdef _MSDOS
#ifndef _WINDOWS
#include <alloc.h>
#include <mem.h>
#include <dos.h>
#include <malloc.h>
#endif
#else
#include <memory.h>
#endif

#include <float.h>
#include <stdlib.h>

#ifndef __SET_H__
#include "set.h"
#endif

#include <stdio.h>

#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif

static unsigned char checkmask[] = {254,253,251,247,239,223,191,127};
static unsigned char setmask[] = {1,2,4,8,16,32,64,128};

#define BITSET(bit,byte)  ((byte | checkmask[bit]) ^ checkmask[bit])

#define SET_BIT(bit,byte)  (byte | setmask[bit])

/* Warning: UNSET_BIT should only be called if the bit is not
   already set.  If it is already set, this macro may actually
   turn the bit on. */
#define UNSET_BIT(bit,byte)  (byte ^ setmask[bit])

/* The number of bytes in the set.  The byte buffer should
   only access bytes 0 through (NBYTES(set)-1).  */
#define NBYTES(set)  ((set.size>>3L) + 1L)

#ifndef max
#define max(a,b)   ( (a > b) ? a : b )
#endif

/* #define BOUNDSCHECK 1 */

#ifdef PROTO
   static char set_byte (int32 nbyte, set_type set)
#else
   static char set_byte (nbyte, set)
      int32 nbyte;
      set_type set;
#endif
   {
   if ( (nbyte < 0) || (nbyte >= NBYTES(set)) ) return 0;
   return set.buf[nbyte];
   }

#define SET_BYTE( nbyte, set, byte ) 				\
   if ( (nbyte < 0) || (nbyte >= NBYTES(set)) )                 \
      byte = 0;							\
   else								\
      byte = set.buf[nbyte];


/*************************************************************************
 *  set_off
 *   Purpose:
 *   Parameters:
 *    set <inout> == (set_type) set to be acted upon.
 *   External Variables:
 *   Functions Called:
 *    None
 *************************************************************************/
#ifdef PROTO
   void set_off (set_type set)
#else
   void set_off (set)
      set_type set;
#endif

   {
#if 0
   memset(set.buf,0,NBYTES(set));
#endif
   gmemset (set.buf, 0, NBYTES (set));
   }

/*************************************************************************
 *
 *N  set_on
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Turns each element in the set 'on'.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    set <inout> == (set_type) set to be acted upon.
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
*************************************************************************/
#ifdef PROTO
   void set_on (set_type set)
#else
   void set_on (set)
      set_type set;
#endif

{
   register int32 i;
   unsigned char byte=255;

   /* Turn on all bits up through set.size. */
   /* All but the last byte. */
#if 0
   memset(set.buf,byte,(set.size>>3L));
#endif
   gmemset (set.buf, byte, (set.size>>3L));

   /* The valid bits of the last byte. */
   for (i=(set.size>>3L)*8L;i<=set.size;i++)
      set_insert(i,set);
}



/*************************************************************************
 *
 *N  set_init
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Initialize the set for 'n' elements.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    n       <input> == (int32) maximum number of elements in the set.
 *    return <output> == (set_type) initialized set.
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *************************************************************************/
#ifdef PROTO
   set_type set_init (int32 n)
#else
   set_type set_init (n)
      int32 n;
#endif

{
   set_type s;
   int32 nbytes;

   s.size = n;
   nbytes = NBYTES(s);
#if 1			    /* Passe passe a thomas !!! LAS inc.*/
   s.buf = (char*)xvt_zmalloc (nbytes+1L);
   s.buf_handle =  s.buf;
#else

   s.buf_handle = galloc (nbytes+1L);
   s.buf = glock (s.buf_handle);
#endif

   if (s.buf == (char*)NULL)
      {
      xvt_error ("SET_INIT: Out of Memory!");
      }

   set_off(s);

   return s;
}

/*************************************************************************
 *
 *N  set_empty
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns TRUE if the given set is empty; else it
 *     returns FALSE.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    set     <input> == (set_type) set.
 *    return <output> == (int) TRUE[1] or FALSE[0].
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *************************************************************************/
#ifdef PROTO
   int set_empty (set_type set)
#else
   int set_empty (set)
      set_type set;
#endif
         
{
   register int32 i, nbytes;

   nbytes = NBYTES(set);
   for (i=0;i<nbytes;i++) {
      if (set_byte(i,set)) {
	 return FALSE;
      }
   }
   return TRUE;
}

/*************************************************************************
 *
 *  set_insert
 *   Purpose:
 *     This function inserts the given element into the specified set.
 *   Parameters:
 *    element <input> == (int32) element to insert into the set.
 *    set     <inout> == (set_type) set.
 *   External Variables:
 *    None
 *   Functions Called:
 *    None
 *************************************************************************/
#ifdef PROTO
   void set_insert (int32 element, set_type set)
#else
   void set_insert (element, set)
      int32 element;
      set_type set;
#endif

   {
   int32 nbyte,bit;
   unsigned char byte;

   if ((element < 0) || (element > set.size))
     {
     return;
     }
   nbyte = element >> 3L; /* element/8 */
   bit = element % 8L;
   SET_BYTE (nbyte, set, byte);
   byte = SET_BIT (bit, byte);
   set.buf[nbyte] = byte;
   }

/*************************************************************************
 *
 *N  set_delete
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function deletes the given element from the specified set.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    element <input> == (int32) element to delete from the set.
 *    set     <inout> == (set_type) set.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    void unset_bit()    BITSTUFF.C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
#ifdef PROTO
   void set_delete (int32 element, set_type set)
#else
   void set_delete (element, set)
      int32 element;
      set_type set;
#endif

{
   int32 nbyte,bit;
   unsigned char byte;

   if ((element<0)||(element>set.size)) {
#ifdef BOUNDSCHECK
      fprintf(stderr,"Invalid call to set_delete!\n");
      exit(1);
#endif
      return;
   }
   nbyte = element>>3L;  /* element/8 */
   bit = element%8L;
   SET_BYTE(nbyte,set,byte);
   if (!BITSET(bit,byte)) return;
   byte = UNSET_BIT(bit,byte);
   set.buf[nbyte] = byte;
}

/*************************************************************************
 *
 *N  set_member
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function determines whether a given element is a member of
 *     the specified set.  It returns either TRUE or FALSE.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    element <input> == (int32) element to check in the set.
 *    set     <input> == (set_type) set.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    void bitset()    BITSTUFF.C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
#ifdef PROTO
   int set_member (int32 element,	set_type set)
#else
   int set_member (element, set)
      int32 element;
      set_type set;
#endif

{
   int32 nbyte,bit;
   unsigned char byte;

   if ((element < 0)||(element > set.size)) return FALSE;
   nbyte = element>>3L;  /* element/8L */
   bit = element%8L;
   SET_BYTE(nbyte,set,byte);
   return BITSET(bit,byte);
}

/*************************************************************************
 *
 *N  set_min
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the minimum element in the given set.
 *     If the set is empty, the return value is MAXLONG.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    set     <input> == (set_type) set.
 *    return <output> == (int32) minimum element in the set.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    void bitset()    BITSTUFF.C
 *    int set_empty( set_type set )     SET.C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
#ifdef PROTO
   int32 set_min (set_type set)
#else
   int32 set_min (set)
      set_type set;
#endif

{
   register int32 nbyte, bit, element, nbytes;
   unsigned char byte = ' ';

   if (!set.size) return (int32) MAXLONG;

   /* Find the first byte with a bit set */
   nbytes = NBYTES(set);

   for (nbyte=0;nbyte<nbytes;nbyte++)
      if (set.buf[nbyte]) {
         byte = set.buf[nbyte];
	 break;
      }

   /* Now find the first bit set in the byte */
   element = nbyte*8L;
   for (bit=0; bit<8; bit++,element++) {
      if (element > set.size) return (int32) MAXLONG;
      if (BITSET(bit,byte)) return element;
   }
   return (int32) MAXLONG;
}

/*************************************************************************
 *
 *N  set_max
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the maximum element in the given set.
 *     If the set is empty, the return value is -MAXLONG.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    set     <input> == (set_type) set.
 *    return <output> == (int32) maximum element in the set.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *    Brian Glover     Nov  1992                  MDB upgrade
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
#ifdef PROTO
   int32 set_max (set_type set)
#else
   int32 set_max (set)
      set_type set;
#endif

{
   register int32 bytenum, bit, nbytes;
   unsigned char byte;

   if (!set.size) return (int32) -MAXLONG;

   /* Find the last byte with a bit set */
   nbytes = NBYTES(set);
   bytenum = nbytes;

   for (bytenum=bytenum-1;bytenum>=0;bytenum--) {
     if (set.buf[bytenum]) {
       byte = set.buf[bytenum];
       break;
     }
   }

   if (bytenum < 0) return (int32) -MAXLONG;

   for (bit=7;bit>=0;bit--) {
     if (BITSET(bit,byte)) {
       return ((bytenum*8L)+bit);
     }
   }

   return (int32) -MAXLONG;
}

/*************************************************************************
 *
 *N  num_in_set
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the number of elements in the given set.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    set     <input> == (set_type) set.
 *    return <output> == (int32) number of elements in the set.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    void bitset()    BITSTUFF.C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
#ifdef PROTO
   int32 num_in_set (set_type set)
#else
   int32 num_in_set (set)
      set_type set;
#endif

{
   register int32 nbyte,bit,n=0L, nbytes;
   unsigned char byte;

   if (set.size == 0) return n;
   nbytes = NBYTES(set);
   for (nbyte=0;nbyte<nbytes;nbyte++) {
      byte = set_byte(nbyte,set);
      if (byte) {
	 for (bit=0;bit<8;bit++)
	    if (BITSET(bit,byte)) n++;
      }
   }
   return n;
}


/*************************************************************************
 *
 *N  set_equal
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function determines whether two sets are equal to each other.
 *     It returns TRUE or FALSE.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    a       <input> == (set_type) first set to compare.
 *    b       <input> == (set_type) second set to compare.
 *    return <output> == (int) TRUE if (a==b) or FALSE if (a!=b).
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
#ifdef PROTO
   int  set_equal (set_type a, set_type b)
#else
   int set_equal (a, b)
      set_type a, b;
#endif

{

   if (a.size != b.size) return FALSE;
   if (memcmp(a.buf,b.buf,NBYTES(a))==0)
      return TRUE;
   else
      return FALSE;
}


/*************************************************************************
 *
 *N  set_assign
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function assigns set a to be equal to set b.  If a and b are
 *     different sizes, the function will reallocate a to match b.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    a       <input> == (set_type *) set to be assigned.
 *    b       <input> == (set_type) set to assign to a.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
#ifdef PROTO
   void set_assign (set_type *a, set_type b)
#else
   void set_assign (a, b)
      set_type *a, b;
#endif

{
   register int32 nbytes;

   nbytes = NBYTES(b);

   if (a->size == b.size) {
      memcpy(a->buf,b.buf,nbytes);
   } else {    /* a and b are different sizes */
      a->buf = (char *)xvt_realloc(a->buf,nbytes+1L);
      if (!a->buf) {
         fprintf(stderr,"Memory reallocation error in set_assign\n");
         exit(1);
      }
      memcpy(a->buf,b.buf,nbytes);
      a->size = b.size;
   }
}

/*************************************************************************
 *
 *N  set_union
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Return the set C such that C = (A U B).  C is initialized within
 *     this function, and should be nuked when no longer needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    a       <input> == (set_type) set to be unioned.
 *    b       <input> == (set_type) set to be unioned.
 *    return <output> == (set_type) (A U B).
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    set_type set_init()   SET.C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
#ifdef PROTO
   set_type set_union (set_type a, set_type b)
#else
   set_type set_union (a, b)
      set_type a, b;
#endif

{
   register int32 i, nbytes;
   set_type c;

   c = set_init( (int32)max(a.size,b.size) );

   nbytes = NBYTES(c);

   for (i=0;i<nbytes;i++)
      c.buf[i] = set_byte(i,a) | set_byte(i,b);

   return c;
}


/*************************************************************************
 *
 *N  set_intersection
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Return the set C such that C = (A o B).  C is initialized within
 *     this function, and should be nuked when no longer needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    a       <input> == (set_type) set to be intersectioned.
 *    b       <input> == (set_type) set to be intersectioned.
 *    return <output> == (set_type) (A o B).
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    set_type set_init()   SET.C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
#ifdef PROTO
   set_type set_intersection (set_type a,  set_type b)
#else
   set_type set_intersection (a, b)
      set_type a, b;
#endif

{
   register int32 i, nbytes;
   set_type c;

   c = set_init( (int32)max(a.size,b.size) );

   nbytes = NBYTES(c);
   for (i=0;i<nbytes;i++)
      c.buf[i] = set_byte(i,a) & set_byte(i,b);

   
   return c;
}

/*************************************************************************
 *
 *N  set_difference
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Return the set C such that C = (A - B).  C is initialized within
 *     this function, and should be nuked when no longer needed.
 *
 *     NOTE:  This function can be sped up, if necessary, by direct
 *     manipulation of the bytes and bits rather than the abstract
 *     set function calls used presently.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    a       <input> == (set_type) set to subtract from.
 *    b       <input> == (set_type) set to be subtracted.
 *    return <output> == (set_type) (A - B).
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    set_type set_init()   SET.C
 *    int set_member()      SET.C
 *    void set_insert()     SET.C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
#ifdef PROTO
   set_type set_difference (set_type a,  set_type b)
#else
   set_type set_deffenence (a, b)
      set_type a, b;
#endif

{
   register int32 i;
   set_type c;

   c = set_init( a.size );

   for (i=0;i<=a.size;i++) {
      if ( i > b.size ) {
	 if (set_member(i,a)) set_insert( i, c );
      } else {
	 if ((set_member(i,a)) && (!set_member(i,b))) set_insert(i,c);
      }
   }

   return c;
}


/*************************************************************************
 *
 *N  set_nuke
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Nucleate a set from existence.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    set     <inout> == (set_type *) set to be nuked.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   July 1990                  DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This function conforms to ANSI C standards.
 *E
 *************************************************************************/
#ifdef PROTO
   void set_nuke (set_type *set)
#else
   void set_nuke (set)
      set_type *set;
#endif

   {
   if (set->buf)
#if 0
       xvt_free (set->buf);
#endif
      {
//      gunlock (set->buf_handle);
      gfree (set->buf_handle);
      }
   set->size = -1;
   }

