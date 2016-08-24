#include <signal.h>
#include <wait.h>
#include <rpc/xdr.h>
#include <stdio.h>

#define ogdi_IXDR_PUT_LONG(buf, v) { \
	    long ZF = ((long)IXDR_PUT_INT32(buf, (long)(v))); \
	    ZF = ZF; \
	    } 

#define ogdi_IXDR_PUT_U_LONG(buf, v)  	ogdi_IXDR_PUT_LONG(buf, (long)(v))




// Hook fread/fwrite/fgets/system. Check returns for errors and print tham verbose if any.

#define ogdi_read(p,s,fp)   { \
        unsigned int ZF = read(p,s,fp); \
            if (ZF == -1 ) \
                printf("Error: read error\n"); \
                               }

#define ogdi_fread(p,s,n,fp)   { \
        unsigned int ZF = fread(p,s,n,fp); \
            if (ZF != (unsigned) (n)) \
                printf("Error: fread found %d bytes, not %d at %d\n", ZF, (int)(n), (int) ftell(fp)); \
                               }

#define ogdi_fwrite(p,s,n,fp) { \
	unsigned int ZF = fwrite(p,s,n,fp); \
            if (ZF != (unsigned) (n)) \
                printf("Error: fwrite wrote %d bytes, not %d at %d\n", ZF, (int)(n), (int) ftell(fp)); \
                              }

#define ogdi_fgets(p,n,fp) { \
        void *ZF = fgets(p,n,fp); \
             if (ZF == NULL) \
                printf("Error: fgets seek error at %d byte\n",  (int)(n)); \
                              }

#define ogdi_system(fp) { \
	int ZF = system(fp); \
           if (WIFSIGNALED(ZF) && \
              (WTERMSIG(ZF) == SIGINT || WTERMSIG(ZF) == SIGQUIT)) \
	            printf("Error: shell not found or command error.\n"); \
                              }

