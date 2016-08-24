/******************************************************************************
 *
 * Component: OGDI gltpd Server
 * Purpose: GLTPD Mainline.
 * 
 ******************************************************************************
 * Copyright (C) 1995 Logiciels et Applications Scientifiques (L.A.S.) Inc
 * Permission to use, copy, modify and distribute this software and
 * its documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies, that
 * both the copyright notice and this permission notice appear in
 * supporting documentation, and that the name of L.A.S. Inc not be used 
 * in advertising or publicity pertaining to distribution of the software 
 * without specific, written prior permission. L.A.S. Inc. makes no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 ******************************************************************************
 *
 * $Log: asyncsvr.c,v $
 * Revision 1.7  2016/06/28 14:32:45  erouault
 * Fix all warnings about unused variables raised by GCC 4.8
 *
 * Revision 1.6  2007/02/12 21:01:48  cbalint
 *      Fix win32 target. It build and works now. (tested with VC6)
 *
 * Revision 1.5  2007/02/12 16:09:06  cbalint
 *   *  Add hook macros for all GNU systems, hook fread,fwrite,read,fgets.
 *   *  Handle errors in those macro, if there are any.
 *   *  Fix some includes for GNU systems.
 *   *  Reduce remaining warnings, now we got zero warnings with GCC.
 *
 *  Modified Files:
 *  	config/unix.mak contrib/ogdi_import/dbfopen.c
 *  	contrib/ogdi_import/shapefil.h contrib/ogdi_import/shpopen.c
 *  	ogdi/c-api/ecs_xdr.c ogdi/c-api/ecsinfo.c ogdi/c-api/server.c
 *  	ogdi/datum_driver/canada/nadconv.c ogdi/driver/adrg/adrg.c
 *  	ogdi/driver/adrg/adrg.h ogdi/driver/adrg/object.c
 *  	ogdi/driver/adrg/utils.c ogdi/driver/rpf/rpf.h
 *  	ogdi/driver/rpf/utils.c ogdi/gltpd/asyncsvr.c
 *  	ogdi/glutil/iofile.c vpflib/vpfprim.c vpflib/vpfspx.c
 *  	vpflib/vpftable.c vpflib/vpftidx.c vpflib/xvt.h
 *
 * Revision 1.4  2002/02/21 16:38:19  warmerda
 * undefine svc_fdset if defined - helps avoid odd library requirements on linux
 *
 * Revision 1.3  2001/04/09 15:04:35  warmerda
 * applied new source headers
 *
 */

#include "ecs.h"

#include <ogdi_macro.h>

ECS_CVSID("$Id: asyncsvr.c,v 1.7 2016/06/28 14:32:45 erouault Exp $");

#ifdef _WINDOWS
#  include "rpc/pmap_cln.h"
#  include "time.h"
#else
#  include <sys/wait.h>
#  include "time.h"
#endif

#include <unistd.h>

#ifdef _WINDOWS
#  include <errno.h>
#else
#  include <sys/errno.h>
#endif

#ifdef HAVE_STD_RPC_INCLUDES
#  include <rpc/pmap_clnt.h>
#endif

#ifdef svc_fdset
#undef svc_fdset
#endif

#define COMTIMEOUT 900

long timecount;

static void dispatchno_1();
extern void ecsprog_1();
u_long newprogramno;
static char str1[255];
static char *argv0;
static void gltpd_svc_run();



FILE *gltpdstate = NULL;

