/******************************************************************************

	    Copyright (c) 1989-2007 Macrovision Europe Ltd. and/or Macrovision Corporation. All Rights Reserved.
	This software has been provided pursuant to a License Agreement
	containing restrictions on its use.  This software contains
	valuable trade secrets and proprietary information of
	Macrovision Corporation and is protected by law.  It may
	not be copied or distributed in any form or medium, disclosed
	to third parties, reverse engineered or used in any manner not
	provided for in said License Agreement except with the prior
	written authorization from Macrovision Corporation.

 *****************************************************************************/
/*

 *
 *	Description:	Reads an existing license file and fills in the
 *			encryption codes in the FEATURE and FEATURESET lines.
 *
 *	Usage:		lmcrypt [iofiles] [-i inputfile] [-o outputfile]
 *
 *
 */

#include "lmprikey.h"

#include "lmclient.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>


#ifdef PC
#include <malloc.h>
#include <io.h>
#ifdef OS2
#define mktemp(x) tmpnam(NULL)
#endif /* OS2 */
#else
char *mktemp();
char *getenv();
#endif /* PC */

#include "lm_code.h"
#include "lmseeds.h"
#include "lm_attr.h"

LM_HANDLE *lm_job;

/* forward ref */
static char * safe_malloc lm_args((int));
void usage();
char *errfilename;
FILE *efp;
int forceit = LM_CRYPT_FORCE; /* new default in v5.1 */
int dofilecrypt lm_args((char *infilename, char *outfilename,
			VENDORCODE *code));
int dofile1crypt lm_args((char *filename, VENDORCODE *code));
int dofpcrypt lm_args((FILE *ifp, FILE *ofp, char *ifname,
		char *ofname, VENDORCODE *code));

LM_CODE_NEW(site_code, ENCRYPTION_SEED1, ENCRYPTION_SEED2,
            VENDOR_KEY1, VENDOR_KEY2, VENDOR_KEY3,
            VENDOR_KEY4, VENDOR_KEY5,
            FLEXLM_VERSION, FLEXLM_REVISION, FLEXLM_PATCH, LM_VER_BEHAVIOR,
            TRL_KEY1, TRL_KEY2, LM_STRENGTH);
