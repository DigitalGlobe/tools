/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		expr.cpp
 *	DESCRIPTION:	Expression parser
 *
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#include "firebird.h"

#include "../jrd/ibase.h"
#include "../dudley/ddl.h"
#include "../dudley/parse.h"
#include "../jrd/acl.h"
#include "../jrd/intl.h"
#include "../dudley/ddl_proto.h"
#include "../dudley/expr_proto.h"
#include "../dudley/lex_proto.h"
#include "../dudley/parse_proto.h"

static CON make_numeric_constant(const TEXT*, USHORT);
static DUDLEY_NOD parse_add(USHORT *, bool *);
static DUDLEY_NOD parse_and(USHORT *);
static DUDLEY_NOD parse_field();
static DUDLEY_NOD parse_from(USHORT *, bool *);
static DUDLEY_NOD parse_function();
static DUDLEY_NOD parse_gen_id();
static CON parse_literal();
static void parse_matching_paren();
static DUDLEY_NOD parse_multiply(USHORT *, bool *);
static DUDLEY_NOD parse_not(USHORT *);
static DUDLEY_NOD parse_primitive_value(USHORT *, bool *);
static DUDLEY_CTX parse_relation();
static DUDLEY_NOD parse_relational(USHORT *);
static DUDLEY_NOD parse_sort();
static DUDLEY_NOD parse_statistical();
static void parse_terminating_parens(USHORT *, USHORT *);

static struct nod_types {
	enum kwwords nod_t_keyword;
	enum nod_t nod_t_node;
} statisticals[] = {
	{ KW_MAX, nod_max },
	{ KW_MIN, nod_min },
	{ KW_COUNT, nod_count },
	{ KW_AVERAGE, nod_average },
	{ KW_TOTAL, nod_total }
};

static enum nod_t relationals[] = {
	nod_eql, nod_neq, nod_leq, nod_lss, nod_gtr, nod_geq, nod_containing,
	nod_starts, nod_missing, nod_between, nod_sleuth, nod_matches,
	nod_and, nod_or, nod_not, nod_any, nod_unique, (enum nod_t) 0
};


DUDLEY_NOD EXPR_boolean(USHORT * paren_count)
{
/**************************************
 *
 *	E X P R _ b o o l e a n
 *
 **************************************
 *
 * Functional description
 *	Parse a general boolean expression.  By precedence, handle an OR
 *	here.
 *
 **************************************/
	DUDLEY_NOD expr, node;
	USHORT local_count;

	if (!paren_count) {
		local_count = 0;
		paren_count = &local_count;
	}

	expr = parse_and(paren_count);

	if (!PARSE_match(KW_OR)) {
		parse_terminating_parens(paren_count, &local_count);
		return expr;
	}

	node = PARSE_make_node(nod_or, 2);
	node->nod_arg[0] = expr;
	node->nod_arg[1] = EXPR_boolean(paren_count);
	parse_terminating_parens(paren_count, &local_count);

	return node;
}


