//----------------------------------------------------------------------------
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: OSSIM HDF5 utility class.
//
//----------------------------------------------------------------------------
// $Id

#include <ossimH5Util.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimScalarTypeLut.h>

//---
// This includes everything!  Note the H5 includes are order dependent; hence,
// the mongo include.
//---
#include <hdf5.h>
#include <H5Cpp.h>

#include <iostream> /* tmp */
#include <ostream>
using namespace std;

std::ostream& ossim_hdf5::print(H5::H5File* file, std::ostream& out)
{
   if ( file )
   {
      std::string groupName     = "/";
      std::string prefix        = "hdf5";
      ossim_uint32 recurseCount = 0;
      
      ossim_hdf5::printIterative( file, groupName, prefix, recurseCount, out );
   }

   return out;
}

void ossim_hdf5::printIterative( H5::H5File* file,
                                 const std::string& groupName,
                                 const std::string& prefix,
                                 ossim_uint32& recursedCount,
                                 std::ostream& out )
{
   if ( file && groupName.size() )
   {
      ++recursedCount;
      
      H5::Group* group = new H5::Group( file->openGroup(groupName) );

      // Print attributes:
      const ossim_uint32 ATTRS_COUNT = group->getNumAttrs();
      for ( ossim_uint32 aIdx = 0; aIdx < ATTRS_COUNT; ++aIdx )
      {
         H5::Attribute attr( group->openAttribute( aIdx ) );
         ossim_hdf5::printAttribute( attr, prefix, out );
         attr.close();
      }
      
      const hsize_t OBJ_COUNT = group->getNumObjs();
      for ( hsize_t i = 0; i < OBJ_COUNT; ++i )
      {
         std::string objName = group->getObjnameByIdx(i);

         if ( objName.size() )
         {
            char separator = '/';
            std::string combinedName;
            combine( groupName, objName, separator, combinedName );
            
            separator = '.';
            std::string combinedPrefix;
            combine( prefix, objName, separator, combinedPrefix );
            
            H5G_obj_t objType   = group->getObjTypeByIdx(i);

#if 0
            std::cout << "combinedName: " << combinedName
                      << "\ncombinedPrefix: " << combinedPrefix
                      << "\ngetObjnameByIdx[" << i << "]: " << objName
                      << "\ngetObjTypeByIdx[" << i << "]: " << objType
                      << std::endl;
#endif
            
            if ( objType == H5G_GROUP )
            {
               // Recursive call:
               if ( recursedCount < ossim_hdf5::MAX_RECURSION_LEVEL )
               {
                  ossim_hdf5::printIterative(
                     file, combinedName, combinedPrefix, recursedCount, out );
               }
               else
               {
                  ossimNotify(ossimNotifyLevel_WARN)
                     << "ossim_hdf5::printIterative WARNING!"
                     << "\nMax iterations reached!" << std::endl;
               }
            }
            else if ( objType == H5G_DATASET )
            {
               printObject( file, combinedName, combinedPrefix, out );
            }
            else
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "ossim_hdf5::printIterative WARNING!"
                  << "\nUnhandled object type: " << objType << std::endl;
            }
         }
      }
      
      group->close();
      delete group;
      group = 0;
      --recursedCount;
      
   } // Matches: if ( file )
   
} // End: void ossim_hdf5::printIterative method.

