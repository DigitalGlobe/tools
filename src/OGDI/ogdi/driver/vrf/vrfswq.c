/******************************************************************************
 *
 * Component: OGDI VRF Driver
 * Purpose: Implements VPF query capability based on SWQ in a manner similar
 *          to the vpfquery.c code distributed with vpflib. 
 * 
 ******************************************************************************
 * Copyright (C) 1995 Logiciels et Applications Scientifiques (L.A.S.) Inc
 * Permission to use, copy, modify and distribute this software and
 * its documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies, that
 * both the copyright notice and this permission notice appear in
 * supporting documentation, and that the name of L.A.S. Inc not be used 
 * in advertising or publicity pertaining to distribution of the software 
 * without specific, written prior permission. L.A.S. Inc. makes no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 ******************************************************************************
 *
 * $Log: vrfswq.c,v $
 * Revision 1.7  2007/05/09 20:46:28  cbalint
 * From: Even Rouault <even.rouault@mines-paris.org>
 * Date: Friday 21:14:18
 *
 *         * fix filename case sensitivy problems (for Unix-like systems).
 *
 *         * fix incorrect use of sprintf in vrf_GetMetadata.
 *
 *         * report wgs84 instead of nad83, not sure whether that is true
 *         for all VPF products, but at least it's correct for VMAP products
 *         that *must* be WGS84. A better fix would be to read the VPF table
 *         that contains this information.
 *
 *         * fix a few minor memory leaks and memory usage issues.
 *
 *         * enable XMIN, YMIN, XMAX and YMAX columns to be of type double
 *         in EBR and FBR files (for read the VMAP2i 'MIG2i000' product).
 *
 *         * add .pjt and .tjt as possible extensions for join tables
 *         (VMAP2i 'MIG2i000' product).
 *
 *         * fix duplicated layers report (VMAP2i 'MIG2i000' product).
 *
 *         * handle 'L' (Latin1) type for text files (GEOCAPI 'MIGxxx' products).
 *
 *         * optionnaly, convert text to UTF-8 when environment variable
 *         CONVERT_OGDI_TXT_TO_UTF8 is defined. This part is not portable
 *         on Windows I guess (only tested on Linux) and maybe too specific.
 *
 *         * enable reading of VPF products without table indexes file
 *         (GEOCAPI 'MIG013' and 'MIG016' products). VPF norm says that when
 *         there is a variable length field in one table, an index should exist,
 *         but some test products don't follow this. The approach here is to read
 *         the whole table content and build the index in memory.
 *
 *  Modified Files:
 *  	ChangeLog ogdi/driver/vrf/feature.c ogdi/driver/vrf/object.c
 *  	ogdi/driver/vrf/utils.c ogdi/driver/vrf/vrf.c
 *  	ogdi/driver/vrf/vrfswq.c vpflib/musedir.c vpflib/strfunc.c
 *  	vpflib/vpfbrows.c vpflib/vpfprop.c vpflib/vpfquery.c
 *  	vpflib/vpfread.c vpflib/vpftable.c
 *
 * Revision 1.6  2007/02/12 15:52:57  cbalint
 *
 *    Preliminary cleanup.
 *    Get rif of unitialized variables, and unused ones.
 *
 * Revision 1.5  2004/10/25 21:24:43  warmerda
 * Fixed case of 1 character wide fields as per Stephane's submission
 * in bug 809737.
 *
 * Revision 1.4  2004/10/25 19:09:06  warmerda
 * Fixed so that string comparisons on fields long than 1 character
 * work.  Also fixed so that trailing spaces are trimmed off string
 * values before comparing.
 *
 * Revision 1.3  2001/06/26 00:57:34  warmerda
 * fixed strcasecmp on WIN32
 *
 * Revision 1.2  2001/06/21 20:30:15  warmerda
 * added ECS_CVSID
 *
 * Revision 1.1  2001/06/20 21:49:16  warmerda
 * New
 *
 */

#include "ecs.h"
#include "vrf.h"
#include "swq.h"

ECS_CVSID("$Id: vrfswq.c,v 1.7 2007/05/09 20:46:28 cbalint Exp $");

#ifndef __LINKLIST_H__
#include "linklist.h"
#endif
#ifndef _VPFTABLE_H_
#include "vpftable.h"
#endif
#ifndef __SET_H__
#include "set.h"
#endif
#ifndef __STRFUNC_H__
#include "strfunc.h"
#endif
#ifndef _MACHINE_
#include "machine.h"
#endif

#ifdef WIN32
#  define strcasecmp stricmp
#endif

typedef struct {
    row_type	row;
    vpf_table_type  table;
} evaluator_info;

/************************************************************************/
/*                         vrf_swq_evaluator()                          */
/*                                                                      */
/*      Evaluate a single operation from the WHERE expression on a      */
/*      row from the VRF table.                                         */
/************************************************************************/

int vrf_swq_evaluator( swq_field_op *op, void *raw_info )

