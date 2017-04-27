//
// OpenThread library, Copyright (C) 2002 - 2003  The Open Thread Group
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// 

//
// PThread.c++ - C++ Thread class built on top of posix threads.
// ~~~~~~~~~~~

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

#if defined __linux || defined __sun || defined __APPLE__
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/unistd.h>
#endif

#include <OpenThreads/Thread>
#include "PThreadPrivateData.h"

using namespace OpenThreads;

extern int errno;
const char *OPENTHREAD_VERSION_STRING = "OpenThreads v1.2preAlpha, Posix Threads (Public Implementation)";

#ifdef DEBUG
# define DPRINTF(arg) printf arg
#else
# define DPRINTF(arg)
#endif

//-----------------------------------------------------------------------------
// Initialize the static unique ids.
//
int PThreadPrivateData::nextId = 0;

//-----------------------------------------------------------------------------
// Initialize thread master priority level
//
Thread::ThreadPriority Thread::s_masterThreadPriority =  
                                          Thread::PRIORITY_DEFAULT;

bool Thread::s_isInitialized = false;
pthread_key_t PThreadPrivateData::s_tls_key;

struct ThreadCleanupStruct {

    OpenThreads::Thread *thread;
    volatile bool *runflag;

};

//-----------------------------------------------------------------------------
// This cleanup handler is necessary to ensure that the thread will cleanup
// and set its isRunning flag properly.
//
void thread_cleanup_handler(void *arg) {

    ThreadCleanupStruct *tcs = static_cast<ThreadCleanupStruct *>(arg);
    
    tcs->thread->cancelCleanup();
    *(tcs->runflag) = false;

}

//-----------------------------------------------------------------------------
// Class to support some static methods necessary for pthread's to work
// correctly.
//

namespace OpenThreads {

class ThreadPrivateActions {

    //-------------------------------------------------------------------------
    // We're friendly to Thread, so it can issue the methods.
    //
    friend class Thread;

private:

    //-------------------------------------------------------------------------
    // pthreads standard start routine.
    //
    static void *StartThread(void *data) {

	Thread *thread = static_cast<Thread *>(data);
	PThreadPrivateData *pd =
	    static_cast<PThreadPrivateData *>(thread->_prvData);

	ThreadCleanupStruct tcs;
	tcs.thread = thread;
	tcs.runflag = &pd->isRunning;

	// Set local storage so that Thread::CurrentThread() can return the right thing
	int status = pthread_setspecific(PThreadPrivateData::s_tls_key, thread);
	assert(status == 0);

	pthread_cleanup_push(thread_cleanup_handler, &tcs);

#ifdef ALLOW_PRIORITY_SCHEDULING

	//---------------------------------------------------------------------
	// Set the proper scheduling priorities
	//
	SetThreadSchedulingParams(thread);

#endif // ] ALLOW_PRIORITY_SCHEDULING

        pd->isRunning = true;
        thread->run();
        pd->isRunning = false;

	pthread_cleanup_pop(0);
		
        return 0;

    };

    //-------------------------------------------------------------------------
    // Print information related to thread schduling parameters.
    //
    static void PrintThreadSchedulingInfo(Thread *thread) {

#ifdef ALLOW_PRIORITY_SCHEDULING // [

	if(sysconf(_POSIX_THREAD_PRIORITY_SCHEDULING)) {

	    int status, my_policy, min_priority, max_priority;
	    struct sched_param my_param;
	    
	    status = pthread_getschedparam(thread->getProcessId(), 
					   &my_policy, 
					   &my_param);
	    
	    if(status != 0) {
		printf("THREAD INFO (%d) : Get sched: %s\n",
		       thread->getProcessId(),
		       strerror(status));
	    } else {
		printf(
		    "THREAD INFO (%d) : Thread running at %s / Priority: %d\n",
		    thread->getProcessId(),
		    (my_policy == SCHED_FIFO ? "SCHEDULE_FIFO" 
		     : (my_policy == SCHED_RR ? "SCHEDULE_ROUND_ROBIN"
			: (my_policy == SCHED_OTHER ? "SCHEDULE_OTHER"
			   : "UNKNOWN"))), 
		    my_param.sched_priority);
		
		max_priority = sched_get_priority_max(my_policy);
		min_priority = sched_get_priority_min(my_policy);
		
		printf(
		    "THREAD INFO (%d) : Max priority: %d, Min priority: %d\n",
		    thread->getProcessId(),
		    max_priority, min_priority);
		
	    }
	    
	} else {
	    printf(
		"THREAD INFO (%d) POSIX Priority scheduling not available\n",
		thread->getProcessId());
	}

	fflush(stdout);

#endif // ] ALLOW_PRIORITY_SCHEDULING
    
    }

