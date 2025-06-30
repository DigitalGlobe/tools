# 1 "amdb_rtree.cc"
#pragma GCC set_debug_pwd "/Users/jmh/devel/libgist/src/librtree"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "amdb_rtree.cc"




# 1 "/Users/jmh/devel/libgist/include/amdb_ext.h" 1







# 1 "/Users/jmh/devel/libgist/include/gist_defs.h" 1
# 9 "/Users/jmh/devel/libgist/include/gist_defs.h"
# 1 "/usr/include/sys/types.h" 1 3 4
# 66 "/usr/include/sys/types.h" 3 4
# 1 "/usr/include/sys/appleapiopts.h" 1 3 4
# 67 "/usr/include/sys/types.h" 2 3 4


# 1 "/usr/include/sys/cdefs.h" 1 3 4
# 70 "/usr/include/sys/types.h" 2 3 4


# 1 "/usr/include/machine/types.h" 1 3 4
# 30 "/usr/include/machine/types.h" 3 4
# 1 "/usr/include/ppc/types.h" 1 3 4
# 69 "/usr/include/ppc/types.h" 3 4
typedef signed char int8_t;
typedef unsigned char u_int8_t;
typedef short int16_t;
typedef unsigned short u_int16_t;
typedef int int32_t;
typedef unsigned int u_int32_t;
typedef long long int64_t;
typedef unsigned long long u_int64_t;

typedef int32_t register_t;

typedef long int intptr_t;
typedef unsigned long int uintptr_t;
# 31 "/usr/include/machine/types.h" 2 3 4
# 73 "/usr/include/sys/types.h" 2 3 4

# 1 "/usr/include/machine/ansi.h" 1 3 4
# 33 "/usr/include/machine/ansi.h" 3 4
# 1 "/usr/include/ppc/ansi.h" 1 3 4
# 92 "/usr/include/ppc/ansi.h" 3 4
typedef union {
        char __mbstate8[128];
        long long _mbstateL;
} __mbstate_t;
# 34 "/usr/include/machine/ansi.h" 2 3 4
# 75 "/usr/include/sys/types.h" 2 3 4
# 1 "/usr/include/machine/endian.h" 1 3 4
# 30 "/usr/include/machine/endian.h" 3 4
# 1 "/usr/include/ppc/endian.h" 1 3 4
# 81 "/usr/include/ppc/endian.h" 3 4
extern "C" {
unsigned long htonl (unsigned long);
unsigned short htons (unsigned short);
unsigned long ntohl (unsigned long);
unsigned short ntohs (unsigned short);
}
# 31 "/usr/include/machine/endian.h" 2 3 4
# 76 "/usr/include/sys/types.h" 2 3 4


typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;
typedef unsigned short ushort;
typedef unsigned int uint;


typedef u_int64_t u_quad_t;
typedef int64_t quad_t;
typedef quad_t * qaddr_t;

typedef char * caddr_t;
typedef int32_t daddr_t;
typedef int32_t dev_t;
typedef u_int32_t fixpt_t;
typedef u_int32_t gid_t;
typedef u_int32_t in_addr_t;
typedef u_int16_t in_port_t;
typedef u_int32_t ino_t;
typedef long key_t;
typedef u_int16_t mode_t;
typedef u_int16_t nlink_t;
typedef quad_t off_t;
typedef int32_t pid_t;
typedef quad_t rlim_t;
typedef int32_t segsz_t;
typedef int32_t swblk_t;
typedef u_int32_t uid_t;
# 117 "/usr/include/sys/types.h" 3 4
typedef unsigned long clock_t;




typedef long unsigned int size_t;




typedef int ssize_t;




typedef long time_t;
# 146 "/usr/include/sys/types.h" 3 4
typedef int32_t fd_mask;






typedef struct fd_set {
        fd_mask fds_bits[(((1024) + (((sizeof(fd_mask) * 8)) - 1)) / ((sizeof(fd_mask) * 8)))];
} fd_set;
# 182 "/usr/include/sys/types.h" 3 4
struct _pthread_handler_rec
{
        void (*routine)(void *);
        void *arg;
        struct _pthread_handler_rec *next;
};
# 202 "/usr/include/sys/types.h" 3 4
typedef struct _opaque_pthread_t { long sig; struct _pthread_handler_rec *cleanup_stack; char opaque[596];} *pthread_t;

typedef struct _opaque_pthread_attr_t { long sig; char opaque[36]; } pthread_attr_t;

typedef struct _opaque_pthread_mutexattr_t { long sig; char opaque[8]; } pthread_mutexattr_t;

typedef struct _opaque_pthread_mutex_t { long sig; char opaque[40]; } pthread_mutex_t;

typedef struct _opaque_pthread_condattr_t { long sig; char opaque[4]; } pthread_condattr_t;

typedef struct _opaque_pthread_cond_t { long sig; char opaque[24]; } pthread_cond_t;

typedef struct _opaque_pthread_rwlockattr_t { long sig; char opaque[12]; } pthread_rwlockattr_t;

typedef struct _opaque_pthread_rwlock_t { long sig; char opaque[124]; } pthread_rwlock_t;

typedef struct { long sig; char opaque[4]; } pthread_once_t;



typedef unsigned long pthread_key_t;
# 10 "/Users/jmh/devel/libgist/include/gist_defs.h" 2
# 1 "/usr/include/gcc/darwin/3.3/assert.h" 1 3 4
# 14 "/usr/include/gcc/darwin/3.3/assert.h" 3 4
# 1 "/usr/include/stdlib.h" 1 3 4
# 71 "/usr/include/stdlib.h" 3 4
typedef int rune_t;





typedef int wchar_t;


typedef struct {
        int quot;
        int rem;
} div_t;

typedef struct {
        long quot;
        long rem;
} ldiv_t;
# 99 "/usr/include/stdlib.h" 3 4
extern int __mb_cur_max;




extern "C" {
 void
         abort (void);
 int
         abs (int);
int atexit (void (*)(void));
double atof (const char *);
int atoi (const char *);
long atol (const char *);
void *bsearch (const void *, const void *, size_t, size_t, int (*)(const void *, const void *));

void *calloc (size_t, size_t);
 div_t
         div (int, int);
 void
         exit (int);
void free (void *);
char *getenv (const char *);
 long
         labs (long);
 ldiv_t
         ldiv (long, long);
void *malloc (size_t);
void qsort (void *, size_t, size_t, int (*)(const void *, const void *));

int rand (void);
void *realloc (void *, size_t);
void srand (unsigned);
double strtod (const char *, char **);
long strtol (const char *, char **, int);
unsigned long
         strtoul (const char *, char **, int);
int system (const char *);


int mblen (const char *, size_t);
size_t mbstowcs (wchar_t *, const char *, size_t);
int wctomb (char *, wchar_t);
int mbtowc (wchar_t *, const char *, size_t);
size_t wcstombs (char *, const wchar_t *, size_t);


int putenv (const char *);
int setenv (const char *, const char *, int);



double drand48 (void);
double erand48 (unsigned short[3]);
long jrand48 (unsigned short[3]);
void lcong48 (unsigned short[7]);
long lrand48 (void);
long mrand48 (void);
long nrand48 (unsigned short[3]);
unsigned short
        *seed48 (unsigned short[3]);
void srand48 (long);

void *alloca (size_t);

u_int32_t
         arc4random (void);
void arc4random_addrandom (unsigned char *dat, int datlen);
void arc4random_stir (void);
char *getbsize (int *, long *);
char *cgetcap (char *, char *, int);
int cgetclose (void);
int cgetent (char **, char **, char *);
int cgetfirst (char **, char **);
int cgetmatch (char *, char *);
int cgetnext (char **, char **);
int cgetnum (char *, char *, long *);
int cgetset (char *);
int cgetstr (char *, char *, char **);
int cgetustr (char *, char *, char **);

int daemon (int, int);
char *devname (int, int);
int getloadavg (double [], int);

long a64l (const char *);
char *l64a (long);

char *group_from_gid (unsigned long, int);
int heapsort (void *, size_t, size_t, int (*)(const void *, const void *));

char *initstate (unsigned long, char *, long);
int mergesort (void *, size_t, size_t, int (*)(const void *, const void *));

int radixsort (const unsigned char **, int, const unsigned char *, unsigned);

int sradixsort (const unsigned char **, int, const unsigned char *, unsigned);

int rand_r (unsigned *);
long random (void);
void *reallocf (void *, size_t);
char *realpath (const char *, char resolved_path[]);
char *setstate (char *);
void srandom (unsigned long);
char *user_from_uid (unsigned long, int);

long long
         strtoll(const char *, char **, int);
unsigned long long
         strtoull(const char *, char **, int);
long long
         strtoq (const char *, char **, int);
unsigned long long
         strtouq (const char *, char **, int);

void unsetenv (const char *);

}
# 15 "/usr/include/gcc/darwin/3.3/assert.h" 2 3 4
# 40 "/usr/include/gcc/darwin/3.3/assert.h" 3 4
extern "C" {
extern void __eprintf (const char *, const char *, unsigned, const char *)
    __attribute__ ((noreturn));
}
# 11 "/Users/jmh/devel/libgist/include/gist_defs.h" 2
# 1 "/usr/include/gcc/darwin/3.3/c++/vector" 1 3
# 65 "/usr/include/gcc/darwin/3.3/c++/vector" 3

# 1 "/usr/include/gcc/darwin/3.3/c++/bits/functexcept.h" 1 3
# 34 "/usr/include/gcc/darwin/3.3/c++/bits/functexcept.h" 3
# 1 "/usr/include/gcc/darwin/3.3/c++/exception_defines.h" 1 3
# 35 "/usr/include/gcc/darwin/3.3/c++/bits/functexcept.h" 2 3

namespace std
{

  void
  __throw_bad_exception(void);


  void
  __throw_bad_alloc(void);


  void
  __throw_bad_cast(void);

  void
  __throw_bad_typeid(void);


  void
  __throw_logic_error(const char* __s);

  void
  __throw_domain_error(const char* __s);

  void
  __throw_invalid_argument(const char* __s);

  void
  __throw_length_error(const char* __s);

  void
  __throw_out_of_range(const char* __s);

  void
  __throw_runtime_error(const char* __s);

  void
  __throw_range_error(const char* __s);

  void
  __throw_overflow_error(const char* __s);

  void
  __throw_underflow_error(const char* __s);


  void
  __throw_ios_failure(const char* __s);
}
# 67 "/usr/include/gcc/darwin/3.3/c++/vector" 2 3
# 1 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 1 3
# 64 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 3
# 1 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/c++config.h" 1 3
# 35 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/c++config.h" 3
# 1 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/os_defines.h" 1 3
# 72 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/os_defines.h" 3
extern "C" {
# 83 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/os_defines.h" 3
typedef enum node_kinds {
        NODE_THREAD_SPECIFIC_DATA=1,
        NODE_PROCESSWIDE_PTR=2,
        NODE_LAST_KIND
        } TnodeKind ;





typedef enum node_mode {
        NM_ALLOW_RECURSION=1,
        NM_RECURSION_ILLEGAL=2,
        NM_ENHANCED_LOCKING=3,
        NM_LOCKED=4
        } TnodeMode ;



extern void * _keymgr_get_per_thread_data(unsigned int key) ;
extern void _keymgr_set_per_thread_data(unsigned int key, void *keydata) ;
extern void *_keymgr_get_and_lock_processwide_ptr(unsigned int key) ;
extern void _keymgr_set_and_unlock_processwide_ptr(unsigned int key, void *ptr) ;
extern void _keymgr_unlock_processwide_ptr(unsigned int key) ;
extern void _keymgr_set_lockmode_processwide_ptr(unsigned int key, unsigned int mode) ;
extern unsigned int _keymgr_get_lockmode_processwide_ptr(unsigned int key) ;
extern int _keymgr_get_lock_count_processwide_ptr(unsigned int key) ;
# 155 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/os_defines.h" 3
}
# 36 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/c++config.h" 2 3
# 65 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 2 3
# 1 "/usr/include/gcc/darwin/3.3/c++/cstring" 1 3
# 48 "/usr/include/gcc/darwin/3.3/c++/cstring" 3

# 1 "/usr/include/gcc/darwin/3.3/c++/cstddef" 1 3
# 47 "/usr/include/gcc/darwin/3.3/c++/cstddef" 3

# 1 "/usr/include/stddef.h" 1 3 4
# 66 "/usr/include/stddef.h" 3 4
typedef int ptrdiff_t;
# 49 "/usr/include/gcc/darwin/3.3/c++/cstddef" 2 3

namespace std
{
  using ::ptrdiff_t;
  using ::size_t;
}
# 50 "/usr/include/gcc/darwin/3.3/c++/cstring" 2 3

# 1 "/usr/include/string.h" 1 3 4
# 72 "/usr/include/string.h" 3 4
extern "C" {
void *memchr (const void *, int, size_t);
int memcmp (const void *, const void *, size_t);
void *memcpy (void *, const void *, size_t);
void *memmove (void *, const void *, size_t);
void *memset (void *, int, size_t);
char *strcat (char *, const char *);
char *strchr (const char *, int);
int strcmp (const char *, const char *);
int strcoll (const char *, const char *);
char *strcpy (char *, const char *);
size_t strcspn (const char *, const char *);
char *strerror (int);
size_t strlen (const char *);
char *strncat (char *, const char *, size_t);
int strncmp (const char *, const char *, size_t);
char *strncpy (char *, const char *, size_t);
char *strpbrk (const char *, const char *);
char *strrchr (const char *, int);
size_t strspn (const char *, const char *);
char *strstr (const char *, const char *);
char *strtok (char *, const char *);
size_t strxfrm (char *, const char *, size_t);



int bcmp (const void *, const void *, size_t);
void bcopy (const void *, void *, size_t);
void bzero (void *, size_t);
int ffs (int);
char *index (const char *, int);
void *memccpy (void *, const void *, int, size_t);
char *rindex (const char *, int);
int strcasecmp (const char *, const char *);
char *strdup (const char *);
size_t strlcat (char *, const char *, size_t);
size_t strlcpy (char *, const char *, size_t);
void strmode (int, char *);
int strncasecmp (const char *, const char *, size_t);
char *strsep (char **, const char *);
char *strtok_r (char *, const char *, char **);
void swab (const void *, void *, size_t);

}
# 52 "/usr/include/gcc/darwin/3.3/c++/cstring" 2 3
# 77 "/usr/include/gcc/darwin/3.3/c++/cstring" 3
namespace std
{
  using ::memcpy;
  using ::memmove;
  using ::strcpy;
  using ::strncpy;
  using ::strcat;
  using ::strncat;
  using ::memcmp;
  using ::strcmp;
  using ::strcoll;
  using ::strncmp;
  using ::strxfrm;
  using ::strcspn;
  using ::strspn;
  using ::strtok;
  using ::memset;
  using ::strerror;
  using ::strlen;

  using ::memchr;

  inline void*
  memchr(void* __p, int __c, size_t __n)
  { return memchr(const_cast<const void*>(__p), __c, __n); }

  using ::strchr;

  inline char*
  strchr(char* __s1, int __n)
  { return __builtin_strchr(const_cast<const char*>(__s1), __n); }

  using ::strpbrk;

  inline char*
  strpbrk(char* __s1, const char* __s2)
  { return __builtin_strpbrk(const_cast<const char*>(__s1), __s2); }

  using ::strrchr;

  inline char*
  strrchr(char* __s1, int __n)
  { return __builtin_strrchr(const_cast<const char*>(__s1), __n); }

  using ::strstr;

  inline char*
  strstr(char* __s1, const char* __s2)
  { return __builtin_strstr(const_cast<const char*>(__s1), __s2); }
}
# 66 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 2 3
# 1 "/usr/include/gcc/darwin/3.3/c++/climits" 1 3
# 48 "/usr/include/gcc/darwin/3.3/c++/climits" 3

# 1 "/usr/include/limits.h" 1 3 4
# 62 "/usr/include/limits.h" 3 4
# 1 "/usr/include/gcc/darwin/3.3/machine/limits.h" 1 3 4
# 24 "/usr/include/gcc/darwin/3.3/machine/limits.h" 3 4
# 1 "/usr/include/ppc/limits.h" 1 3 4
# 25 "/usr/include/gcc/darwin/3.3/machine/limits.h" 2 3 4
# 63 "/usr/include/limits.h" 2 3 4
# 1 "/usr/include/sys/syslimits.h" 1 3 4
# 64 "/usr/include/limits.h" 2 3 4
# 50 "/usr/include/gcc/darwin/3.3/c++/climits" 2 3
# 67 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 2 3
# 1 "/usr/include/gcc/darwin/3.3/c++/cstdlib" 1 3
# 48 "/usr/include/gcc/darwin/3.3/c++/cstdlib" 3
# 84 "/usr/include/gcc/darwin/3.3/c++/cstdlib" 3
namespace std
{
  using ::div_t;
  using ::ldiv_t;

  using ::abort;
  using ::abs;
  using ::atexit;
  using ::atof;
  using ::atoi;
  using ::atol;
  using ::bsearch;
  using ::calloc;
  using ::div;
  using ::exit;
  using ::free;
  using ::getenv;
  using ::labs;
  using ::ldiv;
  using ::malloc;
  using ::mblen;
  using ::mbstowcs;
  using ::mbtowc;
  using ::qsort;
  using ::rand;
  using ::realloc;
  using ::srand;
  using ::strtod;
  using ::strtol;
  using ::strtoul;
  using ::system;
  using ::wcstombs;
  using ::wctomb;

  inline long
  abs(long __i) { return labs(__i); }

  inline ldiv_t
  div(long __i, long __j) { return ldiv(__i, __j); }
}
# 68 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 2 3

# 1 "/usr/include/gcc/darwin/3.3/c++/new" 1 3
# 42 "/usr/include/gcc/darwin/3.3/c++/new" 3
# 1 "/usr/include/gcc/darwin/3.3/c++/exception" 1 3
# 40 "/usr/include/gcc/darwin/3.3/c++/exception" 3
extern "C++" {

namespace std
{
# 52 "/usr/include/gcc/darwin/3.3/c++/exception" 3
  class exception
  {
  public:
    exception() throw() { }
    virtual ~exception() throw();


    virtual const char* what() const throw();
  };



  class bad_exception : public exception
  {
  public:
    bad_exception() throw() { }


    virtual ~bad_exception() throw();
  };


  typedef void (*terminate_handler) ();

  typedef void (*unexpected_handler) ();


  terminate_handler set_terminate(terminate_handler) throw();


  void terminate() __attribute__ ((__noreturn__));


  unexpected_handler set_unexpected(unexpected_handler) throw();


  void unexpected() __attribute__ ((__noreturn__));
# 100 "/usr/include/gcc/darwin/3.3/c++/exception" 3
  bool uncaught_exception() throw();
}

namespace __gnu_cxx
{
# 113 "/usr/include/gcc/darwin/3.3/c++/exception" 3
  void __verbose_terminate_handler ();
}

}
# 43 "/usr/include/gcc/darwin/3.3/c++/new" 2 3

extern "C++" {

namespace std
{





  class bad_alloc : public exception
  {
  public:
    bad_alloc() throw() { }


    virtual ~bad_alloc() throw();
  };

  struct nothrow_t { };
  extern const nothrow_t nothrow;


  typedef void (*new_handler)();

  new_handler set_new_handler(new_handler) throw();
}
# 82 "/usr/include/gcc/darwin/3.3/c++/new" 3
void* operator new(std::size_t) throw (std::bad_alloc);
void* operator new[](std::size_t) throw (std::bad_alloc);
void operator delete(void*) throw();
void operator delete[](void*) throw();
void* operator new(std::size_t, const std::nothrow_t&) throw();
void* operator new[](std::size_t, const std::nothrow_t&) throw();
void operator delete(void*, const std::nothrow_t&) throw();
void operator delete[](void*, const std::nothrow_t&) throw();


inline void* operator new(std::size_t, void* __p) throw() { return __p; }
inline void* operator new[](std::size_t, void* __p) throw() { return __p; }


inline void operator delete (void*, void*) throw() { };
inline void operator delete[](void*, void*) throw() { };

}
# 70 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 2 3
# 1 "/usr/include/gcc/darwin/3.3/c++/iosfwd" 1 3
# 44 "/usr/include/gcc/darwin/3.3/c++/iosfwd" 3


# 1 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/c++locale.h" 1 3
# 40 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/c++locale.h" 3

# 1 "/usr/include/gcc/darwin/3.3/c++/clocale" 1 3
# 48 "/usr/include/gcc/darwin/3.3/c++/clocale" 3

# 1 "/usr/include/locale.h" 1 3 4
# 39 "/usr/include/locale.h" 3 4
struct lconv {
        char *decimal_point;
        char *thousands_sep;
        char *grouping;
        char *int_curr_symbol;
        char *currency_symbol;
        char *mon_decimal_point;
        char *mon_thousands_sep;
        char *mon_grouping;
        char *positive_sign;
        char *negative_sign;
        char int_frac_digits;
        char frac_digits;
        char p_cs_precedes;
        char p_sep_by_space;
        char n_cs_precedes;
        char n_sep_by_space;
        char p_sign_posn;
        char n_sign_posn;
};
# 76 "/usr/include/locale.h" 3 4
extern "C" {
struct lconv *localeconv (void);
char *setlocale (int, const char *);
}
# 50 "/usr/include/gcc/darwin/3.3/c++/clocale" 2 3





namespace std
{
  using ::lconv;
  using ::setlocale;
  using ::localeconv;
}
# 42 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/c++locale.h" 2 3



namespace std
{
  typedef int* __c_locale;





  template<typename _Tv>
    int
    __convert_from_v(char* __out, const int __size, const char* __fmt,
                     _Tv __v, const __c_locale&, int __prec = -1)
    {
      char* __old = setlocale(0, 0);
      char* __sav = static_cast<char*>(malloc(strlen(__old) + 1));
      if (__sav)
        strcpy(__sav, __old);
      setlocale(0, "C");

      int __ret;






      if (__prec >= 0)
        __ret = sprintf(__out, __fmt, __prec, __v);
      else
        __ret = sprintf(__out, __fmt, __v);

      setlocale(0, __sav);
      free(__sav);
      return __ret;
    }
}
# 47 "/usr/include/gcc/darwin/3.3/c++/iosfwd" 2 3
# 1 "/usr/include/gcc/darwin/3.3/c++/cctype" 1 3
# 47 "/usr/include/gcc/darwin/3.3/c++/cctype" 3


# 1 "/usr/include/ctype.h" 1 3 4
# 68 "/usr/include/ctype.h" 3 4
# 1 "/usr/include/runetype.h" 1 3 4
# 66 "/usr/include/runetype.h" 3 4
typedef struct {
        rune_t min;
        rune_t max;
        rune_t map;
        unsigned long *types;
} _RuneEntry;

typedef struct {
        int nranges;
        _RuneEntry *ranges;
} _RuneRange;

typedef struct {
        char magic[8];
        char encoding[32];

        rune_t (*sgetrune)
            (const char *, size_t, char const **);
        int (*sputrune)
            (rune_t, char *, size_t, char **);
        rune_t invalid_rune;

        unsigned long runetype[(1 <<8 )];
        rune_t maplower[(1 <<8 )];
        rune_t mapupper[(1 <<8 )];






        _RuneRange runetype_ext;
        _RuneRange maplower_ext;
        _RuneRange mapupper_ext;

        void *variable;
        int variable_len;
} _RuneLocale;



extern _RuneLocale _DefaultRuneLocale;
extern _RuneLocale *_CurrentRuneLocale;
# 69 "/usr/include/ctype.h" 2 3 4
# 100 "/usr/include/ctype.h" 3 4
extern "C" {
int isalnum (int);
int isalpha (int);
int iscntrl (int);
int isdigit (int);
int isgraph (int);
int islower (int);
int isprint (int);
int ispunct (int);
int isspace (int);
int isupper (int);
int isxdigit (int);
int tolower (int);
int toupper (int);


int digittoint (int);
int isascii (int);
int isblank (int);
int ishexnumber (int);
int isideogram (int);
int isnumber (int);
int isphonogram (int);
int isrune (int);
int isspecial (int);
int toascii (int);

}
# 158 "/usr/include/ctype.h" 3 4
extern "C" {
unsigned long ___runetype (int);
int ___tolower (int);
int ___toupper (int);
}
# 180 "/usr/include/ctype.h" 3 4
static inline int
__maskrune(int _c, unsigned long _f)
{
        return ((_c < 0 || _c >= (1 <<8 )) ? ___runetype(_c) :
                _CurrentRuneLocale->runetype[_c]) & _f;
}

static inline int
__istype(int c, unsigned long f)
{
        return !!(__maskrune(c, f));
}

static inline int
__isctype(int _c, unsigned long _f)
{
        return (_c < 0 || _c >= (1 <<8 )) ? 0 :
                !!(_DefaultRuneLocale.runetype[_c] & _f);
}

static inline int
__toupper(int _c)
{
        return (_c < 0 || _c >= (1 <<8 )) ? ___toupper(_c) :
                _CurrentRuneLocale->mapupper[_c];
}

static inline int
__tolower(int _c)
{
        return (_c < 0 || _c >= (1 <<8 )) ? ___tolower(_c) :
                _CurrentRuneLocale->maplower[_c];
}
# 50 "/usr/include/gcc/darwin/3.3/c++/cctype" 2 3
# 71 "/usr/include/gcc/darwin/3.3/c++/cctype" 3
extern "C" {
extern int isalnum(int c);
extern int isalpha(int c);
extern int iscntrl(int c);
extern int isdigit(int c);
extern int isgraph(int c);
extern int islower(int c);
extern int isprint(int c);
extern int ispunct(int c);
extern int isspace(int c);
extern int isupper(int c);
extern int isxdigit(int c);
}



namespace std
{
  using ::isalnum;
  using ::isalpha;
  using ::iscntrl;
  using ::isdigit;
  using ::isgraph;
  using ::islower;
  using ::isprint;
  using ::ispunct;
  using ::isspace;
  using ::isupper;
  using ::isxdigit;
  using ::tolower;
  using ::toupper;
}
# 48 "/usr/include/gcc/darwin/3.3/c++/iosfwd" 2 3
# 1 "/usr/include/gcc/darwin/3.3/c++/bits/stringfwd.h" 1 3
# 43 "/usr/include/gcc/darwin/3.3/c++/bits/stringfwd.h" 3



namespace std
{
  template<typename _Alloc>
    class allocator;

  template<class _CharT>
    struct char_traits;

  template<typename _CharT, typename _Traits = char_traits<_CharT>,
           typename _Alloc = allocator<_CharT> >
    class basic_string;

  template<> struct char_traits<char>;

  typedef basic_string<char> string;






}
# 49 "/usr/include/gcc/darwin/3.3/c++/iosfwd" 2 3
# 1 "/usr/include/gcc/darwin/3.3/c++/bits/fpos.h" 1 3
# 43 "/usr/include/gcc/darwin/3.3/c++/bits/fpos.h" 3

# 1 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/c++io.h" 1 3
# 35 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/c++io.h" 3
# 1 "/usr/include/gcc/darwin/3.3/c++/cstdio" 1 3
# 48 "/usr/include/gcc/darwin/3.3/c++/cstdio" 3




# 1 "/usr/include/stdio.h" 1 3 4
# 91 "/usr/include/stdio.h" 3 4
typedef off_t fpos_t;
# 107 "/usr/include/stdio.h" 3 4
struct __sbuf {
        unsigned char *_base;
        int _size;
};
# 138 "/usr/include/stdio.h" 3 4
typedef struct __sFILE {
        unsigned char *_p;
        int _r;
        int _w;
        short _flags;
        short _file;
        struct __sbuf _bf;
        int _lbfsize;


        void *_cookie;
        int (*_close) (void *);
        int (*_read) (void *, char *, int);
        fpos_t (*_seek) (void *, fpos_t, int);
        int (*_write) (void *, const char *, int);


        struct __sbuf _ub;
        unsigned char *_up;
        int _ur;


        unsigned char _ubuf[3];
        unsigned char _nbuf[1];


        struct __sbuf _lb;


        int _blksize;
        fpos_t _offset;
} FILE;

extern "C" {
extern FILE __sF[];
}
# 241 "/usr/include/stdio.h" 3 4
extern "C" {
void clearerr (FILE *);
int fclose (FILE *);
int feof (FILE *);
int ferror (FILE *);
int fflush (FILE *);
int fgetc (FILE *);
int fgetpos (FILE *, fpos_t *);
char *fgets (char *, int, FILE *);
FILE *fopen (const char *, const char *);
int fprintf (FILE *, const char *, ...);
int fputc (int, FILE *);
int fputs (const char *, FILE *);
size_t fread (void *, size_t, size_t, FILE *);
FILE *freopen (const char *, const char *, FILE *);
int fscanf (FILE *, const char *, ...);
int fseek (FILE *, long, int);
int fsetpos (FILE *, const fpos_t *);
long ftell (FILE *);
size_t fwrite (const void *, size_t, size_t, FILE *);
int getc (FILE *);
int getchar (void);
char *gets (char *);

extern const int sys_nerr;
extern const char *const sys_errlist[];

void perror (const char *);
int printf (const char *, ...);
int putc (int, FILE *);
int putchar (int);
int puts (const char *);
int remove (const char *);
int rename (const char *, const char *);
void rewind (FILE *);
int scanf (const char *, ...);
void setbuf (FILE *, char *);
int setvbuf (FILE *, char *, int, size_t);
int sprintf (char *, const char *, ...);
int sscanf (const char *, const char *, ...);
FILE *tmpfile (void);
char *tmpnam (char *);
int ungetc (int, FILE *);
int vfprintf (FILE *, const char *, __builtin_va_list);
int vprintf (const char *, __builtin_va_list);
int vsprintf (char *, const char *, __builtin_va_list);
int asprintf (char **, const char *, ...);
int vasprintf (char **, const char *, __builtin_va_list);
}
# 298 "/usr/include/stdio.h" 3 4
extern "C" {
char *ctermid (char *);
FILE *fdopen (int, const char *);
int fileno (FILE *);
}






extern "C" {
char *fgetln (FILE *, size_t *);
int fpurge (FILE *);
int fseeko (FILE *, fpos_t, int);
fpos_t ftello (FILE *);
int getw (FILE *);
int pclose (FILE *);
FILE *popen (const char *, const char *);
int putw (int, FILE *);
void setbuffer (FILE *, char *, int);
int setlinebuf (FILE *);
char *tempnam (const char *, const char *);
int snprintf (char *, size_t, const char *, ...);
int vsnprintf (char *, size_t, const char *, __builtin_va_list);
int vscanf (const char *, __builtin_va_list);
int vsscanf (const char *, const char *, __builtin_va_list);
FILE *zopen (const char *, const char *, int);
}
# 338 "/usr/include/stdio.h" 3 4
extern "C" {
FILE *funopen (const void *, int (*)(void *, char *, int), int (*)(void *, const char *, int), fpos_t (*)(void *, fpos_t, int), int (*)(void *));




}







extern "C" {
int __srget (FILE *);
int __svfscanf (FILE *, const char *, __builtin_va_list);
int __swbuf (int, FILE *);
}







static inline int __sputc(int _c, FILE *_p) {
        if (--_p->_w >= 0 || (_p->_w >= _p->_lbfsize && (char)_c != '\n'))
                return (*_p->_p++ = _c);
        else
                return (__swbuf(_c, _p));
}
# 53 "/usr/include/gcc/darwin/3.3/c++/cstdio" 2 3
# 97 "/usr/include/gcc/darwin/3.3/c++/cstdio" 3
namespace std
{
  using ::FILE;
  using ::fpos_t;

  using ::clearerr;
  using ::fclose;
  using ::feof;
  using ::ferror;
  using ::fflush;
  using ::fgetc;
  using ::fgetpos;
  using ::fgets;
  using ::fopen;
  using ::fprintf;
  using ::fputc;
  using ::fputs;
  using ::fread;
  using ::freopen;
  using ::fscanf;
  using ::fseek;
  using ::fsetpos;
  using ::ftell;
  using ::fwrite;
  using ::getc;
  using ::getchar;
  using ::gets;
  using ::perror;
  using ::printf;
  using ::putc;
  using ::putchar;
  using ::puts;
  using ::remove;
  using ::rename;
  using ::rewind;
  using ::scanf;
  using ::setbuf;
  using ::setvbuf;
  using ::sprintf;
  using ::sscanf;
  using ::tmpfile;
  using ::tmpnam;
  using ::ungetc;
  using ::vfprintf;
  using ::vprintf;
  using ::vsprintf;
}
# 36 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/c++io.h" 2 3

# 1 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/gthr.h" 1 3
# 98 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/gthr.h" 3
# 1 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/gthr-default.h" 1 3
# 37 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/gthr-default.h" 3
# 1 "/usr/include/pthread.h" 1 3 4
# 34 "/usr/include/pthread.h" 3 4
# 1 "/usr/include/pthread_impl.h" 1 3 4
# 35 "/usr/include/pthread.h" 2 3 4

# 1 "/usr/include/errno.h" 1 3 4
# 22 "/usr/include/errno.h" 3 4
# 1 "/usr/include/sys/errno.h" 1 3 4
# 68 "/usr/include/sys/errno.h" 3 4
extern "C" {
extern int * __error (void);

}
# 23 "/usr/include/errno.h" 2 3 4
# 37 "/usr/include/pthread.h" 2 3 4
# 1 "/usr/include/sched.h" 1 3 4
# 10 "/usr/include/sched.h" 3 4
struct sched_param { int sched_priority; char opaque[4]; };


extern int sched_yield(void);
extern int sched_get_priority_min(int);
extern int sched_get_priority_max(int);
# 38 "/usr/include/pthread.h" 2 3 4
# 1 "/usr/include/time.h" 1 3 4
# 88 "/usr/include/time.h" 3 4
struct timespec {
        time_t tv_sec;
        long tv_nsec;
};


struct tm {
        int tm_sec;
        int tm_min;
        int tm_hour;
        int tm_mday;
        int tm_mon;
        int tm_year;
        int tm_wday;
        int tm_yday;
        int tm_isdst;
        long tm_gmtoff;
        char *tm_zone;
};
# 115 "/usr/include/time.h" 3 4
extern char *tzname[];


extern "C" {
char *asctime (const struct tm *);
clock_t clock (void);
char *ctime (const time_t *);
double difftime (time_t, time_t);
struct tm *gmtime (const time_t *);
struct tm *localtime (const time_t *);
time_t mktime (struct tm *);
size_t strftime (char *, size_t, const char *, const struct tm *);
time_t time (time_t *);


void tzset (void);



char *asctime_r (const struct tm *, char *);
char *ctime_r (const time_t *, char *);
struct tm *gmtime_r (const time_t *, struct tm *);
struct tm *localtime_r (const time_t *, struct tm *);
char *strptime (const char *, const char *, struct tm *);
char *timezone (int, int);
void tzsetwall (void);
time_t timelocal (struct tm * const);
time_t timegm (struct tm * const);



int nanosleep (const struct timespec *, struct timespec *);

}
# 39 "/usr/include/pthread.h" 2 3 4
# 1 "/usr/include/unistd.h" 1 3 4
# 72 "/usr/include/unistd.h" 3 4
# 1 "/usr/include/sys/unistd.h" 1 3 4
# 73 "/usr/include/unistd.h" 2 3 4
# 91 "/usr/include/unistd.h" 3 4
extern "C" {
 void
         _exit (int);
int access (const char *, int);
unsigned int alarm (unsigned int);
int chdir (const char *);
int chown (const char *, uid_t, gid_t);
int close (int);
size_t confstr (int, char *, size_t);
int dup (int);
int dup2 (int, int);
int execl (const char *, const char *, ...);
int execle (const char *, const char *, ...);
int execlp (const char *, const char *, ...);
int execv (const char *, char * const *);
int execve (const char *, char * const *, char * const *);
int execvp (const char *, char * const *);
pid_t fork (void);
long fpathconf (int, int);
char *getcwd (char *, size_t);
gid_t getegid (void);
uid_t geteuid (void);
gid_t getgid (void);
int getgroups (int, gid_t []);
char *getlogin (void);
pid_t getpgrp (void);
pid_t getpid (void);
pid_t getppid (void);
uid_t getuid (void);
int isatty (int);
int link (const char *, const char *);
off_t lseek (int, off_t, int);
long pathconf (const char *, int);
int pause (void);
int pipe (int *);
ssize_t read (int, void *, size_t);
int rmdir (const char *);
int setgid (gid_t);
int setpgid (pid_t, pid_t);
pid_t setsid (void);
int setuid (uid_t);
unsigned int sleep (unsigned int);
long sysconf (int);
pid_t tcgetpgrp (int);
int tcsetpgrp (int, pid_t);
char *ttyname (int);
int unlink (const char *);
ssize_t write (int, const void *, size_t);

extern char *optarg;
extern int optind, opterr, optopt, optreset;
int getopt (int, char * const [], const char *);



struct timeval;

int acct (const char *);
int async_daemon (void);
char *brk (const char *);
int chroot (const char *);
char *crypt (const char *, const char *);
int des_cipher (const char *, char *, long, int);
int des_setkey (const char *key);
int encrypt (char *, int);
void endusershell (void);
int exect (const char *, char * const *, char * const *);
int fchdir (int);
int fchown (int, int, int);
char *fflagstostr (u_long);
int fsync (int);
int ftruncate (int, off_t);
int getdtablesize (void);
int getgrouplist (const char *, int, int *, int *);
long gethostid (void);
int gethostname (char *, int);
mode_t getmode (const void *, mode_t);
 int
         getpagesize (void);
char *getpass (const char *);
int getpgid (pid_t _pid);
int getsid (pid_t _pid);
char *getusershell (void);
char *getwd (char *);
int initgroups (const char *, int);
int iruserok (unsigned long, int, const char *, const char *);
int issetugid (void);
char *mkdtemp (char *);
int mknod (const char *, mode_t, dev_t);
int mkstemp (char *);
int mkstemps (char *, int);
char *mktemp (char *);
int nfssvc (int, void *);
int nice (int);
ssize_t pread (int, void *, size_t, off_t);




# 1 "/usr/include/signal.h" 1 3 4
# 62 "/usr/include/signal.h" 3 4
# 1 "/usr/include/sys/signal.h" 1 3 4
# 72 "/usr/include/sys/signal.h" 3 4
# 1 "/usr/include/machine/signal.h" 1 3 4
# 27 "/usr/include/machine/signal.h" 3 4
# 1 "/usr/include/ppc/signal.h" 1 3 4
# 32 "/usr/include/ppc/signal.h" 3 4
typedef int sig_atomic_t;
# 50 "/usr/include/ppc/signal.h" 3 4
typedef enum {
        REGS_SAVED_NONE,
        REGS_SAVED_CALLER,


        REGS_SAVED_ALL
} regs_saved_t;
# 66 "/usr/include/ppc/signal.h" 3 4
struct sigcontext {
    int sc_onstack;
    int sc_mask;
        int sc_ir;
    int sc_psw;
    int sc_sp;
        void *sc_regs;
};
# 28 "/usr/include/machine/signal.h" 2 3 4
# 73 "/usr/include/sys/signal.h" 2 3 4
# 134 "/usr/include/sys/signal.h" 3 4
typedef unsigned int sigset_t;

union sigval {

        int sigval_int;
        void *sigval_ptr;
};







typedef struct __siginfo {
        int si_signo;
        int si_errno;
        int si_code;
        int si_pid;
        unsigned int si_uid;
        int si_status;
        void *si_addr;
        union sigval si_value;
        long si_band;
        int pad[7];
} siginfo_t;
# 208 "/usr/include/sys/signal.h" 3 4
union __sigaction_u {
        void (*__sa_handler)(int);
        void (*__sa_sigaction)(int, struct __siginfo *,
                       void *);
};


struct __sigaction {
        union __sigaction_u __sigaction_u;
        void (*sa_tramp)(void *, int, int, siginfo_t *, void *);
        sigset_t sa_mask;
        int sa_flags;
};




struct sigaction {
        union __sigaction_u __sigaction_u;
        sigset_t sa_mask;
        int sa_flags;
};
# 263 "/usr/include/sys/signal.h" 3 4
typedef void (*sig_t) (int);




struct sigaltstack {
        char *ss_sp;
        int ss_size;
        int ss_flags;
};

typedef struct sigaltstack stack_t;
# 285 "/usr/include/sys/signal.h" 3 4
struct sigvec {
        void (*sv_handler)(int);
        int sv_mask;
        int sv_flags;
};
# 303 "/usr/include/sys/signal.h" 3 4
struct sigstack {
        char *ss_sp;
        int ss_onstack;
};
# 336 "/usr/include/sys/signal.h" 3 4
extern "C" {
void (*signal (int, void (*) (int))) (int);
}
# 63 "/usr/include/signal.h" 2 3 4


extern const char *const sys_signame[32];
extern const char *const sys_siglist[32];


extern "C" {
int raise (int);

int kill (pid_t, int);
int sigaction (int, const struct sigaction *, struct sigaction *);
int sigaddset (sigset_t *, int);
int sigdelset (sigset_t *, int);
int sigemptyset (sigset_t *);
int sigfillset (sigset_t *);
int sigismember (const sigset_t *, int);
int sigpending (sigset_t *);
int sigprocmask (int, const sigset_t *, sigset_t *);
int sigsuspend (const sigset_t *);

int killpg (pid_t, int);
int sigblock (int);
int siginterrupt (int, int);
int sighold (int);
int sigrelse (int);
int sigpause (int);
int sigreturn (struct sigcontext *);
int sigsetmask (int);
int sigvec (int, struct sigvec *, struct sigvec *);
void psignal (unsigned int, const char *);


}
# 191 "/usr/include/unistd.h" 2 3 4

int profil (char *, int, int, int);
ssize_t pwrite (int, const void *, size_t, off_t);
int rcmd (char **, int, const char *, const char *, const char *, int *);

char *re_comp (const char *);
int re_exec (const char *);
int readlink (const char *, char *, int);
int reboot (int);
int revoke (const char *);
int rresvport (int *);
int ruserok (const char *, int, const char *, const char *);
char *sbrk (int);
int select (int, fd_set *, fd_set *, fd_set *, struct timeval *);
int setegid (gid_t);
int seteuid (uid_t);
int setgroups (int, const gid_t *);
void sethostid (long);
int sethostname (const char *, int);
int setkey (const char *);
int setlogin (const char *);
void *setmode (const char *);
int setpgrp (pid_t pid, pid_t pgrp);
int setregid (gid_t, gid_t);
int setreuid (uid_t, uid_t);
int setrgid (gid_t);
int setruid (uid_t);
void setusershell (void);
int strtofflags (char **, u_long *, u_long *);
int swapon (const char *);
int symlink (const char *, const char *);
void sync (void);
int syscall (int, ...);
int truncate (const char *, off_t);
int ttyslot (void);
unsigned int ualarm (unsigned int, unsigned int);
int unwhiteout (const char *);
int usleep (unsigned int);
void *valloc (size_t);
pid_t vfork (void);

extern char *suboptarg;
int getsubopt (char **, char * const *, char **);


int getattrlist (const char*,void*,void*,size_t,unsigned long);
int setattrlist (const char*,void*,void*,size_t,unsigned long);
int exchangedata (const char*,const char*,unsigned long);
int checkuseraccess (const char*,uid_t,gid_t*,int,int,unsigned long);
int getdirentriesattr (int,void*,void*,size_t,unsigned long*,unsigned long*,unsigned long*,unsigned long);
int searchfs (const char*,void*,void*,unsigned long,unsigned long,void*);

int fsctl (const char *,unsigned long,void*,unsigned long);



}
# 40 "/usr/include/pthread.h" 2 3 4




