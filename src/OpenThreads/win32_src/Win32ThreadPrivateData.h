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
// Win32PrivateData.h - Private data structure for Thread
// ~~~~~~~~~~~~~~~~~~~~~
#ifndef _Win32PRIVATEDATA_H_
#define _Win32PRIVATEDATA_H_

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0400
#include <windows.h>
#endif

#include <OpenThreads/Thread>

namespace OpenThreads {

	class Win32ThreadPrivateData {
    //-------------------------------------------------------------------------
    // We're friendly to Thread, so it can use our data.
    //
    friend class Thread;
    //-------------------------------------------------------------------------
    // We're friendly to Win32PrivateActions, so it can get at some 
    // variables.
    //
    friend class ThreadPrivateActions;

private:

    Win32ThreadPrivateData() {};
    ~Win32ThreadPrivateData();

    unsigned int stackSize;
    bool stackSizeLocked;
    bool isRunning;

    volatile bool isCanceled;
	int  cancelMode; // 0 - deffered (default) 1-asynch 2-disabled  

    bool detached;
    bool idSet;

    Thread::ThreadPriority threadPriority;

    Thread::ThreadPolicy threadPolicy;

    HANDLE tid;

    int uniqueId;
    static int nextId;

public:

	struct TlsHolder{ // thread local storage slot
		DWORD ID;
		TlsHolder(): ID(TlsAlloc()){
		}
		~TlsHolder(){
			TlsFree(ID);
		}
	};

	static TlsHolder TLS;

};







}







#endif // !_PTHREADPRIVATEDATA_H_



