// $Id: oms.i 23424 2015-07-14 17:46:02Z dburken $


// Copied to cpp wrapper file...
//%module joms
%module(directors="1") joms
%{
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimGrect.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimObjectFactory.h>
#include <ossim/base/ossimProperty.h>
#include <ossim/base/ossimPropertyInterface.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimSource.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimDataObject.h>
#include <ossim/base/ossimRectilinearDataObject.h>
#include <ossim/base/ossimDatumFactoryRegistry.h>
#include <ossim/base/ossimUnitConversionTool.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimColumnVector3d.h>
#include <ossim/base/ossimPointObservation.h>
#include <ossim/init/ossimInit.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageSourceFactoryRegistry.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageRenderer.h>
#include <ossim/imaging/ossimImageWriter.h>
#include <ossim/imaging/ossimImageFileWriter.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/imaging/ossimMemoryImageSource.h>
#include <ossim/parallel/ossimIgen.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimImageViewTransform.h>
#include <ossim/projection/ossimImageViewAffineTransform.h>
#include <ossim/support_data/ossimNmeaMessage.h>
#include <ossim/support_data/ossimNmeaMessageSequencer.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/elevation/ossimElevationAccuracyInfo.h>
#include <vector>
#include <string>

#include <oms/Chipper.h>
#include <oms/GpkgWriter.h>
#include <oms/Object.h>
#include <oms/DataInfo.h>
#include <oms/ElevMgr.h>
#include <oms/ImageData.h>
#include <oms/ImageUtil.h>
#include <oms/Info.h>
#include <oms/Keywordlist.h>
#include <oms/KeywordlistIterator.h>
#include <oms/Projection.h>
#include <oms/SingleImageChain.h>
#include <oms/StringPair.h>
#include <oms/Chain.h>
#include <oms/ImageStager.h>
#include <oms/Constants.h>
#include <oms/WmsMap.h>
#include <oms/ImageModel.h>
#include <oms/AdjustmentModel.h>
#include <oms/GeodeticEvaluator.h>
#include <oms/Init.h>
#include <oms/Mosaic.h>
#include <ossim/base/ossimConstants.h>
#include <oms/Util.h>
#include <oms/WmsView.h>
#include <oms/CoordinateUtility.h>
#include <oms/WktUtility.h>
#include <oms/Video.h>
#include <oms/Ephemeris.h>
#include <oms/MapProjection.h>
#include <oms/Envelope.h>
#include <oms/TileCacheSupport.h>

//#include "RasterEngine.h"

#include <exception>


#if 0
#include <cstdio>
#include <stdexcept>
/**
 *  A stash area embedded in each allocation to hold java handles
 */
struct Jalloc {
  jbyteArray jba;
  jobject ref;
};

static JavaVM *cached_jvm = 0;
static OpenThreads::Mutex mPrivateMutex;
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved)
{
   //  OpenThreads::ScopedLock<OpenThreads::Mutex> lock(mPrivateMutex);
   cached_jvm = jvm;
   return JNI_VERSION_1_2;
}

JNIEXPORT void JNICALL JNI_OnUnLoad(JavaVM *jvm, void *reserved)
{
   std::cout << "JNIEXPORT void JNICALL JNI_OnUnLoad(JavaVM *jvm, void *reserved)"
             << std::endl;
   //  OpenThreads::ScopedLock<OpenThreads::Mutex> lock(mPrivateMutex);
   cached_jvm = 0;
}

static JNIEnv * JNU_GetEnv()
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(mPrivateMutex);
   JNIEnv *env = 0;
   if(!cached_jvm) return env;
   jint rc = cached_jvm->GetEnv((void **)&env, JNI_VERSION_1_4);
   if (rc == JNI_EDETACHED)
      throw std::runtime_error("current thread not attached");
   if (rc == JNI_EVERSION)
      throw std::runtime_error("jni version not supported");
   return env;
}

void * operator new(size_t t)
{
   //  OpenThreads::ScopedLock<OpenThreads::Mutex> lock(mPrivateMutex);
   if (cached_jvm != 0) 
   {
      JNIEnv *env = JNU_GetEnv();
      jbyteArray jba = env->NewByteArray(t + sizeof(Jalloc));
      if (env->ExceptionOccurred())
         throw std::bad_alloc();
      void *jbuffer = static_cast<void *>(env->GetByteArrayElements(jba, 0));
      if (env->ExceptionOccurred())
         throw std::bad_alloc();
      Jalloc *pJalloc = static_cast<Jalloc *>(jbuffer);
      pJalloc->jba = jba;
      /* Assign a global reference so byte array will persist until delete'ed */
      pJalloc->ref = env->NewGlobalRef(jba);
      if (env->ExceptionOccurred())
      {
         throw std::bad_alloc();
      }
      return static_cast<void *>(static_cast<char *>(jbuffer) + sizeof(Jalloc));
   }
   else 
   { /* JNI_OnLoad not called, use malloc and mark as special */
      Jalloc *pJalloc = static_cast<Jalloc *>(malloc(t + sizeof(Jalloc)));
      if (!pJalloc)
         throw std::bad_alloc();
      pJalloc->ref = 0;
      return static_cast<void *>(
         static_cast<char *>(static_cast<void *>(pJalloc)) + sizeof(Jalloc));
   }
}

void operator delete(void *v)
{
   //  OpenThreads::ScopedLock<OpenThreads::Mutex> lock(mPrivateMutex);
   void *buffer = static_cast<void *>( static_cast<char *>(v) - sizeof(Jalloc));
   Jalloc *pJalloc = static_cast<Jalloc *>(buffer);
   if (pJalloc->ref) 
   {
      JNIEnv *env = JNU_GetEnv();
      env->DeleteGlobalRef(pJalloc->ref);
      env->ReleaseByteArrayElements(pJalloc->jba, static_cast<jbyte *>(buffer), 0);
   }
   else 
   {
      free(buffer);
   }
}
#endif // commented out the operator new and delete override for now

%}


//---
// Handle any std::exception including ossimException.
// Note: Must be placed before any methods or libraries to be wrapped:
//---
%include <exception.i>
%exception
{
   try
   {
      $action
   }
   catch (const std::exception& e)
   {
      SWIG_exception(SWIG_RuntimeError, e.what());
   }
}


// generate directors for all classes that have virtual methods
//%feature("director") oms::SingleImageChain;

%typemap(in) void * {
   /* %typemap(in) void * */
   $1 = jenv->GetDirectBufferAddress($input);
 }

/* These 3 typemaps tell SWIG what JNI and Java types to use */
%typemap(jni) void * "jobject"
%typemap(jtype) void * "java.nio.ByteBuffer"
%typemap(jstype) void * "java.nio.ByteBuffer"
%typemap(javain) void * "$javainput"
%typemap(javaout) void * {
    return $jnicall;
  }

%include "std_map.i"
%include "std_vector.i"
%include "std_string.i"
%include "typemaps.i"
%include "arrays_java.i"
%include "carrays.i"
// %include "std_iostream.i"

//%rename (assignTo) oms::Irect::operator=;
//%rename (printOut) operator<<;
%rename (assignToGrect) ossimGrect::operator=;
%rename (printConversionTool) ossimUnitConversionTool::operator<<;
%include <ossim/base/ossimConstants.h>
%include <ossim/base/ossimUnitConversionTool.h>
%include <oms/Constants.h>
%include <oms/ImageUtil.h>
%include <oms/Object.h>
%include <ossim/base/ossimConstants.h>
%include <ossim/base/ossimException.h>
%include <ossim/support_data/ossimNmeaMessage.h>
%include <ossim/support_data/ossimNmeaMessageSequencer.h>
%include <ossim/elevation/ossimElevationAccuracyInfo.h>

//%include <oms/Irect.h>
%include "various.i"


//%rename (assignToFloat64) oms::Float64::operator=;
//%rename (printOut) operator<<;
%array_functions( double, double_array )
%apply char **STRING_ARRAY { char* argv[] };
%apply short[] {ossim_sint16 *};
%apply int[] {ossim_sint32 *};
%apply int[] {ossim_int32 *};
%apply int[] {int *};
%apply unsigned int[] {ossim_uint32 *};
%apply unsigned short[] {ossim_uint16 *};
%apply float[] {ossim_float32 *};
%apply double[] {ossim_float64 *};
%apply unsigned char[]  { ossim_uint8 * }
%apply char* BYTE  { ossim_int8 * }

