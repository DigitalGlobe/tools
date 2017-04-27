/******************************************************************************

	    Copyright (c) 1988-2007 Macrovision Europe Ltd. and/or Macrovision Corporation. All Rights Reserved.
	This software has been provided pursuant to a License Agreement
	containing restrictions on its use.  This software contains
	valuable trade secrets and proprietary information of 
	Macrovision Corporation and is protected, by law.  It may 
	not be copied or distributed in any form or medium, disclosed 
	to third parties, reverse engineered or used in any manner not 
	provided for in said License Agreement except with the prior 
	written authorization from Macrovision Corporation.

 *****************************************************************************/
/*
 *	Module:	lsserver.h v1.35.0.0
 *
 *	Description: Local definitions for license manager servers
 *
 */

#ifndef _LS_SERVER_H_
#define _LS_SERVER_H_

#include "lmclient.h"

#ifdef getenv
#undef getenv
#define getenv(x) l_getenv(lm_job, x)
#endif /* getenv */
#include <stdio.h>
#include <sys/types.h>
#ifndef PC
#include <netinet/in.h>
#endif
/*
 *	P3090:  BASE_EXIT was based on NSIG which is a number that 
 *		varies across platforms and across OS releases.  
 *		This has caused serious problems when the vendor daemon 
 *		is built with one BASE_EXIT, but lmgrd another.
 *		In particular, redundant servers fail.
 *		Therefore, we're now identifying existing platforms
 *		and hardcoding BASE_EXIT, and all new platforms should
 *		hard-code BASE_EXIT.
 */

/*
 *	BASE_EXIT must be a number higher than the signals available
 *	on the o/s, and low enough that BASE_SIG + <num> is < 255
 *	which is the highest possible exit value on unix.
 */
#ifdef SUNOS4
#define BASE_EXIT 32
#endif /* SUNOS4 */
#ifdef SUNOS5
#ifdef i386
#define BASE_EXIT 44
#else
#define BASE_EXIT 34 /* sparc */
#endif /* i386 */
#endif /* SUNOS5 */
#ifdef UNIXWARE
#define BASE_EXIT 35
#endif /* UNIXWARE */
#ifdef DEC_UNIX_3
#define BASE_EXIT 32
#endif /* ALPHA_OSF */
#ifdef DGX86
#define BASE_EXIT 65
#endif /* DGX86 */
#ifdef SGI
#define BASE_EXIT 65
#endif /* SGI */
#ifdef HP700
#ifdef HP64
#define BASE_EXIT 45
#else
#define BASE_EXIT 31
#endif /* HP64 */
#endif /* HP700 */
#ifdef PMAX
#define BASE_EXIT 32
#endif /* PMAX -- ultrix mips */
#ifdef HP300
#define BASE_EXIT 33
#endif /* HP300 */
#ifdef LINUX
#define BASE_EXIT 32
#endif /* LINUX */
#ifdef MAC10
#define BASE_EXIT 32
#endif /* LINUX */
#ifdef NEC
#define BASE_EXIT 32
#endif /* NEC */
#ifdef RS6000 /* ppc also */
#define BASE_EXIT 64
#endif /* RS6000 */
#ifdef SINIX 
#define BASE_EXIT 32
#endif /* SINIX */
#ifdef SCO 
#define BASE_EXIT 32
#endif /* SCO */
#ifdef FREEBSD
#define BASE_EXIT 32
#endif /* FREEBSD */
#ifdef BSDI
#define BASE_EXIT 32
#endif /* BSDI */
#ifdef sony_news
#define BASE_EXIT 32
#endif /* sony_news */
#ifdef TANDEM
#define BASE_EXIT 35
#endif /* sony_news */
#ifdef HPINTEL_64
#define BASE_EXIT 45
#endif /* HPINTEL_64 */

#ifdef HPINTEL_32
#define BASE_EXIT 45
#endif /* HPINTEL_32 */

#ifdef PC
#define BASE_EXIT 23
#endif /* PC */

#if defined (LYNX) || defined (LYNX_PPC) 
#define BASE_EXIT 22
#endif /* LYNX */

#ifdef cray
#define BASE_EXIT 65
#endif /* cray */

#ifndef BASE_EXIT
	ERROR -- Must define BASE_EXIT
#endif /* BASE_EXIT */


/*
 *	Error exit codes - These are used by the master to tell
 *		what went wrong with the vendor daemon
 */

