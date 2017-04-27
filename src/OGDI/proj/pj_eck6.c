/* Eckert VI projection */
#ifndef lint
static const char SCCSID[]="@(#)PJ_eck6.c	4.4	93/06/13	GIE	REL";
#endif
#define PJ_LIB__
#include	<projects.h>
#define C1R	.44101277172455148219
#define C2R	.88202554344910296438
#define RC2R	1.13375401361911319568
#define C5	2.57079632679489661922
#define RC5	.38898452964834271062
#define EPS	1e-10
#define EPS10	1e-10
#define NITER	10
	static double
theta(double ph) {
	double th, dth;
	int i;

	ph = C5 * sin(th = ph);
	for (i = NITER; i ; --i) {
		th += ( dth = (ph - th - sin(th)) / (1. + cos(th)) );
		if (fabs(dth) < EPS)
			break;
	}
	return th;
}
FORWARD(s_forward); /* spheroid */
	double th;

	xy.x = C1R * lp.lam * (1. + cos(th = theta(lp.phi)));
	xy.y = C2R * th;
	return (xy);
}
INVERSE(s_inverse); /* spheroid */
	double s, t;

	t = (xy.y *= RC2R);
	if ((s = fabs(xy.y = (xy.y + sin(xy.y)) * RC5)) < 1.) {
		lp.lam = xy.x / (C1R * (1. + cos(t)));
		lp.phi = asin(xy.y);
	} else if ((s - EPS10) > 1.) I_ERROR
	else {
		lp.lam = 0.;
		lp.phi = xy.y < 0. ? -HALFPI : HALFPI;
	}
	return (lp);
}
FREEUP; if (P) pj_dalloc(P); }
ENTRY0(pj_eck6) P->es = 0.; P->inv = s_inverse; P->fwd = s_forward; ENDENTRY(P)
