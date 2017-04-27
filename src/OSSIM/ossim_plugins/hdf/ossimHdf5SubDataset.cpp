//----------------------------------------------------------------------------
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description:  Class definition for HDF reader.
//
//----------------------------------------------------------------------------
// $Id: ossimHdf5SubDataset.cpp 2645 2011-05-26 15:21:34Z oscar.kramer $

//ossim includes
#include "ossimHdf5SubDataset.h"
#include <ossim/base/ossimScalarTypeLut.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include <ossim/imaging/ossimImageGeometryRegistry.h>
#include <ossim/imaging/ossimImageDataFactory.h>

#include <cstdlib> /* for malloc */

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id";
#endif

static ossimTrace traceDebug("ossimHdf5SubDataset:debug");

RTTI_DEF_INST(ossimHdf5SubDataset, "ossimHdf5SubDataset")

herr_t Hdf5SubAttrIterate(hid_t hdfObjId, const char* attrName, void* reader)
{
   hid_t           attrId;
   hid_t           attrTypeID;
   hid_t           attrNativeType;
   hid_t           attrSpace;

   char*           dataValue = NULL;
   hsize_t         size[64];
   unsigned int    attrElmts;
   hsize_t         attrSize;
   hsize_t         i;
   void*           buf = NULL;
   ossim_uint32    attrDims;
   ossimString     attrValue;

   ossimHdf5SubDataset* hdf5Reader = (ossimHdf5SubDataset*)reader;

   attrId          = H5Aopen_name(hdfObjId, attrName);
   attrTypeID      = H5Aget_type(attrId);
   attrNativeType  = H5Tget_native_type(attrTypeID, H5T_DIR_DEFAULT);
   attrSpace       = H5Aget_space(attrId);
   attrDims        = H5Sget_simple_extent_dims(attrSpace, size, NULL);

   if( H5Tget_class(attrNativeType) == H5T_STRING ) 
   {
      attrSize = H5Tget_size(attrTypeID);
      dataValue = (char*)malloc(attrSize+1);
      H5Aread(attrId, attrNativeType, dataValue);
      dataValue[attrSize]='\0';
      attrValue = ossimString(dataValue);
      hdf5Reader->setMeta(ossimString(attrName), attrValue);
   }
   else 
   {
      attrElmts = 1;
      for( i=0; i < attrDims; i++ ) 
      {
         attrElmts *= size[i];
      }
      if( attrElmts > 0 ){
         buf = (void *)malloc(attrElmts*H5Tget_size(attrNativeType));
         dataValue = (char*)malloc(8192);
         dataValue[0]='\0';
         H5Aread(attrId, attrNativeType, buf);
      }

      if (H5Tequal(H5T_NATIVE_CHAR, attrNativeType))
      {
         for(i = 0; i < attrElmts; i++ ) 
         {
            attrValue += ossimString(((ossim_int8*)buf)[i]) + " ";
         }
      }
      if (H5Tequal( H5T_NATIVE_UCHAR, attrNativeType))
      {
         for(i = 0; i < attrElmts; i++ ) 
         {
            attrValue += ossimString(((ossim_uint8*)buf)[i]) + " ";
         }
      }
      else if(H5Tequal( H5T_NATIVE_SHORT, attrNativeType)) 
      {
         for(i = 0; i < attrElmts; i++ ) 
         {
            attrValue += ossimString::toString(((ossim_int16*) buf)[i]) + " ";
         }
      }
      else if(H5Tequal(H5T_NATIVE_USHORT, attrNativeType)) 
      {
         for(i = 0; i < attrElmts; i++) 
         {
            attrValue += ossimString::toString(((ossim_uint16*)buf)[i]) + " ";
         }
      }
      else if(H5Tequal( H5T_NATIVE_INT, attrNativeType)) 
      {
         for(i = 0; i < attrElmts; i++) 
         {
            attrValue += ossimString::toString(((ossim_int32*) buf)[i]) + " ";
         }
      }
      else if(H5Tequal( H5T_NATIVE_UINT, attrNativeType ) ) 
      {
         for(i = 0; i < attrElmts; i++)  
         {
            attrValue += ossimString::toString(((ossim_uint32*) buf)[i]) + " ";   
         }
      }
      else if(H5Tequal( H5T_NATIVE_LONG, attrNativeType)) 
      {
         for(i = 0; i < attrElmts; i++)  
         {
            attrValue += ossimString::toString(((ossim_int64*)buf)[i]) + " ";         
         }
      }
      else if(H5Tequal( H5T_NATIVE_ULONG, attrNativeType))
      {
         for(i = 0; i < attrElmts; i++)  
         {
            attrValue += ossimString::toString(((ossim_uint64*)buf)[i]) + " ";
         }
      }
      else if(H5Tequal( H5T_NATIVE_FLOAT, attrNativeType)) 
      {
         for(i = 0; i < attrElmts; i++)  
         {
            attrValue += ossimString::toString(((ossim_float32*)buf)[i]) + " ";
         }
      }
      else if(H5Tequal( H5T_NATIVE_DOUBLE, attrNativeType)) 
      {
         for(i = 0; i < attrElmts; i++)  
         {
            attrValue += ossimString::toString(((ossim_float64*)buf)[i]);
         }
      }
      hdf5Reader->setMeta(ossimString(attrName), attrValue);

      free(buf);
   }
   H5Aclose(attrId);
   free(dataValue);

   return 0;
}