%warnfilter(516) ossimIrect;
%warnfilter(516) ossimGrect;

//---
// ossimException handled in above rule so this is commented out.
// drb - 20140714
// %typemap(throws, throws="java.lang.Exception") ossimException {
//  jclass excep = jenv->FindClass("java/lang/Exception");
//  if (excep)
//    jenv->ThrowNew(excep, $1.what());
//  return $null;
// }

template<class T> class ossimRefPtr
{
public:
	T* operator->();
	bool valid() const;
	T* get();
};

%extend ossimRefPtr
{
public:
   void set(T* value)
   {
      self->operator=(value);
   }
};
%newobject ossimObject::dup()const;
%feature("abstract") ossimDataObject;


class ossimObject : public ossimReferenced
{
public:
   virtual ~ossimObject();
   /*!
    * Method to save the state of the object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;
   
   /*!
    * Method to the load (recreate) the state of the object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

protected:
   ossimObject();
   virtual ossimObject* dup()const;
};

class ossimDatum
{
public:
   ossimDatum(const ossimString &code, const ossimString &name,
              const ossimEllipsoid* anEllipsoid,
              ossim_float64 sigmaX, ossim_float64 sigmaY, ossim_float64 sigmaZ,
              ossim_float64 westLongitude, ossim_float64 eastLongitude,
              ossim_float64 southLatitude, ossim_float64 northLatitude);
   virtual ossimGpt shift(const ossimGpt    &aPt)const=0;
   //utility functions to shift to and from the standard coordinates.
   //Users should use the shift instead!!!
   //
   virtual ossimGpt       shiftToWgs84(const ossimGpt &aPt)const = 0;
   virtual ossimGpt       shiftFromWgs84(const ossimGpt &aPt)const = 0;


   virtual bool  isTheSameAs(const ossimDatum *aDatum)const;
   virtual const ossimString& code()const;
   virtual const ossimString& name()const;
   virtual const ossimEllipsoid* ellipsoid()const;
   virtual ossim_float64 sigmaX()const;
   virtual ossim_float64 sigmaY()const;
   virtual ossim_float64 sigmaZ()const;

   virtual ossim_float64 westLongitude()const;
   virtual ossim_float64 eastLongitude()const;
   virtual ossim_float64 southLatitude()const;
   virtual ossim_float64 northLatitude()const;

   virtual ossim_float64 param1()const=0;
   virtual ossim_float64 param2()const=0;
   virtual ossim_float64 param3()const=0;
   virtual ossim_float64 param4()const=0;
   virtual ossim_float64 param5()const=0;
   virtual ossim_float64 param6()const=0;
   virtual ossim_float64 param7()const=0;

   virtual bool isInside(ossim_float64 latitude, ossim_float64 longitude)const;

protected:
   ~ossimDatum();
};

class ossimPropertyInterface
{
public:
   virtual void setProperty(const ossimString& name,
                            const ossimString& value);
   
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   void getPropertyList(std::vector<ossimRefPtr<ossimProperty> >& propertyList)const;
   void setProperties(std::vector<ossimRefPtr<ossimProperty> >& propertyList);
   
protected:
   ossimPropertyInterface();
};

%extend ossimPropertyInterface
{
public:
   void setProperty(const std::string& name, const std::string& value)
   {
      self->setProperty(ossimString(name), ossimString(value));
   }
}


class ossimConnectableObject : public ossimObject
{
protected:
   ossimConnectableObject();

public:
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   void getPropertyList(std::vector<ossimRefPtr<ossimProperty> >& propertyList)const;
   void setProperties(std::vector<ossimRefPtr<ossimProperty> >& propertyList);
   virtual ossim_int32 connectMyInputTo(ossimConnectableObject* inputObject,
                                        bool makeOutputConnection=true,
                                        bool createEventFlag=true);
   virtual ossim_int32 connectMyInputTo(ossim_int32 inputIndex,
                                        ossimConnectableObject* inputObject,
                                        bool makeOutputConnection=true,
                                        bool createEventFlag=true);
   virtual bool connectMyInputTo(ossimConnectableObject::ConnectableObjectList& inputList,
                                 bool makeOutputConnection=true,
                                 bool createEventFlag = true);
   virtual void disconnectAllInputs();
   virtual ossim_uint32 getNumberOfInputs()const
      {
         return (ossim_uint32)theInputObjectList.size();
      }
   
   virtual ossim_uint32 getNumberOfOutputs()const
      {
         return (ossim_uint32)theOutputObjectList.size();
      }

   ossimConnectableObject* getInput(ossim_uint32 index=0);

//   const ossimConnectableObject* getInput(ossim_uint32 index=0)const;

   ossimConnectableObject* getOutput(ossim_uint32 index=0);

//   const ossimConnectableObject* getOutput(ossim_uint32 index=0)const;

   virtual void setNumberOfInputs(ossim_int32 numberOfInputs);

   virtual bool getInputListIsFixedFlag()const;

   virtual bool getOutputListIsFixedFlag()const;
   virtual void setNumberOfOutputs(ossim_int32 numberOfInputs);
};

%extend ossimConnectableObject
{
public:
   virtual ossimPropertyInterface* getPropertyInterface()
   {
      return (ossimPropertyInterface*)self;
   }
   ossim_int32 addInput(ossimConnectableObject* input)
   {
      return self->connectMyInputTo(input);
   }
};

class ossimSource : public ossimConnectableObject
{
public:
   //virtual void initialize();
   
protected:
    ossimSource();
};

class ossimString
{
public:
   ossimString();
   ossimString(const char*);
};
%extend ossimString {
public:
   std::string toString()
   {
      // return self->c_str();
      return self->string();
   }
};

class ossimFilename : public ossimString
{
public:
   ossimFilename(const char* src);
};
%extend ossimFilename
{
public:
   ossimFilename(const std::string& value)
   :ossimString(value)
   {}
};


class ossimIpt
{
public:
   ossimIpt(ossim_int32 anX, ossim_int32 aY);
   ossim_int32 x;
   ossim_int32 y;
};


class ossimIrect
{
public:
   ossimIrect();
   ossimIrect(ossimIpt ul_corner,
              ossimIpt lr_corner,
              ossimCoordSysOrientMode mode=OSSIM_LEFT_HANDED);
   ossimIrect(ossim_int32 ul_corner_x,
              ossim_int32 ul_corner_y,
              ossim_int32 lr_corner_x,
              ossim_int32 lr_corner_y,
              ossimCoordSysOrientMode mode=OSSIM_LEFT_HANDED);
   ossimIrect(const ossimDrect& rect);
   
   ossimIpt ul() const;
   ossimIpt ur() const;
   ossimIpt lr() const;
   ossimIpt ll() const;
   
   ossimIrect clipToRect(const ossimIrect& rect)const;
   
   inline ossimIpt midPoint()const;

   ossimIrect combine(const ossimIrect& rect)const;
   ossim_uint32 width() const;
   ossim_uint32 height() const;
};

%extend ossimIrect
{
public:
   void setOriginWidthHeight(ossim_int32 x, ossim_int32 y, ossim_uint32 w, ossim_uint32 h)
   {
      *self = ossimIrect(x,y, x+w-1, y+h-1);
   }
   ossim_uint32 getWidth()const
   {
      return self->width();
   }
   ossim_uint32 getHeight()const
   {
      return self->height();
   }
   ossimIpt getMidPoint()const
   {
      return self->midPoint();
   }
   void print()
   {
      std::cout << *self << std::endl;
   }
};

class ossimDpt
{
public:
   ossimDpt(double anX, double aY);
   ossimDpt(const ossimIpt& pt);
   bool hasNans()const;
   bool isNan()const;
   double x;
   double y;
};

class ossimDrect
{
public:
   ossimDrect();
   ossimDrect(ossimDpt ul_corner,
              ossimDpt lr_corner,
              ossimCoordSysOrientMode mode=OSSIM_LEFT_HANDED);
   ossimDrect(const double& ul_corner_x,
              const double& ul_corner_y,
              const double& lr_corner_x,
              const double& lr_corner_y,
              ossimCoordSysOrientMode mode=OSSIM_LEFT_HANDED);
   
   ossimDpt ul() const;
   ossimDpt ur() const;
   ossimDpt lr() const;
   ossimDpt ll() const;
   ossim_float64 width() const;
   ossim_float64 height() const;
   ossimDrect clipToRect(const ossimDrect& rect)const;
   ossimDrect combine(const ossimDrect& rect)const;
   inline ossimDpt midPoint()const;
   
};

%extend ossimDrect
{
public:
   void setOriginWidthHeight(double x, double y, double w, double h)
   {
      *self = ossimDrect(x,y, x+w-1,y+h-1);
   }
   double getWidth()const
   {
      return self->width();
   }
   double getHeight()const
   {
      return self->height();
   }
   ossimDpt getMidPoint()const
   {
      return self->midPoint();
   }
   void print()
   {
      std::cout << *self << std::endl;
   }
};

class ossimGrect
{
public:
    ossimGrect();

   /**
    * Copies the passed in rectangle to this
    * object.
    */
   ossimGrect(const ossimGrect& rect);

