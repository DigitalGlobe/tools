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
// Win32Barrier.c++ - C++ Barrier class built on top of POSIX threads.
// ~~~~~~~~~~~~~~~~~~
//

#include  <OpenThreads/Barrier>
#include "Win32BarrierPrivateData.h"
using namespace OpenThreads;

// so compiler can place it somewhere
Win32BarrierPrivateData::~Win32BarrierPrivateData()
{
};

//----------------------------------------------------------------------------
//
// Decription: Constructor
//
// Use: public.
//
Barrier::Barrier(int numThreads) {
    Win32BarrierPrivateData *pd = new Win32BarrierPrivateData();
    pd->cnt = 0;
    pd->phase = 0;
    pd->maxcnt = numThreads;
    _prvData = static_cast<void *>(pd);
}
//----------------------------------------------------------------------------
//
// Decription: Destructor
//
// Use: public.
//
Barrier::~Barrier() {
    Win32BarrierPrivateData *pd =
        static_cast<Win32BarrierPrivateData*>(_prvData);
    delete pd;
}
//----------------------------------------------------------------------------
//
// Decription: Reset the barrier to its original state
//
// Use: public.
//
void Barrier::reset() {
    Win32BarrierPrivateData *pd =
        static_cast<Win32BarrierPrivateData*>(_prvData);
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
    Win32BarrierPrivateData *pd =
        static_cast<Win32BarrierPrivateData*>(_prvData);

    if(numThreads != 0) pd->maxcnt = numThreads;
    int my_phase;

    pd->lock.lock();
    my_phase = pd->phase;
    ++pd->cnt;

    if (pd->cnt == pd->maxcnt) {             // I am the last one
		pd->cnt = 0;                         // reset for next use
		pd->phase = 1 - my_phase;            // toggle phase
		pd->cond.broadcast();
    }else{ 
	    while (pd->phase == my_phase) {
			pd->cond.wait(&pd->lock);

		}
	}
    pd->lock.unlock();
}
