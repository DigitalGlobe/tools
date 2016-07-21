#include <ossimPredator/ossimPredatorKlvTable.h>
#include <iostream>
#include <iomanip>

#include <ossim/base/ossimTrace.h>
static const ossimTrace traceDebug("ossimPredatorKlvTable:debug");
static const  ossimPredatorKlvInfoType OSSIM_PREDATOR_UDS_TABLE[]=
{
{KLV_KEY_STREAM_ID,"stream ID",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x03, 0x01, 0x03, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00}},
{KLV_KEY_ORGANIZATIONAL_PROGRAM_NUMBER,"Organizational Program Number",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x03, 0x01, 0x03, 0x05, 0x01, 0x00, 0x00, 0x00, 0x00}},
{KLV_KEY_UNIX_TIMESTAMP,"UNIX Timestamp",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04, 0x07, 0x02, 0x01, 0x01, 0x01, 0x05, 0x00, 0x00}}, // TIME STAMP
{KLV_KEY_USER_DEFINED_UTC_TIMESTAMP, "User Defined UTC", {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x07, 0x02, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00}},
{KLV_KEY_USER_DEFINED_TIMESTAMP_MICROSECONDS_1970, "User Defined Timestamp Microseconds since 1970", {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x03, 0x07, 0x02, 0x01, 0x01, 0x01, 0x05, 0x00, 0x00}},
{KLV_KEY_VIDEO_START_DATE_TIME_UTC, "Video Timestamp Start Date and Time",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x07, 0x02, 0x01, 0x02, 0x01, 0x01, 0x00, 0x00}},
{KLV_TIMESYSTEM_OFFSET, "Time System Offset From UTC", {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x03, 0x01, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00}},
{KLV_UAS_DATALINK_LOCAL_DATASET, "UAS Datalink Local Data Set",{0x06, 0x0E, 0x2B, 0x34, 0x02, 0x0B, 0x01, 0x01, 0x0E, 0x01, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00}},
{KLV_BASIC_UNIVERSAL_METADATA_SET, "Universal Metadata Set",{0x06, 0x0E, 0x2B, 0x34, 0x02, 0x01, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x02, 0x01, 0x01, 0x00, 0x00}},
{KLV_SECURITY_METADATA_UNIVERSAL_SET, "Security metadata universal set", {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x01, 0x01, 0x01, 0x02, 0x08, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00}},
{KLV_URL_STRING, "URL String", {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00}},
{KLV_KEY_SECURITY_CLASSIFICATION_SET, "Security Classification Set", {0x06, 0x0E, 0x2B, 0x34, 0x02, 0x01, 0x01, 0x01, 0x02, 0x08, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00}},
{KLV_KEY_BYTE_ORDER, "Byte Order", {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x03, 0x01, 0x02, 0x01, 0x02, 0x00, 0x00, 0x00}},
{KLV_KEY_MISSION_NUMBER,"Mission Number",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x01, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00}},
{KLV_KEY_OBJECT_COUNTRY_CODES, "Object Country Codes", {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x03, 0x07, 0x01, 0x20, 0x01, 0x02, 0x01, 0x01, 0x00}},
{KLV_KEY_SECURITY_CLASSIFICATION, "Security Classification", {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x03, 0x02, 0x08, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00}},
{KLV_KEY_SECURITY_RELEASE_INSTRUCTIONS, "Release Instructions", {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x03, 0x07, 0x01, 0x20, 0x01, 0x02, 0x09, 0x00, 0x00}},
{KLV_KEY_SECURITY_CAVEATS, "Caveats", {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x03, 0x02, 0x08, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00}},
{KLV_KEY_CLASSIFICATION_COMMENT, "Classification Comment", {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x03, 0x02, 0x08, 0x02, 0x07, 0x00, 0x00, 0x00, 0x00}},
{KLV_KEY_ORIGINAL_PRODUCER_NAME, "Original Producer Name", {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x02, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00}},
{KLV_KEY_PLATFORM_GROUND_SPEED,"Platform Ground Speed",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x01, 0x05, 0x00, 0x00, 0x00}},
{KLV_KEY_PLATFORM_MAGNETIC_HEADING_ANGLE,"Platform Magnetic Heading Angle",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x01, 0x08, 0x00, 0x00, 0x00}},
{KLV_KEY_PLATFORM_HEADING_ANGLE,"Platform Heading Angle",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x07, 0x07, 0x01, 0x10, 0x01, 0x06, 0x00, 0x00, 0x00}},
{KLV_KEY_PLATFORM_PITCH_ANGLE,"Platform Pitch Angle",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x07, 0x07, 0x01, 0x10, 0x01, 0x05, 0x00, 0x00, 0x00}},
{KLV_KEY_PLATFORM_ROLL_ANGLE, "Platform Roll Angle",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x07, 0x07, 0x01, 0x10, 0x01, 0x04, 0x00, 0x00, 0x00}},
{KLV_KEY_INDICATED_AIR_SPEED,"Platform Indicated Air Speed",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x01, 0x0B, 0x00, 0x00, 0x00}},
{KLV_KEY_PLATFORM_DESIGNATION,"Platform Designation",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00}},
{KLV_KEY_PLATFORM_DESIGNATION2,"Platform Designation",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x03, 0x01, 0x01, 0x21, 0x01, 0x00, 0x00, 0x00, 0x00}},
{KLV_KEY_IMAGE_SOURCE_SENSOR,"Image Source Sensor",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x04, 0x20, 0x01, 0x02, 0x01, 0x01, 0x00, 0x00}},
{KLV_KEY_IMAGE_COORDINATE_SYSTEM,"Image Coordinate System",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x07, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00}},
{KLV_KEY_SENSOR_LATITUDE,"Sensor Latitude",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x03, 0x07, 0x01, 0x02, 0x01, 0x02, 0x04, 0x02, 0x00}},
{KLV_KEY_SENSOR_LONGITUDE,"Sensor Longitude",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x03, 0x07, 0x01, 0x02, 0x01, 0x02, 0x06, 0x02, 0x00}},
{KLV_KEY_SENSOR_TRUE_ALTITUDE,"Sensor True Altitude",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x07, 0x01, 0x02, 0x01, 0x02, 0x02, 0x00, 0x00}},
{KLV_KEY_SENSOR_HORIZONTAL_FOV,"Sensor Horizontal Field Of View",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02, 0x04, 0x20, 0x02, 0x01, 0x01, 0x08, 0x00, 0x00}},
{KLV_KEY_SENSOR_VERTICAL_FOV1,"Sensor Vertical Field Of View",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x07, 0x04, 0x20, 0x02, 0x01, 0x01, 0x0A, 0x01, 0x00}},
{KLV_KEY_SENSOR_VERTICAL_FOV2,"Sensor Vertical Field Of View",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x03, 0x04, 0x20, 0x02, 0x01, 0x01, 0x0A, 0x01, 0x00}},
{KLV_KEY_SLANT_RANGE,"Slant Range",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x07, 0x01, 0x08, 0x01, 0x01, 0x00, 0x00, 0x00}},
{KLV_KEY_OBLIQUITY_ANGLE,"Obliquity Angle",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x07, 0x01, 0x10, 0x01, 0x03, 0x00, 0x00, 0x00}},
{KLV_KEY_ANGLE_TO_NORTH, "Angle To North", {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x07, 0x01, 0x10, 0x01, 0x02, 0x00, 0x00, 0x00}},
{KLV_KEY_TARGET_WIDTH,"Target Width",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x07, 0x01, 0x09, 0x02, 0x01, 0x00, 0x00, 0x00}},
{KLV_KEY_FRAME_CENTER_LATITUDE,"Frame Center Latitude",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x07, 0x01, 0x02, 0x01, 0x03, 0x02, 0x00, 0x00}},
{KLV_KEY_FRAME_CENTER_LONGITUDE,"Frame Center Longitude",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x07, 0x01, 0x02, 0x01, 0x03, 0x04, 0x00, 0x00}},
{KLV_KEY_FRAME_CENTER_ELEVATION,"Frame Center elevation",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x06, 0x07, 0x01, 0x02, 0x03, 0x10, 0x00, 0x00, 0x00}},   
{KLV_KEY_CORNER_LATITUDE_POINT_1,"Corner Latitude Point 1",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x03, 0x07, 0x01, 0x02, 0x01, 0x03, 0x07, 0x01, 0x00}},
{KLV_KEY_CORNER_LONGITUDE_POINT_1,"Corner Longitude Point 1",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x03, 0x07, 0x01, 0x02, 0x01, 0x03, 0x0B, 0x01, 0x00}},
{KLV_KEY_CORNER_LATITUDE_POINT_2,"Corner Latitude Point 2",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x03, 0x07, 0x01, 0x02, 0x01, 0x03, 0x08, 0x01, 0x00}},
{KLV_KEY_CORNER_LONGITUDE_POINT_2,"Corner Longitude Point 2",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x03, 0x07, 0x01, 0x02, 0x01, 0x03, 0x0C, 0x01, 0x00}},
{KLV_KEY_CORNER_LATITUDE_POINT_3,"Corner Latitude Point 3",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x03, 0x07, 0x01, 0x02, 0x01, 0x03, 0x09, 0x01, 0x00}},
{KLV_KEY_CORNER_LONGITUDE_POINT_3,"Corner Longitude Point 3",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x03, 0x07, 0x01, 0x02, 0x01, 0x03, 0x0D, 0x01, 0x00}},
{KLV_KEY_CORNER_LATITUDE_POINT_4,"Corner Latitude Point 4",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x03, 0x07, 0x01, 0x02, 0x01, 0x03, 0x0A, 0x01, 0x00}},
{KLV_KEY_CORNER_LONGITUDE_POINT_4,"Corner Longitude Point 4",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x03, 0x07, 0x01, 0x02, 0x01, 0x03, 0x0E, 0x01, 0x00}},
{KLV_KEY_DEVICE_ABSOLUTE_SPEED,"Device Absolute Speed",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x07, 0x01, 0x03, 0x01, 0x01, 0x01, 0x00, 0x00}},
{KLV_KEY_DEVICE_ABSOLUTE_HEADING,"Device Absolute Heading",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x07, 0x01, 0x03, 0x01, 0x01, 0x02, 0x00, 0x00}},
{KLV_KEY_ABSOLUTE_EVENT_START_DATE,"Absolute Event Start Date",{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x07, 0x02, 0x01, 0x02, 0x07, 0x01, 0x00, 0x00}},
{KLV_KEY_SENSOR_ROLL_ANGLE,"Sensor Roll Angle", {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x07, 0x01, 0x10, 0x01, 0x01, 0x00, 0x00, 0x00}},
{KLV_KEY_SENSOR_RELATIVE_ELEVATION_ANGLE,"Sensor Relative Elevation Angle", {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x02, 0x05, 0x00, 0x00, 0x00}},
{KLV_KEY_SENSOR_RELATIVE_AZIMUTH_ANGLE,"Sensor Relative Azimuth Angle", {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x02, 0x04, 0x00, 0x00, 0x00}},
{KLV_KEY_SENSOR_RELATIVE_ROLL_ANGLE,"Sensor Relative Roll Angle", {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x02, 0x06, 0x00, 0x00, 0x00}},
{KLV_KEY_UAS_LDS_VERSION_NUMBER,"UAS LDS Version Number", {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x0E, 0x01, 0x02, 0x03, 0x03, 0x00, 0x00, 0x00}},
{KLV_KEY_GENERIC_FLAG_DATA_01,"Generic Flag Data 01", {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x03, 0x01, 0x00, 0x00, 0x00}},
{KLV_KEY_STATIC_PRESSURE,"Static Pressure", {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01, 0x0E, 0x01, 0x01, 0x01, 0x0F, 0x00, 0x00, 0x00}},
{KLV_KEY_INVALID,"Invalid Key",{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}}
};