   /**
    * WIll take two ground points and fill the
    * bounding rect appropriately.
    */
   ossimGrect(const ossimGpt& ul,
              const ossimGpt& lr);

   /**
    * Takes the upper left and lower right ground
    * points
    */
   ossimGrect(double ulLat,
              double ulLon,
              double lrLat,
              double lrLon);
   ossimGrect(double ulLat,
              double ulLon,
              double lrLat,
              double lrLon,
              ossimDatum* datum);
    ossimGrect(const ossimGpt& point,
              double latSpacingInDegrees,
              double lonSpacingInDegrees);
   ossimGrect(std::vector<ossimGpt>& points);
   ossimGrect(const ossimGpt& p1,
              const ossimGpt& p2,
              const ossimGpt& p3,
              const ossimGpt& p4);


   const ossimGrect& operator=(const ossimGrect& rect);

   ossimGpt midPoint()const;

   /**
    * Returns the height of a rectangle.
    */
   ossim_float64 height() const;

   /**
    * Returns the width of a rectangle.
    */
   ossim_float64 width()  const;

   const ossimGpt& ul()const;
   const ossimGpt ur()const;
   const ossimGpt ll()const;
   const ossimGpt& lr()const;


   void makeNan();

   bool isLonLatNan()const;

   bool hasNans()const;

   bool isNan()const;

   /*!
    * Returns true if "this" rectangle is contained completely within the
    * input rectangle "rect".
    */
   bool completely_within(const ossimGrect& rect) const;

   /*!
    * Returns true if any portion of an input rectangle "rect" intersects
    * "this" rectangle.
    */
   bool intersects(const ossimGrect& rect) const;

   ossimGrect clipToRect(const ossimGrect& rect)const;

   ossimGrect combine(const ossimGrect& rect)const;

   /**
    * METHOD: pointWithin(ossimGpt)
    *
    * @param gpt Point to test for withinness.
    *
    * @return true if argument is inside of horizontal rectangle
    *
    * @note Height is not considered and there is no datum shift applied if
    * gpt is of a different datum than this datum.
    */
   bool pointWithin(const ossimGpt& gpt) const; //inline below

   ossimGrect stretchToEvenBoundary(double latSpacingInDegrees,
                                    double lonSpacingInDegrees)const;

   void computeEvenTiles(std::vector<ossimGrect>& result,
                         double latSpacingInDegrees,
                         double lonSpacingInDegrees,
                         bool clipToGeographicBounds = true)const;
private:
   ossimGpt theUlCorner;
   ossimGpt theUrCorner;
   ossimGpt theLrCorner;
   ossimGpt theLlCorner;

};

class ossimKeywordlist
{
public:
  ossimKeywordlist(char delimiter = DEFAULT_DELIMITER);
  ossimKeywordlist(const std::map<std::string, std::string>& keywordMap);

  ossimKeywordlist(const char*     file,
               char            delimiter = DEFAULT_DELIMITER,
               bool            ignoreBinaryChars = false);

   ossimKeywordlist(const ossimFilename& fileName,
               char            delimiter = DEFAULT_DELIMITER,
               bool            ignoreBinaryChars = false);


   /*!
    *  Reads file and adds keywords to the KeywordMap.
    *  Returns true if file was parsed, false on error.
    */
   bool addFile(const char* file);

   /*!
    *  Reads file and adds keywords to the KeywordMap.
    *  Returns true if file was parsed, false on error.
    */
   bool addFile(const ossimFilename& file);

  // Methods to add keywords to list.
   void add(const char*   key,
            const char*   value,
            bool          overwrite = true);

   void add(const char*   prefix,
            const char*   key,
            const char*   value,
            bool          overwrite = true);



   const char* find(const char* key) const;
   const char* find(const char* prefix,
                    const char* key) const;

   void remove(const char * key);
   void remove(const char* prefix, const char * key);

   void clear();

  /**
    * Methods to dump the ossimKeywordlist to a file on disk.
    * @return true on success, false on error.
    */
   bool write(const char* file) const;
};

%extend ossimKeywordlist {
public:
   std::string toString()
   {
      return self->toString().string();
      // return self->toString();
   }
};

class ossimGpt
{
public:
   ossimGpt();
   /*!
    * Constructor.  The values are assumed to be in DEGREES.
    */
   ossimGpt(double lat, double lon);
   ossimGpt(double lat, double lon, double hgt);
   ossimGpt(double lat, double lon, double hgt, const ossimDatum* datum);


   ossimGpt(const ossimGpt& src);

   /*!
    * Will convert the radian measure to degrees.
    */
   double latd() const;

   /*!
    * Will convert the radian measure to degrees.
    */
   double lond() ;

   ossimDpt metersPerDegree() const;

   double height() const;

   void makeNan();
   bool isNan()const;
   bool hasNans()const;
   bool isLatNan()const;
   bool isLonNan()const;
   bool isHgtNan()const;
   double distanceTo(const ossimGpt& arg_gpt) const;
};

%extend ossimGpt
{
public:
   void setLatd(double value)
   {
      self->latd(value);
   }
   void setLond(double value)
   {
      self->lond(value);
   }
   double getLatd()
   {
    return self->latd();
   }
   double getLond()
   {
    return self->lond();
   }
   double getHeight()
   {
      return self->height();
   }
   void setHeight(double value)
   {
      self->height(value);
   }
   void assign(const ossimGpt& pt)
   {
    *self = pt;
   }
};

class ossimEcefPoint
{
public:
     ossimEcefPoint();

   ossimEcefPoint(const ossimEcefPoint& copy_this);

   ossimEcefPoint(const ossimGpt& convert_this);

   ossimEcefPoint(const double& x,
                  const double& y,
                  const double& z);
   double    x() const;
   double    y() const;
   double    z() const;
};
%extend ossimEcefPoint
{
public:
  void setX(double x)
  {
    self->x() = x;
  }
  double getX()
  {
    return self->x();
  }
  void setY(double y)
  {
    self->y() = y;
  }
  double getY()
  {
    return self->y();
  }
  void setZ(double z)
  {
    self->z() = z;
  }
  double getZ()
  {
    return self->z();
  }
  void assign(const ossimGpt& gpt)
  {
    *self = ossimEcefPoint(gpt);
  }
  void assign(const ossimEcefPoint& src)
  {
    *self = src;
  }
}

%newobject ossimObjectFactory::createObject(const ossimString& typeName)const;
%newobject ossimObjectFactory::createObject(const ossimKeywordlist& kwl, const char* prefix)const;
%template( ossimPropertyRefPtr ) ossimRefPtr<ossimProperty>;
%template( ossimConnectableObjectRefPtr ) ossimRefPtr<ossimConnectableObject>;
%template( ossimObjectRefPtr ) ossimRefPtr<ossimObject>;

class ossimObjectFactory : public ossimObject
{
public:

   /*!
    * Creates an object given a type name.
    */
   virtual ossimObject* createObject(const ossimString& typeName)const=0;

