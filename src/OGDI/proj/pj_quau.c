/*  Quartic Authalic */
#ifndef lint
static const char SCCSID[]="@(#)PJ_quau.c	4.4	93/06/14	GIE	REL";
#endif
#define PJ_LIB__
# include	<projects.h>
#define ONEEPS	1.0000001
FORWARD(s_forward); /* spheroid */
	xy.x = lp.lam * cos(lp.phi);
	xy.x /= cos(lp.phi *= 0.5);
	xy.y = 2. * sin(lp.phi);
	return (xy);
}
INVERSE(s_inverse); /* spheroid */
	if (fabs(xy.y *= .5) >= 1.)
		if (fabs(xy.y) > ONEEPS)	I_ERROR
		else		lp.phi = xy.y < 0. ? PI : -PI;
	else
		lp.phi = 2. * asin(xy.y);
	if ((lp.lam = cos(lp.phi)) == 0.)
		lp.lam = 0.;
	else
		lp.lam = xy.x * cos(.5 * lp.phi) / lp.lam;
	return (lp);
}
FREEUP; if (P) pj_dalloc(P); }
ENTRY0(pj_quau) P->es = 0.; P->inv = s_inverse; P->fwd = s_forward; ENDENTRY(P)
