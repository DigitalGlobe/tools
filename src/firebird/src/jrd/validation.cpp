/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		validation.cpp
 *	DESCRIPTION:	Validation and garbage collection
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
 */


/*

                      Database Validation and Repair
                      ==============================

                              Deej Bredenberg
                              March 16, 1994
			Updated: 1996-Dec-11 David Schnepper


I. TERMINOLOGY

   The following terminology will be helpful to understand in this discussion:

   record fragment: The smallest recognizable piece of a record; multiple
                    fragments can be linked together to form a single version.
   record version:  A single version of a record representing an INSERT, UPDATE
                    or DELETE by a particular transaction (note that deletion
                    of a record causes a new version to be stored as a
                    deleted stub).
   record chain:    A linked list of record versions chained together to
                    represent a single logical "record".
   slot:            The line number of the record on page.  A
                    variable-length array on each data page stores the
                    offsets to the stored records on
                    that page, and the slot is an index into that array.

   For more information on data page format, see my paper on the internals
   of the InterBase Engine.

II. COMMAND OPTIONS

   Here are all the options for gfix which have to do with validation, and
   what they do:

   gfix switch   dpb parameter
   -----------   -------------

   -validate    isc_dpb_verify  (gds__dpb_verify prior to 4.0)

    Invoke validation and repair.  All other switches modify this switch.

   -full        isc_dpb_records

    Visit all records.  Without this switch, only page structures will be
    validated, which does involve some limited checking of records.

   -mend        isc_dpb_repair

    Attempts to mend the database where it can to make it viable for reading;
    does not guarantee to retain data.

   -no_update   isc_dpb_no_update

    Specifies that orphan pages not be released, and allocated pages not
    be marked in use when found to be free.  Actually a misleading switch
    name since -mend will update the database, but if -mend is not specified
    and -no_update is specified, then no updates will occur to the database.

   -ignore      isc_dpb_ignore

    Tells the engine to ignore checksums in fetching pages.  Validate will
    report on the checksums, however.  Should probably not even be a switch,
    it should just always be in effect.  Otherwise checksums will disrupt
    the validation.  Customers should be advised to always use it.
    NOTE: Unix 4.0 (ODS 8.0) does not have on-page checksums, and all
          platforms under ODS 9.0 (NevaStone & above) does not have
          checksums.

III.  OPERATION

   Validation runs only with exclusive access to the database, to ensure
   that database structures are not modified during validation.  On attach,
   validate attempts to obtain an exclusive lock on the database.

   If other attachments are already made locally or through the same multi-
   client server, validate gives up with the message:

   "Lock timeout during wait transaction
   -- Object "database_filename.fdb" is in use"

   If other processes or servers are attached to the database, validate
   waits for the exclusive lock on the database (i.e. waits for every
   other server to get out of the database).

   NOTE: Ordinarily when processes gain exclusive access to the database,
   all active transactions are marked as dead on the Transaction Inventory
   Pages.  This feature is turned off for validation.

IV. PHASES OF VALIDATION

   There are two phases to the validation, the first of which is a walk through
   the entire database (described below).  During this phase, all pages visited
   are stored in a bitmap for later use during the garbage collection phase.

   A. Visiting Pages

      During the walk-through phase, any page that is fetched goes through a
      basic validation:

      1. Page Type Check

         Each page is check against its expected type.  If the wrong type
         page is found in the page header, the message:

         "Page xxx wrong type (expected xxx encountered xxx)"

         is returned.  This could represent a) a problem with the database
         being overwritten, b) a bug with InterBase page allocation mechanisms
         in which one page was written over another, or c) a page which was
         allocated but never written to disk (most likely if the encountered
         page type was 0).

         The error does not tell you what page types are what, so here
         they are for reference:

         define pag_undefined     0    // purposely undefined
         define pag_header        1    // Database header page
         define pag_pages         2    // Page inventory page
         define pag_transactions  3    // Transaction inventory page
         define pag_pointer       4    // Pointer page
         define pag_data          5    // Data page
         define pag_root          6    // Index root page
         define pag_index         7    // Index (B-tree) page
         define pag_blob          8    // Blob data page
         define pag_ids           9    // Gen-ids
         define pag_log           10   // Write ahead log page: 4.0 only

      2. Checksum

         If -ignore is specified, the checksum is specifically checked in
         validate instead of in the engine.  If the checksum is found to
         be wrong, the error:

         "Checksum error on page xxx"

         is returned.  This is harmless when found by validate, and the page
         will still continue to be validated--if data structures can be
         validated on page, they will be.  If -mend is specified, the page
         will be marked for write, so that when the page is written to disk
         at the end of validation the checksum will automatically be
         recalculated.

         Note: For 4.0 only Windows & NLM platforms keep page checksums.

      3. Revisit

         We check each page fetched against the page bitmap to make sure we
         have not visited already.  If we have, the error:

          "Page xxx doubly allocated"

         is returned.  This should catch the case when a page of the same type
         is allocated for two different purposes.

	 Data pages are not checked with the Revisit mechanism - when walking
         record chains and fragments they are frequently revisited.

   B. Garbage Collection

      During this phase, the Page Inventory (PIP) pages are checked against the
      bitmap of pages visited.  Two types of errors can be detected during
      this phase.

      1. Orphan Pages

         If any pages in the page inventory were not visited
         during validation, the following error will be returned:

         "Page xxx is an orphan"

         If -no_update was not specified, the page will be marked as free
         on the PIP.

      2. Improperly Freed Pages

         If any pages marked free in the page inventory were in fact
         found to be in use during validation, the following error
         will be returned:

         "Page xxx is use but marked free"  (sic)

         If -no_update was not specified, the page will be marked in use
         on the PIP.

      NOTE:  If errors were found during the validation phase, no changes will
      be made to the PIP pages.  This assumes that we did not have a chance to
      visit all the pages because invalid structures were detected.

V. WALK-THROUGH PHASE

   A. Page Fetching

      In order to ensure that all pages are fetched during validation, the
      following pages are fetched just for the most basic validation:

      1. The header page (and for 4.0 any overflow header pages).
      2. Log pages for after-image journalling (4.0 only).
      3. Page Inventory pages.
      4. Transaction Inventory pages

         If the system relation RDB$PAGES could not be read or did not
         contain any TIP pages, the message:

         "Transaction inventory pages lost"

         will be returned.  If a particular page is missing from the
         sequence as established by RDB$PAGE_SEQUENCE, then the following
         message will be returned:

         "Transaction inventory page lost, sequence xxx"

         If -mend is specified, then a new TIP will be allocated on disk and
         stored in RDB$PAGES in the proper sequence.  All transactions which
         would have been on that page are assumed committed.

         If a TIP page does not point to the next one in sequence, the
         following message will be returned:

         "Transaction inventory pages confused, sequence xxx"

      5. Generator pages as identified in RDB$PAGES.

   B. Relation Walking

      All the relations in the database are walked.  For each relation, all
      indices defined on the relation are fetched, and all pointer and
      data pages associated with the relation are fetched (see below).

      But first, the metadata is scanned from RDB$RELATIONS to fetch the
      format of the relation.  If this information is missing or
      corrupted the relation cannot be walked.
      If any bugchecks are encountered from the scan, the following
      message is returned:

      "bugcheck during scan of table xxx (<table_name>)"

      This will prevent any further validation of the relation.

      NOTE: For views, the metadata is scanned but nothing further is done.

   C. Index Walking

      Prior to 4.5 (NevaStone) Indices were walked before data pages.
      In NevaStone Index walking was moved to after data page walking.
      Please refer to the later section entitled "Index Walking".

   D. Pointer Pages

      All the pointer pages for the relation are walked.  As they are walked
      all child data pages are walked (see below).  If a pointer page cannot
      be found, the following message is returned:

      "Pointer page (sequence xxx) lost"

      If the pointer page is not part of the relation we expected or
      if it is not marked as being in the proper sequence, the following
      message is returned:

      "Pointer page xxx is inconsistent"

      If each pointer page does not point to the next pointer page as
      stored in the RDB$PAGE_SEQUENCE field in RDB$PAGES, the following
      error is returned:

      "Pointer page (sequence xxx) inconsistent"

   E. Data Pages

      Each of the data pages referenced by the pointer page is fetched.
      If any are found to be corrupt at the page level, and -mend is
      specified, the page is deleted from its pointer page.  This will
      cause a whole page of data to be lost.

      The data page is corrupt at the page level if it is not marked as
      part of the current relation, or if it is not marked as being in
      the proper sequence.  If either of these conditions occurs, the
      following error is returned:

      "Data page xxx (sequence xxx) is confused"

   F. Slot Validation

      Each of the slots on the data page is looked at, up to the count
      of records stored on page.  If the slot is non-zero, the record
      fragment at the specified offset is retrieved.  If the record
      begins before the end of the slots array, or continues off the
      end of the page, the following error is returned:

      "Data page xxx (sequence xxx), line xxx is bad"

      where "line" means the slot number.

      NOTE: If this condition is encountered, the data page is considered
      corrupt at the page level (and thus will be removed from its
      pointer page if -mend is specified).

   G. Record Validation

      The record at each slot is looked at for basic validation, regardless
      of whether -full is specified or not.  The fragment could be any of the
      following:

      1.  Back Version

          If the fragment is marked as a back version, then it is skipped.
          It will be fetched as part of its record.

      2.  Corrupt

          If the fragment is determined to be corrupt for any reason, and -mend
          is specified, then the record header is marked as damaged.

      3.  Damaged

          If the fragment is marked damaged already from a previous visit or
          a previous validation, the following error is returned:

          "Record xxx is marked as damaged"

          where xxx is the record number.

      4.  Bad Transaction

          If the record is marked with a transaction id greater than the last
          transaction started in the database, the following error is returned:

          "Record xxx has bad transaction xxx"

   H. Record Walking

      If -full is specified, and the fragment is the first fragment in a logical
      record, then the record at this slot number is fully retrieved.  This
      involves retrieving all versions, and all fragments of each
      particular version.  In other
      words, the entire logical record will be retrieved.

      1. Back Versions

         If there are any back versions, they are visited at this point.
         If the back version is on another page, the page is fetched but
         not validated since it will be walked separately.

         If the slot number of the back version is greater than the max
         records on page, or there is no record stored at that slot number,
         or it is a blob record, or it is a record fragment, or the
         fragment itself is invalid, the following error
         message is returned:

         "Chain for record xxx is broken"

      2. Incomplete

         If the record header is marked as incomplete, it means that there
         are additional fragments to be fetched--the record was too large
         to be stored in one slot.
         A pointer is stored in the record to the next fragment in the list.

         For fragmented records, all fragments are fetched to form a full
         record version.  If any of the fragments is not in a valid position,
         or is not the correct length, the following error is returned:

         "Fragmented record xxx is corrupt"

      Once the full record has been retrieved, the length of the format is
      checked against the expected format stored in RDB$FORMATS (the
      format number is stored with the record, representing the exact
      format of the relation at the time the record was stored.)
      If the length of the reconstructed record does not match
      the expected format length, the following error is returned:

      "Record xxx is wrong length"

      For delta records (record versions which represent updates to the record)
      this check is not made.

   I. Blob Walking

      If the slot on the data page points to a blob record, then the blob
      is fetched (even without -full).  This has several cases, corresponding
      to the various blob levels.  (See the "Engine Internals" document for a
      discussion of blob levels.)

      Level                      Action
      -----   -----------------------------------------------------------------
        0     These are just records on page, and no further validation is done.
        1     All the pages pointed to by the blob record are fetched and
              validated in sequence.
        2     All pages pointed to by the blob pointer pages are fetched and
              validated.
        3     The blob page is itself a blob pointer page; all its children
              are fetched and validated.

      For each blob page found, some further validation is done.  If the
      page does not point back to the lead page, the following error
      is returned:

      "Warning: blob xxx appears inconsistent"

      where xxx corresponds to the blob record number.  If any of the blob pages
      are not marked in the sequence we expect them to be in, the following
      error is returned:

      "Blob xxx is corrupt"

      Tip: the message for the same error in level 2 or 3 blobs is slightly
           different:

      "Blob xxx corrupt"

      If we have lost any of the blob pages in the sequence, the following error
      is returned:

      "Blob xxx is truncated"

      If the fetched blob is determined to be corrupt for any of the above
      reasons, and -mend is specified, then the blob record is marked as
      damaged.

   J. Index Walking

      In 4.5 (NevaStone) Index walking was moved to after the completion
      of data page walking.

      The indices for the relation are walked.  If the index root page
      is missing, the following message is returned:

      "Missing index root page"

      and the indices are not walked.  Otherwise the index root page
      is fetched and all indices on the page fetched.

      For each index, the btree pages are fetched from top-down, left to
      right.
      Basic validation is made on non-leaf pages to ensure that each node
      on page points to another index page.  If -full validation is specified
      then the lower level page is fetched to ensure it is starting index
      entry is consistent with the parent entry.
      On leaf pages, the records pointed to by the index pages are not
      fetched, the keys are looked at to ensure they are in correct
      ascending order.

      If a visited page is not part of the specified relation and index,
      the following error is returned:

      "Index xxx is corrupt at page xxx"

      If there are orphan child pages, i.e. a child page does not have its entry
      as yet in the parent page, however the child's left sibling page has it's
      btr_sibling updated, the following error is returned

      "Index xxx has orphan child page at page xxx"

      If the page does not contain the number of nodes we would have
      expected from its marked length, the following error is returned:

      "Index xxx is corrupt on page xxx"

      While we are walking leaf pages, we keep a bitmap of all record
      numbers seen in the index.  At the conclusion of the index walk
      we compare this bitmap to the bitmap of all records in the
      relation (calculated during data page/Record Validation phase).
      If the bitmaps are not equal then we have a corrupt index
      and the following error is reported:

      "Index %d is corrupt (missing entries)"

      We do NOT check that each version of each record has a valid
      index entry - nor do we check that the stored key for each item
      in the index corresponds to a version of the specified record.

   K. Relation Checking

      We count the number of backversions seen while walking pointer pages,
      and separately count the number of backversions seen while walking
      record chains.  If these numbers do not match it indicates either
      "orphan" backversion chains or double-linked chains.  If this is
      see the following error is returned:

     "Relation has %ld orphan backversions (%ld in use)"

      Currently we do not try to correct this condition, mearly report
      it.  For "orphan" backversions the space can be reclaimed by
      a backup/restore.  For double-linked chains a SWEEP should
      remove all the backversions.

VI. ADDITIONAL NOTES

   A.  Damaged Records

      If any corruption of a record fragment is seen during validation, the
      record header is marked as "damaged".  As far as I can see, this has no
      effect on the engine per se.  Records marked as damaged will still be
      retrieved by the engine itself.  There is some question in my mind as
      to whether this record should be retrieved at all during a gbak.

      If a damaged record is visited, the following error message will appear:

      "Record xxx is marked as damaged"

      Note that when a damaged record is first detected, this message is not
      actually printed.  The record is simply marked as damaged.  It is only
      thereafter when the record is visited that this message will appear.
      So I would postulate that unless a full validation is done at some point,
      you would not see this error message; once the full validation is done,
      the message will be returned even if you do not specify -full.

   B. Damaged Blobs

      Blob records marked as damaged cannot be opened and will not be deleted
      from disk.  This means that even during backup the blob structures marked
      as damaged will not be fetched and backed up.  (Why this is done
      differently for blobs than for records I cannot say.
      Perhaps it was viewed as too difficult to try to retrieve a damaged blob.)

*/