ossimHdf5SubDataset::ossimHdf5SubDataset()
                         : m_fileName(""),
                           m_dataset_name(""),
                           m_fid(-1),
                           m_dataset_id(-1),
                           m_rank(0),
                           m_native(-1),
                           m_dataType(-1),
                           m_dataspace(0),
                           m_size(0),
                           m_dataSize(0),
                           m_numberOfBands(0),
                           m_numberOfSamples(0),
                           m_numberOfLines(0),
                           m_scalarType(OSSIM_SCALAR_UNKNOWN),
                           m_hdfDataType(),
                           m_meta()
{
}

ossimHdf5SubDataset::~ossimHdf5SubDataset()
{
   close();
}

void ossimHdf5SubDataset::close()
{
   H5Tclose(m_dataType);
   H5Dclose(m_dataset_id);
   H5Sclose(m_dataspace);
   H5Fclose(m_fid);

   m_meta.clear();
}

ossim_uint32 ossimHdf5SubDataset::getNumberOfLines( ossim_uint32 /* resLevel */) const
{
   return m_numberOfLines;
}

ossim_uint32 ossimHdf5SubDataset::getNumberOfSamples( ossim_uint32 /* resLevel */) const
{
   return m_numberOfSamples;
}

bool ossimHdf5SubDataset::open()
{
   static const char MODULE[] = "ossimHdf5SubDataset::open";

   bool result = false;

   m_fid = H5Fopen(m_fileName.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
   if (m_fid < 0)
   {
      H5Fclose(m_fid);
      return false;
   }

   // Third arg added for compile fix but not checked. (drb)
   m_dataset_id = H5Dopen(m_fid, m_dataset_name, 0); 

   if (m_dataset_id < 0)
   {
      close();
      return false;
   }

   m_dataType = H5Dget_type(m_dataset_id);
   m_native = H5Tget_native_type(m_dataType, H5T_DIR_ASCEND );
   setOutputScalarType();

   H5Tget_class(m_dataType);   
   H5Tget_order(m_dataType);

   m_size = H5Tget_size(m_dataType);

   m_dataspace = H5Dget_space(m_dataset_id);    /* dataspace handle */
   m_rank = H5Sget_simple_extent_ndims(m_dataspace);

   if (m_rank == 4)
   {
      hsize_t  dims_out[4];
      herr_t status_n  = H5Sget_simple_extent_dims(m_dataspace, dims_out, NULL);
      if (status_n >= 0)
      {
         m_numberOfBands = dims_out[2];
         m_numberOfLines = dims_out[0];
         m_numberOfSamples = dims_out[1];
      }
   }
   else if (m_rank == 3)
   {
      hsize_t  dims_out[3];
      herr_t status_n  = H5Sget_simple_extent_dims(m_dataspace, dims_out, NULL);
      if (status_n >= 0)
      {
         if (dims_out[2] < dims_out[0] && dims_out[2] < dims_out[1])
         {
            m_numberOfBands = dims_out[2];
            m_numberOfLines = dims_out[0];
            m_numberOfSamples = dims_out[1];
         }
         else
         {
            m_numberOfBands = dims_out[0];
            m_numberOfLines = dims_out[1];
            m_numberOfSamples = dims_out[2];
         }
      }
   }
   else if (m_rank == 2)
   {
      hsize_t  dims_out[2];
      herr_t status_n  = H5Sget_simple_extent_dims(m_dataspace, dims_out, NULL);
      if (status_n >= 0)
      {
         m_numberOfBands = 1;
         m_numberOfLines = dims_out[0];
         m_numberOfSamples = dims_out[1];
      }
   }
   else if (m_rank == 1)
   {
      hsize_t  dims_out[1];
      herr_t status_n  = H5Sget_simple_extent_dims(m_dataspace, dims_out, NULL);
      if (status_n >= 0)
      {
         m_numberOfBands = 0;
         m_numberOfLines = dims_out[0];
         m_numberOfSamples = 0;
      }
   }
   else
   {
      m_numberOfBands = 0;
      m_numberOfLines = 0;
      m_numberOfSamples = 0;
   }

   result = initMeta();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }

   return result;
}

bool ossimHdf5SubDataset::initMeta()
{
   m_meta.clear();

   //insert name first
   m_meta.push_back("name: " + m_dataset_name);

   ossimHdf5SubDataset* reader = this;

   //---
   // H5Aiterate1 id depreciated.  Need to fix. (drb)
   // Should call H5Aiterate2 which is typedef H5Aiterate.
   // herr_t ret = H5Aiterate(m_dataset_id, (H5_index_t)0, Hdf5SubAttrIterate, (void*)reader);
   //---
   herr_t ret = H5Aiterate1(m_dataset_id, 0, Hdf5SubAttrIterate, (void*)reader);   

   if (ret == 0)
   {
      return true;
   }
   return false;
}

