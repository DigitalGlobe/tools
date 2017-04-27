using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Esri.FileGDB;
using System.IO;

namespace TableSchema
{
    //
    // Sample: TableSchema
    //
    // Demonstrates how to create a new table , then make
    // several modifications to the table's schema, including adding a field,
    // adding an index, assigning an alias name to field, and enabling subtypes.
    //
    //

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
  class TableSchema
  {
    static void Main(string[] args)
    {
      try
      {
        try
        {
          // Delete the geodatabase in the current directory if it's already there. -2147024893
          Geodatabase.Delete("../TableSchema/TableSchemaDemo.gdb");
          Console.WriteLine("The geodatabase has been deleted");
        }
        catch (FileGDBException ex)
        {
          if (ex.ErrorCode == -2147024893)
            Console.WriteLine("The geodatabase does not exist, no need to delete");
          else
          {
            Console.WriteLine("{0} - {1}", ex.Message, ex.ErrorCode);
            return;
          }
        }
        catch (Exception ex)
        {
          Console.WriteLine("An error occurred while deleting the geodatabase.  " + ex.Message);
          return;
        }

        Geodatabase geodatabase = Geodatabase.Create("../TableSchema/TableSchemaDemo.gdb");
        Console.WriteLine("The geodatabase has been created.");

        // Load a feature class XML definition into a string. An example of a
        // data element is provided in the sample as "Streets.xml".
        string featureClassDef = "";
        using (StreamReader sr = new StreamReader("../../Samples/TableSchemaXML/Streets.xml"))
        {
          while (sr.Peek() >= 0)
          {
            featureClassDef += sr.ReadLine() + "\n";
          }
          sr.Close();
        }
        Table streetsTable = geodatabase.CreateTable(featureClassDef, "");
        Console.WriteLine("The streets table has been created.");

        // Add a new field to the table. Populate the string using a helper method.
        string streetTypeFieldDef = "";
        CreateStreetTypeField(ref streetTypeFieldDef);

        streetsTable.AddField(streetTypeFieldDef);
        Console.WriteLine("The StreetType field has been added.");

        streetsTable.DeleteField("SpeedLimit");
        Console.WriteLine("The SpeedLimit field has been deleted.");

        // Add a new index to the table.  Populate the string using a helper method.
        string indexDef = "";
        CreateStreetTypeIndex(ref indexDef);
        streetsTable.AddIndex(indexDef);
        Console.WriteLine("The StreetType field has been indexed.");

        // Create a new subtype.  Populate the string using a helper method.
        string subtypeDef = "";
        CreateSubtypeDefinition(ref subtypeDef);
        streetsTable.EnableSubtypes("StreetType", subtypeDef);
        Console.WriteLine("Enabled the Trunk Highways subtype");

        // Get the Default Subtype Code.
        Console.WriteLine("The Default Subtype code is: {0}", streetsTable.DefaultSubtypeCode);

        // Set the Default Subtype Code.
        streetsTable.DefaultSubtypeCode = 87;
        Console.WriteLine("The Default Subtype code is now: {0}", streetsTable.DefaultSubtypeCode);

        // Alter a subtype. Populate the string using a helper method.
        string alteredSubtypeDef = "";
        CreateSubtypeDefinitionAltered(ref alteredSubtypeDef);
        streetsTable.AlterSubtype(alteredSubtypeDef);
        Console.WriteLine("Altered the Trunk Highways subtype");

        // Create another subtype. Populate the string using a helper method.
        string subtypeDef2 = "";
        CreateSubtypeDefinition2(ref subtypeDef2);
        streetsTable.CreateSubtype(subtypeDef2);
        Console.WriteLine("Added Local Streets subtype");

        // Delete the Local Streets subtype
        streetsTable.DeleteSubtype("Local Streets");
        Console.WriteLine("Deleted the Local Streets subtype");

        // Close the table.
        streetsTable.Close();

        // Close the geodatabase
        geodatabase.Close();
      }
      catch (FileGDBException ex)
      {
        Console.WriteLine("{0} - {1}", ex.Message, ex.ErrorCode);
      }
      catch (Exception ex)
      {
        Console.WriteLine("General exception.  " + ex.Message);
      }
    }

    // A helper method for creating a field XML definition.
    static void CreateStreetTypeField(ref string fieldDef)
    {
      fieldDef = "<esri:Field xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:esri='http://www.esri.com/schemas/ArcGIS/10.1' xsi:type='esri:Field'>";
      fieldDef += "<Name>StreetType</Name>";
      fieldDef += "<Type>esriFieldTypeSmallInteger</Type>";
      fieldDef += "<IsNullable>true</IsNullable>";
      fieldDef += "<Length>2</Length>";
      fieldDef += "<Precision>0</Precision>";
      fieldDef += "<Scale>0</Scale>";
      fieldDef += "<AliasName>day</AliasName>";
      fieldDef += "<ModelName>night</ModelName>";
      fieldDef += "<DefaultValue xsi:type=\"xs:short\">1</DefaultValue>";
      fieldDef += "</esri:Field>";
    }