#include "firebird.h"
#include "memory_routines.h"
#include <stdio.h>
#include "../jrd/common.h"
#include <stdarg.h>
#include "../jrd/jrd.h"
#include "../jrd/ods.h"
#include "../jrd/pag.h"
#include "../jrd/ibase.h"
#include "../jrd/val.h"
#include "../jrd/btr.h"
#include "../jrd/btn.h"
#include "../jrd/lck.h"
#include "../jrd/cch.h"
#include "../jrd/rse.h"
#include "../jrd/sbm.h"
#include "../jrd/tra.h"
#include "../jrd/svc.h"
#include "../jrd/btr_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/dbg_proto.h"
#include "../jrd/dpm_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/jrd_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/tra_proto.h"
#include "../jrd/val_proto.h"
#include "../jrd/thread_proto.h"

#include "../common/utils_proto.h"
#include "../common/classes/ClumpletWriter.h"
#include "../common/classes/PublicHandle.h"
#include "../jrd/intl_proto.h"
#include "../jrd/lck_proto.h"
#include "../jrd/isc_f_proto.h"

#ifdef DEBUG_VAL_VERBOSE
#include "../jrd/dmp_proto.h"
/* Control variable for verbose output during debug of
   validation.
   0 == logged errors only
   1 == logical output also
   2 == physical page output also */
static USHORT VAL_debug_level = 0;
#endif

using namespace Jrd;
using namespace Ods;
using namespace Firebird;

// Validation/garbage collection/repair control block
enum FETCH_CODE {
	fetch_ok,
	//fetch_checksum,
	fetch_type,
	fetch_duplicate
};

enum RTN {
	rtn_ok,
	rtn_corrupt,
	rtn_eof
};


class Validation
{
public:
	Validation(thread_db*, UtilSvc* uSvc = NULL);
	~Validation();

	void run(thread_db*, USHORT);
private:

	RTN corrupt(thread_db*, USHORT, const jrd_rel*, ...);
	FETCH_CODE fetch_page(thread_db*, SLONG, USHORT, WIN *, void *, bool = true);
	void garbage_collect(thread_db*);
#ifdef DEBUG_VAL_VERBOSE
	void print_rhd(USHORT, const rhd*);
#endif

	void parse_args(thread_db*);
	void output(const char*, ...);

	RTN walk_blob(thread_db*, jrd_rel*, blh*, USHORT, RecordNumber);
	RTN walk_chain(thread_db*, jrd_rel*, rhd*, RecordNumber);
	void walk_database(thread_db*);
	RTN walk_data_page(thread_db*, jrd_rel*, SLONG, SLONG);
	void walk_generators(thread_db*);
	void walk_header(thread_db*, SLONG);
	RTN walk_index(thread_db*, jrd_rel*, index_root_page&, USHORT);
	void walk_log(thread_db*);
	void walk_pip(thread_db*);
	RTN walk_pointer_page(thread_db*, jrd_rel*, int);
	RTN walk_record(thread_db*, jrd_rel*, rhd*, USHORT, RecordNumber, bool);
	RTN walk_relation(thread_db*, jrd_rel*);
	RTN walk_root(thread_db*, jrd_rel*);
	RTN walk_tip(thread_db*, SLONG);

	PageBitmap* vdr_page_bitmap;
	SLONG vdr_max_page;
	USHORT vdr_flags;
	USHORT vdr_errors;
	SLONG vdr_max_transaction;
	ULONG vdr_rel_backversion_counter;	// Counts slots w/rhd_chain
	PageBitmap* vdr_backversion_pages;	// 1 bit per visited table page
	ULONG vdr_rel_chain_counter;		// Counts chains w/rdr_chain
	PageBitmap* vdr_chain_pages;	// 1 bit per visited record chain page
	RecordBitmap* vdr_rel_records;		// 1 bit per valid record
	RecordBitmap* vdr_idx_records;		// 1 bit per index item

	UtilSvc* vdr_service;
	PatternMatcher* vdr_tab_incl;
	PatternMatcher* vdr_tab_excl;
	PatternMatcher* vdr_idx_incl;
	PatternMatcher* vdr_idx_excl;
	int vdr_lock_tout;
	void checkDPinPP(thread_db *tdbb, jrd_rel *relation, SLONG page_number);
	void checkDPinPIP(thread_db *tdbb, jrd_rel *relation, SLONG page_number);
};

// vdr_flags

const USHORT VDR_online		= 0x01;		// online validation (no exclusive attachment)
const USHORT VDR_update		= 0x02;		// fix simple things
const USHORT VDR_repair		= 0x04;		// fix non-simple things (-mend)
const USHORT VDR_records	= 0x08;		// Walk all records
const USHORT VDR_partial	= 0x10;		// Walk only (some) relations


static PatternMatcher* createPatternMatcher(thread_db* tdbb, const char* pattern)
{
	PatternMatcher* matcher = NULL;
	try 
	{
		if (pattern)
		{
			const int len = strlen(pattern);

			Collation* obj = INTL_texttype_lookup(tdbb, CS_UTF8);
			matcher = obj->createSimilarToMatcher(*tdbb->getDefaultPool(), 
				(const UCHAR*) pattern, len, (UCHAR*)"\\", 1);
		}
	}
	catch(const Exception& ex)
	{
		ISC_STATUS_ARRAY temp;
		ex.stuff_exception(temp);

		Arg::StatusVector status(temp);
		status << Arg::Gds(isc_random) << Arg::Str(pattern);
		status.raise();
	}
	return matcher;
}


#pragma FB_COMPILER_MESSAGE("This table goes to gds__log and it's not localized")

static const TEXT msg_table[VAL_MAX_ERROR][80] =
{
	"Page %ld wrong type (expected %d encountered %d)",	// 0
	"Checksum error on page %ld",
	"Page %ld doubly allocated",
	"Page %ld is used but marked free",
	"Page %ld is an orphan",
	"Warning: blob %" QUADFORMAT"d appears inconsistent",	// 5
	"Blob %" QUADFORMAT"d is corrupt",
	"Blob %" QUADFORMAT"d is truncated",
	"Chain for record %" QUADFORMAT"d is broken",
	"Data page %ld (sequence %ld) is confused",
	"Data page %ld (sequence %ld), line %ld is bad",	// 10
	"Index %d is corrupt on page %ld level %d at offset %d. File: %s, line: %d\n\t",
	"Pointer page (sequence %ld) lost",
	"Pointer page (sequence %ld) inconsistent",
	"Record %" QUADFORMAT"d is marked as damaged",
	"Record %" QUADFORMAT"d has bad transaction %ld",	// 15
	"Fragmented record %" QUADFORMAT"d is corrupt",
	"Record %" QUADFORMAT"d is wrong length",
	"Missing index root page",
	"Transaction inventory pages lost",
	"Transaction inventory page lost, sequence %ld",	// 20
	"Transaction inventory pages confused, sequence %ld",
	"Relation has %ld orphan backversions (%ld in use)",
	"Index %d is corrupt (record %" QUADFORMAT"d have missing entries)",
	"Index %d has orphan child page at page %ld",
	"Index %d has a circular reference at page %ld",	// 25
	"Index %d has inconsistent left sibling pointer, page %ld level %d",
	"Index %d misses node on page %ld level %d",
	"Data page %ld marked as free in PIP (%ld:%ld)",
	"Data page %ld is not in PP (%ld). Slot (%d) is not found",
	"Data page %ld is not in PP (%ld). Slot (%d) has value %ld",
	"Pointer page is not found for data page %ld. dpg_sequence (%ld) is invalid"
};


