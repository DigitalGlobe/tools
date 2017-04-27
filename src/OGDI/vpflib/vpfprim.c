/***********************************************************************
 *
 *N  Module VPFPRIM  -  VPF Primitives
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This module contains functions for reading VPF primitives
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    N/A
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   April 1991                  DOS Turbo C
 *E
 **********************************************************************/
#ifndef INCL_XVTH
#include <xvt.h>
#endif

#ifdef _MSDOS
#include <io.h>
#include <dos.h>
#else
#include <float.h>
#endif

#ifndef _VPFTABLE_H_
#include "vpftable.h"
#endif
#ifndef __VPFPROJ_H__
#include "vpfproj.h"
#endif
#ifndef __COORGEOM_H__
#include "coorgeom.h"
#endif
#ifndef __VPFPRIM_H__
#include "vpfprim.h"
#endif
#ifndef H_MUSEDIR
#include "musedir.h"
#endif


/***********************************************************************
 *
 *N  create_edge_rec
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function creates an edge record internal structure from a
 *     row of a VPF edge table.   NOTE:  This function allocates memory
 *     for "edge_rec.coord". This array should be freed when no longer
 *     needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    row        <input> == (row_type) VPF table row.
 *    edge_table <input> == (vpf_table_type) opened VPF table to read.
 *    return    <output> == (edge_rec_type) returned edge record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 **********************************************************************/
#ifdef PROTO
   edge_rec_type create_edge_rec (row_type row, 
	                               vpf_table_type edge_table, int32 (*projfunc)())
#else
   edge_rec_type create_edge_rec (row, edge_table, projfunc)
      row_type row;
      vpf_table_type edge_table;
      int32 (*projfunc)();
#endif

   {
   edge_rec_type edge;
   int32 rowid, start, end, right_face, left_face, right_edge, left_edge;
   int32 coord, count, i;
   id_triplet_type key;
   coordinate_type *Ccoord;
   tri_coordinate_type *Zcoord;
   double_tri_coordinate_type *Ycoord;

   if (!row) {
     edge.id = -1;
     edge.npts = 0;
     return edge;
   }

   rowid = table_pos( "ID", edge_table );
   start = table_pos( "START_NODE", edge_table );
   end = table_pos( "END_NODE", edge_table );
   right_face = table_pos( "RIGHT_FACE", edge_table );
   left_face = table_pos( "LEFT_FACE", edge_table );
   right_edge = table_pos( "RIGHT_EDGE", edge_table );
   left_edge = table_pos( "LEFT_EDGE", edge_table );
   coord = table_pos( "COORDINATES", edge_table );

   get_table_element( rowid, row, edge_table, &(edge.id), &count );

   if (start >= 0)
      get_table_element( start, row, edge_table, &(edge.start_node), &count );
   else
      edge.start_node = 0;

   if (end >= 0)
      get_table_element( end, row, edge_table, &(edge.end_node), &count );
   else
      edge.end_node = 0;

   if (right_face >= 0) {
      if (edge_table.header[right_face].type=='K') {
	 get_table_element( right_face, row, edge_table, &key, &count );
	 edge.right_face = key.id;
      } else if (edge_table.header[right_face].type=='I') {
	 get_table_element( right_face, row, edge_table, &(edge.right_face),
			    &count );
      } else {
	 edge.right_face=1;
      }
   } else {
      edge.right_face = 1;
   }

   if (left_face >= 0) {
      if (edge_table.header[right_face].type=='K') {
	 get_table_element( left_face, row, edge_table, &key, &count );
	 edge.left_face = key.id;
      } else if (edge_table.header[left_face].type=='I') {
	 get_table_element( left_face, row, edge_table, &(edge.left_face),
			    &count );
      } else {
	 edge.left_face=1;
      }
   } else {
      edge.left_face = 1;
   }

   if (edge_table.header[right_edge].type=='K') {
      get_table_element( right_edge, row, edge_table, &key, &count );
      edge.right_edge = key.id;
   } else if (edge_table.header[right_edge].type=='I') {
      get_table_element( right_edge, row, edge_table, &(edge.right_edge),
			 &count );
   } else {
      edge.right_edge=0;
   }

   if (edge_table.header[left_edge].type=='K') {
      get_table_element( left_edge, row, edge_table, &key, &count );
      edge.left_edge = key.id;
   } else if (edge_table.header[left_edge].type=='I') {
      get_table_element( left_edge, row, edge_table, &(edge.left_edge),
			 &count );
   } else {
      edge.left_edge=0;
   }

   switch (edge_table.header[coord].type) {
      case 'C':
         Ccoord = (coordinate_type *)get_table_element( coord,
						      row, edge_table,
						      NULL, &count );
	 edge.coords = (double_coordinate_type*)xvt_malloc((size_t)count *
					  sizeof(double_coordinate_type));
	 if (edge.coords) {
	    for (i=0;i<count;i++) {
	       edge.coords[i].x = (double) Ccoord[i].x;
	       edge.coords[i].y = (double) Ccoord[i].y;
	    }
	 }
	 xvt_free((char*)Ccoord);
	 break;
      case 'Z':
	 Zcoord = (tri_coordinate_type *)get_table_element( coord,
						      row, edge_table,
						      NULL, &count );
	 edge.coords = (double_coordinate_type*)xvt_malloc((size_t)count *
					  sizeof(double_coordinate_type));
	 if (edge.coords) {
	    for (i=0;i<count;i++) {
	       edge.coords[i].x = (double) Zcoord[i].x;
	       edge.coords[i].y = (double) Zcoord[i].y;
	    }
	 }
	 xvt_free((char*)Zcoord);
	 break;
      case 'B':
	 edge.coords = (double_coordinate_type *)get_table_element( coord,
						      row, edge_table,
						      NULL, &count );
	 break;
      case 'Y':
	 Ycoord = (double_tri_coordinate_type *)get_table_element( coord,
						      row, edge_table,
						      NULL, &count );
	 edge.coords = (double_coordinate_type*)xvt_malloc((size_t)count *
					  sizeof(double_coordinate_type));
	 if (edge.coords) {
	    for (i=0;i<count;i++) {
	       edge.coords[i].x = Ycoord[i].x;
	       edge.coords[i].y = Ycoord[i].y;
	    }
	 }
	 xvt_free((char*)Ycoord);
	 break;
      default:
	 xvt_note ("Invalid coordinate type: %c\n",
		 edge_table.header[coord].type);
	 count = 0;
	 break;
   }
   edge.coord_type = edge_table.header[coord].type;
   edge.npts = count;

   edge.fp = NULL;
   if (!edge.coords) {
      edge.fp = edge_table.fp;
      edge.startpos = index_pos (edge.id,edge_table) +
		      row_offset ((int32)coord, row, edge_table) +
		      (int32)sizeof (int32);
      edge.pos = -1;
   }

   edge.current_coordinate = -1;

   edge.dir = ' ';

   if (projfunc != NULL) {
     for (i=0;i<count;i++) {
       projfunc( &(edge.coords[i].x), &(edge.coords[i].y) );
     }
   }

   return edge;
}


