
 

 

 


 

 
 
 

 
 
 
 


#pragma	comment(exestr, "xpg4plus @(#) stdio.h 20.1 94/12/04 ")

#pragma	pack(4)


typedef unsigned int	size_t;

typedef long	fpos_t;

typedef long	wchar_t;

typedef long	wint_t;














 





typedef struct _FILE_	 
{
	int		__cnt;	 
	unsigned char	*__ptr;	 
	unsigned char	*__base;	 
	unsigned char	__flag;	 
	unsigned char	__file;	 
	unsigned char	__buf[2]; 
} FILE;


extern FILE	__iob[];




extern int	remove(const char *);
extern int	rename(const char *, const char *);
extern FILE	*tmpfile(void);
extern char	*tmpnam(char *);
extern int	fclose(FILE *);
extern int	fflush(FILE *);
extern FILE	*fopen(const char *, const char *);
extern FILE	*freopen(const char *, const char *, FILE *);
extern void	setbuf(FILE *, char *);
extern int	setvbuf(FILE *, char *, int, size_t);
		 
extern int	fprintf(FILE *, const char *, ...);
		 
extern int	fscanf(FILE *, const char *, ...);
		 
extern int	printf(const char *, ...);
		 
extern int	scanf(const char *, ...);
		 
extern int	sprintf(char *, const char *, ...);
		 
extern int	sscanf(const char *, const char *, ...);
extern int	vfprintf(FILE *, const char *,  char * ) ;
extern int	vprintf(const char *,  char * ) ;
extern int	vsprintf(char *, const char *,  char * ) ;
extern int	fgetc(FILE *);
extern char	*fgets(char *, int, FILE *);
extern int	fputc(int, FILE *);
extern int	fputs(const char *, FILE *);
extern int	getc(FILE *);
extern int	getchar(void);
extern char	*gets(char *);
extern int	putc(int, FILE *);
extern int	putchar(int);
extern int	puts(const char *);
extern int	ungetc(int, FILE *);
extern size_t	fread(void *, size_t, size_t, FILE *);
#pragma	int_to_unsigned fread
extern size_t	fwrite(const void *, size_t, size_t, FILE *);
#pragma	int_to_unsigned fwrite
extern int	fgetpos(FILE *, fpos_t *);
extern int	fseek(FILE *, long, int);
extern int	fsetpos(FILE *, const fpos_t *);
extern long	ftell(FILE *);
extern void	rewind(FILE *);
extern void	clearerr(FILE *);
extern int	feof(FILE *);
extern int	ferror(FILE *);
extern void	perror(const char *);

extern int	__filbuf(FILE *);
extern int	__flsbuf(int, FILE *);





extern int	(fileno)(FILE *);  
extern char	*ctermid(char *);
extern FILE	*fdopen(int, const char *);

extern FILE	*popen(const char *, const char *);
extern char	*tempnam(const char *, const char *);
extern int	getw(FILE *);
extern int	putw(int, FILE *);
extern int	pclose(FILE *);

 

extern char	*optarg;
extern int	optind, opterr, optopt;
extern char	*cuserid(char *);  
extern int	getopt(int, char *const*, const char *);



extern int	system(const char *);
extern wint_t	fgetwc(FILE *);
extern wchar_t	*fgetws(wchar_t *, int, FILE *);
extern wint_t	fputwc(wint_t, FILE *);
extern int	fputws(const wchar_t *, FILE *);
extern wint_t	getwc(FILE *);
extern wint_t	getwchar(void);
extern wint_t	putwc(wint_t, FILE *);
extern wint_t	putwchar(wint_t);
extern wint_t	ungetwc(wint_t, FILE *);
		 
extern int	fwprintf(FILE *, const wchar_t *, ...);
		 
extern int	fwscanf(FILE *, const wchar_t *, ...);
		 
extern int	wprintf(const wchar_t *, ...);
		 
extern int	wscanf(const wchar_t *, ...);
		 
