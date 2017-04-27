/*
 *	PROGRAM:	JRD Data Definition Utility
 *	MODULE:		expand.cpp
 *	DESCRIPTION:	Expand field references to get context.
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
#include <string.h>
#include "../dudley/ddl.h"
#include "../jrd/ibase.h"
#include "../jrd/gdsassert.h"
#include "../dudley/ddl_proto.h"
#include "../dudley/expan_proto.h"
#include "../dudley/hsh_proto.h"
#include "../dudley/parse_proto.h"
#include "../jrd/gds_proto.h"
#include "../common/classes/SafeArg.h"

using MsgFormat::SafeArg;


static void expand_action(ACT);
static void expand_error(USHORT, const SafeArg& arg);
static void expand_field(DUDLEY_FLD);
static void expand_global_field(DUDLEY_FLD);
static void expand_index(ACT);
static void expand_relation(DUDLEY_REL);
static void expand_trigger(DUDLEY_TRG);
static DUDLEY_FLD field_context(DUDLEY_NOD, dudley_lls*, DUDLEY_CTX *);
static DUDLEY_FLD field_search(DUDLEY_NOD, dudley_lls*, DUDLEY_CTX *);
static DUDLEY_CTX lookup_context(SYM, dudley_lls*);
static DUDLEY_FLD lookup_field(DUDLEY_FLD);
static DUDLEY_FLD lookup_global_field(DUDLEY_FLD);
static DUDLEY_REL lookup_relation(DUDLEY_REL);
static DUDLEY_TRG lookup_trigger(DUDLEY_TRG);
static DUDLEY_CTX make_context(const TEXT *, DUDLEY_REL, USHORT);
static DUDLEY_NOD resolve(DUDLEY_NOD, dudley_lls*, dudley_lls*);
static void resolve_rse(DUDLEY_NOD, dudley_lls**);

static SSHORT context_id;
static dudley_lls* request_context;

void EXP_actions()
{
/**************************************
 *
 *	E X P _ a c t i o n s
 *
 **************************************
 *
 * Functional description
 *	Expand the output of the parser.
 *	Look for field references and put
 *	them in appropriate context.
 *
 **************************************/
	ACT action;

	if (!dudleyGlob.DDL_actions)
		return;

	for (action = dudleyGlob.DDL_actions; action; action = action->act_next)
		if (!(action->act_flags & ACT_ignore))
			expand_action(action);
}


static void expand_action( ACT action)
{
/**************************************
 *
 *	e x p a n d _ a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Generate an error message.
 *
 **************************************/

/* Set up an environment to catch syntax errors.  If one occurs, scan looking
   for semi-colon to continue processing. */

	try {

	dudleyGlob.DDL_line = action->act_line;

	switch (action->act_type) {
	case act_a_field:
		expand_field((DUDLEY_FLD) action->act_object);
		break;

	case act_m_field:
	case act_d_field:
		lookup_field((DUDLEY_FLD) action->act_object);
		break;

	case act_d_filter:
/*	lookup_filter (action->act_object);
*/ break;

	case act_a_function:
	case act_a_function_arg:
		break;

	case act_d_function:
/*	lookup_function (action->act_object);
*/ break;

	case act_a_gfield:
		expand_global_field((DUDLEY_FLD) action->act_object);
		break;

	case act_m_gfield:
		lookup_global_field((DUDLEY_FLD) action->act_object);
		expand_global_field((DUDLEY_FLD) action->act_object);
		break;

	case act_d_gfield:
		lookup_global_field((DUDLEY_FLD) action->act_object);
		break;

	case act_a_index:
		expand_index((ACT) action->act_object);
		break;

	case act_m_relation:
		lookup_relation((DUDLEY_REL) action->act_object);
	case act_a_relation:
		expand_relation((DUDLEY_REL) action->act_object);
		break;

	case act_d_relation:
		lookup_relation((DUDLEY_REL) action->act_object);
		break;

	case act_d_trigger:
		lookup_trigger((DUDLEY_TRG) action->act_object);
		break;

	case act_m_trigger:
		lookup_trigger((DUDLEY_TRG) action->act_object);

	case act_a_trigger:
		expand_trigger((DUDLEY_TRG) action->act_object);
		break;

	case act_c_database:
	case act_m_database:
	case act_a_filter:
	case act_m_index:
	case act_d_index:
	case act_a_security:
	case act_d_security:
	case act_a_trigger_msg:
	case act_m_trigger_msg:
	case act_d_trigger_msg:
	case act_a_type:
	case act_m_type:
	case act_d_type:
	case act_grant:
	case act_revoke:
	case act_a_shadow:
	case act_d_shadow:
	case act_a_generator:
	case act_s_generator:
		break;

	default:
		DDL_msg_put(97);	/* msg 97: object can not be resolved */
	}

	}	// try
	catch (const Firebird::Exception&) {
	}

	return;
}


