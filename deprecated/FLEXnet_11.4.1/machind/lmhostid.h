/*****************************************************************************

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

/**   @file lmhostid.h
 *    @brief host id definitions

 *
  ************************************************************/

/* Currently used HOSTIDs  */
#define HOSTID_DEFAULT -1		    /* for lc_hostid() */
#define HOSTID_LONG 1			    /* Longword hostid, eg, SUN */
#define HOSTID_ETHER 2			    /* Ethernet address, eg, VAX */
#define HOSTID_ANY 3			    /* Any hostid; used for ISV testing and development */
#define HOSTID_USER 4			    /* Username HOSTID=USER=xxx */
#define HOSTID_DISPLAY 5		    /* Display */
#define HOSTID_HOSTNAME 6		    /* Node name HOSTID=HOSTNAME=xxx */
#define HOSTID_ID_STRING 9          /* HOSTID=ID_STRING=xxx, equivalent to HOSTID_STRING */
#define HOSTID_DISK_SERIAL_NUM 11	/* Windows, and NT disk serial number */
#define HOSTID_INTERNET 12		    /* ###.###.###.### */
#define HOSTID_DEMO 13              /* HOSTID=DEMO for ISVs to use in their application */
#define HOSTID_ID 18                /* HOSTID=ID=#, equivalent to HOSTID_SERNUM_ID */
#define HOSTID_FLEXID6 23           /* FLEXID6 */
#define HOSTID_FLEXID7 10           /* FLEXID7 */
#define HOSTID_FLEXID8 14           /* FLEXID8 */
#define HOSTID_FLEXID9 15           /* FLEXID9 */

/* Not currently used, but reserved for future use */
#define HOSTID_FLEXID4_KEY 16
#define HOSTID_FLEXID5_KEY 17
#define HOSTID_DOMAIN 19

/* Backward Compatible HOSTIDs */
#define HOSTID_FLEXID6_KEY 23		/* HOSTID_FLEXID6 */
#define HOSTID_FLEXID1_KEY 10		/* HOSTID_FLEXID7 */
#define HOSTID_FLEXID2_KEY 14		/* HOSTID_FLEXID8 */
#define HOSTID_FLEXID3_KEY 15		/* HOSTID_FLEXID9 */
#define HOSTID_STRING 9			    /* string ID  MAX HOSTID_LEN */
#define HOSTID_SERNUM_ID 18

/* Obsolete HOSTIDs */
#define HOSTID_INTEL32 24		/* 32-bit */
#define HOSTID_INTEL64 25		/* 64-bit */
#define HOSTID_INTEL96 26		/* 96-bit */
#define HOSTID_PC_CPU 27
#define HOSTID_ID_MODULE 8		/* hp300 id-module */
/* #define	NOHOSTID 0 -- removed v5 -- now NULL pointer */

#define HOSTID_CPU 28                   /* not used */
#define HOSTID_DISK_GEOMETRY 29         /* not used */
#define HOSTID_BIOS 30                  /* not used */
#define HOSTID_COMPOSITE 31             /* Composite HostID */
#define HOSTID_HWETHERNET		32			/* Ethernet address in hardware. */

/*
 *	HOSTID's 50-100 reserved for future FLEXIDs
 */
#define HOSTID_FLEXID_FILE_KEY 50		/* for testing purposes */

#define LM_IS_FLEXID(x) (\
			(x == HOSTID_FLEXID1_KEY) || \
		((x >= HOSTID_FLEXID2_KEY) && (x <= HOSTID_FLEXID5_KEY)) ||\
			(x == HOSTID_FLEXID6_KEY) || \
		((x >= 50) && (x <= 100)) \
		)
#define HOSTID_VENDOR 1000		/* Start Vendor-defined types here */
