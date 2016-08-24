
/***************************************************************************
 *
 *N  Module COORGEOM.C
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P 
 *   Coordinate Geometry Module
 *
 *   Contains functions for performing general purpose coordinate
 *   geometry operations.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * 
 *   History:
 *H
 *    Barry Michaels    Sep 1992
 *    Dan Maddux        Jan 1994
 *E
 **************************************************************************/

#include <math.h>
#include <stdlib.h>
#include <float.h>

#ifdef _MSDOS   
#include <limits.h>
#define MAXFLOAT FLT_MAX
#endif

#ifdef _MAC
#undef MAXFLOAT
#define MAXFLOAT    3.37E+38
#endif

#ifndef MISSING_FLOAT_H
#include <float.h>
#define MAXFLOAT FLT_MAX
#endif

#ifndef __COORGEOM_H__
#include "coorgeom.h"
#endif



/*************************************************************************
 *
 *N  gc_distance
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This function computes the distance between two points on the
 *    surface of the (spherical) earth.  The points are specified in
 *    geographic coordinates (lat1,lon1) and (lat2,lon2).  The algorithm
 *    used here is taken directly from ELEMENTS OF CARTOGRAPHY, 4e. -
 *    Robinson, Sale, Morrison - John Wiley & Sons, Inc. - pp. 44-45.
 *    Geometrically, the function computes the arc distance dtheta on
 *    the sphere between two points A and B by the following formula:
 *
 *              cos dtheta = (sin a sin b) + (cos a cos b cos p)
 *
 *              where:
 *
 *                 dtheta = arc distance between A and B
 *                 a = latitude of A
 *                 b = latitude of B
 *                 p = degrees of longitude between A and B
 *
 *    Once the arc distance is determined, it is converted into miles by
 *    taking the ratio between the circumference of the earth (2*PI*R) and
 *    the number of degrees in a circle (360):
 *
 *        distance in miles (d)            arc distance in degrees (dtheta)
 *   ------------------------------   =    --------------------------------
 *   earth's circumference in miles        earth's circumference in degrees
 *
 *                                    or
 *
 *    d = (dtheta * (2*PI*R)) / 360  =>
 *    d = (dtheta*PI*R)/180
 *
 *   The calculated distance is referred to as the Great Circle Distance.
 *
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    lat1  <input> == (double) latitude of point A.
 *    lon1  <input> == (double) longitude of point A.
 *    lat2  <input> == (double) latitude of point B.
 *    lon2  <input> == (double) longitude of point B.
 *    units <input> == (int32) flag to indicate output distance units.
 *                      Must be one of the coord_units_type enumerations.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   November 1990   Original Version   DOS Turbo C
 *                     September 1992  UNIX Port -  Additional Units
 *E
 *************************************************************************/
#ifdef PROTO
   double gc_distance( double lat1, double lon1, double lat2, double lon2, int32 units)
#else
   double gc_distance (lat1, lon1, lat2, lon2, units)
      double lat1, lon1, lat2, lon2;
      int32 units;
#endif
{
   double a,b,p,dtheta,d;
#ifndef PI
   double PI=3.141592654;
#endif
   double DEG2RAD=(PI/180.0), RAD2DEG=(180.0/PI);
   static double R[] =
   { 3958.754, 6370997.0, 1.00000, 2090221.12, 250826653.44  , 6370.997 };
   /* miles     meters     degrees   feet         inches      kilometers */

   if ((units<Miles)||(units>Kilometers)) units = Miles;

   if (lat1 > 90.0) lat1 -= 180.0;
   if (lat2 > 90.0) lat2 -= 180.0;

   a = lat1*DEG2RAD;  /* Degrees must be converted to radians */
   b = lat2*DEG2RAD;
   p = fabs(lon1-lon2)*DEG2RAD;
   dtheta = (sin(a)*sin(b)) + (cos(a)*cos(b)*cos(p));
   dtheta = acos(dtheta)*RAD2DEG;   /* Compute arc distance in degrees */
   d = (dtheta*PI*R[units])/180.0;  /* Compute distance in miles or km */
   return d;
}