# 1 "/usr/include/mach/mach_types.h" 1 3 4
# 64 "/usr/include/mach/mach_types.h" 3 4
# 1 "/usr/include/gcc/darwin/3.3/stdint.h" 1 3 4
# 34 "/usr/include/gcc/darwin/3.3/stdint.h" 3 4
typedef u_int8_t uint8_t;
typedef u_int16_t uint16_t;
typedef u_int32_t uint32_t;
typedef u_int64_t uint64_t;



typedef int8_t int_least8_t;
typedef int16_t int_least16_t;
typedef int32_t int_least32_t;
typedef int64_t int_least64_t;
typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;



typedef int8_t int_fast8_t;
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef int64_t int_fast64_t;
typedef uint8_t uint_fast8_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
typedef uint64_t uint_fast64_t;
# 68 "/usr/include/gcc/darwin/3.3/stdint.h" 3 4
typedef long long intmax_t;
typedef unsigned long long uintmax_t;
# 65 "/usr/include/mach/mach_types.h" 2 3 4

# 1 "/usr/include/mach/host_info.h" 1 3 4
# 62 "/usr/include/mach/host_info.h" 3 4
# 1 "/usr/include/mach/vm_statistics.h" 1 3 4
# 63 "/usr/include/mach/vm_statistics.h" 3 4
# 1 "/usr/include/mach/machine/vm_types.h" 1 3 4
# 27 "/usr/include/mach/machine/vm_types.h" 3 4
# 1 "/usr/include/mach/ppc/vm_types.h" 1 3 4
# 77 "/usr/include/mach/ppc/vm_types.h" 3 4
typedef unsigned int natural_t;
# 86 "/usr/include/mach/ppc/vm_types.h" 3 4
typedef int integer_t;





typedef natural_t vm_offset_t;






typedef natural_t vm_size_t;




typedef unsigned int space_t;
# 28 "/usr/include/mach/machine/vm_types.h" 2 3 4
# 64 "/usr/include/mach/vm_statistics.h" 2 3 4

struct vm_statistics {
        integer_t free_count;
        integer_t active_count;
        integer_t inactive_count;
        integer_t wire_count;
        integer_t zero_fill_count;
        integer_t reactivations;
        integer_t pageins;
        integer_t pageouts;
        integer_t faults;
        integer_t cow_faults;
        integer_t lookups;
        integer_t hits;
};

typedef struct vm_statistics *vm_statistics_t;
typedef struct vm_statistics vm_statistics_data_t;
# 99 "/usr/include/mach/vm_statistics.h" 3 4
struct pmap_statistics {
        integer_t resident_count;
        integer_t wired_count;
};

typedef struct pmap_statistics *pmap_statistics_t;
# 63 "/usr/include/mach/host_info.h" 2 3 4
# 1 "/usr/include/mach/machine.h" 1 3 4
# 60 "/usr/include/mach/machine.h" 3 4
# 1 "/usr/include/mach/boolean.h" 1 3 4
# 127 "/usr/include/mach/boolean.h" 3 4
# 1 "/usr/include/mach/machine/boolean.h" 1 3 4
# 27 "/usr/include/mach/machine/boolean.h" 3 4
# 1 "/usr/include/mach/ppc/boolean.h" 1 3 4
# 129 "/usr/include/mach/ppc/boolean.h" 3 4
typedef int boolean_t;
# 28 "/usr/include/mach/machine/boolean.h" 2 3 4
# 128 "/usr/include/mach/boolean.h" 2 3 4
# 61 "/usr/include/mach/machine.h" 2 3 4
# 71 "/usr/include/mach/machine.h" 3 4
struct machine_info {
        integer_t major_version;
        integer_t minor_version;
        integer_t max_cpus;
        integer_t avail_cpus;
        vm_size_t memory_size;
};

typedef struct machine_info *machine_info_t;
typedef struct machine_info machine_info_data_t;

typedef integer_t cpu_type_t;
typedef integer_t cpu_subtype_t;
# 64 "/usr/include/mach/host_info.h" 2 3 4

# 1 "/usr/include/mach/time_value.h" 1 3 4
# 62 "/usr/include/mach/time_value.h" 3 4
struct time_value {
        integer_t seconds;
        integer_t microseconds;
};
typedef struct time_value time_value_t;
# 106 "/usr/include/mach/time_value.h" 3 4
typedef struct mapped_time_value {
        integer_t seconds;
        integer_t microseconds;
        integer_t check_seconds;
} mapped_time_value_t;
# 66 "/usr/include/mach/host_info.h" 2 3 4




typedef integer_t *host_info_t;


typedef integer_t host_info_data_t[(1024)];


typedef char kernel_version_t[(512)];


typedef char kernel_boot_info_t[(4096)];







typedef integer_t host_flavor_t;







struct host_basic_info {
        integer_t max_cpus;
        integer_t avail_cpus;
        vm_size_t memory_size;
        cpu_type_t cpu_type;
        cpu_subtype_t cpu_subtype;
};

typedef struct host_basic_info host_basic_info_data_t;
typedef struct host_basic_info *host_basic_info_t;



struct host_sched_info {
        integer_t min_timeout;
        integer_t min_quantum;
};

typedef struct host_sched_info host_sched_info_data_t;
typedef struct host_sched_info *host_sched_info_t;



struct kernel_resource_sizes {
        vm_size_t task;
        vm_size_t thread;
        vm_size_t port;
        vm_size_t memory_region;
        vm_size_t memory_object;
};

typedef struct kernel_resource_sizes kernel_resource_sizes_data_t;
typedef struct kernel_resource_sizes *kernel_resource_sizes_t;



struct host_priority_info {
        integer_t kernel_priority;
        integer_t system_priority;
        integer_t server_priority;
        integer_t user_priority;
        integer_t depress_priority;
        integer_t idle_priority;
        integer_t minimum_priority;
        integer_t maximum_priority;
};

typedef struct host_priority_info host_priority_info_data_t;
typedef struct host_priority_info *host_priority_info_t;
# 152 "/usr/include/mach/host_info.h" 3 4
struct host_load_info {
        integer_t avenrun[3];
        integer_t mach_factor[3];
};

typedef struct host_load_info host_load_info_data_t;
typedef struct host_load_info *host_load_info_t;







struct host_cpu_load_info {
        unsigned long cpu_ticks[4];
};
typedef struct host_cpu_load_info host_cpu_load_info_data_t;
typedef struct host_cpu_load_info *host_cpu_load_info_t;
# 67 "/usr/include/mach/mach_types.h" 2 3 4


# 1 "/usr/include/mach/memory_object_types.h" 1 3 4
# 67 "/usr/include/mach/memory_object_types.h" 3 4
# 1 "/usr/include/mach/port.h" 1 3 4
# 91 "/usr/include/mach/port.h" 3 4
typedef natural_t port_name_t;
typedef port_name_t *port_name_array_t;


typedef port_name_t port_t;
# 117 "/usr/include/mach/port.h" 3 4
typedef port_t mach_port_t;
typedef port_t *mach_port_array_t;
typedef port_name_t mach_port_name_t;
typedef mach_port_name_t *mach_port_name_array_t;
# 165 "/usr/include/mach/port.h" 3 4
typedef natural_t mach_port_right_t;
# 174 "/usr/include/mach/port.h" 3 4
typedef natural_t mach_port_type_t;
typedef mach_port_type_t *mach_port_type_array_t;
# 206 "/usr/include/mach/port.h" 3 4
typedef natural_t mach_port_urefs_t;
typedef integer_t mach_port_delta_t;



typedef natural_t mach_port_seqno_t;
typedef natural_t mach_port_mscount_t;
typedef natural_t mach_port_msgcount_t;
typedef natural_t mach_port_rights_t;






typedef unsigned int mach_port_srights_t;

typedef struct mach_port_status {
        mach_port_name_t mps_pset;
        mach_port_seqno_t mps_seqno;
        mach_port_mscount_t mps_mscount;
        mach_port_msgcount_t mps_qlimit;
        mach_port_msgcount_t mps_msgcount;
        mach_port_rights_t mps_sorights;
        boolean_t mps_srights;
        boolean_t mps_pdrequest;
        boolean_t mps_nsrequest;
        unsigned int mps_flags;
} mach_port_status_t;




typedef struct mach_port_limits {
        mach_port_msgcount_t mpl_qlimit;
} mach_port_limits_t;

typedef integer_t *mach_port_info_t;


typedef int mach_port_flavor_t;
# 262 "/usr/include/mach/port.h" 3 4
typedef struct mach_port_qos {
        boolean_t name:1;
        boolean_t prealloc:1;
        boolean_t pad1:30;
        natural_t len;
} mach_port_qos_t;
# 68 "/usr/include/mach/memory_object_types.h" 2 3 4
# 1 "/usr/include/mach/vm_types.h" 1 3 4
# 35 "/usr/include/mach/vm_types.h" 3 4
typedef vm_offset_t pointer_t;
typedef vm_offset_t vm_address_t;
typedef uint64_t vm_object_offset_t;


typedef mach_port_t vm_map_t;
# 49 "/usr/include/mach/vm_types.h" 3 4
typedef mach_port_t upl_t;
typedef mach_port_t vm_named_entry_t;
# 69 "/usr/include/mach/memory_object_types.h" 2 3 4






typedef unsigned long long memory_object_offset_t;
typedef unsigned long long memory_object_size_t;






typedef mach_port_t memory_object_t;
typedef mach_port_t memory_object_control_t;


typedef memory_object_t *memory_object_array_t;




typedef mach_port_t memory_object_name_t;



typedef mach_port_t memory_object_default_t;
# 106 "/usr/include/mach/memory_object_types.h" 3 4
typedef int memory_object_copy_strategy_t;
# 142 "/usr/include/mach/memory_object_types.h" 3 4
typedef int memory_object_return_t;
# 169 "/usr/include/mach/memory_object_types.h" 3 4
typedef int *memory_object_info_t;
typedef int memory_object_flavor_t;
typedef int memory_object_info_data_t[(1024)];
# 182 "/usr/include/mach/memory_object_types.h" 3 4
struct old_memory_object_behave_info {
        memory_object_copy_strategy_t copy_strategy;
        boolean_t temporary;
        boolean_t invalidate;
};

struct old_memory_object_attr_info {
        boolean_t object_ready;
        boolean_t may_cache;
        memory_object_copy_strategy_t copy_strategy;
};

typedef struct old_memory_object_behave_info *old_memory_object_behave_info_t;
typedef struct old_memory_object_behave_info old_memory_object_behave_info_data_t;
typedef struct old_memory_object_attr_info *old_memory_object_attr_info_t;
typedef struct old_memory_object_attr_info old_memory_object_attr_info_data_t;







struct memory_object_perf_info {
        vm_size_t cluster_size;
        boolean_t may_cache;
};

struct memory_object_attr_info {
        memory_object_copy_strategy_t copy_strategy;
        vm_offset_t cluster_size;
        boolean_t may_cache_object;
        boolean_t temporary;
};

struct memory_object_behave_info {
        memory_object_copy_strategy_t copy_strategy;
        boolean_t temporary;
        boolean_t invalidate;
        boolean_t silent_overwrite;
        boolean_t advisory_pageout;
};


typedef struct memory_object_behave_info *memory_object_behave_info_t;
typedef struct memory_object_behave_info memory_object_behave_info_data_t;

typedef struct memory_object_perf_info *memory_object_perf_info_t;
typedef struct memory_object_perf_info memory_object_perf_info_data_t;

typedef struct memory_object_attr_info *memory_object_attr_info_t;
typedef struct memory_object_attr_info memory_object_attr_info_data_t;
# 264 "/usr/include/mach/memory_object_types.h" 3 4
struct upl_page_info {
        vm_offset_t phys_addr;
        unsigned int
                        pageout:1,
                        absent:1,
                        dirty:1,
                        precious:1,
                        device:1,
                        :0;
};

typedef struct upl_page_info upl_page_info_t;
typedef upl_page_info_t *upl_page_info_array_t;
typedef upl_page_info_array_t upl_page_list_ptr_t;
# 70 "/usr/include/mach/mach_types.h" 2 3 4
# 1 "/usr/include/mach/exception_types.h" 1 3 4
# 56 "/usr/include/mach/exception_types.h" 3 4
# 1 "/usr/include/mach/machine/exception.h" 1 3 4
# 27 "/usr/include/mach/machine/exception.h" 3 4
# 1 "/usr/include/mach/ppc/exception.h" 1 3 4
# 28 "/usr/include/mach/machine/exception.h" 2 3 4
# 57 "/usr/include/mach/exception_types.h" 2 3 4
# 147 "/usr/include/mach/exception_types.h" 3 4
# 1 "/usr/include/mach/thread_status.h" 1 3 4
# 70 "/usr/include/mach/thread_status.h" 3 4
# 1 "/usr/include/mach/machine/thread_status.h" 1 3 4
# 27 "/usr/include/mach/machine/thread_status.h" 3 4
# 1 "/usr/include/mach/ppc/thread_status.h" 1 3 4
# 60 "/usr/include/mach/ppc/thread_status.h" 3 4
typedef struct ppc_thread_state {
        unsigned int srr0;
        unsigned int srr1;
        unsigned int r0;
        unsigned int r1;
        unsigned int r2;
        unsigned int r3;
        unsigned int r4;
        unsigned int r5;
        unsigned int r6;
        unsigned int r7;
        unsigned int r8;
        unsigned int r9;
        unsigned int r10;
        unsigned int r11;
        unsigned int r12;
        unsigned int r13;
        unsigned int r14;
        unsigned int r15;
        unsigned int r16;
        unsigned int r17;
        unsigned int r18;
        unsigned int r19;
        unsigned int r20;
        unsigned int r21;
        unsigned int r22;
        unsigned int r23;
        unsigned int r24;
        unsigned int r25;
        unsigned int r26;
        unsigned int r27;
        unsigned int r28;
        unsigned int r29;
        unsigned int r30;
        unsigned int r31;

        unsigned int cr;
        unsigned int xer;
        unsigned int lr;
        unsigned int ctr;
        unsigned int mq;

        unsigned int vrsave;
} ppc_thread_state_t;



typedef struct ppc_float_state {
        double fpregs[32];

        unsigned int fpscr_pad;
        unsigned int fpscr;
} ppc_float_state_t;

typedef struct ppc_vector_state {
        unsigned long save_vr[32][4];
        unsigned long save_vscr[4];
        unsigned int save_pad5[4];
        unsigned int save_vrvalid;
        unsigned int save_pad6[7];
} ppc_vector_state_t;
# 132 "/usr/include/mach/ppc/thread_status.h" 3 4
typedef struct ppc_thread_state ppc_saved_state_t;
# 148 "/usr/include/mach/ppc/thread_status.h" 3 4
typedef struct ppc_exception_state {
        unsigned long dar;
        unsigned long dsisr;
        unsigned long exception;
        unsigned long pad0;

        unsigned long pad1[4];
} ppc_exception_state_t;
# 28 "/usr/include/mach/machine/thread_status.h" 2 3 4
# 71 "/usr/include/mach/thread_status.h" 2 3 4
# 1 "/usr/include/mach/machine/thread_state.h" 1 3 4
# 27 "/usr/include/mach/machine/thread_state.h" 3 4
# 1 "/usr/include/mach/ppc/thread_state.h" 1 3 4
# 28 "/usr/include/mach/machine/thread_state.h" 2 3 4
# 72 "/usr/include/mach/thread_status.h" 2 3 4





typedef natural_t *thread_state_t;


typedef int thread_state_data_t[(144)];



typedef int thread_state_flavor_t;
typedef thread_state_flavor_t *thread_state_flavor_array_t;
# 148 "/usr/include/mach/exception_types.h" 2 3 4





typedef int exception_type_t;
typedef integer_t exception_data_type_t;
typedef int exception_behavior_t;
typedef integer_t *exception_data_t;
typedef unsigned int exception_mask_t;
typedef exception_mask_t *exception_mask_array_t;
typedef exception_behavior_t *exception_behavior_array_t;
typedef thread_state_flavor_t *exception_flavor_array_t;
typedef mach_port_t *exception_port_array_t;
# 71 "/usr/include/mach/mach_types.h" 2 3 4

# 1 "/usr/include/mach/processor_info.h" 1 3 4
# 65 "/usr/include/mach/processor_info.h" 3 4
# 1 "/usr/include/mach/machine/processor_info.h" 1 3 4
# 27 "/usr/include/mach/machine/processor_info.h" 3 4
# 1 "/usr/include/mach/ppc/processor_info.h" 1 3 4
# 43 "/usr/include/mach/ppc/processor_info.h" 3 4
typedef union {
        unsigned int word;
        struct {
                unsigned int dis : 1;
                unsigned int dp : 1;
                unsigned int du : 1;
                unsigned int dms : 1;
                unsigned int dmr : 1;
                unsigned int reserved3 : 1;
                unsigned int reserved4 : 1;
                unsigned int reserved5 : 2;
                unsigned int reserved6 : 1;
                unsigned int threshold : 6;
                unsigned int reserved7 : 1;
                unsigned int reserved8 : 1;
                unsigned int reserved9 : 1;
                unsigned int pmc1select : 7;
                unsigned int pmc2select : 6;
        }bits;
}mmcr0_t;

typedef union {
        unsigned int word;
        struct {
                unsigned int pmc3select : 5;
                unsigned int pmc4select : 5;
                unsigned int reserved : 22;
        }bits;
}mmcr1_t;

typedef union {
        unsigned int word;
        struct {
                unsigned int threshmult : 1;
                unsigned int reserved : 31;
        }bits;
}mmcr2_t;

typedef union {
        unsigned int word;
        struct {
                unsigned int ov : 1;
                unsigned int cv : 31;
        }bits;
}pmcn_t;





struct processor_pm_regs {
      union {
        mmcr0_t mmcr0;
        mmcr1_t mmcr1;
        mmcr2_t mmcr2;
      }u;
      pmcn_t pmc[2];
};

typedef struct processor_pm_regs processor_pm_regs_data_t;
typedef struct processor_pm_regs *processor_pm_regs_t;
# 119 "/usr/include/mach/ppc/processor_info.h" 3 4
typedef unsigned int processor_temperature_data_t;
typedef unsigned int *processor_temperature_t;



union processor_control_data {
        processor_pm_regs_data_t cmd_pm_regs[3];
};

struct processor_control_cmd {
    integer_t cmd_op;
    cpu_type_t cmd_cpu_type;
    cpu_subtype_t cmd_cpu_subtype;
    union processor_control_data u;
};

typedef struct processor_control_cmd processor_control_cmd_data_t;
typedef struct processor_control_cmd *processor_control_cmd_t;
# 28 "/usr/include/mach/machine/processor_info.h" 2 3 4
# 66 "/usr/include/mach/processor_info.h" 2 3 4




typedef integer_t *processor_info_t;
typedef integer_t *processor_info_array_t;


typedef integer_t processor_info_data_t[(1024)];


typedef integer_t *processor_set_info_t;


typedef integer_t processor_set_info_data_t[(1024)];




typedef int processor_flavor_t;





struct processor_basic_info {
        cpu_type_t cpu_type;
        cpu_subtype_t cpu_subtype;
        boolean_t running;
        int slot_num;
        boolean_t is_master;
};

typedef struct processor_basic_info processor_basic_info_data_t;
typedef struct processor_basic_info *processor_basic_info_t;



struct processor_cpu_load_info {
        unsigned long cpu_ticks[4];
};

typedef struct processor_cpu_load_info processor_cpu_load_info_data_t;
typedef struct processor_cpu_load_info *processor_cpu_load_info_t;
# 118 "/usr/include/mach/processor_info.h" 3 4
typedef int processor_set_flavor_t;


struct processor_set_basic_info {
        int processor_count;
        int default_policy;
};

typedef struct processor_set_basic_info processor_set_basic_info_data_t;
typedef struct processor_set_basic_info *processor_set_basic_info_t;





struct processor_set_load_info {
        int task_count;
        int thread_count;
        integer_t load_average;
        integer_t mach_factor;
};

typedef struct processor_set_load_info processor_set_load_info_data_t;
typedef struct processor_set_load_info *processor_set_load_info_t;
# 73 "/usr/include/mach/mach_types.h" 2 3 4
# 1 "/usr/include/mach/task_info.h" 1 3 4
# 66 "/usr/include/mach/task_info.h" 3 4
# 1 "/usr/include/mach/policy.h" 1 3 4
# 72 "/usr/include/mach/policy.h" 3 4
typedef int policy_t;
typedef integer_t *policy_info_t;
typedef integer_t *policy_base_t;
typedef integer_t *policy_limit_t;
# 108 "/usr/include/mach/policy.h" 3 4
struct policy_timeshare_base {
        integer_t base_priority;
};
struct policy_timeshare_limit {
        integer_t max_priority;
};
struct policy_timeshare_info {
        integer_t max_priority;
        integer_t base_priority;
        integer_t cur_priority;
        boolean_t depressed;
        integer_t depress_priority;
};

typedef struct policy_timeshare_base *policy_timeshare_base_t;
typedef struct policy_timeshare_limit *policy_timeshare_limit_t;
typedef struct policy_timeshare_info *policy_timeshare_info_t;

typedef struct policy_timeshare_base policy_timeshare_base_data_t;
typedef struct policy_timeshare_limit policy_timeshare_limit_data_t;
typedef struct policy_timeshare_info policy_timeshare_info_data_t;
# 142 "/usr/include/mach/policy.h" 3 4
struct policy_rr_base {
        integer_t base_priority;
        integer_t quantum;
};
struct policy_rr_limit {
        integer_t max_priority;
};
struct policy_rr_info {
        integer_t max_priority;
        integer_t base_priority;
        integer_t quantum;
        boolean_t depressed;
        integer_t depress_priority;
};

typedef struct policy_rr_base *policy_rr_base_t;
typedef struct policy_rr_limit *policy_rr_limit_t;
typedef struct policy_rr_info *policy_rr_info_t;

typedef struct policy_rr_base policy_rr_base_data_t;
typedef struct policy_rr_limit policy_rr_limit_data_t;
typedef struct policy_rr_info policy_rr_info_data_t;
# 176 "/usr/include/mach/policy.h" 3 4
struct policy_fifo_base {
        integer_t base_priority;
};
struct policy_fifo_limit {
        integer_t max_priority;
};
struct policy_fifo_info {
        integer_t max_priority;
        integer_t base_priority;
        boolean_t depressed;
        integer_t depress_priority;
};

typedef struct policy_fifo_base *policy_fifo_base_t;
typedef struct policy_fifo_limit *policy_fifo_limit_t;
typedef struct policy_fifo_info *policy_fifo_info_t;

typedef struct policy_fifo_base policy_fifo_base_data_t;
typedef struct policy_fifo_limit policy_fifo_limit_data_t;
typedef struct policy_fifo_info policy_fifo_info_data_t;
# 208 "/usr/include/mach/policy.h" 3 4
struct policy_bases {
        policy_timeshare_base_data_t ts;
        policy_rr_base_data_t rr;
        policy_fifo_base_data_t fifo;
};

struct policy_limits {
        policy_timeshare_limit_data_t ts;
        policy_rr_limit_data_t rr;
        policy_fifo_limit_data_t fifo;
};

struct policy_infos {
        policy_timeshare_info_data_t ts;
        policy_rr_info_data_t rr;
        policy_fifo_info_data_t fifo;
};

typedef struct policy_bases policy_base_data_t;
typedef struct policy_limits policy_limit_data_t;
typedef struct policy_infos policy_info_data_t;
# 67 "/usr/include/mach/task_info.h" 2 3 4






typedef natural_t task_flavor_t;
typedef integer_t *task_info_t;


typedef integer_t task_info_data_t[(1024)];







struct task_basic_info {
        integer_t suspend_count;
        vm_size_t virtual_size;
        vm_size_t resident_size;
        time_value_t user_time;

        time_value_t system_time;

        policy_t policy;
};

typedef struct task_basic_info task_basic_info_data_t;
typedef struct task_basic_info *task_basic_info_t;






struct task_events_info {
        integer_t faults;
        integer_t pageins;
        integer_t cow_faults;
        integer_t messages_sent;
        integer_t messages_received;
        integer_t syscalls_mach;
        integer_t syscalls_unix;
        integer_t csw;
};
typedef struct task_events_info task_events_info_data_t;
typedef struct task_events_info *task_events_info_t;






struct task_thread_times_info {
        time_value_t user_time;

        time_value_t system_time;

};

typedef struct task_thread_times_info task_thread_times_info_data_t;
typedef struct task_thread_times_info *task_thread_times_info_t;
# 74 "/usr/include/mach/mach_types.h" 2 3 4
# 1 "/usr/include/mach/task_policy.h" 1 3 4
# 37 "/usr/include/mach/task_policy.h" 3 4
# 1 "/usr/include/mach/mach_types.h" 1 3 4
# 38 "/usr/include/mach/task_policy.h" 2 3 4
# 56 "/usr/include/mach/task_policy.h" 3 4
typedef natural_t task_policy_flavor_t;
typedef integer_t *task_policy_t;
# 111 "/usr/include/mach/task_policy.h" 3 4
enum task_role {
        TASK_RENICED = -1,
        TASK_UNSPECIFIED = 0,
        TASK_FOREGROUND_APPLICATION,
        TASK_BACKGROUND_APPLICATION,
        TASK_CONTROL_APPLICATION,
        TASK_GRAPHICS_SERVER
};

typedef enum task_role task_role_t;

struct task_category_policy {
        task_role_t role;
};

typedef struct task_category_policy task_category_policy_data_t;
typedef struct task_category_policy *task_category_policy_t;
# 75 "/usr/include/mach/mach_types.h" 2 3 4
# 1 "/usr/include/mach/task_special_ports.h" 1 3 4
# 66 "/usr/include/mach/task_special_ports.h" 3 4
typedef int task_special_port_t;
# 76 "/usr/include/mach/mach_types.h" 2 3 4
# 1 "/usr/include/mach/thread_info.h" 1 3 4
# 76 "/usr/include/mach/thread_info.h" 3 4
typedef natural_t thread_flavor_t;
typedef integer_t *thread_info_t;


typedef integer_t thread_info_data_t[(1024)];






struct thread_basic_info {
        time_value_t user_time;
        time_value_t system_time;
        integer_t cpu_usage;
        policy_t policy;
        integer_t run_state;
        integer_t flags;
        integer_t suspend_count;
        integer_t sleep_time;

};

typedef struct thread_basic_info thread_basic_info_data_t;
typedef struct thread_basic_info *thread_basic_info_t;
# 77 "/usr/include/mach/mach_types.h" 2 3 4
# 1 "/usr/include/mach/thread_policy.h" 1 3 4
# 56 "/usr/include/mach/thread_policy.h" 3 4
typedef natural_t thread_policy_flavor_t;
typedef integer_t *thread_policy_t;
# 91 "/usr/include/mach/thread_policy.h" 3 4
struct thread_standard_policy {
        natural_t no_data;
};

typedef struct thread_standard_policy thread_standard_policy_data_t;
typedef struct thread_standard_policy *thread_standard_policy_t;
# 114 "/usr/include/mach/thread_policy.h" 3 4
struct thread_extended_policy {
        boolean_t timeshare;
};

typedef struct thread_extended_policy thread_extended_policy_data_t;
typedef struct thread_extended_policy *thread_extended_policy_t;
# 153 "/usr/include/mach/thread_policy.h" 3 4
struct thread_time_constraint_policy {
        uint32_t period;
        uint32_t computation;
        uint32_t constraint;
        boolean_t preemptible;
};

typedef struct thread_time_constraint_policy thread_time_constraint_policy_data_t;

typedef struct thread_time_constraint_policy *thread_time_constraint_policy_t;
# 181 "/usr/include/mach/thread_policy.h" 3 4
struct thread_precedence_policy {
        integer_t importance;
};

typedef struct thread_precedence_policy thread_precedence_policy_data_t;
typedef struct thread_precedence_policy *thread_precedence_policy_t;
# 78 "/usr/include/mach/mach_types.h" 2 3 4
# 1 "/usr/include/mach/thread_special_ports.h" 1 3 4
# 79 "/usr/include/mach/mach_types.h" 2 3 4


# 1 "/usr/include/mach/clock_types.h" 1 3 4
# 46 "/usr/include/mach/clock_types.h" 3 4
typedef int alarm_type_t;
typedef int sleep_type_t;
typedef int clock_id_t;
typedef int clock_flavor_t;
typedef int *clock_attr_t;
typedef int clock_res_t;




struct mach_timespec {
        unsigned int tv_sec;
        clock_res_t tv_nsec;
};
typedef struct mach_timespec mach_timespec_t;
# 82 "/usr/include/mach/mach_types.h" 2 3 4
# 1 "/usr/include/mach/vm_attributes.h" 1 3 4
# 70 "/usr/include/mach/vm_attributes.h" 3 4
typedef unsigned int vm_machine_attribute_t;
# 79 "/usr/include/mach/vm_attributes.h" 3 4
typedef int vm_machine_attribute_val_t;
# 83 "/usr/include/mach/mach_types.h" 2 3 4
# 1 "/usr/include/mach/vm_inherit.h" 1 3 4
# 69 "/usr/include/mach/vm_inherit.h" 3 4
typedef unsigned int vm_inherit_t;
# 84 "/usr/include/mach/mach_types.h" 2 3 4
# 1 "/usr/include/mach/vm_behavior.h" 1 3 4
# 41 "/usr/include/mach/vm_behavior.h" 3 4
typedef int vm_behavior_t;
# 85 "/usr/include/mach/mach_types.h" 2 3 4
# 1 "/usr/include/mach/vm_prot.h" 1 3 4
# 69 "/usr/include/mach/vm_prot.h" 3 4
typedef int vm_prot_t;
# 86 "/usr/include/mach/mach_types.h" 2 3 4

# 1 "/usr/include/mach/vm_sync.h" 1 3 4
# 60 "/usr/include/mach/vm_sync.h" 3 4
typedef unsigned vm_sync_t;
# 88 "/usr/include/mach/mach_types.h" 2 3 4

# 1 "/usr/include/mach/vm_region.h" 1 3 4
# 47 "/usr/include/mach/vm_region.h" 3 4
typedef int *vm_region_info_t;
typedef int *vm_region_info_64_t;
typedef int *vm_region_recurse_info_t;
typedef int *vm_region_recurse_info_64_t;
typedef int vm_region_flavor_t;
typedef int vm_region_info_data_t[(1024)];



struct vm_region_basic_info_64 {
        vm_prot_t protection;
        vm_prot_t max_protection;
        vm_inherit_t inheritance;
        boolean_t shared;
        boolean_t reserved;
        vm_object_offset_t offset;
        vm_behavior_t behavior;
        unsigned short user_wired_count;
};

typedef struct vm_region_basic_info_64 *vm_region_basic_info_64_t;
typedef struct vm_region_basic_info_64 vm_region_basic_info_data_64_t;





struct vm_region_basic_info {
        vm_prot_t protection;
        vm_prot_t max_protection;
        vm_inherit_t inheritance;
        boolean_t shared;
        boolean_t reserved;



        vm_offset_t offset;

        vm_behavior_t behavior;
        unsigned short user_wired_count;
};

typedef struct vm_region_basic_info *vm_region_basic_info_t;
typedef struct vm_region_basic_info vm_region_basic_info_data_t;
# 116 "/usr/include/mach/vm_region.h" 3 4
struct vm_region_extended_info {
        vm_prot_t protection;
        unsigned int user_tag;
        unsigned int pages_resident;
        unsigned int pages_shared_now_private;
        unsigned int pages_swapped_out;
        unsigned int pages_dirtied;
        unsigned int ref_count;
        unsigned short shadow_depth;
        unsigned char external_pager;
        unsigned char share_mode;
};

typedef struct vm_region_extended_info *vm_region_extended_info_t;
typedef struct vm_region_extended_info vm_region_extended_info_data_t;







struct vm_region_top_info {
        unsigned int obj_id;
        unsigned int ref_count;
        unsigned int private_pages_resident;
        unsigned int shared_pages_resident;
        unsigned char share_mode;
};

typedef struct vm_region_top_info *vm_region_top_info_t;
typedef struct vm_region_top_info vm_region_top_info_data_t;
# 173 "/usr/include/mach/vm_region.h" 3 4
struct vm_region_submap_info {
        vm_prot_t protection;
        vm_prot_t max_protection;
        vm_inherit_t inheritance;



        vm_offset_t offset;

        unsigned int user_tag;
        unsigned int pages_resident;
        unsigned int pages_shared_now_private;
        unsigned int pages_swapped_out;
        unsigned int pages_dirtied;
        unsigned int ref_count;
        unsigned short shadow_depth;
        unsigned char external_pager;
        unsigned char share_mode;
        boolean_t is_submap;
        vm_behavior_t behavior;
        vm_offset_t object_id;
        unsigned short user_wired_count;
};

typedef struct vm_region_submap_info *vm_region_submap_info_t;
typedef struct vm_region_submap_info vm_region_submap_info_data_t;






struct vm_region_submap_info_64 {
        vm_prot_t protection;
        vm_prot_t max_protection;
        vm_inherit_t inheritance;
        vm_object_offset_t offset;
        unsigned int user_tag;
        unsigned int pages_resident;
        unsigned int pages_shared_now_private;
        unsigned int pages_swapped_out;
        unsigned int pages_dirtied;
        unsigned int ref_count;
        unsigned short shadow_depth;
        unsigned char external_pager;
        unsigned char share_mode;
        boolean_t is_submap;
        vm_behavior_t behavior;
        vm_offset_t object_id;
        unsigned short user_wired_count;
};

typedef struct vm_region_submap_info_64 *vm_region_submap_info_64_t;
typedef struct vm_region_submap_info_64 vm_region_submap_info_data_64_t;





struct vm_read_entry {
        vm_address_t address;
        vm_size_t size;
};



typedef struct vm_read_entry vm_read_entry_t[(256)];
# 90 "/usr/include/mach/mach_types.h" 2 3 4
# 1 "/usr/include/mach/kmod.h" 1 3 4
# 34 "/usr/include/mach/kmod.h" 3 4
# 1 "/usr/include/mach/kern_return.h" 1 3 4
# 64 "/usr/include/mach/kern_return.h" 3 4
# 1 "/usr/include/mach/machine/kern_return.h" 1 3 4
# 27 "/usr/include/mach/machine/kern_return.h" 3 4
# 1 "/usr/include/mach/ppc/kern_return.h" 1 3 4
# 135 "/usr/include/mach/ppc/kern_return.h" 3 4
typedef int kern_return_t;
# 28 "/usr/include/mach/machine/kern_return.h" 2 3 4
# 65 "/usr/include/mach/kern_return.h" 2 3 4
# 35 "/usr/include/mach/kmod.h" 2 3 4
# 54 "/usr/include/mach/kmod.h" 3 4
typedef int kmod_t;
typedef int kmod_control_flavor_t;
typedef void* kmod_args_t;



