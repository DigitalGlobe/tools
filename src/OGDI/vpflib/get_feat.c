/**********************************************************************/
/* GET_FEATURE.C                                                      */
/**********************************************************************/

#ifndef INCL_XVTH
#include <xvt.h>
#endif
#ifndef __COORGEOM_H__
#include "coorgeom.h"
#endif
#ifndef _VPFTABLE_H_
#include "vpftable.h"
#endif
#ifndef __VPFPRIM_H__
#include "vpfprim.h"
#endif
#ifndef GET_FEATURE_H
#include "get_feat.h"
#endif

/* Global Variables */
/*extern vpf_projection_type NOPROJ; */
vpf_projection_type NOPROJ = {DDS, 0.0, 0.0, 0.0, 0.0, 0, 0.0, 0.0,
                              NULL, NULL, "Decimal Degrees     "};




/**********************************************************************/
/* GET_LINE_FEATURE                                                   */
/**********************************************************************/
#ifdef PROTO
   void get_line_feature (LINE_FEATURE *line, row_type row, vpf_table_type table)
#else
   void get_line_feature (line, row, table)
      LINE_FEATURE *line;
      row_type row;
      vpf_table_type table;
#endif

   {
   int32 pos, id, count;

   /* Go ahead and set up code to handle compound line features */
   line->nr_segs = 1;
   line->segs = (SEGMENT**)xvt_zmalloc ((size_t)line->nr_segs * sizeof (SEGMENT*));
   if (line->segs == NULL)
      xvt_fatal ("GET_LINE_FEATURE:1 Out of memory!");
   
   line->segs[0] = (SEGMENT*)xvt_zmalloc (sizeof (SEGMENT));
   if (line->segs[0] == NULL)
      xvt_fatal ("GET_LINE_FEATURE:2 Out of memory!");      
   
   pos = table_pos ("ID", table);
   get_table_element (pos, row, table, &id, &count);
   line->segs[0]->id = id;

   pos = table_pos ("COORDINATES", table);
   line->segs[0]->coords = (COORDINATE*)get_xy (table, row, pos, &count);
   line->segs[0]->nr_coords = count;   

   return;
   }

/**********************************************************************/
/* GET_TEXT_FEATURE                                                   */
/**********************************************************************/
#ifdef PROTO
   void get_text_feature (TEXT_FEATURE *text, row_type row, vpf_table_type table)
#else
   void get_text_feature (text, row, table)
      TEXT_FEATURE *text;
      row_type row;
      vpf_table_type table;
#endif

   {
   int32 pos, id, count;

   pos = table_pos ("ID", table);
   get_table_element (pos, row, table, &id, &count);
   text->id = id;

   pos = table_pos ("STRING", table);
   text->string = (char*)get_table_element (pos, row, table, NULL, &count);

   pos = table_pos ("SHAPE_LINE", table);
   text->coords = (COORDINATE*)get_xy (table, row, pos, &count);
   text->nr_coords = count;

   return;
   }

/**********************************************************************/
/*  GET_POINT_FEATURE                        */
/*     Derived from draw_point_row  [vpfdraw.c]                       */
/**********************************************************************/
#ifdef PROTO
   void get_point_feature
            (POINT_FEATURE *point, row_type row, vpf_table_type table)
#else
   void get_point_feature (point, row, table)
      POINT_FEATURE *point;
      row_type row;
      vpf_table_type table;
#endif

   {
   int32 pos, id, count;

   pos = table_pos ("ID", table);
   get_table_element (pos, row, table, &id, &count);
   point->id = id;

   pos = table_pos ("COORDINATE", table);
   point->coord = (COORDINATE*)get_xy (table, row, pos, &count);

   return;
   }


/**********************************************************************/
/*  GET_AREA_FEATURE                                                  */
/*    Derived from outline_face  [vpfdraw.c]                          */
/**********************************************************************/
#ifdef PROTO
   void get_area_feature (AREA_FEATURE *area, int32 face_id,
                       vpf_table_type facetable,
                       vpf_table_type ringtable,
                       vpf_table_type edgetable)
#else
   void get_area_feature (area, face_id, facetable, ringtable, edgetable)
      AREA_FEATURE *area;
      int32 face_id;
      vpf_table_type facetable, ringtable, edgetable;
