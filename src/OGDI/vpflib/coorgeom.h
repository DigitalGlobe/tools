

#ifndef __COORGEOM_H__
#define __COORGEOM_H__ 1

#include "machine.h"

/* Geographic extent or line segment structure of type double */
typedef struct
   {
   double x1, y1, x2, y2;
} extent_type, line_segment_type;

/* Geographic extent structure of type float  */
typedef struct {
   float x1, y1, x2, y2;
} fextent_type;

typedef enum { 
   Miles, 
   Meters, 
   Degrees, 
   Feet, 
   Inches, 
   Kilometers 
} coord_units_type;

/* For latitude-longitude */
typedef struct {
   int32 degrees;
   int32 minutes;
   float seconds;
} dms_type;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/* Cartesian distance formula. */
/* All parameters must be float or double. */
#define DISTANCE( x1, y1, x2, y2 )  \
        sqrt( ((x2-x1)*(x2-x1)) + ((y2-y1)*(y2-y1)) )

#ifndef _WINDOWS
#ifndef max
#define max(a,b)     ((a > b) ? a : b)
#endif
 
#ifndef min
#define min(a,b)     ((a < b) ? a : b)
#endif
#endif
 
#define MILES_TO_KM(x) (x*1.6093)
#define MILES_TO_METERS(x) (1000.0*x*1.6093)
#define MILES_TO_FEET(x) (x*5280.0);
#define MILES_TO_INCHES(x) (x*5280.0*12.0)

#define KM_TO_MILES(x) (x*0.6214)
#define METERS_TO_MILES(x) (x*0.0006214)
#define FEET_TO_MILES(x) (x/5280.0)
#define INCHES_TO_MILES(x) (x/63360.0)

/* Functions */
#ifdef PROTO
   double gc_distance (double lat1, double lon1, double lat2, double lon2, int32 units);
   int32 contained( extent_type extent1, extent_type extent2 );
   int32 geo_intersect (fextent_type extent1, fextent_type extent2);
   int32 completely_within( extent_type extent1, extent_type extent2 );
   int32 fwithin( double x, double y, extent_type extent );
   int32 intersect( line_segment_type l1, line_segment_type l2, double *xint, double *yint );
   int32 perpendicular_intersection( line_segment_type lseg, double xsearch, double ysearch,
                                                               double *xint, double *yint );
   dms_type float_to_dms( double coordinate );
#else
   double dms_to_float ();
   double gc_distance ();
   int32 contained ();
   int32 geo_intersect ();
   int32 completely_within ();
   int32 fwithin ();
   int32 intersect ();
   int32 perpendicular_intersection ();
   dms_type float_to_dms ();
   double dms_to_float ();
#endif /* If PROTO */

#endif

