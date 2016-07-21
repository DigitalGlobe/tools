#include <oms/AdjustmentModel.h>
#include <ossim/base/ossimObservationSet.h>
#include <ossim/base/ossimAdjustmentExecutive.h>
#include <ossim/base/ossimReferenced.h>


class oms::AdjustmentModel::PrivateData
{
public:
   PrivateData()
   {}

   virtual ~PrivateData()
   {
       m_AdjExec = 0;
       m_ObsSet = 0;
   }

   ossimRefPtr<ossimAdjustmentExecutive> m_AdjExec;
   ossimRefPtr<ossimObservationSet> m_ObsSet;
   std::ofstream m_RepFile;
};


oms::AdjustmentModel::AdjustmentModel(const std::string& report) :
   thePrivateData(new PrivateData())
{
   thePrivateData->m_ObsSet = new ossimObservationSet();
   
   if (report == "cout")
   {
      thePrivateData->m_AdjExec = new ossimAdjustmentExecutive(std::cout);
   }
   else
   {
      thePrivateData->m_RepFile.open(report.data());
      thePrivateData->m_AdjExec = 
         new ossimAdjustmentExecutive(thePrivateData->m_RepFile);
   }
}


oms::AdjustmentModel::~AdjustmentModel()
{
   if(thePrivateData)
   {
      if (thePrivateData->m_RepFile.is_open())
         thePrivateData->m_RepFile.close();
      delete thePrivateData;
      thePrivateData = 0;
   }
}


bool oms::AdjustmentModel::addMeasurement(const std::string& idm,
                                          const ossimDpt& iPt,
                                          const std::string& imgFile)
{
   bool addOK = false;  

   for (ossim_uint32 i=0; i<thePrivateData->m_ObsSet->numObs(); ++i)
   {
      if (thePrivateData->m_ObsSet->observ(i)->ID() == idm)
      {
         thePrivateData->m_ObsSet->observ(i)->addMeasurement(iPt, imgFile);
         addOK = true;
      }
   }

   return addOK;
}


bool oms::AdjustmentModel::addObservation(ossimPointObservation& obs)
{
   bool addOK  = false;

   addOK = thePrivateData->m_ObsSet->addObservation(new ossimPointObservation(obs));

   return addOK;
}


bool oms::AdjustmentModel::initAdjustment()
{
   bool initOK = false;

   initOK = thePrivateData->m_AdjExec->initializeSolution(*thePrivateData->m_ObsSet);

   if(!initOK)
   {
      delete thePrivateData;
      thePrivateData = 0;
   }
   return initOK;
}


bool oms::AdjustmentModel::runAdjustment()
{
   bool runOK  = false;

   // Perform solution
   runOK = thePrivateData->m_AdjExec->runSolution();

   // Report results
   if (runOK)
      thePrivateData->m_AdjExec->summarizeSolution();

   return runOK;
}


bool oms::AdjustmentModel::isValid()
{
   return thePrivateData->m_AdjExec->isValid();
}
