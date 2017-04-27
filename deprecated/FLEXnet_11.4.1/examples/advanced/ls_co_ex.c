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
 *	Function: 	ls_co_filter_example
 *
 *	Description:
 * 			example of how to use ls_checkout() and
 *			ls_get_attr() in a vendor daemon checkout filter.
 *			The tricky part of this is that ls_get_attr()
 *			often returns a type unacceptable to
 *			ls_checkout, and the value
 *			has to be converted, as shown in the example.
 *
 * Return:
 * 	  0 - This function has processed the checkout request.
 *	      If the checkout fails then the errno should be set in the
 *	      global lm_job->lm_errno
 *	  1 - The request was not processed so normal server checkout
 *	      should occur.
 *
 *	D. Birns
 *	9/39/94
 *
 */
#include "lmclient.h"
#include <stdio.h>
#include "ls_attr.h"
#include "lsserver.h"
#include "lm_code.h"

/**********************************************************************
 * NOTE:
 * 	the function prototype for ls_checkout() that is
 *	used in this example is not in the include file so it
 *	is included here.
 *********************************************************************/

/**********************************************************************
 * Checkout a feature
 *
 * Return:
 * 	  0 - checkout not available
 *	  >0 - checkout done
 *	  <0 - checkout queued
 *
 *********************************************************************/
int ls_checkout(char *	feature,	/* The feature desired */
		char *	ndesired,	/* Number of licenses desired */
		char *	waitflag,	/* "Wait until available".
		     			 * if *wait == '1' the request will be
					 * queued if the license is not available */
		CLIENT_DATA *who,	/* Who is asking for it */
		char *	version,	/* The feature's version number */
		char *	dup_sel,	/* Duplicate selection criteria */
		char *	linger,		/* How long license lingers after checkin */
		char *	code,		/* Encryption code from license file line */
		int	always_zero, 	/* historical. Set to zero */
		int	always_one);	/* historical. Set to one */

/**********************************************************************
 * Example of a vendor daemon checkout filter using ls_checkout()
 * and ls_get_attr()
 *
 * To use this example set int (*ls_outfilter)() = ls_co_filter_example
 * in lsvendor.c
 *
 * Return:
 * 	  0 - This function has processed the checkout request.
 *	      If the checkout fails then the errno should be set in the
 *	      global lm_job->lm_errno
 *	  1 - The request was not processed so normal server checkout
 *	      should occur.
 *********************************************************************/
int ls_co_filter_example()
{
	char *str_feature;
	char str_num_lic_requested[MAX_LONG_LEN+1];
	char str_wait_flag[MAX_LONG_LEN+1];
	char *str_version;
	char str_dup_sel[MAX_LONG_LEN+1];
	char str_linger[MAX_LONG_LEN+1];
	char *str_code;
	CLIENT_DATA *client_data;
	int num_lic_requested;
	int wait_flag;
	int dup_sel;
	int linger;
	int did_checkout;

	/* which feature does the client want to checkout.
	 * See ls_attr.h for valid attribute values. */
	ls_get_attr(LS_ATTR_FEATURE, &str_feature);

	/* get the number of licenses requested */
	ls_get_attr(LS_ATTR_NLIC, &num_lic_requested);
	sprintf(str_num_lic_requested, "%d", num_lic_requested);  /* convert to string for ls_checkout() */

	/* get the "wait until available" flag */
	ls_get_attr(LS_ATTR_FLAG, &wait_flag);
	sprintf(str_wait_flag, "%d", wait_flag); /* convert to string for ls_checkout() */

	/* get the version of the requested feature */
	ls_get_attr(LS_ATTR_VERSION, &str_version);

	/* all the info about the client.
	 * There is a lot of stuff that can be filtered on in here. Be careful.
	 * See lsserver.h  */
	ls_get_attr(LS_ATTR_CLIENT, &client_data);

	/* duplicate selection criteria */
	ls_get_attr(LS_ATTR_DUP_SEL, &dup_sel);
	sprintf(str_dup_sel, "%d", dup_sel);  /* convert to string for ls_checkout() */

	/* how long will the license linger after checkin */
	ls_get_attr(LS_ATTR_LINGER, &linger);
	sprintf(str_linger, "%d", linger); /* convert to string for ls_checkout() */

	/* encryption code from license file */
	ls_get_attr(LS_ATTR_CODE, &str_code);

	/* do the checkout */
	did_checkout = ls_checkout(str_feature,
					str_num_lic_requested,
					str_wait_flag,
					client_data,
					str_version,
					str_dup_sel,
					str_linger,
					str_code,
					0,
					1);
	if ( did_checkout ==0 )
	{
	     	/* the checkout failed.
		 * may want to do something here   */
		;
	}

	return 0; /* this means we've done the ls_checkout call */
}