void ossim_hdf5::printObject(  H5::H5File* file,
                               const std::string& objectName,
                               const std::string& prefix,
                               std::ostream& out )
{
#if 0
   std::cout << "printObject entered..."
             << "\nobjectName: " << objectName
             << "\nprefix: " << prefix
             << std::endl;
#endif
   
   H5::DataSet dataset = file->openDataSet( objectName );
   
   // Get the class of the datatype that is used by the dataset.
   H5T_class_t type_class = dataset.getTypeClass();
   out << prefix << ".class_type: "
       << ossim_hdf5::getDatatypeClassType( type_class ) << std::endl;

   const ossim_uint32 ATTRS_COUNT = dataset.getNumAttrs();
   for ( ossim_uint32 aIdx = 0; aIdx < ATTRS_COUNT; ++aIdx )
   {
      H5::Attribute attr = dataset.openAttribute( aIdx );
      ossim_hdf5::printAttribute( attr, prefix, out );
      attr.close();
   }

   // Extents:
   std::vector<ossim_uint32> extents;
   ossim_hdf5::getExtents( &dataset, extents );
   for ( ossim_uint32 i = 0; i < extents.size(); ++i )
   {
      ossimString os;
      std::string exStr = ".extent";
      exStr += os.toString(i).string();
      out << prefix << exStr << ": " << extents[i] << std::endl;
   }

   // ossimScalarType scalar = getScalarType( type_class, dataset.getId() );
   ossimScalarType scalar = ossim_hdf5::getScalarType( dataset.getId() );
   if ( scalar != OSSIM_SCALAR_UNKNOWN)
   {
      out << prefix << "." << ossimKeywordNames::SCALAR_TYPE_KW << ": "
          << ossimScalarTypeLut::instance()->getEntryString( scalar ) << std::endl;

      if ( ossim::scalarSizeInBytes( scalar ) > 1 )
      {
         ossimByteOrder byteOrder = ossim_hdf5::getByteOrder( &dataset );
         std::string byteOrderString = "little_endian";
         if ( byteOrder == OSSIM_BIG_ENDIAN )
         {
            byteOrderString = "big_endian";
         }
         out << prefix << "." <<ossimKeywordNames::BYTE_ORDER_KW << ": "
             << byteOrderString << std::endl;
      }
   }

#if 0
   // Attributes:
   int numberOfAttrs = dataset.getNumAttrs();
   cout << "numberOfAttrs: " << numberOfAttrs << endl;
   for ( ossim_int32 attrIdx = 0; attrIdx < numberOfAttrs; ++attrIdx )
   {
      H5::Attribute attribute = dataset.openAttribute( attrIdx );
      cout << "attribute.from class: " << attribute.fromClass() << endl;
   }
#endif
   dataset.close();
   
} // End: printObject

bool ossim_hdf5::getGroupAttributeValue( H5::H5File* file,
                                         const std::string& group,
                                         const std::string& key,
                                         std::string& value )
{
   static const char MODULE[] = "ossim_hdf5::getGroupAttributeValue";
   
   bool result = false;
   
   if (  file )
   {
      try // HDF5 library throws exceptions so wrap with try{}catch...
      {
         // Open the root group:
         H5::Group* h5Group = new H5::Group( file->openGroup( group ) );
         
         // Lookw for key:
         H5::Attribute attr      = h5Group->openAttribute( key );
         std::string   name      = attr.getName();
         H5::DataType  type      = attr.getDataType();
         H5T_class_t   typeClass = attr.getTypeClass();
         
         if ( ( name == key ) && ( typeClass == H5T_STRING ) )
         {
            attr.read( type, value );
            result = true;
         }

         // Cleanup:
         attr.close();
         h5Group->close();
         delete h5Group;
         h5Group = 0;
      }
      catch( const H5::Exception& e )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " WARNING: Caught exception!\n"
            << e.getDetailMsg() << std::endl;
      }
      catch( ... )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " WARNING: Caught unknown exception!" << std::endl;
      }      
   }

   return result;
   
} // End: ossim_hdf5::getGroupAttributeValue

