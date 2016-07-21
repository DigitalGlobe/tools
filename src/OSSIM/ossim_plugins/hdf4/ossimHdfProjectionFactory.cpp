//----------------------------------------------------------------------------
// License: MIT
//
// See LICENSE.txt file in the top level directory for more details.
//----------------------------------------------------------------------------
// $Id$

#include "ossimHdfProjectionFactory.h"
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/projection/ossimProjection.h>
#include "ossimHdfGridModel.h"

//---
// Define Trace flags for use within this file:
//---
#include <ossim/base/ossimTrace.h>
static ossimTrace traceExec  = ossimTrace("ossimHdfProjectionFactory:exec");
static ossimTrace traceDebug = ossimTrace("ossimHdfProjectionFactory:debug");

ossimHdfProjectionFactory* ossimHdfProjectionFactory::instance()
{
   static ossimHdfProjectionFactory* factoryInstance =
      new ossimHdfProjectionFactory();

   return factoryInstance;
}
   
ossimProjection* ossimHdfProjectionFactory::createProjection(
   const ossimFilename& filename, ossim_uint32 /*entryIdx*/)const
{
   static const char MODULE[] = "ossimHdfProjectionFactory::createProjection(ossimFilename& filename)";
   ossimRefPtr<ossimProjection> projection = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG: testing ossimHdfGridModel" << std::endl;
   }

   ossimKeywordlist kwl;
   ossimRefPtr<ossimProjection> model = 0;
   ossimFilename geomFile = filename;
   geomFile = geomFile.setExtension("geom");
   
   if(geomFile.exists()&&
      kwl.addFile(filename.c_str()))
   {
      ossimFilename coarseGrid;
      
      const char* type = kwl.find(ossimKeywordNames::TYPE_KW);
      if(type)
      {
         if( ossimString(type) == ossimString("ossimHdfGridModel") )
         {
            ossimFilename coarseGrid = geomFile;
            geomFile.setFile(coarseGrid.fileNoExtension()+"_ocg");
            
            if(coarseGrid.exists() &&(coarseGrid != ""))
            {
               kwl.add("grid_file_name",
                       coarseGrid.c_str(),
                       true);
               projection = new ossimHdfGridModel();
               if ( projection->loadState( kwl ) == false )
               {
                  projection = 0;
               }
            }
         }
      }
   }

   // Must release or pointer will self destruct when it goes out of scope.
   return projection.release();
}

ossimProjection* ossimHdfProjectionFactory::createProjection(
   const ossimString& name)const
{
   static const char MODULE[] = "ossimHdfProjectionFactory::createProjection(ossimString& name)";

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG: Entering ...." << std::endl;
   }

   if ( name == "ossimHdfGridModel" )
   {
      return new ossimHdfGridModel();
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
           << MODULE << " DEBUG: Leaving ...." << std::endl;
   }

   return 0;
}

ossimProjection* ossimHdfProjectionFactory::createProjection(
   const ossimKeywordlist& kwl, const char* prefix)const
{
   ossimRefPtr<ossimProjection> result = 0;
   static const char MODULE[] = "ossimHdfProjectionFactory::createProjection(ossimKeywordlist& kwl)";

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " DEBUG: Start ...." << std::endl;
   }
   
   const char* lookup = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   if (lookup)
   {
      ossimString type = lookup;
      
      if (type == "ossimHdfGridModel")
      {
         result = new ossimHdfGridModel();
         if ( !result->loadState(kwl, prefix) )
         {
            result = 0;
         }
      }
   }

   if(traceDebug())
   {
    	ossimNotify(ossimNotifyLevel_DEBUG)
        	   << MODULE << " DEBUG: End ...." << std::endl;
   }
   
   return result.release();
}

ossimObject* ossimHdfProjectionFactory::createObject(
   const ossimString& typeName)const
{
   return createProjection(typeName);
}

ossimObject* ossimHdfProjectionFactory::createObject(
   const ossimKeywordlist& kwl, const char* prefix)const
{
   return createProjection(kwl, prefix);
}


void ossimHdfProjectionFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(ossimString("ossimHdfGridModel"));
}

ossimHdfProjectionFactory::ossimHdfProjectionFactory()
{
}

