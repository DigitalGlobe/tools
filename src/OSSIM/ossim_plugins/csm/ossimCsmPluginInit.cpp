//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file
//
// Author:  Bill Watkins
//
// Description: //
//----------------------------------------------------------------------------
// $Id: ossimPngPluginInit.cpp 11046 2007-05-25 18:03:03Z gpotts $
#include <ossim/plugin/ossimSharedObjectBridge.h>
#include "CSMSensorModelLoader.h"

#include "ossimPluginConstants.h"
#include "ossimCsmProjectionFactory.h"
#include "ossimCsmSupport.h"
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <vector>
#include <sstream>

static void setDescription(ossimString& description)
{
   std::string pError;
   ossimFilename pluginDir = ossimCsmSupport::instance()->csmPluginDirectory();
   
   if (!pluginDir.empty()) 
   {
      if(*(pluginDir.begin()+(pluginDir.size()-1)) != pluginDir.getPathSeparator())
      {
         pluginDir = ossimFilename(pluginDir + pluginDir.getPathSeparator());
      }
   }
   std::vector<string> pluginNames;
   if(!pluginDir.empty()&&pluginDir.exists())
   {
      pluginNames = CSMSensorModelLoader::getAvailablePluginNames(pluginDir.c_str(), pError );
   }

   std::ostringstream out;
   out  << "Community Sensor Model Plugin\n" << "\nAvailable plugins are: \n\n";
      	 
   if(!pError.empty())
   {
      out << "ERROR loading! Result = " << pError << "\n";
      description = out.str();
      return;
   }
   if(pluginNames.empty())
   {
      out << "No CSM plugins available.  Please make sure your ossim preference variable csm_plugin_path\n"
          << " is set or an environment variable CSM_PLUGIN_PATH is set\n";
   }
   ossim_uint32 idx = 0;
   for(idx = 0; idx < pluginNames.size(); ++idx)
   {
      out << "\t" << pluginNames[idx] << " , supported models:\n";
      std::vector<string> sensorModelNames = CSMSensorModelLoader::getAvailableSensorModelNames( pluginDir, pluginNames[idx].c_str(),  pError );    
      ossim_uint32 idx2=0;
      for(idx2 = 0; idx2 < sensorModelNames.size(); ++idx2)
      { 
         out << "\t\t" << sensorModelNames[idx2] << "\n";
      }       
   }
   description = out.str();
}


extern "C"
{
   ossimSharedObjectInfo  myInfo;
   ossimString theDescription;
   std::vector<ossimString> theObjList;

   const char* getDescription()
   {
      return theDescription.c_str();
   }

   int getNumberOfClassNames()
   {
      return (int)theObjList.size();
   }

   const char* getClassName(int idx)
   {
      if(idx < (int)theObjList.size())
      {
         return theObjList[0].c_str();
      }
      return (const char*)0;
   }

   /* Note symbols need to be exported on windoze... */ 
   OSSIM_PLUGINS_DLL void ossimSharedLibraryInitialize(
      ossimSharedObjectInfo** info, 
      const char* /* options */ )
   {    
      myInfo.getDescription = getDescription;
      myInfo.getNumberOfClassNames = getNumberOfClassNames;
      myInfo.getClassName = getClassName;
      
      *info = &myInfo;
      /* Register the readers... */
      ossimProjectionFactoryRegistry::instance()->
        registerFactoryToFront(ossimCsmProjectionFactory::instance());
 
      setDescription(theDescription);

   }

   /* Note symbols need to be exported on windoze... */ 
   OSSIM_PLUGINS_DLL void ossimSharedLibraryFinalize()
   {
      ossimProjectionFactoryRegistry::instance()->
         unregisterFactory(ossimCsmProjectionFactory::instance());
   }
}
