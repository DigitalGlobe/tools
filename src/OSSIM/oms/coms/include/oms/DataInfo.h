//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Simple container class to encapsulate image info.
//
//----------------------------------------------------------------------------
// $Id: ImageInfo.h 12991 2008-06-04 19:14:59Z gpotts $
#ifndef omsImageInfo_HEADER
#define omsImageInfo_HEADER 1

#include <vector>
#include <string>
#include <oms/Constants.h>
#include <oms/Object.h>
#include <ossim/base/ossimGpt.h>

class ossimImageGeometry;
class ossimImageHandler;

namespace oms
{
   class  DataInfoPrivateData;

   class OMSDLL DataInfo : public oms::Object
   {
   public:
      /** default constructor */
      DataInfo();
      
      /** destructor */
      virtual ~DataInfo();
      
      /**
       * @brief Open method.
       * @param file File to open.
       * @return true on success, false on error.
       */
      bool open(const std::string& file, bool failIfNoGeometryFlag=true);
      bool setHandler(ossimImageHandler* handler, bool failIfNoGeometryFlag=true);
      void close();
      bool isVideo()const;
      bool isImagery()const;
      std::string getInfo()const;
      std::string getImageInfo(int entry=-1);
      std::string getVideoInfo();

      bool getGroundCorners(std::vector<ossimGpt>& corners, int entry=-1);
      double getMetersPerPixel(int entry=-1, int resolution = 0);
      int getNumberOfEntries()const;
      int getNumberOfResolutionLevels(int entry)const;
      void getWidthHeight(int entry, int resolution, int* w, int* h)const;

      static std::string readInfo(const std::string& file, bool failIfNoGeometryFlag=true);
      
   private:
      std::string convertIdatimToXmlDate(const std::string& idatim)const;
      std::string convertAcqDateToXmlDate(const std::string& idatim)const;
      std::string checkAndGetThumbnail(const std::string& baseName)const;

      void appendRasterEntry(std::string& outputString,
                             const std::string& indentation,
                             const std::string& separator)const;
      void appendRasterEntries(std::string& outputString,
                               const std::string& indentation,
                               const std::string& separator)const;

      /**
       * @brief Appends non ossim generated associate files to <fileObjects> as
       * <RasterFile> tags.
       * @param outputString Initialized by this.
       * @param indentation e.g. "      "
       * @param separator e.g. "\n"
       */
      void appendAssociatedFiles(std::string& outputString,
                                 const std::string& indentation,
                                 const std::string& separator)const;
      
      void appendAssociatedRasterEntryFileObjects(std::string& outputString,
                                                  const std::string& indentation,
                                                  const std::string& separator)const;
      void appendBitDepthAndDataType(std::string& outputString,
                                     const std::string& indentation,
                                     const std::string& separator)const;
      void appendGeometryInformation(std::string& outputString,
                                     const std::string& indentation,
                                     const std::string& separator)const;
      void appendRasterDataSetMetadata(std::string& outputString,
                                       const std::string& indentation,
                                       const std::string& separator)const;
      void appendVideoDataSetMetadata(std::string& outputString,
                                       const std::string& indentation,
                                       const std::string& separator)const;
      void appendRasterEntryDateTime(std::string& outputString,
                                     const std::string& indentation,
                                     const std::string& separator)const;
      void appendRasterEntryMetadata(std::string& outputString,
                                     const std::string& indentation,
                                     const std::string& separator)const;

      /**
       * @brief Gets azimuth angle from keyword list.
       * @param kwl Keyword list to query.
       * @param azimuthAngle Initialized by this.
       */
      void getAzimuthAngle( const ossimKeywordlist& kwl,
                            std::string& azimuthAngle ) const;
      
     /**
       * @brief Gets BE number from keyword list.
       * @param kwl Keyword list to query.
       * @param beNumber Initialized by this.
       */
      void getBeNumber( const ossimKeywordlist& kwl,
                        std::string& beNumber ) const;

      /**
       * @brief Gets country code from keyword list.
       * @param kwl Keyword list to query.
       * @param countryCode Initialized by this.
       */
      void getCountryCode( const ossimKeywordlist& kwl,
                           std::string& countryCode ) const;
      
      /**
       * @brief Gets date from keyword list.
       * @param kwl Keyword list to query.
       * @param dateValue Initialized by this.
       */
      void getDate( const ossimKeywordlist& kwl,
                    std::string& dateValue ) const;