typedef struct kmod_reference {
        struct kmod_reference *next;
        struct kmod_info *info;
} kmod_reference_t;
# 73 "/usr/include/mach/kmod.h" 3 4
typedef kern_return_t kmod_start_func_t(struct kmod_info *ki, void *data);
typedef kern_return_t kmod_stop_func_t(struct kmod_info *ki, void *data);

typedef struct kmod_info {
        struct kmod_info *next;
        int info_version;
        int id;
        char name[64];
        char version[64];
        int reference_count;
        kmod_reference_t *reference_list;
        vm_address_t address;
        vm_size_t size;
        vm_size_t hdr_size;
        kmod_start_func_t *start;
        kmod_stop_func_t *stop;
} kmod_info_t;



typedef kmod_info_t *kmod_info_array_t;
# 134 "/usr/include/mach/kmod.h" 3 4
typedef struct kmod_load_extension_cmd {
        int type;
        char name[64];
} kmod_load_extension_cmd_t;

typedef struct kmod_load_with_dependencies_cmd {
        int type;
        char name[64];
        char dependencies[1][64];
} kmod_load_with_dependencies_cmd_t;

typedef struct kmod_generic_cmd {
        int type;
        char data[1];
} kmod_generic_cmd_t;
# 91 "/usr/include/mach/mach_types.h" 2 3 4






typedef mach_port_t task_t;
typedef mach_port_t thread_t;
typedef mach_port_t thread_act_t;
typedef mach_port_t ipc_space_t;
typedef mach_port_t host_t;
typedef mach_port_t host_priv_t;
typedef mach_port_t host_security_t;
typedef mach_port_t processor_t;
typedef mach_port_t processor_set_t;
typedef mach_port_t processor_set_control_t;
typedef mach_port_t semaphore_t;
typedef mach_port_t lock_set_t;
typedef mach_port_t ledger_t;
typedef mach_port_t alarm_t;
typedef mach_port_t clock_serv_t;
typedef mach_port_t clock_ctrl_t;







typedef processor_set_t processor_set_name_t;




typedef mach_port_t clock_reply_t;
typedef mach_port_t bootstrap_t;
typedef mach_port_t mem_entry_name_port_t;
typedef mach_port_t exception_handler_t;
typedef exception_handler_t *exception_handler_array_t;
typedef mach_port_t vm_task_entry_t;
typedef mach_port_t io_master_t;
typedef mach_port_t UNDServerRef;
# 141 "/usr/include/mach/mach_types.h" 3 4
typedef task_t *task_array_t;
typedef thread_t *thread_array_t;
typedef processor_set_t *processor_set_array_t;
typedef processor_set_t *processor_set_name_array_t;
typedef processor_t *processor_array_t;
typedef thread_act_t *thread_act_array_t;
typedef ledger_t *ledger_array_t;
# 156 "/usr/include/mach/mach_types.h" 3 4
typedef task_t task_port_t;
typedef task_array_t task_port_array_t;
typedef thread_t thread_port_t;
typedef thread_array_t thread_port_array_t;
typedef ipc_space_t ipc_space_port_t;
typedef host_t host_name_t;
typedef host_t host_name_port_t;
typedef processor_set_t processor_set_port_t;
typedef processor_set_t processor_set_name_port_t;
typedef processor_set_array_t processor_set_name_port_array_t;
typedef processor_set_t processor_set_control_port_t;
typedef processor_t processor_port_t;
typedef processor_array_t processor_port_array_t;
typedef thread_act_t thread_act_port_t;
typedef thread_act_array_t thread_act_port_array_t;
typedef semaphore_t semaphore_port_t;
typedef lock_set_t lock_set_port_t;
typedef ledger_t ledger_port_t;
typedef ledger_array_t ledger_port_array_t;
typedef alarm_t alarm_port_t;
typedef clock_serv_t clock_serv_port_t;
typedef clock_ctrl_t clock_ctrl_port_t;
typedef exception_handler_t exception_port_t;
typedef exception_handler_array_t exception_port_arrary_t;
# 198 "/usr/include/mach/mach_types.h" 3 4
typedef integer_t ledger_item_t;
typedef vm_offset_t *emulation_vector_t;
typedef char *user_subsystem_t;





# 1 "/usr/include/mach/std_types.h" 1 3 4
# 207 "/usr/include/mach/mach_types.h" 2 3 4
# 45 "/usr/include/pthread.h" 2 3 4
# 74 "/usr/include/pthread.h" 3 4
extern "C" {
# 168 "/usr/include/pthread.h" 3 4
# 1 "/usr/include/sys/time.h" 1 3 4
# 68 "/usr/include/sys/time.h" 3 4
struct timeval {
        int32_t tv_sec;
        int32_t tv_usec;
};
# 93 "/usr/include/sys/time.h" 3 4
struct timezone {
        int tz_minuteswest;
        int tz_dsttime;
};
# 141 "/usr/include/sys/time.h" 3 4
struct itimerval {
        struct timeval it_interval;
        struct timeval it_value;
};




struct clockinfo {
        int hz;
        int tick;
        int tickadj;
        int stathz;
        int profhz;
};
# 179 "/usr/include/sys/time.h" 3 4
extern "C" {
int adjtime (const struct timeval *, struct timeval *);
int futimes (int, const struct timeval *);
int getitimer (int, struct itimerval *);
int gettimeofday (struct timeval *, struct timezone *);
int setitimer (int, const struct itimerval *, struct itimerval *);
int settimeofday (const struct timeval *, const struct timezone *);
int utimes (const char *, const struct timeval *);
}
# 169 "/usr/include/pthread.h" 2 3 4




int pthread_attr_destroy (pthread_attr_t *attr);
int pthread_attr_getdetachstate (const pthread_attr_t *attr, int *detachstate);

int pthread_attr_getinheritsched (const pthread_attr_t *attr, int *inheritsched);

int pthread_attr_getschedparam (const pthread_attr_t *attr, struct sched_param *param);

int pthread_attr_getschedpolicy (const pthread_attr_t *attr, int *policy);

int pthread_attr_getstackaddr (const pthread_attr_t *attr, void **stackaddr);

int pthread_attr_getstacksize (const pthread_attr_t *attr, size_t *stacksize);

int pthread_attr_getstack (const pthread_attr_t *attr, void **stackaddr, size_t *stacksize);

int pthread_attr_init (pthread_attr_t *attr);
int pthread_attr_setdetachstate (pthread_attr_t *attr, int detachstate);

int pthread_attr_setinheritsched (pthread_attr_t *attr, int inheritsched);

int pthread_attr_setschedparam (pthread_attr_t *attr, const struct sched_param *param);

int pthread_attr_setschedpolicy (pthread_attr_t *attr, int policy);

int pthread_attr_setstackaddr (pthread_attr_t *attr, void *stackaddr);

int pthread_attr_setstacksize (pthread_attr_t *attr, size_t stacksize);

int pthread_attr_setstack (pthread_attr_t *attr, void *stackaddr, size_t stacksize);

int pthread_cancel (pthread_t thread);
int pthread_setcancelstate (int state, int *oldstate);
int pthread_setcanceltype (int type, int *oldtype);
void pthread_testcancel (void);
int pthread_cond_broadcast (pthread_cond_t *cond);
int pthread_cond_destroy (pthread_cond_t *cond);
int pthread_cond_init (pthread_cond_t *cond, const pthread_condattr_t *attr);

int pthread_cond_signal (pthread_cond_t *cond);
int pthread_cond_wait (pthread_cond_t *cond, pthread_mutex_t *mutex);

int pthread_cond_timedwait (pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);


int pthread_condattr_init (pthread_condattr_t *attr);
int pthread_condattr_destroy (pthread_condattr_t *attr);
int pthread_condattr_getpshared (const pthread_condattr_t *attr, int *pshared);

int pthread_condattr_setpshared (pthread_condattr_t *attr, int pshared);

int pthread_create (pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);



int pthread_detach (pthread_t thread);
int pthread_equal (pthread_t t1, pthread_t t2);

void pthread_exit (void *value_ptr);
int pthread_kill (pthread_t, int);
int pthread_sigmask (int, const sigset_t *, sigset_t *);
int sigwait (const sigset_t *, int *);
int pthread_getschedparam (pthread_t thread, int *policy, struct sched_param *param);


int pthread_join (pthread_t thread, void **value_ptr);

int pthread_mutex_destroy (pthread_mutex_t *mutex);
int pthread_mutex_getprioceiling (const pthread_mutex_t *mutex, int *prioceiling);

int pthread_mutex_init (pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);

int pthread_mutex_lock (pthread_mutex_t *mutex);
int pthread_mutex_setprioceiling (pthread_mutex_t *mutex, int prioceiling, int *old_prioceiling);


int pthread_mutex_trylock (pthread_mutex_t *mutex);
int pthread_mutex_unlock (pthread_mutex_t *mutex);
int pthread_mutexattr_destroy (pthread_mutexattr_t *attr);
int pthread_mutexattr_getprioceiling (const pthread_mutexattr_t *attr, int *prioceiling);

int pthread_mutexattr_getprotocol (const pthread_mutexattr_t *attr, int *protocol);

int pthread_mutexattr_getpshared (const pthread_mutexattr_t *attr, int *pshared);

int pthread_mutexattr_gettype (const pthread_mutexattr_t *attr, int *type);

int pthread_mutexattr_init (pthread_mutexattr_t *attr);
int pthread_mutexattr_setprioceiling (pthread_mutexattr_t *attr, int prioceiling);

int pthread_mutexattr_setprotocol (pthread_mutexattr_t *attr, int protocol);

int pthread_mutexattr_settype (pthread_mutexattr_t *attr, int type);

int pthread_mutexattr_setpshared (pthread_mutexattr_t *attr, int pshared);

int pthread_once (pthread_once_t *once_control, void (*init_routine)(void));

pthread_t pthread_self (void);
int pthread_setschedparam (pthread_t thread, int policy, const struct sched_param *param);


int pthread_key_create (pthread_key_t *key, void (*destructor)(void *));

int pthread_key_delete (pthread_key_t key);
int pthread_setspecific (pthread_key_t key, const void *value);

void *pthread_getspecific (pthread_key_t key);
int pthread_attr_getscope (pthread_attr_t *, int *);
int pthread_attr_setscope (pthread_attr_t *, int);
int pthread_getconcurrency (void);
int pthread_setconcurrency (int);
int pthread_rwlock_destroy (pthread_rwlock_t * rwlock);
int pthread_rwlock_init (pthread_rwlock_t * rwlock, const pthread_rwlockattr_t *attr);

int pthread_rwlock_rdlock (pthread_rwlock_t *rwlock);
int pthread_rwlock_tryrdlock (pthread_rwlock_t *rwlock);
int pthread_rwlock_wrlock (pthread_rwlock_t *rwlock);
int pthread_rwlock_trywrlock (pthread_rwlock_t *rwlock);
int pthread_rwlock_unlock (pthread_rwlock_t *rwlock);
int pthread_rwlockattr_init (pthread_rwlockattr_t *attr);
int pthread_rwlockattr_destroy (pthread_rwlockattr_t *attr);
int pthread_rwlockattr_getpshared (const pthread_rwlockattr_t *attr, int *pshared);

int pthread_rwlockattr_setpshared (pthread_rwlockattr_t *attr, int pshared);




int pthread_is_threaded_np (void);


int pthread_main_np (void);


mach_port_t pthread_mach_thread_np (pthread_t);
size_t pthread_get_stacksize_np (pthread_t);
void * pthread_get_stackaddr_np (pthread_t);


int pthread_cond_signal_thread_np (pthread_cond_t *, pthread_t);


int pthread_cond_timedwait_relative_np (pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *reltime);




int pthread_create_suspended_np (pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);



void pthread_yield_np (void);

}
# 38 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/gthr-default.h" 2 3


typedef pthread_key_t __gthread_key_t;
typedef pthread_once_t __gthread_once_t;
typedef pthread_mutex_t __gthread_mutex_t;
# 96 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/gthr-default.h" 3
static inline int
__gthread_active_p (void)
{
  return 1;
}
# 449 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/gthr-default.h" 3
static inline int
__gthread_once (__gthread_once_t *once, void (*func) (void))
{
  if (__gthread_active_p ())
    return pthread_once (once, func);
  else
    return -1;
}

static inline int
__gthread_key_create (__gthread_key_t *key, void (*dtor) (void *))
{
  return pthread_key_create (key, dtor);
}

static inline int
__gthread_key_dtor (__gthread_key_t key, void *ptr)
{

  if (ptr)
    return pthread_setspecific (key, 0);
  else
    return 0;
}

static inline int
__gthread_key_delete (__gthread_key_t key)
{
  return pthread_key_delete (key);
}

static inline void *
__gthread_getspecific (__gthread_key_t key)
{
  return pthread_getspecific (key);
}

static inline int
__gthread_setspecific (__gthread_key_t key, const void *ptr)
{
  return pthread_setspecific (key, ptr);
}

static inline int
__gthread_mutex_lock (__gthread_mutex_t *mutex)
{
  if (__gthread_active_p ())
    return pthread_mutex_lock (mutex);
  else
    return 0;
}

static inline int
__gthread_mutex_trylock (__gthread_mutex_t *mutex)
{
  if (__gthread_active_p ())
    return pthread_mutex_trylock (mutex);
  else
    return 0;
}

static inline int
__gthread_mutex_unlock (__gthread_mutex_t *mutex)
{
  if (__gthread_active_p ())
    return pthread_mutex_unlock (mutex);
  else
    return 0;
}
# 99 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/gthr.h" 2 3
# 38 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/c++io.h" 2 3

namespace std
{

  typedef long streamoff;
  typedef ptrdiff_t streamsize;



  typedef fpos_t __c_streampos;

  typedef __gthread_mutex_t __c_lock;


  typedef FILE __c_file;


  struct __ios_flags
  {
    typedef short __int_type;

    static const __int_type _S_boolalpha = 0x0001;
    static const __int_type _S_dec = 0x0002;
    static const __int_type _S_fixed = 0x0004;
    static const __int_type _S_hex = 0x0008;
    static const __int_type _S_internal = 0x0010;
    static const __int_type _S_left = 0x0020;
    static const __int_type _S_oct = 0x0040;
    static const __int_type _S_right = 0x0080;
    static const __int_type _S_scientific = 0x0100;
    static const __int_type _S_showbase = 0x0200;
    static const __int_type _S_showpoint = 0x0400;
    static const __int_type _S_showpos = 0x0800;
    static const __int_type _S_skipws = 0x1000;
    static const __int_type _S_unitbuf = 0x2000;
    static const __int_type _S_uppercase = 0x4000;
    static const __int_type _S_adjustfield = 0x0020 | 0x0080 | 0x0010;
    static const __int_type _S_basefield = 0x0002 | 0x0040 | 0x0008;
    static const __int_type _S_floatfield = 0x0100 | 0x0004;


    static const __int_type _S_badbit = 0x01;
    static const __int_type _S_eofbit = 0x02;
    static const __int_type _S_failbit = 0x04;


    static const __int_type _S_app = 0x01;
    static const __int_type _S_ate = 0x02;
    static const __int_type _S_bin = 0x04;
    static const __int_type _S_in = 0x08;
    static const __int_type _S_out = 0x10;
    static const __int_type _S_trunc = 0x20;
  };
}
# 45 "/usr/include/gcc/darwin/3.3/c++/bits/fpos.h" 2 3
# 1 "/usr/include/gcc/darwin/3.3/c++/cwchar" 1 3
# 48 "/usr/include/gcc/darwin/3.3/c++/cwchar" 3



# 1 "/usr/include/gcc/darwin/3.3/c++/ctime" 1 3
# 48 "/usr/include/gcc/darwin/3.3/c++/ctime" 3
# 64 "/usr/include/gcc/darwin/3.3/c++/ctime" 3
namespace std
{
  using ::clock_t;
  using ::time_t;
  using ::tm;

  using ::clock;
  using ::difftime;
  using ::mktime;
  using ::time;
  using ::asctime;
  using ::ctime;
  using ::gmtime;
  using ::localtime;
  using ::strftime;
}
# 52 "/usr/include/gcc/darwin/3.3/c++/cwchar" 2 3
# 60 "/usr/include/gcc/darwin/3.3/c++/cwchar" 3
extern "C"
{
  typedef struct
  {
    int __fill[6];
  } mbstate_t;
}


namespace std
{
  using ::mbstate_t;
}
# 46 "/usr/include/gcc/darwin/3.3/c++/bits/fpos.h" 2 3

namespace std
{






  template<typename _StateT>
    class fpos
    {
    public:

      typedef _StateT __state_type;

    private:
      streamoff _M_off;
      __state_type _M_st;

    public:
      __state_type
      state() const { return _M_st; }

      void
      state(__state_type __st) { _M_st = __st; }



      fpos(): _M_off(streamoff()), _M_st(__state_type()) { }

      fpos(streamoff __off, __state_type __st = __state_type())
      : _M_off(__off), _M_st(__st) { }

      operator streamoff() const { return _M_off; }

      fpos&
      operator+=(streamoff __off) { _M_off += __off; return *this; }

      fpos&
      operator-=(streamoff __off) { _M_off -= __off; return *this; }

      fpos
      operator+(streamoff __off)
      {
        fpos __t(*this);
        __t += __off;
        return __t;
      }

      fpos
      operator-(streamoff __off)
      {
        fpos __t(*this);
        __t -= __off;
        return __t;
      }

      bool
      operator==(const fpos& __pos) const
      { return _M_off == __pos._M_off; }

      bool
      operator!=(const fpos& __pos) const
      { return _M_off != __pos._M_off; }

      streamoff
      _M_position() const { return _M_off; }

      void
      _M_position(streamoff __off) { _M_off = __off; }
    };


  typedef fpos<mbstate_t> streampos;




}
# 50 "/usr/include/gcc/darwin/3.3/c++/iosfwd" 2 3
# 1 "/usr/include/gcc/darwin/3.3/c++/bits/functexcept.h" 1 3
# 34 "/usr/include/gcc/darwin/3.3/c++/bits/functexcept.h" 3
# 1 "/usr/include/gcc/darwin/3.3/c++/exception_defines.h" 1 3
# 35 "/usr/include/gcc/darwin/3.3/c++/bits/functexcept.h" 2 3

namespace std
{

  void
  __throw_bad_exception(void);


  void
  __throw_bad_alloc(void);


  void
  __throw_bad_cast(void);

  void
  __throw_bad_typeid(void);


  void
  __throw_logic_error(const char* __s);

  void
  __throw_domain_error(const char* __s);

  void
  __throw_invalid_argument(const char* __s);

  void
  __throw_length_error(const char* __s);

  void
  __throw_out_of_range(const char* __s);

  void
  __throw_runtime_error(const char* __s);

  void
  __throw_range_error(const char* __s);

  void
  __throw_overflow_error(const char* __s);

  void
  __throw_underflow_error(const char* __s);


  void
  __throw_ios_failure(const char* __s);
}
# 51 "/usr/include/gcc/darwin/3.3/c++/iosfwd" 2 3

namespace std
{
  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_ios;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_streambuf;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_istream;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_ostream;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_iostream;

  template<typename _CharT, typename _Traits = char_traits<_CharT>,
            typename _Alloc = allocator<_CharT> >
    class basic_stringbuf;

  template<typename _CharT, typename _Traits = char_traits<_CharT>,
           typename _Alloc = allocator<_CharT> >
    class basic_istringstream;

  template<typename _CharT, typename _Traits = char_traits<_CharT>,
           typename _Alloc = allocator<_CharT> >
    class basic_ostringstream;

  template<typename _CharT, typename _Traits = char_traits<_CharT>,
           typename _Alloc = allocator<_CharT> >
    class basic_stringstream;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_filebuf;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_ifstream;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_ofstream;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class basic_fstream;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class istreambuf_iterator;

  template<typename _CharT, typename _Traits = char_traits<_CharT> >
    class ostreambuf_iterator;



  class ios_base;
# 136 "/usr/include/gcc/darwin/3.3/c++/iosfwd" 3
  typedef basic_ios<char> ios;
  typedef basic_streambuf<char> streambuf;
  typedef basic_istream<char> istream;
  typedef basic_ostream<char> ostream;
  typedef basic_iostream<char> iostream;
  typedef basic_stringbuf<char> stringbuf;
  typedef basic_istringstream<char> istringstream;
  typedef basic_ostringstream<char> ostringstream;
  typedef basic_stringstream<char> stringstream;
  typedef basic_filebuf<char> filebuf;
  typedef basic_ifstream<char> ifstream;
  typedef basic_ofstream<char> ofstream;
  typedef basic_fstream<char> fstream;
# 166 "/usr/include/gcc/darwin/3.3/c++/iosfwd" 3
}
# 71 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 2 3
# 1 "/usr/include/gcc/darwin/3.3/c++/bits/stl_pair.h" 1 3
# 64 "/usr/include/gcc/darwin/3.3/c++/bits/stl_pair.h" 3
namespace std
{


template <class _T1, class _T2>
struct pair {
  typedef _T1 first_type;
  typedef _T2 second_type;

  _T1 first;
  _T2 second;




  pair() : first(), second() {}




  pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}


  template <class _U1, class _U2>
  pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}
};


template <class _T1, class _T2>
inline bool operator==(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
  return __x.first == __y.first && __x.second == __y.second;
}


template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
  return __x.first < __y.first ||
         (!(__y.first < __x.first) && __x.second < __y.second);
}


template <class _T1, class _T2>
inline bool operator!=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) {
  return !(__x == __y);
}


template <class _T1, class _T2>
inline bool operator>(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) {
  return __y < __x;
}


template <class _T1, class _T2>
inline bool operator<=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) {
  return !(__y < __x);
}


template <class _T1, class _T2>
inline bool operator>=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) {
  return !(__x < __y);
}
# 140 "/usr/include/gcc/darwin/3.3/c++/bits/stl_pair.h" 3
template <class _T1, class _T2>


inline pair<_T1, _T2> make_pair(_T1 __x, _T2 __y)



{
  return pair<_T1, _T2>(__x, __y);
}

}
# 72 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 2 3
# 1 "/usr/include/gcc/darwin/3.3/c++/bits/type_traits.h" 1 3
# 53 "/usr/include/gcc/darwin/3.3/c++/bits/type_traits.h" 3
# 90 "/usr/include/gcc/darwin/3.3/c++/bits/type_traits.h" 3
struct __true_type {};
struct __false_type {};

template <class _Tp>
struct __type_traits {
   typedef __true_type this_dummy_member_must_be_first;
# 113 "/usr/include/gcc/darwin/3.3/c++/bits/type_traits.h" 3
   typedef __false_type has_trivial_default_constructor;
   typedef __false_type has_trivial_copy_constructor;
   typedef __false_type has_trivial_assignment_operator;
   typedef __false_type has_trivial_destructor;
   typedef __false_type is_POD_type;
};




template<> struct __type_traits<bool> {
   typedef __true_type has_trivial_default_constructor;
   typedef __true_type has_trivial_copy_constructor;
   typedef __true_type has_trivial_assignment_operator;
   typedef __true_type has_trivial_destructor;
   typedef __true_type is_POD_type;
};

template<> struct __type_traits<char> {
   typedef __true_type has_trivial_default_constructor;
   typedef __true_type has_trivial_copy_constructor;
   typedef __true_type has_trivial_assignment_operator;
   typedef __true_type has_trivial_destructor;
   typedef __true_type is_POD_type;
};

template<> struct __type_traits<signed char> {
   typedef __true_type has_trivial_default_constructor;
   typedef __true_type has_trivial_copy_constructor;
   typedef __true_type has_trivial_assignment_operator;
   typedef __true_type has_trivial_destructor;
   typedef __true_type is_POD_type;
};

template<> struct __type_traits<unsigned char> {
   typedef __true_type has_trivial_default_constructor;
   typedef __true_type has_trivial_copy_constructor;
   typedef __true_type has_trivial_assignment_operator;
   typedef __true_type has_trivial_destructor;
   typedef __true_type is_POD_type;
};

template<> struct __type_traits<wchar_t> {
   typedef __true_type has_trivial_default_constructor;
   typedef __true_type has_trivial_copy_constructor;
   typedef __true_type has_trivial_assignment_operator;
   typedef __true_type has_trivial_destructor;
   typedef __true_type is_POD_type;
};

template<> struct __type_traits<short> {
   typedef __true_type has_trivial_default_constructor;
   typedef __true_type has_trivial_copy_constructor;
   typedef __true_type has_trivial_assignment_operator;
   typedef __true_type has_trivial_destructor;
   typedef __true_type is_POD_type;
};

template<> struct __type_traits<unsigned short> {
   typedef __true_type has_trivial_default_constructor;
   typedef __true_type has_trivial_copy_constructor;
   typedef __true_type has_trivial_assignment_operator;
   typedef __true_type has_trivial_destructor;
   typedef __true_type is_POD_type;
};

template<> struct __type_traits<int> {
   typedef __true_type has_trivial_default_constructor;
   typedef __true_type has_trivial_copy_constructor;
   typedef __true_type has_trivial_assignment_operator;
   typedef __true_type has_trivial_destructor;
   typedef __true_type is_POD_type;
};

template<> struct __type_traits<unsigned int> {
   typedef __true_type has_trivial_default_constructor;
   typedef __true_type has_trivial_copy_constructor;
   typedef __true_type has_trivial_assignment_operator;
   typedef __true_type has_trivial_destructor;
   typedef __true_type is_POD_type;
};

template<> struct __type_traits<long> {
   typedef __true_type has_trivial_default_constructor;
   typedef __true_type has_trivial_copy_constructor;
   typedef __true_type has_trivial_assignment_operator;
   typedef __true_type has_trivial_destructor;
   typedef __true_type is_POD_type;
};

template<> struct __type_traits<unsigned long> {
   typedef __true_type has_trivial_default_constructor;
   typedef __true_type has_trivial_copy_constructor;
   typedef __true_type has_trivial_assignment_operator;
   typedef __true_type has_trivial_destructor;
   typedef __true_type is_POD_type;
};

template<> struct __type_traits<long long> {
   typedef __true_type has_trivial_default_constructor;
   typedef __true_type has_trivial_copy_constructor;
   typedef __true_type has_trivial_assignment_operator;
   typedef __true_type has_trivial_destructor;
   typedef __true_type is_POD_type;
};

template<> struct __type_traits<unsigned long long> {
   typedef __true_type has_trivial_default_constructor;
   typedef __true_type has_trivial_copy_constructor;
   typedef __true_type has_trivial_assignment_operator;
   typedef __true_type has_trivial_destructor;
   typedef __true_type is_POD_type;
};

template<> struct __type_traits<float> {
   typedef __true_type has_trivial_default_constructor;
   typedef __true_type has_trivial_copy_constructor;
   typedef __true_type has_trivial_assignment_operator;
   typedef __true_type has_trivial_destructor;
   typedef __true_type is_POD_type;
};

template<> struct __type_traits<double> {
   typedef __true_type has_trivial_default_constructor;
   typedef __true_type has_trivial_copy_constructor;
   typedef __true_type has_trivial_assignment_operator;
   typedef __true_type has_trivial_destructor;
   typedef __true_type is_POD_type;
};

template<> struct __type_traits<long double> {
   typedef __true_type has_trivial_default_constructor;
   typedef __true_type has_trivial_copy_constructor;
   typedef __true_type has_trivial_assignment_operator;
   typedef __true_type has_trivial_destructor;
   typedef __true_type is_POD_type;
};

template <class _Tp>
struct __type_traits<_Tp*> {
   typedef __true_type has_trivial_default_constructor;
   typedef __true_type has_trivial_copy_constructor;
   typedef __true_type has_trivial_assignment_operator;
   typedef __true_type has_trivial_destructor;
   typedef __true_type is_POD_type;
};





template <class _Tp> struct _Is_integer {
  typedef __false_type _Integral;
};

template<> struct _Is_integer<bool> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<char> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<signed char> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<unsigned char> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<wchar_t> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<short> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<unsigned short> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<int> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<unsigned int> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<long> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<unsigned long> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<long long> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<unsigned long long> {
  typedef __true_type _Integral;
};

template<typename _Tp> struct _Is_normal_iterator {
   typedef __false_type _Normal;
};


namespace __gnu_cxx
{
  template<typename _Iterator, typename _Container> class __normal_iterator;
}

template<typename _Iterator, typename _Container>
struct _Is_normal_iterator< __gnu_cxx::__normal_iterator<_Iterator, _Container> > {
   typedef __true_type _Normal;
};
# 73 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 2 3
# 1 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator_base_types.h" 1 3
# 68 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator_base_types.h" 3

namespace std
{
# 80 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator_base_types.h" 3
  struct input_iterator_tag {};

  struct output_iterator_tag {};

  struct forward_iterator_tag : public input_iterator_tag {};

  struct bidirectional_iterator_tag : public forward_iterator_tag {};

  struct random_access_iterator_tag : public bidirectional_iterator_tag {};
# 102 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator_base_types.h" 3
  template<typename _Category, typename _Tp, typename _Distance = ptrdiff_t,
           typename _Pointer = _Tp*, typename _Reference = _Tp&>
    struct iterator
    {

      typedef _Category iterator_category;

      typedef _Tp value_type;

      typedef _Distance difference_type;

      typedef _Pointer pointer;

      typedef _Reference reference;
    };







  template<typename _Iterator>
    struct iterator_traits {
      typedef typename _Iterator::iterator_category iterator_category;
      typedef typename _Iterator::value_type value_type;
      typedef typename _Iterator::difference_type difference_type;
      typedef typename _Iterator::pointer pointer;
      typedef typename _Iterator::reference reference;
    };

  template<typename _Tp>
    struct iterator_traits<_Tp*> {
      typedef random_access_iterator_tag iterator_category;
      typedef _Tp value_type;
      typedef ptrdiff_t difference_type;
      typedef _Tp* pointer;
      typedef _Tp& reference;
    };

  template<typename _Tp>
    struct iterator_traits<const _Tp*> {
      typedef random_access_iterator_tag iterator_category;
      typedef _Tp value_type;
      typedef ptrdiff_t difference_type;
      typedef const _Tp* pointer;
      typedef const _Tp& reference;
    };







  template<typename _Iter>
    inline typename iterator_traits<_Iter>::iterator_category
    __iterator_category(const _Iter&)
    { return typename iterator_traits<_Iter>::iterator_category(); }

}
# 74 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 2 3
# 1 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator_base_funcs.h" 1 3
# 68 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator_base_funcs.h" 3
# 1 "/usr/include/gcc/darwin/3.3/c++/bits/concept_check.h" 1 3
# 39 "/usr/include/gcc/darwin/3.3/c++/bits/concept_check.h" 3
# 69 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator_base_funcs.h" 2 3

namespace std
{
  template<typename _InputIterator>
    inline typename iterator_traits<_InputIterator>::difference_type
    __distance(_InputIterator __first, _InputIterator __last,
               input_iterator_tag)
    {

     

      typename iterator_traits<_InputIterator>::difference_type __n = 0;
      while (__first != __last) {
        ++__first; ++__n;
      }
      return __n;
    }

  template<typename _RandomAccessIterator>
    inline typename iterator_traits<_RandomAccessIterator>::difference_type
    __distance(_RandomAccessIterator __first, _RandomAccessIterator __last,
               random_access_iterator_tag)
    {

     
      return __last - __first;
    }
# 109 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator_base_funcs.h" 3
  template<typename _InputIterator>
    inline typename iterator_traits<_InputIterator>::difference_type
    distance(_InputIterator __first, _InputIterator __last)
    {

      return __distance(__first, __last, __iterator_category(__first));
    }

  template<typename _InputIter, typename _Distance>
    inline void
    __advance(_InputIter& __i, _Distance __n, input_iterator_tag)
    {

     
      while (__n--) ++__i;
    }

  template<typename _BidirectionalIterator, typename _Distance>
    inline void
    __advance(_BidirectionalIterator& __i, _Distance __n,
              bidirectional_iterator_tag)
    {

     

      if (__n > 0)
        while (__n--) ++__i;
      else
        while (__n++) --__i;
    }

  template<typename _RandomAccessIterator, typename _Distance>
    inline void
    __advance(_RandomAccessIterator& __i, _Distance __n,
              random_access_iterator_tag)
    {

     
      __i += __n;
    }
# 162 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator_base_funcs.h" 3
  template<typename _InputIterator, typename _Distance>
    inline void
    advance(_InputIterator& __i, _Distance __n)
    {

      __advance(__i, __n, __iterator_category(__i));
    }
}
# 75 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 2 3
# 1 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator.h" 1 3
# 68 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator.h" 3
namespace std
{
# 89 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator.h" 3
  template<typename _Iterator>
    class reverse_iterator
    : public iterator<typename iterator_traits<_Iterator>::iterator_category,
                      typename iterator_traits<_Iterator>::value_type,
                      typename iterator_traits<_Iterator>::difference_type,
                      typename iterator_traits<_Iterator>::pointer,
                      typename iterator_traits<_Iterator>::reference>
    {
    protected:
      _Iterator current;

    public:
      typedef _Iterator iterator_type;
      typedef typename iterator_traits<_Iterator>::difference_type
                                                               difference_type;
      typedef typename iterator_traits<_Iterator>::reference reference;
      typedef typename iterator_traits<_Iterator>::pointer pointer;

    public:



      reverse_iterator() { }




      explicit
      reverse_iterator(iterator_type __x) : current(__x) { }




      reverse_iterator(const reverse_iterator& __x)
      : current(__x.current) { }





      template<typename _Iter>
        reverse_iterator(const reverse_iterator<_Iter>& __x)
        : current(__x.base()) { }




      iterator_type
      base() const { return current; }






      reference
      operator*() const
      {
        _Iterator __tmp = current;
        return *--__tmp;
      }






      pointer
      operator->() const { return &(operator*()); }






      reverse_iterator&
      operator++()
      {
        --current;
        return *this;
      }






      reverse_iterator
      operator++(int)
      {
        reverse_iterator __tmp = *this;
        --current;
        return __tmp;
      }






      reverse_iterator&
      operator--()
      {
        ++current;
        return *this;
      }






      reverse_iterator operator--(int)
      {
        reverse_iterator __tmp = *this;
        ++current;
        return __tmp;
      }






      reverse_iterator
      operator+(difference_type __n) const
      { return reverse_iterator(current - __n); }






      reverse_iterator&
      operator+=(difference_type __n)
      {
        current -= __n;
        return *this;
      }






      reverse_iterator
      operator-(difference_type __n) const
      { return reverse_iterator(current + __n); }






      reverse_iterator&
      operator-=(difference_type __n)
      {
        current += __n;
        return *this;
      }






      reference
      operator[](difference_type __n) const { return *(*this + __n); }
    };
# 269 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator.h" 3
  template<typename _Iterator>
    inline bool
    operator==(const reverse_iterator<_Iterator>& __x,
               const reverse_iterator<_Iterator>& __y)
    { return __x.base() == __y.base(); }

  template<typename _Iterator>
    inline bool
    operator<(const reverse_iterator<_Iterator>& __x,
              const reverse_iterator<_Iterator>& __y)
    { return __y.base() < __x.base(); }

  template<typename _Iterator>
    inline bool
    operator!=(const reverse_iterator<_Iterator>& __x,
               const reverse_iterator<_Iterator>& __y)
    { return !(__x == __y); }

  template<typename _Iterator>
    inline bool
    operator>(const reverse_iterator<_Iterator>& __x,
              const reverse_iterator<_Iterator>& __y)
    { return __y < __x; }

  template<typename _Iterator>
    inline bool
    operator<=(const reverse_iterator<_Iterator>& __x,
                const reverse_iterator<_Iterator>& __y)
    { return !(__y < __x); }

  template<typename _Iterator>
    inline bool
    operator>=(const reverse_iterator<_Iterator>& __x,
               const reverse_iterator<_Iterator>& __y)
    { return !(__x < __y); }

  template<typename _Iterator>
    inline typename reverse_iterator<_Iterator>::difference_type
    operator-(const reverse_iterator<_Iterator>& __x,
              const reverse_iterator<_Iterator>& __y)
    { return __y.base() - __x.base(); }

  template<typename _Iterator>
    inline reverse_iterator<_Iterator>
    operator+(typename reverse_iterator<_Iterator>::difference_type __n,
              const reverse_iterator<_Iterator>& __x)
    { return reverse_iterator<_Iterator>(__x.base() - __n); }
# 329 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator.h" 3
  template<typename _Container>
    class back_insert_iterator
    : public iterator<output_iterator_tag, void, void, void, void>
    {
    protected:
      _Container* container;

    public:

      typedef _Container container_type;


      explicit
      back_insert_iterator(_Container& __x) : container(&__x) { }
# 355 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator.h" 3
      back_insert_iterator&
      operator=(typename _Container::const_reference __value)
      {
        container->push_back(__value);
        return *this;
      }


      back_insert_iterator&
      operator*() { return *this; }


      back_insert_iterator&
      operator++() { return *this; }


      back_insert_iterator
      operator++(int) { return *this; }
    };
# 386 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator.h" 3
  template<typename _Container>
    inline back_insert_iterator<_Container>
    back_inserter(_Container& __x)
    { return back_insert_iterator<_Container>(__x); }
# 401 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator.h" 3
  template<typename _Container>
    class front_insert_iterator
    : public iterator<output_iterator_tag, void, void, void, void>
    {
    protected:
      _Container* container;

    public:

      typedef _Container container_type;


      explicit front_insert_iterator(_Container& __x) : container(&__x) { }
# 426 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator.h" 3
      front_insert_iterator&
      operator=(typename _Container::const_reference __value)
      {
        container->push_front(__value);
        return *this;
      }


      front_insert_iterator&
      operator*() { return *this; }


      front_insert_iterator&
      operator++() { return *this; }


      front_insert_iterator
      operator++(int) { return *this; }
    };
# 457 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator.h" 3
  template<typename _Container>
    inline front_insert_iterator<_Container>
    front_inserter(_Container& __x)
    { return front_insert_iterator<_Container>(__x); }
# 476 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator.h" 3
  template<typename _Container>
    class insert_iterator
    : public iterator<output_iterator_tag, void, void, void, void>
    {
    protected:
      _Container* container;
      typename _Container::iterator iter;

    public:

      typedef _Container container_type;





      insert_iterator(_Container& __x, typename _Container::iterator __i)
      : container(&__x), iter(__i) {}
# 518 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator.h" 3
      insert_iterator&
      operator=(const typename _Container::const_reference __value)
      {
        iter = container->insert(iter, __value);
        ++iter;
        return *this;
      }


      insert_iterator&
      operator*() { return *this; }


      insert_iterator&
      operator++() { return *this; }


      insert_iterator&
      operator++(int) { return *this; }
    };
# 550 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator.h" 3
  template<typename _Container, typename _Iterator>
    inline insert_iterator<_Container>
    inserter(_Container& __x, _Iterator __i)
    {
      return insert_iterator<_Container>(__x,
                                         typename _Container::iterator(__i));
    }
}