   /*!
    * Creates and object given a keyword list.
    */
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const=0;

   /*!
    * This should return the type name of all objects in all factories.
    * This is the name used to construct the objects dynamially and this
    * name must be unique.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const=0;

protected:
	ossimObjectFactory();
};

class ossimInit
{
public:
	static ossimInit* instance();
	void initialize( int argc, char *argv[]);

protected:
	ossimInit();
};

class ossimPreferences
{
public:
   static ossimPreferences* instance();
   bool loadPreferences(const ossimFilename& pathname);
   const char* findPreference(const char* key) const;
   void addPreference(const char* key,
                      const char* value);
   ossimFilename getPreferencesFilename() const;
   
protected:
   ossimPreferences();
};

%feature("abstract") ossimDataObject;
class ossimDataObject : public ossimObject
{
public:
   ossimDataObject(ossimSource* source=0,
                   ossimDataObjectStatus status=OSSIM_STATUS_UNKNOWN);
   virtual void initialize()=0;
   virtual void  setDataObjectStatus(ossimDataObjectStatus status) const;
   virtual ossimDataObjectStatus getDataObjectStatus() const;
};

%feature("abstract") ossimRectilinearDataObject;
class ossimRectilinearDataObject : public ossimDataObject
{
public:
//   ossimRectilinearDataObject(const ossimRectilinearDataObject& rhs);
   virtual ossimScalarType getScalarType() const;
//protected:
//   ossimRectilinearDataObject();
};


/******************************************************* OSSIM IMAGING ***********************************************/
%template( ossimIgenRefPtr ) ossimRefPtr<ossimIgen>;
class ossimIgen : public ossimReferenced
{
public:
   ossimIgen();

   virtual void initialize(const ossimKeywordlist& kwl) throw(ossimException);
   virtual void outputProduct() throw(ossimException);
};



class ossimImageData : public ossimRectilinearDataObject
{
public:
   ossimImageData(const ossimImageData &rhs);

   virtual ossim_uint32 getHeight()const;
   virtual ossim_uint32 getWidth()const;
   virtual ossimIrect getImageRectangle() const;
   virtual void getWidthHeight(ossim_uint32& w, ossim_uint32& h);
   virtual void setWidth(ossim_uint32 width);
   virtual void setHeight(ossim_uint32 height);
   virtual void setWidthHeight(ossim_uint32 w, ossim_uint32 h);
   virtual void setImageRectangleAndBands(const ossimIrect& rect,
                                          ossim_uint32 numberOfBands);
   virtual void setImageRectangle(const ossimIrect& rect);
   virtual void setOrigin(const ossimIpt& origin);
    virtual ossim_float64   getMinNormalizedPix() const;
   virtual ossimString     getScalarTypeAsString()const;
   virtual ossim_uint32    getNumberOfBands() const;
   virtual void            setNumberOfBands(ossim_uint32 bands,
                                            bool reallocate=false);
   /**
    * Will return the pixel at location position.  Note it will first get
    * the passed in position relative to the origin or upper left
    * corner of this tile and then return the result.
    */
   virtual ossim_float64 getPix(const ossimIpt& position,
                                ossim_uint32 band=0) const;

   /**
    * Will return the pixel at offset and band number.
    */
   virtual ossim_float64 getPix(ossim_uint32 offset,
                                ossim_uint32 band = 0) const;
   void fill(ossim_uint32 band, ossim_float64 value);
   void fill(ossim_float64 value);
   bool isNull(ossim_uint32 offset)const;
   bool isNull(ossim_uint32 offset, ossim_uint32 band)const;
   void setNull(ossim_uint32 offset);
   void setNull(ossim_uint32 offset, ossim_uint32 band);

   bool isNull(const ossimIpt& pt)const;
   void setNull(const ossimIpt& pt);
   bool isNull(const ossimIpt& pt, ossim_uint32 band)const;
   void setNull(const ossimIpt& pt, ossim_uint32 band);

   virtual bool   isValidBand(ossim_uint32 band) const;
   virtual ossimDataObjectStatus validate() const;
   virtual ossimRefPtr<ossimImageData> newNormalizedFloat()const;
   virtual void getNormalizedFloat(ossim_uint32 offset,
                                   ossim_uint32 bandNumber,
                                   ossim_float32& result)const;

  virtual void setNormalizedFloat(ossim_uint32 offset,
                                   ossim_uint32 bandNumber,
                                   ossim_float32 input);

   virtual void convertToNormalizedFloat(ossimImageData* result)const;

   virtual ossimImageData* newNormalizedDouble()const;

   virtual void convertToNormalizedDouble(ossimImageData* result)const;

   virtual void unnormalizeInput(ossimImageData* normalizedInput);

   virtual ossim_float64 computeAverageBandValue(
      ossim_uint32 bandNumber = 0) const;

   virtual ossim_float64 computeMeanSquaredError(
      ossim_float64 meanValue,
      ossim_uint32 bandNumber = 0) const;

   virtual const void* getBuf() const;
   virtual ossim_uint32 getSize() const;
   virtual ossim_uint32 getSizePerBand()const;
   virtual ossim_uint32 getSizePerBandInBytes() const;
   virtual ossim_uint32  getSizeInBytes() const;
   virtual void makeBlank();
   virtual void initialize();

   virtual void unloadTile(void* dest,
                           const ossimIrect& dest_rect,
                           ossimInterleaveType il_type) const;

   virtual void unloadTile(void* dest,
                           const ossimIrect& dest_rect,
                           const ossimIrect& clip_rect,
                           ossimInterleaveType il_type) const;
protected:
   ossimImageData();
};

%extend ossimImageData
{
public:
   void copyToBuffer(ossim_int8* destinationBuffer, int bandIdx)
   {
      memcpy(destinationBuffer, self->getBuf(bandIdx), self->getSizePerBandInBytes());
   }
   void copyToBuffer(ossim_uint8* destinationBuffer, int bandIdx)
   {
      memcpy(destinationBuffer, self->getBuf(bandIdx), self->getSizePerBandInBytes());
   }

   void copyToBufferS16(ossim_sint16* destinationBuffer, int bandIdx)
   {
      memcpy(destinationBuffer, self->getBuf(bandIdx), self->getSizePerBandInBytes());
   }
   void copyToBufferU16(ossim_uint16* destinationBuffer, int bandIdx)
   {
      memcpy(destinationBuffer, self->getBuf(bandIdx), self->getSizePerBandInBytes());
   }
   void copyToBufferf32(ossim_float32* destinationBuffer, int bandIdx)
   {
      memcpy(destinationBuffer, self->getBuf(bandIdx), self->getSizePerBandInBytes());
   }
   void copyToBufferf64(ossim_float64* destinationBuffer, int bandIdx)
   {
      memcpy(destinationBuffer, self->getBuf(bandIdx), self->getSizePerBandInBytes());
   }
   void loadTile8(ossim_int8* src,
           ossimInterleaveType il_type)
   {
      self->loadTile((void*)src, self->getImageRectangle(), il_type);
   }
   void loadTile8WithAlpha(ossim_int8* src,
           ossimInterleaveType il_type)
   {
      self->loadTileWithAlpha((void*)src, self->getImageRectangle(), il_type);
   }

};
%template( ossimImageDataRefPtr ) ossimRefPtr< ossimImageData >;



%newobject ossimImageDataFactory::create(ossimSource* owner,
                                         ossimScalarType scalar,
                                         ossim_uint32 bands = 1)const;
%newobject ossimImageDataFactory::create(ossimSource* owner,
                                         ossimScalarType scalar,
                                         ossim_uint32 bands,
                                         ossim_uint32 width,
                                         ossim_uint32 height)const;
%newobject ossimImageDataFactory::create(
                                         ossimSource* owner,
                                         ossim_uint32 bands,
                                         ossimImageSource* inputSource)const;
%newobject ossimImageDataFactory::create(
                                         ossimSource* owner,
                                         ossimImageSource* inputSource)const;
class ossimImageDataFactory
{
public:
public:
   virtual ~ossimImageDataFactory();
   static ossimImageDataFactory* instance();

