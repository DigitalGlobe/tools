#ifndef ossimPlanetQtImage_HEADER
#define ossimPlanetQtImage_HEADER
#include <QtGui/QImage>
#include <ossim/imaging/ossimImageData.h>
#include <ossimPlanetQt/ossimPlanetQtExport.h>

class OSSIMPLANETQT_DLL ossimPlanetQtImage : public QImage
{
public:
   ossimPlanetQtImage(const QImage& img);
   ossimPlanetQtImage(const ossimPlanetQtImage& img);
   
   ossimRefPtr<ossimImageData> getOssimImage()const;
   
protected:
   void convertToOssimImage();
   
   ossimRefPtr<ossimImageData> theImage;
};

#endif