const ossim_uint8 ossimPredatorKlvTable::theKlvKey[4] = { 0x06,0x0e,0x2b,0x34 };
#define PRINT_KEY(x) printf("%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n", \
(x)[0], (x)[1], (x)[2], (x)[3], (x)[4], (x)[5], (x)[6], (x)[7], (x)[8], (x)[9], (x)[10], (x)[11], (x)[12], (x)[13], (x)[14], (x)[15])

static double mapValue(double value, double a, double b, double targetA, double targetB)
{
   double t = (value - a)/(b-a);
   return (targetA + (t*(targetB-targetA)));
}

ossimPredatorKlvTable::Node ossimPredatorKlvTable::convertValue(int id, const std::vector<ossim_uint8>& bufferValue)
{
  ossimPredatorKlvTable::Node result;
  result.theId = id;
  switch(id)
  {
    case KLV_KEY_FRAME_CENTER_LATITUDE:
    case KLV_KEY_SENSOR_LATITUDE:
    {
      ossim_int32 buf = *reinterpret_cast<const ossim_int32*>(&bufferValue.front());
      if(theNeedToSwapFlag)
      {
         theEndian.swap(buf);
      }

      //LAT: Map -(2^31-1)..(2^31-1) to +/-90.
      ossim_float64 value  = mapValue(buf, -2147483647, 2147483647, -90, 90);//(360.0*(((double)buf + 2147483647.0)/(4294967294.0)))-180.0; //180.0*((buf)/(double)((ossim_int64)(1<<31) - 1));
      //std::cout << "LAT == " << value << std::endl;
      if(theNeedToSwapFlag)
      {
         theEndian.swap(value);
      }
      ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
      result.theValue = std::vector<ossim_uint8>(tempBuf, tempBuf+8);
      //            std::cout << "SENSOR LAT!!" << std::endl;
      break;
    }

    case KLV_KEY_FRAME_CENTER_LONGITUDE:
    case KLV_KEY_SENSOR_LONGITUDE:
    {
      ossim_int32 buf = *reinterpret_cast<const ossim_int32*>(&bufferValue.front());
      if(theNeedToSwapFlag)
      {
         theEndian.swap(buf);
      }

      //LAT: Map -(2^31-1)..(2^31-1) to +/-180.
      ossim_float64 value  = mapValue(buf, -2147483647, 2147483647, -180, 180);//(360.0*(((double)buf + 2147483647.0)/(4294967294.0)))-180.0; //180.0*((buf)/(double)((ossim_int64)(1<<31) - 1));
      //std::cout << "LAT == " << value << std::endl;
      if(theNeedToSwapFlag)
      {
         theEndian.swap(value);
      }
      ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
      result.theValue = std::vector<ossim_uint8>(tempBuf, tempBuf+8);
      //            std::cout << "SENSOR LAT!!" << std::endl;
      break;
    }
    case KLV_KEY_SENSOR_TRUE_ALTITUDE:
    {
      ossim_uint16 buf = *reinterpret_cast<const ossim_uint16*>(&bufferValue.front());
      if(theNeedToSwapFlag)
      {
         theEndian.swap(buf);
      }
      ossim_float64 value  = mapValue(buf, 0, 65535, -900, 19000); 
      //std::cout << "ALTITUDE == " << value << std::endl;
      if(theNeedToSwapFlag)
      {
         theEndian.swap(value);
      }
      ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
      result.theValue = std::vector<ossim_uint8>(tempBuf, tempBuf+8);

      break;
    }
    case KLV_KEY_SENSOR_RELATIVE_ELEVATION_ANGLE:
    {
      //Map -(2^31-1)..(2^31-1) to +/-180.
      ossim_int32 buf = *reinterpret_cast<const ossim_int32*>(&bufferValue.front());
      if(theNeedToSwapFlag)
      {
         theEndian.swap(buf);
      }
      ossim_float64 value  = mapValue(buf, -2147483647.0,2147483647.0,-180.0,180.0);
      //std::cout << "REALATIVE Elevation == " << value << std::endl;
      if(theNeedToSwapFlag)
      {
         theEndian.swap(value);
      }
      ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
      result.theValue = std::vector<ossim_uint8>(tempBuf, tempBuf+8);
      break;
    }
    case KLV_KEY_SENSOR_VERTICAL_FOV1:
    case KLV_KEY_SENSOR_VERTICAL_FOV2:
    case KLV_KEY_SENSOR_HORIZONTAL_FOV:
    {
      // Map 0..(2^16-1) to 0..180.
      ossim_uint16 buf = *reinterpret_cast<const ossim_uint16*>(&bufferValue.front());
      if(theNeedToSwapFlag)
      {
         theEndian.swap(buf);
      }
      ossim_float32 value  = mapValue(buf, 0, 65535, 0, 180); 
      //std::cout << "VFOV == " << value << std::endl;
      if(theNeedToSwapFlag)
      {
         theEndian.swap(value);
      }
      ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
      result.theValue = std::vector<ossim_uint8>(tempBuf, tempBuf+4);
      break;  
    }
     case KLV_KEY_SENSOR_RELATIVE_AZIMUTH_ANGLE://KLV_KEY_SENSOR_RELATIVE_AZIMUTH_ANGLE
     {
        // Map 0..(2^32-1) to 0..360.
      ossim_uint32 buf = *reinterpret_cast<const ossim_uint32*>(&bufferValue.front());
        if(theNeedToSwapFlag)
        {
           theEndian.swap(buf);
        }
        ossim_float64 value  = mapValue(buf, 0, 4294967295, 0, 360); 
        //std::cout << "REALATIVE AZIMUTH == " << value << std::endl;
        if(theNeedToSwapFlag)
        {
           theEndian.swap(value);
        }
        ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
        result.theValue = std::vector<ossim_uint8>(tempBuf, tempBuf+8);
       //            std::cout << "SENSOR REALTIVE AZIMUTH ANGLE!!" << std::endl;
        break;
     }
     case KLV_KEY_SENSOR_RELATIVE_ROLL_ANGLE: //KLV_KEY_SENSOR_RELATIVE_ROLL_ANGLE
     {
        //0..(2^32-1) to 0..360.
        ossim_uint32 buf = *reinterpret_cast<const ossim_uint32*>(&bufferValue.front());
        if(theNeedToSwapFlag)
        {
           theEndian.swap(buf);
        }
        ossim_float64 value  = mapValue(buf, 0, 4294967295, 0, 360); 
        //std::cout << "REALATIVE roll == " << value << std::endl;
        //            std::cout << "SENSOR REALTIVE ROLL ANGLE!!" << std::endl;
        if(theNeedToSwapFlag)
        {
           theEndian.swap(value);
        }
        ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
        result.theValue = std::vector<ossim_uint8>(tempBuf, tempBuf+8);
        break;
     }
     case KLV_KEY_SLANT_RANGE: // KLV_KEY_SLANT_RANGE
     {
        // Map 0..(2^32-1) to 0..5000000
        ossim_uint32 buf = *reinterpret_cast<const ossim_uint32*>(&bufferValue.front());
        if(theNeedToSwapFlag)
        {
           theEndian.swap(buf);
        }
        ossim_float64 value  = mapValue(buf, 0, 4294967295, 0, 5000000); 
        //std::cout << "SLANT range == " << value << std::endl;
        //            std::cout << "SENSOR SLANT ANGLE!!" << std::endl;
        if(theNeedToSwapFlag)
        {
           theEndian.swap(value);
        }
        ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
        result.theValue = std::vector<ossim_uint8>(tempBuf, tempBuf+8);
        break;
     }
     case KLV_KEY_TARGET_WIDTH: //KLV_KEY_TARGET_WIDTH
     {
        //Map 0..(2^16-1) to 0..10000
        ossim_uint16 buf = *reinterpret_cast<const ossim_uint16*>(&bufferValue.front());
        if(theNeedToSwapFlag)
        {
           theEndian.swap(buf);
        }
        ossim_float32 value  = mapValue(buf, 0, 65535, 0, 10000); 
        //std::cout << "TARGET width == " << value << std::endl;
        //            std::cout << "TARGET WIDTH!!" << std::endl;
         if(theNeedToSwapFlag)
        {
           theEndian.swap(value);
        }
        ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
        result.theValue = std::vector<ossim_uint8>(tempBuf, tempBuf+4);
        break;
     }
     case KLV_KEY_FRAME_CENTER_ELEVATION:
     {
       // Map 0..(2^16-1) to -900..19000
        ossim_uint16 buf = *reinterpret_cast<const ossim_uint16*>(&bufferValue.front());
        if(theNeedToSwapFlag)
        {
           theEndian.swap(buf);
        }
        ossim_float64 value  = mapValue(buf, 0, 65535, -900, 19000); 
        //std::cout << "FRAME CENTER ELEV == " << value << std::endl;
        //            std::cout << "SENSOR SLANT ANGLE!!" << std::endl;
        if(theNeedToSwapFlag)
        {
           theEndian.swap(value);
        }
        ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
        result.theValue = std::vector<ossim_uint8>(tempBuf, tempBuf+8);
        break;
     }
     case KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_1:
     case KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_1:
     case KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_2:
     case KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_2:
     case KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_3:
     case KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_3:
     case KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_4:
     case KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_4:
     {
        // Map -(2^15-1)..(2^15-1) to +/-0.075.
        ossim_int16 buf = *reinterpret_cast<const ossim_int16*>(&bufferValue.front());
        if(theNeedToSwapFlag)
        {
           theEndian.swap(buf);
        }
        ossim_float64 value  = mapValue(buf, -32767, 32767, -0.075, 0.075); 
        if(theNeedToSwapFlag)
        {
           theEndian.swap(value);
        }
        ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
        result.theValue = std::vector<ossim_uint8>(tempBuf, tempBuf+8);
        break;
     }
     case KLV_KEY_STATIC_PRESSURE:
     {
        // Map 0..(2^16-1) to 0..5000 mbar.
        ossim_uint16 buf = *reinterpret_cast<const ossim_uint16*>(&bufferValue.front());
        if(theNeedToSwapFlag)
        {
           theEndian.swap(buf);
        }
        ossim_uint16 value  = mapValue(buf, 0, 65535, 0, 5000); 
        
        if(theNeedToSwapFlag)
        {
           theEndian.swap(value);
        }
        ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
        result.theValue = std::vector<ossim_uint8>(tempBuf, tempBuf+2);
        break;
     }
     case KLV_KEY_PLATFORM_MAGNETIC_HEADING_ANGLE:// KLV_KEY_PLATFORM_MAGNETIC_HEADING_ANGLE
     {
        // Map 0..(2^16-1) to 0..360.
        ossim_uint16 buf = *reinterpret_cast<const ossim_uint16*>(&bufferValue.front());
        if(theNeedToSwapFlag)
        {
           theEndian.swap(buf);
        }
        ossim_float64 value  = mapValue(buf, 0, 65535, 0, 360); 
        if(theNeedToSwapFlag)
        {
           theEndian.swap(value);
        }
        ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
        result.theValue = std::vector<ossim_uint8>(tempBuf, tempBuf+8);
       //            std::cout << "SENSOR REALTIVE AZIMUTH ANGLE!!" << std::endl;
       // platform magnetic heading
        break;
     }
    default:
    {
      result.theValue = bufferValue;
    }
  
  }
  return result;
}

