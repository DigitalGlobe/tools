//----------------------------------------------------------------------------
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description:  Class definition for HDF reader.
//
//----------------------------------------------------------------------------
// $Id: ossimHdf4SubDataset.cpp 2645 2011-05-26 15:21:34Z oscar.kramer $

//Std includes
#include <set>

//ossim includes
#include "ossimHdf4SubDataset.h"
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include <ossim/imaging/ossimImageGeometryRegistry.h>
#include <ossim/imaging/ossimImageDataFactory.h>
//#include <ossim/base/ossimDrect.h>

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id";
#endif

static ossimTrace traceDebug("ossimHdf4SubDataset:debug");

RTTI_DEF_INST(ossimHdf4SubDataset, "ossimHdf4SubDataset")

bool doubleEquals(ossim_float64 left, ossim_float64 right, ossim_float64 epsilon) 
{
  return (fabs(left - right) < epsilon);
}

ossimHdf4SubDataset::ossimHdf4SubDataset() :
   m_fileName(""), 
   m_sd_name(""),
   m_sds_index(-1),
   m_sds_id(-1),
   m_sd_id(-1),
   m_rank(0),
   m_numberOfBands(0),
   m_numberOfSamples(0),
   m_numberOfLines(0),
   m_attributes(0),
   m_isGeoField(false),
   m_scalarType(OSSIM_SCALAR_UNKNOWN),
   m_dataType(0),
   m_hdfDataType(),
   m_meta(0)
{
}

ossimHdf4SubDataset::~ossimHdf4SubDataset()
{
   SDendaccess (m_sds_id); 
   SDend (m_sd_id); 
   m_meta.clear();
}

ossim_uint32 ossimHdf4SubDataset::getNumberOfLines(ossim_uint32 /* resLevel */) const
{
   return m_numberOfLines;
}

ossim_uint32 ossimHdf4SubDataset::getNumberOfSamples(ossim_uint32 /* resLevel */) const
{
   return m_numberOfSamples;
}

bool ossimHdf4SubDataset::open()
{
   static const char MODULE[] = "ossimHdf4SubDataset::open";

   bool result = false;
 
   m_sd_id = SDstart(m_fileName.c_str(), DFACC_READ);
   if (m_sd_id < 0)
   {
      result = false;
   }
   else
   {
      result = true;
   }

   m_sds_id = SDselect(m_sd_id, m_sds_index);
   if (m_sds_id < 0)
   {
      result = false;
   }
   else
   {
      result = true;
   }

   if (result == false)
   {
      SDendaccess (m_sds_id); 
      SDend (m_sd_id); 
   }
   
   initMeta();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }

   return result;
}

void ossimHdf4SubDataset::initMeta()
{
   m_meta.clear();

   //insert name first
   m_meta.push_back("name: " + ossimString(m_hdfDataType + ":" + m_sd_name));

   char	attrName[100];
   int32 numValues;
   int32 dataType;

   for (ossim_int32 i = 0; i < m_attributes; i++ )
   {
      // Get information about the attribute. Note that the first
      // parameter is an SD interface identifier.
      SDattrinfo(m_sds_id, i, attrName, &dataType, &numValues);
      
      ossim_int32 size = getDataTypeSize(dataType);
      char* attrValue = new char[size * numValues + 1];
      
      SDreadattr(m_sds_id, i, attrValue);

      if ( dataType == DFNT_CHAR8 || dataType == DFNT_UCHAR8 )
      {
         attrValue[numValues] = '\0';
         m_meta.push_back(ossimString(attrName) + ": " + ossimString(attrValue).trim());
      }
      else
      {
         ossimString values = getAttribuitValues(dataType, numValues, attrValue);
         m_meta.push_back(ossimString(attrName) + ": " + values);
      }
      delete [] attrValue;
   }
}

ossimString ossimHdf4SubDataset::getAttribuitValues(int32 dataType, 
                                                int32 numValues,
                                                void* attrValue)
{
   ossimString values;

   if (dataType == DFNT_INT8)
   {
      int8* int8Data = (int8*)attrValue;
      for (ossim_int32 j = 0; j < numValues; j++)
      {
         ossimString valueStr = ossimString::toString(int8Data[j]);
         values = values + ossimString(valueStr + " ");
      }
   }
   else if (dataType == DFNT_UINT8)
   {
      uint8* uint8Data = (uint8*)attrValue;
      for (ossim_int32 j = 0; j < numValues; j++)
      {
         ossimString valueStr = ossimString::toString(uint8Data[j]);
         values = values + ossimString(valueStr + " ");
      }
   }
   else if (dataType == DFNT_INT16)
   {
      int16* int16Data = (int16*)attrValue;
      for (ossim_int32 j = 0; j < numValues; j++)
      {
         ossimString valueStr = ossimString::toString(int16Data[j]);
         values = values + ossimString(valueStr + " ");
      }
   }
   else if (dataType == DFNT_UINT16)
   {
      uint16* uint16Data = (uint16*)attrValue;
      for (ossim_int32 j = 0; j < numValues; j++)
      {
         ossimString valueStr = ossimString::toString(uint16Data[j]);
         values = values + ossimString(valueStr + " ");
      }
   }
   else if (dataType == DFNT_INT32 || dataType == DFNT_UINT32)
   {
      ossim_int32* int32Data = (ossim_int32*)attrValue;
      for (ossim_int32 j = 0; j < numValues; j++)
      {
         ossimString valueStr = ossimString::toString(int32Data[j]);
         values = values + ossimString(valueStr + " ");
      }
   }
   else if (dataType == DFNT_FLOAT64 || dataType == DFNT_DOUBLE)
   {
      double* doubleData = (double*)attrValue;
      for (ossim_int32 j = 0; j < numValues; j++)
      {
         ossimString valueStr = ossimString::toString(doubleData[j]);
         values = values + ossimString(valueStr + " ");
      }
   }
   else
   {
      float32* floatData = (float32*)attrValue;
      for (ossim_int32 j = 0; j < numValues; j++)
      {
         ossimString valueStr = ossimString::toString(floatData[j]);
         values = values + ossimString(valueStr + " ");
      }
   }
   return values;
}