bool VAL_validate(thread_db* tdbb, USHORT switches)
{
/**************************************
 *
 *	V A L _ v a l i d a t e
 *
 **************************************
 *
 * Functional description
 *	Validate a database.
 *
 **************************************/
	MemoryPool* val_pool = NULL;

	SET_TDBB(tdbb);
	Database* dbb = tdbb->getDatabase();
	Attachment* att = tdbb->getAttachment();

	try {

		val_pool = dbb->createPool();
		Jrd::ContextPoolHolder context(tdbb, val_pool);

		USHORT flags = 0;
		if (switches & isc_dpb_records)
			flags |= VDR_records;

		if (switches & isc_dpb_repair)
			flags |= VDR_repair;

		if (!(switches & isc_dpb_no_update))
			flags |= VDR_update;

		// initialize validate errors

		tdbb->tdbb_flags |= TDBB_sweeper;
		
		Validation control(tdbb);
		control.run(tdbb, flags);

		CCH_flush(tdbb, FLUSH_FINI, 0);

		tdbb->tdbb_flags &= ~TDBB_sweeper;
	}	// try
	catch (const Firebird::Exception& ex)
	{
		Firebird::stuff_exception(tdbb->tdbb_status_vector, ex);
		dbb->deletePool(val_pool);
		tdbb->tdbb_flags &= ~TDBB_sweeper;
		return false;
	}

	dbb->deletePool(val_pool);
	return true;
}

void Validation::run(thread_db* tdbb, USHORT flags)
{
	Attachment* att = tdbb->getAttachment();

	if (!att->att_val_errors) {
		att->att_val_errors = vcl::newVector(*att->att_pool, VAL_MAX_ERROR);
	}
	else
	{
		for (USHORT i = 0; i < VAL_MAX_ERROR; i++)
			(*att->att_val_errors)[i] = 0;
	}

	vdr_flags = flags;

	walk_database(tdbb);

	if (vdr_errors)
		vdr_flags &= ~VDR_update;

	if (!(vdr_flags & VDR_online) && !(vdr_flags & VDR_partial))
		garbage_collect(tdbb);
}


Validation::Validation(thread_db* tdbb, UtilSvc* uSvc)
{
	vdr_page_bitmap = NULL;
	vdr_max_page = 0;
	vdr_flags = 0;
	vdr_errors = 0;
	vdr_max_transaction = 0;
	vdr_rel_backversion_counter = 0;
	vdr_backversion_pages = NULL;
	vdr_rel_chain_counter = 0;
	vdr_chain_pages = NULL;
	vdr_rel_records = NULL;		// 1 bit per valid record
	vdr_idx_records = NULL;

	vdr_service = uSvc;
	vdr_tab_incl = vdr_tab_excl = NULL;
	vdr_idx_incl = vdr_idx_excl = NULL;
	vdr_lock_tout = -10;

	if (uSvc) {
		parse_args(tdbb);
	}
	output("Validation started\n\n");
}

Validation::~Validation()
{
	delete vdr_tab_incl;
	delete vdr_tab_excl;
	delete vdr_idx_incl;
	delete vdr_idx_excl;

	output("Validation finished\n");
}


void Validation::parse_args(thread_db* tdbb)
{
	in_sw_tab_t local_sw_table[sizeof(val_option_in_sw_table) / sizeof(val_option_in_sw_table[0])];
	memcpy(local_sw_table, val_option_in_sw_table, sizeof(val_option_in_sw_table));

	const in_sw_tab_t* action_sw = NULL;
	const char** argv = vdr_service->argv.begin();
	const char* const* end = vdr_service->argv.end();
	for (++argv; argv < end; argv++)
	{
		if (!*argv)
			continue;

		in_sw_tab_t* sw = fb_utils::findSwitch(&local_sw_table[0], *argv);
		if (!sw)
			continue;

		if (sw->in_sw_state)
		{
			string s;
			s.printf("Switch %s specified more then once", sw->in_sw_name);

			(Arg::Gds(isc_random) << Arg::Str(s)).raise();
		}

		sw->in_sw_state = true;

		switch (sw->in_sw)
		{
		case IN_SW_VAL_TAB_INCL:
		case IN_SW_VAL_TAB_EXCL:
		case IN_SW_VAL_IDX_INCL:
		case IN_SW_VAL_IDX_EXCL:
		case IN_SW_VAL_LOCK_TIMEOUT:
			*argv++ = NULL;
			if (argv >= end || !(*argv))
			{
				string s;
				s.printf("Switch %s requires value", sw->in_sw_name);

				(Arg::Gds(isc_random) << Arg::Str(s)).raise();
			}
			break;

		default:
			break;
		}

		switch (sw->in_sw)
		{
		case IN_SW_VAL_TAB_INCL:
			vdr_tab_incl = createPatternMatcher(tdbb, *argv);
			break;

		case IN_SW_VAL_TAB_EXCL:
			vdr_tab_excl = createPatternMatcher(tdbb, *argv);
			break;

		case IN_SW_VAL_IDX_INCL:
			vdr_idx_incl = createPatternMatcher(tdbb, *argv);
			break;

		case IN_SW_VAL_IDX_EXCL:
			vdr_idx_excl = createPatternMatcher(tdbb, *argv);
			break;

		case IN_SW_VAL_LOCK_TIMEOUT:
			{
				char* end = (char*) *argv;
				vdr_lock_tout = -strtol(*argv, &end, 10);

				if (end && *end)
				{
					string s;
					s.printf("Value (%s) is not a valid number", *argv);

					(Arg::Gds(isc_random) << Arg::Str(s)).raise();
				}
			}
			break;

		default:
			break;
		}
	}
}


void Validation::output(const char* format, ...)
{
	if (!vdr_service)
		return;

	va_list params;
	va_start(params, format);

	string s;
	tm now;
	int ms;
	TimeStamp::getCurrentTimeStamp().decode(&now, &ms);

//	s.printf("%04d-%02d-%02d %02d:%02d:%02d.%04d ", 
	s.printf("%02d:%02d:%02d.%02d ", 
//		now.tm_year + 1900, now.tm_mon + 1, now.tm_mday,
		now.tm_hour, now.tm_min, now.tm_sec, ms / 100);
	vdr_service->outputVerbose(s.c_str());

	s.vprintf(format, params);
	va_end(params);

	vdr_service->outputVerbose(s.c_str());
}

static int validate(Service* svc)
{
	PathName dbName;
	string userName;

	const in_sw_tab_t* action_sw = NULL;
	const char** argv = svc->argv.begin();
	const char* const* end = svc->argv.end();
	for (++argv; argv < end; argv++)
	{
		if (!*argv)
			continue;

		in_sw_tab_t* sw = fb_utils::findSwitch(&val_option_in_sw_table[0], *argv);
		if (!sw)
			continue;

		switch (sw->in_sw)
		{
		case IN_SW_VAL_DATABASE:
			*argv = NULL;
			argv++;
			if (argv < end && *argv)
				dbName = *argv;
			else
				;// error
			break;

		case IN_SW_VAL_TRUSTED_USER:
			*argv = NULL;
			argv++;
			if (argv < end && *argv)
				userName = *argv;
			else;
				// error
			break;

		default:
			break;
		}
	}

	ClumpletWriter dpb(ClumpletReader::Tagged, MAX_DPB_SIZE, isc_dpb_version1);
	if (!userName.isEmpty())
	{
		dpb.insertString(isc_dpb_trusted_auth, userName);
	}

	PathName expandedFilename = dbName;
	ISC_expand_filename(expandedFilename, NULL);

	if (dbName != expandedFilename)
		dpb.insertPath(isc_dpb_org_filename, dbName);

	ISC_STATUS_ARRAY status = {0};
	Attachment *att = NULL;

	if (jrd8_attach_database(status, expandedFilename.c_str(), &att, dpb.getBufferLength(), dpb.getBuffer()))
	{
		svc->setServiceStatus(status);
		return FB_FAILURE;
	}
	Database* dbb = att->att_database;

	svc->started();

	MemoryPool* val_pool = NULL;
	int ret_code = FB_SUCCESS;
	try
	{
		ThreadContextHolder tdbb(status);

		// should be AttachmentHolder but it is declared in jrd.cpp
		PublicHandleHolder attHolder(att, "validate");

		tdbb->setDatabase(dbb);
		tdbb->setAttachment(att);
		tdbb->tdbb_flags |= TDBB_sweeper;

		// should be DatabaseContextHolder but it is declared in jrd.cpp
		Database::SyncGuard dsGuard(dbb);

		val_pool = dbb->createPool();
		Jrd::ContextPoolHolder context(tdbb, val_pool);

		Validation control(tdbb, svc);
		control.run(tdbb, VDR_records | VDR_online | VDR_partial);
	}
	catch(const Exception& ex)
	{
		ex.stuff_exception(status);
		svc->setServiceStatus(status);
		ret_code = FB_FAILURE;
	}

	dbb->deletePool(val_pool);
	jrd8_detach_database(status, &att);
	return ret_code;
}


THREAD_ENTRY_DECLARE VAL_service(THREAD_ENTRY_PARAM arg)
{
	Service* svc = (Service*) arg;
	svc->initStatus();

	int exit_code = FB_SUCCESS;

	try
	{
		exit_code = validate(svc);
	}
	catch (const Exception& e)
	{
		ISC_STATUS_ARRAY status;
		e.stuff_exception(status);
		svc->setServiceStatus(status);
		exit_code = FB_FAILURE;
	}

	svc->started();
	svc->finish();

	return (THREAD_ENTRY_RETURN)(IPTR) exit_code;
}

RTN Validation::corrupt(thread_db* tdbb, USHORT err_code, const jrd_rel* relation, ...)
{
/**************************************
 *
 *	c o r r u p t
 *
 **************************************
 *
 * Functional description
 *	Corruption has been detected.
 *
 **************************************/

	SET_TDBB(tdbb);
	Attachment* att = tdbb->getAttachment();
	if (err_code < att->att_val_errors->count())
		(*att->att_val_errors)[err_code]++;

	const TEXT* err_string = err_code < VAL_MAX_ERROR ? msg_table[err_code]: "Unknown error code";

	string s;
	va_list ptr;
	const char* fn = tdbb->getAttachment()->att_filename.c_str();

	va_start(ptr, relation);
	s.vprintf(err_string, ptr);
	va_end(ptr);

#ifdef DEBUG_VAL_VERBOSE
	if (VAL_debug_level >= 0)
	{
		if (relation)
		{
			fprintf(stdout, "LOG:\tDatabase: %s\n\t%s in table %s (%d)\n",
				fn, s.c_str(), relation->rel_name.c_str(), relation->rel_id);
		}
		else
			fprintf(stdout, "LOG:\tDatabase: %s\n\t%s\n", fn, s.c_str());
	}
#endif

	if (relation)
	{
		gds__log("Database: %s\n\t%s in table %s (%d)",
			fn, s.c_str(), relation->rel_name.c_str(), relation->rel_id);
	}
	else
		gds__log("Database: %s\n\t%s", fn, s.c_str());

	++vdr_errors;

	s.append("\n");
	output(s.c_str());

	return rtn_corrupt;
}

