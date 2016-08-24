#ifndef INCL_XVTH
#include <xvt.h>
#endif
#include <math.h>
#if 0
#include <stdio.h>
#include <malloc.h>
#endif
#include "machine.h"
#ifndef THIN_DIG_H
#include "reduce2.h"
#endif

//#define  MAX(m,n) (((m) > (n)) ? (m) : (n))
//#define  MIN(m,n) (((m) < (n)) ? (m) : (n))
#define  DOCNT(i,t,n)   (_d_l=n, (_d_m=(t-(i)+_d_l)/_d_l) > 0 ? _d_m : 0L )

/*    General purpose tools for polyline reduction:
       Reduca -      Closed polygon reduction which preserves small shapes.
       Reduc1 -      Preprocessor to set up reduction parameters.
       Reduc2 -      Douglas Peuker reduction.
       Xcheck -      Post processing check/repair to preserve topology.
       Plnint -      Detects intersection of polylines.


-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*|*-*-*-*-
 UNIT NAME:                REDUCA

 FUNCTIONAL DESCRIPTION:   Customizes Douglas-Peuker polygon reduction by 
                           changing resolution tolerance as required to
                           prevent small areas from degenerating.

 INPUT FILES/PARAMETERS:   in, nin      - polyline of length nin
                           relem        - Resolution element length
                           CHGMAX       - Limit % area change for polygons.
                           merc         - Mercator compensation for world map

 OUTPUT FILES/PARAMETERS:  out, nout    - reduced polyline

 SUBROUTINES CALLED:       Reduc1       - For polyline reduction

 PROGRAMMER:               DATE OF LAST CHANGE      REASON FOR CHANGE
    Tom Wescott NOSC       April 92                 Original entry
    Jim Bellenger SC/WGEA  Dec 92                   Cleaned up the FORTRAN
                                                    conversion

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*|*-*-*- */

#define   CHGMAX   0.2E0

#if XVT_CC_PROTO
void Reduca (COORDS *in, int32 *nin, COORDS *out, int32 *nout, 
             double *relem, int32 *merc)
#else
void Reduca (in, nin, out, nout, relem, merc)
COORDS *in;
int32 *nin;
COORDS *out;
int32 *nout;
double *relem;
int32 *merc;
#endif
   {
   int32 i, j;
   double ain, aout, change, toler;
   double x1, x2, y1, y2;
   void Reduc1();

   toler = *relem;

  /* Calculate area of input polygon. */

   ain = 0.E0;

   if ( (in[0].x == in[*nin - 1].x) && (in[0].y == in[*nin - 1].y) ) 
      {
      for (i=0; i < *nin; i++ )
         {
         j = i + 1;
         if( j == *nin )
            j = 0;
         x1 = in[i].x;
         x2 = in[j].x;
         y1 = in[i].y;
         y2 = in[j].y;
         ain = ain + 0.5E0*(x1*y2 - y1*x2);
         }
      }
   else
      {
     /* Leave ain = 0.0 to fully reduce all unclosed polylines.  */
      }

   /* Do Until (% change under CHGMAX) */

    do
       {

      /* Reduce the polygon. */

       Reduc1( in, nin, out, nout, &toler, merc );

      /* Check for drastic change in area. */

       if (ain == 0.0 )
          change = 0.0;
       else 
          {
         /* Calculate area of reduced polygon. */

          aout = 0.0;
          for (i=0; i < *nout; i++)
             {
             j = i + 1;
             if (j == *nout )
                j = 0;
             x1 = out[i].x;
             x2 = out[j].x;
             y1 = out[i].y;
             y2 = out[j].y;
             aout = aout + 0.5E0*(x1*y2 - y1*x2);
             }
          change = fabs( (aout - ain)/ain );
          }

      /* Decrease the tolerance. if change too small */

       if ( change > CHGMAX )
          toler = toler*.707E0;

       } while (change > CHGMAX); /* End do */

   return;

   } /* end of function */

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*|*-*-*-
 UNIT NAME:                REDUC1

 FUNCTIONAL DESCRIPTION:   Customizes Douglas-Peuker polyline reduction by 
                           breaking int32 input polylines, and defining the
                           coordinate system used.

 INPUT FILES/PARAMETERS:   in, nin    - Polyline of length nin
                           relem          - Resolution element length
                           merc           - Mercator compensation if not 0.
                           DLIMIT         - Prohibits long output segments
                                            from reducing to two points.

 OUTPUT FILES/PARAMETERS:  out, nout - Reduced polyline

 SUBROUTINES CALLED:       Reduc2         - Actual D-P reduction.

 PROGRAMMER:               DATE OF LAST CHANGE      REASON FOR CHANGE
    Tom Wescott NOSC       March 90                 Original entry
    Jim Bellenger SC/WGEA  Dec 92                   Cleaned up the FORTRAN
                                                    conversion

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*|*-*-*-*/
#if XVT_CC_PROTO
void Reduc1 (COORDS *in,  int32 *nin, COORDS *out, int32 *nout, 
             double *relem, int32 *merc)
