Esri File Geodatabase API
Build Date:  Mar 02, 2017
Build Version:  1.5.0.248
Version:  v 1.5


          Esri File Geodatabase API for Windows 32-bit and Windows 64-bit README
          ----------------------------------------------------------------------

SUPPORTED PLATFORMS:
    The supported platforms (for development) are:

        Windows 2008 Server Standard, Enterprise & Datacenter (32-bit and 64-bit) SP2
        Windows 2008 R2 Server Standard, Enterprise, and Datacenter (64-bit)
        Windows 2012 Server Standard and Datacenter (64-bit)  
        Windows 2012 R2 Server Standard and Datacenter (64-bit)     
        Windows 7 Ultimate, Enterprise, Professional, Home Premium (32-bit and 64-bit)
        Windows 8.1 Enterprise, Professional  (32-bit and 64-bit)
        Windows 10 Enterprise, Professional (32-bit and 64-bit)  

        For more information, see http://www.microsoft.com/visualstudio/11/en-us/products/compatibility
        See the same link for deployment options.

SOFTWARE REQUIREMENTS:
        Visual Studio 2015 SP1 (C++) Premium, Professional, Ultimate or Team Editions required for development.
        Visual Studio 2015 C and C++ Runtimes required for deployment.
        .NET 4.5.1 Framework is required for the .NET wrapper.

TO BUILD THE SAMPLES ON WINDOWS 32-bit:
    C++
        1) Open Visual Studio.
        2) Open the samples project (FileGDB_API\samples\samples_VS2013.sln).
        3) In the solution explorer, right click on the solution (the top of the tree) and 
        select "Rebuild Solution". The samples will begin to build. Ignore all warnings. You will
        see a failure in the Display sample. It requires that you install GLUT. You will also see a
        failure in the ProcessTopologies sample which requires that you install libXml.

        Note: GLUT can be downloaded from: http://www.opengl.org/resources/libraries/glut/
        Download the pre-compiled version of GLUT or the source and compile it yourself. The 
        GLUT README included instruction on how to compile the source. To install GLUT unzip 
        the GLUT zip file onto your local disk. Depending on where you install GLUT you may
        have to modify the project include and library directory paths and dependencies. You
        will need to copy the glut32.dll to the bin (samples\bin) directory.
        
        Note: If you get linker errors such as "LNK2026: module unsafe for SAFESEH image" when 
        building the Display sample, open the project settings under Linker -> Advanced and set
        Image Has Safe Exception Handlers to No (/SAFESEH:NO).

        libXml can be downloaded from http://xmlsoft.org/downloads.html. Depending on where 
        you install libXml you may have to modify the project include and library directory 
        paths and dependencies.  New versions of libXml may have additional dependencies such 
        as iconv.dll and zlib.dll.  These other dependencies can be downloaded at the same 
        location as libXml.

    C#
        1) Open Visual Studio.
        2) Open the samples project (FileGDB_API\samplesC#\samplesCsharp_VS2013.sln).
        3) In the solution explorer, right click on the solution (the top of the tree) and 
        select "Rebuild Solution". The samples will begin to build. Ignore all warnings. You will
        see a failure in the Display sample. It requires that you install OpenTK. 

        Note: OpenTK can be downloaded from: http://www.opentk.com/


TO BUILD THE SAMPLES ON WINDOWS 64-bit:
    C++
        1) Open Visual Studio.
        2) Open the samples project (FileGDB_API\samples\samples_VS2013.sln).
        3) Change the Solution Platform to x64.
        4) In the solution explorer, right click on the solution (the top of the tree) and 
        select "Rebuild Solution". The samples will begin to build. Ignore all warnings. You will
        see a failure in the Display sample. It requires that you install GLUT. You will also see a
        failure in the ProcessTopologies sample which requires that you install libXml.

        Note: GLUT64 can be downloaded from the web. Depending on where you install GLUT64 you may
        have to modify the project include and library directory paths and dependencies. You
        will need to copy the glut64.dll to the bin (samples\bin64) directory.
        
        Note: If you get linker errors such as "LNK2026: module unsafe for SAFESEH image" when 
        building the Display sample, open the project settings under Linker -> Advanced and set
        Image Has Safe Exception Handlers to No (/SAFESEH:NO).

        libXml can be downloaded from http://xmlsoft.org/downloads.html. Depending on where 
        you install libXml you may have to modify the project include and library directory 
        paths and dependencies.  New versions of libXml may have additional dependencies such 
        as iconv.dll and zlib.dll.  These other dependencies can be downloaded at the same 
        location as libXml.

    C#
        1) Open Visual Studio.
        2) Open the samples project (FileGDB_API\samplesC#\samplesCsharp_VS2013.sln).
        3) Change the Solution Platform to x64.
        4) In the solution explorer, right click on the solution (the top of the tree) and 
        select "Rebuild Solution". The samples will begin to build. Ignore all warnings. You will
        see a failure in the Display sample. It requires that you install OpenTK. 

        Note: OpenTK can be downloaded from: http://www.opentk.com/

