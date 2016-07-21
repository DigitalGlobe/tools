// Sample: ProcessTopologies
//
// Demonstrates how to extract the topology rules from a topology. The Definition of the 
// topology does not include the names of the origin and destination feature classes. This
// sample uses libXML the extract the feature class names and the topology rules. 
//
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