/***********************************************************************
 *
 *N  read_edge
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function reads a VPF edge record from the input VPF table.
 *     It performs a search for the specified line number, and, if found,
 *     allocates, reads, and returns the edge record.   NOTE:  This function
 *     allocates memory for "edge_rec.coord". This array should be freed
 *     when no longer needed.  If an invalid row id is passed in, this
 *     function will have unpredictable results.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    id         <input> == (int32) edge id number.
 *    edge_table <input> == (vpf_table_type) opened VPF table to read.
 *    return    <output> == (edge_rec_type) returned edge record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 **********************************************************************/
#ifdef PROTO
   edge_rec_type read_edge( int32  id, vpf_table_type edge_table,
			                  int32 (*projfunc)() )
#else
   edge_rec_type read_edge (id, edge_table, projfunc)
   int32 id;
   vpf_table_type edge_table;
   int32 (*projfunc)();
#endif

   {
   edge_rec_type edge;
   row_type row;

   row = get_row( id, edge_table );
   edge = create_edge_rec( row, edge_table, projfunc );
   free_row( row, edge_table );
   return edge;
}




/***********************************************************************
 *
 *N  read_next_edge
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function reads the next VPF edge record from the input VPF table.
 *     NOTE:  This function allocates memory for "edge_rec.coord".
 *     This array should be freed when no longer needed.
 *     Must have called vpf_open_table with DISK as the storage type,
 *     since this function accesses the disk to read the next row.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    edge_table  <input> == (vpf_table_type) opened VPF table to read.
 *    return     <output> == (edge_rec_type) returned edge record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 **********************************************************************/
#ifdef PROTO
   edge_rec_type read_next_edge (vpf_table_type edge_table, int32 (*projfunc)() )
#else
   edge_rec_type read_next_edge (edge_table, projfunc)
      vpf_table_type edge_table;
      int32 (*projfunc)();
#endif

   {
   edge_rec_type edge;
   row_type row;

   row = read_next_row( edge_table );
   edge = create_edge_rec( row, edge_table, projfunc );
   free_row( row, edge_table );

   return edge;
}

/***********************************************************************
 *
 *N  first_edge_coordinate
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the first coordinate of the given VPF edge
 *     record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *   edge_rec    <inout> == (edge_rec_type *) VPF edge record structure.
 *   return     <output> == (double_coordinate_type) first coordinate
 *                           in the edge record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 **********************************************************************/
#ifdef PROTO
   double_coordinate_type first_edge_coordinate (edge_rec_type *edge_rec)
#else
   double_coordinate_type first_edge_coordinate (edge_rec)
      edge_rec_type *edge_rec;
