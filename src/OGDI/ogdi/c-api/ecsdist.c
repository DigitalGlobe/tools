/******************************************************************************
 *
 * Component: OGDI Core C API
 * Purpose: Functions for computing distances, centroids, and point-in-polygon.
 * 
 ******************************************************************************
 * Copyright (C) 1995 Logiciels et Applications Scientifiques (L.A.S.) Inc
 * Permission to use, copy, modify and distribute this software and
 * its documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies, that
 * both the copyright notice and this permission notice appear in
 * supporting documentation, and that the name of L.A.S. Inc not be used 
 * in advertising or publicity pertaining to distribution of the software 
 * without specific, written prior permission. L.A.S. Inc. makes no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 ******************************************************************************
 *
 * $Log: ecsdist.c,v $
 * Revision 1.5  2001/04/12 19:25:27  warmerda
 * added RGB<->Pixel functions
 *
 * Revision 1.4  2001/04/09 15:04:34  warmerda
 * applied new source headers
 *
 */

#include <stdio.h>
#include <math.h>
#include "ecs.h"

ECS_CVSID("$Id: ecsdist.c,v 1.5 2001/04/12 19:25:27 warmerda Exp $");

double currenttolerance = 0.0;

/*
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

   ecs_DistanceSegment: Calculate the distance between a point (posx,posy)
   and a line segment (xl,yl), (xu,yu).

   IN
          xl,y1 : First point of segment
	  xu,yu : Second point of segment
	  posx,posy : Point position

   OUT
          return double : Calculated distance.
     
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
   */

double ecs_DistanceSegment(xl,yl,xu,yu,posx,posy)
     double xl;
     double yl;
     double xu;
     double yu;
     double posx;
     double posy;
{
  double angle1;
  double angle2;
  double firstat;
  double secondat;
  double thirdat;
  double quad1,quad2;
  double distance;

  if ((xu-xl)==0.0) {
    if (yu>yl) 
      firstat = 1.5707963;
    else
      firstat = -1.5707963;
  } else {
    firstat = atan((yu-yl)/(xu-xl));
    if (xu<xl)
      firstat += 3.141592654;
  }

  if ((posx-xl)==0.0) {
    if (posy>yl) 
      secondat = 1.5707963;
    else
      secondat = -1.5707963;
  } else {
    secondat = atan((posy-yl)/(posx-xl));
    if (posx<xl)
      secondat += 3.141592654;
  }

  if ((posx-xu)==0.0) {
    if (posy>yu) 
      thirdat = 1.5707963;
    else
      thirdat = -1.5707963;
  } else {
    thirdat = atan((posy-yu)/(posx-xu));
    if (posx<xu)
      thirdat += 3.141592654;
  }

  /*
    Calculate the quadrant of each angle. If they are
    the same, the coordinate is not valid
    */

  angle1 = firstat - secondat;
  angle2 = firstat - thirdat;

  quad1 = 1;
  if ((angle1 > 1.5707963) || (angle1 < -1.5707963))
    quad1 = 2;
  quad2 = 1;
  if ((angle2 > 1.5707963) || (angle2 < -1.5707963))
    quad2 = 2;

  /* Check if the distance to the segment is a
     distance to point1, a distance to point 2 or
     the distance between the coordinate and
     the segment itself */

  if (quad1 == 2 && quad2 == 2) {
    /* Calculate the distance to point (xl,yl) */
    distance = sqrt(((posx-xl)*(posx-xl))+((posy-yl)*(posy-yl)));
  } else if (quad1 == 1 && quad2 == 1) {
    /* Calculate the distance to point (xu,yu) */
    distance = sqrt(((posx-xu)*(posx-xu))+((posy-yu)*(posy-yu)));
  } else {
    /* Calculate the distance between the segment (xl,yl),(xu,yu) and
       the point (posx,posy) */
       
    distance = sin(angle1)*sqrt(((posx-xl)*(posx-xl))+((posy-yl)*(posy-yl)));
    if (distance<0.0) 
      distance = -distance;
  }
  return distance;
}


/*
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

   ecs_DistanceObject: Calculate the distance between a point (posx,posy)
   and a ecs_Object

   IN
          ecs_Object *obj: Geographic object
	  posx,posy : Point position

   OUT
          return double : Calculated distance. If an error occur, the
	  function return a negative value.
     
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
   */


