//----------------------------------------------------------------------------
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description:  Class definition for HDF4 reader.
//
//----------------------------------------------------------------------------
// $Id: ossimHdf4Reader.cpp 2645 2011-05-26 15:21:34Z oscar.kramer $

#include "ossimHdf4Reader.h"

#include <fstream>
#include <iostream>
#include <string>

//ossim includes
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimIoStream.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimUnitTypeLut.h>

#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimImageGeometryRegistry.h>

#include <ossim/projection/ossimProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimEpsgProjectionFactory.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimLambertConformalConicProjection.h>
#include <ossim/projection/ossimMercatorProjection.h>
#include <ossim/projection/ossimPolarStereoProjection.h>
#include <ossim/projection/ossimPolyconicProjection.h>
#include <ossim/projection/ossimAzimEquDistProjection.h>
#include <ossim/projection/ossimObliqueMercatorProjection.h>
#include <ossim/projection/ossimSpaceObliqueMercatorProjection.h>
#include <ossim/projection/ossimSinusoidalProjection.h>

#include <ossim/support_data/ossimAuxFileHandler.h>


/////////////////////////////////////////
#include <ossim/base/ossimTieGptSet.h>
//////////////////////////////////////////

#include "ossimHdf4SubDataset.h"
#include "ossimHdfGridModel.h"

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id";
#endif

const ossimString ossimHdf4Reader::HDF4_EOS_GRID = "GRID";              
const ossimString ossimHdf4Reader::HDF4_EOS_SWATH = "SWATH";                   
const ossimString ossimHdf4Reader::HDF4_SEAWIFS_L1A = "SEAWIFS_L1A";		    
const ossimString ossimHdf4Reader::HDF4_SEAWIFS_L2 = "SEAWIFS_L2";	          
const ossimString ossimHdf4Reader::HDF4_SEAWIFS_L3 = "SEAWIFS_L3";		       
const ossimString ossimHdf4Reader::HDF4_HYPERION_L1 = "HYPERION_L1"; 
const ossimString ossimHdf4Reader::HDF4_SDS = "HDF4_SDS";

static ossimTrace traceDebug("ossimHdf4Reader:debug");
static ossimTrace traceDump("ossimHdf4Reader:dump");

RTTI_DEF1_INST(ossimHdf4Reader,
               "ossimHdf4Reader",
               ossimImageHandler)

               ossimHdf4Reader::ossimHdf4Reader()
               : ossimImageHandler(),
               m_fid(-1),
               m_sd_id(-1),
               m_imageRect(),
               m_numberOfBands(0),
               m_scalarType(OSSIM_SCALAR_UNKNOWN),
               m_tile(0),
               m_isHdfEos(false),
               m_currentEntryRender(0),
               m_currentSubDataset(NULL),
               m_structMetadata("")
{
}

ossimHdf4Reader::~ossimHdf4Reader()
{
   closeEntry();
}

ossimString ossimHdf4Reader::getShortName()const
{
   return ossimString("ossim_hdf4_reader");
}

ossimString ossimHdf4Reader::getLongName()const
{
   return ossimString("ossim hdf4 reader");
}

ossimString ossimHdf4Reader::getClassName()const
{
   return ossimString("ossimHdf4Reader");
}

ossim_uint32 ossimHdf4Reader::getNumberOfLines(ossim_uint32 resLevel) const
{
   ossim_uint32 result = 0;
   if ( isValidRLevel(resLevel) )
   {
      if (resLevel == 0)
      {
         if (m_currentSubDataset != NULL)
         {
            return m_currentSubDataset->getNumberOfLines();
         }
      }
      else if (theOverview.valid())
      {
         result = theOverview->getNumberOfSamples(resLevel);
      }
   }
   return result;
}

ossim_uint32 ossimHdf4Reader::getNumberOfSamples(ossim_uint32 resLevel) const
{
   ossim_uint32 result = 0;
   if ( isValidRLevel(resLevel) )
   {
      if (resLevel == 0)
      {
         if (m_currentSubDataset != NULL)
         {
            return m_currentSubDataset->getNumberOfSamples();
         }
      }
      else if (theOverview.valid())
      {
         result = theOverview->getNumberOfSamples(resLevel);
      }
   }
   return result;
}

bool ossimHdf4Reader::open()
{
   static const char MODULE[] = "ossimHdf4Reader::open";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered...\n"
         << "image: " << theImageFile << "\n";
   }

   bool result = false;

   if (Hishdf(theImageFile.c_str()) <= 0)
   {
      return false;
   }

   if (m_subDatasets.size() == 0)
   {
      if(isOpen())
      {
         closeEntry();
      }

      if (m_subDatasets.size() == 0)
      {
         m_fid = Hopen(theImageFile.c_str(), DFACC_READ, 0);
         if (m_fid <= 0)
         {
            closeEntry();
            return false;  
         }

         m_sd_id = SDstart(theImageFile.c_str(), DFACC_READ );

         if (m_sd_id == -1)
         {
            closeEntry();
            return false;  
         }

         result = initGlobalMeta();      
      }
   }

   if (m_currentEntryRender < m_subDatasets.size())
   {
      //close file before open file in sub-dataset
      closeEntry();
      ossimHdfSubdataset subData = m_subDatasets[m_currentEntryRender];
      m_numberOfBands = subData.m_bands;
      m_currentSubDataset = new ossimHdf4SubDataset();
      m_currentSubDataset->setHdfSdsIndex(subData.m_sdsIndex);
      m_currentSubDataset->setOutputScalarType(subData.m_dataType);
      m_currentSubDataset->setNumberOfBands(subData.m_bands);
      m_currentSubDataset->setNumberOfLines(subData.m_rows);
      m_currentSubDataset->setNumberOfSamples(subData.m_cols);
      m_currentSubDataset->setAttributeCount(subData.m_attributes);
      m_currentSubDataset->setSdsRank(subData.m_rank);
      m_currentSubDataset->setSdsName(subData.m_name);
      m_currentSubDataset->setHdfDataType(m_hdfDataType);

      m_currentSubDataset->setHdfFilename(theImageFile);

      m_tile = ossimImageDataFactory::instance()->create(this, this);
      m_tile->initialize();

      // Call the base complete open to pick up overviews.
      completeOpen();
      result = true;
   }
   else
   {
      closeEntry();
      result = false;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }

   return result;
}

