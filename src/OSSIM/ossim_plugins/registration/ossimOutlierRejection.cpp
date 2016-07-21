
#include "ossimOutlierRejection.h"
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimEcefVector.h>
#include <ossim/projection/ossimOptimizableProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/base/ossimFilenameProperty.h>
#include <vector>
#include <algorithm>
#include <cmath>

RTTI_DEF1(ossimOutlierRejection, "ossimOutlierRejection", ossimModelOptimizer);

ossimOutlierRejection::ossimOutlierRejection() : 
       ossimModelOptimizer(),
       theInlierRatio(0.6),
       theInlierImageAccuracy(1.0),
       theInlierOutputFilename("inlier.xml")
{
   //change default name to avoid confusion
   setGeomOutputFilename("rejout.xml");
}

bool
ossimOutlierRejection::saveGMLTieSet(const ossimString& filepath)
{
   //create a XML document
   ossimXmlDocument gmlDoc;
   
   gmlDoc.initRoot(theTieSet.exportAsGmlNode());

   bool res = gmlDoc.write(filepath);
   if (!res) {
      ossimNotify(ossimNotifyLevel_WARN) << 
         "WARNING: ossimOutlierRejection::loadGMLTieSet cannot save tie points as file "<<filepath<<"\n";
      return false;
   }

   return true;
}

/*!
 * removeOutliers(): use RANdom SAmple Consensus method (RANSAC)
 * but stops at the first good fit, without looking for better fits
 */
bool
ossimOutlierRejection::removeOutliers(ossim_float64* result_var_pix2,
                                      ossim_float64* target_var_pix2)
{
   //init optional variance to error
   if (result_var_pix2!=NULL) *result_var_pix2 = -1.0;

   if(!theModel.valid()) return false;
   
   static const double SUCCESS = 0.97; //required chance of success for RANSAC
   static const double NSTD    = 2.2;  //deviations allowed around mean
   //check args
   if ((theInlierRatio<=0.0) || (theInlierRatio>1.0))
   {
      ossimNotify(ossimNotifyLevel_WARN) << 
         "WARNING: ossimOutlierRejection::removeOutliers() bad theInlierRatio\n";
      return false;
   }
   ossimOptimizableProjection* optProj = PTR_CAST(ossimOptimizableProjection, theModel.get());
   if(!optProj) return false;
   
   //direction of transform (cached)
   bool useForward = optProj->useForward();

   //keep a copy of projection if it needs initialization
   ossimKeywordlist initialState;
   bool nis=optProj->needsInitialState(); //cached
   if (nis)
   {
      //save state //TBD: make the copy/restore faster
      if (!theModel->saveState(initialState))
      {
         ossimNotify(ossimNotifyLevel_WARN) << 
         "WARNING: ossimOutlierRejection::removeOutliers() can't save projection initial state\n";
      }
   }

   // convert error to ground if necessary
   double imageToGroundRatio;
   if (useForward)
   {
      imageToGroundRatio = 1.0;
   } else {
      //convert image error to ground error (meters)
      imageToGroundRatio = theModel->getMetersPerPixel().length()/std::sqrt(2.0);
   }
   double maxFitError = imageToGroundRatio * theInlierImageAccuracy; //either pixels or meters

   unsigned long dof = optProj->degreesOfFreedom();
   unsigned long dofPerPoint = 2L; //use 2 because only 2 uncorelated equations (height is correlated)
   unsigned long miniPts = (dof+ dofPerPoint-1) / dofPerPoint; //TBD: shouldn't be lower than nbsamples
   unsigned long miniInliers = (unsigned long)ceil(theTieSet.size()*theInlierRatio);

   //calculate maximum number of iterations (average and std)
   double probaAllGood = std::pow(theInlierRatio, (int)miniPts);
   double avg_iter_max = std::log(SUCCESS) / std::log(1.0-probaAllGood);
   double std_iter_max = std::sqrt(1.0-probaAllGood) / probaAllGood;
   long iter_max = (long)ceil(avg_iter_max + NSTD * std_iter_max);
   ossimNotify(ossimNotifyLevel_WARN) << 
         "INFO: max number of RANSAC iterations = "<<iter_max<<"\n"; //TBR

   unsigned long nbsamples = theTieSet.size();

   ossimNotify(ossimNotifyLevel_WARN) << 
        "INFO: samples before RANSAC = "<<nbsamples<<"\n"; //TBR

   //array for random draws : init on identity permutation
   //high memory cost?
   vector<unsigned long> vshuf(nbsamples);

   //init random / inliers tie point set from current tie set, but with no ties
   ossimTieGptSet randSelection;
   randSelection.setMasterPath(theTieSet.getMasterPath());
   randSelection.setSlavePath(theTieSet.getSlavePath());
   randSelection.setImageCov(theTieSet.getImageCov());
   randSelection.setGroundCov(theTieSet.getGroundCov());
   double bestfit;

   long iterations = 0;
   unsigned long npos=0;
   while(iterations < iter_max)
   {

      //TBD: find out why we need to reset that index table at every iteration
      for(unsigned long c=0; c<nbsamples; ++c) vshuf[c]=c;

      //randomly select miniPts indices
      for(unsigned long s=0;s<miniPts;++s)
      {
         unsigned long rpick = s+(unsigned long)(((double)std::rand())/(1.0+RAND_MAX)*(nbsamples-s)); //between 0 and nbsamples-1 inc.
         vshuf[s]     = rpick;
         vshuf[rpick] = s;
      }

      //use miniPts random tie points for selection
      randSelection.clearTiePoints();
      for(unsigned long c=0;c<miniPts;++c) randSelection.addTiePoint( theTieSet.getTiePoints()[vshuf[c]] );
      
      //optimize model with random selection
      double fitvar = optProj->optimizeFit(randSelection, NULL); //best possible fit, do not use target variance here

      //if optimization goes fine, then test it further
      if ((fitvar>=0) && (fitvar <= maxFitError*maxFitError)) //RMS must be below max distance
      {
         //find other compatible points (and enqueue them)
         npos = miniPts;
         double perr;
         for(unsigned long c=miniPts; c<nbsamples; ++c) 
         {
            ossimRefPtr< ossimTieGpt > rt = theTieSet.getTiePoints()[vshuf[c]];
            if (useForward)
            {
               perr = (theModel->forward(*rt) - rt->tie).length(); //image error, unit pixels
            } else {
               perr = (ossimEcefPoint(theModel->inverse(rt->tie)) - ossimEcefPoint(*rt)).magnitude(); //ground error, unit meters
            }
            if (perr <= maxFitError)
            {
               //keep the indices after the miniPts first elts
               vshuf[npos] = c;
               vshuf[c]    = npos;
               ++npos;
            }
         }

         //DEBUG TBR
         ossimNotify(ossimNotifyLevel_WARN) << 
            "INFO: model finds "<<100.0*npos/((double)nbsamples)<<"% inliers\n"; //TBR

         if (npos >= miniInliers )
         {
            //re-optimize model with the new possible inliers :
            //-add new inliers to sleection
            for(unsigned long c=miniPts;c<npos;++c) randSelection.addTiePoint( theTieSet.getTiePoints()[vshuf[c]] );

            //use target variance
            if (target_var_pix2!=NULL)
            {
               ossim_float64 target_var = (*target_var_pix2) * imageToGroundRatio * imageToGroundRatio;
               bestfit = optProj->optimizeFit(randSelection, &target_var); //best possible fit with target variance
            } else {
               bestfit = optProj->optimizeFit(randSelection); //best possible fit
            }
            //assume fit to be OK (which might NOT be the case...)
            //TBD iterate over minimal number of times
            break;
         }
      }

      if (nis)
      {
         //restore initial state //TBD: make the copy/restore faster
         if (!theModel->loadState(initialState))
         {
            ossimNotify(ossimNotifyLevel_WARN) << 
            "WARNING: ossimOutlierRejection::removeOutliers() can't reload projection initial state at iteration "<<iterations<<"\n";
         }
      }

      iterations++;
   }
   if (iterations >= iter_max) 
   {
      ossimNotify(ossimNotifyLevel_WARN) << 
         "WARNING: RANSAC didn't find a solution after "<<iterations<<" iterations"<<endl;
      return false;
   }

   //keep inliers
   theTieSet = randSelection;
   //display results, with RMS always in pixels
   ossimNotify(ossimNotifyLevel_WARN) << 
         "INFO: RANSAC solution found after "<<iterations<<" iterations, nbinliers="<<npos<<endl;

   //convert to pixel2 variance
   ossim_float64 variance_pix2 = bestfit / (imageToGroundRatio * imageToGroundRatio);
   //normal exit : return optional variance
   if (result_var_pix2!=NULL) *result_var_pix2 = variance_pix2;

   return true;
}