double ecs_DistanceObject(obj,X,Y)
     ecs_Object *obj;
     double X;
     double Y;
{
  int i,j;
  double d1,d2;

  if (obj==NULL)
    return -1.0;
  
  switch(obj->geom.family) {
  case Area:
    d2 = HUGE_VAL;

    for(i=0;(int) i<(int) (obj->geom.ecs_Geometry_u.area.ring.ring_len); i++) {
      for(j=0;(int) j<(int) (obj->geom.ecs_Geometry_u.area.ring.ring_val[i].c.c_len - 1); j++) {
	d1 = ecs_DistanceSegment(obj->geom.ecs_Geometry_u.area.ring.ring_val[i].c.c_val[j].x,
				 obj->geom.ecs_Geometry_u.area.ring.ring_val[i].c.c_val[j].y,
				 obj->geom.ecs_Geometry_u.area.ring.ring_val[i].c.c_val[j+1].x,
				 obj->geom.ecs_Geometry_u.area.ring.ring_val[i].c.c_val[j+1].y,
				 X,Y);
	if (d1<d2) d2 = d1;
      }
    }

    if (ecs_IsPointInPolygon(obj->geom.ecs_Geometry_u.area.ring.ring_val[0].c.c_len,
			     obj->geom.ecs_Geometry_u.area.ring.ring_val[0].c.c_val,X,Y) == TRUE) {
      if (obj->geom.ecs_Geometry_u.area.ring.ring_len > 1) {
	for(i=0;(int) i<(int) (obj->geom.ecs_Geometry_u.area.ring.ring_len); i++) {
	  if (ecs_IsPointInPolygon(obj->geom.ecs_Geometry_u.area.ring.ring_val[i].c.c_len,
				   obj->geom.ecs_Geometry_u.area.ring.ring_val[i].c.c_val,X,Y) == TRUE) {
	    return d2;
	  }
	}
      }

      return (d2/2.0);
    }

    return d2;
    break;
  case Line:
    d2 = HUGE_VAL;
    for(i=0;(int) i<(int) (obj->geom.ecs_Geometry_u.line.c.c_len - 1);i++) {
      d1 = ecs_DistanceSegment(obj->geom.ecs_Geometry_u.line.c.c_val[i].x,
			       obj->geom.ecs_Geometry_u.line.c.c_val[i].y,
			       obj->geom.ecs_Geometry_u.line.c.c_val[i+1].x,
			       obj->geom.ecs_Geometry_u.line.c.c_val[i+1].y,
			       X,Y);
      if (d1<d2) d2 = d1;
    }
    return d2;
    break;
  case Point:
    return sqrt((X-obj->geom.ecs_Geometry_u.point.c.x)*(X-obj->geom.ecs_Geometry_u.point.c.x)+
		(Y-obj->geom.ecs_Geometry_u.point.c.y)*(Y-obj->geom.ecs_Geometry_u.point.c.y));
    break;
  case Text:
    return sqrt((X-obj->geom.ecs_Geometry_u.text.c.x)*(X-obj->geom.ecs_Geometry_u.text.c.x)+
		(Y-obj->geom.ecs_Geometry_u.text.c.y)*(Y-obj->geom.ecs_Geometry_u.text.c.y));
    break;
  case Matrix:
    return -1;
    break;
  case Image:
    return -1;
    break;
  default:
    return -1;
    break;
  }
  
  return -1;
}


/*
  ----------------------------------------------------------------------
  
   FUNCTION_INFORMATION
  
   NAME
      ecs_DistanceObjectWithTolerance
      
   DESCRIPTION
      Calculate the distance between an object and a point with a
      tolerance factor. The tolerance factor was previously calculated
      in ecs_SetTolerance
   END_DESCRIPTION
  
   PARAMETERS
       ecs_Object *obj: The geographic object
       double *X: Pointer to X, the x coordinate of the geographic point to convert
       double *Y: Pointer to Y, the y coordinate of the geographic point to convert
   END_PARAMETERS
  
   RETURN_VALUE
      double: The result distance
  
   END_FUNCTION_INFORMATION
    
  ----------------------------------------------------------------------
 */


