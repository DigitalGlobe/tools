/******************************************************************************

	    Copyright (c) 1988-2007 Macrovision Europe Ltd. and/or Macrovision Corporation. All Rights Reserved.
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
 *	Description: Definitions of feature in-memory data
 *
 *
 */

/*
 **************************************************************************
 *	NOTE: If you modify this file, you MUST, MUST, update lm_noserver.c
 **************************************************************************
 */

/*
 **************************************************************************
 *	NOTE: Changing these values will affect report log data.
 **************************************************************************
 */
/*
 *	Per-feature user-specified options
 */
#ifndef _INCLUDE_LSFEATURE_H
#define _INCLUDE_LSFEATURE_H
typedef struct options {
			struct options *next; /* Link */
			char type;	/* Type of option */
#define RESERVE 1			/* Reserve a feature */
#define INCLUDE 2			/* Only these users allowed */
#define EXCLUDE 3			/* These users disallowed */
#define OPT_BORROW  4			
#define OPT_TRANSPORT  5		
#define OPT_OVERDRAFT  6		/* Reduce overdraft cnt */
#define OPT_MAX  7			/* MAX count for group/feature */
#define DYNAMIC_RESERVE	8
#define DYNAMIC_RESERVE_EXCLUDE_PACKAGE	9	/*	if specified, set option to DYNAMIC_RESERVE but don't reserve the package itself */
#define OPT_EXTERNAL_FILTERS 10
			int type2;
#define OPT_HOST		0x00000001			/* applies to host */
#define OPT_USER		0x00000002			/* applies to user */
#define RES_USER_v1_0	0x00000002				/* Comm v1.0 value */
#define OPT_DISPLAY		0x00000004			/* applies to display */
#define OPT_GROUP		0x00000008			/* applies to group */
#define OPT_HOST_GROUP	0x00000010			/* applies to group */
#define OPT_INTERNET	0x00000020			/* applies to client internet address */
#define OPT_PROJECT		0x00000040			/* $LM_PROJECT */
#define OPT_INET_GROUP	0x00000080
#define OPT_BUNDLE		0x00000100			/* part of a bundle, means options usuable depending on what SUITE_DUP_GROUP is */
#define OPT_POST_INFILTER		0x00000200	/* external filter types */
#define OPT_PRE_OUTFILTER		0x00000400
#define OPT_POST_OUTFAILFILTER	0x00000800
			char *name;	/* Name of user/host/group */
			short inet_addr[4]; /* Internet address - deprecated should not be used */
			char addr_type;	/* Internet address type a/b/c */
			int count;	/* MAX option only */
			char *internal_use1; /* for growth */
			char *internal_use2; /* for vendor user */
			int dup_select;		/*	Used by BUNDLE to determine who the reservation is for, corresponds to what SUITE_DUP_GROUP is set to */
			char *	pszUser;
			char *	pszHost;
			char *	pszDisplay;
			char *	pszVendor;
			int numRes;
			int timeout;		/* ReadTimeout - used by the external filter */

			/* private data for options. size will change across
			 * releases */
			char internalData[52];

			} OPTIONS;



/*
 *	The user database for this server
 */


typedef struct _u {
/*
 *		    Links and structure ID.
 */
		    int dup_select; 	/* actual dup-select used by
					   this feature-list, in case
					   there's more than one */
		    struct _u *next;
		    struct _u *brother;	/* "Brothers" if this feature supports
						       grouping duplicates */
		    OPTIONS *o;	/* Listhead of reservation structs */
		    int serialno;	/* Serial number of this struct */
		    int seq;		/* Sequence # of this struct */
		    int license_handle;	/* 100xserialno + seq%100 */
/*
 *		    Client-supplied data
 */
			char name[MAX_LONGNAME_SIZE + 1];
			char node[MAX_LONGNAME_SIZE + 1];
			char display[MAX_LONGNAME_SIZE + 1];
		    char project[MAX_PROJECT_LEN + 1];
		    char vendor_def[MAX_VENDOR_CHECKOUT_DATA + 1];
		    char version[MAX_VER_LEN+1];  /* Version number */
		    char code[MAX_CRYPT_LEN+1];	/* Code from license file */
		    int count;		/* How many of these licenses he has */
		    int linger;		/* How long license is to linger */
/*
 *		    Derived license information
 */
		    long time;		/* Time user started using feature */
		    long endtime;	/* Time user stopped using feature */
		    long reread_time; 	/* when the last reread occurred */
		    int normal_count;	/* How many "normal" licenses */
		    HANDLE_T handle;
		    HANDLE_T group_handle; 
		    long flags;		/* 32 flags */
#define LM_ULF_BORROWED			0x1
#define LM_ULF_REMOVED			0x00000002	/*	Used to indicate that this user was removed via lmremove */
#define LM_ULF_BORROW_INIT		0x00000004	/*	Indicates that this user was created after a server restart */
		    char *internal_use1; /* for growth */
		    char *internal_use2; /* for vendor user */
			HANDLE_T orgHandle;	
			CLIENT_DATA	*lingerClient; /* store client data for disconnected lingered client.
									      Used only in report log.  If you decide to use it in other 
										  areas, you are on your own.  */
		  } USERLIST;
		  
USERLIST *f_list();


/*
 *	linked list of old_codes -- for reread 
 */

typedef struct old_codes_list {
				struct old_codes_list *next;
				char code[MAX_CRYPT_LEN+1];
			      } OLD_CODE_LIST;




