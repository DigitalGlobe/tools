// Copyright (C) 2006 Apogee Imaging International. 
//
//  OSSIM LGPL.  See top level LICENSE.txt in the ossim repository
//  
// Author: Tisham Dhar (tisham@apogee.com.au)
// Description: Implementation of the CFAR Edge Detector as a series 
// of convolutions and thresholding
//
//*************************************************************************
// $Id: ossimCFARFilter.cpp 13026 2008-06-13 17:07:35Z sbortman $
//
#include <cstdlib>
#include <ossim/base/ossimRefPtr.h>
#include "ossimCFARFilter.h"
#include <ossim/imaging/ossimU8ImageData.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimMatrixProperty.h>
#include <ossim/base/ossimContainerProperty.h>
#include <ossim/base/ossimNumericProperty.h>



   //TODO: Implement the tile wide CFAR algorithm here
   //Need to consider different thresholds across the
   //Image for ScanSAR modes and WideSwath Modes

   //CFAR Thresholds k-values determined by analysis of
   //Image windows.

	//Refer to ossim3x3ConvolutionFilter - for manual implementation of convolution
	//Refer to ossimNCC_FFTW - for FFTW3 based optimized implementation of convolution
	
   //Step 1: Find Mean with a small window use convolution
   //Step 2: Find Mean with a large window with a hole in the middle
   //Step 3: Find Std.dev. of the large window - std. dev = sqrt(mean of squares - square of means)
   //So square and convolve again to obtain mean
   //Step 4: Find k-statistics using formula per pixel
   //Step 5: Threshold k-statistics

RTTI_DEF1(ossimCFARFilter, "ossimCFARFilter", ossimImageSourceFilter);

ossimCFARFilter::ossimCFARFilter(ossimObject* owner)
   :ossimImageSourceFilter(owner),
    theTile(NULL),
    theThreshold(2.0),
    theNullPixValue(0),
    theMinPixValue(0),
    theMaxPixValue(0)
{
   theKernel[0][0] = 0.0625; theKernel[0][1] = 0.0625; theKernel[0][2] = 0.0625;
   theKernel[0][3] = 0.0625; theKernel[0][4] = 0.0625;
   theKernel[1][0] = 0.0625; theKernel[1][1] = 0.0; theKernel[1][2] = 0.0;
   theKernel[1][3] = 0.0;    theKernel[1][4] = 0.0625;
   theKernel[2][0] = 0.0625; theKernel[2][1] = 0.0; theKernel[2][2] = 0.0;
   theKernel[2][3] = 0.0;theKernel[2][4] = 0.0625;
   theKernel[3][0] = 0.0625; theKernel[3][1] = 0.0; theKernel[3][2] = 0.0;
   theKernel[3][3] = 0.0;theKernel[3][4] = 0.0625;
   theKernel[4][0] = 0.0625; theKernel[4][1] = 0.0625; theKernel[4][2] = 0.0625;
   theKernel[4][3] = 0.0625;theKernel[4][4] = 0.0625;
   
}

ossimCFARFilter::~ossimCFARFilter()
{
}