bool ossim_hdf5::getDatasetAttributeValue( H5::H5File* file,
                                           const std::string& objectName,
                                           const std::string& key,
                                           std::string& value )
{
   static const char MODULE[] = "ossim_hdf5::getDatasetAttributeValue";

   bool result = false;
   
   if (  file )
   {
      try // HDF5 library throws exceptions so wrap with try{}catch...
      {
         // Open the dataset:
         H5::DataSet dataset = file->openDataSet( objectName );
         
         // Lookw for key:
         H5::Attribute attr = dataset.openAttribute( key );

         std::string  name = attr.getName();
         H5::DataType type = attr.getDataType();
         H5T_class_t  typeClass = attr.getTypeClass();
         
         if ( ( name == key ) && ( typeClass == H5T_STRING ) )
         {
            attr.read( type, value );
            result = true;
         }

         // Cleanup:
         attr.close();
         dataset.close();
      }
      catch( const H5::Exception& e )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " WARNING: Caught exception!\n"
            << e.getDetailMsg() << std::endl;
      }
      catch( ... )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE << " WARNING: Caught unknown exception!" << std::endl;
      }      
   }

   return result;
   
} // End: ossim_hdf5::getDatasetAttributeValue

void ossim_hdf5::printAttribute( const H5::Attribute& attr,
                                 const std::string& prefix,
                                 std::ostream& out )
{
   std::string  name      = attr.getName();
   H5::DataType type      = attr.getDataType();
   H5T_class_t  typeClass = attr.getTypeClass();
   size_t       size      = type.getSize();
   
   std::string  value; // Initialized below.

   if ( ( typeClass == H5T_INTEGER ) || ( typeClass == H5T_FLOAT ) )
   {
      H5::IntType intType = attr.getIntType();

      ossimScalarType scalar = ossim_hdf5::getScalarType( intType.getId() );
      
      ossimByteOrder order = ossim_hdf5::getByteOrder( &attr );
      ossimEndian* endian = 0;
      if ( ( size > 1 ) && ( order != ossim::byteOrder() ) )
      {
         endian = new ossimEndian(); // If set used as flag to byte swap.
      }
      
      if ( typeClass == H5T_INTEGER )
      {
         switch ( scalar )
         {
            case OSSIM_UINT8:
            {
               if ( size == 1 )
               {
                  ossim_uint8 i;
                  attr.read( type, (void*)&i );
                  value = ossimString::toString( ossim_int32(i) ).string();
               }
               break;
            }
            case OSSIM_SINT8:
            {
               if ( size == 1 )
               {
                  ossim_sint8 i;
                  attr.read( type, (void*)&i );
                  value = ossimString::toString( ossim_int32(i) ).string();
               }
               break;
            }
            case OSSIM_UINT16:            
            {
               if ( size == 2 )
               {
                  ossim_uint16 i;
                  attr.read( type, (void*)&i );
                  if ( endian )
                  {
                     endian->swap( i );
                  }  
                  value = ossimString::toString( i ).string();
               }
               break;
            }
            case OSSIM_SINT16:
            {
               if ( size == 2 )
               {
                  ossim_sint16 i;
                  attr.read( type, (void*)&i );
                  if ( endian )
                  {
                     endian->swap( i );
                  }
                  value = ossimString::toString( i ).string();
               }
               break;
            }
            case OSSIM_UINT32:        
            {
               if ( size == 4 )
               {
                  ossim_uint32 i;
                  attr.read( type, (void*)&i );
                  if ( endian )
                  {
                     endian->swap( i );
                  }  
                  value = ossimString::toString( i ).string();
               }
               break;
            }
            case OSSIM_SINT32:
            {
               if ( size == 4 )
               {
                  ossim_sint32 i;
                  attr.read( type, (void*)&i );
                  if ( endian )
                  {
                     endian->swap( i );
                  }
                  value = ossimString::toString( i ).string();
               }
               break;
            }
            case OSSIM_UINT64:        
            {
               if ( size == 8 )
               {
                  ossim_uint64 i;
                  attr.read( type, (void*)&i );
                  if ( endian )
                  {
                     endian->swap( i );
                  }  
                  value = ossimString::toString( i ).string();
               }
               break;
            }
            case OSSIM_SINT64:
            {
               if ( size == 8 )
               {
                  ossim_sint32 i;
                  attr.read( type, (void*)&i );
                  if ( endian )
                  {
                     endian->swap( i );
                  }
                  value = ossimString::toString( i ).string();
               }
               break;
            }
            default:
               break;
         }
      }
      else if ( typeClass == H5T_FLOAT )
      {
         if ( scalar == OSSIM_FLOAT32 )
         {
            if ( size == 4 )
            {
               ossim_float32 f;
               attr.read( type, (void*)&f );
               if ( endian )
               {
                  endian->swap( f );
               }
               value = ossimString::toString( f ).string();
            }
         }
         if ( scalar == OSSIM_FLOAT64 )
         {
            if ( size == 8 )
            {
               ossim_float64 f;
               attr.read( type, (void*)&f );
               if ( endian )
               {
                  endian->swap( f );
               }
               value = ossimString::toString( f ).string();
            }
         }
      }

      if ( endian )
      {
         delete endian;
         endian = 0;
      }
   }
   else if ( typeClass == H5T_STRING )
   {
      attr.read( type, value );
   }
   else
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimH5Util::printAttribute WARN: Unhandled type class: " << typeClass
         << std::endl;
   }

   out << prefix << "." << name << ": " << value << std::endl;
   
} // End: ossim_hdf5::printAttribute

