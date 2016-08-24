#ifndef H_MUSE_IPC

#define H_MUSE_IPC

/*
 *	Function prototypes
 */

#if XVT_OS == XVT_OS_MAC
/***************************************************************
@   my_event_hook(int dlg_id, BOOLEAN is_modal, EventRecord *my_event);
****************************************************************
*/

#if XVT_CC_PROTO
BOOLEAN my_event_hook(WINDOW dlg_id, BOOLEAN is_modal, EventRecord *my_event);
#else
BOOLEAN my_event_hook();
#endif

/***************************************************************
@   xvtmi_init(void)
****************************************************************
*/
void
#if XVT_CC_PROTO
	xvtmi_init(void);
#else
	xvtmi_init();
#endif

#if XVT_CC_PROTO
VOID get_message(int task_id);
#else
VOID get_message( );
#endif

#if XVT_CC_PROTO
VOID look_for_message(int task_id);
#else
VOID look_for_message( );
#endif

#endif

/* end of mac specific prototypes */


/*
 *	The communications buffer size
 */
#define COMM_BUFFER_SIZE 4096
#define COMM_DATA_SIZE   (COMM_BUFFER_SIZE-4)

/*
 *	Function prototypes
 */

#if XVT_CC_PROTO
int  muse_ipc_spawn        (char *program);
VOID muse_ipc_send         (int task_id, int status);
VOID muse_ipc_command      (int task_id, int command);
int  muse_ipc_receive      (int task_id);
int  muse_ipc_peek	   (int task_id);
VOID muse_ipc_init_child   (int *argc, char **argv);

VOID muse_ipc_call_back    (int task_id, VOID (*f)(int) );

int  muse_ipc_setup_client (char *host, char *program, int port);
int  muse_ipc_setup_server (int port);

int  muse_ipc_echo (VOID);

VOID muse_ipc_close (int taskid);

VOID muse_ipc_init_send(int task_id);

VOID muse_ipc_add_string  (int task_id, char *s);
VOID muse_ipc_add_bytes   (int task_id, VOID *b, int n);
VOID muse_ipc_add_char    (int task_id, int c);
VOID muse_ipc_add_short   (int task_id, int n);
VOID muse_ipc_add_int     (int task_id, int n);
VOID muse_ipc_add_long    (int task_id, int32 n);
VOID muse_ipc_add_float   (int task_id, double n);
VOID muse_ipc_add_double  (int task_id, double n);
VOID muse_ipc_add_shorts  (int task_id, int n, short *array );
VOID muse_ipc_add_ints    (int task_id, int n, int *array );
VOID muse_ipc_add_longs   (int task_id, int n, int32 *array );
VOID muse_ipc_add_floats  (int task_id, int n, float *array );
VOID muse_ipc_add_doubles (int task_id, int n, double *array );

VOID muse_ipc_get_string  (int task_id, char *s);
VOID muse_ipc_get_bytes   (int task_id, VOID *b, int n);
VOID muse_ipc_get_char    (int task_id, char *c);
VOID muse_ipc_get_short   (int task_id, short *n);
VOID muse_ipc_get_int     (int task_id, int *n);
VOID muse_ipc_get_long    (int task_id, int32 *n);
VOID muse_ipc_get_float   (int task_id, float *n);
VOID muse_ipc_get_double  (int task_id, double *n);
VOID muse_ipc_get_shorts  (int task_id, int n, short *array );
VOID muse_ipc_get_ints    (int task_id, int n, int *array );
VOID muse_ipc_get_longs   (int task_id, int n, int32 *array );
VOID muse_ipc_get_floats  (int task_id, int n, float *array );
VOID muse_ipc_get_doubles (int task_id, int n, double *array );
#else
int  muse_ipc_spawn ();
VOID muse_ipc_send ();
VOID muse_ipc_command ();
int  muse_ipc_receive ();
int  muse_ipc_peek ();
VOID muse_ipc_init_child ();

VOID muse_ipc_call_back ();

int  muse_ipc_setup_client ();
int  muse_ipc_setup_server ();

int  muse_ipc_echo ();

VOID muse_ipc_close ();

VOID muse_ipc_init_send();

VOID muse_ipc_add_string ();
VOID muse_ipc_add_bytes ();
VOID muse_ipc_add_char ();
VOID muse_ipc_add_short ();
VOID muse_ipc_add_int ();
VOID muse_ipc_add_long ();
VOID muse_ipc_add_float ();
VOID muse_ipc_add_double ();
VOID muse_ipc_add_shorts ();
VOID muse_ipc_add_ints ();
VOID muse_ipc_add_longs ();
VOID muse_ipc_add_floats ();
VOID muse_ipc_add_doubles ();

VOID muse_ipc_get_string ();
VOID muse_ipc_get_bytes ();
VOID muse_ipc_get_char ();
VOID muse_ipc_get_short ();
VOID muse_ipc_get_int ();
VOID muse_ipc_get_long ();
VOID muse_ipc_get_float ();
VOID muse_ipc_get_double ();
VOID muse_ipc_get_shorts ();
VOID muse_ipc_get_ints ();
VOID muse_ipc_get_longs ();
VOID muse_ipc_get_floats ();
VOID muse_ipc_get_doubles ();

#endif

#endif /* H_MUSE_IPC */
