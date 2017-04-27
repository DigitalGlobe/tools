/*
 *	PROGRAM:	InterBase Access Method
 *	MODULE:		functions.cpp
 *	DESCRIPTION:	External entrypoint definitions
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
#include "../jrd/common.h"
#include <stdio.h>
#include <string.h>
#include "../jrd/jrd.h"  // For MAXPATHLEN Bug #126614
#include "../jrd/license.h"
#include "../jrd/tra.h"
#include "../jrd/dsc_proto.h"
#include "../jrd/fun_proto.h"
#include "../jrd/pag_proto.h"
#include "../jrd/thread_proto.h"
#include "../jrd/trace/TraceManager.h"
#include "../jrd/trace/TraceObjects.h"

using namespace Jrd;
using namespace Firebird;

struct FN
{
	const char* fn_module;
	const char* fn_entrypoint;
	FPTR_INT fn_function;
};


static int test(const long*, char*);
static DSC* ni(DSC*, DSC*);
static SLONG* byteLen(const dsc*);
static SLONG set_context(const vary* ns_vary, const vary* name_vary, const vary* value_vary);
static vary* get_context(const vary* ns_vary, const vary* name_vary);

// Context variable function names. Do not forget to change functions.h too if changing
static const char
	RDB_GET_CONTEXT[] = "RDB$GET_CONTEXT",
	RDB_SET_CONTEXT[] = "RDB$SET_CONTEXT";

// Namespace names
static const char
	SYSTEM_NAMESPACE[] = "SYSTEM",
	USER_SESSION_NAMESPACE[] = "USER_SESSION",
	USER_TRANSACTION_NAMESPACE[] = "USER_TRANSACTION";

// System variables names
static const char
	// global and database wise items
	ENGINE_VERSION[] = "ENGINE_VERSION",
	DATABASE_NAME[] = "DB_NAME",
	// connection wise items
	SESSION_ID_NAME[] = "SESSION_ID",
	NETWORK_PROTOCOL_NAME[] = "NETWORK_PROTOCOL",
	CLIENT_ADDRESS_NAME[] = "CLIENT_ADDRESS",
	CLIENT_PID_NAME[] = "CLIENT_PID",
	CLIENT_PROCESS_NAME[] = "CLIENT_PROCESS",
	CURRENT_USER_NAME[] = "CURRENT_USER",
	CURRENT_ROLE_NAME[] = "CURRENT_ROLE",
	// transaction wise items
	TRANSACTION_ID_NAME[] = "TRANSACTION_ID",
	ISOLATION_LEVEL_NAME[] = "ISOLATION_LEVEL",
	LOCK_TIMEOUT_NAME[] = "LOCK_TIMEOUT",
	READ_ONLY_NAME[] = "READ_ONLY";

// Isolation values modes
static const char
	READ_COMMITTED_VALUE[] = "READ COMMITTED",
	CONSISTENCY_VALUE[] = "CONSISTENCY",
	SNAPSHOT_VALUE[] = "SNAPSHOT";

// Boolean values
static const char
	FALSE_VALUE[] = "FALSE",
	TRUE_VALUE[] = "TRUE";


#define FUNCTION(ROUTINE, FUNCTION_NAME, MODULE_NAME, ENTRYPOINT, RET_ARG) \
	{MODULE_NAME, ENTRYPOINT, (FPTR_INT) ROUTINE},
#define END_FUNCTION
#define FUNCTION_ARGUMENT(MECHANISM, TYPE, SCALE, LENGTH, SUB_TYPE, CHARSET, PRECISION, CHAR_LENGTH)

static const FN isc_functions[] =
{
#ifndef DECLARE_EXAMPLE_IUDF_AUTOMATICALLY
	{"test_module", "test_function", (FPTR_INT) test}, // see functions.h
#endif
	{"test_module", "ni", (FPTR_INT) ni},
	{"test_module", "ns", (FPTR_INT) ni},
	{"test_module", "nn", (FPTR_INT) ni},
#ifndef DECLARE_EXAMPLE_IUDF_AUTOMATICALLY
	{"test_module", "byte_len", (FPTR_INT) byteLen}, // see functions.h
#endif

#include "../jrd/functions.h"

	{0, 0, 0}
};


FPTR_INT FUNCTIONS_entrypoint(const char* module, const char* entrypoint)
{
/**************************************
 *
 *	F U N C T I O N S _ e n t r y p o i n t
 *
 **************************************
 *
 * Functional description
 *	Lookup function in hardcoded table.  The module and
 *	entrypoint names are null terminated, but may contain
 *	insignificant trailing blanks.
 *
 **************************************/
	char temp[MAXPATHLEN + 128];  /* Bug #126614 Fix */

	char* p = temp;

	while (*module && *module != ' ')
		*p++ = *module++;

	*p++ = 0;
	const char* const ep = p;

	while (*entrypoint && *entrypoint != ' ')
		*p++ = *entrypoint++;

	*p = 0;

	for (const FN* function = isc_functions; function->fn_module; ++function)
	{
		if (!strcmp(temp, function->fn_module) && !strcmp(ep, function->fn_entrypoint))
		{
			return function->fn_function;
		}
	}

	return 0;
}