WHAT'S IN THE DIRECTORY:

FileGDB_API\bin
FileGDB_API\bin64
FileGDB_API\lib
FileGDB_API\lib64
FileGDB_API\doc\html
FileGDB_API\doc\htmlC#
FileGDB_API\samples
FileGDB_API\samplesC#
FileGDB_API\include
FileGDB_API\license
FileGDB_API\xmlResources

SAMPLES:

C++

To access the html help open FileGDB_API\doc\html\index.html in a web browser.

The samples directory contains C++ samples that demonstrate various aspects 
of the API. A MS Visual Studio 2015 Solution and Projects are included for 
all. The XMLsamples directory provides example XML for creating tables, feature
classes, feature datasets, indexes, subtypes, and domains. The display sample 
requires that GLUT 3.7.6 be installed. 
http://www.opengl.org/resources/libraries/glut/

The include directory contains the required include files plus a convenience 
include, FileGDBAPI.h, which references all of the others.

C#

To access the html help open FileGDB_API\doc\htmlC#\index.html in a web browser.

The samples directory contains C# samples that demonstrate various aspects 
of the API. A MS Visual Studio 2015 Solution and Projects are included for 
all. The display sample requires that OpenTK be installed. 
http://www.opentk.com/


SUPPORTED:
    * Create, Open and Delete file geodatabases
    * Read the schema of the geodatabase
          o All content within a geodatabase can be opened for read access
    * Create schema for objects within the simple feature model
          o Tables
          o Point, Line, and Polygon feature classes
          o Feature datasets
          o Domains
          o Subtypes

    * Read the contents of datasets in a geodatabase
          o All dataset content within a geodatabase can be read
    * Insert, Delete and Edit the contents of simple datasets:
          o Tables
          o Point, Line, Polygon, Multipoint, and Multipatch feature classes 
    * Perform attribute and (limited) spatial queries on datasets
          o Spatial queries will be limited to the envelope-intersects operator
    * Version 10.x file geodatabases only, previous versions cannot be opened.

NOT SUPPORTED:

While the Esri File Geodatabase API supports reading the schema and data of complex geodatabase types, the API does not honor geodatabase behavior on inserts, deletes or updates to the following dataset types:

    * Annotation and Dimension feature classes
    * Relationship Classes
    * Networks (GN and ND)
    * Topologies
    * Terrains
    * Representations
    * Parcel Fabrics

In most cases the API will prevent users from attempting to edit objects with complex behavior, but the onus is on the developer to understand what they should and should not edit through the API and avoid editing datasets that have geodatabase behavior. 

Other limitations of the Esri File Geodatabase API:

    * Rasters (Raster Dataset, Raster Catalog, Mosaic Datasets and Raster Attributes) are not supported.
    * Spatial queries with the File Geodatabase API are limited to the envelope-intersects operator.
    * Vertical Datums are not supported.  
    * The API is not thread safe.
    * Secure(licensed) data is not supported.

WHAT'S NEW:

  * MS Visual Studio 2015 support.

KNOWN ISSUES:

    * Concurrent access from Windows and Linux clients to the same File GeoDatabase can corrupt data. This combination should be avoided.
    * SQL joins are not supported.

FIXED ISSUES:

BUG-000100781 Allow the use of the Spatial Reference ID to create the spatial reference.
BUG-000100780 The File GDB API FdoErrorManager crashes on some platforms.
BUG-000100782 File Geodatabase: COUNT(0) should return 0 with a WHERE clause that returns no rows (1=0)
BUG-000100783 Remove Windows FileGDBAPI.dll's reliance on Microsoft.VC90.CRT
BUG-000088293 FileGDB API throws -2147217395 error when executing SQL query with SQL functions.
BUG-000097116 Table::FreeWriteLock does not clear the lock.