extern int	swprintf(wchar_t *, size_t, const wchar_t *, ...);
		 
extern int	swscanf(const wchar_t *, const wchar_t *, ...);
extern int	vfwprintf(FILE *, const wchar_t *,  char * ) ;
extern int	vfwscanf(FILE *, const wchar_t *,  char * ) ;
extern int	vwprintf(const wchar_t *,  char * ) ;
extern int	vwscanf(const wchar_t *,  char * ) ;
extern int	vswprintf(wchar_t *, size_t, const wchar_t *,  char * ) ;
extern int	vswscanf(const wchar_t *, const wchar_t *,  char * ) ;
extern void	funflush(FILE *);
		 
extern int	snprintf(char *, size_t, const char *, ...);
extern int	vsnprintf(char *, size_t, const char *,  char * ) ;
extern int	vfscanf(FILE *, const char *,  char * ) ;
extern int	vscanf(const char *,  char * ) ;
extern int	vsscanf(const char *, const char *,  char * ) ;

 
		 
extern int	nl_fprintf(FILE *, const char *, ...);
		 
extern int	nl_fscanf(FILE *, const char *, ...);
		 
extern int	nl_printf(const char *, ...);
		 
extern int	nl_scanf(const char *, ...);
		 
extern int	nl_sprintf(char *, const char *, ...);
		 
extern int	nl_sscanf(const char *, const char *, ...);




#pragma	pack()

 


 

 
 
 

 
 
 
 


#pragma	comment(exestr, "xpg4plus @(#) stdlib.h 20.1 94/12/04 ")


#pragma	pack(4)

typedef	struct
{
	int	quot;
	int	rem;
} div_t;

typedef struct
{
	long	quot;
	long	rem;
} ldiv_t;


typedef int	ssize_t;





 


extern unsigned char	__ctype[];


extern double	atof(const char *);
extern int	atoi(const char *);
extern long	atol(const char *);
extern double	strtod(const char *, char **);
extern float	strtof(const char *, char **);
extern long	strtol(const char *, char **, int);
extern unsigned long	strtoul(const char *, char **, int);

extern int	rand(void);
extern void	srand(unsigned int);

extern void	*calloc(size_t, size_t);
extern void	free(void *);
extern void	*malloc(size_t);
extern void	*realloc(void *, size_t);

extern void	abort(void);
extern int	atexit(void (*)(void));
extern void	exit(int);
extern char	*getenv(const char *);
extern int	system(const char *);

extern void	*bsearch(const void *, const void *, size_t, size_t,
			int (*)(const void *, const void *));
extern void	qsort(void *, size_t, size_t,
			int (*)(const void *, const void *));

extern int	(abs)(int);	 

extern div_t	div(int, int);
extern long	labs(long);
extern ldiv_t	ldiv(long, long);

extern int	mbtowc(wchar_t *, const char *, size_t);
extern int	mblen(const char *, size_t);
extern int	wctomb(char *, wchar_t);

extern size_t	mbstowcs(wchar_t *, const char *, size_t);
extern size_t	wcstombs(char *, const wchar_t *, size_t);


