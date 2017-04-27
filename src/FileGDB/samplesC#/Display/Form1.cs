using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using OpenTK.Graphics;
using OpenTK.Graphics.OpenGL;
using Esri.FileGDB;

namespace Display
{
  public class city
  {
    public double xCoord;
    public double yCoord;
    public string cityName;
    public int popCategory;
  };

  public partial class Form1 : Form
  {
    bool loaded = false;

    private List<city> cities = new List<city>();

    // Inland Southern California  only
    const double xMin = -117.6;
    const double yMin = 33.5;
    const double xMax = -116.9;
    const double yMax = 34.2;

    public Form1()
    {
      InitializeComponent();
      // Load the cities based on a search extent.
      if (LoadCities() != 0)
      {
        this.Close();
      }
    }

    private void glControl1_Load(object sender, EventArgs e)
    {
      loaded = true;

      int w = glControl1.Width;
      int h = glControl1.Height;
      GL.MatrixMode(MatrixMode.Projection);
      GL.LoadIdentity();

      // Determine the map extent.
      double deltaX = xMax - xMin;
      double deltaY = yMax - yMin;
      if (deltaX > deltaY)
      {
        double centerY = (yMax + yMin) / 2;
        GL.Ortho(xMin, xMax, centerY - (deltaX / 2), centerY + (deltaX / 2), -1.0, 1.0);
      }
      else
      {
        double centerX = (xMax + xMin) / 2;
        GL.Ortho(centerX - (deltaY / 2), centerX + (deltaY / 2), yMin, yMax, -1.0, 1.0);
      }

    }

    private void glControl1_Resize(object sender, EventArgs e)
    {
      if (!loaded)
        return;
    }

    private void glControl1_Paint(object sender, PaintEventArgs e)
    {
      if (!loaded) // Play nice
        return;

      GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);
      glControl1.SwapBuffers();

      GL.MatrixMode(MatrixMode.Modelview);
      GL.LoadIdentity();
      foreach (city currentCity in cities)
      {
        DrawCity(currentCity);
      }

      glControl1.SwapBuffers();
    }

    long LoadCities()
    {
      try
      {
        // Open the geodatabase.
        Geodatabase geodatabase = Geodatabase.Open("../../samples/data/Querying.gdb");

        Table table = geodatabase.OpenTable("Cities");

        // Create an envelope and perform the query.
        Envelope envelope = new Envelope();
        envelope.xMin = xMin;
        envelope.yMin = yMin;
        envelope.xMax = xMax;
        envelope.yMax = yMax;

        // Iterate through the results, populating the results vector.
        foreach (Row row in table.Search("SHAPE, CITY_NAME, POPCAT", "", envelope, RowInstance.Recycle))
        {
          city currentCity = new city();
          currentCity.cityName = row.GetString("CITY_NAME");
          PointShapeBuffer geometry = row.GetGeometry();
          Esri.FileGDB.Point point = geometry.point;
          currentCity.xCoord = point.x; 
          currentCity.yCoord = point.y; 
          currentCity.popCategory = row.GetInteger("PopCat");

          // Add the city to the vector.
          cities.Add(currentCity);
        }

        // Close the table
        table.Close();

        // Close the geodatabase
        geodatabase.Close();
      }
      catch (FileGDBException ex)
      {
        MessageBox.Show(string.Format("{0} - {1}", ex.Message, ex.ErrorCode));
        return -2;
      }
      catch (Exception ex)
      {
        MessageBox.Show("General exception.  " + ex.Message);
        return -1;
      }
      return 0;
    }

    void DrawCity(city currentCity)
    {
      // Draw the city's symbol and label.
      switch (currentCity.popCategory)
      {
        case 1: // For small cities, draw a green diamond.
          {
            GL.Begin(BeginMode.Quads);
              GL.Color3(Color.Green);
              GL.Vertex3(currentCity.xCoord, currentCity.yCoord - 0.02, 0);
              GL.Vertex3(currentCity.xCoord + 0.02, currentCity.yCoord, 0);
              GL.Vertex3(currentCity.xCoord, currentCity.yCoord + 0.02, 0);
              GL.Vertex3(currentCity.xCoord - 0.02, currentCity.yCoord, 0);
            GL.End();
          }
          break;
        case 2: // For mid-size cities, draw a blue square.
          {
            GL.Begin(BeginMode.Quads);
              GL.Color3(Color.Blue);
              GL.Vertex3(currentCity.xCoord - 0.02, currentCity.yCoord - 0.02, 0);
              GL.Vertex3(currentCity.xCoord + 0.02, currentCity.yCoord - 0.02, 0);
              GL.Vertex3(currentCity.xCoord + 0.02, currentCity.yCoord + 0.02, 0);
              GL.Vertex3(currentCity.xCoord - 0.02, currentCity.yCoord + 0.02, 0);
            GL.End();
          }
          break;
        case 3: // For large cities, draw a red triangle.
          {
            GL.Begin(BeginMode.Triangles);
              GL.Color3(Color.Red);
              GL.Vertex3(currentCity.xCoord, currentCity.yCoord + 0.025, 0);
              GL.Vertex3(currentCity.xCoord - 0.025, currentCity.yCoord - 0.025, 0);
              GL.Vertex3(currentCity.xCoord + 0.025, currentCity.yCoord - 0.025, 0);
            GL.End();
          }
          break;
      }
    }
  }
}
