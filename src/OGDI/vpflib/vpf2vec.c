/**************************************************************************/
/* VPF2VEC.C                                                              */
/*                                                                        */
/*      Contents:                                                         */
/*    vpf2vec.c                                                           */
/*    get_features                                                        */
/*    get_selected_tile_primitives                                        */
/*    primitives_within_extent                                            */
/*    get_tile_set                                                        */
/*    bit_fix                                                             */
/*                                                                        */
/**************************************************************************/

#include "xvt.h"
#include "vpf.h"
#include "vpfview.h"
#include "vpfrelat.h"
#include "vpfspx.h"
#include "vpfprim.h"
#include "vpfproj.h"
#include "vpfquery.h"
#include "vpfselec.h"
#include "get_feat.h"
#include "params.h"
#include "vec_d.h" 
#include "musedir.h"
#include "vpf2vec.h"
#include "strfunc.h"
#include "i_stat.h"

/* Prototype */
set_type spatial_index_search( char *fname,float x1, float y1, float x2, float y2 );

/**************************************************************************/
/*   VPF2VEC                                                              */
/**************************************************************************/

#ifdef PROTO
   void vpf2vec (USER_PARAMS *params, VEC *vec)
#else
   void vpf2vec (params, vec)
      USER_PARAMS *params;
      VEC *vec;
#endif
   {
   view_type view;
   map_environment_type mapenv;
   char StatusMessage[40], string[40];
   static char sep[2] = {DIR_SEPARATOR,'\0'};

   /* Create the Status Window */
   strcpy (StatusMessage, "INITIALIZING!");
   strcpy (string, "Please be patient");
   if (iCreateStatus (StatusMessage, string) != TRUE)
      {
      xvt_note ("VPF2VEC: Cannot create Status Window.");
      return;
      }

  /* Allocate space for view, database, & library structures */
   view.path = (char*)xvt_malloc (PATHSIZE);
   if (view.path == (char*)NULL)
      {
      xvt_note ("VPF2VEC: Out of memory!");
      clean_up (view);
      return;
      }

   strcpy (view.name, "MUSE");
   view.ndb = 1L;
   view.nthemes = 1L;

   view.database = (database_type*) xvt_zmalloc (sizeof (database_type));
   if (view.database == (database_type*)NULL)
      {
      xvt_note ("VPF2VEC: Out of memory!");
      clean_up (view);
      return;
      }

   view.database->path = (char*)xvt_zmalloc (PATHSIZE);
   view.database->nlibraries = 1;

   view.database->library = (library_type*)xvt_zmalloc (sizeof (library_type));
   if (view.database->library == (library_type*)NULL)
      {
      xvt_note ("VPF2VEC: Out of memory!");
      clean_up (view);
      return;
      }

   view.database->library->viewable = TRUE;
   view.database->library->path = (char*)xvt_zmalloc (PATHSIZE);
   if (view.database->library->path == (char*)NULL)
      {
      xvt_note ("VPF2VEC: Out of memory!");
      clean_up (view);
      return;
      }

   strcpy (view.database->path, params->db_path);
   strcat (view.database->path, sep);
   strcpy (view.database->name, params->db_name);
   strcpy (view.database->library->name, params->lib_name);

   /* Set Spatial Extents */
   mapenv.mapextent.x1 = params->x_min;
   mapenv.mapextent.x2 = params->x_max;
   mapenv.mapextent.y1 = params->y_min;
   mapenv.mapextent.y2 = params->y_max;

   /* Accomidate crossing the 180 meridian */
   if (mapenv.mapextent.x1 > mapenv.mapextent.x2)
      mapenv.mapextent.x2 += 360.E0;

   strcpy (view.database->library->path, view.database->path);
   strcat (view.database->library->path, view.database->name);
   strcat (view.database->library->path, sep); 
   strcat (view.database->library->path, view.database->library->name);
   strcat (view.database->library->path, sep);

   view.database->library->projection = DDS;
   view.database->library->units = DDS;

   strcpy (view.path, view.database->path);
   strcat (view.path, view.database->name);


   /* THEMES */
   view.theme = (theme_type*)xvt_malloc (sizeof (theme_type));
   if (view.theme == NULL)
      {
      xvt_note ("VPF2VEC: Out of memory!");
      clean_up (view);
      return;
      }

   view.nthemes = 1;

   /* Set Theme parameters */
   view.theme->database = (char*)xvt_malloc (SZ_FNAME);
   if (view.theme->database == (char*)NULL)
      {
      xvt_note ("VPF2VEC: Out of memory!");
      clean_up (view);
      return;
      }

   strcpy (view.theme->database, view.database->path);
   view.theme->database = strcat
   (view.theme->database, view.database->name);

   view.theme->library = (char*)xvt_malloc (SZ_FNAME);
   if (view.theme->library == NULL)
      {
      xvt_note ("VPF2VEC: Out of memory!");
      clean_up (view);
      return;
      }

   strcpy (view.theme->library, view.database->library->name);

   view.theme->coverage = (char*)xvt_malloc (SZ_FNAME);
   if (view.theme->coverage == (char*)NULL)
      {
      xvt_note ("VPF2VEC: Out of memory!");
      clean_up (view);
      return;
      }

   strcpy (view.theme->coverage, params->cov_name);

   view.theme->expression = (char*)xvt_malloc (80 * sizeof (char));
   if (view.theme->expression == (char*)NULL)
      {
      xvt_note ("VPF2VEC: Out of memory!");
      clean_up (view);
      return;
      }

   strcpy (view.theme->expression, params->expression);

   view.theme->fc = (char*)xvt_malloc (SZ_FNAME);
   if (view.theme->fc == (char*)NULL)
      {
      xvt_note ("VPF2VEC: Out of memory!");
      clean_up (view);
      return;
      }

   strcpy (view.theme->fc, params->fclass);

   /* Feature Table Path */
   view.theme->ftable = (char*)xvt_malloc (SZ_FNAME);
   if (view.theme->ftable == (char*)NULL)
      {
      xvt_note ("VPF2VEC: Out of memory!");
      clean_up (view);
      return;
      }

   strcpy (view.theme->ftable, view.theme->database);
   strcat (view.theme->ftable, sep);
   strcat (view.theme->ftable, view.theme->library);
   strcat (view.theme->ftable, sep);
   strcat (view.theme->ftable, view.theme->coverage);
   strcat (view.theme->ftable, sep);
   strcat (view.theme->ftable, view.theme->fc);

   /* Allocate space for vec structure */
   vec->magic = MAGIC_VEC2_DATA;

   vec->extents = (VEC_EXTENT*)xvt_zmalloc (sizeof (VEC_EXTENT));
   if (vec->extents == (VEC_EXTENT*)NULL)
      {
      xvt_note ("VPF2VEC: Out of memory!");
      clean_up (view);
      return;
      }

   vec->extents->xmin = params->x_min;
   vec->extents->xmax = params->x_max;
   vec->extents->ymin = params->y_min;
   vec->extents->ymax = params->y_max;

   /* Accomidate crossing the 180 meridian */
   if (vec->extents->xmin > vec->extents->xmax)
      vec->extents->xmax += 360.E0;


   vec->feature_type = params->feature_type;

   switch (params->feature_type)
      {
      case LINE:
         {
         strcat (view.theme->ftable, ".lft");
         break;
         }
      case AREA:
         {
         strcat (view.theme->ftable, ".aft");
         break;
         }
      case ANNO:
         {
         strcat (view.theme->ftable, ".tft");
         break;
         }
      case VPFPOINTS:
         {
         strcat (view.theme->ftable, ".pft");
         break;
         }
      case COMPLEX:
         break;
      default:
         {
         xvt_note ("VPF_SS: Invalid feature type.");
         return;
         }
      }


   get_features (vec, &view, &mapenv);

   clean_up (view);

   return;
   }

