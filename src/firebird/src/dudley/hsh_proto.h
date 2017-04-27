/*
 *	PROGRAM:	Data Definition Utility
 *	MODULE:		hsh_proto.h
 *	DESCRIPTION:	Prototype header file for hsh.cpp
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

#ifndef DUDLEY_HSH_PROTO_H
#define DUDLEY_HSH_PROTO_H

void	HSH_init();
void	HSH_insert(SYM);
SYM		HSH_lookup(const SCHAR*, USHORT);
void	HSH_remove(SYM);
SYM		HSH_typed_lookup(const TEXT*, USHORT, enum sym_t);

#endif /* DUDLEY_HSH_PROTO_H */

