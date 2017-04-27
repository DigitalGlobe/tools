// $Id$

#include <ostream>
#include <iomanip>

#include <pqe/ColumnVector4d.h>


std::ostream& operator<<(std::ostream& out, const pqe::ColumnVector4d& v)
{
   // Capture the original flags.
   std::ios_base::fmtflags f = out.flags();

   return out << std::setiosflags(std::ios_base::fixed)
              << std::setprecision(15)
              << v[0] << " " << v[1] <<" "
              << v[2] << " " << v[3];

   // Reset flags.
   out.setf(f);

   return out;
}