/*********************************************************************/
/* CLEAN_UP                                                          */
/*********************************************************************/
#ifdef PROTO
   void clean_up (view_type view)
#else
   void clean_up (view)
      view_type view;
#endif

   {
      /* Free allocated memory */
   if (view.path)
      xvt_free (view.path);
   if (view.theme)
      {
      if (view.theme->database)
         xvt_free (view.theme->database);
      if (view.theme->library)
         xvt_free (view.theme->library);
      if (view.theme->fc)
         xvt_free (view.theme->fc);
      if (view.theme->ftable)
         xvt_free (view.theme->ftable);
      xvt_free ((char*)view.theme);
      }
   if (view.database)
      {
      if (view.database->library)
         {
         if (view.database->library->path)
            xvt_free (view.database->library->path);
         if (view.database->library->tile_set.buf)
            set_nuke (&view.database->library->tile_set);
         xvt_free ((char*)view.database->library);
         }
      if (view.database->path)
         xvt_free (view.database->path);
      xvt_free ((char*)view.database);
      }

   /* Destroy Status Window */
   iDestroyStatus ();

   return;
   }



/*****************************************************************************/
/* GET_FEATURES                                                              */
/*                                                                           */
/*   Purpose:                                                                */ 
/*      This function retreives the selected features from a specified       */
/*      feature class based upon a selection expression.                     */
/*   Parameters:                                                             */
/*     view   <input>  == (view_type*) view structure.                       */
/*     mapenv <input>  == (map_environment_type*) map environment structure. */
/*     return <output> == (int32) completion status:                          */
/*                        1 if completed successfully,                       */
/*                        0 if an error occurred.                            */
/*****************************************************************************/
#ifdef PROTO
   int32 get_features (VEC *vec, view_type *view, map_environment_type *mapenv)
#else
   int32 get_features (vec, view, mapenv)
      VEC                  *vec;
      view_type            *view;
      map_environment_type *mapenv;