int main(argc,argv)
     int argc;
     char **argv;
{
  SVCXPRT *transp;
  int num;
  int isDispatch = TRUE;
  char *debug;

#ifdef _WINDOWS
  rpc_nt_init();
#endif  

  debug = getenv("GLTPDLOGFILE");

  if (debug) {
    gltpdstate = (FILE *) ".";
    if (gltpdstate) {
      printf("%d: Start in the main\n",getpid());
      for(num=0;num<argc;num++) {
	printf("%s ",argv[num]);
      }
      printf("\n");
    }
  }

  argv0 = argv[0];

  /* Analyser la requete */

  switch(argc) {
  case 2:
    sscanf(argv[1],"%s",str1);    
    if (strncmp(str1,"-d",2) == 0) {
      isDispatch = TRUE;
    } else {
      isDispatch = FALSE;
      sscanf(argv[1],"%d",&num);
    }
    break;
  case 1:
    isDispatch = TRUE;
    strcpy(str1,"");
    break;
  default:
    printf("Wrong number of arguments\n");
    exit(0);
    break;
  };


  /* Si un argument existe pour ce serveur, creer directement
     ce serveur sans passer par le dispatcher */

  /* Creer le serveur directement */   
  
  if (isDispatch) {
    /* Passer par le dispatcher normalement */

    if (debug && gltpdstate) {
      printf("%d: Start dispatcher: Call pmap_unset\n",getpid());
    }

    (void)pmap_unset(ECSPROG, ECSVERS);

    if (debug && gltpdstate) {
      printf("%d: Start dispatcher: Call svctcp_create\n",getpid());
    }
    
    transp = svctcp_create(RPC_ANYSOCK, 0, 0);
    if (transp == NULL) {
      (void)fprintf(stderr, "cannot create tcp service.\n");
      if (debug && gltpdstate) {
	printf("%d: Start dispatcher: cannot create tcp service.\n",getpid());
      }    
      return 0;
    }

    if (debug && gltpdstate) {
      printf("%d: Start dispatcher: Call svc_register\n",getpid());
    }
    
    if (!svc_register(transp, ECSPROG, ECSVERS,
		      dispatchno_1, IPPROTO_TCP)) {
      (void)fprintf(stderr, "unable to register (DISPATCHNO, DISPATCHVERS, tcp).\n");
      if (debug && gltpdstate) {
	printf("%d: Start dispatcher: unable to register the dispatcher.\n",
               getpid());
      }    
      return 0;
    }
    if (debug && gltpdstate) {
      printf("%d: Start dispatcher: Call svc_run.\n",getpid());
    }    
    svc_run();
    (void)fprintf(stderr, "svc_run returned\n");
  } else {
    if (debug && gltpdstate) {
      printf("%d: Start server: Call pmap_unset at rpcaddress %d version %d\n",getpid(),num,(int) ECSVERS);
    }

    (void)pmap_unset(num, ECSVERS);
    
    /* Enregistrer le nouveau serveur */
    if (debug && gltpdstate) {
      printf("%d: Start server: Call svctcp_create\n",getpid());
    }

    transp = svctcp_create(RPC_ANYSOCK, 0, 0);
    if (transp == NULL) {
      (void)fprintf(stderr, "cannot create tcp service.\n");
      return 0;
    }
    
    if (debug && gltpdstate) {
      printf("%d: Start server: Call svc_register\n",getpid());
    }

    if (!svc_register(transp, num, ECSVERS, ecsprog_1, IPPROTO_TCP)) {
      (void)fprintf(stderr, "unable to register (ECSPROG, ECSVERS, tcp).\n");
      if (debug && gltpdstate) {
	printf("%d: Start server: unable to register the dispatcher.\n",getpid());
      }    
      exit(1);
    }
    if (debug && gltpdstate) {
      printf("%d: Start server: Call svc_run.\n",getpid());
    }    

    gltpd_svc_run();


  }
  
#ifdef _WINDOWS
  rpc_nt_exit();
#endif  

  return 1;
}

