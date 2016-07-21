using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Esri.FileGDB;

namespace Querying
{
    //
    // Sample: Querying
    //
    // Demonstrates how to perform simple spatial and attribute queries.

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
  class Querying
  {
    static void Main(string[] args)
    {
      try
      {
        // Open the geodatabase.
        Geodatabase geodatabase = Geodatabase.Open("../../Samples/data/Querying.gdb");

        // Open the Cities table.
        Table table = geodatabase.OpenTable("Cities");

        // Perform a simple attribute query: find the names of every city
        // with a 'TYPE' value equal to 'City'. Return the NAME, Pop1990 and
        // the X, Y coordinates of each feature.
        RowCollection attrQueryRows = table.Search("Shape, CITY_NAME, POP1990", "TYPE = 'city' AND OBJECTID < 10", RowInstance.Recycle);
        foreach (Row attrQueryRow in attrQueryRows)
        {
          PointShapeBuffer geometry = attrQueryRow.GetGeometry();
          Point point = geometry.point;
          Console.WriteLine("{0}\t{1}\t{2:0.####},{3:0.####}",
            attrQueryRow.GetString("CITY_NAME"), attrQueryRow.GetInteger("POP1990"), point.x, point.y);
        }

        // Through in a newline to separate the examples.
        Console.WriteLine();
        // Perform a spatial query: find the names of every city within a
        // specified envelope.
        Envelope envelope = new Envelope();
        envelope.xMin = -118.219;
        envelope.yMin = 22.98;
        envelope.xMax = -117.988;
        envelope.yMax = 34.0;
        RowCollection spQueryRows = table.Search("CITY_NAME", "", envelope, RowInstance.Recycle);
        foreach (Row spQueryRow in spQueryRows)
        {
          Console.WriteLine(spQueryRow.GetString("CITY_NAME"));
        }

        // Close the table
        table.Close();

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