extern long	a64l(const char *);
extern int	dup2(int, int);
extern char	*ecvt(double, int, int *, int *);
extern char	*ecvtl(long double, int, int *, int *);
extern char	*fcvt(double, int, int *, int *);
extern char	*fcvtl(long double, int, int *, int *);
extern char	*getcwd(char *, size_t);
extern char	*getlogin(void);
extern int	getopt(int, char *const *, const char *);
extern int	getsubopt(char **, char *const *, char **);
extern char	*initstate(unsigned, char *, int);
extern int	grantpt(int);
extern char	*optarg;
extern int	optind, opterr, optopt;
extern char	*getpass(const char *);
extern int	getpw(int, char *);
extern char	*gcvt(double, int, char *);
extern char	*gcvtl(long double, int, char *);
extern int	isatty(int);
extern void	l3tol(long *, const char *, int);
extern char	*l64a(long);
extern char	*l64a_r(long, char *, size_t);
extern void	ltol3(char *, const long *, int);
extern void	*memalign(size_t, size_t);
extern char	*mktemp(char *);
extern int	mkstemp(char *);
extern char	*ptsname(int);
extern long	random(void);
extern int	rand_r(unsigned int *);
extern char	*realpath(const char *, char *);
extern char	*setstate(char *);
extern void	srandom(unsigned);
extern long double	strtold(const char *, char **);
extern void	swab(const void *, void *, ssize_t);
extern char	*ttyname(int);
extern int	ttyslot(void);
extern int	unlockpt(int);
extern void	*valloc(size_t);
extern double	wcstod(const wchar_t *, wchar_t **);
extern float	wcstof(const wchar_t *, wchar_t **);
extern long	wcstol(const wchar_t *, wchar_t **, int);
extern long double	wcstold(const wchar_t *, wchar_t **);
extern unsigned long	wcstoul(const wchar_t *, wchar_t **, int);



extern double	drand48(void);
extern double	erand48(unsigned short *);
extern long	jrand48(unsigned short *);
extern void	lcong48(unsigned short *);
extern long	lrand48(void);
extern long	mrand48(void);
extern long	nrand48(unsigned short *);
extern int	putenv(const char *);
extern unsigned short	*seed48(unsigned short *);
extern void	setkey(const char *);
extern void	srand48(long);





#pragma	pack()

 


 

 
 
 

 
 
 
 


#pragma	comment(exestr, "xpg4plus @(#) string.h 20.1 94/12/04 ")






extern void	*memchr(const void *, int, size_t);
extern void	*memcpy(void *, const void *, size_t);
extern void	*memccpy(void *, const void *, int, size_t);
extern void	*memmove(void *, const void *, size_t);
extern void	*memset(void *, int, size_t);

extern char	*strchr(const char *, int);
extern char	*strcpy(char *, const char *);
extern char	*strncpy(char *, const char *, size_t);
extern char	*strcat(char *, const char *);
extern char	*strncat(char *, const char *, size_t);
extern char	*strpbrk(const char *, const char *);
extern char	*strrchr(const char *, int);
extern char	*strstr(const char *, const char *);
extern char	*strtok(char *, const char *);
extern char	*strtok_r(char *, const char *, char **);
extern char	*strerror(int);
extern char	*strlist(char *, const char *, ...);

extern int	memcmp(const void *, const void *, size_t);
extern int	strcmp(const char *, const char *);
extern int	strcoll(const char *, const char *);
extern int	strncmp(const char *, const char *, size_t);

extern void perror(const char *);
extern char *strdup(const char *);
extern int     strncoll(const char *, const char *, int);
extern size_t  strnxfrm(char *, const char *, size_t , int);

extern size_t	strxfrm(char *, const char *, size_t);
extern size_t	strcspn(const char *, const char *);
extern size_t	strspn(const char *, const char *);
extern size_t	strlen(const char *);

#pragma	int_to_unsigned strcspn
#pragma	int_to_unsigned strspn
#pragma	int_to_unsigned strlen

 


extern int	ffs(int);
 
extern int nl_strcmp(char *, char *);
extern int nl_strncmp(char *, char *, int n);




 


 

 
 
 

 
 
 
 


#pragma	comment(exestr, "xpg4plus @(#) malloc.h 20.1 94/12/04 ")


#pragma	pack(4)

 
 
struct mallinfo  {
	int arena;	 
	int ordblks;	 
	int smblks;	 
	int hblks;	 
	int hblkhd;	 
	int usmblks;	 
	int fsmblks;	 
	int uordblks;	 
	int fordblks;	 
	int keepcost;	 
};	