void ossim_hdf5::combine( const std::string& left,
                          const std::string& right,
                          char separator,
                          std::string& result )
{
   if ( left.size() && right.size() )
   {
      result = left;
      if ( ( left[ left.size()-1 ] != separator ) && ( right[0] != separator ) )
      {
         result.push_back(separator);
      }
      result += right;
   }
}

void ossim_hdf5::getDatasetNames(H5::H5File* file,
                                 std::vector<std::string>& datasetNames )
{
   datasetNames.clear();
   
   if ( file )
   {
      std::string groupName = "/";
      ossim_uint32 recurseCount = 0;
      ossim_hdf5::iterateGroupForDatasetNames( file, groupName, datasetNames, recurseCount );
#if 0
      std::vector<std::string>::const_iterator i = datasetNames.begin();
      while ( i != datasetNames.end() )
      {
         std::cout << "dataset: " << (*i) << std::endl;
         ++i;
      }
#endif
   }
}

std::string ossim_hdf5::getDatatypeClassType( ossim_int32 type )
{
   H5T_class_t classType = (H5T_class_t)type;
   
   std::string result;
   switch ( classType )
   {
      case H5T_INTEGER:
         result = "H5T_INTEGER";
         break;
      case H5T_FLOAT:
         result = "H5T_FLOAT";
         break;         
      case H5T_TIME:
         result = "H5T_TIME";
         break;
      case H5T_STRING:
         result = "H5T_STRING";
         break;
      case H5T_BITFIELD:
         result = "H5T_BITFIELD";
         break;
      case H5T_OPAQUE:
         result = "H5T_OPAQUE";
         break;         
      case H5T_COMPOUND:
         result = "H5T_COMPOUND";
         break;
      case H5T_REFERENCE:
         result = "H5T_REFERENCE";
         break;         
      case H5T_ENUM:
         result = "H5T_ENUM";
         break;
      case H5T_VLEN:
         result = "H5T_VLEN";
         break;         
      case H5T_ARRAY:
         result = "H5T_ARRAY";
         break;
      case H5T_NO_CLASS:
      default:
         result = "H5T_ARRAY";
         break;
   }
   return result;
}

