//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file
//
// Author:  Bill Watkins
//
// Description: //
//----------------------------------------------------------------------------
// $Id: ossimPngPluginInit.cpp 11046 2007-05-25 18:03:03Z gpotts $
#include "ossimCsmSupport.h"
#include <ossim/base/ossimEnvironmentUtility.h>
#include <ossim/base/ossimPreferences.h>
#include <iostream>
ossimCsmSupport* ossimCsmSupport::m_instance=0;

ossimCsmSupport::ossimCsmSupport()
{
   m_instance = this;
}

ossimCsmSupport* ossimCsmSupport::instance()
{
   if(!m_instance)
   {
      m_instance = new ossimCsmSupport();
      m_instance->init();
   }
   
   return m_instance;
}

const ossimFilename& ossimCsmSupport::csmPluginDirectory()
{
   return m_pluginDirectory;
}

void ossimCsmSupport::init()
{
   m_pluginDirectory = ossimFilename(ossimPreferences::instance()->findPreference("csm_plugin_path"));

   if(m_pluginDirectory.empty() || !m_pluginDirectory.exists())
   {
      m_pluginDirectory = ossimFilename(ossimEnvironmentUtility::instance()->getEnvironmentVariable("CSM_PLUGIN_PATH"));
   }
}