#endif

   {
   int32 status, finished=1, tilecover, TILEPATH_=0, prim;
   int32 number_relate_paths, relpathnum;
   vpf_table_type rngtable,edgtable,fbrtable, tile_table, fcs;
   vpf_table_type fca, ft;
   int32 fit=0L, fc_id;
   row_type row;
   char ptype[4], StatusMessage[40], string[40];
   int32 i, j, n=0L, tile, tileid;
   int32 pclass;
   int32 starttile, endtile, startprim, endprim;
   int32 count, nr, TABLE_NAME_, PRIM_TYPE_;
   char path[255], libpath[255], covpath[255], tiledir[255], ftable[255];
   char *buf, *primtype;
   static char sep[2] = {DIR_SEPARATOR,'\0'};
   set_type sprims, primitives, feature_rows;
   fcrel_type fcrel;
   int32 nr_features;
   LINE_FEATURE **temp_lines;
   AREA_FEATURE **temp_areas;
   TEXT_FEATURE **temp_text;
   POINT_FEATURE **temp_points;
   BOOLEAN stop_processing = FALSE;



   strcpy (libpath, view->theme->database);
   strcat (libpath, sep);
   strcat (libpath, view->theme->library);
   strcat (libpath, sep);

   strcpy (covpath, libpath);
   strcat (covpath, view->theme->coverage);
   strcat (covpath, sep);





   /* Look for feature class entry in FCA */
   strcpy (path, covpath);
   strcat (path, "fca");

   if (muse_access (path, 0) == 0)
      {
      fca = vpf_open_table (path, disk, "rb", NULL);
      j = table_pos ("FCLASS",fca);

      for (i=1; i<=fca.nrows; i++)
         {
         row = read_next_row (fca);
         buf = (char*)get_table_element (j, row, fca, NULL, &count);
         rightjust (buf);

         if (Mstrcmpi (buf, view->theme->fc) == 0)
            {
            fc_id = (int32)i;
            i = fca.nrows + 2;
            }
         xvt_free (buf);
         free_row (row, fca);
         }
      vpf_close_table(&fca);
      }

   /* Get the set of Feature_IDs that satisfy the thematic expression */
   strcpy (path, view->theme->ftable);
   ft = vpf_open_table (path, disk, "rb", NULL);

   feature_rows = query_table (view->theme->expression, ft);
   vpf_close_table (&ft);
   nr_features = num_in_set (feature_rows);

   /* Return if no features were found */
   if (nr_features == 0)
      return (-1);  

   /* Set Primitive Class type and                 */
   /* allocate memory for array of feature pointes */
   switch (vec->feature_type)
      {
      case LINE:
         {
         vec->lines = (LINE_FEATURE**)xvt_zmalloc
                      (MAXLINES * sizeof (LINE_FEATURE*));
         if (vec->lines == NULL)
            {
            xvt_fatal ("GET_FEATURES: Out of memory!");
            return (-1);
            }
         strcpy (ptype, "EDG");
         pclass = 1;
         break;
         }
      case AREA:
         {
         vec->areas = (AREA_FEATURE**)xvt_zmalloc
                      (MAXAREAS * sizeof (AREA_FEATURE*));
         if (vec->areas == NULL)
            {
            xvt_fatal ("GET_FEATURES: Out of memory!");
            return (-1);
            }
         strcpy (ptype, "FAC");
         pclass = 2;
         break;
         }
      case ANNO:
         {
         vec->text = (TEXT_FEATURE**)xvt_zmalloc
                     (MAXTEXT * sizeof (TEXT_FEATURE*));
          if (vec->text == NULL)
            {
            xvt_fatal ("GET_FEATURES: Out of memory!");
            return (-1);
            }
         strcpy (ptype, "TXT");
         pclass = 3;
         break;
         }
      case VPFPOINTS:
         {
         vec->points = (POINT_FEATURE**)xvt_zmalloc
                       (MAXPOINTS * sizeof (POINT_FEATURE*));
         if (vec->points == NULL)
            {
            xvt_fatal ("GET_FEATURES: Out of memory!");
            return (-1);
            }
         /* Determine if points are Entity Nodes "END" or */
         /* Connected Nodes "CND".                        */
         strcpy (string, view->theme->fc);
         strcat (string, ".PFT");

         /* Open Feature Class Schema table "FCS" */
         strcpy (path, covpath);
         strcat (path, "fcs");
         fcs= vpf_open_table (path, disk, "rb", NULL);

         /* Find the column that contains the feature table name */
         TABLE_NAME_ = table_pos ("TABLE1", fcs);
         PRIM_TYPE_  = table_pos ("TABLE2", fcs);

         /* Find the correct row for the feature table */
         for (i=0; i<fcs.nrows; i++)
            {
            row = read_next_row (fcs);
            buf = (char*)get_table_element(TABLE_NAME_, row, fcs, NULL, &n);
            buf = leftjust (buf);
            buf = rightjust (buf);
            if (strcmpi (string, buf) == 0)
               {
               /* Get the primitive type */
               primtype = (char*)get_table_element (PRIM_TYPE_, row, fcs, NULL, &n);
               primtype = leftjust (primtype);
               primtype = rightjust (primtype);
               free_row (row, fcs);
               xvt_free (buf);
               break;
               }
            free_row (row, fcs);
            xvt_free (buf);
            }
         vpf_close_table (&fcs);
         strcpy (ptype, primtype);
         xvt_free (primtype);
         pclass = 4;
         n = 0;
         break;
         }
      case COMPLEX:
         break;
      } 


   /*** Look for Feature Index Table (FIT) ***/
   strcpy (path, covpath);
   strcat (path, ptype);
   strcat (path,".FIT");
   fit = (muse_access (path, 0) == 0);

   /* Set up the feature class table relate chain.        */
   /* The feature table is fcrel.table[0].                */
   /* The primitive table is the last table in the chain: */
   /*  fcrel.table[ fcrel.nchain-1 ].                     */

   if (fit)
      {
      number_relate_paths = 1;
      }
   else
      {
      strcpy (path, covpath);
      strcat (path, "FCS");
      fcs = vpf_open_table (path, disk, "rb", NULL);
      number_relate_paths = num_relate_paths (ptype,
                                              view->theme->fc, fcs);
      vpf_close_table (&fcs);
      }


   /* Look for the primitive table at the coverage level */
   /* to determine if the coverage is tiled.             */
   strcpy (path, covpath);
   strcat (path, ptype);
   if (muse_access (path, 0) == 0)
      {
      tilecover = FALSE;
      starttile = 1;
      endtile = 1;
      }
   else
      {
      view->database->library->tile_set = get_tile_set
      (libpath, mapenv->mapextent, &stop_processing);

   if (stop_processing)
      return (-1);

      /* The coverage is tiled, open the TILEREF.AFT table */
      strcpy (path, libpath);
      strcat (path, "tileref");
      strcat (path, sep);
      strcat (path, "tileref.aft");
      if ((muse_access (path, 0)) == 0)
         {
         tilecover = TRUE;
         starttile = set_min (view->database->library->tile_set);
         endtile = set_max (view->database->library->tile_set);
         tile_table = vpf_open_table (path, disk, "rb", NULL);
         TILEPATH_ = (int32)table_pos ("TILE_NAME", tile_table);       
         }
      else
         {
         /* No primitive table found & no tileref table found */
         /* This is a problem.                                */
         xvt_error ("GET_FEATURES: Can't find primitive table.");
         return (-1);
         }
      }



   /**** RELATIVE PATH LOOP ***************************************/

   for (relpathnum=0; relpathnum<number_relate_paths; relpathnum++)
      {
      if (stop_processing)
         break;

      if (fit)
         {
         /* Only need the primitive table */
         fcrel.table = (vpf_table_type*)xvt_zmalloc (sizeof (vpf_table_type));
         if (fcrel.table == NULL)
            xvt_fatal ("GET_FEATURES: Out of memory!");

         fcrel.relate_list = NULL;
         fcrel.nchain = 1;
         }
      else
         {
         /* Initialize the feature class relates */
         j = strlen (view->theme->ftable) - 1;
         while (view->theme->ftable[j] != DIR_SEPARATOR && j>0)
            j--;
         if (view->theme->ftable[j] == DIR_SEPARATOR)
            strcpy (ftable, &view->theme->ftable[j+1]);
         else
            strcpy (ftable, view->theme->ftable);
         rightjust (ftable);
         fcrel = select_feature_class_relate (covpath,
         view->theme->fc, ptype, ftable, relpathnum);
         }
      prim = 0;

      /* Open the Feature_Table */
      ft = vpf_open_table (view->theme->ftable, disk, "rb", NULL);




      /**** Tile Loop ********************************************/
      /* If the coverage is untiled still go thru the loop once. */
      /* That is, treat an untiled coverage as a single tile.    */
      /***********************************************************/
      for (tile = starttile; tile <= endtile; tile++)
         {
         if (stop_processing)
            break;

         /* Update Status Window */
         sprintf (StatusMessage, "Retrieve Features");
         sprintf (string, "Searching Tile Nr. %ld", tile);
         if (iUpdateStatus (StatusMessage, string) != TRUE)
            xvt_note ("VPF2vec: Update Status failed.");
         if (iCheckCancel() != TRUE)
            return (-1);

         if (tilecover)
            {
            if (!set_member(tile,view->database->library->tile_set))
               continue;
            row = get_row (tile, tile_table);
            buf = (char*)get_table_element (TILEPATH_, row, tile_table,
                                                               NULL, &count);
            free_row (row, tile_table);
            /* Convert uppercase chars to lowercase */
            buf = strlwr (buf);
            strcpy (tiledir, buf);
            rightjust (tiledir);
            /* Filter path for system specific file separators */
            muse_check_path (tiledir);
            strcat (tiledir, sep);
            xvt_free (buf);
            tileid = tile;
            }
         else
            {
            strcpy (tiledir, "");
            tileid = 0;
            }
         finished = TRUE;

         strcpy (path, covpath);
         strcat (path, tiledir);
         strcat (path, ptype);

         /* Check for primitive table within the current tile */
         if (muse_access (path, 0) != 0)
            continue;
         fcrel.table[prim] = vpf_open_table (path, disk, "rb", NULL);

         wait_cursor ();

         if (fit)
            {
            /* Get set of primitives that meet spatial constraints */
            sprims = primitives_within_extent (mapenv->mapextent, covpath,
                     tiledir, pclass, fcrel.table[prim].nrows);
            nr = num_in_set (sprims);

            /* Get set of primitives that meet both spatial and */
            /* thematic constraints.                            */
            primitives = get_fit_primitives (covpath, pclass,
                         feature_rows, sprims, tileid, fc_id,
                         fcrel.table[prim].nrows, &stop_processing);
            nr = num_in_set (primitives);
            set_nuke (&sprims);
            }
         else
            {
            primitives = get_selected_tile_primitives (covpath, fcrel,
                         pclass, feature_rows, mapenv, tileid, tiledir,
                         &status);
            }

         if (primitives.size < 1)
            {
            vpf_close_table (&fcrel.table[prim]);
            continue;
            }

         if (set_empty (primitives))
            {
            set_nuke (&primitives);
            vpf_close_table (&fcrel.table[prim]);
            continue;
            }

         finished = 1;

         startprim = set_min (primitives);
         endprim = set_max (primitives);

         /* It turns out to be MUCH faster off of a CD-ROM to */
         /* read each row and discard unwanted ones than to   */
         /* forward seek past them.  It's about the same off  */
         /* of a hard disk.          */

         fseek (fcrel.table[prim].fp,
         index_pos (startprim, fcrel.table[prim]), SEEK_SET);

         /* Load the primitives into the vec structure */
         switch (pclass)
            {
            case EDGE:
               {
               for (i=startprim; i<endprim; i++)
                  {
                  if (stop_processing)
                     break;
                  row = read_next_row (fcrel.table[prim]);
                  if (set_member (i, primitives))
                     {
                     vec->lines[n] = (LINE_FEATURE*)xvt_zmalloc (sizeof (LINE_FEATURE));
                     if (vec->lines[n] == NULL)
                        xvt_fatal ("GET_FEATURES: Out of memory!");
                     vec->lines[n]->id = i;
                     get_line_feature (vec->lines[n], row, fcrel.table[prim]);
                     vec->nr_lines += 1;
                     n++;

                     sprintf (string, "%ld Found", vec->nr_lines);
                     if (iUpdateStatus (StatusMessage, string) != TRUE)
                        xvt_note ("VPF2vec: Update Status failed.");
                     if (iCheckCancel() != TRUE)
                        stop_processing = TRUE;
                     
                     if (vec->nr_lines == MAXLINES)
                        {
                        xvt_note ("Line feature limit reached (MAXLINES)");
                        stop_processing = TRUE;
                        }
                     }
                  free_row (row, fcrel.table[prim]);
                  }
               break;
               }
            case ENTITY_NODE:
            case CONNECTED_NODE:
               {
               for (i=startprim; i<endprim; i++)
                  {
                  if (stop_processing)
                     break;
                  row = read_next_row (fcrel.table[prim]);
                  if (set_member (i, primitives))
                     {
                     vec->points[n] = (POINT_FEATURE*)xvt_zmalloc (sizeof (POINT_FEATURE));
                     if (vec->points[n] == NULL)
                        xvt_fatal ("GET_FEATURES: Out of memory!");

                     get_point_feature (vec->points[n], row, fcrel.table[prim]);
                     vec->nr_points += 1;
                     n++;

                     sprintf (string, "%ld Found", vec->nr_points);
                     if (iUpdateStatus (StatusMessage, string) != TRUE)
                        xvt_note ("VPF2vec: Update Status failed.");
                     if (iCheckCancel() != TRUE)
                        stop_processing = TRUE;

                     if (vec->nr_points == MAXPOINTS)
                        {
                        xvt_note ("Point feature limit reached (MAXPOINTS)");
                        stop_processing = TRUE;
                        }
                     }
                  free_row (row, fcrel.table[prim]);
                  }
               break;
               }
            case FACE:
               {
               /* Must also open RNG, EDG, and FBR for drawing faces. */
               strcpy (path, covpath);
               strcat (path, tiledir);
               strcat (path, "rng");
               rngtable = vpf_open_table (path, disk, "rb", NULL);
    
               strcpy (path, covpath);
               strcat (path, tiledir);
               strcat (path, "edg");
               edgtable = vpf_open_table (path, disk, "rb", NULL);

               strcpy (path, covpath);
               strcat (path, tiledir);
               strcat (path, "fbr");
               fbrtable = vpf_open_table (path, disk, "rb", NULL);
               
               /* startprim should never be 1 i.e. face_id=1 which is the   */
               /* universal face. DCW shows face_id=1 to be a valid feature */
               /* which is incorrect.                                       */
               
               if (startprim == 1)
                  startprim = 2;


               for (i=startprim; i<=endprim; i++)
                  {
                  if (stop_processing)
                     break;
                  row = read_next_row (fcrel.table[prim]);
                  if (set_member (i, primitives))
                     {
                     vec->areas[n] = (AREA_FEATURE*)xvt_zmalloc (sizeof (AREA_FEATURE));
                     if (vec->areas[n] == NULL)
                        xvt_fatal ("GET_FEATURES: Out of memory!");

                     vec->areas[n]->id = n+1;
                     get_area_feature (vec->areas[n], i, fcrel.table[prim],
                                       rngtable, edgtable);

                     vec->nr_areas += 1;
                     n++;

                     sprintf (string, "%ld Found", vec->nr_areas);
                     if (iUpdateStatus (StatusMessage, string) != TRUE)
                        xvt_note ("VPF2vec: Update Status failed.");
                     if (iCheckCancel() != TRUE)
                        stop_processing = TRUE;

                     if (vec->nr_areas == MAXAREAS)
                        {
                        xvt_note ("Area feature limit reached (MAXAREAS)");
                        stop_processing = TRUE;
                        }
                     }
                  free_row (row, fcrel.table[prim]);
                  }
               vpf_close_table (&rngtable);
               vpf_close_table (&edgtable);
               vpf_close_table (&fbrtable);
               break;
               }
            case TEXT:
               {
               for (i=startprim; i<endprim; i++)
                  {
                  if (stop_processing)
                     break;
                  row = read_next_row (fcrel.table[prim]);
                  if (set_member (i, primitives))
                     {
                     vec->text[n] = (TEXT_FEATURE*)xvt_zmalloc (sizeof (TEXT_FEATURE));
                     if (vec->text[n] == NULL)
                        xvt_fatal ("GET_FEATURES: Out of memory!");

                     get_text_feature (vec->text[n], row, fcrel.table[prim]);
                     vec->nr_text += 1;
                     n ++;

                     sprintf (string, "%ld Found", vec->nr_text);
                     if (iUpdateStatus (StatusMessage, string) != TRUE)
                        xvt_note ("VPF2vec: Update Status failed.");
                     if (iCheckCancel() != TRUE)
                        stop_processing = TRUE;

                     if (vec->nr_text == MAXTEXT)
                        {
                        xvt_note ("Text feature limit reached (MAXTEXT)");
                        stop_processing = TRUE;
                        }
                     }
                  free_row (row, fcrel.table[prim]);
                  }
               break;
               }
            } /* switch (pclass) */

         vpf_close_table (&fcrel.table[prim]);
         set_nuke (&primitives);
         } /* Tile Loop */

      if (!finished)
         {
         status = 0;
         deselect_feature_class_relate (&fcrel);
         break;
         }
      status = 1;
      deselect_feature_class_relate (&fcrel);
     } /* relpathnum loop */


   /* Finished getting the features, now clean up */

   /* Close the Feature Table if open */
   vpf_close_table (&ft);

   if (tilecover)
      {
      vpf_close_table (&tile_table);
      }

   set_nuke (&feature_rows);


   /* Reallocate space for feature pointer addresses */
   switch (pclass)
      {
      case EDGE:
         {
         temp_lines = (LINE_FEATURE**)xvt_zmalloc
                      (vec->nr_lines * sizeof (LINE_FEATURE*));
         if (temp_lines == (LINE_FEATURE**)NULL)
            {
            xvt_fatal ("GET_FEATURES: Out of memory!");
            return (-1);
            }
         else
            {
            memcpy (temp_lines, vec->lines, (vec->nr_lines * sizeof (LINE_FEATURE*)));
            xvt_free ((char*)vec->lines);
            vec->lines = temp_lines;
            vec->nr_features = vec->nr_lines;
            }
         break;
         }
      case ENTITY_NODE:
      case CONNECTED_NODE:
         {
         temp_points = (POINT_FEATURE**)xvt_zmalloc
                      (vec->nr_points * sizeof (POINT_FEATURE*));
         if (temp_points == (POINT_FEATURE**)NULL)
            {
            xvt_fatal ("GET_FEATURES: Out of memory!");
            return (-1);
            }
         else
            {
            memcpy (temp_points, vec->points, (vec->nr_points * sizeof (POINT_FEATURE*)));
            xvt_free ((char*)vec->points);
            vec->points = temp_points;
            vec->nr_features = vec->nr_points;
            }
         break;
         }
      case FACE:
         {
         temp_areas = (AREA_FEATURE**)xvt_zmalloc
                      (vec->nr_areas * sizeof (AREA_FEATURE*));
         if (temp_areas == (AREA_FEATURE**)NULL)
            {
            xvt_fatal ("GET_FEATURES: Out of memory!");
            return (-1);
            }
         else
            {
            memcpy (temp_areas, vec->areas, (vec->nr_areas * sizeof (AREA_FEATURE*)));
            xvt_free ((char*)vec->areas);
            vec->areas = temp_areas;
            vec->nr_features = vec->nr_areas;
            }
         break;
         }
      case TEXT:
         {
         temp_text = (TEXT_FEATURE**)xvt_zmalloc
                      (vec->nr_text * sizeof (TEXT_FEATURE*));
         if (temp_text == (TEXT_FEATURE**)NULL)
            {
            xvt_fatal ("GET_FEATURES: Out of memory!");
            return (-1);
            }
         else
            {
            memcpy (temp_text, vec->text, (vec->nr_text * sizeof (TEXT_FEATURE*)));
            xvt_free ((char*)vec->text);
            vec->text = temp_text;
            vec->nr_features = vec->nr_text;
            }
         break;
         }
      } /* switch(pclass) */


   /* Destroy the Status Window */
   iDestroyStatus();

   return (0);
   }