static void
dispatchno_1(rqstp, transp)
     struct svc_req *rqstp;
     SVCXPRT *transp;
{
#ifdef _WINDOWS
  STARTUPINFO  si;
  SECURITY_ATTRIBUTES saProcess, saThread;
  PROCESS_INFORMATION piProcessB;
  bool_t result;
#endif
  char temp[256];
  char *debug;
  union {
    int fill;
  } argument;
  SVCXPRT *newtransp = NULL;

  debug = getenv("GLTPDLOGFILE");

  if (debug) {
    gltpdstate = (FILE *) (void *) 1;
    if (gltpdstate) {
      printf("%d: Start in the dispatcher function\n",getpid());
    }
  }

  switch (rqstp->rq_proc) {
  case NULLPROC:
    if (debug && gltpdstate) {
      printf("%d: Dispatcher function: NULLPROC case\n",getpid());
    }

    (void)svc_sendreply(transp, (xdrproc_t) xdr_void, (char *)NULL);
    return;
    
  case 1:
    /* Dispatcher de service */
    if (debug && gltpdstate) {
      printf("%d: Dispatcher function: Call the dispatcher request\n",getpid());
    }
 
    memset((char *)&argument,0, sizeof(argument));
    if (!svc_getargs(transp, (xdrproc_t) xdr_void, (char *) &argument)) {
      svcerr_decode(transp);
      return;
    }
    
#ifndef _WINDOWS
    /* Reap zombie children */
    /*    while (waitpid(-1, NULL, WNOHANG) > 0);
    if (fork()) {
      return;
    }*/
#endif    

    /* Trouver le numero du nouveau serveur
     * Note: On Windows NT, this really should not be created here because
     * the child creates its own socket and registers it.  Leave it for now.
     */

    if (debug && gltpdstate) {
      printf("%d: Dispatcher function: Call the svctcp_create function\n",getpid());
    }
    
    newtransp = svctcp_create(RPC_ANYSOCK, 0, 0);
    
    /* Trouver dynamiquement un numero d'identifiant */
    
    newprogramno = ECS_TRANSIENT_MIN;
    while( !pmap_set(newprogramno, ECSVERS, IPPROTO_TCP, newtransp->xp_port)) {
      newprogramno++;
      if (newprogramno == ECS_TRANSIENT_MAX) {
	newprogramno = ECS_TRANSIENT_MIN;
      }
    }

    svc_destroy(newtransp);
    
    if (debug && gltpdstate) {
      printf("%d: Dispatcher function: RPC number: %ld\n",
             getpid(), newprogramno);
    }

#ifdef _WINDOWS
    /* Cas Windows avec CreateProcess */

    ZeroMemory(&si,sizeof(si));
    si.cb = sizeof(si);

    saProcess.nLength = sizeof(saProcess);
    saProcess.lpSecurityDescriptor =NULL;
    saProcess.bInheritHandle=TRUE;
    
    saThread.nLength = sizeof(saThread);
    saThread.lpSecurityDescriptor =NULL;
     saThread.bInheritHandle=FALSE;

    /* spawn process */

    if (strncmp(str1,"-d",2) == 0) {

      if (debug && gltpdstate) {
	printf("%d: Dispatcher function: Call the svc_sendreply function\n",getpid());
      }
      
      if (!svc_sendreply(transp, (xdrproc_t) xdr_u_long, (char *) &newprogramno)) {
	svcerr_systemerr(transp);
	printf("erreur reply\n");
      }
      if (!svc_freeargs(transp, xdr_void, &argument)) {
	(void)fprintf(stderr, "unable to free arguments\n");
	exit(1);
      }
      
      /* Enregistrer le nouveau serveur */

      if (debug && gltpdstate) {
	printf("%d: Dispatcher function: Call svc_register\n",getpid());
      }
      
      if (!svc_register(newtransp, newprogramno, ECSVERS, ecsprog_1, 0)) {
	if (debug && gltpdstate) {
	  printf("%d: Dispatcher function: unable to register the server\n",getpid());
	}

	(void)fprintf(stderr, "unable to register (ECSPROG, ECSVERS, tcp).\n");
	exit(1);
      }

      if (debug && gltpdstate) {
	printf("%d: Dispatcher function: unable to register the server\n",getpid());
      }

      gltpd_svc_run();

      exit(1);
    }

    sprintf(temp,"%s %d",argv0, newprogramno); 
    if (debug && gltpdstate) {
      printf("%d: Dispatcher function: Call CreateProcess\n",getpid());
    }
    
    result = CreateProcess (argv0,  
			    temp,		/* command line */
			    &saProcess, &saThread,
			    FALSE,
			    DETACHED_PROCESS, 
			    NULL, NULL,&si,
			    &piProcessB);
    if (result) {
      CloseHandle(piProcessB.hProcess);
      CloseHandle(piProcessB.hThread);
    } else {
      if (debug && gltpdstate) {
	printf("%d: Dispatcher function: Error occur during CreateProcess %s\n",getpid(),temp);
      }
      return;
    }
    /* Rtourner le resultat au client, mais auparavent on laisse le
       temps au serveur de s'intialiser */
    Sleep(3000);

    if (debug && gltpdstate) {
      printf("%d: Dispatcher function: Call sendreply\n",getpid());
    }

    if (!svc_sendreply(transp, (xdrproc_t) xdr_u_long, (char *) &newprogramno)) {
      svcerr_systemerr(transp);
      printf("erreur reply\n");
    }
    if (!svc_freeargs(transp, xdr_void, &argument)) {
      (void)fprintf(stderr, "unable to free arguments\n");
      exit(1);
    }



#else

    /*

     Cas UNIX avec fork 

    if (debug && gltpdstate) {
      printf("%d: Dispatcher function: Call sendreply\n",getpid());
      fclose(gltpdstate);
    }

    if (!svc_sendreply(transp, (xdrproc_t) xdr_u_long, (char *) &newprogramno)) {
      svcerr_systemerr(transp);
      printf("erreur reply\n");
    }
    if (!svc_freeargs(transp, xdr_void, &argument)) {
      (void)fprintf(stderr, "unable to free arguments\n");
      exit(1);
    }

    if (debug && gltpdstate) {
      printf("%d: Dispatcher function: Call svc_register\n",getpid());
      fclose(gltpdstate);
    }

    if (!svc_register(newtransp, newprogramno, ECSVERS, ecsprog_1, 0)) {
      (void)fprintf(stderr, "unable to register (ECSPROG, ECSVERS, tcp).\n");
      exit(1);
    }
    
    gltpd_svc_run();
    (void)fprintf(stderr, "svc_run returned\n");
    exit(1);
    */

    sprintf(temp,"%s %ld &",argv0, newprogramno); 
    if (debug && gltpdstate) {
      printf("%d: Dispatcher function: Call CreateProcess\n",getpid());
    }
    
    /* Reap zombie children */
    while (waitpid(-1, NULL, WNOHANG) > 0);
    ogdi_system(temp);

    /* Rtourner le resultat au client, mais auparavent on laisse le
       temps au serveur de s'intialiser */
    sleep(1);

    if (debug && gltpdstate) {
      printf("%d: Dispatcher function: Call sendreply\n",getpid());
    }

    if (!svc_sendreply(transp, (xdrproc_t) xdr_u_long, (char *) &newprogramno)) {
      svcerr_systemerr(transp);
      printf("erreur reply\n");
    }
    if (!svc_freeargs(transp, (xdrproc_t) xdr_void, (char *) &argument)) {
      (void)fprintf(stderr, "unable to free arguments\n");
      exit(1);
    }


#endif
    break;
    
  default:
    svcerr_noproc(transp);
    return;
  }
}