static void expand_error(USHORT number, const SafeArg& arg)
{
/**************************************
 *
 *	e x p a n d _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Generate an error message.
 *
 **************************************/

	DDL_err(number, arg);
	Firebird::LongJump::raise();
}


static void expand_field( DUDLEY_FLD field)
{
/**************************************
 *
 *	e x p a n d _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Expand a field definition, resolving references
 *	to its global field and other fields.
 *
 **************************************/
	SYM name;

	if (!field->fld_source) {
		name = field->fld_name;
		if (!(field->fld_source = HSH_typed_lookup(name->sym_string,
												   name->sym_length,
												   SYM_global)))
				expand_error(98, SafeArg() << name->sym_string);	/* msg 98: Global field %s is not defined */
	}

	if (!field->fld_source_field)
		field->fld_source_field = (DUDLEY_FLD) field->fld_source->sym_object;
}


static void expand_global_field( DUDLEY_FLD field)
{
/**************************************
 *
 *	e x p a n d _ g l o b a l _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Expand a global field definition, resolving references
 *	to other fields and stuff. Mark the default context
 *	with this field so we can identify ourselves if we
 *	find ourselves in a computation or validation.
 *
 **************************************/
	DUDLEY_CTX context;

	if (!request_context || !(context = lookup_context(0, request_context))) {
		context = (DUDLEY_CTX) DDL_alloc(sizeof(dudley_ctx));
		LLS_PUSH((DUDLEY_NOD) context, &request_context);
	}

	context->ctx_field = field;

	field->fld_computed = resolve(field->fld_computed, request_context, 0);
	field->fld_validation = resolve(field->fld_validation, request_context, 0);

	context->ctx_field = 0;
}


static void expand_index( ACT action)
{
/**************************************
 *
 *	e x p a n d _ i n d e x
 *
 **************************************
 *
 * Functional description
 *	Expand an index definition, resolving references
 *	to other fields.
 *
 **************************************/

	request_context = NULL;
}


static void expand_relation( DUDLEY_REL relation)
{
/**************************************
 *
 *	e x p a n d _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Expand a relation definition, resolving references
 *	to fields and contexts.
 *
 **************************************/
	DUDLEY_CTX my_context, context;
	DUDLEY_NOD rse;

	context_id = 0;
	dudley_lls* contexts = NULL;
	request_context = NULL;

	if (!relation->rel_rse)
		LLS_PUSH((DUDLEY_NOD) make_context(NULL, relation, context_id++),
				 &request_context);
	else {
		rse = relation->rel_rse;
		contexts = (dudley_lls*) rse->nod_arg[s_rse_contexts];
		my_context = lookup_context(NULL, contexts);
		my_context->ctx_view_rse = true;

		/* drop view context from context stack & build the request stack */

		dudley_lls* stack;
		for (stack = NULL; contexts; contexts = contexts->lls_next)
			LLS_PUSH(contexts->lls_object, &stack);

		while (stack) {
			context = (DUDLEY_CTX) LLS_POP(&stack);
			if (!context->ctx_view_rse)
				LLS_PUSH((DUDLEY_NOD) context, &request_context);
		}
		LLS_PUSH((DUDLEY_NOD) my_context, &contexts);
		resolve_rse(relation->rel_rse, &contexts);

		/* Put view context back on stack for global field resolution to follow */

		LLS_PUSH((DUDLEY_NOD) my_context, &request_context);
		my_context->ctx_view_rse = false;
	}
}


