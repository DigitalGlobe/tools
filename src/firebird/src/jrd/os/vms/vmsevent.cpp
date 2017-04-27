/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		vmsevent.cpp
 *	DESCRIPTION:	Event Manager
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
#include "gen/iberror.h"
#include "../jrd/gdsassert.h"
#include "../jrd/event_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"

#include descrip
#include lckdef
#include ssdef

const int MAX_EVENT_BUFFER	= 65500;


/* Dummy global section header */

struct evh
{
	SLONG evh_length;
};

typedef evh *EVH;

/* Session block */

struct ses
{
	struct ses *ses_next;		/* Next active session */
	struct vms_req *ses_requests;	/* Outstanding requests in session */
	struct rint *ses_interests;	/* Historical interests */
};

typedef ses *SES;

/* Event block */

struct evnt
{
	struct evnt *evnt_next;		/* Next lock */
	struct evnt *evnt_parent;	/* Parent lock, if any */
	struct evnt *evnt_offspring;	/* Offspring locks, if any */
	struct rint *evnt_interests;	/* Interests in event */
	struct lksb evnt_lksb;		/* Event lock status block */
	SLONG evnt_count;			/* Last know lock count */
	USHORT evnt_length;			/* Length of event string */
	TEXT evnt_name[1];
};
typedef evnt *EVNT;

/* Request block */

struct vms_req
{
	struct vms_req *req_next;		/* Next request in session */
	struct ses *req_session;	/* Parent session */
	struct rint *req_interests;	/* Request interests */
	FPTR_EVENT_CALLBACK req_ast;	/* Associated AST (zero is fired) */
	void* req_ast_arg;			/* Argument for ast */
	SLONG req_request_id;		/* Request id */
};
typedef vms_req *VMS_REQ;

/* Request interest block */

struct rint
{
	VMS_REQ rint_request;			/* Parent request block */
	EVNT rint_event;			/* Parent event block */
	struct rint *rint_req_interests;	/* Other interests of request */
	struct rint *rint_evnt_interests;	/* Other interests for event */
	SLONG rint_count;			/* Threadhold count */
};

typedef rint *RINT;


static SES global_sessions;
static EVNT global_parent_events;
static SLONG global_request_id;
static POKE global_pokes;
static bool global_thread_started;
static bool global_delivery_flag;

static UCHAR *alloc(USHORT);
static void blocking_ast(EVNT);
static void delete_event(EVNT);
static void delete_request(VMS_REQ);
static void deliver(EVNT);
static void deliver_request(VMS_REQ);
static void delivery_thread();
static ISC_STATUS error(ISC_STATUS *, TEXT *, ISC_STATUS);
static EVNT find_event(USHORT, const TEXT*, EVNT);
static void free(SCHAR *);
static RINT historical_interest(SES, EVNT);
static EVNT make_event(USHORT, const TEXT*, EVNT);
static void poke_ast(POKE);
static bool request_completed(VMS_REQ);
static int return_ok(ISC_STATUS *);


int EVENT_cancel(SLONG request_id)
{
/**************************************
 *
 *	E V E N T _ c a n c e l
 *
 **************************************
 *
 * Functional description
 *	Cancel an outstanding event.
 *
 **************************************/
	for (SES session = global_sessions; session; session = session->ses_next)
		for (VMS_REQ request = session->ses_requests; request;
			 request = request->req_next)
		{
			if (request->req_request_id == request_id) {
				delete_request(request);
				return;
			}
		}
}


SLONG EVENT_create_session(ISC_STATUS status_vector)
{
/**************************************
 *
 *	E V E N T _ c r e a t e _ s e s s i o n
 *
 **************************************
 *
 * Functional description
 *	Create session.
 *
 **************************************/
	SES session = (SES) alloc(sizeof(struct ses));
	if (!session) {
		fb_assert(false);			/* No error handling */
		return 0L;
	}
	session->ses_next = global_sessions;
	global_sessions = session;

	const SLONG id = (SLONG) session;

	return id;
}


