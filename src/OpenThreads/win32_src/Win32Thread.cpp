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
// Win32Thread.c++ - C++ Thread class built on top of posix threads.
// ~~~~~~~~~~~
#include <memory>
#include <string>

using std::size_t;

#include "Win32ThreadPrivateData.h"

using namespace OpenThreads;

Win32ThreadPrivateData::TlsHolder Win32ThreadPrivateData::TLS;

Win32ThreadPrivateData::~Win32ThreadPrivateData()
{
}

const std::string OPENTHREAD_VERSION_STRING = "OpenThread v1.2preAlpha, WindowThreads (Public Implementation)";


//-----------------------------------------------------------------------------
// Initialize thread master priority level
//
Thread::ThreadPriority Thread::s_masterThreadPriority =  Thread::PRIORITY_DEFAULT;

bool Thread::s_isInitialized = false;
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
		// Win32Threads standard start routine.
		//
		static unsigned long __stdcall StartThread(void *data) {

			Thread *thread = static_cast<Thread *>(data);
			Win32ThreadPrivateData *pd =
				static_cast<Win32ThreadPrivateData *>(thread->_prvData);

			TlsSetValue(Win32ThreadPrivateData::TLS.ID ,data);
			//---------------------------------------------------------------------
			// Set the proper scheduling priorities
			//
			SetThreadSchedulingParams(thread);

			pd->isRunning = true;
			pd->isCanceled = false;

			try{
				thread->run();
			}
			catch(...)
			{
				// abnormal termination but must be caught in win32 anyway
			}

			pd->isRunning = false;
			return 0;
		};

		//-------------------------------------------------------------------------
		// Print information related to thread schduling parameters.
		//
		static void PrintThreadSchedulingInfo(Thread *thread) {
			// NO-OP
		}

		//--------------------------------------------------------------------------
		// Set thread scheduling parameters.  Unfortunately on Linux, there's no
		// good way to set this, as Win32Thread_setschedparam is mostly a no-op.
		//
		static int SetThreadSchedulingParams(Thread *data) {
			
			Thread *thread = static_cast<Thread *>(data);

			Win32ThreadPrivateData *pd =
				static_cast<Win32ThreadPrivateData *>(thread->_prvData);

			int prio = THREAD_PRIORITY_NORMAL;

			switch(thread->getSchedulePriority()) {
			case Thread::PRIORITY_MAX:
				prio = THREAD_PRIORITY_HIGHEST;
				break;
			case Thread::PRIORITY_HIGH:
				prio = THREAD_PRIORITY_ABOVE_NORMAL;
				break;
			case Thread::PRIORITY_NOMINAL:
				prio = THREAD_PRIORITY_NORMAL;
				break;   
			case Thread::PRIORITY_LOW:
				prio = THREAD_PRIORITY_BELOW_NORMAL;
				break;       
			case Thread::PRIORITY_MIN:
				prio = THREAD_PRIORITY_IDLE;
				break;   
			}
			int status = SetThreadPriority( pd->tid , prio);
			PrintThreadSchedulingInfo(thread);   

			return status!=0;
		};
	};
};

Thread* Thread::CurrentThread()
{
	return (Thread* )TlsGetValue(Win32ThreadPrivateData::TLS.ID);
};

//----------------------------------------------------------------------------
//
// Description: Set the concurrency level (no-op)
//
// Use static public
//
int Thread::SetConcurrency(int) {
    return -1;
};

//----------------------------------------------------------------------------
//
// Description: Get the concurrency level
//
// Use static public
//
int Thread::GetConcurrency() {
    return -1;
};

//----------------------------------------------------------------------------
//
// Decription: Constructor
//
// Use: public.
//
Thread::Thread() {

	// there's no need for this
	//    if(!s_isInitialized) Init();

    Win32ThreadPrivateData *pd = new Win32ThreadPrivateData();

    pd->stackSize = 0;

    pd->stackSizeLocked = false;

    pd->idSet = false;

    pd->isRunning = false;

    pd->isCanceled = false;

	pd->cancelMode = 0;

    pd->uniqueId = 0;

    pd->threadPriority = PRIORITY_DEFAULT;

    pd->threadPolicy = SCHEDULE_DEFAULT;

	pd->detached = false; 

    _prvData = static_cast<void *>(pd);

}


//----------------------------------------------------------------------------
//
// Decription: Destructor
//
// Use: public.
//
Thread::~Thread() {
    Win32ThreadPrivateData *pd = static_cast<Win32ThreadPrivateData *>(_prvData);
    if(pd->isRunning) {
		pd->cancelMode = 0;
		cancel();
    }
    delete pd;
}
//-----------------------------------------------------------------------------
// 
// Description: Initialize Threading
//
// Use: public.
//
void Thread::Init() {
//    if(s_isInitialized) return;
//		s_masterThreadPriority = Thread::PRIORITY_DEFAULT;
    s_isInitialized = true;
}

