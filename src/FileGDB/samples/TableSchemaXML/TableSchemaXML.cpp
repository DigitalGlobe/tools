//
// Sample: TableSchemaXML
//
// Demonstrates how to create a new table using an XML definition, then make
// several modifications to the table's schema, including adding a field,
// adding an index, assigning an alias name to field, and enabling subtypes.

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
#include <fstream>
#include <iostream>

#include <FileGDBAPI.h>

#include "TableSchemaXML.h"

using namespace std;
using namespace FileGDBAPI;

int main()
{
  // Delete the geodatabase in the current directory if it's already there.
  fgdbError   hr;
  wstring     errorText;
  Geodatabase geodatabase;
  hr = DeleteGeodatabase(L"../TableSchemaXML/TableSchemaXMLDemo.gdb");
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
  if ((hr = CreateGeodatabase(L"../TableSchemaXML/TableSchemaXMLDemo.gdb", geodatabase)) != S_OK)
  {
    wcout << "An error occurred while creating the geodatabase." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  wcout << "The geodatabase has been created." << endl;

  // Load a feature class XML definition into a string. An example of a data
  // element is provided in the sample as "Streets.xml".
  string featureClassDef;
  string defLine;
  ifstream defFile("../TableSchemaXML/Streets.xml");
  while (getline(defFile, defLine))
    featureClassDef.append(defLine + "\n");
  defFile.close();

  // create a domain
  std::string laneDomain;
  CreateDomainDefinition(laneDomain);
  int recordCount = 0;
  if ((hr = geodatabase.CreateDomain(laneDomain)) != S_OK)
  {
    wcout << "An error occurred while creating the lane domain." << endl;
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
  wcout << "The Lane domain has been created." << endl;

  // Create the new table.
  Table streetsTable;
  recordCount = 0;
  if ((hr = geodatabase.CreateTable(featureClassDef,L"", streetsTable)) != S_OK)
  {
    wcout << "An error occurred while creating the table." << endl;
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
  wcout << "The streets table has been created." << endl;

  // Add a new field to the table. Populate the string using a helper method.
  string streetTypeFieldDef;
  CreateStreetTypeField(streetTypeFieldDef);
  recordCount = 0;
  if ((hr = streetsTable.AddField(streetTypeFieldDef)) != S_OK)
  {
    wcout << "An error occurred while adding the StreetType field." << endl;
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
  wcout << "The StreetType field has been added." << endl;

  if ((hr = streetsTable.DeleteField(L"SpeedLimit")) != S_OK)
  {
    wcout << "An error occurred while deleting the SpeedLimit field." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  wcout << "The SpeedLimit field has been deleted." << endl;

  // Add a new index to the table.  Populate the string using a helper method.
  string indexDef;
  CreateStreetTypeIndex(indexDef);
  if ((hr = streetsTable.AddIndex(indexDef)) != S_OK)
  {
    wcout << "An error occurred while adding an index to the StreetType field." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  wcout << "The StreetType field has been indexed." << endl;

  // Create a new subtype.  Populate the string using a helper method.
  string subtypeDef;
  CreateSubtypeDefinition(subtypeDef);
  if ((hr = streetsTable.EnableSubtypes(L"StreetType", subtypeDef)) != S_OK)
  {
    wcout << "An error occurred while enabling the subtype." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  wcout << "Enabled the Trunk Highways subtype" << endl;

  // Get the Default Subtype Code.
  int defaultCode;
  if ((hr = streetsTable.GetDefaultSubtypeCode(defaultCode)) != S_OK)
  {
    wcout << "An error occurred while getting default subtype code." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  wcout << "The Default Subtype code is: " << defaultCode << endl;

  // Set the Default Subtype Code.
  defaultCode = 87;
  if ((hr = streetsTable.SetDefaultSubtypeCode(defaultCode)) != S_OK)
  {
    wcout << "An error occurred while setting default subtype code." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  wcout << "The Default Subtype code is now: " << defaultCode << endl;

  // Alter a subtype. Populate the string using a helper method.
  string alteredSubtypeDef;
  CreateSubtypeDefinitionAltered(alteredSubtypeDef);
  if ((hr = streetsTable.AlterSubtype(alteredSubtypeDef)) != S_OK)
  {
    wcout << "An error occurred while altering the subtype." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  wcout << "Altered the Trunk Highways subtype" << endl;

  // Create another subtype. Populate the string using a helper method.
  string subtypeDef2;
  CreateSubtypeDefinition2(subtypeDef2);
  if ((hr = streetsTable.CreateSubtype(subtypeDef2)) != S_OK)
  {
    wcout << "An error occurred while getting default subtype code." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  wcout << "Added Local Streets subtype" << endl;

  // Delete the Local Streets subtype.
  if ((hr = streetsTable.DeleteSubtype(L"Local Streets")) != S_OK)
  {
    wcout << "An error occurred while deleting the Local Streets subtype code." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }
  wcout << "Deleted the Local Streets subtype" << endl;


  // disable subtypes
  if ((hr = streetsTable.DisableSubtypes()) != S_OK)
  {
    wcout << "An error occurred while disabling subtypes." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Close the table.
  if ((hr = geodatabase.CloseTable(streetsTable)) != S_OK)
  {
    wcout << "An error occurred while closing Streets." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  // Close the geodatabase.
  if ((hr = CloseGeodatabase(geodatabase)) != S_OK)
  {
    wcout << "An error occurred while closing the geodatabase." << endl;
    ErrorInfo::GetErrorDescription(hr, errorText);
    wcout << errorText << "(" << hr << ")." << endl;
    return -1;
  }

  return 0;
}

// A helper method for creating a field XML definition.
static void CreateStreetTypeField(std::string& fieldDef)
{
  // Create a string containing an XML field definition.
  fieldDef.clear();
  fieldDef.append("<esri:Field xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:esri='http://www.esri.com/schemas/ArcGIS/10.1' xsi:type='esri:Field'>");
  fieldDef.append("<Name>StreetType</Name>");
  fieldDef.append("<Type>esriFieldTypeSmallInteger</Type>");
  fieldDef.append("<IsNullable>true</IsNullable>");
  fieldDef.append("<Length>2</Length>");
  fieldDef.append("<Precision>0</Precision>");
  fieldDef.append("<Scale>0</Scale>");
  fieldDef.append("<AliasName>day</AliasName>");
  fieldDef.append("<ModelName>night</ModelName>");
  fieldDef.append("<DefaultValue xsi:type=\"xs:short\">1</DefaultValue>");
  fieldDef.append("</esri:Field>");
}

// A helper method for creating an index XML definition.
static void CreateStreetTypeIndex(std::string& indexDef)
{
  indexDef.append("<esri:Index xsi:type='esri:Index' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:esri='http://www.esri.com/schemas/ArcGIS/10.1'>");
  indexDef.append("<Name>StreetTypeIdx</Name>");
  indexDef.append("<IsUnique>false</IsUnique>");
  indexDef.append("<IsAscending>true</IsAscending>");
  indexDef.append("<Fields xsi:type='esri:Fields'>");
  indexDef.append("<FieldArray xsi:type='esri:ArrayOfField'>");
  indexDef.append("<Field xsi:type='esri:Field'>");
  indexDef.append("<Name>StreetType</Name>");
  indexDef.append("<Type>esriFieldTypeSmallInteger</Type>");
  indexDef.append("<IsNullable>false</IsNullable>");
  indexDef.append("<Length>2</Length>");
  indexDef.append("<Precision>0</Precision>");
  indexDef.append("<Scale>0</Scale>");
  indexDef.append("<Required>true</Required>");
  indexDef.append("<Editable>false</Editable>");
  indexDef.append("<DomainFixed>true</DomainFixed>");
  indexDef.append("<AliasName>StreetType</AliasName>");
  indexDef.append("<ModelName>StreetType</ModelName>");
  indexDef.append("</Field>");
  indexDef.append("</FieldArray>");
  indexDef.append("</Fields>");
  indexDef.append("</esri:Index>");
}

// A helper method for creating a subtype XML definition.
static void CreateSubtypeDefinition(std::string& subtypeDef)
{
  // Create a string containing an XML subtype definition.
  subtypeDef.clear();
  subtypeDef.append("<esri:Subtype xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:esri='http://www.esri.com/schemas/ArcGIS/10.1\' xsi:type='esri:Subtype'>");
  subtypeDef.append("<SubtypeName>Trunk Highway</SubtypeName>");
  subtypeDef.append("<SubtypeCode>99</SubtypeCode>");
  subtypeDef.append("<FieldInfos xsi:type='esri:ArrayOfSubtypeFieldInfo'>");
  subtypeDef.append("<SubtypeFieldInfo xsi:type='esri:SubtypeFieldInfo'>");
  subtypeDef.append("<FieldName>LaneCount</FieldName>");
  subtypeDef.append("<DomainName>LaneDomain</DomainName>");
  subtypeDef.append("<DefaultValue xsi:type='xs:short'>10</DefaultValue>");
  subtypeDef.append("</SubtypeFieldInfo>");
  subtypeDef.append("</FieldInfos>");
  subtypeDef.append("</esri:Subtype>");
}

// A helper method for creating an altered subtype XML definition.
static void CreateSubtypeDefinitionAltered(std::string& subtypeDef)
{
  // Create a string containing an XML subtype definition.
  subtypeDef.clear();
  subtypeDef.append("<esri:Subtype xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:esri=\"http://www.esri.com/schemas/ArcGIS/10.1\" xsi:type=\"esri:Subtype\">");
  subtypeDef.append("<SubtypeName>Trunk Highway</SubtypeName>");
  subtypeDef.append("<SubtypeCode>98</SubtypeCode>");
  subtypeDef.append("<FieldInfos xsi:type=\"esri:ArrayOfSubtypeFieldInfo\">");
  subtypeDef.append("<SubtypeFieldInfo xsi:type=\"esri:SubtypeFieldInfo\">");
  subtypeDef.append("<FieldName>LaneCount</FieldName>");
  subtypeDef.append("<DomainName>LaneDomain</DomainName>");
  subtypeDef.append("<DefaultValue xsi:type=\"xs:short\">8</DefaultValue>");
  subtypeDef.append("</SubtypeFieldInfo>");
  subtypeDef.append("</FieldInfos>");
  subtypeDef.append("</esri:Subtype>");
}

// A helper method for creating a subtype XML definition.
static void CreateSubtypeDefinition2(std::string& subtypeDef)
{
  // Create a string containing an XML subtype definition.
  subtypeDef.clear();
  subtypeDef.append("<esri:Subtype xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:esri=\"http://www.esri.com/schemas/ArcGIS/10.1\" xsi:type=\"esri:Subtype\">");
  subtypeDef.append("<SubtypeName>Local Streets</SubtypeName>");
  subtypeDef.append("<SubtypeCode>4</SubtypeCode>");
  subtypeDef.append("<FieldInfos xsi:type=\"esri:ArrayOfSubtypeFieldInfo\">");
  subtypeDef.append("<SubtypeFieldInfo xsi:type=\"esri:SubtypeFieldInfo\">");
  subtypeDef.append("<FieldName>LaneCount</FieldName>");
  subtypeDef.append("<DomainName>LaneDomain</DomainName>");
  subtypeDef.append("<DefaultValue xsi:type=\"xs:short\">2</DefaultValue>");
  subtypeDef.append("</SubtypeFieldInfo>");
  subtypeDef.append("</FieldInfos>");
  subtypeDef.append("</esri:Subtype>");
}

static void CreateDomainDefinition(std::string& domainDef)
{
  // Create a string containing an XML domain definition.
  domainDef.clear();
  domainDef.append("<esri:Domain xsi:type='esri:RangeDomain' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:esri='http://www.esri.com/schemas/ArcGIS/10.1'>");
  domainDef.append("<DomainName>LaneDomain</DomainName>");
  domainDef.append("<FieldType>esriFieldTypeSmallInteger</FieldType>");
  domainDef.append("<MergePolicy>esriMPTDefaultValue</MergePolicy>");
  domainDef.append("<SplitPolicy>esriSPTDefaultValue</SplitPolicy>");
  domainDef.append("<Description>The number of lanes on a road</Description>");
  domainDef.append("<Owner></Owner>");
  domainDef.append("<MaxValue xsi:type='xs:short'>12</MaxValue>");
  domainDef.append("<MinValue xsi:type='xs:short'>1</MinValue>");
  domainDef.append("</esri:Domain>");
}