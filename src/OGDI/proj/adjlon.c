/* reduce argument to range +/- PI */
#ifndef lint
static const char SCCSID[]="@(#)adjlon.c	4.3	93/06/12	GIE	REL";
#endif
#include <math.h>
/* note: PI adjusted high
** approx. true val:	3.14159265358979323844
*/
#define SPI		3.14159265359
#define TWOPI	6.2831853071795864769
double
adjlon (double lon) {
  double mod,temp;
  temp = lon / TWOPI;
  modf(temp,&mod);
  if (mod > 0.0) {
    lon = lon - ((mod-1.0) * TWOPI);
  } else {
    lon = lon - ((mod+1.0) * TWOPI);
  }
  while ( fabs(lon) > SPI ) {
    lon += lon < 0. ? TWOPI : -TWOPI;
  }
  return( lon );
}
