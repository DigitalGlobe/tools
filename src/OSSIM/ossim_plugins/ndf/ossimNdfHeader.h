//***
// Written by: Kenneth Melero (kmelero@sanz.com)
// Description: This class parses header files in National Land Archive Processing System 
//              Data Format (NDF). It currently supports Landsat MSS, TM,
//              and ETM+ sensors.
//              
// ***
// $Id$
#ifndef ossimNdfHeader_HEADER
#define ossimNdfHeader_HEADER 1

#include "../ossimPluginConstants.h"
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimKeywordlist.h>

#include <ostream>
#include <vector>

class OSSIM_PLUGINS_DLL ossimNdfHeader : public ossimErrorStatusInterface
{
public:
   ossimNdfHeader(const char* file);
   virtual ~ossimNdfHeader();

   friend OSSIM_PLUGINS_DLL std::ostream& operator <<
      (std::ostream& out, const ossimNdfHeader& head);
   
   ossimKeywordlist getGeom();
   std::vector<ossimFilename> getImageFileList() { return m_band_filename; }
   ossim_int64 getLines() { return m_lines; }
   ossim_int64 getSamples() { return m_samples; }
   ossim_uint32 getNumOfBands() { return m_num_bands; }
   
private:
   ossimNdfHeader();  // prevent use
   
   bool isSupportedNlaps();
   void parseHeader();
   
   ossimFilename    m_file;  // Source Filename
   ossimKeywordlist m_kwl;   // Source Header
   ossimKeywordlist m_geom;  // Converted Header to OSSIM Geometry
   
   ossimString  m_revision;
   ossimString  m_projection;
   ossim_sint32 m_utmzone;
   ossimString  m_datum;
   ossim_int64  m_lines;
   ossim_int64  m_samples;
   
   ossimDpt m_ul;
   ossimDpt m_ur;
   ossimDpt m_lr;
   ossimDpt m_ll;
   ossim_float64 m_gsd;
   
   ossimString   m_satellite;
   ossimString   m_instrument;
   ossimString   m_dataset_type;
   ossimString   m_acqdate;
   ossimString   m_interleave;
   ossim_uint32  m_proc_level;
   ossim_uint32  m_num_bands;
   ossim_float64 m_sun_elev;
   ossim_float64 m_sun_azim;
   ossimString   m_wrs;
   ossim_uint32  m_path;
   ossim_uint32  m_row;
   
   std::vector<ossimString>   m_band_name;
   std::vector<ossimFilename> m_band_filename;
   std::vector<ossimString>   m_band_wavelengths;
   std::vector<ossimString>   m_band_radiom_gains_bias;
};

#endif 
