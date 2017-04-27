
/*************************************************************************
 *
 *N  Module LINKLIST.C
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This module contains functions that make up a singly linked list
 *     data structure.  It is generic in the sense that it can hold any
 *     type of data, including user-defined types and structures.  That
 *     is why you must treat the data element as a void pointer and pass
 *     in its size when inserting into the list.  These routines are
 *     assured of working with "non-pointer" types of data elements.
 *     If you try storing other lists, or structures with pointers hanging
 *     off of them, the results will become unpredictable.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    N/A
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990   DOS Turbo C
 *    Dan Maddux       Jan. 1994   Windows, Sun & Mac Port
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
 *    linked_list_type ll_init();
 *    int32 ll_empty( linked_list_type list );
 *    position_type ll_first( linked_list_type list );
 *    position_type ll_last( linked_list_type list );
 *    position_type ll_next( position_type position );
 *    position_type ll_previous( position_type position, linked_list_type
 *                  list );
 *    int32 ll_end( position_type position );
 *    void ll_element( position_type position, void *element );
 *    void ll_insert( void *element, unsigned size, position_type position );
 *    void ll_delete( position_type position );
 *    void ll_reset( linked_list_type list );
 *    position_type ll_locate( void *element, linked_list_type list );
 *    void ll_replace( void *element, position_type position );
 *E
 *************************************************************************/
#ifndef INCL_XVTH
#include <xvt.h>
#endif
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#endif
#ifndef __LINKLIST_H__
#include "linklist.h"
#endif

/*************************************************************************
 *
 *N  ll_init
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function allocates and initializes a new linked list structure.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    ll_init <output> == (linked_list_type) initialized head of the list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
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
 *************************************************************************/
linked_list_type ll_init()
   {
   linked_list_type list;

   if ((list = (linked_list_type) xvt_malloc( sizeof(cell_type) ))==NULL)
      {
      xvt_note ("LL_INIT: Out of memory");
      exit(1);
      }
   list->element = NULL;
   list->element_size = 0;
   list->next = NULL;
   return list;
   }

/*************************************************************************
 *
 *N  ll_empty
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function TRUE if the list is empty and FALSE if it is not.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    list      <input> == (linked_list_type) linked list being checked.
 *    ll_empty <output> == (int32) boolean function result.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
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
 *************************************************************************/

#ifdef PROTO
int32 ll_empty( linked_list_type list )
#else
int32 ll_empty( list )
linked_list_type list;
#endif
{
   if (list == NULL) return TRUE;
   if (list->next == NULL)
      return TRUE;
   else
      return FALSE;
}


/*************************************************************************
 *
 *N  ll_first
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the head of the list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    list      <input> == (linked_list_type) linked list.
 *    ll_first <output> == (position_type) head of the list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
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
 *************************************************************************/
#ifdef PROTO
position_type ll_first( linked_list_type list )
#else
position_type ll_first( list )
linked_list_type list;
#endif
{
   return ((position_type) list);
}


/*************************************************************************
 *
 *N  ll_last
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns *THE* last position in the list, which is
 *     not useable.  Use ll_previous to get to the last USEABLE link in
 *     the list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    list     <input> == (linked_list_type) linked list.
 *    ll_last <output> == (position_type) tail of the list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
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
 *************************************************************************/
#ifdef PROTO
position_type ll_last( linked_list_type list )
#else
position_type ll_last( list )
linked_list_type list;
#endif
{
   position_type p;

   p = (position_type) list;
   while (p->next != NULL)
      p = p->next;
   return p;
}


/*************************************************************************
 *
 *N  ll_next
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the next position in the list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    position <input> == (position_type) current position in the list.
 *    ll_next <output> == (position_type) next position in the list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
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
 *************************************************************************/
#ifdef PROTO
position_type ll_next( position_type position )
#else
position_type ll_next( position )
position_type position;
#endif
{
   return(position->next);
}


/*************************************************************************
 *
 *N  ll_previous
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the previous position in the list.  Note:
 *     This is a singly linked list -> no backward pointer -> this
 *     operation is relatively inefficient.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    position     <input> == (position_type) current position.
 *    list         <input> == (linked_list_type) linked list.
 *    ll_previous <output> == (position_type) previous position in the list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
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
 *************************************************************************/
#ifdef PROTO
position_type ll_previous (position_type position, linked_list_type list)
#else
position_type ll_previous (position, list)
position_type position;
linked_list_type list;
#endif
{
   position_type p;

   if (position==list) return(position);
   p = list;
   while (p->next != position)
      p = p->next;
   return(p);
}


/*************************************************************************
 *
 *N  ll_end
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function determines if the given position is at the end of the
 *     list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    position <input> == (position_type) current position in the list.
 *    ll_end  <output> == (int32) TRUE  -- if position is the end of the list.
 *                              FALSE -- if not.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
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
 *************************************************************************/