#define EXIT_BADCONFIG	    BASE_EXIT + 1	/* Bad configuration data */
#define EXIT_WRONGHOST	    BASE_EXIT + 2	/* Invalid host */
#define EXIT_PORT_IN_USE    BASE_EXIT + 3	/* Internet "ADDRESS ALREADY IN USE" */
#define EXIT_NOFEATURES	    BASE_EXIT + 4	/* No features to serve */
#define EXIT_COMM	    BASE_EXIT + 5	/* Communications error */
#define EXIT_DESC	    BASE_EXIT + 6	/* Not enough descriptors to */
							/* re-create pipes */
#define EXIT_NOMASTER	    BASE_EXIT + 7	/* Couldn't find a master */
#define EXIT_SIGNAL	    BASE_EXIT + 8	/* Exited due to some signal */
#define EXIT_SERVERRUNNING  BASE_EXIT + 9	/* Exited because another server */
							    /* was running */
#define EXIT_MALLOC	    BASE_EXIT + 10	/* malloc() failure */
#define EXIT_BICKER	    BASE_EXIT + 11	/* Servers can't agree on who is */
							     /* the master */
#define EXIT_BADDAEMON	    BASE_EXIT + 12	/* DAEMON name doesn't agree between */
						/* daemon and license file */
#define EXIT_CANTEXEC	    BASE_EXIT + 13	/* Child cannot exec requested server */
#define EXIT_REQUESTED	    BASE_EXIT + 14	/* lmgrd requested vendor daemon down */
#define EXIT_EXPIRED	    BASE_EXIT + 15	/* demo version has expired */
#define EXIT_BADCALL	    BASE_EXIT + 16	/* vendor daemon started incorrectly */
#define EXIT_INCONSISTENT   BASE_EXIT + 17	/* vendor daemon consistency error */
#define EXIT_FEATSET_ERROR  BASE_EXIT + 18	/* Feature-set inconsistent */
#define EXIT_BORROW_ERROR   BASE_EXIT + 19	/* Borrow database corrupted */
#define EXIT_NO_LICENSE_FILE BASE_EXIT + 20	/* No license file */
#define EXIT_NO_SERVERS     BASE_EXIT + 20	/* Vendor keys don't support */


typedef unsigned char SERVERNUM;
#define MAX_SERVER_NUM 256		/* Maximum # of servers in a "chain" */

#ifdef MULTIPLE_VD_SPAWN
extern SERVERNUM snum;	/* Our server number */
#endif

#define TRUE 1
#define FALSE 0
#define INCLUDE_DUMP_CMD	    /* Define to include (debugging) dump command */
#define MASTER_TIMEOUT 10	    /* # of seconds to wait for master to respond */
#define TIMERSECS 60                /* Timer to go off to send data to another lmgrd */
#define FIFTEEN_MINUTES (15 * 60)
#define REPLOG_TIMESTAMP FIFTEEN_MINUTES
#define MASTER_ALIVE (2*TIMERSECS)  /* # seconds to wait for any data from */
				    /* another lmgrd - if we don't get it, 
				      declare the other lmgrd down */

#define CLIENT_FD   0
#define CLIENT_SOCK 1
typedef int HANDLE_T;

#ifdef SUPPORT_FIFO
typedef struct _ls_local_addr {
	int server_write; /* write fd */
	char *name;	/* there are actually two files: with
				 * "cr" or "cw" appended for client read
				 * or client write -- destroyer of
				 * CLIENT_ADDR is responsible for freeing
				 * malloc's memory.  This is the name
				 * that's sent in the original message from
				 * client.
				 */
} LS_LOCAL_DATA;
#endif

typedef union _addr_id  {
			LM_SOCKET	fd;		/* if is_fd */
			struct sockaddr_in sin; 	/* if LM_UDP */
		} ADDR_ID;
	
typedef struct _client_addr {
		ADDR_ID addr; 		/* enough to uniquely identify */
		int is_fd; 		/*boolean -- TRUE if fd address*/
		int transport;          /*LM_TCP, LM_LOCAL*/
#ifdef SUPPORT_FIFO
		LS_LOCAL_DATA local;
#endif
	} CLIENT_ADDR;

#define MAX_CLIENT_HANDLE sizeof(HANDLE_T) /*(32 bit int)*/

typedef struct _client_group {
	struct _client_group *next;	/* Forward link */
	char *	name;	/* Group name */
	char *	list;	/* List for INCLUDE, EXCLUDE */
} CLIENT_GROUP;