DUDLEY_NOD EXPR_rse(bool view_flag)
{
/**************************************
 *
 *	E X P R _ r s e
 *
 **************************************
 *
 * Functional description
 *	Parse a record selection expression.
 *
 **************************************/
	DUDLEY_NOD node, boolean, field_name, a_boolean;
	dudley_lls* stack;
	DUDLEY_CTX context;
	SYM field_sym;

	node = PARSE_make_node(nod_rse, s_rse_count);
	stack = NULL;
	LEX_real();

	if (PARSE_match(KW_ALL))
		LEX_real();

	if (PARSE_match(KW_FIRST))
		node->nod_arg[s_rse_first] = EXPR_value(0, NULL);

	while (true) {
		LLS_PUSH((DUDLEY_NOD) parse_relation(), &stack);
		if (PARSE_match(KW_OVER)) {
			for (;;) {
				if (!stack->lls_next) {
					PARSE_error(234, 0);	/* msg 234: OVER can only be used in CROSS expressions */
				}
				boolean = PARSE_make_node(nod_eql, 2);
				field_sym = PARSE_symbol(tok_ident);
				field_name = PARSE_make_node(nod_field_name, 2);
				context = (DUDLEY_CTX) stack->lls_object;
				field_name->nod_arg[0] = (DUDLEY_NOD) context->ctx_name;
				field_name->nod_arg[1] = (DUDLEY_NOD) field_sym;
				boolean->nod_arg[0] = field_name;
				field_name = PARSE_make_node(nod_over, 2);
				field_name->nod_arg[0] = (DUDLEY_NOD) context->ctx_name;
				field_name->nod_arg[1] = (DUDLEY_NOD) field_sym;
				boolean->nod_arg[1] = field_name;
				if (node->nod_arg[s_rse_boolean]) {
					a_boolean = PARSE_make_node(nod_and, 2);
					a_boolean->nod_arg[0] = boolean;
					a_boolean->nod_arg[1] = node->nod_arg[s_rse_boolean];
					node->nod_arg[s_rse_boolean] = a_boolean;
				}
				else
					node->nod_arg[s_rse_boolean] = boolean;
				if (!PARSE_match(KW_COMMA))
					break;
			}
		}
		if (!PARSE_match(KW_CROSS))
			break;
	}

	node->nod_arg[s_rse_contexts] = (DUDLEY_NOD) stack;

/* Pick up various other clauses */

	if (PARSE_match(KW_WITH))
		if (boolean = EXPR_boolean(0)) {
			if (node->nod_arg[s_rse_boolean]) {
				a_boolean = PARSE_make_node(nod_and, 2);
				a_boolean->nod_arg[0] = boolean;
				a_boolean->nod_arg[1] = node->nod_arg[s_rse_boolean];
				node->nod_arg[s_rse_boolean] = a_boolean;
			}
			else
				node->nod_arg[s_rse_boolean] = boolean;
		}
	if (PARSE_match(KW_SORTED)) {
		if (view_flag)
			PARSE_error(319, 0);	/* msg 234: Unexpected sort clause */
		PARSE_match(KW_BY);
		node->nod_arg[s_rse_sort] = parse_sort();
	}

	if (PARSE_match(KW_REDUCED)) {
		PARSE_match(KW_TO);
		node->nod_arg[s_rse_reduced] = parse_sort();
	}

	return node;
}


DUDLEY_NOD EXPR_statement()
{
/**************************************
 *
 *	E X P R _ s t a t e m e n t
 *
 **************************************
 *
 * Functional description
 *	Parse a single trigger statement.
 *
 **************************************/
	dudley_lls* stack;
	DUDLEY_NOD node;
	int number;

	if (PARSE_match(KW_BEGIN)) {
		stack = NULL;
		while (!PARSE_match(KW_END))
			LLS_PUSH(EXPR_statement(), &stack);
		node = PARSE_make_list(stack);
	}
	else if (PARSE_match(KW_IF)) {
		node = PARSE_make_node(nod_if, 3);
		node->nod_arg[s_if_boolean] = EXPR_boolean(0);
		PARSE_match(KW_THEN);
		node->nod_arg[s_if_true] = EXPR_statement();
		if (PARSE_match(KW_ELSE))
			node->nod_arg[s_if_false] = EXPR_statement();
		else
			node->nod_count = 2;
	}
	else if (PARSE_match(KW_FOR)) {
		node = PARSE_make_node(nod_for, 2);
		node->nod_arg[s_for_rse] = EXPR_rse(false);
		stack = NULL;
		while (!PARSE_match(KW_END_FOR))
			LLS_PUSH(EXPR_statement(), &stack);
		node->nod_arg[s_for_action] = PARSE_make_list(stack);
	}
	else if (PARSE_match(KW_STORE)) {
		node = PARSE_make_node(nod_store, 2);
		node->nod_arg[s_store_rel] = (DUDLEY_NOD) parse_relation();
		PARSE_match(KW_USING);
		stack = NULL;
		while (!PARSE_match(KW_END_STORE))
			LLS_PUSH(EXPR_statement(), &stack);
		node->nod_arg[s_store_action] = PARSE_make_list(stack);
	}
	else if (PARSE_match(KW_ABORT)) {
		node = PARSE_make_node(nod_abort, 1);
		node->nod_count = 0;
		number = PARSE_number();
		if (number > 255)
			PARSE_error(235, 0);	/* msg 235: abort code cannot exceed 255 */
		node->nod_arg[0] = (DUDLEY_NOD) (IPTR) number;
	}
	else if (PARSE_match(KW_ERASE)) {
		node = PARSE_make_node(nod_erase, 1);
		node->nod_count = 0;
		node->nod_arg[0] = (DUDLEY_NOD) PARSE_symbol(tok_ident);
	}
	else if (PARSE_match(KW_MODIFY)) {
		PARSE_match(KW_USING);
		node = PARSE_make_node(nod_modify, 3);
		node->nod_count = 0;
		node->nod_arg[s_mod_old_ctx] = (DUDLEY_NOD) PARSE_symbol(tok_ident);
		PARSE_match(KW_USING);
		stack = NULL;
		while (!PARSE_match(KW_END_MODIFY))
			LLS_PUSH(EXPR_statement(), &stack);
		node->nod_arg[s_mod_action] = PARSE_make_list(stack);
	}
	else if (PARSE_match(KW_POST)) {
		node = PARSE_make_node(nod_post, 1);
		node->nod_count = 1;
		node->nod_arg[0] = EXPR_value(0, NULL);
	}
	else {
		node = PARSE_make_node(nod_assignment, 2);
		node->nod_arg[1] = parse_field();
		if (!PARSE_match(KW_EQUALS))
			PARSE_error(236, dudleyGlob.DDL_token.tok_string, NULL);	/* msg 236: expected =, encountered \"%s\" */
		node->nod_arg[0] = EXPR_value(0, NULL);
	}

	PARSE_match(KW_SEMI);

	return node;
}