#endif

   {
   coordinate_type coord;
   tri_coordinate_type Zcoord;
   double_coordinate_type Bcoord;
   double_tri_coordinate_type Ycoord;
   int32 size;

   edge_rec->current_coordinate = 0;

   if (edge_rec->coords) {
      /* Coordinate array is in memory */
      return edge_rec->coords[0];
   }

   /* Read coordinate from table */
   fseek(edge_rec->fp,edge_rec->startpos,SEEK_SET);
   switch (edge_rec->coord_type) {
      case 'C':
	 ogdi_fread(&coord,sizeof(coord),1,edge_rec->fp);
	 Bcoord.x = (double) coord.x;
	 Bcoord.y = (double) coord.y;
	 size = sizeof(coord);
	 break;
      case 'Z':
	 ogdi_fread(&Zcoord,sizeof(Zcoord),1,edge_rec->fp);
	 Bcoord.x = (double) Zcoord.x;
	 Bcoord.y = (double) Zcoord.y;
	 size = sizeof(Zcoord);
	 break;
      case 'B':
	 ogdi_fread(&Bcoord,sizeof(Bcoord),1,edge_rec->fp);
	 size = sizeof(Bcoord);
	 break;
      case 'Y':
	 ogdi_fread(&Ycoord,sizeof(Ycoord),1,edge_rec->fp);
	 Bcoord.x = Ycoord.x;
	 Bcoord.y = Ycoord.y;
	 size = sizeof(Ycoord);
	 break;
      default:
	 Bcoord.x = (double) NULLINT;
	 Bcoord.y = (double) NULLINT;
	 size = 0;
	 break;
   }

   edge_rec->pos = edge_rec->startpos + size;

   return Bcoord;
}

/***********************************************************************
 *
 *N  next_edge_coordinate
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the next coordinate of the given VPF edge
 *     record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *   edge_rec    <inout> == (edge_rec_type *) VPF edge record structure.
 *   return     <output> == (double_coordinate_type) next coordinate
 *                           in the edge record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 **********************************************************************/
#ifdef PROTO
   double_coordinate_type next_edge_coordinate (edge_rec_type *edge_rec)
#else
   double_coordinate_type next_edge_coordinate (edge_rec)
      edge_rec_type *edge_rec;
#endif

   {
   coordinate_type coord;
   tri_coordinate_type Zcoord;
   double_coordinate_type Bcoord;
   double_tri_coordinate_type Ycoord;
   int32 size;

   if (edge_rec->current_coordinate < 0)
      return first_edge_coordinate(edge_rec);

   edge_rec->current_coordinate++;

   if (edge_rec->current_coordinate >= edge_rec->npts) {
      edge_rec->current_coordinate = edge_rec->npts-1L;
      if (!edge_rec->coords)
	 fseek(edge_rec->fp,edge_rec->startpos +
			    (edge_rec->npts-1L)*sizeof(coord),
	       SEEK_SET);
   }

   if (edge_rec->coords) {
      /* Coordinate array is in memory */
      return edge_rec->coords[edge_rec->current_coordinate];
   }

   /* Read coordinate from table */
   switch (edge_rec->coord_type) {
      case 'C':
	 ogdi_fread(&coord,sizeof(coord),1,edge_rec->fp);
	 Bcoord.x = (double) coord.x;
	 Bcoord.y = (double) coord.y;
	 size = sizeof(coord);
	 break;
      case 'Z':
	 ogdi_fread(&Zcoord,sizeof(Zcoord),1,edge_rec->fp);
	 Bcoord.x = (double) Zcoord.x;
	 Bcoord.y = (double) Zcoord.y;
	 size = sizeof(Zcoord);
	 break;
      case 'B':
	 ogdi_fread(&Bcoord,sizeof(Bcoord),1,edge_rec->fp);
	 size = sizeof(Bcoord);
	 break;
      case 'Y':
	 ogdi_fread(&Ycoord,sizeof(Ycoord),1,edge_rec->fp);
	 Bcoord.x = Ycoord.x;
	 Bcoord.y = Ycoord.y;
	 size = sizeof(Ycoord);
	 break;
      default:
	 Bcoord.x = (double) NULLINT;
	 Bcoord.y = (double) NULLINT;
	 size = 0;
	 break;
   }
   edge_rec->pos = edge_rec->startpos + size;

   return Bcoord;
}

/***********************************************************************
 *
 *N  get_edge_coordinate
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *  This function returns the specified coordinate of the given VPF edge
 *  record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *   edge_rec    <inout> == (edge_rec_type *) VPF edge record structure.
 *   n           <input> == (int32) coordinate array number.
 *   return     <output> == (double_coordinate_type) next coordinate
 *                          in the edge record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 **********************************************************************/
#ifdef PROTO
   double_coordinate_type get_edge_coordinate (int32 n, edge_rec_type *edge_rec)
#else
   double_coordinate_type get_edge_coordinate (n, edge_rec)
      int32 n;
      edge_rec_type *edge_rec;