//-----------------------------------------------------------------------------
//
// Description: Get a unique identifier for this thread.
//
// Use: public
//
int Thread::getThreadId() {
    Win32ThreadPrivateData *pd = static_cast<Win32ThreadPrivateData *> (_prvData);
    return pd->uniqueId;
}
//-----------------------------------------------------------------------------
//
// Description: Get the thread's process id
//
// Use: public
//
int Thread::getProcessId() {

    return (int) GetCurrentProcessId();

}
//-----------------------------------------------------------------------------
//
// Description: Determine if the thread is running
//
// Use: public
//
bool Thread::isRunning() {
    Win32ThreadPrivateData *pd = static_cast<Win32ThreadPrivateData *> (_prvData);
    return pd->isRunning;
}
//-----------------------------------------------------------------------------
//
// Description: Start the thread.
//
// Use: public
//
int Thread::start() {

    Win32ThreadPrivateData *pd = static_cast<Win32ThreadPrivateData *> (_prvData);
    //-------------------------------------------------------------------------
    // Prohibit the stack size from being changed.
    //
    pd->stackSizeLocked = true;
	unsigned long ID;

    pd->tid = CreateThread(NULL,pd->stackSize,ThreadPrivateActions::StartThread,static_cast<void *>(this),0,&ID);

	pd->uniqueId = (int)ID;

    if(pd->tid == INVALID_HANDLE_VALUE) {
		return -1;
    }

    pd->idSet = true;

    return 0;

}

int Thread::startThread() 
{ return start(); }

//-----------------------------------------------------------------------------
//
// Description: Join the thread.
//
// Use: public
//
int Thread::join() {
    Win32ThreadPrivateData *pd = static_cast<Win32ThreadPrivateData *> (_prvData);
	if( pd->detached ) 
		return -1; // cannot wait for detached ;

	if( WaitForSingleObject(pd->tid,INFINITE) != WAIT_OBJECT_0)
		return -1 ;

	return 0;
}



int Thread::detach()
{
    Win32ThreadPrivateData *pd = static_cast<Win32ThreadPrivateData *> (_prvData);
	pd->detached = true; 
	return 0;
}


//-----------------------------------------------------------------------------
//
// Description: Cancel the thread.
//
// Use: public
//
int Thread::cancel() {
    Win32ThreadPrivateData *pd = static_cast<Win32ThreadPrivateData *> (_prvData);

	if( pd->cancelMode == 2 )
		return -1;
	pd->isCanceled = true;

	// wait for 5 sec
	if( (pd->cancelMode == 1) || WaitForSingleObject(pd->tid, 5000) != WAIT_OBJECT_0)
	{	
		// did not terminate cleanly force termination
		return TerminateThread(pd->tid,(DWORD)-1);
	}
	return 0;
}



int Thread::testCancel()
{
    Win32ThreadPrivateData *pd = static_cast<Win32ThreadPrivateData *> (_prvData);

	if(!pd->isCanceled ) return 0;

	if(pd->cancelMode == 2)
		return 0;

	HANDLE curr = GetCurrentThread();

	if( pd->tid != curr )
		return -1;

	ExitThread(0);

	return 0;

}



//-----------------------------------------------------------------------------
//
// Description: Disable cancelibility
//
// Use: public
//
int Thread::setCancelModeDisable() {
    Win32ThreadPrivateData *pd = static_cast<Win32ThreadPrivateData *> (_prvData);
	pd->cancelMode = 2;
	return 0;
}

//-----------------------------------------------------------------------------
//
// Description: set the thread to cancel immediately
//
// Use: public
//
int Thread::setCancelModeAsynchronous() {
    Win32ThreadPrivateData *pd = static_cast<Win32ThreadPrivateData *> (_prvData);
	pd->cancelMode  = 1;
	return 0;
}

//-----------------------------------------------------------------------------
//
// Description: set the thread to cancel at the next convienent point.
//
// Use: public
//
int Thread::setCancelModeDeferred() {
    Win32ThreadPrivateData *pd = static_cast<Win32ThreadPrivateData *> (_prvData);
	pd->cancelMode = 0;
    return 0;
}

//-----------------------------------------------------------------------------
//
// Description: Set the thread's schedule priority (if able)
//
// Use: public
//
int Thread::setSchedulePriority(ThreadPriority priority) {
    Win32ThreadPrivateData *pd = static_cast<Win32ThreadPrivateData *> (_prvData);

    pd->threadPriority = priority;

    if(pd->isRunning) 
		return ThreadPrivateActions::SetThreadSchedulingParams(this);
    else 
		return 0;
}


//-----------------------------------------------------------------------------
//
// Description: Get the thread's schedule priority (if able)
//
// Use: public
//
int Thread::getSchedulePriority() {
    Win32ThreadPrivateData *pd = static_cast<Win32ThreadPrivateData *> (_prvData);
    return pd->threadPriority;
}

//-----------------------------------------------------------------------------
//
// Description: Set the thread's scheduling policy (if able)
//
// Use: public
//
int Thread::setSchedulePolicy(ThreadPolicy policy) {
    Win32ThreadPrivateData *pd = static_cast<Win32ThreadPrivateData *> (_prvData);

    pd->threadPolicy = policy;

    if(pd->isRunning) 
		return ThreadPrivateActions::SetThreadSchedulingParams(this);
    else 
		return 0;
}

//-----------------------------------------------------------------------------
//
// Description: Set the thread's scheduling policy (if able)
//
// Use: public
//
int Thread::getSchedulePolicy() {
    Win32ThreadPrivateData *pd = static_cast<Win32ThreadPrivateData *> (_prvData);
    return pd->threadPolicy;
}

//-----------------------------------------------------------------------------
//
// Description: Set the thread's desired stack size
//
// Use: public
//
int Thread::setStackSize(size_t stackSize) {
    Win32ThreadPrivateData *pd = static_cast<Win32ThreadPrivateData *> (_prvData);
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
	Win32ThreadPrivateData *pd = static_cast<Win32ThreadPrivateData *> (_prvData);
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
#if _WIN32_WINNT < 0x0400 // simulate
int SwitchToThread (void)
{
	Sleep(10);
	return 0;
};
#endif

int Thread::Yield()
{
    return SwitchToThread();
}

