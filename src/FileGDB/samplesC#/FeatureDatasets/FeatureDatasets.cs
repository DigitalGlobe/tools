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
