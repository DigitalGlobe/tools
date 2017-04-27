/*
 *      PROGRAM:        JRD access method
 *      MODULE:         Database.cpp
 *      DESCRIPTION:    Common descriptions
 *
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 *
 * Sean Leyne
 * Claudio Valderrama C.
 */

#include "firebird.h"
// Definition of block types for data allocation in JRD
#include "../include/fb_blk.h"

#include "../jrd/ibase.h"
#include "../jrd/common.h"
#include "../jrd/ods.h"
#include "../jrd/lck.h"
#include "../jrd/Database.h"
#include "../jrd/nbak.h"
#include "../jrd/tra.h"
#include "../jrd/lck_proto.h"
#include "../jrd/os/pio_proto.h"

// Thread data block
#include "../jrd/ThreadData.h"

// recursive mutexes
#include "../common/thd.h"

namespace Jrd
{
	bool Database::onRawDevice() const
	{
#ifdef SUPPORT_RAW_DEVICES
		return PIO_on_raw_device(dbb_filename);
#else
		return false;
#endif
	}

	Firebird::string Database::getUniqueFileId() const
	{
		const PageSpace* const pageSpace = dbb_page_manager.findPageSpace(DB_PAGE_SPACE);

		Firebird::UCharBuffer buffer;
		PIO_get_unique_file_id(pageSpace->file, buffer);

		Firebird::string file_id;
		char* s = file_id.getBuffer(2 * buffer.getCount());
		for (size_t i = 0; i < buffer.getCount(); i++)
		{
			sprintf(s, "%02x", (int) buffer[i]);
			s += 2;
		}

		return file_id;
	}

	int Database::blocking_ast_sweep(void* ast_object)
	{
		try
		{
			Database* dbb = static_cast<Database*>(ast_object);

			AstContextHolder tdbb(dbb);
			Jrd::ContextPoolHolder context(tdbb, dbb->dbb_permanent);

			if (dbb->dbb_flags & DBB_sweep_starting && !(dbb->dbb_flags &  DBB_sweep_in_progress))
			{
				dbb->dbb_flags &= ~DBB_sweep_starting;
				LCK_release(tdbb, dbb->dbb_sweep_lock);
			}
		}
		catch (const Firebird::Exception&)
		{} // no-op

		return 0;
	}

	Lock* Database::createSweepLock(thread_db* tdbb)
	{
		if (!dbb_sweep_lock) 
		{
			dbb_sweep_lock = FB_NEW_RPT(*dbb_permanent, 0) Lock();
			dbb_sweep_lock->lck_dbb = this;
			dbb_sweep_lock->lck_type = LCK_sweep;
			dbb_sweep_lock->lck_owner_handle = LCK_get_owner_handle(tdbb, dbb_sweep_lock->lck_type);
			dbb_sweep_lock->lck_parent = dbb_lock;
			dbb_sweep_lock->lck_length = 0;
			dbb_sweep_lock->lck_ast = blocking_ast_sweep;
			dbb_sweep_lock->lck_object = this;
		}
		return dbb_sweep_lock;
	}

	bool Database::allowSweepThread(thread_db* tdbb)
	{
		if (dbb_flags & DBB_read_only)
			return false;

		Jrd::Attachment* const attachment = tdbb->getAttachment();
		if (attachment->att_flags & ATT_no_cleanup)
			return false;

		if ((dbb_flags & (DBB_sweep_in_progress | DBB_sweep_starting)) || (dbb_ast_flags & DBB_shutdown))
			return false;

		dbb_flags |= DBB_sweep_starting;

		createSweepLock(tdbb);
		if (!LCK_lock(tdbb, dbb_sweep_lock, LCK_EX, LCK_NO_WAIT))
		{
			// clear lock error from status vector
			fb_utils::init_status(tdbb->tdbb_status_vector);

			dbb_flags &= ~DBB_sweep_starting;
			return false;
		}

		return true;
	}

