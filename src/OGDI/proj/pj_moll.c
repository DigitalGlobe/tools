/* Mollweide projection */
#ifndef lint
static const char SCCSID[]="@(#)PJ_moll.c	4.4	93/06/14	GIE	REL";
#endif
#define PJ_LIB__
#include	<projects.h>
#define C1R	.90031631615710606956
#define C2R	1.41421356237309504880
#define EPS	1e-15
#define EPS10	1e-10
#define NITER	10
	static double
theta(double ph) {
	double th, dth;
	int i;

	ph = PI * sin(th = ph);
	for (i = NITER; i ; --i) {
		th += ( dth = (ph - th - sin(th)) / (1. + cos(th)) );
		if (fabs(dth) < EPS)
			break;
	}
	return (.5 * th);
}
FORWARD(s_forward); /* spheroid */
	double th;

	xy.x = C1R * lp.lam * cos(th = theta(lp.phi));
	xy.y = C2R * sin(th);
	return (xy);
}
INVERSE(s_inverse); /* spheroid */
	double th, s;

	if ((s = fabs(th = xy.y / C2R)) < 1.) {
		lp.lam = xy.x / (C1R * cos(th = asin(th)));
		th += th;
		lp.phi = asin((th + sin(th)) / PI);
	} else if ((s - EPS10) > 1.)
		lp.lam = lp.phi = HUGE_VAL;
	else {
		lp.lam = 0.;
		lp.phi = th < 0. ? -HALFPI : HALFPI;
	}
	return (lp);
}
FREEUP; if (P) pj_dalloc(P); }
ENTRY0(pj_moll) P->es = 0.; P->inv = s_inverse; P->fwd = s_forward; ENDENTRY(P)