#endif

   {
   coordinate_type coord;
   tri_coordinate_type Zcoord;
   double_coordinate_type Bcoord;
   double_tri_coordinate_type Ycoord;
   int32 size;

   if (n < 0)
      return first_edge_coordinate(edge_rec);

   if (n >= edge_rec->npts) n = edge_rec->npts-1L;

   edge_rec->current_coordinate = n;

   if (edge_rec->coords) {
      /* Coordinate array is in memory */
      return edge_rec->coords[n];
   }

   /* Read coordinate from table */
   switch (edge_rec->coord_type) {
      case 'C':
	 size = sizeof(coord);
	 break;
      case 'Z':
	 size = sizeof(Zcoord);
	 break;
      case 'B':
	 size = sizeof(Bcoord);
	 break;
      case 'Y':
	 size = sizeof(Ycoord);
	 break;
      default:
	 size = 0;
	 break;
   }
   edge_rec->pos = edge_rec->startpos + (n*size);
   fseek(edge_rec->fp,edge_rec->pos,SEEK_SET);
   switch (edge_rec->coord_type) {
      case 'C':
	 ogdi_fread(&coord,sizeof(coord),1,edge_rec->fp);
	 Bcoord.x = (double) coord.x;
	 Bcoord.y = (double) coord.y;
	 break;
      case 'Z':
	 ogdi_fread(&Zcoord,sizeof(Zcoord),1,edge_rec->fp);
	 Bcoord.x = (double) Zcoord.x;
	 Bcoord.y = (double) Zcoord.y;
	 break;
      case 'B':
	 ogdi_fread(&Bcoord,sizeof(Bcoord),1,edge_rec->fp);
	 break;
      case 'Y':
	 ogdi_fread(&Ycoord,sizeof(Ycoord),1,edge_rec->fp);
	 Bcoord.x = Ycoord.x;
	 Bcoord.y = Ycoord.y;
	 break;
      default:
	 Bcoord.x = (double)NULLINT;
	 Bcoord.y = (double)NULLINT;
	 break;
   }

   return Bcoord;
}




/***********************************************************************
 *
 *N  read_face
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This function reads a VPF face record from the input VPF table.
 *    It performs a search for the specified face number, and, if found,
 *    reads and returns the face record.  If id is out of range,
 *    either the lowest or highest id numbered face record type will be
 *    returned.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    id           <input> == (int32) face id number.
 *    face_table   <input> == (vpf_table_type) opened VPF table to read.
 *    return      <output> == (face_rec_type) returned face record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 **********************************************************************/
#ifdef PROTO
   face_rec_type read_face (int32 id, vpf_table_type face_table)
#else
   face_rec_type read_face (id, face_table)
      int32 id;
      vpf_table_type face_table;
#endif

   {
   face_rec_type face;
   int32 count, rowid, ring;
   row_type row;

   rowid = table_pos( "ID", face_table );
   ring = table_pos( "RING_PTR", face_table );

   row = get_row( id, face_table );

   get_table_element( rowid, row, face_table, &(face.id), &count );

   get_table_element( ring, row, face_table, &(face.ring), &count );

   free_row( row, face_table );

   return face;
}





/***********************************************************************
 *
 *N  read_next_face
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 
 *   Purpose:
 *P
 *  This function reads the next VPF face record from the input VPF
 *  table.  Must have used vpf_open_table with DISK as the storage type.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    face_table   <input> == (vpf_table_type) opened VPF table to read.
 *    return      <output> == (face_rec_type) returned face record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 **********************************************************************/
#ifdef PROTO
   face_rec_type read_next_face( vpf_table_type face_table )
#else
   face_rec_type read_next_face (face_table)
      vpf_table_type face_table;
#endif

   {
   face_rec_type face;
   int32 count, rowid, ring;
   row_type row;

   rowid = table_pos( "ID", face_table );
   ring = table_pos( "RING_PTR", face_table );

   row = read_next_row( face_table );

   get_table_element( rowid, row, face_table, &(face.id), &count );

   get_table_element( ring, row, face_table, &(face.ring), &count );

   free_row( row, face_table );

   return face;
}





/***********************************************************************
 *
 *N  read_ring
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function reads a VPF ring record from the input VPF table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    id           <input> == (int32) ring id number.
 *    ring_table   <input> == (vpf_table_type) opened VPF table to read.
 *    return      <output> == (ring_rec_type) returned ring record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 **********************************************************************/
#ifdef PROTO
   ring_rec_type read_ring (int32 id, vpf_table_type ring_table)
#else
   ring_rec_type read_ring (id, ring_table)
      int32 id;
      vpf_table_type ring_table;
#endif

   {
   ring_rec_type ring;
   int32 count, rowid, face, edge;
   row_type row;

   rowid = table_pos( "ID", ring_table );
   face = table_pos( "FACE_ID", ring_table );
   edge = table_pos( "START_EDGE", ring_table );

   row = get_row( id, ring_table );

   get_table_element( rowid, row, ring_table, &(ring.id), &count );

   get_table_element( face, row, ring_table, &(ring.face), &count );

   get_table_element( edge, row, ring_table, &(ring.edge), &count );

   free_row( row, ring_table );

   return ring;
}