extern void *malloc(size_t);
extern void free(void *);
extern void *realloc(void *, size_t);
extern int mallopt(int, int);
extern struct mallinfo mallinfo(void);
extern void *calloc(size_t, size_t);


#pragma	pack()


 


 

 
 
 

 
 
 
 


#pragma	comment(exestr, "xpg4plus @(#) limits.h 20.2 94/12/15 ")





 





 





 


 


 


 



 


 






 


 


 


 






 


extern long _sysconf(int);



extern  void G_warning (char *msg);



 


 
 










 short dir_create ( ) ;
 short dir_pop ( ) ;
 short dir_push ( ) ;
 short dir_restore ( ) ;
 short dir_save ( ) ;
 short dir_current ( ) ;
 short file_spec_to_string ( ) ;
FILE *file_open ();
int muse_access ();
long muse_filelength ();
void muse_check_path ();











 



 


 


 

 
 
 

 
 
 
 

 


 

 
 
 

 
 
 
 

 


 

 
 
 

 
 
 
 

 


 

 
 
 

 
 
 
 

 


 

 
 
 

 
 
 
 


extern  void G_warning (char *msg);



 


 
 




 
 
typedef struct
   {
   long     size;
   char   * buf ;
    void *  buf_handle ;
   } set_type;


 
   set_type set_init ();
   int  set_empty ();
   void set_insert ();
   void set_delete ();
   int  set_member ();
   long set_min ();
   long set_max ();
   long num_in_set ();
   void set_on ();
   void set_off ();
   int  set_equal ();
   void set_assign ();
   set_type set_union ();
   set_type set_intersection ();
   set_type set_difference ();
   void set_nuke ();



 



 


 

 
 
 

 
 
 
 

 


 

 
 
 

 
 
 
 


#pragma	comment(exestr, "xpg4plus @(#) math.h 20.1 94/12/04 ")

#pragma	pack(4)



struct exception
{
	int	type;
	char	*name;
	double	arg1;
	double	arg2;
	double	retval;
};

extern int	matherr(struct exception *);



extern double	acos(double);
extern double	asin(double);
extern double	atan(double);
extern double	atan2(double, double);
extern double	cos(double);
extern double	sin(double);
extern double	tan(double);

extern double	cosh(double);
extern double	sinh(double);
extern double	tanh(double);

extern double	exp(double);
extern double	frexp(double, int *);
extern double	ldexp(double, int);
extern double	log(double);
extern double	log10(double);
extern double	modf(double, double *);

extern double	pow(double, double);
extern double	sqrt(double);

extern double	ceil(double);
extern double	fabs(double);
extern double	floor(double);
extern double	fmod(double, double);

extern const double __huge_val;


extern double	erf(double);
extern double	erfc(double);
extern double	gamma(double);
extern double	hypot(double, double);
extern double	j0(double);
extern double	j1(double);
extern double	jn(int, double);
extern double	y0(double);
extern double	y1(double);
extern double	yn(int, double);
extern double	lgamma(double);
extern int	isnan(double);





 
 
extern long double	frexpl(long double, int *);
extern long double	ldexpl(long double, int);
extern long double	modfl(long double, long double *);

extern float	acosf(float);
extern float	asinf(float);
extern float	atanf(float);
extern float	atan2f(float, float);
extern float	cosf(float);
extern float	sinf(float);
extern float	tanf(float);

extern float	coshf(float);
extern float	sinhf(float);
extern float	tanhf(float);

extern float	expf(float);
extern float	logf(float);
extern float	log10f(float);

extern float	powf(float, float);
extern float	sqrtf(float);

extern float	ceilf(float);
extern float	fabsf(float);
extern float	floorf(float);
extern float	fmodf(float, float);
extern float	modff(float, float *);

 

extern double	atof(const char *);
extern double	scalb(double, double);
extern double	logb(double);
extern double	log1p(double);
extern double	nextafter(double, double);
extern double	acosh(double);
extern double	asinh(double);
extern double	atanh(double);
extern double	cbrt(double);
extern double	copysign(double, double);
extern double	expm1(double);
extern int	ilogb(double);
extern double	remainder(double, double);
extern double	rint(double);
extern int	unordered(double, double);
extern int	finite(double);

