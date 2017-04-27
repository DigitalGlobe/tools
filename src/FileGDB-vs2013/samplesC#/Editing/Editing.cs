using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Esri.FileGDB;

namespace Editing
{
    //
    // Sample: Editing
    //
    // Demonstrates how to create new rows in a table, how to modify existing rows,
    // and how to delete rows. 

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
  class Editing
  {
    static void Main(string[] args)
    {
      try
      {
        // Open the geodatabase.
        Geodatabase geodatabase = Geodatabase.Open("../../samples/data/Editing.gdb");

        // Open the Cities table.
        Table table = geodatabase.OpenTable("\\Cities");

        // Create a new feature for Cabazon.
        Row cabazonRow = table.CreateRowObject();
        cabazonRow.SetString("AREANAME","Cabazon");
        cabazonRow.SetString("CLASS","town");
        cabazonRow.SetInteger("POP2000", 2939); // 2007

        // Create and assign a point geometry.
        PointShapeBuffer cabazonGeom = new PointShapeBuffer();
        cabazonGeom.Setup(ShapeType.Point);

        Point point= new Point(-116.78443, 33.919902);
        cabazonGeom.point = point;
        cabazonRow.SetGeometry(cabazonGeom);

        table.Insert(cabazonRow);
        Console.WriteLine("Inserted two strings, one integer and a point.");

        // Get the row for Apple Valley by querying the table. Make sure to disable
        // recycling when intending to edit rows.
        foreach (Row avRow in table.Search("*", "AREANAME = 'Apple Valley'", RowInstance.Unique))
        {
          avRow.SetString("CLASS", "city");
          table.Update(avRow);
          break;
        }
        Console.WriteLine("Updated the CLASS field.");

        // Delete the rows that fall into an envelope and have a null elevation value.
        // As with updating, make sure recycling is disabled when deleting rows.
        Envelope envelope = new Envelope();
        envelope.xMin = -117.4;
        envelope.yMin =   33.65;
        envelope.xMax = -116.8;
        envelope.yMax =   33.86;

        foreach (Row deleteRow in table.Search("*", "", envelope, RowInstance.Recycle))
        {
          if (!deleteRow.IsNull("ELEVATION"))
            continue;

          Console.WriteLine("Deleting: {0}", deleteRow.GetString("AREANAME"));
          table.Delete(deleteRow);
        }

        // Close the table.
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
