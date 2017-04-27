/*
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Dmitry Yemanov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2008 Dmitry Yemanov <dimitr@users.sf.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "gen/iberror.h"
#include "../jrd/gdsassert.h"
#include "rwlock.h"
#include "PublicHandle.h"

namespace Firebird
{
	GlobalPtr<SortedArray<const void*> > PublicHandle::handles;
	GlobalPtr<RWLock> PublicHandle::sync;

	PublicHandle::PublicHandle()
		: RefPtr<ExistenceMutex>(FB_NEW(*getDefaultMemoryPool()) ExistenceMutex)
	{
		WriteLockGuard guard(sync);

		if (handles->exist(this))
		{
			fb_assert(false);
		}
		else
		{
			handles->add(this);
		}
	}

	PublicHandle::~PublicHandle()
	{
		WriteLockGuard guard(sync);

		mutex()->objectExists = false;

		size_t pos;
		if (handles->find(this, pos))
		{
			handles->remove(pos);
		}
		else
		{
			fb_assert(false);
		}
	}

	ExistenceMutex* PublicHandle::isKnownHandle() const
	{
		ReadLockGuard guard(sync);
		if (handles->exist(this))
		{
			mutex()->addRef();
			return mutex();
		}

		return NULL;
	}

	bool PublicHandle::executeWithLock(ExecuteWithLock* operation)
	{
		ReadLockGuard guard(sync);
		if (handles->exist(this))
		{
			operation->execute();
			return true;
		}
		return false;
	}

	PublicHandleHolder::PublicHandleHolder()
		: mutex(NULL)
	{ }

	PublicHandleHolder::PublicHandleHolder(PublicHandle* handle, const char* from)
		: mutex(NULL)
	{
		if (!hold(handle, from))
		{
			fb_assert(false);
			(Arg::Gds(isc_random) << "Public object unexpectedly lost").raise();
		}
	}

	bool PublicHandleHolder::hold(PublicHandle* handle, const char* from)
	{
		mutex = handle->isKnownHandle();
		if (mutex)
		{
			mutex->enter(from);
			if (mutex->objectExists)
			{
				return true;
			}
			destroy();
			mutex = NULL;
		}
		return false;
	}

	PublicHandleHolder::~PublicHandleHolder()
	{
		if (mutex)
		{
			destroy();
		}
	}

	void PublicHandleHolder::destroy()
	{
		try
		{
			mutex->leave();
		}
		catch (const Firebird::Exception&)
		{
			DtorException::devHalt();
		}
		mutex->release();
	}
} // namespace Firebird
