using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Esri.FileGDB;

namespace GeodatabaseManagement
{
    //
    // Sample: GeodatabaseManagement
    //
    // Demonstrates how to create a new geodatabase, open a geodatabase, and delete
    // a geodatabase.

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
  class GeodatabaseManagement
  {
    static void Main(string[] args)
    {
      try
      {
        Geodatabase geodatabase = Geodatabase.Create("../GeodatabaseManagement/GdbManagement.gdb");
        Console.WriteLine("The geodatabase has been created.");

        geodatabase.Close();

        geodatabase = Geodatabase.Open("../GeodatabaseManagement/GdbManagement.gdb");
        Console.WriteLine("The geodatabase has been opened.");

        geodatabase.Close();

        Geodatabase.Delete("../GeodatabaseManagement/GdbManagement.gdb");
        Console.WriteLine("The geodatabase has been deleted.");

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
