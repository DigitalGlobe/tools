#ifndef ossimPredatorProjectionFactory_HEADER
#define ossimPredatorProjectionFactory_HEADER
#include <ossim/projection/ossimProjectionFactoryBase.h>
#include <ossimPredator/ossimPredatorExport.h>

class OSSIMPREDATOR_DLL ossimPredatorProjectionFactory : public ossimProjectionFactoryBase
{
public:
   static ossimPredatorProjectionFactory* instance()
   {
      if(!theInstance)
      {
         theInstance = new ossimPredatorProjectionFactory;
         ossimProjectionFactoryRegistry::instance()->registerFactory(theInstance);
      }
   }
   /**
    * takes a filename. This filename can be an image file or
    * it can also be a ossim .geom file.  It could be other
    * forms of geom files as well.  The factories job will be to
    * determine what parser to use and return a projection if
    * successful.
    */
   virtual ossimProjection* createProjection(const ossimFilename& filename,
                                             ossim_uint32 entryIdx)const
   {
      return 0;
   }
   virtual ossimObject* createObject(const ossimString& typeName)const
   {
      return createProjection(typeName);
   }

   /*!
    * Creates and object given a keyword list.
    */
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const
   {
      return createProjection(kwl, prefix);
   }
   

   /**
    * Take a projection type name.
    */
   virtual ossimProjection* createProjection(const ossimString& name)const
   {
      if(name == "ossimUavProjection")
      {
         return new ossimUavProjection();
      }

      return 0;
   }

   /**
    * Take a keywordlist.
    */
   virtual ossimProjection* createProjection(const ossimKeywordlist& kwl,
                                             const char* prefix)const
   {
      const char* type = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
      ossimProjection* result = createProjection(ossimString(type));
      if(result)
      {
         if(result->loadState(kwl, prefix))
         {
            return result;
         }
         delete result;
         result = 0;
      }
      return result;
   }
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const
   {
      typeList.push_back("ossimUavProjection");
   }
protected:
   static ossimPredatorProjectionFactory* theInstance;
};
#endif //ossimPredatorProjectionFactory_HEADER
