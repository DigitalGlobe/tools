// Sample: ProcessTopologies
//
// Demonstrates how to extract the topology rules from a topology. The Definition of the 
// topology does not include the names of the origin and destination feature classes. This
// sample uses libXML the extract the feature class names and the topology rules. 
//
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
#include <fstream>

#include <FileGDBAPI.h>
#include "LibXMLSession.h"
#include "TopologyInterpreter.h"

using namespace std;
using namespace FileGDBAPI;

int main()
{
  fgdbError   hr;
  wstring     errorText;
  Geodatabase geodatabase;
  if ((hr = OpenGeodatabase(wstring(L"../data/Topo.gdb"), geodatabase)) != S_OK)
  {
    wcout << "An error occurred while opening the geodatabase." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  vector<string> childrenDefinitions;
  if ((hr = geodatabase.GetChildDatasetDefinitions(wstring(L"\\USA"), wstring(L"Feature Class"), childrenDefinitions)) != S_OK)
  {
    wcout << "Failure getting child definitions" << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  std::string xml;
  if ((hr = geodatabase.GetDatasetDefinition(wstring(L"\\USA\\USA_Topology"), wstring(L"Topology"), xml)) != S_OK)
  {
    wcout << "Failure getting topology def" << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // manages libxml memory - only call after we are done with geodatabase, etc.
  LibXMLSession xmlSession;

  try
  {
    TopologyInterpreter interpreter(xml, childrenDefinitions);

    cout << interpreter.Name << endl;
    for (size_t idx = 0; idx < interpreter.Records.size(); idx++)
      cout << interpreter.Records[idx].Relationship << " Origin Class: " << interpreter.Records[idx].OriginName << " Destination Class: " << interpreter.Records[idx].DestinationName << endl;
  }
  catch(const wchar_t* e)
  {
    wcout << e << endl;
    return -1;
  }

  return 0;
}



