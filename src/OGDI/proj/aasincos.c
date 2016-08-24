/* arc sin and cosine that will not fail */
#ifndef lint
static const char SCCSID[]="@(#)aasincos.c	4.5	93/06/16	GIE	REL";
#endif
#include <projects.h>
#define ONE_TOL	 1.00000000000001
#define TOL	0.000000001
	double
aasin(double v) {
	double av;

	if ((av = fabs(v)) >= 1.) {
		if (av > ONE_TOL)
			pj_errno = -19;
		return (v < 0. ? -HALFPI : HALFPI);
	}
	return asin(v);
}
	double
aacos(double v) {
	double av;

	if ((av = fabs(v)) >= 1.) {
		if (av > ONE_TOL)
			pj_errno = -19;
		return (v < 0. ? PI : 0.);
	}
	return acos(v);
}
	double
asqrt(double v) { return ((v <= 0) ? 0. : sqrt(v)); }