DUDLEY_NOD EXPR_value(USHORT * paren_count,
					  bool * bool_flag)
{
/**************************************
 *
 *	E X P R _ v a l u e
 *
 **************************************
 *
 * Functional description
 *	Parse a general value expression.  In practice, this means parse the
 *	lowest precedence operator CONCATENATE.
 *
 **************************************/
	DUDLEY_NOD node, arg;
	USHORT local_count;
	bool local_flag;

	if (!paren_count) {
		local_count = 0;
		paren_count = &local_count;
	}
	if (!bool_flag) {
		local_flag = false;
		bool_flag = &local_flag;
	}

	node = parse_add(paren_count, bool_flag);

	while (true) {
		if (!PARSE_match(KW_BAR)) {
			parse_terminating_parens(paren_count, &local_count);
			return node;
		}
		arg = node;
		node = PARSE_make_node(nod_concatenate, 2);
		node->nod_arg[0] = arg;
		node->nod_arg[1] = parse_add(paren_count, bool_flag);
	}
}


static CON make_numeric_constant( const TEXT* string, USHORT length)
{
/**************************************
 *
 *	m a k e _ n u m e r i c _ c o n s t a n t
 *
 **************************************
 *
 * Functional description
 *	Build a constant block for a numeric
 *	constant.  Numeric constants are normally
 *	stored as long words, but especially large
 *	ones become text.  They ought to become
 *      double precision, one would think, but they'd
 *      have to be VAX style double precision which is
 *      more pain than gain.
 *
 **************************************/
	const TEXT* p = string;
	USHORT l = length;

	CON constant = (CON) DDL_alloc(sizeof(con) + sizeof(SLONG));
	constant->con_desc.dsc_dtype = dtype_long;
	constant->con_desc.dsc_length = sizeof(SLONG);
	SLONG* number = (SLONG *) constant->con_data;
	constant->con_desc.dsc_address = (UCHAR*) (IPTR) number;
	USHORT scale = 0;

	do {
		const TEXT c = *p++;
		if (c == '.')
			scale = 1;
		else {
			/* The right side of the following comparison had originally been:

			   ((1 << 31) - 1) / 10

			   For some reason, this doesn't work on 64-bit platforms.  Its
			   replacement does. */

			if (*number > ((SLONG) ~ (1 << 31)) / 10)
				break;
			*number *= 10;
			*number += c - '0';
			constant->con_desc.dsc_scale -= scale;
		}
	} while (--l);

/* if there was an overflow (indicated by the fact that we have
 * some length left over), switch back to text.  Do check that
 * the thing is a plausible number.  Also leave a space so we
 * can negate it later if necessary.
 */

	if (l) {
		length++;
		constant = (CON) DDL_alloc(sizeof(con) + length);
		constant->con_desc.dsc_dtype = dtype_text;
		constant->con_desc.dsc_ttype() = ttype_ascii;
		constant->con_desc.dsc_length = length;
		constant->con_desc.dsc_address = constant->con_data;
		TEXT* q = (TEXT *) constant->con_desc.dsc_address;
		p = string;
		*q++ = ' ';

		bool fraction = false;
		for (l = 1; l < length; p++, l++)
			if (*p >= '0' && *p <= '9')
				*q++ = *p;
			else if (*p == '.') {
				*q++ = *p;
				if (fraction)
					DDL_err(237);	/* msg 237: too many decimal points */
				else
					fraction = true;
			}
			else
				DDL_err(238);	/* msg 238: unrecognized character in numeric string */
	}
	return constant;
}


