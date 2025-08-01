//
// Sample: Editing
//
// Demonstrates how to create new rows in a table, how to modify existing rows,
// and how to delete rows.  Prior to running this example, copy Editing.gdb from
// the data directory to the working directory.

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

using namespace std;
using namespace FileGDBAPI;

int main()
{
  fgdbError hr;
  wstring   errorText;

  // Open the geodatabase.
  Geodatabase geodatabase;
  if ((hr = OpenGeodatabase(L"../data/Editing.gdb", geodatabase)) != S_OK)
  {
    wcout << "An error occurred while opening the geodatabase." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Open the Cities table.
  Table table;
  if ((hr = geodatabase.OpenTable(L"\\Cities", table)) != S_OK)
  {
    wcout << "An error occurred while opening the table." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Create a new feature for Cabazon.
  Row cabazonRow;
  table.CreateRowObject(cabazonRow);

  // Set the row's attributes.
  cabazonRow.SetString(L"AREANAME", L"Cabazon");
  cabazonRow.SetString(L"CLASS", L"town");
  cabazonRow.SetInteger(L"POP2000", 2939); // 2007

  // Create and assign a point geometry.
  PointShapeBuffer cabazonGeom;
  hr = cabazonGeom.Setup(shapePoint);

  Point* point;
  hr = cabazonGeom.GetPoint(point);

  point->x = -116.78443;
  point->y =   33.919902;

  cabazonRow.SetGeometry(cabazonGeom);

  // Store the row.
  if ((hr = table.Insert(cabazonRow)) != S_OK)
  {
    wcout << "An error occurred while inserting a row." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  else
  {
    wcout << "Inserted two strings, one integer and a point." << endl;
  }

  // Get the row for Apple Valley by querying the table. Make sure to disable
  // recycling when intending to edit rows.
  EnumRows avQueryResult;
  if ((hr = table.Search(L"*", L"AREANAME = 'Apple Valley'", false, avQueryResult)) != S_OK)
  {
    wcout << "An error occurred while performing attribute search." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  Row avRow;
  if ((hr = avQueryResult.Next(avRow)) != S_OK)
  {
    wcout << "An error occurred while querying the table." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Modify the "CLASS" attribute and persist the change.
  avRow.SetString(L"CLASS", L"city");
  if ((hr = table.Update(avRow)) != S_OK)
  {
    wcout << "An error occurred while updating a row." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  avQueryResult.Close(); // Close the EnumRows

  wcout << "Updated the CLASS field." << endl;

  // Delete the rows that fall into an envelope and have a null elevation value.
  // As with updating, make sure recycling is disabled when deleting rows.
  Envelope envelope;
  envelope.xMin = -117.4;
  envelope.yMin =   33.65;
  envelope.xMax = -116.8;
  envelope.yMax =   33.86;
  EnumRows deleteRows;
  if ((hr = table.Search(L"*", L"", envelope, false, deleteRows)) != S_OK)
  {
    wcout << "An error occurred while performing spatial search." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  Row     deleteRow;
  wstring cityName;
  bool    isNull;
  while ((hr = deleteRows.Next(deleteRow)) == S_OK)
  {
    deleteRow.GetString(L"AREANAME", cityName);
    deleteRow.IsNull(L"ELEVATION", isNull);
    if (isNull)
    {
      wcout << "Deleting: " << cityName << endl;
      table.Delete(deleteRow);
    }
  }
  deleteRows.Close(); // Close the EnumRows

  // Close the table.
  if ((hr = geodatabase.CloseTable(table)) != S_OK)
  {
    wcout << "An error occurred while closing Cities." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Compact the geodatabase. Recomended after bulk updates.
  if ((hr = geodatabase.CompactDatabase()) != S_OK)
  {
    wcout << "An error occurred while compacting the geodatabase." << endl;
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
