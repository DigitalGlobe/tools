#ifndef ossimPredatorKlvTable_HEADER
#define ossimPredatorKlvTable_HEADER
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimDate.h>
#include <ossim/base/ossimEndian.h>
#include <map>
#include <ossimPredator/ossimPredatorExport.h>

typedef ossim_uint8 OSSIM_PREDATOR_UDS[16];

enum ossimPredatorKlvIndex
{
   KLV_KEY_INVALID = -1,
   KLV_KEY_STREAM_ID = 0,
   KLV_KEY_ORGANIZATIONAL_PROGRAM_NUMBER,
   KLV_KEY_UNIX_TIMESTAMP,
   KLV_KEY_USER_DEFINED_UTC_TIMESTAMP,
   KLV_KEY_USER_DEFINED_TIMESTAMP_MICROSECONDS_1970,
   KLV_KEY_VIDEO_START_DATE_TIME_UTC, //Video time stamp
   KLV_TIMESYSTEM_OFFSET,
   KLV_UAS_DATALINK_LOCAL_DATASET,
   KLV_BASIC_UNIVERSAL_METADATA_SET,
   KLV_URL_STRING,
   KLV_KEY_OBJECT_COUNTRY_CODES,
   KLV_KEY_SECURITY_CLASSIFICATION_SET,
   KLV_KEY_BYTE_ORDER, /* MM=Big Endian ll=Little Endian */
   KLV_KEY_MISSION_NUMBER,
   KLV_KEY_SECURITY_CLASSIFICATION,
   KLV_KEY_SECURITY_RELEASE_INSTRUCTIONS,
   KLV_KEY_SECURITY_CAVEATS,
   KLV_KEY_CLASSIFICATION_COMMENT,
   KLV_KEY_ORIGINAL_PRODUCER_NAME,
   KLV_KEY_PLATFORM_GROUND_SPEED,
   KLV_KEY_PLATFORM_MAGNETIC_HEADING_ANGLE,
   KLV_KEY_PLATFORM_HEADING_ANGLE,
   KLV_KEY_PLATFORM_PITCH_ANGLE,
   KLV_KEY_PLATFORM_ROLL_ANGLE,
   KLV_KEY_PLATFORM_DESIGNATION,
   KLV_KEY_PLATFORM_DESIGNATION2,
   KLV_KEY_INDICATED_AIR_SPEED,
   KLV_KEY_IMAGE_SOURCE_SENSOR,
   KLV_KEY_IMAGE_COORDINATE_SYSTEM,
   KLV_KEY_SENSOR_RELATIVE_ELEVATION_ANGLE,
   KLV_KEY_SENSOR_RELATIVE_AZIMUTH_ANGLE,
   KLV_KEY_SENSOR_RELATIVE_ROLL_ANGLE,
   KLV_KEY_SENSOR_LATITUDE,
   KLV_KEY_SENSOR_LONGITUDE,
   KLV_KEY_SENSOR_TRUE_ALTITUDE,
   KLV_KEY_SENSOR_HORIZONTAL_FOV,
   KLV_KEY_SENSOR_VERTICAL_FOV1, // this is to the SPEC 
   KLV_KEY_SENSOR_VERTICAL_FOV2, // this is not to SPEC but have found undefined key that looks like it should be vertical field of view
   KLV_KEY_SLANT_RANGE,
   KLV_KEY_OBLIQUITY_ANGLE,
   KLV_KEY_ANGLE_TO_NORTH,
   KLV_KEY_TARGET_WIDTH,
   KLV_KEY_FRAME_CENTER_LATITUDE,
   KLV_KEY_FRAME_CENTER_LONGITUDE,
   KLV_KEY_FRAME_CENTER_ELEVATION,
   KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_1,
   KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_1,
   KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_2,
   KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_2,
   KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_3,
   KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_3,
   KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_4,
   KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_4,

   KLV_KEY_CORNER_LATITUDE_POINT_1,
   KLV_KEY_CORNER_LONGITUDE_POINT_1,
   KLV_KEY_CORNER_LATITUDE_POINT_2,
   KLV_KEY_CORNER_LONGITUDE_POINT_2,
   KLV_KEY_CORNER_LATITUDE_POINT_3,
   KLV_KEY_CORNER_LONGITUDE_POINT_3,
   KLV_KEY_CORNER_LATITUDE_POINT_4,
   KLV_KEY_CORNER_LONGITUDE_POINT_4,
   KLV_KEY_DEVICE_ABSOLUTE_SPEED,
   KLV_KEY_DEVICE_ABSOLUTE_HEADING,
   KLV_KEY_ABSOLUTE_EVENT_START_DATE,
   KLV_KEY_SENSOR_ROLL_ANGLE,
   KLV_KEY_PLATFORM_TAIL_NUMBER,
   KLV_KEY_GENERIC_FLAG_DATA_01,
   KLV_KEY_UAS_LDS_VERSION_NUMBER,
   KLV_KEY_STATIC_PRESSURE,
   KLV_SECURITY_METADATA_UNIVERSAL_SET,
   KLV_KEY_TOTAL
};