#else
void Reduc1 (in,  nin, out, nout, relem, merc)
COORDS *in;
int32 *nin;
COORDS *out;
int32 *nout;
double *relem;
int32 *merc;
#endif
   {
   int32 i, iend, j, k, llsph, ng, nlimit;
   double coslat, dlimit, dmax, toler, xmax, xmin, ymax, ymin;
   void Reduc2();

  /* Limit processing group verticies to speed (n-squared) execution. */

   nlimit = 1000;

  /* Limit processing group span to avoid excessively long output segments. */

   dlimit = 30.E0*(*relem);

   j = 0;
   iend = 0;

  /* Break long polylines into groups with ends (i,iend) and length (ng).
     While (input not exhausted) do */

   do
      {

     /* SET UP PROCESSING GROUP */

      toler = *relem;

    /* Set first point in group back to the last input point we want to keep. */

      i = iend;
      ng = 0;
      xmin = in[i].x;
      xmax = in[i].x;
      ymin = in[i].y;
      ymax = in[i].y;
      dmax = 0.0;
      for (k=i; k < *nin; k++)
         {
         xmin = MIN( xmin, in[k].x );
         xmax = MAX( xmax, in[k].x );
         ymin = MIN( ymin, in[k].y );
         ymax = MAX( ymax, in[k].y );
         dmax = MAX( xmax - xmin, ymax - ymin );
         if ( (ng >= 2) && ((dmax > dlimit) || (ng >= nlimit)) ) 
            {
           /* Process group without including this point. */
            break;
            }
         else 
            {
           /* Update last point in group to pt. k */
            ng = ng + 1;
            iend = k;
            }
         }

     /* Do a Douglas-Peuker reduction. */

      if ( *merc != 0 )
         {
        /* SET MERCATOR COMPENSATION:   For low resolution output only...
           If Reduced resolution versions are to be used to draw mercator 
           or similar world maps, modifying toler to decrease with latitude 
           provides uniform display resolution.  Storage penalty is slight.
        */
         coslat = MAX( cos( in[i].y ), 0.1E0 );
         toler = toler*coslat;
        /* Use Plane Geometry for the reduction. */
         llsph = 0;
         }
      else
         {
        /* Use Spherical Geometry for the reduction. */
         llsph = 1;
         }

      Reduc2 (&in[i], &ng, &out[j], nout,
              &toler, &llsph );

     /* Insure first point survives after possible degenerate first group.  */

      if ( j == 0 )
         {
         out[0].x = in[0].x;
         out[0].y = in[0].y;
         }

      j = j + *nout - 1;
      *nout = j;

      } while ( iend < *nin-1 ); /* End while. */

  /* Insure last point survives after possible degenerate final group. */

   if ( (in[*nin - 1].x != out[*nout - 1].x) ||
        (in[*nin - 1].y != out[*nout - 1].y) )
      {
      *nout = *nout + 1;
      out[*nout - 1].x = in[*nin - 1].x;
      out[*nout - 1].y = in[*nin - 1].y;
      }

  /* POST PROCESSING QC:
     Prohibit short line segments from degenerating to single points. */

   if ( (*nout == 1) && ((in[0].x != in[*nin - 1].x) || 
                         (in[0].y != in[*nin - 1].y)) )
      {
      *nout = 2;
      out[*nout - 1].x = in[*nin - 1].x;
      out[*nout - 1].y = in[*nin - 1].y;
      }

   return;
   } /* end of function */

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*|*-*-*-*
 UNIT NAME:                REDUC2

 FUNCTIONAL DESCRIPTION:   DOUGLAS-PEUKER POLYLINE REDUCTION
                           Reduces the number of verticies in a polyline
                           by deleting those producing no effect below a
                           specified resolution or tolerance.

 INPUT FILES/PARAMETERS:   xin,yin,nin  - Polyline of length nin.
                           toler        - Resolution element length, same units
                           lldeg        - Coordinate system to be used.
                                         = 1  Units must be in decimal degrees.
                                              Spherical trig approximated.
                                         <>1  Any units, plane geometry used.

 OUTPUT FILES/PARAMETERS:  xout,yout,nout - reduced polyline

 SUBROUTINES CALLED:       none.

 REFERENCE:                Douglas & Peucker, Canadian Cartographer 1973

 PROGRAMMER:               DATE OF LAST CHANGE      REASON FOR CHANGE
    Tom Wescott NOSC       March 90                 Original entry
    Jim Bellenger SC/WGEA  Dec 92                   Cleaned up the FORTRAN
						    conversion
    Dan Maddux SC/WGEA	   15 Apr 93		    Dynamic allocation