SAMPLE PROGRAM EXPECTED RESULTS (C++)

Domains:
*First run on fresh install

The SpeedLimit domain does not exist, no need to delete
The RoadSurfaceType domain does not exist, no need to delete
The SpeedLimit range domain has been created.
The definition of the MedianType domain is: 
<Domain xsi:type='esri:CodedValueDomain' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:esri='http://www.esri.com/schemas/ArcGIS/10.0'><DomainName>MedianType</DomainName><FieldType>esriFieldTypeSmallInteger</FieldType><MergePolicy>esriMPTDefaultValue</MergePolicy><SplitPolicy>esriSPTDefaultValue</SplitPolicy><Description>Road median types.</Description><Owner></Owner><CodedValues xsi:type='esri:ArrayOfCodedValue'><CodedValue xsi:type='esri:CodedValue'><Name>None</Name><Code xsi:type='xs:short'>0</Code></CodedValue><CodedValue xsi:type='esri:CodedValue'><Name>Cement</Name><Code xsi:type='xs:short'>1</Code></CodedValue></CodedValues></Domain>
The RoadSurfaceType coded value domain has been created.
The RoadSurfaceType coded value domain has been Altered.
The SpeedLimnit (Range) domain has been assigned to the MaxSpeed field.
The RoadSurfaceType (CodedValue) domain has been assigned to the SurfaceType field.

*Second run on fresh install

The SpeedLimit domain has been deleted.
The RoadSurfaceType domain has been deleted.
The SpeedLimit range domain has been created.
The definition of the MedianType domain is: 
<Domain xsi:type='esri:CodedValueDomain' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:esri='http://www.esri.com/schemas/ArcGIS/10.0'><DomainName>MedianType</DomainName><FieldType>esriFieldTypeSmallInteger</FieldType><MergePolicy>esriMPTDefaultValue</MergePolicy><SplitPolicy>esriSPTDefaultValue</SplitPolicy><Description>Road median types.</Description><Owner></Owner><CodedValues xsi:type='esri:ArrayOfCodedValue'><CodedValue xsi:type='esri:CodedValue'><Name>None</Name><Code xsi:type='xs:short'>0</Code></CodedValue><CodedValue xsi:type='esri:CodedValue'><Name>Cement</Name><Code xsi:type='xs:short'>1</Code></CodedValue></CodedValues></Domain>
The RoadSurfaceType coded value domain has been created.
The RoadSurfaceType coded value domain has been Altered.
The SpeedLimnit (Range) domain has been assigned to the MaxSpeed field.
The RoadSurfaceType (CodedValue) domain has been assigned to the SurfaceType field.

Editing:
*First run

Inserted two strings, one integer and a point.
Updated the CLASS field.
Deleting: Hemet

*Second run

Inserted two strings, one integer and a point.
Updated the CLASS field.

ExecutingSQL:

Bellingham	52179
Havre	10201
Anacortes	11451
Mount Vernon	17647
Oak Harbor	17176
Minot	34544
Kalispell	11917
Williston	13131
Port Angeles	17710

SELECT CITY_NAME, POP1990 FROM Cities WHERE CITY_NAME SIMILAR TO 'Redmond|Burien'

Redmond 35800
Burien  25089

SELECT * FROM Cities WHERE TYPE = 'city' AND OBJECTID < 10

1       Geometry        05280   Bellingham      53      Washington      5305280 city    N       -99     52179   2
2       Geometry        35050   Havre   30      Montana 3035050 city    N    2494    10201   1
3       Geometry        01990   Anacortes       53      Washington      5301990 city    N       -99     11451   1
4       Geometry        47560   Mount Vernon    53      Washington      5347560 city    N       -99     17647   1
5       Geometry        50360   Oak Harbor      53      Washington      5350360 city    N       -99     17176   1
6       Geometry        53380   Minot   38      North Dakota    3853380 city    N       1580    34544   2
7       Geometry        40075   Kalispell       30      Montana 3040075 city    N       2955    11917   1
8       Geometry        86220   Williston       38      North Dakota    3886220 city    N       1882    13131   1
9       Geometry        55365   Port Angeles    53      Washington      5355365 city    N       32      17710   1

