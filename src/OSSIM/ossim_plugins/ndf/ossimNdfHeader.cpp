//***
// Written by: Kenneth Melero (kmelero@sanz.com)
// Description: This class parses header files in National Land Archive Processing System 
// Data Format (NDF). It currently supports Landsat MSS, TM, and ETM+ sensors.
//
// $Id$
// ***

#include <ossimNdfHeader.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimTrace.h>

#include <iostream>
#include <cstdlib> /* for abs */

static ossimTrace traceDebug("ossimNdfTileSource:debug");

ossimNdfHeader::ossimNdfHeader(const char* file)
   :
   m_file(file),
   m_kwl(),
   m_geom(),
   m_revision(""),
   m_projection(""),
   m_utmzone(-1),
   m_datum(""),
   m_lines(-1),
   m_samples(-1),
   m_ul(0.0,0.0),
   m_ur(0.0,0.0),
   m_lr(0.0,0.0),
   m_ll(0.0,0.0),
   m_gsd(-1.0),
   m_satellite(""),
   m_instrument(""),
   m_dataset_type(""),
   m_acqdate(""),
   m_interleave(""),
   m_proc_level(-1),
   m_num_bands(-1),
   m_sun_elev(-1.0),
   m_sun_azim(-1.0),
   m_wrs(""),
   m_path(-1),
   m_row(-1)
{
   static const char MODULE[] = "ossimNdfHeader::ossimNdfHeader";
   
   // Set the delimeter to '=' as the default is ':'
   m_kwl.change_delimiter('=');

   // Ensure header file exists
   if(!m_file.exists())
   {
      std::string errMsg = MODULE;
      errMsg += " ERROR: Missing Header File ";
      errMsg += m_file.string();
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      throw ossimException(errMsg);
   }
   
   // Load and parse header with a keywordlist object.
   if ( m_kwl.addFile(m_file) == false )
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
      std::string errMsg = MODULE;
      errMsg += " keyword list error.";
      errMsg += m_file.string();
      throw ossimException(errMsg);
   }

   // Quickly check if file is supported.  
   if(!isSupportedNlaps())
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;      
      std::string errMsg = MODULE;
      errMsg += " unsupported";
      errMsg += m_file.string();
      throw ossimException(errMsg);
   }
   
   // Parse NDF header and further validate.
   parseHeader();
}

ossimNdfHeader::~ossimNdfHeader()
{
}

