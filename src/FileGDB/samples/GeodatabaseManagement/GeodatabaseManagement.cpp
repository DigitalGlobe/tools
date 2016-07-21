//
// Sample: GeodatabaseManagement
//
// Demonstrates how to create a new geodatabase, open a geodatabase, and delete
// a geodatabase.

// Copyright 2015 ESRI
// 
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
// 
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
// 
// See the use restrictions at <your File Geodatabase API install location>/license/userestrictions.txt.
// 


#include <string>
#include <iostream>

#include <FileGDBAPI.h>

using namespace std;
using namespace FileGDBAPI;

int main()
{
  // Create a new geodatabase in the current directory.
  fgdbError   hr;
  wstring     errorText;
  Geodatabase geodatabase;
  if ((hr = CreateGeodatabase(L"../GeodatabaseManagement/GdbManagement.gdb", geodatabase)) != S_OK)
  {
    wcout << "An error occurred while creating the geodatabase." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  wcout << "The geodatabase has been created." << endl;
  CloseGeodatabase(geodatabase);

  // Re-open the geodatabase.
  if ((hr = OpenGeodatabase(L"../GeodatabaseManagement/GdbManagement.gdb", geodatabase)) != S_OK)
  {
    wcout << "An error occurred while opening the geodatabase." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  wcout << "The geodatabase has been opened." << endl;

  // Close the geodatabase before the delete.
  if ((hr = CloseGeodatabase(geodatabase)) != S_OK)
  {
    wcout << "An error occurred while closing the geodatabase." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Delete the geodatabase.
  if ((hr = DeleteGeodatabase(L"../GeodatabaseManagement/GdbManagement.gdb")) != S_OK)
  {
    wcout << "An error occurred while deleting the geodatabase." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  wcout << "The geodatabase has been deleted." << endl;

  return 0;
}
