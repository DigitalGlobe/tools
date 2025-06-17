/* LibTomCrypt, modular cryptographic library -- Tom St Denis
 *
 * LibTomCrypt is a library that provides various cryptographic
 * algorithms in a highly modular and flexible manner.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 */
/**
    @file eax_addheader.c
    EAX implementation, add meta-data, by Tom St Denis
*/
#include "tomcrypt.h"

#ifdef LTC_EAX_MODE

/**
    add header (metadata) to the stream
    @param eax    The current EAX state
    @param header The header (meta-data) data you wish to add to the state
    @param length The length of the header data
    @return CRYPT_OK if successful
*/
int eax_addheader(eax_state *eax, const unsigned char *header,
                  unsigned long length)
{
   LTC_ARGCHK(eax    != NULL);
   LTC_ARGCHK(header != NULL);
   return omac_process(&eax->headeromac, header, length);
}

#endif

/* ref:         tag: v5.0.2 */
/* git commit:  f6d531779d267b91f2a6037c82260ce6f6d10da8 */
/* commit time: 2025-02-11 20:17:04 +0000 */
