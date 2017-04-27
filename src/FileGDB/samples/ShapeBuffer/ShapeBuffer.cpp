//
// Sample: ShapeBuffer
//
// Read and write all shape types.

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
#include <time.h>

#include <FileGDBAPI.h>

using namespace std;
using namespace FileGDBAPI;

// Tests for geometries
static fgdbError PointGeomTest(Geodatabase& geodatabase);
static fgdbError LineGeomTest(Geodatabase& geodatabase);
static fgdbError LineZGeomTest(Geodatabase& geodatabase);
static fgdbError PolygonGeomTest(Geodatabase& geodatabase);
static fgdbError MultipointGeomTest(Geodatabase& geodatabase);

int main()
{
  fgdbError hr;
  wstring   errorText;

  // Open the geodatabase.
  Geodatabase geodatabase;
  if ((hr = OpenGeodatabase(L"../data/Shapes.gdb", geodatabase)) != S_OK)
  {
    wcout << "An error occurred while opening the geodatabase." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Run the tests
  PointGeomTest(geodatabase);
  LineGeomTest(geodatabase);
  LineZGeomTest(geodatabase);
  PolygonGeomTest(geodatabase);
  MultipointGeomTest(geodatabase);

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

static fgdbError PointGeomTest(Geodatabase& geodatabase)
{
  fgdbError hr;
  wstring   errorText;

  wcout << '\n' << "Testing Point Shapes, Read" << '\n' << endl;

  // Open the point test table, cities.
  Table citiesTable;
  if ((hr = geodatabase.OpenTable(L"\\cities", citiesTable)) != S_OK)
  {
    wcout << "An error occurred while opening the table, cities." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  // Return all rows.
  EnumRows enumRows;
  if ((hr = citiesTable.Search(L"*", L"CITY_NAME = 'Woodinville'", true, enumRows)) != S_OK)
  {
    wcout << "An error occurred while performing the attribute query." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  // Get the first returned row.
  Row row;
  if ((hr = enumRows.Next(row)) != S_OK)
  {
    wcout << "An error occurred returning the first row." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  PointShapeBuffer pointGeometry;
  row.GetGeometry(pointGeometry);

  Point* point;
  pointGeometry.GetPoint(point);
  wcout << "Point test:" << endl;
  wcout << "x: " << point->x << " y: " << point->y << endl;

  enumRows.Close();

  // Close the cities table
  if ((hr = geodatabase.CloseTable(citiesTable)) != S_OK)
  {
    wcout << "An error occurred while closing the cities table." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  wcout << '\n' << "Testing Point Shapes, Write" << '\n' << endl;

  // Open the point test table, cities2.
  Table cities2Table;
  if ((hr = geodatabase.OpenTable(L"\\cities2", cities2Table)) != S_OK)
  {
    wcout << "An error occurred while opening the table, cities2." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  // Begin load only mode. This shuts off the update of all indexes. Set a write lock. Setting
  // the write lock is not really required in this case (only one insert), but will improve performance 
  // when bulk loading.
  cities2Table.LoadOnlyMode(true);
  cities2Table.SetWriteLock();

  // Create a new feature.
  Row cities2Row;
  cities2Table.CreateRowObject(cities2Row);

  PointShapeBuffer point2Geometry;
  Point*           point2;
  point2Geometry.Setup(FileGDBAPI::shapePoint);
  point2Geometry.GetPoint(point2);
  point2->x = point->x;
  point2->y = point->y;

  hr = cities2Row.SetGeometry(point2Geometry);

  if ((hr = cities2Table.Insert(cities2Row)) != S_OK)
  {
    wcout << "An error occurred while inserting a row." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }
  else
  {
    wcout << "Inserted a point." << endl;
  }

  // End load only mode. This rebuilds all indexes including the spatial index.
  // If all grid size values in the spatial index are zero, the values will be
  // calculated based on the existing data.
  cities2Table.LoadOnlyMode(false);
  cities2Table.FreeWriteLock();

  // Close the cities table.
  if ((hr = geodatabase.CloseTable(cities2Table)) != S_OK)
  {
    wcout << "An error occurred while closing the cities2 table." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  return S_OK;
}

static fgdbError LineGeomTest(Geodatabase& geodatabase)
{
  fgdbError hr;
  wstring   errorText;
  wcout << '\n' << "Testing Line Shapes, Read" << '\n' << endl;

  // Open the Line test table, intrstat.
  Table intrstatTable;
  if ((hr = geodatabase.OpenTable(L"\\intrstat", intrstatTable)) != S_OK)
  {
    wcout << "An error occurred while opening the table, cities." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  // Return all rows.
  EnumRows enumRows;
  if ((hr = intrstatTable.Search(L"*", L"ROUTE_NUM = 'I10'", true, enumRows)) != S_OK)
  {
    wcout << "An error occurred while performing the attribute query." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  // Get the first returned row.
  Row row;
  if ((hr = enumRows.Next(row)) != S_OK)
  {
    wcout << "An error occurred returning the first row." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  MultiPartShapeBuffer lineGeometry;
  row.GetGeometry(lineGeometry);

  int numPts;
  lineGeometry.GetNumPoints(numPts);

  int numParts;
  lineGeometry.GetNumParts(numParts);

  wcout << "Line test:" << endl;
  wcout << "Points: " << numPts << endl;
  wcout << "Parts: " << numParts << endl;

  int* parts;
  lineGeometry.GetParts(parts);

  Point* points;
  lineGeometry.GetPoints(points);

  int partCount = 0;
  for (int i = 0; i < numPts; i++)
  {
    if ((partCount < numParts) && (i == parts[partCount]))
    {
      wcout << "Part " << partCount << endl;
      partCount++;
    }
    wcout << i << " " <<  points[i].x << ", " << points[i].y << endl;
  }
  enumRows.Close();

  // Close the intrstat table.
  if ((hr = geodatabase.CloseTable(intrstatTable)) != S_OK)
  {
    wcout << "An error occurred while closing the intrstat table." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  wcout << '\n' << "Testing Line Shapes, Write" << '\n' << endl;

  // Open the line test table, intrstat2.
  Table intrstat2Table;
  if ((hr = geodatabase.OpenTable(L"\\intrstat2", intrstat2Table)) != S_OK)
  {
    wcout << "An error occurred while opening the table, intrstat2." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    //return E_FAIL;
  }

  // Begin load only mode. This shuts off the update of all indexes. Set a write lock. Setting
  // the write lock is not really required in this case (only one insert), but will improve performance 
  // when bulk loading.
  intrstat2Table.LoadOnlyMode(true);
  intrstat2Table.SetWriteLock();

  // Create a new feature.
  Row intrstat2Row;
  intrstat2Table.CreateRowObject(intrstat2Row);

  int numPts2   = numPts;
  int numParts2 = numParts;
  MultiPartShapeBuffer intrstat2Geometry;
  intrstat2Geometry.Setup(FileGDBAPI::shapePolyline, numParts2, numPts2);

  // Set the point array to the array from the read geometry.
  Point* points2;
  intrstat2Geometry.GetPoints(points2);

  for (int i = 0; i < numPts; i++)
  {
    points2[i].x = points[i].x;
    points2[i].y = points[i].y;
    if (i == (numPts - 1))
    {
      wcout << points2[i].x << ", " << points2[i].y << endl;
    }
  }

  // Set the parts array to the array from the read geometry.
  int* parts2;
  intrstat2Geometry.GetParts(parts2);
  for (int i = 0; i < numParts2; i++)
  {
    parts2[i] = parts[i];
  }

  intrstat2Geometry.CalculateExtent();

  hr = intrstat2Row.SetGeometry(intrstat2Geometry);

  if ((hr = intrstat2Table.Insert(intrstat2Row)) != S_OK)
  {
    wcout << "An error occurred while inserting a row." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }
  else
  {
    wcout << "Inserted a line." << endl;
  }

  // End load only mode. This rebuilds all indexes including the spatial index.
  // If all grid size values in the spatial index are zero, the values will be
  // calculated based on the existing data.
  intrstat2Table.LoadOnlyMode(false);
  intrstat2Table.FreeWriteLock();

  // Close the intrstat2 table
  if ((hr = geodatabase.CloseTable(intrstat2Table)) != S_OK)
  {
    wcout << "An error occurred while closing the intrstat2 table." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  return S_OK;
}

static fgdbError LineZGeomTest(Geodatabase& geodatabase)
{
  fgdbError hr;
  wstring   errorText;

  wcout << '\n' << "Testing Line Shapes with Zs" << '\n' << endl;

  // Open the Line with Zs test table, zvalues.
  Table zvaluesTable;
  if ((hr = geodatabase.OpenTable(L"\\zvalues", zvaluesTable)) != S_OK)
  {
    wcout << "An error occurred while opening the table, zvalues." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  // Return all rows.
  EnumRows enumRows;
  if ((hr = zvaluesTable.Search(L"*", L"OBJECTID = 1", true, enumRows)) != S_OK)
  {
    wcout << "An error occurred while performing the attribute query." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  // Get the first returned row.
  Row row;
  if ((hr = enumRows.Next(row)) != S_OK)
  {
    wcout << "An error occurred returning the first row." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  MultiPartShapeBuffer lineZGeometry;
  row.GetGeometry(lineZGeometry);

  int     numPts;
  lineZGeometry.GetNumPoints(numPts);

  int     numParts;
  lineZGeometry.GetNumParts(numParts);
  wcout << "Line with Z test:" << endl;
  wcout << "Points: " << numPts << endl;
  wcout << "Parts: " << numParts << endl;

  int*    parts;
  lineZGeometry.GetParts(parts);

  Point*  points;
  lineZGeometry.GetPoints(points);

  double* zArray;
  lineZGeometry.GetZs(zArray);

  int     partCount = 0;
  for (int i = 0; i < numPts; i++)
  {
    if ((partCount < numParts) && (i == parts[partCount]))
    {
      wcout << "Part " << partCount << endl;
      partCount++;
    }
    wcout << i << " " <<  points[i].x << ", " << points[i].y <<  ", " << zArray[i] << endl;
  }
  enumRows.Close();

  // Close the zvalues table
  if ((hr = geodatabase.CloseTable(zvaluesTable)) != S_OK)
  {
    wcout << "An error occurred while closing the zvalues table." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  wcout << '\n' << "Testing Line Shapes, Write" << '\n' << endl;

  // Open the line Z test table, zvalues2.
  Table zvalues2Table;
  if ((hr = geodatabase.OpenTable(L"\\zvalues2", zvalues2Table)) != S_OK)
  {
    wcout << "An error occurred while opening the table, zvalues2." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    //return E_FAIL;
  }

  // Get the current extent.
  Envelope extent;
  zvalues2Table.GetExtent(extent);
  wcout << "xMin: " << extent.xMin << endl;
  wcout << "yMin: " << extent.yMin << endl;
  wcout << "xMax: " << extent.xMax << endl;
  wcout << "yMax: " << extent.yMax << endl;
  wcout << "zMin: " << extent.zMin << endl;
  wcout << "zMax: " << extent.zMax << endl;

  // Begin load only mode. Set a write lock. Setting
  // the write lock is not really required in this case (only one insert), but will improve performance 
  // when bulk loading.
  zvalues2Table.LoadOnlyMode(true);
  zvalues2Table.SetWriteLock();

  // Create a new feature.
  Row zvalues2Row;
  zvalues2Table.CreateRowObject(zvalues2Row);

  int numPts2   = numPts;
  int numParts2 = numParts;
  MultiPartShapeBuffer zvalues2Geometry;
  zvalues2Geometry.Setup(FileGDBAPI::shapePolylineZ, numParts2, numPts2);

  Point*  points2;
  zvalues2Geometry.GetPoints(points2);

  double* zArray2;
  zvalues2Geometry.GetZs(zArray2);

  // Set the points array to the array from the read geometry.
  for (int i = 0; i < numPts; i++)
  {
    points2[i].x = points[i].x;
    points2[i].y = points[i].y;
    zArray2[i]   = zArray[i];
  }

  // Set the parts array to the array from the read geometry.
  int* parts2;
  zvalues2Geometry.GetParts(parts2);
  for (int i = 0; i < numParts2; i++)
  {
    parts2[i] = parts[i];
  }

  zvalues2Geometry.CalculateExtent();

  hr = zvalues2Row.SetGeometry(zvalues2Geometry);

  if ((hr = zvalues2Table.Insert(zvalues2Row)) != S_OK)
  {
    wcout << "An error occurred while inserting a row." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }
  else
  {
    wcout << "Inserted a line with z's." << endl;
  }

  // End load only mode.
  zvalues2Table.LoadOnlyMode(false);
  zvalues2Table.FreeWriteLock();

  // Get the updated extent.
  zvalues2Table.GetExtent(extent);
  wcout << "xMin: " << extent.xMin << endl;
  wcout << "yMin: " << extent.yMin << endl;
  wcout << "xMax: " << extent.xMax << endl;
  wcout << "yMax: " << extent.yMax << endl;
  wcout << "zMin: " << extent.zMin << endl;
  wcout << "zMax: " << extent.zMax << endl;

  // Close the zvalues2 table
  if ((hr = geodatabase.CloseTable(zvalues2Table)) != S_OK)
  {
    wcout << "An error occurred while closing the zvalues2 table." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  return S_OK;
}

static fgdbError PolygonGeomTest(Geodatabase& geodatabase)
{
  fgdbError hr;
  wstring   errorText;
  wcout << '\n' << "Testing Polygon Shapes" << '\n' << endl;

  // Open the Polygon test table, states.
  Table statesTable;
  if ((hr = geodatabase.OpenTable(L"\\states", statesTable)) != S_OK)
  {
    wcout << "An error occurred while opening the table, states." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  // Get back a single row.
  EnumRows enumRows;
  if ((hr = statesTable.Search(L"*", L"STATE_NAME = 'Rhode Island'", true, enumRows)) != S_OK)
  {
    wcout << "An error occurred while performing the attribute query." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  // Get the first returned row.
  Row row;
  if ((hr = enumRows.Next(row)) != S_OK)
  {
    wcout << "An error occurred returning the first row." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  MultiPartShapeBuffer statesGeometry;
  row.GetGeometry(statesGeometry);

  int numPts;
  statesGeometry.GetNumPoints(numPts);

  int numParts;
  statesGeometry.GetNumParts(numParts);
  wcout << "Polygon test:" << endl;
  wcout << "Points: " << numPts << endl;
  wcout << "Parts: " << numParts << endl;

  double xMin, xMax,
    yMin, yMax;

  double* xyExtent;
  statesGeometry.GetExtent(xyExtent);
  xMin = xyExtent[0];
  yMin = xyExtent[1];
  xMax = xyExtent[2];
  yMax = xyExtent[3]; 
  wcout << "xMin: " << xMin << endl;
  wcout << "yMin: " << yMin << endl;
  wcout << "xMax: " << xMax << endl;
  wcout << "yMax: " << yMax << endl;

  int* parts;
  statesGeometry.GetParts(parts);

  Point* points;
  statesGeometry.GetPoints(points);

  int partCount = 0;
  for (int i = 0; i < numPts; i++)
  {
    if ((partCount < numParts) && (i == parts[partCount]))
    {
      wcout << "Part " << partCount << endl;
      partCount++;
    }
    wcout << i << " " <<  points[i].x << ", " << points[i].y << endl;
  }
  enumRows.Close();

  // Close the states table
  if ((hr = geodatabase.CloseTable(statesTable)) != S_OK)
  {
    wcout << "An error occurred while closing the states table." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  wcout << '\n' << "Testing Polygon Shapes, Write" << '\n' << endl;

  // Open the polygon test table, states2.
  Table states2Table;
  if ((hr = geodatabase.OpenTable(L"\\states2", states2Table)) != S_OK)
  {
    wcout << "An error occurred while opening the table, states2." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  // Begin load only mode. This shuts off the update of all indexes. Set a write lock. Setting
  // the write lock is not really required in this case (only one insert), but will improve performance 
  // when bulk loading.
  states2Table.LoadOnlyMode(true);
  states2Table.SetWriteLock();

  // Create a new feature.
  Row states2Row;
  states2Table.CreateRowObject(states2Row);

  int numPts2   = numPts;
  int numParts2 = numParts;
  MultiPartShapeBuffer states2Geometry;
  states2Geometry.Setup(FileGDBAPI::shapePolygon, numParts, numPts);

  // Set the point array to the array from the read geometry.
  Point* points2;
  states2Geometry.GetPoints(points2);

  for (int i = 0; i < numPts; i++)
  {
    points2[i].x = points[i].x;
    points2[i].y = points[i].y;
  }

  // Set the part array to the array from the read geometry.
  int* parts2;
  states2Geometry.GetParts(parts2);
  for (int i = 0; i < numParts2; i++)
  {
    parts2[i] = parts[i];
  }

  states2Geometry.CalculateExtent();

  hr = states2Row.SetGeometry(states2Geometry);

  if ((hr = states2Table.Insert(states2Row)) != S_OK)
  {
    wcout << "An error occurred while inserting a row." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }
  else
  {
    wcout << "Inserted a polygon." << endl;
  }

  // End load only mode. This rebuilds all indexes including the spatial index.
  // If all grid size values in the spatial index are zero, the values will be
  // calculated based on the existing data.
  states2Table.LoadOnlyMode(false);
  states2Table.FreeWriteLock();

  // Close the states2 table.
  if ((hr = geodatabase.CloseTable(states2Table)) != S_OK)
  {
    wcout << "An error occurred while closing the states2 table." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  return S_OK;
}

static fgdbError MultipointGeomTest(Geodatabase& geodatabase)
{
  fgdbError hr;
  wstring   errorText;
  wcout << '\n' << "Testing Multipoint" << '\n' << endl;

  // Open the Line with Zs test table, multipoint.
  Table multipointTable;
  if ((hr = geodatabase.OpenTable(L"\\multipoint", multipointTable)) != S_OK)
  {
    wcout << "An error occurred while opening the table, multipoint." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  // Return all rows.
  EnumRows enumRows;
  if ((hr = multipointTable.Search(L"*", L"OBJECTID = 1", true, enumRows)) != S_OK)
  {
    wcout << "An error occurred while performing the attribute query." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  // Get the first returned row.
  Row row;
  if ((hr = enumRows.Next(row)) != S_OK)
  {
    wcout << "An error occurred returning the first row." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  MultiPointShapeBuffer multipointGeometry;
  row.GetGeometry(multipointGeometry);

  int numPts;
  multipointGeometry.GetNumPoints(numPts);
  wcout << "Multipoint test:" << endl;
  wcout << "Points: " << numPts << endl;

  Point* points;
  multipointGeometry.GetPoints(points);

  double* zArray;
  multipointGeometry.GetZs(zArray);

  for (int i = 0; i < numPts; i++)
  {
    wcout << i << " " <<  points[i].x << ", " << points[i].y <<  ", " << zArray[i] << endl;
  }
  enumRows.Close();

  // Close the multipoint table
  if ((hr = geodatabase.CloseTable(multipointTable)) != S_OK)
  {
    wcout << "An error occurred while closing the multipoint table." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  wcout << '\n' << "Testing Multipoint Shapes, Write" << '\n' << endl;

  // Open the multipoint test table, multipoint2.
  Table multipoint2Table;
  if ((hr = geodatabase.OpenTable(L"\\multipoint2", multipoint2Table)) != S_OK)
  {
    wcout << "An error occurred while opening the table, multipoint2." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    //return E_FAIL;
  }

  // Enter load only mode. This shuts off the update of all indexes. Set a write lock. Setting
  // the write lock is not really required in this case (only one insert), but will improve performance 
  // when bulk loading.
  multipoint2Table.LoadOnlyMode(true);
  multipoint2Table.SetWriteLock();

  // Create a new feature.
  Row multipoint2Row;
  multipoint2Table.CreateRowObject(multipoint2Row);

  MultiPointShapeBuffer multipoint2Geometry;
  multipoint2Geometry.Setup(FileGDBAPI::shapeMultipointZ,numPts);

  Point* points2;
  multipoint2Geometry.GetPoints(points2);

  double* zArray2;
  multipoint2Geometry.GetZs(zArray2);

  for (int i = 0; i < numPts; i++)
  {
    points2[i].x = points[i].x;
    points2[i].y = points[i].y;
    zArray2[i]   = zArray[i];
  }

  multipoint2Geometry.CalculateExtent();

  hr = multipoint2Row.SetGeometry(multipoint2Geometry);

  if ((hr = multipoint2Table.Insert(multipoint2Row)) != S_OK)
  {
    wcout << "An error occurred while inserting a row." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }
  else
  {
    wcout << "Inserted a multipoint." << endl;
  }

  // End load only mode. This rebuilds all indexes including the spatial index.
  // If all grid size values in the spatial index are zero, the values will be
  // calculated based on the existing data.
  multipoint2Table.LoadOnlyMode(false);
  multipoint2Table.FreeWriteLock();

  // Close the multipoint2 table
  if ((hr = geodatabase.CloseTable(multipoint2Table)) != S_OK)
  {
    wcout << "An error occurred while closing the multipoint table." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return E_FAIL;
  }

  return S_OK;
}