#endif

   {                                    
   int32 n=0;
   face_rec_type face_rec;
   ring_rec_type ring_rec;
   RING **temp;

   face_rec = read_face (face_id, facetable);
   ring_rec = read_ring (face_rec.ring, ringtable);

   /* Allocate space to store addresses of all the ring structures */
   area->rings = (RING**)xvt_zmalloc (MAXRINGS * sizeof (RING*));
   if (area->rings == NULL)
      xvt_fatal ("GET_AREA_FEATURE: Out of memory!");
         
   /* Get the outer ring coords */
   area->rings[n] = (RING*)xvt_zmalloc (sizeof (RING));
   if (area->rings[n] == NULL)
      xvt_fatal ("GET_AREA_FEATURE: Out of memory!");
         
   area->rings[n]->id = n+1;

   get_ring_coords (area->rings[n], face_id, ring_rec.edge, edgetable);
   n++;

   /* Get the coords for any inner rings that exist */
   while (ring_rec.face == face_id)
      {
      ring_rec = read_next_ring (ringtable);
      
      if (feof (ringtable.fp))
         break;
      if (ring_rec.face == face_id)
         {
         area->rings[n] = (RING*)xvt_zmalloc (sizeof (RING));
         if (area->rings[n] == NULL)
            xvt_fatal ("GET_AREA_FEATURE: Out of memory!");
               
         area->rings[n]->id = n+1;

         get_ring_coords (area->rings[n], face_id, ring_rec.edge, edgetable);
         n++;                        
         }
      }
   area->nr_rings = n;

   /* Realloc rings array to the required size */
   temp = (RING**)xvt_zmalloc (area->nr_rings * sizeof (RING*));
   if (temp == (RING**)NULL)
      xvt_fatal ("GET_AREA_FEATURE: Out of memory!");
   else
      {
      memcpy (temp, area->rings, (area->nr_rings * sizeof (RING*)));
      xvt_free ((char*)area->rings);
      area->rings = temp;
      }      

   return;
   } 
   
   

/**********************************************************************/
/*  GET_RING_COORDS                                                   */
/*      Derived from outline_face_ring  [vpfdraw.c]                   */
/**********************************************************************/
#ifdef PROTO
   void get_ring_coords (RING *ring, int32 face_id,int32 start_edge,
                         vpf_table_type edgetable)
#else
   void get_ring_coords (ring, face_id, start_edge, edgetable)
      RING *ring;
      int32 face_id, start_edge;
      vpf_table_type edgetable;