static DUDLEY_NOD parse_add(USHORT * paren_count,
							bool * bool_flag)
{
/**************************************
 *
 *	p a r s e _ a d d
 *
 **************************************
 *
 * Functional description
 *	Parse a general value expression.  In practice, this means parse the
 *	lowest precedence operators, ADD and SUBTRACT.
 *
 **************************************/
	DUDLEY_NOD node, arg;
	enum nod_t operatr;

	node = parse_multiply(paren_count, bool_flag);

	while (true) {
		if (PARSE_match(KW_PLUS))
			operatr = nod_add;
		else if (PARSE_match(KW_MINUS))
			operatr = nod_subtract;
		else
			return node;
		arg = node;
		node = PARSE_make_node(operatr, 2);
		node->nod_arg[0] = arg;
		node->nod_arg[1] = parse_multiply(paren_count, bool_flag);
	}
}


static DUDLEY_NOD parse_and( USHORT * paren_count)
{
/**************************************
 *
 *	p a r s e _ a n d
 *
 **************************************
 *
 * Functional description
 *	Parse an AND expression.
 *
 **************************************/
	DUDLEY_NOD expr, node;

	expr = parse_not(paren_count);

	if (!PARSE_match(KW_AND))
		return expr;

	node = PARSE_make_node(nod_and, 2);
	node->nod_arg[0] = expr;
	node->nod_arg[1] = parse_and(paren_count);

	return node;
}


static DUDLEY_NOD parse_field()
{
/**************************************
 *
 *	p a r s e _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Parse a qualified field name.
 *
 **************************************/
	DUDLEY_NOD field, array;
	dudley_lls* stack;

	stack = NULL;

	while (true) {
		LEX_real();
		LLS_PUSH((DUDLEY_NOD) PARSE_symbol(tok_ident), &stack);
		if (!PARSE_match(KW_DOT))
			break;
	}

	field = PARSE_make_list(stack);
	field->nod_type = nod_field_name;

	if (!PARSE_match(KW_L_BRCKET))
		return field;

/* Parse an array reference */

	stack = NULL;
	for (;;) {
		LLS_PUSH(EXPR_value(0, NULL), &stack);
		if (PARSE_match(KW_R_BRCKET))
			break;
		if (!PARSE_match(KW_COMMA))
			PARSE_error(317, dudleyGlob.DDL_token.tok_string, NULL);	/* msg 317, expected comma or right bracket, encountered \"%s\" */
	}

	array = PARSE_make_node(nod_index, 2);
	array->nod_arg[0] = field;
	array->nod_arg[1] = PARSE_make_list(stack);

	return array;
}


