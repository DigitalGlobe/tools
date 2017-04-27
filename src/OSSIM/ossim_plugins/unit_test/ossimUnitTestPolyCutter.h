#ifndef ossimUnitTestPolyCutter_HEADER
#define ossimUnitTestPolyCutter_HEADER
#include <ossim/imaging/ossimPolyCutter.h>

class ossimUnitTestPolyCutter : public ossimPolyCutter
{
public:
   ossimUnitTestPolyCutter();
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
protected:
   void recompute();
   
   ossim_uint32 theSeed;
   ossim_uint32 theNumberOfPoints;
   
   
   
TYPE_DATA
};

#endif