-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*|*-*-*-*/
#define   MXVRT   10000

#if XVT_CC_PROTO
void Reduc2 (COORDS *in,  int32 *nin, COORDS *out, int32 *nout,
             double *toler, int32 *lldeg)
#else
void Reduc2 (in,  nin, out, nout, toler, lldeg)
COORDS *in;
int32 *nin;
COORDS *out;
int32 *nout;
double *toler;
int32 *lldeg;
#endif
   {
   int32 aptr, fptr, maxptr, stkptr, tptr, *stack;
   double coslat, rlat;
   double a, b, cterm, dist2, len2, maxd2, numerator, toler2, xa, xf, 
          xt, ya, yf, yt;
   double pi=4.E0/atan(1.E0);

  /* Allocate stack */
  stack = (int32*)xvt_malloc (MXVRT * sizeof (int32));

  /* Initialize the pointers for the anchor,tester and float pts. */

   *nout = 0;
   aptr = 0;
   fptr = *nin - 1;
   stkptr = 0;
   maxptr = 0;
   toler2 = *toler**toler;
   if ( *lldeg == 1 )
      {
     /* Set an abcissa multiplier to approximate meridian convergence. */
      rlat = in[0].y *pi/180.E0;
      coslat = cos( rlat );
      }
   else
      {
     /* Set a constant multiplier to calculate distances in Plane Geometry. */
      coslat = 1.E0;
      }

  /* While (line not finished) do */

  while (1)
      {

     /* Set the anchor and float points. */

      xa = in[aptr].x *coslat;
      ya = in[aptr].y;
      xf = in[fptr].x *coslat;
      yf = in[fptr].y;

     /* Calculate constant constant values associated with line 
        segment AF  (components, length squared, & cross term.) */

      a = xf - xa;
      b = yf - ya;
      len2 = a*a + b*b;
      cterm = yf*xa - ya*xf;

     /* Identify the intermediate point furthest from line segment AF. */

      maxd2 = 0.E0;
      for (tptr=aptr+1; tptr < fptr-1; tptr++ )
         {
        /* get a test point */
 
         xt = in[tptr].x *coslat;
         yt = in[tptr].y;

        /* Calculate distance squared perpendicular to line segment AF. */

         if ( (a != 0.E0) || (b != 0.E0) )
            {
            numerator = yt*a - xt*b + cterm;
            dist2 = numerator*numerator/len2;
            }
         else 
            {
            dist2 = (xt - xa)*(xt - xa) + (yt - ya)*(yt - ya);
            }

         if ( dist2 > maxd2 ) 
            {
            maxptr = tptr;
            maxd2 = dist2;
            }
         }

      if ( maxd2 > toler2 ) 
         {
        /* The tolerance has been exceeded, push this floater onto the
           stack, and reset float point to the max distance point. */
         stkptr = stkptr + 1;
         if ( stkptr > MXVRT )
            {
	    xvt_note ("Reduc2: stack blew up" );
            }
         stack[stkptr-1] = fptr;
         fptr = maxptr;
         }
      else 
         {
        /* The tolerance has been met.  Output the current anchor point. */

         *nout = *nout + 1;
         out[*nout - 1].x = in[aptr].x;
         out[*nout - 1].y = in[aptr].y;

         if ( stkptr == 0 ) 
            {
           /* Output the final float point and exit. */
            if ( (*nout > 1) || ((pow(in[0].x - in[fptr].x, 2.E0) + 
                                  pow(in[0].y - in[fptr].y, 2.E0)) > toler2) )
               {
               *nout = *nout + 1;
               out[*nout - 1].x = in[fptr].x;
               out[*nout - 1].y = in[fptr].y;
	       }
	    /* Free the stack */
	    if (stack)
	       xvt_free ((char*)stack);
            return;
            }

         else 
            {
           /* Change the current float point into the new anchor point. */
            aptr = fptr;
           /* Pop the stack to get the next floater. */
            fptr = stack[stkptr-1];
            stkptr = stkptr - 1;
            }
        }

    } /* End while */

} /* end of function */