	bool Database::allowSweepRun(thread_db* tdbb)
	{
		if (dbb_flags & DBB_read_only)
			return false;

		Jrd::Attachment* const attachment = tdbb->getAttachment();
		if (attachment->att_flags & ATT_no_cleanup)
			return false;

		if (dbb_flags & DBB_sweep_in_progress)
			return false;

		dbb_flags |= DBB_sweep_in_progress;

		if (!(dbb_flags & DBB_sweep_starting))
		{
			createSweepLock(tdbb);
			if (!LCK_lock(tdbb, dbb_sweep_lock, LCK_EX, -1))
			{
				// clear lock error from status vector
				fb_utils::init_status(tdbb->tdbb_status_vector);

				dbb_flags &= ~DBB_sweep_in_progress;
				return false;
			}
		}
		else
		{
			dbb_flags &= ~DBB_sweep_starting;
		}

		return true;
	}

	void Database::clearSweepFlags(thread_db* tdbb)
	{
		if (!(dbb_flags & (DBB_sweep_starting | DBB_sweep_in_progress)))
			return;

		if (dbb_sweep_lock) {
			LCK_release(tdbb, dbb_sweep_lock);
		}
		dbb_flags &= ~(DBB_sweep_in_progress | DBB_sweep_starting);
	}

	Database::~Database()
	{
		delete dbb_sys_trans;

		destroyIntlObjects();

		while (dbb_sort_buffers.hasData())
			delete dbb_sort_buffers.pop();

		fb_assert(dbb_pools[0] == dbb_permanent);
		for (size_t i = 1; i < dbb_pools.getCount(); ++i)
		{
			MemoryPool::deletePool(dbb_pools[i]);
		}

		delete dbb_monitoring_data;
		delete dbb_backup_manager;

		fb_assert(!locked());
		// This line decrements the usage counter and may cause the destructor to be called.
		// It should happen with the dbb_sync unlocked.
		LockManager::destroy(dbb_lock_mgr);
		EventManager::destroy(dbb_event_mgr);
	}

	void Database::deletePool(MemoryPool* pool)
	{
		if (pool)
		{
			size_t pos;
			if (dbb_pools.find(pool, pos))
			{
				dbb_pools.remove(pos);
			}

			MemoryPool::deletePool(pool);
		}
	}

	// Database::SharedCounter implementation

	Database::SharedCounter::SharedCounter()
	{
		memset(m_counters, 0, sizeof(m_counters));
	}

	Database::SharedCounter::~SharedCounter()
	{
		for (size_t i = 0; i < TOTAL_ITEMS; i++)
		{
			delete m_counters[i].lock;
		}
	}

	void Database::SharedCounter::shutdown(thread_db* tdbb)
	{
		for (size_t i = 0; i < TOTAL_ITEMS; i++)
		{
			if (m_counters[i].lock)
				LCK_release(tdbb, m_counters[i].lock);
		}
	}

	SLONG Database::SharedCounter::generate(thread_db* tdbb, ULONG space, ULONG prefetch)
	{
		fb_assert(space < TOTAL_ITEMS);
		ValueCache* const counter = &m_counters[space];
		Database* const dbb = tdbb->getDatabase();

		if (!counter->lock)
		{
			Lock* const lock = FB_NEW_RPT(*dbb->dbb_permanent, sizeof(SLONG)) Lock();
			counter->lock = lock;
			lock->lck_type = LCK_shared_counter;
			lock->lck_owner_handle = LCK_get_owner_handle(tdbb, lock->lck_type);
			lock->lck_parent = dbb->dbb_lock;
			lock->lck_length = sizeof(SLONG);
			lock->lck_key.lck_long = space;
			lock->lck_dbb = dbb;
			LCK_lock(tdbb, lock, LCK_PW, LCK_WAIT);

			counter->curVal = 1;
			counter->maxVal = 0;
		}

		if (counter->curVal > counter->maxVal)
		{
			LCK_convert(tdbb, counter->lock, LCK_PW, LCK_WAIT);
			counter->curVal = LCK_read_data(tdbb, counter->lock);

			if (!counter->curVal)
			{
				// zero IDs are somewhat special, so let's better skip them
				counter->curVal = 1;
			}

			counter->maxVal = counter->curVal + prefetch - 1;
			LCK_write_data(tdbb, counter->lock, counter->maxVal + 1);
			LCK_convert(tdbb, counter->lock, LCK_SR, LCK_WAIT);
		}

		return counter->curVal++;
	}

} // namespace