ossimRefPtr<ossimImageData> ossimCFARFilter::getTile(const ossimIrect& tileRect,
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

   //---
   // We have a 5x5 matrix so stretch the rect out to cover
   // the required pixels.  We only need 2 pixels to the left
   // and right of the center pixel.
   //---
   ossimIrect newRect(ossimIpt(tileRect.ul().x - 2,
                               tileRect.ul().y - 2),
                      ossimIpt(tileRect.lr().x + 2,
                               tileRect.lr().y + 2));
   
   ossimRefPtr<ossimImageData> data = theInputConnection->getTile(newRect,
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
      case OSSIM_UCHAR:
      {
         if(data->getDataObjectStatus() == OSSIM_FULL)
         {
            convolveFull(static_cast<ossim_uint8>(0), data, theTile);
         }
         else
         {
            convolvePartial(static_cast<ossim_uint8>(0), data, theTile);
         }
         break;
      }
      case OSSIM_FLOAT: 
      case OSSIM_NORMALIZED_FLOAT:
      {
         if(data->getDataObjectStatus() == OSSIM_FULL)
         {
            convolveFull(static_cast<float>(0), data, theTile);
         }
         else
         {
            convolvePartial(static_cast<float>(0), data, theTile);
         }
         break;
      }
      case OSSIM_USHORT16:
      case OSSIM_USHORT11:
      {
         if(data->getDataObjectStatus() == OSSIM_FULL)
         {
            convolveFull(static_cast<ossim_uint16>(0), data, theTile);
         }
         else
         {
            convolvePartial(static_cast<ossim_uint16>(0), data, theTile);
         }
         break;
      }
      case OSSIM_SSHORT16:
      {
         if(data->getDataObjectStatus() == OSSIM_FULL)
         {
            convolveFull(static_cast<ossim_sint16>(0), data, theTile);
         }
         else
         {
            convolvePartial(static_cast<ossim_sint16>(0), data, theTile);
         }
         break;
      }
      case OSSIM_DOUBLE:
      case OSSIM_NORMALIZED_DOUBLE:
      {
         if(data->getDataObjectStatus() == OSSIM_FULL)
         {
            convolveFull(static_cast<double>(0), data, theTile);
      }
      else
      {
         convolvePartial(static_cast<double>(0), data, theTile);
      }
         break;
      }
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "ossimCFARFilter::getTile WARNING:\n"
            << "Scalar type = " << theTile->getScalarType()
            << " Not supported by ossimCFARFilter" << endl;
         break;
      }
   }
   theTile->validate();
   
   return theTile;
}


