//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
// Description: This is a 3-D vector without the homogeneous
//              coordinate.
//
//*******************************************************************
//  $Id: ossimColumnVector3d.cpp 11408 2007-07-27 13:43:00Z dburken $

#include <iomanip>
#include <iostream>
#include <sstream>

#include <pqe/ColumnVector3d.h>
#include <pqe/Common.h>

std::string pqe::ColumnVector3d::toString(pqe_uint32 precision) const
{
   std::ostringstream os;
   os << std::setiosflags(std::ios_base::fixed)
      << std::setprecision(precision);
   
   os << "( ";
   if ( pqe::isnan(data[0]) == false)
   {
      os << data[0];
   }
   else
   {
      os << "nan";
   }
   os << ", ";
   if ( pqe::isnan(data[1]) == false )
   {
      os << data[1];
   }
   else
   {
      os << "nan";
   }
   os << ", ";
   if ( pqe::isnan(data[2]) == false )
   {
      os << data[2];
   }
   else
   {
      os << "nan";
   }
   os << " )";
   
   return os.str();
}

void pqe::ColumnVector3d::toPoint(const std::string& s)
{
   // Nan out the column vector for starters.
   data[0] = pqe::nan();
   data[1] = pqe::nan();
   data[2] = pqe::nan();
  
   std::istringstream is(s);

   // Check the stream.
   if (!is) return;

   //---
   // Expected input format:
   // ( 0.0000000, 0.0000000, 0.00000000 )
   //   -----x---- -----y---- -----z----
   //---
   
   const int SZ = 64; // Handle real big number...
   std::string os;
   char buf[SZ];
   char c = 0;

   const std::string NS = "nan";

   //---
   // X SECTION:
   //---
   
   // Grab data up to the first comma.
   is.get(buf, SZ, ',');

   if (!is) return;

   // Copy to ossim string.
   os = buf;
   
   // Get rid of the '(' if there is any.
   std::string::size_type pos = os.find('(');
   if (pos != std::string::npos)
   {
      os.erase(pos, 1);
   }   

   // Check for nan:
   pos = os.find(NS);
   if (pos == std::string::npos)
   {
      std::istringstream is2(os);
      is2 >> data[0];
   }
   else
   {
      data[0] = pqe::nan();
   }

   // Eat the comma that we stopped at.
   while (c != ',')
   {
      is.get(c);
      if (!is) break;
   }
   
   //---
   // Y SECTION:
   //---
   
   // Grab the data up to the next ','
   is.get(buf, SZ, ',');

   if (!is) return;
   
   // Copy to ossim string.
   os = buf;

   // Check for nan:
   pos = os.find(NS);
   if (pos == std::string::npos)
   {
      std::istringstream is2(os);
      is2 >> data[1];
   }
   else
   {
      data[1] = pqe::nan();
   }
   
   // Eat the comma that we stopped at.
   c = 0;
   while (c != ',')
   {
      is.get(c);
      if (!is) break;
   }

   //---
   // Z SECTION:
   //---
   
   // Grab the data up to the ')'
   is.get(buf, SZ, ')');
   
   if (!is) return;
   
   // Copy to ossim string.
   os = buf;

   // Check for nan:
   pos = os.find(NS);
   if (pos == std::string::npos)
   {
      std::istringstream is2(os);
      is2 >> data[2];
   }
   else
   {
      data[2] = pqe::nan();
   }
}

namespace pqe
{
   std::ostream& operator<<(std::ostream& out, const pqe::ColumnVector3d& v)
   {
      // Capture the original flags.
      std::ios_base::fmtflags f = out.flags();
      
      out << std::setiosflags(std::ios_base::fixed) << std::setprecision(15)
          << v[0] << " " << v[1] <<" "
          << v[2];
      
      // Reset flags.
      out.setf(f);
      
      return out;
   }
   
} // namespace pqe