double ecs_DistanceObjectWithTolerance(obj,X,Y)
     ecs_Object *obj;
     double X;
     double Y;
{
  double res;

  res = ecs_DistanceObject(obj,X,Y);

  if (res > currenttolerance && obj->geom.family != Area) {
    res = HUGE_VAL;
  }

  return res;
}


/*
  ----------------------------------------------------------------------
  
   FUNCTION_INFORMATION
  
   NAME
      ecs_SetTolerance
      
   DESCRIPTION
      Calculate the tolerance of a given region.
   END_DESCRIPTION
  
   PARAMETERS
       ecs_Region *reg: The geographic region
   END_PARAMETERS
  
   RETURN_VALUE
      double: The result tolerance
  
   END_FUNCTION_INFORMATION
    
  ----------------------------------------------------------------------
 */


double ecs_SetTolerance(reg)
     ecs_Region *reg;
{
  currenttolerance = ((reg->north - reg->south)*ECSTOLERANCE);
  return currenttolerance;
}

/*
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

   ecs_DistanceMBR: Calculate the distance between a point (posx,posy)
   and a MBR

   IN
          xl,y1 : First corner of MBR
	  xu,yu : Second corner of MBR
	  posx,posy : Point position

   OUT
          return double : Calculated distance.
     
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
   */

double ecs_DistanceMBR(xl,yl,xu,yu,posx,posy)
     double xl;
     double yl;
     double xu;
     double yu;
     double posx;
     double posy;
{
  double d1,d2;
  
  d2 = HUGE_VAL;

  if ((posx > xl) && (posx < xu) && (posy > yl) && (posy < yu))
    return 0.0;

  d1 = ecs_DistanceSegment(xl,yl,xl,yu,posx,posy);
  if (d1<d2) d2 = d1;

  d1 = ecs_DistanceSegment(xl,yu,xu,yu,posx,posy);
  if (d1<d2) d2 = d1;

  d1 = ecs_DistanceSegment(xu,yu,xu,yl,posx,posy);
  if (d1<d2) d2 = d1;

  d1 = ecs_DistanceSegment(xu,yl,xl,yl,posx,posy);
  if (d1<d2) d2 = d1;

  return d2;
}

/* ==================================================================
   ==================================================================
   ==================================================================
 */
/*
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

   ecs_CalculateCentroid: Calculate the centroid of a polygon

   IN
          nb_segment : the number of segments 
	  coord : an array of ecs_Coordinates
	  centroid : the returned value.

   OUT
          return TRUE
     
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
   */


int ecs_CalculateCentroid(nb_segment,coord,centroid)
     int nb_segment;
     ecs_Coordinate *coord;
     ecs_Coordinate *centroid;
{
  int    i, j;
  double    *intersect;
  int    compar();
  double m,b;
  double xcent,minx,miny,maxx,maxy;  

  /* figure out the max, min, then centre of line */

  minx = maxx = coord[0].x;
  miny = maxy = coord[0].y;


  for(i = 1; i < nb_segment; ++i) {
    if (coord[i].x < minx) { minx= coord[i].x; }
    if (coord[i].y < miny) { miny= coord[i].y; }
    if (coord[i].x > maxx) { maxx= coord[i].x; }
    if (coord[i].y > maxy) { maxy= coord[i].y; }
    /*    minx = min( minx, coord[i].x );
	  miny = min( miny, coord[i].y );*/
    /*    maxx = max( maxx, coord[i].x );
	  maxy = max( maxy, coord[i].y );*/
  }

  xcent = (minx + maxx) / 2.0;

  intersect = (double *) malloc(sizeof(double)*(nb_segment+1));
  if (intersect == NULL) {
    centroid->x = 0.0;
    centroid->y = 0.0; 
    return TRUE;    
  }    

  /* rechercher tous les segments dont les coord en x sont tel
     que x1 < xcent < x2 ou x1 > xcent > x2
     */
   
  for (i = 0,j= 0; i < nb_segment - 1; ++i) {
    if(((double) coord[i].x < xcent && 
	(double) coord[i+1].x >= xcent) ||
       ((double) coord[i].x > xcent &&
	(double) coord[i+1].x <= xcent)) {

      m = (double) (coord[i+1].y - coord[i].y) /
	(double) (coord[i+1].x - coord[i].x);
      b = (double) coord[i].y - m * (double) coord[i].x;
      intersect[j++] = (double)(b + m * xcent);

    }
  }

  /* Last segment */

  if(((double) coord[nb_segment-1].x < xcent && 
      (double) coord[0].x >= xcent) ||
     ((double) coord[nb_segment-1].x > xcent &&
      (double) coord[0].x <= xcent)) {
    
    m = (double) (coord[nb_segment-1].y - coord[0].y) /
      (double) (coord[nb_segment-1].x - coord[0].x);
    b = (double) coord[0].y - m * (double) coord[0].x;
    intersect[j++] = (double)(b + m * xcent);
    
  }


  /* tri les intersections en y */
 
  qsort(intersect,j,sizeof(double),(*compar)); 

  centroid->x = xcent;
  centroid->y = (intersect[0] + intersect[1]) / 2.0; 

  free(intersect);
  return TRUE;
}