bool ossimHdf4Reader::initGlobalMeta()
{
   int32	numAttributes;
   int32 numValues;
   int32 numDatasets;
   int32 dataType;
   char	attrName[100];

   m_globalMeta.clear();
   m_structMetadata = "";

   //Obtain number of SDSs and global attributes in input file. 
   if ( SDfileinfo(m_sd_id, &numDatasets, &numAttributes ) != 0 )
   {
      return false;
   }

   // Loop through the all attributes
   for (ossim_int32 i = 0; i < numAttributes; i++ )
   {
      // Get information about the attribute. Note that the first
      // parameter is an SD interface identifier.
      SDattrinfo(m_sd_id, i, attrName, &dataType, &numValues);

      if (i == 0)
      {
         char* attrVersion = (char*)malloc(numValues * sizeof (dataType));
         attrVersion[numValues] = '\0';
         SDreadattr(m_sd_id, i, attrVersion);
         m_globalMeta.push_back(ossimString(ossimString(attrName).trim() 
            + ": " + ossimString(attrVersion).trim()));
         free(attrVersion);
         continue;
      }

      if ( ossimString(attrName).downcase().contains("coremetadata.")     ||
         ossimString(attrName).downcase().contains("archivemetadata.")    ||
         ossimString(attrName).downcase().contains("productmetadata.")    ||
         ossimString(attrName).downcase().contains("badpixelinformation") ||
         ossimString(attrName).downcase().contains("product_summary")     ||
         ossimString(attrName).downcase().contains("dem_specific")        ||
         ossimString(attrName).downcase().contains("bts_specific")        ||
         ossimString(attrName).downcase().contains("etse_specific")       ||
         ossimString(attrName).downcase().contains("dst_specific")        ||
         ossimString(attrName).downcase().contains( "acv_specific")       ||
         ossimString(attrName).downcase().contains("act_specific")        ||
         ossimString(attrName).downcase().contains("etst_specific")       ||
         ossimString(attrName).downcase().contains("level_1_carryover"))
      {
         char* attrValues = (char*)malloc(numValues * sizeof (dataType));
         attrValues[numValues] = '\0';
         SDreadattr(m_sd_id, i, attrValues);
         parseMetaString(ossimString(attrValues));
         free(attrValues);
      }
      else if (ossimString(attrName).downcase().contains("structmetadata."))
      {
         m_isHdfEos = true;
         char* attrStructureChar = (char*)malloc(numValues * sizeof (dataType));
         attrStructureChar[numValues] = '\0';
         SDreadattr(m_sd_id, i, attrStructureChar);
         m_structMetadata = m_structMetadata + ossimString(attrStructureChar);
         free(attrStructureChar);
         continue;
      }
      else
      {
         if ( dataType == DFNT_CHAR8 || dataType == DFNT_UCHAR8 )
         {
            char* attrValue = (char*)malloc(numValues * sizeof (dataType));
            attrValue[numValues] = '\0';
            SDreadattr(m_sd_id, i, attrValue);
            m_globalMeta.push_back(ossimString(attrName) + ": " + ossimString(attrValue));
            free(attrValue);
         }
         else
         {
            ossim_int32 size = getSizeByType(dataType);
            char* attrValue = new char[size * numValues];
            SDreadattr(m_sd_id, i, (VOIDP)attrValue);
            ossimString values = getAttribuitValues(dataType, numValues, attrValue);
            m_globalMeta.push_back(ossimString(attrName) + ": " + values);
            delete[] attrValue;
         }   
      }
   }

   initHdfDataType();
   initSubdatasetNames();
   initGroupInfos();

   return initSubDatasets(numDatasets);
}

ossim_int32 ossimHdf4Reader::getSizeByType(int32 dataType) const
{
   if ( dataType == DFNT_CHAR8 || dataType == DFNT_UCHAR8 )
   {
      return 1;
   }
   else if (dataType == DFNT_INT8 || dataType == DFNT_UINT8)
   {
      return 1;
   }
   else if (dataType == DFNT_INT16 || dataType == DFNT_UINT16)
   {
      return 2;
   }
   else if (dataType == DFNT_INT32 || dataType == DFNT_UINT32 
      || dataType == DFNT_FLOAT32)
   {
      return 4;
   }
   else if (dataType == DFNT_DOUBLE || dataType == DFNT_FLOAT64)
   {
      return 8;
   }

   return 4;
}

