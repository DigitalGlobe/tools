
#include "ossimExtremaFilter.h"
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/base/ossimBooleanProperty.h>

RTTI_DEF1(ossimExtremaFilter, "ossimExtremaFilter", ossimImageSourceFilter);

static const char* PROP_ISSTRICT  = "IsStrict";
static const char* PROP_ISMAXIMUM = "IsMaximum";

ossimExtremaFilter::ossimExtremaFilter(ossimObject* owner)
   :ossimImageSourceFilter(owner),
    theTile(NULL),
    theIsMaximum(true),
    theIsStrict(true)
{
}

ossimExtremaFilter::ossimExtremaFilter(ossimImageSource* inputSource)
   :ossimImageSourceFilter(inputSource),
    theTile(NULL),
    theIsMaximum(true),
    theIsStrict(true)
{
}

ossimExtremaFilter::ossimExtremaFilter(ossimObject* owner,
                                   ossimImageSource* inputSource)
   :ossimImageSourceFilter(owner, inputSource),
    theTile(NULL),
    theIsMaximum(true),
    theIsStrict(true)
{
}

ossimRefPtr<ossimImageData> 
ossimExtremaFilter::getTile(const ossimIrect& rect, ossim_uint32 resLevel)
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getTile(rect, resLevel);
   }

   // expand the rect out to cover the 3x3 horizontal and vertical kernel.
   ossimIrect requestRect = rect;

   adjustRequestRect(requestRect);
   
   ossimRefPtr<ossimImageData> inputData =
      ossimImageSourceFilter::getTile(requestRect, resLevel);

   if(!inputData.valid() || (!inputData->getBuf()))
   {
      return inputData;
   }

   if(!theTile.valid()) initialize();
   if(!theTile.valid()) return theTile;
      
   theTile->setImageRectangleAndBands(rect, inputData->getNumberOfBands());
   
   switch(theTile->getScalarType())
   {
   case OSSIM_UCHAR:
   {
      runFilter((ossim_uint8)0,
                inputData);
      break;
   }
   case OSSIM_FLOAT:
   case OSSIM_NORMALIZED_FLOAT:
   {
      runFilter((ossim_float32)0,
                inputData);
      break;
   }
   case OSSIM_USHORT16:
   case OSSIM_USHORT11:
   {
      runFilter((ossim_uint16)0,
                inputData);
      break;
   }
   case OSSIM_SSHORT16:
   {
      runFilter((ossim_sint16)0,
                inputData);
      break;
   }
   case OSSIM_DOUBLE:
   case OSSIM_NORMALIZED_DOUBLE:
   {
      runFilter((ossim_float64)0,
                inputData);
      break;
   }
   default:
   {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimExtremaFilter::getTile WARN: Scalar type = " << theTile->getScalarType()
                                         << " Not supported by ossimExtremaFilter" << std::endl;
      break;
   }
   }

   return theTile;
}

void ossimExtremaFilter::initialize()
{
   ossimImageSourceFilter::initialize();

   theTile = NULL;

   if(!isSourceEnabled())
   {
      return;
   }
   
   theTile = ossimImageDataFactory::instance()->create(this, this);
   if(theTile.valid())
   {
      theTile->initialize();
   }

}

void ossimExtremaFilter::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property) return;

   if (property->getName() == PROP_ISMAXIMUM)  {
      ossimBooleanProperty* booleanProperty = PTR_CAST(ossimBooleanProperty,
                                                     property.get());
      if(booleanProperty)
      {
         setIsMaximum(booleanProperty->getBoolean());
      }
   } else if (property->getName() == PROP_ISSTRICT)  {
      ossimBooleanProperty* booleanProperty = PTR_CAST(ossimBooleanProperty,
                                                     property.get());
      if(booleanProperty)
      {
         setIsStrict(booleanProperty->getBoolean());
      }
   } else {
      ossimImageSourceFilter::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimExtremaFilter::getProperty(const ossimString& name)const
{
   if (name == PROP_ISMAXIMUM) {
      ossimBooleanProperty* property = new ossimBooleanProperty(name, isMaximum());
      property->setCacheRefreshBit();
      return property;
   } else if (name == PROP_ISSTRICT) {
      ossimBooleanProperty* property = new ossimBooleanProperty(name, isStrict());
      property->setCacheRefreshBit();
      return property;
   }
   return ossimImageSourceFilter::getProperty(name);
}

void ossimExtremaFilter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSourceFilter::getPropertyNames(propertyNames);
   
   propertyNames.push_back(PROP_ISMAXIMUM);
   propertyNames.push_back(PROP_ISSTRICT);
}


