/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		isc_ipc.cpp
 *	DESCRIPTION:	Handing and posting of signals (Windows)
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
 * Solaris x86 changes - Konstantin Kuznetsov, Neil McCalden
 *
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete ports:
 *                          - EPSON, DELTA, IMP, NCR3000 and M88K
 *
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "UNIXWARE" port
 *
 * 2002.10.28 Sean Leyne - Completed removal of obsolete "DGUX" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 * 2002.08.27 Nickolay Samofatov - create Windows version of this module
 *
 */


#include "firebird.h"
#include "../../../common/classes/init.h"
#include "../../../common/utils_proto.h"
#include "../jrd/common.h"
#include "gen/iberror.h"
#include "../jrd/isc.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/os/isc_i_proto.h"
#include "../jrd/isc_s_proto.h"

#include <windows.h>
#include <process.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

// signals may be not defined in MINGW
#ifndef SIG_SGE
#define SIG_SGE (void (__cdecl *)(int))3           /* signal gets error */
#endif
#ifndef SIG_ACK
#define SIG_ACK (void (__cdecl *)(int))4           /* acknowledge */
#endif

static int process_id = 0;

const USHORT MAX_OPN_EVENTS	= 40;

struct opn_event_t
{
	SLONG opn_event_pid;
	SLONG opn_event_signal;		/* pseudo-signal number */
	HANDLE opn_event_lhandle;	/* local handle to foreign event */
	ULONG opn_event_age;
};


static struct opn_event_t opn_events[MAX_OPN_EVENTS];
static USHORT opn_event_count;
static ULONG opn_event_clock;

static void signal_cleanup(void*);

int ISC_kill(SLONG pid, SLONG signal_number, void *object_hndl)
{
/**************************************
 *
 *	I S C _ k i l l		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Notify somebody else.
 *
 **************************************/

/* If we're simply trying to poke ourselves, do so directly. */
	ISC_signal_init();

	if (pid == process_id) {
		SetEvent(object_hndl);
		return 0;
	}

	opn_event_t* oldest_opn_event = NULL;
	ULONG oldest_age = ~0;

	opn_event_t* opn_event = opn_events;
	const opn_event_t* const end_opn_event = opn_event + opn_event_count;
	for (; opn_event < end_opn_event; opn_event++) {
		if (opn_event->opn_event_pid == pid &&
			opn_event->opn_event_signal == signal_number)
		{
			break;
		}
		if (opn_event->opn_event_age < oldest_age) {
			oldest_opn_event = opn_event;
			oldest_age = opn_event->opn_event_age;
		}
	}

	if (opn_event >= end_opn_event) {
		HANDLE lhandle = ISC_make_signal(false, false, pid, signal_number);
		if (!lhandle)
			return -1;

		if (opn_event_count < MAX_OPN_EVENTS)
			opn_event_count++;
		else {
			opn_event = oldest_opn_event;
			CloseHandle(opn_event->opn_event_lhandle);
		}

		opn_event->opn_event_pid = pid;
		opn_event->opn_event_signal = signal_number;
		opn_event->opn_event_lhandle = lhandle;
	}

	opn_event->opn_event_age = ++opn_event_clock;

	return SetEvent(opn_event->opn_event_lhandle) ? 0 : -1;
}

void* ISC_make_signal(bool create_flag, bool manual_reset, int process_idL, int signal_number)
{
/**************************************
 *
 *	I S C _ m a k e _ s i g n a l		( W I N _ N T )
 *
 **************************************
 *
 * Functional description
 *	Create or open a Windows/NT event.
 *	Use the signal number and process id
 *	in naming the object.
 *
 **************************************/
	ISC_signal_init();

	const BOOL man_rst = manual_reset ? TRUE : FALSE;

	if (!signal_number)
		return CreateEvent(NULL, man_rst, FALSE, NULL);

	TEXT event_name[BUFFER_TINY];
	sprintf(event_name, "_firebird_process%u_signal%d", process_idL, signal_number);

	if (!fb_utils::prefix_kernel_object_name(event_name, sizeof(event_name)))
	{
		SetLastError(ERROR_FILENAME_EXCED_RANGE);
		return NULL;
	}

	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, TRUE, event_name);

	if (create_flag) {
		fb_assert(!hEvent);
		hEvent = CreateEvent(ISC_get_security_desc(), man_rst, FALSE, event_name);
	}

	if (hEvent) {
		SetHandleInformation(hEvent, HANDLE_FLAG_INHERIT, 0);
	}
	return hEvent;
}

namespace
{
	class SignalInit
	{
	public:
		static void init()
		{
			gds__register_cleanup(signal_cleanup, 0);
			process_id = getpid();
			ISC_get_security_desc();
		}

		static void cleanup()
		{
			process_id = 0;

			opn_event_t* opn_event = opn_events + opn_event_count;
			opn_event_count = 0;
			while (opn_event-- > opn_events)
				CloseHandle(opn_event->opn_event_lhandle);
		}
	};

	Firebird::InitMutex<SignalInit> signalInit;
} // anonymous namespace

void ISC_signal_init()
{
/**************************************
 *
 *	I S C _ s i g n a l _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize any system signal handlers.
 *
 **************************************/

	signalInit.init();
}


static void signal_cleanup(void*)
{
/**************************************
 *
 *	s i g n a l _ c l e a n u p
 *
 **************************************
 *
 * Functional description
 *	Module level cleanup handler.
 *
 **************************************/
	signalInit.cleanup();
}
