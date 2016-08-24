/* Sinusoidal Projection */
#ifndef lint
static const char SCCSID[]="@(#)PJ_sinu.c	4.4	93/06/13	GIE	REL";
#endif
#define PROJ_PARMS__ \
	double	*en;
#define PJ_LIB__
#include	<projects.h>
#define EPS10	1e-10
FORWARD(e_forward); /* ellipsoid */
	double s, c;

	xy.y = pj_mlfn(lp.phi, s = sin(lp.phi), c = cos(lp.phi), P->en);
	xy.x = lp.lam * c / sqrt(1. - P->es * s * s);
	return (xy);
}
FORWARD(s_forward); /* sphere */
	xy.x = lp.lam * cos(xy.y = lp.phi);
	return (xy);
}
INVERSE(e_inverse); /* ellipsoid */
	double s;

	if ((s = fabs(lp.phi = pj_inv_mlfn(xy.y, P->es, P->en))) < HALFPI) {
		s = sin(lp.phi);
		lp.lam = xy.x * sqrt(1. - P->es * s * s) / cos(lp.phi);
	} else if ((s - EPS10) < HALFPI)
		lp.lam = 0.;
	else I_ERROR;
	return (lp);
}
INVERSE(s_inverse); /* sphere */
	double s;

	if ((s = fabs(lp.phi = xy.y)) < HALFPI)
		lp.lam = xy.x / cos(lp.phi);
	else if ((s - EPS10) < HALFPI)
		lp.lam = 0.;
	else I_ERROR;
	return (lp);
}
SPECIAL(spc) {
	if (!P->es) {
		fac->der.y_l = 0.;
		fac->der.y_p = 1.;
		fac->der.x_l = cos(lp.phi);
		fac->der.x_p = - lp.lam * sin(lp.phi);
		fac->code = IS_ANAL_XL_YL + IS_ANAL_XP_YP;
	}
}
FREEUP; if (P) { if (P->en) pj_dalloc(P->en); pj_dalloc(P); } }
ENTRY0(pj_sinu)
	if (!(P->en = pj_enfn(P->es)))
		E_ERROR_0;
	if (P->es) {
		P->en = pj_enfn(P->es);
		P->inv = e_inverse;
		P->fwd = e_forward;
	} else {
		P->inv = s_inverse;
		P->fwd = s_forward;
		P->spc = spc;
	}
ENDENTRY(P)