/***********************************************************************
 *
 *N  read_next_ring
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *  This function reads the next VPF ring record from the input VPF table.
 *  If read_next_ring goes past the end of the vpftable, ring_rec_type
 *  id will be a garbage number.  The programmer must ensure that this
 *  does NOT happen.  Must have called vpf_open_table with DISK as the
 *  storage type.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    ring_table   <input> == (vpf_table_type) opened VPF table to read.
 *    return      <output> == (ring_rec_type) returned ring record.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 **********************************************************************/
#ifdef PROTO
   ring_rec_type read_next_ring (vpf_table_type ring_table )
#else
   ring_rec_type read_next_ring (ring_table)
      vpf_table_type ring_table;
#endif

   {
   ring_rec_type ring;
   int32 count, rowid, face, edge;
   row_type row;

   rowid = table_pos( "ID", ring_table );
   face = table_pos( "FACE_ID", ring_table );
   edge = table_pos( "START_EDGE", ring_table );

   row = read_next_row( ring_table );

   get_table_element( rowid, row, ring_table, &(ring.id), &count );

   get_table_element( face, row, ring_table, &(ring.face), &count );

   get_table_element( edge, row, ring_table, &(ring.edge), &count );

   free_row( row, ring_table );

   return ring;
}



/***********************************************************************
 *
 *N  read_node
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *   This function reads a VPF node record from the input VPF table.
 *   It performs a search for the specified node number, and, if found,
 *   reads and returns the node record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    id            <input> == (int32) node id number.
 *    node_table   <input> == (vpf_table_type) opened VPF table to read.
 *    return       <output> == (node_rec_type) returned node record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 **********************************************************************/
#ifdef PROTO
   node_rec_type read_node (int32 id, vpf_table_type node_table, int32 (*projfunc)() )
#else
   node_rec_type read_node (id, node_table,  projfunc)
      int32 id;
      vpf_table_type node_table;
      int32 (*projfunc)();
#endif

					      
   {
   node_rec_type node;
   int32 count, rowid, face, first_edge, coord;
   coordinate_type c;
   tri_coordinate_type Zcoord;
   double_coordinate_type Bcoord;
   double_tri_coordinate_type Ycoord;
   row_type row;

   rowid = table_pos( "ID", node_table );
   face = table_pos( "CONTAINING_FACE", node_table );
   first_edge = table_pos( "FIRST_EDGE", node_table );
   coord = table_pos( "COORDINATE", node_table );

   row = get_row( id, node_table );

   get_table_element( rowid, row, node_table, &(node.id), &count );

   if (face>0)
      get_table_element( face, row, node_table, &(node.face), &count );
   else
      node.face = 0;

   if (first_edge>0)
      get_table_element( first_edge, row, node_table, &(node.first_edge),
                         &count );
   else
      node.first_edge = 0;

   switch (node_table.header[coord].type) {
      case 'C':
	 get_table_element( coord, row, node_table, &c, &count );
	 node.x = (double) c.x;
	 node.y = (double) c.y;
	 break;
      case 'Z':
	 get_table_element( coord, row, node_table, &Zcoord, &count );
	 node.x = (double) Zcoord.x;
	 node.y = (double) Zcoord.y;
	 break;
      case 'B':
	 get_table_element( coord, row, node_table, &Bcoord, &count );
	 node.x = Bcoord.x;
	 node.y = Bcoord.y;
	 break;
      case 'Y':
	 get_table_element( coord, row, node_table, &Ycoord, &count );
	 node.x = Ycoord.x;
	 node.y = Ycoord.y;
	 break;
      default:
	 node.x = (double)NULLINT;
	 node.y = (double)NULLINT;
	 break;
   }

   free_row( row, node_table );

   if (projfunc != NULL) {
     projfunc( &(node.x), &(node.y) );
   }

   return node;
}




/***********************************************************************
 *
 *N  read_next_node
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function reads the next VPF node record from the input
 *     VPF table.  Must have called vpf_open_table with DISK as the
 *     storage type.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    node_table   <input> == (vpf_table_type) opened VPF table to read.
 *    return       <output> == (node_rec_type) returned node record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 *********************************************************************/
#ifdef PROTO
   node_rec_type read_next_node (vpf_table_type node_table, int32 (*projfunc)())
#else
   node_rec_type read_next_node (node_table, projfunc)
      vpf_table_type node_table;
      int32 (*projfunc)();
