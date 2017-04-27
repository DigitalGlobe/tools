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
#include "/sys5/usr/include/sys/types.h"
#include "/sys5/usr/include/sys/ipc.h"
#include "/sys5/usr/include/sys/shm.h"
#include "/sys5/usr/include/sys/sem.h"
//#include "lock.h"

int main( int argc, char **argv)
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	const int semid = semget(SEM_KEY, 2, 0);
	if (semid == -1)
		return;
	return semctl(semid, 0, IPC_RMID, 0);
}