extern long double	scalbl(long double, long double);
extern long double	logbl(long double);
extern long double	nextafterl(long double, long double);
extern int	unorderedl(long double, long double);
extern int	finitel(long double);




extern int	signgam;










#pragma	pack()







 


 

 
 
 

 
 
 
 

 


 

 
 
 

 
 
 
 

 


 

 
 
 

 
 
 
 

 


 

 
 
 

 
 
 
 

 


 

 
 
 

 
 
 
 


extern  void G_warning (char *msg);



 


 
 





typedef struct
{
    long            machine;
    long            input;
    long            output;
} xBYTE_ORDER;







 
 


 


 

 
 
 

 
 
 
 


 


 



 
typedef enum {
  VpfNull,
  VpfChar,
  VpfShort,
  VpfInteger,
  VpfFloat,
  VpfDouble,
  VpfDate,
  VpfKey,
  VpfCoordinate,
  VpfTriCoordinate,
  VpfDoubleCoordinate,
  VpfDoubleTriCoordinate,
  VpfUndefined
} VpfDataType ;





















 
   long VpfRead ();
   long VpfWrite ();







 
typedef char date_type[21] ;    

 
typedef union
   {
   char      *Char;
   short int Short;
   long int  Int;
   float     Float;
   double    Double;
   date_type Date;
   char      Other;
   } null_field;

 
typedef struct {
   char *name;            
   char *tdx;             
   char *narrative;       
   long int  count;       
   char description[81];  
   char keytype;          
   char vdt[13];          
   char type;             
   null_field nullval ;   
} header_cell, *header_type;

typedef enum { ram, disk, either, compute } storage_type;

typedef enum { Read, Write } file_mode ;


 
 
typedef struct
   {
   long count;
   void *ptr;
   } column_type;

 
typedef column_type *row_type;
typedef column_type   * ROW ;

 
 
typedef struct {
   unsigned long  pos;
   unsigned long  length;
} index_cell;

typedef index_cell   * index_type ;

 
typedef struct {
   char            *path;            
   long            nfields;          
   long            nrows;            
   long            reclen;           
   long            ddlen;            
   FILE            *fp;              
   FILE            *xfp;             
   index_type      index;            
    void *         idx_handle ;       
   storage_type    storage;          
   storage_type    xstorage;         
   header_type     header;           
   ROW             *row;             
    void *         row_handle ;       
   file_mode       mode;             
   char            *defstr ;         
   char            name[13];         
   char            description[81];  
   char            narrative[13];    
   unsigned char   status;           
   unsigned char   byte_order;       
} vpf_table_type;

typedef struct {
   float x,y;
} coordinate_type;

typedef struct {
   double x,y;
} double_coordinate_type;

typedef struct {
   float x,y,z;
} tri_coordinate_type;

typedef struct {
   double x,y, z;
} double_tri_coordinate_type;

 
typedef union {
   unsigned char  f1;
   unsigned short f2;
   unsigned long  f3;
} key_field;

 
typedef struct {
   unsigned char type;
   long  id, tile, exid;
} id_triplet_type;

 


 


 


 






 
   double quiet_nan ();
   void vpf_nullify_table ();
   vpf_table_type vpf_open_table ();
   void vpf_close_table ();
   char *read_text_defstr ();
   long index_length ();
   long index_pos ();
   id_triplet_type read_key ();
   long row_offset ();
   row_type  read_next_row ();
   row_type rowcpy ();
   row_type  read_row ();
   void free_row ();
   row_type get_row ();
   long table_pos ();
   void *get_table_element ();
   void *named_table_element ();
   void *table_element ();
   long is_vpf_table ();
   long is_vpf_null_float ();
   long is_vpf_null_double ();
   long parse_data_def ();

    
   long int write_key ();
   long int write_next_row ();
   long int write_row ();
   row_type create_row ();
   void nullify_table_element ();
   long put_table_element ();
   void swap_two ();
   void swap_four ();
   void swap_eight ();
   void format_date ();

  extern FILE * errorfp;




 



