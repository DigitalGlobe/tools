/*
 *	PROGRAM:		Firebird utilities
 *	MODULE:			burpMain.cpp
 *	DESCRIPTION:	Proxy for real gbak main function
 *
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Alex Peshkov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2007 Alex Peshkov <peshkoff at mail dot ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#include "firebird.h"
#include "../burp/burp_proto.h"
#include "../common/classes/auto.h"
#include "../jrd/ibase.h"


static int shutdownCallback(const int reason, const int, void*)
{
	static bool recursion = false;

	if (!recursion)
	{
		recursion = true;
		fb_shutdown(0, reason);
		recursion = false;
		return FB_FAILURE;
	}
	return FB_SUCCESS;
}


int CLIB_ROUTINE main( int argc, char* argv[])
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	Invoke real gbak main function
 *
 **************************************/
	fb_shutdown_callback(NULL, shutdownCallback, fb_shut_confirmation, NULL);

	Firebird::AutoPtr<Firebird::UtilSvc> uSvc(Firebird::UtilSvc::createStandalone(argc, argv));
 	return gbak(uSvc);
}
