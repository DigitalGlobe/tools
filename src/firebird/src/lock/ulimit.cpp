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
 */
#include "firebird.h"
#include "/bsd4.2/usr/include/sys/time.h"
#include "/bsd4.2/usr/include/sys/resource.h"

int ulimit( int cmd, int new_limit)
{
/**************************************
 *
 *	u l i m i t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	rlimit rlp;

	getrlimit(RLIMIT_DATA, &rlp);
	return rlp.rlim_max + 4000000;
}