static void printHex(const std::vector<ossim_uint8>& buf)
{
   ossim_uint32 idx = 0;
   while(idx < buf.size())
   {
      printf("%02X ", buf[idx]);
      if((idx%32) == 0)
      {
        // printf("\n");      
      }
      ++idx;
   }
}

bool ossimPredatorKlvTable::addKeys(const std::vector<ossim_uint8>& buffer)
{
   return addKeys(&buffer.front(), buffer.size());
}

ossim_int64 klv_decode_length(const ossim_uint8* buf, ossim_uint32& offset)
{
   ossim_int64 size = buf[offset];
   ossim_uint8 longTest = buf[offset];
   ++offset;
   if (longTest & 0x80)
   { /* long form */
      int bytes_num = longTest & 0x7f;
      /* SMPTE 379M 5.3.4 guarantee that bytes_num must not exceed 8 bytes */
      if (bytes_num > 8)
      {
         return -1;
      }
      size = 0;
      
      while (bytes_num--)
      {
         size = size << 8 | buf[offset];
         ++offset;
      }
   }
   
   return size;
}

ossimPredatorKlvTable::ossimPredatorKlvTable()
{
   theNeedToSwapFlag = theEndian.getSystemEndianType()!=OSSIM_BIG_ENDIAN;
}

bool ossimPredatorKlvTable::addKeys(const ossim_uint8* buffer, ossim_uint32 length)
{
  // std::cout << "addKeys---------------------------------\n";
  // std::cout << "ADDING KEYS!!!!!!!!!!!!\n";
   std::vector<ossim_uint8> bufferToParse;
   bufferToParse = theNeedToParseBuffer;
   if(buffer)
   {
      bufferToParse.insert(bufferToParse.end(),
                           buffer,
                           buffer + length);

   }
   theNeedToParseBuffer.clear();
   ossim_uint32 currentIdx = 0;
   ossim_uint32 totalLen = bufferToParse.size();
  // std::cout << "BUFFER TOTAL SIZE: " << totalLen << std::endl;
   if(totalLen < 1){
    //std::cout << "FIRST RETURN!!!!!!!!\n";
    return false;
   } 
   const ossim_uint8* bufPtr = &bufferToParse.front();
   //PRINT_KEY(&bufPtr[currentIdx]);
   while(std::equal(theKlvKey,
                    theKlvKey + 4,
                    &bufPtr[currentIdx])) // while we are looking at klv keys keep parsing
   {
      //std::cout << "CURRENT IDX, totallength ==== " << currentIdx << "," << totalLen << std::endl;
      //std::cout << "((currentIdx+16) >= totalLen)?" << ((currentIdx+16) >= totalLen) << std::endl;
      if((currentIdx+16) >= totalLen)
      {
         theNeedToParseBuffer.clear();
         theNeedToParseBuffer.insert(theNeedToParseBuffer.end(),
                                     &bufPtr[currentIdx],
                                     &bufPtr[totalLen]);
  //  std::cout << "SECOND RETURN!!!!!!!!\n";
         return true;
      }
      ossimPredatorKlvTable::Node node;
      ossim_uint32 saveIdx = currentIdx;
      ossim_int32 indexValue = findPredatorKlvIndex(&bufPtr[currentIdx]);

      if(indexValue >= 0)
      {
        node.theId = OSSIM_PREDATOR_UDS_TABLE[indexValue].theId;
      }
      else
      {
        node.theId = KLV_KEY_INVALID;
      }
      if(node.theId < 0)
      {
         if(traceDebug())
         {
            ossim_uint32 tempId = currentIdx + 16;
            std::cout << "**************UNDEFINED KEY**************\n";
            std::cout << "WITH SIZE === " << klv_decode_length(bufPtr, tempId) << std::endl;
            PRINT_KEY(&bufPtr[currentIdx]);
         }
         //if(bufPtr[currentIdx] == 0xff) return false;
         //ossim_uint32 tempId = currentIdx + 16;
         //std::cout << "**************UNDEFINED KEY**************\n";
         //std::cout << "WITH SIZE === " << klv_decode_length(bufPtr, tempId) << std::endl;
         // PRINT_KEY(&bufPtr[currentIdx]);
         //
         //ossim_float32 f = *reinterpret_cast<const ossim_float32*>(&bufPtr[tempId]);
         //theEndian.swap(f);
         //          std::cout << "POSSIBLE VALUE === " << f << std::endl; 
      }
    //  PRINT_KEY(&bufPtr[currentIdx]);
      currentIdx += 16;
      int klvLength = klv_decode_length(bufPtr, currentIdx);
      //std::cout << "offsets: " << (currentIdx+klvLength) << std::endl;
      if(klvLength > 0)
      {
         bool needMore = false;
         if((currentIdx + klvLength) <= totalLen)
         {
            node.theValue.insert(node.theValue.end(),
                                 &bufPtr[currentIdx], &bufPtr[currentIdx + klvLength]);
            currentIdx += klvLength;
            //std::cout << "KLV LENGTH ======= " << klvLength << std::endl;
            if(node.theId >=0)
            {
              //std::cout << "node.theId?  " << node.theId << std::endl;
               if((node.theId == KLV_BASIC_UNIVERSAL_METADATA_SET)||
                  (node.theId == KLV_KEY_SECURITY_CLASSIFICATION_SET)||
                  (node.theId == KLV_SECURITY_METADATA_UNIVERSAL_SET))
               {
                 addAbsoluteKeyDefinitions(node.theValue, needMore);
               }
               /*
               if((node.theId == KLV_BASIC_UNIVERSAL_METADATA_SET)||
                  (node.theId == KLV_KEY_SECURITY_CLASSIFICATION_SET))
               {
                  ossim_uint32 tempIdx = 0;
                  while(tempIdx < node.theValue.size())
                  {
                     if(node.theValue[tempIdx] == 0x06)
                     {
                        break;
                     }
                     ++tempIdx;
                  }
                     //addKeys(&node.theValue[tempIdx], node.theValue.size() - tempIdx);


                  if(currentIdx < totalLen)
                  {
                     theNeedToParseBuffer.insert(theNeedToParseBuffer.begin(),
                                                 &bufPtr[currentIdx],
                                                 &bufPtr[totalLen]);
                  }
//                  std::cout << "??????????????";
                  if(tempIdx < node.theValue.size())
                      theNeedToParseBuffer.insert(theNeedToParseBuffer.begin(),
                                                  node.theValue.begin()+tempIdx,
                                                  node.theValue.end());
                  //node.theValue.clear();
                  //node.theId = -1;
                  //PRINT_KEY(&theNeedToParseBuffer.front());
                  ////std::cout << "CALLING RECURSIVE!!!!!!!!!!!!!!!!!!!!!!" << node.theId << std::endl;
                  return addKeys(0,0);

               }
               */
               else if(node.theId == KLV_UAS_DATALINK_LOCAL_DATASET)
               {
                //std::cout << "KLV_UAS_DATALINK_LOCAL_DATASET\n";
                 klvMapType tempTable;
                 ossim_uint16 checksum;
                 // lod 16 byte key
                 std::vector<ossim_uint8> checksumBuffer(&bufPtr[saveIdx], &bufPtr[currentIdx-2]);
                 addUasDatalinkLocalDataSet(node.theValue, tempTable, checksum);

                 // now verify checksum
                 ossim_uint16 checksumCompare = compute16BitChecksum(checksumBuffer);
                    
                 if(checksum == checksumCompare)
                 {
                    klvMapType::iterator iter = tempTable.begin();
                    while(iter != tempTable.end())
                    {
                      theKlvParameters.insert(std::make_pair(iter->first, iter->second));
                      ++iter;
                    }
                 }
                 else
                 {
                //  std::cout << "WHAT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
                 }
 
                  //print(std::cout);
                  //node.theValue.clear();
                  //node.theId = -1;
                 // std::cout << "CURRENT IDX ====== " << currentIdx << ", " << totalLen << std::endl;
                 // if(currentIdx < totalLen)
                 // {
                 //    theNeedToParseBuffer.insert(theNeedToParseBuffer.begin(),
                 //                                &bufPtr[currentIdx],
                 //                                &bufPtr[totalLen]);
                 // }
                 // if(currentIdx == totalLen)
                 // {
                 //    return false;
                 // }
               }
               else
               {
                //if(klvLength > 16)
               // {
               //   addAbsoluteKeyDefinitions(node.theValue, needMore);

                //}
                //else
               // {
//                  std::cout << "INSERTING THIS THING HERE!!!!\n";
//                  std::cout << "KLV LENGTH ========================= " << klvLength << std::endl;
                  theKlvParameters.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(node.theId),
                                                        convertValue(node.theId, node.theValue)));
                //}
               }
                 if(needMore)
                 {

//                    std::cout << "DOING MORE STUFFFFF!!!!!!!!!!!!" << std::endl;
                    ossim_uint32 tempIdx = currentIdx - (klvLength+16);
                    theNeedToParseBuffer.clear();
                    theNeedToParseBuffer.insert(theNeedToParseBuffer.end(),
                                     &bufPtr[tempIdx],
                                     &bufPtr[totalLen]);

//    std::cout << "THIRD RETURN!!!!!!!!\n";
                    return true;
                 }


            }
         }
         else
         {
            theNeedToParseBuffer.clear();

            theNeedToParseBuffer.insert(theNeedToParseBuffer.end(),
                                        &bufPtr[saveIdx],
                                        &bufPtr[totalLen]);
//    std::cout << "FOURTH RETURN!!!!!!!!\n";
            return true;
         }
      }
   }

//std::cout << this->print(std::cout);
//std::cout << "___________________________\n";
//    std::cout << "FIFTH RETURN!!!!!!!!\n";
   return false;
}


void ossimPredatorKlvTable::clear()
{
   theKlvParameters.clear();
}

void ossimPredatorKlvTable::addAbsoluteKeyDefinitions(const std::vector<ossim_uint8>& buffer,
                                                      bool& needMore)
{
   needMore = false;
   ossim_uint32 currentIdx = 0;
   ossim_uint32 bufferSize = buffer.size();
   while(currentIdx < bufferSize)
   {
      if(buffer[currentIdx] == 0x06)
      {
        break;
      }
      ++currentIdx;
   }
   const ossim_uint8* bufPtr = &buffer.front();

   while((currentIdx < bufferSize)&&
         (std::equal(theKlvKey,
                    theKlvKey + 4,
                    &bufPtr[currentIdx])))
   {
      if((currentIdx+16) >= bufferSize)
      {
        break;
      }
      ossimPredatorKlvTable::Node node;
      ossim_uint32 saveIdx = currentIdx;
      ossim_int32 klvIndex = findPredatorKlvIndex(&bufPtr[currentIdx]);
      if(klvIndex >= 0)
      {
        node.theId = OSSIM_PREDATOR_UDS_TABLE[klvIndex].theId;
      }
      else
      {
        node.theId = KLV_KEY_INVALID;
      }
//      if(node.theId < 0)
//      {
//      }
      currentIdx += 16;
      int klvLength = klv_decode_length(bufPtr, currentIdx);
      //std::cout << "offsets: " << (currentIdx+klvLength) << std::endl;
      if(klvLength > 0)
      {

         if((currentIdx + klvLength) <= bufferSize)
         {
            std::vector<ossim_uint8> value;

            value.insert(value.end(),
                         &bufPtr[currentIdx], &bufPtr[currentIdx + klvLength]);
            if(node.theId >=0)
            {
              node = convertValue(node.theId, value);
             // switch(node.theId)
             // {
             //   default:
             //   {
                  theKlvParameters.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(node.theId),
                                                         node));
             //     break;
             //   }
             // }
            }
         }
         currentIdx += klvLength;
      }
   }
   if(currentIdx < bufferSize) needMore = true;
}