template<class T> void ossimCFARFilter::convolvePartial(
   T,
   ossimRefPtr<ossimImageData> inputData,
   ossimRefPtr<ossimImageData> outputData)
{
   // let's set up some temporary variables so we don't
   // have to call the functions in loops.  Iknow that compilers
   // typically optimize this out but if we are in debug mode 
   // with no optimization it will still run fast
   //
   double sum = 0.0,sqrsum = 0.0, variance = 0.0;
   ossim_int32 inputW          = (ossim_int32)inputData->getWidth();
   ossim_int32 outputW         = (ossim_int32)outputData->getWidth();
   ossim_int32 outputH         = (ossim_int32)outputData->getHeight();
   ossim_int32 numberOfBands   = (ossim_int32)inputData->getNumberOfBands();
   ossimIpt outputOrigin  = outputData->getOrigin();
   ossimIpt inputOrigin   = inputData->getOrigin();
   
   ossim_int32 startInputOffset = std::abs(outputOrigin.y - inputOrigin.y)*
      inputW + std::abs(outputOrigin.x - inputOrigin.x);
      
   ossim_int32 ulKernelStart    = -(2*inputW) - 2;
   ossim_int32 ul1KernelStart    = -inputW - 1;
   ossim_int32 leftKernelStart  = -2;
   ossim_int32 ll1KernelStart    = inputW  - 1;
   ossim_int32 llKernelStart    = (2*inputW)  - 2;
   
   //populate kernel offset indices
   
   ossim_int32 KernelStart[BOXSIZE];
   T* KernelStartBuf[BOXSIZE];
   
   for(ossim_uint16 i=0;i<BOXSIZE;i++)
   {
      int offset = i-(BOXSIZE/2);
   	KernelStart[i] = offset*inputW + offset;
   	KernelStartBuf[i] = NULL;
   }
   
   
   
   T* ulKernelStartBuf   = NULL;
   T* ul1KernelStartBuf   = NULL;
   T* leftKernelStartBuf = NULL;
   T* ll1KernelStartBuf   = NULL;   
   T* llKernelStartBuf   = NULL;
   
   for(ossim_int32 band = 0; band < numberOfBands; ++band)
   {
      T* inputBuf  = static_cast<T*>(inputData->getBuf(band))+startInputOffset;
      T* outputBuf = static_cast<T*>(outputData->getBuf(band));
      T maxPix     = static_cast<T>(getMaxPixelValue(band));
      T minPix     = static_cast<T>(getMinPixelValue(band));      
      T nullPix    = static_cast<T>(inputData->getNullPix(band));
      T oNullPix   = static_cast<T>(getNullPixelValue(band));
      
      if(inputBuf&&outputBuf)
      {
         for(ossim_int32 row = 0; row < outputW; ++row)
         {
            ossim_int32 rowOffset    = inputW*row;
            ulKernelStartBuf   = inputBuf + (rowOffset + ulKernelStart);
            ul1KernelStartBuf   = inputBuf + (rowOffset + ul1KernelStart);
            leftKernelStartBuf = inputBuf + (rowOffset + leftKernelStart);
            ll1KernelStartBuf   = inputBuf + (rowOffset + ll1KernelStart);
            llKernelStartBuf   = inputBuf + (rowOffset + llKernelStart);
            
            for(ossim_uint16 i=0;i<BOXSIZE;i++)
            {
            	KernelStartBuf[i] = inputBuf + (rowOffset + KernelStart[i]);
            }
                        
            for(ossim_int32 col = 0; col < outputH; ++col)
            {
					//TODO: Need to have an efficient means of detecting nulls
					
               if((ulKernelStartBuf[0]   != nullPix)&&
                  (ulKernelStartBuf[1]   != nullPix)&&
                  (ulKernelStartBuf[2]   != nullPix)&&
                  (leftKernelStartBuf[0] != nullPix)&&
                  (leftKernelStartBuf[1] != nullPix)&&
                  (leftKernelStartBuf[2] != nullPix)&&
                  (llKernelStartBuf[0]   != nullPix)&&
                  (llKernelStartBuf[1]   != nullPix)&&
                  (llKernelStartBuf[2]   != nullPix))
               {
               	//calculate mean
               	sum = 0.0;
               	sqrsum = 0.0;
               	
               	
            		for(ossim_uint32 r=0; r<5; ++r)
            			sum += theKernel[0][r]*(double)ulKernelStartBuf[r];
            		for(ossim_uint32 r=0; r<5; ++r)
            			sum += theKernel[1][r]*(double)ul1KernelStartBuf[r];
            		for(ossim_uint32 r=0; r<5; ++r)
            			sum += theKernel[2][r]*(double)leftKernelStartBuf[r];
            		for(ossim_uint32 r=0; r<5; ++r)
            			sum += theKernel[3][r]*(double)ll1KernelStartBuf[r];
	            	for(ossim_uint32 r=0; r<5; ++r)
   	         		sum += theKernel[4][r]*(double)llKernelStartBuf[r];
   	         	
   	         	/*
   	         	for(ossim_uint16 i=0;i<BOXSIZE;i++)
	   	         {
	   	         	for(ossim_uint32 r=0; r<5; ++r)
	   	         	{
								sum += theKernel[i][r]*(double)KernelStartBuf[i][r];
								sqrsum += theKernel[i][r]*(double)KernelStartBuf[i][r]
         	   			*(double)KernelStartBuf[i][r];
         	   		}
   	      	   }
   	      	   */  	         	
   	         	//calculate mean of squares
   	         	
   	         	for(ossim_uint32 r=0; r<5; ++r)
            			sqrsum += theKernel[0][r]*(double)ulKernelStartBuf[r]
            			*(double)ulKernelStartBuf[r];
            		for(ossim_uint32 r=0; r<5; ++r)
            			sqrsum += theKernel[1][r]*(double)ul1KernelStartBuf[r]
            			*(double)ul1KernelStartBuf[r];
            		for(ossim_uint32 r=0; r<5; ++r)
            			sqrsum += theKernel[2][r]*(double)leftKernelStartBuf[r]
            			*(double)leftKernelStartBuf[r];
            		for(ossim_uint32 r=0; r<5; ++r)
            			sqrsum += theKernel[3][r]*(double)ll1KernelStartBuf[r]
            			*(double)ll1KernelStartBuf[r];
            		for(ossim_uint32 r=0; r<5; ++r)
            			sqrsum += theKernel[4][r]*(double)llKernelStartBuf[r]
            			*(double)llKernelStartBuf[r];
            		
						//calculate variance
						variance = sqrsum - (sum*sum);
					
						
						//calculate k-value
						sum = ((double)leftKernelStartBuf[2] - sum)/sqrt(variance);;
					
						//Threshold k-value
						if(sum < theThreshold)
							sum = minPix;
						else
							sum = maxPix;
						
                  /*
                  sum = theKernel[0][0]*(double)ulKernelStartBuf[0] +
                        theKernel[0][1]*(double)ulKernelStartBuf[1] +
                        theKernel[0][2]*(double)ulKernelStartBuf[2] +
                        theKernel[1][0]*(double)leftKernelStartBuf[0] +
                        theKernel[1][1]*(double)leftKernelStartBuf[1] +
                        theKernel[1][2]*(double)leftKernelStartBuf[2] +
                        theKernel[2][0]*(double)llKernelStartBuf[0] +
                        theKernel[2][1]*(double)llKernelStartBuf[1] +
                        theKernel[2][2]*(double)llKernelStartBuf[2];
                  */
                  if(sum > maxPix)
                  {
                     *outputBuf = maxPix;
                  }
                  else if(sum < minPix)
                  {
                     *outputBuf = minPix;
                  }
                  else
                  {
                     *outputBuf = static_cast<T>(sum);
                  }
               }
               else {
                  *outputBuf = oNullPix;
               }
               //
               // Need to implement the convolution here
               //
               
               ++ulKernelStartBuf;
               ++ul1KernelStartBuf;
               ++leftKernelStartBuf;
               ++ll1KernelStartBuf;
               ++llKernelStartBuf;
               ++outputBuf;
            }
         }
      }
   }
}

