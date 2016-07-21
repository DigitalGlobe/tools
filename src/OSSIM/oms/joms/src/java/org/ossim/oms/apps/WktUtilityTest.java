package org.ossim.oms.apps;
import joms.oms.Init;
import joms.oms.WktUtility;
import joms.oms.ossimUnitType;
import joms.oms.ossimGpt;

public class WktUtilityTest
{
   public static void usage()
   {
      System.out.println("WktUtilityTest <lat> <lon> <radius> <radius-unit-type>");
   }
   public static void main(String[] args)
   {
      WktUtilityTest test = new WktUtilityTest();
      ossimUnitType unitType = ossimUnitType.OSSIM_METERS;
      if(args.length != 4)
      {
         usage();
         return;
      }
      if(args[1].equals("feet"))
      {
         unitType = ossimUnitType.OSSIM_FEET;
      }
      else if(args[3].equals("meters"))
      {
         unitType = ossimUnitType.OSSIM_METERS;
      }
      else if(args[3].equals("us-survey-feet"))
      {
         unitType = ossimUnitType.OSSIM_US_SURVEY_FEET;
      }
      else if(args[3].equals("degrees"))
      {
         unitType = ossimUnitType.OSSIM_DEGREES;
      }
      else if(args[3].equals("minutes"))
      {
         unitType = ossimUnitType.OSSIM_MINUTES;
      }
      else if(args[3].equals("seconds"))
      {
         unitType = ossimUnitType.OSSIM_SECONDS;
      }
      else if(args[3].equals("radians"))
      {
         unitType = ossimUnitType.OSSIM_RADIANS;
      }
      else if(args[3].equals("miles"))
      {
         unitType = ossimUnitType.OSSIM_MILES;
      }
      else if(args[3].equals("nautical-miles"))
      {
         unitType = ossimUnitType.OSSIM_NAUTICAL_MILES;
      }
      else if(args[3].equals("millimeters"))
      {
         unitType = ossimUnitType.OSSIM_MILLIMETERS;
      }
      else if(args[3].equals("microns"))
      {
         unitType = ossimUnitType.OSSIM_MICRONS;
      }
      else if(args[3].equals("centimeters"))
      {
         unitType = ossimUnitType.OSSIM_CENTIMETERS;
      }
      else if(args[3].equals("yards"))
      {
         unitType = ossimUnitType.OSSIM_YARDS;
      }
      else if(args[3].equals("inches"))
      {
         unitType = ossimUnitType.OSSIM_INCHES;
      }
      else
      {
         System.out.println("Unrecognized value type " + args[3]);
         test.usage();
         return;
      }
      ossimGpt centerGpt = new ossimGpt(Double.valueOf(args[0]),
                                        Double.valueOf(args[1]));
      double radius = Double.valueOf(args[2]);
      WktUtility wktUtility = new WktUtility();
      
      System.out.println(wktUtility.toWktGeometryGivenCenterRadius(centerGpt, radius, unitType, 360, -1));
   }
}