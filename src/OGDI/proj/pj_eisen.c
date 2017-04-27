/*  Eisenlohr Projection */
#ifndef lint
static const char SCCSID[]="@(#)PJ_eisen.c	4.4	93/06/13	GIE	REL";
#endif
#define PJ_LIB__
#include	<projects.h>
#define RSQTWO	0.70710678118654752440
#define FACT	5.82842712474619009760
FORWARD(s_forward); /* spheroid */
	double s1, c1, t, c, v, cp2, cps, cp;

	s1 = sin(lp.lam *= 0.5);
	c1 = cos(lp.lam);
	cp = cos(lp.phi);
	cp2 = cos(lp.phi *= 0.5);
	t = sin(lp.phi)/(cp2 + 2. * (cps = RSQTWO * sqrt(cp)) * c1);
	c = sqrt(2./(1. + t * t));
	v = sqrt((cp2 + cps * (c1 + s1)) / ( cp2 +
		cps * (c1 - s1)));
	xy.x = FACT * ( -2. * log(v) + c * (v - 1./v));
	xy.y = FACT * ( -2. * atan(t) + c * t * (v + 1./v));
	return (xy);
}
FREEUP; if (P) pj_dalloc(P); }
ENTRY0(pj_eisen) P->es = 0.; P->fwd = s_forward; ENDENTRY(P)
