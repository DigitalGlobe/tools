#include <ossimPlanetQt/ossimPlanetQtImage.h>

ossimPlanetQtImage::ossimPlanetQtImage(const QImage& img)
   :QImage(img)
{
   convertToOssimImage();
}

ossimPlanetQtImage::ossimPlanetQtImage(const ossimPlanetQtImage& img)
   :QImage(img)
{
   convertToOssimImage();
}

ossimRefPtr<ossimImageData> ossimPlanetQtImage::getOssimImage()const
{
   return theImage;
}

void ossimPlanetQtImage::convertToOssimImage()
{
   if(format() != QImage::Format_RGB32)
   {
      (*this) = convertToFormat(QImage::Format_RGB32);
   }

   ossim_uint32 w = width();
   ossim_uint32 h = height();
   ossim_uint32 area = w*h;
   const ossim_uint32* bitsPtr = (ossim_uint32*)bits();
   theImage = new ossimImageData(0,
                                 OSSIM_UINT8,
                                 3,
                                 w,
                                 h);
   theImage->initialize();
   
   ossim_uint8* outBufRed   = (ossim_uint8*)theImage->getBuf(0);
   ossim_uint8* outBufGreen = (ossim_uint8*)theImage->getBuf(1);
   ossim_uint8* outBufBlue  = (ossim_uint8*)theImage->getBuf(2);
                                 
   if(bitsPtr)
   {
      ossim_uint32 offset = 0;

      while(offset < area)
      {
         *outBufRed   = (ossim_uint8)(((*bitsPtr)&0x00ff0000)>>16);
         *outBufGreen = (ossim_uint8)(((*bitsPtr)&0x0000ff00)>>8);
         *outBufBlue  = (ossim_uint8)(((*bitsPtr)&0x000000ff));
         
         ++outBufRed;
         ++outBufGreen;
         ++outBufBlue;
         ++bitsPtr;
         ++offset;
      }
   }
   theImage->validate();
}
