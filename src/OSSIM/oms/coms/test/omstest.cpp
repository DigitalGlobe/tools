// $Id: omstest.cpp 9821 2006-10-30 19:24:06Z dburken $

#include <iostream>
#include <string>
using namespace std;

#include <oms/omsIpt.h>
#include <oms/omsDpt.h>
#include <oms/omsIrect.h>
#include <oms/omsDrect.h>

using namespace std;


int main(int argc, char *argv[])
{
   std::string s1 = "POINT(1 2)";
   std::string s2 = "POINT( 3 4 )";
   std::string s3 = "POINT( 5 6)";
   std::string s4 = "POINT ( 7  8 )";
   std::string s5 = "garbage ( 7  8 )";

   omsIpt p1;
   omsIpt p2;
   omsIpt p3;
   omsIpt p4;
   omsIpt p5;
   

   p1.fromWkt(s1);
   p2.fromWkt(s2);
   p3.fromWkt(s3);
   p4.fromWkt(s4);
   p5.fromWkt(s5);

   cout << "p1:  " << p1.toWkt().c_str()
        << "\np2:  " << p2.toWkt().c_str()
        << "\np3:  " << p3.toWkt().c_str()
        << "\np4:  " << p4.toWkt().c_str()
        << "\np5:  " << p5.toWkt().c_str()            
        << endl;

   s1 = "POINT(1.0000 2.123456)";
   s2 = "POINT( 3.3987495 4.876979 )";
   s3 = "POINT( 5.97697 6.97979)";
   s4 = "POINT ( 7.0979879  8.9798797 )";
   s5 = "garbage ( 7.7665756  8.9789787 )";

   omsDpt dp1;
   omsDpt dp2;
   omsDpt dp3;
   omsDpt dp4;
   omsDpt dp5;
   

   dp1.fromWkt(s1);
   dp2.fromWkt(s2);
   dp3.fromWkt(s3);
   dp4.fromWkt(s4);
   dp5.fromWkt(s5);

   cout << "dp1:  " << dp1.toWkt(4).c_str()
        << "\ndp2:  " << dp2.toWkt(8).c_str()
        << "\ndp3:  " << dp3.toWkt(12).c_str() 
        << "\ndp4:  " << dp4.toWkt().c_str() // precision = 15
        << "\ndp5:  " << dp5.toWkt().c_str()            
        << endl;

   s1 = "POLYGON((0 0,10 0,10 10,0 10,0 0))";
   
   omsIrect ir1;
   ir1.fromWkt(s1);
   cout << "\n\ns1:  " << s1
        << "\nir1:  " << ir1
        << "\nir1.toWkt():  " << ir1.toWkt().c_str()
        << endl;

   s1 = "POLYGON((0.1 0.1, 10.2 0.1, 10.2 10.2, 0.1 10.2, 0.1 0.1))";

   omsDrect dr1;
   dr1.fromWkt(s1);
   cout << "\n\ns1:  " << s1
        << "\ndr1:  " << dr1
        << "\ndr1.toWkt():  " << dr1.toWkt().c_str()
        << endl;

   return 0;
}