#if 0
/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*|*-*-*-*
 UNIT NAME:                Xcheck

 FUNCTIONAL DESCRIPTION:   Post processing QC/Repair for polyline reduction.
                           Takes precaution against creating a self or mutual 
                           crossing condition in the output polyline(s).
                           Plane geometry is used to detect intersections. !?
                           Repair is done by replacing as many points from
                           original polyline as reqd to eliminate crossings.

 INPUT FILES/PARAMETERS:   xr,yr,nr      - Reduced polyline. 
                           xo,yo,no      - Original unreduced version of xr,yr
                           xc,yc,nc      - Polyline to be checked for crossings.
                                           (may be a copy of xr,yr)

 OUTPUT FILES/PARAMETERS:  xr,yr,nr      - Reduced polyline (crossings removed)
                           ierror        - Reports a failed correction.
                                           (may have had crossing in original)

 SUBROUTINES CALLED:       plnint        - Counts line-polyline intersections.

 PROGRAMMER:               DATE OF LAST CHANGE      REASON FOR CHANGE
    Tom Wescott NOSC       March 92                 Original entry
    Jim Bellenger SC/WGEA  Dec 92                   Cleaned up the FORTRAN
                                                    conversion

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*|*-*-*-*/
#define   BIG   1.0E99

void Xcheck(int32 *idr, 
            double *xr, double *yr, int32 *nr, 
            double *xo, double *yo, int32 *no, 
            int32 *idc, 
            double *xc, double *yc, int32 *nc,
            int32 *ierror)
{

   int32 i, j, k, iend, istep, 
        istr, ivrt, lcross, len, line, loop, 
        lstart, m, maxints, maxlin, n, ncross, ndelet, nints, nreplc;
   int32 _d_l, _d_m, _do4, _do5;
   double tx[MXVRT], ty[MXVRT], x1, x2, xcmax, xcmin, xmax, xmin, xrmax, 
          xrmin, y1, y2, ycmax, ycmin, ymax, ymin, yrmax, yrmin;
   void Plnint();
   char string[256];

   *ierror = 0;
   loop = 0;
   if ( (*nr >= 2) && (*nc > 2) ) 
      {
     /* Use minimax test to screen cases where intersection is impossible. */
      xrmin =  BIG;
      yrmin =  BIG;
      xrmax = -BIG;
      yrmax = -BIG;
      for (i=0; i<*nr; i++ )
         {
         xrmin = MIN( xmin, xr[i] );
         yrmin = MIN( ymin, yr[i] );
         xrmax = MAX( xmax, xr[i] );
         yrmax = MAX( ymax, yr[i] );
         }

      xcmin =  BIG;
      ycmin =  BIG;
      xcmax = -BIG;
      ycmax = -BIG;
      for (i=0; i<*nc; i++)
         {
         xcmin = MIN( xmin, xc[i] );
         ycmin = MIN( ymin, yc[i] );
         xcmax = MAX( xmax, xc[i] );
         ycmax = MAX( ymax, yc[i] );
         }

      if ( (((xrmax > xcmin) && (xrmin < xcmax)) && (yrmax > ycmin)) && 
             (yrmin < ycmax) ) 
         {

         while (1)
            {

           /* Exhaustive check from first line segment begins here. */
            lstart = 1;
            ncross = 0;

            while (1)
               {
              /* Check from next unrepaired segment begins here. */

               loop = loop + 1;

              /* Find the line segment with the most crossings. */

               maxints = 0;
               maxlin = 0;
               lcross = 0;
               for ( line = lstart; line < *nr-1; line++ )
                  {
                  x1 = xr[line];
                  y1 = yr[line];
                  x2 = xr[line + 1];
                  y2 = yr[line + 1];

                  if ( *idr != *idc ) 
                     Plnint( &x1, &y1, &x2, &y2, xc, yc, nc, &nints );
                  else 
                     Plnint( &x1, &y1, &x2, &y2, xr, yr, nr, &nints );

                  if ( nints > 0 ) 
                     lcross = lcross + 1;

                  if ( nints > maxints ) 
                     {
                     maxints = nints;
                     n = line;
                     }
                  }

               if ( maxints > 0 ) 
                  {
                 /* Repair bad line segment with the most crossings. */
                  x1 = xr[n - 1];
                  y1 = yr[n - 1];
                  x2 = xr[n];
                  y2 = yr[n];

                  if ( *idr != *idc )
                     Plnint( &x1, &y1, &x2, &y2, xc, yc, nc, &nints );
                  else
                     Plnint( &x1, &y1, &x2, &y2, xr, yr, nr, &nints );

                  ncross = ncross + nints;
                  if ( nints > 0 ) 
                     {
		     xvt_note ("    SID %d   Reduced line%d  crosses%d  lines in SID %d     loop# %d \n",
                                       *idr, n, nints, *idc, loop );

                    /* Find ends of bad segment in input polyline.   */

                     istr = MXVRT;
                     for ( j=0; j < *no; j++)
                        {
                        if ( istr == MXVRT ) 
                           {
                           if ( (x1 == xo[j]) && (y1 == yo[j]) )
                              istr = j;
                           }
                        else 
                           {
                           if ( (x2 == xo[j]) && (y2 == yo[j]) )
                              iend = j;
                           }
                        }

                     ndelet = iend - istr - 1;
		     xvt_note ("      Repairing segment,str,end = %d %d %d    SID = %d \n",
                              n, istr, iend, *idr );

                    /* Generate a more accurate replacement for the bad segment.
                       Inserting a single central input point in the segment on 
                       each iteration results in near minimum length
                       polylines. */

                     istep = MAX( 1, (iend - istr)/2 );

                     len = 0;
                     for ( ivrt = istr, _do4=DOCNT(istr,iend,_do5=istep); _do4 > 0; ivrt += _do5, _do4-- )
                        {
                        len = len + 1;
                        tx[len - 1] = xo[ivrt];
                        ty[len - 1] = yo[ivrt];
                        }
                     nreplc = len - 2;

                     if ( nreplc > 0 )
                        {
                       /* Push rest of polyline down its buffer and 
                          replace deletes. */

                        for ( j = *nr; j >= n+1; j-- )
                           {
                           xr[j + nreplc] = xr[j];
                           yr[j + nreplc] = yr[j];
                           }

                        for ( k=0; k < nreplc; k++ )
                           {
                           xr[n + k] = tx[1 + k];
                           yr[n + k] = ty[1 + k];
			   xvt_note ("      Replaced input pt %d     (x,y) = %g %g \n",
                                         istr + k*istep, xr[n + k], yr[n + k] );
                           }
                        *nr = *nr + nreplc;
			xvt_note ("      Number of points in,out = %d %d             SID = %d \n",
                                         *no, *nr, *idr );

                       /* Polyline modified.  Repeat crossing check 
                          from beginning. */

                        if ( loop <= (*nr - 1) )
                           break;
                        }

                     else 
                        {
                       /* No modification.  Continue crossing check from 
                          next line. */
                        lstart = n + 1;
                        if ( loop <= (*nr - 1) )
                           continue;
                        }
                     }
                  }

               if ( ncross != 0 ) 
                  {
                  m = m + 1;
                  sprintf (string," **** Error # %d    SID = %d \n", m, *idr );
                  note (string);
                  *ierror = 2;
                  }
               }
            }
         }
      }

   return;
} /* end of function */

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*|*-*-*-*
 UNIT NAME:               PLNINT

 FUNCTIONAL DESCRIPTION:  Counts crossings of a line segment by a polyline.

 INPUT FILES/PARAMETERS:  x1,y1, x2,y2  - Line segment.
                          x(n),y(n),len - Polyline arrays.

 LOCAL VARIABLES:         xi,yi         - Current point in input polyline.
                          xprv,yprv     - Previous point.
                          s1, s2        - Sign of sN tells half plane.
                          dX, dY, D     - Coefficients of test line in
                                          form:    dY*x - dX*y + D = 0
                          ddx,ddy,dd    - Coefs of polyline segment.

 OUTPUT:                  nint          - Number of crossings.

 PROGRAMMER:                   DATE OF LAST CHANGE     REASON FOR CHANGE
     Tom Wescott (NOSC)        Jan 89                  Original code
     Jim Bellenger SC/WGEA     Dec 92                  Cleaned up the FORTRAN
                                                       conversion