void EVENT_delete_session(SLONG session_id)
{
/**************************************
 *
 *	E V E N T _ d e l e t e _ s e s s i o n
 *
 **************************************
 *
 * Functional description
 *	Delete a session.
 *
 **************************************/
	SES session = (SES) session_id;

/* Delete all requests */

	while (session->ses_requests)
		delete_request(session->ses_requests);

/* Delete any historical interests */

	RINT interest;
	while (interest = session->ses_interests) {
		session->ses_interests = interest->rint_req_interests;
		EVNT event = interest->rint_event;
		for (RINT* int_ptr = &event->evnt_interests; *int_ptr;
			 int_ptr = &(*int_ptr)->rint_req_interests)
		{
			if (*int_ptr == interest) {
				*int_ptr = interest->rint_req_interests;
				break;
			}
		}
		free(interest);
		if (!event->evnt_interests)
			delete_event(event);
	}

/* Delete from global list of sessions */

	for (SES* ptr = &global_sessions; *ptr; ptr = &(*ptr)->ses_next)
		if (*ptr == session) {
			*ptr = session->ses_next;
			break;
		}

	free(session);
}


EVH EVENT_init(ISC_STATUS* status_vector, bool server_flag)
{
/**************************************
 *
 *	E V E N T _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize for access to shared global region.  If "server_flag" is true,
 *	create region if it doesn't exist.  Return address of header if region
 *	exits, otherwise return NULL.
 *
 **************************************/

	return_ok(status_vector);

	return (EVH) TRUE;
}


int EVENT_post(ISC_STATUS* status_vector,
			   USHORT major_length,
			   const TEXT* major,
			   USHORT minor_length,
			   const TEXT* minor,
			   USHORT count)
{
/**************************************
 *
 *	E V E N T _ p o s t
 *
 **************************************
 *
 * Functional description
 *	Post an event.
 *
 **************************************/

/* Get parent lock.  If it isn't held, we will get an invalid lock block */

	struct dsc$descriptor desc;
	ISC_make_desc(major, &desc, major_length);

	lock_status lksb;
	int status = sys$enq(0,			/* event flag */
					 LCK$K_CRMODE,	/* lock mode */
					 &lksb,		/* Lock status block */
					 LCK$M_SYSTEM,	/* flags */
					 &desc,		/* resource name */
					 0,			/* parent id */
					 0,			/* ast address */
					 0,			/* ast argument */
					 0,			/* blocking ast */
					 0,			/* access mode */
					 0);

	if (!(status & 1))
		return error(status_vector, "sys$enqw", status);

/* Find a free poke block to use */

	POKE poke;
	for (poke = global_pokes; poke; poke = poke->poke_next)
		if (!poke->poke_use_count)
			break;

	if (!poke) {
		poke = (POKE) alloc(sizeof(struct poke));
		/* FREE: unknown */
		if (!poke)				/* NOMEM: */
			return error(status_vector, "gds__alloc", 0);
		poke->poke_next = global_pokes;
		global_pokes = poke;
	}

	++poke->poke_use_count;

/* If the parent lock looks kosher, try the next level down */

	poke->poke_value = count;
	poke->poke_parent_id = lksb.lksb_lock_id;
	ISC_make_desc(minor, &desc, minor_length);

	status = sys$enq(0,			/* event flag */
					 LCK$K_PWMODE,	/* lock mode */
					 &poke->poke_lksb,	/* Lock status block */
					 LCK$M_SYSTEM | LCK$M_VALBLK,	/* flags */
					 &desc,		/* resource name */
					 poke->poke_parent_id,	/* parent id */
					 poke_ast,	/* ast address */
					 poke,		/* ast argument */
					 0,			/* blocking ast */
					 0,			/* access mode */
					 0);

	if (!(status & 1))
		return error(status_vector, "sys$enqw", status);

	return return_ok(status_vector);
}