#ifdef PROTO
int32 ll_end (position_type position)
#else
int32 ll_end (position)
position_type position;
#endif
{
   if (position == NULL)
      return(TRUE);
   else {
      if (position->next == NULL)
         return(TRUE);
      else
         return(FALSE);
   }
}


/*************************************************************************
 *
 *N  ll_element
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function copies the contents of the element at position
 *     currently being pointed to by "position" into the output data
 *     element.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    position <input> == (position_type) position in the list.
 *    element <output> == (void *) pointer to the element data.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
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
 *************************************************************************/
#ifdef PROTO
void ll_element (position_type position, void *element)
#else
void ll_element (position, element)
position_type position;
void *element;
#endif
   {
   memcpy (element, position->next->element, position->next->element_size);
   return;
   }


/*************************************************************************
 *
 *N  ll_element_size
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the size of the contents of the element at
 *     position currently being pointed to by "position".
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    position <input> == (position_type) position in the list.
 *    size <output>    == (size_t) size in byte of the current element.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dan Maddux            May 1993
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
 *************************************************************************/
#ifdef PROTO
size_t ll_element_size (position_type position)
#else
size_t ll_element_size (position)
position_type position;
#endif
   {
   return (position->next->element_size);
   }



/*************************************************************************
 *
 *N  ll_insert
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function inserts a new cell into the list at position that will
 *     contain the data pointed to by element.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    element  <input> == (void *) pointer to the data element to insert.
 *    size     <input> == (size_t) size of the data element.
 *    position <input> == (position_type) position to insert the new cell.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
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
 *************************************************************************/
#ifdef PROTO
void ll_insert (void *element, size_t size, position_type position)
#else
void ll_insert (element, size, position)
void *element;
size_t size;
position_type position;
#endif

   {
   position_type temp;

   if ((temp = (position_type)xvt_malloc (sizeof(cell_type))) == NULL)
      {
      xvt_note ("LL_INSERT: out of memory");
      abort();
      }
   temp->next = position->next;
   position->next = temp;
   temp->element_size = size;
   if ((temp->element = xvt_malloc (size)) == NULL)
      {
      xvt_note ("LL_INSERT: out of memory");
      abort();
      }
   memcpy (temp->element, element, size);
   return;
   }


/*************************************************************************
 *
 *N  ll_delete
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function deletes and disposes of a cell from the linked list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    position <input> == (position_type) position in the list to delete.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
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
 *************************************************************************/
#ifdef PROTO
void ll_delete( position_type position )
#else
void ll_delete( position )
position_type position;
#endif
{
   position_type    p;

   if (position != NULL) {  /* Cut the element out of the chain */
      p = position->next;
      position->next = p->next;
      xvt_free( p->element );
      xvt_free ((char*)p);
   }
}




/*************************************************************************
 *
 *N  ll_reset
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function empties out a linked list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    list <inout> == (linked_list_type) linked list to be emptied.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
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
 *    void ll_delete( position_type position );
 *    int32 ll_empty( linked_list_type list );
 *E
 *************************************************************************/
#ifdef PROTO
void ll_reset (linked_list_type list)
#else
void ll_reset ( list )
linked_list_type list;
#endif
   {
   while (! ll_empty(list))
      ll_delete(ll_first(list));
   xvt_free ((char*)list);
   list = NULL;
   }



/*************************************************************************
 *
 *N  ll_locate
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function locates a position in the list by comparing data.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    element <input> == (void *) pointer to the data element to locate.
 *    list    <input> == (linked_list_type) linked list.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
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
 *************************************************************************/
#ifdef PROTO
position_type ll_locate( void *element, linked_list_type list )
#else
position_type ll_locate( element, list )
void *element;
linked_list_type list;
#endif
{
   position_type p;

   p = list;
   while (p->next != NULL) {
      if ( memcmp(p->next->element,element,p->next->element_size) == 0 )
         return p;
      else
         p = p->next;
   }
   return NULL;
}


/*************************************************************************
 *
 *N  ll_replace
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function replaces an element in the linked list at the given
 *     position.  WARNING:  The new data element must be the same size as
 *     the previous data element or you will get some rather INTERESTING
 *     results.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    element  <input> == (void *) data element to replace existing data.
 *    position <input> == (position_type) position in the list to replace
 *                        the data.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Feb. 1990                      DOS Turbo C
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
 *************************************************************************/
#ifdef PROTO
void ll_replace (void *element, position_type position)
#else
void ll_replace (element, position)
void *element;
position_type position;
#endif
   {
   memcpy (position->next->element, element, position->next->element_size);
   }