template<class T> void ossimCFARFilter::convolveFull(
   T,
   ossimRefPtr<ossimImageData> inputData,
   ossimRefPtr<ossimImageData> outputData)
{
   // let's set up some temporary variables so we don't
   // have to call the functions in loops.  Iknow that compilers
   // typically optimize this out but if we are in debug mode 
   // with no optimization it will still run fast
   //
   double sum = 0.0,sqrsum = 0.0,variance = 0.0;
   ossim_int32 inputW = static_cast<ossim_int32>(inputData->getWidth());
   ossim_uint32 outputW       = outputData->getWidth();
   ossim_uint32 outputH       = outputData->getHeight();
   ossim_uint32 numberOfBands = inputData->getNumberOfBands();
   ossimIpt outputOrigin = outputData->getOrigin();
   ossimIpt inputOrigin  = inputData->getOrigin();
   
   ossim_int32 startInputOffset = std::abs(outputOrigin.y - inputOrigin.y)*
      inputW + std::abs(outputOrigin.x - inputOrigin.x);
   
   ossim_int32 ulKernelStart    = -(2*inputW) - 2;
   ossim_int32 ul1KernelStart    = -inputW - 1;
   ossim_int32 leftKernelStart  = -2;
   ossim_int32 ll1KernelStart    = inputW  - 1;
   ossim_int32 llKernelStart    = (2*inputW)  - 2;
   
   //populate kernel offset indices
   
   ossim_int32 KernelStart[BOXSIZE];
   T* KernelStartBuf[BOXSIZE];
   
   for(ossim_uint16 i=0;i<BOXSIZE;i++)
   {
   	int offset = i-(BOXSIZE/2);
   	KernelStart[i] = offset*inputW + offset;
   	KernelStartBuf[i] = NULL;
   }
   
   T* ulKernelStartBuf   = NULL;
   T* ul1KernelStartBuf   = NULL;
   T* leftKernelStartBuf = NULL;
   T* ll1KernelStartBuf   = NULL;   
   T* llKernelStartBuf   = NULL;
   
   for(ossim_uint32 band = 0; band < numberOfBands; ++band)
   {
      T* inputBuf  = static_cast<T*>(inputData->getBuf(band))+startInputOffset;
      T* outputBuf = static_cast<T*>(outputData->getBuf(band));
      T maxPix     = static_cast<T>(getMaxPixelValue(band));
      T minPix     = static_cast<T>(getMinPixelValue(band));
      
      if(inputBuf&&outputBuf)
      {
         for(ossim_uint32 row = 0; row < outputW; ++row)
         {
            ossim_int32 rowOffset    = inputW*row;
            ulKernelStartBuf   = inputBuf + (rowOffset + ulKernelStart);
            ul1KernelStartBuf   = inputBuf + (rowOffset + ul1KernelStart);
            leftKernelStartBuf = inputBuf + (rowOffset + leftKernelStart);
            ll1KernelStartBuf   = inputBuf + (rowOffset + ll1KernelStart);
            llKernelStartBuf   = inputBuf + (rowOffset + llKernelStart);
            
            for(ossim_uint16 i=0;i<BOXSIZE;i++)
            {
            	KernelStartBuf[i] = inputBuf + (rowOffset + KernelStart[i]);
            }
            
            for(ossim_uint32 col = 0; col < outputH; ++col)
            {
            	//calculate mean
            	sum = 0.0;
            	sqrsum = 0.0;
            	
            	for(ossim_uint32 r=0; r<5; ++r)
            		sum += theKernel[0][r]*(double)ulKernelStartBuf[r];
            	for(ossim_uint32 r=0; r<5; ++r)
            		sum += theKernel[1][r]*(double)ul1KernelStartBuf[r];
            	for(ossim_uint32 r=0; r<5; ++r)
            		sum += theKernel[2][r]*(double)leftKernelStartBuf[r];
            	for(ossim_uint32 r=0; r<5; ++r)
            		sum += theKernel[3][r]*(double)ll1KernelStartBuf[r];
            	for(ossim_uint32 r=0; r<5; ++r)
            		sum += theKernel[4][r]*(double)llKernelStartBuf[r];
            	/*
            	for(ossim_uint16 i=0;i<BOXSIZE;i++)
	   	      {
	   	         for(ossim_uint32 r=0; r<5; ++r)
	   	         {
							sum += theKernel[i][r]*(double)KernelStartBuf[i][r];
							sqrsum += theKernel[i][r]*(double)KernelStartBuf[i][r]
         	   		*(double)KernelStartBuf[i][r];
         	   	}
   	      	}
   	      	*/
            	
            	//calculate mean of squares
            	
            	for(ossim_uint32 r=0; r<5; ++r)
            		sqrsum += theKernel[0][r]*(double)ulKernelStartBuf[r]
            		*(double)ulKernelStartBuf[r];
            	for(ossim_uint32 r=0; r<5; ++r)
            		sqrsum += theKernel[1][r]*(double)ul1KernelStartBuf[r]
            		*(double)ul1KernelStartBuf[r];
            	for(ossim_uint32 r=0; r<5; ++r)
            		sqrsum += theKernel[2][r]*(double)leftKernelStartBuf[r]
            		*(double)leftKernelStartBuf[r];
            	for(ossim_uint32 r=0; r<5; ++r)
            		sqrsum += theKernel[3][r]*(double)ll1KernelStartBuf[r]
            		*(double)ll1KernelStartBuf[r];
            	for(ossim_uint32 r=0; r<5; ++r)
            		sqrsum += theKernel[4][r]*(double)llKernelStartBuf[r]
            		*(double)llKernelStartBuf[r];
					
					//calculate variance
					variance = sqrsum - (sum*sum);
					
					//calculate k-value
					sum = ((double)leftKernelStartBuf[2] - sum)/sqrt(variance);
					
					//Threshold k-value
					if(sum < theThreshold)
						sum = minPix;
					else
						sum = maxPix;
					
					/*            
               sum = theKernel[0][0]*(double)ulKernelStartBuf[0] +
                     theKernel[0][1]*(double)ulKernelStartBuf[1] +
                     theKernel[0][2]*(double)ulKernelStartBuf[2] +
                     theKernel[1][0]*(double)leftKernelStartBuf[0] +
                     theKernel[1][1]*(double)leftKernelStartBuf[1] +
                     theKernel[1][2]*(double)leftKernelStartBuf[2] +
                     theKernel[2][0]*(double)llKernelStartBuf[0] +
                     theKernel[2][1]*(double)llKernelStartBuf[1] +
                     theKernel[2][2]*(double)llKernelStartBuf[2];
               */
               if(sum > maxPix)
               {
                  *outputBuf = maxPix;
               }
               else if(sum < minPix)
               {
                  *outputBuf = minPix;
               }
               else
               {
                  *outputBuf = static_cast<T>(sum);
               }
               //
               // Need to implement the convolution here.
               //

               
               ++ulKernelStartBuf;
               ++ul1KernelStartBuf;
               ++leftKernelStartBuf;
               ++ll1KernelStartBuf;
               ++llKernelStartBuf;
               ++outputBuf;
            }
         }
      }
   }
}