#endif

   {
   edge_rec_type edge_rec;
   int32 next_edge, prevnode, i, n=0;
   boolean done=FALSE;
   vpf_projection_type proj;
   double_coordinate_type  dcoord;
   SEGMENT **temp;

   proj = NOPROJ;

   edge_rec = read_edge (start_edge, edgetable, proj.inverse_proj);
   edge_rec.dir = '+';
   prevnode = edge_rec.start_node;

   if (edge_rec.start_node == edge_rec.end_node)
      done = TRUE;
   next_edge = next_face_edge (&edge_rec, &prevnode, face_id);
   
   /* Allocate plenty of space for array of segment addresses */
   ring->segs = (SEGMENT**)xvt_zmalloc (MAXSEGS * sizeof (SEGMENT*));
   if (ring->segs == NULL)
      xvt_fatal ("GET_RING_COORDS:1 Out of memory!");


   /* Load the first segment of the ring */
   ring->segs[n] = (SEGMENT*)xvt_zmalloc (sizeof (SEGMENT));
   if (ring->segs[n] == NULL)
      xvt_fatal ("GET_RING_COORDS:2 Out of memory!");

   ring->segs[n]->nr_coords = edge_rec.npts;
   ring->segs[n]->id = n+1;

   /* Allocate space for the coordinates of the first segment */
   ring->segs[n]->coords = (COORDINATE*)xvt_zmalloc ((size_t)edge_rec.npts *
                                                                           sizeof (COORDINATE));
   if (ring->segs[n]->coords == NULL)
      xvt_fatal ("GET_RING_COORDS:3 Out of memory!");

                                                
  /* If the direction is - load in reverse order */
   if (edge_rec.dir == '-')
      {
      for (i=(edge_rec.npts-1); i>=0; i--)
         {
         dcoord = next_edge_coordinate (&edge_rec);
         ring->segs[n]->coords[i].x = (float)dcoord.x;
         ring->segs[n]->coords[i].y = (float)dcoord.y;
         }
      }
   else
      {
      for (i=0; i<edge_rec.npts; i++)
         {
         dcoord = next_edge_coordinate (&edge_rec);
         ring->segs[n]->coords[i].x = (float)dcoord.x;
         ring->segs[n]->coords[i].y = (float)dcoord.y;
         }
      }

   n++;
   if (edge_rec.coords)
      xvt_free ((char*)edge_rec.coords);

   while (!done)
      {
      if (next_edge < 0)
         {
         xvt_note (
         "topology error! Edge: %d  face: %d  left: %d  right: %d\n",
         edge_rec.id,face_id,edge_rec.left_face,edge_rec.right_face);
         done = TRUE;
         }

      if (next_edge == 0)
         {
         xvt_note ("Next edge(%d) = 0\n",(int)edge_rec.id);
         done = TRUE;
         }

      if (next_edge == start_edge)
         done = TRUE;

      if (!done)
         {
         edge_rec = read_edge( next_edge, edgetable, NOPROJ.inverse_proj);
         next_edge = next_face_edge( &edge_rec, &prevnode, face_id );
 
         /* Allocate space for the next segment */
         ring->segs[n] = (SEGMENT*)xvt_zmalloc (sizeof (SEGMENT));
         if (ring->segs[n] == NULL)
            xvt_fatal ("GET_RING_COORDS:4 Out of memory!");

         
         /* Allocate space for the segment coordinates */
         ring->segs[n]->coords = (COORDINATE*)xvt_zmalloc ((size_t)edge_rec.npts *
                                                                          sizeof (COORDINATE));
         if (ring->segs[n]->coords == NULL)
            xvt_fatal ("GET_RING_COORDS:5 Out of memory!");

         ring->segs[n]->nr_coords = edge_rec.npts;
         ring->segs[n]->id = n+1;


         /* If the direction is - load in reverse order */
         if (edge_rec.dir == '-')
            {
            for (i=(edge_rec.npts-1); i>=0; i--)
               {
               dcoord = next_edge_coordinate (&edge_rec);
               ring->segs[n]->coords[i].x = (float)dcoord.x;
               ring->segs[n]->coords[i].y = (float)dcoord.y;
               }
            }
         else
            {
            for (i=0; i<edge_rec.npts; i++)
               {
               dcoord = next_edge_coordinate (&edge_rec);
               ring->segs[n]->coords[i].x = (float)dcoord.x;
               ring->segs[n]->coords[i].y = (float)dcoord.y;
               }
            }
         n++;
         if (edge_rec.coords)
            xvt_free ((char*)edge_rec.coords);

         } /* if (!done) */
      } /* while */              
   ring->nr_segs = n;

   /* Realloc the segs array to free unused memory */
   temp = (SEGMENT**)xvt_zmalloc (ring->nr_segs * sizeof (SEGMENT*));
   if (temp == (SEGMENT**)NULL)
      xvt_fatal ("GET_RING_COORDS:6 Out of memory!");
   else
      {
      memcpy (temp, ring->segs, (ring->nr_segs * sizeof (SEGMENT*)));
      xvt_free ((char*)ring->segs);
      ring->segs = temp;
      }

   return;
   } 

/**********************************************************************/
/*   NEXT_FACE_EDGE                                                   */
/*      Derived from next_face_edge   [vpfdraw.c]                     */
/**********************************************************************/
#ifdef PROTO
   int32 next_face_edge (edge_rec_type *edge_rec, int32 *prevnode, int32 face_id)
#else
   int32 next_face_edge (edge_rec, prevnode, face_id)
      edge_rec_type *edge_rec;
      int32 *prevnode, face_id;