static vary* make_result_str(const char* str, size_t str_len)
{
	vary *result_vary = (vary*) IbUtil::alloc(str_len + 2);
	result_vary->vary_length = str_len;
	memcpy(result_vary->vary_string, str, result_vary->vary_length);
	return result_vary;
}

static vary* make_result_str(const Firebird::string& str)
{
	return make_result_str(str.c_str(), str.length());
}

vary* get_context(const vary* ns_vary, const vary* name_vary)
{
	// Complain if namespace or variable name is null
	if (!ns_vary || !name_vary)
		ERR_post(Arg::Gds(isc_ctx_bad_argument) << Arg::Str(RDB_GET_CONTEXT));

	thread_db* tdbb = JRD_get_thread_data();

	Database* dbb;
	Attachment* attachment;
	jrd_tra* transaction;

	// See if JRD thread data structure looks sane for occasion
	if (!tdbb ||
		!(dbb = tdbb->getDatabase()) ||
		!(attachment = tdbb->getAttachment()) ||
		!(transaction = tdbb->getTransaction()))
	{
		fb_assert(false);
		return NULL;
	}

	const Firebird::string ns_str(ns_vary->vary_string, ns_vary->vary_length);
	const Firebird::string name_str(name_vary->vary_string, name_vary->vary_length);

	Database::SyncGuard dsGuard(dbb);

	// Handle system variables
	if (ns_str == SYSTEM_NAMESPACE)
	{
		if (name_str == ENGINE_VERSION)
		{
			Firebird::string version;
			version.printf("%s.%s.%s", FB_MAJOR_VER, FB_MINOR_VER, FB_REV_NO);

			return make_result_str(version);
		}

		if (name_str == DATABASE_NAME)
		{
			return make_result_str(dbb->dbb_database_name.ToString());
		}

		if (name_str == SESSION_ID_NAME)
		{
			Firebird::string session_id;
			const SLONG att_id = PAG_attachment_id(tdbb);
			session_id.printf("%d", att_id);
			return make_result_str(session_id);
		}

		if (name_str == NETWORK_PROTOCOL_NAME)
		{
			if (attachment->att_network_protocol.isEmpty())
				return NULL;

			return make_result_str(attachment->att_network_protocol);
		}

		if (name_str == CLIENT_ADDRESS_NAME)
		{
			if (attachment->att_remote_address.isEmpty())
				return NULL;

			return make_result_str(attachment->att_remote_address);
		}

		if (name_str == CLIENT_PID_NAME)
		{
			if (!attachment->att_remote_pid)
				return NULL;

			Firebird::string client_pid;
			client_pid.printf("%d", attachment->att_remote_pid);
			return make_result_str(client_pid);
		}

		if (name_str == CLIENT_PROCESS_NAME)
		{
			if (attachment->att_remote_process.isEmpty())
				return NULL;

			return make_result_str(attachment->att_remote_process.ToString());
		}

		if (name_str == CURRENT_USER_NAME)
		{
			if (!attachment->att_user || attachment->att_user->usr_user_name.isEmpty())
				return NULL;

			return make_result_str(attachment->att_user->usr_user_name);
		}

		if (name_str == CURRENT_ROLE_NAME)
		{
			if (!attachment->att_user || attachment->att_user->usr_sql_role_name.isEmpty())
				return NULL;

			return make_result_str(attachment->att_user->usr_sql_role_name);
		}

		if (name_str == TRANSACTION_ID_NAME)
		{
			Firebird::string transaction_id;
			transaction_id.printf("%d", transaction->tra_number);
			return make_result_str(transaction_id);
		}

		if (name_str == ISOLATION_LEVEL_NAME)
		{
			const char* isolation;

			if (transaction->tra_flags & TRA_read_committed)
				isolation = READ_COMMITTED_VALUE;
			else if (transaction->tra_flags & TRA_degree3)
				isolation = CONSISTENCY_VALUE;
			else
				isolation = SNAPSHOT_VALUE;

			return make_result_str(isolation, strlen(isolation));
		}

		if (name_str == LOCK_TIMEOUT_NAME)
		{
			Firebird::string lock_timeout;
			lock_timeout.printf("%d", transaction->tra_lock_timeout);
			return make_result_str(lock_timeout);
		}

		if (name_str == READ_ONLY_NAME)
		{
			const char* read_only = (transaction->tra_flags & TRA_readonly) ?
				TRUE_VALUE : FALSE_VALUE;

			return make_result_str(read_only, strlen(read_only));
		}

		// "Context variable %s is not found in namespace %s"
		ERR_post(Arg::Gds(isc_ctx_var_not_found) << Arg::Str(name_str) <<
													Arg::Str(ns_str));
	}

	// Handle user-defined variables
	if (ns_str == USER_SESSION_NAMESPACE)
	{
		Firebird::string result_str;

		if (!attachment->att_context_vars.get(name_str, result_str))
			return NULL;

		return make_result_str(result_str);
	}

	if (ns_str == USER_TRANSACTION_NAMESPACE)
	{
		Firebird::string result_str;

		if (!transaction->tra_context_vars.get(name_str, result_str))
			return NULL;

		return make_result_str(result_str);
	}

	// "Invalid namespace name %s passed to %s"
	ERR_post(Arg::Gds(isc_ctx_namespace_invalid) << Arg::Str(ns_str) << Arg::Str(RDB_GET_CONTEXT));
	return NULL;
}

