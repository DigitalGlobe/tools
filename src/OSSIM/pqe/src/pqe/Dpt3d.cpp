// $Id$

#include <iostream>
#include <iomanip>

#include <pqe/Dpt3d.h>

pqe::Dpt3d::Dpt3d(const pqe::Dpt &aPt)
   :x(aPt.x),
    y(aPt.y),
    z(0.0)
{
   if(aPt.isNan())
   {
      makeNan();
   }
}

std::ostream &operator << (std::ostream &out, const pqe::Dpt3d &rhs)
{
   return out << std::setiosflags(std::ios::fixed)
              << std::setprecision(15)
              << rhs.x
              << " "
              << rhs.y
              << " "
              << rhs.z
              << std::endl;
}
