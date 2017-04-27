/*
 *	PROGRAM:	JRD Lock Manager
 *	MODULE:		vmslock.cpp
 *	DESCRIPTION:	VMS Lock Manager
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
#include descrip
#include psldef
#include ssdef
#include lckdef

#include "../jrd/common.h"
#include "../jrd/ibase.h"
#include "../jrd/isc_proto.h"
#include "../jrd/vmslo_proto.h"

typedef int PTR;
const SLONG EVENT_FLAG	= 15;

static SCHAR lock_types[] =
{
	0,
	LCK$K_NLMODE,
	LCK$K_CRMODE,
	LCK$K_CWMODE,
	LCK$K_PRMODE,
	LCK$K_PWMODE,
	LCK$K_EXMODE
};

// Conflict with isc.h or redefinition???
struct lock_status
{
	SSHORT lksb_status;
	SSHORT lksb_reserved;
	SLONG lksb_lock_id;
	SLONG lksb_value[4];
};

static bool lock_error(ISC_STATUS *, UCHAR *, int);
static SLONG write_data(SLONG, SLONG);


bool LOCK_convert(PTR lock_id,
				  UCHAR type,
				  SSHORT wait,
				  lock_ast_t ast_routine,
	void* ast_argument, ISC_STATUS* status_vector)
{
/**************************************
 *
 *	L O C K _ c o n v e r t
 *
 **************************************
 *
 * Functional description
 *	Perform a lock conversion, if possible.  If the lock cannot be
 *	granted immediately, either return immediately or wait depending
 *	on a wait flag.  If the lock is granted return true, otherwise
 *	return false.  Note: if the conversion would cause a deadlock,
 *	false is returned even if wait was requested.
 *
 **************************************/
	lock_status lksb;
	struct dsc$descriptor_s desc;

	lksb.lksb_lock_id = lock_id;

	int status = sys$enq(EVENT_FLAG,
					 lock_types[type],
					 &lksb,
					 wait ? LCK$M_CONVERT : LCK$M_CONVERT | LCK$M_NOQUEUE,
					 &desc, NULL,	/* Lock parent (not used) */
					 gds__completion_ast,	/* AST routine when granted */
					 (int*) ast_argument, ast_routine, NULL, NULL);

	if (status & 1)
		ISC_wait(&lksb, EVENT_FLAG);

	if ((status & 1) && ((status = lksb.lksb_status) & 1))
		return true;

	if (!wait && status == SS$_NOTQUEUED)
		status = SS$_DEADLOCK;

	return lock_error(status_vector, "sys$enq", status);
}


int LOCK_deq(PTR lock_id)
{
/**************************************
 *
 *	L O C K _ d e q
 *
 **************************************
 *
 * Functional description
 *	Release an outstanding lock.
 *
 **************************************/
	int status = sys$deq(lock_id, NULL, NULL, NULL);

/* The following is a deliberate compile error to force this to
   be fixed during the next VMS port.  Return FALSE if any error
   TRUE if everything OK. */
	return the_appropriate_value;
}


