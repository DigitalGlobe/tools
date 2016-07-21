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
// Win32Mutex.c++ - C++ Mutex class .
// The idea for it is borrowed from SGI STL
// It looks like it's hard to use win32 CRITICALL_SECTIONS withour introducing race 
// conditions on InitializeCriticalSection() . So we use spin mutex here.
// ~~~~~~~~~~~~~~~~
//

#include <OpenThreads/Mutex>
#include "Win32MutexPrivateData.h"
using namespace OpenThreads;

Win32MutexPrivateData::~Win32MutexPrivateData()
{
}



template <int instance>
struct WIN32MutexSpin {

  enum { __low_max = 30, __high_max = 1000 };
  // Low if we suspect uniprocessor, high for multiprocessor.
  static unsigned __max;
  static unsigned __last;
};

template <int instance>
unsigned WIN32MutexSpin <instance>::__max = WIN32MutexSpin <instance>::__low_max;

template <int instance>
unsigned WIN32MutexSpin <instance>::__last = 0;

static void _S_nsec_sleep(int __log_nsec) {

    if (__log_nsec <= 20) {
        Sleep(0);
     } else {
        Sleep(1 << (__log_nsec - 20));
     }
}


#if defined(_MSC_VER) && _MSC_VER <= 1300
	template WIN32MutexSpin <0>;
#endif

//----------------------------------------------------------------------------
//
// Decription: Constructor
//
// Use: public.
//
Mutex::Mutex() {
    Win32MutexPrivateData *pd = new Win32MutexPrivateData();
    pd->mutex  = 0;
    _prvData = static_cast<void *>(pd);
}

//----------------------------------------------------------------------------
//
// Decription: Destructor
//
// Use: public.
//
Mutex::~Mutex() {
    unlock();
    delete static_cast<Win32MutexPrivateData*>(_prvData);
}
//----------------------------------------------------------------------------
//
// Decription: lock the mutex
//
// Use: public.
//
int Mutex::lock() {
    Win32MutexPrivateData *pd =
        static_cast<Win32MutexPrivateData*>(_prvData);

    volatile unsigned long* lock = &pd->mutex;
	// InterlockedExchange returns old value
	// if old_value  == 0 mutex wasn't locked , now it is
	if( !InterlockedExchange((long*)lock, 1L)) {
       return 0;
    }

    unsigned my_spin_max = WIN32MutexSpin<0>::__max;
    unsigned my_last_spins = WIN32MutexSpin<0>::__last;
    volatile unsigned junk = 17;      
    unsigned i;

    for (i = 0; i < my_spin_max; i++) {
      if (i < my_last_spins/2 || *lock) {
        junk *= junk; junk *= junk;
        junk *= junk; junk *= junk;
        continue;
      }

      if (!InterlockedExchange((long*)lock, 1L)) {
        // got it!
        // Spinning worked.  Thus we're probably not being scheduled
        // against the other process with which we were contending.
        // Thus it makes sense to spin longer the next time.
        WIN32MutexSpin<0>::__last = i;
        WIN32MutexSpin<0>::__max = WIN32MutexSpin<0>::__high_max;
        return 0;
      }
    }
    // We are probably being scheduled against the other process.  Sleep.
    WIN32MutexSpin<0>::__max = WIN32MutexSpin<0>::__low_max;
    for (i = 0 ;; ++i) {
      int __log_nsec = i + 6;
      if (__log_nsec > 27) __log_nsec = 27;
      if (!InterlockedExchange((long*)lock, 1L)) {
        return 0;
      }
      _S_nsec_sleep(__log_nsec);
    }
	return -1;
}

//----------------------------------------------------------------------------
//
// Decription: unlock the mutex
//
// Use: public.
//
int Mutex::unlock() {
    Win32MutexPrivateData *pd =
        static_cast<Win32MutexPrivateData*>(_prvData);

    volatile unsigned long* lock = &pd->mutex;
    *lock = 0;
    // This is not sufficient on many multiprocessors, since
    // writes to protected variables and the lock may be reordered.
	return 0;
}

//----------------------------------------------------------------------------
//
// Decription: test if the mutex may be locked
//
// Use: public.
//
int Mutex::trylock() {
    Win32MutexPrivateData *pd =
        static_cast<Win32MutexPrivateData*>(_prvData);

    volatile unsigned long* lock = &pd->mutex;

	if( !InterlockedExchange((long*)lock, 1L)) {
      return 1; // TRUE
    }

	return 0; // FALSE

}