static DUDLEY_NOD parse_from(USHORT * paren_count,
							 bool * bool_flag)
{
/**************************************
 *
 *	p a r s e _ f r o m
 *
 **************************************
 *
 * Functional description
 *	Parse either an explicit or implicit FIRST ... FROM statement.
 *
 **************************************/
	DUDLEY_NOD node, value;

	if (PARSE_match(KW_FIRST)) {
		value = parse_primitive_value(0, NULL);
		if (!PARSE_match(KW_FROM))
			PARSE_error(239, dudleyGlob.DDL_token.tok_string, NULL);
		/* msg 239: expected FROM rse clause, encountered \"%s\" */
	}
	else {
		value = parse_primitive_value(paren_count, bool_flag);
		if (!PARSE_match(KW_FROM))
			return value;
	}

	node = PARSE_make_node(nod_from, s_stt_count);
	node->nod_arg[s_stt_value] = value;
	node->nod_arg[s_stt_rse] = EXPR_rse(false);

	if (PARSE_match(KW_ELSE))
		node->nod_arg[s_stt_default] = EXPR_value(0, NULL);

	return node;
}


static DUDLEY_NOD parse_function()
{
/**************************************
 *
 *	p a r s e _ f u n c t i o n
 *
 **************************************
 *
 * Functional description
 *	Parse a function reference.  If not a function, return NULL.
 *
 **************************************/
	SYM symbol;
	DUDLEY_NOD node;
	dudley_lls* stack;

/* Look for symbol of type function.  If we don't find it, give up */

	for (symbol = dudleyGlob.DDL_token.tok_symbol; symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_function)
			break;

	if (!symbol)
		return NULL;

	node = PARSE_make_node(nod_function, 3);
	node->nod_count = 1;
	node->nod_arg[1] = (DUDLEY_NOD) PARSE_symbol(tok_ident);
	node->nod_arg[2] = (DUDLEY_NOD) symbol->sym_object;
	stack = NULL;

	if (PARSE_match(KW_LEFT_PAREN) && !PARSE_match(KW_RIGHT_PAREN))
		for (;;) {
			LLS_PUSH(EXPR_value(0, NULL), &stack);
			if (PARSE_match(KW_RIGHT_PAREN))
				break;
			if (!PARSE_match(KW_COMMA))
				PARSE_error(240, dudleyGlob.DDL_token.tok_string, NULL);
			/* msg 240: expected comma or right parenthesis, encountered \"%s\" */
		}

	node->nod_arg[0] = PARSE_make_list(stack);

	return node;
}


static DUDLEY_NOD parse_gen_id()
{
/**************************************
 *
 *	p a r s e _ g e n _ i d
 *
 **************************************
 *
 * Functional description
 *	Parse GEN_ID expression.  Syntax is:
 *
 *		GEN_ID (<relation_name>, <value>)
 *
 **************************************/
	DUDLEY_NOD node;

	LEX_token();

	if (!PARSE_match(KW_LEFT_PAREN))
		PARSE_error(241, dudleyGlob.DDL_token.tok_string, NULL);	/* msg 241: expected left parenthesis, encountered \"%s\" */

	node = PARSE_make_node(nod_gen_id, 2);
	node->nod_count = 1;
	node->nod_arg[1] = (DUDLEY_NOD) PARSE_symbol(tok_ident);
	PARSE_match(KW_COMMA);
	node->nod_arg[0] = EXPR_value(0, NULL);
	parse_matching_paren();

	return node;
}


static CON parse_literal()
{
/**************************************
 *
 *	p a r s e _ l i t e r a l
 *
 **************************************
 *
 * Functional description
 *	Parse a literal expression.
 *
 **************************************/
	CON constant;

	const TEXT* q = dudleyGlob.DDL_token.tok_string;
	USHORT l = dudleyGlob.DDL_token.tok_length;

	if (dudleyGlob.DDL_token.tok_type == tok_quoted) {
		q++;
		l -= 2;
		constant = (CON) DDL_alloc(sizeof(con) + l);
		constant->con_desc.dsc_dtype = dtype_text;
		constant->con_desc.dsc_ttype() = ttype_ascii;
		TEXT* p = (TEXT *) constant->con_data;
		constant->con_desc.dsc_address = (UCHAR*) p;
		if (constant->con_desc.dsc_length = l)
		{
			do
			{
				*p++ = *q++;
			} while (--l);
		}
	}
	else if (dudleyGlob.DDL_token.tok_type == tok_number) {
		constant = make_numeric_constant(dudleyGlob.DDL_token.tok_string,
										 dudleyGlob.DDL_token.tok_length);
	}
	else {
		PARSE_error(242, dudleyGlob.DDL_token.tok_string, NULL);	/* msg 242: expected value expression, encountered \"%s\" */
		return NULL; // silence non initialized warning
	}

	LEX_token();

	return constant;
}