void ossimCFARFilter::initialize()
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

void ossimCFARFilter::allocate()
{   
   if(theInputConnection)
   {
      ossimImageDataFactory* idf = ossimImageDataFactory::instance();
      
      theTile = idf->create(this,
                            this);
      
      theTile->initialize();
   }
}

void ossimCFARFilter::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property) return;
   ossimString name = property->getName();

	if(name == "threshold")
	{
		theThreshold = property->valueToString().toDouble();
	}
   else if(name == "Kernel")
   {
      ossimMatrixProperty* matrixProperty = PTR_CAST(ossimMatrixProperty,
                                                     property.get());
      if(matrixProperty)
      {
      	for(ossim_int32 r = 0; r < 5; r++)
   		{
      		for(ossim_int32 c = 0; c < 5; c++)
      		{
      			theKernel[r][c] = (*matrixProperty)(r,c);
      		}
   		}
   		/*
         theKernel[0][0] = (*matrixProperty)(0,0);
         theKernel[1][0] = (*matrixProperty)(1,0);
         theKernel[2][0] = (*matrixProperty)(2,0);
         theKernel[0][1] = (*matrixProperty)(0,1);
         theKernel[1][1] = (*matrixProperty)(1,1);
         theKernel[2][1] = (*matrixProperty)(2,1);
         theKernel[0][2] = (*matrixProperty)(0,2);
         theKernel[1][2] = (*matrixProperty)(1,2);
         theKernel[2][2] = (*matrixProperty)(2,2);
			*/
      }
      else
      {
         ossimImageSourceFilter::setProperty(property);
      }
   }
   else
   {
      ossimImageSourceFilter::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimCFARFilter::getProperty(const ossimString& name)const
{
   if(name == "Kernel")
   {
      ossimMatrixProperty* property = new ossimMatrixProperty(name);
      property->resize(5,5);
      for(ossim_int32 r = 0; r < 5; r++)
   	{
      	for(ossim_int32 c = 0; c < 5; c++)
      	{
      		(*property)(r,c) = theKernel[r][c];
      	}
   	}
   	/*
      (*property)(0,0) = theKernel[0][0];
      (*property)(1,0) = theKernel[1][0];
      (*property)(2,0) = theKernel[2][0];
      (*property)(3,0) = theKernel[3][0];
      (*property)(4,0) = theKernel[4][0];
      (*property)(0,1) = theKernel[0][1];
      (*property)(1,1) = theKernel[1][1];
      (*property)(2,1) = theKernel[2][1];
      (*property)(3,1) = theKernel[3][1];
      (*property)(4,1) = theKernel[4][1];
      (*property)(0,2) = theKernel[0][2];
      (*property)(1,2) = theKernel[1][2];
      (*property)(2,2) = theKernel[2][2];
      (*property)(3,2) = theKernel[3][2];
      (*property)(4,2) = theKernel[4][2];
      (*property)(0,3) = theKernel[0][3];
      (*property)(1,3) = theKernel[1][3];
      (*property)(2,3) = theKernel[2][3];
      (*property)(3,3) = theKernel[3][3];
      (*property)(4,3) = theKernel[4][3];
      (*property)(0,4) = theKernel[0][4];
      (*property)(1,4) = theKernel[1][4];
      (*property)(2,4) = theKernel[2][4];
      (*property)(3,4) = theKernel[3][4];
      (*property)(4,4) = theKernel[4][4];
      */
      property->setCacheRefreshBit();

      return property;
   }
   if(name == "threshold")
   {
   	 ossimNumericProperty* numeric = new ossimNumericProperty(name,
                                                               ossimString::toString(getThreshold()),
                                                               -10.0, 10.0);
      numeric->setNumericType(ossimNumericProperty::ossimNumericPropertyType_FLOAT64);
      numeric->setCacheRefreshBit();
      return numeric;
   }
   return ossimImageSourceFilter::getProperty(name);
}

void ossimCFARFilter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSourceFilter::getPropertyNames(propertyNames);
   propertyNames.push_back("Kernel");
   propertyNames.push_back("threshold");
}


