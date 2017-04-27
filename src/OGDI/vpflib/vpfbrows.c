/*  VPFBROWS.C  */

#include "xvt.h"
#include "musedir.h"
#include "vpftable.h"
#include "vpfbrows.h"
#include "ibrowse.h"


#define  MAXLINES 10000
#define  MAXLENGTH 255

static int32 maxlength;


#ifdef PROTO
   void vpf_browse (BROWSE *browse)
#else
   void vpf_browse (browse)
      BROWSE *browse;
#endif

   {
   SLIST slist;
   SLIST_ELT elt;
   char *string;

   /* Display results in Browse Window */
   wait_cursor ();

   if (browse->string_length < MAXLENGTH)
      maxlength = browse->string_length;
   else
      maxlength = MAXLENGTH;

/*********************** QUIKFIX ****************************/
/* Allowing the user to select the stringlength is causing  */
/* too many problems to correct at this time so force       */
/* maxlength to always = 255.                 dgm           */
/************************************************************/
   maxlength = MAXLENGTH;

   slist = table_to_slist (browse);
   elt = slist_first (slist);
   string = slist_get (slist, elt, 0L);
   ibrowse_slist (string, slist);
   return;
   }


#ifdef PROTO
   SLIST table_to_slist (BROWSE *browse)
#else
   SLIST table_to_slist (browse)
      BROWSE browse;