void ossimNdfHeader::parseHeader()
{
   const char* lookup;
   
   // NDF Revision
   lookup = m_kwl.find("NDF_REVISION");
   if(lookup)
   {
      m_revision = lookup;
      m_revision.trim(";");
      
      m_geom.add("ndf_revision", m_revision, true);
   }
   
   // Projection
   // Support: UTM
   // Available: Data comes in as UTM, SOM, and Albers
   lookup = m_kwl.find("MAP_PROJECTION_NAME");
   if(lookup)
   {
      ossimString tmp;
      tmp = lookup;
      tmp.trim(";");
      
      if(tmp == ossimString("UTM"))
      {
         m_projection = "ossimUtmProjection";
      }
      else
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         return;
      }
      
      m_geom.add(ossimKeywordNames::TYPE_KW, m_projection, true);
   }
   
   // UTM Zone
   lookup = m_kwl.find("USGS_MAP_ZONE");
   if(lookup)
   {
      ossimString tmp;
      tmp = lookup;
      tmp.trim(";");
      m_utmzone = tmp.toInt();
      
      if(m_utmzone > 0)
      {
         m_geom.add(ossimKeywordNames::ZONE_KW, m_utmzone, true);
         m_geom.add(ossimKeywordNames::HEMISPHERE_KW, "N", true);
      }
      
      if(m_utmzone < 0)
      {
         m_geom.add(ossimKeywordNames::ZONE_KW, std::abs(m_utmzone), true);
         m_geom.add(ossimKeywordNames::HEMISPHERE_KW, "S", true);
      }
   }
   
   // Datum
   // Support: NAD83, NAD27, WGS84
   // Available: NAD83, NAD27, WGS84, Ellipsoid
   lookup = m_kwl.find("HORIZONTAL_DATUM");
   if(lookup)
   {
      ossimString tmp;
      tmp = lookup;
      tmp.trim(";");
      
      if(tmp == ossimString("NAD83"))
      {
         m_datum = "NAR-C";
      }
      else if(tmp == ossimString("NAD27"))
      {
         m_datum = "NAS-C";
      }
      else if(tmp == ossimString("WGS84"))
      {
         m_datum = "WGE";
      }
      else
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         return;
      }		 
      
      m_geom.add(ossimKeywordNames::DATUM_KW, m_datum, true);
   }
   
   // Lines
   lookup = m_kwl.find("LINES_PER_DATA_FILE");
   if(lookup)
   {
      ossimString tmp;
      tmp = lookup;
      tmp.trim(";");
      m_lines = tmp.toInt64();
      
      m_geom.add(ossimKeywordNames::NUMBER_LINES_KW,
                 tmp, true);
   }
   
   // Samples
   lookup = m_kwl.find("PIXELS_PER_LINE");
   if(lookup)
   {
      ossimString tmp;
      tmp = lookup;
      tmp.trim(";");
      m_samples = tmp.toInt64();
      
      m_geom.add(ossimKeywordNames::NUMBER_SAMPLES_KW,
                 tmp, true);
   }
   
   // Ground Sample Distance
   lookup = m_kwl.find("PIXEL_SPACING");
   if(lookup)
   {
      ossimString tmp;
      tmp = lookup;
      tmp.trim(";");
      m_gsd = tmp.toDouble();
      
      m_geom.add(ossimKeywordNames::METERS_PER_PIXEL_X_KW,
                 m_gsd, true);
      m_geom.add(ossimKeywordNames::METERS_PER_PIXEL_Y_KW,
                 m_gsd, true);
   }
   
   // Bands
   lookup = m_kwl.find("NUMBER_OF_BANDS_IN_VOLUME");
   if(lookup)
   {
      ossimString tmp;
      tmp = lookup;
      tmp.trim(";");
      m_num_bands = tmp.toInt();
      
      m_geom.add(ossimKeywordNames::NUMBER_BANDS_KW,
                 m_num_bands, true);
   }
   
   // Satellite
   lookup = m_kwl.find("SATELLITE");
   if(lookup)
   {
      m_satellite = lookup;
      m_satellite.trim(";");
      
      m_geom.add("satellite", m_satellite, true);
   }
   
   // Instrument
   lookup = m_kwl.find("SATELLITE_INSTRUMENT");
   if(lookup)
   {
      m_instrument = lookup;
      m_instrument.trim(";");
      
      if(m_instrument == ossimString("MSS") || m_instrument == ossimString("TM") || m_instrument == ossimString("ETM+"))
      {
         // Supported instrument
      }
      else
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         return;
      }
      
      m_geom.add("instrument", m_instrument, true);
   }
   
   // Dataset Type
   lookup = m_kwl.find("DATA_SET_TYPE");
   if(lookup)
   {
      m_dataset_type = lookup;
      m_dataset_type.trim(";");
      
      m_geom.add("dataset_type", m_dataset_type, true);
   }
   
   // Acquisition Date
   lookup = m_kwl.find("ACQUISITION_DATE/TIME");
   if(lookup)
   {
      ossimString tmp;
      tmp = lookup;
      tmp.trim(";");
      
      if(m_instrument == ossimString("MSS"))
      {
         // UTC date and time of acquisition of reference point in ISO compliant format: YYYY-MM-DDThh:mm:ssZ
         m_acqdate = tmp.beforePos(10);			
      }
      else if (m_instrument == ossimString("TM"))
      {
         m_acqdate = tmp.beforePos(6);			
      }
      else if (m_instrument == ossimString("ETM+"))
      {
         m_acqdate = tmp.beforePos(10);			
      }
      
      m_geom.add("acquisition_date",
                 m_acqdate, true);
   }
   
   // Interleave
   lookup = m_kwl.find("DATA_FILE_INTERLEAVING");
   if(lookup)
   {
      m_interleave = lookup;
      m_interleave.trim(";");
      
      m_geom.add(ossimKeywordNames::INTERLEAVE_TYPE_KW,
                 m_interleave, true);
   }
   
   // Processing Level
   // Support:  8, 9, or 10
   // Available: 
   // - 0: Raw, Uncorrected
   // - 1: Radiometrically Calibrated, Uncorrected
   // - 8: Systematically Corrected, Map Projected
   // - 9: Precision Corrected, Map Projected
   // - 10: Terrain Corrected, Map Projected
   lookup = m_kwl.find("PROCESSING_LEVEL");
   if(lookup)
   {
      ossimString tmp;
      tmp = lookup;
      tmp.trim(";");
      
      if(tmp.contains("8") || tmp.contains("9") || tmp.contains("10"))
      {
         m_proc_level = tmp.toInt();
      }
      else
      {
         theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
         return;
      }
      
      m_geom.add("processing_level", m_proc_level, true);
   }
   
   // Sun Elevation
   lookup = m_kwl.find("SUN_ELEVATION");
   if(lookup)
   {
      ossimString tmp;
      tmp = lookup;
      tmp.trim(";");
      
      m_sun_elev = tmp.toDouble();
      
      m_geom.add("sun_elevation",
                 m_sun_elev, true);
   }
   
   // Sun Azimuth
   lookup = m_kwl.find("SUN_AZIMUTH");
   if(lookup)
   {
      ossimString tmp;
      tmp = lookup;
      tmp.trim(";");
      
      m_sun_azim = tmp.toDouble();
      
      m_geom.add("sun_azimuth",
                 m_sun_azim, true);
   }
   
   // WRS (Path and Row)
   lookup = m_kwl.find("WRS");
   if(lookup)
   {
      ossimString tmp;
      tmp = lookup;
      tmp.trim(";");
      
      m_wrs = tmp;
      m_path = tmp.before(ossimString("/")).toInt();
      m_row = tmp.after(ossimString("/")).toInt();
      
      m_geom.add("wrs", m_wrs, true);
      m_geom.add("path", m_path, true);
      m_geom.add("row", m_row, true);
   }
   
   // Upper Left Image Footprint
   lookup = m_kwl.find("UPPER_LEFT_CORNER");
   if(lookup)
   {
      ossimString tmp;
      tmp = lookup;
      tmp.trim(";");
      
      std::vector<ossimString> tmp_vec;
      tmp_vec = tmp.explode(ossimString(","));
      
      m_ul = ossimDpt(tmp_vec[2].toDouble(),
                      tmp_vec[3].toDouble());
      
      m_geom.add(ossimKeywordNames::TIE_POINT_EASTING_KW,
                 m_ul.x, true);
      m_geom.add(ossimKeywordNames::TIE_POINT_NORTHING_KW,
                 m_ul.y, true);
      
      m_geom.add("hdr_ul_easting", m_ul.x, true);
      m_geom.add("hdr_ul_northing", m_ul.y, true);
   }
   
   // Upper Right Image Footprint
   lookup = m_kwl.find("UPPER_RIGHT_CORNER");
   if(lookup)
   {
      ossimString tmp;
      tmp = lookup;
      tmp.trim(";");
      
      std::vector<ossimString> tmp_vec;
      tmp_vec = tmp.explode(ossimString(","));
      
      m_ur = ossimDpt(tmp_vec[2].toDouble(),
                      tmp_vec[3].toDouble());
      
      m_geom.add("hdr_ur_easting", m_ur.x, true);
      m_geom.add("hdr_ur_northing", m_ur.y, true);
   }
   
   // Lower Right Image Footprint
   lookup = m_kwl.find("LOWER_RIGHT_CORNER");
   if(lookup)
   {
      ossimString tmp;
      tmp = lookup;
      tmp.trim(";");
      
      std::vector<ossimString> tmp_vec;
      tmp_vec = tmp.explode(ossimString(","));
      
      m_lr = ossimDpt(tmp_vec[2].toDouble(),
                      tmp_vec[3].toDouble());
      
      m_geom.add("hdr_lr_easting", m_lr.x, true);
      m_geom.add("hdr_lr_northing", m_lr.y, true);
      
   }
   
   // Lower Left Image Footprint
   lookup = m_kwl.find("LOWER_LEFT_CORNER");
   if(lookup)
   {
      ossimString tmp;
      tmp = lookup;
      tmp.trim(";");
      
      std::vector<ossimString> tmp_vec;
      tmp_vec = tmp.explode(ossimString(","));
      
      m_ll = ossimDpt(tmp_vec[2].toDouble(),
                      tmp_vec[3].toDouble());
      
      m_geom.add("hdr_ll_easting", m_ll.x, true);
      m_geom.add("hdr_ll_northing", m_ll.y, true);
   }
   
   // Band Information
   for(ossim_uint32 i = 1; i <= m_num_bands; ++i)
   {
      ossimString srch;
      
      srch = "BAND" + ossimString::toString(i) + "_NAME";
      lookup = m_kwl.find(srch);
      if(lookup)
      {
         m_band_name.push_back(ossimString(lookup).trim(";"));
      }
      
      srch = "BAND" + ossimString::toString(i) + "_FILENAME";
      lookup = m_kwl.find(srch);
      if(lookup)
      {
         ossimFilename img_file =
            m_file.path().dirCat(ossimFilename(ossimString(lookup).trim(";")));
         
         if(img_file.exists())
         {
            m_band_filename.push_back(img_file);
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ERROR: Image File Does Not Exists or Is Missing (" 
                 << img_file << ")" << std::endl;
            theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
            return;
         }
      }
      else
      {
         ossimFilename img_file = m_file.path() + "/" + m_file.fileNoExtension() + ".I" + ossimString::toString(i);

         if(img_file.exists())
         {
            m_band_filename.push_back(img_file);
         }
         else
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ERROR: Image File Does Not Exists or Is Missing (" 
                 << img_file << ")" << std::endl;
            theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
            return;
         }
      }

      srch = "BAND" + ossimString::toString(i) + "_WAVELENGTHS";
      lookup = m_kwl.find(srch);
      if(lookup)
      {
         m_band_wavelengths.push_back(ossimString(lookup).trim(";"));
      }

      srch = "BAND" + ossimString::toString(i) + "_RADIOMETRIC_GAINS/BIAS";
      lookup = m_kwl.find(srch);
      if(lookup)
      {
         m_band_radiom_gains_bias.push_back(ossimString(lookup).trim(";"));
      }
   }
}