/***************************************************************
* This method saves the filter state between property box edits
***************************************************************/
bool ossimCFARFilter::saveState(ossimKeywordlist& kwl,
                                          const char* prefix)const
{   
   kwl.add(prefix,
           "rows",
           5,
           true);
   
   kwl.add(prefix,
           "cols",
           5,
           true);
   
   for(ossim_int32 row = 0; row < 5; ++row)
   {
      for(ossim_int32 col =0; col < 5; ++col)
      {
         ossimString newPrefix = "m" +
                                 ossimString::toString(row+1) + "_" +
                                 ossimString::toString(col+1);
         kwl.add(prefix,
                 newPrefix,
                 theKernel[row][col],
                 true);          
      }
   }   
   
	kwl.add(prefix,
           "threshold",
           theThreshold,
           true);
   
   return ossimImageSourceFilter::saveState(kwl, prefix);
}

/***************************************************************
* This method loads the filter state between property box edits
***************************************************************/
bool ossimCFARFilter::loadState(const ossimKeywordlist& kwl,
                                          const char* prefix)
{
   ossimString newPrefix = prefix;
   newPrefix += ossimString("m");
   
   for(ossim_int32 r = 0; r < 5; r++)
   {
      for(ossim_int32 c = 0; c < 5; c++)
      {
         theKernel[r][c] = 0.0;
         
         ossimString value = ossimString::toString(r+1);
         value += "_";
         value += ossimString::toString(c+1);
         
         const char* v = kwl.find(newPrefix.c_str(),
                                  value.c_str());
         if(v)
         {
            theKernel[r][c] = ossimString(v).toDouble();
         }
      }
   }
   const char* threshold = kwl.find(prefix, "threshold");
   if(threshold)
   {
      theThreshold = ossimString(threshold).toDouble();
   }
   
   return ossimImageSourceFilter::loadState(kwl, prefix);
}