namespace __gnu_cxx
{







  using std::iterator_traits;
  using std::iterator;
  template<typename _Iterator, typename _Container>
    class __normal_iterator
      : public iterator<typename iterator_traits<_Iterator>::iterator_category,
                        typename iterator_traits<_Iterator>::value_type,
                        typename iterator_traits<_Iterator>::difference_type,
                        typename iterator_traits<_Iterator>::pointer,
                        typename iterator_traits<_Iterator>::reference>
    {
    protected:
      _Iterator _M_current;

    public:
      typedef typename iterator_traits<_Iterator>::difference_type
                                                               difference_type;
      typedef typename iterator_traits<_Iterator>::reference reference;
      typedef typename iterator_traits<_Iterator>::pointer pointer;

      __normal_iterator() : _M_current(_Iterator()) { }

      explicit
      __normal_iterator(const _Iterator& __i) : _M_current(__i) { }


      template<typename _Iter>
      inline __normal_iterator(const __normal_iterator<_Iter, _Container>& __i)
        : _M_current(__i.base()) { }


      reference
      operator*() const { return *_M_current; }

      pointer
      operator->() const { return _M_current; }

      __normal_iterator&
      operator++() { ++_M_current; return *this; }

      __normal_iterator
      operator++(int) { return __normal_iterator(_M_current++); }


      __normal_iterator&
      operator--() { --_M_current; return *this; }

      __normal_iterator
      operator--(int) { return __normal_iterator(_M_current--); }


      reference
      operator[](const difference_type& __n) const
      { return _M_current[__n]; }

      __normal_iterator&
      operator+=(const difference_type& __n)
      { _M_current += __n; return *this; }

      __normal_iterator
      operator+(const difference_type& __n) const
      { return __normal_iterator(_M_current + __n); }

      __normal_iterator&
      operator-=(const difference_type& __n)
      { _M_current -= __n; return *this; }

      __normal_iterator
      operator-(const difference_type& __n) const
      { return __normal_iterator(_M_current - __n); }

      const _Iterator&
      base() const { return _M_current; }
    };
# 651 "/usr/include/gcc/darwin/3.3/c++/bits/stl_iterator.h" 3
  template<typename _IteratorL, typename _IteratorR, typename _Container>
  inline bool
  operator==(const __normal_iterator<_IteratorL, _Container>& __lhs,
             const __normal_iterator<_IteratorR, _Container>& __rhs)
  { return __lhs.base() == __rhs.base(); }

  template<typename _Iterator, typename _Container>
  inline bool
  operator==(const __normal_iterator<_Iterator, _Container>& __lhs,
             const __normal_iterator<_Iterator, _Container>& __rhs)
  { return __lhs.base() == __rhs.base(); }

  template<typename _IteratorL, typename _IteratorR, typename _Container>
  inline bool
  operator!=(const __normal_iterator<_IteratorL, _Container>& __lhs,
             const __normal_iterator<_IteratorR, _Container>& __rhs)
  { return __lhs.base() != __rhs.base(); }

  template<typename _Iterator, typename _Container>
  inline bool
  operator!=(const __normal_iterator<_Iterator, _Container>& __lhs,
             const __normal_iterator<_Iterator, _Container>& __rhs)
  { return __lhs.base() != __rhs.base(); }


  template<typename _IteratorL, typename _IteratorR, typename _Container>
  inline bool
  operator<(const __normal_iterator<_IteratorL, _Container>& __lhs,
            const __normal_iterator<_IteratorR, _Container>& __rhs)
  { return __lhs.base() < __rhs.base(); }

  template<typename _Iterator, typename _Container>
  inline bool
  operator<(const __normal_iterator<_Iterator, _Container>& __lhs,
             const __normal_iterator<_Iterator, _Container>& __rhs)
  { return __lhs.base() < __rhs.base(); }

  template<typename _IteratorL, typename _IteratorR, typename _Container>
  inline bool
  operator>(const __normal_iterator<_IteratorL, _Container>& __lhs,
            const __normal_iterator<_IteratorR, _Container>& __rhs)
  { return __lhs.base() > __rhs.base(); }

  template<typename _Iterator, typename _Container>
  inline bool
  operator>(const __normal_iterator<_Iterator, _Container>& __lhs,
            const __normal_iterator<_Iterator, _Container>& __rhs)
  { return __lhs.base() > __rhs.base(); }

  template<typename _IteratorL, typename _IteratorR, typename _Container>
  inline bool
  operator<=(const __normal_iterator<_IteratorL, _Container>& __lhs,
             const __normal_iterator<_IteratorR, _Container>& __rhs)
  { return __lhs.base() <= __rhs.base(); }

  template<typename _Iterator, typename _Container>
  inline bool
  operator<=(const __normal_iterator<_Iterator, _Container>& __lhs,
             const __normal_iterator<_Iterator, _Container>& __rhs)
  { return __lhs.base() <= __rhs.base(); }

  template<typename _IteratorL, typename _IteratorR, typename _Container>
  inline bool
  operator>=(const __normal_iterator<_IteratorL, _Container>& __lhs,
             const __normal_iterator<_IteratorR, _Container>& __rhs)
  { return __lhs.base() >= __rhs.base(); }

  template<typename _Iterator, typename _Container>
  inline bool
  operator>=(const __normal_iterator<_Iterator, _Container>& __lhs,
             const __normal_iterator<_Iterator, _Container>& __rhs)
  { return __lhs.base() >= __rhs.base(); }





  template<typename _IteratorL, typename _IteratorR, typename _Container>
  inline typename __normal_iterator<_IteratorL, _Container>::difference_type
  operator-(const __normal_iterator<_IteratorL, _Container>& __lhs,
             const __normal_iterator<_IteratorR, _Container>& __rhs)
  { return __lhs.base() - __rhs.base(); }

  template<typename _Iterator, typename _Container>
  inline __normal_iterator<_Iterator, _Container>
  operator+(typename __normal_iterator<_Iterator, _Container>::difference_type __n,
            const __normal_iterator<_Iterator, _Container>& __i)
  { return __normal_iterator<_Iterator, _Container>(__i.base() + __n); }
}
# 76 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 2 3


namespace std
{
# 91 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 3
  template<typename _ForwardIter1, typename _ForwardIter2>
    inline void
    iter_swap(_ForwardIter1 __a, _ForwardIter2 __b)
    {
      typedef typename iterator_traits<_ForwardIter1>::value_type _ValueType1;
      typedef typename iterator_traits<_ForwardIter2>::value_type _ValueType2;


     
     
     
     

      _ValueType1 __tmp = *__a;
      *__a = *__b;
      *__b = __tmp;
    }
# 118 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 3
  template<typename _Tp>
    inline void
    swap(_Tp& __a, _Tp& __b)
    {

     

      _Tp __tmp = __a;
      __a = __b;
      __b = __tmp;
    }
# 146 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 3
  template<typename _Tp>
    inline const _Tp&
    min(const _Tp& __a, const _Tp& __b)
    {

     

      if (__b < __a) return __b; return __a;
    }
# 166 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 3
  template<typename _Tp>
    inline const _Tp&
    max(const _Tp& __a, const _Tp& __b)
    {

     

      if (__a < __b) return __b; return __a;
    }
# 186 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 3
  template<typename _Tp, typename _Compare>
    inline const _Tp&
    min(const _Tp& __a, const _Tp& __b, _Compare __comp)
    {

      if (__comp(__b, __a)) return __b; return __a;
    }
# 204 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 3
  template<typename _Tp, typename _Compare>
    inline const _Tp&
    max(const _Tp& __a, const _Tp& __b, _Compare __comp)
    {

      if (__comp(__a, __b)) return __b; return __a;
    }
# 221 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 3
  template<typename _InputIter, typename _OutputIter>
    inline _OutputIter
    __copy(_InputIter __first, _InputIter __last,
           _OutputIter __result,
           input_iterator_tag)
    {
      for ( ; __first != __last; ++__result, ++__first)
        *__result = *__first;
      return __result;
    }

  template<typename _RandomAccessIter, typename _OutputIter>
    inline _OutputIter
    __copy(_RandomAccessIter __first, _RandomAccessIter __last,
           _OutputIter __result,
           random_access_iterator_tag)
    {
      typedef typename iterator_traits<_RandomAccessIter>::difference_type
          _Distance;
      for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
      }
      return __result;
    }

  template<typename _Tp>
    inline _Tp*
    __copy_trivial(const _Tp* __first, const _Tp* __last, _Tp* __result)
    {
      memmove(__result, __first, sizeof(_Tp) * (__last - __first));
      return __result + (__last - __first);
    }

  template<typename _InputIter, typename _OutputIter>
    inline _OutputIter
    __copy_aux2(_InputIter __first, _InputIter __last,
                _OutputIter __result, __false_type)
    { return __copy(__first, __last, __result, __iterator_category(__first)); }

  template<typename _InputIter, typename _OutputIter>
    inline _OutputIter
    __copy_aux2(_InputIter __first, _InputIter __last,
                _OutputIter __result, __true_type)
    { return __copy(__first, __last, __result, __iterator_category(__first)); }

  template<typename _Tp>
    inline _Tp*
    __copy_aux2(_Tp* __first, _Tp* __last,
                _Tp* __result, __true_type)
    { return __copy_trivial(__first, __last, __result); }

  template<typename _Tp>
    inline _Tp*
    __copy_aux2(const _Tp* __first, const _Tp* __last,
                _Tp* __result, __true_type)
    { return __copy_trivial(__first, __last, __result); }

  template<typename _InputIter, typename _OutputIter>
    inline _OutputIter
    __copy_ni2(_InputIter __first, _InputIter __last,
               _OutputIter __result, __true_type)
    {
      typedef typename iterator_traits<_InputIter>::value_type
          _ValueType;
      typedef typename __type_traits<_ValueType>::has_trivial_assignment_operator
          _Trivial;
      return _OutputIter(__copy_aux2(__first, __last,
                                     __result.base(),
                                     _Trivial()));
    }

  template<typename _InputIter, typename _OutputIter>
    inline _OutputIter
    __copy_ni2(_InputIter __first, _InputIter __last,
               _OutputIter __result, __false_type)
    {
      typedef typename iterator_traits<_InputIter>::value_type
          _ValueType;
      typedef typename __type_traits<_ValueType>::has_trivial_assignment_operator
          _Trivial;
      return __copy_aux2(__first, __last,
                         __result,
                         _Trivial());
    }

  template<typename _InputIter, typename _OutputIter>
    inline _OutputIter
    __copy_ni1(_InputIter __first, _InputIter __last,
               _OutputIter __result, __true_type)
    {
      typedef typename _Is_normal_iterator<_OutputIter>::_Normal __Normal;
      return __copy_ni2(__first.base(), __last.base(), __result, __Normal());
    }

  template<typename _InputIter, typename _OutputIter>
    inline _OutputIter
    __copy_ni1(_InputIter __first, _InputIter __last,
               _OutputIter __result, __false_type)
    {
      typedef typename _Is_normal_iterator<_OutputIter>::_Normal __Normal;
      return __copy_ni2(__first, __last, __result, __Normal());
    }
# 339 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 3
  template<typename _InputIter, typename _OutputIter>
    inline _OutputIter
    copy(_InputIter __first, _InputIter __last, _OutputIter __result)
    {

     
     


       typedef typename _Is_normal_iterator<_InputIter>::_Normal __Normal;
       return __copy_ni1(__first, __last, __result, __Normal());
    }




  template<typename _BidirectionalIter1, typename _BidirectionalIter2>
    inline _BidirectionalIter2
    __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                    _BidirectionalIter2 __result,
                    bidirectional_iterator_tag)
    {
      while (__first != __last)
        *--__result = *--__last;
      return __result;
    }

  template<typename _RandomAccessIter, typename _BidirectionalIter>
    inline _BidirectionalIter
    __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                    _BidirectionalIter __result,
                    random_access_iterator_tag)
    {
      typename iterator_traits<_RandomAccessIter>::difference_type __n;
      for (__n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
      return __result;
    }







  template<typename _BidirectionalIter1, typename _BidirectionalIter2,
           typename _BoolType>
    struct __copy_backward_dispatch
    {
      static _BidirectionalIter2
      copy(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
           _BidirectionalIter2 __result)
      {
        return __copy_backward(__first, __last,
                               __result,
                               __iterator_category(__first));
      }
    };

  template<typename _Tp>
    struct __copy_backward_dispatch<_Tp*, _Tp*, __true_type>
    {
      static _Tp*
      copy(const _Tp* __first, const _Tp* __last, _Tp* __result)
      {
        const ptrdiff_t _Num = __last - __first;
        memmove(__result - _Num, __first, sizeof(_Tp) * _Num);
        return __result - _Num;
      }
    };

  template<typename _Tp>
    struct __copy_backward_dispatch<const _Tp*, _Tp*, __true_type>
    {
      static _Tp*
      copy(const _Tp* __first, const _Tp* __last, _Tp* __result)
      {
        return __copy_backward_dispatch<_Tp*, _Tp*, __true_type>
          ::copy(__first, __last, __result);
      }
    };

  template<typename _BI1, typename _BI2>
    inline _BI2
    __copy_backward_aux(_BI1 __first, _BI1 __last, _BI2 __result)
    {
      typedef typename __type_traits<typename iterator_traits<_BI2>::value_type>
                            ::has_trivial_assignment_operator _Trivial;
      return __copy_backward_dispatch<_BI1, _BI2, _Trivial>
                  ::copy(__first, __last, __result);
    }

  template <typename _BI1, typename _BI2>
    inline _BI2
    __copy_backward_output_normal_iterator(_BI1 __first, _BI1 __last,
                                           _BI2 __result, __true_type)
    { return _BI2(__copy_backward_aux(__first, __last, __result.base())); }

  template <typename _BI1, typename _BI2>
    inline _BI2
    __copy_backward_output_normal_iterator(_BI1 __first, _BI1 __last,
                                           _BI2 __result, __false_type)
    { return __copy_backward_aux(__first, __last, __result); }

  template <typename _BI1, typename _BI2>
    inline _BI2
    __copy_backward_input_normal_iterator(_BI1 __first, _BI1 __last,
                                          _BI2 __result, __true_type)
    {
      typedef typename _Is_normal_iterator<_BI2>::_Normal __Normal;
      return __copy_backward_output_normal_iterator(__first.base(), __last.base(),
                                                    __result, __Normal());
    }

  template <typename _BI1, typename _BI2>
    inline _BI2
    __copy_backward_input_normal_iterator(_BI1 __first, _BI1 __last,
                                          _BI2 __result, __false_type)
    {
      typedef typename _Is_normal_iterator<_BI2>::_Normal __Normal;
      return __copy_backward_output_normal_iterator(__first, __last, __result,
                                                    __Normal());
    }
# 477 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 3
  template <typename _BI1, typename _BI2>
    inline _BI2
    copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
    {

     
     
     



      typedef typename _Is_normal_iterator<_BI1>::_Normal __Normal;
      return __copy_backward_input_normal_iterator(__first, __last, __result,
                                                   __Normal());
    }
# 509 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 3
  template<typename _ForwardIter, typename _Tp>
    void
    fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __value)
    {

     

      for ( ; __first != __last; ++__first)
        *__first = __value;
    }
# 531 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 3
  template<typename _OutputIter, typename _Size, typename _Tp>
    _OutputIter
    fill_n(_OutputIter __first, _Size __n, const _Tp& __value)
    {

     

      for ( ; __n > 0; --__n, ++__first)
        *__first = __value;
      return __first;
    }



  inline void
  fill(unsigned char* __first, unsigned char* __last, const unsigned char& __c)
  {
    unsigned char __tmp = __c;
    memset(__first, __tmp, __last - __first);
  }

  inline void
  fill(signed char* __first, signed char* __last, const signed char& __c)
  {
    signed char __tmp = __c;
    memset(__first, static_cast<unsigned char>(__tmp), __last - __first);
  }

  inline void
  fill(char* __first, char* __last, const char& __c)
  {
    char __tmp = __c;
    memset(__first, static_cast<unsigned char>(__tmp), __last - __first);
  }

  template<typename _Size>
    inline unsigned char*
    fill_n(unsigned char* __first, _Size __n, const unsigned char& __c)
    {
      fill(__first, __first + __n, __c);
      return __first + __n;
    }

  template<typename _Size>
    inline signed char*
    fill_n(char* __first, _Size __n, const signed char& __c)
    {
      fill(__first, __first + __n, __c);
      return __first + __n;
    }

  template<typename _Size>
    inline char*
    fill_n(char* __first, _Size __n, const char& __c)
    {
      fill(__first, __first + __n, __c);
      return __first + __n;
    }
# 606 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 3
  template<typename _InputIter1, typename _InputIter2>
    pair<_InputIter1, _InputIter2>
    mismatch(_InputIter1 __first1, _InputIter1 __last1,
             _InputIter2 __first2)
    {

     
     
     

     


      while (__first1 != __last1 && *__first1 == *__first2) {
        ++__first1;
        ++__first2;
      }
      return pair<_InputIter1, _InputIter2>(__first1, __first2);
    }
# 640 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 3
  template<typename _InputIter1, typename _InputIter2, typename _BinaryPredicate>
    pair<_InputIter1, _InputIter2>
    mismatch(_InputIter1 __first1, _InputIter1 __last1,
             _InputIter2 __first2,
             _BinaryPredicate __binary_pred)
    {

     
     

      while (__first1 != __last1 && __binary_pred(*__first1, *__first2)) {
        ++__first1;
        ++__first2;
      }
      return pair<_InputIter1, _InputIter2>(__first1, __first2);
    }
# 668 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 3
  template<typename _InputIter1, typename _InputIter2>
    inline bool
    equal(_InputIter1 __first1, _InputIter1 __last1,
          _InputIter2 __first2)
    {

     
     
     



      for ( ; __first1 != __last1; ++__first1, ++__first2)
        if (!(*__first1 == *__first2))
          return false;
      return true;
    }
# 699 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 3
  template<typename _InputIter1, typename _InputIter2, typename _BinaryPredicate>
    inline bool
    equal(_InputIter1 __first1, _InputIter1 __last1,
          _InputIter2 __first2,
          _BinaryPredicate __binary_pred)
    {

     
     

      for ( ; __first1 != __last1; ++__first1, ++__first2)
        if (!__binary_pred(*__first1, *__first2))
          return false;
      return true;
    }
# 732 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 3
  template<typename _InputIter1, typename _InputIter2>
    bool
    lexicographical_compare(_InputIter1 __first1, _InputIter1 __last1,
                            _InputIter2 __first2, _InputIter2 __last2)
    {

     
     
     

     


      for ( ; __first1 != __last1 && __first2 != __last2
            ; ++__first1, ++__first2) {
        if (*__first1 < *__first2)
          return true;
        if (*__first2 < *__first1)
          return false;
      }
      return __first1 == __last1 && __first2 != __last2;
    }
# 767 "/usr/include/gcc/darwin/3.3/c++/bits/stl_algobase.h" 3
  template<typename _InputIter1, typename _InputIter2, typename _Compare>
    bool
    lexicographical_compare(_InputIter1 __first1, _InputIter1 __last1,
                            _InputIter2 __first2, _InputIter2 __last2,
                            _Compare __comp)
    {

     
     

      for ( ; __first1 != __last1 && __first2 != __last2
            ; ++__first1, ++__first2) {
        if (__comp(*__first1, *__first2))
          return true;
        if (__comp(*__first2, *__first1))
          return false;
      }
      return __first1 == __last1 && __first2 != __last2;
    }

  inline bool
  lexicographical_compare(const unsigned char* __first1, const unsigned char* __last1,
                          const unsigned char* __first2, const unsigned char* __last2)
  {
    const size_t __len1 = __last1 - __first1;
    const size_t __len2 = __last2 - __first2;
    const int __result = memcmp(__first1, __first2, min(__len1, __len2));
    return __result != 0 ? __result < 0 : __len1 < __len2;
  }

  inline bool
  lexicographical_compare(const char* __first1, const char* __last1,
                          const char* __first2, const char* __last2)
  {

    return lexicographical_compare((const signed char*) __first1,
                                   (const signed char*) __last1,
                                   (const signed char*) __first2,
                                   (const signed char*) __last2);






  }

}
# 68 "/usr/include/gcc/darwin/3.3/c++/vector" 2 3
# 1 "/usr/include/gcc/darwin/3.3/c++/bits/stl_alloc.h" 1 3
# 88 "/usr/include/gcc/darwin/3.3/c++/bits/stl_alloc.h" 3
# 1 "/usr/include/gcc/darwin/3.3/c++/bits/functexcept.h" 1 3
# 34 "/usr/include/gcc/darwin/3.3/c++/bits/functexcept.h" 3
# 1 "/usr/include/gcc/darwin/3.3/c++/exception_defines.h" 1 3
# 35 "/usr/include/gcc/darwin/3.3/c++/bits/functexcept.h" 2 3

namespace std
{

  void
  __throw_bad_exception(void);


  void
  __throw_bad_alloc(void);


  void
  __throw_bad_cast(void);

  void
  __throw_bad_typeid(void);


  void
  __throw_logic_error(const char* __s);

  void
  __throw_domain_error(const char* __s);

  void
  __throw_invalid_argument(const char* __s);

  void
  __throw_length_error(const char* __s);

  void
  __throw_out_of_range(const char* __s);

  void
  __throw_runtime_error(const char* __s);

  void
  __throw_range_error(const char* __s);

  void
  __throw_overflow_error(const char* __s);

  void
  __throw_underflow_error(const char* __s);


  void
  __throw_ios_failure(const char* __s);
}
# 89 "/usr/include/gcc/darwin/3.3/c++/bits/stl_alloc.h" 2 3
# 1 "/usr/include/gcc/darwin/3.3/c++/bits/stl_threads.h" 1 3
# 54 "/usr/include/gcc/darwin/3.3/c++/bits/stl_threads.h" 3
namespace std
{




  struct _Refcount_Base
  {

    typedef size_t _RC_t;


    volatile _RC_t _M_ref_count;


    __gthread_mutex_t _M_ref_count_lock;

    _Refcount_Base(_RC_t __n) : _M_ref_count(__n)
    {

      __gthread_mutex_t __tmp = {0x32AAABA7, {}};
      _M_ref_count_lock = __tmp;





    }

    void
    _M_incr()
    {
      __gthread_mutex_lock(&_M_ref_count_lock);
      ++_M_ref_count;
      __gthread_mutex_unlock(&_M_ref_count_lock);
    }

    _RC_t
    _M_decr()
    {
      __gthread_mutex_lock(&_M_ref_count_lock);
      volatile _RC_t __tmp = --_M_ref_count;
      __gthread_mutex_unlock(&_M_ref_count_lock);
      return __tmp;
    }
  };
# 109 "/usr/include/gcc/darwin/3.3/c++/bits/stl_threads.h" 3
  template<int __dummy>
    struct _Swap_lock_struct
    { static __gthread_mutex_t _S_swap_lock; };

  template<int __dummy>
    __gthread_mutex_t
    _Swap_lock_struct<__dummy>::_S_swap_lock = {0x32AAABA7, {}};



  inline unsigned long
  _Atomic_swap(unsigned long * __p, unsigned long __q)
  {
    __gthread_mutex_lock(&_Swap_lock_struct<0>::_S_swap_lock);
    unsigned long __result = *__p;
    *__p = __q;
    __gthread_mutex_unlock(&_Swap_lock_struct<0>::_S_swap_lock);
    return __result;
  }

}
# 155 "/usr/include/gcc/darwin/3.3/c++/bits/stl_threads.h" 3
namespace std
{
  struct _STL_mutex_lock
  {





    __gthread_mutex_t _M_lock;

    void
    _M_initialize()
    {
# 192 "/usr/include/gcc/darwin/3.3/c++/bits/stl_threads.h" 3
    }

    void
    _M_acquire_lock()
    {



      __gthread_mutex_lock(&_M_lock);
    }

    void
    _M_release_lock()
    {



      __gthread_mutex_unlock(&_M_lock);
    }
  };
# 228 "/usr/include/gcc/darwin/3.3/c++/bits/stl_threads.h" 3
  struct _STL_auto_lock
  {
    _STL_mutex_lock& _M_lock;

    _STL_auto_lock(_STL_mutex_lock& __lock) : _M_lock(__lock)
    { _M_lock._M_acquire_lock(); }

    ~_STL_auto_lock() { _M_lock._M_release_lock(); }

  private:
    void operator=(const _STL_auto_lock&);
    _STL_auto_lock(const _STL_auto_lock&);
  };

}
# 90 "/usr/include/gcc/darwin/3.3/c++/bits/stl_alloc.h" 2 3

# 1 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/atomicity.h" 1 3
# 39 "/usr/include/gcc/darwin/3.3/c++/ppc-darwin/bits/atomicity.h" 3
typedef int _Atomic_word;

static inline _Atomic_word
__attribute__ ((__unused__))
__exchange_and_add (volatile _Atomic_word* __mem, int __val)
{
  _Atomic_word __tmp, __res;
  __asm__ __volatile__ (
        "/* Inline exchange & add */\n"
        "0:\t"
        "lwarx    %0,0,%2 \n\t"
        "add%I3   %1,%0,%3 \n\t"
        "stwcx. " "  %1,0,%2 \n\t"
        "bne-     0b \n\t"
        "/* End exchange & add */"
        : "=&b"(__res), "=&r"(__tmp)
        : "r" (__mem), "Ir"(__val)
        : "cr0", "memory");
  return __res;
}

static inline void
__attribute__ ((__unused__))
__atomic_add (volatile _Atomic_word *__mem, int __val)
{
  _Atomic_word __tmp;
  __asm__ __volatile__ (
        "/* Inline atomic add */\n"
        "0:\t"
        "lwarx    %0,0,%1 \n\t"
        "add%I2   %0,%0,%2 \n\t"
        "stwcx. " "  %0,0,%1 \n\t"
        "bne-     0b \n\t"
        "/* End atomic add */"
        : "=&b"(__tmp)
        : "r" (__mem), "Ir"(__val)
        : "cr0", "memory");
}
# 92 "/usr/include/gcc/darwin/3.3/c++/bits/stl_alloc.h" 2 3

namespace std
{
# 103 "/usr/include/gcc/darwin/3.3/c++/bits/stl_alloc.h" 3
  class __new_alloc
  {
  public:
    static void*
    allocate(size_t __n)
    { return ::operator new(__n); }

    static void
    deallocate(void* __p, size_t)
    { ::operator delete(__p); }
  };
# 126 "/usr/include/gcc/darwin/3.3/c++/bits/stl_alloc.h" 3
  template<int __inst>
    class __malloc_alloc_template
    {
    private:
      static void* _S_oom_malloc(size_t);
      static void* _S_oom_realloc(void*, size_t);
      static void (* __malloc_alloc_oom_handler)();

    public:
      static void*
      allocate(size_t __n)
      {
        void* __result = malloc(__n);
        if (__builtin_expect(__result == 0, 0))
          __result = _S_oom_malloc(__n);
        return __result;
      }

      static void
      deallocate(void* __p, size_t )
      { free(__p); }

      static void*
      reallocate(void* __p, size_t , size_t __new_sz)
      {
        void* __result = realloc(__p, __new_sz);
        if (__builtin_expect(__result == 0, 0))
          __result = _S_oom_realloc(__p, __new_sz);
        return __result;
      }

      static void (* __set_malloc_handler(void (*__f)()))()
      {
        void (* __old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = __f;
        return __old;
      }
    };


  template<int __inst>
    void (* __malloc_alloc_template<__inst>::__malloc_alloc_oom_handler)() = 0;

  template<int __inst>
    void*
    __malloc_alloc_template<__inst>::
    _S_oom_malloc(size_t __n)
    {
      void (* __my_malloc_handler)();
      void* __result;

      for (;;)
        {
          __my_malloc_handler = __malloc_alloc_oom_handler;
          if (__builtin_expect(__my_malloc_handler == 0, 0))
            __throw_bad_alloc();
          (*__my_malloc_handler)();
          __result = malloc(__n);
          if (__result)
            return __result;
        }
    }

  template<int __inst>
    void*
    __malloc_alloc_template<__inst>::
    _S_oom_realloc(void* __p, size_t __n)
    {
      void (* __my_malloc_handler)();
      void* __result;

      for (;;)
        {
          __my_malloc_handler = __malloc_alloc_oom_handler;
          if (__builtin_expect(__my_malloc_handler == 0, 0))
            __throw_bad_alloc();
          (*__my_malloc_handler)();
          __result = realloc(__p, __n);
          if (__result)
            return __result;
        }
    }


  typedef __new_alloc __mem_interface;
# 223 "/usr/include/gcc/darwin/3.3/c++/bits/stl_alloc.h" 3
  template<typename _Tp, typename _Alloc>
    class __simple_alloc
    {
    public:
      static _Tp*
      allocate(size_t __n)
      {
        _Tp* __ret = 0;
        if (__n)
          __ret = static_cast<_Tp*>(_Alloc::allocate(__n * sizeof(_Tp)));
        return __ret;
      }

      static _Tp*
      allocate()
      { return (_Tp*) _Alloc::allocate(sizeof (_Tp)); }

      static void
      deallocate(_Tp* __p, size_t __n)
      { if (0 != __n) _Alloc::deallocate(__p, __n * sizeof (_Tp)); }

      static void
      deallocate(_Tp* __p)
      { _Alloc::deallocate(__p, sizeof (_Tp)); }
    };
# 261 "/usr/include/gcc/darwin/3.3/c++/bits/stl_alloc.h" 3
  template<typename _Alloc>
    class __debug_alloc
    {
    private:


      enum {_S_extra = 8};

    public:
      static void*
      allocate(size_t __n)
      {
        char* __result = (char*)_Alloc::allocate(__n + (int) _S_extra);
        *(size_t*)__result = __n;
        return __result + (int) _S_extra;
      }

      static void
      deallocate(void* __p, size_t __n)
      {
        char* __real_p = (char*)__p - (int) _S_extra;
        if (*(size_t*)__real_p != __n)
          abort();
        _Alloc::deallocate(__real_p, __n + (int) _S_extra);
      }

      static void*
      reallocate(void* __p, size_t __old_sz, size_t __new_sz)
      {
        char* __real_p = (char*)__p - (int) _S_extra;
        if (*(size_t*)__real_p != __old_sz)
          abort();
        char* __result = (char*) _Alloc::reallocate(__real_p,
                                                    __old_sz + (int) _S_extra,
                                                    __new_sz + (int) _S_extra);
        *(size_t*)__result = __new_sz;
        return __result + (int) _S_extra;
      }
    };
# 332 "/usr/include/gcc/darwin/3.3/c++/bits/stl_alloc.h" 3
  template<bool __threads, int __inst>
    class __default_alloc_template
    {
    private:
      enum {_ALIGN = 8};
      enum {_MAX_BYTES = 128};
      enum {_NFREELISTS = _MAX_BYTES / _ALIGN};

      union _Obj
      {
        union _Obj* _M_free_list_link;
        char _M_client_data[1];
      };

      static _Obj* volatile _S_free_list[_NFREELISTS];


      static char* _S_start_free;
      static char* _S_end_free;
      static size_t _S_heap_size;

      static _STL_mutex_lock _S_node_allocator_lock;

      static size_t
      _S_round_up(size_t __bytes)
      { return (((__bytes) + (size_t) _ALIGN-1) & ~((size_t) _ALIGN - 1)); }

      static size_t
      _S_freelist_index(size_t __bytes)
      { return (((__bytes) + (size_t)_ALIGN - 1)/(size_t)_ALIGN - 1); }



      static void*
      _S_refill(size_t __n);



      static char*
      _S_chunk_alloc(size_t __size, int& __nobjs);



      struct _Lock
      {
        _Lock() { if (__threads) _S_node_allocator_lock._M_acquire_lock(); }
        ~_Lock() { if (__threads) _S_node_allocator_lock._M_release_lock(); }
      } __attribute__ ((__unused__));
      friend struct _Lock;

      static _Atomic_word _S_force_new;

    public:

      static void*
      allocate(size_t __n)
      {
        void* __ret = 0;




        if (_S_force_new == 0)
          {
            if (getenv("GLIBCPP_FORCE_NEW"))
              __atomic_add(&_S_force_new, 1);
            else
              __atomic_add(&_S_force_new, -1);
          }

        if ((__n > (size_t) _MAX_BYTES) || (_S_force_new > 0))
          __ret = __new_alloc::allocate(__n);
        else
          {
            _Obj* volatile* __my_free_list = _S_free_list
              + _S_freelist_index(__n);



            _Lock __lock_instance;
            _Obj* __restrict__ __result = *__my_free_list;
            if (__builtin_expect(__result == 0, 0))
              __ret = _S_refill(_S_round_up(__n));
            else
              {
                *__my_free_list = __result -> _M_free_list_link;
                __ret = __result;
              }
            if (__builtin_expect(__ret == 0, 0))
              __throw_bad_alloc();
          }
        return __ret;
      }


      static void
      deallocate(void* __p, size_t __n)
      {
        if ((__n > (size_t) _MAX_BYTES) || (_S_force_new > 0))
          __new_alloc::deallocate(__p, __n);
        else
          {
            _Obj* volatile* __my_free_list = _S_free_list
              + _S_freelist_index(__n);
            _Obj* __q = (_Obj*)__p;




            _Lock __lock_instance;
            __q -> _M_free_list_link = *__my_free_list;
            *__my_free_list = __q;
          }
      }

      static void*
      reallocate(void* __p, size_t __old_sz, size_t __new_sz);
    };

  template<bool __threads, int __inst> _Atomic_word
  __default_alloc_template<__threads, __inst>::_S_force_new = 0;

  template<bool __threads, int __inst>
    inline bool
    operator==(const __default_alloc_template<__threads,__inst>&,
               const __default_alloc_template<__threads,__inst>&)
    { return true; }

  template<bool __threads, int __inst>
    inline bool
    operator!=(const __default_alloc_template<__threads,__inst>&,
               const __default_alloc_template<__threads,__inst>&)
    { return false; }





  template<bool __threads, int __inst>
    char*
    __default_alloc_template<__threads, __inst>::
    _S_chunk_alloc(size_t __size, int& __nobjs)
    {
      char* __result;
      size_t __total_bytes = __size * __nobjs;
      size_t __bytes_left = _S_end_free - _S_start_free;

      if (__bytes_left >= __total_bytes)
        {
          __result = _S_start_free;
          _S_start_free += __total_bytes;
          return __result ;
        }
      else if (__bytes_left >= __size)
        {
          __nobjs = (int)(__bytes_left/__size);
          __total_bytes = __size * __nobjs;
          __result = _S_start_free;
          _S_start_free += __total_bytes;
          return __result;
        }
      else
        {
          size_t __bytes_to_get =
            2 * __total_bytes + _S_round_up(_S_heap_size >> 4);

          if (__bytes_left > 0)
            {
              _Obj* volatile* __my_free_list =
                _S_free_list + _S_freelist_index(__bytes_left);

              ((_Obj*)_S_start_free) -> _M_free_list_link = *__my_free_list;
              *__my_free_list = (_Obj*)_S_start_free;
            }
          _S_start_free = (char*) __new_alloc::allocate(__bytes_to_get);
          if (_S_start_free == 0)
            {
              size_t __i;
              _Obj* volatile* __my_free_list;
              _Obj* __p;



              __i = __size;
              for (; __i <= (size_t) _MAX_BYTES; __i += (size_t) _ALIGN)
                {
                  __my_free_list = _S_free_list + _S_freelist_index(__i);
                  __p = *__my_free_list;
                  if (__p != 0)
                    {
                      *__my_free_list = __p -> _M_free_list_link;
                      _S_start_free = (char*)__p;
                      _S_end_free = _S_start_free + __i;
                      return _S_chunk_alloc(__size, __nobjs);


                    }
                }
              _S_end_free = 0;
              _S_start_free = (char*)__new_alloc::allocate(__bytes_to_get);


            }
          _S_heap_size += __bytes_to_get;
          _S_end_free = _S_start_free + __bytes_to_get;
          return _S_chunk_alloc(__size, __nobjs);
        }
    }





  template<bool __threads, int __inst>
    void*
    __default_alloc_template<__threads, __inst>::_S_refill(size_t __n)
    {
      int __nobjs = 20;
      char* __chunk = _S_chunk_alloc(__n, __nobjs);
      _Obj* volatile* __my_free_list;
      _Obj* __result;
      _Obj* __current_obj;
      _Obj* __next_obj;
      int __i;

      if (1 == __nobjs)
        return __chunk;
      __my_free_list = _S_free_list + _S_freelist_index(__n);


      __result = (_Obj*)__chunk;
      *__my_free_list = __next_obj = (_Obj*)(__chunk + __n);
      for (__i = 1; ; __i++)
        {
          __current_obj = __next_obj;
          __next_obj = (_Obj*)((char*)__next_obj + __n);
          if (__nobjs - 1 == __i)
            {
              __current_obj -> _M_free_list_link = 0;
              break;
            }
          else
            __current_obj -> _M_free_list_link = __next_obj;
        }
      return __result;
    }


  template<bool threads, int inst>
    void*
    __default_alloc_template<threads, inst>::
    reallocate(void* __p, size_t __old_sz, size_t __new_sz)
    {
      void* __result;
      size_t __copy_sz;

      if (__old_sz > (size_t) _MAX_BYTES && __new_sz > (size_t) _MAX_BYTES)
        return(realloc(__p, __new_sz));
      if (_S_round_up(__old_sz) == _S_round_up(__new_sz))
        return(__p);
      __result = allocate(__new_sz);
      __copy_sz = __new_sz > __old_sz? __old_sz : __new_sz;
      memcpy(__result, __p, __copy_sz);
      deallocate(__p, __old_sz);
      return __result;
    }

  template<bool __threads, int __inst>
    _STL_mutex_lock
    __default_alloc_template<__threads,__inst>::_S_node_allocator_lock
    = { {0x32AAABA7, {}} };

  template<bool __threads, int __inst>
    char* __default_alloc_template<__threads,__inst>::_S_start_free = 0;

  template<bool __threads, int __inst>
    char* __default_alloc_template<__threads,__inst>::_S_end_free = 0;

  template<bool __threads, int __inst>
    size_t __default_alloc_template<__threads,__inst>::_S_heap_size = 0;

  template<bool __threads, int __inst>
    typename __default_alloc_template<__threads,__inst>::_Obj* volatile
    __default_alloc_template<__threads,__inst>::_S_free_list[_NFREELISTS];

