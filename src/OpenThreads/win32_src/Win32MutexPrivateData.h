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
// Win32MutexPrivateData.h - Private data structure for Mutex
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef _Win32MUTEXPRIVATEDATA_H_
#define _Win32MUTEXPRIVATEDATA_H_

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace OpenThreads {

class Win32MutexPrivateData {
    friend class Mutex;
    friend class Condition;

private:

    Win32MutexPrivateData() {};

    ~Win32MutexPrivateData();

    volatile unsigned long mutex;

};

}

#endif // !_Win32MUTEXPRIVATEDATA_H_





