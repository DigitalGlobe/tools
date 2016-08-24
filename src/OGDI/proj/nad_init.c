/* open structure for NAD27<->NAD83 conversion */
#ifndef lint
static const char SCCSID[]="@(#)nad_init.c	4.3   93/08/25 GIE REL";
#endif
#define PJ_LIB__
#include <projects.h>
#include <stdio.h>
#include <errno.h>
extern FILE *pj_open_lib(char *, char *);
	struct CTABLE *
nad_init(char *name) {
	char fname[FILENAME_MAX+1];
	struct CTABLE *ct;
	FILE *fid;
	size_t i;

	errno = pj_errno = 0;
	if (!(fid = pj_open_lib(name, "rb"))) {
	  strcpy(fname, "nad2783/");
	  strcat(fname, name);
	  if (!(fid = pj_open_lib(fname, "rb"))) {
	    pj_errno = errno;
	    return 0;
	  }
	}
	
	if (!(ct = pj_malloc(124)) ||
	    fread(ct, 124, 1, fid) != 1 ||
	    !(ct->cvs = (FLP *)pj_malloc(i=sizeof(FLP)*ct->lim.lam*ct->lim.phi)) ||
	    fread(ct->cvs, i, 1, fid) != 1) {
	  nad_free(ct);
	  pj_errno = -38;
	  ct = 0;
	  return 0;
	}
	fclose(fid);
	pj_errno = 0;
	return ct;
}
	void
nad_free(struct CTABLE *ct) {
	if (ct) {
		pj_dalloc(ct->cvs);
		pj_dalloc(ct);
	}
}