bool ossim_hdf5::isLoadableAsImage( H5::H5File* file, const std::string& datasetName )
{
   bool result = false;

   // std::cout << "isLoadable entered..." << std::endl;
   if ( file && datasetName.size() )
   {
      if ( isExcludedDataset( datasetName ) == false )
      {
         H5::DataSet dataset = file->openDataSet( datasetName );
         
         // Get the class of the datatype that is used by the dataset.
         H5T_class_t type_class = dataset.getTypeClass();
         // std::cout << "Class type: " << ossim_hdf5::getDatatypeClassType( type_class )
         // << std::endl;
         
         if ( ( type_class == H5T_INTEGER ) || ( type_class == H5T_FLOAT ) )
         {
            // Get dataspace of the dataset.
            // H5::DataSpace dataspace = dataset.getSpace();
            
            // Get the extents:
            std::vector<ossim_uint32> extents;
            ossim_hdf5::getExtents( &dataset, extents );
            
            if ( extents.size() >= 2 )
            {
               if ( ( extents[0] > 1 ) && ( extents[1] > 1 ) )
               {
                  // std::cout << "Accepting dataset: " << datasetName << std::endl;
                  result = true;
               }     
            }
            
         }
         
         dataset.close();
      }
   }
      
   // std::cout << "isLoadable exit status: " << (result?"true":"false") << std::endl;
   
   return result;
}

bool ossim_hdf5::isExcludedDataset( const std::string& datasetName )
{
   bool result = false;
   
   ossimFilename f = datasetName;
   f = f.file();
#if 1
   if ( f != "Radiance" )
   {
      result = true;
   }
#endif
   
#if 0
   if ( ( f == "Latitude" )        ||
        ( f == "Longitude" )       ||
        ( f == "SCAttitude" )      ||
        ( f == "SCPosition" )      ||
        ( f == "SCVelocity" ) )
   {
      result = true;
   }
#endif
   
   return result;
}

void ossim_hdf5::iterateGroupForDatasetNames(  H5::H5File* file,
                                               const std::string& groupName,
                                               std::vector<std::string>& datasetNames,
                                               ossim_uint32& recursedCount )
{
   if ( file && groupName.size() )
   {
      ++recursedCount;
      
      // std::cout << "iterateGroup: " << groupName << std::endl;
      
      H5::Group* group = new H5::Group( file->openGroup(groupName) );
      
      const hsize_t OBJ_COUNT = group->getNumObjs();
      
      for ( hsize_t i = 0; i < OBJ_COUNT; ++i )
      {
         std::string objName = group->getObjnameByIdx(i);

         if ( objName.size() )
         {
            char separator = '/';
            std::string combinedName;
            combine( groupName, objName, separator, combinedName );
            
            H5G_obj_t objType   = group->getObjTypeByIdx(i);

#if 0
            std::cout << "combinedName: " << combinedName
                      << "\ngetObjnameByIdx[" << i << "]: " << objName
                      << "\ngetObjTypeByIdx[" << i << "]: " << objType
                      << std::endl;
#endif

            if ( objType == H5G_GROUP )
            {
               // Recursive call:
               if ( recursedCount < ossim_hdf5::MAX_RECURSION_LEVEL )
               {
                  ossim_hdf5::iterateGroupForDatasetNames(
                     file, combinedName, datasetNames, recursedCount );
               }
               else
               {
                  ossimNotify(ossimNotifyLevel_WARN)
                     << "ossim_hdf5::iterateGroupForDatasetNames WARNING!"
                     << "\nMax iterations reached!" << std::endl;
               }
            }
            else if ( objType == H5G_DATASET )
            {
               datasetNames.push_back( combinedName );
            }
            else
            {
               ossimNotify(ossimNotifyLevel_WARN)
                     << "ossim_hdf5::iterateGroupForDatasetNames WARNING!"
                     << "\nUnhandled object type: " << objType << std::endl;
            }
         }
      }
      
      group->close();
      delete group;
      group = 0;
      --recursedCount;
      
   } // Matches: if ( file )
   
} // End: void ossim_hdf5::iterateGroupForDatasetNames

void ossim_hdf5::getExtents( const H5::DataSet* dataset,
                             std::vector<ossim_uint32>& extents )
{
   extents.clear();
   
   if ( dataset )
   {
      // Get dataspace of the dataset.
      H5::DataSpace dataspace = dataset->getSpace();
      
      // Number of dimensions:
      int ndims = dataspace.getSimpleExtentNdims();
      if ( ndims )
      {
         //hsize_t dims_out[ndims];
         std::vector<hsize_t> dims_out(ndims);
         dataspace.getSimpleExtentDims( &dims_out.front(), 0 );
         for ( ossim_int32 i = 0; i < ndims; ++i )
         {
            extents.push_back(static_cast<ossim_uint32>(dims_out[i]));
         }
      }

      dataspace.close();
   }
}