bool ossimPredatorKlvTable::valueAsString(ossimString& result,
                                          ossimPredatorKlvIndex id)const
{
   bool foundFlag = false;
   klvMapType::const_iterator i = theKlvParameters.find(id);
   if(i != theKlvParameters.end())
   {
      foundFlag = true;
      switch(i->second.theId)
      {
            // convert ossim_uint64 types
         case KLV_KEY_UNIX_TIMESTAMP:
         case KLV_KEY_USER_DEFINED_TIMESTAMP_MICROSECONDS_1970:
         {
//          std::cout << "ossimPredatorKlvTable::valueAsString: byte size ===== " <<i->second.theValue.size() << std::endl; 
            ossim_uint64 value = *reinterpret_cast<const ossim_uint64*>(&(i->second.theValue.front()));
            if(theNeedToSwapFlag)
            {
               theEndian.swap(value);
            }
            result = ossimString::toString(value);
            break;
         }
         // convert uint8 values
         case KLV_KEY_GENERIC_FLAG_DATA_01:
         case KLV_KEY_INDICATED_AIR_SPEED:
         case KLV_KEY_PLATFORM_GROUND_SPEED:
         case KLV_KEY_UAS_LDS_VERSION_NUMBER:
         {
            ossim_uint16 value = *reinterpret_cast<const ossim_uint8*>(&(i->second.theValue.front()));
            result = ossimString::toString(value);

            break;
         }
         // convert uint16 values
         case KLV_KEY_STATIC_PRESSURE:
         {
            ossim_uint16 value = *reinterpret_cast<const ossim_uint16*>(&(i->second.theValue.front()));
            if(theNeedToSwapFlag)
            {
               theEndian.swap(value);
            }
           result = ossimString::toString(value);
            break;
         }
            // convert string values
         case KLV_KEY_OBJECT_COUNTRY_CODES:
         case KLV_KEY_MISSION_NUMBER:
         case KLV_KEY_ORGANIZATIONAL_PROGRAM_NUMBER:
         case KLV_URL_STRING:
         case KLV_KEY_CLASSIFICATION_COMMENT:
         case KLV_KEY_SECURITY_RELEASE_INSTRUCTIONS:
         case KLV_KEY_SECURITY_CAVEATS:
         case KLV_KEY_SECURITY_CLASSIFICATION:
         case KLV_KEY_ORIGINAL_PRODUCER_NAME:
         case KLV_TIMESYSTEM_OFFSET:
         case KLV_KEY_BYTE_ORDER:
         case KLV_KEY_PLATFORM_DESIGNATION:
         case KLV_KEY_PLATFORM_DESIGNATION2:
         case KLV_KEY_IMAGE_SOURCE_SENSOR:
         case KLV_KEY_IMAGE_COORDINATE_SYSTEM:
         case KLV_KEY_ABSOLUTE_EVENT_START_DATE:
         case KLV_KEY_VIDEO_START_DATE_TIME_UTC:
         case KLV_KEY_PLATFORM_TAIL_NUMBER:
         {
				if(i->second.theValue.size()>0)
				{
					const char* bufPtr = reinterpret_cast<const char*>(&(i->second.theValue.front()));
					result = ossimString(bufPtr,
												bufPtr+i->second.theValue.size());
				}
				else
				{
					result = "";
				}
            break;
         }
            // convert double precision values
         case KLV_KEY_SENSOR_RELATIVE_ELEVATION_ANGLE:
         case KLV_KEY_PLATFORM_MAGNETIC_HEADING_ANGLE:
         case KLV_KEY_SENSOR_RELATIVE_ROLL_ANGLE:
         case KLV_KEY_SENSOR_RELATIVE_AZIMUTH_ANGLE:
         case KLV_KEY_SENSOR_LATITUDE:
         case KLV_KEY_SENSOR_LONGITUDE:
         case KLV_KEY_FRAME_CENTER_LATITUDE:
         case KLV_KEY_FRAME_CENTER_LONGITUDE:
         case KLV_KEY_CORNER_LATITUDE_POINT_1:
         case KLV_KEY_CORNER_LATITUDE_POINT_2:
         case KLV_KEY_CORNER_LATITUDE_POINT_3:
         case KLV_KEY_CORNER_LATITUDE_POINT_4:
         case KLV_KEY_CORNER_LONGITUDE_POINT_1:
         case KLV_KEY_CORNER_LONGITUDE_POINT_2:
         case KLV_KEY_CORNER_LONGITUDE_POINT_3:
         case KLV_KEY_CORNER_LONGITUDE_POINT_4:
         case KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_1:
         case KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_1:
         case KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_2:
         case KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_2:
         case KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_3:
         case KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_3:
         case KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_4:
         case KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_4:
         {
            ossim_float64 value = *reinterpret_cast<const ossim_float64*>(&(i->second.theValue.front()));
            if(theNeedToSwapFlag)
            {
               theEndian.swap(value);
            }
            result = ossimString::toString(value);
            break;
         }
         case KLV_KEY_SENSOR_TRUE_ALTITUDE:
         case KLV_KEY_FRAME_CENTER_ELEVATION:
         case KLV_KEY_SLANT_RANGE:
         case KLV_KEY_TARGET_WIDTH:
         case KLV_KEY_SENSOR_ROLL_ANGLE:
         case KLV_KEY_SENSOR_HORIZONTAL_FOV:
         case KLV_KEY_SENSOR_VERTICAL_FOV1:
         case KLV_KEY_SENSOR_VERTICAL_FOV2:
         case KLV_KEY_PLATFORM_HEADING_ANGLE:
         case KLV_KEY_PLATFORM_PITCH_ANGLE:
         case KLV_KEY_PLATFORM_ROLL_ANGLE:
         case KLV_KEY_OBLIQUITY_ANGLE:
         case KLV_KEY_ANGLE_TO_NORTH:
         case KLV_KEY_DEVICE_ABSOLUTE_SPEED:
         case KLV_KEY_DEVICE_ABSOLUTE_HEADING:
         {
            ossim_float32 value = *reinterpret_cast<const ossim_float32*>(&(i->second.theValue.front()));
            
            if(theNeedToSwapFlag)
            {
               theEndian.swap(value);
            }
            result = ossimString::toString(value);
            break;
         }
      }
   }
   return foundFlag;
}

ossimString ossimPredatorKlvTable::valueAsString(ossimPredatorKlvIndex id)const

{
   ossimString result;
   if(valueAsString(result, id))
   {
      return result;
   }
   
   return ossimString("");
}

bool ossimPredatorKlvTable::getCornerPoints(ossimGpt& pt1,
                                            ossimGpt& pt2,
                                            ossimGpt& pt3,
                                            ossimGpt& pt4)const
{
   bool result = false;
   klvMapType::const_iterator endIter = theKlvParameters.end();
   klvMapType::const_iterator lat1i = theKlvParameters.find(KLV_KEY_CORNER_LATITUDE_POINT_1);
   klvMapType::const_iterator lat2i = theKlvParameters.find(KLV_KEY_CORNER_LATITUDE_POINT_2);
   klvMapType::const_iterator lat3i = theKlvParameters.find(KLV_KEY_CORNER_LATITUDE_POINT_3);
   klvMapType::const_iterator lat4i = theKlvParameters.find(KLV_KEY_CORNER_LATITUDE_POINT_4);
   klvMapType::const_iterator lon1i = theKlvParameters.find(KLV_KEY_CORNER_LONGITUDE_POINT_1);
   klvMapType::const_iterator lon2i = theKlvParameters.find(KLV_KEY_CORNER_LONGITUDE_POINT_2);
   klvMapType::const_iterator lon3i = theKlvParameters.find(KLV_KEY_CORNER_LONGITUDE_POINT_3);
   klvMapType::const_iterator lon4i = theKlvParameters.find(KLV_KEY_CORNER_LONGITUDE_POINT_4);
   

//std::cout << "TRIED FRO KEY: " << KLV_KEY_CORNER_LATITUDE_POINT_1 << std::endl;
//   std::cout << "END ITER? " << (lat1i==endIter) << std::endl;
   if((lat1i!=endIter)&&
      (lat2i!=endIter)&&
      (lat3i!=endIter)&&
      (lat4i!=endIter)&&
      (lon1i!=endIter)&&
      (lon2i!=endIter)&&
      (lon3i!=endIter)&&
      (lon4i!=endIter))
   {
      
      ossim_float64  lat1 = *reinterpret_cast<const ossim_float64*>(&(lat1i->second.theValue.front()));
      ossim_float64  lat2 = *reinterpret_cast<const ossim_float64*>(&(lat2i->second.theValue.front()));
      ossim_float64  lat3 = *reinterpret_cast<const ossim_float64*>(&(lat3i->second.theValue.front()));
      ossim_float64  lat4 = *reinterpret_cast<const ossim_float64*>(&(lat4i->second.theValue.front()));
      ossim_float64  lon1 = *reinterpret_cast<const ossim_float64*>(&(lon1i->second.theValue.front()));
      ossim_float64  lon2 = *reinterpret_cast<const ossim_float64*>(&(lon2i->second.theValue.front()));
      ossim_float64  lon3 = *reinterpret_cast<const ossim_float64*>(&(lon3i->second.theValue.front()));
      ossim_float64  lon4 = *reinterpret_cast<const ossim_float64*>(&(lon4i->second.theValue.front()));

      if(theNeedToSwapFlag)
      {
         theEndian.swap(lat1);
         theEndian.swap(lat2);
         theEndian.swap(lat3);
         theEndian.swap(lat4);
         theEndian.swap(lon1);
         theEndian.swap(lon2);
         theEndian.swap(lon3);
         theEndian.swap(lon4);
         
      }
      pt1 = ossimGpt(lat1, lon1);
      pt2 = ossimGpt(lat2, lon2);
      pt3 = ossimGpt(lat3, lon3);
      pt4 = ossimGpt(lat4, lon4);
      
      result = true;
   }
   else
   {
      ossimDpt p1,p2,p3,p4;

      if(getFrameCenterOffsets(p1,p2,p3,p4))
      {
         ossim_float64 lat,lon,elevation;
         if(getFrameCenter(lat, lon, elevation))
         {
            pt1 = ossimGpt(lat+p1.lat, lon+p1.lon);
            pt2 = ossimGpt(lat+p2.lat, lon+p2.lon);
            pt3 = ossimGpt(lat+p3.lat, lon+p3.lon);
            pt4 = ossimGpt(lat+p4.lat, lon+p4.lon);
            result = true;
         }
      }
   }
   
   return result;
   
}

