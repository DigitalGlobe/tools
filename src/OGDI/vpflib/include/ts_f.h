#ifndef H_TS_F

#define H_TS_F

#ifndef H_MUSE1
#include "muse1.h"
#endif

#if XVT_CC_PROTO
short           find_zone(char[]);
void            rc_calc(double *, double *, short *, short *, double *, double *);
void 
latlon_calc(short *, short *, double *, double *,
	    double *, double *);
void            eq2pol(double *, double *, double *, double *, short *);
void            pol2eq(double *, double *, double *, double *);
short           calczone(double, int);
ERRSTATUS       cac_zone_compute(double latitude, int32 *zone);
void 
ts(double *p, double *l, int32 dir,
   short ts_scale, short ts_zone,
   double *y, double *x, int32 *ierr);

ERRSTATUS       ts_config(MGM * mgm);

#else

/* CAC reading function prototypes */
short           find_zone();
void            rc_calc();
void            latlon_calc();
void            eq2pol();
void            pol2eq();
short           calczone();
ERRSTATUS       cac_zone_compute();
void            ts();
ERRSTATUS       ts_config();
#endif

#endif