SLONG EVENT_que(ISC_STATUS * status_vector,
				SLONG session_id,
				USHORT string_length,
				const TEXT* string,
				USHORT events_length,
				const UCHAR* events,
				FPTR_EVENT_CALLBACK ast_routine,
				void *ast_arg)
{
/**************************************
 *
 *	E V E N T _ q u e
 *
 **************************************
 *
 * Functional description
 *	Que an event.
 *
 **************************************/
	SES session = (SES) session_id;

	if (!global_thread_started) {
		global_thread_started = true;
		gds__thread_start(delivery_thread, 0, THREAD_high, THREAD_ast);
	}

/* Cleanup any residual stuff */

	for (VMS_REQ next = session->ses_requests; next;)
		if (next->req_ast)
			next = next->req_next;
		else {
			VMS_REQ request = next;
			next = next->req_next;
			delete_request(request);
		}

/* Allocate request block */

	VMS_REQ request = (VMS_REQ) alloc(sizeof(struct vms_req));
/* FREE: unknown */
	if (!request)				/* NOMEM: */
		return error(status_vector, "gds__alloc", 0);

	request->req_session = session;
	request->req_next = session->ses_requests;
	session->ses_requests = request;

	request->req_ast = ast_routine;
	request->req_ast_arg = ast_arg;
	const SLONG id = ++global_request_id;
	request->req_request_id = id;

/* Find parent block */

	EVNT parent = find_event(string_length, string, 0);
	if (!parent)
		parent = make_event(string_length, string, 0);

/* Process event block */

	RINT* ptr = &request->req_interests;
	const UCHAR* p = events + 1;
	const UCHAR* const end = events + events_length;
	bool flag = false;

	while (p < end) {
		const USHORT count = *p++;

		/* The data in the event block may have trailing blanks.  Strip them off. */

		const UCHAR* find_end;
		for (find_end = p + count; --find_end >= p && *find_end == ' ';);
		const USHORT len = find_end - p + 1;
		EVNT event = find_event(len, (const char*) p, parent);
		if (!event)
			event = make_event(len, (const char*) p, parent);
		p += count;
		RINT interest = historical_interest(session, event);
		if (interest) {
			for (RINT* ptr2 = &session->ses_interests; *ptr2;
				 ptr2 = &(*ptr2)->rint_req_interests)
			{
				if (*ptr2 == interest) {
					*ptr2 = interest->rint_req_interests;
					break;
				}
			}
		}
		else {
			interest = (RINT) alloc(sizeof(struct vms_req));
			/* FREE: unknown */
			if (!interest)		/* NOMEM: */
				return error(status_vector, "gds__alloc", 0);
			interest->rint_evnt_interests = event->evnt_interests;
			event->evnt_interests = interest;
			interest->rint_event = event;
		}
		*ptr = interest;
		ptr = &interest->rint_req_interests;
		interest->rint_request = request;
		interest->rint_count = gds__vax_integer(p, 4);
		p += 4;
		if (interest->rint_count <= event->evnt_count)
			flag = true;
	}

	if (flag)
		sys$dclast(blocking_ast, event, 0);

	return_ok(status_vector);

	return id;
}


static UCHAR *alloc(USHORT size)
{
/**************************************
 *
 *	a l l o c
 *
 **************************************
 *
 * Functional description
 *	Allocate and zero some memory.
 *
 **************************************/
	UCHAR* const block = gds__alloc((SLONG) size);
/* FREE: handled by caller */
/* NOMEM: handled by caller */
	if (!block)
		return block;

	memset(block, 0, size);
	return block;
}


