/*
 *	PROGRAM:	JRD Lock Manager
 *	MODULE:		driver.cpp
 *	DESCRIPTION:	Stand alone test driver
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

#include "firebird.h"
#include <stdio.h>

#include "../jrd/common.h"
#include "../jrd/ibase.h"
#include "../jrd/isc.h"
//#include "../lock/lock.h"

#ifdef WIN_NT
#include <process.h>
#endif

static int ast(void*);
static int lookup_agg(const UCHAR*);
static int lookup_lock(const UCHAR*);
static void print_help();

static const struct tbl
{
	const char* tbl_string;
	SSHORT tbl_code;
} types[] =
	{
		{"null", LCK_null},
		{"pr", LCK_PR},
		{"sr", LCK_SR},
		{"pw", LCK_PW},
		{"sw", LCK_SW},
		{"ex", LCK_EX},
		{NULL, LCK_none}
	};

static const struct tagg
{
	const char* tagg_string;
	SSHORT tagg_code;
} aggs[] =
	{
		{"min", LCK_MIN},
		{"max", LCK_MAX},
		{"cnt", LCK_CNT},
		{"sum", LCK_SUM},
		{"avg", LCK_AVG},
		{"any", LCK_ANY},
		{NULL, 0}
	};

static int wait, sw_release, locks[100], levels[100];
static SLONG lck_owner_handle = 0;


void main( int argc, char **argv)
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	Test driver for lock manager.
 *
 **************************************/
	UCHAR op[500], arg[500];
	SSHORT type;
	SLONG lock;
	ISC_STATUS_ARRAY status_vector;

	printf("\nStand alone Lock Manager driver program.\n");
	printf("****************************************\n\n");
	printf("pid = %d\n\n", getpid());
	printf("\n");

	if (LOCK_init(status_vector, getpid(), 1, &lck_owner_handle))
	{
		printf("LOCK_init failed\n");
		isc_print_status(status_vector);
		exit(0);
	}

	wait = 1;
	sw_release = 1;
	SSHORT slot = 0;

	// Force a dummy parent lock to test query data functionality

	const SLONG parent = LOCK_enq(NULL,			// prior request
							NULL,				// parent request
							0,					// series
							"parent",			// value
							strlen("parent"),	// length of key
							LCK_SR,				// lock type
							NULL, NULL,			// AST and argument
							0, wait, status_vector, lck_owner_handle);

	while (true)
	{
		printf("Request: ");
		ISC_STATUS status = scanf("%s%s", op, arg);
		if (status == EOF)
			continue;

		if (!strcmp(op, "rel"))
		{
			const SSHORT n = atoi(arg);
			if (n < slot && (lock = locks[n]))
			{
				LOCK_deq(lock);
				locks[n] = 0;
			}
			else
				printf("*** BAD LOCK\n");
			continue;
		}
		if (!strcmp(op, "ar"))
		{
			sw_release = atoi(arg);
			continue;
		}
		if (!strcmp(op, "w"))
		{
			wait = atoi(arg);
			continue;
		}
		if (!strcmp(op, "q"))
			exit(0);
		if (!strcmp(op, "rd"))
		{
			const SSHORT n = atoi(arg);
			if (n >= slot || !(lock = locks[n]))
			{
				printf("bad lock\n");
				continue;
			}
			SLONG data = LOCK_read_data(lock);
			printf("lock data = %d\n", data);
			continue;
		}
		if (!strcmp(op, "wd"))
		{
			const SSHORT n = atoi(arg);
			if (n >= slot || !(lock = locks[n]))
			{
				printf("bad lock\n");
				continue;
			}
			scanf("%s", arg);
			SLONG data = atoi(arg);
			LOCK_write_data(lock, data);
			continue;
		}
		if (!strcmp(op, "qd"))
		{
			const SSHORT agg = lookup_agg(arg);
			if (!agg)
			{
				printf("bad query aggregate\n");
				continue;
			}
			SLONG data = LOCK_query_data(parent, 0, agg);
			if (agg == LCK_ANY)
				printf("%s = %s\n", arg, (data ? "true" : "false"));
			else
				printf("%s = %d\n", arg, data);
			continue;
		}
		if (!strcmp(op, "cvt"))
		{
			const SSHORT n = atoi(arg);
			scanf("%s", op);
			if (!(type = lookup_lock(op)))
			{
				printf("bad lock type\n");
				continue;
			}
			if (n >= slot || !(lock = locks[n]))
			{
				printf("bad lock\n");
				continue;
			}
			if (!LOCK_convert(lock, type, wait, NULL, 0, status_vector))
			{
				printf("*** CONVERSION FAILED: status_vector[1] = %d",
						  status_vector[1]);
				switch (status_vector[1])
				{
				case isc_lock_timeout:
					printf(" (isc_lock_timeout)\n");
					break;
				case isc_lock_conflict:
					printf(" (isc_lock_conflict)\n");
					break;
				case isc_deadlock:
					printf(" (isc_deadlock)\n");
					break;
				default:
					printf("\n");
					break;
				}
			}
			continue;
		}
		if (type = lookup_lock(op))
		{
			lock = LOCK_enq(NULL,			// prior request
							parent,			// parent request
							0,				// series
							arg,			// value
							strlen(arg),	// length of key
							type,			// lock type
							(sw_release ? ast : NULL), slot,	// AST and argument
							0, wait, status_vector, lck_owner_handle);
			if (lock)
			{
				printf("lock# %d = %d\n", slot, lock);
				levels[slot] = type;
				locks[slot++] = lock;
			}
			else
			{
				printf("*** LOCK REJECTED: status_vector[1] = %d", status_vector[1]);
				switch (status_vector[1])
				{
				case isc_lock_timeout:
					printf(" (isc_lock_timeout)\n");
					break;
				case isc_lock_conflict:
					printf(" (isc_lock_conflict)\n");
					break;
				case isc_deadlock:
					printf(" (isc_deadlock)\n");
					break;
				default:
					printf("\n");
				}
			}
		}
		else {
			print_help();
		}
	}

}


