/* Perspective Conic */
#ifndef lint
static const char SCCSID[]="@(#)PJ_pconic.c	4.5	93/06/14	GIE	REL";
#endif
#define PROJ_PARMS__ \
	double	phi1; \
	double	phi2; \
	double	n; \
	double	rho; \
	double	rho0; \
	double	c2; \
	double	check; \
	int		negative;
#define PJ_LIB__
# include	<projects.h>
# define EPS10	1.e-10
FORWARD(s_forward); /* sphere & ellipsoid */
	
	if ((P->negative && lp.phi >= P->check) ||
	    (!P->negative && lp.phi <= P->check)) F_ERROR;
	P->rho = P->rho0 - P->c2 * tan(lp.phi - P->phi0);
	xy.x = P->rho * sin( lp.lam *= P->n );
	xy.y = P->rho0 - P->rho * cos(lp.lam);
	return (xy);
}
FREEUP; if (P) pj_dalloc(P); }
ENTRY0(pj_pconic)
	double cosphi, sinphi;

	P->phi1 = pj_param(P->params, "rlat_1").f;
	P->phi2 = pj_param(P->params, "rlat_2").f;
	if (fabs(P->phi0 = 0.5 * (P->phi1 + P->phi2)) < EPS10) E_ERROR(-21);
	P->check = P->phi0 + ((P->negative = P->phi0 < 0.) ? HALFPI : -HALFPI);
	P->n = sinphi = sin(P->phi0);
	cosphi = cos(P->phi0);
	P->c2 = cos(0.5 * (P->phi2 - P->phi1));
	P->rho0 = P->c2 * cosphi / sinphi;
	P->es = 0.; P->fwd = s_forward;
ENDENTRY(P)