void ossimCFARFilter::getKernel(double kernel[5][5])
{

   for(ossim_int32 r = 0; r < 5; r++)
   {
      for(ossim_int32 c = 0; c < 5; c++)
      {
      	kernel[r][c] = theKernel[r][c];
      }
   }
   /*
   kernel[0][0] = theKernel[0][0];
   kernel[0][1] = theKernel[0][1];
   kernel[0][2] = theKernel[0][2];
   kernel[1][0] = theKernel[1][0];
   kernel[1][1] = theKernel[1][1];
   kernel[1][2] = theKernel[1][2];
   kernel[2][0] = theKernel[2][0];
   kernel[2][1] = theKernel[2][1];
   kernel[2][2] = theKernel[2][2];
   */
}

void ossimCFARFilter::setKernel(double kernel[5][5])
{
	for(ossim_int32 r = 0; r < 5; r++)
   {
      for(ossim_int32 c = 0; c < 5; c++)
      {
      	theKernel[r][c] = kernel[r][c];
      }
   }
   /*
   theKernel[0][0] = kernel[0][0];
   theKernel[0][1] = kernel[0][1];
   theKernel[0][2] = kernel[0][2];
   theKernel[1][0] = kernel[1][0];
   theKernel[1][1] = kernel[1][1];
   theKernel[1][2] = kernel[1][2];
   theKernel[2][0] = kernel[2][0];
   theKernel[2][1] = kernel[2][1];
   theKernel[2][2] = kernel[2][2];
	*/
   // Will be recomputed first getTile call.
   clearNullMinMax();
}