int
main(int argc, char * argv[])
{
	int i;
	int estat=0;
	int iofilecount=0;
	int cpyrtflag=0;
	char **iofiles;
	char *infilename = NULL;
	char *outfilename = NULL;
    VENDORCODE *code;
    char primVendName[MAX_LONGNAME_SIZE + 1];

	errfilename = 0;


    LM_CODE_GEN_INIT_NEW(&site_code, ENCRYPTION_SEED1, ENCRYPTION_SEED2,
        l_priseedcnt, lm_prikey, lm_prisize);

    code = &site_code;
    strcpy(primVendName, VENDOR_NAME);
    if (lc_init((LM_HANDLE *)0, primVendName, code, &lm_job))
    {
        lc_perror(lm_job, "lc_init failed");
        exit(-1);
    }

	iofiles = (char **)safe_malloc(argc*sizeof(char *));
	for (i=1; i<argc; i++)
	{
		if (strcmp(argv[i],"-i")==0)
		{
			infilename = argv[++i];
			if (!infilename)
			{
				usage();
				exit(1);
			}
		}
		else if (strcmp(argv[i],"-o")==0)
		{
			outfilename = argv[++i];
			if (!outfilename)
			{
				usage();
				exit(1);
			}
		}
		else if (strcmp(argv[i],"-e")==0)
		{
			errfilename = argv[++i];
			if (!errfilename)
			{
				usage();
				exit(1);
			}
		}
/*
 *		NOTE: LM_CRYPT_FORCE is the new default
 *		so -f does nothing.
 */
		else if (!strcmp(argv[i],"-f"))
			forceit |= LM_CRYPT_FORCE;
		else if (!strcmp(argv[i],"-longkey"))
			lc_set_attr(lm_job, LM_A_LKEY_LONG, (LM_A_VAL_TYPE)1);
		else if (!strcmp(argv[i],"-shortkey"))
			lc_set_attr(lm_job, LM_A_LKEY_LONG, (LM_A_VAL_TYPE)0);
		else if (!strcmp(argv[i], "-java"))
			lc_set_attr(lm_job, LM_A_JAVA_LIC_FMT,(LM_A_VAL_TYPE)1);
		else if (!strcmp(argv[i],"-decimal"))
			forceit |= LM_CRYPT_DECIMAL_FMT;
		else if (!strcmp(argv[i],"-maxlen"))
		{
			int max = 0;
			i++;

			if (i >= argc)
				break;
			sscanf(argv[i], "%d", &max);
			if (!max) fprintf(stderr,
				"Error: -maxlen %s Invalid line length\n", argv[i]);
			else
				lc_set_attr(lm_job, LM_A_MAX_LICENSE_LEN, (LM_A_VAL_TYPE)max);
		}
		else if (!strcmp(argv[i],"-verfmt"))
		{
			i++;
			if (i >= argc)
				break;
			switch(*argv[i])
			{
			case '2':
				lc_set_attr(lm_job, LM_A_LICENSE_FMT_VER, (LM_A_VAL_TYPE)LM_BEHAVIOR_V2);
				break;
			case '3':
				lc_set_attr(lm_job, LM_A_LICENSE_FMT_VER, (LM_A_VAL_TYPE)LM_BEHAVIOR_V3);
				break;
			case '4':
				lc_set_attr(lm_job, LM_A_LICENSE_FMT_VER, (LM_A_VAL_TYPE)LM_BEHAVIOR_V4);
				break;
			case '5':
				if (!strncmp(argv[i],"5.1", 3))
				{
					lc_set_attr(lm_job, LM_A_LICENSE_FMT_VER,
						(LM_A_VAL_TYPE)LM_BEHAVIOR_V5_1);
				}
				else
				{
					lc_set_attr(lm_job,	LM_A_LICENSE_FMT_VER,
						(LM_A_VAL_TYPE)LM_BEHAVIOR_V5);
				}
				break;
			case '6':
				lc_set_attr(lm_job, LM_A_LICENSE_FMT_VER, (LM_A_VAL_TYPE)LM_BEHAVIOR_V6);
				break;
			case '7':
				if (!strncmp(argv[i],"7.1", 3))
				{
					lc_set_attr(lm_job,	LM_A_LICENSE_FMT_VER,
						(LM_A_VAL_TYPE)LM_BEHAVIOR_V7_1);
				}
				else
				{
					lc_set_attr(lm_job,	LM_A_LICENSE_FMT_VER,
						(LM_A_VAL_TYPE)LM_BEHAVIOR_V7);
				}
				break;
			default :
				usage();
			}
		}
		else if (!strcmp(argv[i],"-r"))
		{
			printf("lmcrypt(%s) - "COPYRIGHT_STRING(1991)"\n",	primVendName);
			cpyrtflag = 1;
		}
		else if (argv[i][0]=='-')
		{	/* unknown switch */
			usage();
			exit(1);
		}
		else
			iofiles[iofilecount++] = argv[i];
	}
	if (!errfilename || strcmp(errfilename,"-")==0)
	{
		errfilename = "stderr";
	}
	if (iofilecount)
	{
		for (i=0; i<iofilecount; i++)
		{
			estat |= dofile1crypt(iofiles[i], code);
		}
		if (!infilename && !outfilename)
			exit(estat);
	}
	if (cpyrtflag && !infilename && !outfilename)
		exit(0);
	if (!infilename || strcmp(infilename,"-")==0)
	{
		infilename = "stdin";
	}
	if (!outfilename || strcmp(outfilename,"-")==0)
	{
		outfilename = "stdout";
	}
	estat |= dofilecrypt(infilename,outfilename, code);
	exit(estat);
	return 0;
}

void
usage()
{
fprintf(stderr,"usage: lmcrypt [-i infile] [-o outfile] [-e errfile]\n\t[licenses ...] [-r] [-verfmt ver] [-decimal] [-maxlen n] [-help]\n");
fprintf(stderr,"   [-r]     Print copyright notice.\n");
fprintf(stderr,"   [-help]  Display usage information.\n");
fprintf(stderr,
	"   If no input file, or if specified as - or stdin, stdin is used.\n");
fprintf(stderr,
	"   If no output file, or if specified as - or stdout, stdout is used.\n");
fprintf(stderr,
    "   If no error file, or if specified as - or stderr, stderr is used.\n");
fprintf(stderr,"   licenses are read and written back in place.\n");
fprintf(stderr,"   If no arguments, reads stdin and writes stdout.\n");
fprintf(stderr,"   -verfmt prints in old format and prints errors when \n");
fprintf(stderr,"   is newer than specified. Version formats are 2-5, 5.1, 6, 7 and 7.1\n");

}

