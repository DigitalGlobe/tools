/*
 *	PROGRAM:	JRD Data Definition Utility
 *	MODULE:		generate.cpp
 *	DESCRIPTION:	Blr generator
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
#include "../jrd/ibase.h"
#include "../dudley/ddl.h"
#include "../jrd/acl.h"
#include "../dudley/gener_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/gdsassert.h"
#include "../dudley/trn_proto.h"

static void generate(STR, DUDLEY_NOD);
static void get_set_generator(STR, DUDLEY_NOD);

static inline void check_blr(str* blr, const int l)
{
	if (!(blr->str_current - blr->str_start + l <= blr->str_length) && !TRN_get_buffer(blr, l) )
	{
		DDL_err(289);
	}
}

int GENERATE_acl( SCL sec_class, UCHAR * buffer)
{
/**************************************
 *
 *	G E N E R A T E _ a c l
 *
 **************************************
 *
 * Functional description
 *	Generate an access control list.
 *
 **************************************/
	UCHAR* p = buffer;
	*p++ = ACL_version;

	for (SCE item = sec_class->scl_entries; item; item = item->sce_next) {
		*p++ = ACL_id_list;
		USHORT i;
		const UCHAR* const* id;
		for (i = 0, id = item->sce_idents; i < id_max; id++, i++)
		{
			const UCHAR* q = *id;
			if (q) {
				*p++ = i;
				*p++ = strlen((const char*) q);
				UCHAR c;
				while (c = *q++)
					*p++ = UPPER(c);
			}
		}
		*p++ = priv_end;
		*p++ = ACL_priv_list;
		for (i = 0; i < priv_max; i++)
		{
			if (item->sce_privileges & (1 << i))
				*p++ = i;
		}
		*p++ = id_end;
	}

	*p++ = ACL_end;

	return p - buffer;
}


void GENERATE_blr( STR blr, DUDLEY_NOD node)
{
/**************************************
 *
 *	G E N E R A T E _ b l r
 *
 **************************************
 *
 * Functional description
 *	Compile a node tree into a blr string.
 *
 **************************************/

	check_blr(blr, 1);
	blr->add_byte(blr_version4);
	generate(blr, node);
	check_blr(blr, 1);
	blr->add_byte(blr_eoc);
}