#endif

   {
   int32 next;

   if ((edge_rec->right_face == face_id) && 
       (edge_rec->left_face == face_id))
      {
      /* Dangle - go the opposite dir to continue along the boundary */
      if (*prevnode == edge_rec->start_node)
         {
         edge_rec->dir = '-';
         next = edge_rec->left_edge;
         *prevnode = edge_rec->start_node;
         }
      else if (*prevnode == edge_rec->end_node)
         {
         edge_rec->dir = '+';
         next = edge_rec->right_edge;
         *prevnode = edge_rec->end_node;
         }
      else
         next = -1;
      }
   else if (edge_rec->right_face == face_id)
      {
      /* The face is on the right - take the right forward edge */
      next = edge_rec->right_edge;
      edge_rec->dir = '+';
      *prevnode = edge_rec->end_node;
      }
   else if (edge_rec->left_face == face_id)
      {
      /* The face is on the left - take the left forward edge */
      next = edge_rec->left_edge;
      edge_rec->dir = '-';
      *prevnode = edge_rec->start_node;
      }
   else
      next = -1;

   return next;
   }  

/*****************************************************************************/
/* GET_XY                                                                    */
/*****************************************************************************/
#ifdef PROTO
   COORDINATE *get_xy (vpf_table_type table, row_type row, int32 pos, int32 *count)
#else
   COORDINATE *get_xy (table, row, pos, count)
      vpf_table_type table;
      row_type       row;
      int32           pos;
      int32           *count;
#endif

   {
   int32 i;
   COORDINATE *coord = NULL;

   switch (table.header[pos].type)
      {
      case 'C':
         {
         coordinate_type temp, *ptr;
         ptr = get_table_element (pos, row, table, &temp, count);

         if ((*count == 1) && (ptr == (coordinate_type*)NULL))
            {
            coord = (COORDINATE*)xvt_zmalloc (sizeof (COORDINATE));
            if (coord == (COORDINATE*)NULL)
               xvt_fatal ("GET_XY:1 Out of memory!");
            coord->x = temp.x;
            coord->y = temp.y;
            }
         else
            coord = (COORDINATE*)ptr;
         break;
         }
      case 'Z':
         {
         tri_coordinate_type temp, *ptr;
         ptr = get_table_element (pos, row, table, &temp, count);
         coord = (COORDINATE*)xvt_zmalloc (sizeof (COORDINATE) * (size_t)*count);
         if (coord == (COORDINATE*)NULL)
            xvt_fatal ("GET_XY:2 Out of memory!");
         if ((*count == 1) && (ptr == (tri_coordinate_type*)NULL))
            {
            coord->x = temp.x;
            coord->y = temp.y;
            }
         else
            {
            for (i=0; i<*count; i++)
               {
               coord[i].x = ptr[i].x;
               coord[i].y = ptr[i].y;
               }
            }
         if (ptr)
            xvt_free ((char*)ptr);
         break;
         }
      case 'B':
         {
         double_coordinate_type temp, *ptr;
         ptr = get_table_element (pos, row, table, &temp, count);
         coord = (COORDINATE*)xvt_zmalloc (sizeof (COORDINATE) * (size_t)*count);
         if (coord == (COORDINATE*)NULL)
            xvt_fatal ("GET_XY:3 Out of memory!");
         if ((*count == 1) && (ptr == (double_coordinate_type*)NULL))
            {
            coord->x = (float)temp.x;
            coord->y = (float)temp.y;
            }
         else
            {
            for (i=0; i<*count; i++)
               {
               coord[i].x = (float)ptr[i].x;
               coord[i].y = (float)ptr[i].y;
               }
            }
         if (ptr)
            xvt_free ((char*)ptr);
         break;
         }

      case 'Y':
         {
         double_tri_coordinate_type temp, *ptr;
         ptr = get_table_element (pos, row, table, &temp, count);
         coord = (COORDINATE*)xvt_zmalloc (sizeof (COORDINATE) * (size_t)*count);
         if (coord == (COORDINATE*)NULL)
            xvt_fatal ("GET_XY:4 Out of memory!");
         if ((*count == 1) && (ptr == (double_tri_coordinate_type*)NULL))
            {
            coord->x = (float)temp.x;
            coord->y = (float)temp.y;
            }
         else
            {
            for (i=0; i<*count; i++)
               {
               coord[i].x = (float)ptr[i].x;
               coord[i].y = (float)ptr[i].y;
               }
            }
         if (ptr)
            xvt_free ((char*)ptr);
         break;
         }

      default:
           xvt_note ("GET_XY: no such type %c", table.header[pos].type);
           break;
      } /* switch type */
    return (coord);
    }
