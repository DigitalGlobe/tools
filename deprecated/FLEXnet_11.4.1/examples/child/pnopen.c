/******************************************************************************

	    Copyright (c) 1994-2007 Macrovision Europe Ltd. and/or Macrovision Corporation. All Rights Reserved.
	This software has been provided pursuant to a License Agreement
	containing restrictions on its use.  This software contains
	valuable trade secrets and proprietary information of
	Macrovision Inc and is protected by law.  It may
	not be copied or distributed in any form or medium, disclosed
	to third parties, reverse engineered or used in any manner not
	provided for in said License Agreement except with the prior
	written authorization from Macrovision Corporation.
 *****************************************************************************/
/*
 *
 *	Function:
 *
 *	Description:
 *
 *	Parameters:
 *
 *	Return:
 *
 * Author:  Jim McBeath, Macrovision Corporatin
 *
 *
 */

#include "lmclient.h"

/* pnopen.c- like popen, but with more than one FILE */

#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include "pnopen.h"

extern char *malloc(), *calloc();
extern free();

static int fdalloc=0;
static int *fd=0;

struct pnopen *		/* or NULL on error */
pnopen(command,modes)
char *command;		/* the command to execute */
char *modes;		/* r/w modes of the files of interest */
{
	struct pnopen *pninfo;
	int l;
	char *m;
	int t;
	int d;

	pninfo = (struct pnopen *)calloc(1,sizeof(struct pnopen));
	if (!pninfo) {
		errno = ENOMEM;
		return NULL;	/* can't allocate */
	}
	l = strlen(modes);
	pninfo->numfiles = l;
	pninfo->files = (FILE **)calloc((unsigned)l,sizeof(FILE *));
	if (!pninfo->files) {
		(void)free((char *)pninfo);
		errno = ENOMEM;
		return NULL;	/* can't allocate */
	}
	if (fdalloc<l*2) {
		if (fd) (void)free((char *)fd);
		fd = (int *)malloc((unsigned)(l*2)*sizeof(int));
	}
	l = 0;
	for (m=modes; *m; m++) {
		fd[l*2] = -1;
		fd[l*2+1] = -1;
		switch (*m) {
		case 'r':
		case 'w':
			if (pipe(fd+(l*2))<0)
				goto eclose;	/* errno already set */
			break;
		case '-':
			break;		/* leave this one untouched */
		default:
			errno = EINVAL;		/* invalid argument */
			goto eclose;
		}
		l++;
	}
	t = fork();
	if (t==0) {	/* child */
		m = modes;
		for (l=0; l<pninfo->numfiles; l++) {
			switch (*m++) {
			case 'r':
				(void)close(fd[2*l]);
					/* close read half in chld */
				d = fd[2*l+1];
				break;
			case 'w':
				(void)close(fd[2*l+1]);/* close write half */
				d = fd[2*l];
				break;
			default:	/* must be '-' */
				continue;	/* ignore it */
			}
			if (d!=l) {
				/* move it to the right descriptor */
				if (dup2(d,l)<0)
					goto cclose;
				(void)close(d);
			}
		}
		/* OK, we've rearranged our descriptors, now go for it! */
		execl("/bin/sh","sh","-c",command,(char *)0);
		perror(command);
		exit(1);
cclose:	/* error in child */
		perror("dup2 failed in child");
		exit(1);
	} else if (t>0) {	/* parent */
		pninfo->pid = t;
		m = modes;
		for (l=0; l<pninfo->numfiles; l++) {
			switch (*m++) {
			case 'r':
				(void)close(fd[2*l+1]);
					/* close write half in parent */
				d = fd[2*l];
				pninfo->files[l] = fdopen(d,"r");
				if (!pninfo->files[l]) {
					/* hmmm, should not happen */
					goto peclose;
				}
				break;
			case 'w':
				(void)close(fd[2*l]);/* close read half */
				d = fd[2*l+1];
				pninfo->files[l] = fdopen(d,"w");
				if (!pninfo->files[l]) {
					/* hmmm, should not happen */
					goto peclose;
				}
				break;
			default:	/* must be '-' */
				d = -1;
				break;	/* ignore it */
			}
		}
		return pninfo;
	} else {	/* error */
		/* errno already set by fork */
		return NULL;
	}
	/* NOTREACHED */
eclose:	/* here on error to close up pipes before returning */
	for (; l>=0; --l) {
		if (fd[2*l]>=0)
			(void)close(fd[2*l]);
				/* close both halves of all pipes */
		if (fd[2*l+1]>=0)
			(void)close(fd[2*l+1]);
	}
	return NULL;
peclose:	/* here on error in parent after the fork */
	/* TBD - close up stuff in pninfo->files, then in fd[] */
	return NULL;
}

int		/* returns child exit status, or -1 on error */
pnclose(pninfo)
struct pnopen *pninfo;
{
	int l, t;
	int status;

	for (l=0; l<pninfo->numfiles; l++) {
		if (pninfo->files[l]) {
			(void)fclose(pninfo->files[l]);
				/* TBD - any error checks? */
			pninfo->files[l] = (FILE *)0;
		}
	}
	if (pninfo->pid==0) {
		errno = ESRCH;	/* no such child process */
		return -1;
	}
	while (1) {	/* wait for child to stop */
#ifdef lint
		t = 1;		/* to keep lint from complaining about wait */
#else
		t = wait(&status);
#endif
		if (t== -1)
			return -1;	/* no children */
		if (t==pninfo->pid)
			return status;
	}
}

/* end */
