//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Simple container for output format type with
// mime type, name, and description.
//
//----------------------------------------------------------------------------
// $Id: ImageOutputFormat.h 11216 2007-06-14 21:20:55Z dburken $
#ifndef omsImageOutputFormat_HEADER
#define omsImageOutputFormat_HEADER 1

#include <string>
#include <oms/Constants.h> /* for OMSDLL */

namespace oms
{
   class OMSDLL ImageOutputFormat
   {
   public:
   
      /** @brief default constructor */
      ImageOutputFormat();
   
      /** @brief destructor */
      ~ImageOutputFormat();

      /** @return MIME type like image/jpeg */
      std::string mimeType() const;

      /** @return writer name like ossim_jpeg */
      std::string name() const ;

      /** @return The long description like ossim jpeg writer. */
      std::string description() const;

      /** @param type The MIME type like image/jpeg. */
      void setMimeType(const std::string& type);

      /** @param name The writer name like ossim_jpeg. */
      void setName(const std::string& name);

      /** @param description The long description like ossim jpeg writer. */
      void setDescription(const std::string& description);

   private:

      /** MIME type like image/jpeg */
      std::string theMimeType;

      /** writer name like ossim_jpeg */
      std::string theName;

      /** long name like "The OSSIM jpeg writer." */
      std::string theDescription;
   };

} // End of namespace oms

#endif /* End of #ifndef omsImageOutputFormat_HEADER */
