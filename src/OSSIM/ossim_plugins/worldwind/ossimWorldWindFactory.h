//*******************************************************************
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//*******************************************************************
//  $Id: ossimWorldWindFactory.h 23664 2015-12-14 14:17:27Z dburken $

#ifndef ossimWorldWindFactory_HEADER
#define ossimWorldWindFactory_HEADER

#include <ossim/imaging/ossimImageHandlerFactoryBase.h>
#include <ossim/base/ossimString.h>

class ossimGdal;
class ossimFilename;
class ossimKeywordlist;

//*******************************************************************
// CLASS:  ossimWorldWindFactory
//*******************************************************************
class ossimWorldWindFactory : public ossimImageHandlerFactoryBase
{
public:
   virtual ~ossimWorldWindFactory();
   static ossimWorldWindFactory* instance();
   
   virtual ossimImageHandler* open(const ossimFilename& fileName,
                                   bool openOverview=true) const;
   virtual ossimImageHandler* open(const ossimKeywordlist& kwl,
                                   const char* prefix=0)const;

   
   virtual ossimObject* createObject(const ossimString& typeName)const;
   
   /*!
    * Creates and object given a keyword list.
    */
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   
   /*!
    * This should return the type name of all objects in all factories.
    * This is the name used to construct the objects dynamially and this
    * name must be unique.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
   void getSupportedExtensions(ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const;
 
protected:
   ossimWorldWindFactory(){}
   ossimWorldWindFactory(const ossimWorldWindFactory&){}
   void operator = (const ossimWorldWindFactory&){}

   static ossimWorldWindFactory* theInstance;

TYPE_DATA
};

#endif