static void generate( STR blr, DUDLEY_NOD node)
{
/**************************************
 *
 *	g e n e r a t e
 *
 **************************************
 *
 * Functional description
 *	Generate blr.
 *
 **************************************/
	DUDLEY_FLD field;
	DUDLEY_REL relation;
	SYM symbol;
	CON constant;
	DUDLEY_NOD sub;
	DUDLEY_CTX context;
	SCHAR operatr;
	const char* p;
	SLONG value;
	USHORT l, pos;

	USHORT posi = 0;
	USHORT endi = node->nod_count;

	switch (node->nod_type) {
	case nod_fid:
		check_blr(blr, 4);
		blr->add_byte(blr_fid);
		blr->add_byte(0);
		blr->add_word(0);
		return;

	case nod_field:
		check_blr(blr, 4);
		if (sub = node->nod_arg[s_fld_subs])
			blr->add_byte(blr_index);
		blr->add_byte(blr_field);
		field = (DUDLEY_FLD) node->nod_arg[s_fld_field];
		context = (DUDLEY_CTX) node->nod_arg[s_fld_context];
		blr->add_byte(context->ctx_context_id);
		symbol = field->fld_name;
		blr->add_byte(l = symbol->sym_length);
		p = symbol->sym_string;
		if (l) {
			check_blr(blr, l);
			do {
				blr->add_byte(*p++);
			} while (--l);
		}
		if (sub) {
			check_blr(blr, 1);
			blr->add_byte(sub->nod_count);
			for (pos = 0; pos < sub->nod_count; pos++)
				generate(blr, sub->nod_arg[pos]);
		}
		return;

	case nod_function:
		check_blr(blr, 2);
		blr->add_byte(blr_function);
		symbol = (SYM) node->nod_arg[1];
		blr->add_byte(l = symbol->sym_length);
		p = symbol->sym_string;
		if (l) {
			check_blr(blr, l);
			do {
				blr->add_byte(*p++);
			} while (--l);
		}
		sub = node->nod_arg[0];
		check_blr(blr, 1);
		blr->add_byte(sub->nod_count);
		for (pos = 0; pos < sub->nod_count ; pos++)
			generate(blr, sub->nod_arg[pos]);
		return;

	case nod_context:
		context = (DUDLEY_CTX) node->nod_arg[0];
		relation = context->ctx_relation;
		check_blr(blr, 2);
		blr->add_byte(blr_relation);
		symbol = relation->rel_name;
		blr->add_byte(l = symbol->sym_length);
		p = symbol->sym_string;
		if (l) {
			check_blr(blr, l);
			do {
				blr->add_byte(*p++);
			} while (--l);
		}
		check_blr(blr, 1);
		blr->add_byte(context->ctx_context_id);
		return;

	case nod_gen_id:
		check_blr(blr, 2);
		blr->add_byte(blr_gen_id);
		symbol = (SYM) node->nod_arg[1];
		blr->add_byte(l = symbol->sym_length);
		p = symbol->sym_string;
		if (l) {
			check_blr(blr, l);
			do {
				blr->add_byte(*p++);
			} while (--l);
		}
		generate(blr, node->nod_arg[0]);
		return;

	case nod_set_generator:
		get_set_generator(blr, node);
		return;

	case nod_user_name:
		check_blr(blr, 1);
		blr->add_byte(blr_user_name);
		return;

	case nod_null:
		check_blr(blr, 1);
		blr->add_byte(blr_null);
		return;

	case nod_literal:
		check_blr(blr, 6);
		blr->add_byte(blr_literal);
		constant = (CON) node->nod_arg[0];
		l = constant->con_desc.dsc_length;
		switch (constant->con_desc.dsc_dtype) {
		case dtype_text:
			blr->add_byte(blr_text);
			blr->add_word(l);
			break;

		case dtype_short:
			check_blr(blr, 2);
			blr->add_byte(blr_short);
			blr->add_byte(constant->con_desc.dsc_scale);
			break;

		case dtype_long:
			check_blr(blr, 2);
			blr->add_byte(blr_long);
			blr->add_byte(constant->con_desc.dsc_scale);
			break;

		case dtype_quad:
			check_blr(blr, 2);
			blr->add_byte(blr_quad);
			blr->add_byte(constant->con_desc.dsc_scale);
			break;

		case dtype_real:
			check_blr(blr, 1);
			blr->add_byte(blr_float);
			break;

		case dtype_double:
			check_blr(blr, 1);
			blr->add_byte(blr_double);
			break;

		case dtype_timestamp:
			check_blr(blr, 1);
			blr->add_byte(blr_timestamp);
			break;

		case dtype_int64:
		case dtype_sql_time:
		case dtype_sql_date:
		default:
			DDL_err(95);
			// msg 95: GENERATE_blr: dtype not supported
		}
		p = (const char*) constant->con_data;
		switch (constant->con_desc.dsc_dtype) {
		case dtype_short:
			value = *(SSHORT *) p;
			check_blr(blr, 2);
			blr->add_word(value);
			break;

		case dtype_long:
			value = *(SLONG *) p;
			check_blr(blr, 4);
			blr->add_word(value);
			blr->add_word(value >> 16);
			break;

		case dtype_quad:
		case dtype_timestamp:
			value = *(SLONG *) p;
			check_blr(blr, 8);
			blr->add_word(value);
			blr->add_word(value >> 16);
			value = *(SLONG *) (p + 4);
			blr->add_word(value);
			blr->add_word(value >> 16);
			break;

		default:
			if (l) {
				check_blr(blr, l);
				do {
					blr->add_byte(*p++);
				} while (--l);
			}
		}
		return;

	case nod_rse:
		check_blr(blr, 2);
		blr->add_byte(blr_rse);
		sub = node->nod_arg[s_rse_contexts];
		blr->add_byte(sub->nod_count);
		for (pos = 0; pos < sub->nod_count ; pos++)
			generate(blr, sub->nod_arg[pos]);
		if (sub = node->nod_arg[s_rse_first]) {
			check_blr(blr, 1);
			blr->add_byte(blr_first);
			generate(blr, sub);
		}
		if (sub = node->nod_arg[s_rse_boolean]) {
			check_blr(blr, 1);
			blr->add_byte(blr_boolean);
			generate(blr, sub);
		}
		if (sub = node->nod_arg[s_rse_sort]) {
			check_blr(blr, 2);
			blr->add_byte(blr_sort);
			blr->add_byte(sub->nod_count / 2);
			for (pos = 0; pos < sub->nod_count ; pos += 2) {
				check_blr(blr, 1);
				blr->add_byte((sub->nod_arg[pos + 1]) ? blr_descending : blr_ascending);
				generate(blr, sub->nod_arg[pos]);
			}
		}
		if (sub = node->nod_arg[s_rse_reduced]) {
			check_blr(blr, 2);
			blr->add_byte(blr_project);
			blr->add_byte(sub->nod_count / 2);
			for (pos = 0; pos < sub->nod_count ; pos += 2)
				generate(blr, sub->nod_arg[pos]);
		}
		check_blr(blr, 1);
		blr->add_byte(blr_end);
		return;

	case nod_count:
	case nod_max:
	case nod_min:
	case nod_total:
	case nod_average:
	case nod_from:
		switch (node->nod_type) {
		case nod_count:
			operatr = blr_count;
			break;
		case nod_max:
			operatr = blr_maximum;
			break;
		case nod_min:
			operatr = blr_minimum;
			break;
		case nod_total:
			operatr = blr_total;
			break;
		case nod_average:
			operatr = blr_average;
			break;
		case nod_from:
			operatr = (node->nod_arg[s_stt_default]) ? blr_via : blr_from;
			break;
		default:
			// operatr not assigned
			fb_assert(false);
			return;
		}
		check_blr(blr, 1);
		blr->add_byte(operatr);
		generate(blr, node->nod_arg[s_stt_rse]);
		if (sub = node->nod_arg[s_stt_value])
			generate(blr, sub);
		if (sub = node->nod_arg[s_stt_default])
			generate(blr, sub);
		return;

	case nod_any:
		check_blr(blr, 1);
		blr->add_byte(blr_any);
		generate(blr, node->nod_arg[0]);
		return;

	case nod_unique:
		check_blr(blr, 1);
		blr->add_byte(blr_unique);
		generate(blr, node->nod_arg[0]);
		return;

	case nod_if:
		check_blr(blr, 1);
		blr->add_byte(blr_if);
		generate(blr, node->nod_arg[posi++]);
		generate(blr, node->nod_arg[posi++]);
		if (node->nod_arg[posi])
			generate(blr, node->nod_arg[posi]);
		else {
			check_blr(blr, 1);
			blr->add_byte(blr_end);
		}
		return;

	case nod_list:
		check_blr(blr, 1);
		blr->add_byte(blr_begin);
		for (; posi < endi; posi++)
			generate(blr, node->nod_arg[posi]);
		check_blr(blr, 1);
		blr->add_byte(blr_end);
		return;

	case nod_abort:
		check_blr(blr, 2);
		blr->add_byte(blr_leave);
		blr->add_byte((IPTR) node->nod_arg[0]);
		return;

	case nod_erase:
		check_blr(blr, 2);
		blr->add_byte(blr_erase);
		context = (DUDLEY_CTX) node->nod_arg[0];
		blr->add_byte(context->ctx_context_id);
		return;

	case nod_modify:
		check_blr(blr, 3);
		blr->add_byte(blr_modify);
		context = (DUDLEY_CTX) node->nod_arg[s_mod_old_ctx];
		blr->add_byte(context->ctx_context_id);
		context = (DUDLEY_CTX) node->nod_arg[s_mod_new_ctx];
		blr->add_byte(context->ctx_context_id);
		generate(blr, node->nod_arg[s_mod_action]);
		return;

	case nod_eql:
		operatr = blr_eql;
		break;
	case nod_neq:
		operatr = blr_neq;
		break;
	case nod_gtr:
		operatr = blr_gtr;
		break;
	case nod_geq:
		operatr = blr_geq;
		break;
	case nod_leq:
		operatr = blr_leq;
		break;
	case nod_lss:
		operatr = blr_lss;
		break;
	case nod_between:
		operatr = blr_between;
		break;
	case nod_matches:
		operatr = blr_matching;
		break;
	case nod_containing:
		operatr = blr_containing;
		break;
	case nod_starts:
		operatr = blr_starting;
		break;
	case nod_missing:
		operatr = blr_missing;
		break;
	case nod_and:
		operatr = blr_and;
		break;
	case nod_or:
		operatr = blr_or;
		break;
	case nod_not:
		operatr = blr_not;
		break;
	case nod_add:
		operatr = blr_add;
		break;
	case nod_subtract:
		operatr = blr_subtract;
		break;
	case nod_multiply:
		operatr = blr_multiply;
		break;
	case nod_divide:
		operatr = blr_divide;
		break;
	case nod_negate:
		operatr = blr_negate;
		break;
	case nod_concatenate:
		operatr = blr_concatenate;
		break;
	case nod_for:
		operatr = blr_for;
		break;
	case nod_assignment:
		operatr = blr_assignment;
		break;
	case nod_store:
		operatr = blr_store;
		break;
	case nod_post:
		operatr = blr_post;
		break;
	case nod_uppercase:
		operatr = blr_upcase;
		break;
	case nod_lowercase:
		operatr = blr_lowcase;
		break;
	case nod_sleuth:
		operatr = blr_matching2;
		break;
//	case nod_substr:
//		operatr = blr_substring;
//		break;

	default:
		DDL_err(96);
		// msg 96: GENERATE_blr: node not supported
		return;
	}

// If the user has given us something that has the form
//
//     field {EQ} NULL
//          {NE}
//
//   transform it into
//
//     field [NOT] MISSING

	if ((operatr == blr_eql || operatr == blr_neq) &&
		(node->nod_arg[posi]->nod_type == nod_null || node->nod_arg[posi + 1]->nod_type == nod_null))
	{
		if (operatr == blr_neq) {
			check_blr(blr, 1);
			blr->add_byte(blr_not);
		}
		operatr = blr_missing;
		if (node->nod_arg[posi]->nod_type == nod_null)
			posi++;
		endi = posi + 1;
	}

// Fall thru on reasonable stuff

	check_blr(blr, 1);
	blr->add_byte(operatr);
	for (; posi < endi; posi++)
		generate(blr, node->nod_arg[posi]);
}