/*************************************************************************/
/* geo_intersect                                                         */
/*       This function determines if any portion of an input             */
/*       rectangular "fextent1' is contained within another rectangular  */
/*       'fextent2'. It returns either TRUE of FALSE                     */
/*       DGM   21 Jan 94                                                 */
/*************************************************************************/
#ifdef PROTO
   int32 geo_intersect (fextent_type fextent1, fextent_type fextent2)
#else
   int32 geo_intersect (fextent1, fextent2)
      fextent_type fextent1, fextent2;
#endif
   {
   float merid_180;

   /* Check for crossing the 180 meridian */
   if (fextent1.x1 < -180.0)
      merid_180 = -1.0;
   else if (fextent1.x2 > 180.0)
      merid_180 = 1.0;
   else
      merid_180 = 0.0;

   if (fextent2.x1 > 0.0 && fextent2.x2 < 0.0 && fextent1.x1 > 0.0)
      merid_180 = 1.0;
   else if (fextent2.x1 > 0.0 && fextent2.x2 < 0.0 && fextent1.x1 < 0.0)
      merid_180 = -1.0;

   if ((fextent2.x1 >= fextent1.x1 && fextent2.x1 <= fextent1.x2) ||
       (fextent2.x2 >= fextent1.x1 && fextent2.x2 <= fextent1.x2))
      merid_180 = 0.0;

   if (merid_180 == -1.0)
      {
      if (fextent2.x1 > 0.0)
         fextent2.x1 = fextent2.x1 + merid_180 * 360.0;
      if (fextent2.x2 > 0.0)
         fextent2.x2 = fextent2.x2 + merid_180 * 360.0;
      }
   else if (merid_180 == 1.0)
      {
      if (fextent2.x1 < 0.0)
         fextent2.x1 = fextent2.x1 + merid_180 * 360.0;
      if (fextent2.x2 < 0.0)
         fextent2.x2 = fextent2.x2 + merid_180 * 360.0;
      }

   /* 1 intersects 2 from the top */
   if ((fextent1.y1 >= fextent2.y1) &&
       (fextent1.y1 <  fextent2.y2) &&
       (fextent1.x1 <  fextent2.x2) &&
       (fextent1.x2 >  fextent2.x1))
       return (TRUE);

   /* 1 intersects 2 from the bottom */
   if ((fextent1.y2 <= fextent2.y2) &&
       (fextent1.y2 >  fextent2.y1) &&
       (fextent1.x1 <  fextent2.x2) &&
       (fextent1.x2 >  fextent2.x1))
       return (TRUE);

   /* 1 intersects 2 from the right */
   if ((fextent1.x1 >= fextent2.x1) &&
       (fextent1.x1 <  fextent2.x2) &&
       (fextent1.y2 >  fextent2.y1) &&
       (fextent1.y1 <  fextent2.y2))
       return (TRUE);

   /* 1 intersects 2 from the left */
   if ((fextent1.x2 <= fextent2.x2) &&
       (fextent1.x2 >  fextent2.x1) &&
       (fextent1.y2 >  fextent2.y1) &&
       (fextent1.y1 <  fextent2.y2))
       return (TRUE);

   /* 1 contains 2 */
   if ((fextent1.x1 <= fextent2.x1) &&
       (fextent1.x2 >= fextent2.x2) &&
       (fextent1.y2 >= fextent2.y2)  &&
       (fextent1.y1 <= fextent2.y1))
       return (TRUE);

   /* 1 is contained by 2 */
   if ((fextent1.x1 >= fextent2.x1) &&
       (fextent1.x2 <= fextent2.x2) &&
       (fextent1.y2 <= fextent2.y2) &&
       (fextent1.y1 >= fextent2.y1))
       return (TRUE);

   return (FALSE);
   }


/*************************************************************************
 *
 *N  contained
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function determines whether any portion of an input rectangular
 *     'extent1' is contained within another rectangular 'extent2'.  It
 *     returns either TRUE or FALSE.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    extent1 <input> == (extent_type) rectangular area to be tested within.
 *    extent2 <input> == (extent_type) rectangular area to be tested against.
 *    contained <output> == (int32) boolean:
 *                               TRUE if extent2 contains extent1
 *                               FALSE if extent2 does not contain extent1
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1990   Original Version   DOS Turbo C
 *E
 *************************************************************************/
#ifdef PROTO
   int32 contained (extent_type extent1, extent_type extent2)
