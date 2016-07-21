#ifndef ossimPredatorInit_HEADER
#define ossimPredatorInit_HEADER
#include <ossimPredator/ossimPredatorExport.h>
#include <OpenThreads/Mutex>
class OSSIMPREDATOR_DLL ossimPredatorInit
{
public:
   static ossimPredatorInit* instance();
   OpenThreads::Mutex&  criticalSectionMutex()
   {
      return theCriticalSectionMutex;
   }
   
protected:
   ossimPredatorInit();
   ossimPredatorInit(const ossimPredatorInit& src);

   OpenThreads::Mutex theCriticalSectionMutex;
   static ossimPredatorInit* theInstance;
};

#endif
