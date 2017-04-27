
#include "ecs.h"
#ifdef _WINDOWS
#include "rpc/pmap_cln.h"
#endif
#include <unistd.h>
#include <errno.h>

#define COMTIMEOUT 900

static void dispatchno_1();
extern void ecsprog_1();
u_long newprogramno;
static char str1[255];

void per_svc_run();

int main(argc,argv)
     int argc;
     char **argv;
{
  SVCXPRT *transp;
  int num;
  int isDispatch = TRUE;

#ifdef _WINDOWS
  rpc_nt_init();
#endif  

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
    isDispatch == TRUE;
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

    (void)pmap_unset(ECSPROG, ECSVERS);
    
    transp = svctcp_create(RPC_ANYSOCK, 0, 0);
    if (transp == NULL) {
      (void)fprintf(stderr, "cannot create tcp service.\n");
      return 0;
    }
    if (!svc_register(transp, ECSPROG, ECSVERS,
		      dispatchno_1, IPPROTO_TCP)) {
      (void)fprintf(stderr, 
		    "unable to register (DISPATCHNO, DISPATCHVERS, tcp).\n");
      return 0;
    }
    svc_run();
    (void)fprintf(stderr, "svc_run returned\n");
  } else {

    (void)pmap_unset(num, ECSVERS);
    
    /* Enregistrer le nouveau serveur */
    transp = svctcp_create(RPC_ANYSOCK, 0, 0);
    if (transp == NULL) {
      (void)fprintf(stderr, "cannot create tcp service.\n");
      return 0;
    }
    
    if (!svc_register(transp, num, ECSVERS, ecsprog_1, IPPROTO_TCP)) {
      (void)fprintf(stderr, "unable to register (ECSPROG, ECSVERS, tcp).\n");
      exit(1);
    }

    per_svc_run();
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
#endif
  char temp[256];
  bool_t result;
  union {
    int fill;
  } argument;
  SVCXPRT *newtransp;

  switch (rqstp->rq_proc) {
  case NULLPROC:
    (void)svc_sendreply(transp, (xdrproc_t) xdr_void, (char *)NULL);
    return;
    
  case 1:
    /* Dispatcher de service */
 
    memset((char *)&argument,0, sizeof(argument));
    if (!svc_getargs(transp, xdr_void, &argument)) {
      svcerr_decode(transp);
      return;
    }

#ifndef _WINDOWS    
    if(fork()) return; 
#endif

    /* Trouver le numero du nouveau serveur */

    newtransp = svctcp_create(RPC_ANYSOCK, 0, 0);
    /* Trouver dynamiquement un numero d'identifiant */

    newprogramno = ECS_TRANSIENT_MIN;
    while( !pmap_set( newprogramno, ECSVERS, IPPROTO_TCP, newtransp->xp_port)) {
      newprogramno = ECS_TRANSIENT_MIN + ( newprogramno + 1 )
	% (ECS_TRANSIENT_MAX - ECS_TRANSIENT_MIN + 1);
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

    if (strncmp(str1,"-d",2) != 0) {
      sprintf(temp,"gltpd %d",newprogramno); 
      result = CreateProcess ("gltpd.exe",  
			      temp,		/* command line */
			      &saProcess, &saThread,
			      FALSE,
			      DETACHED_PROCESS, 
			      NULL, NULL,&si,
			      &piProcessB);                   
    } else {
      if (!svc_sendreply(transp, (xdrproc_t) xdr_u_long, (char *) &newprogramno)) {
	svcerr_systemerr(transp);
	printf("erreur reply\n");
      }
      if (!svc_freeargs(transp, xdr_void, &argument)) {
	(void)fprintf(stderr, "unable to free arguments\n");
	exit(1);
      }
      
      /* Enregistrer le nouveau serveur */
      
      if (!svc_register(newtransp, newprogramno, ECSVERS, ecsprog_1, 0)) {
	(void)fprintf(stderr, "unable to register (ECSPROG, ECSVERS, tcp).\n");
	exit(1);
      }
      
      per_svc_run();

      exit(1);
      
    }

    if (result) {
      CloseHandle(piProcessB.hProcess);
      CloseHandle(piProcessB.hThread);
    } else {
      return;
    }
    
    /* Retourner le resultat au client, mais auparavent on laisse le
	temps au serveur de s'intialiser */

    Sleep(1000);

    if (!svc_sendreply(transp, (xdrproc_t) xdr_u_long, (char *) &newprogramno)) {
      svcerr_systemerr(transp);
      printf("erreur reply\n");
    }
    if (!svc_freeargs(transp, xdr_void, &argument)) {
      (void)fprintf(stderr, "unable to free arguments\n");
      exit(1);
    }

#else
    /* Cas UNIX avec fork */
   
    /* Retourner le resultat au client */

    if (!svc_sendreply(transp, (xdrproc_t) xdr_u_long, (char *) &newprogramno)) {
      svcerr_systemerr(transp);
      printf("erreur reply\n");
    }
    if (!svc_freeargs(transp, xdr_void, &argument)) {
      (void)fprintf(stderr, "unable to free arguments\n");
      exit(1);
    }

    /* Enregistrer le nouveau serveur */

    if (!svc_register(newtransp, newprogramno, ECSVERS, ecsprog_1, 0)) {
      (void)fprintf(stderr, "unable to register (ECSPROG, ECSVERS, tcp).\n");
      exit(1);
    }

    per_svc_run();
    (void)fprintf(stderr, "svc_run returned\n");
    exit(1);
#endif
    break;
    
  default:
    svcerr_noproc(transp);
    return;
  }
}



#ifdef _WINDOWS

void per_svc_run()
{
  svc_run();
/*  struct timeval timeout;
  fd_set readfds;
  Message *retour;

  timeout.tv_sec = COMTIMEOUT;
  timeout.tv_usec = 0;
  
  for (;;) {

    readfds = svc_fdset;

    switch (select(0, &readfds, 
		   (struct fd_set *) 0, (struct fd_set *)0, 
		   (struct timeval *) &timeout)) {
    case -1:
      if (WSAerrno == EINTR) {
	continue;
      }
      perror("svc_run: - select failed");
      return;
    case 0:
      retour = disconnecthost_1();
      perror("per_svc_run: - timeout");
      return;
    default:
      svc_getreqset(&readfds);
    }
  }
*/
}

#else

void per_svc_run()
{
  svc_run();

/*  fd_set readfdset;
  extern int errno;
  static int tsize = 0;
  struct timeval timeout;
  Message *retour;
  
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
      perror("svc_run: - select failed");
      return;
    case 0:
      retour = disconnecthost_1();
      perror("per_svc_run: - timeout");
      return;
    default:
      svc_getreqset(&readfdset);
    }
  }
*/
}



#endif



