/*
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
 *  The Original Code was created by Dmitry Yemanov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2002 Dmitry Yemanov <dimitr@users.sf.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *  Adriano dos Santos Fernandes
 */

#include "firebird.h"

#include <windows.h>

#include "fb_types.h"
#include "../../../common/classes/fb_string.h"
#include "../../../common/dllinst.h"

#include "../jrd/os/config_root.h"
#include "../utilities/install/registry.h"
#include "../utilities/install/install_nt.h"

using Firebird::PathName;


/******************************************************************************
 *
 *	Platform-specific root locator
 */
namespace {

bool getRootFromRegistry(PathName& root)
{
	HKEY hkey;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, REG_KEY_ROOT_INSTANCES,
			0, KEY_QUERY_VALUE, &hkey) != ERROR_SUCCESS)
	{
		return false;
	}

	DWORD bufsize = MAXPATHLEN;
	char buffer[MAXPATHLEN];
	DWORD type;
	const long RegRC = RegQueryValueEx(hkey, FB_DEFAULT_INSTANCE,
		NULL, &type, reinterpret_cast<UCHAR*>(buffer), &bufsize);
	RegCloseKey(hkey);
	if (RegRC == ERROR_SUCCESS) {
		root = buffer;
		return true;
	}
	return false;
}


bool getBinFromHInstance(PathName& root)
{
	const HINSTANCE hDllInst = Firebird::hDllInst;
	if (!hDllInst)
	{
		return false;
	}

	char* filename = root.getBuffer(MAX_PATH);
	GetModuleFileName(hDllInst, filename, MAX_PATH);

	root.recalculate_length();

	return root.hasData();
}

} // namespace


void ConfigRoot::osConfigRoot()
{
	// check the registry first
#if defined(SUPERCLIENT)
	if (getRootFromRegistry(root_dir))
	{
		addSlash();
		return;
	}
#endif

	// get the pathname of the running dll / executable
	PathName bin_dir;
	if (!getBinFromHInstance(bin_dir))
	{
		bin_dir = fb_utils::get_process_name();
	}

	if (bin_dir.hasData())
	{
		// get rid of the filename
		size_t index = bin_dir.rfind(PathUtils::dir_sep);
		root_dir = bin_dir.substr(0, index);

		// how should we decide to use bin_dir instead of root_dir? any ideas?
		// ???
#ifndef EMBEDDED
		// go to the parent directory
		index = root_dir.rfind(PathUtils::dir_sep, root_dir.length());
		if (index)
		{
			root_dir = root_dir.substr(0, index);
		}
#endif
		root_dir += PathUtils::dir_sep;

		return;
	}

	// As a last resort get it from the default install directory
	root_dir = FB_PREFIX;
}

void ConfigRoot::osConfigInstallDir()
{

	// get the pathname of the running dll / executable
	PathName bin_dir;
	if (!getBinFromHInstance(bin_dir))
	{
		bin_dir = fb_utils::get_process_name();
	}

	if (bin_dir.hasData())
	{
		// get rid of the filename
		size_t index = bin_dir.rfind(PathUtils::dir_sep);
		install_dir = bin_dir.substr(0, index);

		// how should we decide to use bin_dir instead of root_dir? any ideas?
		// ???
#ifndef EMBEDDED
		// go to the parent directory
		index = install_dir.rfind(PathUtils::dir_sep, install_dir.length());
		if (index)
		{
			install_dir = install_dir.substr(0, index);
		}
#endif

		return;
	}

	// As a last resort get it from the default install directory
	install_dir = FB_PREFIX;
}
