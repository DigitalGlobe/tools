//
// Sample: ExecutingSQL
//
// Demonstrates how to use ExecuteSQL to perform standard SQL requests on a file
// geodatabase.  Prior to running this example, copy the ExecuteSQL.gdb from the
// data directory to the working directory.

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
#include <time.h>

#include <FileGDBAPI.h>

using namespace std;
using namespace FileGDBAPI;

int main()
{
  // Open the geodatabase.
  fgdbError   hr;
  wstring     errorText;
  Geodatabase geodatabase;
  if ((hr = OpenGeodatabase(L"../data/ExecuteSQL.gdb", geodatabase)) != S_OK)
  {
    wcout << "An error occurred while opening the geodatabase." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Perform a simple SELECT: find the names of every city with a 'TYPE' value
  // equal to 'city'.
  wstring sqlStatement(L"SELECT CITY_NAME, POP1990 FROM Cities WHERE TYPE = 'city' AND OBJECTID < 10");
  EnumRows attrQueryRows;
  if ((hr = geodatabase.ExecuteSQL(sqlStatement, true, attrQueryRows)) != S_OK)
  {
    wcout << "An error occurred while performing the attribute query." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Iterate through the returned rows.
  Row     attrQueryRow;
  int32   cityPop;
  wstring cityName;
  while (attrQueryRows.Next(attrQueryRow) == S_OK)
  {
    attrQueryRow.GetInteger(L"POP1990", cityPop);
    attrQueryRow.GetString(L"CITY_NAME", cityName);

    wcout << cityName << '\t' << cityPop << endl;
  }

  // SELECT * - Return all fields.
  sqlStatement.assign(L"SELECT * FROM Cities WHERE TYPE = 'city' AND OBJECTID < 10");
  if ((hr = geodatabase.ExecuteSQL(sqlStatement, true, attrQueryRows)) != S_OK)
  {
    wcout << "An error occurred while performing the attribute query." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Get the field type and name from the row enumerator.
  FieldInfo fieldInfo;
  attrQueryRows.GetFieldInformation(fieldInfo);

  int       fieldCount;
  FieldType fieldType;
  wstring   fieldName;

  // Iterate through the returned rows printing out all field values.
  short     shortField;
  int32     longField;
  float     floatField;
  double    doubleField;
  string    stringField;
  wstring   wstringField;
  tm        dateTimeField;
  char      datetime[80];
  Guid      globalIDField;
  Guid      guidField;
  wstring   strGuid;
  wstring   strGlobalID;
  bool      isNull;

  while (attrQueryRows.Next(attrQueryRow) == S_OK)
  {
    fieldInfo.GetFieldCount(fieldCount);
    for (long fieldNumber = 0; fieldNumber < fieldCount; fieldNumber++)
    {
      fieldInfo.GetFieldType(fieldNumber, fieldType);
      fieldInfo.GetFieldName(fieldNumber, fieldName);
      attrQueryRow.IsNull(fieldName, isNull);
      if (!isNull)
      {
        switch (fieldType)
        {
          case fieldTypeSmallInteger:
            attrQueryRow.GetShort(fieldName, shortField);
            wcout <<  shortField << endl;
          break;

          case fieldTypeInteger:
            attrQueryRow.GetInteger(fieldName, longField);
            wcout << longField << '\t';
          break;

          case fieldTypeSingle:
            attrQueryRow.GetFloat(fieldName, floatField);
            wcout << floatField << '\t';
          break;

          case fieldTypeDouble:
            attrQueryRow.GetDouble(fieldName, doubleField);
            wcout << doubleField << '\t';
          break;

          case fieldTypeString:
            attrQueryRow.GetString(fieldName, wstringField);
            wcout << wstringField << '\t';
          break;

          case fieldTypeDate:
            attrQueryRow.GetDate(fieldName, dateTimeField);
            strftime(datetime,80,"%a %b %d %I:%M:%S%p %Y", &dateTimeField);
            wcout << datetime << '\t';
          break;

          case fieldTypeOID:
            attrQueryRow.GetOID(longField);
            wcout << longField << '\t';
          break;

          case fieldTypeGeometry:
            wcout << "Geometry" << '\t';
          break;

          case fieldTypeBlob:
            wcout << "Blob" << '\t';
          break;

          case fieldTypeGUID:
            attrQueryRow.GetGUID(fieldName, guidField);
            guidField.ToString(strGuid);
            wcout << strGuid << '\t';
          break;

          case fieldTypeGlobalID:
            attrQueryRow.GetGlobalID(globalIDField);
            globalIDField.ToString(strGlobalID);
            wcout << strGlobalID << '\t';
          break;

          default:
          break;
        }
      }
      else
      {
        wcout << "null" << '\t';
      }
    }

    wcout << endl;
  }

  // Perform an UPDATE
  sqlStatement.assign(L"UPDATE CitiesNoGeo SET POP1990 = 65678 WHERE STATE_CITY = '0659962'");
  if ((hr = geodatabase.ExecuteSQL(sqlStatement, true, attrQueryRows)) != S_OK)
  {
    wcout << "An error occurred while performing the update." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  sqlStatement.assign(L"SELECT CITY_NAME, POP1990 FROM CitiesNoGeo WHERE STATE_CITY = '0659962'");
  if ((hr = geodatabase.ExecuteSQL(sqlStatement, true, attrQueryRows)) != S_OK)
  {
    wcout << "An error occurred while performing the attribute query." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Iterate through the returned rows.
  while (attrQueryRows.Next(attrQueryRow) == S_OK)
  {
    attrQueryRow.GetInteger(L"POP1990", cityPop);
    attrQueryRow.GetString(L"CITY_NAME", cityName);

    wcout << "Changed the population of " << cityName << " from 60394 to "  << cityPop << endl;
  }

  // Perform another UPDATE
  sqlStatement.assign(L"UPDATE CitiesNoGeo SET POP1990 = 60394 WHERE STATE_CITY = '0659962'");
  if ((hr = geodatabase.ExecuteSQL(sqlStatement, true, attrQueryRows)) != S_OK)
  {
    wcout << "An error occurred while performing the update." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  sqlStatement.assign(L"SELECT CITY_NAME, POP1990 FROM CitiesNoGeo WHERE STATE_CITY = '0659962'");
  if ((hr = geodatabase.ExecuteSQL(sqlStatement, true, attrQueryRows)) != S_OK)
  {
    wcout << "An error occurred while performing the attribute query." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Iterate through the returned rows.
  while (attrQueryRows.Next(attrQueryRow) == S_OK)
  {
    attrQueryRow.GetInteger(L"POP1990", cityPop);
    attrQueryRow.GetString(L"CITY_NAME", cityName);

    wcout << "Changed the population of " << cityName << " back to "  << cityPop << endl;
  }

  attrQueryRows.Close(); // Close the EnumRows

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