bool ossimPredatorKlvTable::getFrameCenterOffsets(ossimDpt& pt1Offset,
                                                   ossimDpt& pt2Offset,
                                                   ossimDpt& pt3Offset,
                                                   ossimDpt& pt4Offset)const
{
   bool result = false;
   klvMapType::const_iterator lat1i = theKlvParameters.find(KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_1);
   klvMapType::const_iterator lat2i = theKlvParameters.find(KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_2);
   klvMapType::const_iterator lat3i = theKlvParameters.find(KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_3);
   klvMapType::const_iterator lat4i = theKlvParameters.find(KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_4);
   klvMapType::const_iterator lon1i = theKlvParameters.find(KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_1);
   klvMapType::const_iterator lon2i = theKlvParameters.find(KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_2);
   klvMapType::const_iterator lon3i = theKlvParameters.find(KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_3);
   klvMapType::const_iterator lon4i = theKlvParameters.find(KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_4);
   
   if((lat1i!=theKlvParameters.end())&&
      (lat2i!=theKlvParameters.end())&&
      (lat3i!=theKlvParameters.end())&&
      (lat4i!=theKlvParameters.end())&&
      (lon1i!=theKlvParameters.end())&&
      (lon2i!=theKlvParameters.end())&&
      (lon3i!=theKlvParameters.end())&&
      (lon4i!=theKlvParameters.end()))
   {
      ossim_float64  lat1 = *reinterpret_cast<const ossim_float64*>(&(lat1i->second.theValue.front()));
      ossim_float64  lat2 = *reinterpret_cast<const ossim_float64*>(&(lat2i->second.theValue.front()));
      ossim_float64  lat3 = *reinterpret_cast<const ossim_float64*>(&(lat3i->second.theValue.front()));
      ossim_float64  lat4 = *reinterpret_cast<const ossim_float64*>(&(lat4i->second.theValue.front()));
      ossim_float64  lon1 = *reinterpret_cast<const ossim_float64*>(&(lon1i->second.theValue.front()));
      ossim_float64  lon2 = *reinterpret_cast<const ossim_float64*>(&(lon2i->second.theValue.front()));
      ossim_float64  lon3 = *reinterpret_cast<const ossim_float64*>(&(lon3i->second.theValue.front()));
      ossim_float64  lon4 = *reinterpret_cast<const ossim_float64*>(&(lon4i->second.theValue.front()));
      
      if(theNeedToSwapFlag)
      {
         theEndian.swap(lat1);
         theEndian.swap(lat2);
         theEndian.swap(lat3);
         theEndian.swap(lat4);
         theEndian.swap(lon1);
         theEndian.swap(lon2);
         theEndian.swap(lon3);
         theEndian.swap(lon4);
      }
      pt1Offset.lat = lat1;
      pt2Offset.lat = lat2;
      pt3Offset.lat = lat3;
      pt4Offset.lat = lat4;
      pt1Offset.lon = lon1;
      pt2Offset.lon = lon2;
      pt3Offset.lon = lon3;
      pt4Offset.lon = lon4;
      result = true;
   }

   return result;
}


bool ossimPredatorKlvTable::getFrameCenter(ossim_float64& lat,
                                           ossim_float64& lon,
                                           ossim_float64& elevation)const
{
   klvMapType::const_iterator lati  = theKlvParameters.find(KLV_KEY_FRAME_CENTER_LATITUDE);
   klvMapType::const_iterator loni  = theKlvParameters.find(KLV_KEY_FRAME_CENTER_LONGITUDE);
   klvMapType::const_iterator elevi = theKlvParameters.find(KLV_KEY_FRAME_CENTER_ELEVATION);
   
   if((lati != theKlvParameters.end())&&
      (loni != theKlvParameters.end()))
   {
      lat =  *reinterpret_cast<const ossim_float64*>(&(lati->second.theValue.front()));
      lon =  *reinterpret_cast<const ossim_float64*>(&(loni->second.theValue.front()));
      ossim_float32 elev = 0;
      if(elevi != theKlvParameters.end())
      {
         elev = *reinterpret_cast<const ossim_float32*>(&(elevi->second.theValue.front()));
         if(theNeedToSwapFlag)
         {
            theEndian.swap(elev);
         }
      }
      if(theNeedToSwapFlag)
      {
         theEndian.swap(lat);
         theEndian.swap(lon);
      }
      elevation = elev;
      return true;
   }
   
   return false;
}

bool ossimPredatorKlvTable::getSensorPosition(ossim_float64& lat,
                                              ossim_float64& lon,
                                              ossim_float64& elevation)const
{
   klvMapType::const_iterator lati  = theKlvParameters.find(KLV_KEY_SENSOR_LATITUDE);
   klvMapType::const_iterator loni  = theKlvParameters.find(KLV_KEY_SENSOR_LONGITUDE);
   klvMapType::const_iterator elevi = theKlvParameters.find(KLV_KEY_SENSOR_TRUE_ALTITUDE);
   
   if((lati != theKlvParameters.end())&&
      (loni != theKlvParameters.end())&&
      (elevi != theKlvParameters.end()))
   {
      lat =  *reinterpret_cast<const ossim_float64*>(&(lati->second.theValue.front()));
      lon =  *reinterpret_cast<const ossim_float64*>(&(loni->second.theValue.front()));
      ossim_float32 elev = *reinterpret_cast<const ossim_float32*>(&(elevi->second.theValue.front()));
      if(theNeedToSwapFlag)
      {
         theEndian.swap(lat);
         theEndian.swap(lon);
         theEndian.swap(elev);
      }     
      elevation = elev;//*0.304801;
      return true;
   }
   
   return false;
}

bool ossimPredatorKlvTable::getPlatformOrientation(ossim_float32& heading,
                                                   ossim_float32& pitch,
                                                   ossim_float32& roll)const
{
   heading = valueAsString(KLV_KEY_PLATFORM_HEADING_ANGLE).toFloat32();
   pitch = valueAsString(KLV_KEY_PLATFORM_PITCH_ANGLE).toFloat32();
   roll = valueAsString(KLV_KEY_PLATFORM_ROLL_ANGLE).toFloat32();
   
   return true;
}

bool ossimPredatorKlvTable::getSensorRollAngle(ossim_float32& angle)const
{
   ossimString value = valueAsString(KLV_KEY_SENSOR_ROLL_ANGLE);
   if(!value.empty())
   {
      angle = value.toFloat32();
   }
   
   return !value.empty();
}

bool ossimPredatorKlvTable::getObliquityAngle(ossim_float32& angle)const
{
   klvMapType::const_iterator value  = theKlvParameters.find(KLV_KEY_OBLIQUITY_ANGLE);
   angle = 0.0;
   if(value!=theKlvParameters.end())
   {
      angle =  *reinterpret_cast<const ossim_float32*>(&(value->second.theValue.front()));
      if(theNeedToSwapFlag)
      {
         theEndian.swap(angle);
      }
   }
   
   return (value!=theKlvParameters.end());
}

bool ossimPredatorKlvTable::getSlantRange(ossim_float32& range)const
{
   klvMapType::const_iterator value  = theKlvParameters.find(KLV_KEY_SLANT_RANGE);
   range = 0.0;
   if(value!=theKlvParameters.end())
   {
      range =  *reinterpret_cast<const ossim_float32*>(&(value->second.theValue.front()));
      if(theNeedToSwapFlag)
      {
         theEndian.swap(range);
      }
   }
   
   return (value!=theKlvParameters.end());
   
}
bool ossimPredatorKlvTable::getHorizontalFieldOfView(ossim_float32& hfov)const
{
   klvMapType::const_iterator value  = theKlvParameters.find(KLV_KEY_SENSOR_HORIZONTAL_FOV);
   hfov = 0.0;
   if(value!=theKlvParameters.end())
   {
      hfov =  *reinterpret_cast<const ossim_float32*>(&(value->second.theValue.front()));
      if(theNeedToSwapFlag)
      {
         theEndian.swap(hfov);
      }
   }
   
   return (value!=theKlvParameters.end());
}

bool ossimPredatorKlvTable::getVerticalFieldOfView(ossim_float32& vfov)const
{
   klvMapType::const_iterator value  = theKlvParameters.find(KLV_KEY_SENSOR_VERTICAL_FOV1);
   if(value == theKlvParameters.end())
   {
      value  = theKlvParameters.find(KLV_KEY_SENSOR_VERTICAL_FOV2);
   }
   vfov = 0.0;
   if(value!=theKlvParameters.end())
   {
      vfov =  *reinterpret_cast<const ossim_float32*>(&(value->second.theValue.front()));
      if(theNeedToSwapFlag)
      {
         theEndian.swap(vfov);
      }
   }
   
   return (value!=theKlvParameters.end());
}

bool ossimPredatorKlvTable::getAngleToNorth(ossim_float32& angleToNorth)const
{
   klvMapType::const_iterator value  = theKlvParameters.find(KLV_KEY_ANGLE_TO_NORTH);
   angleToNorth = 0.0;
   if(value!=theKlvParameters.end())
   {
      angleToNorth =  *reinterpret_cast<const ossim_float32*>(&(value->second.theValue.front()));
      if(theNeedToSwapFlag)
      {
         theEndian.swap(angleToNorth);
      }
   }
   
   return (value!=theKlvParameters.end());
   
}

bool ossimPredatorKlvTable::getTargetWidthInMeters(ossim_float32& targetWidth)const
{
   ossimString value = valueAsString(KLV_KEY_TARGET_WIDTH);
   
   targetWidth = value.toFloat32();
   
   return !value.empty();
}

ossimString ossimPredatorKlvTable::getUtcTimestamp()const
{
   return valueAsString(KLV_KEY_VIDEO_START_DATE_TIME_UTC);
}


bool ossimPredatorKlvTable::getUnixEpocTimestamp(ossim_int64& timestamp)const
{
   klvMapType::const_iterator timei  = theKlvParameters.end();//theKlvParameters.find(KLV_KEY_USER_DEFINED_TIMESTAMP_MICROSECONDS_1970);
   if(timei == theKlvParameters.end())
   {
      timei  = theKlvParameters.find(KLV_KEY_UNIX_TIMESTAMP);
   }
   if(timei == theKlvParameters.end())
   {
      timei = theKlvParameters.find(KLV_KEY_USER_DEFINED_TIMESTAMP_MICROSECONDS_1970);
   }
   if(timei != theKlvParameters.end())
   {
      timestamp =  *reinterpret_cast<const ossim_uint64*>(&(timei->second.theValue.front()));
      if(theNeedToSwapFlag)
      {
         theEndian.swap(timestamp);
      }
      return true;
   }
   
   return false;
}

bool ossimPredatorKlvTable::getUnixEpocTimestampInSeconds(ossim_int64& seconds,
                                                          ossim_float64& fractionalPart)const
{
   ossim_int64 epocInMicroseconds;
   bool result = getUnixEpocTimestamp(epocInMicroseconds);
   ossim_float64 fractionalSeconds = static_cast<ossim_float64>(epocInMicroseconds*1e-6);
   seconds = static_cast<ossim_uint64>(fractionalSeconds);
   fractionalPart = fractionalSeconds - seconds;
   
   return result;
}

bool ossimPredatorKlvTable::getUnixEpocTimestampInFractionalSeconds(ossim_float64& fractionalSeconds)const
{
   ossim_int64 epocInMicroseconds;
   bool result = getUnixEpocTimestamp(epocInMicroseconds);
   fractionalSeconds = static_cast<ossim_float64>(epocInMicroseconds/10e-6);
   
   return result;
}

bool ossimPredatorKlvTable::getDate(ossimDate& d, bool shiftToGmtZero)const
{
   bool result = getDateUsingUtc(d, shiftToGmtZero);
   
   if(!result)
   {
      result = getDateUsingEpoc(d);
   }
   return result;
}

bool ossimPredatorKlvTable::getDateUsingEpoc(ossimDate& d)const
{
   bool result = false;
   ossim_int64 t;
   ossim_float64 fraction=0;
   
   if(getUnixEpocTimestampInSeconds(t, fraction))
   {
      result = true;
      d.setTimeNoAdjustmentGivenEpoc(t);
      d.setFractionalSecond(fraction);
   }
   
   return result;
}

