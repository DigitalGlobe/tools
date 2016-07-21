//*******************************************************************
// Copyright (C) 2009 Garrett Potts.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//*******************************************************************
//  $Id$

#ifndef ossimNitroImageHandlerFactory_HEADER
#define ossimNitroImageHandlerFactory_HEADER

#include <ossim/imaging/ossimImageHandlerFactoryBase.h>
#include <ossim/base/ossimString.h>

class ossimGdal;
class ossimFilename;
class ossimKeywordlist;

//*******************************************************************
// CLASS:  ossimNitroImageHandlerFactory
//*******************************************************************
class ossimNitroImageHandlerFactory : public ossimImageHandlerFactoryBase
{
public:
   virtual ~ossimNitroImageHandlerFactory();
   static ossimNitroImageHandlerFactory* instance();
   
   virtual ossimImageHandler* open(const ossimFilename& fileName)const;
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
   virtual void getSupportedExtensions(ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const;
  
protected:
   ossimNitroImageHandlerFactory(){}
   ossimNitroImageHandlerFactory(const ossimNitroImageHandlerFactory&){}
   void operator = (const ossimNitroImageHandlerFactory&){}

   static ossimNitroImageHandlerFactory* theInstance;

TYPE_DATA
};

#endif

