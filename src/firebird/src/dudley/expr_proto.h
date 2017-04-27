/*
 *	PROGRAM:	Data Definition Utility
 *	MODULE:		expr_proto.h
 *	DESCRIPTION:	Prototype header file for expr.cpp
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

#ifndef DUDLEY_EXPR_PROTO_H
#define DUDLEY_EXPR_PROTO_H

DUDLEY_NOD	EXPR_boolean(USHORT*);
DUDLEY_NOD	EXPR_rse(bool);
DUDLEY_NOD	EXPR_statement();
DUDLEY_NOD	EXPR_value(USHORT*, bool*);

#endif // DUDLEY_EXPR_PROTO_H