typedef struct client_data {
		CLIENT_ADDR addr;		/* fd / sockaddr*/
		HANDLE_T handle;
		char name[MAX_LONGNAME_SIZE + 1];	/* Username */
		char node[MAX_LONGNAME_SIZE + 1];	/* Node */
		char display[MAX_LONGNAME_SIZE + 1];	/* Display */
		char vendor_def[MAX_VENDOR_CHECKOUT_DATA + 1]; /* Changes with
								each checkout */
		short use_vendor_def;		/* Use vendor_def data */
		short inet_addr[4];		/* Internet addr - deprecated */
		long time;	/* Time started */
		char platform[MAX_PLATFORM_NAME + 1];	/* e.g., sun4_u4 */
		unsigned int lastcomm;	/* Last communications 
					 * received (or heartbeat sent) 
					 * if -1, marked for later deletion
					 */
#define	LS_DELETE_THIS_CLIENT 0xffffffff
		struct _FlexCrypt *encryption; /* Comm encryption code */
		int comm_version; /* Client's communication version */
		int comm_revision; /* Client's communication revision */
		int tcp_timeout;	
		long pid;		/* client's pid */
		HOSTID *hostids;       /* null-terminated array of
					 * client's hostids */
		char *project;		/* $LM_PROJECT for reportlog */
		short capacity;		/* Multiplier for checkout cnt */
		unsigned short flexlm_ver; /* FLEXlm ver (>= v5) */
		unsigned short flexlm_rev; /* FLEXlm revision (>= v5) */
		int curr_cpu_usage[4];	/* Usage numbers */
		int last_logged_cpu_usage[4]; /* Last logged Usage numbers */
#define CPU_USAGE_DEFAULT_DELTA 3 		/* 10ths/sec -- .3 sec */
#define CPU_USAGE_CHECKIN_ONLY	0xffffff	/* no usage with heartbeats */
#define CPU_USAGE_DEFAULT_INTERVAL CPU_USAGE_CHECKIN_ONLY
		CLIENT_GROUP *groups;			/* groups this user */
		CLIENT_GROUP *hostgroups ;		/*   belongs to */
		CLIENT_GROUP *inetgroups ;		
		long flags;
#define CLIENT_FLAG_BADDATE_DETECTED 	0x1
#define CLIENT_FLAG_BORROWER 		0x2
#define CLIENT_FLAG_NOMORE 		0x4
#define CLIENT_FLAG_INTERNAL1 		0x8
#define CLIENT_FLAG_INTERNAL2	       0x10
#define CLIENT_FLAG_INTERNAL3	       0x20
#define CLIENT_FLAG_INTERNAL4          0x40
#define CLIENT_FLAG_LINGER             0x80
#define CLIENT_FLAG_BORROW_INIT			0x00000100	/* Used to indicate that client was created for sole purpose
														of restoring borrow/linger info */
		long internal1;		
		unsigned short ckout_sernum;
		unsigned long group_id;	
#define NO_GROUP_ID 0xffffffff
		char *sn;
		unsigned int borrow_seconds;
		struct _vendor_info *vendorInfo;
		struct _commBuffer *commBufRead;
		int err;
		int featureCount;
		/* private data for client data. size will change across
		 * releases */
		char internalData[52];
	} CLIENT_DATA;


typedef struct lm_quorum {
				int count;      /* How many active */
				int quorum;     /* How many required */
				int master;	/* Index into list of master */
				int n_order;	/* How many order messages we
						   need before we start */
				int alpha_order; /* Use alphabetical master
						   selection algorithm */
				LM_SERVER *list; /* Who they are */
				int debug;	/* Debug flag */
			  } LM_QUORUM;


typedef struct ls_pooled {
			struct ls_pooled *next;
			char date[DATE_LEN+1]; 
			char code[MAX_CRYPT_LEN+1];
			char *lc_vendor_def;
			LM_CHAR_PTR lc_dist_info;
			LM_CHAR_PTR lc_user_info;
			LM_CHAR_PTR lc_asset_info;      
			LM_CHAR_PTR lc_issuer;
			LM_CHAR_PTR lc_notice;  
			LM_CHAR_PTR parent_featname;
			LM_CHAR_PTR parent_featkey;
			char *lc_sign;
			int users; 	/* How many expire at that date */
} LS_POOLED;
			
/*
 *	States involved in connecting to other servers (bitmap)
 */