ossim_float64 ossimCFARFilter::getThreshold()const
{
	return theThreshold;
}


void ossimCFARFilter::setThreshold(ossim_float64 threshold)
{
	theThreshold = threshold;
}

double ossimCFARFilter::getNullPixelValue(ossim_uint32 band)const
{
   if( isSourceEnabled() && (band < theNullPixValue.size()) )
   {
      return theNullPixValue[band];
   }

   return ossim::defaultNull(getOutputScalarType());
}

double ossimCFARFilter::getMinPixelValue(ossim_uint32 band)const
{
   if( isSourceEnabled() && (band < theMinPixValue.size()) )
   {
      return theMinPixValue[band];
   }
   
   return ossimImageSource::getMinPixelValue(band);
}

double ossimCFARFilter::getMaxPixelValue(ossim_uint32 band)const
{
   if( isSourceEnabled() && (band < theMaxPixValue.size()) )
   {
      return theMaxPixValue[band];
   }

   return ossimImageSource::getMaxPixelValue(band);
}

void ossimCFARFilter::clearNullMinMax()
{
   theNullPixValue.clear();
   theMinPixValue.clear();
   theMaxPixValue.clear();
}

void ossimCFARFilter::computeNullMinMax()
{
   const ossim_uint32 BANDS = getNumberOfOutputBands();

   theNullPixValue.resize(BANDS);
   theMinPixValue.resize(BANDS);
   theMaxPixValue.resize(BANDS);

   ossim_float64 defaultNull = ossim::defaultNull(getOutputScalarType());
   ossim_float64 defaultMin = ossim::defaultMin(getOutputScalarType());
   ossim_float64 defaultMax = ossim::defaultMax(getOutputScalarType());
  
   for (ossim_uint32 band = 0; band < BANDS; ++band)
   {
      if(theInputConnection)
      {
         ossim_float64 inputNull = theInputConnection->getNullPixelValue(band);
         ossim_float64 inputMin  = theInputConnection->getMinPixelValue(band);
         ossim_float64 inputMax  = theInputConnection->getMaxPixelValue(band);
         ossim_float64 tempMin   = 0.0;
         ossim_float64 tempMax   = 0.0;
         ossim_float64 k         = 0.0;
         for(int i=0;i<5;++i)
         {
            for(int j=0;j<5;++j)
            {
               k=theKernel[i][j];
               tempMin += (k<0.0) ? k*inputMax : k*inputMin;
               tempMax += (k>0.0) ? k*inputMax : k*inputMin;
            }
         }

         if((inputNull < getMinPixelValue(band)) ||
            (inputNull > getMaxPixelValue(band)))
         {
            theNullPixValue[band] = inputNull;
         }
         else
         {
            theNullPixValue[band] = defaultNull;
         }

         if((tempMin >= defaultMin) && (tempMin <= defaultMax))
         {
            theMinPixValue[band] = tempMin;
         }
         else
         {
            theMinPixValue[band] = defaultMin;
         }

         if((tempMax >= defaultMin) && (tempMax <= defaultMax))
         {
            theMaxPixValue[band] = tempMax;
         }
         else
         {
            theMaxPixValue[band] = defaultMax;
         }
         
      }
      else // No input connection...
      {
         theNullPixValue[band] = defaultNull;
         theMinPixValue[band]  = defaultMin;
         theMaxPixValue[band]  = defaultMax;
      }
      
   } // End of band loop.
}
