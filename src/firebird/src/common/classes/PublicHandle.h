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

#ifndef COMMON_PUBLIC_HANDLE
#define COMMON_PUBLIC_HANDLE

#include "../common/classes/init.h"
#include "../common/classes/array.h"
#include "../common/classes/RefMutex.h"
#include "../common/classes/fb_atomic.h"

namespace Firebird
{
	class RWLock;

	class ExecuteWithLock
	{
	public:
		virtual void execute() = 0;
	};

	class ExistenceMutex : public RefMutex	// master reference mutex - controls object existence
											// together with flag objectExists (a few lines lower)
	{
	public:
		Mutex astMutex;						// Mutex taken when entering AST
		AtomicCounter astDisabled;			// When >0 - AST returns taking no actions
											// Incremented/checked with astMutex locked
											// Atomic is used to decrement asynchronously
		bool objectExists;					// Main flag, reset when object is destroyed
											// Modified/checked with base-class mutex locked
		ExistenceMutex()
			: objectExists(true)
		{ }
	};

	class PublicHandle : public RefPtr<ExistenceMutex>
	{
	public:
		PublicHandle();
		~PublicHandle();

		ExistenceMutex* isKnownHandle() const;
		ExistenceMutex* mutex() const
		{
			return (ExistenceMutex*)(*const_cast<PublicHandle*>(this));
		}
		bool executeWithLock(ExecuteWithLock* operation);

	private:
		static GlobalPtr<SortedArray<const void*> > handles;
		static GlobalPtr<RWLock> sync;
	};

	class PublicHandleHolder
	{
	public:
		PublicHandleHolder();
		PublicHandleHolder(PublicHandle*, const char* from);
		~PublicHandleHolder();

		bool hold(PublicHandle* handle, const char* from);

	private:
		ExistenceMutex* mutex;
		void destroy();
	};

} // namespace

#endif // COMMON_PUBLIC_HANDLE
