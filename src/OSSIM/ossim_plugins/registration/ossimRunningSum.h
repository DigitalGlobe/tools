// class ossimRunningSum
// provides running sum & running sum of squares two dimesnions buffer
//
// IMPORTANT: sums are calculated at object creation (hence const in get accessors)
//
#ifndef ossimRunningSum_HEADER
#define ossimRunningSum_HEADER

#include <vector>

class ossimRunningSum
{
public:
   ossimRunningSum(int slowdim, int fastdim, const double* table);
   virtual ~ossimRunningSum();
   
   // those accessors handle out of bounds positions
   double getRS(int p1, int p2)const;
   double getRS2(int p1, int p2)const;

   double getAverage()const; //for convenience
   double getStd()const; //for convenience

protected:
   int theSd; // slow varying dimension (row number for row-major image)
   int theFd; // fast varying dim (col number for a row-major image)
   double theNormFact;
   std::vector<double> rs;
   std::vector<double> rs2;
};

#endif //ossimRunningSum_HEADER
