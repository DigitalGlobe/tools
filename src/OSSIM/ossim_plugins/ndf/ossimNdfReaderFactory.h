//***
// Written by: Kenneth Melero (kmelero@sanz.com)
// ***

#ifndef ossimNdfReaderFactory_HEADER
#define ossimNdfReaderFactory_HEADER

#include <ossim/imaging/ossimImageHandlerFactoryBase.h>

class ossimString;
class ossimFilename;
class ossimKeywordlist;

class ossimNdfReaderFactory : public ossimImageHandlerFactoryBase
{
public:
   virtual ~ossimNdfReaderFactory();
   static ossimNdfReaderFactory* instance();

   /**
    * @brief open that takes a file name.
    * @param file The file to open.
    * @param openOverview If true image handler will attempt to open overview.
    * default = true 
    * @return pointer to image handler on success, NULL on failure.
    */
   virtual ossimImageHandler* open(const ossimFilename& fileName,
                                   bool openOverview=true) const;

   virtual ossimImageHandler* open(const ossimKeywordlist& kwl,
                                   const char* prefix=0)const;
   virtual ossimObject* createObject(const ossimString& typeName)const;
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
   virtual void getSupportedExtensions(
      ossimImageHandlerFactoryBase::UniqueStringList& extensionList)const;
  
protected:
   ossimNdfReaderFactory();

   ossimNdfReaderFactory(const ossimNdfReaderFactory&);
   void operator=(const ossimNdfReaderFactory&);
   static ossimNdfReaderFactory* theInstance;

TYPE_DATA
};

#endif