static void blocking_ast(EVNT event)
{
/**************************************
 *
 *	b l o c k i n g _ a s t
 *
 **************************************
 *
 * Functional description
 *	Somebody else is trying to post a lock.
 *
 **************************************/
	lock_status* lksb = &event->evnt_lksb;

/* Initially down grade the lock to let the other guy complete */

	int status = sys$enqw(0,		/* event flag */
					  LCK$K_NLMODE,	/* lock mode */
					  lksb,		/* Lock status block */
					  LCK$M_CONVERT,	/* flags */
					  0,		/* resource name */
					  0,		/* parent id */
					  0,		/* ast address */
					  0,		/* ast argument */
					  0,		/* blocking ast */
					  0,		/* access mode */
					  0);

/* Now try to upgrade lock */

	status = sys$enq(0,			/* event flag */
					 LCK$K_PRMODE,	/* lock mode */
					 lksb,		/* Lock status block */
					 LCK$M_CONVERT | LCK$M_VALBLK,	/* flags */
					 0,			/* resource name */
					 0,			/* parent id */
					 deliver,	/* ast address */
					 event,		/* ast argument */
					 blocking_ast,	/* blocking ast */
					 0,			/* access mode */
					 0);
}


static void delete_event(EVNT event)
{
/**************************************
 *
 *	d e l e t e _ e v e n t
 *
 **************************************
 *
 * Functional description
 *	Delete an unused and unloved event.
 *
 **************************************/

/* Delete associated lock */

	int status = sys$deq(event->evnt_lksb.lksb_lock_id, 0, 0, 0);

	EVNT* ptr;
	EVNT parent = event->evnt_parent;
	if (parent)
		ptr = &parent->evnt_offspring;
	else
		ptr = &global_parent_events;

	for (; *ptr; ptr = &(*ptr)->evnt_next)
		if (*ptr == event) {
			*ptr = event->evnt_next;
			break;
		}

	free(event);

	if (parent && !parent->evnt_offspring)
		delete_event(parent);
}


static void delete_request(VMS_REQ request)
{
/**************************************
 *
 *	d e l e t e _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Release an unwanted and unloved request.
 *
 **************************************/
	SES session = request->req_session;

	RINT interest;
	while (interest = request->req_interests) {
		request->req_interests = interest->rint_req_interests;
		if (historical_interest(session, interest->rint_event)) {
			EVNT event = interest->rint_event;
			for (RINT* ptr = &event->evnt_interests; *ptr;
				 ptr = &(*ptr)->rint_evnt_interests)
			{
				if (*ptr == interest) {
					*ptr = interest->rint_evnt_interests;
					break;
				}
			}
			free(interest);
		}
		else {
			interest->rint_req_interests = session->ses_interests;
			session->ses_interests = interest;
			interest->rint_request = NULL;
		}
	}

	for (VMS_REQ* req_ptr = &session->ses_requests; *req_ptr;
		 req_ptr = &(*req_ptr)->req_next)
	{
		if (*req_ptr == request) {
			*req_ptr = request->req_next;
			break;
		}
	}

	free(request);
}


static void deliver(EVNT event)
{
/**************************************
 *
 *	d e l i v e r
 *
 **************************************
 *
 * Functional description
 *	We've been poked -- deliver any satisfying requests.
 *
 **************************************/
	event->evnt_count = event->evnt_lksb.lksb_value[0];

	for (RINT interest = event->evnt_interests; interest;
		 interest = interest->rint_evnt_interests)
	{
		if (VMS_REQ request = interest->rint_request)
			if (request->req_ast && request_completed(request))
				global_delivery_flag = true;
	}

	gds__completion_ast();
}


