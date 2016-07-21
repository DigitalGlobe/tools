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
// PThreadCondition.c++ - C++ Condition class built on top of posix threads.
// ~~~~~~~~~~~~~~~~~~~~
//

#include <sys/time.h>
#include <assert.h>
#include <OpenThreads/Condition>
#include "PThreadConditionPrivateData.h"
#include "PThreadMutexPrivateData.h"

using namespace OpenThreads;

//----------------------------------------------------------------------------
// This cancel cleanup handler is necessary to ensure that the barrier's
// mutex gets unlocked on cancel. Otherwise deadlocks could occur with 
// later joins.
//
void condition_cleanup_handler(void *arg) {

    pthread_mutex_t *mutex = static_cast<pthread_mutex_t *>(arg);
    
    pthread_mutex_unlock(mutex);

}

//----------------------------------------------------------------------------
//
// Decription: Constructor
//
// Use: public.
//
Condition::Condition() {

    PThreadConditionPrivateData *pd =
        new PThreadConditionPrivateData();

    int status = pthread_cond_init( &pd->condition, NULL );
    assert(status == 0);

    _prvData = static_cast<void *>(pd);

}

//----------------------------------------------------------------------------
//
// Decription: Destructor
//
// Use: public.
//
Condition::~Condition() {

    PThreadConditionPrivateData *pd =
        static_cast<PThreadConditionPrivateData *>(_prvData);

    int status = pthread_cond_destroy( &pd->condition );
    assert(status == 0);

    delete pd;

}

//----------------------------------------------------------------------------
//
// Decription: wait on a condition
//
// Use: public.
//
int Condition::wait(Mutex *mutex) {

    PThreadConditionPrivateData *pd =
        static_cast<PThreadConditionPrivateData *>(_prvData);

    PThreadMutexPrivateData *mpd =
        static_cast<PThreadMutexPrivateData *>(mutex->_prvData);

    int status;
    
    pthread_cleanup_push(condition_cleanup_handler, &mpd->mutex);

    status = pthread_cond_wait( &pd->condition, &mpd->mutex );

    pthread_cleanup_pop(0);

    return status;


}

//----------------------------------------------------------------------------
//
// Decription: wait on a condition, for a specified period of time
//
// Use: public.
//
int Condition::wait(Mutex *mutex, unsigned long int ms) {

    PThreadConditionPrivateData *pd =
        static_cast<PThreadConditionPrivateData *>(_prvData);

    PThreadMutexPrivateData *mpd =
        static_cast<PThreadMutexPrivateData *>(mutex->_prvData);

    struct ::timeval now;
    ::gettimeofday( &now, 0 );

    // Wait time is now + ms milliseconds
    unsigned int sec = ms / 1000;
    unsigned int nsec = (ms % 1000) * 1000;
    struct timespec abstime;
    abstime.tv_sec = now.tv_sec + sec;
    abstime.tv_nsec = now.tv_usec*1000 + nsec;

    int status;

    pthread_cleanup_push(condition_cleanup_handler, &mpd->mutex);

    status = pthread_cond_timedwait( &pd->condition, &mpd->mutex, &abstime );

    pthread_cleanup_pop(0);

    return status;

}

//----------------------------------------------------------------------------
//
// Decription: signal a thread to wake up.
//
// Use: public.
//
int Condition::signal() {

    PThreadConditionPrivateData *pd =
        static_cast<PThreadConditionPrivateData *>(_prvData);

    return pthread_cond_signal( &pd->condition );
}

//----------------------------------------------------------------------------
//
// Decription: signal many threads to wake up.
//
// Use: public.
//
int Condition::broadcast() {

    PThreadConditionPrivateData *pd =
        static_cast<PThreadConditionPrivateData *>(_prvData);

    return pthread_cond_broadcast( &pd->condition );
}