    //--------------------------------------------------------------------------
    // Set thread scheduling parameters.  Unfortunately on Linux, there's no
    // good way to set this, as pthread_setschedparam is mostly a no-op.
    //
    static int SetThreadSchedulingParams(Thread *thread) {

	int status = 0;

#ifdef ALLOW_PRIORITY_SCHEDULING // [

	if(sysconf(_POSIX_THREAD_PRIORITY_SCHEDULING)) {

	    int th_policy; 
	    int max_priority, nominal_priority, min_priority;
	    sched_param th_param;
	    pthread_getschedparam(thread->getProcessId(), 
				  &th_policy, &th_param);

#ifndef __linux__

	    switch(thread->getSchedulePolicy()) {

	    case Thread::SCHEDULE_FIFO:
		th_policy = SCHED_FIFO;
		break;

	    case Thread::SCHEDULE_ROUND_ROBIN:
		th_policy = SCHED_RR;
		break;

	    case Thread::SCHEDULE_TIME_SHARE:
		th_policy = SCHED_OTHER;
		break;

	    default:
#ifdef __sgi
		th_policy = SCHED_RR;
#else
		th_policy = SCHED_FIFO;
#endif
		break;
	    };
    
#else
	    th_policy = SCHED_OTHER;  // Must protect linux from realtime.
#endif

#ifdef __linux__

	    max_priority = 0;
	    min_priority = 20;
	    nominal_priority = (max_priority + min_priority)/2;

#else

	    max_priority = sched_get_priority_max(th_policy);
	    min_priority = sched_get_priority_min(th_policy);
	    nominal_priority = (max_priority + min_priority)/2;

#endif

	    switch(thread->getSchedulePriority()) {
	    
	    case Thread::PRIORITY_MAX:
		th_param.sched_priority = max_priority;
		break;
	    
	    case Thread::PRIORITY_HIGH:
		th_param.sched_priority = (max_priority + nominal_priority)/2;
		break;
	    
	    case Thread::PRIORITY_NOMINAL:
		th_param.sched_priority = nominal_priority;
		break;   
	    
	    case Thread::PRIORITY_LOW:
		th_param.sched_priority = (min_priority + nominal_priority)/2;
		break;       
	    
	    case Thread::PRIORITY_MIN:
		th_param.sched_priority = min_priority;
		break;   
	    
	    default:
		th_param.sched_priority = max_priority;
		break;  
	    
	    }
	     
	    status = pthread_setschedparam(thread->getProcessId(), 
					   th_policy,
					   &th_param);


	    if(getenv("OUTPUT_THREADLIB_SCHEDULING_INFO") != 0)
		PrintThreadSchedulingInfo(thread);   
	    
	} 

#endif // ] ALLOW_PRIORITY_SCHEDULING	

	return status;
    };
};

}

//----------------------------------------------------------------------------
//
// Description: Set the concurrency level (no-op)
//
// Use static public
//
int Thread::SetConcurrency(int concurrencyLevel) {
    
#if defined (__sgi) || defined (__sun)
    return pthread_setconcurrency(concurrencyLevel);
#else
    return -1;
#endif
    
};

//----------------------------------------------------------------------------
//
// Description: Get the concurrency level
//
// Use static public
//
int Thread::GetConcurrency() {

#if defined (__sgi) || defined (__sun)
    return pthread_getconcurrency();
#else
    return -1;
#endif

};

//----------------------------------------------------------------------------
//
// Decription: Constructor
//
// Use: public.
//
Thread::Thread() {

    if(!s_isInitialized) Init();

    PThreadPrivateData *pd = new PThreadPrivateData();
    pd->stackSize = 0;
    pd->stackSizeLocked = false;
    pd->idSet = false;
    pd->isRunning = false;
    pd->isCanceled = false;
    pd->uniqueId = pd->nextId;
    pd->nextId++;
    pd->threadPriority = PRIORITY_DEFAULT;
    pd->threadPolicy = SCHEDULE_DEFAULT;

    _prvData = static_cast<void *>(pd);

}

//----------------------------------------------------------------------------
//
// Decription: Destructor
//
// Use: public.
//
Thread::~Thread() {

    PThreadPrivateData *pd = static_cast<PThreadPrivateData *>(_prvData);

    if(pd->isRunning) {

	//---------------------------------------------------------------------
	// Kill the thread when it is destructed
	//
	setCancelModeAsynchronous();
	cancel();
    }

    delete pd;

}