UPDATE CitiesNoGeo SET POP1990 = 65678 WHERE STATE_CITY = '0659962'

SELECT CITY_NAME, POP1990 FROM CitiesNoGeo WHERE STATE_CITY = '0659962'

Changed the population of Redlands from 60394 to 65678

UPDATE CitiesNoGeo SET POP1990 = 60394 WHERE STATE_CITY = '0659962'

SELECT CITY_NAME, POP1990 FROM CitiesNoGeo WHERE STATE_CITY = '0659962'

Changed the population of Redlands back to 60394

FeatureDatasets:
*first run

The geodatabase does not exist, no need to delete
The geodatabase has been created.
The feature dataset has been created.
The table has been created.

*second run

The geodatabase has been deleted
The geodatabase has been created.
The feature dataset has been created.
The table has been created.

GeodatabaseManagement:

The geodatabase has been created.
The geodatabase has been opened.
The geodatabase has been deleted.

ProcessTopologies (with libXml installed):

USA_Topology
esriTRTPointProperlyInsideArea Origin Class: cities Destination Class: states
esriTRTAreaCoveredByAreaClass Origin Class: counties Destination Class: states

Querying:

Bellingham	52179	-122.468,48.7439
Havre	10201	-109.68,48.5438
Anacortes	11451	-122.631,48.4922
Mount Vernon	17647	-122.316,48.4216
Oak Harbor	17176	-122.629,48.3079
Minot	34544	-101.297,48.2337
Kalispell	11917	-114.318,48.1993
Williston	13131	-103.631,48.1623
Port Angeles	17710	-123.456,48.105

Commerce
Pico Rivera
Maywood
Huntington Park
Bell
West Whittier-Los Nietos
Bell Gardens
Whittier
Cudahy
South Gate
Downey
Santa Fe Springs
South Whittier
Lynwood
Norwalk
La Mirada
Paramount
Bellflower
Cerritos
Artesia
Buena Park
La Palma
Lakewood
Hawaiian Gardens
Cypress
Stanton
Los Alamitos
Long Beach
Seal Beach
Westminster
Huntington Beach


Querying compressed data...

Bellingham      52179   -122.468,48.7439
Havre   10201   -109.68,48.5438
Anacortes       11451   -122.631,48.4922
Mount Vernon    17647   -122.316,48.4216
Oak Harbor      17176   -122.629,48.3079
Minot   34544   -101.297,48.2337
Kalispell       11917   -114.318,48.1993
Williston       13131   -103.631,48.1623
Port Angeles    17710   -123.456,48.105

TableSchema\TableSchemaXML:

*first run

The geodatabase does not exist, no need to delete
The geodatabase has been created.
The streets table has been created.
The StreetType field has been added.
The SpeedLimit field has been deleted.
The StreetType field has been indexed.
Enabled the Trunk Highways subtype
The Default Subtype code is: 99
The Default Subtype code is now: 87
Altered the Trunk Highways subtype
Added Local Streets subtype
Deleted the Local Streets subtype

*second run

The geodatabase has been deleted
The geodatabase has been created.
The streets table has been created.
The StreetType field has been added.
The SpeedLimit field has been deleted.
The StreetType field has been indexed.
Enabled the Trunk Highways subtype
The Default Subtype code is: 99
The Default Subtype code is now: 87
Altered the Trunk Highways subtype
Added Local Streets subtype
Deleted the Local Streets subtype

ShapeBuffer:
Testing Point Shapes, Read

Point test:
x: -122.113 y: 47.746

Testing Point Shapes, Write

Inserted a point.

Testing Line Shapes, Read

