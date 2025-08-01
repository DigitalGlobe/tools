//
// Sample: GeodatabaseManagement
//
// Demonstrates how to create a new geodatabase, open a geodatabase, and delete
// a geodatabase.

/*
   Copyright © 2025 Esri

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

   A local copy of the license and additional notices are located with the
   source distribution at:

   http://github.com/Esri/file-geodatabase-api/FileGDB_API_1.5.3
*/

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
