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
// PThreadBarrier.c++ - C++ Barrier class built on top of POSIX threads.
// ~~~~~~~~~~~~~~~~~~
//

#include <stdio.h>
#include <unistd.h>
#include <OpenThreads/Barrier>
#include "PThreadBarrierPrivateData.h"

using namespace OpenThreads;

//----------------------------------------------------------------------------
// This cancel cleanup handler is necessary to ensure that the barrier's
// mutex gets unlocked on cancel. Otherwise deadlocks could occur with 
// later joins.
//
void barrier_cleanup_handler(void *arg) {

    pthread_mutex_t *mutex = static_cast<pthread_mutex_t *>(arg);
    
    pthread_mutex_unlock(mutex);

}

//----------------------------------------------------------------------------
//
// Decription: Constructor
//
// Use: public.
//
Barrier::Barrier(int numThreads) {

    PThreadBarrierPrivateData *pd = new PThreadBarrierPrivateData();

    pd->cnt = 0;
    pd->phase = 0;
    pd->maxcnt = numThreads;

    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init( &mutex_attr );

#ifndef __linux__ // (not available until NPTL) [
    pthread_mutexattr_settype( &mutex_attr, PTHREAD_MUTEX_ERRORCHECK );
#endif // ] __linux__

#ifdef ALLOW_PRIORITY_SCHEDULING // [

#ifdef __sun // [
    pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_NONE);
#endif // ] __sun

    //-------------------------------------------------------------------------
    // Initialization is a bit tricky, since we have to be able to be aware
    // that on many-to-many execution vehicle systems, we may run into
    // priority inversion deadlocks if a mutex is shared between threads
    // of differing priorities.  Systems that do this should provide the 
    // following protocol attributes to prevent deadlocks.  Check at runtime.
    //
    //  PRIO_INHERIT causes any thread locking the mutex to temporarily become
    //  the same priority as the highest thread also blocked on the mutex. 
    //  Although more expensive, this is the prefered method.
    //
    //  PRIO_PROTECT causes any thread locking the mutex to assume the priority
    //  specified by setprioceiling.  pthread_mutex_lock will fail if
    //  the priority ceiling is lower than the thread's priority.  Therefore,
    //  the priority ceiling must be set to the max priority in order to 
    //  garantee no deadlocks will occur.
    //
#if defined (_POSIX_THREAD_PRIO_INHERIT) || defined (_POSIX_THREAD_PRIO_PROTECT) // [

    if(sysconf(_POSIX_THREAD_PRIO_INHERIT)) {

	pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);
	
    } else if (sysconf(_POSIX_THREAD_PRIO_PROTECT)) {

	int th_policy;
	struct sched_param th_param;
	pthread_getschedparam(pthread_self(), &th_policy, &th_param);

	pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_PROTECT);
	pthread_mutexattr_setprioceiling(&mutex_attr, 
					 sched_get_priority_max(th_policy));

    }

#endif // ] Priority sheduling

#endif // ] ALLOW_PRIORITY_SCHEDULING

    pthread_mutex_init(&(pd->lock), &mutex_attr);

    pthread_cond_init(&(pd->cond), NULL);

    _prvData = static_cast<void *>(pd);

}

//----------------------------------------------------------------------------
//
// Decription: Destructor
//
// Use: public.
//
Barrier::~Barrier() {

    PThreadBarrierPrivateData *pd =
        static_cast<PThreadBarrierPrivateData*>(_prvData);

    pthread_mutex_destroy(&(pd->lock));

    pthread_cond_destroy(&(pd->cond));

    delete pd;

}

//----------------------------------------------------------------------------
//
// Decription: Reset the barrier to its original state
//
// Use: public.
//
void Barrier::reset() {
    
    PThreadBarrierPrivateData *pd =
        static_cast<PThreadBarrierPrivateData*>(_prvData);

    pd->cnt = 0;
    pd->phase = 0;

}

//----------------------------------------------------------------------------
//
// Decription: Block until numThreads threads have entered the barrier.
//
// Use: public.
//
void Barrier::block(unsigned int numThreads) {

    PThreadBarrierPrivateData *pd =
        static_cast<PThreadBarrierPrivateData*>(_prvData);

    if(numThreads != 0) pd->maxcnt = numThreads;

    int my_phase;

    pthread_mutex_lock(&(pd->lock));
    my_phase = pd->phase;
    ++pd->cnt;
    
    if (pd->cnt == pd->maxcnt) {             // I am the last one
	pd->cnt = 0;                         // reset for next use
	pd->phase = 1 - my_phase;            // toggle phase
	pthread_cond_broadcast(&(pd->cond));
    } 

    while (pd->phase == my_phase) {

	pthread_cleanup_push(barrier_cleanup_handler, &(pd->lock));
		
	pthread_cond_wait(&(pd->cond), &(pd->lock));

	pthread_cleanup_pop(0);

    }

    pthread_mutex_unlock(&(pd->lock));

}