-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*|*-*-*- */

void Plnint (double *x1, double *y1, 
             double *x2, double *y2, 
             double *x,  double *y,
             int32 *len, int32 *nints)

{
   int32 inptr;
   double s1, s2, s3, s4;
   double ax, ay, bx, by, d, dd, ddx, ddy, dx, dy, xi, xprv, yi, yprv;

  /* NOTE:  This test requires high precision arithmetic when applied to
     short segmented polylines.   */

   *nints = 0;
   ax = *x1;
   ay = *y1;
   bx = *x2;
   by = *y2;

  /* Compute vector (also half plane) general form coefficients
     from directed line segment  (ax,ay) to (bx,by). */

   dy = ay - by;
   dx = ax - bx;
   d = ax*by - ay*bx;

  /* Initialize counters, and set up loop with previous point. */

   inptr = 0;
   xprv = x[inptr];
   yprv = y[inptr];
   s1 = dy*xprv - dx*yprv + d;

  /* Count intersections of directed segment and polyline.
     While (verts remain) do. */

   while (1)
      {
      inptr = inptr + 1;
      if ( inptr >= *len )
         break;

      xi = x[inptr];
      yi = y[inptr];
      s2 = dy*xi - dx*yi + d;

   /* Perform initial screen to insure polyline segment crosses line segmemt. */
      if ( (s1 <= 0.E0) && (s2 <= 0.E0) ) 
         {
        /* Do nothing. */
         }
      else if( (s1 >= 0.E0) && (s2 >= 0.E0) )
         {
        /*           Do nothing. */
         }
      else 
         {
        /* Secondary screen to insure line segment crosses polyline segment. */

         ddx = xprv - xi;
         ddy = yprv - yi;
         dd = xprv*yi - yprv*xi;
         s3 = ddy*ax - ddx*ay + dd;
         s4 = ddy*bx - ddx*by + dd;
         if ( (s3 >= 0.E0) && (s4 >= 0.E0) )
            {
           /* No cross. */
            }
         else if ( (s3 <= 0.E0) && (s4 <= 0.E0) )
            {
           /* No cross */
            }
         else
            {
            *nints = *nints + 1;
            }
         }

      if ( s2 != 0.E0 )
         s1 = s2;

      xprv = xi;
      yprv = yi;

      } /* End while. */

   return;

} /* end of function */
#endif /* if 0 */
