/* INIFUNC.H */

#ifndef INIFUNC_H
#define INIFUNC_H

#if XVT_CC_PROTO
extern FILE *ini_open(char *type);
extern int32 get_ini_string(FILE *fp, char *section, char *entry,
                            char *def, char *string, size_t sizestr);
extern int32 start_ini_section(char *ini_file, char *section);
extern int32 section_exist(FILE *fp, char *section);
#else
extern FILE *ini_open();
extern int32 get_ini_string();
extern int32 start_ini_section();
extern int32 section_exist();
#endif

#endif
