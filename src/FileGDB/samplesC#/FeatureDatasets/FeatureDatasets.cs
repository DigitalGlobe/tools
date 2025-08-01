using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Esri.FileGDB;

namespace FeatureDatasets
{
    //
    // Sample: FeatureDatasets
    //
    // Demonstrates how to create a new geodatabase, create a feature dataset, then
    // create a table in the feature dataset.

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

  class FeatureDatasets
  {
    static void Main(string[] args)
    {
      try
      {
        try
        {
          // Delete the geodatabase in the current directory if it's already there.
          Geodatabase.Delete("../FeatureDatasets/FeatureDatasetDemo.gdb");
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

        // Create a new geodatabase in the current directory.
        Geodatabase geodatabase = Geodatabase.Create("../FeatureDatasets/FeatureDatasetDemo.gdb");
        Console.WriteLine("The geodatabase has been created.");

        // Load a feature dataset XML definition into a string. An example of a data
        // element is provided in the sample as "TransitFD.xml".
        string featureDatasetDef = "";
        using (StreamReader sr = new StreamReader("../../Samples/FeatureDatasets/TransitFD.xml"))
        {
          while (sr.Peek() >= 0)
          {
            featureDatasetDef += sr.ReadLine() + "\n";
          }
          sr.Close();
        }
        geodatabase.CreateFeatureDataset(featureDatasetDef);
        Console.WriteLine("The feature dataset has been created.");

        // Load a table XML definition into a string. There are several differences
        // between this example and adding a table at the root level of the geodatabase:
        //  - The table must have a shape field
        //  - The spatial reference must match that of the feature dataset
        //  - The definition's CatalogPath must include the feature dataset
        string tableDef = "";
        using (StreamReader sr = new StreamReader("../../Samples/FeatureDatasets/BusStopsTable.xml"))
        {
          while (sr.Peek() >= 0)
          {
            tableDef += sr.ReadLine() + "\n";
          }
          sr.Close();
        }
        // Create the table.
        Table table = geodatabase.CreateTable(tableDef, "\\Transit");
        Console.WriteLine("The table has been created.");

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