FETCH_CODE Validation::fetch_page(thread_db* tdbb, SLONG page_number, USHORT type, 
								  WIN* window, void *page_pointer, bool mark)
{
/**************************************
 *
 *	f e t c h _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Fetch page and return type of illness, if any.  If a "mark"
 *	is true, check for doubly allocated pages and account for page
 *	use.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->getDatabase();
	CHECK_DBB(dbb);

	if (--tdbb->tdbb_quantum < 0)
	{
		 JRD_reschedule(tdbb, 0, true);

		 if (vdr_service && vdr_service->finished())
		 {
			 CCH_unwind(tdbb, false);
			 Arg::Gds(isc_att_shutdown).raise();
		 }
	}

	window->win_page = page_number;
	window->win_flags = 0;
	*(PAG*) page_pointer = CCH_FETCH_NO_SHADOW(tdbb, window, 
		(vdr_flags & VDR_online ? LCK_read : LCK_write), 
		0);


	if ((*(PAG*) page_pointer)->pag_type != type)
	{
		corrupt(tdbb, VAL_PAG_WRONG_TYPE, 0, page_number, type,
				(*(PAG*) page_pointer)->pag_type);
		return fetch_type;
	}

	if (!mark)
		return fetch_ok;

	// If "damaged" flag was set, checksum may be incorrect.  Check.

	if ((dbb->dbb_flags & DBB_damaged) && !CCH_validate(window))
	{
		corrupt(tdbb, VAL_PAG_CHECKSUM_ERR, 0, page_number);
		if (vdr_flags & VDR_repair)
			CCH_MARK(tdbb, window);
	}

	vdr_max_page = MAX(vdr_max_page, page_number);

	// For walking back versions & record fragments on data pages we
	// sometimes will fetch the same page more than once.  In that
	// event we don't report double allocation.  If the page is truely
	// double allocated (to more than one relation) we'll find it
	// when the on-page relation id doesn't match

	if ((type != pag_data) && PageBitmap::test(vdr_page_bitmap, page_number))
	{
		corrupt(tdbb, VAL_PAG_DOUBLE_ALLOC, 0, page_number);
		return fetch_duplicate;
	}


	PBM_SET(tdbb->getDefaultPool(), &vdr_page_bitmap, page_number);

	return fetch_ok;
}

void Validation::garbage_collect(thread_db* tdbb)
{
/**************************************
 *
 *	g a r b a g e _ c o l l e c t
 *
 **************************************
 *
 * Functional description
 *	The database has been walked; compare the page inventory against
 *	the bitmap of pages visited.
 *
 **************************************/
	SET_TDBB(tdbb);

	Database* dbb = tdbb->getDatabase();

	PageManager& pageSpaceMgr = dbb->dbb_page_manager;
	PageSpace* pageSpace = pageSpaceMgr.findPageSpace(DB_PAGE_SPACE);
	fb_assert(pageSpace);

	WIN window(DB_PAGE_SPACE, -1);

	for (SLONG sequence = 0, number = 0; number < vdr_max_page; sequence++)
	{
		const SLONG page_number = sequence ? sequence * pageSpaceMgr.pagesPerPIP - 1 : pageSpace->ppFirst;
		page_inv_page* page = 0;
		fetch_page(tdbb, page_number, pag_pages, &window, &page, false);
		UCHAR* p = page->pip_bits;
		const UCHAR* const end = p + pageSpaceMgr.bytesBitPIP;
		while (p < end && number < vdr_max_page)
		{
			UCHAR byte = *p++;
			for (int i = 8; i; --i, byte >>= 1, number++)
			{
				if (PageBitmap::test(vdr_page_bitmap, number))
				{
					if (byte & 1)
					{
						corrupt(tdbb, VAL_PAG_IN_USE, 0, number);
						if (vdr_flags & VDR_update)
						{
							CCH_MARK(tdbb, &window);
							p[-1] &= ~(1 << (number & 7));
						}
						DEBUG;
					}
				}
				else if (!(byte & 1) && (vdr_flags & VDR_records))
				{
					// Page is potentially an orphan - but don't declare it as such
					// unless we think we walked all pages

					corrupt(tdbb, VAL_PAG_ORPHAN, 0, number);
					if (vdr_flags & VDR_update)
					{
						CCH_MARK(tdbb, &window);
						p[-1] |= 1 << (number & 7);
					}
					DEBUG;
				}
			}
		}
		const UCHAR test_byte = p[-1];
		CCH_RELEASE(tdbb, &window);
		if (test_byte & 0x80)
			break;
	}

#ifdef DEBUG_VAL_VERBOSE
	// Dump verbose output of all the pages fetched
	if (VAL_debug_level >= 2)
	{
		// We are assuming RSE_get_forward
		if (vdr_page_bitmap->getFirst())
		{
			do {
				SLONG dmp_page_number = vdr_page_bitmap->current();
				DMP_page(dmp_page_number, dbb->dbb_page_size);
			} while (vdr_page_bitmap->getNext());
		}
	}
#endif
}

#ifdef DEBUG_VAL_VERBOSE
void Validation::print_rhd(USHORT length, const rhd* header)
{
/**************************************
 *
 *	p r i n t _ r h d
 *
 **************************************
 *
 * Functional description
 *	Debugging routine to print a
 *	Record Header Data.
 *
 **************************************/
	if (VAL_debug_level)
	{
		fprintf(stdout, "rhd: len %d TX %d format %d ",
				   length, header->rhd_transaction, (int) header->rhd_format);
		fprintf(stdout, "BP %d/%d flags 0x%x ",
				   header->rhd_b_page, header->rhd_b_line, header->rhd_flags);
		if (header->rhd_flags & rhd_incomplete)
		{
			const rhdf* fragment = (rhdf*) header;
			fprintf(stdout, "FP %d/%d ", fragment->rhdf_f_page, fragment->rhdf_f_line);
		}
		fprintf(stdout, "%s ", (header->rhd_flags & rhd_deleted) ? "DEL" : "   ");
		fprintf(stdout, "%s ", (header->rhd_flags & rhd_chain) ? "CHN" : "   ");
		fprintf(stdout, "%s ", (header->rhd_flags & rhd_fragment) ? "FRG" : "   ");
		fprintf(stdout, "%s ", (header->rhd_flags & rhd_incomplete) ? "INC" : "   ");
		fprintf(stdout, "%s ", (header->rhd_flags & rhd_blob) ? "BLB" : "   ");
		fprintf(stdout, "%s ", (header->rhd_flags & rhd_delta) ? "DLT" : "   ");
		fprintf(stdout, "%s ", (header->rhd_flags & rhd_large) ? "LRG" : "   ");
		fprintf(stdout, "%s ", (header->rhd_flags & rhd_damaged) ? "DAM" : "   ");
		fprintf(stdout, "\n");
	}
}
#endif

RTN Validation::walk_blob(thread_db* tdbb, jrd_rel* relation, blh* header, USHORT length, RecordNumber number)
{
/**************************************
 *
 *	w a l k _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Walk a blob.
 *
 **************************************/
	SET_TDBB(tdbb);

#ifdef DEBUG_VAL_VERBOSE
	if (VAL_debug_level)
	{
		fprintf(stdout,
				   "walk_blob: level %d lead page %d max pages %d max segment %d\n",
				   header->blh_level, header->blh_lead_page,
				   header->blh_max_sequence, header->blh_max_segment);
		fprintf(stdout, "           count %d, length %d sub_type %d\n",
				   header->blh_count, header->blh_length,
				   header->blh_sub_type);
	}
#endif

	// Level 0 blobs have no work to do.
	if (header->blh_level == 0)
		return rtn_ok;

	// Level 1 blobs are a little more complicated
	WIN window1(DB_PAGE_SPACE, -1), window2(DB_PAGE_SPACE, -1);
	window1.win_flags = window2.win_flags = WIN_garbage_collector;

	const SLONG* pages1 = header->blh_page;
	const SLONG* const end1 = pages1 + ((USHORT) (length - BLH_SIZE) >> SHIFTLONG);
	SLONG sequence;

	for (sequence = 0; pages1 < end1; pages1++)
	{
		blob_page* page1 = 0;
		fetch_page(tdbb, *pages1, pag_blob, &window1, &page1);
		if (page1->blp_lead_page != header->blh_lead_page)
			corrupt(tdbb, VAL_BLOB_INCONSISTENT, relation, number.getValue());
		if ((header->blh_level == 1 && page1->blp_sequence != sequence))
		{
			corrupt(tdbb, VAL_BLOB_CORRUPT, relation, number.getValue());
			CCH_RELEASE_TAIL(tdbb, &window1);
			return rtn_corrupt;
		}
		if (header->blh_level == 1)
			sequence++;
		else
		{
			const SLONG* pages2 = page1->blp_page;
			const SLONG* const end2 = pages2 + (page1->blp_length >> SHIFTLONG);
			for (; pages2 < end2; pages2++, sequence++)
			{
				blob_page* page2 = 0;
				fetch_page(tdbb, *pages2, pag_blob, &window2, &page2);
				if (page2->blp_lead_page != header->blh_lead_page || page2->blp_sequence != sequence)
				{
					corrupt(tdbb, VAL_BLOB_CORRUPT, relation, number.getValue());
					CCH_RELEASE_TAIL(tdbb, &window1);
					CCH_RELEASE_TAIL(tdbb, &window2);
					return rtn_corrupt;
				}
				CCH_RELEASE_TAIL(tdbb, &window2);
			}
		}
		CCH_RELEASE_TAIL(tdbb, &window1);
	}

	if (sequence - 1 != header->blh_max_sequence)
		return corrupt(tdbb, VAL_BLOB_TRUNCATED, relation, number.getValue());

	return rtn_ok;
}

RTN Validation::walk_chain(thread_db* tdbb, jrd_rel* relation, rhd* header, RecordNumber head_number)
{
/**************************************
 *
 *	w a l k _ c h a i n
 *
 **************************************
 *
 * Functional description
 *	Make sure chain of record versions is completely intact.
 *
 **************************************/
#ifdef DEBUG_VAL_VERBOSE
	USHORT counter = 0;
#endif

	SET_TDBB(tdbb);

	SLONG page_number = header->rhd_b_page;
	USHORT line_number = header->rhd_b_line;
	WIN window(DB_PAGE_SPACE, -1);
	window.win_flags = WIN_garbage_collector;

	while (page_number)
	{
		const bool delta_flag = (header->rhd_flags & rhd_delta) ? true : false;
#ifdef DEBUG_VAL_VERBOSE
		if (VAL_debug_level)
			fprintf(stdout, "  BV %02d: ", ++counter);
#endif
		data_page* page = 0;
		fetch_page(tdbb, page_number, pag_data, &window, &page);

		if (page->dpg_relation != relation->rel_id)
		 {
			  ++vdr_errors;
			  CCH_RELEASE_TAIL(tdbb, &window);
			  return corrupt(tdbb, VAL_DATA_PAGE_CONFUSED, relation, page_number, page->dpg_sequence);
		 }

		vdr_rel_chain_counter++;
		PBM_SET(tdbb->getDefaultPool(), &vdr_chain_pages, page_number);

		const data_page::dpg_repeat* line = &page->dpg_rpt[line_number];
		header = (rhd*) ((UCHAR *) page + line->dpg_offset);
		if (page->dpg_count <= line_number || !line->dpg_length ||
			(header->rhd_flags & (rhd_blob | rhd_fragment)) ||
			!(header->rhd_flags & rhd_chain) ||
			walk_record(tdbb, relation, header, line->dpg_length,
						head_number, delta_flag) != rtn_ok)
		{
			CCH_RELEASE_TAIL(tdbb, &window);
			return corrupt(tdbb, VAL_REC_CHAIN_BROKEN, relation, head_number.getValue());
		}
		page_number = header->rhd_b_page;
		line_number = header->rhd_b_line;
		CCH_RELEASE_TAIL(tdbb, &window);
	}

	return rtn_ok;
}

