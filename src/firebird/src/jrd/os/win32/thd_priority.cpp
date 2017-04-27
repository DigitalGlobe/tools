/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		thd_priority.cpp
 *	DESCRIPTION:	Thread priorities scheduler
 *
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Alexander Peshkoff
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2002 Alexander Peshkoff <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 * 2002.10.20 Alexander Peshkoff: Created this scheduler, changing
 *   priorities of Win32 threads. to avoid side effects of Windows
 *   native priorities scheduling.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#include "firebird.h"

#ifdef THREAD_PSCHED

#include "../jrd/common.h"
#include "../jrd/ThreadStart.h"
#include "../jrd/os/thd_priority.h"
#include "../common/config/config.h"
#include "../jrd/gds_proto.h"
#include "../common/classes/fb_tls.h"

#include <stdio.h>
#include <errno.h>
#include <process.h>
#include <windows.h>

// #define DEBUG_THREAD_PSCHED

Firebird::GlobalPtr<Firebird::Mutex> ThreadPriorityScheduler::mutex;
ThreadPriorityScheduler* ThreadPriorityScheduler::chain = 0;
Firebird::InitMutex<ThreadPriorityScheduler> ThreadPriorityScheduler::initialized;
ThreadPriorityScheduler::OperationMode ThreadPriorityScheduler::opMode = ThreadPriorityScheduler::Running;
ThreadPriorityScheduler::TpsPointers* ThreadPriorityScheduler::toDetach = 0;
HANDLE ThreadPriorityScheduler::masterHandle = INVALID_HANDLE_VALUE;
bool ThreadPriorityScheduler::active = true;

namespace {

TLS_DECLARE (ThreadPriorityScheduler*, currentScheduler);

}

// Get instance for current thread
ThreadPriorityScheduler* ThreadPriorityScheduler::get()
{
	return TLS_GET(currentScheduler);
}

// Goes to low priority zone
void ThreadPriorityScheduler::enter()
{
	ThreadPriorityScheduler* t = get();
	fb_assert(t);
	t->inside = true;
	t->gonein = true;
}

// Goes from low priority zone
void ThreadPriorityScheduler::exit()
{
	ThreadPriorityScheduler* t = get();
	fb_assert(t);
	t->inside = false;
}

void ThreadPriorityScheduler::init()
{
	if (opMode != Running)
	{
		Firebird::fatal_exception::raise("Attempt to initialize after shutdown");
	}

	active = Config::getUsePriorityScheduler();
	toDetach = FB_NEW(*getDefaultMemoryPool()) TpsPointers(*getDefaultMemoryPool());

	// allocate thps for current (i.e. server's main) thread
	ThreadPriorityScheduler* tps =
		FB_NEW(*getDefaultMemoryPool()) ThreadPriorityScheduler(0, 0, THPS_PSCHED);

	tps->attach();

	// start scheduler
	if (active)
	{
		unsigned thread_id;
		masterHandle = (HANDLE) _beginthreadex(NULL, 0, schedulerMain, 0, 0, &thread_id);
		if (! masterHandle)
		{
			Firebird::system_call_failed::raise("_beginthreadex", GetLastError());
		}
		SetThreadPriority(masterHandle, THREAD_PRIORITY_TIME_CRITICAL);
	}

	// register
	gds__register_cleanup(Cleanup, 0);
}

void ThreadPriorityScheduler::Cleanup(void*)
{
	initialized.cleanup();
}

void ThreadPriorityScheduler::cleanup()
{
	{	// scope for MutexLockGuard
		Firebird::MutexLockGuard guard(mutex);
		opMode = Stopping;
	}

	if (active)
	{
		WaitForSingleObject(masterHandle, INFINITE);
		CloseHandle(masterHandle);
	}
}

void ThreadPriorityScheduler::attach()
{
	if (active)
	{
		HANDLE process = GetCurrentProcess();
		HANDLE thread = GetCurrentThread();
		if (! DuplicateHandle(process, thread, process, &handle, 0, FALSE, DUPLICATE_SAME_ACCESS))
		{
			Firebird::system_call_failed::raise("DuplicateHandle", GetLastError());
		}

		{	// scope for MutexLockGuard
			Firebird::MutexLockGuard guard(mutex);
			next = chain;
			chain = this;
		}

#ifdef DEBUG_THREAD_PSCHED
		gds__log("^ handle=%p priority=%d", handle, flags & THPS_BOOSTED ? highPriority : lowPriority);
#endif
	}
	else
	{
		next = 0;
		handle = INVALID_HANDLE_VALUE;
	}
	TLS_SET(currentScheduler, this);
}