typedef struct linked_list_cell
   {
   void                    *element;
   size_t                  element_size;
   struct linked_list_cell *next;
   } cell_type, *linked_list_type, *position_type;






   linked_list_type ll_init ();
   long ll_empty ();
   position_type ll_first ();
   position_type ll_last ();
   position_type ll_next ();
   position_type ll_previous ();
   long ll_end ();
   void ll_element ();
   void ll_insert ();
   void ll_delete ();
   void ll_reset ();
   position_type ll_locate ();
   void ll_replace ();
   size_t ll_element_size ();





 




 


 
typedef enum { VPF_0_7, VPF_0_8, VPF_0_9, VPF_1_0 } vpf_version_type;


 
typedef enum { LINE=1, AREA, ANNO, VPFPOINTS, VPFCOMPLEX=6 } vpf_feature_type;

 
typedef enum { EDGE=1, FACE, TEXT, ENTITY_NODE, CONNECTED_NODE }
   vpf_primitive_type;

typedef struct {
   unsigned char edge;
   unsigned char face;
   unsigned char text;
   unsigned char entity_node;
   unsigned char connected_node;
} primitive_class_type;


typedef enum { UNKNOWN_SECURITY, UNCLASSIFIED, RESTRICTED, CONFIDENTIAL,
               SECRET, TOP_SECRET } security_type;


 
typedef enum { UNKNOWN_UNITS, METERS, FEET, INCHES,
               KILOMETERS, OTHER_UNITS, DEC_DEGREES } vpf_units_type;


 

typedef enum {
   DDS,    
   AC,    
   AK,    
   AL,    
   GN,    
   LE,    
   LJ,    
   MC,    
   OC,    
   OD,    
   PG,    
   TC,    
   UT,    
   PC     
} vpf_projection_code;


typedef struct
   {
   vpf_projection_code code;
   double parm1, parm2, parm3, parm4;
   vpf_units_type units;
   double false_easting, false_northing;
   long (*forward_proj)();
   long (*inverse_proj)();
   char name[21];
   } vpf_projection_type;

typedef unsigned char boolean;





 
typedef struct
   {
   double x1, y1, x2, y2;
} extent_type, line_segment_type;

 
typedef struct {
   float x1, y1, x2, y2;
} fextent_type;

typedef enum { 
   Miles, 
   Meters, 
   Degrees, 
   Feet, 
   Inches, 
   Kilometers 
} coord_units_type;

 
typedef struct {
   long degrees;
   long minutes;
   float seconds;
} dms_type;


 
 

 
 


 
   double dms_to_float ();
   double gc_distance ();
   long contained ();
   long geo_intersect ();
   long completely_within ();
   long fwithin ();
   long intersect ();
   long perpendicular_intersection ();
   dms_type float_to_dms ();
   double dms_to_float ();





 
typedef struct {
   char name[9];             
   boolean viewable;         
   char *path;               
   long int ntiles;          
   set_type tile_set;        
   vpf_projection_code projection;  
   vpf_units_type units;     
} library_type;

 
typedef struct {
   char name[9];       
   char *path;         
   library_type *library;  
   long  nlibraries;    
} database_type;

 
 
 
 
typedef struct {
   long point_color;
   long point;
   long line_color;
   long line;
   long area_color;
   long area;
   long text_color;
   long text;
} theme_symbol_type;

 
 
 
typedef struct {
   char *description;               
   char *database;                  
   char *library;                   
   char *coverage;                  
   char *fc;                        
   char *ftable;                    
   primitive_class_type primclass;  
   char *expression;                
} theme_type;


 
 
