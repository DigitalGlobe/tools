#include <oms/Envelope.h>
#include <sstream>
#include <iomanip>

namespace oms{

   std::string Envelope::toString()const
   {
      std::ostringstream out;

      out << std::setprecision(15)<< m_minx << ", " << m_miny << ", " 
          << m_maxx << ", " << m_maxy << ", " << m_epsgCode;

      return std::string(out.str());
   }
}