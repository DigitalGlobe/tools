//
// Sample: FeatureDatasets
//
// Demonstrates how to create a new geodatabase, create a feature dataset, then
// create a table in the feature dataset.

// Copyright 2017 ESRI
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
  // Delete the geodatabase in the current directory if it's already there.
  fgdbError   hr;
  wstring     errorText;
  Geodatabase geodatabase;
  hr = DeleteGeodatabase(L"../FeatureDatasets/FeatureDatasetDemo.gdb");
  if (hr == S_OK)
  {
    wcout << "The geodatabase has been deleted" << endl;
  }
  else if (hr == -2147024893)
  {
    wcout << "The geodatabase does not exist, no need to delete" << endl;
  }
  else
  {
    wcout << "An error occurred while deleting the geodatabase." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Create a new geodatabase in the current directory.
  if ((hr = CreateGeodatabase(L"../FeatureDatasets/FeatureDatasetDemo.gdb", geodatabase)) != S_OK)
  {
    wcout << "An error occurred while creating the geodatabase." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  wcout << "The geodatabase has been created." << endl;

  // Create the spatial reference.
  SpatialReference spatialReference;
  spatialReference.SetSpatialReferenceID(32620);
  spatialReference.SetSpatialReferenceText(L"PROJCS[\"WGS_1984_UTM_Zone_20N\",GEOGCS[\"GCS_WGS_1984\",DATUM[\"D_WGS_1984\",SPHEROID[\" WGS_1984\",6378137.0,298.257223563]],PRIMEM[\" Greenwich\",0.0],UNIT[\"Degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"False_Easting\",500000.0],PARAMETER[\"False_Northing\",0.0],PARAMETER[\"Central_Meridian\",-63.0],PARAMETER[\"Scale_Factor\",0.9996],PARAMETER[\"Latitude_Of_Origin\",0.0],UNIT[\"Meter\",1.0],AUTHORITY[\"EPSG\",32620]]");

  // Create the feature dataset.
  if ((hr = geodatabase.CreateFeatureDataset(L"\\Transit",spatialReference)) != S_OK)
  {
    wcout << "An error occurred while creating the feature dataset." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  wcout << "The feature dataset has been created." << endl;

  // There are several differences between this example and adding a table at the root level of the geodatabase:
  //  - The table must have a shape field
  //  - The spatial reference must match that of the feature dataset

  // Create the geometryDef
  GeometryDef geometryDef;
  geometryDef.SetGeometryType (geometryPoint);
  geometryDef.SetSpatialReference(spatialReference);
  geometryDef.SetHasZ(false); //Set to true if the feature class is to be Z enabled. Defaults to FALSE.
  geometryDef.SetHasM(false); //Set to true if the feature class is to be M enabled. Defaults to FALSE.

  // Create the field def for the table.
  std::vector<FieldDef> fieldDefs;

  FieldDef fieldDef1;
  fieldDef1.SetName (L"OBJECTID");
  fieldDef1.SetType (fieldTypeOID);
  fieldDef1.SetIsNullable(false);
  fieldDefs.push_back(fieldDef1);

  FieldDef fieldDef2;
  fieldDef2.SetName (L"Shape");
  fieldDef2.SetType (fieldTypeGeometry);
  fieldDef2.SetIsNullable(true);
  fieldDef2.SetGeometryDef (geometryDef);
  fieldDefs.push_back(fieldDef2);

  FieldDef fieldDef3;
  fieldDef3.SetName (L"StopID");
  fieldDef3.SetType (fieldTypeInteger);
  fieldDef3.SetIsNullable(true);
  fieldDef3.SetAlias (L"Stop Identifier");
  fieldDefs.push_back(fieldDef3);

  FieldDef fieldDef4;
  fieldDef4.SetName (L"StopType");
  fieldDef4.SetType (fieldTypeSmallInteger);
  fieldDef4.SetIsNullable(true);
  fieldDefs.push_back(fieldDef4);

  // Create the table.
  Table table;
  if ((hr = geodatabase.CreateTable(L"\\Transit\\BusStops", fieldDefs, L"DEFAULTS", table)) != S_OK)
  {
    wcout << "An error occurred while creating the table." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  wcout << "The table has been created." << endl;

  // Close the table.
  if ((hr = geodatabase.CloseTable(table) != S_OK))
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
