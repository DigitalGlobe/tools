/******************************************************************************
 *
 * Component: OGDI Driver Support Library
 * Purpose: Generic SQL WHERE Expression Evaluator Declarations.
 * Author: Frank Warmerdam <warmerdam@pobox.com>
 * 
 ******************************************************************************
 * Copyright (C) 2001 Information Interoperability Institute (3i)
 * Permission to use, copy, modify and distribute this software and
 * its documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies, that
 * both the copyright notice and this permission notice appear in
 * supporting documentation, and that the name of 3i not be used 
 * in advertising or publicity pertaining to distribution of the software 
 * without specific, written prior permission.  3i makes no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 ******************************************************************************
 *
 * $Log: swq.h,v $
 * Revision 1.1  2001/06/20 21:49:16  warmerda
 * New
 *
 */

#ifndef _SWQ_H_INCLUDED_
#define _SWQ_H_INCLUDED_

typedef enum {
    SWQ_OR,
    SWQ_AND,
    SWQ_NOT,
    SWQ_EQ,
    SWQ_NE,
    SWQ_GE,
    SWQ_LE,
    SWQ_LT,
    SWQ_GT,
    SWQ_UNKNOWN
} swq_op;

typedef enum {
    SWQ_INTEGER,
    SWQ_FLOAT,
    SWQ_STRING, 
    SWQ_BOOLEAN,
    SWQ_OTHER
} swq_field_type;

typedef struct {
    swq_op      operation;

    /* only for logical expression on subexpression */
    struct swq_node_s  *first_sub_expr;
    struct swq_node_s  *second_sub_expr;

    /* only for binary field operations */
    int 	field_index;
    swq_field_type field_type;
    char	*string_value;
    int		int_value;
    double	float_value;
} swq_field_op;

typedef swq_field_op swq_expr;

typedef int (*swq_op_evaluator)(swq_field_op *op, void *record_handle);

/* Compile an SQL WHERE clause into an internal form.  The field_list is
** the list of fields in the target 'table', used to render where into 
** field numbers instead of names. 
*/
const char *swq_expr_compile( const char *where_clause, 
                         int field_count,
                         char **field_list,
                         swq_field_type *field_types,
                         swq_expr **expr );

/*
** Evaluate an expression for a particular record using an application
** provided field operation evaluator, and abstract record handle. 
*/
int swq_expr_evaluate( swq_expr *expr, swq_op_evaluator fn_evaluator,
                       void *record_handle );

void swq_expr_free( swq_expr * );

#endif /* def _SWQ_H_INCLUDED_ */

