//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Simple container class for an image point.
//
//----------------------------------------------------------------------------
// $Id$

#include <ostream>
#include <sstream>
#include <iomanip>

#include <pqe/Dpt.h>

std::string pqe::Dpt::toString() const
{
   std::ostringstream os;
   print(os, 15);
   return std::string(os.str());
}

std::string pqe::Dpt::toString(pqe_uint32 precision) const
{
   std::ostringstream os;
   print(os, precision);
   return std::string(os.str());
}

void pqe::Dpt::toPoint(const std::string& s)
{
   std::istringstream is(s);
   is >> *this;
}

std::ostream& pqe::Dpt::print(std::ostream& out, pqe_uint32 precision) const
{
   // Capture the original flags.
   std::ios_base::fmtflags f = out.flags();

   out << std::setiosflags(std::ios_base::fixed)
       << std::setprecision(precision);

   out << "( ";
   if ( !pqe::isnan(x) )
   {
      out << x << ", ";
   }
   else
   {
      out << "nan, ";
   }
   if ( !pqe::isnan(y) )
   {
      out << y << " )";
   }
   else
   {
      out << "nan )";
   }

   // Reset flags.
   out.setf(f);

   return out;
}

namespace pqe
{
   std::ostream& operator<<(std::ostream& out, const pqe::Dpt& obj)
   {
      return obj.print(out, 15);
   }
   
   std::istream& operator>>(std::istream& is, pqe::Dpt& pt)
   {
      //---
      // Expected input format:
      // ( 30.00000000000000, -90.00000000000000 )
      //   --------x--------  ---------y--------
      //---
      
      // Start with a nan point.
      pt.makeNan();
      
      // Check the stream.
      if (!is) return is;
      
      const int SZ = 64; // Handle real big number...
      std::string os;
      char buf[SZ];
      
      //---
      // X SECTION:
      //---
      
      // Grab data up to the first comma.
      is.get(buf, SZ, ',');
      
      if (!is) return is;
      
      // Copy to std::string.
      os = buf;
      
      // Get rid of the '(' if there is any.
      std::string::size_type pos = os.find('(');
      if (pos != std::string::npos)
      {
         os.erase(pos, 1);
      }   
      
      // Check for nan x value.
      if ( !os.empty() && (os.find("nan") == std::string::npos) )
      {
         std::istringstream is(os);
         is >> pt.x;
      }
      else
      {
         pt.x = pqe::nan();
      }
      
      //---
      // Y SECTION:
      //---
      
      // Grab the data up to the ')'
      is.get(buf, SZ, ')');
      
      if (!is) return is;
      
      // Copy to ossim string.
      os = buf;
      
      // Get rid of the ',' if there is any.
      pos = os.find(',');
      if (pos != std::string::npos)
      {
         os.erase(pos, 1);
      }
      
      // Check for nan y value.
      if ( !os.empty() && (os.find("nan") == std::string::npos) )
      {
         std::istringstream is(os);
         is >> pt.y;
      }
      else
      {
         pt.y = pqe::nan();
      } 
      
      // Gobble the trailing ")".
      char c = '\0';
      while (c != ')')
      {
         is.get(c);
         if (!is) break;
      }
      
      // Finished
      return is;
   }
   
} // namespace pqe