ossimKeywordlist ossimNdfHeader::getGeom()
{
   return m_geom;
}

bool ossimNdfHeader::isSupportedNlaps()
{
   ossimString r = m_kwl.find("NDF_REVISION");

   if(r.size() > 0)
      return true;
   else
      return false;
}

std::ostream& operator << (std::ostream& o, const ossimNdfHeader& head)
{
   if(head.theErrorStatus == ossimErrorCodes::OSSIM_ERROR)
   {
      // Error exists parsing file
      return o;
   }
   else
   {
      o << std::endl;
      o << "NDF Revision: " << head.m_revision << std::endl;
      o << "Map Projection: " << head.m_projection << std::endl;
      o << "UTM Zone: " << head.m_utmzone << std::endl;
      o << "Datum: " << head.m_datum << std::endl;
      o << "Lines: " << head.m_lines << std::endl;
      o << "Samples: " << head.m_samples << std::endl;
	
      o << "UL: " << head.m_ul << std::endl;
      o << "UR: " << head.m_ur << std::endl;
      o << "LR: " << head.m_lr << std::endl;
      o << "LL: " << head.m_ll << std::endl;
      o << "GSD: " << head.m_gsd << std::endl;

      o << "Satellite: " << head.m_satellite << std::endl;
      o << "Instrument: " << head.m_instrument << std::endl;
      o << "Dataset Type: " << head.m_dataset_type << std::endl;
      o << "Acquisition Date: " << head.m_acqdate << std::endl;
      o << "Interleave: " << head.m_interleave << std::endl;
      o << "Processing Level: " << head.m_proc_level << std::endl;
      o << "Number of Bands: " << head.m_num_bands << std::endl;
      o << "Sun Elevation: " << head.m_sun_elev << std::endl;
      o << "Sun Azimuth: " << head.m_sun_azim << std::endl;
      o << "WRS: " << head.m_wrs << std::endl;
      o << "Path: " << head.m_path << std::endl;
      o << "Row: " << head.m_row << std::endl;

      for(ossim_uint32 i = 0; i < head.m_num_bands; ++i)
      {
         o << "Band Name: " << head.m_band_name[i] << std::endl;
         o << "Band Wavelengths: " << head.m_band_wavelengths[i] << std::endl;
         o << "Band Radmiometry Gains/Bias: " << head.m_band_radiom_gains_bias[i] << std::endl;
         o << "Band Filename: " << head.m_band_filename[i] << std::endl;
      }

      return o;
   }
}