static void deliver_request(VMS_REQ request)
{
/**************************************
 *
 *	d e l i v e r _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Request has been satisfied, send updated event block to user, then
 *	Clean up request.
 *
 **************************************/
	FPTR_EVENT_CALLBACK ast = request->req_ast;
	void* arg = request->req_ast_arg;

	UCHAR buffer[512]
	UCHAR* event_buffer = buffer;
	UCHAR* p = event_buffer;
	const UCHAR* const end = buffer + sizeof(buffer);
	*p++ = 1;

/* Loop thru interest block picking up event name, counts, and unlinking
   stuff */

	for (RINT interest = request->req_interests; interest;
		 interest = interest->rint_req_interests)
	{
		EVNT event = interest->rint_event;
		if (end < p + event->evnt_length + 5) {
			UCHAR *new_buffer;
			/* Running out of space - allocate some more and copy it over */
			fb_assert(event_buffer == buffer);	/* we're in this block only once */
			new_buffer = gds__alloc((SLONG) MAX_EVENT_BUFFER);
			/* FREE: at procedure return */
			if (!new_buffer) {	/* NOMEM: */
				gds__log("failed to post all events");
				break;			/* exit loop and send what we have */
			}
			event_buffer = new_buffer;
			memcpy(event_buffer, buffer, p - buffer);
			p = event_buffer + (p - buffer);
			end = event_buffer + MAX_EVENT_BUFFER;
		}
		*p++ = event->evnt_length;
		memcpy(p, event->evnt_name, event->evnt_length);
		p += event->evnt_length;
		const SLONG count = event->evnt_count + 1;
		*p++ = count;
		*p++ = count >> 8;
		*p++ = count >> 16;
		*p++ = count >> 24;
	}

	(*ast) (arg, p - buffer, buffer);
	if (event_buffer != buffer)
		gds__free(event_buffer);
}


static void delivery_thread()
{
/**************************************
 *
 *	d e l i v e r y _ t h r e a d
 *
 **************************************
 *
 * Functional description
 *	Look for events to deliver.
 *
 **************************************/
	for (;;) {
		global_delivery_flag = false;
		for (SES session = global_sessions; session; session = session->ses_next)
			for (VMS_REQ request = session->ses_requests; request;
				 request = request->req_next)
			{
				if (request->req_ast && request_completed(request))
				{
					deliver_request(request);
					request->req_ast = NULL;
				}
			}
		gds__thread_wait(delivery_wait, 0);
	}
}


static int delivery_wait()
{
/**************************************
 *
 *	d e l i v e r y _ w a i t
 *
 **************************************
 *
 * Functional description
 *	See if the deliver thread should wake up.
 *
 **************************************/
	return global_delivery_flag;
}


static ISC_STATUS error(ISC_STATUS * status_vector,
						TEXT * string,
						ISC_STATUS status)
{
/**************************************
 *
 *	e r r o r
 *
 **************************************
 *
 * Functional description
 *	Things suck.
 *
 **************************************/

	*status_vector++ = isc_arg_gds;
	*status_vector++ = isc_sys_request;
	*status_vector++ = isc_arg_string;
	*status_vector++ = (ISC_STATUS) string;
	*status_vector++ = isc_arg_vms;
	*status_vector++ = status;
	*status_vector++ = isc_arg_end;
	return isc_sys_request;
}


static EVNT find_event(USHORT length,
					   const TEXT* string,
					   EVNT parent)
{
/**************************************
 *
 *	f i n d _ e v e n t
 *
 **************************************
 *
 * Functional description
 *	Lookup an event.
 *
 **************************************/
	EVNT event = parent ? parent->evnt_offspring : global_parent_events;
	for (; event; event = event->evnt_next)
		if (event->evnt_length == length &&
			!strncmp(string, event->evnt_name, length))
		{
			return event;
		}
	return NULL;
}


static void free(SCHAR * block) {
/**************************************
 *
 *	f r e e
 *
 **************************************
 *
 * Functional description
 *	Release a block.
 *
 **************************************/

	gds__free(block);
}


static RINT historical_interest(SES session,
								EVNT event)
{
/**************************************
 *
 *	h i s t o r i c a l _ i n t e r e s t
 *
 **************************************
 *
 * Functional description
 *	Find a historical interest, if any, of an event with a session.
 *
 **************************************/
	RINT interest;
	for (RINT* ptr = &session->ses_interests; interest = *ptr;
		 ptr = &(*ptr)->rint_req_interests)
	{
		if (interest->rint_event == event)
			return interest;
	}
	return NULL;
}


