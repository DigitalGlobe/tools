//*******************************************************************
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfEmbeddedRpfDes.h 23664 2015-12-14 14:17:27Z dburken $
#ifndef ossimNitfEmbeddedRpfDes_HEADER
#define ossimNitfEmbeddedRpfDes_HEADER 1

#include <iosfwd>
#include <ossim/support_data/ossimNitfRegisteredTag.h>
class ossimNitfFileHeader;

class OSSIMDLLEXPORT ossimNitfEmbeddedRpfDes : public ossimNitfRegisteredTag
{
public:
   ossimNitfEmbeddedRpfDes();
   
   virtual void parseStream(std::istream &in);

   /**
    * @brief Print method that outputs a key/value type format
    * adding prefix to keys.
    * @param out Stream to output to.
    * @param prefix Prefix added to key like "image0.";
    */
   virtual std::ostream& print(std::ostream& out,
                               const std::string& prefix=std::string()) const;
protected:
   virtual ~ossimNitfEmbeddedRpfDes();

TYPE_DATA
private:

};

#endif

