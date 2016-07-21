#ifndef ossimWorldWindJpegImage_HEADER
#define ossimWorldWindJpegImage_HEADER
#include <iostream>
#include <ossim/base/ossimConstants.h>

class ossimWorldWindJpegImage
{
public:
   static int decode(std::istream& inputStream,
                      ossim_uint8* destBuffer);
   
};

#endif