   virtual ossimRefPtr<ossimImageData> create(ossimSource* owner,
                                              ossimScalarType scalar,
                                              ossim_uint32 bands = 1)const;

   virtual ossimRefPtr<ossimImageData> create(ossimSource* owner,
                                              ossimScalarType scalar,
                                              ossim_uint32 bands,
                                              ossim_uint32 width,
                                              ossim_uint32 height)const;

   virtual ossimRefPtr<ossimImageData> create(
      ossimSource* owner,
      ossim_uint32 bands,
      ossimImageSource* inputSource)const;


   virtual ossimRefPtr<ossimImageData> create(
      ossimSource* owner,
      ossimImageSource* inputSource)const;
protected:
   ossimImageDataFactory(); // hide
   ossimImageDataFactory(const ossimImageDataFactory&){}//hide
};


class ossimImageSource : public ossimSource
{
public:
   ossimImageSource(ossimObject* owner = 0);
   ossimImageSource(ossimObject* owner,
                    ossim_uint32 inputListSize,
                    ossim_uint32 outputListSize,
                    bool inputListIsFixedFlag=true,
                    bool outputListIsFixedFlag=true);

   virtual ~ossimImageSource();


   /**
    * @return from origin out to tile_width-1, and tile_height-1.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIpt& origin,
                                               ossim_uint32 resLevel=0);

  /**
   * @return the requested region of interest
   */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0);


  /**
   * Will return the decimation factor for the given resolution
   * level.  the decimation is the scale from Resolution 0 or full
   * res.  Usually this is a power of 2 decimation where
   * the decimation result is 1.0/2^resoltion.
   */
   virtual void getDecimationFactor(ossim_uint32 resLevel,
                                    ossimDpt& result)const;

   /**
    * Will return an array of all decimations for each resolution level.
    */
   virtual void getDecimationFactors(std::vector<ossimDpt>& decimations) const;

   /**
    * Will return the number of resolution levels.  Note: resolution
    * level 0 is included in the return count.
    */
   virtual ossim_uint32 getNumberOfDecimationLevels() const;

   virtual ossim_uint32 getNumberOfInputBands() const = 0;

   /**
    * Returns the number of bands in a tile returned from this TileSource.
    */
   virtual ossim_uint32 getNumberOfOutputBands() const;

   /**
    * Initializes bandList to the zero based order of output bands.
    */
   virtual void getOutputBandList(std::vector<ossim_uint32>& bandList) const;

   /**
    * This will be used to query the output pixel type of the tile source.
    * Please ignore the argument.  It will soon be removed.
    */
   virtual ossimScalarType getOutputScalarType() const;

   /**
    * Returns the default processing tile width
    */
   virtual ossim_uint32 getTileWidth() const;

   /**
    * Returns the default processing tile height
    */
   virtual ossim_uint32 getTileHeight() const;

   /**
    * Each band has a null pixel associated with it.  The null pixel
    * represents an invalid value.
    */
   virtual double getNullPixelValue(ossim_uint32 band=0)const;


   /**
    * Returns the min pixel of the band.
    */
   virtual double getMinPixelValue(ossim_uint32 band=0)const;

   /**
    * Returns the max pixel of the band.
    */
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;

   /**
    * This will return the bounding rect of the source.  We can have several
    * sources which are in a chain to modify the bounding image rect.
    * lets say you are next to an image handler then it will return the
    * bounding rect for that image.  If you are at the right side of a
    * resampler then you will get a bounding rect along the image view plane.
    * This is going to be a very import method for both image writers,
    * mosaics or anything that needs to operate only within the bounds of an
    * image.
    */
   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0) const;

   /**
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /**
    * ordering specifies how the vertices should be arranged.
    * valid image vertices is basically the tightly fit convex hull
    * of the image.  Usually an image has NULL values and are
    * internally not upright rectangular.  This can cause
    * problems some spatial filters.
    *
    * The default implementation is to return the bounding rect.
    */
   virtual void getValidImageVertices(std::vector<ossimIpt>& validVertices,
                                      ossimVertexOrdering ordering=OSSIM_CLOCKWISE_ORDER,
                                      ossim_uint32 resLevel=0)const;

   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();
   /**
    * the default is to find the first input source that is of
    * type ossimImageSourceInterface and return its input geometry.
    */
//   virtual bool getImageGeometry(ossimKeywordlist& kwl,
//                                 const char* prefix=0);

   /**
    * Default method to call input's setImageGeometry.
    */
   virtual void setImageGeometry(ossimImageGeometry* geom);

   virtual void initialize();

protected:
   ossimImageSource (const ossimImageSource& rhs);
   const ossimImageSource& operator= (const ossimImageSource&);

};
%template( ossimImageSourceRefPtr ) ossimRefPtr< ossimImageSource >;

%feature("notabstract") ossimImageChain;
class ossimImageChain : public ossimImageSource
{
public:
   ossimImageChain();

   virtual ossimConnectableObject* getConnectableObject(ossim_uint32 index);
   virtual ossim_int32 indexOf(ossimConnectableObject* obj)const;

   bool replace(ossimConnectableObject* newObj,
                ossimConnectableObject* oldObj);
protected:

};

class ossimImageHandler : public ossimImageSource
{
public:
	virtual bool isOpen()const=0;

   virtual ossim_uint32 getCurrentEntry()const;
   virtual ossim_uint32 	getNumberOfEntries () const;
   virtual ossim_uint32 getNumberOfReducedResSets() const;
   virtual bool setCurrentEntry (ossim_uint32 entryIdx);

   virtual void close() = 0;
   /**
    *  Pure virtual open.  Derived classes must implement.
    *
    *  @return Returns true on success, false on error.
    *
    *  @note This method relies on the data member ossimImageData::theImageFile
    *  being set.  Callers should do a "setFilename" prior to calling this
    *  method or use the ossimImageHandler::open that takes a file name and an
    *  entry index.
    */
   virtual bool open() = 0;

   /**
    *  Opens the image file.
    *
    *  @param imageFile File to open.
    *
    *  @param entryIndex
    *  @return true on success, false on error.
    */
   virtual bool open(const ossimFilename& imageFile,
                     ossim_uint32 entryIndex);

   virtual bool open(const ossimFilename& imageFile);
   virtual bool hasOverviews() const;
   virtual bool openOverview();
   virtual bool openOverview(const ossimFilename& overview_file);
   virtual void closeOverview();
   virtual ossimFilename createDefaultOverviewFilename() const;
   virtual ossimFilename createDefaultGeometryFilename() const;
   virtual ossimFilename createDefaultMetadataFilename() const;
   virtual ossimFilename createDefaultHistogramFilename() const;
   virtual ossimFilename createDefaultValidVerticesFilename() const;
   virtual bool openValidVertices(const ossimFilename& vertices_file);
   virtual bool openValidVertices();
protected:
	ossimImageHandler();
};


%newobject ossimImageHandlerRegistry::open(const ossimFilename& fileName)const;
%newobject ossimImageHandlerRegistry::open(const std::string& filename)const;
class ossimImageHandlerRegistry : public ossimObjectFactory
{
public:
	static ossimImageHandlerRegistry* instance();
   	ossimImageHandler* open(const ossimFilename& fileName)const;

protected:
	ossimImageHandlerRegistry();
};

%extend ossimImageHandlerRegistry
{
   	ossimImageHandler* open(const std::string& filename)const
	{
		return self->open(ossimFilename(filename));
	}
	virtual void getSupportedExtensions(std::vector<std::string>& extensionList)const
	{
		ossimImageHandlerFactoryBase::UniqueList<ossimString> list;

		self->getSupportedExtensions(list);
		extensionList.clear();

		for ( int i = 0, max = list.size(); i < max; i++)
		{
			extensionList.push_back(list[i]);
		}
	};
}

%newobject ossimImageSourceFactoryBase::createImageSource(const ossimString& name)const;
%newobject ossimImageSourceFactoryBase::createImageSource(const ossimKeywordlist& kwl,
                                                          const char* prefix=0)const;