void ossimHdf4Reader::initSubdatasetNames()
{
   m_subDatasetNames.clear();
   std::vector<ossimString> attrList = m_structMetadata.split("\n");
   std::vector<ossimString> attrInfos;
   ossimString groupIdentifier;
   ossimString endGroupIdentifier;
   ossimString groupName;

   for (ossim_uint32 i  = 0; i < attrList.size(); i++)
   {
      ossimString attrInfo = attrList[i].trim();
      if (attrInfo.contains("GROUP=SwathStructure") && m_subDatasetNames.size() == 0)//initialize the type and group name
      {
         m_hdfDataType = HDF4_EOS_SWATH;
         groupIdentifier = "GROUP=SWATH_";
         groupName = "SwathName";
      }
      else if (attrInfo.contains("GROUP=GridStructure") && m_subDatasetNames.size() == 0)//initialize the type and group name
      {
         m_hdfDataType = HDF4_EOS_GRID;
         groupIdentifier = "GROUP=GRID_";
         groupName = "GridName";
      }
      else //get sub dataset infos
      {
         if (attrInfo.contains(groupIdentifier) && attrInfo.contains("END_") == false)
         {
            endGroupIdentifier = "END_" + attrInfo;
         }
         if (attrInfo != endGroupIdentifier)
         {
            attrInfos.push_back(attrInfo);
         } 
         else
         {
            parseGroupName(attrInfos, groupName);
            attrInfos.clear();
         }
      }
   }
}

void ossimHdf4Reader::parseGroupName(std::vector<ossimString> groupInfo,
                                     ossimString groupName)
{
   for (ossim_uint32 i = 0; i < groupInfo.size(); i++)
   {
      ossimString info = groupInfo[i];
      if (info.contains(groupName))
      {
         groupName = info.split("=")[1].trim();
         groupName = groupName.substitute("\"", "", true);
      }
      if (info.contains("GeoFieldName") || info.contains("DataFieldName"))
      {
         ossimString fieldName = info.split("=")[1].trim();
         fieldName = fieldName.substitute("\"", "", true);
         m_subDatasetNames.insert(ossimString(groupName + ":" + fieldName));
      }
   }
}

void ossimHdf4Reader::initGroupInfos()
{
   m_groupInfos.clear();

   char vgName[1024];	/* Object name */
   char vgClass[1024];	/* Object class */
   int32 vgRef = -1;
   int32 vgroupId;

   Vstart(m_fid);
   while (1)
   {
      // Get Vgroup reference number 
      vgRef = Vgetid(m_fid, vgRef);

      // If no more then exist search loop
      if (vgRef == -1)
      {
         break;
      }

      // Get Vgroup ID, name, and class 
      vgroupId = Vattach(m_fid, vgRef, "r");
      Vgetname(vgroupId, vgName);
      Vgetclass(vgroupId, vgClass);

      //get "Fields" and "Attributes" info (SDS ids) from Vgroups
      if (m_isHdfEos)
      {
         if (ossimString(vgClass).upcase() == m_hdfDataType)
         {
            int32 numObjects = Vntagrefs(vgroupId); //Get # of entries within this Vgroup and search for SDS

            int32* tags = (int32 *) malloc(sizeof(int32) * numObjects);
            int32* refs = (int32 *) malloc(sizeof(int32) * numObjects);

            Vgettagrefs(vgroupId, tags, refs, numObjects); 

            for (ossim_int32 i = 0; i < numObjects; i++) // Get tag and ref # for each Vgroup objects
            {
               int32 vgRefId = Vattach(m_fid, refs[i], "r");

               int32 numRefObjects = Vntagrefs(vgRefId);
               int32* tagsObj = (int32 *) malloc(sizeof(int32) * numRefObjects);
               int32* refsObj = (int32 *) malloc(sizeof(int32) * numRefObjects);

               Vgettagrefs(vgRefId, tagsObj, refsObj, numRefObjects);

               for (ossim_int32 j = 0; j < numRefObjects; j++)
               {
                  if (tagsObj[j] == DFTAG_NDG)
                  {
                     int32 index = SDreftoindex(m_sd_id, refsObj[j]);
                     int32 sdid = SDselect(m_sd_id, index);
                     m_groupInfos[sdid] = ossimString(vgName);
                  }
               }
               free(tagsObj);
               free(refsObj);
            }
            free(tags);
            free(refs);
         }
      }
      // Detach Vgroup 
      Vdetach(vgroupId);
   }
   Vend(m_fid);  
}