      /**
       * @brief Gets description from keyword list.
       * @param kwl Keyword list to query.
       * @param description Initialized by this.
       */
      void getDescription( const ossimKeywordlist& kwl,
                           std::string& description ) const;
      
      /**
       * @brief Gets grazing angle from keyword list.
       * @param kwl Keyword list to query.
       * @param grazingAngle Initialized by this.
       */
      void getGrazingAngle( const ossimKeywordlist& kwl,
                            std::string& grazingAngle ) const;

      /**
       * @brief Gets image category from keyword list.
       * @param kwl Keyword list to query.
       * @param imageCategory Initialized by this.
       */
      void getImageCategory( const ossimKeywordlist& kwl,
                             std::string& imageCategory ) const;

      /**
       * @brief Gets image ID from keyword list.
       * @param kwl Keyword list to query.
       * @param imageId Initialized by this.
       */
      void getImageId( const ossimKeywordlist& kwl,
                       std::string& imageId ) const;
      
      /**
       * @brief Gets image representation from keyword list.
       * @param kwl Keyword list to query.
       * @param imageRepresentation Initialized by this.
       */
      void getImageRepresentation( const ossimKeywordlist& kwl,
                                   std::string& imageRepresentation ) const;
 
      /**
       * @brief Gets isorce from keyword list.
       * @param kwl Keyword list to query.
       * @param isorce Initialized by this.
       */
      void getIsorce( const ossimKeywordlist& kwl,
                      std::string& isorce ) const;
      
      /**
       * @brief Gets mission from keyword list.
       * @param kwl Keyword list to query.
       * @param missionId Initialized by this.
       */
      void getMissionId( const ossimKeywordlist& kwl,
                         std::string& missionId ) const;

      /**
       * @brief Gets niirs from keyword list.
       * @param kwl Keyword list to query.
       * @param niirs Initialized by this.
       */
      void getNiirs( const ossimKeywordlist& kwl,
                     std::string& niirs ) const;
      /**
       * @brief Gets organization from keyword list.
       * @param kwl Keyword list to query.
       * @param organization Initialized by this.
       */
      void getOrganization( const ossimKeywordlist& kwl,
                            std::string& organization ) const;
       /**
       * @brief Gets product ID from keyword list.
       * @param kwl Keyword list to query.
       * @param productId Initialized by this.
       */
      void getProductId( const ossimKeywordlist& kwl,
                         std::string& productId ) const;
       
      /**
       * @brief Gets sensor ID from keyword list.
       * @param kwl Keyword list to query.
       * @param sensorId Initialized by this.
       */
      void getSensorId( const ossimKeywordlist& kwl,
                        std::string& sensorId ) const;
      /**
       * @brief Gets security classification from keyword list.
       * @param kwl Keyword list to query.
       * @param mission Initialized by this.
       */
      void getSecurityClassification(
         const ossimKeywordlist& kwl,
         std::string& securityClassification ) const;

      /**
       * @brief Gets sun azimuth from keyword list.
       * @param kwl Keyword list to query.
       * @param sunAzimuth Initialized by this.
       */
      void getSunAzimuth( const ossimKeywordlist& kwl,
                          std::string& sunAzimuth ) const;

      /**
       * @brief Gets sun elevation from keyword list.
       * @param kwl Keyword list to query.
       * @param sunElevation Initialized by this.
       */
      void getSunElevation( const ossimKeywordlist& kwl,
                            std::string& sunElevation ) const;

      /**
       * @brief Gets target ID from keyword list.
       * @param kwl Keyword list to query.
       * @param targetId Initialized by this.
       */
      void getTargetId( const ossimKeywordlist& kwl,
                        std::string& targetId ) const;

      /**
       * @brief Gets title from keyword list.
       * @param kwl Keyword list to query.
       * @param title Initialized by this.
       */
      void getTitle( const ossimKeywordlist& kwl,
                     std::string& title ) const;

      /**
       * @brief Gets a wkt polygon from "wkt_footprint" key in geometry if
       * found.
       * @param geom Pointer to image geometry.
       * @param s Initialized by this.
       */
      bool getWktFootprint( const ossimImageGeometry* geom, std::string& s ) const;

      /**
       * @brief Derives the omd filename from image handler.
       * @param omdFile Initialized by this.
       */
      void getOmdFile( ossimFilename& omdFile ) const;
      
      DataInfoPrivateData* thePrivateData;
      
   }; // End: class DataInfo

} // end of namespace oms

#endif /* End of #ifndef omsImageInfo_HEADER */