%newobject ossimImageSourceFactoryBase::createImageSource(const std::string& filename)const;
%nodefaultctor ossimImageSourceFactoryBase;
class ossimImageSourceFactoryBase : public ossimObjectFactory
{
public:
   /*!
    * Convenient conversion method.  Gurantees an ossimImageSource is returned.  Returns
    * NULL otherwise
    */
   virtual ossimImageSource* createImageSource(const ossimString& name)const;
   virtual ossimImageSource* createImageSource(const ossimKeywordlist& kwl,
                                               const char* prefix=0)const;
};

%extend ossimImageSourceFactoryBase
{
public:
   virtual ossimImageSource* createImageSource(const std::string& name)const
   {
      return self->createImageSource(ossimString(name));
   }
}


class ossimImageSourceFactoryRegistry : public ossimImageSourceFactoryBase
{
public:
	static ossimImageSourceFactoryRegistry* instance();

protected:
	ossimImageSourceFactoryRegistry();
};


class ossimOutputSource : public ossimSource
{
public:
   ossimOutputSource(ossimObject* owner=0);
   virtual bool isOpen()const = 0;
   virtual bool open()=0;
};

class ossimImageWriter : public ossimOutputSource
{
public:
   ossimImageWriter(ossimObject* owner=NULL);
};

class ossimImageFileWriter : public ossimImageWriter
{
public:
   ossimImageFileWriter(const ossimFilename& filename = ossimFilename(),
                        ossimImageSource* inputSource=0,
                        ossimObject* owner=0);
//   virtual ~ossimImageFileWriter();
   virtual void getImageTypeList(std::vector<ossimString>& imageTypeList)const=0;
   virtual bool hasImageType(const ossimString& imageType) const;
   virtual void setTileSize(const ossimIpt& tileSize);
   virtual void initialize();
   virtual bool execute();
   virtual void  setOutputImageType(ossim_int32 type);
   virtual void  setOutputImageType(const ossimString& type);
   virtual ossim_int32 getOutputImageType() const;
   virtual ossimString getOutputImageTypeString() const;
   virtual void setAreaOfInterest(const ossimIrect& inputRect);

   virtual void setOutputName(const ossimString& outputName);

   virtual void setFilename(const ossimFilename& file);

   virtual const ossimFilename& getFilename()const;

   virtual bool         getWriteImageFlag()            const;
   virtual bool         getWriteHistogramFlag()        const;
   virtual bool         getWriteOverviewFlag()         const;
   virtual bool         getScaleToEightBitFlag()       const;

   virtual bool         getWriteEnviHeaderFlag()       const;
   virtual bool         getWriteErsHeaderFlag()              const;
   virtual bool         getWriteExternalGeometryFlag() const;
   virtual bool         getWriteFgdcFlag()             const;
   virtual bool         getWriteJpegWorldFileFlag()    const;
   virtual bool         getWriteReadmeFlag()           const;
   virtual bool         getWriteTiffWorldFileFlag()    const;

   virtual void         setWriteImageFlag(bool flag);
   virtual void         setWriteOverviewFlag(bool flag);
   virtual void         setWriteHistogramFlag(bool flag);
   virtual void         setScaleToEightBitFlag(bool flag);

   virtual void         setWriteEnviHeaderFlag(bool flag);
   virtual void         setWriteErsHeaderFlag(bool flag);
   virtual void         setWriteExternalGeometryFlag(bool flag);
   virtual void         setWriteFgdcFlag(bool flag);
   virtual void         setWriteJpegWorldFile(bool flag);
   virtual void         setWriteReadme(bool flag);
   virtual void         setWriteTiffWorldFile(bool flag);

   virtual ossim_uint16 getOverviewCompressType() const;
   virtual ossim_int32  getOverviewJpegCompressQuality() const;

   virtual void         setOverviewCompressType(ossim_uint16 type);
   virtual void         setOverviewJpegCompressQuality(ossim_int32 quality);
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
};


%newobject ossimImageWriterFactoryRegistry::createWriterFromExtension(const ossimString& fileExtension)const;
%newobject ossimImageWriterFactoryRegistry::createWriter(const ossimKeywordlist &kwl,
                                                         const char *prefix=0)const;
%newobject ossimImageWriterFactoryRegistry::createWriter(const ossimString& typeName)const;
%newobject ossimImageWriterFactoryRegistry:: createWriterFromFilename(const std::string& file)const;
%newobject ossimImageWriterFactoryRegistry::createWriter(const std::string& typeName)const;
class ossimImageWriterFactoryRegistry : public ossimObjectFactory
{
public:
   static ossimImageWriterFactoryRegistry* instance();
   ossimImageFileWriter *createWriterFromExtension(const ossimString& fileExtension)const;
   ossimImageFileWriter *createWriter(const ossimKeywordlist &kwl,
                                      const char *prefix=0)const;
   ossimImageFileWriter* createWriter(const ossimString& typeName)const;

   /**
    * getTypeNameList.  This should return the class type of the object being
    * used to perform the writting.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;

   /**
    * getImageTypeList.  This is the actual image type name.  So for
    * example, ossimTiffWriter has several image types.  Some of these
    * include TIFF_TILED, TIFF_TILED_BAND_SEPARATE ... etc.
    * The ossimGdalWriter
    * may include GDAL_IMAGINE_HFA, GDAL_RGB_NITF, GDAL_JPEG20000, ... etc
    * A writer should be able to be instantiated by this name as well as a
    * class name
    */
   virtual void getImageTypeList(std::vector<ossimString>& imageTypeList)const;
protected:
   ossimImageWriterFactoryRegistry();
   ossimImageWriterFactoryRegistry(const ossimImageWriterFactoryRegistry&);
   void operator=(const ossimImageWriterFactoryRegistry&);
};

%extend ossimImageWriterFactoryRegistry
{
public:
   ossimImageFileWriter* createWriterFromFilename(const std::string& file)
   {
      ossimFilename temp(file);
      ossimImageFileWriter* result = self->createWriterFromExtension(temp.ext());
      if(result)
      {
         result->setFilename(ossimFilename(file));
      }

      return result;
   }
   ossimImageFileWriter* createWriter(const std::string& typeName)const
   {
	return ossimImageWriterFactoryRegistry::instance()->createWriter(ossimString(typeName));
   }
}

%feature("notabstract") ossimMemoryImageSource;

class OSSIM_DLL ossimMemoryImageSource : public ossimImageSource
{
public:
   ossimMemoryImageSource();

   void setImage(ossimRefPtr<ossimImageData> image);
   void setImage(ossimScalarType scalarType,
                 ossim_uint32 numberOfBands,
                 ossim_uint32 width,
                 ossim_uint32 height);
   void setRect(ossim_uint32 ulx,
                ossim_uint32 uly,
                ossim_uint32 width,
                ossim_uint32 height);
};

/******************************************* PROJECTION INTERFACES ************************************************/
%newobject ossimProjection::dup()const;
class ossimProjection	 : public ossimObject
{
public:
   virtual ~ossimProjection() {}

   virtual ossimObject *dup()const=0;

   /*!
    * METHOD: origin()
    * Returns projection's ground point origin. That is the GP corresponding
    * to line=0, sample=0.
    */
   virtual ossimGpt origin()const=0;

   /*!
    * METHODS: forward(), reverse()
    * OBSOLETE -- provided for existing GUI code only. Bogus return value.
    */
   virtual ossimDpt forward(const ossimGpt &wp) const;  //inline below
   virtual ossimGpt inverse(const ossimDpt &pp) const;  //inline below

   /*!
    * METHOD: worldToLineSample()
    * Performs the forward projection from ground point to line, sample.
    */
   virtual void worldToLineSample(const ossimGpt& worldPoint,
                                  ossimDpt&       lineSampPt) const = 0;

   /*!
    * METHOD: lineSampleToWorld()
    * Performs the inverse projection from line, sample to ground (world):
    */
   virtual void lineSampleToWorld(const ossimDpt& lineSampPt,
                                  ossimGpt&       worldPt) const = 0;

   /*!
    * METHOD: lineSampleHeightToWorld
    * This is the pure virtual that projects the image point to the given
    * elevation above ellipsoid, thereby bypassing reference to a DEM. Useful
    * for projections that are sensitive to elevation (such as sensor models).
    */
   virtual void lineSampleHeightToWorld(const ossimDpt& lineSampPt,
                                        const double&   heightAboveEllipsoid,
                                        ossimGpt&       worldPt) const = 0;

