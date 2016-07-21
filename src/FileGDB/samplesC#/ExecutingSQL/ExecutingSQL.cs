using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Esri.FileGDB;

namespace ExecutingSQL
{
    //
    // Sample: ExecutingSQL
    //
    // Demonstrates how to use ExecuteSQL to perform standard SQL requests on a file
    // geodatabase.

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
  class ExecutingSQL
  {
    static void Main(string[] args)
    {
      try
      {
        // Open the geodatabase.
        Geodatabase geodatabase = Geodatabase.Open("../../samples/data/ExecuteSQL.gdb");

        // Perform a simple SELECT: find the names of every city with a 'TERM' value
        // equal to 'City'.

        string sqlStatement = "SELECT CITY_NAME, POP1990 FROM Cities WHERE TYPE = 'city' AND OBJECTID < 10";
        foreach (Row attrQueryRow in geodatabase.ExecuteSQL(sqlStatement))
        {
          Console.WriteLine("{0}\t{1}", attrQueryRow.GetString("CITY_NAME"), attrQueryRow.GetInteger("POP1990"));
        }

        // SELECT * - Return all fields discover the names and types from the
        // the row.
        sqlStatement = "SELECT * FROM Cities WHERE TYPE = 'city' AND OBJECTID < 10";
        foreach (Row attrQueryRow in geodatabase.ExecuteSQL(sqlStatement))
        {
          for (int nFieldNumber = 0; nFieldNumber < attrQueryRow.FieldInformation.Count; nFieldNumber++)
          {
            string fieldName = attrQueryRow.FieldInformation.GetFieldName(nFieldNumber);
            if (attrQueryRow.IsNull(fieldName))
            {
              Console.Write("null\t");
              continue;
            }

            //Console.Write("{0}", attrQueryRow[fieldName]);
            switch (attrQueryRow.FieldInformation.GetFieldType(nFieldNumber))
            {
              case FieldType.SmallInteger:
                Console.WriteLine("{0}", attrQueryRow.GetShort(fieldName));
                break;
              case FieldType.Integer:
                Console.Write("{0}\t", attrQueryRow.GetInteger(fieldName));
                break;
              case FieldType.Single:
                Console.Write("{0}\t", attrQueryRow.GetFloat(fieldName));
                break;
              case FieldType.Double:
                Console.Write("{0}\t", attrQueryRow.GetDouble(fieldName));
                break;
              case FieldType.String:
                Console.Write("{0}\t", attrQueryRow.GetString(fieldName));
                break;
              case FieldType.Date:
                Console.Write("{0}\t", attrQueryRow.GetDate(fieldName).ToShortTimeString());
                break;
              case FieldType.OID:
                Console.Write("{0}\t", attrQueryRow.GetOID());
                break;
              case FieldType.Geometry:
                Console.Write("Geometry\t");
                break;
              case FieldType.Blob:
                Console.Write("Blob\t");
                break;
              case FieldType.GUID:
                Console.Write("{0}\t", attrQueryRow.GetGUID(fieldName).ToString());
                break;
              case FieldType.GlobalID:
                Console.Write("{0}\t", attrQueryRow.GetGlobalID().ToString());
                break;
              default:
                break;
            }
          }
          Console.WriteLine("");
        }

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
  }
}
