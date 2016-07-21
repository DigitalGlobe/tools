#ifndef ossimContribImageSourceFactory_HEADER
#define ossimContribImageSourceFactory_HEADER
#include <ossim/imaging/ossimImageSourceFactoryBase.h>

class ossimContribImageSourceFactory : public ossimImageSourceFactoryBase
{
public:
   ossimContribImageSourceFactory()
   {
      theInstance = this;
   }
   virtual ~ossimContribImageSourceFactory();
   
   static ossimContribImageSourceFactory* instance();
   virtual ossimObject* createObject(const ossimString& typeName)const;
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
protected:
   static ossimContribImageSourceFactory* theInstance;

TYPE_DATA
};


#endif