  typedef __default_alloc_template<true,0> __alloc;
  typedef __default_alloc_template<false,0> __single_client_alloc;
# 635 "/usr/include/gcc/darwin/3.3/c++/bits/stl_alloc.h" 3
  template<typename _Tp>
    class allocator
    {
      typedef __alloc _Alloc;
    public:
      typedef size_t size_type;
      typedef ptrdiff_t difference_type;
      typedef _Tp* pointer;
      typedef const _Tp* const_pointer;
      typedef _Tp& reference;
      typedef const _Tp& const_reference;
      typedef _Tp value_type;

      template<typename _Tp1>
        struct rebind
        { typedef allocator<_Tp1> other; };

      allocator() throw() {}
      allocator(const allocator&) throw() {}
      template<typename _Tp1>
        allocator(const allocator<_Tp1>&) throw() {}
      ~allocator() throw() {}

      pointer
      address(reference __x) const { return &__x; }

      const_pointer
      address(const_reference __x) const { return &__x; }



      _Tp*
      allocate(size_type __n, const void* = 0)
      {
        _Tp* __ret = 0;
        if (__n)
          {
            if (__n <= this->max_size())
              __ret = static_cast<_Tp*>(_Alloc::allocate(__n * sizeof(_Tp)));
            else
              __throw_bad_alloc();
          }
        return __ret;
      }


      void
      deallocate(pointer __p, size_type __n)
      { _Alloc::deallocate(__p, __n * sizeof(_Tp)); }

      size_type
      max_size() const throw() { return size_t(-1) / sizeof(_Tp); }

      void construct(pointer __p, const _Tp& __val) { new(__p) _Tp(__val); }
      void destroy(pointer __p) { __p->~_Tp(); }
    };

  template<>
    class allocator<void>
    {
    public:
      typedef size_t size_type;
      typedef ptrdiff_t difference_type;
      typedef void* pointer;
      typedef const void* const_pointer;
      typedef void value_type;

      template<typename _Tp1>
        struct rebind
        { typedef allocator<_Tp1> other; };
    };


  template<typename _T1, typename _T2>
    inline bool
    operator==(const allocator<_T1>&, const allocator<_T2>&)
    { return true; }

  template<typename _T1, typename _T2>
    inline bool
    operator!=(const allocator<_T1>&, const allocator<_T2>&)
    { return false; }
# 731 "/usr/include/gcc/darwin/3.3/c++/bits/stl_alloc.h" 3
  template<typename _Tp, typename _Alloc>
    struct __allocator
    {
      _Alloc __underlying_alloc;

      typedef size_t size_type;
      typedef ptrdiff_t difference_type;
      typedef _Tp* pointer;
      typedef const _Tp* const_pointer;
      typedef _Tp& reference;
      typedef const _Tp& const_reference;
      typedef _Tp value_type;

      template<typename _Tp1>
        struct rebind
        { typedef __allocator<_Tp1, _Alloc> other; };

      __allocator() throw() {}
      __allocator(const __allocator& __a) throw()
      : __underlying_alloc(__a.__underlying_alloc) {}

      template<typename _Tp1>
        __allocator(const __allocator<_Tp1, _Alloc>& __a) throw()
        : __underlying_alloc(__a.__underlying_alloc) {}

      ~__allocator() throw() {}

      pointer
      address(reference __x) const { return &__x; }

      const_pointer
      address(const_reference __x) const { return &__x; }



      _Tp*
      allocate(size_type __n, const void* = 0)
      {
        _Tp* __ret = 0;
        if (__n)
          __ret = static_cast<_Tp*>(_Alloc::allocate(__n * sizeof(_Tp)));
        return __ret;
      }


      void
      deallocate(pointer __p, size_type __n)
      { __underlying_alloc.deallocate(__p, __n * sizeof(_Tp)); }

      size_type
      max_size() const throw() { return size_t(-1) / sizeof(_Tp); }

      void
      construct(pointer __p, const _Tp& __val) { new(__p) _Tp(__val); }

      void
      destroy(pointer __p) { __p->~_Tp(); }
    };

  template<typename _Alloc>
    struct __allocator<void, _Alloc>
    {
      typedef size_t size_type;
      typedef ptrdiff_t difference_type;
      typedef void* pointer;
      typedef const void* const_pointer;
      typedef void value_type;

      template<typename _Tp1>
        struct rebind
        { typedef __allocator<_Tp1, _Alloc> other; };
    };

  template<typename _Tp, typename _Alloc>
    inline bool
    operator==(const __allocator<_Tp,_Alloc>& __a1,
               const __allocator<_Tp,_Alloc>& __a2)
    { return __a1.__underlying_alloc == __a2.__underlying_alloc; }

  template<typename _Tp, typename _Alloc>
    inline bool
    operator!=(const __allocator<_Tp, _Alloc>& __a1,
               const __allocator<_Tp, _Alloc>& __a2)
    { return __a1.__underlying_alloc != __a2.__underlying_alloc; }







  template<int inst>
    inline bool
    operator==(const __malloc_alloc_template<inst>&,
               const __malloc_alloc_template<inst>&)
    { return true; }

  template<int __inst>
    inline bool
    operator!=(const __malloc_alloc_template<__inst>&,
               const __malloc_alloc_template<__inst>&)
    { return false; }

  template<typename _Alloc>
    inline bool
    operator==(const __debug_alloc<_Alloc>&, const __debug_alloc<_Alloc>&)
    { return true; }

  template<typename _Alloc>
    inline bool
    operator!=(const __debug_alloc<_Alloc>&, const __debug_alloc<_Alloc>&)
    { return false; }
# 884 "/usr/include/gcc/darwin/3.3/c++/bits/stl_alloc.h" 3
  template<typename _Tp, typename _Allocator>
    struct _Alloc_traits
    {
      static const bool _S_instanceless = false;
      typedef typename _Allocator::template rebind<_Tp>::other allocator_type;
    };

  template<typename _Tp, typename _Allocator>
    const bool _Alloc_traits<_Tp, _Allocator>::_S_instanceless;


  template<typename _Tp, typename _Tp1>
    struct _Alloc_traits<_Tp, allocator<_Tp1> >
    {
      static const bool _S_instanceless = true;
      typedef __simple_alloc<_Tp, __alloc> _Alloc_type;
      typedef allocator<_Tp> allocator_type;
    };




  template<typename _Tp, int __inst>
    struct _Alloc_traits<_Tp, __malloc_alloc_template<__inst> >
    {
      static const bool _S_instanceless = true;
      typedef __simple_alloc<_Tp, __malloc_alloc_template<__inst> > _Alloc_type;
      typedef __allocator<_Tp, __malloc_alloc_template<__inst> > allocator_type;
    };

  template<typename _Tp, bool __threads, int __inst>
    struct _Alloc_traits<_Tp, __default_alloc_template<__threads, __inst> >
    {
      static const bool _S_instanceless = true;
      typedef __simple_alloc<_Tp, __default_alloc_template<__threads, __inst> >
      _Alloc_type;
      typedef __allocator<_Tp, __default_alloc_template<__threads, __inst> >
      allocator_type;
    };

  template<typename _Tp, typename _Alloc>
    struct _Alloc_traits<_Tp, __debug_alloc<_Alloc> >
    {
      static const bool _S_instanceless = true;
      typedef __simple_alloc<_Tp, __debug_alloc<_Alloc> > _Alloc_type;
      typedef __allocator<_Tp, __debug_alloc<_Alloc> > allocator_type;
    };





  template<typename _Tp, typename _Tp1, int __inst>
    struct _Alloc_traits<_Tp,
                         __allocator<_Tp1, __malloc_alloc_template<__inst> > >
    {
      static const bool _S_instanceless = true;
      typedef __simple_alloc<_Tp, __malloc_alloc_template<__inst> > _Alloc_type;
      typedef __allocator<_Tp, __malloc_alloc_template<__inst> > allocator_type;
    };

  template<typename _Tp, typename _Tp1, bool __thr, int __inst>
    struct _Alloc_traits<_Tp, __allocator<_Tp1, __default_alloc_template<__thr, __inst> > >
    {
      static const bool _S_instanceless = true;
      typedef __simple_alloc<_Tp, __default_alloc_template<__thr,__inst> >
      _Alloc_type;
      typedef __allocator<_Tp, __default_alloc_template<__thr,__inst> >
      allocator_type;
    };

  template<typename _Tp, typename _Tp1, typename _Alloc>
    struct _Alloc_traits<_Tp, __allocator<_Tp1, __debug_alloc<_Alloc> > >
    {
      static const bool _S_instanceless = true;
      typedef __simple_alloc<_Tp, __debug_alloc<_Alloc> > _Alloc_type;
      typedef __allocator<_Tp, __debug_alloc<_Alloc> > allocator_type;
    };






  extern template class allocator<char>;
  extern template class allocator<wchar_t>;
  extern template class __default_alloc_template<true,0>;

}
# 69 "/usr/include/gcc/darwin/3.3/c++/vector" 2 3
# 1 "/usr/include/gcc/darwin/3.3/c++/bits/stl_construct.h" 1 3
# 67 "/usr/include/gcc/darwin/3.3/c++/bits/stl_construct.h" 3
namespace std
{






  template <class _T1, class _T2>
    inline void
    _Construct(_T1* __p, const _T2& __value)
    { new (static_cast<void*>(__p)) _T1(__value); }







  template <class _T1>
    inline void
    _Construct(_T1* __p)
    { new (static_cast<void*>(__p)) _T1(); }
# 98 "/usr/include/gcc/darwin/3.3/c++/bits/stl_construct.h" 3
  template <class _ForwardIterator>
    inline void
    __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, __false_type)
    { for ( ; __first != __last; ++__first) _Destroy(&*__first); }
# 112 "/usr/include/gcc/darwin/3.3/c++/bits/stl_construct.h" 3
  template <class _ForwardIterator>
    inline void
    __destroy_aux(_ForwardIterator, _ForwardIterator, __true_type)
    { }






  template <class _Tp>
    inline void
    _Destroy(_Tp* __pointer)
    { __pointer->~_Tp(); }
# 134 "/usr/include/gcc/darwin/3.3/c++/bits/stl_construct.h" 3
  template <class _ForwardIterator>
    inline void
    _Destroy(_ForwardIterator __first, _ForwardIterator __last)
    {
      typedef typename iterator_traits<_ForwardIterator>::value_type
                       _Value_type;
      typedef typename __type_traits<_Value_type>::has_trivial_destructor
                       _Has_trivial_destructor;

      __destroy_aux(__first, __last, _Has_trivial_destructor());
    }
}
# 70 "/usr/include/gcc/darwin/3.3/c++/vector" 2 3
# 1 "/usr/include/gcc/darwin/3.3/c++/bits/stl_uninitialized.h" 1 3
# 66 "/usr/include/gcc/darwin/3.3/c++/bits/stl_uninitialized.h" 3
namespace std
{



  template<typename _InputIter, typename _ForwardIter>
    inline _ForwardIter
    __uninitialized_copy_aux(_InputIter __first, _InputIter __last,
                             _ForwardIter __result,
                             __true_type)
    { return copy(__first, __last, __result); }

  template<typename _InputIter, typename _ForwardIter>
    _ForwardIter
    __uninitialized_copy_aux(_InputIter __first, _InputIter __last,
                             _ForwardIter __result,
                             __false_type)
    {
      _ForwardIter __cur = __result;
      try {
        for ( ; __first != __last; ++__first, ++__cur)
          _Construct(&*__cur, *__first);
        return __cur;
      }
      catch(...)
        {
          _Destroy(__result, __cur);
          throw;
        }
    }
# 106 "/usr/include/gcc/darwin/3.3/c++/bits/stl_uninitialized.h" 3
  template<typename _InputIter, typename _ForwardIter>
    inline _ForwardIter
    uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result)
    {
      typedef typename iterator_traits<_ForwardIter>::value_type _ValueType;
      typedef typename __type_traits<_ValueType>::is_POD_type _Is_POD;
      return __uninitialized_copy_aux(__first, __last, __result, _Is_POD());
    }

  inline char*
  uninitialized_copy(const char* __first, const char* __last, char* __result)
  {
    memmove(__result, __first, __last - __first);
    return __result + (__last - __first);
  }

  inline wchar_t*
  uninitialized_copy(const wchar_t* __first, const wchar_t* __last,
                     wchar_t* __result)
  {
    memmove(__result, __first, sizeof(wchar_t) * (__last - __first));
    return __result + (__last - __first);
  }



  template<typename _ForwardIter, typename _Tp>
    inline void
    __uninitialized_fill_aux(_ForwardIter __first, _ForwardIter __last,
                             const _Tp& __x, __true_type)
    { fill(__first, __last, __x); }

  template<typename _ForwardIter, typename _Tp>
    void
    __uninitialized_fill_aux(_ForwardIter __first, _ForwardIter __last,
                             const _Tp& __x, __false_type)
    {
      _ForwardIter __cur = __first;
      try {
        for ( ; __cur != __last; ++__cur)
          _Construct(&*__cur, __x);
      }
      catch(...)
        {
          _Destroy(__first, __cur);
          throw;
        }
    }
# 164 "/usr/include/gcc/darwin/3.3/c++/bits/stl_uninitialized.h" 3
  template<typename _ForwardIter, typename _Tp>
    inline void
    uninitialized_fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __x)
    {
      typedef typename iterator_traits<_ForwardIter>::value_type _ValueType;
      typedef typename __type_traits<_ValueType>::is_POD_type _Is_POD;
      __uninitialized_fill_aux(__first, __last, __x, _Is_POD());
    }



  template<typename _ForwardIter, typename _Size, typename _Tp>
    inline _ForwardIter
    __uninitialized_fill_n_aux(_ForwardIter __first, _Size __n,
                               const _Tp& __x, __true_type)
    {
      return fill_n(__first, __n, __x);
    }

  template<typename _ForwardIter, typename _Size, typename _Tp>
    _ForwardIter
    __uninitialized_fill_n_aux(_ForwardIter __first, _Size __n,
                               const _Tp& __x, __false_type)
    {
      _ForwardIter __cur = __first;
      try {
        for ( ; __n > 0; --__n, ++__cur)
          _Construct(&*__cur, __x);
        return __cur;
      }
      catch(...)
        {
          _Destroy(__first, __cur);
          throw;
        }
    }
# 210 "/usr/include/gcc/darwin/3.3/c++/bits/stl_uninitialized.h" 3
  template<typename _ForwardIter, typename _Size, typename _Tp>
    inline _ForwardIter
    uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
    {
      typedef typename iterator_traits<_ForwardIter>::value_type _ValueType;
      typedef typename __type_traits<_ValueType>::is_POD_type _Is_POD;
      return __uninitialized_fill_n_aux(__first, __n, __x, _Is_POD());
    }
# 227 "/usr/include/gcc/darwin/3.3/c++/bits/stl_uninitialized.h" 3
  template<typename _InputIter1, typename _InputIter2, typename _ForwardIter>
    inline _ForwardIter
    __uninitialized_copy_copy(_InputIter1 __first1, _InputIter1 __last1,
                              _InputIter2 __first2, _InputIter2 __last2,
                              _ForwardIter __result)
    {
      _ForwardIter __mid = uninitialized_copy(__first1, __last1, __result);
      try {
        return uninitialized_copy(__first2, __last2, __mid);
      }
      catch(...)
        {
          _Destroy(__result, __mid);
          throw;
        }
    }




  template<typename _ForwardIter, typename _Tp, typename _InputIter>
    inline _ForwardIter
    __uninitialized_fill_copy(_ForwardIter __result, _ForwardIter __mid,
                              const _Tp& __x,
                              _InputIter __first, _InputIter __last)
    {
      uninitialized_fill(__result, __mid, __x);
      try {
        return uninitialized_copy(__first, __last, __mid);
      }
      catch(...)
        {
          _Destroy(__result, __mid);
          throw;
        }
    }




  template<typename _InputIter, typename _ForwardIter, typename _Tp>
    inline void
    __uninitialized_copy_fill(_InputIter __first1, _InputIter __last1,
                              _ForwardIter __first2, _ForwardIter __last2,
                              const _Tp& __x)
    {
      _ForwardIter __mid2 = uninitialized_copy(__first1, __last1, __first2);
      try {
        uninitialized_fill(__mid2, __last2, __x);
      }
      catch(...)
        {
          _Destroy(__first2, __mid2);
          throw;
        }
    }

}
# 71 "/usr/include/gcc/darwin/3.3/c++/vector" 2 3
# 1 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 1 3
# 65 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
# 1 "/usr/include/gcc/darwin/3.3/c++/bits/functexcept.h" 1 3
# 34 "/usr/include/gcc/darwin/3.3/c++/bits/functexcept.h" 3
# 1 "/usr/include/gcc/darwin/3.3/c++/exception_defines.h" 1 3
# 35 "/usr/include/gcc/darwin/3.3/c++/bits/functexcept.h" 2 3

namespace std
{

  void
  __throw_bad_exception(void);


  void
  __throw_bad_alloc(void);


  void
  __throw_bad_cast(void);

  void
  __throw_bad_typeid(void);


  void
  __throw_logic_error(const char* __s);

  void
  __throw_domain_error(const char* __s);

  void
  __throw_invalid_argument(const char* __s);

  void
  __throw_length_error(const char* __s);

  void
  __throw_out_of_range(const char* __s);

  void
  __throw_runtime_error(const char* __s);

  void
  __throw_range_error(const char* __s);

  void
  __throw_overflow_error(const char* __s);

  void
  __throw_underflow_error(const char* __s);


  void
  __throw_ios_failure(const char* __s);
}
# 66 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 2 3


namespace std
{






  template<typename _Tp, typename _Allocator, bool _IsStatic>
    class _Vector_alloc_base
    {
    public:
      typedef typename _Alloc_traits<_Tp, _Allocator>::allocator_type
      allocator_type;

      allocator_type
      get_allocator() const { return _M_data_allocator; }

      _Vector_alloc_base(const allocator_type& __a)
      : _M_data_allocator(__a), _M_start(0), _M_finish(0), _M_end_of_storage(0)
      { }

    protected:
      allocator_type _M_data_allocator;
      _Tp* _M_start;
      _Tp* _M_finish;
      _Tp* _M_end_of_storage;

      _Tp*
      _M_allocate(size_t __n) { return _M_data_allocator.allocate(__n); }

      void
      _M_deallocate(_Tp* __p, size_t __n)
      { if (__p) _M_data_allocator.deallocate(__p, __n); }
    };


  template<typename _Tp, typename _Allocator>
    class _Vector_alloc_base<_Tp, _Allocator, true>
    {
    public:
      typedef typename _Alloc_traits<_Tp, _Allocator>::allocator_type
             allocator_type;

      allocator_type
      get_allocator() const { return allocator_type(); }

      _Vector_alloc_base(const allocator_type&)
      : _M_start(0), _M_finish(0), _M_end_of_storage(0)
      { }

    protected:
      _Tp* _M_start;
      _Tp* _M_finish;
      _Tp* _M_end_of_storage;

      typedef typename _Alloc_traits<_Tp, _Allocator>::_Alloc_type _Alloc_type;

      _Tp*
      _M_allocate(size_t __n) { return _Alloc_type::allocate(__n); }

      void
      _M_deallocate(_Tp* __p, size_t __n) { _Alloc_type::deallocate(__p, __n);}
    };







  template<typename _Tp, typename _Alloc>
    struct _Vector_base
    : public _Vector_alloc_base<_Tp, _Alloc,
                                _Alloc_traits<_Tp, _Alloc>::_S_instanceless>
    {
    public:
      typedef _Vector_alloc_base<_Tp, _Alloc,
                                 _Alloc_traits<_Tp, _Alloc>::_S_instanceless>
         _Base;
      typedef typename _Base::allocator_type allocator_type;

      _Vector_base(const allocator_type& __a)
      : _Base(__a) { }

      _Vector_base(size_t __n, const allocator_type& __a)
      : _Base(__a)
      {
        _M_start = _M_allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + __n;
      }

      ~_Vector_base()
      { _M_deallocate(_M_start, _M_end_of_storage - _M_start); }
    };
# 184 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
  template<typename _Tp, typename _Alloc = allocator<_Tp> >
    class vector : protected _Vector_base<_Tp, _Alloc>
    {

     

      typedef _Vector_base<_Tp, _Alloc> _Base;
      typedef vector<_Tp, _Alloc> vector_type;

    public:
      typedef _Tp value_type;
      typedef value_type* pointer;
      typedef const value_type* const_pointer;
      typedef __gnu_cxx::__normal_iterator<pointer, vector_type> iterator;
      typedef __gnu_cxx::__normal_iterator<const_pointer, vector_type>
      const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
      typedef std::reverse_iterator<iterator> reverse_iterator;
      typedef value_type& reference;
      typedef const value_type& const_reference;
      typedef size_t size_type;
      typedef ptrdiff_t difference_type;
      typedef typename _Base::allocator_type allocator_type;

    protected:






      using _Base::_M_allocate;
      using _Base::_M_deallocate;
      using _Base::_M_start;
      using _Base::_M_finish;
      using _Base::_M_end_of_storage;

    public:





      explicit
      vector(const allocator_type& __a = allocator_type())
      : _Base(__a) { }
# 238 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
      vector(size_type __n, const value_type& __value,
             const allocator_type& __a = allocator_type())
      : _Base(__n, __a)
      { _M_finish = uninitialized_fill_n(_M_start, __n, __value); }
# 250 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
      explicit
      vector(size_type __n)
      : _Base(__n, allocator_type())
      { _M_finish = uninitialized_fill_n(_M_start, __n, value_type()); }
# 264 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
      vector(const vector& __x)
      : _Base(__x.size(), __x.get_allocator())
      { _M_finish = uninitialized_copy(__x.begin(), __x.end(), _M_start); }
# 282 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
      template<typename _InputIterator>
        vector(_InputIterator __first, _InputIterator __last,
               const allocator_type& __a = allocator_type())
        : _Base(__a)
        {

          typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
          _M_initialize_dispatch(__first, __last, _Integral());
        }






      ~vector() { _Destroy(_M_start, _M_finish); }
# 307 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
      vector&
      operator=(const vector& __x);
# 320 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
      void
      assign(size_type __n, const value_type& __val)
      { _M_fill_assign(__n, __val); }
# 336 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
      template<typename _InputIterator>
        void
        assign(_InputIterator __first, _InputIterator __last)
        {

          typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
          _M_assign_dispatch(__first, __last, _Integral());
        }


      allocator_type
      get_allocator() const { return _Base::get_allocator(); }






      iterator
      begin() { return iterator (_M_start); }






      const_iterator
      begin() const { return const_iterator (_M_start); }






      iterator
      end() { return iterator (_M_finish); }





      const_iterator
      end() const { return const_iterator (_M_finish); }






      reverse_iterator
      rbegin() { return reverse_iterator(end()); }






      const_reverse_iterator
      rbegin() const { return const_reverse_iterator(end()); }






      reverse_iterator
      rend() { return reverse_iterator(begin()); }






      const_reverse_iterator
      rend() const { return const_reverse_iterator(begin()); }



      size_type
      size() const { return size_type(end() - begin()); }


      size_type
      max_size() const { return size_type(-1) / sizeof(value_type); }
# 432 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
      void
      resize(size_type __new_size, const value_type& __x)
      {
        if (__new_size < size())
          erase(begin() + __new_size, end());
        else
          insert(end(), __new_size - size(), __x);
      }
# 451 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
      void
      resize(size_type __new_size) { resize(__new_size, value_type()); }





      size_type
      capacity() const
      { return size_type(const_iterator(_M_end_of_storage) - begin()); }





      bool
      empty() const { return begin() == end(); }
# 486 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
      void
      reserve(size_type __n);
# 500 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
      reference
      operator[](size_type __n) { return *(begin() + __n); }
# 514 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
      const_reference
      operator[](size_type __n) const { return *(begin() + __n); }

    protected:

      void
      _M_range_check(size_type __n) const
      {
        if (__n >= this->size())
          __throw_out_of_range("vector [] access out of range");
      }

    public:
# 538 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
      reference
      at(size_type __n) { _M_range_check(__n); return (*this)[__n]; }
# 552 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
      const_reference
      at(size_type __n) const { _M_range_check(__n); return (*this)[__n]; }





      reference
      front() { return *begin(); }





      const_reference
      front() const { return *begin(); }





      reference
      back() { return *(end() - 1); }





      const_reference
      back() const { return *(end() - 1); }
# 594 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
      void
      push_back(const value_type& __x)
      {
        if (_M_finish != _M_end_of_storage)
          {
            _Construct(_M_finish, __x);
            ++_M_finish;
          }
        else
          _M_insert_aux(end(), __x);
      }
# 614 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
      void
      pop_back()
      {
        --_M_finish;
        _Destroy(_M_finish);
      }
# 632 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
      iterator
      insert(iterator __position, const value_type& __x);
# 669 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
      void
      insert(iterator __pos, size_type __n, const value_type& __x)
      { _M_fill_insert(__pos, __n, __x); }
# 687 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
      template<typename _InputIterator>
        void
        insert(iterator __pos, _InputIterator __first, _InputIterator __last)
        {

          typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
          _M_insert_dispatch(__pos, __first, __last, _Integral());
        }
# 711 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
      iterator
      erase(iterator __position);
# 732 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
      iterator
      erase(iterator __first, iterator __last);
# 744 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
      void
      swap(vector& __x)
      {
        std::swap(_M_start, __x._M_start);
        std::swap(_M_finish, __x._M_finish);
        std::swap(_M_end_of_storage, __x._M_end_of_storage);
      }







      void
      clear() { erase(begin(), end()); }

    protected:






      template<typename _ForwardIterator>
        pointer
        _M_allocate_and_copy(size_type __n,
                             _ForwardIterator __first, _ForwardIterator __last)
        {
          pointer __result = _M_allocate(__n);
          try
            {
              uninitialized_copy(__first, __last, __result);
              return __result;
            }
          catch(...)
            {
              _M_deallocate(__result, __n);
              throw;
            }
        }





      template<typename _Integer>
        void
        _M_initialize_dispatch(_Integer __n, _Integer __value, __true_type)
        {
          _M_start = _M_allocate(__n);
          _M_end_of_storage = _M_start + __n;
          _M_finish = uninitialized_fill_n(_M_start, __n, __value);
        }


      template<typename _InputIter>
        void
        _M_initialize_dispatch(_InputIter __first, _InputIter __last,
                               __false_type)
        {
          typedef typename iterator_traits<_InputIter>::iterator_category
            _IterCategory;
          _M_range_initialize(__first, __last, _IterCategory());
        }


      template<typename _InputIterator>
        void
        _M_range_initialize(_InputIterator __first,
                            _InputIterator __last, input_iterator_tag)
        {
          for ( ; __first != __last; ++__first)
            push_back(*__first);
        }


      template<typename _ForwardIterator>
        void
        _M_range_initialize(_ForwardIterator __first,
                            _ForwardIterator __last, forward_iterator_tag)
        {
          size_type __n = distance(__first, __last);
          _M_start = _M_allocate(__n);
          _M_end_of_storage = _M_start + __n;
          _M_finish = uninitialized_copy(__first, __last, _M_start);
        }






      template<typename _Integer>
        void
        _M_assign_dispatch(_Integer __n, _Integer __val, __true_type)
        {
          _M_fill_assign(static_cast<size_type>(__n),
                         static_cast<value_type>(__val));
        }


      template<typename _InputIter>
        void
        _M_assign_dispatch(_InputIter __first, _InputIter __last, __false_type)
        {
          typedef typename iterator_traits<_InputIter>::iterator_category
            _IterCategory;
          _M_assign_aux(__first, __last, _IterCategory());
        }


      template<typename _InputIterator>
        void
        _M_assign_aux(_InputIterator __first, _InputIterator __last,
                      input_iterator_tag);


      template<typename _ForwardIterator>
        void
        _M_assign_aux(_ForwardIterator __first, _ForwardIterator __last,
                      forward_iterator_tag);



      void
      _M_fill_assign(size_type __n, const value_type& __val);





      template<typename _Integer>
        void
        _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __val,
                           __true_type)
        {
          _M_fill_insert(__pos, static_cast<size_type>(__n),
                         static_cast<value_type>(__val));
        }


      template<typename _InputIterator>
        void
        _M_insert_dispatch(iterator __pos, _InputIterator __first,
                           _InputIterator __last, __false_type)
        {
          typedef typename iterator_traits<_InputIterator>::iterator_category
            _IterCategory;
          _M_range_insert(__pos, __first, __last, _IterCategory());
        }


      template<typename _InputIterator>
        void
        _M_range_insert(iterator __pos, _InputIterator __first,
                        _InputIterator __last, input_iterator_tag);


      template<typename _ForwardIterator>
        void
        _M_range_insert(iterator __pos, _ForwardIterator __first,
                        _ForwardIterator __last, forward_iterator_tag);



      void
      _M_fill_insert(iterator __pos, size_type __n, const value_type& __x);


      void
      _M_insert_aux(iterator __position, const value_type& __x);





    };
# 934 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
  template<typename _Tp, typename _Alloc>
    inline bool
    operator==(const vector<_Tp,_Alloc>& __x, const vector<_Tp,_Alloc>& __y)
    {
      return __x.size() == __y.size() &&
             equal(__x.begin(), __x.end(), __y.begin());
    }
# 953 "/usr/include/gcc/darwin/3.3/c++/bits/stl_vector.h" 3
  template<typename _Tp, typename _Alloc>
    inline bool
    operator<(const vector<_Tp,_Alloc>& __x, const vector<_Tp,_Alloc>& __y)
    {
      return lexicographical_compare(__x.begin(), __x.end(),
                                     __y.begin(), __y.end());
    }


  template<typename _Tp, typename _Alloc>
    inline bool
    operator!=(const vector<_Tp,_Alloc>& __x, const vector<_Tp,_Alloc>& __y)
    { return !(__x == __y); }


  template<typename _Tp, typename _Alloc>
    inline bool
    operator>(const vector<_Tp,_Alloc>& __x, const vector<_Tp,_Alloc>& __y)
    { return __y < __x; }


  template<typename _Tp, typename _Alloc>
    inline bool
    operator<=(const vector<_Tp,_Alloc>& __x, const vector<_Tp,_Alloc>& __y)
    { return !(__y < __x); }


  template<typename _Tp, typename _Alloc>
    inline bool
    operator>=(const vector<_Tp,_Alloc>& __x, const vector<_Tp,_Alloc>& __y)
    { return !(__x < __y); }


  template<typename _Tp, typename _Alloc>
    inline void
    swap(vector<_Tp,_Alloc>& __x, vector<_Tp,_Alloc>& __y)
    { __x.swap(__y); }
}
# 72 "/usr/include/gcc/darwin/3.3/c++/vector" 2 3
# 1 "/usr/include/gcc/darwin/3.3/c++/bits/stl_bvector.h" 1 3
# 64 "/usr/include/gcc/darwin/3.3/c++/bits/stl_bvector.h" 3
namespace std
{
  typedef unsigned long _Bit_type;
  enum { _M_word_bit = int(8 * sizeof(_Bit_type)) };

struct _Bit_reference {

  _Bit_type * _M_p;
  _Bit_type _M_mask;
  _Bit_reference(_Bit_type * __x, _Bit_type __y)
    : _M_p(__x), _M_mask(__y) {}

public:
  _Bit_reference() : _M_p(0), _M_mask(0) {}
  operator bool() const { return !!(*_M_p & _M_mask); }
  _Bit_reference& operator=(bool __x)
  {
    if (__x) *_M_p |= _M_mask;
    else *_M_p &= ~_M_mask;
    return *this;
  }
  _Bit_reference& operator=(const _Bit_reference& __x)
    { return *this = bool(__x); }
  bool operator==(const _Bit_reference& __x) const
    { return bool(*this) == bool(__x); }
  bool operator<(const _Bit_reference& __x) const
    { return !bool(*this) && bool(__x); }
  void flip() { *_M_p ^= _M_mask; }
};

struct _Bit_iterator_base : public iterator<random_access_iterator_tag, bool>
{
  _Bit_type * _M_p;
  unsigned int _M_offset;

  _Bit_iterator_base(_Bit_type * __x, unsigned int __y)
    : _M_p(__x), _M_offset(__y) {}

  void _M_bump_up() {
    if (_M_offset++ == _M_word_bit - 1) {
      _M_offset = 0;
      ++_M_p;
    }
  }
  void _M_bump_down() {
    if (_M_offset-- == 0) {
      _M_offset = _M_word_bit - 1;
      --_M_p;
    }
  }

  void _M_incr(ptrdiff_t __i) {
    difference_type __n = __i + _M_offset;
    _M_p += __n / _M_word_bit;
    __n = __n % _M_word_bit;
    if (__n < 0) {
      _M_offset = (unsigned int) __n + _M_word_bit;
      --_M_p;
    } else
      _M_offset = (unsigned int) __n;
  }

  bool operator==(const _Bit_iterator_base& __i) const {
    return _M_p == __i._M_p && _M_offset == __i._M_offset;
  }
  bool operator<(const _Bit_iterator_base& __i) const {
    return _M_p < __i._M_p || (_M_p == __i._M_p && _M_offset < __i._M_offset);
  }
  bool operator!=(const _Bit_iterator_base& __i) const {
    return !(*this == __i);
  }
  bool operator>(const _Bit_iterator_base& __i) const {
    return __i < *this;
  }
  bool operator<=(const _Bit_iterator_base& __i) const {
    return !(__i < *this);
  }
  bool operator>=(const _Bit_iterator_base& __i) const {
    return !(*this < __i);
  }
};

inline ptrdiff_t
operator-(const _Bit_iterator_base& __x, const _Bit_iterator_base& __y) {
  return _M_word_bit * (__x._M_p - __y._M_p) + __x._M_offset - __y._M_offset;
}


struct _Bit_iterator : public _Bit_iterator_base
{
  typedef _Bit_reference reference;
  typedef _Bit_reference* pointer;
  typedef _Bit_iterator iterator;

  _Bit_iterator() : _Bit_iterator_base(0, 0) {}
  _Bit_iterator(_Bit_type * __x, unsigned int __y)
    : _Bit_iterator_base(__x, __y) {}

  reference operator*() const { return reference(_M_p, 1UL << _M_offset); }
  iterator& operator++() {
    _M_bump_up();
    return *this;
  }
  iterator operator++(int) {
    iterator __tmp = *this;
    _M_bump_up();
    return __tmp;
  }
  iterator& operator--() {
    _M_bump_down();
    return *this;
  }
  iterator operator--(int) {
    iterator __tmp = *this;
    _M_bump_down();
    return __tmp;
  }
  iterator& operator+=(difference_type __i) {
    _M_incr(__i);
    return *this;
  }
  iterator& operator-=(difference_type __i) {
    *this += -__i;
    return *this;
  }
  iterator operator+(difference_type __i) const {
    iterator __tmp = *this;
    return __tmp += __i;
  }
  iterator operator-(difference_type __i) const {
    iterator __tmp = *this;
    return __tmp -= __i;
  }

  reference operator[](difference_type __i) { return *(*this + __i); }
};

inline _Bit_iterator
operator+(ptrdiff_t __n, const _Bit_iterator& __x) { return __x + __n; }


struct _Bit_const_iterator : public _Bit_iterator_base
{
  typedef bool reference;
  typedef bool const_reference;
  typedef const bool* pointer;
  typedef _Bit_const_iterator const_iterator;

  _Bit_const_iterator() : _Bit_iterator_base(0, 0) {}
  _Bit_const_iterator(_Bit_type * __x, unsigned int __y)
    : _Bit_iterator_base(__x, __y) {}
  _Bit_const_iterator(const _Bit_iterator& __x)
    : _Bit_iterator_base(__x._M_p, __x._M_offset) {}

  const_reference operator*() const {
    return _Bit_reference(_M_p, 1UL << _M_offset);
  }
  const_iterator& operator++() {
    _M_bump_up();
    return *this;
  }
  const_iterator operator++(int) {
    const_iterator __tmp = *this;
    _M_bump_up();
    return __tmp;
  }
  const_iterator& operator--() {
    _M_bump_down();
    return *this;
  }
  const_iterator operator--(int) {
    const_iterator __tmp = *this;
    _M_bump_down();
    return __tmp;
  }
  const_iterator& operator+=(difference_type __i) {
    _M_incr(__i);
    return *this;
  }
  const_iterator& operator-=(difference_type __i) {
    *this += -__i;
    return *this;
  }
  const_iterator operator+(difference_type __i) const {
    const_iterator __tmp = *this;
    return __tmp += __i;
  }
  const_iterator operator-(difference_type __i) const {
    const_iterator __tmp = *this;
    return __tmp -= __i;
  }
  const_reference operator[](difference_type __i) {
    return *(*this + __i);
  }
};

inline _Bit_const_iterator
operator+(ptrdiff_t __n, const _Bit_const_iterator& __x) { return __x + __n; }






template <class _Allocator, bool __is_static>
class _Bvector_alloc_base {
public:
  typedef typename _Alloc_traits<bool, _Allocator>::allocator_type
          allocator_type;
  allocator_type get_allocator() const { return _M_data_allocator; }

  _Bvector_alloc_base(const allocator_type& __a)
    : _M_data_allocator(__a), _M_start(), _M_finish(), _M_end_of_storage(0) {}

protected:
  _Bit_type * _M_bit_alloc(size_t __n)
    { return _M_data_allocator.allocate((__n + _M_word_bit - 1)/_M_word_bit); }
  void _M_deallocate() {
    if (_M_start._M_p)
      _M_data_allocator.deallocate(_M_start._M_p,
                                   _M_end_of_storage - _M_start._M_p);
  }

  typename _Alloc_traits<_Bit_type, _Allocator>::allocator_type
          _M_data_allocator;
  _Bit_iterator _M_start;
  _Bit_iterator _M_finish;
  _Bit_type * _M_end_of_storage;
};


template <class _Allocator>
class _Bvector_alloc_base<_Allocator, true> {
public:
  typedef typename _Alloc_traits<bool, _Allocator>::allocator_type
          allocator_type;
  allocator_type get_allocator() const { return allocator_type(); }

  _Bvector_alloc_base(const allocator_type&)
    : _M_start(), _M_finish(), _M_end_of_storage(0) {}

protected:
  typedef typename _Alloc_traits<_Bit_type, _Allocator>::_Alloc_type
          _Alloc_type;

  _Bit_type * _M_bit_alloc(size_t __n)
    { return _Alloc_type::allocate((__n + _M_word_bit - 1)/_M_word_bit); }
  void _M_deallocate() {
    if (_M_start._M_p)
      _Alloc_type::deallocate(_M_start._M_p,
                              _M_end_of_storage - _M_start._M_p);
  }

  _Bit_iterator _M_start;
  _Bit_iterator _M_finish;
  _Bit_type * _M_end_of_storage;
};

template <class _Alloc>
class _Bvector_base
  : public _Bvector_alloc_base<_Alloc,
                               _Alloc_traits<bool, _Alloc>::_S_instanceless>
{
  typedef _Bvector_alloc_base<_Alloc,
                              _Alloc_traits<bool, _Alloc>::_S_instanceless>
          _Base;
public:
  typedef typename _Base::allocator_type allocator_type;

  _Bvector_base(const allocator_type& __a) : _Base(__a) {}
  ~_Bvector_base() { _Base::_M_deallocate(); }
};

}



