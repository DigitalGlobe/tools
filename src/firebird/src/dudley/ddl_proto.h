/*
 *	PROGRAM:	Data Definition Utility
 *	MODULE:		ddl_proto.h
 *	DESCRIPTION:	Prototype header file for ddl.cpp
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

#ifndef DUDLEY_DDL_PROTO_H
#define DUDLEY_DDL_PROTO_H

#include "../common/classes/SafeArg.h"

UCHAR*		DDL_alloc(int);
int			DDL_db_error(ISC_STATUS*, USHORT, const MsgFormat::SafeArg& arg = MsgFormat::SafeArg());
int			DDL_err(USHORT, const MsgFormat::SafeArg& arg = MsgFormat::SafeArg());
void		DDL_error_abort(ISC_STATUS*, USHORT, const MsgFormat::SafeArg& arg = MsgFormat::SafeArg());
void		DDL_exit(int);
void		DDL_msg_partial(USHORT, const MsgFormat::SafeArg& arg = MsgFormat::SafeArg());
void		DDL_msg_put(USHORT, const MsgFormat::SafeArg& arg = MsgFormat::SafeArg());
DUDLEY_NOD	DDL_pop(dudley_lls**);
void		DDL_push(DUDLEY_NOD, dudley_lls**);
bool		DDL_yes_no(USHORT);

inline void LLS_PUSH(DUDLEY_NOD object, dudley_lls** stack)
{
	DDL_push(object, stack);
}

inline DUDLEY_NOD LLS_POP(dudley_lls** stack)
{
	return DDL_pop(stack);
}

#endif // DUDLEY_DDL_PROTO_H

