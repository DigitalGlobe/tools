//
// Sample: Domains
//
// Demonstrates how to create a new range domain, a code value domain,
// to get the definition of an existing domain, delete a domains, alter
// a domain and to assign it to a field.

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

using namespace std;
using namespace FileGDBAPI;

int main()
{
  // Open the geodatabase.
  fgdbError   hr;
  wstring     errorText;
  Geodatabase geodatabase;
  if ((hr = OpenGeodatabase(L"../data/Domains.gdb", geodatabase)) != S_OK)
  {
    wcout << "An error occurred while opening the geodatabase." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Delete the SpeedLimit domain.
  hr = geodatabase.DeleteDomain(L"SpeedLimit");
  if (hr == S_OK)
  {
    wcout << "The SpeedLimit domain has been deleted." << endl;
  }
  else if (hr == -2147209215)
  {
    wcout << "The SpeedLimit domain does not exist, no need to delete" << endl;
  }
  else
  {
    wcout << "An error occurred while deleting the domain SpeedLimit." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Delete the RoadSurfaceType domain.
  hr = geodatabase.DeleteDomain(L"RoadSurfaceType");
  if (hr == S_OK)
  {
    wcout << "The RoadSurfaceType domain has been deleted." << endl;
  }
  else if (hr == -2147209215)
  {
    wcout << "The RoadSurfaceType domain does not exist, no need to delete" << endl;
  }
  else
  {
    wcout << "An error occurred while deleting the RoadSurfaceType." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Open the SpeedLimit.xml to get a range domain definition.
  string   speedLimitDef;
  string   defLine;
  ifstream defFile("../Domains/SpeedLimit.xml");
  while (getline(defFile, defLine))
    speedLimitDef.append(defLine + "\n");
  defFile.close();

  // Create the SpeedLimit range domain in the geodatabase.
  if ((hr = geodatabase.CreateDomain(speedLimitDef)) != S_OK)
  {
    wcout << "An error occurred while creating the SpeedLimit domain." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  wcout << "The SpeedLimit range domain has been created." << endl;

  // Get the Definition of an existing domain.
  string domainDef;
  if ((hr = geodatabase.GetDomainDefinition(L"MedianType", domainDef)) != S_OK)
  {
    wcout << "An error occurred while getting the domain definition." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  wstring domainDefW(domainDef.begin(), domainDef.end());
  wcout << "The definition of the MedianType domain is: " << '\n' << domainDefW << endl;

  // Open the RoadSurfaceType.xml to get a coded value domain definition.
  string   roadTypeDef;
  string   defLine2;
  ifstream defFile2("../Domains/RoadSurfaceType.xml");
  while (getline(defFile2, defLine2))
    roadTypeDef.append(defLine2 + "\n");
  defFile2.close();

  // Create the RoadSurfaceType domain in the geodatabase.
  if ((hr = geodatabase.CreateDomain(roadTypeDef)) != S_OK)
  {
    wcout << "An error occurred while creating the RoadSurfaceType domain." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  wcout << "The RoadSurfaceType coded value domain has been created." << endl;

  // Open the RoadSurfaceTypeALtered.xml to get a altered coded value domain definition.
  string   roadTypeDefAlter;
  string   defLineAlter;
  ifstream defFileAlter("../Domains/RoadSurfaceTypeAltered.xml");
  while (getline(defFileAlter, defLineAlter))
    roadTypeDefAlter.append(defLineAlter + "\n");
  defFileAlter.close();

  // Alter the RoadSurfaceType domain in the geodatabase.
  if ((hr = geodatabase.AlterDomain(roadTypeDefAlter)) != S_OK)
  {
    wcout << "An error occurred while Altering the RoadSurfaceType domain." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  wcout << "The RoadSurfaceType coded value domain has been Altered." << endl;

  //Assign the SpeedLimit (Range) domain to the MaxSpeed field
  //
  Table table;
  if ((hr = geodatabase.OpenTable(L"\\Roads", table)) != S_OK)
  {
    wcout << "An error occurred while opening the table." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Load the xml definition for the field.
  string   fieldDef;
  string   defLine3;
  ifstream defFile3("../Domains/AlterMaxSpeed.xml");
  while (getline(defFile3, defLine3))
    fieldDef.append(defLine3 + "\n");
  defFile3.close();

  // Alter the table.
  int recordCount = 0;
  if ((hr = table.AlterField(fieldDef)) != S_OK)
  {
    wcout << "An error occurred while altering the field." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;

    // Report any extended errors (XML).
    ErrorInfo::GetErrorRecordCount(recordCount);
    for (int i = 0;i <= (recordCount - 1); i++)
    {
      ErrorInfo::GetErrorRecord(i,hr,errorText);
      wcout << errorText << endl;
    }
    ErrorInfo::ClearErrors();
    return -1;
  }
  wcout << "The SpeedLimnit (Range) domain has been assigned to the MaxSpeed field." << endl;

  // Assign the RoadSurfaceType (CodedValue) domain to the SurfaceType field

  // Load the xml definition for the field.
  string   fieldDef2;
  string   defLine4;
  ifstream defFile4("../Domains/AlterSurfaceType.xml");
  while (getline(defFile4, defLine4))
    fieldDef2.append(defLine4 + "\n");
  defFile4.close();

  // Alter the table.
  if ((hr = table.AlterField(fieldDef2)) != S_OK)
  {
    wcout << "An error occurred while altering the field." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;

    // Report any extended errors (XML).
    ErrorInfo::GetErrorRecordCount(recordCount);
    for (int i = 0;i <= (recordCount - 1); i++)
    {
      ErrorInfo::GetErrorRecord(i,hr,errorText);
      wcout << errorText << endl;
    }
    ErrorInfo::ClearErrors();
    return -1;
  }
  wcout << "The RoadSurfaceType (CodedValue) domain has been assigned to the SurfaceType field." << endl;


  // Close the table
  if ((hr = geodatabase.CloseTable(table)) != S_OK)
  {
    wcout << "An error occurred while closing Cities." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Close the geodatabase
  if ((hr = CloseGeodatabase(geodatabase)) != S_OK)
  {
    wcout << "An error occurred while closing the geodatabase." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  return 0;
}