char* ossimHdf4SubDataset::getTileBuf(const  ossimIrect& rect, ossim_uint32 band)
{
   // bool status = false;
   int32 start[H4_MAX_NC_DIMS], edges[H4_MAX_NC_DIMS];
   if (m_rank == 3)
   {
      start[0] = band;
      start[1] = rect.ul().y;
      start[2] = rect.ul().x;
      edges[0] = 1;
      edges[1] = rect.height(); 
      edges[2] = rect.width(); 
   }
   else if (m_rank == 2)
   {
      start[0] = rect.ul().y;
      start[1] = rect.ul().x;
      edges[0] = rect.height(); 
      edges[1] = rect.width(); 
   }
   
   int32 numValues = rect.width() * rect.height();
   ossim_int32 size = getDataTypeSize(m_dataType);
   
   char* data = new char[size * numValues];
   // intn statusSd = SDreaddata(m_sds_id, start, NULL, edges, (VOIDP)data);
   SDreaddata(m_sds_id, start, NULL, edges, (VOIDP)data);   
  
   return data;
}

ossim_uint32 ossimHdf4SubDataset::getNumberOfInputBands() const
{
   return m_numberOfBands;
}

ossimScalarType ossimHdf4SubDataset::getOutputScalarType() const
{
   return m_scalarType;
}

void ossimHdf4SubDataset::setNumberOfLines(int32 lines)
{
   m_numberOfLines = lines;
}

void ossimHdf4SubDataset::setNumberOfSamples(int32 samples)
{
   m_numberOfSamples = samples;
}

void ossimHdf4SubDataset::setNumberOfBands(int32 bands)
{
   m_numberOfBands = bands;
}

void ossimHdf4SubDataset::setSdsName(ossimString name)
{
   m_sd_name = name;
}

void ossimHdf4SubDataset::setHdfSdsIndex(int32 sds_index)
{
   m_sds_index = sds_index;
}

void ossimHdf4SubDataset::setAttributeCount(int32 attributes)
{
   m_attributes = attributes;
}

void ossimHdf4SubDataset::setSdsRank(int32 rank)
{
   m_rank = rank;
}

void ossimHdf4SubDataset::setHdfFilename(ossimString fileName)
{
   m_fileName = fileName;
   open();
}

void ossimHdf4SubDataset::setOutputScalarType(int32 dataType)
{
   m_dataType = dataType;
   switch (dataType)
   {
   case DFNT_UINT8:
      {
         m_scalarType = OSSIM_UINT8;
         break;
      }
   case DFNT_INT8:
      {
         m_scalarType = OSSIM_SINT8;
         break;
      }
   case DFNT_UINT16:
      {
         m_scalarType = OSSIM_UINT16;
         break;
      }
   case DFNT_INT16:
      {
         m_scalarType = OSSIM_SINT16;
         break;
      }
   case DFNT_UINT32:
      {
         m_scalarType = OSSIM_UINT32;
         break;
      }
   case DFNT_INT32:
      {
         m_scalarType = OSSIM_SINT32;
         break;
      }
   case DFNT_FLOAT32:
      {
         m_scalarType = OSSIM_FLOAT32;
         break;
      }
   case DFNT_FLOAT64:
      {
         m_scalarType = OSSIM_FLOAT64;
         break;
      }
   default:
      {
         m_scalarType = OSSIM_SCALAR_UNKNOWN;
         break;
      }
   }
}

ossim_int32 ossimHdf4SubDataset::getDataTypeSize(int32 type)
{
   ossim_int32 size = 0;
   if ( type == DFNT_CHAR8 || type == DFNT_UCHAR8 )
   {
      size = 1;
   }
   else if (type == DFNT_INT8 || type == DFNT_UINT8)
   {
      size = 1;
   }
   else if (type == DFNT_INT16 || type == DFNT_UINT16)
   {
      size = 2;
   }
   else if (type == DFNT_INT32 || type == DFNT_UINT32 
      || type == DFNT_FLOAT32)
   {
      size = 4;
   }
   else if (type == DFNT_DOUBLE || type == DFNT_FLOAT64)
   {
      size = 8;
   }
   else
   {
      size = 4;
   }
   return size;
}

