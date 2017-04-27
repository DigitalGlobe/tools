#ifndef omsImageData_HEADER
#define omsImageData_HEADER 1

#include <oms/Constants.h>
#include <oms/Object.h>
#include <string>
#include <ossim/base/ossimIpt.h>

namespace oms
{
   class OMSDLL ImageData : public oms::Object
   {
   public:
      class PrivateData;
		
      ImageData(void* imageData=0);
		
      virtual ~ImageData();
      void setOssimImageData(ossim_uint64 width, ossim_uint64 height, ossim_uint32 bands, ossimScalarType scalarType);
      /**
       * This should be a pointer of type ossimImageData.  It will set an internal variable that is an ossimRefPtr<ossimImageData> 
       */
      void setOssimImageData(void* imageData);
      void* getOssimImageData();
      const void* getOssimImageData()const;
		
      void* getBandBuffer(int idx);
      const void* getBandBuffer(int idx)const;
		
      bool isBlank()const;

      void makeBlank();
      /**
       * Will return the byte size for the band array
       */
      int getBandSizeInBytes()const;
		
      /**
       * Returns the scalar type for the band array. Please see ossimConstants.h for a complete list of the current scalar types supported
       */
      ossimScalarType getOssimScalarType()const;
		
      /**
       * Returns the bit size of the scalar type.  So if it is a ossim_uint8 then the bits returned
       * is 8 and for osism_uint32 it is 32.
       */
      int getScalarTypeBitSize()const;
		
      /**
       * Returns the number of pixel/data elements for the band.
       */
      int getNumberOfElementsInEachBand()const;
      int getTotalNumberOfElements()const;
      int getNumberOfBands()const;
		
      ossimIpt getOrigin()const;
		
      /****************** Native one-to-one copy****************/
      void copyOssimImageDataBandToBuffer(ossim_int8* destinationBuffer, int bandIdx);
      void copyOssimImageDataBandToBuffer(ossim_uint8* destinationBuffer, int bandIdx);
      void copyOssimImageDataBandToBuffer(ossim_sint16* destinationBuffer, int bandIdx);
      void copyOssimImageDataBandToBuffer(ossim_uint16* destinationBuffer, int bandIdx);
      void copyOssimImageDataBandToBuffer(ossim_float32* destinationBuffer, int bandIdx);
      void copyOssimImageDataBandToBuffer(ossim_float64* destinationBuffer, int bandIdx);
		

      void copyJava4ByteAlphaToOssimImageDataBuffer(ossim_int8* srcBufferPtr, int* bandOffsets);    
      //void copyJavaIntARGBToOssimImageDataBuffer(ossim_uint8* srcBufferPtr);    
   protected:
      PrivateData* thePrivateData;
   };
}

#endif
