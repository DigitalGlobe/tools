/******************************************************************************
 *
 * Component: OGDI Core C API
 * Purpose: Handling of feature attribute list parsing.
 * 
 ******************************************************************************
 * Copyright (C) 1995 Logiciels et Applications Scientifiques (L.A.S.) Inc
 * Permission to use, copy, modify and distribute this software and
 * its documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies, that
 * both the copyright notice and this permission notice appear in
 * supporting documentation, and that the name of L.A.S. Inc not be used 
 * in advertising or publicity pertaining to distribution of the software 
 * without specific, written prior permission. L.A.S. Inc. makes no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 ******************************************************************************
 *
 * $Log: ecslist.c,v $
 * Revision 1.2  2001/04/09 15:04:34  warmerda
 * applied new source headers
 *
 */

#include "ecs.h"

ECS_CVSID("$Id: ecslist.c,v 1.2 2001/04/09 15:04:34 warmerda Exp $");

#define UCHAR(c) ((unsigned char) (c))

/*
 *----------------------------------------------------------------------
 *
 * ecs_Backslash --
 *
 *	Figure out how to handle a backslash sequence.
 *
 * Results:
 *	The return value is the character that should be substituted
 *	in place of the backslash sequence that starts at src.  If
 *	readPtr isn't NULL then it is filled in with a count of the
 *	number of characters in the backslash sequence.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

char
ecs_Backslash(src, readPtr)
    char *src;			/* Points to the backslash character of
				 * a backslash sequence. */
    int *readPtr;		/* Fill in with number of characters read
				 * from src, unless NULL. */
{
    register char *p = src+1;
    char result;
    int count;

    count = 2;

    switch (*p) {
	case 'a':
	    result = 0x7;	/* Don't say '\a' here, since some compilers */
	    break;		/* don't support it. */
	case 'b':
	    result = '\b';
	    break;
	case 'f':
	    result = '\f';
	    break;
	case 'n':
	    result = '\n';
	    break;
	case 'r':
	    result = '\r';
	    break;
	case 't':
	    result = '\t';
	    break;
	case 'v':
	    result = '\v';
	    break;
	case 'x':
	    if (isxdigit(UCHAR(p[1]))) {
		char *end;

		result = strtoul(p+1, &end, 16);
		count = end - src;
	    } else {
		count = 2;
		result = 'x';
	    }
	    break;
	case '\n':
	    do {
		p++;
	    } while (isspace(UCHAR(*p)));
	    result = ' ';
	    count = p - src;
	    break;
	case 0:
	    result = '\\';
	    count = 1;
	    break;
	default:
	    if (isdigit(UCHAR(*p))) {
		result = *p - '0';
		p++;
		if (!isdigit(UCHAR(*p))) {
		    break;
		}
		count = 3;
		result = (result << 3) + (*p - '0');
		p++;
		if (!isdigit(UCHAR(*p))) {
		    break;
		}
		count = 4;
		result = (result << 3) + (*p - '0');
		break;
	    }
	    result = *p;
	    count = 2;
	    break;
    }

    if (readPtr != NULL) {
	*readPtr = count;
    }
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * ecs_FindElement --
 *
 *	Given a pointer into a list, locate the first (or next)
 *	element in the list.
 *
 * Results:
 *	The return value is normally TRUE, which means that the
 *	element was successfully located.  If FALSE is returned
 *	it means that list didn't have proper list structure;
 *
 *	If TRUE is returned, then *elementPtr will be set to point
 *	to the first element of list, and *nextPtr will be set to point
 *	to the character just after any white space following the last
 *	character that's part of the element.  If this is the last argument
 *	in the list, then *nextPtr will point to the NULL character at the
 *	end of list.  If sizePtr is non-NULL, *sizePtr is filled in with
 *	the number of characters in the element.  If the element is in
 *	braces, then *elementPtr will point to the character after the
 *	opening brace and *sizePtr will not include either of the braces.
 *	If there isn't an element in the list, *sizePtr will be zero, and
 *	both *elementPtr and *termPtr will refer to the null character at
 *	the end of list.  Note:  this procedure does NOT collapse backslash
 *	sequences.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int ecs_FindElement(list, elementPtr, nextPtr, sizePtr, bracePtr)
     register char *list;	/* String containing Tcl list with zero
				 * or more elements (possibly in braces). */
     char **elementPtr;		/* Fill in with location of first significant
				 * character in first element of list. */
     char **nextPtr;		/* Fill in with location of character just
				 * after all white space following end of
				 * argument (i.e. next argument or end of
				 * list). */
     int *sizePtr;		/* If non-zero, fill in with size of
				 * element. */
     int *bracePtr;		/* If non-zero fill in with non-zero/zero
				 * to indicate that arg was/wasn't
				 * in braces. */
{
  register char *p;
  int openBraces = 0;
  int inQuotes = 0;
  int size;
  
  /*
   * Skim off leading white space and check for an opening brace or
   * quote.   Note:  use of "isascii" below and elsewhere in this
   * procedure is a temporary hack (7/27/90) because Mx uses characters
   * with the high-order bit set for some things.  This should probably
   * be changed back eventually, or all of Tcl should call isascii.
   */
  
  while (isspace(UCHAR(*list))) {
    list++;
  }
  if (*list == '{') {
    openBraces = 1;
    list++;
  } else if (*list == '"') {
    inQuotes = 1;
    list++;
  }
  if (bracePtr != 0) {
    *bracePtr = openBraces;
  }
  p = list;
  
  /*
   * Find the end of the element (either a space or a close brace or
   * the end of the string).
   */
  
  while (1) {
    switch (*p) {
      
      /*
       * Open brace: don't treat specially unless the element is
       * in braces.  In this case, keep a nesting count.
       */
      
    case '{':
      if (openBraces != 0) {
	openBraces++;
      }
      break;
      
      /*
       * Close brace: if element is in braces, keep nesting
       * count and quit when the last close brace is seen.
       */

    case '}':
      if (openBraces == 1) {
	char *p2;
	
	size = p - list;
	p++;
	if (isspace(UCHAR(*p)) || (*p == 0)) {
	  goto done;
	}
	for (p2 = p; (*p2 != 0) && (!isspace(UCHAR(*p2)))
	     && (p2 < p+20); p2++) {
	  /* null body */
	}
	return FALSE;
      } else if (openBraces != 0) {
	openBraces--;
      }
      break;
      
      /*
       * Backslash:  skip over everything up to the end of the
       * backslash sequence.
       */
      
    case '\\': {
      int size;
      
      (void) ecs_Backslash(p, &size);
      p += size - 1;
      break;
    }
      
      /*
       * Space: ignore if element is in braces or quotes;  otherwise
       * terminate element.
       */
      
    case ' ':
    case '\f':
    case '\n':
    case '\r':
    case '\t':
    case '\v':
      if ((openBraces == 0) && !inQuotes) {
	size = p - list;
	goto done;
      }
      break;
      
      /*
       * Double-quote:  if element is in quotes then terminate it.
       */
      
    case '"':
      if (inQuotes) {
	char *p2;

	size = p-list;
	p++;
	if (isspace(UCHAR(*p)) || (*p == 0)) {
	  goto done;
	}
	for (p2 = p; (*p2 != 0) && (!isspace(UCHAR(*p2)))
	     && (p2 < p+20); p2++) {
	  /* null body */
	}
	return FALSE;
      }
      break;
      
      /*
       * End of list:  terminate element.
       */
      
    case 0:
      /*      return FALSE;*/
      size = p - list;
      goto done;
    }
    p++;
  }
  
 done:
  while (isspace(UCHAR(*p))) {
    p++;
  }
  *elementPtr = list;
  *nextPtr = p;
  if (sizePtr != 0) {
    *sizePtr = size;
  }
  return TRUE;
}

/*
 *----------------------------------------------------------------------
 *
 * ecs_CopyAndCollapse --
 *
 *	Copy a string and eliminate any backslashes that aren't in braces.
 *
 * Results:
 *	There is no return value.  Count chars. get copied from src
 *	to dst.  Along the way, if backslash sequences are found outside
 *	braces, the backslashes are eliminated in the copy.
 *	After scanning count chars. from source, a null character is
 *	placed at the end of dst.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

void
ecs_CopyAndCollapse(count, src, dst)
    int count;			/* Total number of characters to copy
				 * from src. */
    register char *src;		/* Copy from here... */
    register char *dst;		/* ... to here. */
{
    register char c;
    int numRead;

    for (c = *src; count > 0; src++, c = *src, count--) {
	if (c == '\\') {
	    *dst = ecs_Backslash(src, &numRead);
	    dst++;
	    src += numRead-1;
	    count -= numRead-1;
	} else {
	    *dst = c;
	    dst++;
	}
    }
    *dst = 0;
}

/*
 *----------------------------------------------------------------------
 *
 * ecs_SplitList --
 *
 *	Splits a list up into its constituent fields.
 *
 * Results
 *	The return value is normally TRUE, which means that
 *	the list was successfully split up.  If FALSE is
 *	returned, it means that "list" didn't have proper list
 *	structure;  interp->result will contain a more detailed
 *	error message.
 *
 *	*argvPtr will be filled in with the address of an array
 *	whose elements point to the elements of list, in order.
 *	*argcPtr will get filled in with the number of valid elements
 *	in the array.  A single block of memory is dynamically allocated
 *	to hold both the argv array and a copy of the list (with
 *	backslashes and braces removed in the standard way).
 *	The caller must eventually free this memory by calling free()
 *	on *argvPtr.  Note:  *argvPtr and *argcPtr are only modified
 *	if the procedure returns normally.
 *
 * Side effects:
 *	Memory is allocated.
 *
 *----------------------------------------------------------------------
 */

int ecs_SplitList(list, argcPtr, argvPtr)
     char *list;		/* Pointer to string with list structure. */
     int *argcPtr;		/* Pointer to location to fill in with
				 * the number of elements in the list. */
     char ***argvPtr;		/* Pointer to place to store pointer to array
				 * of pointers to list elements. */
{
  char **argv;
  register char *p;
  int size, i, result, elSize, brace;
  char *element;

  /*
   * Figure out how much space to allocate.  There must be enough
   * space for both the array of pointers and also for a copy of
   * the list.  To estimate the number of pointers needed, count
   * the number of space characters in the list.
   */

  for (size = 1, p = list; *p != 0; p++) {
    if (isspace(UCHAR(*p))) {
      size++;
    }
  }
  size++;			/* Leave space for final NULL pointer. */
  argv = (char **) malloc((unsigned)
			  ((size * sizeof(char *)) + (p - list) + 1));
  for (i = 0, p = ((char *) argv) + size*sizeof(char *); *list != 0; i++) {
    result = ecs_FindElement(list, &element, &list, &elSize, &brace);
    if (result != TRUE) {
      free((char *) argv);
      return result;
    }
    if (*element == 0) {
      break;
    }
    if (i >= size) {
      free((char *) argv);
      return FALSE;
    }
    argv[i] = p;
    if (brace) {
      strncpy(p, element, (size_t) elSize);
      p += elSize;
      *p = 0;
      p++;
    } else {
      ecs_CopyAndCollapse(elSize, element, p);
      p += elSize+1;
    }
  }
  
  argv[i] = NULL;
  *argvPtr = argv;
  *argcPtr = i;
  return TRUE;
}
