//
// Sample: Querying
//
// Demonstrates how to perform simple spatial and attribute queries.

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
  // Open the geodatabase.
  fgdbError   hr;
  wstring     errorText;
  Geodatabase geodatabase;
  if ((hr = OpenGeodatabase(L"../data/Querying.gdb", geodatabase)) != S_OK)
  {
    wcout << "An error occurred while opening the geodatabase." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Open the Cities table.
  Table table;
  if ((hr = geodatabase.OpenTable(L"Cities", table)) != S_OK)
  {
    wcout << "An error occurred while opening the table." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Perform a simple attribute query: find the names of every city with a 'TERM'
  // value equal to 'City'.  Return the NAME, Pop1996 and the X, Y coordinates of
  // each feature.
  EnumRows attrQueryRows;
  if ((hr = table.Search(L"Shape, CITY_NAME, POP1990", L"TYPE = 'city' AND OBJECTID < 10", true, attrQueryRows)) != S_OK)
  {
    wcout << "An error occurred while performing the attribute query." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Iterate through the returned rows.
  Row              attrQueryRow;
  wstring          cityName;
  int32            cityPop;
  PointShapeBuffer geometry;
  Point*           point;

  while (attrQueryRows.Next(attrQueryRow) == S_OK)
  {
    attrQueryRow.GetString(L"CITY_NAME", cityName);
    attrQueryRow.GetInteger(L"POP1990", cityPop);
    attrQueryRow.GetGeometry(geometry);
    geometry.GetPoint(point);

    wcout << cityName << '\t' << cityPop << '\t' << point->x << "," << point->y << endl;
  }
  attrQueryRows.Close(); // Close the EnumRows

  // Throw in a newline to separate the examples.
  wcout << '\n' << endl;

  // Perform a spatial query: find the names of every city within a
  // specified envelope.
  Envelope envelope;
  envelope.xMin = -118.219;
  envelope.yMin =  22.98;
  envelope.xMax = -117.988;
  envelope.yMax =  34.0;
  EnumRows spQueryRows;
  if ((hr = table.Search(L"CITY_NAME", L"", envelope, true, spQueryRows)) != S_OK)
  {
    wcout << "An error occurred while performing the spatial query." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Iterate through the returned rows.
  Row spQueryRow;
  while (spQueryRows.Next(spQueryRow) == S_OK)
  {
    spQueryRow.GetString(L"CITY_NAME", cityName);
    wcout << cityName << endl;
  }
  spQueryRows.Close(); // Close the EnumRows

  // Close the table
  if ((hr = geodatabase.CloseTable(table)) != S_OK)
  {
    wcout << "An error occurred while closing Cities." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Throw in a newline to separate the examples.
  wcout << '\n' << endl;
  wcout << "Querying compressed data...\n" << endl;

  // Open the compressed CitiesCDF table.
  Table tableCDF;
  if ((hr = geodatabase.OpenTable(L"CitiesCDF", tableCDF)) != S_OK)
  {
    wcout << "An error occurred while opening the table." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Perform a simple attribute query against a compressed table: find the names of 
  // every city with a 'TERM' value equal to 'City'.  Return the NAME, Pop1996 and 
  // the X, Y coordinates of each feature.
  EnumRows attrQueryRowsCDF;
  if ((hr = tableCDF.Search(L"Shape, CITY_NAME, POP1990", L"TYPE = 'city' AND OBJECTID < 10", true, attrQueryRowsCDF)) != S_OK)
  {
    wcout << "An error occurred while performing the attribute query." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Iterate through the returned rows.
  while (attrQueryRowsCDF.Next(attrQueryRow) == S_OK)
  {
    attrQueryRow.GetString(L"CITY_NAME", cityName);
    attrQueryRow.GetInteger(L"POP1990", cityPop);
    attrQueryRow.GetGeometry(geometry);
    geometry.GetPoint(point);

    wcout << cityName << '\t' << cityPop << '\t' << point->x << "," << point->y << endl;
  }
  attrQueryRowsCDF.Close(); // Close the EnumRows

  // Close the compressed table
  if ((hr = geodatabase.CloseTable(tableCDF)) != S_OK)
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