ossimScalarType ossim_hdf5::getScalarType( const H5::DataSet* dataset )
{
   // cout << "typeClass: " << typeClass << "\nid: " << id << endl;

   ossimScalarType scalar = OSSIM_SCALAR_UNKNOWN;

   if ( dataset )
   {
      ossim_int32 typeClass = dataset->getTypeClass();
      
      if ( ( typeClass == H5T_INTEGER ) || ( typeClass == H5T_FLOAT ) )
      {
         // hid_t mem_type_id = H5Dget_type(id);
         hid_t mem_type_id = H5Dget_type( dataset->getId() );
         
         // cout << "mem_type_id: " << mem_type_id << endl;
         
         if( mem_type_id > -1 )
         {
            hid_t native_type = H5Tget_native_type(mem_type_id, H5T_DIR_DEFAULT);
            // hid_t native_type = H5Tget_native_type(id, H5T_DIR_DEFAULT);
            
            if( H5Tequal(H5T_NATIVE_CHAR, native_type) )
            {
               scalar = OSSIM_SINT8;
            }
            else if ( H5Tequal( H5T_NATIVE_UCHAR, native_type) )
            {
               scalar = OSSIM_UINT8;
            }
            else if( H5Tequal( H5T_NATIVE_SHORT, native_type) ) 
            {
               scalar = OSSIM_SINT16;
            }
            else if(H5Tequal(H5T_NATIVE_USHORT, native_type)) 
            {
               scalar = OSSIM_UINT16;
            }
            else if(H5Tequal( H5T_NATIVE_INT, native_type)) 
            {
               scalar = OSSIM_SINT32;
            }
            else if(H5Tequal( H5T_NATIVE_UINT, native_type ) ) 
            {
               scalar = OSSIM_UINT32;
            }
            else if(H5Tequal( H5T_NATIVE_LONG, native_type)) 
            {
               scalar = OSSIM_SINT32;
            }
            else if(H5Tequal( H5T_NATIVE_ULONG, native_type))
            {
               scalar = OSSIM_UINT32;
            }
            else if(H5Tequal( H5T_NATIVE_LLONG, native_type)) 
            {
               scalar = OSSIM_SINT64;
            }
            else if(H5Tequal( H5T_NATIVE_ULLONG, native_type))
            {
               scalar = OSSIM_UINT64;
            }
            else if(H5Tequal( H5T_NATIVE_FLOAT, native_type)) 
            {
               scalar = OSSIM_FLOAT32;
            }
            else if(H5Tequal( H5T_NATIVE_DOUBLE, native_type)) 
            {
               scalar = OSSIM_FLOAT64;
            }
         }
         
      } // Matches: if ( ( typeClass == ...
      
   } // Matches: if ( dataset ){
   
   return scalar;
   
} // End: ossim_hdf5::getScalarType( const H5::DataSet* dataset )

