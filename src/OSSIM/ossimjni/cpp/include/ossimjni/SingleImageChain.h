//-----------------------------------------------------------------------------
// File:  SingleImageChain.h
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Class declaration for SingleImageChain.
//
//-----------------------------------------------------------------------------
// $Id: SingleImageChain.h 19748 2011-06-12 15:34:56Z dburken $

#ifndef ossimjniSingleImageChain_HEADER
#define ossimjniSingleImageChain_HEADER 1

#include <ossimjni/Constants.h>

#include <string>
#include <vector>

class ossimSingleImageChain;

namespace ossimjni
{
   /**
    * @class SingleImageChain
    *
    * Interface to/from java/ossim for ossimSingleImageChain.
    *
    * @note Interfaces with integers signed for java.
    */
   class OSSIMJNIDLL SingleImageChain
   {
   public:

      /** @brief default constructor. */
      SingleImageChain();
      
      /** @brief destructor */
      ~SingleImageChain();

      /**
       * @brief open method that takes an image file.
       *
       * Opens file and creates a simple chain with ossimImageHandler.
       *
       * @param file File to open.
       *
       * @return true on success, false on error.
       *
       * @note This will close previous chain if one was opened.
       */
      bool open(const std::string& file);

      /**
       * @brief open method that takes an image file and an entry.
       *
       * Opens file and creates a simple chain with ossimImageHandler.
       *
       * @param file File to open.
       *
       * @param entry Zero based entry.
       *
       * @return true on success, false on error.
       *
       * @note This will close previous chain if one was opened.
       */
      bool open(const std::string& file, ossimjni_int32 entry);

      /** @return true if image handler is opened. */
      bool isOpen() const;

      /** @brief close method to delete the image handler. */
      void close();

      /**
       * @param entryList This is the list to initialize with entry indexes.
       *
       * @note This implementation returns puts one entry "0" in the list.
       */
      std::vector<ossimjni_int32> getEntryList() const;

      /** @return The filename of the image. */
      std::string getFilename() const;
      
      /**
       * @brief Create a rendered image chain.
       *
       * Typical usage is to call this after "open" method returns true like:
       * if ( myChain->open(myFile) == true )
       * {
       *    myChain->createRenderedChain();
       *    code-goes-here();
       * }
       *
       * Typical chain is:
       * 
       * 1) image handler
       * 2) band selector (optional)
       * 3) histogram remapper(optional)
       * 4) scalar remapper (optional)
       * 5) resampler cache
       * 6) resampler
       * 7) band selector (optional when going one band to three)
       * 8) chain cache
       *
       * NOTES:
       * 1) If doing a sequential write where tiles to the right of the
       *    resampler will not be revisited the chain cache could be
       *    disabled to save memory.
       */
      void createRenderedChain();

      /**
       * @brief selectBands
       *
       * Used to select which bands to use.
       *
       * @note Bands are zero based.
       *
       * @return true on success, false if not open or bands out of range.
       */
      bool selectBands(const std::vector<ossimjni_uint32>& bands);
      
      /**
       * @brief getBandSelection
       *
       * @note Bands are zero based.
       *
       * @return A list of bands selected to use in the output product.
       */
      std::vector<ossimjni_uint32> getBandSelection() const;

      /** @return Number of bands or 0 if not open. */
      ossimjni_uint32 getNumberOfBands() const;

      /**
       * @brief setHistogram
       *
       * Used to identify a histogram file associated with the source image. 
       *  @param his Fully qualified path to a histogram file.
       */
      bool setHistogram(const std::string& his);

      /** @return Histogram file. */
      std::string getHistogramFile() const;

      /** @brief setOverview */
      bool setOverview(const std::string& ovr);

      /** @return overview file. */
      std::string getOverviewFile() const;
      
   private:

      /** @returns true if bands in range, false if not or if image is not open. */
      bool verifyBandRange(const std::vector<ossimjni_uint32>& bands) const;

      ossimSingleImageChain* m_img;
   };

} // End of namespace ossimjni.

#endif /* #ifndef ossimjniSingleImageChain_HEADER */
