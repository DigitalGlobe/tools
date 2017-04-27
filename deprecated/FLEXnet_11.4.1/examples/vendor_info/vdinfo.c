
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
 *	Function:	Example program illustrating use of
			lc_get_attr(..., LM_A_VD_*, ...)
 *
 *	Description:    These attributes can retrieve important information
 *			from your vendor daemon.  Note that these functions
 *			will only work on your own vendor daemon, not
 *			another vendor daemon.
 *
 *			These routines send and receive messages from
 *			the vendor daemon in a far more efficient
 *			manner than the older function lm_userlist().
 *
 *	D. Birns
 *	9/30/94
 *
 *	Last changed:  4/13/95
 *
 */
#include "lmclient.h"
#include "lm_attr.h"
#include "lm_code.h"
LM_CODE(code, ENCRYPTION_SEED1, ENCRYPTION_SEED2, VENDOR_KEY1,
	VENDOR_KEY2, VENDOR_KEY3, VENDOR_KEY4, VENDOR_KEY5);

main(argc, argv)
char **argv;
{
  LM_HANDLE *lm_job;
  int rc;

	if (argc != 2)
	{
		printf("Usage: vdinfo feature\n");
		exit(1);
	}
	if ((rc = lc_init((LM_HANDLE *)0, VENDOR_NAME, &code, &lm_job)) &&
							rc != LM_DEMOKIT)
	{
		lc_perror(lm_job, "lm_init failed");
		exit(rc);
	}
	/*lc_set_attr(lm_job, LM_A_PORT_HOST_PLUS, 0);*/
	vendor_daemon_info(lm_job, argv[1]);
}

/*
 * Print out GENERIC and FEATURE information for every
 * license file line for a given feature name
 */
vendor_daemon_info(job, feature)
LM_HANDLE *job; /* if you want to use lc_* functions instead */
char *feature;
{
  CONFIG *conf, *c;
  LM_VD_GENERIC_INFO gi;
  LM_VD_FEATURE_INFO fi;
  int first = 1;
  char err[512];

	c = (CONFIG *)0;

	for (conf = lc_next_conf(job, feature, &c);conf;
					conf=lc_next_conf(job, feature, &c))
	{
		if (first)
		{
/*
 * 			get generic daemon info
 */
			gi.feat = conf;
			if (lc_get_attr(job, LM_A_VD_GENERIC_INFO,
								(short *)&gi))
			{
				lc_perror(job, "LM_A_VD_GENERIC_INFO");
			}
			else
			{
				printf("Generic Info: \n");
				printf("%-30s%d\n", "conn_timeout",
					gi.conn_timeout);
				printf("%-30s%d\n", "user_init1", gi.user_init1);
				printf("%-30s%d\n", "user_init2", gi.user_init2);
				printf("%-30s%d\n", "outfilter", gi.outfilter);
				printf("%-30s%d\n", "infilter", gi.infilter);
				printf("%-30s%d\n", "callback", gi.callback);
				printf("%-30s%d\n", "vendor_msg", gi.vendor_msg);
				printf("%-30s%d\n", "vendor_challenge",
							gi.vendor_challenge);
				printf("%-30s%d\n", "lockfile", gi.lockfile);
				printf("%-30s%d\n", "read_wait", gi.read_wait);
				printf("%-30s%d\n", "dump_send_data",
							gi.dump_send_data);
				printf("%-30s%d\n", "normal_hostid",
							gi.normal_hostid);
				printf("%-30s%d\n", "conn_timeout",
							gi.conn_timeout);
				printf("%-30s%d\n", "enforce_startdate",
							gi.enforce_startdate);
				printf("%-30s%d\n", "tell_startdate",
							gi.tell_startdate);
				printf("%-30s%d\n", "minimum_user_timeout",
					   	gi.minimum_user_timeout);
				printf("%-30s%d\n", "min_lmremove",
							gi.min_lmremove);
				printf("%-30s%d\n", "use_featset",
							gi.use_featset);
				printf("%-30s%x\n", "dup_sel", gi.dup_sel);
				printf("%-30s%d\n", "use_all_feature_lines",
					    	gi.use_all_feature_lines);
				printf("%-30s%d\n", "do_checkroot",
							gi.do_checkroot);
				printf("%-30s%d\n", "show_vendor_def",
							gi.show_vendor_def);
				printf("%-30s%d\n", "allow_borrow",
							gi.allow_borrow);
			}
			first = 0;
		}
		fi.feat = conf;
		if (lc_get_attr(job, LM_A_VD_FEATURE_INFO, (short *)&fi))
		{
			sprintf(err,
			"LM_A_VD_FEATURE_INFO failed. feature %s, code %s",
						conf->feature, conf->code);
			lc_perror(job,err);
		}
		else
		{
/*
 * 			get specific feature info
 */
			printf("\n%-30s%s\n", "feature", conf->feature);
			printf("%-30s%s\n", "code", conf->code);
			printf("%-30s%d\n", "rev", fi.rev);
			printf("%-30s%d\n", "timeout", fi.timeout);
			printf("%-30s%d\n", "linger", fi.linger);
			printf("%-30s%x\n", "dup_select", fi.dup_select);
			printf("%-30s%d\n", "res", fi.res);
			printf("%-30s%d\n", "tot_lic_in_use", fi.tot_lic_in_use);
			printf("%-30s%d\n", "float_in_use", fi.float_in_use);
			printf("%-30s%d\n", "user_cnt", fi.user_cnt);
			printf("%-30s%d\n", "num_lic", fi.num_lic);
			printf("%-30s%d\n", "queue_cnt", fi.queue_cnt);
			printf("%-30s%d\n", "overdraft", fi.overdraft);
		}
		printf("CONFIG info:\n");
		printf("%-30s%d\n", "type", conf->type);
		printf("%-30s%s\n", "version", conf->version);
		printf("%-30s%s\n", "exp date", conf->date);
		printf("%-30s%d\n", "num users",conf->users);
		printf("%-30s%s\n", "license key",conf->code);
		if (conf->lc_vendor_def)
			printf("%-30s%s\n", "vendor_def",conf->lc_vendor_def);
	}
	if (first)
	{
		if (lc_get_errno(job))
			printf("Can't get feature %s (%s)\n", feature,
				lc_errstring(job));
		else
			printf("No such feature %s\n", feature);
	}
}

