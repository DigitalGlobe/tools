//
// Sample: Display
//
// Uses glut to display a point feature class. To build this program you will have to 
// install glut and adjust the project settings or make file to reference your install.
// 

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

#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glut.h>

#include <FileGDBAPI.h>

using namespace std;
using namespace FileGDBAPI;

// Inland Southern California  only
const double xMin = -117.6;
const double yMin =  33.5;
const double xMax = -116.9;
const double yMax =  34.2;

struct city
{
  double  xCoord;
  double  yCoord;
  string  cityName;
  int     popCategory;
};

vector<city> cities;

long LoadCities(double xMin, double yMin, double xMax, double yMax, vector<city>& results)
{
  // Open the geodatabase and the table.
  fgdbError   hr;
  Geodatabase geodatabase;
  if ((hr = OpenGeodatabase(L"../data/Querying.gdb", geodatabase)) != S_OK)
    return hr;

  Table table;
  if ((hr = geodatabase.OpenTable(L"Cities", table)) != S_OK)
    return hr;

  // Create an envelope and perform the query.
  Envelope envelope;
  envelope.xMin = xMin;
  envelope.yMin = yMin;
  envelope.xMax = xMax;
  envelope.yMax = yMax;
  EnumRows enumRows;
  if ((hr = table.Search(L"SHAPE, CITY_NAME, POPCAT", L"", envelope, true, enumRows)) != S_OK)
    return hr;

  // Iterate through the results, populating the results vector.
  results.clear();
  Row              row;
  PointShapeBuffer geometry;
  Point*           point;
  while (enumRows.Next(row) == S_OK)
  {
    city    currentCity;
    wstring cityName;
    string  simpleString;

    row.GetString(L"CITY_NAME", cityName);

    // Convert wstring to string. The conversion used here may only be
    // good for English characters.
    currentCity.cityName = simpleString.assign(cityName.begin(), cityName.end());

    // Extract the geometry
    row.GetGeometry(geometry);
    geometry.GetPoint(point);
    currentCity.xCoord = point->x;
    currentCity.yCoord = point->y;
    int32 popCat;
    row.GetInteger(L"PopCat", popCat);
    currentCity.popCategory = popCat;

    // Add the city to the vector.
    results.push_back(currentCity);
  }

  // Close the table
  if ((hr = geodatabase.CloseTable(table)) != S_OK)
  {
    wcout << "An error occurred while closing Cities." << endl;
    wcout << "Error code: " << hr << endl;
    return -1;
  }

  // Close the geodatabase
  if ((hr = CloseGeodatabase(geodatabase)) != S_OK)
  {
    wcout << "An error occurred while closing the geodatabase." << endl;
    wcout << "Error code: " << hr << endl;
    return -1;
  }
  return S_OK;
}

void DrawCity(double xCoord, double yCoord, long popCategory, string name)
{
  // Setup the label properties.
  const char* cityName = name.c_str();
  double      labelX = xCoord;
  double      labelY = yCoord - 0.01;

  // Draw the city's symbol and label.
  switch (popCategory)
  {
    case 1:
      // For small cities, draw a green diamond.
      glBegin(GL_QUADS);
      glColor3f(0.0, 1.0, 0.0);
      glVertex3f(xCoord, yCoord - 0.002, 0);
      glVertex3f(xCoord + 0.002, yCoord, 0);
      glVertex3f(xCoord, yCoord + 0.002, 0);
      glVertex3f(xCoord - 0.002, yCoord, 0);
      glEnd();
    break;

    case 2:
      // For mid-size cities, draw a blue square.
      glBegin(GL_QUADS);
      glColor3f(0.0, 0.0, 1.0);
      glVertex3f(xCoord - 0.002, yCoord - 0.002, 0);
      glVertex3f(xCoord + 0.002, yCoord - 0.002, 0);
      glVertex3f(xCoord + 0.002, yCoord + 0.002, 0);
      glVertex3f(xCoord - 0.002, yCoord + 0.002, 0);
      glEnd();
    break;

    case 3:
      // For large cities, draw a red triangle.
      glBegin(GL_TRIANGLES);
      glColor3f(1.0, 0.0, 0.0);
      glVertex3f(xCoord, yCoord + 0.0025, 0);
      glVertex3f(xCoord - 0.0025, yCoord - 0.0025, 0);
      glVertex3f(xCoord + 0.0025, yCoord - 0.0025, 0);
      glEnd();
    break;
  }

  // Draw the label.
  glRasterPos2f(labelX, labelY);
  while (*cityName)
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *cityName);
    cityName++;
  }
}

void Display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  // Determine the map extent.
  double deltaX = xMax - xMin;
  double deltaY = yMax - yMin;
  if (deltaX > deltaY)
  {
    double centerY = (yMax + yMin) / 2;
    glOrtho(xMin, xMax, centerY - (deltaX / 2), centerY + (deltaX / 2), -1.0, 1.0);
  }
  else
  {
    double centerX = (xMax + xMin) / 2;
    glOrtho(centerX - (deltaY / 2), centerX + (deltaY / 2), yMin, yMax, -1.0, 1.0);
  }

  // Draw the cities.
  for (unsigned int i = 0; i < cities.size(); i++)
  {
    city currentCity = cities.at(i);
    DrawCity(currentCity.xCoord, currentCity.yCoord, currentCity.popCategory, currentCity.cityName);
  }

  glFlush();			//Finish rendering
}

int main(int argc, char **argv)
{
  // Initialize GLUT.
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(800, 800);
  glutCreateWindow("Display Spatial Query");
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glutDisplayFunc(Display);

  // Load the cities based on a search extent.
  LoadCities(xMin, yMin, xMax, yMax, cities);

  // Run.
  glutMainLoop();

  return 0;
}