static void expand_trigger( DUDLEY_TRG trigger)
{
/**************************************
 *
 *	e x p a n d _ t r i g g e r
 *
 **************************************
 *
 * Functional description
 *	Expand a trigger definition, resolving references
 *	to fields and contexts.
 *
 **************************************/
	DUDLEY_CTX old, new_ctx;
	DUDLEY_REL relation;

	context_id = 2;
	old = new_ctx = NULL;
	request_context = NULL;
	dudley_lls* contexts = NULL;
	dudley_lls* update = NULL;

	relation = trigger->trg_relation;

	if ((trigger->trg_type != trg_store) && (trigger->trg_type != trg_post_store)) {
		if (!old)
			old = make_context("OLD", relation, 0);
		LLS_PUSH((DUDLEY_NOD) old, &contexts);
	}

	if ((trigger->trg_type != trg_erase) && (trigger->trg_type != trg_pre_erase)) {
		if (!new_ctx)
			new_ctx = make_context("NEW", relation, 1);
		LLS_PUSH((DUDLEY_NOD) new_ctx, &contexts);
		LLS_PUSH((DUDLEY_NOD) new_ctx, &update);
	}

	resolve(trigger->trg_statement, contexts, update);
	context_id = 0;
}


static DUDLEY_FLD field_context( DUDLEY_NOD node, dudley_lls* contexts, DUDLEY_CTX * output_context)
{
/**************************************
 *
 *	f i e l d _ c o n t e x t
 *
 **************************************
 *
 * Functional description
 *	Lookup a field reference, guessing the
 *	context.  Since by now all field references
 *	ought to be entered in the hash table, this is
 *	pretty easy.  We may be looking up a global
 *	field reference, in which case the context
 *	relation will be null.
 *
 *
 **************************************/
	DUDLEY_FLD field;
	DUDLEY_CTX context;
	SYM name, symbol;

/* If the field is unqualified, resolve it to the default context */

	if (node->nod_count == 1) {
		context = lookup_context(0, contexts);
		name = (SYM) node->nod_arg[0];
	}
	else if (node->nod_count == 2) {
		context = lookup_context((SYM) node->nod_arg[0], contexts);
		name = (SYM) node->nod_arg[1];
	}
	else
		return NULL;

	if (!context)
		return NULL;

	*output_context = context;

	symbol = HSH_lookup(name->sym_string, name->sym_length);

	for (; symbol; symbol = symbol->sym_homonym) {
		if (context->ctx_relation) {
			if (symbol->sym_type == SYM_field) {
				field = (DUDLEY_FLD) symbol->sym_object;
				if (field->fld_relation == context->ctx_relation)
					return field;
			}
		}
		else if (symbol->sym_type == SYM_global)
			return (DUDLEY_FLD) symbol->sym_object;
	}

	if (context->ctx_relation)
		expand_error(99, SafeArg() << name->sym_string <<
					 context->ctx_relation->rel_name->sym_string);
	/* msg 99: field %s doesn't exist in relation %s */
	else
		expand_error(100, SafeArg() << name->sym_string);
	/* msg 100: field %s doesn't exist */

	return NULL;
}


static DUDLEY_FLD field_search( DUDLEY_NOD node, dudley_lls* contexts, DUDLEY_CTX * output_context)
{
/**************************************
 *
 *	f i e l d _ s e a r c h
 *
 **************************************
 *
 * Functional description
 *	We have, or believe we have, a field
 *	reference that has to be resolved
 *	iteratively.  The context indicated is
 *	the current context.  Get to there, then
 *	work backward.
 *
 **************************************/
	DUDLEY_FLD field;
	DUDLEY_CTX context, old_context;
	SYM name, symbol, ctx_name;

	name = (SYM) node->nod_arg[1];
	symbol = HSH_lookup(name->sym_string, name->sym_length);
	ctx_name = (SYM) node->nod_arg[0];

	if (symbol) {
		/* wander down to the old context level */
		for (; contexts; contexts = contexts->lls_next) {
			old_context = (DUDLEY_CTX) contexts->lls_object;
			if ((name = old_context->ctx_name) &&
				!strcmp(ctx_name->sym_string, name->sym_string))
				break;
		}

		name = symbol;
		for (contexts = contexts->lls_next; contexts;
			 contexts = contexts->lls_next)
		{
			context = (DUDLEY_CTX) contexts->lls_object;
			if (context->ctx_relation && !context->ctx_view_rse) {
				*output_context = context;
				for (symbol = name; symbol; symbol = symbol->sym_homonym) {
					if (symbol->sym_type == SYM_field) {
						field = (DUDLEY_FLD) symbol->sym_object;
						if (field->fld_relation == context->ctx_relation)
							return field;
					}
				}
			}
		}
	}

	expand_error(101, SafeArg() << name->sym_string);	/* msg 101: field %s can't be resolved */
	return NULL;
}