static void parse_matching_paren()
{
/**************************************
 *
 *	p a r s e _ m a t c h i n g _ p a r e n
 *
 **************************************
 *
 * Functional description
 *	Check for balancing parenthesis.  If missing, barf.
 *
 **************************************/

	if (!PARSE_match(KW_RIGHT_PAREN))
		PARSE_error(243, dudleyGlob.DDL_token.tok_string, NULL);	/* msg 243: expected right parenthesis, encountered \"%s\" */
}


static DUDLEY_NOD parse_multiply(USHORT * paren_count,
								 bool * bool_flag)
{
/**************************************
 *
 *	p a r s e _ m u l t i p l y
 *
 **************************************
 *
 * Functional description
 *	Parse the operators * and /.
 *
 **************************************/
	DUDLEY_NOD node, arg;
	enum nod_t operatr;

	node = parse_from(paren_count, bool_flag);

	while (true) {
		if (PARSE_match(KW_ASTERISK))
			operatr = nod_multiply;
		else if (PARSE_match(KW_SLASH))
			operatr = nod_divide;
		else
			return node;
		arg = node;
		node = PARSE_make_node(operatr, 2);
		node->nod_arg[0] = arg;
		node->nod_arg[1] = parse_from(paren_count, bool_flag);
	}
}


static DUDLEY_NOD parse_not( USHORT * paren_count)
{
/**************************************
 *
 *	p a r s e _ n o t
 *
 **************************************
 *
 * Functional description
 *	Parse a prefix NOT expression.
 *
 **************************************/
	DUDLEY_NOD node;

	LEX_real();

	if (!PARSE_match(KW_NOT))
		return parse_relational(paren_count);

	node = PARSE_make_node(nod_not, 1);
	node->nod_arg[0] = parse_not(paren_count);

	return node;
}


static DUDLEY_NOD parse_primitive_value(USHORT * paren_count,
										bool * bool_flag)
{
/**************************************
 *
 *	p a r s e _ p r i m i t i v e _ v a l u e
 *
 **************************************
 *
 * Functional description
 *	Pick up a value expression.  This may be either a field reference
 *	or a constant value.
 *
 **************************************/
	DUDLEY_NOD node, sub;
	CON constant;
	USHORT local_count;

	if (!paren_count) {
		local_count = 0;
		paren_count = &local_count;
	}

	TEXT *p;

	switch (PARSE_keyword()) {
	case KW_LEFT_PAREN:
		LEX_token();
		(*paren_count)++;
		if (bool_flag && *bool_flag)
			node = EXPR_boolean(paren_count);
		else
			node = EXPR_value(paren_count, bool_flag);
		parse_matching_paren();
		(*paren_count)--;
		break;

	case KW_MINUS:
		LEX_token();
		sub = parse_primitive_value(paren_count, NULL);
		if (sub->nod_type == nod_literal) {
			constant = (CON) sub->nod_arg[0];
			p = (TEXT *) constant->con_desc.dsc_address;
			switch (constant->con_desc.dsc_dtype) {
			case dtype_long:
				*(SLONG *) p = -*(SLONG *) p;
				return sub;
			case dtype_text:
				*p = '-';
				return sub;
			}
		}
		node = PARSE_make_node(nod_negate, 1);
		node->nod_arg[0] = sub;
		break;

	case KW_COUNT:
	case KW_MAX:
	case KW_MIN:
	case KW_AVERAGE:
	case KW_TOTAL:
		node = parse_statistical();
		break;

	case KW_NULL:
		LEX_token();
		node = PARSE_make_node(nod_null, 0);
		break;

	case KW_USER_NAME:
		LEX_token();
		node = PARSE_make_node(nod_user_name, 0);
		break;

	case KW_GEN_ID:
		node = parse_gen_id();
		break;

	case KW_UPPERCASE:
		LEX_token();
		sub = parse_primitive_value(0, NULL);
		node = PARSE_make_node(nod_uppercase, 1);
		node->nod_arg[0] = sub;
		break;

	default:
		if (dudleyGlob.DDL_token.tok_type == tok_ident) {
			if (!(node = parse_function()))
				node = parse_field();
			break;
		}
		node = PARSE_make_node(nod_literal, 1);
		node->nod_arg[0] = (DUDLEY_NOD) parse_literal();
	}

	return node;
}