/*****************************************************************************/
/* GET_SELECTED_TILE_PRIMITIVES                                              */
/*                                                                           */
/*   Purpose:                                                                */
/*      This function determines all of the selected primitive rows from     */
/*      the selected features of a given tile.                               */
/*                                                                           */
/*      This function expects a feature class relationship structure that    */
/*      has been successfully created with select_feature_class_relate()     */
/*      from the feature table to the primitive.  The primitive table in     */
/*      the feature class relate structure must have been successfully       */
/*      opened for the given tile.                                           */
/*   Parameters:                                                             */
/*      covpath      <input>  == (char*) path th the VPF coverage.           */
/*      fclass       <input>  == (char*) feature class.                      */
/*      fcrel        <input>  == (fcrel_type) feature class relate structure.*/
/*      primclass    <input>  == (int32) primitive class to select.           */
/*      feature_rows <input>  == (set_type) set of selected features.        */
/*      mapenv       <input>  == (map_environment_type *) map environment.   */
/*      tile         <input>  == (int32) tile number.                         */
/*      tiledir      <input>  == (char*) path to the tile directory.         */
/*      status       <output> == (int32*) status of the function:             */
/*                               1 if completed, 0 if user escape.           */
/*      return       <output> == (set_type) set of primitives for the        */
/*                               features.                                   */
/*****************************************************************************/
#ifdef PROTO
   set_type get_selected_tile_primitives (char *covpath,
                                          fcrel_type fcrel,
                                          int32 primclass,
                                          set_type feature_rows,
                                          map_environment_type *mapenv,
                                          int32 tile,
                                          char *tiledir,
                                          int32 *status)