#ifdef _WINDOWS

void gltpd_svc_run()
{
  xdrproc_t xdr_argument;
  struct timeval timeout;
#ifdef FD_SETSIZE
  fd_set readfds;
#else
  int readfds;
#endif
  
  timeout.tv_sec = COMTIMEOUT;
  timeout.tv_usec = 1;

  for (;;) {
#ifdef FD_SETSIZE
    readfds = svc_fdset;
#else
    readfds = svc_fds;
#endif
    switch (select(0, &readfds, (int *)NULL, (int *)NULL, (struct timeval *)&timeout)) {
    case -1:
      if (WSAerrno == EINTR) {
	continue;
      }
      return;
    case 0:
      perror("gltpd_svc_run: - timeout");
      return;
    default:
      svc_getreqset(&readfds);
    }
  }
}
	    
#else

void gltpd_svc_run()
{
  fd_set readfdset;
  extern int errno;
  static int tsize = 0;
  struct timeval timeout;
  xdrproc_t xdr_argument;
  /*xdrproc_t xdr_result;*/
  long currenttime;
  
  timeout.tv_sec = COMTIMEOUT;
  timeout.tv_usec = 0;

  if (!tsize) 
    tsize = getdtablesize(); 
  
  for (;;) {
    readfdset = svc_fdset;
    switch (select(tsize, &readfdset, (fd_set*) NULL, (fd_set*) NULL, 
		   (struct timeval *) &timeout)) {
    case -1:					 
      if (errno == EBADF) {
	continue;
      }	   
      if (errno == EINTR) {
	time(&currenttime);
	if (currenttime - timecount > COMTIMEOUT) {
	  xdr_argument = (xdrproc_t) xdr_void;
	  /*xdr_result = (xdrproc_t) xdr_ecs_Result;*/
	  destroyserver_1_svc(xdr_argument,NULL);
	  perror("gltpd_svc_run: - timeout");
	  return;
	}
	continue;
      }	   

      perror("gltpd_svc_run: - select failed");
      return;
    case 0:
      xdr_argument = (xdrproc_t) xdr_void;
      /*xdr_result = (xdrproc_t) xdr_ecs_Result;*/
      destroyserver_1_svc(xdr_argument,NULL);
      perror("gltpd_svc_run: - timeout");
      return;
    default:
      svc_getreqset(&readfdset);
      time(&timecount);      
    }
  }

}



#endif