Line test:
Points: 1530
Parts: 3
Part 0
0 -118.486, 34.0147
1 -118.475, 34.0227
2 -118.454, 34.0276
3 -118.449, 34.0282
4 -118.443, 34.0297
5 -118.434, 34.0311
6 -118.431, 34.0315
7 -118.422, 34.0318
8 -118.417, 34.0318
9 -118.395, 34.029
10 -118.382, 34.036
11 -118.378, 34.0368
12 -118.376, 34.0364
13 -118.369, 34.0353
14 -118.368, 34.0351
15 -118.362, 34.0338
16 -118.35, 34.0341
17 -118.334, 34.0351
18 -118.309, 34.0371
19 -118.292, 34.037
20 -118.284, 34.0372
21 -118.28, 34.0379
22 -118.274, 34.0383
23 -118.27, 34.0366
24 -118.267, 34.0345
25 -118.263, 34.0325
26 -118.256, 34.0296
27 -118.249, 34.026
28 -118.245, 34.0236
29 -118.242, 34.0238
30 -118.239, 34.0245
31 -118.234, 34.0264
32 -118.232, 34.0274
33 -118.226, 34.0286
34 -118.221, 34.0339
Part 1
35 -118.214, 34.0551
36 -118.207, 34.0536
37 -118.203, 34.0546
38 -118.185, 34.0555
39 -118.173, 34.0607
40 -118.165, 34.0612
41 -118.146, 34.0711
42 -118.134, 34.0714
43 -118.123, 34.0715
44 -118.108, 34.0717

Gap...

925 -95.4095, 29.7775
926 -95.4007, 29.7765
927 -95.3849, 29.7769
928 -95.3762, 29.779
929 -95.3688, 29.7755
930 -95.3656, 29.7699
931 -95.3631, 29.768
932 -95.3594, 29.7671
933 -95.3505, 29.77
934 -95.3491, 29.77
935 -95.3413, 29.7696
936 -95.3362, 29.7691
937 -95.3227, 29.774
938 -95.3154, 29.7742
939 -95.3016, 29.7742
940 -95.2924, 29.7775
941 -95.284, 29.778
Part 2
942 -95.2585, 29.7747
943 -95.2372, 29.7738
944 -95.2213, 29.7706
945 -95.2133, 29.7707
946 -95.1997, 29.7716
947 -95.1838, 29.7706
948 -95.1538, 29.7714
949 -95.1251, 29.7781
950 -95.0922, 29.7899

Gap...

1510 -82.1231, 30.259
1511 -82.1142, 30.2592
1512 -82.1046, 30.2629
1513 -82.0704, 30.2774
1514 -82.0496, 30.2814
1515 -82.0376, 30.2834
1516 -82.0214, 30.2861
1517 -81.9828, 30.289
1518 -81.9586, 30.2912
1519 -81.9272, 30.301
1520 -81.8998, 30.3044
1521 -81.8487, 30.309
1522 -81.7999, 30.3148
1523 -81.7705, 30.3153
1524 -81.7426, 30.3155
1525 -81.7308, 30.3174
1526 -81.7226, 30.3207
1527 -81.6992, 30.3215
1528 -81.6908, 30.3206
1529 -81.6821, 30.3213

Testing Line Shapes, Write

-81.6821, 30.3213
Inserted a line.

Testing Line Shapes with Zs

Line with Z test:
Points: 989
Parts: 1
Part 0
0 86899, 889938, 120.853
1 86902.3, 889951, 121.048
2 86912.5, 889992, 121.627
3 86913.7, 889997, 121.641
4 86924.8, 890041, 121.602
5 86933.7, 890077, 121.318
6 86937.8, 890093, 121.099
7 86944.9, 890122, 121.203
8 86945.2, 890126, 121.216
9 86947.4, 890156, 121.196
10 86950.8, 890204, 121.026
11 86952.1, 890222, 120.961
12 86952.3, 890226, 120.913
13 86955.5, 890270, 123.852
14 86956.5, 890284, 124.422
15 86959.9, 890331, 124.042
16 86962, 890361, 123.806
17 86962.1, 890363, 123.804

Gap...

971 87242.8, 890094, 119.67
972 87243.2, 890087, 119.67
973 87243.3, 890086, 119.67
974 87243.9, 890078, 119.67
975 87244, 890076, 119.67
976 87244, 890076, 119.67
977 87244.2, 890074, 119.67
978 87244.7, 890067, 119.67
979 87244.7, 890067, 119.67
980 87245.2, 890060, 119.67
981 87245.2, 890059, 119.67
982 87245.8, 890052, 119.67
983 87245.9, 890049, 119.67
984 87246.2, 890046, 119.67
985 87246.9, 890036, 119.67
986 87246.9, 890035, 119.69
987 87248, 890021, 120.251
988 87248, 890021, 120.25