#endif

   {
   node_rec_type node;
   int32 count, rowid, face, first_edge, coord;
   coordinate_type c;
   tri_coordinate_type Zcoord;
   double_coordinate_type Bcoord;
   double_tri_coordinate_type Ycoord;
   row_type row;

   rowid = table_pos( "ID", node_table );
   face = table_pos( "CONTAINING_FACE", node_table );
   first_edge = table_pos( "FIRST_EDGE", node_table );
   coord = table_pos( "COORDINATE", node_table );

   row = read_next_row( node_table );

   get_table_element( rowid, row, node_table, &(node.id), &count );

   if (face>0)
      get_table_element( face, row, node_table, &(node.face), &count );
   else
      node.face = 0;

   if (first_edge>0)
      get_table_element( first_edge, row, node_table, &(node.first_edge),
                         &count );
   else
      node.first_edge = 0;

   switch (node_table.header[coord].type) {
      case 'C':
	 get_table_element( coord, row, node_table, &c, &count );
	 node.x = (double) c.x;
	 node.y = (double) c.y;
	 break;
      case 'Z':
	 get_table_element( coord, row, node_table, &Zcoord, &count );
	 node.x = (double) Zcoord.x;
	 node.y = (double) Zcoord.y;
	 break;
      case 'B':
	 get_table_element( coord, row, node_table, &Bcoord, &count );
	 node.x = Bcoord.x;
	 node.y = Bcoord.y;
	 break;
      case 'Y':
	 get_table_element( coord, row, node_table, &Ycoord, &count );
	 node.x = Ycoord.x;
	 node.y = Ycoord.y;
	 break;
      default:
	 node.x = (double)NULLINT;
	 node.y = (double)NULLINT;
	 break;
   }

   free_row( row, node_table );

   if (projfunc != NULL) {
     projfunc( &(node.x), &(node.y) );
   }

   return node;
}






/***********************************************************************
 *
 *N  read_text
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 * This function reads a VPF annotation record from the input VPF table.
 * It performs a search for the specified annotation id, and, if
 * found, reads and returns the annotation record.  NOTE: This function
 * allocates memory for "txt.text".  This must be freed when no
 * longer needed.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *   id           <input> == (int32) annotation id.
 *   text_table   <input> == (vpf_table_type) opened VPF table to read.
 *   return      <output> == (text_rec_type) returned annotation record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 **********************************************************************/
#ifdef PROTO
   text_rec_type read_text (int32 id, vpf_table_type text_table, int32 (*projfunc)())
#else
   text_rec_type read_text (id, text_table, projfunc)
      int32 id;
      vpf_table_type text_table;
      int32 (*projfunc)();
#endif

   {
   text_rec_type txt;
   int32 count, rowid, text, coord;
   coordinate_type *c;
   tri_coordinate_type *Zcoord;
   double_coordinate_type *Bcoord;
   double_tri_coordinate_type *Ycoord;
   row_type row;

   rowid = table_pos( "ID", text_table );
   text = table_pos( "STRING", text_table );
   coord = table_pos( "SHAPE_LINE", text_table );

   row = get_row( id, text_table );

   get_table_element( rowid, row, text_table, &(txt.id), &count );

   txt.text = get_table_element( text, row, text_table, NULL, &count );

   switch (text_table.header[coord].type) {
      case 'C':
	 c = (coordinate_type *)get_table_element( coord, row,
					  text_table, NULL, &count );
	 txt.x = (double) c[0].x;
	 txt.y = (double) c[0].y;
	 xvt_free((char*)c);
	 break;
      case 'Z':
	 Zcoord = (tri_coordinate_type *)get_table_element( coord, row,
					       text_table, NULL, &count );
	 txt.x = (double) Zcoord[0].x;
	 txt.y = (double) Zcoord[0].y;
	 xvt_free((char*)Zcoord);
	 break;
      case 'B':
	 Bcoord = (double_coordinate_type *)get_table_element( coord, row,
					       text_table, NULL, &count );
	 txt.x = Bcoord[0].x;
	 txt.y = Bcoord[0].y;
	 xvt_free((char*)Bcoord);
	 break;
      case 'Y':
	 Ycoord = (double_tri_coordinate_type *)get_table_element( coord,
					 row, text_table, NULL, &count );
	 txt.x = Ycoord[0].x;
	 txt.y = Ycoord[0].y;
	 xvt_free ((char*)Ycoord);
	 break;
      default:
	 txt.x = (double)NULLINT;
	 txt.y = (double)NULLINT;
	 break;
   }

   free_row( row, text_table );

   if (projfunc != NULL) {
     projfunc( &(txt.x), &(txt.y) );
   }

   return txt;
}




/***********************************************************************
 *
 *N  read_next_text
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *  This function reads the next VPF annotation record from the input
 *  VPF table.  Must have called vpf_open_table with DISK as the storage
 *  type.  NOTE: This function allocates memory for "txt.text".  This
 *  must be freed when no longer needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *   text_table   <input> == (vpf_table_type) opened VPF table to read.
 *   return      <output> == (text_rec_type) returned annotation record.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1991                      DOS Turbo C
 *E
 **********************************************************************/
#ifdef PROTO
   text_rec_type read_next_text( vpf_table_type text_table, int32 (*projfunc)())
#else
   text_rec_type read_next_text (text_table, projfunc)
      vpf_table_type text_table;
      int32 (*projfunc)();