bool ossimHdf4Reader::initSubDatasets(int32 numDatasets)
{
   m_subDatasets.clear();
   m_groupDatasets.clear();

   int32 dim_sizes[H4_MAX_VAR_DIMS];
   int32 rank, data_type, n_attrs;
   char  name[H4_MAX_NC_NAME];
   int32 bands, rows, cols;

   std::vector<ossimHdfSubdataset> subDatasets;
   for (ossim_int32 index = 0; index < numDatasets; index++)
   {
      int32 sds_id = SDselect (m_sd_id, index);
      // int32 status = SDgetinfo (sds_id, name, &rank, dim_sizes, &data_type, &n_attrs);
      SDgetinfo (sds_id, name, &rank, dim_sizes, &data_type, &n_attrs);

      //get bands, rows, cols info
      if (rank == 3)
      {
         bands = dim_sizes[0];
         rows = dim_sizes[1];
         cols = dim_sizes[2];
      }
      else if (rank == 2)
      {
         bands = 1;
         rows = dim_sizes[0];
         cols = dim_sizes[1];
      }
      else if (rank == 1)
      {
         bands = dim_sizes[0];
         rows = 0;
         cols = 0;
      }
      else
      {
         bands = 0;
         rows = 0;
         cols = 0;
      }

      if (m_isHdfEos)
      {
         std::map<int32, ossimString>::iterator it = m_groupInfos.find(sds_id);
         if (it != m_groupInfos.end())
         {
            ossimString groupName = it->second;

            ossimString subsetdataName = groupName + ":" + ossimString(name);

            std::set<ossimString>::iterator subIt = m_subDatasetNames.find(subsetdataName);

            if (subIt != m_subDatasetNames.end())
            {
               subDatasets.push_back(ossimHdfSubdataset(index, 
                  subsetdataName, 
                  data_type, 
                  rank, 
                  bands, 
                  rows, 
                  cols,
                  n_attrs));
            }
         }
      }
      else
      {
         if (m_hdfDataType == HDF4_HYPERION_L1)
         {
            if (rank == 3)
            {
               bands = dim_sizes[1];
               rows = dim_sizes[0];
               cols = dim_sizes[2];
            }
         }
         subDatasets.push_back(ossimHdfSubdataset(index, 
            ossimString(name), 
            data_type, 
            rank, 
            bands, 
            rows, 
            cols,
            n_attrs));
      }

      SDendaccess(sds_id);
   }

   if (subDatasets.size() > 0)
   {
      if (m_isHdfEos)
      {
         ossimString subName = subDatasets[0].m_name;
         ossimString groupName = subName.split(":")[0];
         ossim_uint32 index;
         std::vector<ossimHdfSubdataset> geoSubDatasets;
         std::vector<ossimHdfSubdataset> dataSubDatasets;
         for (index = 0; index < subDatasets.size(); index++)
         {
            ossimString subNameTmp = subDatasets[index].m_name;
            ossimString groupNameTmp = subNameTmp.split(":")[0];
            ossimString fieldName = subNameTmp.split(":")[1];
            if (groupNameTmp == groupName)
            {
               if (fieldName == "Longitude" || fieldName == "Latitude")
               {
                  geoSubDatasets.push_back(subDatasets[index]);
               }
               else
               {
                  dataSubDatasets.push_back(subDatasets[index]);
               }
            }
            else
            {
               m_groupDatasets.push_back(ossimHdfGroupData(groupName,
                  geoSubDatasets,
                  dataSubDatasets));

               groupName = groupNameTmp;
               geoSubDatasets.clear();
               dataSubDatasets.clear();

               //insert first dataset for a different group
               if (fieldName == "Longitude" || fieldName == "Latitude")
               {
                  geoSubDatasets.push_back(subDatasets[index]);
               }
               else
               {
                  dataSubDatasets.push_back(subDatasets[index]);
               }
            }
         }

         //insert last datasets to the group 
         if (index == subDatasets.size())
         {
            m_groupDatasets.push_back(ossimHdfGroupData(groupName,
               geoSubDatasets,
               dataSubDatasets));
         }
      }
      else
      {
         m_subDatasets = subDatasets;
      }
   }

   //only insert data field data to the vector of sub datasets, not geo field data
   for (ossim_uint32 index = 0; index < m_groupDatasets.size(); index++)
   {
      std::vector<ossimHdfSubdataset> dataSubdatasets = m_groupDatasets[index].m_dataSubdatasets;
      for (ossim_uint32 dataIndex = 0; dataIndex < dataSubdatasets.size(); dataIndex++)
      {
         m_subDatasets.push_back(dataSubdatasets[dataIndex]);
      }
   }

   if (m_subDatasets.size() > 0)
   {
      return true;
   }
   return false;
}

ossimString ossimHdf4Reader::getAttribuitValues(int32 dataType, 
                                                int32 numValues,
                                                void* attrValue) const
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

void ossimHdf4Reader::parseMetaString(ossimString attrStr)
{
   std::vector<ossimString> attrList = attrStr.split("\n");
   ossimString className = "";
   for (ossim_uint32 i  = 0; i < attrList.size(); i++)
   {
      ossimString attrValue = attrList[i];

      if (attrValue.contains("OBJECT") && attrValue.contains("=") && !attrValue.contains("END_OBJECT"))
      {
         if (attrValue.split("=").size() > 1)
         {
            if (attrValue.split("=")[1].trim() != "PARAMETERVALUE") //will use the value of this class
            {
               className = attrValue.split("=")[1].trim();
            }
         }
      }
      else if (attrValue.contains("VALUE") && attrValue.contains("="))
      {
         if (attrValue.split("=").size() > 1)
         {
            ossimString classValue = attrValue.split("=")[1].trim();
            classValue = classValue.substitute("\"", "", true);
            classValue = classValue.substitute("(", "", true);
            classValue = classValue.substitute(")", "", true);
            if (className == "ADDITIONALATTRIBUTENAME") //skip this class name to get the value for this class
            {
               className = classValue;
               continue;
            }
            if (classValue != "PARAMETERVALUE")
            {
               m_globalMeta.push_back(ossimString(className +  ": " + classValue));
            }
         }
      }
   }
}

void ossimHdf4Reader::initHdfDataType()
{
   for (ossim_uint32 i = 0; i < m_globalMeta.size(); i++)
   {
      ossimString meta = m_globalMeta[i];
      ossimString metaName = meta.split(":")[0];
      ossimString metaValue = meta.split(":")[1];
      if (metaName == "Title")
      {
         if (metaValue == "SeaWiFS Level-1A Data")
         {
            m_hdfDataType = HDF4_SEAWIFS_L1A;
         }
         else if (metaValue == "SeaWiFS Level-2 Data")
         {
            m_hdfDataType = HDF4_SEAWIFS_L2;
         }
         else if (metaValue == "SeaWiFS Level-3 Standard Mapped Image")
         {
            m_hdfDataType = HDF4_SEAWIFS_L3;
         }
      }
      else if (metaName == "L1 File Generated By")
      {
         if (metaValue.contains("HYP version "))
         {
            m_hdfDataType = HDF4_HYPERION_L1;
         }
      }
      else if (metaName == "HDFEOSVersion")
      {
         m_isHdfEos = true;
      }
      else
      {
         m_hdfDataType = "UNKNOWN";
      }
   }
}

bool ossimHdf4Reader::isOpen()const
{
   return m_currentSubDataset != NULL;
}

