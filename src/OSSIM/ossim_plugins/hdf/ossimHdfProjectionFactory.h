//----------------------------------------------------------------------------
// License: MIT
//
// See LICENSE.txt file in the top level directory for more details.
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimHdfProjectionFactory_HEADER
#define ossimHdfProjectionFactory_HEADER 1

#include <ossimPluginConstants.h>
#include <ossim/projection/ossimProjectionFactoryBase.h>

class OSSIM_PLUGINS_DLL ossimHdfProjectionFactory : public ossimProjectionFactoryBase
{
public:

   static ossimHdfProjectionFactory* instance();

   /**
    * @bief Takes a filename. This filename can be an image file or
    * it can also be a ossim .geom file.  It could be other
    * forms of geom files as well.  The factories job will be to
    * determine what parser to use and return a projection if
    * successful.
    */
   virtual ossimProjection* createProjection(const ossimFilename& filename,
                                             ossim_uint32 entryIdx)const;
   
   /** @brief Take a projection type name. */
   virtual ossimProjection* createProjection(const ossimString& name)const;

   /** @brief Take a keywordlist. */
   virtual ossimProjection* createProjection(const ossimKeywordlist& kwl,
                                             const char* prefix=0)const;
   
   virtual ossimObject* createObject(const ossimString& typeName)const;
   
   /*!
    * Creates and object given a keyword list.
    */
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;

protected:
   
   ossimHdfProjectionFactory();
   
}; // End: class ossimHdfProjectionFactory{ ... }

#endif /* #ifndef ossimHdfProjectionFactory_HEADER */

