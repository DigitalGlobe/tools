
#include "ossimDensityReducer.h"
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/base/ossimNumericProperty.h>

#include <algorithm>

using namespace std;

RTTI_DEF1(ossimDensityReducer, "ossimDensityReducer", ossimImageSourceFilter);

static const char* PROP_MAXDENSITY  = "MaxDensity";

//functor for decreasing comparison (strict weak ordering, like greater), for pairs on the first element
template<class T> class greaterFirst
{
   public:
      bool operator()(const T& x, const T& y)
      {
         return x.first > y.first;
      }
};

ossimDensityReducer::ossimDensityReducer(ossimObject* owner)
   :ossimImageSourceFilter(owner),
    theTile(NULL),
    theMaxDensity(1.0)
{
}

ossimDensityReducer::ossimDensityReducer(ossimImageSource* inputSource)
   :ossimImageSourceFilter(inputSource),
    theTile(NULL),
    theMaxDensity(1.0)
{
}

ossimDensityReducer::ossimDensityReducer(ossimObject* owner,
                                   ossimImageSource* inputSource)
   :ossimImageSourceFilter(owner, inputSource),
    theTile(NULL),
    theMaxDensity(1.0)
{
}

ossimRefPtr<ossimImageData> 
ossimDensityReducer::getTile(const ossimIrect& rect, ossim_uint32 resLevel)
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getTile(rect, resLevel);
   }

   ossimRefPtr<ossimImageData> inputData =
      ossimImageSourceFilter::getTile(rect, resLevel);

   if(!inputData.valid() || (!inputData->getBuf()) || (inputData->getDataObjectStatus() ==  OSSIM_EMPTY) || (getMaxDensity()>=1.0))
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
      ossimNotify(ossimNotifyLevel_WARN) << "ossimDensityReducer::getTile WARN: Scalar type = " << theTile->getScalarType()
                                         << " Not supported by ossimDensityReducer" << std::endl;
      break;
   }
   }

   return theTile;
}

void ossimDensityReducer::initialize()
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

void ossimDensityReducer::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property) return;

   if (property->getName() == PROP_MAXDENSITY)  {
      ossimNumericProperty* numProperty = PTR_CAST(ossimNumericProperty,
                                                     property.get());
      if(numProperty)
      {
         setMaxDensity(numProperty->asFloat64());
      }
   }  else {
      ossimImageSourceFilter::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimDensityReducer::getProperty(const ossimString& name)const
{
   if (name == PROP_MAXDENSITY) {
      ossimNumericProperty* property = new ossimNumericProperty(name, ossimString::toString(getMaxDensity()),0.0,1.0);
      property->setCacheRefreshBit();
      return property;
   } 
   return ossimImageSourceFilter::getProperty(name);
}

void ossimDensityReducer::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSourceFilter::getPropertyNames(propertyNames);
   
   propertyNames.push_back(PROP_MAXDENSITY);
}


template <class T>
void ossimDensityReducer::runFilter(T dummyVariable,
                                ossimRefPtr<ossimImageData> inputData)
{
   // so far neighborhood is 3x3
   ossim_uint32 bandIdx = 0;
   ossim_uint32 numberOfBands = inputData->getNumberOfBands();
  
   ossim_uint32 width  = theTile->getWidth();
   ossim_uint32 height = theTile->getHeight();
   ossim_uint32 tsize  = width * height; //surely a tile cannot hold more than 4 billion pixels?

   //max number of pixels
   if (getMaxDensity()<=0)
   {
      //nullify tile
      theTile->makeBlank();
      return;
   }
   //we always want at least one point (if density > 0)
   ossim_uint32 maxp = (ossim_uint32)ceil(getMaxDensity() * tsize);
   if (maxp >= tsize)
   {
      //return everything
      theTile = inputData; //TBC: should we make a deep copy?
      return;
   }

   //initially nullify output tile
   theTile->makeBlank();

   //init a storage vector for non null pixels : pre-allocate maximum size
   // stores value and offset from start
   // will be used for all bands
   vector< pair< T , ossim_uint32 > > sortv(tsize);

   for(bandIdx = 0; bandIdx < numberOfBands; ++bandIdx)
   {
      T* inputBuf  = static_cast<T*>(inputData->getBuf(bandIdx));
      T* outputBuf = static_cast<T*>(theTile->getBuf(bandIdx));
      T np         = static_cast<T>(inputData->getNullPix(bandIdx)); //changed to input Null            
      if(inputBuf&&outputBuf)
      {
         //get only non null pixels, and store offset
         typename vector< pair< T , ossim_uint32 > >::iterator it = sortv.begin();
         ossim_uint32 o;
         for(o = 0; o < tsize; ++o,++it)
         {
            if (inputBuf[o] != np)
            {
               it->first  = inputBuf[o] ;
               it->second = o ;
            }
         }

         //o is now the number of valid elemnts in the vector
         //arrange vector so that the maxp first elements have larger value than the others
         if (maxp<o)
         {
            nth_element< typename vector< pair< T , ossim_uint32 > >::iterator , greaterFirst< pair< T , ossim_uint32 > > >(
               sortv.begin(),
               sortv.begin()+maxp, 
               sortv.begin()+o, 
               greaterFirst< pair< T , ossim_uint32 > >());
            o=maxp;
         } //else : density below max, nothing to do

         //only display elements between offsets maxp and o
         typename vector< pair< T , ossim_uint32 > >::iterator nit = sortv.begin();
         for (ossim_uint32 p=0; p<o; ++p,++nit)
         {
            outputBuf[nit->second] = nit->first;
         }

      }
   }
   theTile->validate();
}

bool ossimDensityReducer::saveState(ossimKeywordlist& kwl,
                                          const char* prefix)const
{  
   kwl.add(prefix,
          PROP_MAXDENSITY,
          ossimString::toString(getMaxDensity()),
          true);
   
   return ossimImageSourceFilter::saveState(kwl, prefix);
}


bool ossimDensityReducer::loadState(const ossimKeywordlist& kwl,
                                          const char* prefix)
{
   const char* md = kwl.find(prefix,PROP_MAXDENSITY );
   if(md)
   {
      setMaxDensity(ossimString::toDouble(md));
   } else {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimDensityReducer::loadState WARN: no "<<PROP_MAXDENSITY<<" found" << std::endl;
   }  
   return ossimImageSourceFilter::loadState(kwl, prefix);
}