void ThreadPriorityScheduler::run()
{
	attach();
	routine(arg);
}

void ThreadPriorityScheduler::detach()
{
	if (active)
	{
		Firebird::MutexLockGuard guard(mutex);
		TLS_SET(currentScheduler, 0);
		if (opMode == ShutdownComplete)
		{
			for (ThreadPriorityScheduler** pt = &chain; *pt; pt = &(*pt)->next)
			{
				if (*pt == this)
				{
					*pt = this->next;
#ifdef DEBUG_THREAD_PSCHED
					gds__log("~ handle=%p", handle);
#endif
					delete this;
					break;
				}
			}
		}
		else
		{
			toDetach->add(this);
		}
	}
	else
	{
#ifdef DEBUG_THREAD_PSCHED
		gds__log("~ handle=%p", handle);
#endif
		delete this;
	}
}

unsigned int __stdcall ThreadPriorityScheduler::schedulerMain(LPVOID)
{
	for (;;)
	{
		if (opMode == Stopping)
		{
/*			* It's no use cleaning something when server
			* prepares to shutdown. Moreover, in some rare cases
			* mutex may be already deleted, therefore AV happens.
			mutex.enter();
			doDetach();
			opMode = ShutdownComplete;
			mutex.leave(); */
			break;
		}
		Sleep(Config::getPrioritySwitchDelay());
		// We needn't lock mutex, because we don't modify
		// next here, and new thps object may be added
		// only in the beginning of the chain - even if it
		// happens, we safely ignore it here.
		// To be sure that nothing is deleted from chain,
		// toDetach member is added to this class.
		for (ThreadPriorityScheduler *t = chain; t; t = t->next)
		{
			const UCHAR p_flags = t->flags;
			if (p_flags & THPS_PSCHED)
			{
				const bool gonein = t->gonein;
				t->gonein = false;
				t->flags &= ~(THPS_UP | THPS_LOW); // clean them
#pragma FB_COMPILER_MESSAGE("Fix! May have problems with long running UDFs.")
				if ((! gonein) && (! (p_flags & THPS_BOOSTED)))
				{
					if (p_flags & THPS_UP)
					{
				// 1.	thread exited single thread zone and didn't
				//		return into it since this &last cycle:
				//			increase priority
						if (! SetThreadPriority(t->handle, highPriority))
						{
							Firebird::system_call_failed::raise("SetThreadPriority");
						}

#ifdef DEBUG_THREAD_PSCHED
						gds__log("+ handle=%p priority=%d", t->handle, highPriority);
#endif
						t->flags |= THPS_BOOSTED;
						continue;
					}
				// 2.	thread exited single thread zone
				//		and never returned there during this cycle:
				//			candidate for priority increase
					t->flags |= THPS_UP;
					continue;
				}
				if ((gonein || t->inside) && (p_flags & THPS_BOOSTED))
				{
					if (p_flags & THPS_LOW)
					{
				// 3.	thread entered single thread zone
				//		last cycle and didn't leave it completely
				//		this cycle:
				//		decrease priority
						if (! SetThreadPriority(t->handle, lowPriority))
						{
							Firebird::system_call_failed::raise("SetThreadPriority");
						}
#ifdef DEBUG_THREAD_PSCHED
						gds__log("- handle=%p priority=%d", t->handle, lowPriority);
#endif
						t->flags &= ~THPS_BOOSTED;
						continue;
					}
				// 4.	thread entered single thread zone this cycle:
				//		candidate for priority decrease
					t->flags |= THPS_LOW;
					continue;
				}
			}
		}

		if (toDetach->getCount() > 0)
		{
			Firebird::MutexLockGuard guard(mutex);
			doDetach();
		}
	}
	return 0;
}

// *** mutex MUST be locked before doDetach ***
void ThreadPriorityScheduler::doDetach()
{
	for (ThreadPriorityScheduler ** pt = &chain; *pt; pt = &(*pt)->next)
	{
start_label:
		size_t pos;
		ThreadPriorityScheduler *m = *pt;
		if (! toDetach->find(m, pos))
		{
			continue;
		}
		toDetach->remove(pos);
		*pt = m->next;
#ifdef DEBUG_THREAD_PSCHED
		gds__log("~ handle=%p", m->handle);
#endif
		delete m;
		if (*pt)
			goto start_label;

		break;
	}
}

#endif //THREAD_PSCHED
