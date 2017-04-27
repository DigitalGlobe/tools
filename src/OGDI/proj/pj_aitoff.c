/*  Aitoff and Winkel Tripel Projections */
#ifndef lint
static const char SCCSID[]="@(#)PJ_aitoff.c	4.5	93/06/13	GIE	REL";
#endif
#define PROJ_PARMS__ \
	double	cosphi1; \
	int		mode;
#define EPS	1e-8
#define PJ_LIB__
#include	<projects.h>
FORWARD(s_forward); /* spheroid */
	double c, d;

	if (d = acos(cos(lp.phi) * cos(0.5 * lp.lam))) { /* basic Aitoff */
		c = sin(lp.phi) / sin(d);
		if ((xy.x = 1. - c * c) < EPS)
			if (xy.x < -EPS) F_ERROR
			else xy.x = 0.;
		else
			xy.x = 2. * d * sqrt(xy.x);
		if (lp.lam < 0.) xy.x = - xy.x;
		xy.y = d * c;
	} else
		xy.x = xy.y = 0.;
	if (P->mode) { /* Winkel Tripel */
		xy.x = (xy.x + lp.lam * P->cosphi1) * 0.5;
		xy.y = (xy.y + lp.phi) * 0.5;
	}
	return (xy);
}
FREEUP; if (P) pj_dalloc(P); }
	static PJ *
setup(PJ *P) {
	P->inv = 0;
	P->fwd = s_forward;
	P->es = 0.;
	return P;
}
ENTRY0(pj_aitoff)
	P->mode = 0;
ENDENTRY(setup(P))
ENTRY0(pj_wintri)
	P->mode = 1;
	if ((P->cosphi1 = cos(pj_param(P->params, "rlat_1").f)) == 0.)
		E_ERROR(-22);
ENDENTRY(setup(P))
