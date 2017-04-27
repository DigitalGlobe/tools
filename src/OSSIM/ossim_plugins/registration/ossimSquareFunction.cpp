
#include "ossimSquareFunction.h"
#include <ossim/base/ossimIrect.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimMatrixProperty.h>

RTTI_DEF1(ossimSquareFunction, "ossimSquareFunction", ossimImageSourceFilter);

ossimSquareFunction::ossimSquareFunction(ossimObject* owner)
   :ossimImageSourceFilter(owner),
    theTile(NULL)
{
}

ossimSquareFunction::~ossimSquareFunction()
{
}

ossimRefPtr<ossimImageData> ossimSquareFunction::getTile(
   const ossimIrect& tileRect,
   ossim_uint32 resLevel)
{
   if(!theInputConnection)
   {
      return theTile;
   }

   if(!isSourceEnabled())
   {
      return theInputConnection->getTile(tileRect, resLevel);
   }

   ossimRefPtr<ossimImageData> data = theInputConnection->getTile(tileRect,
                                                                  resLevel);

   if(!data.valid() || !data->getBuf())
   {
      return data;
   }

    // First time through or after an initialize()...
   if (!theTile.valid())
   {
      allocate();
      if (!theTile.valid()) // Should never happen!
      {
         return data;
      }
   }

   // First time through, after an initialize() or a setKernel()...
   if (!theNullPixValue.size())
   {
      computeNullMinMax();
      if (!theNullPixValue.size()) // Should never happen!
      {
         return data;
      }
   }

   theTile->setImageRectangle(tileRect);
   theTile->makeBlank();
   
   switch(data->getScalarType())
   {
      case OSSIM_UINT8:
      { 
         applyFunction(static_cast<ossim_uint8>(0), data, theTile);
         break;
      }
      case OSSIM_SINT8:
      { 
         applyFunction(static_cast<ossim_sint8>(0), data, theTile);
         break;
      }
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
      {
         applyFunction(static_cast<ossim_uint16>(0), data, theTile);         
         break;
      }
      case OSSIM_SINT16:
      {
         applyFunction(static_cast<ossim_sint16>(0), data, theTile);         
         break;
      }
      case OSSIM_UINT32:
      {
         applyFunction(static_cast<ossim_uint32>(0), data, theTile);         
         break;
      }
      case OSSIM_SINT32:
      {
         applyFunction(static_cast<ossim_sint32>(0), data, theTile);         
         break;
      }
      case OSSIM_FLOAT32: 
      case OSSIM_NORMALIZED_FLOAT:
      {
         applyFunction(static_cast<ossim_float32>(0), data, theTile);         
         break;
      }
      case OSSIM_FLOAT64:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         applyFunction(static_cast<ossim_float64>(0), data, theTile);
         break;
      }
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimSquareFunction::getTile WARNING:\n"
            << "Scalar type = " << theTile->getScalarType()
            << " Not supported by ossimSquareFunction" << endl;
         break;
      }
   }
   theTile->validate();
   
   return theTile;
}


template<class T> 
void
ossimSquareFunction::applyFunction( T,
   ossimRefPtr<ossimImageData> inputData,
   ossimRefPtr<ossimImageData> outputData)
{
   // let's set up some temporary variables so we don't
   // have to call the functions in loops.  Iknow that compilers
   // typically optimize this out but if we are in debug mode 
   // with no optimization it will still run fast
   //  
   double v;
   ossim_uint32 outputW = outputData->getWidth();
   ossim_uint32 outputH = outputData->getHeight();
   for(ossim_uint32 band = 0; band < getNumberOfInputBands(); ++band)
   {
      T* inputBuf  = static_cast<T*>(inputData->getBuf(band));
      T* outputBuf = static_cast<T*>(outputData->getBuf(band));
      T maxPix     = static_cast<T>(getMaxPixelValue(band));
      T minPix     = static_cast<T>(getMinPixelValue(band));     
      T nullPix    = static_cast<T>(inputData->getNullPix(band));
      T oNullPix   = static_cast<T>(getNullPixelValue(band));
      
      if(inputBuf&&outputBuf)
      {
         for(ossim_uint32 row = 0; row < outputW; ++row)
         {
            for(ossim_uint32 col = 0; col < outputH; ++col)
            {

               if(*inputBuf != nullPix)
               {
                  v = *inputBuf * (*inputBuf);
                  if(v > maxPix)
                  {
                     *outputBuf = maxPix;
                  }
                  else if(v < minPix)
                  {
                     *outputBuf = minPix;
                  }
                  else
                  {
                     *outputBuf = static_cast<T>(v);
                  }
               }
               else {
                  *outputBuf = oNullPix;
               }
               ++inputBuf;
               ++outputBuf;
            }
         }
      }
   }
}