    // A helper method for creating an index XML definition.
    static void CreateStreetTypeIndex(ref string indexDef)
    {
      indexDef += "<esri:Index xsi:type='esri:Index' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:esri='http://www.esri.com/schemas/ArcGIS/10.1'>";
      indexDef += "<Name>StreetTypeIdx</Name>";
      indexDef += "<IsUnique>false</IsUnique>";
      indexDef += "<IsAscending>true</IsAscending>";
      indexDef += "<Fields xsi:type='esri:Fields'>";
      indexDef += "<FieldArray xsi:type='esri:ArrayOfField'>";
      indexDef += "<Field xsi:type='esri:Field'>";
      indexDef += "<Name>StreetType</Name>";
      indexDef += "<Type>esriFieldTypeSmallInteger</Type>";
      indexDef += "<IsNullable>false</IsNullable>";
      indexDef += "<Length>2</Length>";
      indexDef += "<Precision>0</Precision>";
      indexDef += "<Scale>0</Scale>";
      indexDef += "<Required>true</Required>";
      indexDef += "<Editable>false</Editable>";
      indexDef += "<DomainFixed>true</DomainFixed>";
      indexDef += "<AliasName>StreetType</AliasName>";
      indexDef += "<ModelName>StreetType</ModelName>";
      indexDef += "</Field>";
      indexDef += "</FieldArray>";
      indexDef += "</Fields>";
      indexDef += "</esri:Index>";
    }

    // A helper method for creating a subtype XML definition.
    static void CreateSubtypeDefinition(ref string subtypeDef)
    {
      // Create a string containing an XML subtype definition.
      subtypeDef  = "<esri:Subtype xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:esri='http://www.esri.com/schemas/ArcGIS/10.1\' xsi:type='esri:Subtype'>";
      subtypeDef += "<SubtypeName>Trunk Highway</SubtypeName>";
      subtypeDef += "<SubtypeCode>99</SubtypeCode>";
      subtypeDef += "<FieldInfos xsi:type='esri:ArrayOfSubtypeFieldInfo'>";
      subtypeDef += "<SubtypeFieldInfo xsi:type='esri:SubtypeFieldInfo'>";
      subtypeDef += "<FieldName>LaneCount</FieldName>";
      subtypeDef += "<DomainName></DomainName>";
      subtypeDef += "<DefaultValue xsi:type='xs:short'>10</DefaultValue>";
      subtypeDef += "</SubtypeFieldInfo>";
      subtypeDef += "</FieldInfos>";
      subtypeDef += "</esri:Subtype>";
    }

    // A helper method for creating an altered subtype XML definition.
    static void CreateSubtypeDefinitionAltered(ref string subtypeDef)
    {
      // Create a string containing an XML subtype definition.
      subtypeDef  = "<esri:Subtype xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:esri=\"http://www.esri.com/schemas/ArcGIS/10.1\" xsi:type=\"esri:Subtype\">";
      subtypeDef += "<SubtypeName>Trunk Highway</SubtypeName>";
      subtypeDef += "<SubtypeCode>98</SubtypeCode>";
      subtypeDef += "<FieldInfos xsi:type=\"esri:ArrayOfSubtypeFieldInfo\">";
      subtypeDef += "<SubtypeFieldInfo xsi:type=\"esri:SubtypeFieldInfo\">";
      subtypeDef += "<FieldName>LaneCount</FieldName>";
      subtypeDef += "<DomainName></DomainName>";
      subtypeDef += "<DefaultValue xsi:type=\"xs:short\">8</DefaultValue>";
      subtypeDef += "</SubtypeFieldInfo>";
      subtypeDef += "</FieldInfos>";
      subtypeDef += "</esri:Subtype>";
    }

    // A helper method for creating a subtype XML definition.
    static void CreateSubtypeDefinition2(ref string subtypeDef)
    {
      // Create a string containing an XML subtype definition.
      subtypeDef  = "<esri:Subtype xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xs=\"http://www.w3.org/2001/XMLSchema\" xmlns:esri=\"http://www.esri.com/schemas/ArcGIS/10.1\" xsi:type=\"esri:Subtype\">";
      subtypeDef += "<SubtypeName>Local Streets</SubtypeName>";
      subtypeDef += "<SubtypeCode>4</SubtypeCode>";
      subtypeDef += "<FieldInfos xsi:type=\"esri:ArrayOfSubtypeFieldInfo\">";
      subtypeDef += "<SubtypeFieldInfo xsi:type=\"esri:SubtypeFieldInfo\">";
      subtypeDef += "<FieldName>LaneCount</FieldName>";
      subtypeDef += "<DomainName></DomainName>";
      subtypeDef += "<DefaultValue xsi:type=\"xs:short\">2</DefaultValue>";
      subtypeDef += "</SubtypeFieldInfo>";
      subtypeDef += "</FieldInfos>";
      subtypeDef += "</esri:Subtype>";
    }
  }
}
