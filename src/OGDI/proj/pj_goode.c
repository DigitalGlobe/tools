/*  Goode Homolosine Projection */
#ifndef lint
static const char SCCSID[]="@(#)PJ_goode.c	4.4	93/06/14	GIE	REL";
#endif
#define PROJ_PARMS__ \
	struct PJconsts	*sinu; \
	struct PJconsts	*moll;
#define PJ_LIB__
#include	<projects.h>
	extern PJ
*pj_sinu(PJ *), *pj_moll(PJ *);
#define Y_COR		0.05280
#define PHI_LIM	.71093078197902358062
FORWARD(s_forward); /* spheroid */
	if (fabs(lp.phi) <= PHI_LIM)
		xy = P->sinu->fwd(lp, P->sinu);
	else {
		xy = P->moll->fwd(lp, P->sinu);
		xy.y -= lp.phi >= 0.0 ? Y_COR : -Y_COR;
	}
	return (xy);
}
FREEUP;
	if (P) {
		if (P->sinu)
			(*(P->sinu->pfree))(P->sinu);
		if (P->moll)
			(*(P->moll->pfree))(P->moll);
		pj_dalloc(P);
	}
}
ENTRY2(pj_goode, sinu, moll)
	P->es = 0.;
	if (!(P->sinu = pj_sinu(0)) || !(P->moll = pj_moll(0)))
		E_ERROR_0;
	if (!(P->sinu = pj_sinu(P->sinu)) || !(P->moll = pj_moll(P->moll)))
		E_ERROR_0;
	P->fwd = s_forward;
ENDENTRY(P)
