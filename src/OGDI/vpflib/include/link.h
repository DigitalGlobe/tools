#ifndef LINK_H
#define LINK_H

/*****************************************************************************
  From "Motif Programming, The Essentials...and More" by Marshall Brain.
  Published by Digital Press, ISBN 1-55558-089.
  This is the header file for the link library.
  ******************************************************************************/

/* link.h */

/* Link module.v1.1, 2/6/92 by Marshal Brain */

/*This module allows a program to form links to other separately executing
  programs and communicate with them.  Links can be opened and closed,
  and the program using this library can write to and read from the other
  program over the link. */

/*Warning--This module will not link with all programs.  If the program
  does anything weird with stdout, or if it fails to flush stdout correctly,
  then this module will fail.  If you are creating a stand-alone program
  that you wish to link to another program with this library, then you
  MUST make sure that stdout is flushed correctly.  Either call 
  "fflush(stdout)" after every printf, or call setbuf(stdout,NULL)" at the
  beginning of the pr9ogram to eliminate buffering. */

#include "xvt.h"

#if XVT_OS_ISUNIX == TRUE    /* only on SUN */

#include <stdio.h>
#include <strings.h>
#include <signal.h>
#include <sys/ioctl.h>

typedef struct		/* holds all info relevant to one link */
{
  int pipefd1[2], pipefd2[2];
  int pid;
  FILE *fpin, *fpout;
} LinkHandle;

/* Open a link to another program named name, passing a param to the
   program if desired.  This routine will execute name in parallel and you can
   start communicating with it with LinkRead and LinkWrite.
   Note - convert over to argv/argc format for long parameter lists -- DONE 24AUG93-sdc .
   */
extern LinkOpen(
#if XVT_CC_PROTO
		LinkHandle *l,
		char       *argList[]
#endif
		);

/* Close the link to a program that has terminated.  Use linkkill if the
   program needs to be terminated as well.
   */
extern LinkClose (
#if XVT_CC_PROTO
		   LinkHandle *l
#endif
		   );

/* Close the link to a program that has terminated.  Use linkkill if the
   program needs to be terminated as well.
   */
extern LinkCloseNoWait (
#if XVT_CC_PROTO
		   LinkHandle *l
#endif
		   );

/* Read from the program started with linkopen.  Returns a 0 if there was
   stuff to read, or a 1 if the linked program terminated.
   */
extern int LinkRead (
#if XVT_CC_PROTO
		      LinkHandle *l,
		      char        s[]
#endif
		      );

/* Returns the number of bytes waiting in the input buffer.
   If 0, then linkread will block if it is called. 
   */
extern int LinkInputWaiting (
#if XVT_CC_PROTO
			       LinkHandle *l
#endif
			       );

/* Write a char, without a newline, to the program.
 */
extern LinkWriteChar (
#if XVT_CC_PROTO
			LinkHandle *l,
			int         c
#endif
			);

/* Write a string to the program, with a newline.
 */
extern LinkWrite (
#if XVT_CC_PROTO
		   LinkHandle *l,
		   char        s[]
#endif
		   );

/* Write a string to the program, without a newline.
 */
extern LinkWriteNoEOL (
#if XVT_CC_PROTO
		   LinkHandle *l,
		   char        s[]
#endif
		   );

/* Kill the program and close the link.  If the program has terminated on its
   own use LinkClose instead.
   */
extern LinkKill (
#if XVT_CC_PROTO
		  LinkHandle *l
#endif
		  );

/* Quit the program and close the link.  If the program has terminated on its
   own use LinkClose instead.
   */
extern LinkQuit (
#if XVT_CC_PROTO
		  LinkHandle *l
#endif
		  );

/* Stop the program and close the link.  If the program has terminated on its
   own use LinkClose instead.
   */
extern LinkStop (
#if XVT_CC_PROTO
		  LinkHandle *l
#endif
		  );

#endif

#endif /* LINK_H */