Thread *Thread::CurrentThread() {
    
    Thread *thread = 
	static_cast<Thread *>(pthread_getspecific(PThreadPrivateData::s_tls_key));

    return thread;
	
}

//-----------------------------------------------------------------------------
// 
// Description: Initialize Threading
//
// Use: public.
//
void Thread::Init() {

    if(s_isInitialized) return;

    // Allocate a key to be used to access thread local storage
    int status = pthread_key_create(&PThreadPrivateData::s_tls_key, NULL);
    assert(status == 0);

#ifdef ALLOW_PRIORITY_SCHEDULING

    //--------------------------------------------------------------------------
    // If we've got priority scheduling, set things to nominal.
    // 
    if(sysconf(_POSIX_THREAD_PRIORITY_SCHEDULING)) {
       
	int max_priority, nominal_priority, min_priority;

	int th_policy; 
	sched_param th_param;
	pthread_getschedparam(pthread_self(), 
			      &th_policy, &th_param);
	
	max_priority = sched_get_priority_max(th_policy);
	min_priority = sched_get_priority_min(th_policy);
	nominal_priority = (max_priority + min_priority)/2;
	
	th_param.sched_priority = nominal_priority;

	pthread_setschedparam(pthread_self(), 
			      th_policy,
			      &th_param);

	s_masterThreadPriority = Thread::PRIORITY_NOMINAL;

    } else {

	s_masterThreadPriority = Thread::PRIORITY_DEFAULT;

    }

#endif // ] ALLOW_PRIORITY_SCHEDULING

    s_isInitialized = true;

}

//-----------------------------------------------------------------------------
//
// Description: Get a unique identifier for this thread.
//
// Use: public
//
int Thread::getThreadId() {

    PThreadPrivateData *pd = static_cast<PThreadPrivateData *> (_prvData);
    return pd->uniqueId;
}

//-----------------------------------------------------------------------------
//
// Description: Get the thread's process id
//
// Use: public
//
int Thread::getProcessId() {
    
    PThreadPrivateData *pd = static_cast<PThreadPrivateData *> (_prvData);

    if(pd->idSet == false) return (unsigned int) pthread_self();
    
    return (int)(pd->tid);
}

//-----------------------------------------------------------------------------
//
// Description: Determine if the thread is running
//
// Use: public
//
bool Thread::isRunning() {

    PThreadPrivateData *pd = static_cast<PThreadPrivateData *> (_prvData);
    return pd->isRunning;

}

//-----------------------------------------------------------------------------
//
// Description: Start the thread.
//
// Use: public
//
int Thread::start() {

    int status;
    pthread_attr_t thread_attr;

    status = pthread_attr_init( &thread_attr );
    if(status != 0) {
	return status;
    }

    PThreadPrivateData *pd = static_cast<PThreadPrivateData *> (_prvData);

    size_t defaultStackSize;
    pthread_attr_getstacksize( &thread_attr, &defaultStackSize);
    if(status != 0) {
	return status;
    }

    if(defaultStackSize < pd->stackSize) {
	
	pthread_attr_setstacksize( &thread_attr, pd->stackSize);
	if(status != 0) {
	    return status;
	}	
    }

    //-------------------------------------------------------------------------
    // Now get what we actually have...
    //
    pthread_attr_getstacksize( &thread_attr, &defaultStackSize);
    if(status != 0) {
	return status;
    }

    pd->stackSize = defaultStackSize;

    //-------------------------------------------------------------------------
    // Prohibit the stack size from being changed.
    //
    pd->stackSizeLocked = true;

#ifdef ALLOW_PRIORITY_SCHEDULING

    status = pthread_attr_setinheritsched( &thread_attr,
					   PTHREAD_EXPLICIT_SCHED );

    pthread_attr_setscope(&thread_attr, PTHREAD_SCOPE_SYSTEM);

#endif // ] ALLOW_PRIORITY_SCHEDULING

    if(status != 0) {
	return status;
    }

    status = pthread_create(&(pd->tid), &thread_attr,
                           ThreadPrivateActions::StartThread,
                           static_cast<void *>(this));

    if(status != 0) {
	return status;
    }

    pd->idSet = true;

    return 0;

}

//-----------------------------------------------------------------------------
//
// Description: Alternate thread start routine.
//
// Use: public
//
int Thread::startThread() { return start(); }

//-----------------------------------------------------------------------------
//
// Description: Join the thread.
//
// Use: public
//
int Thread::detach() {

    PThreadPrivateData *pd = static_cast<PThreadPrivateData *> (_prvData);
    return pthread_detach(pd->tid);

}