void ossimHdf4Reader::closeEntry()
{
   m_tile = 0;
   SDendaccess(m_sd_id);
   SDend(m_sd_id); 
   Hclose(m_fid);

   if (m_currentSubDataset != NULL)
   {
      delete m_currentSubDataset;
      m_currentSubDataset = 0;
   }
   ossimImageHandler::close();
}

ossimRefPtr<ossimImageData> ossimHdf4Reader::getTile(const ossimIrect& tile_rect, 
                                                     ossim_uint32 resLevel)
{
   if (m_tile.valid())
   {
      // Image rectangle must be set prior to calling getTile.
      m_tile->setImageRectangle(tile_rect);

      if ( getTile( m_tile.get(), resLevel ) == false )
      {
         if (m_tile->getDataObjectStatus() != OSSIM_NULL)
         {
            m_tile->makeBlank();
         }
      }
   }

   return m_tile;
}

bool ossimHdf4Reader::getTile(ossimImageData* result,
                              ossim_uint32 resLevel)
{
   bool status = false;

   //---
   // Not open, this tile source bypassed, or invalid res level,
   // return a blank tile.
   //---
   if( isOpen() && isSourceEnabled() && isValidRLevel(resLevel) &&
      result && (result->getNumberOfBands() == getNumberOfOutputBands()) )
   {
      result->ref();  // Increment ref count.

      //---
      // Check for overview tile.  Some overviews can contain r0 so always
      // call even if resLevel is 0.  Method returns true on success, false
      // on error.
      //---
      status = getOverviewTile(resLevel, result);

      if (!status) // Did not get an overview tile.
      {
         status = true;

         ossimIrect tile_rect = result->getImageRectangle();     

         if (getImageRectangle().intersects(tile_rect))
         {
            // Make a clip rect.
            ossimIrect clipRect = tile_rect.clipToRect(getImageRectangle());

            if (tile_rect.completely_within(clipRect) == false)
            {
               // Not filling whole tile so blank it out first.
               result->makeBlank();
            }

            if (m_currentSubDataset != NULL)
            {
               for (ossim_uint32 band = 0; band < getNumberOfInputBands(); band++)
               {
                  void* dataBuffer = m_currentSubDataset->getTileBuf(clipRect, band);
                  result->loadBand(dataBuffer, clipRect, band);
                  delete[] (char*)dataBuffer;
               }
            }
         }
         else // No intersection...
         {
            result->makeBlank();
         }
      }
      result->validate();
      result->unref();  // Decrement ref count.
   }

   return status;
}

ossim_uint32 ossimHdf4Reader::getNumberOfInputBands() const
{
   if (m_currentSubDataset != NULL)
   {
      return m_currentSubDataset->getNumberOfInputBands();
   }
   return m_numberOfBands;
}

ossim_uint32 ossimHdf4Reader::getNumberOfOutputBands()const
{
   return getNumberOfInputBands();
}

ossim_uint32 ossimHdf4Reader::getImageTileWidth() const
{
   return m_imageRect.width();
}

ossim_uint32 ossimHdf4Reader::getImageTileHeight() const
{
   return m_imageRect.height();
}

ossimScalarType ossimHdf4Reader::getOutputScalarType() const
{
   if (m_currentSubDataset != NULL)
   {
      return m_currentSubDataset->getOutputScalarType();
   }
   return m_scalarType;
}

ossimRefPtr<ossimImageGeometry> ossimHdf4Reader::getImageGeometry()
{
   if ( !theGeometry )
   {
      //---
      // Check for external geom:
      //---
      theGeometry = getExternalImageGeometry();

      if ( !theGeometry )
      {
         //---
         // Check the internal geometry first to avoid a factory call.
         //---
         theGeometry = getInternalImageGeometry();

         // At this point it is assured theGeometry is set.

         // Check for set projection.
         if ( !theGeometry->getProjection() )
         {
            // Try factories for projection.
            ossimImageGeometryRegistry::instance()->extendGeometry(this);
         }
      }

      // Set image things the geometry object should know about.
      initImageParameters( theGeometry.get() );
   }

   return theGeometry;
}

ossimRefPtr<ossimImageGeometry> ossimHdf4Reader::getInternalImageGeometry() const
{
   static const char MODULE[] = "ossimHdf4Reader::getInternalImageGeometry";
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   ossimRefPtr<ossimImageGeometry> geom = new ossimImageGeometry();

   // Set the projection and return this handler's image geometry:
   if (m_hdfDataType == HDF4_EOS_SWATH)
   {
      geom->setProjection(processSwathProjection().get());
   }
   else if (m_hdfDataType == HDF4_EOS_GRID)
   {
      geom->setProjection(processGridProjection().get());
   }
   else if (m_hdfDataType == HDF4_SEAWIFS_L3)
   {
      if (m_currentEntryRender < m_subDatasets.size())
      {
         ossim_float64 ulx = getMetaValue("Westernmost Longitude").toDouble();
         ossim_float64 uly = getMetaValue("Northernmost Latitude").toDouble();
         ossim_float64 lrx = getMetaValue("Easternmost Longitude").toDouble();
         ossim_float64 lry = getMetaValue("Southernmost Latitude").toDouble();

         int32 rows = m_subDatasets[m_currentEntryRender].m_rows;
         int32 cols = m_subDatasets[m_currentEntryRender].m_cols;

         ossim_float64 xRes = (lrx - ulx)/cols;
         ossim_float64 yRes = (uly - lry)/rows;
         ossimDpt gsd(fabs(xRes), fabs(yRes));

         ossimGpt gTie(uly, ulx);
         ossimString name = "EPSG:4326";
         ossimRefPtr<ossimProjection> proj = ossimEpsgProjectionFactory::instance()->createProjection(name);
         ossimMapProjection* mapProj = dynamic_cast<ossimMapProjection*>(proj.get());
         mapProj->setUlTiePoints(gTie);
         mapProj->setDecimalDegreesPerPixel(gsd);

         geom->setProjection(mapProj);
      }
   }

   return geom;
}