static SLONG set_context(const vary* ns_vary, const vary* name_vary, const vary* value_vary)
{
	// Complain if namespace or variable name is null
	if (!ns_vary || !name_vary)
		ERR_post(Arg::Gds(isc_ctx_bad_argument) << Arg::Str(RDB_SET_CONTEXT));

	thread_db* tdbb = JRD_get_thread_data();

	Database* dbb;
	Attachment* attachment;
	jrd_tra* transaction;

	// See if JRD thread data structure looks sane for occasion
	if (!tdbb ||
		!(dbb = tdbb->getDatabase()) ||
		!(attachment = tdbb->getAttachment()) ||
		!(transaction = tdbb->getTransaction()))
	{
		fb_assert(false);
		return 0;
	}

	const Firebird::string ns_str(ns_vary->vary_string, ns_vary->vary_length);
	const Firebird::string name_str(name_vary->vary_string, name_vary->vary_length);

	Database::SyncGuard dsGuard(dbb);

	Firebird::StringMap* context_vars = NULL;

	bool result = false;

	if (ns_str == USER_SESSION_NAMESPACE)
	{
		context_vars = &attachment->att_context_vars;
	}
	else if (ns_str == USER_TRANSACTION_NAMESPACE)
	{
		if (!transaction)
		{
			fb_assert(false);
			return 0;
		}

		context_vars = &transaction->tra_context_vars;
	}
	else
	{
		// "Invalid namespace name %s passed to %s"
		ERR_post(Arg::Gds(isc_ctx_namespace_invalid) << Arg::Str(ns_str) << Arg::Str(RDB_SET_CONTEXT));
	}

	if (!value_vary) {
		result = context_vars->remove(name_str);
	}
	else if (context_vars->count() == MAX_CONTEXT_VARS)
	{
		string* rc = context_vars->get(name_str);
		if (rc)
		{
			rc->assign(value_vary->vary_string, value_vary->vary_length);
			result = true;
		}
		else
			ERR_post(Arg::Gds(isc_ctx_too_big)); // "Too many context variables"
	}
	else
	{
		result = context_vars->put(name_str, Firebird::string(value_vary->vary_string, value_vary->vary_length));
	}

	if (attachment->att_trace_manager->needs().event_set_context)
	{
		TraceConnectionImpl conn(attachment);
		TraceTransactionImpl tran(transaction);

		const Firebird::string* value_str = NULL;
		if (value_vary)
			value_str = context_vars->get(name_str);

		TraceContextVarImpl ctxvar(ns_str.c_str(), name_str.c_str(),
			value_str ? value_str->c_str() : NULL);

		attachment->att_trace_manager->event_set_context(&conn, &tran, &ctxvar);
	}

	return (SLONG) result;
}

