package org.ossim.oms.apps;
import joms.oms.Init;
import joms.oms.ossimUnitConversionTool;
import joms.oms.ossimUnitType;

public class UnitConversionToolTest
{
   public void usage()
   {
      System.out.println("USAGE: UnitConversionToolTest <value> <value-unit-type>");
      System.out.println("<value-unit-type> can be one of");
      System.out.println("     feet, meters, us-survey-feet, degrees, minutes, seconds, radians, miles, nautical-miles, millimeters, microns, centimeters, yards, inches");
   }
   public void printAllUnits(ossimUnitConversionTool tool)
   {
      System.out.println("***************** LINEAR UNITS ********************");
      System.out.println("OSSIM_METERS          = " + tool.getValue(ossimUnitType.OSSIM_METERS));
      System.out.println("OSSIM_KILOMETERS      = " + tool.getValue(ossimUnitType.OSSIM_KILOMETERS));
      System.out.println("OSSIM_CENTIMETERS     = " + tool.getValue(ossimUnitType.OSSIM_CENTIMETERS));
      System.out.println("OSSIM_MILLIMETERS     = " + tool.getValue(ossimUnitType.OSSIM_MILLIMETERS));
      System.out.println("OSSIM_MICRONS         = " + tool.getValue(ossimUnitType.OSSIM_MICRONS));
      System.out.println("OSSIM_MILES           = " + tool.getValue(ossimUnitType.OSSIM_MILES));
      System.out.println("OSSIM_NAUTICAL_MILES  = " + tool.getValue(ossimUnitType.OSSIM_NAUTICAL_MILES));
      System.out.println("OSSIM_YARDS           = " + tool.getValue(ossimUnitType.OSSIM_YARDS));
      System.out.println("OSSIM_FEET            = " + tool.getValue(ossimUnitType.OSSIM_FEET));
      System.out.println("OSSIM_US_SURVEY_FEET  = " + tool.getValue(ossimUnitType.OSSIM_US_SURVEY_FEET));
      System.out.println("OSSIM_INCHES          = " + tool.getValue(ossimUnitType.OSSIM_INCHES));
      System.out.println("***************** ANGULAR UNITS ********************");
      System.out.println("OSSIM_DEGREES         = " + tool.getValue(ossimUnitType.OSSIM_DEGREES));
      System.out.println("OSSIM_RADIANS         = " + tool.getValue(ossimUnitType.OSSIM_RADIANS));
      System.out.println("OSSIM_SECONDS         = " + tool.getValue(ossimUnitType.OSSIM_SECONDS));
      System.out.println("OSSIM_MINUTES         = " + tool.getValue(ossimUnitType.OSSIM_MINUTES));
   }
   public static void main(String[] args)
   {
      UnitConversionToolTest test = new UnitConversionToolTest();
      if(args.length != 2)
      {
         test.usage();
         return;
      }
      
      ossimUnitConversionTool tool = new ossimUnitConversionTool();
      if(args[1].equals("feet"))
      {
         tool.setValue(Double.valueOf(args[0]), ossimUnitType.OSSIM_FEET);
      }
      else if(args[1].equals("meters"))
      {
         tool.setValue(Double.valueOf(args[0]), ossimUnitType.OSSIM_METERS);
      }
      else if(args[1].equals("us-survey-feet"))
      {
         tool.setValue(Double.valueOf(args[0]), ossimUnitType.OSSIM_US_SURVEY_FEET);
      }
      else if(args[1].equals("degrees"))
      {
         tool.setValue(Double.valueOf(args[0]), ossimUnitType.OSSIM_DEGREES);
      }
      else if(args[1].equals("minutes"))
      {
         tool.setValue(Double.valueOf(args[0]), ossimUnitType.OSSIM_MINUTES);
      }
      else if(args[1].equals("seconds"))
      {
         tool.setValue(Double.valueOf(args[0]), ossimUnitType.OSSIM_SECONDS);
      }
      else if(args[1].equals("radians"))
      {
         tool.setValue(Double.valueOf(args[0]), ossimUnitType.OSSIM_RADIANS);
      }
      else if(args[1].equals("miles"))
      {
         tool.setValue(Double.valueOf(args[0]), ossimUnitType.OSSIM_MILES);
      }
      else if(args[1].equals("nautical-miles"))
      {
         tool.setValue(Double.valueOf(args[0]), ossimUnitType.OSSIM_NAUTICAL_MILES);
      }
      else if(args[1].equals("millimeters"))
      {
         tool.setValue(Double.valueOf(args[0]), ossimUnitType.OSSIM_MILLIMETERS);
      }
      else if(args[1].equals("microns"))
      {
         tool.setValue(Double.valueOf(args[0]), ossimUnitType.OSSIM_MICRONS);
      }
      else if(args[1].equals("centimeters"))
      {
         tool.setValue(Double.valueOf(args[0]), ossimUnitType.OSSIM_CENTIMETERS);
      }
      else if(args[1].equals("yards"))
      {
         tool.setValue(Double.valueOf(args[0]), ossimUnitType.OSSIM_YARDS);
      }
      else if(args[1].equals("inches"))
      {
         tool.setValue(Double.valueOf(args[0]), ossimUnitType.OSSIM_INCHES);
      }
      else
      {
         System.out.println("Unrecognized value type " + args[1]);
         test.usage();
         return;
      }
      test.printAllUnits(tool);
   }
}