#define C_NONE 0	/* Not started */
#define C_SOCK 1	/* Socket to other server connected */
#define C_SENT 2	/* (fd1) Socket connected, Msg sent to other master */
#define C_CONNECT1 4	/* (fd1) Connection to other server complete */
#define C_CONNECT2 8	/* (fd2) Other server has established connection */
#define C_CONNECTED 16	/* Completely connected */
#define C_MASTER_READY 32 /* Connection complete, master elected and ready */
#define C_TIMEDOUT 64	/* Connection timed out - don't retry */
#define C_NOHOST 128	/* This host doesn't exist */

/*
 *	Output to error log file
 *
 *	The LOG_INFO macros are used to generate the documentation
 *	on all the LOG calls.  They are always ignored, as far as
 *	the code is concerned.
 */

#define LOGERR(x) \
	{\
		ls_log_prefix(LL_LOGTO_BOTH, LL_ERROR); \
		ls_log_error x;\
	}
#define _LOGERR(x) ls_log_error x ;
#ifndef RELEASE_VERSION
#define LOG(x) {ls_log_prefix(LL_LOGTO_ASCII, 0);(void) ls_log_asc_printf x; }
#else
#define LOG(x) {ls_log_prefix(LL_LOGTO_ASCII, 0); (void) ls_log_asc_printf x;}
#endif
#define LOG_INFO(x)

/*
 *	If LOG_TIME_AT_START is defined, all daemon logging is
 *	done with timestamps at the beginning of the lines, rather
 *	than just certain (important) lines being timestamped at the
 *	end.  LOG_TIME_AT_START became the default in v1.3.
 */
#define LOG_TIME_AT_START	/* Log time @ start of log lines */

/* 
 *	Default directory for lock file and lmgrd info file
 */

#ifdef WINNT 
#define LS_LOCKDIR "C:\\flexlm"
#else
#if defined(BSDI) || defined (MONTAVISTA)
#define LS_LOCKDIR "/tmp/.flexlm"
#else
#ifdef MAC10
#define LS_LOCKDIR "/var/tmp/.flexlm"
#else
#define LS_LOCKDIR "/usr/tmp/.flexlm"
#endif /* MAC10 */
#endif /* BSDI */
#endif /* WINNT */

/*
 *	Output to log without header (for building strings)
 */
#define _LOG(x) { (void) ls_log_asc_printf x; }


extern int dlog_on;
extern int _ls_fifo_fd;		/* Global fifo fd for reading */
extern LM_HANDLE *lm_job;	/* Server's one (and only) job */

#ifndef RELEASE_VERSION
#define DLOG(x) { if (dlog_on) \
	  {ls_log_prefix(LL_LOGTO_ASCII, -1); ls_log_file_line(LL_LOGTO_ASCII,__FILE__,__LINE__); (void) ls_log_asc_printf x; } }
#else
#define DLOG(x) { if (dlog_on) \
	  { ls_log_prefix(LL_LOGTO_ASCII, -1); (void) ls_log_asc_printf x; } }
#endif 
/*
 *	Output to log without header (for building strings)
 */
#define _DLOG(x) { if (dlog_on) { (void) ls_log_asc_printf x; } }


/*
 *	Exchange descriptors so that parent-child communication is happy
 */

#define XCHANGE(_p) { int _t = _p[3]; _p[3] = _p[1]; _p[1] = _t; }

/*
 *	Server's malloc - simply logs an error if malloc fails, and exits
 */

extern char *ls_malloc();
#define LS_MALLOC(x)	ls_malloc((x), __LINE__, __FILE__)

/*
 *	listen() backlog - number of pending connections
 */
/*  bumped backlog from 50 to 500. this is a partial 
 * fix for IOA-00009837 */
#define LISTEN_BACKLOG 500
/*
 *	Get the time 
 */
/* 
 *	Sun/Vax uses _TIME_ to detect <sys/time.h>, Apollo uses the 
 *	same include file for both <time.h> and <sys/time.h>
 */

#ifndef ITIMER_REAL	
#include <time.h>
#endif
#ifdef THREAD_SAFE_TIME
struct tm *ls_gettime(struct tm * ptst);
#else /* !THREAD_SAFE_TIME */
struct tm *ls_gettime();
#endif

/*************************************************************
 * More externs 
 **************************************************************/
extern CLIENT_GROUP *groups;
extern CLIENT_GROUP *hostgroups;


typedef void (LM_CALLBACK_TYPE * LS_CB_USERDOWN) (CLIENT_DATA *);
#define LS_CB_USERDOWN_TYPE (LS_CB_USERDOWN)

/*
 * write a error message to the debug log
 */
lm_extern void ls_log_message(char *message);
lm_extern int ls_set_log_vendor_enable(char * string, int enable);


#endif 	/* _LS_SERVER_H_ */
