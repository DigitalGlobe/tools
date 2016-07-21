#ifndef ossimUnitTestImageSourceFactory_HEADER
#define ossimUnitTestImageSourceFactory_HEADER
#include <ossim/imaging/ossimImageSourceFactoryBase.h>

class ossimUnitTestImageSourceFactory : public ossimImageSourceFactoryBase
{
public:
   virtual ~ossimUnitTestImageSourceFactory();
   static ossimUnitTestImageSourceFactory* instance();
   virtual ossimObject* createObject(const ossimString& name)const;
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
protected:
   // Hide from use.
   ossimUnitTestImageSourceFactory();
   ossimUnitTestImageSourceFactory(const ossimUnitTestImageSourceFactory&);
   const ossimUnitTestImageSourceFactory& operator=(ossimUnitTestImageSourceFactory&);

   static ossimUnitTestImageSourceFactory* theInstance;
TYPE_DATA};
#endif