#else
   set_type get_selected_tile_primitives (covpath, fcrel,  primclass,
                                          feature_rows, mapenv, tile,
                                          tiledir, status)
      char *covpath;
      fcrel_type fcrel;
      int32 primclass;
      set_type feature_rows;
      map_environment_type *mapenv;
      int32 tile;
      char *tiledir;
      int32 *status;
#endif

   {
   int32 feature, prim;
   row_type row;
   set_type primitive_rows;
   set_type primitives;
   int32 feature_rownum;
   register int32 i, start,end;
   linked_list_type feature_list;
   position_type p;
   extent_type pextent;

   prim = 0;
   feature = fcrel.nchain-1;

   /* Assume that fcrel.table[prim] has been opened */

   primitives.size = 0;
   primitives.buf = NULL;

   primitives = set_init(fcrel.table[prim].nrows+1);

   /* All VPF products produced so far are in Decimal Degrees */
   /* so no calls to mapextent is needed */
   pextent = mapenv->mapextent;  /*DGM*/

   /* Get the set of primitive rows within the map extent */
   primitive_rows = primitives_within_extent (pextent, covpath, tiledir,
                                              primclass,
                                              fcrel.table[prim].nrows);

   if (set_empty (primitive_rows))
      {
      set_nuke (&primitive_rows);
      return primitives;
      }

   end = set_max (primitive_rows);
   start = set_min (primitive_rows);
   if (end>0 && start<1)
      start=1;
   if (end>fcrel.table[prim].nrows)
      end = fcrel.table[prim].nrows;


   /* It turns out to be MUCH faster off of a CD-ROM to */
   /* read each row and discard unwanted ones than to   */
   /* forward seek past them.  It's about the same off  */
   /* of a hard disk.                                   */

   fseek(fcrel.table[prim].fp,index_pos(start,fcrel.table[prim]),
   SEEK_SET);

   for (i=start; i<=end; i++)
      {
      if (i>fcrel.table[prim].nrows)
         break;

      row = read_next_row (fcrel.table[prim]);
      if (!row)
         break;

      if (!set_member (i, primitive_rows))
         {
         free_row (row, fcrel.table[prim]);
         continue;
         }

      feature_list = fc_row_numbers (row, fcrel, tile);

      free_row (row, fcrel.table[prim]);

      if (!ll_empty(feature_list))
         {
         p = ll_first(feature_list);
         while (!ll_end(p))
            {
            ll_element(p,&feature_rownum);
            if ((feature_rownum < 1) ||
                (feature_rownum > fcrel.table[feature].nrows))
               {
               p = ll_next(p);
               continue;
               }
            if (set_member (feature_rownum, feature_rows))
               {
               set_insert (i, primitives);
               break;
               }
            p = ll_next (p);
            } 
         }
      if (feature_list) ll_reset (feature_list);
      }
   set_nuke(&primitive_rows);

   *status = 1;
   return primitives;
   }



