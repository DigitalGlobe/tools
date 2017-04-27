/*  Hammer & Eckert-Greifendorff Projections */
#ifndef lint
static const char SCCSID[]="@(#)PJ_hammer.c	4.7	93/06/16	GIE	REL";
#endif
#define PROJ_PARMS__ \
	double w; \
	double m, rm;
#define PJ_LIB__
# include	<projects.h>
FORWARD(s_forward); /* spheroid */
	double cosphi, d;

	d = sqrt(2./(1. + (cosphi = cos(lp.phi)) * cos(lp.lam *= P->w)));
	xy.x = P->m * d * cosphi * sin(lp.lam);
	xy.y = P->rm * d * sin(lp.phi);
	return (xy);
}
FREEUP; if (P) pj_dalloc(P); }
ENTRY0(pj_hammer)
	if (pj_param(P->params, "tW").i) {
		if ((P->w = fabs(pj_param(P->params, "dW").f)) <= 0.) E_ERROR(-27);
	} else
		P->w = .5;
	if (pj_param(P->params, "tM").i) {
		if ((P->m = fabs(pj_param(P->params, "dM").f)) <= 0.) E_ERROR(-27);
	} else
		P->m = 1.;
	P->rm = 1. / P->m;
	P->m /= P->w;
	P->es = 0.; P->fwd = s_forward;
ENDENTRY(P)