ossimRefPtr<ossimProjection> ossimHdf4Reader::processGridProjection() const
{
   if (m_structMetadata.empty())
      return 0;

   int32   zoneCode = 0;
   int32   sphereCode = 0;
   int32   xSize, ySize;
   std::vector<ossim_float64> upLeftValues;
   std::vector<ossim_float64> lowRightValues;
   std::vector<ossim_float64> projParms;
   ossimString projName;

   std::vector<ossimString> attrList = m_structMetadata.split("\n");
   for (ossim_uint32 i = 0; i < attrList.size(); i++)
   {
      ossimString attrInfo = attrList[i].trim();
      attrInfo = attrInfo.substitute("\"", "", true);
      attrInfo = attrInfo.substitute("(", "", true);
      attrInfo = attrInfo.substitute(")", "", true);
      if (attrInfo.contains("="))
      {
         ossimString metaName = attrInfo.split("=")[0].trim();
         ossimString metaValue = attrInfo.split("=")[1].trim();
         if (metaName == "XDim")
         {
            xSize = metaValue.toInt();
         }
         if (metaName == "YDim")
         {
            ySize = metaValue.toInt32();
         }
         if (metaName == "Projection")
         {
            projName = metaValue;
         }
         if (metaName == "ZoneCode")
         {
            zoneCode = metaValue.toInt32();
         }
         if (metaName == "ProjParams")
         {
            for (ossim_uint32 paramIndex = 0; paramIndex < metaValue.split(",").size(); paramIndex++)
            {
               projParms.push_back(metaValue.split(",")[paramIndex].toDouble());
            }
         }
         if (metaName == "UpperLeftPointMtrs")
         {
            for (ossim_uint32 ulIndex = 0; ulIndex < metaValue.split(",").size(); ulIndex++)
            {
               upLeftValues.push_back(metaValue.split(",")[ulIndex].toDouble());
            }
         }
         if (metaName == "LowerRightMtrs")
         {
            for (ossim_uint32 lrIndex = 0; lrIndex < metaValue.split(",").size(); lrIndex++)
            {
               lowRightValues.push_back(metaValue.split(",")[lrIndex].toDouble());
            }
         }
         if (metaName == "SphereCode")
         {
            sphereCode = metaValue.toInt32();
         } 
      }
   }

   //calculate w-e pixel resolution and n-s pixel resolution 
   ossim_float64 xRes = 0.0;
   ossim_float64 yRes = 0.0;
   if (lowRightValues.size() == 2 && upLeftValues.size() == 2 && ySize > 0 && xSize > 0)
   {
      xRes = (lowRightValues[0] - upLeftValues[0]) / xSize;
      yRes = (lowRightValues[1] - upLeftValues[1]) / ySize;
   }
   ossimDpt gsd(fabs(xRes), fabs(yRes));

   //create the map projection
   ossimRefPtr<ossimMapProjection> mapProj = 0;
   if (projName == "GCTP_GEO") //Geographic
   {
      // Tie point is expressed as lat lon:
      ossimGpt gTie(upLeftValues[1], upLeftValues[0]);
      ossimString epsg("EPSG:4326");
      mapProj = PTR_CAST(ossimMapProjection, 
         ossimEpsgProjectionFactory::instance()->createProjection(epsg));
      mapProj->setUlTiePoints(gTie);
      mapProj->setDecimalDegreesPerPixel(gsd);
   }
   else 
   {
      // Map projection, tie point expressed as easting northing:
      ossimDpt tie(upLeftValues[0], upLeftValues[1]);

      if (projName == "GCTP_UTM")
         mapProj = new ossimUtmProjection(zoneCode);
      else if (projName == "GCTP_LAMCC") 
         mapProj = new ossimLambertConformalConicProjection();
      else if (projName == "GCTP_PS") 
         mapProj = new ossimPolarStereoProjection();
      else if (projName == "GCTP_POLYC") 
         mapProj = new ossimPolyconicProjection();
      else if (projName == "GCTP_TM") 
         mapProj = new ossimMercatorProjection();
      else if (projName == "GCTP_LAMAZ") 
         mapProj = new ossimAzimEquDistProjection();
      else if (projName == "GCTP_HOM") 
         mapProj = new ossimObliqueMercatorProjection();
      else if (projName == "GCTP_SOM") 
         mapProj = new ossimSpaceObliqueMercatorProjection();
      else if (projName == "GCTP_ISINUS" || projName == "GCTP_SNSOID") 
         mapProj = new ossimSinusoidalProjection();

      mapProj->setUlTiePoints(tie);
      mapProj->setMetersPerPixel(gsd);
   }

   ossimRefPtr<ossimProjection> return_pointer (mapProj.get());
   return return_pointer;
}

