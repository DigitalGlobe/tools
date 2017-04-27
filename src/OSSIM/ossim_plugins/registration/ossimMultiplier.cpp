// class ossimMultiplier implementation
// REQUIRES FFTW version 3.x (Fast Fourier Transform)

#include "ossimMultiplier.h"
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimIrect.h>

RTTI_DEF1( ossimMultiplier, "ossimMultiplier", ossimImageCombiner );

ossimMultiplier::ossimMultiplier()
   :
   ossimImageCombiner()
{
}

ossimMultiplier::ossimMultiplier(ossimObject *owner,         
                                 ossimImageSource *left,
                                 ossimImageSource *right)
   :ossimImageCombiner(owner,
   2,
   0,
   true,
   false)
{
   connectMyInputTo(0, left);
   connectMyInputTo(1, right);
}

ossimMultiplier::~ossimMultiplier()
{ 
}

bool
ossimMultiplier::canConnectMyInputTo(ossim_int32 index,const ossimConnectableObject* object)const
{
   //check that inputs just have one band
   //max 3 inputs
   //TBD: check scalar types
   switch(index)
   {
   case 0:
   case 1:
      {
      ossimImageSource* imagep=PTR_CAST(ossimImageSource, object);
      if (imagep)
      {
         return true;
      } else {
         return false;
      }
      }
      break;
   default:
      return false;
   }
}

ossimRefPtr<ossimImageData>
ossimMultiplier::getTile(const ossimIrect &rect, ossim_uint32 resLevel)
{
   if ( (!theNormTile) || (!theMin.size()) )
   {
      initialize();
   }

   long w = rect.width();
   long h = rect.height();
   long tw = theNormTile->getWidth();
   long th = theNormTile->getHeight();

   theNormTile->setImageRectangle(rect);

   if(w*h != tw*th)
   {
     theNormTile->initialize();
   }
   else
   {
      theNormTile->makeBlank();
   }

   switchTypeRun(rect, resLevel);
   
   return theNormTile;
}

//switchTypeRun: updates theNormTile
void
ossimMultiplier::switchTypeRun(const ossimIrect &tileRect, ossim_uint32 resLevel)
{   
   //get scalar type & check
   if (hasDifferentInputs())
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "All input scalar types must be identical"<< endl;
      return;
   }
   //type template
   switch(theNormTile->getScalarType())
   {
      case OSSIM_UINT8:
      {
            operate(static_cast<ossim_uint8>(0),
                               tileRect,
                               resLevel);
            break;
      }
      case OSSIM_SINT8:
      {
            operate(static_cast<ossim_sint8>(0),
                               tileRect,
                               resLevel);
            break;
      }
      case OSSIM_UINT16:
      case OSSIM_USHORT11:
      {
           operate(static_cast<ossim_uint16>(0),
                               tileRect,
                               resLevel);
           break;
      }
      case OSSIM_SINT16:
      {
           operate(static_cast<ossim_sint16>(0),
                               tileRect,
                               resLevel);
           break;
      }
      case OSSIM_SINT32:
      {
            operate(static_cast<ossim_sint32>(0),
                               tileRect,
                               resLevel);
            break;
      }
      case OSSIM_UINT32:
      {
            operate(static_cast<ossim_uint32>(0),
                               tileRect,
                               resLevel);
            break;
      }
      case OSSIM_FLOAT: 
      case OSSIM_NORMALIZED_FLOAT:
      {
           operate(static_cast<ossim_float32>(0),
                               tileRect,
                               resLevel);
           break;
      }
      case OSSIM_DOUBLE:
      case OSSIM_NORMALIZED_DOUBLE:
      {
            operate(static_cast<ossim_float64>(0),
                               tileRect,
                               resLevel);
            break;
      }
      case OSSIM_SCALAR_UNKNOWN:
      default:
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Scalar type = " << theNormTile->getScalarType()
            << " Not supported by ossimMultiplier" << endl;
         break;
      }
   }
   theNormTile->validate();
}

