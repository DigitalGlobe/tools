// class ossimMultiplier : Image Combiner

#ifndef ossimMultiplier_HEADER
#define ossimMultiplier_HEADER

#include <ossim/imaging/ossimImageCombiner.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimTDpt.h>
#include <vector>
#include "ossimRegistrationExports.h"
/**
 * class ossimMultiplier : Image Combiner
 * product of input images, of same scalar type
 *
 * restrict to minimum number of bands of inputs
 * no NODATA values used
 * TODO : any number of inputs
 *        support any operation : + / -
 */
class OSSIM_REGISTRATION_DLL ossimMultiplier : public ossimImageCombiner
{
public:
   //own public methods
   ossimMultiplier();
   ossimMultiplier(ossimObject *owner,         
                   ossimImageSource *left,
                   ossimImageSource *right);
   virtual ~ossimMultiplier();   
   
   //inherited public methods
   virtual void                        initialize();
   virtual bool                        canConnectMyInputTo(ossim_int32 index,const ossimConnectableObject* object)const;
   virtual double                      getNullPixelValue(ossim_uint32 band)const;
   virtual double                      getMinPixelValue(ossim_uint32 band=0)const;
   virtual double                      getMaxPixelValue(ossim_uint32 band=0)const;

   virtual ossim_uint32                getNumberOfOutputBands()const;
   virtual ossimIrect                  getBoundingRect(ossim_uint32 resLevel=0)const;

   virtual ossimRefPtr<ossimImageData> getTile (const ossimIrect &rect, ossim_uint32 resLevel=0);   

protected:
   std::vector<double> theMin;
   std::vector<double> theMax;

   void computeMinMax();
   void switchTypeRun(const ossimIrect &rect, ossim_uint32 resLevel);
   template<class T> void operate( T, const ossimIrect& tileRect, ossim_uint32 resLevel);

TYPE_DATA
};
#endif