/******************************************************************************/
/* PRIMITIVES_WITHIN_EXTENT                                                   */
/*                                                                            */
/*    Spatially select all of the primitives for a primitive class in a       */
/*    specified tile that are contained within the specified map extent.      */
/*    The map extent must be specified in decimal degrees.                    */
/*    Parameters:                                                             */
/*     extent   <input> == (extent_type) spatial extent in decimal degrees.   */
/*     covpath  <input> == (char*) directory path to a VPF coverage.          */
/*     tiledir  <input> == (char*) tile directory from the tileref.aft.       */
/*     primclass<input> == (int) primitive class identifier.  Must be         */
/*                         EDGE, FACE, ENTITY_NODE, CONNECTED_NODE, or TEXT.  */
/*     numprims <input> == (int32) number of rows in the associated            */
/*                         primitive table.                                   */
/*     return  <output> == (set_type) set of selected primitives.             */
/******************************************************************************/
#ifdef PROTO
   set_type primitives_within_extent (extent_type mapextent, char *covpath, 
                                     char *tiledir, int32 primclass,
                                     int32 numprims)

#else
   set_type primitives_within_extent (mapextent, covpath, tiledir, primclass, 
                                     numprims)
      extent_type mapextent;
      char *covpath;
      char *tiledir;
      int32 primclass;
      int32 numprims;
