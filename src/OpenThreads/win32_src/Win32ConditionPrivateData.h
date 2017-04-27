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
// WIN32ConditionPrivateData.h - Private data structure for Condition
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
#ifndef _WIN32CONDITIONPRIVATEDATA_H_
#define _WIN32CONDITIONPRIVATEDATA_H_

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#define InterlockedGet(x) InterlockedExchangeAdd(x,0)

namespace OpenThreads {

class Condition;

class Win32ConditionPrivateData {
public:
	friend class Condition;
	/// number of waiters.
	long waiters_;

	Win32ConditionPrivateData ()
	{
		waiters_ = 0;
		sema_ = CreateSemaphore(NULL,0,0x7fffffff,NULL);
		waiters_done_ = CreateEvent(NULL,FALSE,FALSE,NULL);
	}
	~Win32ConditionPrivateData ();

	inline int broadcast ()
	{
  	    int have_waiters = 0;
		long w = InterlockedGet(&waiters_);
		if (w > 0)
		{
		  // we are broadcasting.  
	      was_broadcast_ = 1;
		  have_waiters = 1;
		}

		int result = 0;
		if (have_waiters)
	    {
			// Wake up all the waiters.
			ReleaseSemaphore(sema_,waiters_,NULL);
			WaitForSingleObject(waiters_done_,INFINITE) ;
			//end of broadcasting
			was_broadcast_ = 0;
	    }
		return result;
	}

	inline int signal()
	{
		long w = InterlockedGet(&waiters_);
	    int have_waiters = w > 0;
 
		int result = 0;

		if (have_waiters)
	    {
			if( !ReleaseSemaphore(sema_,1,NULL) )
				result = -1;
	    }
		return result;
	}

	inline int wait (Mutex& external_mutex, long timeout_ms)
	{

		// Prevent race conditions on the <waiters_> count.
		InterlockedIncrement(&waiters_);

		int result = 0;
        external_mutex.unlock();

		DWORD dwResult = WaitForSingleObject(sema_,timeout_ms);
		if(dwResult != WAIT_OBJECT_0)
			result = (int)dwResult;

		// We're ready to return, so there's one less waiter.
		InterlockedDecrement(&waiters_);
		long w = InterlockedGet(&waiters_);
		int last_waiter = was_broadcast_ && w == 0;

		if (result != -1 && last_waiter)
			SetEvent(waiters_done_);

		external_mutex.lock();
		return result;
	}
protected:
  /// Serialize access to the waiters count.
  /// Mutex waiters_lock_;
  /// Queue up threads waiting for the condition to become signaled.
  HANDLE sema_;
  /**
   * An auto reset event used by the broadcast/signal thread to wait
   * for the waiting thread(s) to wake up and get a chance at the
   * semaphore.
   */
  HANDLE waiters_done_;
  /// Keeps track of whether we were broadcasting or just signaling.
  size_t was_broadcast_;
};

#undef InterlockedGet

}







#endif // !_WIN32CONDITIONPRIVATEDATA_H_