Testing Line Shapes, Write

xMin: 85659
yMin: 889938
xMax: 87661.3
yMax: 893346
zMin: 119.67
zMax: 280.799
Inserted a line with z's.
xMin: 85659
yMin: 889938
xMax: 87661.3
yMax: 893346
zMin: 119.67
zMax: 280.799

Testing Polygon Shapes

Polygon test:
Points: 41
Parts: 3
xMin: -71.8667
yMin: 41.3228
xMax: -71.1171
yMax: 42.0137
Part 0
0 -71.7902, 41.6013
1 -71.7926, 41.6418
2 -71.7882, 41.7216
3 -71.7978, 42.0043
4 -71.4974, 42.0093
5 -71.3786, 42.0137
6 -71.3824, 41.9793
7 -71.384, 41.8884
8 -71.3331, 41.896
9 -71.3425, 41.8758
10 -71.3345, 41.8579
11 -71.3455, 41.8132
12 -71.3398, 41.7844
13 -71.3193, 41.7722
14 -71.2666, 41.7497
15 -71.229, 41.7077
16 -71.284, 41.6795
17 -71.3674, 41.7414
18 -71.3936, 41.7612
19 -71.369, 41.7033
20 -71.4192, 41.6522
21 -71.4273, 41.4867
22 -71.4899, 41.3921
23 -71.7223, 41.3273
24 -71.8667, 41.3228
25 -71.8478, 41.3253
26 -71.8369, 41.342
27 -71.846, 41.4039
28 -71.8027, 41.4158
29 -71.7902, 41.6013
Part 1
30 -71.1988, 41.6785
31 -71.1412, 41.6553
32 -71.1171, 41.4931
33 -71.1999, 41.4633
34 -71.1988, 41.6785
Part 2
35 -71.2692, 41.6213
36 -71.2194, 41.6356
37 -71.2387, 41.4748
38 -71.288, 41.4836
39 -71.3495, 41.4459
40 -71.2692, 41.6213

Testing Polygon Shapes, Write

Inserted a polygon.

Testing Multipoint

Multipoint test:
Points: 9
0 6.30115e+006, 1.91301e+006, 435
1 6.30117e+006, 1.91333e+006, 424
2 6.30144e+006, 1.91356e+006, 654
3 6.30145e+006, 1.91284e+006, 454
4 6.30157e+006, 1.91254e+006, 654
5 6.3016e+006, 1.91314e+006, 654
6 6.30183e+006, 1.91279e+006, 556
7 6.30198e+006, 1.91328e+006, 564
8 6.30222e+006, 1.91256e+006, 899

Testing Multipoint Shapes, Write

Inserted a multipoint.