static DUDLEY_CTX lookup_context( SYM symbol, dudley_lls* contexts)
{
/**************************************
 *
 *	l o o k u p _ c o n t e x t
 *
 **************************************
 *
 * Functional description
 *	Search the context stack.  If a context name is given, consider
 *	only named contexts; otherwise return the first unnamed context.
 *	In either case, if nothing matches, return NULL.
 *
 **************************************/
	DUDLEY_CTX context;
	SYM name;

/* If no name is given, look for a nameless one. */

	if (!symbol) {
		for (; contexts; contexts = contexts->lls_next) {
			context = (DUDLEY_CTX) contexts->lls_object;
			if (!context->ctx_name && !context->ctx_view_rse)
				return context;
		}
		return NULL;
	}

/* Other search by name */

	for (; contexts; contexts = contexts->lls_next) {
		context = (DUDLEY_CTX) contexts->lls_object;
		if ((name = context->ctx_name) && !strcmp(name->sym_string, symbol->sym_string))
			return context;
	}

	return NULL;
}


static DUDLEY_FLD lookup_field( DUDLEY_FLD old_field)
{
/**************************************
 *
 *	l o o k u p _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Lookup a field reference, from a modify or
 *	delete field statement, and make sure we
 *	found the thing originally.
 *	context.  Since by now all field references
 *	ought to be entered in the hash table, this is
 *	pretty easy.
 *
 **************************************/
	DUDLEY_REL relation;
	SYM symbol, name;
	DUDLEY_FLD field;

	if (old_field->fld_source)
		lookup_global_field(old_field);

	name = old_field->fld_name;
	relation = old_field->fld_relation;

	symbol = HSH_lookup(name->sym_string, name->sym_length);

	for (; symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_field) {
			field = (DUDLEY_FLD) symbol->sym_object;
			if (field->fld_relation == relation)
				return field;
		}

	expand_error(102, SafeArg() << name->sym_string << relation->rel_name->sym_string);
	/* msg 102: field %s isn't defined in relation %s */

	return NULL;
}


static DUDLEY_FLD lookup_global_field( DUDLEY_FLD field)
{
/**************************************
 *
 *	l o o k u p _ g l o b a l _ f i e l d
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	SYM symbol, name;

/* Find symbol */

	name = (field->fld_source) ? field->fld_source : field->fld_name;
	if ((symbol = HSH_typed_lookup(name->sym_string, name->sym_length, SYM_global)))
		return (DUDLEY_FLD) symbol->sym_object;

	expand_error(103, SafeArg() << name->sym_string);	/* msg 103: global field %s isn't defined */

	return NULL;
}


static DUDLEY_REL lookup_relation( DUDLEY_REL relation)
{
/**************************************
 *
 *	l o o k u p _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	SYM symbol, name;

/* Find symbol */

	name = (relation->rel_name);
	if ((symbol = HSH_typed_lookup(name->sym_string, name->sym_length, SYM_relation)) &&
		symbol->sym_object)
	{
		return (DUDLEY_REL) symbol->sym_object;
	}

	expand_error(104, SafeArg() << name->sym_string);	/* msg 104: relation %s isn't defined */

	return NULL;
}


static DUDLEY_TRG lookup_trigger( DUDLEY_TRG trigger)
{
/**************************************
 *
 *	l o o k u p _ t r i g g e r
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	SYM symbol, name;

/* Find symbol */

	name = (trigger->trg_name);
	if ((symbol = HSH_typed_lookup(name->sym_string, name->sym_length, SYM_trigger)))
		return (DUDLEY_TRG) symbol->sym_object;

	expand_error(105, SafeArg() << name->sym_string);	/* msg 105: trigger %s isn't defined */

	return NULL;
}


static DUDLEY_CTX make_context( const TEXT * string, DUDLEY_REL relation, USHORT id)
{
/**************************************
 *
 *	m a k e _ c o n t e x t
 *
 **************************************
 *
 * Functional description
 *	Make context for trigger.
 *
 **************************************/
	DUDLEY_CTX context;
	SYM symbol;

	context = (DUDLEY_CTX) DDL_alloc(sizeof(dudley_ctx));
	context->ctx_context_id = id;
	context->ctx_relation = relation;

	if (string) {
		context->ctx_name = symbol = (SYM) DDL_alloc(SYM_LEN);
		symbol->sym_object = context;
		symbol->sym_string = string;
		symbol->sym_length = strlen(string);
		symbol->sym_type = SYM_context;
	}

	return context;
}