static DUDLEY_CTX parse_relation()
{
/**************************************
 *
 *	p a r s e _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Parse a relation expression.  Syntax is:
 *
 *	[ <context_variable> IN ] <relation>
 *
 **************************************/
	SYM symbol;
	DUDLEY_CTX context;

	context = (DUDLEY_CTX) DDL_alloc(sizeof(dudley_ctx));
	context->ctx_name = symbol = PARSE_symbol(tok_ident);
	symbol->sym_type = SYM_context;
	symbol->sym_object = context;

	if (!PARSE_match(KW_IN))
		PARSE_error(244, dudleyGlob.DDL_token.tok_string, NULL);	/* msg 244: expected IN, encountered \"%s\" */

	context->ctx_relation = PARSE_relation();

	return context;
}


static DUDLEY_NOD parse_relational( USHORT * paren_count)
{
/**************************************
 *
 *	p a r s e _ r e l a t i o n a l
 *
 **************************************
 *
 * Functional description
 *	Parse a relational expression.
 *
 **************************************/
	DUDLEY_NOD node, expr2;

	if (PARSE_match(KW_ANY)) {
		node = PARSE_make_node(nod_any, 1);
		node->nod_arg[0] = EXPR_rse(false);
		return node;
	}

	if (PARSE_match(KW_UNIQUE)) {
		node = PARSE_make_node(nod_unique, 1);
		node->nod_arg[0] = EXPR_rse(false);
		return node;
	}

	bool local_flag = true;
	DUDLEY_NOD expr1 = EXPR_value(paren_count, &local_flag);
	if (dudleyGlob.DDL_token.tok_keyword == KW_RIGHT_PAREN)
		return expr1;

	bool negation = false;
	node = NULL;
	LEX_real();

	if (PARSE_match(KW_NOT)) {
		negation = true;
		LEX_real();
	}

	enum nod_t operatr;

	switch (PARSE_keyword())
	{
	case KW_EQUALS:
	case KW_EQ:
		operatr = negation ? nod_neq : nod_eql;
		negation = false;
		break;

	case KW_NE:
		operatr = negation ? nod_eql : nod_neq;
		negation = false;
		break;

	case KW_GT:
		operatr = negation ? nod_leq : nod_gtr;
		negation = false;
		break;

	case KW_GE:
		operatr = negation ? nod_lss : nod_geq;
		negation = false;
		break;

	case KW_LE:
		operatr = negation ? nod_gtr : nod_leq;
		negation = false;
		break;

	case KW_LT:
		operatr = negation ? nod_geq : nod_lss;
		negation = false;
		break;

	case KW_CONTAINING:
		operatr = nod_containing;
		break;

	case KW_MATCHES:
		LEX_token();
		expr2 = EXPR_value(0, NULL);
		if (PARSE_match(KW_USING)) {
			operatr = nod_sleuth;
			node = PARSE_make_node(nod_sleuth, 3);
			node->nod_arg[2] = EXPR_value(0, NULL);
		}
		else {
			operatr = nod_matches;
			node = PARSE_make_node(nod_matches, 2);
		}
		node->nod_arg[0] = expr1;
		node->nod_arg[1] = expr2;
		break;

	case KW_STARTS:
		LEX_token();
		PARSE_match(KW_WITH);
		operatr = nod_starts;
		node = PARSE_make_node(nod_starts, 2);
		node->nod_arg[0] = expr1;
		node->nod_arg[1] = EXPR_value(0, NULL);
		break;

	case KW_MISSING:
		LEX_token();
		operatr = nod_missing;
		node = PARSE_make_node(nod_missing, 1);
		node->nod_arg[0] = expr1;
		break;

	case KW_BETWEEN:
		LEX_token();
		operatr = nod_between;
		node = PARSE_make_node(nod_between, 3);
		node->nod_arg[0] = expr1;
		node->nod_arg[1] = EXPR_value(0, NULL);
		PARSE_match(KW_AND);
		node->nod_arg[2] = EXPR_value(0, NULL);
		break;

	default:
		for (enum nod_t* rel_ops = relationals; *rel_ops != (enum nod_t) 0; rel_ops++)
			if (expr1->nod_type == *rel_ops)
				return expr1;
		PARSE_error(245, dudleyGlob.DDL_token.tok_string, NULL);
		/* msg 245: expected relational operator, encountered \"%s\" */
		return NULL; // silence non initialized warning
	}

/* If we haven't already built a node, it must be an ordinary binary operator.
   Build it. */

	if (!node) {
		LEX_token();
		node = PARSE_make_node(operatr, 2);
		node->nod_arg[0] = expr1;
		node->nod_arg[1] = EXPR_value(paren_count, &local_flag);
	}

/* If a negation remains to be handled, zap the node under a NOT. */

	if (negation) {
		expr1 = PARSE_make_node(nod_not, 1);
		expr1->nod_arg[0] = node;
		node = expr1;
	}

/*  If the node isn't an equality, we've done.  Since equalities can be structured
    as implicit ORs, build them here. */

	if (operatr != nod_eql)
		return node;

/* We have an equality operation, which can take a number of values.  Generate
   implicit ORs */

	while (PARSE_match(KW_COMMA)) {
		PARSE_match(KW_OR);
		DUDLEY_NOD or_node = PARSE_make_node(nod_or, 2);
		or_node->nod_arg[0] = node;
		or_node->nod_arg[1] = node = PARSE_make_node(nod_eql, 2);
		node->nod_arg[0] = expr1;
		node->nod_arg[1] = EXPR_value(paren_count, &local_flag);
		node = or_node;
	}

	return node;
}