#endif

   {
   SLIST	               browse_list;
   vpf_table_type       table;
   date_type            dval, *dptr;
   id_triplet_type      kval, *kptr;
   coordinate_type	   cval, *cptr;
   tri_coordinate_type  zval, *zptr;
   row_type	            row;
   int32 	               i, j, k, n;
   int		            ival, *iptr;
   int32 	               lval, *lptr;
   float	               fval, *fptr;
   char 	               *buf, ch, date[40];
   char 	               *string, *temp, index[4];

 
   /* Initialize string count to 0 */
   str2list (NULL, NULL);

   memset (&table, '\0', sizeof (vpf_table_type));
   memset (&index, '\0', sizeof (index));
   string = xvt_zmalloc (maxlength);
   temp = xvt_zmalloc (maxlength);
   browse_list = slist_new();



   /* Add the table path and name as the first string on the list */
   strcpy (string, browse->path);
   str2list (browse_list, string);

   strcpy (string, " ");
   str2list (browse_list, string);


   /* Determine if the file to be browsed is an INDEX */
   if ((strncmp (&browse->path[(strlen (browse->path) - 1)], "x", 1) == 0) ||
		 (strncmp (&browse->path[(strlen (browse->path) - 1)], "X", 1) == 0) ||
       (strncmp (&browse->path[(strlen (browse->path) - 3)], "fcz", 3) == 0))
      {
      browse_list = idx_to_str (browse, browse_list);
      xvt_free (string);
      xvt_free (temp);
      return (browse_list);
      }
   else if ((strncmp (&browse->path[(strlen (browse->path) - 2)], "si", 2) == 0) ||
				(strncmp (&browse->path[(strlen (browse->path) - 2)], "SI", 2) == 0))
      {
      browse_list = si_to_str (browse, browse_list);
      xvt_free (string);
      xvt_free (temp);
      return (browse_list);
      }
   else if ((strncmp (&browse->path[(strlen (browse->path) - 2)], "ti", 2) == 0) ||
			   (strncmp (&browse->path[(strlen (browse->path) - 2)], "TI", 2) == 0))
      {
      browse_list = ti_to_str (browse, browse_list);
      xvt_free (string);
      xvt_free (temp);
      return (browse_list);
      }

   table = vpf_open_table (browse->path, DISK, "rb", NULL);

   if (browse->hdr_data)
      {
      /* Dump Table Header Info */
      strcpy (string, "******  TABLE HEADER DATA ******");
      str2list (browse_list, string);

      sprintf (string, "NAME: %s", table.name);
      str2list (browse_list, string);

      strcpy (string, "DESCRIPTION: ");
      if (table.description)
         strcat (string, table.description);
      else
         strcat (string, "NULL");
      str2list (browse_list, string);

      strcpy (string, "NARRATIVE TABLE: ");
      if (table.narrative)
         strcat (string, table.narrative);
      else
         strcat (string, "NULL");
      str2list (browse_list, string);

      sprintf (string, "BYTE ORDER: %c", table.byte_order);
      str2list (browse_list, string);

      sprintf (string, "HEADER LENGTH: %ld", table.ddlen);
      str2list (browse_list, string);

      sprintf (string, "RECORD LENGTH: %ld", table.reclen);
      str2list (browse_list, string);

      sprintf (string, "NR RECORDS: %ld", table.nrows);
      str2list (browse_list, string);

      sprintf (string, "NR FIELDS: %ld", table.nfields);
      str2list (browse_list, string);

      strcpy (string, " ");
      str2list (browse_list, string);
      }


   if (browse->field_desc)
      {
      strcpy (string, "****** FIELD DESCRIPTORS ******");
      str2list (browse_list, string);

      for (i=0; i<table.nfields; i++)
         {
         sprintf (string, "%ld", (i+1));
         sprintf (&string[ strlen (string)], " <%s>", table.header[i].name);
         sprintf (&string[ strlen (string)], "<%c>", table.header[i].type);
         sprintf (&string[ strlen (string)], "<%ld>", table.header[i].count);
         sprintf (&string[ strlen (string)], "<%c>", table.header[i].keytype);
         sprintf (&string[ strlen (string)], "<%s>", table.header[i].description);
         sprintf (&string[ strlen (string)], "<%s>", table.header[i].vdt);
         sprintf (&string[ strlen (string)], "<%s>", table.header[i].tdx);
         sprintf (&string[ strlen (string)], "<%s>", table.header[i].narrative);
         str2list (browse_list, string);
         }

      strcpy (string, " ");
      str2list (browse_list, string);
      }

   strcpy (string, "******  TABLE RECORD DATA ******");
   str2list (browse_list, string);

   if (browse->start_record < 1)
      browse->start_record = 1;
   if (browse->end_record > table.nrows)
      browse->end_record = table.nrows;

   for (i=browse->start_record; i<=browse->end_record; i++)
      {
      row = get_row (i, table);
      memset (string, '\0', maxlength);
      sprintf (string, "%ld: ", i);
      for (j=0; j<table.nfields; j++)
	      {
	      switch (table.header[j].type)
	         {
            case 'T':  /* Text */
               if ((int)table.header[j].count==1) /* String is one char */
		            {
                  get_table_element (j, row, table, &ch, &n);
                  sprintf (temp, "<%c>", ch);
                  if (add_string (browse_list, string, temp) == FALSE)
                     {
                     free_row (row, table);
                     vpf_close_table (&table);
                     xvt_free (string);
                     xvt_free (temp);
                     return (browse_list);
                     }
                  }
               else  /* String is greater than one char int32 */
                  {
                  buf = (char*)get_table_element (j, row, table, NULL, &n);
                  n = strlen (buf);
                  /* Filter out carriage control characters */
                  for (k=0; k<n; k++)
                     if (buf[k] == '\n' || buf[k] == '\r')
                        buf[k] = ' ';
						sprintf (temp, "<%s>", buf);
                  if (add_string (browse_list, string, temp) == FALSE)
                     {
                     xvt_free (buf);
                     free_row (row, table);
                     vpf_close_table (&table);
                     xvt_free (string);
                     xvt_free (temp);
                     return (browse_list);
                     }
                  xvt_free (buf);
                  }
               break;
            case 'I': /* Int32 Integer */
               if ((int)table.header[j].count==1)
                  {
                  get_table_element (j, row, table, &lval, &n);
                  sprintf (temp, "<%ld>", lval);
                  if (add_string (browse_list, string, temp) == FALSE)
                     {
                     free_row (row, table);
                     vpf_close_table (&table);
                     xvt_free (string);
                     xvt_free (temp);
                     return (browse_list);
                     }
                  }
               else
                  {
                  lptr = (int32*)get_table_element (j, row, table, NULL, &n);
                  for (k=0; k<n; k++)
                     {
                     sprintf (temp, "<%ld>",lptr[k]);
                     if (add_string (browse_list, string, temp) == FALSE)
                        {
                        free_row (row, table);
                        vpf_close_table (&table);
                        xvt_free (string);
                        xvt_free (temp);
                        return (browse_list);
                        }
                     }
                  xvt_free ((char*)lptr);
                  }
               break;
            case 'S': /* Short */
               if ((int)table.header[j].count==1)
                  {
                  get_table_element (j, row, table, &ival, &n);
                  sprintf (temp, "<%d>", ival);
                  if (add_string (browse_list, string, temp) == FALSE)
                     {
                     free_row (row, table);
                     vpf_close_table (&table);
                     xvt_free (string);
                     xvt_free (temp);
                     return (browse_list);
                     }
                  }
               else
                  {
                  iptr = (int*)get_table_element (j, row, table, NULL, &n);
                  for (k=0; k<n; k++)
                     {
                     sprintf (temp, "<%d>",iptr[k]);
                     if (add_string (browse_list, string, temp) == FALSE)
                        {
                        free_row (row, table);
                        vpf_close_table (&table);
                        xvt_free (string);
                        xvt_free (temp);
                        return (browse_list);
                        }
                     }
                  xvt_free ((char*)iptr);
                  }
               break;
            case 'F': /* Float */
               if ((int)table.header[j].count==1)
                  {
                  get_table_element (j, row, table, &fval, &n);
                  sprintf (temp, "<%f>", fval);
                  if (add_string (browse_list, string, temp) == FALSE)
                     {
                     free_row (row, table);
                     vpf_close_table (&table);
                     xvt_free (string);
                     xvt_free (temp);
                     return (browse_list);
                     }
                  }
               else
                  {
                  fptr = (float*)get_table_element (j, row, table, NULL, &n);
                  for (k=0; k<n; k++)
                     {
                     sprintf (temp, "<%f>", fptr[k]);
                     if (add_string (browse_list, string, temp) == FALSE)
                        {
                        free_row (row, table);
                        vpf_close_table (&table);
                        xvt_free (string);
                        xvt_free (temp);
                        return (browse_list);
                        }
                     }
                  xvt_free ((char*)fptr);
                  }
               break;
            case 'C': /* Coordinate pair */
               if ((int)table.header[j].count==1)
                  {
                  get_table_element (j, row, table, &cval, &n);
                  sprintf (temp,"<%lf,%lf>", cval.x, cval.y);
                  if (add_string (browse_list, string, temp) == FALSE)
                     {
                     free_row (row, table);
                     vpf_close_table (&table);
                     xvt_free (string);
                     xvt_free (temp);
                     return (browse_list);
                     }
                  }
               else
                  {
                  cptr = (coordinate_type*)get_table_element
                                              (j, row, table, NULL, &n);
#if 0
                  for (k=0; k<n; k++)
                     {
                     sprintf (temp, "<%lf,%lf>", cptr[k].x, cptr[k].y);
                     if (add_string (browse_list, string, temp) == FALSE)
                        {
                        free_row (row, table);
                        vpf_close_table (&table);
                        xvt_free (string);
                        xvt_free (temp);
                        return (browse_list);
                        }
#endif
                  if (str2list (browse_list, string) != TRUE)
                     {
                     free_row (row, table);
                     vpf_close_table (&table);
                     xvt_free (string);
                     xvt_free (temp);
                     return (browse_list);
                     }
                  for (k=0; k<n; k++)
                     {
                     sprintf (string, "   %lf, %lf", cptr[k].x, cptr[k].y);
                     if (str2list (browse_list, string) != TRUE)
                        {
                        free_row (row, table);
                        vpf_close_table (&table);
                        xvt_free (string);
                        xvt_free (temp);
                        return (browse_list);
                        }
                     }
                  xvt_free ((char*)cptr);
                  }
               break;
            case 'K':
	       if ((int)table.header[j].count==1)
		  {
		  get_table_element (j, row, table, &kval, &n);
		  sprintf (temp, "<%ld,%ld,%ld>",
					       kval.id, kval.tile, kval.exid);
		  if (add_string (browse_list, string, temp) == FALSE)
		     {
		     free_row (row, table);
		     vpf_close_table (&table);
		     xvt_free (string);
           xvt_free (temp);
		     return (browse_list);
		     }
		  }
	       else
		  {
		  kptr = get_table_element (j, row, table, NULL, &n);
		  for (k=0; k<n; k++)
		     {
		     sprintf (temp, "<%ld,%ld,%ld>",
				      kptr[k].id, kptr[k].tile, kptr[k].exid);
		     if (add_string (browse_list, string, temp) == FALSE)
			{
			free_row (row, table);
			vpf_close_table (&table);
			xvt_free (string);
         xvt_free (temp);
			return (browse_list);
			}
		     }
		  xvt_free ((char*)kptr);
		  }
	       break;
            case 'D':  /* Date */
	       if ((int)table.header[j].count==1)
		  {
		  get_table_element (j, row, table, &dval, &n);
		  format_date (dval, date);
		  sprintf (temp, "<%s>", date);
		  if (add_string (browse_list, string, temp) == FALSE)
		     {
		     free_row (row, table);
		     vpf_close_table (&table);
		     xvt_free (string);
           xvt_free (temp);
		     return (browse_list);
		     }
		  }
	       else
		  {
		  dptr = get_table_element (j, row, table, NULL, &n);
		  for (k=0; k<n; k++)
		     {
		     format_date (dptr[k], date);
		     sprintf (temp, "<%s>", date);
		     if (add_string (browse_list, string, temp) == FALSE)
			{
			free_row (row, table);
			vpf_close_table (&table);
			xvt_free (string);
         xvt_free (temp);
			return (browse_list);
			}
		     }
		  xvt_free ((char*)dptr);
		  }
	       break;
            case 'Z': /* 3D Coordinates */
               if ((int)table.header[j].count==1)
                  {
                  get_table_element (j, row, table, &zval, &n);
                  sprintf (temp, "<%f,%f,%f>", zval.x, zval.y, zval.z);
                  if (add_string (browse_list, string, temp) == FALSE)
                     {
                     free_row (row, table);
                     vpf_close_table (&table);
                     xvt_free (string);
                     xvt_free (temp);
                     return (browse_list);
                     }
                  }
               else
                  {
                  zptr = (tri_coordinate_type*)get_table_element
                         (j, row, table, NULL, &n);
                  for (k=1; k<n; k++)
                     {
                     sprintf (temp, "<%f,%f,%f>",
                             zptr[k].x, zptr[k].y, zptr[k].z);
                     if (add_string (browse_list, string, temp) == FALSE)
                        {
                        free_row (row, table);
                        vpf_close_table (&table);
                        xvt_free (string);
                        xvt_free (temp);
                        return (browse_list);
                        }
                     }
                  xvt_free ((char*)zptr);
                  }
               break;
            default:
               break;
         } /* switch */
      }
      if (str2list (browse_list, string) == FALSE)
         {
         free_row (row, table);
         vpf_close_table (&table);
         xvt_free (string);
         xvt_free (temp);
         return (browse_list);
         }
      free_row (row, table);
      }
   strcpy (string, "END_OF_TABLE");
   slist_add (browse_list, NULL, string, 0L);
   vpf_close_table (&table);
   if (string)
      xvt_free (string);
   if (temp)
      xvt_free (temp);
   return (browse_list);
   }



/**************************************************************************/
/* IDX_TO_STR                                                             */
/**************************************************************************/
#ifdef PROTO
   SLIST idx_to_str (BROWSE *browse, SLIST browse_list)
#else
   SLIST idx_to_str (browse, browse_list)
      BROWSE *browse;
      SLIST browse_list;
#endif

   {
   FILE *file;
   char *string;
   int32 entries, size, offset, length, i;

   /* Initialize string count to 0 */
   str2list (NULL, NULL);

   file = muse_file_open (browse->path, "rb");
   string = xvt_zmalloc(sizeof (char) * maxlength);

   VpfRead (&entries, VpfInteger, 1, file);

   VpfRead (&size, VpfInteger, 1, file);

   /* Print Header Data */
   if (browse->hdr_data)
      {
      strcpy (string, "******  VARIABLE LENGTH INDEX HEADER DATA ******");
      str2list (browse_list, string);

      sprintf (string, "Entries: %ld", entries);
      str2list (browse_list, string);

      sprintf (string, "Size: %ld", size);
      str2list (browse_list, string);

      strcpy (string, " ");
      str2list (browse_list, string);
      }


   strcpy (string, "***** RECORD DATA *****");
   str2list (browse_list, string);

   if (browse->start_record < 1)
      browse->start_record = 1;
   if (browse->end_record > entries)
      browse->end_record = entries;

   for (i = browse->start_record; i <= browse->end_record; i++)
      {
      VpfRead (&offset, VpfInteger, 1, file);
      VpfRead (&length, VpfInteger, 1, file);
      sprintf (string, "%ld: %ld, %ld", i, offset, length);
      if (str2list (browse_list, string) == FALSE)
         {
         fclose (file);
         xvt_free (string);
         return (browse_list);
         }
      }
   fclose (file);
   strcpy (string, "END_OF_TABLE");
   slist_add (browse_list, NULL, string, 0L);
   xvt_free (string);
   return (browse_list);
   } /* END IDX_TO_STR */



/**************************************************************************/
/* SI_TO_STR								                                      */
/**************************************************************************/
#ifdef PROTO
   SLIST si_to_str (BROWSE *browse, SLIST browse_list)
#else
   SLIST si_to_str (browse, browse_list)
      BROWSE  *browse;
      SLIST browse_list;
#endif

   {
   FILE          *file;
   char          *string;
   unsigned char x1, y1, x2, y2;
   int32          numprim, nnode, id, offset, count, i;
   float	 xmin, ymin, xmax, ymax;

   /* Initialize string count to 0 */
   str2list (NULL, NULL);

   file = muse_file_open (browse->path, "rb");
   string = xvt_zmalloc(sizeof (char) * maxlength);

   /* Header Info */
   VpfRead (&numprim, VpfInteger, 1, file);
   VpfRead (&xmin, VpfFloat, 1, file);
   VpfRead (&ymin, VpfFloat, 1, file);
   VpfRead (&xmax, VpfFloat, 1, file);
   VpfRead (&ymax, VpfFloat, 1, file);
   VpfRead (&nnode, VpfInteger, 1, file);

   if (browse->hdr_data)
      {
      strcpy (string, "******  SPATIAL INDEX HEADER DATA ******");
      str2list (browse_list, string);

      sprintf (string, "Number of Primitives: %ld", numprim);
      str2list (browse_list, string);

      sprintf (string, "Xmin: %f", xmin);
      str2list (browse_list, string);

      sprintf (string, "Ymin: %f", ymin);
      str2list (browse_list, string);

      sprintf (string, "Xmax: %f", xmax);
      str2list (browse_list, string);

      sprintf (string, "Ymax: %f", ymax);
      str2list (browse_list, string);

      sprintf (string, "Number of Nodes: %ld", nnode);
      str2list (browse_list, string);
      }

   strcpy (string, "******  SPATIAL INDEX BIN ARRAY RECORDS ******");
   str2list (browse_list, string);

   if (browse->start_record < 1)
      browse->start_record = 1;
   if (browse->end_record > nnode)
      browse->end_record = nnode;

   for (i=browse->start_record; i<=browse->end_record; i++)
      {
      VpfRead (&offset, VpfInteger, 1, file);
      VpfRead (&count, VpfInteger, 1, file);
      sprintf (string, "Node: %ld, Offset: %ld, Count: %ld",
               i, offset, count);
      if (str2list (browse_list, string) == FALSE)
         {
         if (file)
            fclose (file);
         xvt_free (string);
         return (browse_list);
         }
      }
   strcpy (string, " ");
   str2list (browse_list, string);

   /* Position the file pointer to the beginning of the Bin Data Records */
   /* if not there already.                                              */
   if (browse->end_record != nnode)
      {
      while (i<=nnode)
         {
         VpfRead (&offset, VpfInteger, 1, file);
         VpfRead (&count, VpfInteger, 1, file);
         i++;
         }
      }


   strcpy (string, "******  SPATIAL INDEX BIN DATA RECORDS ******");
   str2list (browse_list, string);

   if (browse->start_record < 1)
      browse->start_record = 1;
   if (browse->end_record > numprim)
      browse->end_record = numprim;

   for (i=browse->start_record; i<=browse->end_record; i++)
      {
      VpfRead (&x1, VpfChar, 1, file);
      VpfRead (&y1, VpfChar, 1, file);
      VpfRead (&x2, VpfChar, 1, file);
      VpfRead (&y2, VpfChar, 1, file);
      VpfRead (&id, VpfInteger, 1, file);
      sprintf (string, "X1: %3d, Y1: %3d, X2: %3d, Y2: %3d, ID: %d",
               x1, y1, x2, y2, id);
      if (str2list (browse_list, string) == FALSE)
         {
         xvt_free (string);
         if (file)
            fclose (file);
         return (browse_list);
         }
      }
   if (file)
      fclose (file);
   strcpy (string, "END_OF_TABLE");
   slist_add (browse_list, NULL, string, 0L);
   xvt_free (string);
   return (browse_list);
   }



/**************************************************************************/
/* TI_TO_STR                                                              */
/**************************************************************************/
#ifdef PROTO
   SLIST ti_to_str (BROWSE *browse, SLIST browse_list)
#else
   SLIST ti_to_str (browse, browse_list)
      BROWSE *browse;
      SLIST browse_list;
#endif

   {
   size_t nr_elts, nr_entries, i, maxlines;
   int32   length, nr_rows, *count;
   char   indx_type, data_type, index_data_type;
   char   *string, *temp, name[13], col[26], unused[5];
   int32   offset, nr_records;
   char   *t_elt;
   int32   i_elt;
   short  s_elt;
   float  f_elt;
   double r_elt;
   FILE   *file;

   /* Initialize string count to 0 */
   str2list (NULL, NULL);

   file = muse_file_open (browse->path, "rb");
   string = (char*)xvt_zmalloc (sizeof (char) * maxlength);
   temp = (char*)xvt_zmalloc (sizeof (char) * maxlength);
   memset (name, '\0', sizeof(name));
   memset (col, '\0', sizeof(col));

   VpfRead (&length, VpfInteger, 1, file);
   VpfRead (&nr_entries, VpfInteger, 1, file);
   VpfRead (&nr_rows, VpfInteger, 1, file);
   VpfRead (&indx_type, VpfChar, 1, file);
   VpfRead (&data_type, VpfChar, 1, file);
   VpfRead (&nr_elts, VpfInteger, 1, file);
   VpfRead (&index_data_type, VpfChar, 1, file);
   VpfRead (&name, VpfChar, 12, file);
   VpfRead (&col, VpfChar, 25, file);
   VpfRead (&unused, VpfChar, 4, file);

   if (browse->hdr_data)
      {
      strcpy (string, "***** THEMATIC INDEX HEADER DATA *****");
      str2list (browse_list, string);

      sprintf (string, "Header_size: %ld", length);
      str2list (browse_list, string);

      sprintf (string, "Nr_entries: %ld", (int32)nr_entries);
      str2list (browse_list, string);

      sprintf (string, "Nr_rows: %ld", nr_rows);
      str2list (browse_list, string);

      sprintf (string, "Index_type: %c", indx_type);
      str2list (browse_list, string);

      sprintf (string, "Data_type: %c", data_type);
      str2list (browse_list, string);

      sprintf (string, "Nr_elements: %ld", (int32)nr_elts);
      str2list (browse_list, string);

      sprintf (string, "Index_data_type: %c", index_data_type);
      str2list (browse_list, string);

      sprintf (string, "Table_name: %s", name);
      str2list (browse_list, string);

      sprintf (string, "Column_name: %s", col);
      str2list (browse_list, string);

      sprintf (string, "Unused_field: %s", unused);
      str2list (browse_list, string);
      }
   strcpy (string, " ");
   str2list (browse_list, string);


   count = (int32*)xvt_zmalloc (sizeof(int32) * nr_entries);

   /* Divide the remaining allowable text lines between the Index Directory */
   /* records and the Index Data records				    */
   maxlines = MAXLINES / 2;

   strcpy (string, "***** INDEX DIRECTORY RECORDS *****");
   str2list (browse_list, string);

   /*  Read Index Directory Records */
   switch (data_type)
      {
      case 'L': /* Character String */
      case 'T': /* Character String */
         t_elt = (char*) xvt_zmalloc ((size_t) nr_elts);
         for (i=0; i<nr_entries; i++)
            {
            VpfRead (t_elt, VpfChar, nr_elts, file);
            VpfRead (&offset, VpfInteger, 1, file);
            VpfRead (&nr_records, VpfInteger, 1, file);
            count[i] = nr_records;
            sprintf (string, "Elt: %s Offset: %ld Nr_records: %ld",
            t_elt, offset, nr_records);
            if ((str2list (browse_list, string) == FALSE) || (i == maxlines))
               {
               xvt_free (t_elt);
               break;
               }
            }
         xvt_free (t_elt);
         break;
      case 'S': /* Short Integer */
         for (i=0; i<nr_entries; i++)
            {
            VpfRead (&s_elt, VpfShort, nr_elts, file);
            VpfRead (&offset, VpfInteger, 1, file);
            VpfRead (&nr_records, VpfInteger, 1, file);
            count[i] = nr_records;
            sprintf (string, "Elt: %d Offset: %ld Nr_records: %ld",
            s_elt, offset, nr_records);
            if ((str2list (browse_list, string) == FALSE) || (i == maxlines))
               break;
            }
         break;
      case 'I': /* Int32 Integer */
         for (i=0; i<nr_entries; i++)
            {
            VpfRead (&i_elt, VpfInteger, nr_elts, file);
            VpfRead (&offset, VpfInteger, 1, file);
            VpfRead (&nr_records, VpfInteger, 1, file);
            count[i] = nr_records;
            sprintf (string, "Elt: %ld Offset: %ld Nr_records: %ld",
            i_elt, offset, nr_records);
            if ((str2list (browse_list, string) == FALSE) || (i == maxlines))
               break;
            }
         break;
      case 'F': /* 4 Byte Float */
        for (i=0; i<nr_entries; i++)
           {
           VpfRead (&f_elt, VpfFloat, nr_elts, file);
           VpfRead (&offset, VpfInteger, 1, file);
           VpfRead (&nr_records, VpfInteger, 1, file);
           count[i] = nr_records;
           sprintf (string, "Elt: %f Offset: %ld Nr_records: %ld",
		     f_elt, offset, nr_records);
           if ((str2list (browse_list, string) == FALSE) || (i == maxlines))
              break;
           }
         break;
      case 'R': /* 8 Byte Float */
         for (i=0; i<nr_entries; i++)
            {
            VpfRead (&r_elt, VpfDouble, nr_elts, file);
            VpfRead (&offset, VpfInteger, 1, file);
            VpfRead (&nr_records, VpfInteger, 1, file);
            count[i] = nr_records;
            sprintf (string, "Elt: %f Offset: %ld Nr_records: %ld",
            r_elt, offset, nr_records);
           if ((str2list (browse_list, string) == FALSE) || (i == maxlines))
              break;
           }
         break;
      default:
	 break;
      }
   strcpy (string, " ");
   str2list (browse_list, string);


   /* Reset maxlines to the defined value */
   maxlines = MAXLINES;

   strcpy (string, "***** INDEX DATA RECORDS *****");
   str2list (browse_list, string);

   /*  Read Index Data Records */
   if ((indx_type == 'I') || (indx_type == 'T'))
      {
      switch (index_data_type)
         {
         case 'S': /* Short Integer */
            {
            short *row_nrs;
            int32 j;

            for (i=0; i<nr_entries; i++)
               {
               row_nrs = (short*)xvt_zmalloc (sizeof(short) * (size_t)count[i]);
               VpfRead (row_nrs, VpfShort, (size_t)count[i],  file);
               sprintf (string, "%d  Count: %ld  %s: ", (i+1), count[i], col);
               for (j=0; j<count[i]; j++)
                  {
                  sprintf (temp, "<%hd>",row_nrs[j]);
                  if (add_string (browse_list, string, temp) == FALSE)
                     {
                     xvt_free ((char*)row_nrs);
                     xvt_free ((char*)count);
                     xvt_free (temp);
                     xvt_free (string);
                     return (browse_list);
                     }
                  }
               xvt_free ((char*)row_nrs);
               }
            break;
            } /* case S */
         case 'I': /* Int32 Integer */
            {
            int32  j, *row_nrs;

            for (i=0; i<nr_entries; i++)
               {
               row_nrs = (int32*)xvt_zmalloc (sizeof(int32) * (size_t)count[i]);
               VpfRead (row_nrs, VpfInteger, (size_t)count[i],	file);
               sprintf (string, "%d  Count: %ld  %s: ", (i+1), count[i], col);
               for (j=0; j<count[i]; j++)
                  {
                  sprintf (temp, "<%ld>",row_nrs[j]);
                  if (add_string (browse_list, string, temp) == FALSE)
                     {
                     xvt_free ((char*)row_nrs);
                     xvt_free ((char*)count);
                     xvt_free (temp);
                     xvt_free (string);
                     return (browse_list);
                     }
                  }
               xvt_free ((char*)row_nrs);
               }
            break;
            } /* case I */
         } /* switch */
      } /* if */

   strcpy (string, "END_OF_TABLE");
   slist_add (browse_list, NULL, string, 0L);
   xvt_free (string);
   xvt_free (temp);
   if (count)
      xvt_free ((char*)count);
   fclose (file);
   return (browse_list);
   }



/***************************************************************************/
/* ADD_STRING								   */
/***************************************************************************/

#ifdef PROTO
    BOOLEAN add_string (SLIST list, char *string, char* temp)
#else
    BOOLEAN add_string (list, string, temp)
       SLIST list;
       char  *string;
       char  *temp;
#endif


   {
   size_t l1, l2;


   /* See if the string has enough room left to cat temp */
   l1 = strlen (temp);
   l2 = strlen (string);

   if ((l1 + l2) > maxlength)
      {
      /* Not enough room left in string so add string to slist and start fresh */
      if (str2list (list, string) != TRUE)
         {
         /* MAXLINES reached */
         return (FALSE);
         }
      strcpy (string, temp);
      return (TRUE);
      }
   /* See if temp will fit into string */
   if (l1 < (maxlength - l2))
      {
      /* Copy contents of temp to string */
      strcat (string, temp);
      }
   else  /* Temp is too big to fit into one string so break it up */
      {
      short start, end = 0;
      BOOLEAN done = FALSE;

      while (!done)
         {
         start = end;
         end += maxlength - 1;
         strncpy (string, &temp[start], end);
         if (str2list (list, string) != TRUE)
            {
            /* MAXLINES reached */
            return (FALSE);
            }
         if (end >= l1)
            done = TRUE;
         }
      }
   return (TRUE);
   }


/***************************************************************************/
/* STR2LIST								   */
/***************************************************************************/

#ifdef PROTO
   BOOLEAN str2list (SLIST list, char* string)
#else
   BOOLEAN str2list (list, string)
      SLIST list;
      char *string;
#endif

   {
   static int32 nr_lines;

   /* Use a Null string to initialize nr_lines */
   if (string == (char*)NULL)
      {
      nr_lines = 0L;
      return (TRUE);
      }

   slist_add (list, NULL, string, 0L);
   nr_lines++;

   /* Check nr_lines for MAXLINES */
   if (nr_lines >= MAXLINES)
	   {
	   /* Close up shop */
	   strcpy (string, "MAXLINES REACHED");
	   slist_add (list, NULL, string, 0L);
	   return (FALSE);
	   }
   else
	   /* Start a new string */
	   memset (string, '\0', maxlength);

   return (TRUE);
   }