/*
 *	The feature database
 */

typedef struct feature_list {
/*
 *			Links
 */
			struct feature_list *next;  /* Forward link */
			struct feature_list *last;  /* Backward link */
			struct feature_list *package;  /* If component 
						or LM_PKG_PARENT if 
						it's the enabling feature 
						or 0 if it's neither */
#define LM_PKG_PARENT (FEATURE_LIST *)1L
			USERLIST *u;	/* Current users */
			USERLIST *queue;/* Users waiting */
			OPTIONS *opt;	/* User-specified options */
			OPTIONS *include; /* User-specified INCLUDE list */
			OPTIONS *reread_include; /* waiting for ls_user_based_reread_delay */
			OPTIONS *exclude; /* User-specified EXCLUDE list */
			OPTIONS *b_include;  /* User's BORROW include list */
			OPTIONS *b_exclude;  /* User's BORROW exclude list */
			OPTIONS *res_list;  
			OPTIONS *reread_reserves; /* for reread */

			OPTIONS *precheckout;			/* External pre check out filter list */
			OPTIONS *postcheckoutfailure;	/* External post check out failure filter list */
			OPTIONS *postcheckin;			/* External post check in filter list */

/*
 *			Static data from license
 */
			char *feature;	/* Name of the feature (NULL at end) */
			char version[MAX_VER_LEN+1];	/* Version of feature */
			char expdate[DATE_LEN+1]; /* Expiration date */
			char *vendor_def; /* Vendor-defined string */
			char code[MAX_CRYPT_LEN+1]; /* Encryption code */
			OLD_CODE_LIST *old_codes; /* old codes -- for reread */
			int nlic;	/* Number of licenses available --
					   includes overdraft --  # of "legal" 
					   licenses is nlic - overdraft */
			int overdraft;	/* # of overdraft licenses */
			char *id;	/* Node-lock hostid (pre-conversion) */
			HOSTID *hostid;	/* Node-lock hostid (convert if need) */
/*
 *			Dynamic license data
 */
			short sticky_dup; /* dup_select specified in license */
			short dup_select; /* How does feature count dups */
			int sticky_linger;/* LINGER in license file */
			int linger;	  /* User or license file LINGER
					     specification */
			LS_POOLED *pooled; /* expire once a day */
/*
 *			End-user specified options
 *				(Note that linger is also end-user specified
 *				or from license file)
 */
			int lowwater;	/* Minimum to allow for borrow */
			int timeout;	/* User inactivity timeout */
			int res;	/* # reserved licenses */
			int maxborrow;	/* Maximum BORROW interval (hours) */
			unsigned int type_mask;	/* from CONFIG lc_type_mask;*/
			int user_based;
			int host_based;
			int minimum;
			int flags;
#define LM_FL_FLAG_USER_BASED_ERR 	 0x1
#define LM_FL_FLAG_UNCOUNTED 		 0x2
#define LM_FL_FLAG_LS_HOST_REMOVED 	 0x4
#define LM_FL_FLAG_REREAD_REMOVED 	 0x8
#define LM_FL_FLAG_ALL_RESERVED        	0x10
#define LM_FL_FLAG_METER_BORROW        	0x20
#define LM_FL_FLAG_SUITE_PARENT        	0x40
/*
 *	If it's a NONSUITE, it's also NONBUNDLE
 */
#define LM_FL_FLAG_NONSUITE_PARENT      0x80
#define LM_FL_FLAG_SUITE_COMPONENT     0x100
#define LM_FL_FLAG_NONSUITE_COMPONENT  0x200
#define LM_FL_FLAG_INTERNAL1  	       0x400
#define LM_FL_FLAG_INTERNAL2  	       0x800
#define LM_FL_FLAG_INTERNAL3  	      0x1000
#define LM_FL_FLAG_BUNDLE_PARENT      0x2000
#define LM_FL_FLAG_BUNDLE_COMPONENT   0x4000
#define LM_FL_FLAG_PKG_PARENT      	(LM_FL_FLAG_SUITE_PARENT | \
					LM_FL_FLAG_NONSUITE_PARENT | \
					LM_FL_FLAG_BUNDLE_PARENT  )
#define LM_FL_FLAG_COMPONENT  	       (LM_FL_FLAG_SUITE_COMPONENT| \
					LM_FL_FLAG_BUNDLE_COMPONENT | \
					LM_FL_FLAG_NONSUITE_COMPONENT)
#define LM_FL_FLAG_SUITEBUNDLE_PARENT	(LM_FL_FLAG_SUITE_PARENT | \
					 LM_FL_FLAG_BUNDLE_PARENT)
#define LM_FL_FLAG_SUITEBUNDLE_COMPONENT	(LM_FL_FLAG_SUITE_COMPONENT | \
					 LM_FL_FLAG_BUNDLE_COMPONENT)
			char **platforms; /* from CONFIG lc_platforms */
			CONFIG *conf; 	/* first feat that created this fl */
			int conf_dup_group;
			int suite_dup_group;
			char oldkey[MAX_CRYPT_LEN+1];/* to prevent dup INCR */
		    	char *internal_use1; /* for growth */
		    	char *internal_use2; /* for vendor user */
			} FEATURE_LIST;

#define LM_HOSTID_REQUESTED -9999
#define LM_LINGERED_HANDLE_VALUE	-2147483647


extern long ls_currtime;

lm_extern FEATURE_LIST *ls_get_next_feature(FEATURE_LIST *current);

#endif 
