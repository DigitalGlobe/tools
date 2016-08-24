/*
	sdms.c

	This program will convert from second back into degrees, minutes and
	seconds.

        Copyright (C) 1996 Logiciels et Applications Scientifiques (L.A.S.) Inc
        Permission to use, copy, modify and distribute this software and
        its documentation for any purpose and without fee is hereby granted,
        provided that the above copyright notice appear in all copies, that
        both the copyright notice and this permission notice appear in
        supporting documentation, and that the name of L.A.S. Inc not be used 
        in advertising or publicity pertaining to distribution of the software 
        without specific, written prior permission. L.A.S. Inc. makes no
        representations about the suitability of this software for any purpose.
        It is provided "as is" without express or implied warranty.
	
	Author: William Lau
*/ 
#include <stdio.h>
#include <math.h>

#include "nadconv.h"

#define TIMED	3600.000
#define TIMEM	60.000

N_COORD* sdms(n_lat,n_lon)
	double n_lat,n_lon;
{
	double rn[6] = {0.5,0.05,0.005,0.0005,0.00005,0.00005};
	double ga,gb,ilat,ilon;
	N_COORD *n_cd;

	n_cd = calloc(1,sizeof(N_COORD));
	if (!n_cd) {
		printf("cannot initialize the new coordinate table");
		return NULL;
	}

	ga = fabs(n_lat);
	gb = fabs(n_lon);
/*
	if((k < 0) || (k >= 6))
		k = 5;
	ga = ga - rn[k];
	gb = gb - rn[k];
*/
	ilat = fmod(ga,TIMED);
	ilon = fmod(gb,TIMED);

	n_cd->dlatn = abs(ga/TIMED);
	n_cd->dlonn = abs(gb/TIMED);

	n_cd->mlatn = abs(ilat/TIMEM);
	n_cd->mlonn = abs(ilon/TIMEM);

	n_cd->slatn = fmod(ilat,TIMEM);
	n_cd->slonn = fmod(ilon,TIMEM);

	n_cd->slatn = fabs(n_cd->slatn);
	n_cd->slonn = fabs(n_cd->slonn);

	if (n_cd->slatn == TIMEM)
	{
		n_cd->slatn = 0.0;
		n_cd->mlatn += 1;
	}

	if (n_cd->slonn == TIMEM)
	{
		n_cd->slonn = 0.0;
		n_cd->mlonn += 1;
	}

	if (n_cd->mlatn == 60)
	{
		n_cd->mlatn = 0;
		n_cd->dlatn += 1;
	}

	if (n_cd->mlonn == 60)
	{
		n_cd->mlonn = 0;
		n_cd->dlonn += 1;
	}

	if (n_lat < 0.0)
		n_cd->dlatn = -n_cd->dlatn;
	if (n_lon < 0.0)
		n_cd->dlonn = -n_cd->dlonn;

	return n_cd;
}