void Validation::walk_database(thread_db* tdbb)
{
/**************************************
 *
 *	w a l k _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->getDatabase();

#ifdef DEBUG_VAL_VERBOSE
	if (VAL_debug_level)
	{
		fprintf(stdout,
				   "walk_database: %s\nODS: %d.%d  (creation ods %d)\nPage size %d\n",
				   dbb->dbb_filename.c_str(), dbb->dbb_ods_version,
				   dbb->dbb_minor_version, dbb->dbb_minor_original,
				   dbb->dbb_page_size);
	}
#endif

	DPM_scan_pages(tdbb);
	WIN window(DB_PAGE_SPACE, -1);
	header_page* page = 0;
	fetch_page(tdbb, (SLONG) HEADER_PAGE, pag_header, &window, &page);
	vdr_max_transaction = page->hdr_next_transaction;

	if (vdr_flags & VDR_online) {
		CCH_RELEASE(tdbb, &window);
	}

	if (!(vdr_flags & VDR_partial))
	{
		walk_header(tdbb, page->hdr_next_page);
		walk_log(tdbb);
		walk_pip(tdbb);
		walk_tip(tdbb, page->hdr_next_transaction);
		walk_generators(tdbb);
	}

	vec<jrd_rel*>* vector;
	for (USHORT i = 0; (vector = dbb->dbb_relations) && i < vector->count(); i++)
	{
#ifdef DEBUG_VAL_VERBOSE
		if (i > dbb->dbb_max_sys_rel) // Why not system flag instead?
			VAL_debug_level = 2;
#endif
		jrd_rel* relation = (*vector)[i];
		if (relation)
		{

			// Seems we need rel_flags only and can replace relatively heavy and potentially 
			// dangerous (if DB is hard corrupted) MET_scan_relation by more lightweight
			// MET_lookup_relation_id

			relation = MET_lookup_relation_id(tdbb, relation->rel_id, false);
			if (!relation)  {
				continue;
			}

			// Can't validate system relations online as they could be modified
			// by system transaction which not acquires relation locks
			if ((vdr_flags & VDR_online) && relation->isSystem())
				continue;

			if (vdr_tab_incl)
			{
				vdr_tab_incl->reset();
				if (!vdr_tab_incl->process((UCHAR*)relation->rel_name.c_str(), relation->rel_name.length()) ||
					!vdr_tab_incl->result())
					continue;
			}

			if (vdr_tab_excl)
			{
				vdr_tab_excl->reset();
				if (!vdr_tab_excl->process((UCHAR*)relation->rel_name.c_str(), relation->rel_name.length()) ||
					vdr_tab_excl->result())
					continue;
			}

			// We can't realiable track double allocated page's when validating online.
			// All we can check is that page is not double allocated at the same relation.
			if (vdr_flags & VDR_online)
				vdr_page_bitmap->clear();

			string relName;
			relName.printf("Relation %d (%s)", relation->rel_id, relation->rel_name.c_str());
			output("%s\n", relName.c_str());

			int errs = vdr_errors;
			walk_relation(tdbb, relation);
			errs = vdr_errors - errs;

			if (!errs)
				output("%s is ok\n\n", relName.c_str());
			else
				output("%s : %d ERRORS found\n\n", relName.c_str(), errs);
		}
	}

	if (!(vdr_flags & VDR_online)) {
		CCH_RELEASE(tdbb, &window);
	}
}

RTN Validation::walk_data_page(thread_db* tdbb, jrd_rel* relation, SLONG page_number, SLONG sequence)
{
/**************************************
 *
 *	w a l k _ d a t a _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Walk a single data page.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->getDatabase();

	WIN window(DB_PAGE_SPACE, -1);
	window.win_flags = WIN_garbage_collector;

	data_page* page = 0;
	fetch_page(tdbb, page_number, pag_data, &window, &page);

#ifdef DEBUG_VAL_VERBOSE
	if (VAL_debug_level)
	{
		fprintf(stdout,
				   "walk_data_page: page %d rel %d seq %d count %d\n",
				   page_number, page->dpg_relation, page->dpg_sequence,
				   page->dpg_count);
	}
#endif

	if (page->dpg_relation != relation->rel_id || page->dpg_sequence != sequence)
	{
		++vdr_errors;
		CCH_RELEASE_TAIL(tdbb, &window);
		return corrupt(tdbb, VAL_DATA_PAGE_CONFUSED, relation, page_number, sequence);
	}

	// Walk records

	const UCHAR* const end_page = (UCHAR *) page + dbb->dbb_page_size;
	const data_page::dpg_repeat* const end = page->dpg_rpt + page->dpg_count;
	RecordNumber number((SINT64)sequence * dbb->dbb_max_records);

	for (const data_page::dpg_repeat* line = page->dpg_rpt; line < end; line++, number.increment())
	{
#ifdef DEBUG_VAL_VERBOSE
		if (VAL_debug_level)
		{
			fprintf(stdout, "Slot %02d (%d,%d): ",
					   line - page->dpg_rpt, line->dpg_offset, line->dpg_length);
		}
#endif
		if (line->dpg_length)
		{
			rhd* header = (rhd*) ((UCHAR *) page + line->dpg_offset);
			if ((UCHAR *) header < (UCHAR *) end || (UCHAR *) header + line->dpg_length > end_page)
			{
				CCH_RELEASE_TAIL(tdbb, &window);
				return corrupt(tdbb, VAL_DATA_PAGE_LINE_ERR, relation, page_number,
								sequence, (SLONG) (line - page->dpg_rpt));
			}
			if (header->rhd_flags & rhd_chain)
			{
				vdr_rel_backversion_counter++;
				PBM_SET(tdbb->getDefaultPool(), &vdr_backversion_pages, page_number);
			}

			// Record the existance of a primary version of a record

			if ((vdr_flags & VDR_records) &&
				!(header->rhd_flags & (rhd_chain | rhd_fragment | rhd_blob)))
			{
				// Only set committed (or limbo) records in the bitmap. If there
				// is a backversion then at least one of the record versions is
				// committed. If there's no backversion then check transaction
				// state of the lone primary record version.

				if (header->rhd_b_page)
					RBM_SET(tdbb->getDefaultPool(), &vdr_rel_records, number.getValue());
				else
				{
					int state;
					if (header->rhd_transaction < dbb->dbb_oldest_transaction)
						state = tra_committed;
					else
						state = TRA_fetch_state(tdbb, header->rhd_transaction);
					if (state == tra_committed || state == tra_limbo)
						RBM_SET(tdbb->getDefaultPool(), &vdr_rel_records, number.getValue());
				}
			}

#ifdef DEBUG_VAL_VERBOSE
			if (VAL_debug_level)
			{
				if (header->rhd_flags & rhd_chain)
					fprintf(stdout, "(backvers)");
				if (header->rhd_flags & rhd_fragment)
					fprintf(stdout, "(fragment)");
				if (header->rhd_flags & (rhd_fragment | rhd_chain))
					print_rhd(line->dpg_length, header);
			}
#endif
			if (!(header->rhd_flags & rhd_chain) &&
				((header->rhd_flags & rhd_large) || (vdr_flags & VDR_records)))
			{
				const RTN result = (header->rhd_flags & rhd_blob) ?
					walk_blob(tdbb, relation, (blh*) header, line->dpg_length, number) :
					walk_record(tdbb, relation, header, line->dpg_length, number, false);
				if ((result == rtn_corrupt) && (vdr_flags & VDR_repair))
				{
					CCH_MARK(tdbb, &window);
					header->rhd_flags |= rhd_damaged;
				}
			}
		}
#ifdef DEBUG_VAL_VERBOSE
		else if (VAL_debug_level)
			fprintf(stdout, "(empty)\n");
#endif
	}

	CCH_RELEASE_TAIL(tdbb, &window);

#ifdef DEBUG_VAL_VERBOSE
	if (VAL_debug_level)
		fprintf(stdout, "------------------------------------\n");
#endif

	return rtn_ok;
}

void Validation::walk_generators(thread_db* tdbb)
{
/**************************************
 *
 *	w a l k _ g e n e r a t o r s
 *
 **************************************
 *
 * Functional description
 *	Walk the page inventory pages.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->getDatabase();
	CHECK_DBB(dbb);
	WIN window(DB_PAGE_SPACE, -1);

	vcl* vector = dbb->dbb_gen_id_pages;
	if (vector)
	{
        vcl::iterator ptr, end;
		for (ptr = vector->begin(), end = vector->end(); ptr < end; ++ptr)
		{
			if (*ptr)
			{
#ifdef DEBUG_VAL_VERBOSE
				if (VAL_debug_level)
					fprintf(stdout, "walk_generator: page %d\n", *ptr);
#endif
				pointer_page* page = 0;
				fetch_page(tdbb, *ptr, pag_ids, &window, &page);
				CCH_RELEASE(tdbb, &window);
			}
		}
	}
}

void Validation::walk_header(thread_db* tdbb, SLONG page_num)
{
/**************************************
 *
 *	w a l k _ h e a d e r
 *
 **************************************
 *
 * Functional description
 *	Walk the overflow header pages
 *
 **************************************/
	SET_TDBB(tdbb);

	while (page_num)
	{
#ifdef DEBUG_VAL_VERBOSE
		if (VAL_debug_level)
			fprintf(stdout, "walk_header: page %d\n", page_num);
#endif
		WIN window(DB_PAGE_SPACE, -1);
		header_page* page = 0;
		fetch_page(tdbb, page_num, pag_header, &window, &page);
		page_num = page->hdr_next_page;
		CCH_RELEASE(tdbb, &window);
	}
}

