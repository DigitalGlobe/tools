/*
 *	PROGRAM:	JRD Lock Manager
 *	MODULE:		vmslo_proto.h
 *	DESCRIPTION:	Prototype header file for vmslock.cpp
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

#ifndef JRD_VMSLO_PROTO_H
#define JRD_VMSLO_PROTO_H

bool	LOCK_convert(SLONG, UCHAR, SSHORT, lock_ast_t, void*, ISC_STATUS*);
int		LOCK_deq(SLONG);
UCHAR	LOCK_downgrade(SLONG, ISC_STATUS *);
SLONG	LOCK_enq(SLONG, SLONG, USHORT, const UCHAR*, USHORT, UCHAR,
					  lock_ast_t, int*, SLONG, SSHORT, ISC_STATUS*, SLONG);
void	LOCK_fini(ISC_STATUS*, SLONG *);
int		LOCK_init(ISC_STATUS*, bool, LOCK_OWNER_T, UCHAR, SLONG *);
void	LOCK_manager(SLONG);
SLONG	LOCK_read_data(SLONG);
SLONG	LOCK_read_data2(SLONG, USHORT, UCHAR *, USHORT, SLONG);
void	LOCK_re_post(lock_ast_t, int*, SLONG);
SLONG	LOCK_write_data(SLONG, SLONG);

#endif // JRD_VMSLO_PROTO_H

