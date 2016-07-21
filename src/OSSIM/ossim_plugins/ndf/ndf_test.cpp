//***
// Written by: Kenneth Melero (kmelero@sanz.com)
// Description:  This application unit tests the nlaps plugin methods.
// ***

#include <iostream>

#include <ossimNdfHeader.h>
#include <ossim/init/ossimInit.h>

using namespace std;

int main(int argc, char *argv[])
{
	ossimInit::instance()->initialize(argc, argv);
   
	ossimNdfHeader lnh(argv[1]);
	cout << lnh;
	cout << lnh.getGeom();
	
	cout << "Status: " << lnh.getErrorStatus() << endl;

	return lnh.getErrorStatus();
}