RTN Validation::walk_index(thread_db* tdbb, jrd_rel* relation, index_root_page& root_page, USHORT id)
{
/**************************************
 *
 *	w a l k _ i n d e x
 *
 **************************************
 *
 * Functional description
 *	Walk all btree pages left-to-right and top-down.
 *	Check all the pointers and keys for consistency
 *	relative to each other, and check sibling pointers.
 *
 *	NOTE: id is the internal index id, relative for each
 *	relation.  It is 1 less than the user level index id.
 *	So errors are reported against index id+1
 *
 **************************************/

	SET_TDBB(tdbb);
	Database* dbb = tdbb->getDatabase();
	CHECK_DBB(dbb);

	const SLONG page_number = root_page.irt_rpt[id].irt_root;
	if (!page_number) {
		return rtn_ok;
	}

	const bool unique = (root_page.irt_rpt[id].irt_flags & (irt_unique | idx_primary));
	const bool descending = (root_page.irt_rpt[id].irt_flags & irt_descending);

	temporary_key nullKey, *null_key = 0;
	if (unique && tdbb->getDatabase()->dbb_ods_version >= ODS_VERSION11)
	{
		const bool isExpression = root_page.irt_rpt[id].irt_flags & irt_expression;
		if (isExpression)
			root_page.irt_rpt[id].irt_flags &= ~irt_expression;

		index_desc idx;
		BTR_description(tdbb, relation, &root_page, &idx, id);
		if (isExpression)
			root_page.irt_rpt[id].irt_flags |= irt_expression;

		null_key = &nullKey;
		BTR_make_null_key(tdbb, &idx, null_key);
	}

	SLONG next = page_number;
	SLONG down = page_number;
	temporary_key key;
	key.key_length = 0;
	SLONG previous_number = 0;

	RecordBitmap::reset(vdr_idx_records);

	bool firstNode = true;
	bool nullKeyNode = false;			// current node is a null key of unique index
	bool nullKeyHandled = !(unique && null_key);	// null key of unique index was handled

	UCHAR flags = 0;
	UCHAR* pointer;
	IndexNode node, lastNode;
	PageBitmap visited_pages; // used to check circular page references, Diane Downie 2007-02-09

	while (next)
	{
		WIN window(DB_PAGE_SPACE, -1);
		window.win_flags = WIN_garbage_collector;

		btree_page* page = 0;
		fetch_page(tdbb, next, pag_index, &window, &page);

		// remember each page for circular reference detection
		visited_pages.set(next);

		if ((next != page_number) &&
			(page->btr_header.pag_flags & BTR_FLAG_COPY_MASK) != (flags & BTR_FLAG_COPY_MASK))
		{
			corrupt(tdbb, VAL_INDEX_PAGE_CORRUPT, relation,
					id + 1, next, page->btr_level, 0, __FILE__, __LINE__);
		}
		flags = page->btr_header.pag_flags;
		const bool leafPage = (page->btr_level == 0);
		const bool useJumpInfo = (flags & btr_jump_info);
		const bool useAllRecordNumbers = (flags & btr_all_record_number);

		if (!useAllRecordNumbers)
			nullKeyHandled = true;

		if (page->btr_relation != relation->rel_id || page->btr_id != (UCHAR) (id % 256))
		{
			corrupt(tdbb, VAL_INDEX_PAGE_CORRUPT, relation, id + 1,
					next, page->btr_level, 0, __FILE__, __LINE__);
			CCH_RELEASE_TAIL(tdbb, &window);
			return rtn_corrupt;
		}

		if (useJumpInfo)
		{
			IndexJumpInfo jumpInfo;
			pointer = BTreeNode::getPointerFirstNode(page, &jumpInfo);
			const USHORT headerSize = (pointer - (UCHAR*)page);
			// Check if firstNodeOffset is not out of page area.
			if ((jumpInfo.firstNodeOffset < headerSize) ||
				(jumpInfo.firstNodeOffset > page->btr_length))
			{
				corrupt(tdbb, VAL_INDEX_PAGE_CORRUPT, relation,
						id + 1, next, page->btr_level, pointer - (UCHAR*)page, __FILE__, __LINE__);
			}

			USHORT n = jumpInfo.jumpers;
			USHORT firstNodeOffset = (UCHAR*)page->btr_nodes - (UCHAR*)page; 
			USHORT jumpDataLen = 0;
			IndexNode checknode;
			IndexJumpNode jumpNode;
			while (n)
			{
				pointer = BTreeNode::readJumpNode(&jumpNode, pointer, flags);
				firstNodeOffset += BTreeNode::getJumpNodeSize(&jumpNode, flags);
				// Check if jump node offset is inside page.
				if ((jumpNode.offset < jumpInfo.firstNodeOffset) ||
					(jumpNode.offset > page->btr_length))
				{
					corrupt(tdbb, VAL_INDEX_PAGE_CORRUPT, relation,
							id + 1, next, page->btr_level, pointer - (UCHAR*)page, __FILE__, __LINE__);
				}
				else
				{
					// Check if jump node has same length as data node prefix.
					BTreeNode::readNode(&checknode, (UCHAR*)page + jumpNode.offset, flags, leafPage);
					if ((jumpNode.prefix + jumpNode.length) != checknode.prefix) {
						corrupt(tdbb, VAL_INDEX_PAGE_CORRUPT, relation,
								id + 1, next, page->btr_level, jumpNode.offset, __FILE__, __LINE__);

					}

					// First jump node should have zero prefix
					if (n == jumpInfo.jumpers && jumpNode.prefix) {
						corrupt(tdbb, VAL_INDEX_PAGE_CORRUPT, relation,
								id + 1, next, page->btr_level, jumpNode.offset, __FILE__, __LINE__);
					}

					// jump node prefix can't be more than previous jump data length
					if (n != jumpInfo.jumpers && jumpNode.prefix > jumpDataLen) {
						corrupt(tdbb, VAL_INDEX_PAGE_CORRUPT, relation,
								id + 1, next, page->btr_level, jumpNode.offset, __FILE__, __LINE__);
					}

					jumpDataLen = jumpNode.prefix + jumpNode.length;
				}
				n--;
			}

			if (firstNodeOffset > jumpInfo.firstNodeOffset) {
				corrupt(tdbb, VAL_INDEX_PAGE_CORRUPT, relation,
						id + 1, next, page->btr_level, jumpInfo.firstNodeOffset, __FILE__, __LINE__);
			}
		}

		// go through all the nodes on the page and check for validity
		pointer = BTreeNode::getPointerFirstNode(page);
		if (useAllRecordNumbers && firstNode) {
			BTreeNode::readNode(&lastNode, pointer, flags, leafPage);
		}

		const UCHAR* const endPointer = ((UCHAR *) page + page->btr_length);
		while (pointer < endPointer)
		{

			pointer = BTreeNode::readNode(&node, pointer, flags, leafPage);
			if (pointer > endPointer) {
				break;
			}

			if (node.prefix > key.key_length)
			{
				corrupt(tdbb, VAL_INDEX_PAGE_CORRUPT, relation,
						id + 1, next, page->btr_level, node.nodePointer - (UCHAR*)page, __FILE__, __LINE__);
				CCH_RELEASE_TAIL(tdbb, &window);
				return rtn_corrupt;
			}

			const UCHAR* p;
			const UCHAR* q;
			USHORT l; // temporary variable for length

			// make sure the current key is not less than the previous key
			bool duplicateNode = !firstNode && !node.isEndLevel &&
				(key.key_length == (node.length + node.prefix));
			q = node.data;
			p = key.key_data + node.prefix;
			l = MIN(node.length, (USHORT) (key.key_length - node.prefix));
			for (; l; l--, p++, q++)
			{
				if (*p > *q)
				{
					duplicateNode = false;
					corrupt(tdbb, VAL_INDEX_PAGE_CORRUPT, relation,
							id + 1, next, page->btr_level, q - (UCHAR*)page, __FILE__, __LINE__);
				}
				else if (*p < *q)
				{
					duplicateNode = false;
					break;
				}
			}

			// Two checks below are about case where one (shorter) key is
			// a full prefix of another (longer) key, for example:
			// 'aa' and 'aaa', '' and 'a', etc

			// in ascending index short key is less then long key ('aa' < 'aaa')
			// the only exception is end-of-level node with zero length

			if (!firstNode && !descending && !node.isEndLevel && 
				node.prefix < key.key_length && node.length == 0)
			{
				duplicateNode = false;
				corrupt(tdbb, VAL_INDEX_PAGE_CORRUPT, relation,
						id + 1, next, page->btr_level, node.nodePointer - (UCHAR*)page, __FILE__, __LINE__);
			}

			// in descending index short key is greater then long key ('aaa' < 'aa')
			// the only exception is first node after start-of-level node at 
			// non-leaf level (also known as degenerate node)

			if (!firstNode && descending && 
				node.prefix == key.key_length && node.length > 0)
			{
				bool ok = (page->btr_left_sibling == 0 && page->btr_level > 0 && 
						   key.key_length == 0);
				if (ok)
				{
					UCHAR* p = BTreeNode::getPointerFirstNode(page);

					IndexNode first;
					p = BTreeNode::readNode(&first, p, flags, false);
					ok = (node.nodePointer == p);
				}

				if (!ok)
				{
					duplicateNode = false;
					corrupt(tdbb, VAL_INDEX_PAGE_CORRUPT, relation,
							id + 1, next, page->btr_level, node.nodePointer - (UCHAR*)page, __FILE__, __LINE__);
				}
			}

			if (!duplicateNode && nullKeyNode)
			{
				nullKeyHandled = true;
				nullKeyNode = false;
			}

			if (useAllRecordNumbers && (node.recordNumber.getValue() >= 0) && !firstNode &&
				!node.isEndLevel)
			{
				// If this node is equal to the previous one and it's
				// not a MARKER, record number should be same or higher.
				if (duplicateNode)
				{
					if ((!unique || (unique && nullKeyNode)) &&
						(node.recordNumber < lastNode.recordNumber))
					{
						corrupt(tdbb, VAL_INDEX_PAGE_CORRUPT, relation,
							id + 1, next, page->btr_level, node.nodePointer - (UCHAR*)page, __FILE__, __LINE__);
					}
				}

				lastNode = node;
			}

			// save the current key
			memcpy(key.key_data + node.prefix, node.data, node.length);
			//key.key_length = key.key_data + node.prefix + node.length - key.key_data;
			key.key_length = node.prefix + node.length;

			if (!nullKeyHandled && !nullKeyNode && !duplicateNode)
			{
				nullKeyNode = (leafPage || (!leafPage && !firstNode) ) &&
					!node.isEndLevel && (null_key->key_length == key.key_length) &&
					(memcmp(null_key->key_data, key.key_data, null_key->key_length) == 0);
			}

			if (firstNode) {
				firstNode = false;
			}

			if (node.isEndBucket || node.isEndLevel) {
				break;
			}

			// Record the existance of a primary version of a record
			if (leafPage && (vdr_flags & VDR_records)) {
			  RBM_SET(tdbb->getDefaultPool(), &vdr_idx_records, node.recordNumber.getValue());
			}

			// fetch the next page down (if full validation was specified)
			if (!leafPage && (vdr_flags & VDR_records))
			{
				const SLONG down_number = node.pageNumber;
				const RecordNumber down_record_number = node.recordNumber;

				// Note: mark == false for the fetch_page() call here
				// as we don't want to mark the page as visited yet - we'll
				// mark it when we visit it for real later on
				WIN down_window(DB_PAGE_SPACE, -1);
				down_window.win_flags = WIN_garbage_collector;

				btree_page* down_page = 0;
				fetch_page(tdbb, down_number, pag_index, &down_window, &down_page, false);
				const bool downLeafPage = (down_page->btr_level == 0);

				// make sure the initial key is greater than the pointer key
				UCHAR* downPointer = BTreeNode::getPointerFirstNode(down_page);

				IndexNode downNode;
				downPointer = BTreeNode::readNode(&downNode, downPointer, flags, downLeafPage);

				p = downNode.data;
				q = key.key_data;
				l = MIN(key.key_length, downNode.length);
				for (; l; l--, p++, q++)
				{
					if (*p < *q)
					{
						corrupt(tdbb, VAL_INDEX_PAGE_CORRUPT, relation,
								id + 1, next, page->btr_level, node.nodePointer - (UCHAR*)page, __FILE__, __LINE__);
					}
					else if (*p > *q) {
						break;
					}
				}

				// Only check record-number if this isn't the first page in
				// the level and it isn't a MARKER.
				// Also don't check on primary/unique keys, because duplicates aren't
				// sorted on recordnumber, except for NULL keys.
				if (useAllRecordNumbers && down_page->btr_left_sibling &&
					!(downNode.isEndBucket || downNode.isEndLevel) && (!unique || nullKeyNode))
				{
					// Check record number if key is equal with node on
					// pointer page. In that case record number on page
					// down should be same or larger.
					if ((l == 0) && (key.key_length == downNode.length) &&
						(downNode.recordNumber < down_record_number))
					{
						corrupt(tdbb, VAL_INDEX_PAGE_CORRUPT, relation,
								id + 1, next, page->btr_level, node.nodePointer - (UCHAR*)page, __FILE__, __LINE__);
					}
				}

				// check the left and right sibling pointers against the parent pointers
				if (previous_number != down_page->btr_left_sibling)
				{
					corrupt(tdbb, VAL_INDEX_BAD_LEFT_SIBLING, relation,
							id + 1, next, page->btr_level, node.nodePointer - (UCHAR*)page, __FILE__, __LINE__);
				}

				BTreeNode::readNode(&downNode, pointer, flags, leafPage);
				const SLONG next_number = downNode.pageNumber;

				if (!(downNode.isEndBucket || downNode.isEndLevel) &&
					(next_number != down_page->btr_sibling))
				{
					corrupt(tdbb, VAL_INDEX_MISSES_NODE, relation,
							id + 1, next, page->btr_level, node.nodePointer - (UCHAR*)page, __FILE__, __LINE__);
				}

				if (downNode.isEndLevel && down_page->btr_sibling) {
					corrupt(tdbb, VAL_INDEX_ORPHAN_CHILD, relation, id + 1, next);
				}
				previous_number = down_number;

				CCH_RELEASE_TAIL(tdbb, &down_window);
			}
		}

		if (pointer != endPointer || page->btr_length > dbb->dbb_page_size)
		{
			corrupt(tdbb, VAL_INDEX_PAGE_CORRUPT, relation, id + 1,
					next, page->btr_level, pointer - (UCHAR*)page, __FILE__, __LINE__);
		}

		if (next == down)
		{
			if (page->btr_level)
			{
				IndexNode newPageNode;
				BTreeNode::readNode(&newPageNode, BTreeNode::getPointerFirstNode(page), flags, false);
				down = newPageNode.pageNumber;
			}
			else {
				down = 0;
			}
		}

		if (!(next = page->btr_sibling))
		{
			next = down;
			key.key_length = 0;
			previous_number = 0;
			firstNode = true;
			nullKeyNode = false;
			nullKeyHandled = !(unique && null_key);
		}

		// check for circular referenes
		if (next && visited_pages.test(next))
		{
			corrupt(tdbb, VAL_INDEX_CYCLE, relation, id + 1, next);
			next = 0;
		}
		CCH_RELEASE_TAIL(tdbb, &window);
	}

	// If the index & relation contain different sets of records we
	// have a corrupt index
	if (vdr_flags & VDR_records)
	{
		Database::Checkout dcoHolder(dbb);
		RecordBitmap::Accessor accessor(vdr_rel_records);
		if (accessor.getFirst())
			do {
				SINT64 next_number = accessor.current();
				if (!RecordBitmap::test(vdr_idx_records, next_number)) {
					return corrupt(tdbb, VAL_INDEX_MISSING_ROWS, relation, id + 1, next_number);
				}
			} while (accessor.getNext());
	}

	return rtn_ok;
}