ossimRefPtr<ossimProjection> ossimHdf4Reader::processSwathProjection() const
{
   if (m_currentEntryRender < m_subDatasets.size())
   {
      ossim_int32 latGridIndex = -1;
      ossim_int32 lonGridIndex = -1;
      ossim_int32 spacingX = -1;
      ossim_int32 spacingY = -1;
      ossim_int32 latticePointIndex = -1;

      ossim_int32 latGridRows = 0;
      ossim_int32 latGridCols = 0;
      ossim_int32 lonGridRows = 0;
      ossim_int32 lonGridCols = 0;
      bool hasGeoData = false;
    
      std::vector<float64> latticeYPoints;
      std::vector<float64> latticeXPoints;

      ossimString subDatasetName = m_subDatasets[m_currentEntryRender].m_name;
      ossimString groupName = subDatasetName.split(":")[0];

      for (ossim_uint32 i = 0; i < m_groupDatasets.size(); i++)
      {
         if (groupName == m_groupDatasets[i].m_groupName)
         {
            //get lat and lon info
            if (m_groupDatasets[i].m_geoSubdatasets.size() > 0)
            {
               for (ossim_uint32 j = 0; j < m_groupDatasets[i].m_geoSubdatasets.size(); j++)
               {
                  if (m_groupDatasets[i].m_geoSubdatasets[j].m_name.contains("Longitude"))
                  {
                     lonGridIndex = m_groupDatasets[i].m_geoSubdatasets[j].m_sdsIndex;
                     lonGridCols = m_groupDatasets[i].m_geoSubdatasets[j].m_cols;
                     lonGridRows = m_groupDatasets[i].m_geoSubdatasets[j].m_rows;
                     hasGeoData = true;
                  }
                  if (m_groupDatasets[i].m_geoSubdatasets[j].m_name.contains("Latitude"))
                  {
                     latGridIndex = m_groupDatasets[i].m_geoSubdatasets[j].m_sdsIndex;
                     latGridCols = m_groupDatasets[i].m_geoSubdatasets[j].m_cols;
                     latGridRows = m_groupDatasets[i].m_geoSubdatasets[j].m_rows;
                     hasGeoData = true;
                  }
               }
            }

            //get lattice point info
            if (m_groupDatasets[i].m_dataSubdatasets.size() > 0)
            {
               for (ossim_uint32 j = 0; j < m_groupDatasets[i].m_dataSubdatasets.size(); j++)
               {
                  if (m_groupDatasets[i].m_dataSubdatasets[j].m_name.contains("LatticePoint"))
                  {
                     latticePointIndex = m_groupDatasets[i].m_dataSubdatasets[j].m_sdsIndex;
                  }
               }
            }
         }
      }

      if (hasGeoData)
      {
         if (latticePointIndex < 0)
         {
            ossim_uint32 rows = m_subDatasets[m_currentEntryRender].m_rows;
            ossim_uint32 cols = m_subDatasets[m_currentEntryRender].m_cols;
            if (rows % latGridRows == 0 && cols % lonGridCols == 0)
            {
               spacingY = rows/latGridRows; //line increment step
               spacingX = cols/lonGridCols; //pixel increment step
            }
            else if (rows % (latGridRows-1) == 0 && cols % (lonGridCols-1) == 0)//ASTER L1A and L1B
            {
               spacingY = rows/(latGridRows-1); //line increment step
               spacingX = cols/(lonGridCols-1); //pixel increment step
            }
            else if (rows % latGridRows == 0 && cols % (lonGridCols-1) == 0)
            {
               spacingY = rows/latGridRows; //line increment step
               spacingX = cols/(lonGridCols-1); //pixel increment step
            }
            else if (rows % (latGridRows-1) == 0 && cols % lonGridCols == 0)
            {
               spacingY = rows/(latGridRows-1); //line increment step
               spacingX = cols/lonGridCols; //pixel increment step
            }
            else if ((rows+1) % latGridRows == 0 && (cols+1) % lonGridCols == 0)
            {
               spacingY = (rows+1)/latGridRows; //line increment step
               spacingX = (cols+1)/lonGridCols; //pixel increment step
            }
            else if ((rows+1) % latGridRows == 0 && cols % lonGridCols == 0)
            {
               spacingY = (rows+1)/latGridRows; //line increment step
               spacingX = cols/lonGridCols; //pixel increment step
            }
            else if (rows % latGridRows == 0 && (cols+1) % lonGridCols == 0)
            {
               spacingY = rows/latGridRows; //line increment step
               spacingX = (cols+1)/lonGridCols; //pixel increment step
            }
            ossimIpt spacing(spacingX, spacingY);
            ossimDrect rect(0, 0, cols-1, rows-1);
            return new ossimHdfGridModel(theImageFile, rect, 
                                         ossimString::toString(latGridIndex),
                                         ossimString::toString(lonGridIndex), spacing);
         }  
         else
         {
            int32 dim_sizes[H4_MAX_VAR_DIMS];
            int32 rank, data_type, n_attrs;
            char  name[H4_MAX_NC_NAME];
            int32 start[H4_MAX_NC_DIMS], edges[H4_MAX_NC_DIMS];

            std::vector<float64> lat;
            std::vector<float64> lon;
            int32 sd_id = SDstart(theImageFile.c_str(), DFACC_READ);
            int32 sds_id = SDselect(sd_id, latGridIndex);
            int32 status = SDgetinfo(sds_id, name, &rank, dim_sizes, &data_type, &n_attrs);

            if (status != -1)
            {
               start[0] = 0;
               start[1] = 0;
               edges[0] = dim_sizes[0];
               edges[1] = dim_sizes[1];

               ossim_int32 size = getSizeByType(data_type);
               char* values = new char[size * dim_sizes[0]* dim_sizes[1]];

               // intn statusN = SDreaddata(sds_id, start, NULL, edges, (VOIDP)values);
               SDreaddata(sds_id, start, NULL, edges, (VOIDP)values);

               if (data_type == DFNT_FLOAT64 || data_type == DFNT_DOUBLE)
               {
                  float64* doubleData = (float64*)values;
                  for (int i = 0; i < dim_sizes[0]* dim_sizes[1]; i++)
                  {
                     lat.push_back(doubleData[i]);
                  }
               }
               else
               {
                  float32* floatData = (float32*)values;
                  for (int i = 0; i < dim_sizes[0]* dim_sizes[1]; i++)
                  {
                     lat.push_back(floatData[i]);
                  }
               }
               delete[] values;
            }
            SDendaccess(sds_id); 

            sds_id = SDselect(sd_id, lonGridIndex);
            status = SDgetinfo(sds_id, name, &rank, dim_sizes, &data_type, &n_attrs);

            if (status != -1)
            {
               start[0] = 0;
               start[1] = 0;
               edges[0] = dim_sizes[0];
               edges[1] = dim_sizes[1];

               ossim_int32 size = getSizeByType(data_type);
               char* values = new char[size * dim_sizes[0]* dim_sizes[1]];

               // intn statusN = SDreaddata(sds_id, start, NULL, edges, (VOIDP)values);
               SDreaddata(sds_id, start, NULL, edges, (VOIDP)values);

               if (data_type == DFNT_FLOAT64 || data_type == DFNT_DOUBLE)
               {
                  float64* doubleData = (float64*)values;
                  for (int i = 0; i < dim_sizes[0]* dim_sizes[1]; i++)
                  {
                     lon.push_back(doubleData[i]);
                  }
               }
               else
               {
                  float32* floatData = (float32*)values;
                  for (int i = 0; i < dim_sizes[0]* dim_sizes[1]; i++)
                  {
                     lon.push_back(floatData[i]);
                  }
               }
               delete[] values;
               SDendaccess(sds_id); 


               sds_id = SDselect(sd_id, latticePointIndex);
               status = SDgetinfo(sds_id, name, &rank, dim_sizes, &data_type, &n_attrs);
               if (rank == 3)
               {
                  int32 loopIndex = dim_sizes[0];
                  int32 latticeY = dim_sizes[1];
                  int32 latticeX = dim_sizes[2];
                  for (int32 latticeIndex = 0; latticeIndex < loopIndex; latticeIndex++)
                  {
                     start[0] = latticeIndex;
                     start[1] = 0;
                     start[2] = 0;
                     edges[0] = 1;
                     edges[1] = latticeY; 
                     edges[2] = latticeX; 

                     ossim_int32 size = getSizeByType(data_type);
                     char* values = new char[size * latticeY*latticeX];

                     // intn statusLattice = SDreaddata(sds_id, start, NULL, edges, (VOIDP)values);
                     SDreaddata(sds_id, start, NULL, edges, (VOIDP)values);

                     if (data_type == DFNT_INT32)
                     {
                        int32* intData = (int32*)values;
                        for (int i = 0; i < latticeY*latticeX; i+=2)
                        {
                           latticeXPoints.push_back(intData[i]);
                           latticeYPoints.push_back(intData[i+1]);
                        }
                     }
                     else
                     {
                        float32* floatData = (float32*)values;
                        for (int i = 0; i < latticeY* latticeX; i+=2)
                        {
                           latticeXPoints.push_back(floatData[i]);
                           latticeYPoints.push_back(floatData[i+1]);
                        }
                     }
                     delete[] values;
                  }
               }
               SDendaccess(sds_id);
               SDend (sd_id); 
            }

            ossimTieGptSet tieSet;
            int iStepX = MAX(1,((dim_sizes[1]-1) / 11));
            int iStepY = MAX(1,((dim_sizes[0]-1) / 11));

            for (int y = 0; y < dim_sizes[0]; y += iStepY)
            {
               for (int x = 0; x < dim_sizes[1]; x += iStepX)
               {
                  int index = x + (y * dim_sizes[1]);
                  if (index%10 == 0)
                  {
                     double dPixel = latticeXPoints[index] + 0.5;
                     double dLine = latticeYPoints[index] + 0.5;

                     ossimDpt dpt(dPixel, dLine);
                     ossimGpt gpt(lat[index], lon[index]);
                     tieSet.addTiePoint(new ossimTieGpt(gpt, dpt, .5));
                  }
               }
            }

            ossimRefPtr<ossimBilinearProjection> bilinProj = new ossimBilinearProjection;
            bilinProj->optimizeFit(tieSet);
            return bilinProj.get();
         }
      }
   }

   return ossimRefPtr<ossimProjection>();
}

