//-----------------------------------------------------------------------------
// File:  ImageData.h
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Wrapper class for ossimImageData with swig readable interfaces.
//
//-----------------------------------------------------------------------------
// $Id$

#ifndef ossimjniImageData_HEADER
#define ossimjniImageData_HEADER 1

#include <ossimjni/Constants.h>
#include <string>

class ossimImageData;

namespace ossimjni
{
   class OSSIMJNIDLL ImageData
   {
   public:

      /** @brief default constructor. */
      ImageData();
      
      /** @brief destructor */
      ~ImageData();

      /**
       * @brief copy constructor
       * @param obj Copy this
       */
      ImageData( const ImageData& obj );

      /**
       * @brief Assignment operator.
       * @param pair StringPair to assign from.
       */
      const ImageData& operator=( const ImageData& rhs );

      /** @return true if m_imageData is initialized, false if not. */
      bool valid() const;
      
      /**
       * @retrurn a const pointer to the ossimImageData.
       * @note getImageData not overloaded for swig warning.
       */
      const ossimImageData* getConstImageData() const;

      /** @retrurn a const pointer to the ossimImageData. */
      ossimImageData* getImageData();

      /**
       * @brief Set method.
       * @param imageData Pointer to image data. This owns memory and will
       * delete on destruction or a subsequent call to setImageData.
       */
      void setImageData( ossimImageData* imageData );
      
      /**
       * @brief Writes tile to file.
       *
       * This will write the buffer to the file assuming a contiguous buffer in
       * BSQ format.  Currently does not support converting to BIP or BIL
       * or byte swapping but probably should add at some point.
       *
       * @param f File to write.
       *
       * @return true on success, false on error.
       */
      bool write(const std::string& file) const;

   private:

      ossimImageData* m_imageData;
   };

} // End of namespace ossimjni.

#endif /* #ifndef ossimjniImageData_HEADER */