SLONG LOCK_enq(PTR prior_request,
			   PTR parent_request,
			   USHORT series,
			   const UCHAR* value,
			   USHORT length,
			   UCHAR type,
			   lock_ast_t ast_routine,
			   int *ast_argument, SLONG data, USHORT wait,
			   ISC_STATUS* status_vector
{
/**************************************
 *
 *	L O C K _ e n q
 *
 **************************************
 *
 * Functional description
 *	Enque on a lock.  If the lock can't be granted immediately,
 *	return an event count on which to wait.  If the lock can't
 *	be granted because of deadlock, return NULL.
 *
 **************************************/
	int lock_id, lock_type, flags;
	UCHAR buffer[256];
	lock_status lksb;
	struct dsc$descriptor_s desc;
	if (prior_reqeust)
	LOCK_deq(prior_request);
	UCHAR* p = buffer;
	*p++ = series;
	fb_assert(length < sizeof(buffer));
	memcpy(p, value, length);
	p += length;
	desc.dsc$b_class = DSC$K_CLASS_S;
	desc.dsc$b_dtype = DSC$K_DTYPE_T;
	desc.dsc$w_length = p - buffer;
	desc.dsc$a_pointer = buffer;
	flags = wait ? LCK$M_SYSTEM | LCK$M_VALBLK : LCK$M_SYSTEM | LCK$M_NOQUEUE;
	lock_type = lock_types[type];
	int status =
		sys$enq(EVENT_FLAG, lock_type, &lksb, flags, &desc, parent_request, gds__completion_ast,	/* AST routine when granted */
		   ast_argument, ast_routine, PSL$C_USER, NULL);
	if (status & 1)
		ISC_wait(&lksb, EVENT_FLAG);
	if ((status & 1) &&
	    (((status = lksb.lksb_status) & 1) || status == SS$_VALNOTVALID))
	{
		if (data) {
			if (lock_type == LCK$K_EXMODE)
			write_data(lksb.lksb_lock_id, data);
			else
			LOCK_write_data(lksb.lksb_lock_id, data);
		}
		return lksb.lksb_lock_id;
	}

	if (!wait && status == SS$_NOTQUEUED)
		status = SS$_DEADLOCK;
	return lock_error(status_vector, "sys$enq", status);
}


void LOCK_fini(ISC_STATUS* status_vector, PTR * owner_offset)
{
/**************************************
 *
 *	L O C K _ f i n i
 *
 **************************************
 *
 * Functional description
 *	Release the process block, any outstanding locks,
 *	and unmap the lock manager.  This is usually called
 *	by the cleanup handler.
 *
 **************************************/

   return FB_SUCCESS;
}


int LOCK_init(ISC_STATUS* status_vector,
							 bool owner_flag,
							 LOCK_OWNER_T owner_id,
							 UCHAR owner_type, SLONG * owner_handle)
{
/**************************************
 *
 *	L O C K _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize lock manager for process.
 *
 **************************************/

	return FB_SUCCESS;
}


SLONG LOCK_read_data(PTR lock_id)
{
/**************************************
 *
 *	L O C K _ r e a d _ d a t a
 *
 **************************************
 *
 * Functional description
 *	Read data associated with a lock.
 *
 **************************************/
	lock_status lksb;
	struct dsc$descriptor_s desc;
	lksb.lksb_lock_id = lock_id;
	lksb.lksb_value[0] = 0;
	int status = sys$enq(EVENT_FLAG,
					LCK$K_NLMODE, &lksb,
					LCK$M_CONVERT | LCK$M_VALBLK, &desc, NULL,	/* Lock parent (not used) */
					gds__completion_ast,	/* AST routine when granted */
					NULL, NULL, NULL, NULL); if (status & 1)
	ISC_wait(&lksb, EVENT_FLAG);
	if (!status && !(lksb.lksb_status & 1))
		return 0;
	return lksb.lksb_value[0];
}


void LOCK_re_post(lock_ast_t ast_routine,
								 int ast_argument, PTR owner_offset)
{
/**************************************
 *
 *	L O C K _ r e _ p o s t
 *
 **************************************
 *
 * Functional description
 *	Re-post an AST.
 *
 **************************************/
	int status;
	status = sys$dclast(ast_routine, ast_argument, PSL$C_USER);
}


SLONG LOCK_write_data(PTR lock_id, SLONG data)
{
/**************************************
 *
 *	L O C K _ w r i t e _ d a t a
 *
 **************************************
 *
 * Functional description
 *	Perform a conversion to exclusive in preparation
 *	for a downward conversion to write data to lock.
 *
 **************************************/
	int status;
	lock_status lksb;
	struct dsc$descriptor_s desc;
	lksb.lksb_lock_id = lock_id;
	status =
	sys$enq(EVENT_FLAG, LCK$K_EXMODE, &lksb, LCK$M_CONVERT, &desc, NULL,	/* Lock parent (not used) */
		   gds__completion_ast,	/* AST routine when granted */
		   NULL, NULL, NULL, NULL); if (status & 1)
	ISC_wait(&lksb, EVENT_FLAG);
	if (!(status & 1) || !((status = lksb.lksb_status) & 1))
		return 0;
	return write_data(lock_id, data);
}


static bool lock_error(ISC_STATUS * status_vector,
									 UCHAR* string, int code)
{
/**************************************
 *
 *	l o c k _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Report lock manager error.
 *
 **************************************/

	if (code == SS$_DEADLOCK) {
		*status_vector++ = isc_arg_gds;
		*status_vector++ = isc_deadlock;
	}

	*status_vector++ = isc_arg_gds;
	*status_vector++ = isc_sys_request;
	*status_vector++ = isc_arg_string;
	*status_vector++ = string;
	*status_vector++ = isc_arg_vms;
	*status_vector++ = code;
	*status_vector++ = 0;
	return false;
}


static SLONG write_data(SLONG lock_id, SLONG data)
{
/**************************************
 *
 *	w r i t e _ d a t a
 *
 **************************************
 *
 * Functional description
 *	Write a longword into the lock block.
 *
 **************************************/
	lock_status lksb;
	struct dsc$descriptor_s desc;
	lksb.lksb_lock_id = lock_id;
	lksb.lksb_value[0] = data;
	lksb.lksb_value[1] = 0;
	lksb.lksb_value[2] = 0;
	lksb.lksb_value[3] = 0;
	int status = sys$enqw(EVENT_FLAG,
					 LCK$K_PWMODE, &lksb,
					 LCK$M_CONVERT | LCK$M_VALBLK, &desc, NULL,	/* Lock parent (not used) */
					 gds__completion_ast,	/* AST routine when granted */
					 NULL,
					 NULL,
					 NULL,
					 NULL);
	if (!(status & 1) || !((status = lksb.lksb_status) & 1))
	return 0;
}