static
char *
safe_malloc(int n)
{
	char * p = NULL;

	p = (char *)malloc(n);
	if (p == NULL)
	{
		fprintf(stderr, "lmcrypt: no more memory\n");
		exit(2);
	}
	return p;
}

int
dofile1crypt(char * filename, VENDORCODE *code)
{
	char obuf[LM_MAXPATHLEN+10] = {'\0'};
	int estat = 0;
	int t = 0;

#ifdef PC
	char * cp = NULL;

	strcpy( obuf, filename );
	if ( cp = strchr(obuf, '.') )
		*cp = 0;
	strcat( obuf, ".XXXXXX" );
#else
	(void)sprintf(obuf,"%s.XXXXXX",filename);
#endif /* PC */
	(void)mktemp(obuf);
	estat = dofilecrypt(filename,obuf, code);
	if (estat==0)
	{
		/* succeeded, move new file onto old */
#ifdef PC
		unlink( filename );
#endif
		t = rename(obuf,filename);
		if (t)
		{
			perror(filename);
			estat = 1;
		}
	}
	return estat;
}

int
dofilecrypt(
	char *			infilename,
	char *			outfilename,
	VENDORCODE *	code)
{
	FILE * ifp = NULL;
	FILE * ofp = NULL;
	int ret = 0;

	if (strcmp(infilename,"stdin")==0)
		ifp = stdin;
	else
		ifp = fopen(infilename,"r");
	if (!ifp)
	{
		perror(infilename);
		return 1;
	}
	if (strcmp(errfilename,"stderr")==0)
		efp = stderr;
	else
		efp = fopen(errfilename,"w");
	if (!efp)
	{
		perror(errfilename);
		return 1;
	}
	if (strcmp(outfilename,"stdout")==0)
		ofp = stdout;
	else
		ofp = fopen(outfilename,"w");
	if (!ofp)
	{
		perror(outfilename);
		return 1;
	}

	ret = dofpcrypt(ifp,ofp,infilename,outfilename, code);
	fclose(ifp);
	fclose(ofp);
	return ret;
}



int	/* 0 if OK, 1 if error */
dofpcrypt(
	FILE *			ifp,
	FILE *			ofp,
	char *			ifname,	/* for error messages */
	char *			ofname,	/* for error messages */
	VENDORCODE *	code)
{
  struct stat st;
  char * str = NULL, * cur = NULL, * ofile_str = NULL, * err = NULL;
  FILE *tmpf = (FILE *)0;
  int c;
  int std_in = 0;
  char *tempname = 0;
#ifndef PC
  char *temp_prefix;
  char tmp_buf[500] = {'\0'};
#endif /* PC */
  /*VENDORCODE v;*/

	if (!strcmp(ifname, "stdin"))
	{
		std_in = 1;
/*
 *		read stdin into temp file, and use that as input file
 */
#ifdef PC
		tempname = ifname =  mktemp(".\\lmXXXXXX");
#else
		if (!(temp_prefix = getenv("TMPDIR")))
			temp_prefix = "/usr/tmp";
		sprintf(tmp_buf, "%s/lmcryptXXXXXX", temp_prefix);	/* OVERRUN */
		tempname = ifname =  mktemp(tmp_buf);
#endif /* PC */
		if (!*ifname)
		{
			perror("Can't create temporary file");
			return 1;
		}
		if (!(tmpf = fopen(ifname, "w")))
		{
			perror("Can't open temporary file");
			return 1;
		}
		while ((c = getchar()) != EOF)
			fputc(c, tmpf);
		fclose(tmpf);

		ifp = fopen(ifname, "r");
	}
	if (stat(ifname, &st))
	{
		perror("Can't stat input file");
		return 1;
	}
	cur = str = (char *)safe_malloc(st.st_size + 1);
	while (fgets(cur, (int)st.st_size, ifp))	/* overrun checked */
		cur += strlen(cur);
	if (std_in)
		ifname = "stdin";
	if (lc_cryptstr(lm_job, str, &ofile_str, /*&v*/code,
			forceit, ifname, &err))
	{
		if (efp == stderr)
		{
			fprintf(ofp, "%s\n", err);
			if (ofp != stdout)
				fprintf(stderr, "%s\n", err);
		}
		else
			fprintf(efp, "%s\n", err);
	}
	if (ofile_str)
	{
		fputs(ofile_str, ofp);
		lc_free_mem(lm_job, ofile_str);
	}
	free(str);
	if (tempname)
	{
		if (tmpf)
			fclose(tmpf);
		remove(tempname);
	}
	return 0;
}
