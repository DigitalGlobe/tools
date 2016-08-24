
/*************************************************************************
 *
 *N  Module VPFSELEC - VPF SELECTED FEATURES
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This module contains functions for selecting VPF features and
 *     primitives.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     N/A
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   Nov 1991                           DOS Turbo C
 *                     Feb 1992 - Optimized for CD-ROM performance.
 *E
 *************************************************************************/

#ifndef __VPF_H__
#include "vpf.h"
#endif
#ifndef H_MUSEDIR
#include "musedir.h"
#endif
#ifndef __VPFQUERY_H__
#include "vpfquery.h"
#endif
#ifndef __VPFTIDX_H__
#include "vpftidx.h"
#endif
#ifndef __VPFSELEC_H__
#include "vpfselec.h"
#endif

/*************************************************************************
 *
 *N  get_fit_tile_primitives
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Get the set of primitives in the given tile for the selected features
 *    in the given feature class.  Use the Feature Index Table instead of
 *    the schema relationships.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     covpath   <input>==(char *) path th the VPF coverage.
 *     primclass <input>==(int) primitive class to select.
 *     expression<input>==(char *) expression to apply to the feature table.
 *     feature_table <input>==(vpf_table_type) feature table.
 *     tile      <input>==(int32) tile number.
 *     fca_id    <input>==(int) Feature Class Attribute table id of the
 *                              selected feature class.
 *     numprims  <input>==(int) number of rows in the specified tile's
 *                              primitive table for the specified primitive
 *                              class.
 *     status   <output>==(int *) status of the function:
 *                         1 if completed, 0 if user escape.
 *     return   <output>==(set_type) set of primitives for the features
 *                         in the corresponding tile.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/

#ifdef PROTO
int32 query_table_row (char *expression, row_type row, vpf_table_type table);
#else
int32 query_table_row (char *expression, row_type row, vpf_table_type table);
#endif

#ifdef PROTO
   set_type get_fit_tile_primitives (char *covpath, int32 primclass, char *expression,
				                      vpf_table_type feature_table, int32 tile, int32 fca_id,
				                      int32 numprims, int32 *status)
#else
   set_type get_fit_tile_primitives (covpath, primclass, expression, feature_table, tile,
                                     fca_id, numprims, status)
      char *covpath;
      int32 primclass;
      char *expression;
      vpf_table_type feature_table;
      int32 tile, fca_id, numprims, *status;
#endif

   {
   set_type primitives, tileset, fcset, selset;
   int32 i, start, end, prim_id, tile_id, fc_id, feature_id, count;
   short short_tile_id;
   int32 PRIM_ID_, TILE_ID_, FC_ID_, FEATURE_ID_;
   vpf_table_type fit;
   row_type row, frow;
   char path[255];
   static char *ptable[] = {"","EDG","FAC","TXT","END","CND"};

   primitives = set_init (numprims+1);

   strcpy (path, covpath);
   strcat (path, ptable[primclass]);
   strcat (path, ".FIT");
   muse_check_path (path);

   if (muse_access (path,0) != 0)
      return primitives;

   fit = vpf_open_table (path, disk, "rb", NULL);
   if (!fit.fp)
      return primitives;

   TILE_ID_ = table_pos ("TILE_ID", fit);
   PRIM_ID_ = table_pos ("PRIM_ID", fit);
   FC_ID_ = table_pos ("FC_ID", fit);
   if (FC_ID_ < 0)
      FC_ID_ = table_pos ("FCA_ID", fit);
   FEATURE_ID_ = table_pos ("FEATURE_ID", fit);
   if ((TILE_ID_ < 0 && tile) || PRIM_ID_ < 0 ||
       FC_ID_ < 0 || FEATURE_ID_ < 0)
      {
      vpf_close_table (&fit);
      *status = 0;
      return primitives;
      }

   /* Look for TILE_ID thematic index */
   tileset.size = 0;
   if (tile)
      {
      if (fit.header[TILE_ID_].tdx)
         {
	      strcpy (path, covpath);
	      strcat (path, fit.header[TILE_ID_].tdx);
	      muse_check_path (path);
	      if (muse_access (path,0) == 0)
	         {
	         if (fit.header[TILE_ID_].type == 'I')
	            {
	            tile_id = (int32)tile;
	            tileset = read_thematic_index (path, (char*)&tile_id);
	            }
	         else if (fit.header[TILE_ID_].type == 'S')
	            {
	            short_tile_id = tile;
	            tileset = read_thematic_index(path,(char *)&short_tile_id);
	            }
	         } /* if muse_access */
         } /* if tile_id.tdx */
      } /* if tile */

   if (!tileset.size)
      {
      tileset = set_init (fit.nrows+1);
      set_on (tileset);
      set_delete (0, tileset);
      }

   /* Look for FC_ID thematic index */
   fcset.size = 0;
   if (fit.header[FC_ID_].tdx)
      {
      strcpy (path, covpath);
      strcat (path, fit.header[FC_ID_].tdx);
      muse_check_path (path);
      if (muse_access (path, 0) == 0)
         {
         fc_id = (int32)fca_id;
         fcset = read_thematic_index (path, (char*)&fc_id);
         }
      } /* if fc_id.tdx */

   if (!fcset.size) 
      {
      fcset = set_init (fit.nrows+1);
      set_on (fcset);
      set_delete (0, fcset);
      }

   /* Get the set of all FIT rows in the search tile that match the
      search fca_id */
   selset = set_intersection (tileset, fcset);
   set_nuke (&tileset);
   set_nuke (&fcset);

   if (set_empty (selset))
      {
      vpf_close_table (&fit);
      set_nuke (&selset);
      *status = 1;
      return primitives;
      }


   /* Now loop through the FIT and get the matching primitive ids */
   start = set_min (selset);
   end = set_max (selset);

   /* Set file pointer to start record */
   fseek (fit.fp, index_pos (start, fit), SEEK_SET);

   for (i=start; i<=end; i++)
      {
      /* Read each row of the fit starting as start. IF the row is a member */
      /* of selset then get the tile_id.                                    */
      row = read_next_row (fit);   
      if (set_member (i, selset))
         {
      
         /* i is a member of selset so now we must test it to see if it */
         /* meets the conditions of the thematic expression             */
         get_table_element(PRIM_ID_,row,fit,&prim_id,&count);
         get_table_element(FC_ID_,row,fit,&fc_id,&count);
         get_table_element(FEATURE_ID_,row,fit,&feature_id,&count);
         tile_id = 0;
         if (tile)
            {
            if (fit.header[TILE_ID_].type == 'I')
               {
               get_table_element (TILE_ID_, row, fit, &tile_id, &count);
               }
            else if (fit.header[TILE_ID_].type == 'S')
               {
               get_table_element(TILE_ID_, row, fit, &short_tile_id, &count);
               tile_id = short_tile_id;
               }
            } /* if tile */
         free_row (row, fit);

         if (tile_id != tile  ||  fc_id != fca_id)
            continue;
         frow = get_row (feature_id, feature_table);
         if (query_table_row (expression, frow, feature_table))
	         set_insert (prim_id, primitives);
         free_row(frow,feature_table);
         }  /* if set_member */
      free_row (row, fit);
      } /* for i */

   vpf_close_table (&fit);
   set_nuke (&selset);

   *status = 1;

   return primitives;
   }