static EVNT make_event(USHORT length,
					   const TEXT* string,
					   EVNT parent)
{
/**************************************
 *
 *	m a k e _ e v e n t
 *
 **************************************
 *
 * Functional description
 *	Allocate an link in an event.
 *
 **************************************/
	EVNT event = (EVNT) alloc(sizeof(struct evnt) + length);
	if (!event)
		return NULL;

	EVNT* ptr;
	SLONG parent_id;
	if (parent) {
		ptr = &parent->evnt_offspring;
		parent_id = parent->evnt_lksb.lksb_lock_id;
	}
	else
	{
		ptr = &global_parent_events;
		parent_id = 0;
	}

	event->evnt_next = *ptr;
	*ptr = event;
	event->evnt_parent = parent;
	event->evnt_length = length;
	strncpy(event->evnt_name, string, length);
/* Request VMS lock on event */
	struct dsc$descriptor desc;
	ISC_make_desc(string, &desc, length);
	lock_status* lksb = &event->evnt_lksb;
	int status = sys$enqw(0,	/* event flag */
							LCK$K_PRMODE,	/* lock mode */
							lksb,	/* Lock status block */
							LCK$M_SYSTEM | LCK$M_VALBLK,	/* flags */
							&desc,	/* resource name */
							parent_id,	/* parent id */
							0,	/* ast address */
							event,	/* ast argument */
							blocking_ast,	/* blocking ast */
							0,	/* access mode */
							0);
	event->evnt_count = lksb->lksb_value[0];
/* If the lock block is invalid, clean it up immediately */
	if ((status & 1) &&
		event->evnt_lksb.lksb_status == SS$_VALNOTVALID)
	{
		status = sys$enqw(0,	/* event flag */
					  LCK$K_PWMODE,	/* lock mode */
					  lksb,	/* Lock status block */
					  LCK$M_CONVERT,	/* flags */
					  0,	/* resource name */
					  0,	/* parent id */
					  0,	/* ast address */
					  event,	/* ast argument */
					  blocking_ast,	/* blocking ast */
					  0,	/* access mode */
					  0);
		event->evnt_count = lksb->lksb_value[0] = 0;
		status = sys$enqw(0,	/* event flag */
					  LCK$K_PRMODE,	/* lock mode */
					  lksb,	/* Lock status block */
					  LCK$M_CONVERT | LCK$M_VALBLK,	/* flags */
					  0,	/* resource name */
					  0,	/* parent id */
					  0,	/* ast address */
					  event,	/* ast argument */
					  blocking_ast,	/* blocking ast */
					  0,	/* access mode */
					  0);
	}

	return event;
}


static void poke_ast(POKE poke)
{
/**************************************
 *
 *	p o k e _ a s t
 *
 **************************************
 *
 * Functional description
 *	Lock to poke event has completed.  Update value block
 *	and deque the lock.
 *
 **************************************/
	lock_status* lksb = &poke->poke_lksb;
	lksb->lksb_value[0] += poke->poke_value;
	sys$deq(lksb->lksb_lock_id, lksb->lksb_value, 0, 0);
	sys$deq(poke->poke_parent_id, 0, 0, 0);
	--poke->poke_use_count;
}


static bool request_completed(VMS_REQ request)
{
/**************************************
 *
 *	r e q u e s t _ c o m p l e t e d
 *
 **************************************
 *
 * Functional description
 *	See if request is completed.
 *
 **************************************/
	for (RINT interest = request->req_interests; interest;
		 interest = interest->rint_req_interests)
	{
		EVNT event = interest->rint_event;
		if (interest->rint_count <= event->evnt_count)
			return true;
	}

	return false;
}


static int return_ok(ISC_STATUS * status_vector)
{
/**************************************
 *
 *	r e t u r n _ o k
 *
 **************************************
 *
 * Functional description
 *	Everything is ducky -- return success.
 *
 **************************************/

	*status_vector++ = isc_arg_gds;
	*status_vector++ = 0;
	*status_vector = isc_arg_end;
	return 0;
}

