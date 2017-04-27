/*
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
 * Adriano dos Santos Fernandes - refactored from pass1.cpp, gen.cpp, cmp.cpp, par.cpp and exe.cpp
 */

#include "firebird.h"
#include "../jrd/common.h"
#include "../dsql/StmtNodes.h"
#include "../jrd/jrd.h"
#include "../jrd/blr.h"
#include "../jrd/exe.h"
#include "../jrd/tra.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/exe_proto.h"
#include "../jrd/par_proto.h"
#include "../jrd/tra_proto.h"
#include "../jrd/vio_proto.h"
#include "../dsql/gen_proto.h"
#include "../dsql/pass1_proto.h"

using namespace Jrd;

#include "gen/blrtable.h"


namespace Jrd {


template <typename T>
class RegisterNode
{
public:
	explicit RegisterNode(UCHAR blr)
	{
		PAR_register(blr, T::parse);
	}
};


//--------------------


DmlNode* DmlNode::pass2(thread_db* tdbb, CompilerScratch* csb, jrd_nod* aNode)
{
	node = aNode;
	return pass2(tdbb, csb);
}


//--------------------


RegisterNode<InAutonomousTransactionNode> regInAutonomousTransactionNode(blr_auto_trans);


DmlNode* InAutonomousTransactionNode::parse(thread_db* tdbb, MemoryPool& pool,
		CompilerScratch* csb)
{
	InAutonomousTransactionNode* node = FB_NEW(pool) InAutonomousTransactionNode(pool);

	if (csb->csb_blr_reader.getByte() != 0)	// Reserved for future improvements. Should be 0 for now.
		PAR_syntax_error(csb, "0");

	node->action = PAR_parse_node(tdbb, csb, STATEMENT);

	return node;
}


InAutonomousTransactionNode* InAutonomousTransactionNode::dsqlPass()
{
	const bool autoTrans = compiledStatement->req_flags & REQ_in_auto_trans_block;
	compiledStatement->req_flags |= REQ_in_auto_trans_block;

	InAutonomousTransactionNode* node = FB_NEW(getPool()) InAutonomousTransactionNode(getPool());
	node->compiledStatement = compiledStatement;
	node->dsqlAction = PASS1_statement(compiledStatement, dsqlAction);

	if (!autoTrans)
		compiledStatement->req_flags &= ~REQ_in_auto_trans_block;

	return node;
}


void InAutonomousTransactionNode::print(Firebird::string& text,
	Firebird::Array<dsql_nod*>& nodes) const
{
	text = "in autonomous transaction";
	nodes.add(dsqlAction);
}


void InAutonomousTransactionNode::genBlr()
{
	stuff(compiledStatement, blr_auto_trans);
	stuff(compiledStatement, 0);	// to extend syntax in the future
	GEN_statement(compiledStatement, dsqlAction);
}


InAutonomousTransactionNode* InAutonomousTransactionNode::pass1(thread_db* tdbb, CompilerScratch* csb)
{
	action = CMP_pass1(tdbb, csb, action);
	return this;
}


InAutonomousTransactionNode* InAutonomousTransactionNode::pass2(thread_db* tdbb, CompilerScratch* csb)
{
	impureOffset = CMP_impure(csb, sizeof(Impure));
	action = CMP_pass2(tdbb, csb, action, node);
	return this;
}


jrd_nod* InAutonomousTransactionNode::execute(thread_db* tdbb, jrd_req* request)
{
	Database* const dbb = tdbb->getDatabase();
	Attachment* const attachment = tdbb->getAttachment();

	Impure* const impure = (Impure*) ((UCHAR*) request + impureOffset);

	if (request->req_operation == jrd_req::req_evaluate)
	{
		// Force unconditional reschedule. It prevents new transactions being
		// started after an attachment or a database shutdown has been initiated.
		JRD_reschedule(tdbb, 0, true);

		jrd_tra* const org_transaction = request->req_transaction;
		fb_assert(tdbb->getTransaction() == org_transaction);

		jrd_tra* const transaction = TRA_start(tdbb, org_transaction->tra_flags,
											   org_transaction->tra_lock_timeout,
											   org_transaction);

		TRA_attach_request(transaction, request);
		tdbb->setTransaction(transaction);

		request->req_auto_trans.push(org_transaction);
		impure->traNumber = transaction->tra_number;

		VIO_start_save_point(tdbb, transaction);
		impure->savNumber = transaction->tra_save_point->sav_number;

		if (!(attachment->att_flags & ATT_no_db_triggers))
		{
			// run ON TRANSACTION START triggers
			EXE_execute_db_triggers(tdbb, transaction, jrd_req::req_trigger_trans_start);
		}

		return action;
	}

	jrd_tra* transaction = request->req_transaction;
	fb_assert(transaction);
	fb_assert(transaction != dbb->dbb_sys_trans);

	if (!impure->traNumber)
		return node->nod_parent;

	fb_assert(transaction->tra_number == impure->traNumber);

	switch (request->req_operation)
	{
	case jrd_req::req_return:
		if (!(attachment->att_flags & ATT_no_db_triggers))
		{
			// run ON TRANSACTION COMMIT triggers
			EXE_execute_db_triggers(tdbb, transaction, jrd_req::req_trigger_trans_commit);
		}

		if (transaction->tra_save_point &&
			!(transaction->tra_save_point->sav_flags & SAV_user) &&
			!transaction->tra_save_point->sav_verb_count)
		{
			VIO_verb_cleanup(tdbb, transaction);
		}

		{ // scope
			Firebird::AutoSetRestore2<jrd_req*, thread_db> autoNullifyRequest(
				tdbb, &thread_db::getRequest, &thread_db::setRequest, NULL);
			TRA_commit(tdbb, transaction, false);
		} // end scope
		break;

	case jrd_req::req_unwind:
		if (request->req_flags & req_leave)
		{
			try
			{
				if (!(attachment->att_flags & ATT_no_db_triggers))
				{
					// run ON TRANSACTION COMMIT triggers
					EXE_execute_db_triggers(tdbb, transaction,
											jrd_req::req_trigger_trans_commit);
				}

				if (transaction->tra_save_point &&
					!(transaction->tra_save_point->sav_flags & SAV_user) &&
					!transaction->tra_save_point->sav_verb_count)
				{
					VIO_verb_cleanup(tdbb, transaction);
				}

				Firebird::AutoSetRestore2<jrd_req*, thread_db> autoNullifyRequest(
					tdbb, &thread_db::getRequest, &thread_db::setRequest, NULL);
				TRA_commit(tdbb, transaction, false);
			}
			catch (...)
			{
				request->req_flags &= ~req_leave;
				throw;
			}
		}
		else
		{
			ThreadStatusGuard temp_status(tdbb);

			if (!(attachment->att_flags & ATT_no_db_triggers))
			{
				try
				{
					// run ON TRANSACTION ROLLBACK triggers
					EXE_execute_db_triggers(tdbb, transaction,
											jrd_req::req_trigger_trans_rollback);
				}
				catch (const Firebird::Exception&)
				{
					if (dbb->dbb_flags & DBB_bugcheck)
						throw;
				}
			}

			try
			{
				Firebird::AutoSetRestore2<jrd_req*, thread_db> autoNullifyRequest(
					tdbb, &thread_db::getRequest, &thread_db::setRequest, NULL);

				// undo all savepoints up to our one
				for (const Savepoint* save_point = transaction->tra_save_point;
					save_point && impure->savNumber <= save_point->sav_number;
					save_point = transaction->tra_save_point)
				{
					++transaction->tra_save_point->sav_verb_count;
					VIO_verb_cleanup(tdbb, transaction);
				}

				TRA_rollback(tdbb, transaction, false, false);
			}
			catch (const Firebird::Exception&)
			{
				if (dbb->dbb_flags & DBB_bugcheck)
					throw;
			}
		}
		break;

	default:
		fb_assert(false);
	}

	impure->traNumber = impure->savNumber = 0;
	transaction = request->req_auto_trans.pop();

	TRA_attach_request(transaction, request);
	tdbb->setTransaction(transaction);

	return node->nod_parent;
}


}	// namespace Jrd