void Validation::walk_log(thread_db* tdbb)
{
/**************************************
 *
 *	w a l k _ l o g
 *
 **************************************
 *
 * Functional description
 *	Walk the log and overflow pages
 *
 **************************************/
	log_info_page* page = 0;
	SLONG page_num = LOG_PAGE;

	SET_TDBB(tdbb);

	while (page_num)
	{
		WIN window(DB_PAGE_SPACE, -1);
		fetch_page(tdbb, page_num, pag_log, &window, &page);
		page_num = page->log_next_page;
		CCH_RELEASE(tdbb, &window);
	}
}

void Validation::walk_pip(thread_db* tdbb)
{
/**************************************
 *
 *	w a l k _ p i p
 *
 **************************************
 *
 * Functional description
 *	Walk the page inventory pages.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = tdbb->getDatabase();
	CHECK_DBB(dbb);

	PageManager& pageSpaceMgr = dbb->dbb_page_manager;
	const PageSpace* pageSpace = pageSpaceMgr.findPageSpace(DB_PAGE_SPACE);
	fb_assert(pageSpace);

	page_inv_page* page = 0;

	for (USHORT sequence = 0; true; sequence++)
	{
		const SLONG page_number =
			sequence ? sequence * pageSpaceMgr.pagesPerPIP - 1 : pageSpace->ppFirst;
#ifdef DEBUG_VAL_VERBOSE
		if (VAL_debug_level)
			fprintf(stdout, "walk_pip: page %d\n", page_number);
#endif
		WIN window(DB_PAGE_SPACE, -1);
		fetch_page(tdbb, page_number, pag_pages, &window, &page);
		const UCHAR byte = page->pip_bits[pageSpaceMgr.bytesBitPIP - 1];
		CCH_RELEASE(tdbb, &window);
		if (byte & 0x80)
			break;
	}
}

RTN Validation::walk_pointer_page(thread_db* tdbb, jrd_rel* relation, int sequence)
{
/**************************************
 *
 *	w a l k _ p o i n t e r _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Walk a pointer page for a relation.  Return rtn_ok if there are more to go.
 *
 **************************************/
	SET_TDBB(tdbb);

	Database* dbb = tdbb->getDatabase();

	const vcl* vector = relation->getBasePages()->rel_pages;

	if (!vector || sequence >= static_cast<int>(vector->count())) {
		return corrupt(tdbb, VAL_P_PAGE_LOST, relation, sequence);
	}

	pointer_page* page = 0;
	WIN window(DB_PAGE_SPACE, -1);
	window.win_flags = WIN_garbage_collector;

	fetch_page(tdbb, (*vector)[sequence], pag_pointer, &window, &page);

#ifdef DEBUG_VAL_VERBOSE
	if (VAL_debug_level)
	{
		fprintf(stdout, "walk_pointer_page: page %d relation %d sequence %d\n",
				   (*vector)[sequence], relation->rel_id, sequence);
	}
#endif

	// Give the page a quick once over

	if (page->ppg_relation != relation->rel_id || page->ppg_sequence != sequence)
	{
		CCH_RELEASE_TAIL(tdbb, &window);
		return corrupt(tdbb, VAL_P_PAGE_INCONSISTENT, relation, sequence);
	}

	// Walk the data pages (someday we may optionally walk pages with "large objects"

	SLONG seq = (SLONG) sequence *dbb->dbb_dp_per_pp;

	USHORT slot = 0;
	for (SLONG* pages = page->ppg_page; slot < page->ppg_count; slot++, pages++, seq++)
	{
		if (*pages)
		{
			const RTN result = walk_data_page(tdbb, relation, *pages, seq);
			if (result != rtn_ok && (vdr_flags & VDR_repair))
			{
				CCH_MARK(tdbb, &window);
				*pages = 0;
			}
		}
	}

	// If this is the last pointer page in the relation, we're done

	if (page->ppg_header.pag_flags & ppg_eof)
	{
		CCH_RELEASE_TAIL(tdbb, &window);
		return rtn_eof;
	}

	// Make sure the "next" pointer agrees with the pages relation

	if (++sequence >= static_cast<int>(vector->count()) ||
		(page->ppg_next && page->ppg_next != (*vector)[sequence]))
	{
		CCH_RELEASE_TAIL(tdbb, &window);

		if (vdr_flags & VDR_online)
		{
			// relation could be extended before we acquired its lock in PR mode
			// let's re-read pointer pages and check again 

			DPM_scan_pages(tdbb);

			vector = relation->getBasePages()->rel_pages;

			--sequence;
			if (!vector || sequence >= static_cast<int>(vector->count())) {
				return corrupt(tdbb, VAL_P_PAGE_LOST, relation, sequence);
			}

			fetch_page(tdbb, (*vector)[sequence], pag_pointer, &window, &page, false);

			++sequence;
			const bool error = sequence >= static_cast<int>(vector->count()) ||
				(page->ppg_next && page->ppg_next != (*vector)[sequence]);

			CCH_RELEASE_TAIL(tdbb, &window);

			if (!error)
				return rtn_ok;
		}

		return corrupt(tdbb, VAL_P_PAGE_INCONSISTENT, relation, sequence);
	}

	CCH_RELEASE_TAIL(tdbb, &window);
	return rtn_ok;
}


RTN Validation::walk_record(thread_db* tdbb, jrd_rel* relation, rhd* header, USHORT length, 
							RecordNumber number, bool delta_flag)
{
/**************************************
 *
 *	w a l k _ r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Walk a record.
 *
 **************************************/
	SET_TDBB(tdbb);

#ifdef DEBUG_VAL_VERBOSE
	if (VAL_debug_level)
	{
		fprintf(stdout, "record: number %ld (%d/%d) ",
				   number,
				   (USHORT) number / tdbb->getDatabase()->dbb_max_records,
				   (USHORT) number % tdbb->getDatabase()->dbb_max_records);
		print_rhd(length, header);
	}
#endif

	if (header->rhd_flags & rhd_damaged)
	{
		corrupt(tdbb, VAL_REC_DAMAGED, relation, number.getValue());
		return rtn_ok;
	}

	if (header->rhd_transaction > vdr_max_transaction)
	{
		corrupt(tdbb, VAL_REC_BAD_TID, relation, number.getValue(), header->rhd_transaction);
	}

	// If there's a back pointer, verify that it's good

	if (header->rhd_b_page && !(header->rhd_flags & rhd_chain))
	{
		const RTN result = walk_chain(tdbb, relation, header, number);
		if (result != rtn_ok)
			return result;
	}

	// If the record is a fragment, not large, or we're not interested in
	// chasing records, skip the record

	if (header->rhd_flags & (rhd_fragment | rhd_deleted) ||
		!((header->rhd_flags & rhd_large) || (vdr_flags & VDR_records)) )
	{
		return rtn_ok;
	}

	// Pick up what length there is on the fragment

	const rhdf* fragment = (rhdf*) header;

	const char* p;
	const char* end;
	if (header->rhd_flags & rhd_incomplete)
	{
		p = (SCHAR*) fragment->rhdf_data;
		end = p + length - OFFSETA(rhdf*, rhdf_data);
	}
	else
	{
		p = (SCHAR*) header->rhd_data;
		end = p + length - OFFSETA(rhd*, rhd_data);
	}

	USHORT record_length = 0;

	while (p < end)
	{
		const signed char c = *p++;
		if (c >= 0)
		{
			record_length += c;
			p += c;
		}
		else
		{
			record_length -= c;
			p++;
		}
	}

	// Next, chase down fragments, if any

	SLONG page_number = fragment->rhdf_f_page;
	USHORT line_number = fragment->rhdf_f_line;
	USHORT flags = fragment->rhdf_flags;

	data_page* page = 0;
	while (flags & rhd_incomplete)
	{
		WIN window(DB_PAGE_SPACE, -1);
		window.win_flags = WIN_garbage_collector;

		fetch_page(tdbb, page_number, pag_data, &window, &page);
		const data_page::dpg_repeat* line = &page->dpg_rpt[line_number];
		if (page->dpg_relation != relation->rel_id ||
			line_number >= page->dpg_count || !(length = line->dpg_length))
		{
			corrupt(tdbb, VAL_REC_FRAGMENT_CORRUPT, relation, number.getValue());
			CCH_RELEASE_TAIL(tdbb, &window);
			return rtn_corrupt;
		}
		fragment = (rhdf*) ((UCHAR *) page + line->dpg_offset);
#ifdef DEBUG_VAL_VERBOSE
		if (VAL_debug_level)
		{
			fprintf(stdout, "fragment: pg %d/%d ", page_number, line_number);
			print_rhd(line->dpg_length, (rhd*) fragment);
		}
#endif
		if (fragment->rhdf_flags & rhd_incomplete)
		{
			p = (SCHAR *) fragment->rhdf_data;
			end = p + line->dpg_length - OFFSETA(rhdf*, rhdf_data);
		}
		else
		{
			p = (SCHAR *) ((rhd*) fragment)->rhd_data;
			end = p + line->dpg_length - OFFSETA(rhd*, rhd_data);
		}
		while (p < end)
		{
			const signed char c = *p++;
			if (c >= 0)
			{
				record_length += c;
				p += c;
			}
			else
			{
				record_length -= c;
				p++;
			}
		}
		page_number = fragment->rhdf_f_page;
		line_number = fragment->rhdf_f_line;
		flags = fragment->rhdf_flags;
		CCH_RELEASE_TAIL(tdbb, &window);
	}

	// Check out record length and format

	const Format* format = MET_format(tdbb, relation, header->rhd_format);

	if (!delta_flag && record_length != format->fmt_length)
		return corrupt(tdbb, VAL_REC_WRONG_LENGTH, relation, number.getValue());

	return rtn_ok;
}

#define DECOMPOSE(n, divisor, q, r) {r = n % divisor; q = n / divisor;}

