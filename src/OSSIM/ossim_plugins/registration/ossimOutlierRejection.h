//*******************************************************************
//
// License: MIT
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Frederic Claudel (fclaudel@laposte.net)
//         ICT4EO,Meraka Institute,CSIR
//
// Description: generic model optimization with RANSAC, high-level interface
//
//*************************************************************************
#ifndef ossimOutlierRejection_HEADER
#define ossimOutlierRejection_HEADER

#include "ossimModelOptimizer.h"

/*!
 * class ossimOutlierRejection
 * class for optimizing a model, while dealing with outliers
 *
 * exports a model and a set of tie points (opt)
 * you can use ossimImageCorrelator to get a tie point input (object ossimTieGptSet)
 */
class OSSIM_REGISTRATION_DLL ossimOutlierRejection : public ossimModelOptimizer
{
public:
   ossimOutlierRejection();
   virtual inline ~ossimOutlierRejection() {}

  /**
   *accessors to parms
   */   
   inline void  setInlierRatio(const ossim_float64& v) { theInlierRatio = v; }
   inline const ossim_float64& getInlierRatio()const { return theInlierRatio; }

   inline void  setInlierImageAccuracy(const ossim_float64& v) { theInlierImageAccuracy = v; }
   inline const ossim_float64& getInlierImageAccuracy()const { return theInlierImageAccuracy; }

   /*!
    * export (modified) tie point set
    */
   bool saveGMLTieSet(const ossimString& filepath);

   /*!
    * remove outliers (using RANSAC)
    * the projection will be modified, and fitted to inliers if success
    *
    * RETURNS: true for success
    *          variance , unit pixel^2 (optional, if pointer not NULL)
    * INPUT:  target variance in pixel^2 (optional, and not used so far - feb 2006)
    */
   virtual bool removeOutliers(ossim_float64* result_var_pix2 = NULL,
                               ossim_float64* target_var_pix2 = NULL);

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   virtual bool execute();
   
protected:
   ossim_float64 theInlierRatio;         //!between 0 and 1
   ossim_float64 theInlierImageAccuracy; //!unit pixels
   ossimFilename theInlierOutputFilename;
   //disable copy constructor
   inline ossimOutlierRejection(const ossimOutlierRejection& copyme) {}
   
TYPE_DATA   
};

#endif //ossimOutlierRejection_HEADER

