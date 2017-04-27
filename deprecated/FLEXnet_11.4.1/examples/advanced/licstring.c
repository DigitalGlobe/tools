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
 *	Description: 	Example of a license file in a buffer
 *
 *	D. Birns
 *	9/30/97
 *
 *	Last changed:  10/8/97
 *
 */
#include "lmclient.h"
#include "lm_code.h"
#include "lm_attr.h"
#include <stdio.h>
LM_CODE(code, ENCRYPTION_SEED1, ENCRYPTION_SEED2, VENDOR_KEY1,
	VENDOR_KEY2, VENDOR_KEY3, VENDOR_KEY4, VENDOR_KEY5);
char *feature_line = "FEATURE f0 demo 1.0 1-jan-2001 1 0 HOSTID=any";
main()
{
  LM_HANDLE *lm_job;
  char input[MAX_CONFIG_LINE];
  /* buf is big enough for FEATURE line plus SERVER line(s) plus prefix/suffix*/
  char config_text[MAX_CONFIG_LINE + 1];
  char buf[MAX_CONFIG_LINE + (80*4)];
  char server_line[80*3];
  char **cpp;
  CONFIG *conf;
  int days;

	*server_line = 0;
	if (lc_init((LM_HANDLE *)0, VENDOR_NAME, &code, &lm_job))
	{
		lc_perror(lm_job, "lc_init failed");
		exit(lc_get_errno(lm_job));
	}
	lc_set_attr(lm_job, LM_A_DISABLE_ENV, (LM_A_VAL_TYPE)1);
	*config_text = 0;
	while (fgets(input, MAX_CONFIG_LINE, stdin))
	{
		if (!strncmp(input, "SERVER", 6)) /* save the server line */
		{
			strncat(server_line, buf, 79);
		}
		else
		{
			strcat(config_text, input);
			if (input[strlen(input) - 1] != '\\') /* continue */
			{
				sprintf(buf,
					"START_LICENSE\n%s\n%s\nEND_LICENSE",
					server_line, config_text);
				lc_set_attr(lm_job, LM_A_LICENSE_FILE,
					(LM_A_VAL_TYPE)buf);
				cpp = lc_feat_list(lm_job, 0, 0);
				if (cpp && *cpp)
				{
					conf = lc_get_config(lm_job, *cpp);
					days = lc_expire_days(lm_job, conf);
					if (days == LM_FOREVER)
						printf("%s is permanent\n",
							conf->feature);
					else
						printf(
						"%s expires in %d days\n",
							conf->feature, days);
				}
				*config_text = 0;
			}


		}
	}
	lc_free_job(lm_job);
}
