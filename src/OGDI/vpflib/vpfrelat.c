
/***************************************************************************
 *
 *  Module VPFRELAT.C
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *
 *    This module contains functions supporting relates between VPF
 *    feature classes and primitives (and vice versa).  It relies
 *    upon the information provided by the Feature Class Schema table.
 *    This table is used to generate a feature class relationship (fcrel)
 *    data structure for a feature class.  This structure contains all
 *    of the tables and their primary and foreign keys for the
 *    relationships between a feature table and its primitive, or
 *    from a primitive to its feature table (each relate chain is one way).
 *    This module tries to be as much of a black box as it can to
 *    enable a programmer to simply return the corresponding primitive
 *    row of a feature record, or the corresponding feature row of a
 *    primitive record.
 *
 *    This is one of the most difficult modules required to support
 *    a truly 'generic' VPF application, since VPF allows so many
 *    variations of feature-primitive relationships.  The final version
 *    of this module must support every allowed relationship.
 **************************************************************************/
#ifndef INCL_XVTH
#include <xvt.h>
#endif

#ifndef __SET_H__
#include "set.h"
#endif
#ifndef _VPFTABLE_H_
#include "vpftable.h"
#endif
#ifndef __VPFVIEW_H__
#include "vpfview.h"
#endif
#ifndef __VPFRELAT_H__
#include "vpfrelat.h"
#endif
#ifndef __VPFTIDX_H__
#include "vpftidx.h"
#endif
#ifndef __VPFQUERY_H__
#include "vpfquery.h"
#endif
#ifndef __STRFUNC_H__
#include "strfunc.h"  
#endif
#ifndef __VPFPROP_H__
#include "vpfprop.h"   
#endif
#ifndef H_MUSEDIR
#include "musedir.h" 
#endif
#ifndef _MACHINE_
#include "machine.h"
#endif


/* Determine if the given table name is in the given list of */
/* vpf relate structures.                    */
#ifdef PROTO
   int32 table_in_list (char *tablename, linked_list_type rlist)
#else
   int32 table_in_list (tablename, rlist)
      char *tablename;
      linked_list_type rlist;
#endif

   {
   position_type p;
   vpf_relate_struct rcell;

   p = ll_first (rlist);
   while (!ll_end (p))
      {
      ll_element (p, &rcell);
      if (strcmp(rcell.table1, tablename) == 0)
          return 1;
      p = ll_next (p);
      }
   return 0;
   }


/**************************************************************************
 *
 *N  num_relate_paths
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Find the number of possible relate paths for the feature class
 *    from the start table.  (Complex features can have several relates
 *    to a single primitive table.)
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    start_table  <input> == (char *) table to start from.
 *    fcname       <input> == (char *) feature class name.
 *    fcs          <input> == (vpf_table_type) feature class schema table.
 *    num_relate_paths<output> == (int32) number of relate paths found.
 *E
*************************************************************************/
#ifdef PROTO
int32 num_relate_paths( char *start_table,
                      char *fcname, 
                      vpf_table_type fcs )
#else
int32 num_relate_paths( start_table, fcname, fcs )
char *start_table;
char *fcname;
vpf_table_type fcs;
#endif
{
   set_type fcset;
   int32 n;
   char qstr[80];

   sprintf(qstr,"FEATURE_CLASS = %s AND TABLE1 = %s",
           fcname,start_table);
   fcset = query_table(qstr,fcs);
   n = (int32) num_in_set (fcset);
   set_nuke(&fcset);
   return n;
}


/**************************************************************************
 *
 *N  fcs_relate_list
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Read the feature class schema table and create the list of
 *    tables to chain through.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    fcname       <input> == (char *) feature class name.
 *    start_table  <input> == (char *) table to start from.
 *    end_table    <input> == (char *) table to end with.
 *    fcs          <input> == (vpf_table_type) feature class schema table.
 *    npath    <input> == (int32) relate path number.
 *    fcs_relate_list <output> == (linked_list_type) list of tables to
 *                                chain through.
 *************************************************************************/
#ifdef PROTO
linked_list_type fcs_relate_list( char *fcname, char *start_table,
                  char *end_table, vpf_table_type fcs,
                  int32 npath )
