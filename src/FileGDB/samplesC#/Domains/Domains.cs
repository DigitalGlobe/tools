using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Esri.FileGDB;

namespace Domains
{
    //
    // Sample: Domains
    //
    // Demonstrates how to create a new range domain, a code value domain,
    // to get the definition of an existing domain, delete a domains, alter
    // a domain and to assign it to a field.

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
 
  class Domains
  {
    static void Main(string[] args)
    {
      try
      {
        // Open the geodatabase.
        Geodatabase geodatabase = Geodatabase.Open("../../samples/data/Domains.gdb");

        try
        {
          // Delete the SpeedLimit domain.
          geodatabase.DeleteDomain("SpeedLimit");
          Console.WriteLine("The SpeedLimit domain has been deleted.");
        }
        catch (FileGDBException ex)
        {
          if (ex.ErrorCode == -2147209215)
            Console.WriteLine("The SpeedLimit domain does not exist, no need to delete");
          else
            Console.WriteLine("An error occurred while deleting the domain SpeedLimit.  {0}({1})", ex.Message, ex.ErrorCode);
        }

        try
        {
          // Delete the RoadSurfaceType domain.
          geodatabase.DeleteDomain("RoadSurfaceType");
          Console.WriteLine("The RoadSurfaceType domain has been deleted.");
        }
        catch (FileGDBException ex)
        {
          if (ex.ErrorCode == -2147209215)
            Console.WriteLine("The RoadSurfaceType domain does not exist, no need to delete");
          else
            Console.WriteLine("An error occurred while deleting the RoadSurfaceType.  {0}({1})", ex.Message, ex.ErrorCode);
        }

        // Open the SpeedLimit.xml to get a range domain definition.
        string speedLimitDef = "";
        using (StreamReader sr = new StreamReader("../../Samples/Domains/SpeedLimit.xml"))
        {
          while (sr.Peek() >= 0)
          {
            speedLimitDef += sr.ReadLine() + "\n";
          }
          sr.Close();
        }

        // Create the SpeedLimit range domain in the geodatabase.
        geodatabase.CreateDomain(speedLimitDef);
        Console.WriteLine("The SpeedLimit range domain has been created.");

        // Get the Definition of an existing domain.
        Console.WriteLine("The definition of the MedianType domain is: {0}", geodatabase.GetDomainDefinition("MedianType"));

        // Open the RoadSurfaceType.xml to get a coded value domain definition.
        string roadTypeDef = "";
        using (StreamReader sr = new StreamReader("../../Samples/Domains/RoadSurfaceType.xml"))
        {
          while (sr.Peek() >= 0)
          {
            roadTypeDef += sr.ReadLine() + "\n";
          }
          sr.Close();
        }

        // Create the RoadSurfaceType domain in the geodatabase.
        geodatabase.CreateDomain(roadTypeDef);
        Console.WriteLine("The RoadSurfaceType coded value domain has been created.");

        // Open the RoadSurfaceTypeALtered.xml to get a altered coded value domain definition.
        string roadTypeDefAlter = "";
        using (StreamReader sr = new StreamReader("../../Samples/Domains/RoadSurfaceTypeAltered.xml"))
        {
          while (sr.Peek() >= 0)
          {
            roadTypeDefAlter += sr.ReadLine() + "\n";
          }
          sr.Close();
        }

        // Alter the RoadSurfaceType domain in the geodatabase.
        geodatabase.AlterDomain(roadTypeDefAlter);
        Console.WriteLine("The RoadSurfaceType coded value domain has been Altered.");

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