void Validation::checkDPinPP(thread_db* tdbb, jrd_rel* relation, SLONG page_number)
{
	/**************************************
	*
	* Functional description
	*	Check if data page presented in pointer page.
	* If not we try to fix it by setting pointer page slot in the page_number.
	* Early in walk_chain we observed that this page in related to the relation so we skip such kind of check here.
	**************************************/

	WIN window(DB_PAGE_SPACE, page_number);
	data_page* dpage;
	fetch_page(tdbb, page_number, pag_data, &window, &dpage, false);
	const SLONG sequence = dpage->dpg_sequence;
	CCH_RELEASE_TAIL(tdbb, &window);

	USHORT slot;
	ULONG pp_sequence;
	Database* dbb = tdbb->getDatabase();
	DECOMPOSE(sequence, dbb->dbb_dp_per_pp, pp_sequence, slot);

	const vcl* vector = relation->getBasePages()->rel_pages;
	pointer_page* ppage = 0;
	if (pp_sequence < vector->count())
	{
		fetch_page(tdbb, (*vector)[pp_sequence], pag_pointer, &window, &ppage, false);
		if (slot >= ppage->ppg_count)
		{
			corrupt(tdbb, VAL_DATA_PAGE_SLOT_NOT_FOUND, relation, page_number, window.win_page.getPageNum(), slot);
			if (vdr_flags & VDR_update && slot < dbb->dbb_dp_per_pp)
			{
				CCH_MARK(tdbb, &window);
				for (USHORT i = ppage->ppg_count; i < slot; i++)
				{
					ppage->ppg_page[i] = 0;

					// Clear control fields
					UCHAR* byte = (UCHAR *) (&ppage->ppg_page[dbb->dbb_dp_per_pp]) + (i >> 2);
					UCHAR bits = 3 << ((i & 3) << 1);
					*byte &= ~bits;
				}
				ppage->ppg_page[slot] = page_number;
				ppage->ppg_count = slot + 1;

				// Restore control fields
				UCHAR* byte = (UCHAR *) (&ppage->ppg_page[dbb->dbb_dp_per_pp]) + (slot >> 2);
				UCHAR bit = 1 << ((slot & 3) << 1);

				const UCHAR flags = dpage->dpg_header.pag_flags;
				if (flags & dpg_full)
					*byte |= bit;
				else
					*byte &= ~bit;

				bit <<= 1;

				if (flags & dpg_large)
					*byte |= bit;
				else
					*byte &= ~bit;
			}
		}
		else if (page_number != ppage->ppg_page[slot])
		{
			corrupt(tdbb, VAL_DATA_PAGE_SLOT_BAD_VAL, relation, page_number, window.win_page.getPageNum(), slot, ppage->ppg_page[slot]);
			if (vdr_flags & VDR_update && !ppage->ppg_page[slot])
			{
				CCH_MARK(tdbb, &window);
				ppage->ppg_page[slot] = page_number;

				// Restore control fields
				UCHAR* byte = (UCHAR *) (&ppage->ppg_page[dbb->dbb_dp_per_pp]) + (slot >> 2);
				UCHAR bit = 1 << ((slot & 3) << 1);

				const UCHAR flags = dpage->dpg_header.pag_flags;
				if (flags & dpg_full)
					*byte |= bit;
				else
					*byte &= ~bit;

				bit <<= 1;

				if (flags & dpg_large)
					*byte |= bit;
				else
					*byte &= ~bit;
			}
		}
	}
	else
		corrupt(tdbb, VAL_DATA_PAGE_HASNO_PP, relation, page_number, dpage->dpg_sequence);

	CCH_RELEASE_TAIL(tdbb, &window);
}

void Validation::checkDPinPIP(thread_db* tdbb, jrd_rel* relation, SLONG page_number)
{
	/**************************************
	*
	* Functional description
	*	Check if data page presented in page inventory page.
	* If not we try to fix it by clearing the necessary bit on the page.
	**************************************/

	Database* dbb = tdbb->getDatabase();

	PageManager& pageMgr = dbb->dbb_page_manager;
	PageSpace* pageSpace = pageMgr.findPageSpace(DB_PAGE_SPACE);
	fb_assert(pageSpace);

	const SLONG sequence = page_number / pageMgr.pagesPerPIP;
	const SLONG relative_bit = page_number % pageMgr.pagesPerPIP;

	WIN pip_window(DB_PAGE_SPACE, (sequence == 0) ? pageSpace->ppFirst : sequence * pageMgr.pagesPerPIP - 1);

	page_inv_page* pages;
	fetch_page(tdbb, pip_window.win_page.getPageNum(), pag_pages, &pip_window, &pages, false);
	if (pages->pip_bits[relative_bit >> 3] & (1 << (relative_bit & 7)))
	{
		corrupt(tdbb, VAL_DATA_PAGE_ISNT_IN_PIP, relation, page_number, pip_window.win_page.getPageNum(), relative_bit);
		if (vdr_flags & VDR_update)
		{
			CCH_MARK(tdbb, &pip_window);
			pages->pip_bits[relative_bit >> 3] &= ~(1 << (relative_bit & 7));
		}
	}

	CCH_RELEASE_TAIL(tdbb, &pip_window);
}

RTN Validation::walk_relation(thread_db* tdbb, jrd_rel* relation)
{
/**************************************
 *
 *	w a l k _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Walk all pages associated with a given relation.
 *
 **************************************/

	SET_TDBB(tdbb);

	try {

	// skip deleted relations
	if (relation->rel_flags & (REL_deleted | REL_deleting)) {
		return rtn_ok;
	}

#ifdef DEBUG_VAL_VERBOSE
	if (VAL_debug_level)
		fprintf(stdout, "walk_relation: id %d Format %d %s %s\n",
				   relation->rel_id, relation->rel_current_fmt,
				   relation->rel_name.c_str(), relation->rel_owner_name.c_str());
#endif

	// If it's a view, external file or virtual table, skip this

	if (relation->rel_view_rse || relation->rel_file || relation->isVirtual()) {
		return rtn_ok;
	}

	AutoLock lckRead(tdbb);
	jrd_rel::GCExclusive lckGC(tdbb, relation);
	if (vdr_flags & VDR_online) 
	{
		lckRead = jrd_rel::createLock(tdbb, NULL, relation, LCK_relation, false);
		if (!LCK_lock(tdbb, lckRead, LCK_PR, vdr_lock_tout))
		{
			output("Acquire relation lock failed\n");
			vdr_errors++;
			return rtn_ok;
		}

		if (!lckGC.acquire(vdr_lock_tout))
		{
			output("Acquire garbage collection lock failed\n");
			vdr_errors++;
			return rtn_ok;
		}

		WIN window(DB_PAGE_SPACE, -1);
		header_page* page = 0;
		fetch_page(tdbb, (SLONG) HEADER_PAGE, pag_header, &window, &page, false);
		vdr_max_transaction = page->hdr_next_transaction;
		CCH_RELEASE(tdbb, &window);
	}


	// Walk pointer and selected data pages associated with relation

	vdr_rel_backversion_counter = 0;
	PageBitmap::reset(vdr_backversion_pages);
	vdr_rel_chain_counter = 0;
	PageBitmap::reset(vdr_chain_pages);
	RecordBitmap::reset(vdr_rel_records);

	for (SLONG sequence = 0; true; sequence++)
	{
		const vcl* vector = relation->getBasePages()->rel_pages;
		const int ppCnt = vector ? vector->count() : 0;

		output("  process pointer page %4d of %4d\n", sequence, ppCnt);

		const RTN result = walk_pointer_page(tdbb, relation, sequence);
		if (result == rtn_eof) {
			break;
		}
		if (result != rtn_ok) {
			return result;
		}
	}

	// Walk indices for the relation
	walk_root(tdbb, relation);

	lckGC.release();

	// Compare backversion pages and chain pages
	if ((vdr_flags & VDR_records) &&
		vdr_chain_pages && (vdr_rel_chain_counter > vdr_rel_backversion_counter))
	{
		if (vdr_backversion_pages)
		{
			PageBitmap::Accessor c(vdr_chain_pages);
			PageBitmap::Accessor b(vdr_backversion_pages);
			if (c.getFirst() && b.getFirst()) {
				for (bool next = true; next; next = c.getNext() )
				{
					if (c.current() == b.current())
						b.getNext();
					else if ((c.current() < b.current()) || !b.getNext()) {
						//fprintf(stdout, "chain page was visited not via data pages %d\n", c.current());
						checkDPinPP(tdbb, relation, c.current());
						checkDPinPIP(tdbb, relation, c.current());
					}
				}
			}
		}
		else
		{
			PageBitmap::Accessor c(vdr_chain_pages);
			if (c.getFirst())
			{
				for (bool next = true; next; next = c.getNext() )
				{
					//fprintf(stdout, "chain page was visited not via data pages %d\n", c.current());
					checkDPinPP(tdbb, relation, c.current());
					checkDPinPIP(tdbb, relation, c.current());
				}
			}
		}
	}

	// See if the counts of backversions match
	if ((vdr_flags & VDR_records) &&
		(vdr_rel_backversion_counter > vdr_rel_chain_counter))
	{
		 return corrupt(tdbb, VAL_REL_CHAIN_ORPHANS, relation,
						vdr_rel_backversion_counter -  vdr_rel_chain_counter,
						 vdr_rel_chain_counter);
	}

	}	// try
	catch (const Firebird::Exception&)
	{
		if (!(vdr_flags & VDR_online))
		{
			const char* msg = relation->rel_name.length() > 0 ?
				"bugcheck during scan of table %d (%s)" :
				"bugcheck during scan of table %d";
			gds__log(msg, relation->rel_id, relation->rel_name.c_str());
		}
#ifdef DEBUG_VAL_VERBOSE
		if (VAL_debug_level)
		{
			char s[256];
			SNPRINTF(s, sizeof(s), msg, relation->rel_id, relation->rel_name.c_str());
			fprintf(stdout, "LOG:\t%s\n", s);
		}
#endif
		throw;
	}

	return rtn_ok;
}


RTN Validation::walk_root(thread_db* tdbb, jrd_rel* relation)
{
/**************************************
 *
 *	w a l k _ r o o t
 *
 **************************************
 *
 * Functional description
 *	Walk index root page for a relation as well as any indices.
 *
 **************************************/
	SET_TDBB(tdbb);

	// If the relation has an index root, walk it
	RelationPages* relPages = relation->getBasePages();

	if (!relPages->rel_index_root) {
		return corrupt(tdbb, VAL_INDEX_ROOT_MISSING, relation);
	}

	index_root_page* page = 0;
	WIN window(DB_PAGE_SPACE, -1);
	fetch_page(tdbb, relPages->rel_index_root, pag_root, &window, &page);

	for (USHORT i = 0; i < page->irt_count; i++) 
	{
		if (page->irt_rpt[i].irt_root == 0)
			continue;

		MetaName index;
		MET_lookup_index(tdbb, index, relation->rel_name, i + 1);

		if (vdr_idx_incl)
		{
			vdr_idx_incl->reset();
			if (!vdr_idx_incl->process((UCHAR*)index.c_str(), index.length()) || !vdr_idx_incl->result())
				continue;
		}

		if (vdr_idx_excl)
		{
			vdr_idx_excl->reset();
			if (!vdr_idx_excl->process((UCHAR*)index.c_str(), index.length()) || vdr_idx_excl->result())
				continue;
		}

		output("Index %d (%s)\n", i + 1, index.c_str());
		walk_index(tdbb, relation, *page, i);
	}

	CCH_RELEASE(tdbb, &window);

	return rtn_ok;
}

RTN Validation::walk_tip(thread_db* tdbb, SLONG transaction)
{
/**************************************
 *
 *	w a l k _ t i p
 *
 **************************************
 *
 * Functional description
 *	Walk transaction inventory pages.
 *
 **************************************/

	SET_TDBB(tdbb);
	Database* dbb = tdbb->getDatabase();
	CHECK_DBB(dbb);

	const vcl* vector = dbb->dbb_t_pages;
	if (!vector) {
		return corrupt(tdbb, VAL_TIP_LOST, 0);
	}

	tx_inv_page* page = 0;
	const ULONG pages = transaction / dbb->dbb_page_manager.transPerTIP;

	for (ULONG sequence = 0; sequence <= pages; sequence++)
	{
		if (!(*vector)[sequence] || sequence >= vector->count())
		{
			corrupt(tdbb, VAL_TIP_LOST_SEQUENCE, 0, sequence);
			if (!(vdr_flags & VDR_repair))
				continue;
			TRA_extend_tip(tdbb, sequence);
			vector = dbb->dbb_t_pages;
		}

		WIN window(DB_PAGE_SPACE, -1);
		fetch_page(tdbb, (*vector)[sequence], pag_transactions, &window, &page);

#ifdef DEBUG_VAL_VERBOSE
		if (VAL_debug_level)
			fprintf(stdout, "walk_tip: page %d next %d\n", (*vector)[sequence], page->tip_next);
#endif
		if (page->tip_next && page->tip_next != (*vector)[sequence + 1])
		{
			corrupt(tdbb, VAL_TIP_CONFUSED, 0, sequence);
		}
		CCH_RELEASE(tdbb, &window);
	}

	return rtn_ok;
}