ossimScalarType ossim_hdf5::getScalarType( ossim_int32 id )
{
   ossimScalarType scalar = OSSIM_SCALAR_UNKNOWN;
   
   H5T_class_t type_class = H5Tget_class(id);
   size_t      size       = H5Tget_size(id);
   H5T_sign_t  sign       = H5Tget_sign(id);
   
   if ( type_class == H5T_INTEGER )
   {
      if ( size == 1 && sign == H5T_SGN_2)
      {
         scalar = OSSIM_SINT8;
      }
      else if ( size == 2 && sign == H5T_SGN_2)
      {
         scalar = OSSIM_SINT16;
      }
      else if ( size == 4 && sign == H5T_SGN_2)
      {
         scalar = OSSIM_SINT32;
      }
      else if ( size == 8 && sign == H5T_SGN_2)
      {
         scalar = OSSIM_SINT64;
      }
      else if ( size == 1 && sign == H5T_SGN_NONE)
      {
         scalar = OSSIM_UINT8;
      }
      else if ( size == 2 && sign == H5T_SGN_NONE)
      {
         scalar = OSSIM_UINT16;
      }
      else if ( size == 4 && sign == H5T_SGN_NONE)
      {
         scalar = OSSIM_UINT32;
      }
      else if ( size == 8 && sign == H5T_SGN_NONE)
      {
         scalar = OSSIM_UINT64;
      }
   }
   else if ( type_class == H5T_FLOAT )
   {
      if ( size == 4)
      {
         scalar = OSSIM_FLOAT32;
      }
      else if ( size == 8)
      {
         scalar = OSSIM_FLOAT64;
      }
   }
   
   return scalar;
}

ossimByteOrder ossim_hdf5::getByteOrder( const H5::AbstractDs* obj )
{
   ossimByteOrder byteOrder = ossim::byteOrder();
   if ( obj )
   {
      // Get the class of the datatype that is used by the dataset.
      H5T_class_t typeClass = obj->getTypeClass();

      H5T_order_t order = H5T_ORDER_NONE;
      
      if ( typeClass == H5T_INTEGER )
      {
         H5::IntType intype = obj->getIntType();
         order = intype.getOrder();
      }
      else if ( typeClass == H5T_FLOAT )
      {
         H5::FloatType floatType = obj->getFloatType();
         order = floatType.getOrder(); 
      }
      
      if ( order == H5T_ORDER_LE )
      {
         byteOrder = OSSIM_LITTLE_ENDIAN;
      }
      else if ( order == H5T_ORDER_BE )
      {
         byteOrder = OSSIM_BIG_ENDIAN;
      }
   }
   return byteOrder;
}