int compar(s1,s2)
     double *s1,*s2;
{
  if (*s1 > *s2)
    return 1;
  else if (*s1 == *s2)
    return 0;
  return -1;
} 


/*
  ----------------------------------------------------------------------
 
  cln_IsPointInPolygon
  
  Check if a point is in a polygon.
  
  PARAMETERS
  INPUT
 	 int npoints: Indicate the point quantity in poly
	 ecs_Coordinate *poly: Polygon
	 double x: X coordinate of the point to check
	 double y: Y coordinate of the point to check
	 
  RETURN_VALUE
         int : The boolean indication if the point is outside or inside the polygon
  
  ----------------------------------------------------------------------
  */

int ecs_IsPointInPolygon(npoints,poly,x,y)
     int npoints;
     ecs_Coordinate *poly;
     double x;
     double y;
{
  int inside = 0;
  double xnew,ynew;
  double xold,yold;
  double x1,y1;
  double x2,y2;
  int i;
  
  if (npoints < 3) 
    return FALSE;

  xold = poly[npoints-1].x;
  yold = poly[npoints-1].y;
  for(i=0;i<npoints;i++) {
    xnew = poly[i].x;
    ynew = poly[i].y;
    if (xnew > xold) {
      x1 = xold;
      y1 = yold;
      x2 = xnew;
      y2 = ynew;
    } else {
      x2 = xold;
      y2 = yold;
      x1 = xnew;
      y1 = ynew;
    }
    if ((xnew < x) == (x <= xold) &&
	((y-y1)*(x2-x1) < (y2-y1)*(x-x1)))
      inside = !inside;
    xold = xnew;
    yold = ynew;
  }
  
  if (inside == 1) return TRUE;

  return FALSE;
}

/*
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

   ecs_GetRGBFromPixel: Fetch RGB/t components from 32bit pixel value.

   IN
     unsigned int pixel: the source 32bit value.
     unsigned char *t:   location to set with transparency flag (1=opaque)
     unsigned char *r:   location to set with red component.
     unsigned char *g:   location to set with green component.
     unsigned char *b:   location to set with blue component.

   OUT
     
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
   */

void ecs_GetRGBFromPixel( unsigned int pixel, 
                          unsigned char *transparent, 
                          unsigned char *r, 
                          unsigned char *g, 
                          unsigned char *b )

{
    unsigned char	*byte_pixel = (unsigned char *) &pixel;

    if( transparent != NULL )
        *transparent = byte_pixel[3];
    *r = byte_pixel[0];
    *g = byte_pixel[1];
    *b = byte_pixel[2];
}

/*
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

   ecs_GetPixelFromRGB: Fetch 32bit pixel value from RGB components.

   IN
     int t:   transparency flag (1=opaque,0=transparent)
     int r:   red component (0-255).
     int g:   green component (0-255).
     int b:   blue component (0-255).

   OUT
     32bit pixel value.
     
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
   */

unsigned int ecs_GetPixelFromRGB( int t, int r, int g, int b )

{
    unsigned int        pixel;
    unsigned char	*byte_pixel = (unsigned char *) &pixel;

    byte_pixel[0] = r;
    byte_pixel[1] = g;
    byte_pixel[2] = b;
    byte_pixel[3] = t;

    return pixel;
}

                          