void ossimExtremaFilter::adjustRequestRect(ossimIrect& requestRect)const
{
   requestRect = ossimIrect(requestRect.ul().x - 1,
                            requestRect.ul().y - 1,
                            requestRect.lr().x + 1,
                            requestRect.lr().y + 1);
}

template <class T>
void ossimExtremaFilter::runFilter(T dummyVariable,
                                ossimRefPtr<ossimImageData> inputData)
{
   // so far neighborhood is 3x3
   ossim_uint32 bandIdx = 0;
   ossim_uint32 numberOfBands = inputData->getNumberOfBands();
  
   ossim_uint32 x = 0;
   ossim_uint32 y = 0;
   ossim_uint32 width  = theTile->getWidth();
   ossim_uint32 height = theTile->getHeight();
   ossim_int32 rowIncrement  = inputData->getWidth();
   ossim_int32 rowIncrement2 = 2*inputData->getWidth(); 
         
   T centerv;
   T extremum;
   bool center_better;
   for(bandIdx = 0; bandIdx < numberOfBands; ++bandIdx)
   {
      //inputBuf has a 1 pixel edge compared to outputBuf
      T* inputBuf  = static_cast<T*>(inputData->getBuf(bandIdx));
      T* outputBuf = static_cast<T*>(theTile->getBuf(bandIdx));
      T np         = static_cast<T>(inputData->getNullPix(bandIdx)); //changed to input Null            
      if(inputBuf&&outputBuf)
      {
         //one pass: maybe faster if changed to two passes
         T* outB;
         T* inB;
         
      	outB = outputBuf;         
         inB  = inputBuf;         
         for(y = 0; y < height; ++y)
         {
            for(x = 0; x < width; ++x)
            {
               centerv = inB[1+rowIncrement];
               if (centerv != np)
               {
                  if (isMaximum())
                  {
                     extremum = max<T>(
                           max<T>(
                            max<T>(inB[0],inB[1]),
                            max<T>(inB[2],inB[rowIncrement])),
                           max<T>(
                            max<T>(inB[rowIncrement+2],inB[rowIncrement2]),
                            max<T>(inB[rowIncrement2+1],inB[rowIncrement2+2])
                            ));
                     if (isStrict())
                     {
                        center_better = centerv > extremum;
                     } else {
                        center_better = centerv >= extremum;
                     }
                  } else {
                     extremum = min<T>(
                           min<T>(
                            min<T>(inB[0],inB[1]),
                            min<T>(inB[2],inB[rowIncrement])),
                           min<T>(
                            min<T>(inB[rowIncrement+2],inB[rowIncrement2]),
                            min<T>(inB[rowIncrement2+1],inB[rowIncrement2+2])
                            ));
                     if (isStrict())
                     {
                        center_better = centerv < extremum;
                     } else {
                        center_better = centerv <= extremum;
                     }
                  }
                  //nullify if not extremum
                  *outB = center_better ? centerv : np ;
               }
               else
               {
                  *outB = np;
               }
               ++outB;
               ++inB;
            }
            inB+=2; //go to next line, jump due to edge
         }       
      }      
   }
   theTile->validate();
}

bool ossimExtremaFilter::saveState(ossimKeywordlist& kwl,
                                          const char* prefix)const
{  
   kwl.add(prefix,
          PROP_ISMAXIMUM,
          ossimString::toString(isStrict()),
          true);
   kwl.add(prefix,
          PROP_ISSTRICT,
           ossimString::toString(isStrict()),
           true);
   
   return ossimImageSourceFilter::saveState(kwl, prefix);
}


bool ossimExtremaFilter::loadState(const ossimKeywordlist& kwl,
                                          const char* prefix)
{
   const char* im = kwl.find(prefix,PROP_ISMAXIMUM );
   if(im)
   {
      setIsMaximum(ossimString::toBool(im));
   } else {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimExtremaFilter::loadState WARN: no "<<PROP_ISMAXIMUM<<" found" << std::endl;
   }
   const char* is = kwl.find(prefix,PROP_ISSTRICT );
   if(is)
   {
      setIsStrict(ossimString::toBool(is));
   } else {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimExtremaFilter::loadState WARN: no "<<PROP_ISSTRICT<<" found" << std::endl;
   }
        
   return ossimImageSourceFilter::loadState(kwl, prefix);
}