template <class T>
void
ossimMultiplier::operate( T,// dummy template variable 
                             const ossimIrect& tileRect,
                             ossim_uint32 resLevel)
{
   ossimImageSource* left  = PTR_CAST(ossimImageSource, getInput(0));
   ossimImageSource* right = PTR_CAST(ossimImageSource, getInput(1));
   double v;
   if (left && right)
   {
      ossimRefPtr<ossimImageData> leftD  = left->getTile(tileRect, resLevel);
      ossimRefPtr<ossimImageData> rightD = right->getTile(tileRect, resLevel);
      if ((leftD.valid()) && (rightD.valid()) && (leftD->getDataObjectStatus() != OSSIM_EMPTY) && (rightD->getDataObjectStatus() != OSSIM_EMPTY))
      {
         ossim_uint32 upperBound = theNormTile->getWidth() * theNormTile->getHeight();
         for(ossim_uint32 band = 0; band < theNormTile->getNumberOfBands(); ++band)
         {
            T* leftBand      = static_cast<T*>(leftD->getBuf(band));
            T* rightBand     = static_cast<T*>(rightD->getBuf(band));
            T* destBand      = static_cast<T*>(theNormTile->getBuf(band));
            double minv      = theMin[band];
            double maxv      = theMax[band];
            double leftNull  = leftD->getNullPix(band);
            double rightNull = rightD->getNullPix(band);

            if(leftBand&&rightBand)
            {
               for(ossim_uint32 offset = 0; offset < upperBound; ++offset)
               {
                  if (((double)*leftBand != leftNull) && ((double)*rightBand != rightNull))
                  {
                     v = *leftBand * (*rightBand);
                     //must still check for "overflows"
                     if (v > maxv) v = maxv;
                     if (v < minv) v = minv;
                  
                     *destBand = static_cast<T>(v);
                  } //else : destination already blank
                  
                  ++leftBand; ++rightBand; ++destBand;
               }
            }
         }
      }
   }
}

void 
ossimMultiplier::initialize()
{
   ossimImageCombiner::initialize();
   theNormTile = ossimImageDataFactory::instance()->create(this, this);
   theNormTile->initialize();  //TBD: change tile creation so it isn't within initialize 
   computeMinMax(); //TBC : is it the right place?
}

double
ossimMultiplier::getNullPixelValue(ossim_uint32 band)const
{
   return ossim::defaultNull(getOutputScalarType());
}

double
ossimMultiplier::getMinPixelValue(ossim_uint32 band)const
{
   if (band<theMin.size())
   {
      return theMin[band];
   } else {
      return ossim::defaultMin(getOutputScalarType());
   }
}

double 
ossimMultiplier::getMaxPixelValue(ossim_uint32 band)const
{
   if (band<theMax.size())
   {
      return theMax[band];
   } else {
      return ossim::defaultMax(getOutputScalarType());
   }
}

void
ossimMultiplier::computeMinMax()
{
   // use monotony of (x,y) |-> x*y on lines and rows
   // => test only four corners and keep min/max
   ossimImageSource* left  = PTR_CAST(ossimImageSource, getInput(0));
   ossimImageSource* right = PTR_CAST(ossimImageSource, getInput(1));

   //maximum span: default values
   double defMin = ossim::defaultMin(getOutputScalarType());
   double defMax = ossim::defaultMax(getOutputScalarType());

   theMin.clear();
   theMax.clear();

   //new values
   if (left && right)
   {
      for (ossim_uint32 band=0;band<getNumberOfOutputBands();++band)
      {
         double leftspan[2] = { left->getMinPixelValue(band)  , left->getMaxPixelValue(band)  };
         double rightspan[2]= { right->getMinPixelValue(band) , right->getMaxPixelValue(band) };
         double v;
         double minv = defMax; //crossed min-max
         double maxv = defMin;
         for (int l=0;l<2;++l)
         {
            for (int r=0;r<2;++r)
            {
               v = leftspan[l] * rightspan[r];
               if (v < minv) minv=v;
               if (v > maxv) maxv=v;
            }
         }         
         theMin.push_back( (minv>defMin) ? minv : defMin );
         theMax.push_back( (maxv<defMax) ? maxv : defMax );
      }
   }
}

ossim_uint32
ossimMultiplier::getNumberOfOutputBands()const
{   
   ossimImageSource* left  = PTR_CAST(ossimImageSource, getInput(0));
   ossimImageSource* right = PTR_CAST(ossimImageSource, getInput(1));
   if (left && right)
   {
      return std::min( left->getNumberOfOutputBands(), right->getNumberOfOutputBands() );
   } else {
      return 0;
   }
}

ossimIrect
ossimMultiplier::getBoundingRect(ossim_uint32 resLevel)const
{
   ossimImageSource* left  = PTR_CAST(ossimImageSource, getInput(0));
   ossimImageSource* right = PTR_CAST(ossimImageSource, getInput(1));
   ossimIrect res;
   if (left && right)
   {
     //Intersect rectangles
     res = left->getBoundingRect(resLevel);
     res.clipToRect(right->getBoundingRect(resLevel));
   } 
   else
   {
      res.makeNan();
   }
   return res;
}