static DUDLEY_NOD resolve( DUDLEY_NOD node, dudley_lls* right, dudley_lls* left)
{
/**************************************
 *
 *	r e s o l v e
 *
 **************************************
 *
 * Functional description
 *	Resolve field names in an expression.  This is called after
 *	an expression has been parsed and after the appropriate context
 *	variables have been entered in the hash table.  If an unqualified
 *	field is to be resolved to a known relation, a relation block
 *	is passed in.
 *
 **************************************/
	DUDLEY_NOD field, sub;
	SYM symbol, name;
	DUDLEY_CTX context, old_context;
	TEXT name_string[65], *p;

	if (!node)
		return NULL;

	switch (node->nod_type) {
	case nod_field_name:
	case nod_over:
		break;

	case nod_rse:
		expand_error(106, SafeArg());	// msg 106: bugcheck
		return node;

	case nod_field:
	case nod_literal:
		return node;

	case nod_count:
	case nod_max:
	case nod_min:
	case nod_total:
	case nod_average:
	case nod_from:
		if (sub = node->nod_arg[s_stt_default])
			node->nod_arg[s_stt_default] = resolve(sub, right, 0);
		resolve_rse(node->nod_arg[s_stt_rse], &right);
		if (node->nod_arg[s_stt_value])
			node->nod_arg[s_stt_value] = resolve(node->nod_arg[s_stt_value], right, 0);
		return node;

	case nod_unique:
	case nod_any:
		resolve_rse(node->nod_arg[s_stt_rse], &right);
		return node;

	case nod_for:
		resolve_rse(node->nod_arg[s_for_rse], &right);
		resolve(node->nod_arg[s_for_action], right, left);
		return node;

	case nod_store:
		context = (DUDLEY_CTX) node->nod_arg[s_store_rel];
		context->ctx_context_id = ++context_id;
		name = context->ctx_relation->rel_name;
		if (!HSH_typed_lookup(name->sym_string, name->sym_length, SYM_relation))
			expand_error(107, SafeArg() << name->sym_string);
			// msg 107: relation %s is not defined */
		LLS_PUSH((DUDLEY_NOD) context, &left);
		sub = PARSE_make_node(nod_context, 1);
		sub->nod_arg[0] = (DUDLEY_NOD) context;
		node->nod_arg[s_store_rel] = sub;
		resolve(node->nod_arg[s_store_action], right, left);
		return node;

	case nod_erase:
		symbol = (SYM) node->nod_arg[0];
		if (!(node->nod_arg[0] = (DUDLEY_NOD) lookup_context(symbol, right)))
			expand_error(108, SafeArg() << symbol->sym_string);
			// msg 108: context %s is not defined
		return node;

	case nod_modify:
		symbol = (SYM) node->nod_arg[s_mod_old_ctx];
		if (!(old_context = lookup_context(symbol, right)))
			expand_error(108, SafeArg() << symbol->sym_string);
			// msg 108: context %s is not defined
		node->nod_arg[s_mod_old_ctx] = (DUDLEY_NOD) old_context;
		context = (DUDLEY_CTX) DDL_alloc(sizeof(dudley_ctx));
		node->nod_arg[s_mod_new_ctx] = (DUDLEY_NOD) context;
		context->ctx_name = symbol;
		context->ctx_context_id = ++context_id;
		context->ctx_relation = old_context->ctx_relation;
		LLS_PUSH((DUDLEY_NOD) context, &left);
		node->nod_arg[s_mod_action] = resolve(node->nod_arg[s_mod_action], right, left);
		return node;

	case nod_index:
		sub = resolve(node->nod_arg[1], right, left);
		field = resolve(node->nod_arg[0], right, left);
		field->nod_arg[s_fld_subs] = sub;
		return field;

	case nod_assignment:
		node->nod_arg[1] = resolve(node->nod_arg[1], left, 0);
		node->nod_arg[0] = resolve(node->nod_arg[0], right, 0);
		return node;

	case nod_post:
		node->nod_arg[0] = resolve(node->nod_arg[0], right, 0);
		return node;

	default:
		for (int pos = 0; pos < node->nod_count; pos++)
			node->nod_arg[pos] = resolve(node->nod_arg[pos], right, left);
		return node;
	}

// We've dropped thru to resolve a field name node.  If we can find it,
// make up a field node, otherwise generate an error.  We may be looking
// for either a local or a global field, which is known by whether the
// context has a relation or not.  Do something reasonable in either case

	DUDLEY_FLD fld;

	switch (node->nod_type) {
	case nod_field_name:
		fld = field_context(node, right, &context);
		break;

	case nod_over:
		fld = field_search(node, right, &context);
		break;

	default:
		// fld not defined
		fb_assert(false);
		return NULL;
	}

	if (fld) {
		if (context->ctx_field &&
			((!context->ctx_relation && fld == context->ctx_field) ||
			 (context->ctx_relation && fld->fld_source &&
			  !(strcmp(fld->fld_source->sym_string,
					context->ctx_field->fld_name-> sym_string)))))
		{
			return PARSE_make_node(nod_fid, 0);
		}
		field = PARSE_make_node(nod_field, s_fld_count);
		field->nod_arg[s_fld_name] = node;
		field->nod_arg[s_fld_field] = (DUDLEY_NOD) fld;
		field->nod_arg[s_fld_context] = (DUDLEY_NOD) context;
		return field;
	}

	p = name_string;
	for (int pos = 0; pos < node->nod_count ; pos++) {
		if (symbol = (SYM) node->nod_arg[pos]) {
			const char* q = symbol->sym_string;
			while (*q)
				*p++ = *q++;
			*p++ = '.';
		}
	}

	p[-1] = 0;
	expand_error(109, SafeArg() << name_string);
	// msg 109:  Can't resolve field \"%s\"

	return node;
}