#else
   int32 contained (extent1, extent2)
      extent_type extent1, extent2;
#endif

   {
   /* 1 intersects 2 from the top */
   if ((extent1.y1 >= extent2.y1) &&
       (extent1.y1 <  extent2.y2) &&
       (extent1.x1 <  extent2.x2) &&
       (extent1.x2 >  extent2.x1))
       return (TRUE);

   /* 1 intersects 2 from the bottom */
   if ((extent1.y2 <= extent2.y2) &&
       (extent1.y2 >  extent2.y1) &&
       (extent1.x1 <  extent2.x2) &&
       (extent1.x2 >  extent2.x1))
       return (TRUE);

   /* 1 intersects 2 from the right */
   if ((extent1.x1 >= extent2.x1) &&
       (extent1.x1 <  extent2.x2) &&
       (extent1.y2 >  extent2.y1) &&
       (extent1.y1 <  extent2.y2))
       return (TRUE);

   /* 1 intersects 2 from the left */
   if ((extent1.x2 <= extent2.x2) &&
       (extent1.x2 >  extent2.x1) &&
       (extent1.y2 >  extent2.y1) &&
       (extent1.y1 <  extent2.y2))
       return (TRUE);

   /* 1 contains 2 */
   if ((extent1.x1 <= extent2.x1) &&
       (extent1.x2 >= extent2.x2) &&
       (extent1.y2 >= extent2.y2)  &&
       (extent1.y1 <= extent2.y1))
       return (TRUE);

   /* 1 is contained by 2 */
   if ((extent1.x1 >= extent2.x1) &&
       (extent1.x2 <= extent2.x2) &&
       (extent1.y2 <= extent2.y2) &&
       (extent1.y1 >= extent2.y1))
       return (TRUE);

   return (FALSE);
   }

/*************************************************************************
 *
 *N  completely_within
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This function determines whether extent1 is completely within
 *    extent2.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     extent1 <input>==(extent_type) first extent to compare.
 *     extent2 <input>==(extent_type) second extent to compare.
 *     return  <output>==(int32) 1 if extent1 is completely within extent2,
 *                             0 if not.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *    
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *E
 *************************************************************************/
#ifdef PROTO
   int32 completely_within (extent_type extent1, extent_type extent2)
#else
   int32 completely_within (extent1, extent2)
      extent_type extent1, extent2;
#endif

{
   if (extent1.x1 < extent2.x1) return 0;
   if (extent1.y1 < extent2.y1) return 0;
   if (extent1.x2 > extent2.x2) return 0;
   if (extent1.y2 > extent2.y2) return 0;
   return 1;
}
 



/*************************************************************************
 *
 *N  fwithin
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    This function determines whether the given (floating point) 
 *    coordinate is contained within the specified extent.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     x      <input>==(double) X coordiante.
 *     y      <input>==(double) Y coordiante.
 *     extent <input>==(extent_type) extent to compare.
 *     return <output>==(int32) TRUE if the coordinate is contained within
 *                            the extent; FALSE if not.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *    
 *   History:
 *H
 *    Barry Michaels   May 1991                           DOS Turbo C
 *                     Sep 1992    UNIX Port
 *E
 *************************************************************************/
#ifdef PROTO
   int32 fwithin (double x, double y, extent_type extent)
#else
   int32 fwithin (x, y, extent)
      double x, y;
      extent_type extent;
#endif

{
   if ( x >= extent.x1 && x <= extent.x2 &&
        y >= extent.y1 && y <= extent.y2 )
      return TRUE;
   else
      return FALSE;
}


/* Check if the value n0 is in between n1 and n2 */
#define BETWEEN(n0,n1,n2) ((n0>=n1 && n0<=n2) || (n0<=n1 && n0>=n2))

/**************************************************************************
 *
 *N  intersect
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Test whether line segment l1 intersects line segment l2
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    l1   <input> == (line_segment_type) line segment one.
 *    l2   <input> == (line_segment_type) line segment two.
 *    xint <inout> == (double *) intersection point x coordinate.
 *    yint <inout> == (double *) intersection point y coordinate.
 *    intersect <output> == (int32) boolean:
 *                                1 --> line segments intersect
 *                                0 --> line segments do not intersect
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels  DOS Turbo C
 *E
 *************************************************************************/
