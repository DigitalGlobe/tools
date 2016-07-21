#ifndef ossimCFARFilter_HEADER
#define ossimCFARFilter_HEADER
#include <ossim/plugin/ossimSharedObjectBridge.h>
#include <ossim/base/ossimString.h>
#include <ossim/imaging/ossimImageSourceFilter.h>

#define BOXSIZE 5

class ossimCFARFilter : public ossimImageSourceFilter
{
public:
   ossimCFARFilter(ossimObject* owner=NULL);
   virtual ~ossimCFARFilter();

   virtual ossimString getShortName()const{return ossimString("CFAR detector");}
   virtual ossimString getLongName()const{return ossimString("Constant False Alarm Rate Edge Detector");}

   virtual void getKernel(double kernel[5][5]);
   
   virtual void setKernel(double kernel[5][5]);
   
   virtual ossim_float64 getThreshold() const;
   
   virtual void setThreshold(ossim_float64 threshold);

   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);

   virtual void initialize();

   virtual double getNullPixelValue(ossim_uint32 band=0) const;
   virtual double getMinPixelValue(ossim_uint32 band=0)  const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)  const;

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;


   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = 0);
   
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix = 0)const;
   
protected:

   /**
    * Allocates theTile.
    */
   void allocate();

   /**
    * Clears data members theNullPixValue, theMinPixValue, and
    * theMaxPixValue.
    */
   void clearNullMinMax();

   /**
    * Computes null, min, and max considering input connection and theKernel.
    */
   void computeNullMinMax();
   
   ossimRefPtr<ossimImageData> theTile;
   double theKernel[5][5];
   ossim_float64 theThreshold;

   vector<double> theNullPixValue;
   vector<double> theMinPixValue;
   vector<double> theMaxPixValue;   
   
   /*!
    * Convolve full means that the input data is full and has
    * no null data.  We don't have to compare for nulls here
    */
   template<class T>
   void convolveFull(T,
                     ossimRefPtr<ossimImageData> inputData,
                     ossimRefPtr<ossimImageData> outputData);
   
   /*!
    * Convolve partial means that the input data is has some
    * null data.  We will have to compare nulls
    */
   template<class T>
   void convolvePartial(T,
                        ossimRefPtr<ossimImageData> inputData,
                        ossimRefPtr<ossimImageData> outputData);
                       


TYPE_DATA
};

#endif
