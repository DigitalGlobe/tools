/*  McBryde-Thomas Flat-Polar Sinusoidal */
#ifndef lint
static const char SCCSID[]="@(#)PJ_mbtfps.c	4.4	93/06/14	GIE	REL";
#endif
#define PJ_LIB__
#include	<projects.h>
#define NITER	20
#define EPS	1e-7
#define ONETOL 1.000001
#define C	1.78539816339744830961
#define RC	0.56009915351155737591
#define FYC	0.91659627441275150748
#define RYC	1.09099286994231339007
#define FXC	0.61106418294183433832
#define RXC	1.63648930491347008510
FORWARD(s_forward); /* spheroid */
	double th1, c;
	int i;

	c = C * sin(lp.phi);
	for (i = NITER; i; --i) {
		lp.phi -= th1 = (0.5 * lp.phi + sin(lp.phi) - c )/(0.5 + cos(lp.phi));
		if (fabs(th1) < EPS) break;
	}
	xy.x = FXC * lp.lam * (0.5 + cos(lp.phi));
	xy.y = FYC * lp.phi;
	return (xy);
}
INVERSE(s_inverse); /* spheroid */
	lp.phi = RYC * xy.y;
	lp.lam = RXC * xy.x / (0.5 + cos(lp.phi));
	lp.phi = RC * (0.5 * lp.phi + sin(lp.phi));
	if (fabs(lp.phi) > 1.)
		if (fabs(lp.phi) > ONETOL)	I_ERROR
		else			lp.phi = lp.phi > 0. ? HALFPI : - HALFPI;
	else
		lp.phi = asin(lp.phi);
	return (lp);
}
FREEUP; if (P) pj_dalloc(P); }
ENTRY0(pj_mbtfps) P->es = 0.; P->inv = s_inverse; P->fwd = s_forward;
ENDENTRY(P)