#ifdef PROTO
   int32 intersect (line_segment_type l1, line_segment_type l2,
                   double *xint, double *yint)
#else
   int32 intersect (l1, l2, xint, yint)
      line_segment_type l1, l2;
      double *xint, *yint;
#endif

{
   double m1,m2,b1=0,b2=0;
   double tempy1,tempy2;

   if ( (l1.x1==l1.x2) && (l2.y1==l2.y2) ) {
      /* l1 is vertical and l2 is horizontal */

      if ( ( ((l2.x1 <= l1.x1) && (l1.x1 <= l2.x2)) ||    /* X coordinate */
             ((l2.x2 <= l1.x1) && (l1.x1 <= l2.x1)) ) &&  /*  intersects  */
           ( ((l1.y1 <= l2.y1) && (l2.y1 <= l1.y2)) ||    /* Y coordinate */
             ((l1.y2 <= l2.y1) && (l2.y1 <= l1.y1)) ) )   /*  intersects  */
      {
       *xint = l1.x1;
       *yint = l2.y1;
       return 1;
       } 
     } else
       return 0;

   if ( (l2.x1==l2.x2) && (l1.y1==l1.y2) ) {
      /* l2 is vertical and l1 is horizontal */

      if ( ( ((l1.x1 <= l2.x1) && (l2.x1 <= l1.x2)) ||    /* X coordinate */
             ((l1.x2 <= l2.x1) && (l2.x1 <= l1.x1)) ) &&  /*  intersects  */
           ( ((l2.y1 <= l1.y1) && (l1.y1 <= l2.y2)) ||    /* Y coordinate */
             ((l2.y2 <= l1.y1) && (l1.y1 <= l2.y1)) ) )   /*  intersects  */
      {
       *xint = l2.x1;
       *yint = l1.y1;
       return 1;
       }
     } else
       return 0;

   if ( (l1.x1==l2.x1) && (l1.y1==l2.y1) ) {
      *xint = l1.x1;
      *yint = l1.y1;
      return 1;
   }
   if ( (l1.x2==l2.x2) && (l1.y2==l2.y2) ) {
      *xint = l1.x2;
      *yint = l1.y2;
      return 1;
   }
   if ( (l1.x1==l2.x2) && (l1.y1==l2.y2) ) {
      *xint = l1.x1;
      *yint = l1.y1;
      return 1;
   }
   if ( (l1.x2==l2.x1) && (l1.y2==l2.y1) ) {
      *xint = l1.x2;
      *yint = l1.y2;
      return 1;
   }

   if (l1.x1 != l1.x2) {
      m1 = (l1.y2-l1.y1)/(l1.x2-l1.x1);
      b1 = -1.0*m1*l1.x1 + l1.y1;
   } else {  /* l1 is a vertical line */
      m1 = MAXFLOAT;
   }

   if (l2.x1 != l2.x2) {
      m2 = (l2.y2-l2.y1)/(l2.x2-l2.x1);
      b2 = -1.0*m2*l2.x1 + l2.y1;
   } else {  /* l2 is a vertical line */
      m2 = MAXFLOAT;
   }


   /* Find intersection point of lines */

   if ((m1 == m2) && (m1 != MAXFLOAT)) {
      if (b1 == b2) {
         /* Slopes are equal (and not vertical). */
         /* Check end points for overlap.        */
         if (BETWEEN(l1.x1,l2.x1,l2.x2)) {
            *xint = l1.x1;
            *yint = l1.y1;
            return 1;
         }
         if (BETWEEN(l1.x2,l2.x1,l2.x2)) {
            *xint = l1.x2;
            *yint = l1.y2;
            return 1;
         }
         if (BETWEEN(l2.x1,l1.x1,l1.x2)) {
            *xint = l2.x1;
            *yint = l2.y1;
            return 1;
         }
         if (BETWEEN(l2.x2,l1.x1,l1.x2)) {
            *xint = l2.x2;
            *yint = l2.y2;
            return 1;
         }
      }
      /* Endpoints don't overlap - no intersection */
      return 0;
   }

   if ( (m1 != MAXFLOAT) && (m2 != MAXFLOAT) ) {
   /* neither line is a vertical line */
      *xint = (b2-b1)/(m1-m2);
      if (m1 == 0)            /* if l1 is a horizontal line */
       *yint = l1.y1;
      else
      {
       if (m2 == 0)       /* if l2 is a horizontal line */
   *yint = l2.y1;
       else
   *yint = m1*(*xint)+b1;
      }

   } else {                   /* At least one vertical line */
      if (m1==m2) {           /* Both vertical lines */
    if (l1.x1==l2.x1) {  /* Coincident vertical lines */
       *xint = l1.x1;
            tempy1 = (double) min( l1.y1, l1.y2 );
            tempy2 = (double) min( l2.y1, l2.y2 );
       *yint = (double) max( tempy1, tempy2 );
    } else {             /* Non-coincident vertical lines */
       return 0;
    }
      } else if (m1==MAXFLOAT) {   /* l1 is vertical */
         if ( ((l2.x1 <= l1.x1) && (l2.x2 >= l1.x1)) ||
              ((l2.x2 <= l1.x1) && (l2.x1 >= l1.x1)) )
         {
     *yint = m2*l1.x1 + b2;  /* l2 intersects l1 */
     *xint = l1.x1;
         }
         else
          return 0;       /* l2 does not intersect l1 */

      } else {                     /* l2 is vertical */
         if ( ((l1.x1 <= l2.x1) && (l1.x2 >= l2.x1)) ||
              ((l1.x2 <= l2.x1) && (l1.x1 >= l2.x1)) )
         {
          *yint = m1*l2.x1 + b1;  /* l1 intersects l2 */
     *xint = l2.x1;
         }
         else
          return 0;       /* l1 does not intersect l2 */

      }
   }

   /* See if intersection point lies on both line segments */
   return ( (*xint >= (double)min(l1.x1,l1.x2)) &&
       (*xint <= (double)max(l1.x1,l1.x2)) &&
       (*yint >= (double)min(l1.y1,l1.y2)) &&
       (*yint <= (double)max(l1.y1,l1.y2)) &&
       (*xint >= (double)min(l2.x1,l2.x2)) &&
       (*xint <= (double)max(l2.x1,l2.x2)) &&
       (*yint >= (double)min(l2.y1,l2.y2)) &&
       (*yint <= (double)max(l2.y1,l2.y2)) );

}




