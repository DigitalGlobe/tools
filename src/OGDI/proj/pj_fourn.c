/*  Fournier Globular I */
#ifndef lint
static const char SCCSID[]="@(#)PJ_fourn.c	4.4	93/06/14	GIE	REL";
#endif
#define PJ_LIB__
#include	<projects.h>
#define EPS	1e-10
#define C	2.46740110027233965467
FORWARD(s_forward); /* spheroid */
	if (fabs(lp.lam) < EPS) {
		xy.x = 0;
		xy.y = lp.phi;
	} else if (fabs(lp.phi) < EPS) {
		xy.x = lp.lam;
		xy.y = 0.;
	} else if (fabs(fabs(lp.lam) - HALFPI) < EPS) {
		xy.x = lp.lam * cos(lp.phi);
		xy.y = HALFPI * sin(lp.phi);
	} else {
		double p, s, at;

		p = fabs(PI * sin(lp.phi));
		s = (C - lp.phi * lp.phi)/(p - 2. * fabs(lp.phi));
		at = lp.lam * lp.lam / C - 1.;
		if ((xy.y = s * s - at * (C - p * s - lp.lam * lp.lam)) < 0.) {
			if (xy.y < -EPS) F_ERROR
			else xy.y = -s / at;
		} else
			xy.y = (sqrt(xy.y) - s) / at;
		if (lp.phi < 0.) xy.y = -xy.y;
		if ((xy.x = 1. - xy.y * xy.y / C) < 0.) {
			if (xy.x < -EPS) F_ERROR
			else xy.x = 0;
		} else
			xy.x = lp.lam * sqrt(xy.x);
	}
	return (xy);
}
FREEUP; if (P) pj_dalloc(P); }
ENTRY0(pj_fourn) P->es = 0.; P->fwd = s_forward; ENDENTRY(P)
