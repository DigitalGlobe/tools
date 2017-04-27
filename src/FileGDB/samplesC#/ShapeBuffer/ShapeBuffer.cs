using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Esri.FileGDB;

namespace ShapeBuffer
{
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
  class ShapeBuffer
  {
    Geodatabase _geodatabase;

    static void Main(string[] args)
    {
      ShapeBuffer p = new ShapeBuffer();
      p.Run();
    }

    public void Run()
    {
      try
      {
        _geodatabase = Geodatabase.Open(@"../../Samples/data/Shapes.gdb");

        // Run the tests
        PointGeomTest();
        LineGeomTest();
        LineZGeomTest();
        PolygonGeomTest();
        MultipointGeomTest();

        _geodatabase.Close();
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

    void PointGeomTest()
    {
      try
      {
        Console.WriteLine();
        Console.WriteLine("Testing Point Shapes, Read");

        // Open the point test table, cities.
        Table citiesTable = _geodatabase.OpenTable(@"\\cities");

        // Return all rows.
        Point point = new Point();
        RowCollection rows = citiesTable.Search("*", "CITY_NAME = 'Woodinville'", RowInstance.Recycle);
        foreach (Row row in rows)
        {
          PointShapeBuffer pointGeometry = row.GetGeometry();
          point = pointGeometry.point;
          Console.WriteLine("Point test:");
          Console.WriteLine("x: {0} y: {1}", point.x, point.y);
          break;
        }
        citiesTable.Close();

        Console.WriteLine();
        Console.WriteLine("Testing Point Shapes, Write");

        // Open the point test table, cities.
        Table cities2Table = _geodatabase.OpenTable(@"\\cities2");

        // Begin load only mode. This shuts off the update of all indexes.
        cities2Table.LoadOnlyMode(true);

        // Create a new feature.
        Row cities2Row = cities2Table.CreateRowObject();
        PointShapeBuffer point2Geometry = new PointShapeBuffer();
        point2Geometry.Setup(ShapeType.Point);
        Point point2 = point2Geometry.point;
        point2.x = point.x;
        point2.y = point.y;
        point2Geometry.point = point2;

        cities2Row.SetGeometry(point2Geometry);

        cities2Table.Insert(cities2Row);

        Console.WriteLine("Inserted a point.");

        // End load only mode. This rebuilds all indexes including the spatial index.
        // If all grid size values in the spatial index are zero, the values will be
        // calculated based on the existing data.
        cities2Table.LoadOnlyMode(false);

        // Close the cities table.
        cities2Table.Close();
      }
      catch (FileGDBException ex)
      {
        Console.WriteLine("PointGeomTest failed {0} - {1}", ex.Message, ex.ErrorCode);
      }
      catch (Exception ex)
      {
        Console.WriteLine("PointGeomTest failed General exception.  " + ex.Message);
      }
    }

    void LineGeomTest()
    {
      try
      {
        Console.WriteLine();
        Console.WriteLine("Testing Line Shapes, Read");

        // Open the Line test table, intrstat.
        Table intrstatTable = _geodatabase.OpenTable("\\intrstat");

        MultiPartShapeBuffer lineGeometry = new MultiPartShapeBuffer(); ;
        RowCollection rows = intrstatTable.Search("*", "ROUTE_NUM = 'I10'", RowInstance.Recycle);
        foreach (Row row in rows)
        {
          lineGeometry = row.GetGeometry();
          break;
        }

        int numPts = lineGeometry.NumPoints;
        int numParts = lineGeometry.NumParts;

        Console.WriteLine("Line test:");
        Console.WriteLine("Points: {0}", numPts);
        Console.WriteLine("Parts: {0}", numParts);

        int[] parts = lineGeometry.Parts;
        Point[] points = lineGeometry.Points;
        int partCount = 0;
        for (int i = 0; i < numPts; i++)
        {
          if ((partCount < numParts) && (i == parts[partCount]))
          {
            Console.WriteLine("Part {0}", partCount);
            partCount++;
          }
          Console.WriteLine("{0} {1}, {2}", i, points[i].x, points[i].y);
        }

        // Close the intrstat table.
        intrstatTable.Close();

        Console.WriteLine();
        Console.WriteLine("Testing Line Shapes, Write");

        // Open the line test table, intrstat2.
        Table intrstat2Table = _geodatabase.OpenTable("\\intrstat2");

        // Begin load only mode. This shuts off the update of all indexes.
        intrstat2Table.LoadOnlyMode(true);

        // Create a new feature.
        Row intrstat2Row = intrstat2Table.CreateRowObject();

        int numPts2   = numPts;
        int numParts2 = numParts;
        MultiPartShapeBuffer intrstat2Geometry = new MultiPartShapeBuffer();
        intrstat2Geometry.Setup(ShapeType.Polyline, numParts2, numPts2);

        // Set the point array to the array from the read geometry.
        Point[] points2 = intrstat2Geometry.Points;
        for (int i = 0; i < numPts; i++)
        {
          points2[i].x = points[i].x;
          points2[i].y = points[i].y;
          if (i == (numPts - 1))
          {
            Console.WriteLine("{0}, {1}", points2[i].x, points2[i].y);
          }
        }
        intrstat2Geometry.Points = points2;

        // Set the parts array to the array from the read geometry.
        int[] parts2 = intrstat2Geometry.Parts;
        for (int i = 0; i < numParts2; i++)
        {
          parts2[i] = parts[i];
        }
        intrstat2Geometry.Parts = parts2;

        intrstat2Geometry.CalculateExtent();

        intrstat2Row.SetGeometry(intrstat2Geometry);
        intrstat2Table.Insert(intrstat2Row);
        Console.WriteLine("Inserted a line.");

        // End load only mode. This rebuilds all indexes including the spatial index.
        // If all grid size values in the spatial index are zero, the values will be
        // calculated based on the existing data.
        intrstat2Table.LoadOnlyMode(false);

        // Close the intrstat2 table
        intrstat2Table.Close();
      }
      catch (FileGDBException ex)
      {
        Console.WriteLine("LineGeomTest failed {0} - {1}", ex.Message, ex.ErrorCode);
      }
      catch (Exception ex)
      {
        Console.WriteLine("LineGeomTest failed General exception.  " + ex.Message);
      }
    }

    void LineZGeomTest()
    {
      try
      {
        Console.WriteLine();
        Console.WriteLine("Testing Line Shapes with Zs");

        // Open the Line with Zs test table, zvalues.
        Table zvaluesTable = _geodatabase.OpenTable("\\zvalues");

        MultiPartShapeBuffer lineZGeometry = new MultiPartShapeBuffer();
        // Get the first returned row.
        RowCollection rows = zvaluesTable.Search("*", "OBJECTID = 1", RowInstance.Recycle);
        foreach (Row row in rows)
        {
          lineZGeometry = row.GetGeometry();
          break;
        }
        int numPts = lineZGeometry.NumPoints;

        int numParts = lineZGeometry.NumParts;
        Console.WriteLine("Line with Z test:");
        Console.WriteLine("Points: {0}", numPts);
        Console.WriteLine("Parts: {0}", numParts);

        int[] parts = lineZGeometry.Parts;

        Point[] points = lineZGeometry.Points;

        double[] zArray = lineZGeometry.Zs;

        int partCount = 0;
        for (int i = 0; i < numPts; i++)
        {
          if ((partCount < numParts) && (i == parts[partCount]))
          {
            Console.WriteLine("Part {0}", partCount);
            partCount++;
          }
          Console.WriteLine("{0} {1}, {2}, {3}", i, points[i].x, points[i].y, zArray[i]);
        }

        // Close the zvalues table
        zvaluesTable.Close();

        Console.WriteLine("Testing Line Shapes, Write");

        // Open the line Z test table, zvalues2.
        Table zvalues2Table = _geodatabase.OpenTable("\\zvalues2");

        // Begin load only mode.
        zvalues2Table.LoadOnlyMode(true);

        // Create a new feature.
        Row zvalues2Row = zvalues2Table.CreateRowObject();

        int numPts2   = numPts;
        int numParts2 = numParts;
        MultiPartShapeBuffer zvalues2Geometry = new MultiPartShapeBuffer();
        zvalues2Geometry.Setup(ShapeType.PolylineZ, numParts2, numPts2);

        Point[]  points2 = zvalues2Geometry.Points;

        double[] zArray2 = zvalues2Geometry.Zs;

        // Set the points array to the array from the read geometry.
        for (int i = 0; i < numPts; i++)
        {
          points2[i].x = points[i].x;
          points2[i].y = points[i].y;
          zArray2[i]   = zArray[i];
        }
        zvalues2Geometry.Points = points2;
        zvalues2Geometry.Zs = zArray2;

        // Set the parts array to the array from the read geometry.
        int[] parts2 = zvalues2Geometry.Parts;
        for (int i = 0; i < numParts2; i++)
        {
          parts2[i] = parts[i];
        }
        zvalues2Geometry.Parts = parts2;

        zvalues2Geometry.CalculateExtent();
        zvalues2Row.SetGeometry(zvalues2Geometry);

        zvalues2Table.Insert(zvalues2Row);

        Console.WriteLine("Inserted a line with z's.");

          // End load only mode.
        zvalues2Table.LoadOnlyMode(false);

        // Close the zvalues2 table
        zvalues2Table.Close();

      }
      catch (FileGDBException ex)
      {
        Console.WriteLine("LineZGeomTest failed {0} - {1}", ex.Message, ex.ErrorCode);
      }
      catch (Exception ex)
      {
        Console.WriteLine("LineZGeomTest failed General exception.  " + ex.Message);
      }
    }

    void PolygonGeomTest()
    {
      try
      {
        Console.WriteLine("Testing Polygon Shapes");

        // Open the Polygon test table, states.
        Table statesTable = _geodatabase.OpenTable("\\states");

        // Get back a single row.
        RowCollection rows = statesTable.Search("*", "STATE_NAME = 'Rhode Island'", RowInstance.Recycle);

        MultiPartShapeBuffer statesGeometry = new MultiPartShapeBuffer();
        // Get the first returned row.
        foreach (Row row in rows)
        {
          statesGeometry = row.GetGeometry();
          break;
        }

        int numPts = statesGeometry.NumPoints;
        int numParts = statesGeometry.NumParts;
        Console.WriteLine("Polygon test:");
        Console.WriteLine("Points: {0}", numPts);
        Console.WriteLine("Parts: {0}", numParts);

        Console.WriteLine("Feature Extent:");
        Console.WriteLine("xMin: {0}", statesGeometry.Extent.xMin);
        Console.WriteLine("yMin: {0}", statesGeometry.Extent.yMin);
        Console.WriteLine("xMax: {0}", statesGeometry.Extent.xMax);
        Console.WriteLine("yMax: {0}", statesGeometry.Extent.yMax);

        int[] parts = statesGeometry.Parts;

        Point[] points = statesGeometry.Points;

        int partCount = 0;
        for (int i = 0; i < numPts; i++)
        {
          if ((partCount < numParts) && (i == parts[partCount]))
          {
            Console.WriteLine("Part {0}", partCount);
            partCount++;
          }
          Console.WriteLine("{0} {1}, {2}", i, points[i].x, points[i].y);
        }

        // Close the states table
        statesTable.Close();

        Console.WriteLine("Testing Polygon Shapes, Write");

        // Open the polygon test table, states2.
        Table states2Table = _geodatabase.OpenTable("\\states2");

        // Begin load only mode. This shuts off the update of all indexes.
        states2Table.LoadOnlyMode(true);

        // Create a new feature.
        Row states2Row = states2Table.CreateRowObject();

        int numPts2 = numPts;
        int numParts2 = numParts;
        MultiPartShapeBuffer states2Geometry = new MultiPartShapeBuffer();
        states2Geometry.Setup(ShapeType.Polygon, numParts, numPts);

        // Set the point array to the array from the read geometry.
        Point[] points2 = states2Geometry.Points;
        for (int i = 0; i < numPts; i++)
        {
          points2[i].x = points[i].x;
          points2[i].y = points[i].y;
        }
        states2Geometry.Points = points2;

        // Set the part array to the array from the read geometry.
        int[] parts2 = states2Geometry.Parts;
        for (int i = 0; i < numParts2; i++)
        {
          parts2[i] = parts[i];
        }
        states2Geometry.Parts = parts2;

        states2Geometry.CalculateExtent();

        states2Row.SetGeometry(states2Geometry);

        states2Table.Insert(states2Row);
        Console.WriteLine("Inserted a polygon.");

        // End load only mode. This rebuilds all indexes including the spatial index.
        // If all grid size values in the spatial index are zero, the values will be
        // calculated based on the existing data.
        states2Table.LoadOnlyMode(false);

        // Close the states2 table.
        states2Table.Close();
      }
      catch (FileGDBException ex)
      {
        Console.WriteLine("PolygonGeomTest failed {0} - {1}", ex.Message, ex.ErrorCode);
      }
      catch (Exception ex)
      {
        Console.WriteLine("PolygonGeomTest failed General exception.  " + ex.Message);
      }
    }

    void MultipointGeomTest()
    {
      try
      {
        Console.WriteLine("Testing Multipoint");

        // Open the Line with Zs test table, multipoint.
        Table multipointTable = _geodatabase.OpenTable("\\multipoint");

        // Return all rows.
        RowCollection rows = multipointTable.Search("*", "OBJECTID = 1", RowInstance.Recycle);

        MultiPointShapeBuffer multipointGeometry = new MultiPointShapeBuffer();
        // Get the first returned row.
        foreach (Row row in rows)
        {
          multipointGeometry = row.GetGeometry();
          break;
        }

        int numPts = multipointGeometry.NumPoints;
        Console.WriteLine("Multipoint test:");
        Console.WriteLine("Points: {0}", numPts);

        Point[] points = multipointGeometry.Points;

        double[] zArray = multipointGeometry.Zs;

        for (int i = 0; i < numPts; i++)
        {
          Console.WriteLine("{0} {1}, {2}, {3}", i, points[i].x, points[i].y, zArray[i]);
        }

        // Close the multipoint table
        multipointTable.Close();

        Console.WriteLine("Testing Multipoint Shapes, Write");

        // Open the multipoint test table, multipoint2.
        Table multipoint2Table = _geodatabase.OpenTable("\\multipoint2");

        // Enter load only mode. This shuts off the update of all indexes.
        multipoint2Table.LoadOnlyMode(true);

        // Create a new feature.
        Row multipoint2Row = multipoint2Table.CreateRowObject();

        MultiPointShapeBuffer multipoint2Geometry = new MultiPointShapeBuffer();
        multipoint2Geometry.Setup(ShapeType.MultipointZ, numPts);

        Point[] points2 = multipoint2Geometry.Points;

        double[] zArray2 = multipoint2Geometry.Zs;

        for (int i = 0; i < numPts; i++)
        {
          points2[i].x = points[i].x;
          points2[i].y = points[i].y;
          zArray2[i] = zArray[i];
        }
        multipoint2Geometry.Points = points2;
        multipoint2Geometry.Zs = zArray2;

        multipoint2Geometry.CalculateExtent();

        multipoint2Row.SetGeometry(multipoint2Geometry);

        multipoint2Table.Insert(multipoint2Row);
        Console.WriteLine("Inserted a multipoint.");

        // End load only mode. This rebuilds all indexes including the spatial index.
        // If all grid size values in the spatial index are zero, the values will be
        // calculated based on the existing data.
        multipoint2Table.LoadOnlyMode(false);

        // Close the multipoint2 table
        multipoint2Table.Close();
      }
      catch (FileGDBException ex)
      {
        Console.WriteLine("MultipointGeomTest failed {0} - {1}", ex.Message, ex.ErrorCode);
      }
      catch (Exception ex)
      {
        Console.WriteLine("MultipointGeomTest failed General exception.  " + ex.Message);
      }
    }
  }
}
