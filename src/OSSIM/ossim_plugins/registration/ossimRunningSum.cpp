// class ossimRunningSum : impl.
//
#include "ossimRunningSum.h"
#include <math.h>

using namespace std;

ossimRunningSum::ossimRunningSum(int slowdim, int fastdim, const double* table) :
    theSd(slowdim), 
    theFd(fastdim),
    theNormFact(1.0/((double)slowdim*fastdim))
{
   //allocate space
   unsigned long size = slowdim*fastdim;
   rs.resize(size);
   rs2.resize(size);

   int i,j;
   //use two swipes : first integrate along fastdim, then slowdim

   //fisrt swipe (reading table)
   vector<double>::iterator is  = rs.begin();
   vector<double>::iterator is2 = rs2.begin();
   double t=0.0;
   double s,s2;
   for (i=0;i<theSd;++i)
   {
      t=*(table++);
      *is  = s  =t; //init first element of row
      ++is;
      *is2 = s2 =t*t;
      ++is2;
      for (j=1;j<theFd;++j)
      {
         t=*(table++);
         *is  = s  = t   + s;
         ++is;
         *is2 = s2 = t*t + s2;
         ++is2;
      }
   }
   //second swipe (not reading table), starting on second row
/* 
   is  = rs.begin()  + theFd;
   is2 = rs2.begin() + theFd;

   for (i=1;i<theSd;++i)
   {
       for (j=0;j<theFd;++j)
       {
         *(is++)  += *(is -theFd);
         *(is2++) += *(is2-theFd);
      }
   }
*/
   for (i=1;i<theSd;++i) 
   {
       for (j=0;j<theFd;++j)
       {
         rs [i*theFd+j] += rs [(i-1)*theFd+j];
         rs2[i*theFd+j] += rs2[(i-1)*theFd+j];
      }
   }
   
}

ossimRunningSum::~ossimRunningSum()
{
}
   
double
ossimRunningSum::getRS(int p1, int p2)const
{
   //boundary correction
   if ((p1<0) || (p2<0))
   { 
      return 0.0;
   }
   if (p1>=theSd) p1=theSd-1;
   if (p2>=theFd) p2=theFd-1;   
   return rs[p1*theFd + p2];
}

double
ossimRunningSum::getRS2(int p1, int p2)const
{
   //boundary correction
   if ((p1<0) || (p2<0))
   { 
      return 0.0;
   }
   if (p1>=theSd) p1=theSd-1;
   if (p2>=theFd) p2=theFd-1;   
   return rs2[p1*theFd + p2];
}

double
ossimRunningSum::getAverage()const
{
   return rs[theSd*theFd-1] * theNormFact;
}

double
ossimRunningSum::getStd()const
{
   //fabs avoids nan for uniform data
   return sqrt(fabs( (rs2[theSd*theFd-1]  - rs[theSd*theFd-1]*rs[theSd*theFd-1]*theNormFact) * theNormFact));
}
