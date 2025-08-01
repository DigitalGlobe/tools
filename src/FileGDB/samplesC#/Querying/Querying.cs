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

   http://github.com/Esri/file-geodatabase-api/FileGDB_API_1.5.1
*/

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