static int ast(void* slot_void)
{
/**************************************
 *
 *	a s t
 *
 **************************************
 *
 * Functional description
 *	Dummy blocking ast.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;
	int sw_release_use = sw_release;

	const int slot = (int)(IPTR) slot_void; // static cast

	printf("*** blocking AST for lock# %d ", slot);

	if (sw_release < 0)
	{
		printf("In the AST routine.\n");
		printf("Enter ar value [1=nothing, 2=release, 3=downgrade]:");
		scanf("%d", &sw_release_use);
	}

	if (sw_release_use == 1)
	{
		printf("-- ignored ***\n");
		return 0;
	}

	if (sw_release_use > 2 && levels[slot] == LCK_EX)
	{
		LOCK_convert(locks[slot], LCK_SR, wait, NULL, 0, status_vector);
		levels[slot] = LCK_SR;
		printf("-- down graded to SR ***\n");
		return 0;
	}

	LOCK_deq(locks[slot]);
	printf("-- released ***\n");
	return 0;
}


static int lookup_agg(const UCHAR* string)
{
/**************************************
 *
 *	l o o k u p _ a g g
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	for (const tagg* ptr = aggs; ptr->tagg_string; ptr++)
	{
		if (strcmp(ptr->tagg_string, reinterpret_cast<const char*>(string)) == 0)
			return ptr->tagg_code;
	}

	return 0;
}


static int lookup_lock(const UCHAR* string)
{
/**************************************
 *
 *	l o o k u p _ l o c k
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	for (const tbl* ptr = types; ptr->tbl_string; ptr++)
	{
		if (strcmp(ptr->tbl_string, string) == 0)
			return ptr->tbl_code;
	}

	return LCK_none;
}


static void print_help()
{
/**************************************
*
*      p r i n t _ h e l p
*
**************************************
*
* Functional description
*      Prints the command syntax.
*
***************************************/

	printf("Command syntax is:\n");
	printf("\tnull <key>\tAcquire an 'null' lock on <key> (returns <lock#>)\n");
	printf("\tsr <key>\tAcquire an 'sr' lock on <key> (returns <lock#>)\n");
	printf("\tpr <key>\tAcquire an 'pr' lock on <key> (returns <lock#>)\n");
	printf("\tsw <key>\tAcquire an 'sw' lock on <key> (returns <lock#>)\n");
	printf("\tpw <key>\tAcquire an 'pw' lock on <key> (returns <lock#>)\n");
	printf("\tex <key>\tAcquire an 'ex' lock on <key> (returns <lock#>)\n");
	printf("\tw <value>\tSet wait parameter to LOCK_enq:\n");
	printf("\t\t\t\tvalue>0   willing to wait forever [default]\n");
	printf("\t\t\t\tvalue=0   not willing to wait\n");
	printf("\t\t\t\tvalue<0   willing to wait for that many seconds\n");
	printf("\tar <value>\tControls the AST\n");
	printf("\t\t\t\tvalue=1   AST will not release nor downgrade [default]\n");
	printf("\t\t\t\tvalue=2   AST will release lock\n");
	printf("\t\t\t\tvalue=3   AST will attempt to downgrade first\n");
	printf("\t\t\t\tvalue<0   AST will prompt\n");
	printf("\t\t\t\tvalue=0   AST is not supplied\n");
	printf("\trel <lock#>\tRelease lock <lock#>\n");
	printf("\tcvt <lock#> <lock-type>\n\t\t\tConvert lock <lock#> to type <lock-type>\n");
	printf("\trd <lock#>\tRead data <lock#>\n");
	printf("\twd <lock#> <data>\tWrite <data> to <lock#>\n");
	printf("\tqd <agg>\tQuery data for <agg := min,max,cnt,sum,avg,any>\n");
	printf("\tq <don't-care>\tQuit\n");
	printf("\nRemember the isc_config modifier:\n");
	printf("\tV4_LOCK_GRANT_ORDER {0 | 1}\n");
	printf("\t\t\t0 = disable lock-grant-order (no lock-fairness)\n");
	printf("\t\t\t1 = enable lock-grant-order [default]\n");
}