class OSSIMPREDATOR_EXPORT ossimPredatorKlvInfoType
{
public:
   ossimPredatorKlvIndex theId; // my ID as defined in the KlvIndex enumeration
   const char*      theName;
   OSSIM_PREDATOR_UDS     theKey;
};


class OSSIMPREDATOR_EXPORT ossimPredatorKlvTable : public ossimReferenced
{
public:
   struct Node
   {
      Node(int id=0, 
           const std::vector<ossim_uint8>& value=std::vector<ossim_uint8>())
      :theId(id),
      theValue(value)
      {
         
      }
      int theId;
      
      std::vector<ossim_uint8> theValue;
   };
   typedef std::map<ossimPredatorKlvIndex, ossimPredatorKlvTable::Node> klvMapType;
   ossimPredatorKlvTable();
   ossimPredatorKlvTable(const ossimPredatorKlvTable& src)
      :theNeedToParseBuffer(src.theNeedToParseBuffer),
      theKlvParameters(src.theKlvParameters),
      theNeedToSwapFlag(src.theNeedToSwapFlag)
   {
   }
   ossimPredatorKlvTable* dup()const
   {
      return new ossimPredatorKlvTable(*this);
   }
   bool addKeys(const std::vector<ossim_uint8>& buffer);
   bool addKeys(const ossim_uint8* buffer, ossim_uint32 length);
   void clear();
   //ossimString getValueAsString(ossimPredatorKlvIndex id)const;
   bool valueAsString(ossimString& result,
                      ossimPredatorKlvIndex id)const;
   ossimString valueAsString(ossimPredatorKlvIndex id)const;
   
   bool getCornerPoints(ossimGpt& pt1,
                        ossimGpt& pt2,
                        ossimGpt& pt3,
                        ossimGpt& pt4)const;
   bool getFrameCenterOffsets(ossimDpt& pt1Offset,
                              ossimDpt& pt2Offset,
                              ossimDpt& pt3Offset,
                              ossimDpt& pt4Offset)const;
   /**
    * Nothing has been shifted.  The Elevation is still relative to mean sea level.
    */ 
   bool getFrameCenter(ossim_float64& lat,
                       ossim_float64& lon,
                       ossim_float64& elevation)const;

   bool getSensorPosition(ossim_float64& lat,
                          ossim_float64& lon,
                          ossim_float64& elev)const;
   bool getPlatformOrientation(ossim_float32& heading,
                               ossim_float32& pitch,
                               ossim_float32& roll)const;
   bool getSensorRollAngle(ossim_float32& angle)const;
   bool getObliquityAngle(ossim_float32& angle)const;
   bool getSlantRange(ossim_float32& range)const;
   bool getHorizontalFieldOfView(ossim_float32& hfov)const;
   bool getVerticalFieldOfView(ossim_float32& hfov)const;
   bool getAngleToNorth(ossim_float32& angleToNorth)const;
   bool getTargetWidthInMeters(ossim_float32& targetWidth)const;
   ossimString getUtcTimestamp()const;
   
   /**
    * This is defined to be in microseconds
    */ 
   bool getUnixEpocTimestamp(ossim_int64& timestamp)const;

   /**
    * Converts the microseconds to be a decmail seconds and it's fractional part
    */ 
   bool getUnixEpocTimestampInSeconds(ossim_int64& seconds,
                                      ossim_float64& fractionalPart)const;

   /**
    * Converts the microseconds past to be in a decimal seconds
    */ 
   bool getUnixEpocTimestampInFractionalSeconds(ossim_float64& fractionalSeconds)const;

   bool getDate(ossimDate& d, bool shiftToGmtZero = false)const;
   bool getDateUsingEpoc(ossimDate& d)const;
   bool getDateUsingUtc(ossimDate& d, bool shiftToGmtZero = false)const;
   
   virtual std::ostream& print(std::ostream& out)const;
protected:
  ossimPredatorKlvTable::Node convertValue(int id, const std::vector<ossim_uint8>& bufferValue);
  int findPredatorKlvIndex(const ossim_uint8* buf)const;
  int findPredatorKlvIndexByKey(ossim_uint32 key)const;
  void addAbsoluteKeyDefinitions(const std::vector<ossim_uint8>& buffer, bool& needMore);

  void addSecurityMetadataLocalSetElements(const std::vector<ossim_uint8>& buffer, klvMapType& tempTable);
  void addUasDatalinkLocalDataSet(const std::vector<ossim_uint8>& buffer, klvMapType& tempTable, ossim_uint16& checksum);
  void loadVariableDataNullTerminated(std::vector<ossim_uint8>& result, 
                                       const std::vector<ossim_uint8>& buf, 
                                       ossim_uint32& idx)const;
  ossim_uint16 compute16BitChecksum(const std::vector<ossim_uint8>& checksumBuffer)const;
   std::vector<ossim_uint8> theNeedToParseBuffer;
   static const ossim_uint8 theKlvKey[4];//{ 0x06,0x0e,0x2b,0x34 }
   klvMapType               theKlvParameters;
   ossimEndian              theEndian;
   bool                     theNeedToSwapFlag; 
};

#endif