typedef struct {
   char name[9];              
   database_type *database;   
   long  ndb;		       
   char *path;		      
   long  nthemes;	       
   theme_type *theme;         
   set_type selected;         
   set_type displayed;        
   linked_list_type sellist;  
   extent_type extent;        
   double tileheight;         
   char sympath[255];         
} view_type;


 
typedef struct {
   extent_type mapextent;            
   boolean     mapchanged;           
   boolean     mapdisplayed;         
   boolean     user_escape;          
   boolean     study_area_selected;  
   boolean     latlongrid;           
   boolean     scale_bar;            
   vpf_projection_type projection;   
   coord_units_type distance_unit;   
   coord_units_type scale_bar_unit;  
   vpf_units_type   locator_unit;    
} map_environment_type;

typedef struct {
  extent_type  	mapextent;	 
  boolean	mapdisplayed;	 
  boolean	latlongrid;	 
  boolean	tilegrid;	 
  boolean	points;		 
  boolean	text;		 
  vpf_units_type locator_unit;   
} libref_map_environment_type;

 

 
 

 





 
   set_type query_table ();
   linked_list_type parse_expression ();



 


 

 
 
 

 
 
 
 

 
typedef struct {                         
  long int      nbytes ,                 
                nbins ,                  
                table_nrows ;            
  char          index_type ,             
                column_type ;            
  long int      type_count ;             
  char          id_data_type ,           
		vpf_table_name[13] ,
		vpf_column_name[25] ,    
		sort ,                   
		padding[3] ;             
} ThematicIndexHeader ;
  
 
 

typedef union
   {
   char    cval , *strval;
   long    ival;
   short   sval;
   float   fval;
   double  dval;
   } ThematicIndexValue;

typedef struct {                         
  ThematicIndexValue value;              
  long int      binid,
                start_offset ,
                num_items ;                
} ThematicIndexDirectory ;
 
typedef struct {
   ThematicIndexHeader h;
   ThematicIndexDirectory *d, *gid;
   FILE *fp;
} ThematicIndex;
 
 
   long create_thematic_index ();
   set_type read_thematic_index ();
   ThematicIndex open_thematic_index ();
   set_type search_thematic_index ();
   void close_thematic_index ();
   long create_gazetteer_index ();
   set_type search_gazetteer_index ();
   set_type read_gazetteer_index ();
 






 



typedef struct {
   char table1[40];
   char key1[40];
   char table2[40];
   char key2[40];
} vpf_relate_struct;