static int test(const long* n, char *result)
{
/**************************************
 *
 *	t e s t
 *
 **************************************
 *
 * Functional description
 *	Sample extern function.  Defined in database by:
 *
 *	QLI:
 *	define function test module_name "test_module" entry_point "test_function"
 *	    long by reference //by value,   CVC: BY VALUE is deprecated for input params
 *	    char [20] by reference return_argument;
 *	ISQL:
 *	declare external function test
 * 	int null, // CVC: with NULL signaling
 *	char(20) returns parameter 2
 *	entry_point 'test_function' module_name 'test_module';
 *
 **************************************/

	if (n)
		sprintf(result, "%ld is a number", *n);
	else
		sprintf(result, "is NULL");
	const char* const end = result + 20;

	while (*result)
		result++;

	while (result < end)
		*result++ = ' ';

	return 0;
}


static dsc* ni(dsc* v, dsc* v2)
{
	return v ? v : v2;
}


// byteLen: return the length in bytes of a given argument. For NULL, return NULL, too.
// v = input descriptor
// rc = return value, allocated dynamically. To be freed by the engine.
// The declaration through SQL is:
// declare external function sys_byte_len
// int by descriptor
// returns int free_it
// entry_point 'byte_len' module_name 'test_module';
static SLONG* byteLen(const dsc* v)
{
	if (!v || !v->dsc_address || (v->dsc_flags & DSC_null))
		return 0;

	SLONG& rc = *(SLONG*) IbUtil::alloc(sizeof(SLONG));
	switch (v->dsc_dtype)
	{
	case dtype_text:
		{
			const UCHAR* const ini = v->dsc_address;
			const UCHAR* end = ini + v->dsc_length;
			while (ini < end && *--end == ' ')
				; // empty loop body
			rc = end - ini + 1;
			break;
		}
	case dtype_cstring:
		{
			rc = 0;
			for (const UCHAR* p = v->dsc_address; *p; ++p, ++rc)
				; // empty loop body
			break;
		}
	case dtype_varying:
		rc = reinterpret_cast<const vary*>(v->dsc_address)->vary_length;
		break;
	default:
		rc = DSC_string_length(v);
		break;
	}

	return &rc;
}
