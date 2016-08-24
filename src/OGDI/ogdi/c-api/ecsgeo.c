/******************************************************************************
 *
 * Component: OGDI Core C API
 * Purpose: Computing distances and areas related to feature objects.
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
 * $Log: ecsgeo.c,v $
 * Revision 1.3  2001/04/09 15:04:34  warmerda
 * applied new source headers
 *
 */

#include "ecs.h"

ECS_CVSID("$Id: ecsgeo.c,v 1.3 2001/04/09 15:04:34 warmerda Exp $");

static double ecs_QA, ecs_QB, ecs_QC;
static double ecs_QbarA, ecs_QbarB, ecs_QbarC, ecs_QbarD;
static double ecs_AE;  /* a^2(1-e^2) */
static double ecs_Qp;  /* Q at the north pole */
static double ecs_E;   /* area of the earth */
static double ecs_TwoPI;

/**************************************************************************/

void ecs_begin_ellipsoid_polygon_area (a, e2)
     double a, e2;
{
  double e4, e6;

  /* Put default values if a and e2 are 0 */

  if ((a==0.0) && (e2==0.0)) {
    a = 6378206.4000000004;
    e2 = 0.0067686580;
  }
  
  ecs_TwoPI = PI+PI;
  
  e4 = e2 * e2;
  e6 = e4 * e2;
  
  ecs_AE = a * a * (1 - e2);
  
  ecs_QA = (2.0/3.0)*e2;
  ecs_QB = (3.0/5.0)*e4;
  ecs_QC = (4.0/7.0)*e6;
  
  ecs_QbarA = -1.0 - (2.0/3.0)*e2 - (3.0/5.0)*e4  -  (4.0/7.0)*e6;
  ecs_QbarB =        (2.0/9.0)*e2 + (2.0/5.0)*e4  +  (4.0/7.0)*e6;
  ecs_QbarC =                     - (3.0/25.0)*e4 - (12.0/35.0)*e6;
  ecs_QbarD =                                        (4.0/49.0)*e6;
  
  ecs_Qp = ecs_Q(PI/2);
  ecs_E  = 4 * PI * ecs_Qp * ecs_AE;
  if (ecs_E < 0.0) ecs_E = -ecs_E;
}


/**************************************************************************/

double ecs_Q(x)
     double x;
{
  double sinx, sinx2;
  
  sinx = sin(x);
  sinx2 = sinx * sinx;
  
  return sinx * (1 + sinx2 * (ecs_QA + sinx2 * (ecs_QB + sinx2 * ecs_QC)));
}

/**************************************************************************/

double ecs_Qbar(x)
     double x;
{
  double cosx, cosx2;
  
  cosx = cos(x);
  cosx2 = cosx * cosx;
  
  return cosx * (ecs_QbarA + cosx2 * (ecs_QbarB + cosx2 * (ecs_QbarC + cosx2 * ecs_QbarD)));
}

/**************************************************************************/

double ecs_planimetric_polygon_area(n,coord)
     int n;
     ecs_Coordinate *coord;
{
  double x1,y1,x2,y2;
  double area;
  int pos;
  
  x2 = coord[n-1].x;
  y2 = coord[n-1].y;
  
  pos = 0;
  area = 0;
  while (--n >= 0) {
    x1 = x2;
    y1 = y2;
    
    x2 = coord[pos].x;
    y2 = coord[pos].y;
    pos++;

    area += (y2+y1)*(x2-x1);
  }
  
  if((area /= 2.0) < 0.0)
    area = -area;
  
  return area;
}

/**************************************************************************/

double ecs_ellipsoid_polygon_area (n, coord)
     int n;
     ecs_Coordinate *coord;
{
  double x1,y1,x2,y2,dx,dy;
  double Qbar1, Qbar2;
  double area;
  int pos;

  x2 = coord[n-1].x * DEG_TO_RAD;
  y2 = coord[n-1].y * DEG_TO_RAD;
  Qbar2 = ecs_Qbar(y2);
  
  area = 0.0;
  pos = 0;
  while (--n >= 0) {
    x1 = x2;
    y1 = y2;
    Qbar1 = Qbar2;
    
    x2 = coord[pos].x * DEG_TO_RAD;
    y2 = coord[pos].y * DEG_TO_RAD;
    pos++;
    Qbar2 = ecs_Qbar(y2);
    
    if (x1 > x2)
      while (x1 - x2 > PI)
	x2 += ecs_TwoPI;
    else if (x2 > x1)
      while (x2 - x1 > PI)
	x1 += ecs_TwoPI;
    
    dx = x2 - x1;
    area += dx * (ecs_Qp - ecs_Q(y2));
    
    if ((dy = y2 - y1) != 0.0)
      area += dx * ecs_Q(y2) - (dx/dy)*(Qbar2-Qbar1);
  }
  if((area *= ecs_AE) < 0.0)
    area = -area;

  /* kludge - if polygon circles the south pole the area will be
   * computed as if it cirlced the north pole. The correction is
   * the difference between total surface area of the earth and
   * the "north pole" area.
   */
  if (area > ecs_E) 
    area = ecs_E;
  if (area > ecs_E/2) 
    area = ecs_E - area;
  
  return area;
}

/**************************************************************************/