bool ossimPredatorKlvTable::getDateUsingUtc(ossimDate& d, bool shiftToGmtZero)const
{
   bool result = false;
   ossimString utc = getUtcTimestamp();
   if(utc.size() > 0)
   {
      result = d.setIso8601(utc, shiftToGmtZero);
   }
   return result;
}

int ossimPredatorKlvTable::findPredatorKlvIndex(const ossim_uint8* buf)const
{
   ossim_int32 idx = 0;
   

   while(OSSIM_PREDATOR_UDS_TABLE[idx].theId != -1)
   {
      if(std::equal(buf,
                    buf+16,
                    OSSIM_PREDATOR_UDS_TABLE[idx].theKey))
      {
         return idx;
      }
      ++idx;
   }
   
   return -1;
}

int ossimPredatorKlvTable::findPredatorKlvIndexByKey(ossim_uint32 key)const
{
   ossim_int32 idx = 0;
   

   while(OSSIM_PREDATOR_UDS_TABLE[idx].theId != -1)
   {
      if(key == OSSIM_PREDATOR_UDS_TABLE[idx].theId)
      {
        return idx;
      }
      ++idx;
   }
   
   return -1;
}

void ossimPredatorKlvTable::loadVariableDataNullTerminated(std::vector<ossim_uint8>& result, 
                                                           const std::vector<ossim_uint8>& buf, 
                                                           ossim_uint32& idx)const
{
   while((idx < buf.size())&&(buf[idx] != '\0'))
   {
      result.push_back(buf[idx]);
      ++idx;
   }
   if(idx < buf.size())
   {
      ++idx;
   }
}


void ossimPredatorKlvTable::addSecurityMetadataLocalSetElements(const std::vector<ossim_uint8>& buffer, klvMapType& tempTable)
{
   //std::cout << "addSecurityMetadataLocalSetElements: ....................... entered\n";
   if(buffer.size() == 0) return;
   ossim_uint32 idx = 0;
   bool done = false;
   while(!done&&((idx+2) < buffer.size()))
   {
      ossim_uint32 key    = buffer[idx++];
      ossim_uint32 length = buffer[idx++];
      if(length > 0)
      {
        switch(key)
        {
           case 1://Security Classification
           {
              ossim_uint16 classification = (ossim_uint16)(*(buffer.begin()+idx));
              ossimString value;
              switch(classification)
              {
                 case 0x01:
                 {
                    value = "UNCLASSIFIED";
                    break;
                 }
                 case 0x02:
                 {
                    value = "RESTRICTED";
                    break;
                 }
                 case 0x03:
                 {
                    value = "CONFIDENTIAL";
                    break;
                 }
                 case 0x04:
                 {
                    value = "SECRET";
                    break;
                 }
                 case 0x05:
                 {
                    value = "TOP SECRET";
                    break;
                 }
                 
              }
              //UNCLASSIFIED (0x01) RESTRICTED (0x02) CONFIDENTIAL (0x03) SECRET (0x04) TOPSECRET(0x05)
              //std::cout << "SECURITY CLASS: " << value << std::endl;

              Node n(KLV_KEY_SECURITY_CLASSIFICATION,
                     std::vector<ossim_uint8>(value.begin(), value.end()));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              break;
           }
           case 2://Classifying Country and Releasing Instructions Country Coding Method
           {
              ossim_uint16 classifyingCountry = (*(buffer.begin()+idx));
              // ISO-3166 Two Letter (0x01) ISO-3166 Three Letter (0x02) FIPS 10-4 Two Letter (0x03) FIPS 10-4 Four Letter (0x04)
              if(traceDebug())  std::cout << "Classifying Country and Releasing Instructions Country Coding Method Not Handled: " << std::endl;
              //ossim_uint8 value = (*(buffer.begin()+idx));

              break;
           }
           case 3://Classifying Country
           {
              if(traceDebug())  std::cout << "Classifying Country Not Handled: " << std::endl;
              break;
           }
           case 12://Object Country Coding Method
           {
               //std::cout << "Object Country Coding Method: " << (int)(*(buffer.begin()+idx)) << std::endl;
              //ossim_uint8 value = (*(buffer.begin()+idx));
             break;
           }
           case 13://Object Country Codes
           {
              Node n(KLV_KEY_OBJECT_COUNTRY_CODES,
                     std::vector<ossim_uint8>(buffer.begin()+idx, buffer.begin()+(idx+length)));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              //std::cout << "Object Country Codes: " << ossimString(buffer.begin() +idx, buffer.begin()+(idx+length)) << std::endl;
              break;
           }
           case 4://Security-SCI information
           case 5://Caveats
           case 6://Releasing Instructions
           case 7://Classified By
           case 8://Derived From
           case 9://Classification Reason
           case 10://Declassification Date
           case 11://Classification and Marking System
           case 14://Classification Comments
           case 15://UMID Video
           case 16://UMID Audio
           case 17://UMID Data
           case 18://UMID System
           case 19://Stream ID
           case 20://Transport Stream ID
           case 21://Item Designator ID
           default:
           {
              if(traceDebug()) std::cout << "SECURITY KEY " << key << " Not handled\n";
              break;
           }
        }
      }
      idx +=length;
   }
}