char* ossimHdf5SubDataset::getTileBuf(const  ossimIrect& rect, ossim_uint32 band)
{
   hsize_t     count[3];
   hsize_t     offset[3];
   hid_t       memspace;
   hsize_t     col_dims[3];

   if (m_rank == 3)
   {
      offset[0] = band;
      offset[1] = rect.ul().y;
      offset[2] = rect.ul().x;

      count[0] = 1;
      count[1] = rect.height();
      count[2] = rect.width();

      col_dims[0] = 1;
      col_dims[1] = rect.height();
      col_dims[2] = rect.width();
   }
   else
   {
      offset[0] = rect.ul().y;
      offset[1] = rect.ul().x;

      count[0] = rect.height();
      count[1] = rect.width();

      col_dims[0] = rect.height();
      col_dims[1] = rect.width();
   }

   // herr_t status =  H5Sselect_hyperslab(m_dataspace, H5S_SELECT_SET, offset, NULL, count, NULL );
   H5Sselect_hyperslab(m_dataspace, H5S_SELECT_SET, offset, NULL, count, NULL );   

   memspace = H5Screate_simple(m_rank, col_dims, NULL);
   hsize_t mem_offset[3];
   mem_offset[0] = 0;
   mem_offset[1] = 0;
   mem_offset[2] = 0;
   // int status =  H5Sselect_hyperslab(memspace, H5S_SELECT_SET, mem_offset, NULL, count, NULL);
   H5Sselect_hyperslab(memspace, H5S_SELECT_SET, mem_offset, NULL, count, NULL);

   ossim_int32 numValues = rect.width() * rect.height();
   char* data = new char[m_dataSize * numValues];
   // status = H5Dread(m_dataset_id, m_dataType, memspace, m_dataspace, H5P_DEFAULT, (void*)data);
   H5Dread(m_dataset_id, m_dataType, memspace, m_dataspace, H5P_DEFAULT, (void*)data);   

   H5Sclose(memspace);
   return data;
}

ossim_uint32 ossimHdf5SubDataset::getNumberOfInputBands() const
{
   return m_numberOfBands;
}

ossimScalarType ossimHdf5SubDataset::getOutputScalarType() const
{
   return m_scalarType;
}

void ossimHdf5SubDataset::setSubdatasetName(const ossimString& name)
{
   m_dataset_name = name;
}

void ossimHdf5SubDataset::setHdfFilename(ossimString fileName)
{
   m_fileName = fileName;
   open();
}

void ossimHdf5SubDataset::setOutputScalarType()
{
   if( H5Tequal(H5T_NATIVE_CHAR, m_dataType ) || H5Tequal(H5T_NATIVE_UCHAR,  m_dataType))
   {
      m_scalarType = OSSIM_UINT8;
      m_dataSize = 1;
   }
   else if( H5Tequal( H5T_NATIVE_SHORT, m_dataType))
   {
      m_scalarType = OSSIM_SSHORT16;
      m_dataSize = 2;
   }
   else if( H5Tequal( H5T_NATIVE_USHORT, m_dataType)) 
   {
      m_scalarType = OSSIM_UINT16;
      m_dataSize = 2;
   }
   else if( H5Tequal(H5T_NATIVE_INT, m_dataType)) 
   {
      m_scalarType = OSSIM_SINT32;
      m_dataSize = 4;
   }  
   else if( H5Tequal(H5T_NATIVE_UINT, m_dataType)) 
   {
      m_scalarType = OSSIM_UINT32;
      m_dataSize = 4;
   }
   else if( H5Tequal(H5T_NATIVE_LONG, m_dataType)) 
   {
      if( sizeof(long) == 4 )
      {
         m_scalarType = OSSIM_SINT32;
         m_dataSize = 4;
      }
   }
   else if(H5Tequal(H5T_NATIVE_ULONG, m_dataType)) 
   {
      if( sizeof(unsigned long) == 4 )
      {
         m_scalarType = OSSIM_UINT32;
         m_dataSize = 4;
      }
   }
   else if( H5Tequal(H5T_NATIVE_FLOAT,  m_dataType)) 
   {
      m_scalarType = OSSIM_FLOAT32;
      m_dataSize = 4;
   }
   else if( H5Tequal(H5T_NATIVE_DOUBLE, m_dataType)) 
   {
      m_scalarType = OSSIM_FLOAT64;
      m_dataSize = 8;
   }
   else if( H5Tequal( H5T_NATIVE_LLONG, m_dataType) || H5Tequal( H5T_NATIVE_ULLONG, m_dataType )) 
   {
      m_scalarType = OSSIM_SCALAR_UNKNOWN;
   }
   else if( H5Tequal( H5T_NATIVE_DOUBLE, m_dataType)) 
   {
      m_scalarType = OSSIM_SCALAR_UNKNOWN;
      m_dataSize = 8;
   }
}

void ossimHdf5SubDataset::setMeta(const ossimString& key, const ossimString& value)
{
   ossimString meta = ossimString(key + ": " + value);
   m_meta.push_back(meta);
}