{
    int32 lval, count;
    short sval;
    float fval;
    char *tptr;

    evaluator_info *info = (evaluator_info *) raw_info;
    row_type	row = info->row;
    vpf_table_type  table = info->table;

/* -------------------------------------------------------------------- */
/*      String field comparison.                                        */
/* -------------------------------------------------------------------- */
    if( table.header[op->field_index].type == 'T' || table.header[op->field_index].type == 'L' )
    {
        int	ret_result, i;

        /* count=1 is a special case because the value is returned into 
           a char instead of returning an allocated string */
	if (table.header[op->field_index].count == 1) {
	  
            char cval;
            get_table_element( op->field_index, row, table, &cval, &count );

            if( op->operation == SWQ_EQ )
                ret_result = (cval == op->string_value[0]);
            else
                ret_result = (cval != op->string_value[0]);

	} else {
            tptr = (char *)get_table_element( op->field_index, row, table,
                                              NULL, &count );

            /* trim whitepsace */
            for( i = strlen(tptr)-1; i >= 0 && tptr[i] == ' '; i-- )
                tptr[i] = '\0';

            if( op->operation == SWQ_EQ )
                ret_result = (strcasecmp(tptr,op->string_value) == 0);
            else
                ret_result = (strcasecmp(tptr,op->string_value) != 0);
            
            xvt_free(tptr);
        }

        return ret_result;
    }

/* -------------------------------------------------------------------- */
/*      Numeric field comparison.                                       */
/* -------------------------------------------------------------------- */
    else
    {
        if (table.header[op->field_index].count != 1)
            return FALSE;

        if( table.header[op->field_index].type == 'S' )
        {
            get_table_element( op->field_index, row, table, &sval, &count );
            fval = sval;
        }
        else if( table.header[op->field_index].type == 'I' )
        {
            get_table_element( op->field_index, row, table, &lval, &count );
            fval = lval;
        }
        else
        {
            get_table_element( op->field_index, row, table, &fval, &count );
        }

        switch( op->operation )
        {
          case SWQ_EQ:
            return fval == op->float_value;

          case SWQ_NE:
            return fval != op->float_value;

          case SWQ_GT:
            return fval > op->float_value;

          case SWQ_LT:
            return fval < op->float_value;

          case SWQ_GE:
            return fval >= op->float_value;

          case SWQ_LE:
            return fval <= op->float_value;
            
          default:
            return FALSE;
        }
    }
}


/************************************************************************
 *                            query_table2()                            
 *
 *     This function returns the set of selected rows of a VPF table
 *     based upon the evaluation of the given selection expression string.
 *
 *     see swq.h/swq.c for details on the expression syntax.
 *
 *   Parameters:
 *
 *    expression <input>==(char *) selection expression string.
 *    table      <input>==(vpf_table_type) VPF table structure.
 *    return    <output>==(set_type) set of selected rows.
 ************************************************************************/

set_type query_table2( char *expression, vpf_table_type table )
{
   register int32 i, ipos;
   set_type select_set;

   swq_expr *expr;
   const char *error;
   int	nfields;
   char **fieldname;
   swq_field_type *fieldtype;
   evaluator_info ev_info;

   select_set = set_init (table.nrows);

/* -------------------------------------------------------------------- */
/*      If the expression is "*" then just turn on all members of       */
/*      select_set and return.                                          */
/* -------------------------------------------------------------------- */
   if (strcmp (expression, "*") == 0)
   {
       set_on(select_set);
       return select_set;
   }

/* -------------------------------------------------------------------- */
/*      Prepare the field list.                                         */
/* -------------------------------------------------------------------- */
   nfields = (int32)table.nfields;

   fieldname = (char**) malloc(nfields * sizeof(char *));
   fieldtype = (swq_field_type *) malloc(nfields * sizeof(swq_field_type));

   for (i=0; i < nfields; i++)
   {
       fieldname[i] = (char *) table.header[i].name;
       if( table.header[i].type == 'T' || table.header[i].type == 'L' )
           fieldtype[i] = SWQ_STRING;
       else if( table.header[i].type == 'F' )
           fieldtype[i] = SWQ_FLOAT;
       else if( table.header[i].type == 'I' || table.header[i].type == 'S' )
           fieldtype[i] = SWQ_INTEGER;
       else
           fieldtype[i] = SWQ_OTHER;
   }
   
/* -------------------------------------------------------------------- */
/*      Compile the WHERE expression.                                   */
/* -------------------------------------------------------------------- */
   error = swq_expr_compile( expression, nfields, fieldname, fieldtype, 
                             &expr );

   if ( error != NULL || expr == NULL ) 
       return select_set;

/* -------------------------------------------------------------------- */
/*      Process the table, one record at a time.                        */
/* -------------------------------------------------------------------- */
   if (table.storage == DISK)
   {
       ipos = index_pos (1L, table);
       fseek( table.fp, ipos, SEEK_SET );
   }

   ev_info.table = table;

   for (i=1;i<=table.nrows;i++) {

      if (table.storage == DISK)
          ev_info.row = read_next_row(table);
      else
          ev_info.row = get_row( i, table );

      if( swq_expr_evaluate( expr, vrf_swq_evaluator, (void *) &ev_info ) )
          set_insert( i, select_set );

      free_row(ev_info.row, table);
   }

/* -------------------------------------------------------------------- */
/*      Cleanup                                                         */
/* -------------------------------------------------------------------- */
   free( fieldtype );
   free( fieldname );
   swq_expr_free( expr );

   return select_set;
}
