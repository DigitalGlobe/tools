/*
 *	PROGRAM:		Firebird interface.
 *	MODULE:			GetPlugins.h
 *	DESCRIPTION:	Tools to help access plugins.
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
 *  Copyright (c) 2010 Alex Peshkov <peshkoff at mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#ifndef FB_COMMON_CLASSES_GET_PLUGINS
#define FB_COMMON_CLASSES_GET_PLUGINS

#include "../common/classes/ImplementHelper.h"
#include "../common/classes/auto.h"
#include "../common/config/config.h"
#include "../common/StatusHolder.h"
#include "../common/classes/fb_string.h"

namespace Firebird {

// Template to help with loop in the set of plugins
template <typename P>
class GetPlugins
{
public:
	GetPlugins(unsigned int iType, const char* namesList = NULL)
		: pluginList(*getDefaultMemoryPool()),
		  masterInterface(), pluginInterface(),
		  currentPlugin(NULL),
		  ls(*getDefaultMemoryPool()), status(&ls),
		  interfaceType(iType)
	{
		pluginList = namesList ? namesList : Config::getDefaultConfig()->getPlugins(interfaceType);
		pluginSet.assignRefNoIncr(pluginInterface->getPlugins(&status, interfaceType,
			pluginList.c_str(), NULL));
		check(&status);

		getPlugin();
	}

	GetPlugins(unsigned int iType,
			   const Config* conf, const char* namesList = NULL)
		: pluginList(*getDefaultMemoryPool()),
		  masterInterface(), pluginInterface(),
		  knownConfig(conf), currentPlugin(NULL),
		  ls(*getDefaultMemoryPool()), status(&ls),
		  interfaceType(iType)
	{
		pluginList = namesList ? namesList : knownConfig->getPlugins(interfaceType);
		pluginSet.assignRefNoIncr(pluginInterface->getPlugins(&status, interfaceType,
			pluginList.c_str(), FB_NEW FirebirdConf(knownConfig)));
		check(&status);

		getPlugin();
	}

	bool hasData() const
	{
		return currentPlugin;
	}

	const char* name() const
	{
		return hasData() ? pluginSet->getName() : NULL;
	}

	P* plugin() const
	{
		return currentPlugin;
	}

	P* makeInstance()
	{
		if (!hasData())
			return NULL;

		P* p = (P*) pluginSet->getPlugin(&status);
		check(&status);
		return p;
	}

	void next()
	{
		if (hasData())
		{
			removePlugin();

			pluginSet->next(&status);
			check(&status);
			getPlugin();
		}
	}

	void set(const char* newName)
	{
		removePlugin();

		pluginList = newName;
		pluginSet->set(&status, pluginList.c_str());
		check(&status);

		getPlugin();
	}

	void set(const Config* conf)
	{
		removePlugin();

		knownConfig = conf;
		pluginList = knownConfig->getPlugins(interfaceType);
		pluginSet.assignRefNoIncr(pluginInterface->getPlugins(&status, interfaceType,
			pluginList.c_str(), FB_NEW FirebirdConf(knownConfig)));
		check(&status);

		getPlugin();
	}

	void rewind()
	{
		removePlugin();

		pluginSet.assignRefNoIncr(pluginInterface->getPlugins(&status, interfaceType,
			pluginList.c_str(), knownConfig.hasData() ? FB_NEW FirebirdConf(knownConfig) : NULL));
		check(&status);

		getPlugin();
	}

	~GetPlugins()
	{
		removePlugin();
	}

private:
	PathName pluginList;
	MasterInterfacePtr masterInterface;
	PluginManagerInterfacePtr pluginInterface;
	RefPtr<const Config> knownConfig;
	RefPtr<IPluginSet> pluginSet;
	P* currentPlugin;
	LocalStatus ls;
	CheckStatusWrapper status;
	unsigned interfaceType;

	void getPlugin()
	{
		currentPlugin = (P*) pluginSet->getPlugin(&status);
		check(&status);
	}

	void removePlugin()
	{
		if (hasData())
		{
			pluginInterface->releasePlugin(currentPlugin);
			currentPlugin = NULL;
		}
	}
};

// template required to use AutoPtr for plugins

template <typename P>
class ReleasePlugin
{
public:
	static void clear(P* ptr)
	{
		if (ptr)
			PluginManagerInterfacePtr()->releasePlugin(ptr);
	}
};

template <typename P>
class AutoPlugin : public AutoPtr<P, ReleasePlugin>
{
public:
	AutoPlugin(P* p = nullptr)
		: AutoPtr<P, ReleasePlugin>(p)
	{ }
};

} // namespace Firebird


#endif // FB_COMMON_CLASSES_GET_PLUGINS