typedef struct {
   long nchain;
   vpf_table_type *table;
   linked_list_type relate_list;
} feature_class_relate_type, fcrel_type;


   long num_relate_paths ();
   linked_list_type fcs_relate_list ();
   long related_row ();
   linked_list_type related_rows ();
   fcrel_type select_feature_class_relate ();
   long fc_row_number ();
   linked_list_type fc_row_numbers ();
   void deselect_feature_class_relate ();


   set_type get_fit_tile_primitives ();


 
   set_type get_fit_tile_primitives (covpath, primclass, expression, feature_table, tile,
                                     fca_id, numprims, status)
      char *covpath;
      long primclass;
      char *expression;
      vpf_table_type feature_table;
      long tile, fca_id, numprims, *status;

   {
   set_type primitives, tileset, fcset, selset;
   long i, start, end, prim_id, tile_id, fc_id, feature_id, count;
   short short_tile_id;
   long PRIM_ID_, TILE_ID_, FC_ID_, FEATURE_ID_;
   vpf_table_type fit;
   row_type row, frow;
   char path[255];
   static char *ptable[] = {"","EDG","FAC","TXT","END","CND"};

   primitives = set_init (numprims+1);

    __std_hdr_strcpy ( path , covpath ) ;
   strcat (path, ptable[primclass]);
   strcat (path, ".FIT");
   muse_check_path (path);

   if (muse_access (path,0) != 0)
      return primitives;

   fit = vpf_open_table (path, disk, "rb",  0 ) ;
   if (!fit.fp)
      return primitives;

   TILE_ID_ = table_pos ("TILE_ID", fit);
   PRIM_ID_ = table_pos ("PRIM_ID", fit);
   FC_ID_ = table_pos ("FC_ID", fit);
   if (FC_ID_ < 0)
      FC_ID_ = table_pos ("FCA_ID", fit);
   FEATURE_ID_ = table_pos ("FEATURE_ID", fit);
   if ((TILE_ID_ < 0 && tile) || PRIM_ID_ < 0 ||
       FC_ID_ < 0 || FEATURE_ID_ < 0)
      {
      vpf_close_table (&fit);
      *status = 0;
      return primitives;
      }

    
   tileset.size = 0;
   if (tile)
      {
      if (fit.header[TILE_ID_].tdx)
         {
	       __std_hdr_strcpy ( path , covpath ) ;
	      strcat (path, fit.header[TILE_ID_].tdx);
	      muse_check_path (path);
	      if (muse_access (path,0) == 0)
	         {
	         if (fit.header[TILE_ID_].type == 'I')
	            {
	            tile_id = (long int)tile;
	            tileset = read_thematic_index (path, (char*)&tile_id);
	            }
	         else if (fit.header[TILE_ID_].type == 'S')
	            {
	            short_tile_id = tile;
	            tileset = read_thematic_index(path,(char *)&short_tile_id);
	            }
	         }  
         }  
      }  

   if (!tileset.size)
      {
      tileset = set_init (fit.nrows+1);
      set_on (tileset);
      set_delete (0, tileset);
      }

    
   fcset.size = 0;
   if (fit.header[FC_ID_].tdx)
      {
       __std_hdr_strcpy ( path , covpath ) ;
      strcat (path, fit.header[FC_ID_].tdx);
      muse_check_path (path);
      if (muse_access (path, 0) == 0)
         {
         fc_id = (long)fca_id;
         fcset = read_thematic_index (path, (char*)&fc_id);
         }
      }  

   if (!fcset.size) 
      {
      fcset = set_init (fit.nrows+1);
      set_on (fcset);
      set_delete (0, fcset);
      }

    
   selset = set_intersection (tileset, fcset);
   set_nuke (&tileset);
   set_nuke (&fcset);

   if (set_empty (selset))
      {
      vpf_close_table (&fit);
      set_nuke (&selset);
      *status = 1;
      return primitives;
      }


    
   start = set_min (selset);
   end = set_max (selset);

    
   fseek (fit.fp, index_pos (start, fit),  0 ) ;

   for (i=start; i<=end; i++)
      {
       
       
      row = read_next_row (fit);   
      if (set_member (i, selset))
         {
      
          
          
         get_table_element(PRIM_ID_,row,fit,&prim_id,&count);
         get_table_element(FC_ID_,row,fit,&fc_id,&count);
         get_table_element(FEATURE_ID_,row,fit,&feature_id,&count);
         tile_id = 0;
         if (tile)
            {
            if (fit.header[TILE_ID_].type == 'I')
               {
               get_table_element (TILE_ID_, row, fit, &tile_id, &count);
               }
            else if (fit.header[TILE_ID_].type == 'S')
               {
               get_table_element(TILE_ID_, row, fit, &short_tile_id, &count);
               tile_id = short_tile_id;
               }
            }  
         free_row (row, fit);

         if (tile_id != tile  ||  fc_id != fca_id)
            continue;
         frow = get_row (feature_id, feature_table);
         if (query_table_row (expression, frow, feature_table))
	         set_insert (prim_id, primitives);
         free_row(frow,feature_table);
         }   
      free_row (row, fit);
      }  

   vpf_close_table (&fit);
   set_nuke (&selset);

   *status = 1;

   return primitives;
   }

#ident "acomp: Release 5.0.0a 16Mar95"