//-----------------------------------------------------------------------------
//
// Description: Join the thread.
//
// Use: public
//
int Thread::join() {

    void *threadResult = 0; // Dummy var.
    PThreadPrivateData *pd = static_cast<PThreadPrivateData *> (_prvData);
    return pthread_join(pd->tid, &threadResult);

}

//-----------------------------------------------------------------------------
//
// Description: test the cancel state of the thread.
//
// Use: public
//
int Thread::testCancel() {
    
    PThreadPrivateData *pd = static_cast<PThreadPrivateData *> (_prvData);

    if(pthread_self() != pd->tid)
	return -1;
    
    pthread_testcancel();

    return 0;

}


//-----------------------------------------------------------------------------
//
// Description: Cancel the thread.
//
// Use: public
//
int Thread::cancel() {

    PThreadPrivateData *pd = static_cast<PThreadPrivateData *> (_prvData);
    pd->isCanceled = true;
    int status = pthread_cancel(pd->tid);
    return status;

}

//-----------------------------------------------------------------------------
//
// Description: Disable cancelibility
//
// Use: public
//
int Thread::setCancelModeDisable() {

    return pthread_setcancelstate( PTHREAD_CANCEL_DISABLE, 0 );

}

//-----------------------------------------------------------------------------
//
// Description: set the thread to cancel immediately
//
// Use: public
//
int Thread::setCancelModeAsynchronous() {

    int status = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);
    if(status != 0) return status;

    return pthread_setcancelstate( PTHREAD_CANCEL_ENABLE, 0);
}

//-----------------------------------------------------------------------------
//
// Description: set the thread to cancel at the next convienent point.
//
// Use: public
//
int Thread::setCancelModeDeferred() {

    int status = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, 0);
    if(status != 0) return status;

    return pthread_setcancelstate( PTHREAD_CANCEL_ENABLE, 0);

}

//-----------------------------------------------------------------------------
//
// Description: Set the thread's schedule priority (if able)
//
// Use: public
//
int Thread::setSchedulePriority(ThreadPriority priority) {

#ifdef ALLOW_PRIORITY_SCHEDULING

    PThreadPrivateData *pd = static_cast<PThreadPrivateData *> (_prvData);
    
    pd->threadPriority = priority;
    
    if(pd->isRunning) 
	return ThreadPrivateActions::SetThreadSchedulingParams(this);
    else 
	return 0;

#else
    return -1;
#endif

}

//-----------------------------------------------------------------------------
//
// Description: Get the thread's schedule priority (if able)
//
// Use: public
//
int Thread::getSchedulePriority() {

    PThreadPrivateData *pd = static_cast<PThreadPrivateData *> (_prvData);
    
    return pd->threadPriority;
    
}

//-----------------------------------------------------------------------------
//
// Description: Set the thread's scheduling policy (if able)
//
// Use: public
//
int Thread::setSchedulePolicy(ThreadPolicy policy) {

#ifdef ALLOW_PRIORITY_SCHEDULING

    PThreadPrivateData *pd = static_cast<PThreadPrivateData *> (_prvData);
    
    pd->threadPolicy = policy;
    
    if(pd->isRunning) 
	return ThreadPrivateActions::SetThreadSchedulingParams(this);
    else 
	return 0;
#else
    return -1;
#endif

}

//-----------------------------------------------------------------------------
//
// Description: Set the thread's scheduling policy (if able)
//
// Use: public
//
int Thread::getSchedulePolicy() {

    PThreadPrivateData *pd = static_cast<PThreadPrivateData *> (_prvData);
    
    return pd->threadPolicy;
    
}


//-----------------------------------------------------------------------------
//
// Description: Set the thread's desired stack size
//
// Use: public
//
int Thread::setStackSize(size_t stackSize) {
    
    PThreadPrivateData *pd = static_cast<PThreadPrivateData *> (_prvData);
    
    if(pd->stackSizeLocked == true) return 13;  // EACESS

    pd->stackSize = stackSize;
    
    return 0;

}

//-----------------------------------------------------------------------------
//
// Description: Get the thread's stack size.
//
// Use: public
//
size_t Thread::getStackSize() {

   PThreadPrivateData *pd = static_cast<PThreadPrivateData *> (_prvData);
    
   return pd->stackSize;

}

//-----------------------------------------------------------------------------
//
// Description:  Print the thread's scheduling information to stdout.
//
// Use: public
//
void Thread::printSchedulingInfo() {
    
    ThreadPrivateActions::PrintThreadSchedulingInfo(this);
    
}

//-----------------------------------------------------------------------------
//
// Description:  Yield the processor
//
// Use: protected
//
int Thread::Yield() {

    return sched_yield();

}