/**************************************************************************
 *
 *N  perpendicular_intersection
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Test whether a perpendicular to a line segment intersects a search
 *    point.  lseg must be of a type similar to line_segment_type.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    lseg      <input> == (line_segment_type) line segment.
 *    xsearch   <input> == (double) search point x coordinate.
 *    ysearch   <input> == (double) search point y coordinate.
 *    xint      <inout> == (double *) intersection point x coordinate.
 *    yint      <inout> == (double *) intersection point y coordinate.
 *    return   <output> == (int32) TRUE if a perpendicular to the line
 *                         segment intersects the search point;
 *                         FALSE if there is no intersection.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels, DOS Turbo C   1991
 *    Ronald Rozensky, vertical and horizontal line handlers.
 *    Barry Michaels  Sep 1992  UNIX Port - double precision
 *E
 *************************************************************************/
#ifdef PROTO
   int32 perpendicular_intersection (line_segment_type lseg,
                                    double xsearch, double ysearch,
                                    double *xint, double *yint)
#else
   int32 perpendicular_intersection (lseg, xsearch, ysearch, xint, yint)
      line_segment_type lseg;
      double xsearch, ysearch, *xint, *yint;
#endif

{
   double m1,mp,b1=0,bp; /* m's are for slopes, b's are for y intersections */
             /* for the old y=mx+b point slope formula.         */

   if (lseg.y1 == lseg.y2)
      if (((lseg.x1 <= xsearch) && (xsearch <= lseg.x2)) ||
     ((lseg.x2 <= xsearch) && (xsearch <= lseg.x1)))
    {
    *xint = xsearch;
    *yint = lseg.y1;
    return 1;
    }

   if (lseg.x1 == lseg.x2)

      if (((lseg.y1 <= ysearch) && (ysearch <= lseg.y2)) ||
     ((lseg.y2 <= ysearch) && (ysearch <= lseg.y1)))
    {
    *xint = lseg.x1;
    *yint = ysearch;
    return 1;
    }

   if ( (lseg.x1==xsearch) && (lseg.y1==ysearch) ) {
      *xint = xsearch;
      *yint = ysearch;
      return 1;
   }
   if ( (lseg.x2==xsearch) && (lseg.y2==ysearch) ) {
      *xint = xsearch;
      *yint = ysearch;
      return 1;
   }

   if (lseg.x1 != lseg.x2) {
      m1 = (lseg.y2-lseg.y1)/(lseg.x2-lseg.x1);
      b1 = -1*m1*lseg.x1 + lseg.y1;
      if (m1 != 0.0) {
    mp = -1.0/m1;       /* perpendicular */
      } else {
    mp = MAXFLOAT;
      }
   } else {
      m1 = MAXFLOAT;
      mp = 0.0;
   }

   if (fabs(mp) < 1000000.0) {
      bp = ysearch - mp*xsearch;
   } else {
      bp = MAXFLOAT;
   }

   /* Find intersection point of lseg and its perpendicular */

   if ( (m1 != MAXFLOAT) && (mp != MAXFLOAT) ) {
      *xint = (bp-b1)/(m1-mp);
      *yint = m1*(*xint)+b1;
   } else {                   /* At least one vertical line */
      if (m1==MAXFLOAT) {          /* lseg is vertical */
    *yint = ysearch;
    *xint = lseg.x1;
      } else {                     /* perp is vertical */
    *yint = lseg.y1;
    *xint = xsearch;
      }
   }

   /* See if intersection point lies on both line segments */
   return ( (*xint >= (float)min(lseg.x1,lseg.x2)) &&
       (*xint <= (float)max(lseg.x1,lseg.x2)) &&
       (*yint >= (float)min(lseg.y1,lseg.y2)) &&
       (*yint <= (float)max(lseg.y1,lseg.y2)) );
}


