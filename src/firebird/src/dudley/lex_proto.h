/*
 *	PROGRAM:	Data Definition Utility
 *	MODULE:		lex_proto.h
 *	DESCRIPTION:	Prototype header file for lex.cpp
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

#ifndef DUDLEY_LEX_PROTO_H
#define DUDLEY_LEX_PROTO_H

tok*	LEX_filename();
void	LEX_fini();
void	LEX_flush();
void	LEX_get_text(UCHAR*, TXT);
void	LEX_init(void*);
void	LEX_put_text(FB_API_HANDLE, TXT);
void	LEX_real();
tok*	LEX_token();

#endif // DUDLEY_LEX_PROTO_H

