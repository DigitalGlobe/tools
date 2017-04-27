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
// Win32Condition.c++ - C++ Condition class built on top of posix threads.
// ~~~~~~~~~~~~~~~~~~~~
//
#include <OpenThreads/Condition>
#include "Win32ConditionPrivateData.h"

using namespace OpenThreads;
Win32ConditionPrivateData::~Win32ConditionPrivateData()
{
}

//----------------------------------------------------------------------------
//
// Decription: Constructor
//
// Use: public.
//
Condition::Condition() {
    Win32ConditionPrivateData *pd =
        new Win32ConditionPrivateData();
    _prvData = static_cast<void *>(pd);
}
//----------------------------------------------------------------------------
//
// Decription: Destructor
//
// Use: public.
//
Condition::~Condition() {
    Win32ConditionPrivateData *pd =
       static_cast<Win32ConditionPrivateData *>(_prvData);

    delete pd;
}

//----------------------------------------------------------------------------
//
// Decription: wait on a condition
//
// Use: public.
//
int Condition::wait(Mutex *mutex) {

    Win32ConditionPrivateData *pd =
        static_cast<Win32ConditionPrivateData *>(_prvData);

    return pd->wait(*mutex, INFINITE);
}
//----------------------------------------------------------------------------
//
// Decription: wait on a condition, for a specified period of time
//
// Use: public.
//
int Condition::wait(Mutex *mutex, unsigned long ms) {
    Win32ConditionPrivateData *pd =
        static_cast<Win32ConditionPrivateData *>(_prvData);

    return pd->wait(*mutex, ms);
}
//----------------------------------------------------------------------------
//
// Decription: signal a thread to wake up.
//
// Use: public.
//
int Condition::signal() {
    Win32ConditionPrivateData *pd =
        static_cast<Win32ConditionPrivateData *>(_prvData);
    return pd->signal();
}
//----------------------------------------------------------------------------
//
// Decription: signal many threads to wake up.
//
// Use: public.
//
int Condition::broadcast() {
    Win32ConditionPrivateData *pd =
        static_cast<Win32ConditionPrivateData *>(_prvData);
    return pd->broadcast();
}
