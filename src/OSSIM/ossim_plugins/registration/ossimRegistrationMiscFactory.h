#ifndef ossimRegistrationMiscFactory_HEADER
#define ossimRegistrationMiscFactory_HEADER
#include "ossimRegistrationExports.h"
#include <ossim/base/ossimObjectFactory.h>

class OSSIM_REGISTRATION_DLL ossimRegistrationMiscFactory : public ossimObjectFactory
{
public:
   ossimRegistrationMiscFactory(){}
    virtual ~ossimRegistrationMiscFactory(){theInstance = 0;}
   
   static ossimRegistrationMiscFactory* instance();
   virtual ossimObject* createObject(const ossimString& typeName)const;
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   /*!
    * This should return the type name of all objects in all factories.
    * This is the name used to construct the objects dynamially and this
    * name must be unique.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
   
protected:
   static ossimRegistrationMiscFactory* theInstance;
   
TYPE_DATA

  
};

#endif