/*************************************************************************
 *
 *N  float_to_dms
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function converts a floating point lat lon coordinate to
 *     degrees-minutes-seconds format.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    coord   <input> == (double) floating point lat lon coordinate.
 *    return <output> == (dms_type) degrees-minutes-seconds coordinate.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1990    Original Version    DOS Turbo C
 *E
 *************************************************************************/
#ifdef PROTO
   dms_type float_to_dms (double coord)
#else
   dms_type float_to_dms (coord)
      double coord;
#endif

{
   dms_type dms_coord;

   dms_coord.degrees = (int32)coord;
   dms_coord.minutes = (int32)((double)(coord-(int32)coord)*60.0);
   dms_coord.seconds = (float)(((double)(coord-(int32)coord)* 60.0) -
         (double)dms_coord.minutes) * 60.0;
   dms_coord.minutes = (int32)abs ((short)dms_coord.minutes);
   dms_coord.seconds = (float)fabs (dms_coord.seconds);

   if (dms_coord.seconds >= 60.0) {
      dms_coord.minutes++;
      dms_coord.seconds -= (float)60.0;
   } 
 
   if (dms_coord.minutes == 60) {
      if (dms_coord.degrees >= 0)
    dms_coord.degrees++;
      else
    dms_coord.degrees--;
      dms_coord.minutes = 0;
   }

   if ((dms_coord.degrees == 0)&&(coord < 0.0)) dms_coord.minutes *= -1;

   return dms_coord;
}


/*************************************************************************
 *
 *N  dms_to_float
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function converts a coordinate in degrees-minutes-seconds 
 *     format to a (double precision) floating point decimal degree value.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    coord   <input> == (dms_type) degrees-minutes-seconds coordinate.
 *    return <output> == (double) floating point lat lon coordinate.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    April 1990    Original Version    DOS Turbo C
 *E
 *************************************************************************/
#ifdef PROTO
   double dms_to_float (dms_type dms_coord)
#else
   double dms_to_float (dms_coord)
      dms_type dms_coord;
#endif

{
   double fcoord;

   if (dms_coord.degrees >= 0)
      fcoord = (double)dms_coord.degrees +
               (((double)dms_coord.minutes)/60.0) +
               (dms_coord.seconds/3600.0);
   else
      fcoord = (double)dms_coord.degrees -
               (((double)dms_coord.minutes)/60.0) -
               (dms_coord.seconds/3600.0);

   return fcoord;
}