bool ossim_hdf5::getBoundingRect( H5::DataSet& dataset, ossimIrect& rect )
{
   bool result = false;
   
   H5::DataSpace dataspace = dataset.getSpace();
         
   // Number of dimensions of the input dataspace:
   const ossim_int32 DIM_COUNT = dataspace.getSimpleExtentNdims();
         
   if ( DIM_COUNT == 2 )
   {
      // Get the extents. Assuming dimensions are same for lat lon dataset. 
      std::vector<hsize_t> dimsOut(DIM_COUNT);
      dataspace.getSimpleExtentDims( &dimsOut.front(), 0 );

      if ( dimsOut[0] && dimsOut[1] )
      {
         //---
         // Capture the rectangle:
         // dimsOut[0] is height, dimsOut[1] is width:
         //---
         rect = ossimIrect( 0, 0,
                            static_cast<ossim_int32>( dimsOut[1]-1 ),
                            static_cast<ossim_int32>( dimsOut[0]-1 ) );
               
         std::vector<hsize_t> inputCount(DIM_COUNT);
         std::vector<hsize_t> inputOffset(DIM_COUNT);
         
         inputOffset[0] = 0;
         inputOffset[1] = 0;
         
         inputCount[0] = 1;
         inputCount[1] = 1;
         
         // Output dataspace dimensions.
         const ossim_int32 OUT_DIM_COUNT = 3;
         std::vector<hsize_t> outputCount(OUT_DIM_COUNT);
         outputCount[0] = 1; // single band
         outputCount[1] = 1; // single line
         outputCount[2] = 1; // single sample
               
         // Output dataspace offset.
         std::vector<hsize_t> outputOffset(OUT_DIM_COUNT);
         outputOffset[0] = 0;
         outputOffset[1] = 0;
         outputOffset[2] = 0;
               
         ossimScalarType scalar = ossim_hdf5::getScalarType( &dataset );
         if ( scalar == OSSIM_FLOAT32 )
         {
            // See if we need to swap bytes:
            ossimEndian* endian = 0;
            if ( ( ossim::byteOrder() != ossim_hdf5::getByteOrder( &dataset ) ) )
            {
               endian = new ossimEndian();
            }

            // Native type:
            H5::DataType datatype = dataset.getDataType();
                  
            // Output dataspace always the same one pixel.
            H5::DataSpace bufferDataSpace( OUT_DIM_COUNT, &outputCount.front());
            bufferDataSpace.selectHyperslab( H5S_SELECT_SET,
                                             &outputCount.front(),
                                             &outputOffset.front() );

            //---
            // Dataset sample has NULL lines at the end so scan for valid rect.
            // Use "<= -999" for test as per NOAA as it seems the NULL value is
            // fuzzy.  e.g. -999.3.
            //---
            const ossim_float32 NULL_VALUE = -999.0;
            ossim_float32 value = 0.0;

            // Find the ul pixel:
            ossimIpt ulIpt = rect.ul();
            bool found = false;
                  
            // Line loop to find upper left pixel:
            while ( ulIpt.y <= rect.lr().y )
            {
               inputOffset[0] = static_cast<hsize_t>(ulIpt.y);
                     
               // Sample loop:
               ulIpt.x = rect.ul().x;
               while ( ulIpt.x <= rect.lr().x )
               {
                  inputOffset[1] = static_cast<hsize_t>(ulIpt.x);
                        
                  dataspace.selectHyperslab( H5S_SELECT_SET,
                                                &inputCount.front(),
                                                &inputOffset.front() );
                        
                  // Read data from file into the buffer.
                  dataset.read( &value, datatype, bufferDataSpace, dataspace );
                        
                  if ( endian )
                  {
                     // If the endian pointer is initialized(not zero) swap the bytes.
                     endian->swap( value );
                  }
                        
                  if ( value > NULL_VALUE )
                  {
                     found = true;
                     break;
                  }
                  ++ulIpt.x;
                     
               } // End: sample loop
                     
               if ( found )
               {
                  break;
               }

               ++ulIpt.y;
                     
            } // End line loop to find ul pixel:

            // Find the lower right pixel:
            ossimIpt lrIpt = rect.lr();
            found = false;
                  
            // Line loop to find last pixel:
            while ( lrIpt.y >= rect.ul().y )
            {
               inputOffset[0] = static_cast<hsize_t>(lrIpt.y);
                     
               // Sample loop:
               lrIpt.x = rect.lr().x;
               while ( lrIpt.x >= rect.ul().x )
               {
                  inputOffset[1] = static_cast<hsize_t>(lrIpt.x);
                        
                  dataspace.selectHyperslab( H5S_SELECT_SET,
                                             &inputCount.front(),
                                             &inputOffset.front() );
                        
                  // Read data from file into the buffer.
                  dataset.read( &value, datatype, bufferDataSpace, dataspace );
                        
                  if ( endian )
                  {
                     // If the endian pointer is initialized(not zero) swap the bytes.
                     endian->swap( value );
                  }
                  
                  if ( value > NULL_VALUE )
                  {
                     found = true;
                     break;
                  }
                  --lrIpt.x;
                     
               } // End: sample loop
                     
               if ( found )
               {
                  break;
               }

               --lrIpt.y;
                     
            } // End line loop to find lower right pixel.

            rect = ossimIrect( ulIpt, lrIpt );

            result = true;

            //cout << "ossim_hdf5::getBoundingRect rect: " << rect << endl;
            
         } 
         else // Matches: if ( scalar == OSSIM_FLOAT32 ){...}
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossim_hdf5::getBoundingRect WARNING!"
               << "\nUnhandled scalar type: "
               << ossimScalarTypeLut::instance()->getEntryString( scalar )
               << std::endl;
         }
               
      } // Matches: if ( dimsOut...
            
   } // Matches: if ( IN_DIM_COUNT == 2 )
         
   dataspace.close();

   return result;
   
} // End: ossim_hdf5::getBoundingRect(...)