bool ossimHdf4Reader::loadState(const ossimKeywordlist& kwl,
                                const char* prefix)
{
   bool result = false;

   if ( ossimImageHandler::loadState(kwl, prefix) )
   {
      result = open();
   }

   return result;
}

ossim_uint32 ossimHdf4Reader::getNumberOfEntries()const
{
   return (ossim_uint32) m_subDatasets.size();
}

bool ossimHdf4Reader::setCurrentEntry(ossim_uint32 entryIdx)
{
   if (m_currentEntryRender == entryIdx)
   {
      return true; // Nothing to do...
   }
   m_currentEntryRender = entryIdx;
   theGeometry = 0;
   theOverview = 0;
   theOverviewFile.clear();
   m_tile = 0;
   return open();
}

std::vector<ossimString> ossimHdf4Reader::getSubDatasetMeta()
{
   if (m_currentSubDataset != NULL)
   {
      return m_currentSubDataset->getMetadata();
   }
   return std::vector<ossimString>();
}

void ossimHdf4Reader::getEntryList(std::vector<ossim_uint32>& entryList)const
{
   for (ossim_uint32 i = 0; i < m_subDatasets.size(); i++)
   {
      entryList.push_back(i);
   }
}

ossimString ossimHdf4Reader::getMetaValue(const ossimString& key) const
{
   for (ossim_uint32 i = 0; i < m_globalMeta.size(); i++)
   {
      ossimString info = m_globalMeta[i];
      ossimString keyTmp = info.split(":")[0].trim();
      ossimString value = info.split(":")[1].trim();
      if (keyTmp == key)
      {
         return value;
      }
   }
   return "";
}

ossim_uint32 ossimHdf4Reader::getCurrentEntry()const
{
   return m_currentEntryRender;
}