#endif

   {
   char path[255];
   BOOLEAN index_exists = FALSE;
   set_type primitive_rows;
   static char *spxname[] = {"","ESI","FSI","TSI","NSI","CSI"};
   static char *brname[] = {"","EBR","FBR","TBR","NBR","CBR"};


   primitive_rows.size = 0;
   primitive_rows.buf = (char *)NULL;

   /* 20 is a fairly arbitrary cutoff of the number of         */
   /* primitives that make a spatial index search worth while. */
   if (numprims > 20)
      {
      strcpy (path, covpath);
      strcat (path, tiledir);
      strcat (path, spxname[primclass]);
      if (muse_access (path, 0) == 0)
         {
         index_exists = TRUE;
         primitive_rows = spatial_index_search (path,
                          (float)mapextent.x1, (float)mapextent.y1,
                          (float)mapextent.x2, (float)mapextent.y2);
         }
      }

   if ((numprims <= 20) || (index_exists == FALSE))
      {
      /* Next best thing - bounding rectangle table */
      strcpy (path, covpath);
      strcat (path, tiledir);
      strcat (path, brname[primclass]);
      if ((muse_access (path,0) == 0) && (numprims > 20))
         primitive_rows = bounding_select (path, mapextent, NOPROJ);
      }

   /* if the spatial index or bounding rectangle table were found then */
   /* just turn on the entire set.                                     */
   if (primitive_rows.size == 0)
      {
      primitive_rows = set_init (numprims+1L);
      set_on (primitive_rows);
      }

   return (primitive_rows);
   }



/******************************************************************************/
/* bitfix                                                                     */
/******************************************************************************/
#ifdef PROTO
   static float bitfix (float f)
#else
   static float bitfix (f)
      float f;
#endif

   {
   int32 machine_byte_order = MACHINE_BYTE_ORDER;
   union
      {
      float f;
      unsigned char c[4];
      } fltchr;

      fltchr.f = f;
      
      if (machine_byte_order == LEAST_SIGNIFICANT)
         fltchr.c[0] = ((fltchr.c[0] >> 3) << 3);
      else
         fltchr.c[3] = ((fltchr.c[3] >> 3) << 3);

      return (fltchr.f);
      }


/******************************************************************************/
/* GET_TILE_SET                                                               */
/******************************************************************************/
#ifdef PROTO
set_type get_tile_set (char* lib_path,  extent_type extent, BOOLEAN *cancel_process)
#else
set_type get_tile_set (lib_path,  extent, cancel_process)
char* lib_path;
extent_type extent;
BOOLEAN *cancel_process;
#endif
   {
   char path[255], StatusMessage[40], string[40];
   int32 XMIN_, YMIN_, XMAX_, YMAX_, FAC_ID_;
   float delta, retval;
   int32 i, n, face_id, nr_tiles=0L;
   static char sep[2] = {DIR_SEPARATOR,'\0'};
   row_type row;
   vpf_table_type fbr, aft;
   fextent_type tile_extent, area_extent;
   set_type tile_set;

   /* Convert the double area extents to float */
   area_extent.x1 = (float)extent.x1;
   area_extent.y1 = (float)extent.y1;
   area_extent.x2 = (float)extent.x2;
   area_extent.y2 = (float)extent.y2;

   /* Initialize the set to contain 0 elements */
   tile_set = set_init (0L);

   if (area_extent.y2 > 90.0)
      {
      delta = (area_extent.y2 - 90.0);
      area_extent.y2 = 90.0;
      area_extent.y1 -= delta;
      }

/* Open the tileref.fbr table */
   strcpy (path, lib_path);
   strcat (path, "tileref");
   strcat (path, sep);
   strcat (path, "fbr");
   if (muse_access (path,0)!=0)
      return (tile_set);

   fbr = vpf_open_table (path, disk, "rb", NULL);
   if (!fbr.fp)
      {
	   vpf_close_table (&fbr);
	   return (tile_set);
      }
   XMIN_ = table_pos("XMIN",fbr);
   YMIN_ = table_pos("YMIN",fbr);
   XMAX_ = table_pos("XMAX",fbr);
   YMAX_ = table_pos("YMAX",fbr);

   /* Check for invalid fbr */
   if (XMIN_ < 0 || YMIN_ < 0 || XMAX_ < 0 || YMAX_ < 0)
      {
      vpf_close_table(&fbr);
      *cancel_process = TRUE;
      return (tile_set);
      }

   /* Initialize tile_set */
   tile_set = set_init (fbr.nrows);


   /* Open the Tileref.aft table */
   strcpy (path, lib_path);
   strcat (path, "tileref");
   strcat (path, sep);
   strcat (path, "tileref.aft");
   aft = vpf_open_table (path, disk, "rb", NULL);
   if (!aft.fp)
      {
      vpf_close_table (&fbr);
      vpf_close_table (&aft);
      return (tile_set);
      }

   FAC_ID_ = table_pos ("FAC_ID", aft);
   
   for (i=1; i<=aft.nrows; i++)
      {
	   if (FAC_ID_ >= 0)
	      {
	      /* Uses the MIL-STD-2407 (26 May 1993) style of
	      the tile reference coverage with a FAC_ID */
	      row = read_next_row (aft);
	      get_table_element (FAC_ID_, row, aft, &face_id, &n);
         free_row (row, aft);
         }
      else
         {
         /* Assume the library use the MIL-STD-600006
         (before 26 May 1993) style of the tile reference
         coverage, which assumes a 1:1 relationship between
         the TILEREF.AFT and the FAC table, and discards
         the universe face 1. */
         face_id = i;
         }

      /* Ignore the Universal Face (face_id = 1) */
      if (face_id == 1)
         continue;

      row = read_row (face_id, fbr);

      get_table_element (XMIN_, row, fbr, &retval, &n);
      tile_extent.x1 = bitfix (retval);
      get_table_element (YMIN_, row, fbr, &retval, &n);
      tile_extent.y1 = retval;
      get_table_element (XMAX_, row, fbr, &retval, &n);
      tile_extent.x2 = retval;
      get_table_element (YMAX_, row, fbr, &retval, &n);
      tile_extent.y2 = retval;

      free_row (row, fbr);

      if (geo_intersect (area_extent, tile_extent))
         {
         set_insert (i, tile_set);
         nr_tiles++;
         }

      /* Update Status Window */
      sprintf (StatusMessage, "TILE SEARCH");
      sprintf (string, "%ld Found", nr_tiles);
      if (iUpdateStatus (StatusMessage, string) != TRUE)
         xvt_note ("VPF2vec: Update Status failed.");
      if (iCheckCancel() != TRUE)
         {
         set_nuke (&tile_set);
         vpf_close_table (&fbr);
         vpf_close_table (&aft);
         *cancel_process = TRUE;
         return (tile_set);
         }

      }
   vpf_close_table (&fbr);
   vpf_close_table (&aft);

   return (tile_set);
   }