#else
linked_list_type fcs_relate_list( fcname, start_table, end_table, fcs, npath )
char *fcname;
char *start_table;
char *end_table;
vpf_table_type fcs;
int32 npath;
#endif
{
   linked_list_type rlist;
   vpf_relate_struct rstruct;
   set_type fcset;
   char tablename[255], *buf, expr[255];
   row_type row;
   int32 i, rownum, n;
   int32 TABLE1_, KEY1_, TABLE2_, KEY2_;
   char prevstr[80];

   rlist = ll_init();

   sprintf(expr,"FEATURE_CLASS = %s AND TABLE1 = %s",fcname,start_table);

   fcset = query_table(expr,fcs);

   if (set_empty(fcset)) {
      set_nuke(&fcset);
      return rlist;
   }

   TABLE1_ = table_pos("TABLE1",fcs);
   KEY1_ = table_pos("FOREIGN_KEY",fcs);
   if (KEY1_ < 0) {
      KEY1_ = table_pos("TABLE1_KEY",fcs);
   }
   TABLE2_ = table_pos("TABLE2",fcs);
   KEY2_ = table_pos("PRIMARY_KEY",fcs);
   if (KEY2_ < 0) {
      KEY2_ = table_pos("TABLE2_KEY",fcs);
   }

   /* Get to the relate path number */
   n = -1;
   rownum = 0;
   for (i=1; i<fcs.nrows; i++)
      {
      if (set_member (i, fcset))
         {
         rownum = i;
         n++;
         if (n>=npath) break;
        }
      }
   if (n<npath) rownum = set_max(fcset);

   set_nuke(&fcset);

   row = get_row(rownum,fcs);

   buf = (char *)get_table_element(TABLE1_,row,fcs,NULL,&n);
   strcpy(rstruct.table1,buf);
   rightjust(rstruct.table1);
   xvt_free(buf);

   buf = (char *)get_table_element(KEY1_,row,fcs,NULL,&n);
   strcpy(rstruct.key1,buf);
   rightjust(rstruct.key1);
   xvt_free(buf);

   buf = (char *)get_table_element(TABLE2_,row,fcs,NULL,&n);
   strcpy(rstruct.table2,buf);
   rightjust(rstruct.table2);
   xvt_free(buf);

   buf = (char*)get_table_element (KEY2_, row, fcs, NULL, &n);
   strcpy (rstruct.key2,buf);
   rightjust (rstruct.key2);
   xvt_free (buf);

   free_row( row, fcs );

   ll_insert( &rstruct, sizeof(rstruct), ll_last(rlist) );

   strcpy( tablename, rstruct.table2 );
   strcpy( prevstr, rstruct.table1 );
#ifdef _MAC /* REM */
   while ( strcmpi(tablename,end_table) != 0) {
#elif defined(_WINDOWS)
   while ( stricmp(tablename,end_table) != 0) {
#else
     while (strcasecmp(tablename, end_table) != 0) {
#endif
      sprintf(expr,"FEATURE_CLASS = %s AND TABLE1 = %s AND TABLE2 <> %s",
              fcname,tablename,prevstr);

      fcset = query_table(expr,fcs);
      if (set_empty(fcset)) {
     set_nuke(&fcset);
     return rlist;
      }
      rownum = set_min(fcset);

      set_nuke(&fcset);

      row = get_row(rownum,fcs);

      buf = (char *)get_table_element(TABLE1_,row,fcs,NULL,&n);
      strcpy(rstruct.table1,buf);
      rightjust(rstruct.table1);
      xvt_free(buf);

      buf = (char *)get_table_element(KEY1_,row,fcs,NULL,&n);
      strcpy(rstruct.key1,buf);
      rightjust(rstruct.key1);
      xvt_free(buf);

      buf = (char *)get_table_element(TABLE2_,row,fcs,NULL,&n);
      strcpy(rstruct.table2,buf);
      rightjust(rstruct.table2);
      xvt_free(buf);

      buf = (char *)get_table_element(KEY2_,row,fcs,NULL,&n);
      strcpy(rstruct.key2,buf);
      rightjust(rstruct.key2);
      xvt_free(buf);

      free_row( row, fcs );

      if (table_in_list(rstruct.table1, rlist)) break;

      ll_insert( &rstruct, sizeof(rstruct), ll_last(rlist) );

      strcpy( tablename, rstruct.table2 );
      strcpy( prevstr, rstruct.table1 );
   }

   return rlist;
}




/**************************************************************************
 *
 *N  related_row
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Return the related row of table2 based upon the value of table 1's key
 *    Table 2 must be the '1' side of an n:1 relationship  --  If it isn't,
 *    use 'related_rows()'.
 *    Supported data types - I, S, K, and T<n>.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    keyval1  <input> == (void *) key value from table 1 in the relate.
 *    table2   <input> == (vpf_table_type) table 2 in the relate.
 *    key2     <input> == (char *) key column name in table 2.
 *    return  <output> == (int32) first related row number in table 2.
 *                        0 is returned if no related rows are found.
 *************************************************************************/
#ifdef PROTO
int32 related_row( void *keyval1,
              vpf_table_type table2, char *key2,
              int32 tile_id )
#else
int32 related_row( keyval1, table2, key2, tile_id )
void *keyval1;
vpf_table_type table2;
char *key2;
int32 tile_id;
#endif
{
   int32 rowid, i, ival, n, tile, start,end;
   short int sval;
   row_type row;
   int32 KEY2_,TILE_;
   char cval, *tval, path[255], *keystring;
   set_type idxset, tileset, searchset;
   id_triplet_type idtrip;

   if (strcmp(key2,"ID")==0) {
      memcpy( &rowid, keyval1, sizeof(rowid) );
      return rowid;
   }

   rowid = 0;

   KEY2_ = table_pos(key2,table2);

   if ((table2.header[KEY2_].type != 'I')&&
       (table2.header[KEY2_].type != 'S')&& 
       (table2.header[KEY2_].type != 'K')&& 
       (table2.header[KEY2_].type != 'T')) return rowid;

   if ((table2.header[KEY2_].type != 'T')&&
       (table2.header[KEY2_].count != 1)) return rowid;

   if (tile_id > 0)
      TILE_ = table_pos("TILE_ID",table2);
   else
      TILE_ = -1;

   idxset.size = 0;
   if (table2.header[KEY2_].tdx)
      {
      strcpy (path, table2.path);
      rightjust (path);
      /* Remove the filename from the path */
      i = strlen (path);
      while (path[i] != DIR_SEPARATOR)
         i--;
      path[i+1] = '\0';
      strcat (path, table2.header[KEY2_].tdx);

      if (muse_access (path, 4) == 0)
         {
         idxset = read_thematic_index(path, (char*)keyval1);
         if (TILE_ < 0)
            {
            /* Don't bother checking TILE_ID. */
            /* Take the first value in the set. */
            i = set_min (idxset);
            if (i>table2.nrows || i<0)
               i = 0;
            set_nuke(&idxset);
            return i;
            }
         }
      }
   if (idxset.size == 0)
      {
      idxset = set_init (table2.nrows);
      set_on (idxset);
      }

   tileset.size = 0;
   if (TILE_  >=  0)
      {
      if (table2.header[TILE_].tdx)
         {
         strcpy (path, table2.path);
         rightjust (path);
         /* Remove the filename from the path */
         i = strlen (path);
         while (path[i] != DIR_SEPARATOR)
            i--;
         path[i+1] = '\0';
         strcat (path, table2.header[TILE_].tdx);

         if (muse_access( path, 4) == 0)
            {
            tile = tile_id;
            if (table2.header[TILE_].type == 'S')
               {
               sval = (short)tile;
               tileset = read_thematic_index(path,(char *)&sval);
               }
            else if (table2.header[TILE_].type == 'I')
               {
               tileset = read_thematic_index (path, (char*)&tile);
               }
            }
         }
      }
   if (tileset.size == 0)
      {
      tileset = set_init (table2.nrows);
      set_on (tileset);
      }

   searchset = set_intersection (tileset, idxset);
   set_nuke (&tileset);
   set_nuke (&idxset);

   if (table2.header[KEY2_].type == 'T')
      {
      keystring = (char*)xvt_malloc (strlen((char*)keyval1));
      strcpy (keystring, (char*)keyval1);

      rightjust(keystring);
      }
   else
      {
      keystring = NULL;
      }

   start = set_min(searchset);
   end = set_max(searchset);
   if (start < 1) start = 1;
   if (end > table2.nrows) end = table2.nrows;

   for (i=start;i<=end;i++) {
      if (!set_member(i,searchset)) continue;
      row = get_row(i,table2);

      if (TILE_>0) {
     tile = tile_id;
     if (table2.header[TILE_].type == 'S') {
        get_table_element(TILE_,row,table2,&sval,&n);
        tile = sval;
     } else if (table2.header[TILE_].type == 'I') {
        get_table_element(TILE_,row,table2,&tile,&n);
     }
     if (tile != tile_id) {
        free_row(row,table2);
        continue;
     }
      }

      if (table2.header[KEY2_].type == 'I') {
     get_table_element(KEY2_,row,table2,&ival,&n);
     if (memcmp(&ival,keyval1,sizeof(ival))==0) rowid = i;
      } else if (table2.header[KEY2_].type == 'S') {
     get_table_element(KEY2_,row,table2,&sval,&n);
     ival = (int32)sval;
     if (memcmp(&ival,keyval1,sizeof(ival))==0) rowid = i;
      } else if (table2.header[KEY2_].type == 'K') {
     get_table_element(KEY2_,row,table2,&idtrip,&n);
     ival = (int32)idtrip.exid;
     if (memcmp(&ival,keyval1,sizeof(ival))==0) rowid = i;
      } else if (table2.header[KEY2_].type == 'T') {
     if (table2.header[KEY2_].count==1) {
        get_table_element(KEY2_,row,table2,&cval,&n);
        if (memcmp(&cval,keyval1,sizeof(ival))==0) rowid = i;
     } else {
        tval = get_table_element(KEY2_,row,table2,NULL,&n);
            rightjust(tval);
#ifdef _MAC /*REM*/
        if (strcmpi(tval,keystring)==0) rowid = i;
#elif defined(_WINDOWS)
        if (stricmp(tval,keystring)==0) rowid = i;
#else 
        if (strcasecmp(tval,keystring)==0) rowid = i;	
#endif
     }
      }
      free_row(row,table2);
      if (rowid > 0) break;

   }

   set_nuke(&searchset);

   if (keystring) xvt_free(keystring);

   return rowid;
}


/**************************************************************************
 *
 *N  related_rows
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Return the list of related rows of table2 based upon the value of
 *    table 1's key.
 *    Supported data types - I, S, K, and T<n>.
 *    Thematic index used, if present on key column.
 *    NOTE: A sequential search operation will search the entire
 *          table if no thematic index ...zzz...
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    keyval1  <input> == (void *) key value from table 1 in the relate.
 *    table2   <input> == (vpf_table_type) table 2 in the relate.
 *    key2     <input> == (char *) key column name in table 2.
 *    return  <output> == (linked_list_type) list of (int32) related rows
 *                        in table 2.  If no related rows are found, the
 *                        returned list is empty.
 *************************************************************************/
#ifdef PROTO
   linked_list_type related_rows (void *keyval1, vpf_table_type table2, char *key2,
                                  int32 tile_id)
#else
   linked_list_type related_rows (keyval1, table2, key2, tile_id)
      void *keyval1;
      vpf_table_type table2;
      char *key2;
      int32 tile_id;
#endif

   {
   linked_list_type rowlist;
   set_type idxset, tileset, searchset;
   int32 rowid, i, ival, n, start,end, tile;
   short short_tile, sval;
   row_type row;
   int32 KEY2_, TILE_;
   char cval, *tval, path[255], *keystring;
   id_triplet_type idtrip = {' ',0,0,0};

   rowlist = ll_init ();

   if (strcmp (key2, "ID") == 0)
      {
      memcpy (&rowid, keyval1, sizeof (rowid));
      ll_insert (&rowid, sizeof (rowid), rowlist);
      return rowlist;
      }

   KEY2_ = table_pos (key2, table2);

   if ((table2.header[KEY2_].type != 'I') &&
       (table2.header[KEY2_].type != 'S') &&
       (table2.header[KEY2_].type != 'K') &&
       (table2.header[KEY2_].type != 'T')) return rowlist;

   if ((table2.header[KEY2_].type == 'I') &&
       (table2.header[KEY2_].count != 1)) return rowlist;

   if (tile_id > 0)
      TILE_ = table_pos ("TILE_ID", table2);
   else
      TILE_ = -1;

   idxset.size = 0;

   /* Use thematic index for column KEY2_ if one exists */
   if (table2.header[KEY2_].tdx)
      {
      strcpy (path, table2.path);
      rightjust (path);

      /* Remove the filename from the path */
      i = strlen (path);
      while (path[i] != DIR_SEPARATOR)
         i--;
      path[i+1] = '\0';

      strcat (path, table2.header[KEY2_].tdx);

      if (muse_access(path,4)==0)
         {
         idxset = read_thematic_index(path,(char *)keyval1);
         if (TILE_ < 0)
            {
            /* don't bother to check TILE_ID for a match */
            start = set_min(idxset);
            end = set_max(idxset);
            for (i=start;i<end;i++)
               if (set_member(i,idxset))
                  ll_insert(&i,sizeof(i),ll_last(rowlist));
               set_nuke(&idxset);
            return rowlist;
            }
         }
      }
   if (idxset.size == 0)
      {
      idxset = set_init(table2.nrows);
      set_on(idxset);
      }

   /* Tiled Data */
   tileset.size = 0;
   if (TILE_ >= 0)
      {
      /* Use thematic index for TILE_ column if it exists */
      if (table2.header[TILE_].tdx)
         {
         strcpy(path,table2.path);
         rightjust(path);

         /* Remove the filename from the path */
         i = strlen (path);
         while (path[i] != DIR_SEPARATOR)
            i--;
         path[i+1] = '\0';
         strcat (path, table2.header[TILE_].tdx);

         if (muse_access(path,4)==0)
            {
            tile = tile_id;
            if (table2.header[TILE_].type == 'S')
               {
               sval = (short int)tile;
               tileset = read_thematic_index(path,(char *)&sval);
               }
            else if (table2.header[TILE_].type == 'I')
               {
               tileset = read_thematic_index(path,(char *)&tile);
               }
            }
         }
      }

   if (tileset.size == 0)
      {
      tileset = set_init (table2.nrows);
      set_on (tileset);
      }

   searchset = set_intersection (tileset, idxset);
   set_nuke (&tileset);
   set_nuke (&idxset);

   if (table2.header[KEY2_].type == 'T')
      {
      keystring = (char*)xvt_malloc (strlen ((char*)keyval1));
      strcpy (keystring, (char*)keyval1);

      rightjust(keystring);
      }
   else
      {
      keystring = NULL;
      }

   start = set_min(searchset);
   end = set_max(searchset);
   if (start < 1) start = 1;
   if (end > table2.nrows) end = table2.nrows;

   for (i=start;i<=end;i++)
      {
      if (set_member(i,searchset))
         {
         row = get_row(i,table2);

         if (TILE_>0)
            {
            tile = tile_id;
            if (table2.header[TILE_].type == 'S')
               {
               get_table_element(TILE_,row,table2,&short_tile,&n);
               tile = short_tile;
               }
            else if (table2.header[TILE_].type == 'I')
               {
               get_table_element(TILE_,row,table2,&tile,&n);
               }
            if (tile != tile_id)
               {
               free_row(row,table2);
               continue;
               }
            }

         if (table2.header[KEY2_].type == 'I')
            {
            get_table_element(KEY2_,row,table2,&ival,&n);
            if (memcmp(&ival,keyval1,sizeof(ival))==0)
               ll_insert(&i,sizeof(i),ll_last(rowlist));
            }
         else if (table2.header[KEY2_].type == 'S')
            {
            get_table_element(KEY2_,row,table2,&sval,&n);
            if (memcmp(&sval,keyval1,sizeof(sval))==0)
               ll_insert(&i,sizeof(i),ll_last(rowlist));
            }
         else if (table2.header[KEY2_].type == 'K')
            {
            get_table_element(KEY2_,row,table2,&sval,&n);
            ival = idtrip.exid;
            if (memcmp(&ival,keyval1,sizeof(ival))==0)
               ll_insert(&i,sizeof(i),ll_last(rowlist));
            }
         else if (table2.header[KEY2_].type == 'T')
            {
            if (table2.header[KEY2_].count==1)
               {
               get_table_element(KEY2_,row,table2,&cval,&n);
               if (memcmp(&cval,keyval1,sizeof(ival))==0)
                  ll_insert(&i,sizeof(i),ll_last(rowlist));
               }
            else
               {
               tval = get_table_element(KEY2_,row,table2,NULL,&n);
               rightjust(tval);
#ifdef _MAC /*REM*/
               if (strcmpi(tval,keystring)==0)
#elif defined(_WINDOWS)
               if (stricmp(tval,keystring)==0)
#else
               if (strcasecmp(tval,keystring)==0)
#endif
                  ll_insert(&i,sizeof(i),ll_last(rowlist));
               }
            }
         free_row(row,table2);
         }
      }
   set_nuke (&searchset);

   return rowlist;
   }


/**************************************************************************
 *
 *N  select_feature_class_relate
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Set up the relationships between features and primitives or between
 *    primitives and features (one way only) for a specified feature class.
 *************************************************************************/
#ifdef PROTO
fcrel_type select_feature_class_relate( char *covpath, char *fcname,
                               char *start_table, char *end_table, int32 npath)
#else
fcrel_type select_feature_class_relate( covpath, fcname, start_table,
    end_table, npath)
char *covpath;
char *fcname;
char *start_table;
char *end_table;
int32 npath;
#endif
{
   storage_type storage;
   vpf_table_type fcs;
   int32 i;
   char *path;
   position_type p;
   vpf_relate_struct rcell;
   fcrel_type fcrel;

   path = (char*)xvt_zmalloc (255 * sizeof (char));
   fcrel.nchain = 0;
   fcrel.table = NULL;
   fcrel.relate_list = NULL;

   rightjust(covpath);
   sprintf( path, "%sfcs", covpath );
   /* Feature Class Schema table */
   fcs = vpf_open_table (path, disk, "rb", NULL );
   if (!fcs.fp)
      {
      xvt_note ("select_feature_class_relate: Error opening %s\n",path);
      return fcrel;
      }

   fcrel.relate_list = fcs_relate_list (fcname, start_table, end_table,
                                                           fcs, npath);

   if (ll_empty (fcrel.relate_list))
      {
      ll_reset (fcrel.relate_list);
      xvt_note ("ERROR in feature class relationship!");
      return fcrel;
      }

   /* Find the number of tables in the relate chain */
   p = ll_first (fcrel.relate_list);
   fcrel.nchain = 0;
   while (!ll_end(p))
      {
      fcrel.nchain++;
      p = ll_next (p);
      }
   /* Allow for last table2 */
   fcrel.nchain++;

   fcrel.table = (vpf_table_type*)
          xvt_malloc((size_t)(fcrel.nchain + 1) * sizeof(vpf_table_type));
   if (!fcrel.table) 
      {
      xvt_note ("Out of memory in select_feature_class_relate\n");
      exit(1);
      }

   for (i=0; i<fcrel.nchain+1; i++)
      {
      vpf_nullify_table (&(fcrel.table[i]));
      }


   p = ll_first (fcrel.relate_list);
   for (i=0; i<fcrel.nchain-1; i++) 
      {
      ll_element (p, &rcell);

      /** Can't open primitive table - may be several under tile **/
      /** directories.  Open all others **/
      if (!is_primitive (rcell.table1))
         {
         strcpy (path, covpath);
         strcat (path, rcell.table1);

         if (is_join(rcell.table1))
            storage = ram;
         else
            storage = disk;
     
      
         fcrel.table[i] = vpf_open_table (path, storage, "rb", (char*)NULL);
         }
      if (!ll_end(p))
         p = ll_next(p);
      }

   /* End of relate chain */
   i = fcrel.nchain-1;
   if (!is_primitive (rcell.table2))
      {
      strcpy (path, covpath);
      strcat (path, rcell.table2);
      storage = disk;
      fcrel.table[i] = vpf_open_table (path, storage, "rb", (char*)NULL);
      }

   vpf_close_table( &fcs );
   if (path)
      xvt_free (path);

   return fcrel;
}


/**************************************************************************
 *
 *N  fc_row_number
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Given the starting row of a feature class relationship, return the
 *    row number of the table at the end of the feature class relate
 *    chain.
 *    If your relate goes from the feature to the primitive, this will
 *    return the primitive id for the given feature row.
 *    If your relate goes from the primitive to the feature, this will
 *    return the feature id of the given primitive row.
 *
 *    Currently only supports relates on 'I', 'S', or 'K' fields.
 *************************************************************************/
#ifdef PROTO
int32 fc_row_number( row_type row, fcrel_type fcrel, int32 tile )
#else
int32 fc_row_number( row, fcrel, tile )
row_type row;
fcrel_type fcrel;
int32 tile;
#endif
{
   row_type relrow;
   int32 count;
   int32 i, rownum, keyval;
   short int sval;
   id_triplet_type triplet_keyval;
   int32 KEY1_, KEY_;
   position_type p;
   vpf_relate_struct rcell;

   p = ll_first(fcrel.relate_list);
   ll_element(p,&rcell);
   KEY1_ = table_pos(rcell.key1,fcrel.table[0]);

   get_table_element(0,row,fcrel.table[0],&rownum,&count);

   if (KEY1_ == 0)  /* "ID" */
      {
      keyval = rownum;
      }
   else
      {
      switch (fcrel.table[0].header[KEY1_].type)
         {
         case 'I':
            get_table_element(KEY1_,row,fcrel.table[0],&keyval,&count);
            break;
         case 'S':
            get_table_element(KEY1_,row,fcrel.table[0],&sval,&count);
            keyval = sval;
            break;
         case 'K':
            get_table_element(KEY1_,row,fcrel.table[0],&triplet_keyval,
                  &count);
            keyval = triplet_keyval.exid;
            if (tile != triplet_keyval.tile)
               {
               return -2;
               }
            break;
         default:
            keyval = 0;
            break;
         }
      }

   p = ll_first(fcrel.relate_list);
   for (i=1;i<(fcrel.nchain-1);i++) {
      /* Relate through Join table(s) */
      rownum = related_row (&keyval,fcrel.table[i],rcell.key2,tile);
      if (rownum < 1) break;
      relrow = get_row(rownum,fcrel.table[i]);

      p = ll_next(p);
      ll_element(p,&rcell);
      KEY_ = table_pos(rcell.key1,fcrel.table[i]);

      if (KEY_ == 0) {     /* "ID" */
     keyval = rownum;
      } else {
     switch (fcrel.table[i].header[KEY_].type) {
     case 'I':
        get_table_element(KEY_,relrow,fcrel.table[i],&keyval,&count);
        break;
     case 'S':
        get_table_element(KEY_,relrow,fcrel.table[i],&sval,&count);
            keyval = sval;
        break;
     case 'K':
        get_table_element(KEY_,relrow,fcrel.table[i],&triplet_keyval,
                  &count);
        keyval = triplet_keyval.exid;
        if (tile != triplet_keyval.tile) {
           return -2;
        }
        break;
     default:
        keyval = 0;
        break;
     }
      }

      free_row(relrow,fcrel.table[i]);
   }

   if (rownum < 1) return 0;

   if (strcmp(rcell.key2,"ID")==0)
      rownum = keyval;
   else
      rownum = related_row(&keyval,fcrel.table[i],rcell.key2,tile);

   return rownum;
}


/**************************************************************************
 *
 *N  fc_row_numbers
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Given the starting row of a feature class relationship, return the
 *    list of row numbers of the table at the end of the feature class
 *    relate chain.
 *    If your relate goes from the feature to the primitive, this will
 *    return the primitive ids for the given feature row.
 *    If your relate goes from the primitive to the feature, this will
 *    return the feature ids of the given primitive row.
 *
 *    Currently only supports relates on 'I', 'S', or 'K' fields.
 *************************************************************************/
#ifdef PROTO
linked_list_type fc_row_numbers (row_type row, fcrel_type fcrel,
                                 int32 tile)
#else
linked_list_type fc_row_numbers (row, fcrel, tile)
row_type row;
fcrel_type fcrel;
int32 tile;
#endif
   {
   row_type relrow;
   int32 count;
   int32 n, rownum, keyval;
   id_triplet_type triplet_keyval;
   int32 KEY1_, KEY_;
   position_type p, prow, pkey;
   vpf_relate_struct rcell;
   linked_list_type rowlist, keylist, templist;

   p = ll_first (fcrel.relate_list);
   ll_element (p,&rcell);
   KEY1_ = table_pos (rcell.key1, fcrel.table[0]);

   get_table_element (0L, row, fcrel.table[0], &rownum, &count);

   if (KEY1_ == 0)
      {
      /* "ID" */
      keyval = rownum;
      }
   else
      {
      switch (fcrel.table[0].header[KEY1_].type)
         {
         case 'I':
            get_table_element (KEY1_, row, fcrel.table[0], &keyval, &count);
            break;
         case 'K':
            get_table_element (KEY1_, row, fcrel.table[0], &triplet_keyval,
                                                                   &count);
            keyval = triplet_keyval.exid;
            if (tile != triplet_keyval.tile)
               {
               keyval = -2;
               }
            break;
         default:
            keyval = 0;
            break;
         }
      }

   keylist = ll_init();
   ll_insert (&keyval, sizeof (keyval), keylist);

   n = 0;

   p = ll_first (fcrel.relate_list);
   for (n=1; n<(fcrel.nchain-1); n++)
     {
     /* Relate through Join table(s) */
     rowlist = ll_init ();
     pkey = ll_first (keylist);
     while (!ll_end (pkey))
        {
        ll_element (pkey, &keyval);
        templist = related_rows (&keyval, fcrel.table[n], rcell.key2, tile);
        prow = ll_first (templist);
        while (!ll_end (prow)) 
           {
           ll_element (prow, &rownum);
           if (!ll_locate (&rownum, rowlist))
              ll_insert (&rownum, sizeof (rownum), ll_last (rowlist));
           prow = ll_next(prow);
           }
        ll_reset (templist);
        pkey = ll_next (pkey);
        }
     ll_reset (keylist);

     p = ll_next (p);
     ll_element (p, &rcell);
     KEY_ = table_pos (rcell.key1, fcrel.table[n]);

     keylist = ll_init ();
     if (ll_empty (rowlist))
        break;
     prow = ll_first (rowlist);
     while (!ll_end (prow))
        {
        ll_element (prow, &rownum);
        relrow = get_row (rownum, fcrel.table[n]);

        if (KEY_ == 0)
           {
           /* "ID" */
           keyval = rownum;
           }
        else
           {
           switch (fcrel.table[n].header[KEY_].type)
              {
              case 'I':
                 get_table_element(KEY_,relrow,fcrel.table[n],&keyval,&count);
                 break;
              case 'K':
                 get_table_element(KEY_,relrow,fcrel.table[n],&triplet_keyval,
                               &count);
                 keyval = triplet_keyval.exid;
                 if (tile != triplet_keyval.tile)
                    {
                    keyval = -2;
                    }
                 break;
              default:
                 keyval = 0;
                 break;
              }
           }
        if (keyval > 0)
           ll_insert(&keyval,sizeof(keyval),ll_last(keylist));
        prow = ll_next(prow);
        free_row(relrow,fcrel.table[n]);
        }
     ll_reset(rowlist);
     }
   rowlist = ll_init();
   if (ll_empty(keylist))
      return rowlist;
   p = ll_first(keylist);
   while (!ll_end(p))
      {
      ll_element(p,&keyval);
      templist = related_rows(&keyval,fcrel.table[n],rcell.key2,tile);
      prow = ll_first(templist);
      while (!ll_end(prow))
         {
         ll_element(prow,&rownum);
         if (!ll_locate(&rownum,rowlist))
            ll_insert(&rownum,sizeof(rownum),ll_last(rowlist));
         prow = ll_next(prow);
         }
      ll_reset(templist);
      p = ll_next(p);
      }
   ll_reset(keylist);

   return rowlist;
   }


/**************************************************************************
 *
 *N  deselect_feature_class_relate
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Clear out a previously allocated feature class relate structure
 *    from memory.
 *************************************************************************/
#ifdef PROTO
   void deselect_feature_class_relate (fcrel_type *fcrel)
#else
   void deselect_feature_class_relate (fcrel)
      fcrel_type *fcrel;
#endif

   {
   register int32 i;

   if (fcrel->nchain > 0)
      {
      for (i=0; i<fcrel->nchain; i++)
         {
         if (fcrel->table[i].status == OPENED)
            {
            vpf_close_table (&(fcrel->table[i]));
            }
         }
      xvt_free ((char*)fcrel->table);
      ll_reset(fcrel->relate_list);
      }
   fcrel->nchain = 0;
   }