SAMPLE PROGRAM EXPECTED RESULTS (C#)

Domains.exe
-----------

The SpeedLimit domain has been deleted.
The RoadSurfaceType domain has been deleted.
The SpeedLimit range domain has been created.
The definition of the MedianType domain is: <Domain xsi:type='esri:CodedValueDomain' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:esri='http://www.esri.com/schemas/ArcGIS/10.0'><DomainName>MedianType</DomainName><FieldType>esriFieldTypeSmallInteger</FieldType><MergePolicy>esriMPTDefaultValue</MergePolicy><SplitPolicy>esriSPTDefaultValue</SplitPolicy><Description>Road median types.</Description><Owner></Owner><CodedValues xsi:type='esri:ArrayOfCodedValue'><CodedValue xsi:type='esri:CodedValue'><Name>None</Name><Code xsi:type='xs:short'>0</Code></CodedValue><CodedValue xsi:type='esri:CodedValue'><Name>Cement</Name><Code xsi:type='xs:short'>1</Code></CodedValue></CodedValues></Domain>
The RoadSurfaceType coded value domain has been created.
The RoadSurfaceType coded value domain has been Altered.

Editing.exe
-----------
Inserted two strings, one integer and a point.
Updated the CLASS field.

ExecutingSQL.exe
----------------
Bellingham	52179
Havre	10201
Anacortes	11451
Mount Vernon	17647
Oak Harbor	17176
Minot	34544
Kalispell	11917
Williston	13131
Port Angeles	17710
1	Geometry	05280	Bellingham	53	Washington	5305280	city	N	-99	52179	2	
2	Geometry	35050	Havre	30	Montana	3035050	city	N	2494	10201	1	
3	Geometry	01990	Anacortes	53	Washington	5301990	city	N	-99	11451	1	
4	Geometry	47560	Mount Vernon	53	Washington	5347560	city	N	-99	17647	1	
5	Geometry	50360	Oak Harbor	53	Washington	5350360	city	N	-99	17176	1	
6	Geometry	53380	Minot	38	North Dakota	3853380	city	N	1580	34544	2	
7	Geometry	40075	Kalispell	30	Montana	3040075	city	N	2955	11917	1	
8	Geometry	86220	Williston	38	North Dakota	3886220	city	N	1882	13131	1	
9	Geometry	55365	Port Angeles	53	Washington	5355365	city	N	32	17710	1

FeatureDatasets.exe
-------------------
*first run

The geodatabase does not exist, no need to delete
The geodatabase has been created.
The feature dataset has been created.
The table has been created.

*second run

The geodatabase has been deleted
The geodatabase has been created.
The feature dataset has been created.
The table has been created.

GeodatabaseManagement.exe
-------------------------
The geodatabase has been created.
The geodatabase has been opened.
The geodatabase has been deleted.

Querying.exe
------------
Bellingham	52179	-122.4682,48.7439
Havre	10201	-109.6799,48.5438
Anacortes	11451	-122.6307,48.4922
Mount Vernon	17647	-122.3158,48.4216
Oak Harbor	17176	-122.6289,48.3079
Minot	34544	-101.2965,48.2337
Kalispell	11917	-114.318,48.1993
Williston	13131	-103.6314,48.1623
Port Angeles	17710	-123.4558,48.105

Commerce
Pico Rivera
Maywood
Huntington Park
Bell
West Whittier-Los Nietos
Bell Gardens
Whittier
Cudahy
South Gate
Downey
Santa Fe Springs
South Whittier
Lynwood
Norwalk
La Mirada
Paramount
Bellflower
Cerritos
Artesia
Buena Park
La Palma
Lakewood
Hawaiian Gardens
Cypress
Stanton
Los Alamitos
Long Beach
Seal Beach
Westminster
Huntington Beach

ShapeBuffer.exe
---------------
Testing Point Shapes, Read
Point test:
x: -122.113135612194 y: 47.7459796279748

Testing Point Shapes, Write
Inserted a point.

Testing Line Shapes, Read
Line test:
Points: 1530
Parts: 3
Part 0
0 -118.485959167132, 34.0147391159564
1 -118.47513304879, 34.0226920378011
2 -118.453953580783, 34.0275981950666
3 -118.449223465899, 34.0282392430885
4 -118.442890323577, 34.0297262859543

Gap...

31 -118.234358571911, 34.0263840609057
32 -118.232275513388, 34.0274059956741
33 -118.226133325065, 34.0285548885407
34 -118.220945200346, 34.033902601464
Part 1
35 -118.213980139, 34.0551495899448
36 -118.20697589621, 34.053569604611
37 -118.202908769764, 34.0545765274485
38 -118.184796179266, 34.0554963431192
39 -118.172718815048, 34.0607100222528

Gap...

937 -95.3227246332516, 29.7740364589253
938 -95.3153844258404, 29.7742274195601
939 -95.3015900393975, 29.7741973578072
940 -95.2923737517326, 29.7775362107718
941 -95.2840045128787, 29.7779961581848
Part 2
942 -95.2585368277109, 29.7747271454029
943 -95.2372422536272, 29.773808093488
944 -95.2212738514521, 29.7705771438914
945 -95.2132856351876, 29.7706781145041

Gap...

1524 -81.7425882365167, 30.315519994108
1525 -81.7308467592697, 30.3174080482186
1526 -81.7225534557689, 30.3207380685704
1527 -81.6992214526952, 30.3215402003823
1528 -81.6907980693793, 30.3205502595642
1529 -81.6821307044713, 30.32133030405

Testing Line Shapes, Write
-81.6821307044713, 30.32133030405
Inserted a line.

Testing Line Shapes with Zs
Line with Z test:
Points: 989
Parts: 1
Part 0
0 86898.9662000015, 889937.9921, 120.852597374023
1 86902.2827999964, 889951.258699998, 121.048097374023
2 86912.5328999981, 889992.2588, 121.626697374023
3 86913.7264000028, 889997.033, 121.640897374023
4 86924.7942000031, 890041.304000001, 121.601597374023

Gap...

984 87246.1821999997, 890045.993099999, 119.669997374024
985 87246.9201999977, 890035.661699999, 119.669997374024
986 87246.9397, 890035.388900001, 119.689597374023
987 87247.9790999964, 890020.8365, 120.251397374023
988 87247.9919999987, 890020.656100001, 120.250497374023
Testing Line Shapes, Write
Inserted a line with z's.
Testing Polygon Shapes
Polygon test:
Points: 41
Parts: 3
Part 0
0 -71.7901942031214, 41.601306879325
1 -71.7926052182919, 41.6417579304638
2 -71.7882488621949, 41.7216033953239
3 -71.7978316087619, 42.0042748046852
4 -71.4974303691298, 42.0092535031423
5 -71.3786442228911, 42.0137133195163
6 -71.3824052822434, 41.9792630768653
7 -71.3839531547034, 41.8884397544727
8 -71.3330859502879, 41.8960311596525
9 -71.3424931202155, 41.8757828914979
10 -71.3345427095385, 41.8579036075384
11 -71.3454831662469, 41.8131613833437
12 -71.33979862314, 41.7844255626959
13 -71.3193277902705, 41.7721958646077
14 -71.2666285816006, 41.7497430522048
15 -71.2289761591778, 41.7076939683674
16 -71.2840016520154, 41.6795489704364
17 -71.3673874451962, 41.7413502009833
18 -71.3935805976545, 41.7611558353253
19 -71.3690125475301, 41.7032911019031
20 -71.4192468515382, 41.6522122329239
21 -71.427318519639, 41.4866893796323
22 -71.4898880400564, 41.3920853196252
23 -71.7222643227053, 41.3272643121838
24 -71.866678442895, 41.3227696452716
25 -71.8477722040922, 41.3253484832965
26 -71.8368696812943, 41.3419614666217
27 -71.8459956537022, 41.4038545416489
28 -71.8027434308056, 41.4158290540059
29 -71.7901942031214, 41.601306879325
Part 1
30 -71.1988086806703, 41.6785003452843
31 -71.1412126344661, 41.6552730544524
32 -71.1171327154705, 41.4930619563068
33 -71.1999371652607, 41.4633184834307
34 -71.1988086806703, 41.6785003452843
Part 2
35 -71.2691694549115, 41.6212683171834
36 -71.2194468005559, 41.635642312712
37 -71.2386732340456, 41.4748497781272
38 -71.2880071528611, 41.4836193369166
39 -71.3495250332551, 41.4458577414549
40 -71.2691694549115, 41.6212683171834
Testing Polygon Shapes, Write
Inserted a polygon.
Testing Multipoint
Multipoint test:
Points: 9
0 6301153.87493064, 1913012.75794823, 435
1 6301172.62522122, 1913331.50796689, 424
2 6301435.1250243, 1913556.50784498, 654
3 6301453.8749868, 1912844.00795764, 454
4 6301566.37508988, 1912544.00790147, 654
5 6301603.87501489, 1913144.00801381, 654
6 6301828.87522106, 1912787.75807014, 556
7 6301978.87492105, 1913275.25775132, 564
8 6302222.62508972, 1912562.75786398, 899
Testing Multipoint Shapes, Write
Inserted a multipoint.

TableSchema.exe
---------------
*first run

The geodatabase does not exist, no need to delete
The geodatabase has been created.
The streets table has been created.
The StreetType field has been added.
The SpeedLimit field has been deleted.
The StreetType field has been indexed.
Enabled the Trunk Highways subtype
The Default Subtype code is: 99
The Default Subtype code is now: 87
Altered the Trunk Highways subtype
Added Local Streets subtype
Deleted the Local Streets subtype

*second run

The geodatabase has been deleted
The geodatabase has been created.
The streets table has been created.
The StreetType field has been added.
The SpeedLimit field has been deleted.
The StreetType field has been indexed.
Enabled the Trunk Highways subtype
The Default Subtype code is: 99
The Default Subtype code is now: 87
Altered the Trunk Highways subtype
Added Local Streets subtype
Deleted the Local Streets subtype



  