   virtual void getRoundTripError(const ossimDpt& imagePoint,
                                  ossimDpt& errorResult)const;

   virtual void getRoundTripError(const ossimGpt& groundPoint,
                                  ossimDpt& errorResult)const;


   virtual void getGroundClipPoints(ossimGeoPolygon& gpts)const;
   /*!
    * METHODS:  saveState, loadState
    * Fulfills ossimObject base-class pure virtuals.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /*!
    * ACCESS METHODS:
    */
   virtual ossimDpt getMetersPerPixel() const=0;

   /**
    * @brief Pure virtual method to query if projection is affected by
    * elevation.
    * @return true if affected, false if not.
    */
   virtual bool isAffectedByElevation() const=0;
protected:
  ossimProjection();
};

%template( ossimProjectionRefPtr ) ossimRefPtr<ossimProjection>;
%template( ossimImageGeometryPtr ) ossimRefPtr<ossimImageGeometry>;

%extend ossimProjection
{
public:
   bool changeOrigin(const ossimGpt& origin, bool adjustTiePointsToOriginFlag=true)
   {
	bool result = false;
	ossimMapProjection* mapProj = dynamic_cast<ossimMapProjection*>(self);
	if(mapProj)
	{
	   mapProj->setOrigin(origin);
           if(adjustTiePointsToOriginFlag)
           {
	      mapProj->setUlTiePoints(origin);
           }
	   result = true;
	}

	return result;
   }
   bool changeGsd(const double& gsd, const ossimUnitType& unitType)
   {
	bool result = false;
	ossimMapProjection* mapProj = dynamic_cast<ossimMapProjection*>(self);
	if(mapProj)
	{
           ossimUnitConversionTool tool(gsd, unitType);
	   ossimDpt mpp(tool.getMeters(), tool.getMeters());
           mapProj->setMetersPerPixel(mpp);
	   result = true;
	}

	return result;
   }
};

%newobject ossimImageGeometry::dup()const;
class ossimImageGeometry : public ossimObject
{
public:
   //! Default constructor defaults to unity transform with no projection.
   ossimImageGeometry();
   ~ossimImageGeometry();


   //! Constructs with projection and transform objects available for referencing. Either pointer
   //! can be NULL -- the associated mapping would be identity.
   ossimImageGeometry(ossim2dTo2dTransform* transform, ossimProjection* projection);

   //! rnToRn is a utility method that takes a rn resolution image point and maps it to the another
   //! rn resolution image point.
   //!
   //! @param inRnPt Is a point in resolution n.
   //! @param inResolutionLevel Is the resolution of the point inRnPt.
   //! @param outResolutionLevel Is the resolution of the point outRnPt.
   //! @param outRnPt Is the result of the transform.
   //!
   void rnToRn(const ossimDpt& inRnPt, ossim_uint32 inResolutionLevel,
               ossim_uint32 outResolutionLevel,ossimDpt& outRnPt) const;


   //! rnToFull is a utility method that takes a rn resolution image point and maps it to the full
   //! image point.
   //!
   //! @param rnPt Is a point in resolution n.
   //! @param resolutionLevel Is the resolution of the point rnPt.  a value of 0 is the local image
   //! @param fullPt Is the result of the transform
   //!
   void rnToFull(const ossimDpt& rnPt, ossim_uint32 resolutionLevel, ossimDpt& fullPt) const;

   //! rnToWorld is a utility method that takes a rn resolution image point and maps it to the
   //! world point.
   //!
   //! @param rnPt Is a point in resolution n.
   //! @param resolutionLevel Is the resolution of the point rnPt.  a value of 0 is the local image
   //! @param wpt Is the result of the transform
   //!
   void rnToWorld(const ossimDpt& rnPt, ossim_uint32 resolutionLevel, ossimGpt& wpt);


   //! worldToRn is a utility method that takes a world point allows one to transform all the way back to
   //! an rn point.
   //!
   //! @param wpt Ground point.
   //! @param resolutionLevel Is the resolution of the point rnPt.  a value of 0 is the local image
   //! @param rnPt Is the resoltion point.
   //!
   void worldToRn(const ossimGpt& wpt, ossim_uint32 resolutionLevel, ossimDpt& rnPt);


   //! Exposes the 3D projection from image to world coordinates. The caller should verify that
   //! a valid projection exists before calling this method. Returns TRUE if a valid ground point
   //! is available in the ground_pt argument. This method depends on the existence of elevation
   //! information. If no DEM is available, the results will be incorrect or inaccurate.
   bool localToWorld(const ossimDpt& local_pt, ossimGpt& world_pt) const;

   //! Exposes the 3D world-to-local image coordinate reverse projection. The caller should verify
   //! that a valid projection exists before calling this method. Returns TRUE if a valid image
   //! point is available in the local_pt argument.
   bool worldToLocal(const ossimGpt& world_pt, ossimDpt& local_pt) const;

   //! Sets the transform to be used for local-to-full-image coordinate transformation
   void setTransform(ossim2dTo2dTransform* transform);

   //! Sets the projection to be used for local-to-world coordinate transformation
   void setProjection(ossimProjection* projection);

   //! Access methods for transform (may be NULL pointer).
   const ossim2dTo2dTransform* getTransform() const;
   //ossim2dTo2dTransform*       getTransform();

   //! Access methods for projection (may be NULL pointer).
   const ossimProjection* getProjection() const ;
   //ossimProjection*       getProjection();

   //! Returns TRUE if valid projection defined
   bool hasProjection() const;

   //! Returns TRUE if valid transform defined
   bool hasTransform() const;

   //! Returns TRUE if this geometry is sensitive to elevation
   bool isAffectedByElevation() const;

   //! Returns the GSD associated with this image in the active projection. Note that this only
   //! makes sense if there is a projection associated with the image. Returns NaNs if no
   //! projection defined.
  ossimDpt getMetersPerPixel() const;

   //! Prints contents to output stream.
   std::ostream& print(std::ostream& out) const;

   //! Returns the decimation factor from R0 for the resolution level specified. For r_index=0, the
   //! decimation factor is by definition 1.0. For the non-discrete case, r_index=1 returns a
   //! decimation of 0.5. If the vector of discrete decimation factors (m_decimationFactors) is
   //! empty, the factor will be computed as f=1/2^n
   ossimDpt decimationFactor(ossim_uint32 r_index) const;

   /**
    * @brief Method to get the decimation factor for a given resolution
    * level.
    *
    * If the array of decimations is not initialized by owner, the default is:
    * r_index=0 is by definition 1.0.
    * r_index=n Where n is some level the factor will be computed as f=1/2^n.
    *
    * @param resLevel Reduced resolution set for requested decimation.
    *
    * @param result ossimDpt to initialize with requested decimation.
    */
   void decimationFactor(ossim_uint32 r_index, ossimDpt& result) const;

   /**
    * @brief Gets array of all decimation levels.
    * @param decimations Array to initialiaze.
    */
   void decimationFactors(std::vector<ossimDpt>& decimations) const;

   //! Sets the decimation scheme to a discrete list of decimation factors.
   void setDiscreteDecimation(const std::vector<ossimDpt>& decimation_list)
      { m_decimationFactors = decimation_list; }

   //! @return The number of decimation factors
   ossim_uint32 getNumberOfDecimations()const;
   //! Creates a new instance of ossimImageGeometry with the same transform and projection.
   //! Overrides base-class version requiring loadState() and saveState() (not implemented yet)
   virtual ossimObject* dup() const;

   //! Attempts to initialize a transform and a projection given the KWL. Returns TRUE if no
   //! error encountered.
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);

   //! Saves the transform (if any) and projection (if any) states to the KWL.
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0) const;

};

%newobject ossimProjectionFactoryRegistry::createProjection(const ossimFilename& filename,
                                                            ossim_uint32 entryIdx)const;
%newobject ossimProjectionFactoryRegistry::createProjection(const ossimString& name)const;
%newobject ossimProjectionFactoryRegistry::createProjection(const ossimKeywordlist& kwl,
                                                            const char* prefix=0)const;