void ossimSquareFunction::initialize()
{
   //---
   // NOTE:
   // Since initialize get called often sequentially we will wipe things slick
   // but not reallocate to avoid multiple delete/allocates.
   //
   // On the first getTile call things will be reallocated/computed.
   //---
   theTile = NULL;
   clearNullMinMax();
}

void ossimSquareFunction::allocate()
{   
   if(theInputConnection)
   {
      ossimImageDataFactory* idf = ossimImageDataFactory::instance();
      
      theTile = idf->create(this,
                            this);
      
      theTile->initialize();
   }
}

double ossimSquareFunction::getNullPixelValue(ossim_uint32 band)const
{
   if( isSourceEnabled() && (band < theMinPixValue.size()) )
   {
      return theNullPixValue[band];
   }
   return ossimImageSource::getNullPixelValue(band);
}

double ossimSquareFunction::getMinPixelValue(ossim_uint32 band)const
{
   if( isSourceEnabled() && (band < theMinPixValue.size()) )
   {
      return theMinPixValue[band];
   }   
   return ossimImageSource::getMinPixelValue(band);
}

double ossimSquareFunction::getMaxPixelValue(ossim_uint32 band)const
{
   if( isSourceEnabled() && (band < theMaxPixValue.size()) )
   {
      return theMaxPixValue[band];
   }
   return ossimImageSource::getMaxPixelValue(band);
}

void ossimSquareFunction::clearNullMinMax()
{
   theNullPixValue.clear();
   theMinPixValue.clear();
   theMaxPixValue.clear();
}

void ossimSquareFunction::computeNullMinMax()
{
   const ossim_uint32 BANDS = getNumberOfOutputBands();

   theNullPixValue.resize(BANDS);
   theMinPixValue.resize(BANDS);
   theMaxPixValue.resize(BANDS);

   ossim_float64 defaultNull = ossim::defaultNull(getOutputScalarType());
   ossim_float64 defaultMin = ossim::defaultMin(getOutputScalarType());
   ossim_float64 defaultMax = ossim::defaultMax(getOutputScalarType());
  
   ossimImageSource* input  = PTR_CAST(ossimImageSource, getInput(0));
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      if(theInputConnection)
      {
         double span[2] = { input->getMinPixelValue(band)  , input->getMaxPixelValue(band)  };
         double v;
         double minv = defaultMax; //crossed min-max
         double maxv = defaultMin;
         for (int l=0;l<2;++l)
         {
            for (int r=0;r<2;++r)
            {
               v = span[l] * span[r];
               if (v < minv) minv=v;
               if (v > maxv) maxv=v;
            }
         }         
         theMinPixValue[band] = (minv>defaultMin) ? minv : defaultMin ;
         theMaxPixValue[band] = (maxv<defaultMax) ? maxv : defaultMax ;
      }
      else // No input connection...
      {    
         theMinPixValue[band]  = defaultMin;
         theMaxPixValue[band]  = defaultMax;
      }
      //always default for null
      theNullPixValue[band] = defaultNull;
      
   } // End of band loop.
}

