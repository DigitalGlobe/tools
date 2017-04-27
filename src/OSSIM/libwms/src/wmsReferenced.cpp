/* -*-c++-*- libwms - Copyright (C) since 2004 Garrett Potts
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the libwms Public License (WMSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * libwms Public License for more details.
*/
#include <typeinfo>
#include <wms/wmsReferenced.h>
#include <wms/wmsNotify.h>

wmsReferenced::~wmsReferenced()
{
    if (theRefCount>0)
    {
        wmsNotify(wmsNotifyLevel_WARN)<<"Warning: deleting still referenced object "<<this<<" of type '"<<typeid(this).name()<<"'"<<std::endl;
        wmsNotify(wmsNotifyLevel_WARN)<<"         the final reference count was "<<theRefCount<<", memory corruption possible."<<std::endl;
    }
}
