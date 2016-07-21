//-----------------------------------------------------------------------------
// File:  GpkgWriter.h
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Class declaration for GpkgWriter. Wrapper for ossimGpkgWriter
// class.
//
//-----------------------------------------------------------------------------
// $Id$
#ifndef omsGpkgWriter_HEADER
#define omsGpkgWriter_HEADER 1

#include <oms/Constants.h>
#include <map>
#include <string>

class ossimGpkgWriterInterface;

namespace oms
{
   class ImageData;
   class Keywordlist;
   
   class OMSDLL GpkgWriter
   {
   public:

      /** @brief constructor */
      GpkgWriter();
      
      
      /** @brief destructor */
      ~GpkgWriter();
      
      /**
       * @brief Error status method.
       *
       * A false error status is indicative of the ossim_plugins/sqlite plugin
       * not being loaded.
       * 
       * @return true if m_writer was created at construction; false on error.
       */
      bool getErrorStatus() const;
      
      /**
       * @brief Opens file for writing, appending, merging without an input
       * connection. I.e. opening, then calling writeTile directly.
       * @param options.  Keyword list containing all options.
       */
      bool openFile( const Keywordlist& options );

      /**
       * @brief Opens file from a std::map<std::string, std::string>.
       * @param options.  Keyword list containing all options.
       * @return true, indicating open was successful.
       */
      bool openFile( const std::map<std::string, std::string>& map);
      
      /**
       * @brief Calls initial sqlite3_prepare_v2 statement.  Must be called
       * prior to calling writeTile.
       * @return SQLITE_OK(0) on success, something other(non-zero) on failure.
       */
      ossim_int32 beginTileProcessing();
      
      /**
       * @brief Direct interface to writing a tile to database.
       * @param tile to write.
       * @param zoolLevel
       * @param row
       * @param col
       * @return true on success, false on error.
       */     
      bool writeTile( ImageData*  tile,
                      ossim_int32 zoomLevel,
                      ossim_int64 row,
                      ossim_int64 col );
      
      bool writeCodecTile(ossim_int8* codecTile,
                          ossim_int32 codecTileSize,
                          ossim_int32 zoomLevel,
                          ossim_int64 row,
                          ossim_int64 col);
      /**
       * @brief Calls sqlite3_finalize(pStmt) terminating tile processing.
       */
      void finalizeTileProcessing();     
      
  private:
      
      ossimGpkgWriterInterface* m_writer;
   };

} // End of namespace oms.

#endif /* #ifndef omsGpkgWriter_HEADER */