//#include <ossim/base/ossimTraceManager.h>
void ossimPredatorKlvTable::addUasDatalinkLocalDataSet(const std::vector<ossim_uint8>& buffer, klvMapType& tempTable, ossim_uint16& checksum)
{
 

//   ossimTraceManager::instance()->setTracePattern("ossimPredatorKlvTable.*");
//   if (traceDebug()) std::cout << "ossimPredatorKlvTable::addUasDatalinkLocalDataSet: entered....................\n";
   if(buffer.size() == 0) return;
   ossim_uint32 idx = 0;
   bool done = false;
   while(!done&&((idx+2) < buffer.size()))
   {
      ossim_uint32 key= buffer[idx++];
      ossim_uint32 length= buffer[idx++];
      //std::cout << "KEY ====================== " << key << std::endl;
      //std::cout << "LENGTH ====================== " << length << std::endl;
      if(length > 0)
      {
        switch(key)
        {
           case 1: // checksum
           {
              //std::cout << "CHECKSUM\n";
              ossim_uint16 buf = *reinterpret_cast<const ossim_uint16*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              checksum = buf;
              break;
           }
           case 2: // unix timestamp
           {
              ossim_uint64 value = *reinterpret_cast<const ossim_uint64*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_UNIX_TIMESTAMP,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              
              break;
           }
           case 3:
           {
              Node n(KLV_KEY_MISSION_NUMBER,
                     std::vector<ossim_uint8>(buffer.begin()+idx,
                                              buffer.begin()+idx+length));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));

              //std::cout << "MISSION ID!!" << std::endl;
              break;
           }
           case 4:
           {
              Node n(KLV_KEY_PLATFORM_TAIL_NUMBER,
                     std::vector<ossim_uint8>(buffer.begin()+idx,
                                              buffer.begin()+idx+length));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));

              //std::cout << "PLATFORM TAIL!! " << valueAsString(KLV_KEY_PLATFORM_TAIL_NUMBER) << std::endl;
              break;
           }
           case 5:
           {
              ossim_uint16 buf = *reinterpret_cast<const ossim_uint16*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_float64 heading  = mapValue(buf, 0.0, (1<<16) - 1, 0.0, 360.0);
              //std::cout << "HEADING == " << heading << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(heading);
              }

              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&heading);
              Node n(KLV_KEY_PLATFORM_HEADING_ANGLE,
                  std::vector<ossim_uint8>(tempBuf,
                                           tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),n));
              //            std::cout << "PLATFORM HEADING!!" << std::endl;
              break;
           }
           case 6:
           {
              ossim_int16 buf = *reinterpret_cast<const ossim_int16*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_float64 value  = mapValue(buf, -((1<<15)-1), ((1<<15)-1), -20,20);//20.0*(buf)/(double)((1<<15) - 1);
              //std::cout << "PITCH == " << value << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_PLATFORM_PITCH_ANGLE,
                  std::vector<ossim_uint8>(tempBuf,
                                           tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),n));
             //            std::cout << "PLATFORM PITCH!!" << std::endl;
              break;
           }
           case 7:
           {
              ossim_int16 buf = *reinterpret_cast<const ossim_int16*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              double value  = mapValue(buf, -((1<<15)-1), ((1<<15)-1), -50,50);
              //std::cout << "ROLL == " << value << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_PLATFORM_ROLL_ANGLE,
                  std::vector<ossim_uint8>(tempBuf,
                                           tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),n));
              //            std::cout << "PLATFORM ROLL!!" << std::endl;
              break;
           }
           case 8:
           {
             //ossim_uint8 buf = *reinterpret_cast<const ossim_uint8*>(&buffer.front()+idx);
              
              if(traceDebug()) std::cout << "PLATFORM TRUE AIRSPEED not handled!!" << std::endl;
              break;
           }
           case 9:
           {
              Node n(KLV_KEY_INDICATED_AIR_SPEED,
                     std::vector<ossim_uint8>(buffer.begin()+idx,
                                              buffer.begin()+idx+length));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));

              //if(traceDebug()) std::cout << "PLATFORM INDICATED AIRSPEED!!" << std::endl;
              break;
           }
           case 10:
           {
              Node n(KLV_KEY_PLATFORM_DESIGNATION,
                     std::vector<ossim_uint8>(buffer.begin()+idx,
                                              buffer.begin()+idx+length));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              //std::cout << "PLATFORM DESIGNATION: " << valueAsString(KLV_KEY_PLATFORM_DESIGNATION) << std::endl;
              //if(traceDebug()) std::cout << "PLATFORM DESIGNATION not handled!!" << std::endl;
              break;
           }
           case 11: // KLV_KEY_IMAGE_SOURCE_SENSOR
           {
              Node n(KLV_KEY_IMAGE_SOURCE_SENSOR,
                     std::vector<ossim_uint8>(buffer.begin()+idx,
                                              buffer.begin()+idx+length));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
             // std::cout << "IMAGE SOURCE SENSOR!!" << valueAsString(KLV_KEY_IMAGE_SOURCE_SENSOR).trim()<< std::endl;
              break;
           }
           case 12:
           {
              Node n(KLV_KEY_IMAGE_COORDINATE_SYSTEM,
                     std::vector<ossim_uint8>(buffer.begin()+idx,
                                              buffer.begin()+idx+length));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));

              //std::cout << "IMAGE COORDINATE SYSTEM!!" << std::endl;
              break;
           }
           case 13://KLV_KEY_SENSOR_LATITUDE
           {
              ossim_int32 buf = *reinterpret_cast<const ossim_int32*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }

              // Map -(2^31-1)..(2^31-1) to +/-90.
              ossim_float64 value  = mapValue(buf, -2147483647, 2147483647, -90, 90);//(360.0*(((double)buf + 2147483647.0)/(4294967294.0)))-180.0; //180.0*((buf)/(double)((ossim_int64)(1<<31) - 1));
              //std::cout << "LAT == " << value << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_SENSOR_LATITUDE,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              //            std::cout << "SENSOR LAT!!" << std::endl;
              break;
           }
           case 14://KLV_KEY_SENSOR_LONGITUDE
           {
              ossim_int32 buf = *reinterpret_cast<const ossim_int32*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              //KLV_KEY_SENSOR_LONGITUDE
              // Map -(2^31-1)..(2^31-1) to +/-180.
              ossim_float64 value  = mapValue(buf, -2147483647, 2147483647, -180, 180); //180.0*((buf)/(double)((ossim_int64)(1<<31) - 1));
              //std::cout << "LON == " << value << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_SENSOR_LONGITUDE,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));

              //            std::cout << "SENSOR LON!!" << std::endl;
              break;
           }
           case 15://KLV_KEY_SENSOR_TRUE_ALTITUDE
           {
              ossim_uint16 buf = *reinterpret_cast<const ossim_uint16*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_float64 value  = mapValue(buf, 0, 65535, -900, 19000); 
              //std::cout << "ALTITUDE == " << value << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_SENSOR_TRUE_ALTITUDE,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
  //            std::cout << "SENSOR TRUE ALTITUDE!!" << std::endl;
              break;
           }
           case 16://KLV_KEY_SENSOR_HORIZONTAL_FOV
           {
              // map -2^16-1..2^16-1 to 0..180
             ossim_uint16 buf = *reinterpret_cast<const ossim_uint16*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_float32 value  = mapValue(buf, 0, 65535, 0, 180); 
              //std::cout << "HFOV == " << value << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_SENSOR_HORIZONTAL_FOV,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+4));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              //            std::cout << "SENSOR HORIZONTAL FOV!!" << std::endl;
              break;
           }
           case 17://KLV_KEY_SENSOR_VERTICAL_FOV1
           {
              // Map 0..(2^16-1) to 0..180.
              ossim_uint16 buf = *reinterpret_cast<const ossim_uint16*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_float32 value  = mapValue(buf, 0, 65535, 0, 180); 
              //std::cout << "VFOV == " << value << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_SENSOR_VERTICAL_FOV1,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+4));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              break;
           }
           case 18://KLV_KEY_SENSOR_RELATIVE_AZIMUTH_ANGLE
           {
              // Map 0..(2^32-1) to 0..360.
              ossim_uint32 buf = *reinterpret_cast<const ossim_uint32*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_float64 value  = mapValue(buf, 0, 4294967295, 0, 360); 
              //std::cout << "REALATIVE AZIMUTH == " << value << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_SENSOR_RELATIVE_AZIMUTH_ANGLE,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
             //            std::cout << "SENSOR REALTIVE AZIMUTH ANGLE!!" << std::endl;
              break;
           }
           case 19://KLV_KEY_SENSOR_RELATIVE_ELEVATION_ANGLE
           {
              //Map -(2^31-1)..(2^31-1) to +/-180.
              ossim_uint16 buf = *reinterpret_cast<const ossim_uint16*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_float64 value  = mapValue(buf, -2147483647.0,2147483647.0,-180.0,180.0);
              //std::cout << "REALATIVE Elevation == " << value << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_SENSOR_RELATIVE_ELEVATION_ANGLE,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              break;
           }
           case 20: //KLV_KEY_SENSOR_RELATIVE_ROLL_ANGLE
           {
              //0..(2^32-1) to 0..360.
              ossim_uint16 buf = *reinterpret_cast<const ossim_uint16*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_float64 value  = mapValue(buf, 0, 4294967295, 0, 360); 
              //std::cout << "REALATIVE roll == " << value << std::endl;
              //            std::cout << "SENSOR REALTIVE ROLL ANGLE!!" << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_SENSOR_RELATIVE_ROLL_ANGLE,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              break;
           }
           case 21: // KLV_KEY_SLANT_RANGE
           {
              // Map 0..(2^32-1) to 0..5000000
              ossim_uint32 buf = *reinterpret_cast<const ossim_uint32*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_float64 value  = mapValue(buf, 0, 4294967295, 0, 5000000); 
              //std::cout << "SLANT range == " << value << std::endl;
              //            std::cout << "SENSOR SLANT ANGLE!!" << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_SLANT_RANGE,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              break;
           }
           case 22: //KLV_KEY_TARGET_WIDTH
           {
              //Map 0..(2^16-1) to 0..10000
              ossim_uint16 buf = *reinterpret_cast<const ossim_uint16*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_float32 value  = mapValue(buf, 0, 65535, 0, 10000); 
              //std::cout << "TARGET width == " << value << std::endl;
              //            std::cout << "TARGET WIDTH!!" << std::endl;
               if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_TARGET_WIDTH,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+4));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              break;
           }
           case 23:
           {
              // Map -(2^31-1)..(2^31-1) to +/-90
              ossim_int32 buf = *reinterpret_cast<const ossim_int32*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_float64 value  = mapValue(buf, -2147483647, 2147483647, -90.0, 90.0); 
              //std::cout << "FRAME CENTER LAT == " << value << std::endl;
              //            std::cout << "SENSOR SLANT ANGLE!!" << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_FRAME_CENTER_LATITUDE,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              break;
           }
           case 24:
           {
              // Map -(2^31-1)..(2^31-1) to +/-180
              ossim_int32 buf = *reinterpret_cast<const ossim_int32*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_float64 value  = mapValue(buf, -2147483647, 2147483647, -180.0, 180.0); 
              //std::cout << "FRAME CENTER LON == " << value << std::endl;
              //            std::cout << "SENSOR SLANT ANGLE!!" << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_FRAME_CENTER_LONGITUDE,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              break;
           }
           case 25:
           {
             // Map 0..(2^16-1) to -900..19000
              ossim_uint16 buf = *reinterpret_cast<const ossim_int16*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_float64 value  = mapValue(buf, 0, 65535, -900, 19000); 
              //std::cout << "FRAME CENTER ELEV == " << value << std::endl;
              //            std::cout << "SENSOR SLANT ANGLE!!" << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_FRAME_CENTER_ELEVATION,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              break;
           }
           case 26: 
           {
              // Map -(2^15-1)..(2^15-1) to +/-0.075.
              ossim_int16 buf = *reinterpret_cast<const ossim_int16*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_float64 value  = mapValue(buf, -32767, 32767, -0.075, 0.075); 
              //std::cout << "offset corner lat 1 == " << value << std::endl;
              //            std::cout << "SENSOR SLANT ANGLE!!" << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_1,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              break;
           }
           case 27:
           {
              // Map -(2^15-1)..(2^15-1) to +/-0.075.
              ossim_int16 buf = *reinterpret_cast<const ossim_int16*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_float64 value  = mapValue(buf, -32767, 32767, -0.075, 0.075); 
              //std::cout << "offset corner lon 1 == " << value << std::endl;
              //            std::cout << "SENSOR SLANT ANGLE!!" << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_1,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              break;
           }
           case 28:
           {
              // Map -(2^15-1)..(2^15-1) to +/-0.075.
              ossim_int16 buf = *reinterpret_cast<const ossim_int16*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_float64 value  = mapValue(buf, -32767, 32767, -0.075, 0.075); 
              //std::cout << "offset corner lat 2 == " << value << std::endl;
              //            std::cout << "SENSOR SLANT ANGLE!!" << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_2,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              break;
           }
           case 29:
           {
              // Map -(2^15-1)..(2^15-1) to +/-0.075.
              ossim_int16 buf = *reinterpret_cast<const ossim_int16*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_float64 value  = mapValue(buf, -32767, 32767, -0.075, 0.075); 
              //std::cout << "offset corner lon 2 == " << value << std::endl;
              //            std::cout << "SENSOR SLANT ANGLE!!" << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_2,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              break;
           }
           case 30:
           {
              // Map -(2^15-1)..(2^15-1) to +/-0.075.
              ossim_int16 buf = *reinterpret_cast<const ossim_int16*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_float64 value  = mapValue(buf, -32767, 32767, -0.075, 0.075); 
              //std::cout << "offset corner lat 3 == " << value << std::endl;
              //            std::cout << "SENSOR SLANT ANGLE!!" << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_3,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              break;
           }
           case 31:
           {
              // Map -(2^15-1)..(2^15-1) to +/-0.075.
              ossim_int16 buf = *reinterpret_cast<const ossim_int16*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_float64 value  = mapValue(buf, -32767, 32767, -0.075, 0.075); 
              //std::cout << "offset corner lon 3 == " << value << std::endl;
              //            std::cout << "SENSOR SLANT ANGLE!!" << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_3,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              break;
           }
           case 32:
           {
              // Map -(2^15-1)..(2^15-1) to +/-0.075.
              ossim_int16 buf = *reinterpret_cast<const ossim_int16*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_float64 value  = mapValue(buf, -32767, 32767, -0.075, 0.075); 
              //std::cout << "offset corner lat 4 == " << value << std::endl;
              //            std::cout << "SENSOR SLANT ANGLE!!" << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_4,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              break;
           }
           case 33:
           {
              // Map -(2^15-1)..(2^15-1) to +/-0.075.
              ossim_int16 buf = *reinterpret_cast<const ossim_int16*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_float64 value  = mapValue(buf, -32767, 32767, -0.075, 0.075); 
              //            std::cout << "SENSOR SLANT ANGLE!!" << std::endl;
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_4,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              break;
           }
           case 34:
           {
              if(traceDebug()) std::cout << "ICING DETECTED Not Handled!!" << std::endl;
              break;
           }
           case 35:
           {
              if(traceDebug()) std::cout << "WIND DIRECTION Not Handled!!" << std::endl;
              break;
           }
           case 36:
           {
              if(traceDebug())  std::cout << "WIND SPEED not handled!!" << std::endl;
              break;
           }
           case 37://KLV_KEY_STATIC_PRESSURE
           {
              // Map 0..(2^16-1) to 0..5000 mbar.
              ossim_uint16 buf = *reinterpret_cast<const ossim_uint16*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_uint16 value  = mapValue(buf, 0, 65535, 0, 5000); 
              
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_STATIC_PRESSURE,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+2));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));


           //   if(traceDebug()) std::cout << "Static Pressure not handled!!" << std::endl;
              break;
           }
           case 38:
           {
              if(traceDebug()) std::cout << "DENSITY ALTITUDE not handled!!" << std::endl;
              break;
           }
           case 39:
           {
              if(traceDebug()) std::cout << "OUTSIDE AIR TEMP not handled!!" << std::endl;
              break;
           }
           case 40:
           {
              if(traceDebug()) std::cout << "Target Location Latitude not handled!!" << std::endl;
              break;
           }
           case 41:
           {
              if(traceDebug()) std::cout << "Target Location Longitude not handled!!" << std::endl;
              break;
           }
           case 42:
           {
              if(traceDebug()) std::cout << "Target Location Elevation not handled!!" << std::endl;
              break;
           }
           case 43:
           {
              if(traceDebug()) std::cout << "Target track gate width not handled!!" << std::endl;
              break;
           }
           case 44:
           {
              if(traceDebug()) std::cout << "Target track gate height not handled!!" << std::endl;
              break;
           }
           case 45:
           {
              if(traceDebug()) std::cout << "Target Error esitimate CE90 not handled!!" << std::endl;
              break;
           }
           case 46:
           {
              if(traceDebug())
              {
                 std::cout << "Target Error esitimate LE90!!" << std::endl;
              }
              break;
           }
           case 47:
           {
              Node n(KLV_KEY_GENERIC_FLAG_DATA_01,
                     std::vector<ossim_uint8>(buffer.begin()+idx,
                                              buffer.begin()+idx+length));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));

              //std::cout << "Generic Flag Data 01!!" << (int)((buffer.front()+idx)) << std::endl;
              break;
           }
           case 48:
           {

              //if(traceDebug())
              //{
                 std::vector<ossim_uint8> tempBuf(buffer.begin()+idx, buffer.begin()+idx+length);
                 addSecurityMetadataLocalSetElements(tempBuf, tempTable);
               //  std::cout << "Security Local Dataset Not Handled!! ADDED KEYS?: "  << std::endl;
              //}
              break;
           }
           case 49:
           {
              if(traceDebug())
              {
                 std::cout << "Differential pressure!!" << std::endl;
              }
              break;
           }
           case 50:
           {
              if(traceDebug())
              {
                 std::cout << "Platform angle of attack!!" << std::endl;
              }
              break;
           }
           case 51:
           {
              if(traceDebug())
              {
                 std::cout << "Platform vertical speed!!" << std::endl;
              }
              break;
           }
           case 52:
           {
              if(traceDebug())
              {
                 std::cout << "Platform side slip angle!!" << std::endl;
              }
              break;
           }
           case 53:
           {
              if(traceDebug())
              {
                 std::cout << "airfilled barometrc pressure!!" << std::endl;
              }
              // airfiled barometrc pressure
              break;
           }
           case 54:
           {
              if(traceDebug())
              {
                 std::cout << "airfiled elevation!! Not handled" << std::endl;
              }
              // airfiled elevation
              break;
           }
           case 55:
           {
              if(traceDebug())
              {
                 std::cout << "relative humidity!! Not handled" << std::endl;
              }
              // relative humidity
              break;
           }
           case 56://KLV_KEY_PLATFORM_GROUND_SPEED
           {
              Node n(KLV_KEY_PLATFORM_GROUND_SPEED,
                     std::vector<ossim_uint8>(buffer.begin()+idx,
                                              buffer.begin()+idx+length));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              // platform ground speed
              break;
           }
           case 57:
           {
              if(traceDebug())
              {
                 std::cout << "ground range!! Not handled" << std::endl;
              }
              // ground range
              break;
           }
           case 58:
           {
              if(traceDebug())
              {
                 std::cout << " platform fuel remaining! Not handled" << std::endl;
              }
              // platform fuel remaining
              break;
           }
           case 59:
           {
              if(traceDebug())
              {
                 std::cout << "platform call sign! Not handled" << std::endl;
              }
             // platform call sign
              break;
           }
           case 60:
           {
              if(traceDebug())
              {
                 std::cout << "weapon load! Not handled" << std::endl;
              }
             // weapon load
              break;
           }
           case 61:
           {
              if(traceDebug())
              {
                  std::cout << "weapon fired Not Handled!" << std::endl;
              }
              // weapon fired
              break;
           }
           case 62:
           {
              if(traceDebug())
              {
                 std::cout << "Laser PRF Code Not handled" << std::endl;
              }
              // Laser PRF Code
              break;
           }
           case 63:
           {
              if(traceDebug())
              {
                 std::cout << "Sensor Field of View Name Not handled" << std::endl;
              }
             //Sensor Field of View Name
              break;
           }
           case 64:// KLV_KEY_PLATFORM_MAGNETIC_HEADING_ANGLE
           {
              // Map 0..(2^16-1) to 0..360.
              ossim_uint16 buf = *reinterpret_cast<const ossim_uint16*>(&buffer.front()+idx);
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(buf);
              }
              ossim_float64 value  = mapValue(buf, 0, 65535, 0, 360); 
              if(theNeedToSwapFlag)
              {
                 theEndian.swap(value);
              }
              ossim_uint8* tempBuf = reinterpret_cast<ossim_uint8*>(&value);
              Node n(KLV_KEY_PLATFORM_MAGNETIC_HEADING_ANGLE,
                     std::vector<ossim_uint8>(tempBuf,
                                              tempBuf+8));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
             //            std::cout << "SENSOR REALTIVE AZIMUTH ANGLE!!" << std::endl;
             // platform magnetic heading
              break;
           }
           case 65:// KLV_KEY_UAS_LDS_VERSION_NUMBER
           {
              Node n(KLV_KEY_UAS_LDS_VERSION_NUMBER,
                     std::vector<ossim_uint8>(buffer.begin()+idx,
                                              buffer.begin()+idx+length));
              tempTable.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(n.theId),
                                                     n));
              //std::cout << "KLV_KEY_UAS_LDS_VERSION_NUMBER: " << valueAsString(KLV_KEY_UAS_LDS_VERSION_NUMBER) <<std::endl;
              // UAS LDS Version Number
              break;
           }
           case 66:
           {
              if(traceDebug()) std::cout << "target location covariance matrix  Not handled" << std::endl;
             // target locatio covariance matrix
              break;
           }
           case 67:
           {
              // alternate platform latitude
              if(traceDebug()) std::cout << " alternate platform latitude No Handled!"  << std::endl;  
              break;
           }
           case 68:
           {
                 if(traceDebug()) std::cout << "alternate platform longitude Not Handled! "  <<key  << std::endl;  
             // alternate platform longitude
              break;
           }
           case 69:
           {
              // alternate platform altitude
                if(traceDebug())  std::cout << "alternate platform altitude Not handled "  <<key  << std::endl;  
             break;
           }
           case 70:
           {
              // alternate platform name
               if(traceDebug()) std::cout << "alternate platform name Not handled "  <<key  << std::endl;  
             break;
           }
           case 71:
           {
              // alternate platform heading
              if(traceDebug()) std::cout << "alternate platform heading Not handled "  <<key  << std::endl;  
              break;
           }
           case 72:
           {
              if(traceDebug()) std::cout << "EVENT START TIME  Not handled "  <<key  << std::endl;  
  //            std::cout << "EVENT START TIME!!" << std::endl;
              break;
           }
           default:
           {
              if(traceDebug() ) std::cout << "KEY NOT HANDLED "  <<key  << std::endl;  
              break;
           }
        }
      }
      idx +=length;
   }
   /*
   ossimDpt pt1,pt2,pt3,pt4;
   if(getFrameCenterOffsets(pt1,pt2,pt3,pt4))
   {
      ossim_float64 lat,lon,elevation;
      if(getFrameCenter(lat, lon, elevation))
      {
         ossim_float64 cornerLat[4];
         ossim_float64 cornerLon[4];

         cornerLat[0] = lat + pt1.lat;
         cornerLat[1] = lat + pt2.lat;
         cornerLat[2] = lat + pt3.lat;
         cornerLat[3] = lat + pt4.lat;
         cornerLon[0] = lon + pt1.lon;
         cornerLon[1] = lon + pt2.lon;
         cornerLon[2] = lon + pt3.lon;
         cornerLon[3] = lon + pt4.lon;


         if(theNeedToSwapFlag)
         {
            theEndian.swap(cornerLat[0]);
            theEndian.swap(cornerLat[1]);
            theEndian.swap(cornerLat[2]);
            theEndian.swap(cornerLat[3]);
            theEndian.swap(cornerLon[0]);
            theEndian.swap(cornerLon[1]);
            theEndian.swap(cornerLon[2]);
            theEndian.swap(cornerLon[3]);
         }

         ossim_uint8* cornerLat1Buf = reinterpret_cast<ossim_uint8*>(&cornerLat[0]);
         Node cornerLat1Node(KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_1,
                std::vector<ossim_uint8>(cornerLat1Buf,
                                         cornerLat1Buf+8));
         theKlvParameters.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(cornerLat1Node.theId),
                                                cornerLat1Node));
         ossim_uint8* cornerLat2Buf = reinterpret_cast<ossim_uint8*>(&cornerLat[1]);
         Node cornerLat2Node(KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_2,
                std::vector<ossim_uint8>(cornerLat2Buf,
                                         cornerLat2Buf+8));
         theKlvParameters.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(cornerLat2Node.theId),
                                                cornerLat2Node));
         ossim_uint8* cornerLat3Buf = reinterpret_cast<ossim_uint8*>(&cornerLat[2]);
         Node cornerLat3Node(KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_3,
                std::vector<ossim_uint8>(cornerLat3Buf,
                                         cornerLat3Buf+8));
         theKlvParameters.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(cornerLat3Node.theId),
                                                cornerLat3Node));
         ossim_uint8* cornerLat4Buf = reinterpret_cast<ossim_uint8*>(&cornerLat[3]);
         Node cornerLat4Node(KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_4,
                std::vector<ossim_uint8>(cornerLat4Buf,
                                         cornerLat4Buf+8));
         theKlvParameters.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(cornerLat4Node.theId),
                                                cornerLat4Node));


         ossim_uint8* cornerLon1Buf = reinterpret_cast<ossim_uint8*>(&cornerLon[0]);
         Node cornerLon1Node(KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_1,
                std::vector<ossim_uint8>(cornerLon1Buf,
                                         cornerLon1Buf+8));
         theKlvParameters.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(cornerLon1Node.theId),
                                                cornerLon1Node));
         ossim_uint8* cornerLon2Buf = reinterpret_cast<ossim_uint8*>(&cornerLon[1]);
         Node cornerLon2Node(KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_2,
                std::vector<ossim_uint8>(cornerLon2Buf,
                                         cornerLon2Buf+8));
         theKlvParameters.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(cornerLon2Node.theId),
                                                cornerLon2Node));
         ossim_uint8* cornerLon3Buf = reinterpret_cast<ossim_uint8*>(&cornerLon[2]);
         Node cornerLon3Node(KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_3,
                std::vector<ossim_uint8>(cornerLon3Buf,
                                         cornerLon3Buf+8));
         theKlvParameters.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(cornerLon3Node.theId),
                                                cornerLon3Node));
         ossim_uint8* cornerLon4Buf = reinterpret_cast<ossim_uint8*>(&cornerLon[3]);
         Node cornerLon4Node(KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_4,
                std::vector<ossim_uint8>(cornerLon4Buf,
                                         cornerLon4Buf+8));
         theKlvParameters.insert(std::make_pair(static_cast<ossimPredatorKlvIndex>(cornerLon4Node.theId),
                                                cornerLon4Node));
       
  // std::cout << "corner1 = " << valueAsString(KLV_KEY_OFFSET_CORNER_LATITUDE_POINT_1) << ", " 
   //          << valueAsString(KLV_KEY_OFFSET_CORNER_LONGITUDE_POINT_1) << std::endl;
      }
   }
   */
}