namespace std
{

template <typename _Alloc>
  class vector<bool, _Alloc> : public _Bvector_base<_Alloc>
  {
  public:
    typedef bool value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef _Bit_reference reference;
    typedef bool const_reference;
    typedef _Bit_reference* pointer;
    typedef const bool* const_pointer;

    typedef _Bit_iterator iterator;
    typedef _Bit_const_iterator const_iterator;

    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;

    typedef typename _Bvector_base<_Alloc>::allocator_type allocator_type;
    allocator_type get_allocator() const {
      return _Bvector_base<_Alloc>::get_allocator();
    }

  protected:
    using _Bvector_base<_Alloc>::_M_bit_alloc;
    using _Bvector_base<_Alloc>::_M_deallocate;
    using _Bvector_base<_Alloc>::_M_start;
    using _Bvector_base<_Alloc>::_M_finish;
    using _Bvector_base<_Alloc>::_M_end_of_storage;

  protected:
    void _M_initialize(size_type __n) {
      _Bit_type * __q = _M_bit_alloc(__n);
      _M_end_of_storage = __q + (__n + _M_word_bit - 1)/_M_word_bit;
      _M_start = iterator(__q, 0);
      _M_finish = _M_start + difference_type(__n);
    }
    void _M_insert_aux(iterator __position, bool __x) {
      if (_M_finish._M_p != _M_end_of_storage) {
        copy_backward(__position, _M_finish, _M_finish + 1);
        *__position = __x;
        ++_M_finish;
      }
      else {
        size_type __len = size()
                          ? 2 * size() : static_cast<size_type>(_M_word_bit);
        _Bit_type * __q = _M_bit_alloc(__len);
        iterator __i = copy(begin(), __position, iterator(__q, 0));
        *__i++ = __x;
        _M_finish = copy(__position, end(), __i);
        _M_deallocate();
        _M_end_of_storage = __q + (__len + _M_word_bit - 1)/_M_word_bit;
        _M_start = iterator(__q, 0);
      }
    }

    template <class _InputIterator>
    void _M_initialize_range(_InputIterator __first, _InputIterator __last,
                             input_iterator_tag) {
      _M_start = iterator();
      _M_finish = iterator();
      _M_end_of_storage = 0;
      for ( ; __first != __last; ++__first)
        push_back(*__first);
    }

    template <class _ForwardIterator>
    void _M_initialize_range(_ForwardIterator __first, _ForwardIterator __last,
                             forward_iterator_tag) {
      size_type __n = distance(__first, __last);
      _M_initialize(__n);
      copy(__first, __last, _M_start);
    }

    template <class _InputIterator>
    void _M_insert_range(iterator __pos,
                         _InputIterator __first, _InputIterator __last,
                         input_iterator_tag) {
      for ( ; __first != __last; ++__first) {
        __pos = insert(__pos, *__first);
        ++__pos;
      }
    }

    template <class _ForwardIterator>
    void _M_insert_range(iterator __position,
                         _ForwardIterator __first, _ForwardIterator __last,
                         forward_iterator_tag) {
      if (__first != __last) {
        size_type __n = distance(__first, __last);
        if (capacity() - size() >= __n) {
          copy_backward(__position, end(), _M_finish + difference_type(__n));
          copy(__first, __last, __position);
          _M_finish += difference_type(__n);
        }
        else {
          size_type __len = size() + max(size(), __n);
          _Bit_type * __q = _M_bit_alloc(__len);
          iterator __i = copy(begin(), __position, iterator(__q, 0));
          __i = copy(__first, __last, __i);
          _M_finish = copy(__position, end(), __i);
          _M_deallocate();
          _M_end_of_storage = __q + (__len + _M_word_bit - 1)/_M_word_bit;
          _M_start = iterator(__q, 0);
        }
      }
    }

  public:
    iterator begin() { return _M_start; }
    const_iterator begin() const { return _M_start; }
    iterator end() { return _M_finish; }
    const_iterator end() const { return _M_finish; }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const {
      return const_reverse_iterator(end());
    }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const {
      return const_reverse_iterator(begin());
    }

    size_type size() const { return size_type(end() - begin()); }
    size_type max_size() const { return size_type(-1); }
    size_type capacity() const {
      return size_type(const_iterator(_M_end_of_storage, 0) - begin());
    }
    bool empty() const { return begin() == end(); }

    reference operator[](size_type __n)
      { return *(begin() + difference_type(__n)); }
    const_reference operator[](size_type __n) const
      { return *(begin() + difference_type(__n)); }

    void _M_range_check(size_type __n) const {
      if (__n >= this->size())
        __throw_out_of_range("vector<bool>");
    }

    reference at(size_type __n)
      { _M_range_check(__n); return (*this)[__n]; }
    const_reference at(size_type __n) const
      { _M_range_check(__n); return (*this)[__n]; }

    explicit vector(const allocator_type& __a = allocator_type())
      : _Bvector_base<_Alloc>(__a) {}

    vector(size_type __n, bool __value,
              const allocator_type& __a = allocator_type())
      : _Bvector_base<_Alloc>(__a)
    {
      _M_initialize(__n);
      fill(_M_start._M_p, _M_end_of_storage, __value ? ~0 : 0);
    }

    explicit vector(size_type __n)
      : _Bvector_base<_Alloc>(allocator_type())
    {
      _M_initialize(__n);
      fill(_M_start._M_p, _M_end_of_storage, 0);
    }

    vector(const vector& __x) : _Bvector_base<_Alloc>(__x.get_allocator()) {
      _M_initialize(__x.size());
      copy(__x.begin(), __x.end(), _M_start);
    }



    template <class _Integer>
    void _M_initialize_dispatch(_Integer __n, _Integer __x, __true_type) {
      _M_initialize(__n);
      fill(_M_start._M_p, _M_end_of_storage, __x ? ~0 : 0);
    }

    template <class _InputIterator>
    void _M_initialize_dispatch(_InputIterator __first, _InputIterator __last,
                                __false_type) {
      _M_initialize_range(__first, __last, __iterator_category(__first));
    }

    template <class _InputIterator>
    vector(_InputIterator __first, _InputIterator __last,
             const allocator_type& __a = allocator_type())
      : _Bvector_base<_Alloc>(__a)
    {
      typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
      _M_initialize_dispatch(__first, __last, _Integral());
    }

    ~vector() { }

    vector& operator=(const vector& __x) {
      if (&__x == this) return *this;
      if (__x.size() > capacity()) {
        _M_deallocate();
        _M_initialize(__x.size());
      }
      copy(__x.begin(), __x.end(), begin());
      _M_finish = begin() + difference_type(__x.size());
      return *this;
    }






    void _M_fill_assign(size_t __n, bool __x) {
      if (__n > size()) {
        fill(_M_start._M_p, _M_end_of_storage, __x ? ~0 : 0);
        insert(end(), __n - size(), __x);
      }
      else {
        erase(begin() + __n, end());
        fill(_M_start._M_p, _M_end_of_storage, __x ? ~0 : 0);
      }
    }

    void assign(size_t __n, bool __x) { _M_fill_assign(__n, __x); }

    template <class _InputIterator>
    void assign(_InputIterator __first, _InputIterator __last) {
      typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
      _M_assign_dispatch(__first, __last, _Integral());
    }

    template <class _Integer>
    void _M_assign_dispatch(_Integer __n, _Integer __val, __true_type)
      { _M_fill_assign((size_t) __n, (bool) __val); }

    template <class _InputIter>
    void _M_assign_dispatch(_InputIter __first, _InputIter __last, __false_type)
      { _M_assign_aux(__first, __last, __iterator_category(__first)); }

    template <class _InputIterator>
    void _M_assign_aux(_InputIterator __first, _InputIterator __last,
                       input_iterator_tag) {
      iterator __cur = begin();
      for ( ; __first != __last && __cur != end(); ++__cur, ++__first)
        *__cur = *__first;
      if (__first == __last)
        erase(__cur, end());
      else
        insert(end(), __first, __last);
    }

    template <class _ForwardIterator>
    void _M_assign_aux(_ForwardIterator __first, _ForwardIterator __last,
                       forward_iterator_tag) {
      size_type __len = distance(__first, __last);
      if (__len < size())
        erase(copy(__first, __last, begin()), end());
      else {
        _ForwardIterator __mid = __first;
        advance(__mid, size());
        copy(__first, __mid, begin());
        insert(end(), __mid, __last);
      }
    }

    void reserve(size_type __n) {
      if (__n > this->max_size())
        __throw_length_error("vector::reserve");
      if (this->capacity() < __n) {
        _Bit_type * __q = _M_bit_alloc(__n);
        _M_finish = copy(begin(), end(), iterator(__q, 0));
        _M_deallocate();
        _M_start = iterator(__q, 0);
        _M_end_of_storage = __q + (__n + _M_word_bit - 1)/_M_word_bit;
      }
    }

    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }
    reference back() { return *(end() - 1); }
    const_reference back() const { return *(end() - 1); }
    void push_back(bool __x) {
      if (_M_finish._M_p != _M_end_of_storage)
        *_M_finish++ = __x;
      else
        _M_insert_aux(end(), __x);
    }
    void swap(vector<bool, _Alloc>& __x) {
      std::swap(_M_start, __x._M_start);
      std::swap(_M_finish, __x._M_finish);
      std::swap(_M_end_of_storage, __x._M_end_of_storage);
    }


    static void swap(reference __x, reference __y) {
      bool __tmp = __x;
      __x = __y;
      __y = __tmp;
    }

    iterator insert(iterator __position, bool __x = bool()) {
      difference_type __n = __position - begin();
      if (_M_finish._M_p != _M_end_of_storage && __position == end())
        *_M_finish++ = __x;
      else
        _M_insert_aux(__position, __x);
      return begin() + __n;
    }



    template <class _Integer>
    void _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __x,
                            __true_type) {
      _M_fill_insert(__pos, __n, __x);
    }

    template <class _InputIterator>
    void _M_insert_dispatch(iterator __pos,
                            _InputIterator __first, _InputIterator __last,
                            __false_type) {
      _M_insert_range(__pos, __first, __last, __iterator_category(__first));
    }

    template <class _InputIterator>
    void insert(iterator __position,
                _InputIterator __first, _InputIterator __last) {
      typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
      _M_insert_dispatch(__position, __first, __last, _Integral());
    }

    void _M_fill_insert(iterator __position, size_type __n, bool __x) {
      if (__n == 0) return;
      if (capacity() - size() >= __n) {
        copy_backward(__position, end(), _M_finish + difference_type(__n));
        fill(__position, __position + difference_type(__n), __x);
        _M_finish += difference_type(__n);
      }
      else {
        size_type __len = size() + max(size(), __n);
        _Bit_type * __q = _M_bit_alloc(__len);
        iterator __i = copy(begin(), __position, iterator(__q, 0));
        fill_n(__i, __n, __x);
        _M_finish = copy(__position, end(), __i + difference_type(__n));
        _M_deallocate();
        _M_end_of_storage = __q + (__len + _M_word_bit - 1)/_M_word_bit;
        _M_start = iterator(__q, 0);
      }
    }

    void insert(iterator __position, size_type __n, bool __x) {
      _M_fill_insert(__position, __n, __x);
    }

    void pop_back() { --_M_finish; }
    iterator erase(iterator __position) {
      if (__position + 1 != end())
        copy(__position + 1, end(), __position);
        --_M_finish;
      return __position;
    }
    iterator erase(iterator __first, iterator __last) {
      _M_finish = copy(__last, end(), __first);
      return __first;
    }
    void resize(size_type __new_size, bool __x = bool()) {
      if (__new_size < size())
        erase(begin() + difference_type(__new_size), end());
      else
        insert(end(), __new_size - size(), __x);
    }
    void flip() {
      for (_Bit_type * __p = _M_start._M_p; __p != _M_end_of_storage; ++__p)
        *__p = ~*__p;
    }

    void clear() { erase(begin(), end()); }
  };


typedef vector<bool, __alloc> bit_vector;

}
# 73 "/usr/include/gcc/darwin/3.3/c++/vector" 2 3


# 1 "/usr/include/gcc/darwin/3.3/c++/bits/vector.tcc" 1 3
# 64 "/usr/include/gcc/darwin/3.3/c++/bits/vector.tcc" 3
namespace std
{
  template<typename _Tp, typename _Alloc>
    void
    vector<_Tp,_Alloc>::
    reserve(size_type __n)
    {
      if (__n > this->max_size())
        __throw_length_error("vector::reserve");
      if (this->capacity() < __n)
        {
          const size_type __old_size = size();
          pointer __tmp = _M_allocate_and_copy(__n, _M_start, _M_finish);
          _Destroy(_M_start, _M_finish);
          _M_deallocate(_M_start, _M_end_of_storage - _M_start);
          _M_start = __tmp;
          _M_finish = __tmp + __old_size;
          _M_end_of_storage = _M_start + __n;
        }
    }

  template<typename _Tp, typename _Alloc>
    typename vector<_Tp,_Alloc>::iterator
    vector<_Tp,_Alloc>::
    insert(iterator __position, const value_type& __x)
    {
      size_type __n = __position - begin();
      if (_M_finish != _M_end_of_storage && __position == end())
      {
        _Construct(_M_finish, __x);
        ++_M_finish;
      }
      else
        _M_insert_aux(__position, __x);
      return begin() + __n;
    }

  template<typename _Tp, typename _Alloc>
    typename vector<_Tp,_Alloc>::iterator
    vector<_Tp,_Alloc>::
    erase(iterator __position)
    {
      if (__position + 1 != end())
        copy(__position + 1, end(), __position);
      --_M_finish;
      _Destroy(_M_finish);
      return __position;
    }

  template<typename _Tp, typename _Alloc>
    typename vector<_Tp,_Alloc>::iterator
    vector<_Tp,_Alloc>::
    erase(iterator __first, iterator __last)
    {
      iterator __i(copy(__last, end(), __first));
      _Destroy(__i, end());
      _M_finish = _M_finish - (__last - __first);
      return __first;
    }

  template<typename _Tp, typename _Alloc>
    vector<_Tp,_Alloc>&
    vector<_Tp,_Alloc>::
    operator=(const vector<_Tp,_Alloc>& __x)
    {
      if (&__x != this)
      {
        const size_type __xlen = __x.size();
        if (__xlen > capacity())
        {
          pointer __tmp = _M_allocate_and_copy(__xlen, __x.begin(), __x.end());
          _Destroy(_M_start, _M_finish);
          _M_deallocate(_M_start, _M_end_of_storage - _M_start);
          _M_start = __tmp;
          _M_end_of_storage = _M_start + __xlen;
        }
        else if (size() >= __xlen)
        {
          iterator __i(copy(__x.begin(), __x.end(), begin()));
          _Destroy(__i, end());
        }
        else
        {
          copy(__x.begin(), __x.begin() + size(), _M_start);
          uninitialized_copy(__x.begin() + size(), __x.end(), _M_finish);
        }
        _M_finish = _M_start + __xlen;
      }
      return *this;
    }

  template<typename _Tp, typename _Alloc>
    void
    vector<_Tp,_Alloc>::
    _M_fill_assign(size_t __n, const value_type& __val)
    {
      if (__n > capacity())
      {
        vector __tmp(__n, __val, get_allocator());
        __tmp.swap(*this);
      }
      else if (__n > size())
      {
        fill(begin(), end(), __val);
        _M_finish = uninitialized_fill_n(_M_finish, __n - size(), __val);
      }
      else
        erase(fill_n(begin(), __n, __val), end());
    }

  template<typename _Tp, typename _Alloc> template<typename _InputIter>
    void
    vector<_Tp,_Alloc>::
    _M_assign_aux(_InputIter __first, _InputIter __last, input_iterator_tag)
    {
      iterator __cur(begin());
      for ( ; __first != __last && __cur != end(); ++__cur, ++__first)
        *__cur = *__first;
      if (__first == __last)
        erase(__cur, end());
      else
        insert(end(), __first, __last);
    }

  template<typename _Tp, typename _Alloc> template<typename _ForwardIter>
    void
    vector<_Tp,_Alloc>::
    _M_assign_aux(_ForwardIter __first, _ForwardIter __last,
                  forward_iterator_tag)
    {
      size_type __len = distance(__first, __last);

      if (__len > capacity())
      {
        pointer __tmp(_M_allocate_and_copy(__len, __first, __last));
        _Destroy(_M_start, _M_finish);
        _M_deallocate(_M_start, _M_end_of_storage - _M_start);
        _M_start = __tmp;
        _M_end_of_storage = _M_finish = _M_start + __len;
      }
      else if (size() >= __len)
      {
        iterator __new_finish(copy(__first, __last, _M_start));
        _Destroy(__new_finish, end());
        _M_finish = __new_finish.base();
      }
      else
      {
        _ForwardIter __mid = __first;
        advance(__mid, size());
        copy(__first, __mid, _M_start);
        _M_finish = uninitialized_copy(__mid, __last, _M_finish);
      }
    }

  template<typename _Tp, typename _Alloc>
    void
    vector<_Tp,_Alloc>::
    _M_insert_aux(iterator __position, const _Tp& __x)
    {
      if (_M_finish != _M_end_of_storage)
      {
        _Construct(_M_finish, *(_M_finish - 1));
        ++_M_finish;
        _Tp __x_copy = __x;
        copy_backward(__position, iterator(_M_finish-2), iterator(_M_finish-1));
        *__position = __x_copy;
      }
      else
      {
        const size_type __old_size = size();
        const size_type __len = __old_size != 0 ? 2 * __old_size : 1;
        iterator __new_start(_M_allocate(__len));
        iterator __new_finish(__new_start);
        try
          {
            __new_finish = uninitialized_copy(iterator(_M_start), __position,
                                              __new_start);
            _Construct(__new_finish.base(), __x);
            ++__new_finish;
            __new_finish = uninitialized_copy(__position, iterator(_M_finish),
                                              __new_finish);
          }
        catch(...)
          {
            _Destroy(__new_start,__new_finish);
            _M_deallocate(__new_start.base(),__len);
            throw;
          }
        _Destroy(begin(), end());
        _M_deallocate(_M_start, _M_end_of_storage - _M_start);
        _M_start = __new_start.base();
        _M_finish = __new_finish.base();
        _M_end_of_storage = __new_start.base() + __len;
      }
    }
# 305 "/usr/include/gcc/darwin/3.3/c++/bits/vector.tcc" 3
  template<typename _Tp, typename _Alloc>
    void
    vector<_Tp,_Alloc>::
    _M_fill_insert(iterator __position, size_type __n, const value_type& __x)
    {
      if (__n != 0)
      {
        if (size_type(_M_end_of_storage - _M_finish) >= __n)
          {
           value_type __x_copy = __x;
           const size_type __elems_after = end() - __position;
           iterator __old_finish(_M_finish);
           if (__elems_after > __n)
             {
               uninitialized_copy(_M_finish - __n, _M_finish, _M_finish);
               _M_finish += __n;
               copy_backward(__position, __old_finish - __n, __old_finish);
               fill(__position, __position + __n, __x_copy);
             }
           else
             {
               uninitialized_fill_n(_M_finish, __n - __elems_after, __x_copy);
               _M_finish += __n - __elems_after;
               uninitialized_copy(__position, __old_finish, _M_finish);
               _M_finish += __elems_after;
               fill(__position, __old_finish, __x_copy);
             }
          }
        else
          {
            const size_type __old_size = size();
            const size_type __len = __old_size + max(__old_size, __n);
            iterator __new_start(_M_allocate(__len));
            iterator __new_finish(__new_start);
            try
              {
                __new_finish = uninitialized_copy(begin(), __position,
                                                  __new_start);
                __new_finish = uninitialized_fill_n(__new_finish, __n, __x);
                __new_finish = uninitialized_copy(__position, end(),
                                                  __new_finish);
              }
            catch(...)
              {
                _Destroy(__new_start,__new_finish);
                _M_deallocate(__new_start.base(),__len);
                throw;
              }
            _Destroy(_M_start, _M_finish);
            _M_deallocate(_M_start, _M_end_of_storage - _M_start);
            _M_start = __new_start.base();
            _M_finish = __new_finish.base();
            _M_end_of_storage = __new_start.base() + __len;
          }
      }
    }

  template<typename _Tp, typename _Alloc> template<typename _InputIterator>
    void
    vector<_Tp,_Alloc>::
    _M_range_insert(iterator __pos,
                    _InputIterator __first, _InputIterator __last,
                    input_iterator_tag)
    {
      for ( ; __first != __last; ++__first)
      {
        __pos = insert(__pos, *__first);
        ++__pos;
      }
    }

  template<typename _Tp, typename _Alloc> template<typename _ForwardIterator>
    void
    vector<_Tp,_Alloc>::
    _M_range_insert(iterator __position,_ForwardIterator __first,
                    _ForwardIterator __last, forward_iterator_tag)
    {
      if (__first != __last)
      {
        size_type __n = distance(__first, __last);
        if (size_type(_M_end_of_storage - _M_finish) >= __n)
        {
          const size_type __elems_after = end() - __position;
          iterator __old_finish(_M_finish);
          if (__elems_after > __n)
          {
            uninitialized_copy(_M_finish - __n, _M_finish, _M_finish);
            _M_finish += __n;
            copy_backward(__position, __old_finish - __n, __old_finish);
            copy(__first, __last, __position);
          }
          else
          {
            _ForwardIterator __mid = __first;
            advance(__mid, __elems_after);
            uninitialized_copy(__mid, __last, _M_finish);
            _M_finish += __n - __elems_after;
            uninitialized_copy(__position, __old_finish, _M_finish);
            _M_finish += __elems_after;
            copy(__first, __mid, __position);
          }
        }
        else
        {
          const size_type __old_size = size();
          const size_type __len = __old_size + max(__old_size, __n);
          iterator __new_start(_M_allocate(__len));
          iterator __new_finish(__new_start);
          try
            {
              __new_finish = uninitialized_copy(iterator(_M_start),
                                                __position, __new_start);
              __new_finish = uninitialized_copy(__first, __last, __new_finish);
              __new_finish = uninitialized_copy(__position, iterator(_M_finish),
                                                __new_finish);
            }
          catch(...)
            {
              _Destroy(__new_start,__new_finish);
              _M_deallocate(__new_start.base(), __len);
              throw;
            }
          _Destroy(_M_start, _M_finish);
          _M_deallocate(_M_start, _M_end_of_storage - _M_start);
          _M_start = __new_start.base();
          _M_finish = __new_finish.base();
          _M_end_of_storage = __new_start.base() + __len;
        }
      }
    }
}
# 76 "/usr/include/gcc/darwin/3.3/c++/vector" 2 3
# 12 "/Users/jmh/devel/libgist/include/gist_defs.h" 2
# 23 "/Users/jmh/devel/libgist/include/gist_defs.h"
enum latch_mode_t { LATCH_NL = 0, LATCH_SH = 1, LATCH_EX = 2 };

typedef short int2;
typedef long int4;
typedef unsigned char uint1;
typedef unsigned short uint2;
typedef unsigned long uint4;
typedef uint4 smsize_t;







typedef unsigned int shpid_t;
typedef int rc_t;
# 79 "/Users/jmh/devel/libgist/include/gist_defs.h"
using namespace std;
# 89 "/Users/jmh/devel/libgist/include/gist_defs.h"
typedef vector<void *> Vector;
# 9 "/Users/jmh/devel/libgist/include/amdb_ext.h" 2
# 1 "/Users/jmh/devel/libgist/include/gist_ext.h" 1
# 9 "/Users/jmh/devel/libgist/include/gist_ext.h"
class gist_p;
# 1 "/Users/jmh/devel/libgist/include/vec_t.h" 1
# 18 "/Users/jmh/devel/libgist/include/vec_t.h"
struct vec_pair_t {
    const unsigned char * ptr;
    size_t len;
};

struct VEC_t {
    int _cnt;
    size_t _size;
    vec_pair_t* _base;

    vec_pair_t _pair[8];
};

class cvec_t : protected VEC_t {
    friend class vec_t;
public:
    enum dummy_enumid { max_small = 8 };
    cvec_t() {
        _cnt = 0;
        _size = 0;
        _base = &_pair[0];
    }
    cvec_t(const cvec_t& v1, const cvec_t& v2) {
        _base= &_pair[0];
        set(v1, v2);
    }
    cvec_t(const void* p, size_t l) {
        _base = &_pair[0];
        set(p, l);
    }
    ~cvec_t();

    bool _is_large() const {return _base != &_pair[0];}
    cvec_t& put(const void* p, size_t l);
    cvec_t& put(const cvec_t& v);
    cvec_t& reset() {
        _cnt = _size = 0;
        return *this;
    }
    cvec_t& set(const cvec_t& v1, const cvec_t& v2) {
        return reset().put(v1).put(v2);
    }
    cvec_t& set(const void* p, size_t l) {
        return reset().put(p, l);
    }

    size_t size() const {
        return _size;
    }

    size_t copy_to(void* p, size_t limit = 0x7fffffff) const;

};

class vec_t : public cvec_t {
public:
    vec_t() : cvec_t() {};
    vec_t(const cvec_t& v1, const cvec_t& v2) : cvec_t(v1, v2) {};
    vec_t(const void* p, size_t l) : cvec_t(p, l) {};



    void* ptr(int index) const { return (index >= 0 && index < _cnt) ?
                                        (void*)_base[index].ptr : 0; }
    size_t len(int index) const { return (index >= 0 && index < _cnt) ?
                                        _base[index].len : 0; }


private:



};
# 11 "/Users/jmh/devel/libgist/include/gist_ext.h" 2
# 20 "/Users/jmh/devel/libgist/include/gist_ext.h"
class gist_query_t;
class gist_cursorext_t;





class gist_penalty_t {
public:
    static const double max_penalty;

    gist_penalty_t() {}
    gist_penalty_t(double p) : p(p) {}
    double p;

    bool operator<(const gist_penalty_t &pen) { return p < pen.p; }
    operator double() const { return p; }
    gist_penalty_t& operator=(const gist_penalty_t &pen) { p = pen.p; return *this; }
    gist_penalty_t& operator=(double pen) { p = pen; return *this; }
};
# 91 "/Users/jmh/devel/libgist/include/gist_ext.h"
class gist_ext_t {
public:


    enum gist_ext_ids {
        rt_point_ext_id,
        rt_rect_ext_id,
        bt_int_ext_id,
        bt_str_ext_id,
        rstar_point_ext_id,
        rstar_rect_ext_id,
        sp_point_ext_id,
        ss_point_ext_id,
        sr_point_ext_id,
        rr_point_ext_id,
        npt_ext_id,
    gist_numext
    };

    const gist_ext_ids myId;
    const char* myName;

    static gist_ext_t *gist_ext_list[gist_numext];






    virtual rc_t insert(
        gist_p& page,
        const vec_t& key,
        const vec_t& data,
        shpid_t child)
        = 0;



    virtual rc_t remove(
        gist_p& page,
        const int slots[],
        int numSlots)
        = 0;



    virtual rc_t updateKey(
        gist_p& page,
        int& slot,
        const vec_t& newKey)
        = 0;



    virtual void findMinPen(
        const gist_p& page,
        const vec_t& key,
        const vec_t& data,
        int& slot)
        = 0;



    virtual void search(
        gist_p& page,
        const gist_query_t* query,
        int matches[],
        int& numMatches)
        = 0;





    virtual void getKey(
        const gist_p& page,
        int slot,
        vec_t& key)
        = 0;
# 186 "/Users/jmh/devel/libgist/include/gist_ext.h"
    virtual rc_t pickSplit(
        gist_p& page,
        int rightEntries[],
        int& numRight,
        const vec_t& oldBp,
        vec_t& leftBp,
        vec_t& rightBp,
        const vec_t& entry1,
        bool& oneGoesRight,
        const vec_t& entry2,
        bool& twoGoesRight)
        = 0;
# 219 "/Users/jmh/devel/libgist/include/gist_ext.h"
    virtual void unionBp(
        const gist_p& page,
        vec_t& bp,
        bool bpIsValid,
        const vec_t& pred1,
        const vec_t& pred2,
        bool& bpChanged) = 0;




    virtual gist_cursorext_t* queryCursor(
        const gist_query_t* query) const = 0;




    virtual bool check(
        const vec_t& bp,
        const vec_t& pred,
        int level)
        = 0;







    typedef void (*PrintPredFct)(
        ostream& s,
        const vec_t& pred,
        int level);
    typedef void (*PrintDataFct)(
        ostream& s,
        const vec_t& data);
    PrintPredFct printPred;
    PrintDataFct printData;
# 266 "/Users/jmh/devel/libgist/include/gist_ext.h"
    typedef rc_t (*ParseFct)(const char* str, void* out, int& len);
    ParseFct parsePred;
    ParseFct parseData;

    typedef rc_t (*ParseQueryFct)(const char* str, gist_query_t*& query);
    ParseQueryFct parseQuery;

    gist_ext_t::gist_ext_t(
        gist_ext_ids id,
        const char *name,
        PrintPredFct printPred,
        PrintDataFct printData,
        ParseFct parsePred,
        ParseFct parseData,
        ParseQueryFct parseQuery) :
        myId(id), myName(name), printPred(printPred), printData(printData),
        parsePred(parsePred), parseData(parseData), parseQuery(parseQuery)
    {
        gist_ext_list[id] = this;
    }

};
# 10 "/Users/jmh/devel/libgist/include/amdb_ext.h" 2
# 1 "/Library/Java/Home/include/jni.h" 1
# 22 "/Library/Java/Home/include/jni.h"
# 1 "/usr/include/gcc/darwin/3.3/stdarg.h" 1 3 4
# 48 "/usr/include/gcc/darwin/3.3/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 110 "/usr/include/gcc/darwin/3.3/stdarg.h" 3 4
typedef __gnuc_va_list va_list;
# 23 "/Library/Java/Home/include/jni.h" 2




# 1 "/Library/Java/Home/include/jni_md.h" 1
# 15 "/Library/Java/Home/include/jni_md.h"
typedef long jint;
typedef long long jlong;
typedef signed char jbyte;
# 28 "/Library/Java/Home/include/jni.h" 2


