#ifndef ossimRegistrationImageSourceFactory_HEADER
#define ossimRegistrationImageSourceFactory_HEADER
#include <ossim/imaging/ossimImageSourceFactoryBase.h>

class ossimRegistrationImageSourceFactory : public ossimImageSourceFactoryBase
{
public:
   virtual ~ossimRegistrationImageSourceFactory();
   static ossimRegistrationImageSourceFactory* instance();
   virtual ossimObject* createObject(const ossimString& name)const;
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
   
protected:
   // Hide from use.
   ossimRegistrationImageSourceFactory();
   ossimRegistrationImageSourceFactory(const ossimRegistrationImageSourceFactory&);
   const ossimRegistrationImageSourceFactory& operator=(ossimRegistrationImageSourceFactory&);

   static ossimRegistrationImageSourceFactory* theInstance;
TYPE_DATA
};

#endif