static void get_set_generator( STR blr, DUDLEY_NOD node)
{
/**************************************
 *
 *	g e t _ s e t _ g e n e r a t o r
 *
 **************************************
 *
 * Functional description
 *	produce blr for set_generator. The
 *	mutable characteristics are the
 *	generator name and the increment.
 *	The rest is canned.
 **************************************/
	static const UCHAR gen_prologue[] = {
		blr_begin, blr_message, 0, 1, 0, blr_long, 0,
		blr_send, 0, blr_begin, blr_assignment, blr_gen_id,
	};
	static const UCHAR gen_epilogue[] = {
		blr_parameter, 0, 0, 0, blr_end, blr_end
	};

/* copy the beginning of the blr into the buffer */

	int l = sizeof(gen_prologue);
	const UCHAR* p = gen_prologue;
	check_blr(blr, l);
	do {
		blr->add_byte(*p++);
	} while (--l);

/* stuff in the name length and the name */

	SYM symbol = (SYM) node->nod_arg[1];
	check_blr(blr, 1);
	blr->add_byte(l = symbol->sym_length);
	p = (const UCHAR*) symbol->sym_string;
	if (l) {
		check_blr(blr, l);
		do {
			blr->add_byte(*p++);
		} while (--l);
	}

/* now for the increment value */

	check_blr(blr, 7);
	blr->add_byte(blr_literal);
	CON constant = (CON) node->nod_arg[0]->nod_arg[0];
	blr->add_byte(blr_long);
	blr->add_byte(constant->con_desc.dsc_scale);
	p = constant->con_data;
	const SLONG value = *(SLONG *) p;
	blr->add_word(value);
	blr->add_word(value >> 16);

/* complete the string */
	l = sizeof(gen_epilogue);
	p = gen_epilogue;
	check_blr(blr, l);
	do {
		blr->add_byte(*p++);
	} while (--l);
}