class ossimProjectionFactoryRegistry : public ossimObjectFactory
{
public:
	static ossimProjectionFactoryRegistry* instance();

   ossimProjection* createProjection(const ossimFilename& filename,
                                     ossim_uint32 entryIdx)const;
   ossimProjection* createProjection(const ossimString& name)const;
   ossimProjection* createProjection(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;

protected:
	ossimProjectionFactoryRegistry();
};


class ossimImageViewTransform
{
public:
   virtual bool isIdentity()const=0;
   virtual void imageToView(const ossimDpt& imagePoint,
                            ossimDpt&       viewPoint)const;
   virtual void viewToImage(const ossimDpt& viewPoint,
                           ossimDpt&       imagePoint)const;
   ossimDpt imageToView(const ossimDpt& imagePoint)const;
   ossimDpt viewToImage(const ossimDpt& viewPoint)const;

};


class ossimColumnVector3d
{
public:
  ossimColumnVector3d(double x, double y, double z=0);
};


class ossimPointObservation
{
public:
  ossimPointObservation();
  ossimPointObservation(const ossimString& anID);
  ossimPointObservation(const ossimGpt& aPt,
                        const ossimString& anID,
                        const ossimColumnVector3d& latLonHgtSigmas);
  void reset();
  void addMeasurement(const double& x,
                      const double& y,
                      const std::string& imgFile,
                      const ossimDpt& measSigma = ossimDpt(1.0,1.0));
  void addMeasurement(const ossimDpt& meas,
                      const ossimFilename& imgFile,
                      const ossimDpt& measSigma = ossimDpt(1.0,1.0));
  ossimDpt getMeasurement(const int index);
  void setGroundPoint(const ossimGpt& mPt);
  void setGroundPoint(const double& lat,
                      const double& lon,
                      const double& hgt);
  void setGroundSigmas(const double& latSig,
                       const double& lonSig,
                       const double& hgtSig);
  void setID(const std::string& anID);
};


class ossimImageViewAffineTransform : public ossimImageViewTransform
{
public:
   ossimImageViewAffineTransform(double rotateDegrees = 0.0,
                                 double imageScaleXValue = 1.0,
                                 double imageScaleYValue = 1.0,
                                 double scaleXValue = 1.0,
                                 double scaleYValue = 1.0,
                                 double translateXValue = 0.0,
                                 double translateYValue = 0.0,
                                 double pivotXValue = 0.0,
                                 double pivotYValue = 0.0);

   virtual bool isIdentity()const;
   virtual bool isValid()const;
   /*!
    * Translate in the x and y direction.
    */
   virtual void translate(double deltaX, double deltaY);

   /*!
    * Translate in the x direction.
    */
   virtual void translateX(double deltaX);

   /*!
    * Translate in the Y direction.
    */
   virtual void translateY(double deltaY);

   /*!
    * Translate the origin for rotation in the x and y direction.
    */
   virtual void pivot(double originX, double originY);

   /*!
    * Translate the origin for rotation in the x direction.
    */
   virtual void pivotX(double originX);

   /*!
    * Translate the origin for rotation in the y direction.
    */
   virtual void pivotY(double originY);

   /*!
    * will allow you to specify a scale
    * for both the x and y direction.
    */
   virtual void scale(double x, double y);

   /*!
    * will alow you to specify a scale
    * along the X direction.
    */
   virtual void scaleX(double x);

   /*!
    * Will allow you to scale along the Y
    * direction.
    */
   virtual void scaleY(double y);

   /*!
    * Will apply a rotation
    */
   virtual void rotate(double degrees);
};

namespace std
{
   %template(StringVector) vector<std::string>;
   %template(UintVector) vector<ossim_uint32>;
   %template(ossimGptVector) vector<ossimGpt>;
   %template(ossimStringVector) vector<ossimString>;
   %template(ossimDptVector) vector<ossimDpt>;
   %template(ossimKeywordlistVector) vector<ossimKeywordlist>;
   %template(ossimPropertyRefPtrVector) vector<ossimRefPtr<ossimProperty> >;
   %template(ossimConnectableObjectVector) vector<ossimRefPtr<ossimConnectableObject> >;
}

// Used for java class generation.

// %ignore oms::ImageData::ImageData(void* imageData=0);
%ignore oms::ImageData::getOssimImageData() const;
%ignore oms::ImageData::setOssimImageData(void *);
%ignore oms::ImageData::getBandBuffer(int) const;
%ignore oms::Source::getNativePointer()const;
%ignore oms::Projection::getNativePointer()const;
%ignore oms::ImageData::copyOssimImageDataBandToBuffer(ossim_uint8*, int);

// definition for data info return by reference
%apply int *OUTPUT { int *w, int *h };

%include <oms/Init.h>
%include <oms/DataInfo.h>
%include <oms/ElevMgr.h>
%include <oms/Info.h>
%include <oms/Keywordlist.h>
%include <oms/KeywordlistIterator.h>
%include <oms/Mosaic.h>
%include <oms/StringPair.h>
%include <oms/WmsMap.h>
%include <oms/ImageModel.h>
%include <oms/AdjustmentModel.h>
%include <oms/GeodeticEvaluator.h>
%include <oms/ImageStager.h>
%include <oms/Projection.h>
%include <oms/ImageData.h>
%include <oms/CoordinateUtility.h>
%include <oms/WktUtility.h>
%include <oms/SingleImageChain.h>
%include <oms/Chain.h>
%include <oms/Video.h>
%include <oms/Ephemeris.h>
%include <oms/MapProjection.h>
%include <oms/Envelope.h>
%include <oms/TileCacheSupport.h>

//---
// Begin std::map
// Used by oms::Chipper::initialize that takes a std::map<std::string, std::string>
// Must be before: %include <oms/Chipper.h>
//---
%typemap(jstype) std::map<std::string, std::string>& "java.util.Map<String,String>"
%typemap(javain,pre="    MapType temp$javainput = $javaclassname.convertMap($javainput);",pgcppname="temp$javainput") std::map<std::string, std::string>& "$javaclassname.getCPtr(temp$javainput)"
%typemap(javacode) std::map<std::string, std::string> %{
  static $javaclassname convertMap(java.util.Map<String,String> in) {
    $javaclassname out = new $javaclassname();
    for (java.util.Map.Entry<String, String> entry : in.entrySet()) {
      out.set(entry.getKey(), entry.getValue());      
    }
    return out;
  }    
%}
%template(MapType) std::map<std::string, std::string>;
// End: std::map

%include <oms/Chipper.h>
%include <oms/GpkgWriter.h>

%newobject  oms::Util::newEightBitImageSpaceThumbnailChain(ossimImageSource* inputSource,
                                                           int xRes,
                                                           int yRes,
                                                           const std::string& histogramFile,
                                                           const std::string& stretchType);
%include <oms/Util.h>


%include <oms/WmsView.h>



/*
%typemap(javaimports) InputStream
%{
import java.io.InputStream;
%}
%typemap(javabase) InputStream "InputStream";

%javaexception ( "java.io.IOException" ) close {
	try {
		$action
	} catch ( ... ) {
		jclass clazz = jenv->FindClass( "java/io/IOException" );
		jenv->ThrowNew( clazz, "An I/O Exception has occurred" );
		return $null;
	}
}


%javaexception ( "java.io.IOException" ) read {
	try {
		$action
	} catch ( ... ) {
		jclass clazz = jenv->FindClass( "java/io/IOException" );
		jenv->ThrowNew( clazz, "An I/O Exception has occurred" );
		return $null;
	}
}
*/


//%include <oms/InputStream.h>

/*
class InputStream
{
public:
	InputStream();
	int read();
	void close();
};
*/


%pragma(java) jniclasscode=%{
  static {
    try {
        System.loadLibrary("joms");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Native code library failed to load. \n" + e);
      //System.exit(1);
    }
  }
%}

%extend oms::ImageData
{
public:
   ossimImageData* getAsOssimImageData()
   {
      return (ossimImageData*)self->getOssimImageData();
   }
};
//%include <oms/Icp.h>


//%include "RasterEngine.h"