static void resolve_rse( DUDLEY_NOD rse, dudley_lls** stack)
{
/**************************************
 *
 *	r e s o l v e _ r s e
 *
 **************************************
 *
 * Functional description
 *	Resolve record selection expression, augmenting
 *	context stack.  At the same time, put a context
 *	node in front of every context and build a list
 *	out of the whole thing;
 *
 **************************************/
	DUDLEY_NOD sub;
	DUDLEY_CTX context;
	SYM name, symbol;

/* Handle FIRST <n> clause in original context */

	if (sub = rse->nod_arg[s_rse_first])
		rse->nod_arg[s_rse_first] = resolve(sub, *stack, 0);

	dudley_lls* contexts = (dudley_lls*) rse->nod_arg[s_rse_contexts];

	dudley_lls* temp;
	for (temp = NULL; contexts;)
		LLS_PUSH(LLS_POP(&contexts), &temp);

	for (contexts = temp; contexts; contexts = contexts->lls_next) {
		context = (DUDLEY_CTX) contexts->lls_object;
		name = context->ctx_relation->rel_name;
		symbol = HSH_typed_lookup(name->sym_string, name->sym_length,
										SYM_relation);
		if (!symbol || !symbol->sym_object)
		{
			expand_error(110, SafeArg() << name->sym_string);
			// msg 110: relation %s is not defined
		}
		else
			context->ctx_relation = (DUDLEY_REL) symbol->sym_object;
		LLS_PUSH((DUDLEY_NOD) context, stack);
	}

	if (sub = rse->nod_arg[s_rse_boolean])
		rse->nod_arg[s_rse_boolean] = resolve(sub, *stack, 0);

	if (sub = rse->nod_arg[s_rse_sort])
		for (int pos = 0; pos < sub->nod_count; pos += 2)
			sub->nod_arg[pos] = resolve(sub->nod_arg[pos], *stack, 0);

	if (sub = rse->nod_arg[s_rse_reduced])
		for (int pos = 0; pos < sub->nod_count; pos += 2)
			sub->nod_arg[pos] = resolve(sub->nod_arg[pos], *stack, 0);

	while (temp) {
		context = (DUDLEY_CTX) LLS_POP(&temp);
		if (!context->ctx_view_rse) {
			context->ctx_context_id = ++context_id;
			sub = PARSE_make_node(nod_context, 1);
			sub->nod_arg[0] = (DUDLEY_NOD) context;
			LLS_PUSH(sub, &contexts);
		}
	}

	rse->nod_arg[s_rse_contexts] = PARSE_make_list(contexts);
}
