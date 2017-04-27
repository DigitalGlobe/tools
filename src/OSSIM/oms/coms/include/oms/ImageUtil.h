//-----------------------------------------------------------------------------
// File:  ImageUtil.h
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Wrapper class declaration for ossimImageUtil with swig readable
// interfaces.
//
//-----------------------------------------------------------------------------
// $Id: ImageUtil.h 21257 2012-07-08 16:13:18Z dburken $

#ifndef omsImageUtil_HEADER
#define omsImageUtil_HEADER 1

#include <oms/Constants.h>
#include <string>

class ossimImageUtil;

namespace oms
{
   class OMSDLL ImageUtil
   {
   public:

      /** @brief default constructor. */
      ImageUtil();
      
      /** @brief destructor */
      ~ImageUtil();

      /**
       * @brief Initial method.
       *
       * Typically called from application prior to execute.  This parses
       * all options and put in keyword list m_kwl.
       * 
       * @param ap Arg parser to initialize from.
       * @return true, indicating process should continue with execute.
       * @note A throw with an error message of "usage" is used to get out when
       * a usage is printed.
       */
      bool initialize(int argc, char* argv[]);

      /**
       * @brief execute method.
       *
       * Performs the execution of image processing, i.e. building overviews,
       * histogram and so on.
       *
       * Note if initialize was passed a directory it will walk the tree and find
       * all image that ossim can open and process them.
       * 
       * @note Throws ossimException on error.
       */
      void execute();

      /**
       * @brief ProcessFile method.
       *
       * This method is linked to the ossimFileWalker::walk method via a callback
       * mechanism.  It is called by the ossimFileWalk (caller).  In turn this
       * class (callee) calls ossimFileWalker::setRecurseFlag and
       * ossimFileWalker::setAbortFlag to control the waking process.
       * 
       * @param file to process.
       */
      void processFile(const std::string& file);

      /**
       * @brief Sets create overviews flag.
       * 
       * @param flag If true overview will be created if image does not already
       * have the required or if the REBUILD_OVERVIEWS_KW is set.
       *
       * @note Number of required overviews is controlled by the ossim preferences
       * keyword overview_stop_dimension.
       */
      void setCreateOverviewsFlag( bool flag );

      /**
       * @brief Sets the rebuild overview flag.
       *
       * @param flag If true forces a rebuild of overviews even if image has
       * required number of reduced resolution data sets.
       *
       * @note Number of required overviews is controlled by the ossim preferences
       * keyword overview_stop_dimension.
       */
      void setRebuildOverviewsFlag( bool flag );

      /**
       * @brief Sets the rebuild histogram flag.
       *
       * @param flag If true forces a rebuild of histogram even if image has one already.
       */
      void setRebuildHistogramFlag( bool flag );

      /**
       * @brief Sets key OVERVIEW_TYPE_KW.
       *
       * Available types depens on plugins.  Know types:
       * 
       * ossim_tiff_box ( defualt )
       * ossim_tiff_nearest
       * ossim_kakadu_nitf_j2k ( kakadu plugin )
       * gdal_tiff_nearest	    ( gdal plugin )
       * gdal_tiff_average	    ( gdal plugin )
       * gdal_hfa_nearest      ( gdal plugin )	
       * gdal_hfa_average      ( gdal plugin )	
       * 
       * @param type One of the above.
       */
      void setOverviewType( const std::string& type );
   
      /**
       * @brief sets the overview stop dimension used by processFile method.
       *
       * The overview builder will decimate the image until both dimensions are
       * at or below this dimension.
       *
       * @param dimension
       *
       * @note Recommend a power of 2 value, i.e. 8, 16, 32 and so on.
       */
      void setOverviewStopDimension( ossim_uint32 dimension );
      void setOverviewStopDimension( const std::string& dimension );

      /**
       * @brief Sets the tile size used by processFile method.
       *
       * @param tileSize
       *
       * @note Must be a multiple of 16, i.e. 64, 128, 256 and so on.
       */
      void setTileSize( ossim_uint32 tileSize );

      /**
       * @brief Sets create histogram flag used by processFile method..
       *
       * @param flag If true a full histogram will be created.
       */
      void setCreateHistogramFlag( bool flag );
      
      /**
       * @brief Sets create histogram flag.
       *
       * @param flag If true a histogram will be created in fast mode.
       */
      void setCreateHistogramFastFlag( bool flag );
      
      /**
       * @brief Sets create histogram "R0" flag keyword CREATE_HISTOGRAM_R0_KW used by
       * processFile method.
       *
       * @param flag If true a histogram will be created from R0.
       */
      void setCreateHistogramR0Flag( bool flag );

      /**
       * @brief Sets scan for min/max flag.
       *
       * @param flag If true a file will be scanned for min/max and a file.omd
       * will be written out.
       */
      void setScanForMinMax( bool flag );
   
      /**
       * @brief Sets scan for min/max/null flag keyword SCAN_MIN_MAX_KW used by
       * processFile method.
       *
       * @param flag If true a file will be scanned for min/max/null and a file.omd
       * will be written out.
       */
      void setScanForMinMaxNull( bool flag );

      /**
       * @brief Sets the writer property for compression quality.
       *
       * @param quality For TIFF JPEG takes values from 1
       * to 100, where 100 is best.  For J2K plugin (if available),
       * numerically_lossless, visually_lossless, lossy.
       */
      void setCompressionQuality( const std::string& quality );
      
      /**
       * @brief Sets the compression type to use when building overviews.
       *  
       * @param compression_type Current supported types:
       * - deflate 
       * - jpeg
       * - lzw
       * - none
       * - packbits
       */
      void setCompressionType( const std::string& type );
      
      /**
       * @brief Sets the overview builder copy all flag.
       * @param flag
       */
      void setCopyAllFlag( bool flag );
      
      /**
       * @brief Sets the output directory.  Typically overviews and histograms
       * are placed parallel to image file.  This overrides.
       *  
       * @param directory
       */
      void setOutputDirectory( const std::string& directory );

      /**
       * @brief Set number of threads to use.
       *
       * This is only used in execute method if a directory is given to
       * application to walk.
       *
       * @param threads Defaults to 1 if THREADS_KW is not found.
       */
      void setNumberOfThreads( ossim_uint32 threads );      
      
   private:

      ossimImageUtil* m_imageUtil;
      
   }; // End: class OMSDLL ImageUtil

} // End of namespace oms.

#endif /* #ifndef omsImageUtil_HEADER */
