/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		dbg.h
 *	DESCRIPTION:	Debugging symbol table definition
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

#ifndef JRD_DBG_H
#define JRD_DBG_H

enum s_type {
	symb_routine,
	symb_offset,
	symb_printer,
	symb_absolute
};

struct symb
{
	char *symb_string;
	int (**symb_value) ();
	s_type symb_type;
	short symb_size;
};

#endif // JRD_DBG_H