std::ostream& ossimPredatorKlvTable::print(std::ostream& out)const
{
  klvMapType::const_iterator iter = theKlvParameters.begin();
  while(iter!=theKlvParameters.end())
  {
    int idx = findPredatorKlvIndexByKey((*iter).second.theId);
    if(idx >= 0)
    {
      out << (*iter).second.theId << "," << (*iter).first<< ", " << OSSIM_PREDATOR_UDS_TABLE[idx].theName << ": " << valueAsString((*iter).first) << "\n";
    }
    ++iter;
  }

  /*
   klvMapType::const_iterator iter = theKlvParameters.begin();
   while(iter!=theKlvParameters.end())
   {
      out << (*iter).second.theId << "," << (*iter).first<< ", " << OSSIM_PREDATOR_UDS_TABLE[(*iter).second.theId].theName << ": " << valueAsString((*iter).first) << "\n";
      ++iter;
   }
   */
   return out;
}

ossim_uint16 ossimPredatorKlvTable::compute16BitChecksum(const std::vector<ossim_uint8>& checksumBuffer)const
{

  const ossim_uint8* buf= &checksumBuffer.front();
  ossim_uint16 len = checksumBuffer.size();
 // Initialize Checksum and counter variables.
  ossim_uint16 bcc = 0, i=0;

  // Sum each 16-bit chunk within the buffer into a checksum
  for ( i = 0 ; i < len; ++i)
    bcc += buf[i] << (8 * ((i + 1) % 2));
  return bcc;
}