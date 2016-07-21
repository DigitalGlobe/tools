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
// PThreadConditionPrivateData.h - Private data structure for Condition
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//

#ifndef _PTHREADCONDITIONPRIVATEDATA_H_
#define _PTHREADCONDITIONPRIVATEDATA_H_

namespace OpenThreads {

#include <pthread.h>
#include <OpenThreads/Condition>

class PThreadConditionPrivateData {

    friend class Condition;

private:

    PThreadConditionPrivateData() {};

    virtual ~PThreadConditionPrivateData() {};

    pthread_cond_t condition;

};

}

#endif // !_PTHREADCONDITIONPRIVATEDATA_H_
