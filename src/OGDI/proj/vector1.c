/* make storage for one and two dimensional matricies */
#ifndef lint
static const char SCCSID[]="@(#)vector1.c	4.3	93/06/12	GIE	REL";
#endif
#include <stdlib.h>
#include <projects.h>
	void * /* one dimension array */
vector1(int hi, int size) { return((void *)pj_malloc(size * (hi + 1))); }
	void ** /* two dimension array */
vector2(int uhi, int vhi, int size) {
	char **s;
	int nu, nv;

	nu = uhi + 1;
	nv = vhi + 1;
	if (s = (char **)pj_malloc(sizeof(void *) * nu + size * nv * nu)) {
		char *t;
		char **r;
		int i;

		t = (char *)(s + nu);
		r = s;
		for (i = 0; i < nu; ++i) {
			*r++ = t;
			t += nv * size;
		}
	}
	return ((void **)s);
}