static DUDLEY_NOD parse_sort()
{
/**************************************
 *
 *	p a r s e _ s o r t
 *
 **************************************
 *
 * Functional description
 *	Parse a sort list.
 *
 **************************************/
	dudley_lls* stack;
	SSHORT direction;

	direction = 0;
	stack = NULL;

	while (true) {
		LEX_real();
		if (PARSE_match(KW_ASCENDING))
			direction = 0;
		else if (PARSE_match(KW_DESCENDING))
			direction = 1;
		LLS_PUSH(EXPR_value(0, NULL), &stack);
		LLS_PUSH((DUDLEY_NOD) (IPTR) direction, &stack);
		if (!PARSE_match(KW_COMMA))
			break;
	}

	return PARSE_make_list(stack);
}


static DUDLEY_NOD parse_statistical()
{
/**************************************
 *
 *	p a r s e _ s t a t i s t i c a l
 *
 **************************************
 *
 * Functional description
 *	Parse statistical expression.
 *
 **************************************/
	nod_types* types;
	DUDLEY_NOD node;
	enum kwwords keyword;

	keyword = PARSE_keyword();
	LEX_token();

	for (types = statisticals;; types++)
		if (types->nod_t_keyword == keyword)
			break;

	node = PARSE_make_node(types->nod_t_node, s_stt_count);

	if (node->nod_type != nod_count)
		node->nod_arg[s_stt_value] = EXPR_value(0, NULL);

	if (!PARSE_match(KW_OF))
		PARSE_error(246, dudleyGlob.DDL_token.tok_string, NULL);	/* msg 246: expected OF, encountered \"%s\" */

	node->nod_arg[s_stt_rse] = EXPR_rse(false);

	return node;
}


static void parse_terminating_parens(USHORT * paren_count,
                                     USHORT * local_count)
{
/**************************************
 *
 *	p a r s e _ t e r m i n a t i n g _ p a r e n s
 *
 **************************************
 *
 * Functional description
 *	Check for balancing parenthesis.  If missing, barf.
 *
 **************************************/

	if (*paren_count && paren_count == local_count)
	{
		do
		{
			parse_matching_paren();
		} while (--(*paren_count));
	}
}
