#ifndef H_MUSEPACY

#if XVT_CC_PROTO
MUSE_API 
set_pac_win(WINDOW task, WINDOW pac);
MUSE_API 
pacify(char *dummy_message, short dummy_percent_complete);
#else
MUSE_API 
set_pac_win();
MUSE_API 
pacify();
#endif

#endif
