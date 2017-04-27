#ifndef H_MUSESYS

#define H_MUSESYS

#if XVT_CC_PROTO
int32 
muse_filelength(char *path);
FILE           *
file_open(char *path, char *mode);
MUSE_API 
dir_restore(char *path);
MUSE_API 
dir_push(char *sub_dir, char *path);
MUSE_API 
dir_create(char *path);
#else
int32 
muse_filelength();
FILE           *
file_open();
MUSE_API 
dir_restore();
MUSE_API 
dir_push();
MUSE_API 
dir_create();
#endif

#endif