extern "C" {
# 39 "/Library/Java/Home/include/jni.h"
typedef unsigned char jboolean;
typedef unsigned short jchar;
typedef short jshort;
typedef float jfloat;
typedef double jdouble;

typedef jint jsize;



class _jobject {};
class _jclass : public _jobject {};
class _jthrowable : public _jobject {};
class _jstring : public _jobject {};
class _jarray : public _jobject {};
class _jbooleanArray : public _jarray {};
class _jbyteArray : public _jarray {};
class _jcharArray : public _jarray {};
class _jshortArray : public _jarray {};
class _jintArray : public _jarray {};
class _jlongArray : public _jarray {};
class _jfloatArray : public _jarray {};
class _jdoubleArray : public _jarray {};
class _jobjectArray : public _jarray {};

typedef _jobject *jobject;
typedef _jclass *jclass;
typedef _jthrowable *jthrowable;
typedef _jstring *jstring;
typedef _jarray *jarray;
typedef _jbooleanArray *jbooleanArray;
typedef _jbyteArray *jbyteArray;
typedef _jcharArray *jcharArray;
typedef _jshortArray *jshortArray;
typedef _jintArray *jintArray;
typedef _jlongArray *jlongArray;
typedef _jfloatArray *jfloatArray;
typedef _jdoubleArray *jdoubleArray;
typedef _jobjectArray *jobjectArray;
# 100 "/Library/Java/Home/include/jni.h"
typedef jobject jweak;

typedef union jvalue {
    jboolean z;
    jbyte b;
    jchar c;
    jshort s;
    jint i;
    jlong j;
    jfloat f;
    jdouble d;
    jobject l;
} jvalue;

struct _jfieldID;
typedef struct _jfieldID *jfieldID;

struct _jmethodID;
typedef struct _jmethodID *jmethodID;
# 153 "/Library/Java/Home/include/jni.h"
typedef struct {
    char *name;
    char *signature;
    void *fnPtr;
} JNINativeMethod;





struct JNINativeInterface_;

struct JNIEnv_;


typedef JNIEnv_ JNIEnv;
# 177 "/Library/Java/Home/include/jni.h"
struct JNIInvokeInterface_;

struct JavaVM_;


typedef JavaVM_ JavaVM;




struct JNINativeInterface_ {
    void *reserved0;
    void *reserved1;
    void *reserved2;

    void *reserved3;


    void* cfm_vectors[225];


    jint ( *GetVersion)(JNIEnv *env);

    jclass ( *DefineClass)
      (JNIEnv *env, const char *name, jobject loader, const jbyte *buf,
       jsize len);
    jclass ( *FindClass)
      (JNIEnv *env, const char *name);

    jmethodID ( *FromReflectedMethod)
      (JNIEnv *env, jobject method);
    jfieldID ( *FromReflectedField)
      (JNIEnv *env, jobject field);

    jobject ( *ToReflectedMethod)
      (JNIEnv *env, jclass cls, jmethodID methodID, jboolean isStatic);

    jclass ( *GetSuperclass)
      (JNIEnv *env, jclass sub);
    jboolean ( *IsAssignableFrom)
      (JNIEnv *env, jclass sub, jclass sup);

    jobject ( *ToReflectedField)
      (JNIEnv *env, jclass cls, jfieldID fieldID, jboolean isStatic);

    jint ( *Throw)
      (JNIEnv *env, jthrowable obj);
    jint ( *ThrowNew)
      (JNIEnv *env, jclass clazz, const char *msg);
    jthrowable ( *ExceptionOccurred)
      (JNIEnv *env);
    void ( *ExceptionDescribe)
      (JNIEnv *env);
    void ( *ExceptionClear)
      (JNIEnv *env);
    void ( *FatalError)
      (JNIEnv *env, const char *msg);

    jint ( *PushLocalFrame)
      (JNIEnv *env, jint capacity);
    jobject ( *PopLocalFrame)
      (JNIEnv *env, jobject result);

    jobject ( *NewGlobalRef)
      (JNIEnv *env, jobject lobj);
    void ( *DeleteGlobalRef)
      (JNIEnv *env, jobject gref);
    void ( *DeleteLocalRef)
      (JNIEnv *env, jobject obj);
    jboolean ( *IsSameObject)
      (JNIEnv *env, jobject obj1, jobject obj2);
    jobject ( *NewLocalRef)
      (JNIEnv *env, jobject ref);
    jint ( *EnsureLocalCapacity)
      (JNIEnv *env, jint capacity);

    jobject ( *AllocObject)
      (JNIEnv *env, jclass clazz);
    jobject ( *NewObject)
      (JNIEnv *env, jclass clazz, jmethodID methodID, ...);
    jobject ( *NewObjectV)
      (JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
    jobject ( *NewObjectA)
      (JNIEnv *env, jclass clazz, jmethodID methodID, jvalue *args);

    jclass ( *GetObjectClass)
      (JNIEnv *env, jobject obj);
    jboolean ( *IsInstanceOf)
      (JNIEnv *env, jobject obj, jclass clazz);

    jmethodID ( *GetMethodID)
      (JNIEnv *env, jclass clazz, const char *name, const char *sig);

    jobject ( *CallObjectMethod)
      (JNIEnv *env, jobject obj, jmethodID methodID, ...);
    jobject ( *CallObjectMethodV)
      (JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
    jobject ( *CallObjectMethodA)
      (JNIEnv *env, jobject obj, jmethodID methodID, jvalue * args);

    jboolean ( *CallBooleanMethod)
      (JNIEnv *env, jobject obj, jmethodID methodID, ...);
    jboolean ( *CallBooleanMethodV)
      (JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
    jboolean ( *CallBooleanMethodA)
      (JNIEnv *env, jobject obj, jmethodID methodID, jvalue * args);

    jbyte ( *CallByteMethod)
      (JNIEnv *env, jobject obj, jmethodID methodID, ...);
    jbyte ( *CallByteMethodV)
      (JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
    jbyte ( *CallByteMethodA)
      (JNIEnv *env, jobject obj, jmethodID methodID, jvalue *args);

    jchar ( *CallCharMethod)
      (JNIEnv *env, jobject obj, jmethodID methodID, ...);
    jchar ( *CallCharMethodV)
      (JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
    jchar ( *CallCharMethodA)
      (JNIEnv *env, jobject obj, jmethodID methodID, jvalue *args);

    jshort ( *CallShortMethod)
      (JNIEnv *env, jobject obj, jmethodID methodID, ...);
    jshort ( *CallShortMethodV)
      (JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
    jshort ( *CallShortMethodA)
      (JNIEnv *env, jobject obj, jmethodID methodID, jvalue *args);

    jint ( *CallIntMethod)
      (JNIEnv *env, jobject obj, jmethodID methodID, ...);
    jint ( *CallIntMethodV)
      (JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
    jint ( *CallIntMethodA)
      (JNIEnv *env, jobject obj, jmethodID methodID, jvalue *args);

    jlong ( *CallLongMethod)
      (JNIEnv *env, jobject obj, jmethodID methodID, ...);
    jlong ( *CallLongMethodV)
      (JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
    jlong ( *CallLongMethodA)
      (JNIEnv *env, jobject obj, jmethodID methodID, jvalue *args);

    jfloat ( *CallFloatMethod)
      (JNIEnv *env, jobject obj, jmethodID methodID, ...);
    jfloat ( *CallFloatMethodV)
      (JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
    jfloat ( *CallFloatMethodA)
      (JNIEnv *env, jobject obj, jmethodID methodID, jvalue *args);

    jdouble ( *CallDoubleMethod)
      (JNIEnv *env, jobject obj, jmethodID methodID, ...);
    jdouble ( *CallDoubleMethodV)
      (JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
    jdouble ( *CallDoubleMethodA)
      (JNIEnv *env, jobject obj, jmethodID methodID, jvalue *args);

    void ( *CallVoidMethod)
      (JNIEnv *env, jobject obj, jmethodID methodID, ...);
    void ( *CallVoidMethodV)
      (JNIEnv *env, jobject obj, jmethodID methodID, va_list args);
    void ( *CallVoidMethodA)
      (JNIEnv *env, jobject obj, jmethodID methodID, jvalue * args);

    jobject ( *CallNonvirtualObjectMethod)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...);
    jobject ( *CallNonvirtualObjectMethodV)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       va_list args);
    jobject ( *CallNonvirtualObjectMethodA)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       jvalue * args);

    jboolean ( *CallNonvirtualBooleanMethod)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...);
    jboolean ( *CallNonvirtualBooleanMethodV)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       va_list args);
    jboolean ( *CallNonvirtualBooleanMethodA)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       jvalue * args);

    jbyte ( *CallNonvirtualByteMethod)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...);
    jbyte ( *CallNonvirtualByteMethodV)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       va_list args);
    jbyte ( *CallNonvirtualByteMethodA)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       jvalue *args);

    jchar ( *CallNonvirtualCharMethod)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...);
    jchar ( *CallNonvirtualCharMethodV)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       va_list args);
    jchar ( *CallNonvirtualCharMethodA)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       jvalue *args);

    jshort ( *CallNonvirtualShortMethod)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...);
    jshort ( *CallNonvirtualShortMethodV)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       va_list args);
    jshort ( *CallNonvirtualShortMethodA)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       jvalue *args);

    jint ( *CallNonvirtualIntMethod)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...);
    jint ( *CallNonvirtualIntMethodV)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       va_list args);
    jint ( *CallNonvirtualIntMethodA)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       jvalue *args);

    jlong ( *CallNonvirtualLongMethod)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...);
    jlong ( *CallNonvirtualLongMethodV)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       va_list args);
    jlong ( *CallNonvirtualLongMethodA)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       jvalue *args);

    jfloat ( *CallNonvirtualFloatMethod)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...);
    jfloat ( *CallNonvirtualFloatMethodV)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       va_list args);
    jfloat ( *CallNonvirtualFloatMethodA)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       jvalue *args);

    jdouble ( *CallNonvirtualDoubleMethod)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...);
    jdouble ( *CallNonvirtualDoubleMethodV)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       va_list args);
    jdouble ( *CallNonvirtualDoubleMethodA)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       jvalue *args);

    void ( *CallNonvirtualVoidMethod)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...);
    void ( *CallNonvirtualVoidMethodV)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       va_list args);
    void ( *CallNonvirtualVoidMethodA)
      (JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
       jvalue * args);

    jfieldID ( *GetFieldID)
      (JNIEnv *env, jclass clazz, const char *name, const char *sig);

    jobject ( *GetObjectField)
      (JNIEnv *env, jobject obj, jfieldID fieldID);
    jboolean ( *GetBooleanField)
      (JNIEnv *env, jobject obj, jfieldID fieldID);
    jbyte ( *GetByteField)
      (JNIEnv *env, jobject obj, jfieldID fieldID);
    jchar ( *GetCharField)
      (JNIEnv *env, jobject obj, jfieldID fieldID);
    jshort ( *GetShortField)
      (JNIEnv *env, jobject obj, jfieldID fieldID);
    jint ( *GetIntField)
      (JNIEnv *env, jobject obj, jfieldID fieldID);
    jlong ( *GetLongField)
      (JNIEnv *env, jobject obj, jfieldID fieldID);
    jfloat ( *GetFloatField)
      (JNIEnv *env, jobject obj, jfieldID fieldID);
    jdouble ( *GetDoubleField)
      (JNIEnv *env, jobject obj, jfieldID fieldID);

    void ( *SetObjectField)
      (JNIEnv *env, jobject obj, jfieldID fieldID, jobject val);
    void ( *SetBooleanField)
      (JNIEnv *env, jobject obj, jfieldID fieldID, jboolean val);
    void ( *SetByteField)
      (JNIEnv *env, jobject obj, jfieldID fieldID, jbyte val);
    void ( *SetCharField)
      (JNIEnv *env, jobject obj, jfieldID fieldID, jchar val);
    void ( *SetShortField)
      (JNIEnv *env, jobject obj, jfieldID fieldID, jshort val);
    void ( *SetIntField)
      (JNIEnv *env, jobject obj, jfieldID fieldID, jint val);
    void ( *SetLongField)
      (JNIEnv *env, jobject obj, jfieldID fieldID, jlong val);
    void ( *SetFloatField)
      (JNIEnv *env, jobject obj, jfieldID fieldID, jfloat val);
    void ( *SetDoubleField)
      (JNIEnv *env, jobject obj, jfieldID fieldID, jdouble val);

    jmethodID ( *GetStaticMethodID)
      (JNIEnv *env, jclass clazz, const char *name, const char *sig);

    jobject ( *CallStaticObjectMethod)
      (JNIEnv *env, jclass clazz, jmethodID methodID, ...);
    jobject ( *CallStaticObjectMethodV)
      (JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
    jobject ( *CallStaticObjectMethodA)
      (JNIEnv *env, jclass clazz, jmethodID methodID, jvalue *args);

    jboolean ( *CallStaticBooleanMethod)
      (JNIEnv *env, jclass clazz, jmethodID methodID, ...);
    jboolean ( *CallStaticBooleanMethodV)
      (JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
    jboolean ( *CallStaticBooleanMethodA)
      (JNIEnv *env, jclass clazz, jmethodID methodID, jvalue *args);

    jbyte ( *CallStaticByteMethod)
      (JNIEnv *env, jclass clazz, jmethodID methodID, ...);
    jbyte ( *CallStaticByteMethodV)
      (JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
    jbyte ( *CallStaticByteMethodA)
      (JNIEnv *env, jclass clazz, jmethodID methodID, jvalue *args);

    jchar ( *CallStaticCharMethod)
      (JNIEnv *env, jclass clazz, jmethodID methodID, ...);
    jchar ( *CallStaticCharMethodV)
      (JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
    jchar ( *CallStaticCharMethodA)
      (JNIEnv *env, jclass clazz, jmethodID methodID, jvalue *args);

    jshort ( *CallStaticShortMethod)
      (JNIEnv *env, jclass clazz, jmethodID methodID, ...);
    jshort ( *CallStaticShortMethodV)
      (JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
    jshort ( *CallStaticShortMethodA)
      (JNIEnv *env, jclass clazz, jmethodID methodID, jvalue *args);

    jint ( *CallStaticIntMethod)
      (JNIEnv *env, jclass clazz, jmethodID methodID, ...);
    jint ( *CallStaticIntMethodV)
      (JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
    jint ( *CallStaticIntMethodA)
      (JNIEnv *env, jclass clazz, jmethodID methodID, jvalue *args);

    jlong ( *CallStaticLongMethod)
      (JNIEnv *env, jclass clazz, jmethodID methodID, ...);
    jlong ( *CallStaticLongMethodV)
      (JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
    jlong ( *CallStaticLongMethodA)
      (JNIEnv *env, jclass clazz, jmethodID methodID, jvalue *args);

    jfloat ( *CallStaticFloatMethod)
      (JNIEnv *env, jclass clazz, jmethodID methodID, ...);
    jfloat ( *CallStaticFloatMethodV)
      (JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
    jfloat ( *CallStaticFloatMethodA)
      (JNIEnv *env, jclass clazz, jmethodID methodID, jvalue *args);

    jdouble ( *CallStaticDoubleMethod)
      (JNIEnv *env, jclass clazz, jmethodID methodID, ...);
    jdouble ( *CallStaticDoubleMethodV)
      (JNIEnv *env, jclass clazz, jmethodID methodID, va_list args);
    jdouble ( *CallStaticDoubleMethodA)
      (JNIEnv *env, jclass clazz, jmethodID methodID, jvalue *args);

    void ( *CallStaticVoidMethod)
      (JNIEnv *env, jclass cls, jmethodID methodID, ...);
    void ( *CallStaticVoidMethodV)
      (JNIEnv *env, jclass cls, jmethodID methodID, va_list args);
    void ( *CallStaticVoidMethodA)
      (JNIEnv *env, jclass cls, jmethodID methodID, jvalue * args);

    jfieldID ( *GetStaticFieldID)
      (JNIEnv *env, jclass clazz, const char *name, const char *sig);
    jobject ( *GetStaticObjectField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID);
    jboolean ( *GetStaticBooleanField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID);
    jbyte ( *GetStaticByteField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID);
    jchar ( *GetStaticCharField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID);
    jshort ( *GetStaticShortField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID);
    jint ( *GetStaticIntField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID);
    jlong ( *GetStaticLongField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID);
    jfloat ( *GetStaticFloatField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID);
    jdouble ( *GetStaticDoubleField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID);

    void ( *SetStaticObjectField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID, jobject value);
    void ( *SetStaticBooleanField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID, jboolean value);
    void ( *SetStaticByteField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID, jbyte value);
    void ( *SetStaticCharField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID, jchar value);
    void ( *SetStaticShortField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID, jshort value);
    void ( *SetStaticIntField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID, jint value);
    void ( *SetStaticLongField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID, jlong value);
    void ( *SetStaticFloatField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID, jfloat value);
    void ( *SetStaticDoubleField)
      (JNIEnv *env, jclass clazz, jfieldID fieldID, jdouble value);

    jstring ( *NewString)
      (JNIEnv *env, const jchar *unicode, jsize len);
    jsize ( *GetStringLength)
      (JNIEnv *env, jstring str);
    const jchar *( *GetStringChars)
      (JNIEnv *env, jstring str, jboolean *isCopy);
    void ( *ReleaseStringChars)
      (JNIEnv *env, jstring str, const jchar *chars);

    jstring ( *NewStringUTF)
      (JNIEnv *env, const char *utf);
    jsize ( *GetStringUTFLength)
      (JNIEnv *env, jstring str);
    const char* ( *GetStringUTFChars)
      (JNIEnv *env, jstring str, jboolean *isCopy);
    void ( *ReleaseStringUTFChars)
      (JNIEnv *env, jstring str, const char* chars);


    jsize ( *GetArrayLength)
      (JNIEnv *env, jarray array);

    jobjectArray ( *NewObjectArray)
      (JNIEnv *env, jsize len, jclass clazz, jobject init);
    jobject ( *GetObjectArrayElement)
      (JNIEnv *env, jobjectArray array, jsize index);
    void ( *SetObjectArrayElement)
      (JNIEnv *env, jobjectArray array, jsize index, jobject val);

    jbooleanArray ( *NewBooleanArray)
      (JNIEnv *env, jsize len);
    jbyteArray ( *NewByteArray)
      (JNIEnv *env, jsize len);
    jcharArray ( *NewCharArray)
      (JNIEnv *env, jsize len);
    jshortArray ( *NewShortArray)
      (JNIEnv *env, jsize len);
    jintArray ( *NewIntArray)
      (JNIEnv *env, jsize len);
    jlongArray ( *NewLongArray)
      (JNIEnv *env, jsize len);
    jfloatArray ( *NewFloatArray)
      (JNIEnv *env, jsize len);
    jdoubleArray ( *NewDoubleArray)
      (JNIEnv *env, jsize len);

    jboolean * ( *GetBooleanArrayElements)
      (JNIEnv *env, jbooleanArray array, jboolean *isCopy);
    jbyte * ( *GetByteArrayElements)
      (JNIEnv *env, jbyteArray array, jboolean *isCopy);
    jchar * ( *GetCharArrayElements)
      (JNIEnv *env, jcharArray array, jboolean *isCopy);
    jshort * ( *GetShortArrayElements)
      (JNIEnv *env, jshortArray array, jboolean *isCopy);
    jint * ( *GetIntArrayElements)
      (JNIEnv *env, jintArray array, jboolean *isCopy);
    jlong * ( *GetLongArrayElements)
      (JNIEnv *env, jlongArray array, jboolean *isCopy);
    jfloat * ( *GetFloatArrayElements)
      (JNIEnv *env, jfloatArray array, jboolean *isCopy);
    jdouble * ( *GetDoubleArrayElements)
      (JNIEnv *env, jdoubleArray array, jboolean *isCopy);

    void ( *ReleaseBooleanArrayElements)
      (JNIEnv *env, jbooleanArray array, jboolean *elems, jint mode);
    void ( *ReleaseByteArrayElements)
      (JNIEnv *env, jbyteArray array, jbyte *elems, jint mode);
    void ( *ReleaseCharArrayElements)
      (JNIEnv *env, jcharArray array, jchar *elems, jint mode);
    void ( *ReleaseShortArrayElements)
      (JNIEnv *env, jshortArray array, jshort *elems, jint mode);
    void ( *ReleaseIntArrayElements)
      (JNIEnv *env, jintArray array, jint *elems, jint mode);
    void ( *ReleaseLongArrayElements)
      (JNIEnv *env, jlongArray array, jlong *elems, jint mode);
    void ( *ReleaseFloatArrayElements)
      (JNIEnv *env, jfloatArray array, jfloat *elems, jint mode);
    void ( *ReleaseDoubleArrayElements)
      (JNIEnv *env, jdoubleArray array, jdouble *elems, jint mode);

    void ( *GetBooleanArrayRegion)
      (JNIEnv *env, jbooleanArray array, jsize start, jsize l, jboolean *buf);
    void ( *GetByteArrayRegion)
      (JNIEnv *env, jbyteArray array, jsize start, jsize len, jbyte *buf);
    void ( *GetCharArrayRegion)
      (JNIEnv *env, jcharArray array, jsize start, jsize len, jchar *buf);
    void ( *GetShortArrayRegion)
      (JNIEnv *env, jshortArray array, jsize start, jsize len, jshort *buf);
    void ( *GetIntArrayRegion)
      (JNIEnv *env, jintArray array, jsize start, jsize len, jint *buf);
    void ( *GetLongArrayRegion)
      (JNIEnv *env, jlongArray array, jsize start, jsize len, jlong *buf);
    void ( *GetFloatArrayRegion)
      (JNIEnv *env, jfloatArray array, jsize start, jsize len, jfloat *buf);
    void ( *GetDoubleArrayRegion)
      (JNIEnv *env, jdoubleArray array, jsize start, jsize len, jdouble *buf);

    void ( *SetBooleanArrayRegion)
      (JNIEnv *env, jbooleanArray array, jsize start, jsize l, jboolean *buf);
    void ( *SetByteArrayRegion)
      (JNIEnv *env, jbyteArray array, jsize start, jsize len, jbyte *buf);
    void ( *SetCharArrayRegion)
      (JNIEnv *env, jcharArray array, jsize start, jsize len, jchar *buf);
    void ( *SetShortArrayRegion)
      (JNIEnv *env, jshortArray array, jsize start, jsize len, jshort *buf);
    void ( *SetIntArrayRegion)
      (JNIEnv *env, jintArray array, jsize start, jsize len, jint *buf);
    void ( *SetLongArrayRegion)
      (JNIEnv *env, jlongArray array, jsize start, jsize len, jlong *buf);
    void ( *SetFloatArrayRegion)
      (JNIEnv *env, jfloatArray array, jsize start, jsize len, jfloat *buf);
    void ( *SetDoubleArrayRegion)
      (JNIEnv *env, jdoubleArray array, jsize start, jsize len, jdouble *buf);

    jint ( *RegisterNatives)
      (JNIEnv *env, jclass clazz, const JNINativeMethod *methods,
       jint nMethods);
    jint ( *UnregisterNatives)
      (JNIEnv *env, jclass clazz);

    jint ( *MonitorEnter)
      (JNIEnv *env, jobject obj);
    jint ( *MonitorExit)
      (JNIEnv *env, jobject obj);

    jint ( *GetJavaVM)
      (JNIEnv *env, JavaVM **vm);

    void ( *GetStringRegion)
      (JNIEnv *env, jstring str, jsize start, jsize len, jchar *buf);
    void ( *GetStringUTFRegion)
      (JNIEnv *env, jstring str, jsize start, jsize len, char *buf);

    void * ( *GetPrimitiveArrayCritical)
      (JNIEnv *env, jarray array, jboolean *isCopy);
    void ( *ReleasePrimitiveArrayCritical)
      (JNIEnv *env, jarray array, void *carray, jint mode);

    const jchar * ( *GetStringCritical)
      (JNIEnv *env, jstring string, jboolean *isCopy);
    void ( *ReleaseStringCritical)
      (JNIEnv *env, jstring string, const jchar *cstring);

    jweak ( *NewWeakGlobalRef)
       (JNIEnv *env, jobject obj);
    void ( *DeleteWeakGlobalRef)
       (JNIEnv *env, jweak ref);

    jboolean ( *ExceptionCheck)
       (JNIEnv *env);

    jobject ( *NewDirectByteBuffer)
       (JNIEnv* env, void* address, jlong capacity);
    void* ( *GetDirectBufferAddress)
       (JNIEnv* env, jobject buf);
    jlong ( *GetDirectBufferCapacity)
       (JNIEnv* env, jobject buf);





};
# 760 "/Library/Java/Home/include/jni.h"
struct JNIEnv_ {
    const struct JNINativeInterface_ *functions;


    jint GetVersion() {
        return functions->GetVersion(this);
    }
    jclass DefineClass(const char *name, jobject loader, const jbyte *buf,
                       jsize len) {
        return functions->DefineClass(this, name, loader, buf, len);
    }
    jclass FindClass(const char *name) {
        return functions->FindClass(this, name);
    }
    jmethodID FromReflectedMethod(jobject method) {
        return functions->FromReflectedMethod(this,method);
    }
    jfieldID FromReflectedField(jobject field) {
        return functions->FromReflectedField(this,field);
    }

    jobject ToReflectedMethod(jclass cls, jmethodID methodID, jboolean isStatic) {
        return functions->ToReflectedMethod(this, cls, methodID, isStatic);
    }

    jclass GetSuperclass(jclass sub) {
        return functions->GetSuperclass(this, sub);
    }
    jboolean IsAssignableFrom(jclass sub, jclass sup) {
        return functions->IsAssignableFrom(this, sub, sup);
    }

    jobject ToReflectedField(jclass cls, jfieldID fieldID, jboolean isStatic) {
        return functions->ToReflectedField(this,cls,fieldID,isStatic);
    }

    jint Throw(jthrowable obj) {
        return functions->Throw(this, obj);
    }
    jint ThrowNew(jclass clazz, const char *msg) {
        return functions->ThrowNew(this, clazz, msg);
    }
    jthrowable ExceptionOccurred() {
        return functions->ExceptionOccurred(this);
    }
    void ExceptionDescribe() {
        functions->ExceptionDescribe(this);
    }
    void ExceptionClear() {
        functions->ExceptionClear(this);
    }
    void FatalError(const char *msg) {
        functions->FatalError(this, msg);
    }

    jint PushLocalFrame(jint capacity) {
        return functions->PushLocalFrame(this,capacity);
    }
    jobject PopLocalFrame(jobject result) {
        return functions->PopLocalFrame(this,result);
    }

    jobject NewGlobalRef(jobject lobj) {
        return functions->NewGlobalRef(this,lobj);
    }
    void DeleteGlobalRef(jobject gref) {
        functions->DeleteGlobalRef(this,gref);
    }
    void DeleteLocalRef(jobject obj) {
        functions->DeleteLocalRef(this, obj);
    }

    jboolean IsSameObject(jobject obj1, jobject obj2) {
        return functions->IsSameObject(this,obj1,obj2);
    }

    jobject NewLocalRef(jobject ref) {
        return functions->NewLocalRef(this,ref);
    }
    jint EnsureLocalCapacity(jint capacity) {
        return functions->EnsureLocalCapacity(this,capacity);
    }

    jobject AllocObject(jclass clazz) {
        return functions->AllocObject(this,clazz);
    }
    jobject NewObject(jclass clazz, jmethodID methodID, ...) {
        va_list args;
        jobject result;
        __builtin_va_start(args,methodID);
        result = functions->NewObjectV(this,clazz,methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jobject NewObjectV(jclass clazz, jmethodID methodID,
                       va_list args) {
        return functions->NewObjectV(this,clazz,methodID,args);
    }
    jobject NewObjectA(jclass clazz, jmethodID methodID,
                       jvalue *args) {
        return functions->NewObjectA(this,clazz,methodID,args);
    }

    jclass GetObjectClass(jobject obj) {
        return functions->GetObjectClass(this,obj);
    }
    jboolean IsInstanceOf(jobject obj, jclass clazz) {
        return functions->IsInstanceOf(this,obj,clazz);
    }

    jmethodID GetMethodID(jclass clazz, const char *name,
                          const char *sig) {
        return functions->GetMethodID(this,clazz,name,sig);
    }

    jobject CallObjectMethod(jobject obj, jmethodID methodID, ...) {
        va_list args;
        jobject result;
        __builtin_va_start(args,methodID);
        result = functions->CallObjectMethodV(this,obj,methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jobject CallObjectMethodV(jobject obj, jmethodID methodID,
                        va_list args) {
        return functions->CallObjectMethodV(this,obj,methodID,args);
    }
    jobject CallObjectMethodA(jobject obj, jmethodID methodID,
                        jvalue * args) {
        return functions->CallObjectMethodA(this,obj,methodID,args);
    }

    jboolean CallBooleanMethod(jobject obj,
                               jmethodID methodID, ...) {
        va_list args;
        jboolean result;
        __builtin_va_start(args,methodID);
        result = functions->CallBooleanMethodV(this,obj,methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jboolean CallBooleanMethodV(jobject obj, jmethodID methodID,
                                va_list args) {
        return functions->CallBooleanMethodV(this,obj,methodID,args);
    }
    jboolean CallBooleanMethodA(jobject obj, jmethodID methodID,
                                jvalue * args) {
        return functions->CallBooleanMethodA(this,obj,methodID, args);
    }

    jbyte CallByteMethod(jobject obj, jmethodID methodID, ...) {
        va_list args;
        jbyte result;
        __builtin_va_start(args,methodID);
        result = functions->CallByteMethodV(this,obj,methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jbyte CallByteMethodV(jobject obj, jmethodID methodID,
                          va_list args) {
        return functions->CallByteMethodV(this,obj,methodID,args);
    }
    jbyte CallByteMethodA(jobject obj, jmethodID methodID,
                          jvalue * args) {
        return functions->CallByteMethodA(this,obj,methodID,args);
    }

    jchar CallCharMethod(jobject obj, jmethodID methodID, ...) {
        va_list args;
        jchar result;
        __builtin_va_start(args,methodID);
        result = functions->CallCharMethodV(this,obj,methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jchar CallCharMethodV(jobject obj, jmethodID methodID,
                          va_list args) {
        return functions->CallCharMethodV(this,obj,methodID,args);
    }
    jchar CallCharMethodA(jobject obj, jmethodID methodID,
                          jvalue * args) {
        return functions->CallCharMethodA(this,obj,methodID,args);
    }

    jshort CallShortMethod(jobject obj, jmethodID methodID, ...) {
        va_list args;
        jshort result;
        __builtin_va_start(args,methodID);
        result = functions->CallShortMethodV(this,obj,methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jshort CallShortMethodV(jobject obj, jmethodID methodID,
                            va_list args) {
        return functions->CallShortMethodV(this,obj,methodID,args);
    }
    jshort CallShortMethodA(jobject obj, jmethodID methodID,
                            jvalue * args) {
        return functions->CallShortMethodA(this,obj,methodID,args);
    }

    jint CallIntMethod(jobject obj, jmethodID methodID, ...) {
        va_list args;
        jint result;
        __builtin_va_start(args,methodID);
        result = functions->CallIntMethodV(this,obj,methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jint CallIntMethodV(jobject obj, jmethodID methodID,
                        va_list args) {
        return functions->CallIntMethodV(this,obj,methodID,args);
    }
    jint CallIntMethodA(jobject obj, jmethodID methodID,
                        jvalue * args) {
        return functions->CallIntMethodA(this,obj,methodID,args);
    }

    jlong CallLongMethod(jobject obj, jmethodID methodID, ...) {
        va_list args;
        jlong result;
        __builtin_va_start(args,methodID);
        result = functions->CallLongMethodV(this,obj,methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jlong CallLongMethodV(jobject obj, jmethodID methodID,
                          va_list args) {
        return functions->CallLongMethodV(this,obj,methodID,args);
    }
    jlong CallLongMethodA(jobject obj, jmethodID methodID,
                          jvalue * args) {
        return functions->CallLongMethodA(this,obj,methodID,args);
    }

    jfloat CallFloatMethod(jobject obj, jmethodID methodID, ...) {
        va_list args;
        jfloat result;
        __builtin_va_start(args,methodID);
        result = functions->CallFloatMethodV(this,obj,methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jfloat CallFloatMethodV(jobject obj, jmethodID methodID,
                            va_list args) {
        return functions->CallFloatMethodV(this,obj,methodID,args);
    }
    jfloat CallFloatMethodA(jobject obj, jmethodID methodID,
                            jvalue * args) {
        return functions->CallFloatMethodA(this,obj,methodID,args);
    }

    jdouble CallDoubleMethod(jobject obj, jmethodID methodID, ...) {
        va_list args;
        jdouble result;
        __builtin_va_start(args,methodID);
        result = functions->CallDoubleMethodV(this,obj,methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jdouble CallDoubleMethodV(jobject obj, jmethodID methodID,
                        va_list args) {
        return functions->CallDoubleMethodV(this,obj,methodID,args);
    }
    jdouble CallDoubleMethodA(jobject obj, jmethodID methodID,
                        jvalue * args) {
        return functions->CallDoubleMethodA(this,obj,methodID,args);
    }

    void CallVoidMethod(jobject obj, jmethodID methodID, ...) {
        va_list args;
        __builtin_va_start(args,methodID);
        functions->CallVoidMethodV(this,obj,methodID,args);
        __builtin_va_end(args);
    }
    void CallVoidMethodV(jobject obj, jmethodID methodID,
                         va_list args) {
        functions->CallVoidMethodV(this,obj,methodID,args);
    }
    void CallVoidMethodA(jobject obj, jmethodID methodID,
                         jvalue * args) {
        functions->CallVoidMethodA(this,obj,methodID,args);
    }

    jobject CallNonvirtualObjectMethod(jobject obj, jclass clazz,
                                       jmethodID methodID, ...) {
        va_list args;
        jobject result;
        __builtin_va_start(args,methodID);
        result = functions->CallNonvirtualObjectMethodV(this,obj,clazz,
                                                        methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jobject CallNonvirtualObjectMethodV(jobject obj, jclass clazz,
                                        jmethodID methodID, va_list args) {
        return functions->CallNonvirtualObjectMethodV(this,obj,clazz,
                                                      methodID,args);
    }
    jobject CallNonvirtualObjectMethodA(jobject obj, jclass clazz,
                                        jmethodID methodID, jvalue * args) {
        return functions->CallNonvirtualObjectMethodA(this,obj,clazz,
                                                      methodID,args);
    }

    jboolean CallNonvirtualBooleanMethod(jobject obj, jclass clazz,
                                         jmethodID methodID, ...) {
        va_list args;
        jboolean result;
        __builtin_va_start(args,methodID);
        result = functions->CallNonvirtualBooleanMethodV(this,obj,clazz,
                                                         methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jboolean CallNonvirtualBooleanMethodV(jobject obj, jclass clazz,
                                          jmethodID methodID, va_list args) {
        return functions->CallNonvirtualBooleanMethodV(this,obj,clazz,
                                                       methodID,args);
    }
    jboolean CallNonvirtualBooleanMethodA(jobject obj, jclass clazz,
                                          jmethodID methodID, jvalue * args) {
        return functions->CallNonvirtualBooleanMethodA(this,obj,clazz,
                                                       methodID, args);
    }

    jbyte CallNonvirtualByteMethod(jobject obj, jclass clazz,
                                   jmethodID methodID, ...) {
        va_list args;
        jbyte result;
        __builtin_va_start(args,methodID);
        result = functions->CallNonvirtualByteMethodV(this,obj,clazz,
                                                      methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jbyte CallNonvirtualByteMethodV(jobject obj, jclass clazz,
                                    jmethodID methodID, va_list args) {
        return functions->CallNonvirtualByteMethodV(this,obj,clazz,
                                                    methodID,args);
    }
    jbyte CallNonvirtualByteMethodA(jobject obj, jclass clazz,
                                    jmethodID methodID, jvalue * args) {
        return functions->CallNonvirtualByteMethodA(this,obj,clazz,
                                                    methodID,args);
    }

    jchar CallNonvirtualCharMethod(jobject obj, jclass clazz,
                                   jmethodID methodID, ...) {
        va_list args;
        jchar result;
        __builtin_va_start(args,methodID);
        result = functions->CallNonvirtualCharMethodV(this,obj,clazz,
                                                      methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jchar CallNonvirtualCharMethodV(jobject obj, jclass clazz,
                                    jmethodID methodID, va_list args) {
        return functions->CallNonvirtualCharMethodV(this,obj,clazz,
                                                    methodID,args);
    }
    jchar CallNonvirtualCharMethodA(jobject obj, jclass clazz,
                                    jmethodID methodID, jvalue * args) {
        return functions->CallNonvirtualCharMethodA(this,obj,clazz,
                                                    methodID,args);
    }

    jshort CallNonvirtualShortMethod(jobject obj, jclass clazz,
                                     jmethodID methodID, ...) {
        va_list args;
        jshort result;
        __builtin_va_start(args,methodID);
        result = functions->CallNonvirtualShortMethodV(this,obj,clazz,
                                                       methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jshort CallNonvirtualShortMethodV(jobject obj, jclass clazz,
                                      jmethodID methodID, va_list args) {
        return functions->CallNonvirtualShortMethodV(this,obj,clazz,
                                                     methodID,args);
    }
    jshort CallNonvirtualShortMethodA(jobject obj, jclass clazz,
                                      jmethodID methodID, jvalue * args) {
        return functions->CallNonvirtualShortMethodA(this,obj,clazz,
                                                     methodID,args);
    }

    jint CallNonvirtualIntMethod(jobject obj, jclass clazz,
                                 jmethodID methodID, ...) {
        va_list args;
        jint result;
        __builtin_va_start(args,methodID);
        result = functions->CallNonvirtualIntMethodV(this,obj,clazz,
                                                     methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jint CallNonvirtualIntMethodV(jobject obj, jclass clazz,
                                  jmethodID methodID, va_list args) {
        return functions->CallNonvirtualIntMethodV(this,obj,clazz,
                                                   methodID,args);
    }
    jint CallNonvirtualIntMethodA(jobject obj, jclass clazz,
                                  jmethodID methodID, jvalue * args) {
        return functions->CallNonvirtualIntMethodA(this,obj,clazz,
                                                   methodID,args);
    }

    jlong CallNonvirtualLongMethod(jobject obj, jclass clazz,
                                   jmethodID methodID, ...) {
        va_list args;
        jlong result;
        __builtin_va_start(args,methodID);
        result = functions->CallNonvirtualLongMethodV(this,obj,clazz,
                                                      methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jlong CallNonvirtualLongMethodV(jobject obj, jclass clazz,
                                    jmethodID methodID, va_list args) {
        return functions->CallNonvirtualLongMethodV(this,obj,clazz,
                                                    methodID,args);
    }
    jlong CallNonvirtualLongMethodA(jobject obj, jclass clazz,
                                    jmethodID methodID, jvalue * args) {
        return functions->CallNonvirtualLongMethodA(this,obj,clazz,
                                                    methodID,args);
    }

    jfloat CallNonvirtualFloatMethod(jobject obj, jclass clazz,
                                     jmethodID methodID, ...) {
        va_list args;
        jfloat result;
        __builtin_va_start(args,methodID);
        result = functions->CallNonvirtualFloatMethodV(this,obj,clazz,
                                                       methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jfloat CallNonvirtualFloatMethodV(jobject obj, jclass clazz,
                                      jmethodID methodID,
                                      va_list args) {
        return functions->CallNonvirtualFloatMethodV(this,obj,clazz,
                                                     methodID,args);
    }
    jfloat CallNonvirtualFloatMethodA(jobject obj, jclass clazz,
                                      jmethodID methodID,
                                      jvalue * args) {
        return functions->CallNonvirtualFloatMethodA(this,obj,clazz,
                                                     methodID,args);
    }

    jdouble CallNonvirtualDoubleMethod(jobject obj, jclass clazz,
                                       jmethodID methodID, ...) {
        va_list args;
        jdouble result;
        __builtin_va_start(args,methodID);
        result = functions->CallNonvirtualDoubleMethodV(this,obj,clazz,
                                                        methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jdouble CallNonvirtualDoubleMethodV(jobject obj, jclass clazz,
                                        jmethodID methodID,
                                        va_list args) {
        return functions->CallNonvirtualDoubleMethodV(this,obj,clazz,
                                                      methodID,args);
    }
    jdouble CallNonvirtualDoubleMethodA(jobject obj, jclass clazz,
                                        jmethodID methodID,
                                        jvalue * args) {
        return functions->CallNonvirtualDoubleMethodA(this,obj,clazz,
                                                      methodID,args);
    }

    void CallNonvirtualVoidMethod(jobject obj, jclass clazz,
                                  jmethodID methodID, ...) {
        va_list args;
        __builtin_va_start(args,methodID);
        functions->CallNonvirtualVoidMethodV(this,obj,clazz,methodID,args);
        __builtin_va_end(args);
    }
    void CallNonvirtualVoidMethodV(jobject obj, jclass clazz,
                                   jmethodID methodID,
                                   va_list args) {
        functions->CallNonvirtualVoidMethodV(this,obj,clazz,methodID,args);
    }
    void CallNonvirtualVoidMethodA(jobject obj, jclass clazz,
                                   jmethodID methodID,
                                   jvalue * args) {
        functions->CallNonvirtualVoidMethodA(this,obj,clazz,methodID,args);
    }

    jfieldID GetFieldID(jclass clazz, const char *name,
                        const char *sig) {
        return functions->GetFieldID(this,clazz,name,sig);
    }

    jobject GetObjectField(jobject obj, jfieldID fieldID) {
        return functions->GetObjectField(this,obj,fieldID);
    }
    jboolean GetBooleanField(jobject obj, jfieldID fieldID) {
        return functions->GetBooleanField(this,obj,fieldID);
    }
    jbyte GetByteField(jobject obj, jfieldID fieldID) {
        return functions->GetByteField(this,obj,fieldID);
    }
    jchar GetCharField(jobject obj, jfieldID fieldID) {
        return functions->GetCharField(this,obj,fieldID);
    }
    jshort GetShortField(jobject obj, jfieldID fieldID) {
        return functions->GetShortField(this,obj,fieldID);
    }
    jint GetIntField(jobject obj, jfieldID fieldID) {
        return functions->GetIntField(this,obj,fieldID);
    }
    jlong GetLongField(jobject obj, jfieldID fieldID) {
        return functions->GetLongField(this,obj,fieldID);
    }
    jfloat GetFloatField(jobject obj, jfieldID fieldID) {
        return functions->GetFloatField(this,obj,fieldID);
    }
    jdouble GetDoubleField(jobject obj, jfieldID fieldID) {
        return functions->GetDoubleField(this,obj,fieldID);
    }

    void SetObjectField(jobject obj, jfieldID fieldID, jobject val) {
        functions->SetObjectField(this,obj,fieldID,val);
    }
    void SetBooleanField(jobject obj, jfieldID fieldID,
                         jboolean val) {
        functions->SetBooleanField(this,obj,fieldID,val);
    }
    void SetByteField(jobject obj, jfieldID fieldID,
                      jbyte val) {
        functions->SetByteField(this,obj,fieldID,val);
    }
    void SetCharField(jobject obj, jfieldID fieldID,
                      jchar val) {
        functions->SetCharField(this,obj,fieldID,val);
    }
    void SetShortField(jobject obj, jfieldID fieldID,
                       jshort val) {
        functions->SetShortField(this,obj,fieldID,val);
    }
    void SetIntField(jobject obj, jfieldID fieldID,
                     jint val) {
        functions->SetIntField(this,obj,fieldID,val);
    }
    void SetLongField(jobject obj, jfieldID fieldID,
                      jlong val) {
        functions->SetLongField(this,obj,fieldID,val);
    }
    void SetFloatField(jobject obj, jfieldID fieldID,
                       jfloat val) {
        functions->SetFloatField(this,obj,fieldID,val);
    }
    void SetDoubleField(jobject obj, jfieldID fieldID,
                        jdouble val) {
        functions->SetDoubleField(this,obj,fieldID,val);
    }

    jmethodID GetStaticMethodID(jclass clazz, const char *name,
                                const char *sig) {
        return functions->GetStaticMethodID(this,clazz,name,sig);
    }

    jobject CallStaticObjectMethod(jclass clazz, jmethodID methodID,
                             ...) {
        va_list args;
        jobject result;
        __builtin_va_start(args,methodID);
        result = functions->CallStaticObjectMethodV(this,clazz,methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jobject CallStaticObjectMethodV(jclass clazz, jmethodID methodID,
                              va_list args) {
        return functions->CallStaticObjectMethodV(this,clazz,methodID,args);
    }
    jobject CallStaticObjectMethodA(jclass clazz, jmethodID methodID,
                              jvalue *args) {
        return functions->CallStaticObjectMethodA(this,clazz,methodID,args);
    }

    jboolean CallStaticBooleanMethod(jclass clazz,
                                     jmethodID methodID, ...) {
        va_list args;
        jboolean result;
        __builtin_va_start(args,methodID);
        result = functions->CallStaticBooleanMethodV(this,clazz,methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jboolean CallStaticBooleanMethodV(jclass clazz,
                                      jmethodID methodID, va_list args) {
        return functions->CallStaticBooleanMethodV(this,clazz,methodID,args);
    }
    jboolean CallStaticBooleanMethodA(jclass clazz,
                                      jmethodID methodID, jvalue *args) {
        return functions->CallStaticBooleanMethodA(this,clazz,methodID,args);
    }

    jbyte CallStaticByteMethod(jclass clazz,
                               jmethodID methodID, ...) {
        va_list args;
        jbyte result;
        __builtin_va_start(args,methodID);
        result = functions->CallStaticByteMethodV(this,clazz,methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jbyte CallStaticByteMethodV(jclass clazz,
                                jmethodID methodID, va_list args) {
        return functions->CallStaticByteMethodV(this,clazz,methodID,args);
    }
    jbyte CallStaticByteMethodA(jclass clazz,
                                jmethodID methodID, jvalue *args) {
        return functions->CallStaticByteMethodA(this,clazz,methodID,args);
    }

    jchar CallStaticCharMethod(jclass clazz,
                               jmethodID methodID, ...) {
        va_list args;
        jchar result;
        __builtin_va_start(args,methodID);
        result = functions->CallStaticCharMethodV(this,clazz,methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jchar CallStaticCharMethodV(jclass clazz,
                                jmethodID methodID, va_list args) {
        return functions->CallStaticCharMethodV(this,clazz,methodID,args);
    }
    jchar CallStaticCharMethodA(jclass clazz,
                                jmethodID methodID, jvalue *args) {
        return functions->CallStaticCharMethodA(this,clazz,methodID,args);
    }

    jshort CallStaticShortMethod(jclass clazz,
                                 jmethodID methodID, ...) {
        va_list args;
        jshort result;
        __builtin_va_start(args,methodID);
        result = functions->CallStaticShortMethodV(this,clazz,methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jshort CallStaticShortMethodV(jclass clazz,
                                  jmethodID methodID, va_list args) {
        return functions->CallStaticShortMethodV(this,clazz,methodID,args);
    }
    jshort CallStaticShortMethodA(jclass clazz,
                                  jmethodID methodID, jvalue *args) {
        return functions->CallStaticShortMethodA(this,clazz,methodID,args);
    }

    jint CallStaticIntMethod(jclass clazz,
                             jmethodID methodID, ...) {
        va_list args;
        jint result;
        __builtin_va_start(args,methodID);
        result = functions->CallStaticIntMethodV(this,clazz,methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jint CallStaticIntMethodV(jclass clazz,
                              jmethodID methodID, va_list args) {
        return functions->CallStaticIntMethodV(this,clazz,methodID,args);
    }
    jint CallStaticIntMethodA(jclass clazz,
                              jmethodID methodID, jvalue *args) {
        return functions->CallStaticIntMethodA(this,clazz,methodID,args);
    }

    jlong CallStaticLongMethod(jclass clazz,
                               jmethodID methodID, ...) {
        va_list args;
        jlong result;
        __builtin_va_start(args,methodID);
        result = functions->CallStaticLongMethodV(this,clazz,methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jlong CallStaticLongMethodV(jclass clazz,
                                jmethodID methodID, va_list args) {
        return functions->CallStaticLongMethodV(this,clazz,methodID,args);
    }
    jlong CallStaticLongMethodA(jclass clazz,
                                jmethodID methodID, jvalue *args) {
        return functions->CallStaticLongMethodA(this,clazz,methodID,args);
    }

    jfloat CallStaticFloatMethod(jclass clazz,
                                 jmethodID methodID, ...) {
        va_list args;
        jfloat result;
        __builtin_va_start(args,methodID);
        result = functions->CallStaticFloatMethodV(this,clazz,methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jfloat CallStaticFloatMethodV(jclass clazz,
                                  jmethodID methodID, va_list args) {
        return functions->CallStaticFloatMethodV(this,clazz,methodID,args);
    }
    jfloat CallStaticFloatMethodA(jclass clazz,
                                  jmethodID methodID, jvalue *args) {
        return functions->CallStaticFloatMethodA(this,clazz,methodID,args);
    }

    jdouble CallStaticDoubleMethod(jclass clazz,
                                   jmethodID methodID, ...) {
        va_list args;
        jdouble result;
        __builtin_va_start(args,methodID);
        result = functions->CallStaticDoubleMethodV(this,clazz,methodID,args);
        __builtin_va_end(args);
        return result;
    }
    jdouble CallStaticDoubleMethodV(jclass clazz,
                                    jmethodID methodID, va_list args) {
        return functions->CallStaticDoubleMethodV(this,clazz,methodID,args);
    }
    jdouble CallStaticDoubleMethodA(jclass clazz,
                                    jmethodID methodID, jvalue *args) {
        return functions->CallStaticDoubleMethodA(this,clazz,methodID,args);
    }

    void CallStaticVoidMethod(jclass cls, jmethodID methodID, ...) {
        va_list args;
        __builtin_va_start(args,methodID);
        functions->CallStaticVoidMethodV(this,cls,methodID,args);
        __builtin_va_end(args);
    }
    void CallStaticVoidMethodV(jclass cls, jmethodID methodID,
                               va_list args) {
        functions->CallStaticVoidMethodV(this,cls,methodID,args);
    }
    void CallStaticVoidMethodA(jclass cls, jmethodID methodID,
                               jvalue * args) {
        functions->CallStaticVoidMethodA(this,cls,methodID,args);
    }

    jfieldID GetStaticFieldID(jclass clazz, const char *name,
                              const char *sig) {
        return functions->GetStaticFieldID(this,clazz,name,sig);
    }
    jobject GetStaticObjectField(jclass clazz, jfieldID fieldID) {
        return functions->GetStaticObjectField(this,clazz,fieldID);
    }
    jboolean GetStaticBooleanField(jclass clazz, jfieldID fieldID) {
        return functions->GetStaticBooleanField(this,clazz,fieldID);
    }
    jbyte GetStaticByteField(jclass clazz, jfieldID fieldID) {
        return functions->GetStaticByteField(this,clazz,fieldID);
    }
    jchar GetStaticCharField(jclass clazz, jfieldID fieldID) {
        return functions->GetStaticCharField(this,clazz,fieldID);
    }
    jshort GetStaticShortField(jclass clazz, jfieldID fieldID) {
        return functions->GetStaticShortField(this,clazz,fieldID);
    }
    jint GetStaticIntField(jclass clazz, jfieldID fieldID) {
        return functions->GetStaticIntField(this,clazz,fieldID);
    }
    jlong GetStaticLongField(jclass clazz, jfieldID fieldID) {
        return functions->GetStaticLongField(this,clazz,fieldID);
    }
    jfloat GetStaticFloatField(jclass clazz, jfieldID fieldID) {
        return functions->GetStaticFloatField(this,clazz,fieldID);
    }
    jdouble GetStaticDoubleField(jclass clazz, jfieldID fieldID) {
        return functions->GetStaticDoubleField(this,clazz,fieldID);
    }

    void SetStaticObjectField(jclass clazz, jfieldID fieldID,
                        jobject value) {
      functions->SetStaticObjectField(this,clazz,fieldID,value);
    }
    void SetStaticBooleanField(jclass clazz, jfieldID fieldID,
                        jboolean value) {
      functions->SetStaticBooleanField(this,clazz,fieldID,value);
    }
    void SetStaticByteField(jclass clazz, jfieldID fieldID,
                        jbyte value) {
      functions->SetStaticByteField(this,clazz,fieldID,value);
    }
    void SetStaticCharField(jclass clazz, jfieldID fieldID,
                        jchar value) {
      functions->SetStaticCharField(this,clazz,fieldID,value);
    }
    void SetStaticShortField(jclass clazz, jfieldID fieldID,
                        jshort value) {
      functions->SetStaticShortField(this,clazz,fieldID,value);
    }
    void SetStaticIntField(jclass clazz, jfieldID fieldID,
                        jint value) {
      functions->SetStaticIntField(this,clazz,fieldID,value);
    }
    void SetStaticLongField(jclass clazz, jfieldID fieldID,
                        jlong value) {
      functions->SetStaticLongField(this,clazz,fieldID,value);
    }
    void SetStaticFloatField(jclass clazz, jfieldID fieldID,
                        jfloat value) {
      functions->SetStaticFloatField(this,clazz,fieldID,value);
    }
    void SetStaticDoubleField(jclass clazz, jfieldID fieldID,
                        jdouble value) {
      functions->SetStaticDoubleField(this,clazz,fieldID,value);
    }

    jstring NewString(const jchar *unicode, jsize len) {
        return functions->NewString(this,unicode,len);
    }
    jsize GetStringLength(jstring str) {
        return functions->GetStringLength(this,str);
    }
    const jchar *GetStringChars(jstring str, jboolean *isCopy) {
        return functions->GetStringChars(this,str,isCopy);
    }
    void ReleaseStringChars(jstring str, const jchar *chars) {
        functions->ReleaseStringChars(this,str,chars);
    }

    jstring NewStringUTF(const char *utf) {
        return functions->NewStringUTF(this,utf);
    }
    jsize GetStringUTFLength(jstring str) {
        return functions->GetStringUTFLength(this,str);
    }
    const char* GetStringUTFChars(jstring str, jboolean *isCopy) {
        return functions->GetStringUTFChars(this,str,isCopy);
    }
    void ReleaseStringUTFChars(jstring str, const char* chars) {
        functions->ReleaseStringUTFChars(this,str,chars);
    }

    jsize GetArrayLength(jarray array) {
        return functions->GetArrayLength(this,array);
    }

    jobjectArray NewObjectArray(jsize len, jclass clazz,
                                jobject init) {
        return functions->NewObjectArray(this,len,clazz,init);
    }
    jobject GetObjectArrayElement(jobjectArray array, jsize index) {
        return functions->GetObjectArrayElement(this,array,index);
    }
    void SetObjectArrayElement(jobjectArray array, jsize index,
                               jobject val) {
        functions->SetObjectArrayElement(this,array,index,val);
    }

    jbooleanArray NewBooleanArray(jsize len) {
        return functions->NewBooleanArray(this,len);
    }
    jbyteArray NewByteArray(jsize len) {
        return functions->NewByteArray(this,len);
    }
    jcharArray NewCharArray(jsize len) {
        return functions->NewCharArray(this,len);
    }
    jshortArray NewShortArray(jsize len) {
        return functions->NewShortArray(this,len);
    }
    jintArray NewIntArray(jsize len) {
        return functions->NewIntArray(this,len);
    }
    jlongArray NewLongArray(jsize len) {
        return functions->NewLongArray(this,len);
    }
    jfloatArray NewFloatArray(jsize len) {
        return functions->NewFloatArray(this,len);
    }
    jdoubleArray NewDoubleArray(jsize len) {
        return functions->NewDoubleArray(this,len);
    }

    jboolean * GetBooleanArrayElements(jbooleanArray array, jboolean *isCopy) {
        return functions->GetBooleanArrayElements(this,array,isCopy);
    }
    jbyte * GetByteArrayElements(jbyteArray array, jboolean *isCopy) {
        return functions->GetByteArrayElements(this,array,isCopy);
    }
    jchar * GetCharArrayElements(jcharArray array, jboolean *isCopy) {
        return functions->GetCharArrayElements(this,array,isCopy);
    }
    jshort * GetShortArrayElements(jshortArray array, jboolean *isCopy) {
        return functions->GetShortArrayElements(this,array,isCopy);
    }
    jint * GetIntArrayElements(jintArray array, jboolean *isCopy) {
        return functions->GetIntArrayElements(this,array,isCopy);
    }
    jlong * GetLongArrayElements(jlongArray array, jboolean *isCopy) {
        return functions->GetLongArrayElements(this,array,isCopy);
    }
    jfloat * GetFloatArrayElements(jfloatArray array, jboolean *isCopy) {
        return functions->GetFloatArrayElements(this,array,isCopy);
    }
    jdouble * GetDoubleArrayElements(jdoubleArray array, jboolean *isCopy) {
        return functions->GetDoubleArrayElements(this,array,isCopy);
    }

    void ReleaseBooleanArrayElements(jbooleanArray array,
                                     jboolean *elems,
                                     jint mode) {
        functions->ReleaseBooleanArrayElements(this,array,elems,mode);
    }
    void ReleaseByteArrayElements(jbyteArray array,
                                  jbyte *elems,
                                  jint mode) {
        functions->ReleaseByteArrayElements(this,array,elems,mode);
    }
    void ReleaseCharArrayElements(jcharArray array,
                                  jchar *elems,
                                  jint mode) {
        functions->ReleaseCharArrayElements(this,array,elems,mode);
    }
    void ReleaseShortArrayElements(jshortArray array,
                                   jshort *elems,
                                   jint mode) {
        functions->ReleaseShortArrayElements(this,array,elems,mode);
    }
    void ReleaseIntArrayElements(jintArray array,
                                 jint *elems,
                                 jint mode) {
        functions->ReleaseIntArrayElements(this,array,elems,mode);
    }
    void ReleaseLongArrayElements(jlongArray array,
                                  jlong *elems,
                                  jint mode) {
        functions->ReleaseLongArrayElements(this,array,elems,mode);
    }
    void ReleaseFloatArrayElements(jfloatArray array,
                                   jfloat *elems,
                                   jint mode) {
        functions->ReleaseFloatArrayElements(this,array,elems,mode);
    }
    void ReleaseDoubleArrayElements(jdoubleArray array,
                                    jdouble *elems,
                                    jint mode) {
        functions->ReleaseDoubleArrayElements(this,array,elems,mode);
    }

    void GetBooleanArrayRegion(jbooleanArray array,
                               jsize start, jsize len, jboolean *buf) {
        functions->GetBooleanArrayRegion(this,array,start,len,buf);
    }
    void GetByteArrayRegion(jbyteArray array,
                            jsize start, jsize len, jbyte *buf) {
        functions->GetByteArrayRegion(this,array,start,len,buf);
    }
    void GetCharArrayRegion(jcharArray array,
                            jsize start, jsize len, jchar *buf) {
        functions->GetCharArrayRegion(this,array,start,len,buf);
    }
    void GetShortArrayRegion(jshortArray array,
                             jsize start, jsize len, jshort *buf) {
        functions->GetShortArrayRegion(this,array,start,len,buf);
    }
    void GetIntArrayRegion(jintArray array,
                           jsize start, jsize len, jint *buf) {
        functions->GetIntArrayRegion(this,array,start,len,buf);
    }
    void GetLongArrayRegion(jlongArray array,
                            jsize start, jsize len, jlong *buf) {
        functions->GetLongArrayRegion(this,array,start,len,buf);
    }
    void GetFloatArrayRegion(jfloatArray array,
                             jsize start, jsize len, jfloat *buf) {
        functions->GetFloatArrayRegion(this,array,start,len,buf);
    }
    void GetDoubleArrayRegion(jdoubleArray array,
                              jsize start, jsize len, jdouble *buf) {
        functions->GetDoubleArrayRegion(this,array,start,len,buf);
    }

    void SetBooleanArrayRegion(jbooleanArray array, jsize start, jsize len,
                               jboolean *buf) {
        functions->SetBooleanArrayRegion(this,array,start,len,buf);
    }
    void SetByteArrayRegion(jbyteArray array, jsize start, jsize len,
                            jbyte *buf) {
        functions->SetByteArrayRegion(this,array,start,len,buf);
    }
    void SetCharArrayRegion(jcharArray array, jsize start, jsize len,
                            jchar *buf) {
        functions->SetCharArrayRegion(this,array,start,len,buf);
    }
    void SetShortArrayRegion(jshortArray array, jsize start, jsize len,
                             jshort *buf) {
        functions->SetShortArrayRegion(this,array,start,len,buf);
    }
    void SetIntArrayRegion(jintArray array, jsize start, jsize len,
                           jint *buf) {
        functions->SetIntArrayRegion(this,array,start,len,buf);
    }
    void SetLongArrayRegion(jlongArray array, jsize start, jsize len,
                            jlong *buf) {
        functions->SetLongArrayRegion(this,array,start,len,buf);
    }
    void SetFloatArrayRegion(jfloatArray array, jsize start, jsize len,
                             jfloat *buf) {
        functions->SetFloatArrayRegion(this,array,start,len,buf);
    }
    void SetDoubleArrayRegion(jdoubleArray array, jsize start, jsize len,
                              jdouble *buf) {
        functions->SetDoubleArrayRegion(this,array,start,len,buf);
    }

    jint RegisterNatives(jclass clazz, const JNINativeMethod *methods,
                         jint nMethods) {
        return functions->RegisterNatives(this,clazz,methods,nMethods);
    }
    jint UnregisterNatives(jclass clazz) {
        return functions->UnregisterNatives(this,clazz);
    }

    jint MonitorEnter(jobject obj) {
        return functions->MonitorEnter(this,obj);
    }
    jint MonitorExit(jobject obj) {
        return functions->MonitorExit(this,obj);
    }

    jint GetJavaVM(JavaVM **vm) {
        return functions->GetJavaVM(this,vm);
    }

    void GetStringRegion(jstring str, jsize start, jsize len, jchar *buf) {
        functions->GetStringRegion(this,str,start,len,buf);
    }
    void GetStringUTFRegion(jstring str, jsize start, jsize len, char *buf) {
        functions->GetStringUTFRegion(this,str,start,len,buf);
    }

    void * GetPrimitiveArrayCritical(jarray array, jboolean *isCopy) {
        return functions->GetPrimitiveArrayCritical(this,array,isCopy);
    }
    void ReleasePrimitiveArrayCritical(jarray array, void *carray, jint mode) {
        functions->ReleasePrimitiveArrayCritical(this,array,carray,mode);
    }

    const jchar * GetStringCritical(jstring string, jboolean *isCopy) {
        return functions->GetStringCritical(this,string,isCopy);
    }
    void ReleaseStringCritical(jstring string, const jchar *cstring) {
        functions->ReleaseStringCritical(this,string,cstring);
    }

    jweak NewWeakGlobalRef(jobject obj) {
        return functions->NewWeakGlobalRef(this,obj);
    }
    void DeleteWeakGlobalRef(jweak ref) {
        functions->DeleteWeakGlobalRef(this,ref);
    }

    jboolean ExceptionCheck() {
        return functions->ExceptionCheck(this);
    }

    jobject NewDirectByteBuffer(void* address, jlong capacity) {
        return functions->NewDirectByteBuffer(this, address, capacity);
    }
    void* GetDirectBufferAddress(jobject buf) {
        return functions->GetDirectBufferAddress(this, buf);
    }
    jlong GetDirectBufferCapacity(jobject buf) {
        return functions->GetDirectBufferCapacity(this, buf);
    }


};

typedef struct JavaVMOption {
    char *optionString;
    void *extraInfo;
} JavaVMOption;

typedef struct JavaVMInitArgs {
    jint version;

    jint nOptions;
    JavaVMOption *options;
    jboolean ignoreUnrecognized;
} JavaVMInitArgs;

typedef struct JavaVMAttachArgs {
    jint version;

    char *name;
    jobject group;
} JavaVMAttachArgs;



typedef struct JDK1_1InitArgs {
    jint version;

    char **properties;
    jint checkSource;
    jint nativeStackSize;
    jint javaStackSize;
    jint minHeapSize;
    jint maxHeapSize;
    jint verifyMode;
    char *classpath;

    jint ( *vfprintf)(FILE *fp, const char *format, va_list args);
    void ( *exit)(jint code);
    void ( *abort)(void);

    jint enableClassGC;
    jint enableVerboseGC;
    jint disableAsyncGC;
    jint verbose;
    jboolean debugging;
    jint debugPort;
} JDK1_1InitArgs;

typedef struct JDK1_1AttachArgs {
    void * __padding;
} JDK1_1AttachArgs;






struct JNIInvokeInterface_ {
    void *reserved0;
    void *reserved1;
    void *reserved2;


    void* cfm_vectors[4];


    jint ( *DestroyJavaVM)(JavaVM *vm);

    jint ( *AttachCurrentThread)(JavaVM *vm, void **penv, void *args);

    jint ( *DetachCurrentThread)(JavaVM *vm);

    jint ( *GetEnv)(JavaVM *vm, void **penv, jint version);

    jint ( *AttachCurrentThreadAsDaemon)(JavaVM *vm, void **penv, void *args);




};

struct JavaVM_ {
    const struct JNIInvokeInterface_ *functions;


    jint DestroyJavaVM() {
        return functions->DestroyJavaVM(this);
    }
    jint AttachCurrentThread(void **penv, void *args) {
        return functions->AttachCurrentThread(this, penv, args);
    }
    jint DetachCurrentThread() {
        return functions->DetachCurrentThread(this);
    }

    jint GetEnv(void **penv, jint version) {
        return functions->GetEnv(this, penv, version);
    }
    jint AttachCurrentThreadAsDaemon(void **penv, void *args) {
        return functions->AttachCurrentThreadAsDaemon(this, penv, args);
    }

};






 jint
JNI_GetDefaultJavaVMInitArgs(void *args);

 jint
JNI_CreateJavaVM(JavaVM **pvm, void **penv, void *args);

 jint
JNI_GetCreatedJavaVMs(JavaVM **, jsize, jsize *);


 jint
JNI_OnLoad(JavaVM *vm, void *reserved);

 void
JNI_OnUnload(JavaVM *vm, void *reserved);






}
# 11 "/Users/jmh/devel/libgist/include/amdb_ext.h" 2
# 32 "/Users/jmh/devel/libgist/include/amdb_ext.h"
class gist_query_t;
class gist_disppredcursor_t;

class amdb_ext_t {

public:

    gist_ext_t::gist_ext_ids id;
    static amdb_ext_t* amdb_ext_list[gist_ext_t::gist_numext];
    static amdb_ext_t* extension(gist_ext_t* ext)
    {
        return (amdb_ext_list[ext->myId]);
    }






    typedef void (*PenaltyFct)(
        gist_ext_t* ext,
        const gist_p& page,
        int slot,
        const void* key,
        int klen,
        const void* data,
        int dlen,
        gist_penalty_t& p);
    PenaltyFct penalty;


    typedef void (*DisplayFct)(
        const gist_p& page,
        JNIEnv* env,
        jint* highlights,
        jint numHighlights,
        jobject graphicsContext,
        jobject normalColor,
        jobject highlightColor,
        jint width,
        jint height);
    DisplayFct display;


    typedef rc_t (*DisplayPredsFct)(
        JNIEnv* env,
        jint width,
        jint height,
        jobject graphicsContext,
        jobject colors[],
        gist_disppredcursor_t& pcursor);
    DisplayPredsFct displayPreds;


    typedef bool (*ConsistentFct)(
        gist_ext_t* ext,
        const gist_query_t* query,
        const vec_t& pred,
        int level);
    ConsistentFct consistent;

    amdb_ext_t(
        gist_ext_t::gist_ext_ids id,
        PenaltyFct p,
        DisplayFct d,
        DisplayPredsFct dp,
        ConsistentFct consistent);

};
# 6 "amdb_rtree.cc" 2
# 1 "/Users/jmh/devel/libgist/include/amdb_support.h" 1
# 9 "/Users/jmh/devel/libgist/include/amdb_support.h"
#pragma interface "amdb_support.h"





# 1 "/Users/jmh/devel/libgist/include/gist_disppredcursor.h" 1
# 10 "/Users/jmh/devel/libgist/include/gist_disppredcursor.h"
# 1 "/Users/jmh/devel/libgist/include/gist_p.h" 1
# 10 "/Users/jmh/devel/libgist/include/gist_p.h"
#pragma interface "gist_p.h"




# 1 "/Users/jmh/devel/libgist/include/gist_file.h" 1
# 10 "/Users/jmh/devel/libgist/include/gist_file.h"
#pragma interface "gist_file.h"



# 1 "/Users/jmh/devel/libgist/include/gist_htab.h" 1





#pragma interface "gist_htab.h"







class gist_htab {

public:

    gist_htab();
    ~gist_htab();

    void reset();
    int operator[](shpid_t page);
    void add(shpid_t key, int value);
    void remove(shpid_t key);

protected:

    struct entry {
        shpid_t key;
        int value;
        bool inUse;
    };

    entry tab[17];

    int hash(shpid_t key);
};
# 15 "/Users/jmh/devel/libgist/include/gist_file.h" 2

class gist_file {

public:


    gist_file();
    ~gist_file();




    rc_t create(const char *filename, const gist_ext_t* ext);

    rc_t open(const char *filename, const gist_ext_t*& ext);




    rc_t flush();


    rc_t close();


    int size() { return fileSize; }

    static const int invalidIdx;

    struct page_descr {
        shpid_t pageNo;
        char *page;
        bool isDirty;
        int pinCount;

        page_descr() : pageNo(0), page(0), isDirty(false), pinCount(0) {}
    };


    bool allUnpinned();

    page_descr *pinPage(shpid_t page);



    void unpinPage(page_descr *descr);


    page_descr *allocPage();



    rc_t freePage(page_descr *descr);


    void setDirty(page_descr *descr, bool isDirty);





    rc_t freelist(shpid_t list[], int& len);

protected:

    bool isOpen;
    int fileHandle;
    shpid_t fileSize;
    gist_htab htab;

    static const char* magic;

    struct header_info {
        char* magicStr;
        gist_ext_t::gist_ext_ids extId;
        shpid_t freeList;
        char* extName;

        header_info() :
            magicStr(0), extId(gist_ext_t::gist_numext), freeList(0), extName(0) {}
    };
    header_info header;
    rc_t _read_header();
    rc_t _write_header();



    page_descr descrs[17];
    void _resetDescrs();

    union {
        double d;
        char buf[17 * 8192];
    } x;
    char *buffers;

    rc_t _write_page(shpid_t pageNo, char *page);
    rc_t _read_page(shpid_t pageNo, char *page);
    int findFreeBuffer();
    shpid_t findFreePage();
    rc_t returnPage(shpid_t page);

};
# 16 "/Users/jmh/devel/libgist/include/gist_p.h" 2

class gist;




struct page_s {
    struct slot_t {
        int2 offset;
        uint2 length;
    };

    class space_t {
    public:
        space_t() {};
        ~space_t() {};

        void init(int);
        int nfree() const;

        int usable();

        rc_t acquire(int amt, int slot_bytes);
        void release(int amt);

    private:

        void _check_reserve();

        int2 _nfree;
    };

    enum {
        data_sz = (8192
                   - sizeof(shpid_t)
                   - sizeof(space_t)
                   - 3 * sizeof(int2)
                   - 1 * sizeof(int4)
                   - 2 * sizeof(slot_t)
                   - 2 * sizeof(int2)),
        max_slot = data_sz / sizeof(slot_t) + 2
    };

    shpid_t pid;
    space_t space;
    uint2 end;
    int2 nslots;
    int2 nvacant;
    int4 fill1;
    char data[data_sz];
    slot_t reserved_slot[1];

    slot_t slot[1];
};

class keyrec_t {
public:



    struct hdr_s {
        uint2 klen;
        uint2 elen;
        shpid_t child;
    };

    const char* key() const;
    const char* elem() const;
    const char* sep() const;
    smsize_t klen() const;
    smsize_t elen() const;
    smsize_t slen() const;
    smsize_t rlen() const;
    shpid_t child() const;

private:
    hdr_s _hdr;
    char* _body() const;
};


struct gistctrl_t {
    shpid_t root;
    int2 level;

    gistctrl_t();
};



class gist_p {
public:

    typedef page_s::slot_t slot_t;

    enum {
        data_sz = page_s::data_sz,
        max_slot = data_sz / sizeof(slot_t) + 2
    };

    enum {
        max_tup_sz = data_sz / 2 - sizeof(slot_t) - sizeof(keyrec_t),

        max_scnt = (data_sz - sizeof(gistctrl_t)) /
            (sizeof(keyrec_t) + sizeof(slot_t)) + 1

    };

    gist_p();
    ~gist_p();

    rc_t format(
        const shpid_t pid,
        const gistctrl_t *hdr);

    rc_t insert(
        const keyrec_t &tup);

    rc_t copy(gist_p& rsib);

    rc_t set_hdr(const gistctrl_t& new_hdr);
    void set_level(int2 level);
    int level() const;
    shpid_t root() const;

    bool is_leaf() const;
    bool is_node() const;
    bool is_root() const;


    const keyrec_t& rec(int idx) const;
    int rec_size(int idx) const;
    int nrecs() const;


    static int rec_size(size_t klen, size_t dlen);


    rc_t insert(
        const cvec_t& key,
        const cvec_t& el,
        int slot,
        shpid_t child = 0);
    rc_t remove(int slot);

    bool is_fixed() const;
    gist_p& operator=(const gist_p& p);
    smsize_t usable_space();
    shpid_t pid() const;

private:
    static const int _HDR_CORRECTION;


    rc_t _insert_expand(
        int idx,
        int cnt,
        const cvec_t tp[]);

    rc_t _remove_compress(int idx, int cnt);
    rc_t _overwrite(
        int idx,
        int start,
        const cvec_t& data);


    page_s* _pp;
    gist_file::page_descr* _descr;

    friend class gist;




    void _compress(int idx = -1);


    inline smsize_t _used_space();
    smsize_t _contig_space();

    smsize_t _tuple_size(int idx) const;
    void* _tuple_addr(int idx) const;
    inline bool _is_tuple_valid(int idx) const;
    const void* _get_hdr() const;


};

inline smsize_t
gist_p::_used_space()
{
    return (data_sz + 2 * sizeof(slot_t) - _pp->space.usable());
}

inline bool
gist_p::_is_tuple_valid(int idx) const
{
    return idx >= 0 && idx < _pp->nslots && _pp->slot[-idx].offset >=0;
}

inline shpid_t
gist_p::root() const
{
    gistctrl_t* hdr = (gistctrl_t *) _get_hdr();
    return hdr->root;
}

inline int
gist_p::rec_size(size_t klen, size_t dlen)
{
    return int(((uint4)((klen + dlen + sizeof(keyrec_t::hdr_s) + (0x8 -1)) & ~(0x8 -1))) + sizeof(slot_t));
}
# 11 "/Users/jmh/devel/libgist/include/gist_disppredcursor.h" 2

class gist;



class gist_disppredcursor_t {
public:

    gist_disppredcursor_t(gist* index);
    ~gist_disppredcursor_t();


    rc_t getNext(void*& key, int& klen, int& level, int& color);


    int numPreds();


    void add(shpid_t pageno, int level, int slot, int color);


    rc_t add(shpid_t pageno, int level, int color);


    void reset() { _next = 0; }

private:

    gist* _index;

    typedef Vector PredInfoVector;
    PredInfoVector _predInfo;
    int _next;


    gist_p _page;
    char _key[gist_p::max_tup_sz];

};
# 16 "/Users/jmh/devel/libgist/include/amdb_support.h" 2
# 27 "/Users/jmh/devel/libgist/include/amdb_support.h"
class gist_ext_t;
class gist_p;
class gist_query_t;
class gist_penalty_t;







class gist_predcursor_t;

class amdb_support
{

public:

    static const int displayDim;


    static void unorderedPenalty(
        gist_ext_t* e,
        const gist_p& page,
        int slot,
        const void* key,
        int klen,
        const void* data,
        int dlen,
        gist_penalty_t& p);

    static bool unorderedConsistent(
        gist_ext_t* ext,
        const gist_query_t* query,
        const vec_t& pred,
        int level);


    static rc_t parseInt(
        const char* str,
        void* out,
        int& len);


    static rc_t parseInt(
        istream& s,
        void* out,
        int& len);

    static rc_t parseStr(
        const char* str,
        void* out,
        int& len);


    static rc_t parsePoint(
        const char* str,
        void* out,
        int& len);


    static rc_t parsePoint(
        istream& s,
        void* out,
        int& len);

    static void displayPoints(
        gist_predcursor_t& pcursor,
        JNIEnv* env,
        jint* highlights,
        jint numHighlights,
        jobject graphicsContext,
        jobject normalColor,
        jobject highlightColor,
        jint width,
        jint height);


    static rc_t parseRect(
        const char* str,
        void* out,
        int& len);


    static rc_t parseRect(
        istream& s,
        void* out,
        int& len);

    static void display_bt_int_pred(
        const gist_p& page,
        JNIEnv* env,
        jint* highlights,
        jint numHighlights,
        jobject graphicsContext,
        jobject normalColor,
        jobject highlightColor,
        jint width,
        jint height);

    static void display_bt_int_data(
        const gist_p& page,
        JNIEnv* env,
        jint* highlights,
        jint numHighlights,
        jobject graphicsContext,
        jobject normalColor,
        jobject highlightColor,
        jint width,
        jint height);

    static void display_bt_str_pred(
        const gist_p& page,
        JNIEnv* env,
        jint* highlights,
        jint numHighlights,
        jobject graphicsContext,
        jobject normalColor,
        jobject highlightColor,
        jint width,
        jint height);

    static void display_bt_str_data(
        const gist_p& page,
        JNIEnv* env,
        jint* highlights,
        jint numHighlights,
        jobject graphicsContext,
        jobject normalColor,
        jobject highlightColor,
        jint width,
        jint height);

    static void displayRects(
        gist_predcursor_t& pcursor,
        JNIEnv* env,
        jint* highlights,
        jint numHighlights,
        jobject graphicsContext,
        jobject normalColor,
        jobject highlightColor,
        jint width,
        jint height);

    static void displayBoundingSpheres(
        gist_predcursor_t& pcursor,
        JNIEnv* env,
        jint* highlights,
        jint numHighlights,
        jobject graphicsContext,
        jobject normalColor,
        jobject highlightColor,
        jint width,
        jint height);

    static void displayCentroidSpheres(
        gist_predcursor_t& pcursor,
        JNIEnv* env,
        jint* highlights,
        jint numHighlights,
        jobject graphicsContext,
        jobject normalColor,
        jobject highlightColor,
        jint width,
        jint height);

    static void displaySphereRects(
        gist_predcursor_t& pcursor,
        JNIEnv* env,
        jint* highlights,
        jint numHighlights,
        jobject graphicsContext,
        jobject normalColor,
        jobject highlightColor,
        jint width,
        jint height);




    static rc_t parseGeoQuery(
        const char* str,
        gist_query_t*& query);

    static rc_t display_rt_point_preds(
        JNIEnv* env,
        jint width,
        jint height,
        jobject graphicsContext,
        jobject colors[],
        gist_disppredcursor_t& pcursor);

    static rc_t display_rt_rect_preds(
        JNIEnv* env,
        jint width,
        jint height,
        jobject graphicsContext,
        jobject colors[],
        gist_disppredcursor_t& pcursor);

    static rc_t display_sp_point_preds(
        JNIEnv* env,
        jint width,
        jint height,
        jobject graphicsContext,
        jobject colors[],
        gist_disppredcursor_t& pcursor);

    static rc_t display_ss_point_preds(
        JNIEnv* env,
        jint width,
        jint height,
        jobject graphicsContext,
        jobject colors[],
        gist_disppredcursor_t& pcursor);

    static rc_t display_sr_point_preds(
        JNIEnv* env,
        jint width,
        jint height,
        jobject graphicsContext,
        jobject colors[],
        gist_disppredcursor_t& pcursor);
};
# 7 "amdb_rtree.cc" 2
# 1 "/Users/jmh/devel/libgist/include/gist_unordered.h" 1
# 11 "/Users/jmh/devel/libgist/include/gist_unordered.h"
class gist_query_t;

class gist_predcursor_t {
public:

    struct entry {
        const void *key;
        int keyLen;
    };


    enum {
        maxElems = gist_p::max_scnt + 2
    };
    int numElems;
    entry elems[maxElems];

    gist_predcursor_t();
    ~gist_predcursor_t();


    void add(const void* data, int len);


    void add(const gist_p& page);


    void reset();
};


class gist_unordered_ext_t {
public:


    virtual bool consistent(
        const gist_query_t* query,
        const void* pred,
        int predLen,
        int level) const = 0;


    virtual void penalty(
        const void* subtreePred,
        int predLen,
        int level,
        const void* newKey,
        int keyLen,
        gist_penalty_t& p) const = 0;


    virtual void union_bp(
        const gist_predcursor_t& pcursor,
        const gist_p& page,
        const vec_t& pred1,
        const vec_t& pred2,
        vec_t& bp,
        bool& bpChanged,
        bool bpIsValid)
        const = 0;
# 81 "/Users/jmh/devel/libgist/include/gist_unordered.h"
    virtual void pickSplit(
        gist_predcursor_t& pcursor,
        const gist_p& page,
        int rightEntries[],
        int& numRightEntries,
        const void* oldBp,
        int oldLen,
        void* leftBp,
        int& leftLen,
        void* rightBp,
        int& rightLen)
        const = 0;

    virtual gist_cursorext_t* queryCursor(
        const gist_query_t* query) const = 0;




    virtual bool check(
        const void* bp,
        int bplen,
        const void* pred,
        int predlen,
        int level)
        const = 0;

};
# 8 "amdb_rtree.cc" 2






static void rt_point_display(
    const gist_p& page,
    JNIEnv* env,
    jint* highlights,
    jint numHighlights,
    jobject g,
    jobject normalColor,
    jobject highlightColor,
    jint width,
    jint height)
{
    gist_predcursor_t pcursor;
    pcursor.add(page);
    if (pcursor.numElems == 0) return;

    if (page.level() == 1) {
        amdb_support::displayPoints(pcursor, env, highlights, numHighlights,
            g, normalColor, highlightColor, width, height);
    } else {
        amdb_support::displayRects(pcursor, env, highlights, numHighlights,
            g, normalColor, highlightColor, width, height);
    }
}

static void rt_rect_display(
    const gist_p& page,
    JNIEnv* env,
    jint* highlights,
    jint numHighlights,
    jobject g,
    jobject normalColor,
    jobject highlightColor,
    jint width,
    jint height)
{
    gist_predcursor_t pcursor;
    pcursor.add(page);
    if (pcursor.numElems == 0) return;

    amdb_support::displayRects(pcursor, env, highlights, numHighlights,
        g, normalColor, highlightColor, width, height);
}

static void sp_point_display(
    const gist_p& page,
    JNIEnv* env,
    jint* highlights,
    jint numHighlights,
    jobject g,
    jobject normalColor,
    jobject highlightColor,
    jint width,
    jint height)
{
    gist_predcursor_t pcursor;
    pcursor.add(page);
    if (pcursor.numElems == 0) return;

    if (page.level() == 1) {
        amdb_support::displayPoints(pcursor, env, highlights, numHighlights,
            g, normalColor, highlightColor, width, height);
    } else {
        amdb_support::displayBoundingSpheres(pcursor, env, highlights, numHighlights,
            g, normalColor, highlightColor, width, height);
    }
}

static void ss_point_display(
    const gist_p& page,
    JNIEnv* env,
    jint* highlights,
    jint numHighlights,
    jobject g,
    jobject normalColor,
    jobject highlightColor,
    jint width,
    jint height)
{
    gist_predcursor_t pcursor;
    pcursor.add(page);
    if (pcursor.numElems == 0) return;

    if (page.level() == 1) {
        amdb_support::displayPoints(pcursor, env, highlights, numHighlights,
            g, normalColor, highlightColor, width, height);
    } else {
        amdb_support::displayCentroidSpheres(pcursor, env, highlights, numHighlights,
            g, normalColor, highlightColor, width, height);
    }
}

static void sr_point_display(
    const gist_p& page,
    JNIEnv* env,
    jint* highlights,
    jint numHighlights,
    jobject g,
    jobject normalColor,
    jobject highlightColor,
    jint width,
    jint height)
{
    gist_predcursor_t pcursor;
    pcursor.add(page);
    if (pcursor.numElems == 0) return;

    if (page.level() == 1) {
        amdb_support::displayPoints(pcursor, env, highlights, numHighlights,
            g, normalColor, highlightColor, width, height);
    } else {
        amdb_support::displaySphereRects(pcursor, env, highlights, numHighlights,
            g, normalColor, highlightColor, width, height);
    }
}

static void rt_point_display_preds(
        JNIEnv* env,
        jint width,
        jint height,
        jobject graphicsContext,
        jobject colors[],
        gist_disppredcursor_t& pcursor)
{

    if (pcursor.numPreds() == 0) return;

}

amdb_ext_t amdb_rt_point_ext(gist_ext_t::rt_point_ext_id,
    amdb_support::unorderedPenalty, rt_point_display,
    amdb_support::display_rt_point_preds,
    amdb_support::unorderedConsistent);

amdb_ext_t amdb_rt_rect_ext(gist_ext_t::rt_rect_ext_id,
    amdb_support::unorderedPenalty, rt_rect_display,
    amdb_support::display_rt_rect_preds,
    amdb_support::unorderedConsistent);

amdb_ext_t amdb_sp_point_ext(gist_ext_t::sp_point_ext_id,
    amdb_support::unorderedPenalty, sp_point_display,
    amdb_support::display_sp_point_preds,
    amdb_support::unorderedConsistent);

amdb_ext_t amdb_ss_point_ext(gist_ext_t::ss_point_ext_id,
    amdb_support::unorderedPenalty, ss_point_display,
    amdb_support::display_ss_point_preds,
    amdb_support::unorderedConsistent);

amdb_ext_t amdb_sr_point_ext(gist_ext_t::sr_point_ext_id,
    amdb_support::unorderedPenalty, sr_point_display,
    amdb_support::display_sr_point_preds,
    amdb_support::unorderedConsistent);

amdb_ext_t amdb_rr_point_ext(gist_ext_t::rr_point_ext_id,
    amdb_support::unorderedPenalty, rt_point_display,
    amdb_support::display_rt_point_preds,
    amdb_support::unorderedConsistent);