#endif

   {
   text_rec_type txt;
   int32 count, rowid, text, coord;
   coordinate_type *c;
   tri_coordinate_type *Zcoord;
   double_coordinate_type *Bcoord;
   double_tri_coordinate_type *Ycoord;
   row_type row;

   rowid = table_pos( "ID", text_table );
   text = table_pos( "STRING", text_table );
   coord = table_pos( "SHAPE_LINE", text_table );

   row = read_next_row( text_table );

   get_table_element( rowid, row, text_table, &(txt.id), &count );

   txt.text = get_table_element( text, row, text_table, NULL, &count );

   switch (text_table.header[coord].type) {
      case 'C':
	 c = (coordinate_type *)get_table_element( coord, row,
					  text_table, NULL, &count );
	 txt.x = (double) c[0].x;
	 txt.y = (double) c[0].y;
	 xvt_free ((char*)c);
	 break;
      case 'Z':
	 Zcoord = (tri_coordinate_type *)get_table_element( coord, row,
					       text_table, NULL, &count );
	 txt.x = (double) Zcoord[0].x;
	 txt.y = (double) Zcoord[0].y;
	 xvt_free ((char*)Zcoord);
	 break;
      case 'B':
	 Bcoord = (double_coordinate_type *)get_table_element( coord, row,
					       text_table, NULL, &count );
	 txt.x = Bcoord[0].x;
	 txt.y = Bcoord[0].y;
	 xvt_free ((char*)Bcoord);
	 break;
      case 'Y':
	 Ycoord = (double_tri_coordinate_type *)get_table_element( coord,
					 row, text_table, NULL, &count );
	 txt.x = Ycoord[0].x;
	 txt.y = Ycoord[0].y;
	 xvt_free ((char*)Ycoord);
	 break;
      default:
	 txt.x = (double)NULLINT;
	 txt.y = (double)NULLINT;
	 break;
   }

   free_row( row, text_table );

   if (projfunc != NULL) {
     projfunc( &(txt.x), &(txt.y) );
   }

   return txt;
}


/***********************************************************************
 *
 *N  bounding_select
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This function reads the bounding rectangle table to weed out the
 *    local primitives.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 * brpath      <input> == (char *) path to the bounding rectangle table.
 * mapextent   <input> == (extent_type) map extent to compare.
 * dec_degrees <input> == (int32) flag to indicate if data is in decimal
 *                                 degrees.
 * projfunc    <input> == (int32 *)() inverse projection function.
 * bounding_select <output> == (set_type) set of bounding rectangle ids.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 **********************************************************************/
#ifdef PROTO
   set_type bounding_select( char *brpath, extent_type extent,
	                           vpf_projection_type invproj )
#else
   set_type bounding_select (brpath, extent, invproj)
      char *brpath;
      extent_type extent;
      vpf_projection_type invproj;
#endif

   {
   vpf_table_type table;
   row_type row;
   set_type set;
   int32 count, id;
   int32 XMIN_, YMIN_, XMAX_, YMAX_;
   extent_type box, pextent;
   float fx1,fy1,fx2,fy2;
   double x1,y1,x2,y2;
   vpf_projection_type pcproj, prevproj;

   prevproj = get_vpf_forward_projection();

   /* Project all extents to plate-carree for cartesian comparisons */
   /* (decimal degree coordinate systems) */

   x1 = extent.x1; y1 = extent.y1;
   x2 = extent.x2; y2 = extent.y2;
   pcproj = set_vpf_projection_parms(PC,extent);
   set_vpf_forward_projection(pcproj);
   plate_carree_fwd(&x1,&y1);
   plate_carree_fwd(&x2,&y2);
   pextent.x1 = x1; pextent.y1 = y1;
   pextent.x2 = x2; pextent.y2 = y2;

   table = vpf_open_table(brpath,disk,"rb",NULL);
   XMIN_ = table_pos("XMIN",table);
   YMIN_ = table_pos("YMIN",table);
   XMAX_ = table_pos("XMAX",table);
   YMAX_ = table_pos("YMAX",table);
   set = set_init(table.nrows+1);
   for (id=1;id<=table.nrows;id++) {
      row = read_next_row( table );
      get_table_element(XMIN_,row,table,&fx1,&count);
      get_table_element(YMIN_,row,table,&fy1,&count);
      get_table_element(XMAX_,row,table,&fx2,&count);
      get_table_element(YMAX_,row,table,&fy2,&count);
      free_row(row,table);
      box.x1 = (double)fx1;
      box.y1 = (double)fy1;
      box.x2 = (double)fx2;
      box.y2 = (double)fy2;

      if (invproj.inverse_proj != NULL) {
        set_vpf_inverse_projection(invproj);
	invproj.inverse_proj( &(box.x1), &(box.y1) );
	invproj.inverse_proj( &(box.x2), &(box.y2) );
      }

      x1 = box.x1; y1 = box.y1;
      x2 = box.x2; y2 = box.y2;
      set_vpf_forward_projection(pcproj);
      plate_carree_fwd(&x1,&y1);
      plate_carree_fwd(&x2,&y2);
      box.x1 = x1; box.y1 = y1;
      box.x2 = x2; box.y2 = y2;

      if ( contained(box,pextent) || contained(pextent,box) ) {
	 set_insert(id,set);
      }
   }
   vpf_close_table(&table);

   set_vpf_forward_projection(prevproj);

   return set;
}