/*****************************************************************************/
/*  get_fit_primitives                                                       */
/*                                                                           */
/*   Purpose:                                                                */
/*    Get the set of primitives in the given tile for the selected features  */
/*    in the given feature class.  Use the Feature Index Table instead of    */
/*    the schema relationships.                                              */
/*   Parameters:                                                             */
/*     covpath       <input>==(char*) path to the VPF coverage.              */
/*     primclass     <input>==(int) primitive class to select.               */
/*     fset          <input> == (set_type*) set of feature IDs that meet the */
/*                              thematic conditions.                         */
/*     sprims        <input> == (set_type*) set of primitive IDs that meet   */
/*                              the spatial conditions.                      */
/*     tile          <input>==(int32) tile number.                        */
/*     fca_id        <input>==(int) Feature Class Attribute table id of the  */
/*                              selected feature class.                      */
/*     numprims      <input>==(int) number of rows in the specified tile's   */
/*                              primitive table for the specified primitive  */
/*                              class.                                       */
/*     status   <output>==(int *) status of the function:                    */
/*                         1 if completed, 0 if user escape.                 */
/*     return   <output>==(set_type) set of primitives for the features      */
/*                                                                           */
/*   Modified 21 June 94  DGM                                                */
/*****************************************************************************/
#ifdef PROTO
   set_type get_fit_primitives (char *covpath, int32 primclass, set_type fset,
				                    set_type sprims, int32 tile, int32 fca_id,
                                int32 numprims, BOOLEAN *stop_processing)
#else
   set_type get_fit_primitives (covpath, primclass, fset, sprims, tile,
                                fca_id, numprims, stop_processing)
      char *covpath;
      int32 primclass;
      set_type fset;
      set_type sprims;
      int32 tile, fca_id, numprims;
      BOOLEAN *stop_processing;
#endif

   {
   set_type primitives, tileset, fcset, selset;
   int32 i, start, end, prim_id, tile_id, fc_id, feature_id, count;
   short short_tile_id;
   int32 PRIM_ID_, TILE_ID_, FC_ID_, FEATURE_ID_;
   vpf_table_type fit;
   row_type row;
   char path[255], StatusMessage[40], string[40];
   static char *ptable[] = {"","EDG","FAC","TXT","END","CND"};

   /* Initialize sets */
   primitives = set_init (numprims+1);

   /* Open the FIT */
   strcpy (path, covpath);
   strcat (path, ptable[primclass]);
   strcat (path, ".FIT");
   muse_check_path (path);

   if (muse_access (path,0) != 0)
      return primitives;

   fit = vpf_open_table (path, disk, "rb", NULL);
   if (!fit.fp)
      return primitives;

   /* Get the column positions of tile_id, primitive_id, feature_class_id, */
   /* feature_id.                                                          */
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


   /* Get the set of all FIT rows in the search tile that match the */
   /* search fca_id.                                                */
   selset = set_intersection (tileset, fcset);
   set_nuke (&tileset);
   set_nuke (&fcset);

   if (set_empty (selset))
      {
      vpf_close_table (&fit);
      set_nuke (&selset);
      return (primitives);
      }

   /* Now loop through the FIT and get the matching primitive ids */
   start = set_min (selset);
   end = set_max (selset);

   /* Set file pointer to start record */
   fseek (fit.fp, index_pos (start, fit), SEEK_SET);
   for (i=start; i<=end; i++)
      {

      if ((i % 10) == 0)
         {
         /* Update Status Window */
         sprintf (StatusMessage, "Retrieve Features");
         sprintf (string, "Searching Tile Nr. %ld", tile);
         if (iUpdateStatus (StatusMessage, string) != TRUE)
            xvt_note ("GET_FIT_PRIMITIVES: Update Status failed.");
         if (iCheckCancel() != TRUE)
            {
            vpf_close_table (&fit);
            set_nuke (&selset);
            *stop_processing = TRUE;
            return (primitives);
            }
         }

      /* Read each row of the fit starting at "start". If the row is a member */
      /* of selset then get the tile_id.                                      */
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
         

         if (tile_id == tile  &&  fc_id == fca_id &&
            set_member (prim_id, sprims) && set_member (feature_id, fset))
            {
            set_insert (prim_id, primitives);
            }
         }  /* if set_member */
      free_row (row, fit);
      } /* for i */

   vpf_close_table (&fit);
   set_nuke (&selset);

   return (primitives);
   }



