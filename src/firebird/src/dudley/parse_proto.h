/*
 *	PROGRAM:	Data Definition Utility
 *	MODULE:		parse_proto.h
 *	DESCRIPTION:	Prototype header file for parse.cpp
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

#ifndef DUDLEY_PARSE_PROTO_H
#define DUDLEY_PARSE_PROTO_H

#include "../dudley/parse.h"

void		PARSE_actions();
void		PARSE_error(USHORT, const TEXT*, const TEXT*);
void		PARSE_error(USHORT, int, int = 0);
FUNC		PARSE_function(int);
enum kwwords PARSE_keyword();
DUDLEY_NOD	PARSE_make_list(dudley_lls*);
DUDLEY_NOD	PARSE_make_node(enum nod_t, USHORT);
bool		PARSE_match(enum kwwords);
int			PARSE_number();
DUDLEY_REL	PARSE_relation();
SYM			PARSE_symbol(enum tok_t);

#endif // DUDLEY_PARSE_PROTO_H