/***********************************************************************
 *
 *N  open_bounding_rect
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This function opens the bounding rectangle table for the specified
 *    primitmive class.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    covpath  <input> == (char *) path to coverage directory.
 *    tiledir  <input> == (char *) directory name of tile directory.
 *    pclass   <input> == (int32) primitive class for bounding rectangle.
 *    open_bounding_rect <output> == (vpf_table_type) bounding rectangle table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Sept 1991                         DOS Turbo C
 *E
 **********************************************************************/
#ifdef PROTO
   vpf_table_type open_bounding_rect (char *covpath, char *tiledir, int32 pclass)
#else
   vpf_table_type open_bounding_rect (covpath, tiledir, pclass)
      char *covpath;
      char *tiledir;
      int32 pclass;
#endif

   {
   vpf_table_type brtable;
   char path[256];
   static char *brname[] = {"","ebr","fbr","tbr","nbr","cbr"};

   strcpy( path, covpath );
   strcat (path, tiledir);
   strcat (path, brname[pclass]);

   if (muse_access(path,0)==0) {
      brtable = vpf_open_table( path, disk, "rb", NULL );
   } else {
      brtable.fp = NULL;
      brtable.status = CLOSED;
   }
   return brtable;
}


/***********************************************************************
 *
 *N  read_bounding_rect
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *  Reads the specified extent from the opened bounding rectangle table.
 *  If the given projection function is not NULL, the bounding
 *  rectangle coordinates are projected through the specified function
 *  before being returned.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    row      <input> == (int32) table row.
 *    brtable  <input> == (vpf_table_type) bounding rectangle table.
 *    projfunc <input> == (int32 *)() inverse projection function.
 *    read_bounding_rect  <output> == (extent_type) spatial extent.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Sept 1991                         DOS Turbo C
 *E
 **********************************************************************/
#ifdef PROTO
   extent_type read_bounding_rect (int32 row, vpf_table_type brtable,
						     int32 (*projfunc)() )
#else
   extent_type read_bounding_rect (row, brtable, projfunc)
      int32 row;
      vpf_table_type brtable;
      int32 (*projfunc)();
#endif

   {
   int32 count;
   extent_type extent;
   float x1,y1,x2,y2;
   row_type tablerow;
   int32 XMIN_,YMIN_,XMAX_,YMAX_;

   XMIN_ = table_pos("XMIN",brtable);
   YMIN_ = table_pos("YMIN",brtable);
   XMAX_ = table_pos("XMAX",brtable);
   YMAX_ = table_pos("YMAX",brtable);

   tablerow = read_row(row,brtable);
   get_table_element(XMIN_,tablerow,brtable,&x1,&count);
   get_table_element(YMIN_,tablerow,brtable,&y1,&count);
   get_table_element(XMAX_,tablerow,brtable,&x2,&count);
   get_table_element(YMAX_,tablerow,brtable,&y2,&count);
   free_row(tablerow,brtable);

   extent.x1 = (double) x1;
   extent.y1 = (double) y1;
   extent.x2 = (double) x2;
   extent.y2 = (double) y2;

   if (projfunc != NULL) {
     projfunc( &(extent.x1), &(extent.y1) );
     projfunc( &(extent.x2), &(extent.y2) );
   }

   return extent;
}


/***********************************************************************
 *
 *N  read_next_bounding_rect
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Reads the next extent from the opened bounding rectangle table.
 *    If the given projection function is not NULL, the bounding
 *    rectangle coordinates are projected through the specified function
 *    before being returned.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    row      <input> == (int32) table row.
 *    brtable  <input> == (vpf_table_type) bounding rectangle table.
 *    projfunc <input> == (int32 *)() inverse projection function.
 *    read_bounding_rect  <output> == (extent_type) spatial extent.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Sept 1991                         DOS Turbo C
 *E
 **********************************************************************/
#ifdef PROTO
   extent_type read_next_bounding_rect( vpf_table_type brtable,
					int32 (*projfunc)() )
#else
   extent_type read_next_bounding_rect (brtable, projfunc)
      vpf_table_type brtable;
      int32 (*projfunc)();
#endif

   {
   int32 count;
   extent_type extent;
   float x1,y1,x2,y2;
   row_type tablerow;
   int32 XMIN_,YMIN_,XMAX_,YMAX_;

   XMIN_ = table_pos("XMIN",brtable);
   YMIN_ = table_pos("YMIN",brtable);
   XMAX_ = table_pos("XMAX",brtable);
   YMAX_ = table_pos("YMAX",brtable);

   tablerow = read_next_row(brtable);
   get_table_element(XMIN_,tablerow,brtable,&x1,&count);
   get_table_element(YMIN_,tablerow,brtable,&y1,&count);
   get_table_element(XMAX_,tablerow,brtable,&x2,&count);
   get_table_element(YMAX_,tablerow,brtable,&y2,&count);
   free_row(tablerow,brtable);

   extent.x1 = (double) x1;
   extent.y1 = (double) y1;
   extent.x2 = (double) x2;
   extent.y2 = (double) y2;

   if (projfunc != NULL) {
     projfunc( &(extent.x1), &(extent.y1) );
     projfunc( &(extent.x2), &(extent.y2) );
   }

   return extent;
}