void
ossimOutlierRejection::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property.valid()) return;
   
   ossimString name = property->getName();

   if(name == "inlier_output_filename")
   {
      theInlierOutputFilename = ossimFilename(property->valueToString());
   }
   else if(name =="inlier_ratio")
   {
      setInlierRatio(property->valueToString().toDouble());
   }
   else if(name == "inlier_image_accuracy")
   {
      setInlierImageAccuracy(property->valueToString().toDouble());
   }
   else {
      ossimModelOptimizer::setProperty(property);
   }
}

ossimRefPtr<ossimProperty>
ossimOutlierRejection::getProperty(const ossimString& name)const
{
   if(name == "inlier_ratio")
   {
      return new ossimNumericProperty(name, ossimString::toString(theInlierRatio), 0.0, 1.0);
   }
   else if(name == "inlier_image_accuracy")
   {
      return new ossimNumericProperty(name, ossimString::toString(theInlierImageAccuracy), 0.0, 1e50);
   }
   else if(name == "inlier_output_filename")
   {
      return new ossimFilenameProperty(name,theInlierOutputFilename); 
   }

   return ossimModelOptimizer::getProperty(name);
}

void
ossimOutlierRejection::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimModelOptimizer::getPropertyNames(propertyNames);

   propertyNames.push_back("inlier_ratio");
   propertyNames.push_back("inlier_image_accuracy");
   propertyNames.push_back("inlier_output_filename");
}

bool
ossimOutlierRejection::execute()
{
   if(!setupModel(theModelDefinitionString))
   {
      return false;
   }
   ossim_float64 variance_pix2;
   if(removeOutliers(&variance_pix2, NULL)) //TBD : use target variance
   {
      //display RMS with unit
      cout<<"RMS="<<std::sqrt(variance_pix2)<<" pixels"<<endl;

      //export results
      if(theGeomOutputFilename!="")
      {
         if(!exportModel(theGeomOutputFilename)) return false;
      }
      if(theInlierOutputFilename!="")
      {
         if(!saveGMLTieSet(theInlierOutputFilename)) return false;
      }
   }
   else
   {
      return false;
   }

   return true;
}
