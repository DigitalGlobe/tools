#ifndef H_VPF_FUNC
#define H_VPF_FUNC

#ifndef H_MUSE1
#include "muse1.h"
#endif

#ifndef _MACHINE_
#include "machine.h"
#endif

/* Prototype Definitions */

#if XVT_CC_PROTO
ID_TRIPLET      read_key(VPFTABLE *, BYTE_ORDER *);
void           *get_table_element(int32, ROW, VPFTABLE *, void *, int32 *);
char           *get_string(int32 *, char *, char);
char            get_char(int32 *, char *);
int32            get_vpf_string(VPFTABLE *, ROW *, int32, char *, int32);
int32            get_number(int32 *, char *, char);
int32            parse_header(VPFTABLE *, BYTE_ORDER *);
ROW             get_row(int32, VPFTABLE *, BYTE_ORDER *);
void            free_row(ROW, VPFTABLE *);
void            format_date(VDATE, char *);
ERRSTATUS       open_table(char *, VPFTABLE *, BYTE_ORDER *);
void            close_table(VPFTABLE *);
void            clear_table(VPFTABLE *);
char           *rightjust(char *);
char           *cpy_del(char *, char, int32 *);
void            swap_two(char *, char *);
void            swap_four(char *, char *);
void            swap_eight(char *, char *);
int32            file_read(void *, VpfDataType, size_t, FILE *, BYTE_ORDER *);
int32            file_write(void *, VpfDataType, size_t, FILE *, BYTE_ORDER *);
ERRSTATUS       header_read(VPT **, VPFTABLE *, BYTE_ORDER *);
ERRSTATUS       dht_read(VPFTABLE *, DHT **, BYTE_ORDER *);
ERRSTATUS       lat_read(VPFTABLE *, LAT **, BYTE_ORDER *);
ERRSTATUS       cat_read(VPFTABLE *, CAT **, BYTE_ORDER *);
ERRSTATUS       grt_read(VPFTABLE *, GRT **, BYTE_ORDER *);
ERRSTATUS       lht_read(VPFTABLE *, LHT **, BYTE_ORDER *);
ERRSTATUS       idx_read(VPFTABLE *, IDX **, BYTE_ORDER *);
ERRSTATUS       edg_read(VPFTABLE *, EDG **, IDX *, BYTE_ORDER *);
ERRSTATUS       txt_read(VPFTABLE *, TXT **, IDX *, BYTE_ORDER *);
ERRSTATUS       end_read(VPFTABLE *, END **, IDX *, BYTE_ORDER *);
ERRSTATUS       vpf_write(char *, VECTOR *, BYTE_ORDER *);
ERRSTATUS       hdr_write(VPFTABLE *, VPT *, BYTE_ORDER *);
ERRSTATUS       dht_write(VPFTABLE *, DHT *, BYTE_ORDER *);
ERRSTATUS       lat_write(VPFTABLE *, LAT *, BYTE_ORDER *);
ERRSTATUS       lht_write(VPFTABLE *, LHT *, BYTE_ORDER *);
ERRSTATUS       grt_write(VPFTABLE *, GRT *, BYTE_ORDER *);
ERRSTATUS       cat_write(VPFTABLE *, CAT *, BYTE_ORDER *);
ERRSTATUS       idx_write(VPFTABLE *, IDX *, BYTE_ORDER *);
ERRSTATUS       edg_write(VPFTABLE *, EDG *, BYTE_ORDER *);
ERRSTATUS       txt_write(VPFTABLE *, TXT *, BYTE_ORDER *);
ERRSTATUS       end_write(VPFTABLE *, END *, BYTE_ORDER *);
#else
ID_TRIPLET      read_key();
ERRSTATUS       open_table();
void            close_table();
void            clear_table();
void           *get_table_element();
char           *get_string();
char            get_char();
int32            get_number();
int32            get_vpf_string();
int32            parse_header();
void            free_row();
void            format_date();
ROW             get_row();
char           *rightjust();
char           *cpy_del();
void            swap_two();
void            swap_four();
void            swap_eight();
int32            file_read();
int32            file_write();
ERRSTATUS       header_read();
ERRSTATUS       dht_read();
ERRSTATUS       lat_read();
ERRSTATUS       cat_read();
ERRSTATUS       grt_read();
ERRSTATUS       lht_read();
ERRSTATUS       idx_read();
ERRSTATUS       edg_read();
ERRSTATUS       txt_read();
ERRSTATUS       end_read();
ERRSTATUS       vpf_write();
ERRSTATUS       hdr_write();
ERRSTATUS       dht_write();
ERRSTATUS       lat_write();
ERRSTATUS       lht_write();
ERRSTATUS       grt_write();
ERRSTATUS       cat_write();
ERRSTATUS       idx_write();
ERRSTATUS       edg_write();
ERRSTATUS       txt_write();
ERRSTATUS       end_write();
#endif

#endif				/* H_VPF_FUNC */