/* 
   ------------------------------------------------------------
   
   ecs_geodesic_distance --
   
   Cette fonction calcule la distance en metres de deux
   points en projection longitude/lattitude. 
   
   ------------------------------------------------------------
   */

double
ecs_geodesic_distance (lon1, lat1, lon2, lat2)
     double lon1, lat1, lon2, lat2;
{
  static double boa = 0.99660992469;
  static double f = 0.003390075305;
  static double ff64 = 0.0000001795720402425;
  static double al = 6378206.4;
  double result,newresult,pente,bo,nlat1,nlat2;

  double a, cd, cdtm, ctm, d, dl, dtm, e,kk, kl, l;
  double sd, sdlmr, sdtm, stm, t, t1r, t2r, tm, u, v, x, y;
  
  while (lon1 > 180.0)
    lon1 -= 360.0;
  while (lon1 < -180.0)
    lon1 += 360.0;
  while (lon2 > 180.0)
    lon2 -= 360.0;
  while (lon2 < -180.0)
    lon2 += 360.0;
  if (lon1 > lon2) {
    t = lon1;
    lon1 = lon2;
    lon2 = t;
    
    t = lat1;
    lat1 = lat2;
    lat2 = t;
  }

  /* Calculer la pente et l'origine de la droite passant
     a travers les deux points. */

  
  if (fmod((lon2-lon1),180.0) == 0.0) {
    lon1 += 0.01;
  }
    
  pente = (lat2-lat1)/(lon2-lon1);
  bo = lat1-pente*lon1;
  
  lat1=lat1*DEG_TO_RAD;
  lon1=lon1*DEG_TO_RAD;
  lat2=lat2*DEG_TO_RAD;
  lon2=lon2*DEG_TO_RAD;
  
  t1r=atan(boa*tan(lat1));
  t2r=atan(boa*tan(lat2));
  
  tm  = (t1r+t2r)/2.0;
  dtm = (t2r-t1r)/2.0;
  
  stm = sin(tm);
  ctm = cos(tm);
  sdtm = sin(dtm);
  cdtm = cos(dtm);
  
  kl = stm*cdtm;
  kk = sdtm*ctm;
  
  sdlmr=sin((lon2-lon1)/2.0);
  l=sdtm*sdtm+sdlmr*sdlmr*(cdtm*cdtm-stm*stm);

  /* Si l = 0 ou l = 1, l'algorithme va necessairement donner
     un resultat infini. */
  if (l==1.0)
    l -= 0.01;
  if (l==0.0)
    l+=0.01;
  cd=1.0-2.0*l;
  dl=acos(cd);
  sd=sin(dl);
  t=dl/sd;
  
  u=2.0*kl*kl/(1.0-l);
  v=2.0*kk*kk/l;
  d=4.0*t*t;
  x=u+v;
  e=-2.0*cd;
  y=u-v;
  a=-d*e;
  
  result = (al*sd*(t-f/4.0*(t*x-y)+ff64*(x*(a+(t-(a+e)/2.0)*x)+y*(-2.0*d+e*y)+d*x*y)));

  /* Compensation si angle obtu */

  if ((lon2-lon1) > PI) {
    nlat1=-90.0*pente+bo;
    nlat2=90.0*pente+bo;
    newresult = ecs_geodesic_distance(-90.0,nlat1,90.0,nlat2);    
    result = 2*newresult-result;
  }

  return result;
}

/* 
   ------------------------------------------------------------
   
   ecs_distance_meters --
   
   Cette fonction calcule la distance en metres de deux
   points dans la projection actuelle de l'affichage.
   Retourne un negatif si c'est un cas d'erreur.
   
   ------------------------------------------------------------
   */

double
ecs_distance_meters (projection,X1, Y1, X2, Y2)
     char *projection;
     double X1,Y1,X2,Y2;
{
  PJ *proj;
  char **argv;
  int argc;
  double lon1,lat1,lon2,lat2;
  double result;
  projUV data;
  
  if (ecs_SplitList(projection,&argc,&argv)==FALSE) {
    return -1;
  }
  
  if (strncmp(argv[0],PROJ_UNKNOWN,7) == 0) {
    free(argv);
    return -1;
  }

  if (strncmp(argv[0],PROJ_LONGLAT,13) != 0) {
    if ((proj = pj_init(argc,argv)) == NULL) {
      free(argv);
      return -1;
    }
    
    data.u = X1;
    data.v = Y1;
    data = pj_inv(data,proj);
    if ((data.u == HUGE_VAL) || (data.v == HUGE_VAL)) {
      pj_free(proj);
      free(argv);
      return -1;
    }
    lon1 = data.u*RAD_TO_DEG;
    lat1 = data.v*RAD_TO_DEG;
    
    data.u = X2;
    data.v = Y2;
    data = pj_inv(data,proj);
    if ((data.u == HUGE_VAL) || (data.v == HUGE_VAL)) {
      pj_free(proj);
      free(argv);
      return -1;
    }
    lon2 = data.u*RAD_TO_DEG;
    lat2 = data.v*RAD_TO_DEG;
    
    pj_free(proj);
  } else {
    lon1 = X1;
    lon2 = X2;
    lat1 = Y1;
    lat2 = Y2;
  }
  
  free(argv);
  
  result = ecs_geodesic_distance(lon1,lat1,lon2,lat2);

  return result;
}



