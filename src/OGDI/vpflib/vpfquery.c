
/*************************************************************************
 *
 *N  Module VPFQUERY
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This module contains functions for querying a VPF table with a
 *     selection expression.  It has one main entry point - query_table().
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    N/A
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    May 1991   DOS Turbo C
 *                      Dec 1992   UNIX mdb port
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions:
 *F
 *    set_type query_table( char *expression, vpf_table_type table );
 *E
*************************************************************************/
#ifndef INCL_XVTH
#include <xvt.h>
#endif
#if 0
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#endif

#ifdef _MSDOS
#include <conio.h>
#endif
#ifndef __LINKLIST_H__
#include "linklist.h"
#endif
#ifndef _VPFTABLE_H_
#include "vpftable.h"
#endif
#ifndef __SET_H__
#include "set.h"
#endif
#ifndef __STRFUNC_H__
#include "strfunc.h"
#endif
#ifndef _MACHINE_
#include "machine.h"
#endif


/* Delimiter tokens */
typedef enum { EQ, NE, LE, GE, LT, GT, AND, OR, QUOTE } delim_type;
delim_type delim;

/* Valid delimeter strings */
char *delimstr[] = { "=", "<>", "<=", ">=", "<", ">", " AND ",
                     " OR ", "\"" };
int32 ndelim = 9;

char **fieldname;
int32 *fieldcol;
int32 nfields;

/* Token types */
#define DELIMETER  1
#define FIELD      2
#define VALUE      3
#define QUOTE      4
#define STRING     5
#define EOL        6
#define FINISHED   7
#define LOP        8   /* Logical Operator */
#define JOIN       9
#define ERRORTOKEN     10

#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif


typedef struct
   {
   int32 field;
   char op;
   char value[255];
   char join;
   } expr_type;

#ifdef PROTO
void *memalloc (size_t size)
#else
void *memalloc (size)
size_t size;
#endif
   {
   void *ptr;

   ptr = (void*)xvt_malloc (size);
   if (!ptr)
      {
      xvt_note ("Memory allocation error in VPFQUERY\n");
      exit(1);
      }
   return ptr;
   }

#ifdef PROTO
int32 is_white( char c )
#else
int32 is_white( c )
char c;
#endif
   {
   if (c==' ' || c=='\t')
      return 1;
   return 0;
   }


/*************************************************************************
 *
 *N  return_token
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the first token string found in the
 *     expression string.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    expr   <input>==(char *) selection expression string.
 *    token <output>==(char *) first token in the string.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    May 1991                          DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
*************************************************************************/
#ifdef PROTO
void return_token (char *expr, char *token)
#else
void return_token ( expr, token)
char *expr;
char *token;
#endif

   {
   register int32 i, j, n, found=0, stopflag;

   n = 0;
   stopflag=0;
   while (expr[0] == ' ')
      {
      for (i=0; i<ndelim; i++)
#ifdef _MAC
         if (strncmpi (expr, delimstr[i], strlen (delimstr[i])) == 0)
#elif defined(_WINDOWS)
         if (strnicmp (expr, delimstr[i], strlen (delimstr[i])) == 0)
#else
         if (strncasecmp (expr, delimstr[i], strlen (delimstr[i])) == 0)
#endif
            {
            stopflag=1;
            break;
            }
      if (stopflag)
         break;
      expr++;
      }
   strcpy (token, expr);
   for (i=0; i< (int32)strlen(token); i++)
      {
      for (j=0; j<ndelim; j++)
         {
#ifdef _MAC
         if (strncmpi (expr, delimstr[j], strlen (delimstr[j])) == 0)
            {
#elif defined(_WINDOWS)
         if (strnicmp (expr, delimstr[j], strlen (delimstr[j])) == 0) 
	   {
#else
         if (strncasecmp (expr, delimstr[j], strlen (delimstr[j])) == 0)
	   {
#endif
            if (n > 0)
               token[i] = '\0';
            else
               token[strlen (delimstr[j])] = '\0';
            found = 1;
            break;
            }
         }
      if ((found) || (!is_white(*expr)))
         n++;
      if ((!found) && (*expr))
         expr++;
      if (found)
         break;
      }
   }



/*************************************************************************
 *
 *N  get_token
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function gets the first token, token type, and token value of
 *     the expression string, and then advances the expression string
 *     past the token.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    expression   <input>==(char *) selection expression string.
 *    token       <output>==(char *) first token in the string.
 *    token_type  <output>==(int32 *) token type.
 *    token_value <output>==(int32 *) token_value.
 *    return      <output>==(char *) new selection expression.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    May 1991                          DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    void return_token( char *expr, char *token ) VPFQUERY.C
 *E
 *************************************************************************/
#ifdef PROTO
char *get_token (char *expression, char *token, int32 *token_type,
                 int32 *token_value)
#else
char *get_token ( expression, token, token_type, token_value)
char *expression;
char *token;
int32 *token_type;
int32 *token_value;
#endif

   {
   register int32 i, stopflag;

   *token_type = 0;

   /* Test for no expression (NULL) */
   if (*expression == '\0')
      {
      *token_type = FINISHED;
      *token_value = 0;
      return expression;
      }

   /* Test for "control/linefeed" (\r) delimiter */
   if (*expression == '\r')
      {
      ++expression;
      ++expression;
      *token = '\r';
      token[1] = '\n';
      token[2] = 0;
      *token_type = DELIMETER;
      }

   stopflag = 0;

   while ((expression[0] == '\"') || (expression[0] == ' '))
      {
      /* Test for any of the expression delimiters */
      for (i=0; i<ndelim; i++)

         if (Mstrncmpi (expression, delimstr[i], strlen (delimstr[i])) == 0)
            {
            stopflag = 1;
            break;
            }
      if (stopflag)
         break;
      expression++;
      }

   return_token (expression, token);
   expression += strlen(token);

   if (*token == '\0')
      {
      *token_type = FINISHED;
      *expression = '\0';
      return expression;
      }

   leftjust (token);
   rightjust (token);

   /* Test for logical operator "AND" */
   if ((strcmp (token, "AND") == 0) || (strcmp (token, "and") == 0))
      {
      strupr (token);
      *token_type = JOIN;
      *token_value = AND;
      while ((expression[0] == '\"') || (expression[0] == ' '))
         expression++;
      return expression;
      }


   /* Test for logical operator "OR" */
   if ((strcmp (token, "OR") == 0) || (strcmp (token, "or") == 0))
      {
      strupr (token);
      *token_type = JOIN;
      *token_value = OR;
      while ((expression[0] == '\"') || (expression[0] == ' '))
         expression++;
      return expression;
      }


   /* Test for quoted strings */
   if (token[0] == '"')
      {
/* 
See http://sourceforge.net/bugs/?func=detailbug&bug_id=122597&group_id=11181 

      if (*expression)
          expression++;
*/

      i = 0;
      while (*expression != '"')
         {
         token[i] = *expression;
         i++;
         expression++;
         if (*expression == '\0')
            {
            *token_type = ERRORTOKEN;
            *token_value = ERRORTOKEN;
            return expression;
            }
         }

      while ((expression[0] == '\"') || (expression[0] == ' '))
         expression++;
      token[i] = '\0';
      *token_type = STRING;
      *token_value = strlen (token);
      return expression;
      }


   /* Test for logical operators */
   for (i=0; i<ndelim; i++)
      {
      if (Mstrcmpi (token, delimstr[i]) == 0)
         {
         *token_type = LOP;
         *token_value = i;
         return expression;
         }
      }

   /* Test for matching fieldnames */
   for (i=0; i<nfields; i++)
      {
      if (Mstrcmpi (token, fieldname[i]) == 0)
         {
         strupr(token);
         *token_type = FIELD;
         *token_value = i;
         return expression;
         }
      }

   *token_type = VALUE;
   *token_value = '\0';

   return expression;
   }


/*************************************************************************
 *
 *N  parse_expression
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns a list of selection expression clause
 *     structures.  This list forms the internal structure of the query
 *     expression.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    expression   <input>==(char *) selection expression string.
 *    table        <input>==(vpf_table_type) VPF table structure.
 *    return      <output>==(linked_list_type) list of expression clauses.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    May 1991                          DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    char *get_token( char *expression, char *token, int32 *token_type,
 *    int32 *token_value) VPFQUERY.C
 *    void display_message( char *input) USER DEFINED
 *    linked_list_type ll_init() LINKLIST.C
 *    void ll_reset( linked_list_type list ) LINKLIST.C
 *    void ll_insert( void *element, unsigned size,
 *    position_type position ) LINKLIST.C
 *E
 *************************************************************************/
#ifdef PROTO
linked_list_type parse_expression (char *expression, vpf_table_type table)
#else
linked_list_type parse_expression (expression, table)
char *expression;
vpf_table_type table;
#endif
   {
   linked_list_type exprlist;
   position_type pos;
   expr_type expr;
   int32 i, token_type, token_value;
   char token[260];
   char *orig_expression;


   orig_expression = (char*)xvt_zmalloc (strlen (expression) + 1); 
   strcpy (orig_expression, expression);

   exprlist = ll_init();
   pos = exprlist;

   /* Set up globals */

   nfields = (int32)table.nfields;

   fieldname = (char**)memalloc ((size_t)(nfields+2) * sizeof (char*));
   fieldcol = (int32*)memalloc ((size_t)(nfields+2) * sizeof(int32));

   for (i=0; i<table.nfields; i++)
      {
      fieldname[i] = (char*)memalloc (40 * sizeof (char));
      strcpy (fieldname[i], table.header[i].name);
      fieldcol[i] = i;
      }

   /*****/
   /* Find the "field" token to the left of the logical operator */
   expression = get_token (expression, token, &token_type, &token_value);

   while (token_type != FINISHED)
      {
      if (token_type != FIELD)
         {
         xvt_note ("Expression syntax error %s\n",orig_expression);
         ll_reset(exprlist);
         exprlist = NULL;
         break;
         }
      expr.field = token_value;

      /* Find the logical operator token in the expression */
      expression = get_token (expression, token, &token_type, &token_value);
      if (token_type != LOP)
         {
         xvt_note ("Expression syntax error %s\n",orig_expression);
         ll_reset(exprlist);
         exprlist = NULL;
         break;
         }
      expr.op = (char)token_value;

      /* Find the "value" token to the right of the logical operator in the expression */
      expression = get_token (expression, token, &token_type, &token_value);
      if (token_type == ERRORTOKEN)
         {
         xvt_note ("Expression syntax error %s\n",orig_expression);
         ll_reset(exprlist);
         exprlist = NULL;
         break;
         }
      strcpy(expr.value,token);

      /* Check for a "join" token to determine if the expression is a compound expression */
      expression = get_token (expression, token, &token_type, &token_value);
      if (token_type == JOIN)
         {
         expr.join = (char)token_value;
         ll_insert (&expr, sizeof (expr), pos);
         pos = pos->next;
         expression = get_token (expression, token, &token_type,
                                                                &token_value);
         }
      else if (token_type == FINISHED)
         {
         expr.join = '\0';
         ll_insert (&expr, sizeof(expr), pos);
         }
      else
         {
         xvt_note ("Expression syntax error %s\n",orig_expression);
         ll_reset (exprlist);
         exprlist = NULL;
         break;
         }
      }

   for (i=0; i<nfields; i++)
      xvt_free(fieldname[i]);

   xvt_free ((char*)fieldname);
   xvt_free ((char*)fieldcol);
   xvt_free(orig_expression);

   return exprlist;
   }



/*************************************************************************
 *
 *N  bufcomp
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function is a general comparison function for comparing two
 *     buffers.  NOTE:  This function compares the bytes of the buffers
 *     as unsigned characters.  Numeric values > 255 should not be
 *     compared with this function.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    val1    <input>==(void *) first buffer to compare.
 *    val2    <input>==(void *) second buffer to compare.
 *    size    <input>==(int32) number of bytes to compare.
 *    op      <input>==(char) logical operator.
 *    return <output>==(int32) TRUE or FALSE.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    May 1991                          DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *************************************************************************/
#ifdef PROTO
int32 bufcomp( void *val1, void *val2, int32 size, char op )
#else
int32 bufcomp( val1, val2, size, op )
void *val1;
void *val2;
int32 size;
char op;
#endif
{
   int32 result;

   result = (int32)memcmp (val1, val2, (size_t)size);

   switch (op) {
      case EQ:
         result = !result;
         break;
      case NE:
         break;
      case LT:
         if (result < 0)
            result = TRUE;
         else
            result = FALSE;
         break;
      case LE:
         if (result <= 0)
            result = TRUE;
         else
            result = FALSE;
         break;
      case GT:
         if (result > 0)
            result = TRUE;
         else
            result = FALSE;
         break;
      case GE:
         if (result >= 0)
            result = TRUE;
         else
            result = FALSE;
         break;
      default:
         xvt_note ("Invalid logical operator (%d)\n",op);
         result = FALSE;
         break;
   }
   return result;
}


/*************************************************************************
 *
 *N  strcompare
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function compares two strings with the given logical operator.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    val1    <input>==(char *) first buffer to compare.
 *    val2    <input>==(char *) second buffer to compare.
 *    op      <input>==(char) logical operator.
 *    return <output>==(int32) TRUE or FALSE.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    May 1991                          DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *************************************************************************/
#ifdef PROTO
int32 strcompare( char *val1, char *val2, char op )
#else
int32 strcompare( val1, val2, op )
char *val1;
char *val2;
char op;
#endif
{
   int32 result;
   char str1[300], str2[300];

   strcpy(str1,val1);
   rightjust(str1);
   strcpy(str2,val2);
   rightjust(val2);

   result = Mstrcmpi (str1, str2);

   switch (op) {
      case EQ:
         result = !result;
         break;
      case NE:
         break;
      case LT:
         if (result < 0)
            result = TRUE;
         else
            result = FALSE;
         break;
      case LE:
         if (result <= 0)
            result = TRUE;
         else
            result = FALSE;
         break;
      case GT:
         if (result > 0)
            result = TRUE;
         else
            result = FALSE;
         break;
      case GE:
         if (result >= 0)
            result = TRUE;
         else
            result = FALSE;
         break;
      default:
         xvt_note ("Invalid logical operator (%d)\n",op);
         result = FALSE;
         break;
   }
   return result;
}


/*************************************************************************
 *
 *N  icompare
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function compares two int32 integers with the given logical
 *     operator.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    val1    <input>==(int32 int) first buffer to compare.
 *    val2    <input>==(int32 int) second buffer to compare.
 *    op      <input>==(char) logical operator.
 *    return <output>==(int32) TRUE or FALSE.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    May 1991                          DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *************************************************************************/
#ifdef PROTO
int32 icompare( int32 val1, int32 val2, char op )
#else
int32 icompare(val1, val2, op )
int32 val1;
int32 val2;
char op;
#endif
{
   int32 result;

   switch (op) {
      case EQ:
         result = (val1 == val2);
         break;
      case NE:
         result = (val1 != val2);
         break;
      case LT:
         result = (val1 < val2);
         break;
      case LE:
         result = (val1 <= val2);
         break;
      case GT:
         result = (val1 > val2);
         break;
      case GE:
         result = (val1 >= val2);
         break;
      default:
         xvt_note ("Invalid logical operator (%d)\n",op);
         result = FALSE;
         break;
   }
   return result;
}


/*************************************************************************
 *
 *N  fcompare
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function compares two floating point numbers with the given
 *     logical operator.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    val1    <input>==(int32) first buffer to compare.
 *    val2    <input>==(int32) second buffer to compare.
 *    op      <input>==(char) logical operator.
 *    return <output>==(int32) TRUE or FALSE.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    May 1991                          DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    None
 *E
 *************************************************************************/
#ifdef PROTO
int32 fcompare( float val1, float val2, char op )
#else
int32 fcompare( val1, val2, op )
float val1;
float val2;
char op;
#endif
{
   int32 result;

   switch (op) {
      case EQ:
         result = (val1 == val2);
         break;
      case NE:
         result = (val1 != val2);
         break;
      case LT:
         result = (val1 < val2);
         break;
      case LE:
         result = (val1 <= val2);
         break;
      case GT:
         result = (val1 > val2);
         break;
      case GE:
         result = (val1 >= val2);
         break;
      default:
         xvt_note ("Invalid logical operator (%d)\n",op);
         result = FALSE;
         break;
   }
   return result;
}


/*************************************************************************
 *
 *N  query_table
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the set of selected rows of a VPF table
 *     based upon the evaluation of the given selection expression string.
 *
 *     The expression is strictly evaluated left to right.  No nesting
 *     is supported, so parentheses are not allowed.  The expression
 *     must match the form:
 *        <field><log op><value> [ <join> <field><log op><value>]
 *     where,
 *        <field> is a valid field name of the table.
 *        <log op> is one of the following: =, <, >, <=, >=, <> (not equal).
 *        <value> is a valid value for the field.
 *        <join> is either " AND " or " OR ".
 *     Any number of clauses (<field><log op><value>) may be joined
 *     together with AND or OR to form the expression.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    expression <input>==(char *) selection expression string.
 *    table      <input>==(vpf_table_type) VPF table structure.
 *    return    <output>==(set_type) set of selected rows.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    May 1991                          DOS Turbo C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    set_type set_init (int32 n) SET.C
 *    void set_insert (int32 element, set_type set) SET.C
 *    linked_list_type parse_expression (char *expression,
 *                                       vpf_table_type table) VPFQUERY.C
 *    row_type read_next_row (vpf_table_type table) VPFREAD.C
 *    position_type ll_first (linked_list_type list) LINKLIST.C
 *    int32 ll_end (position_type position) LINKLIST.C
 *    void ll_element (position_type position, void *element) LINKLIST.C
 *    void *get_table_element (int32 field_number, row_type row,
 *                             vpf_table_type table, void *value,
 *                             int32 *count) VPFREAD.C
 *    void display_message (char *info) USER DEFINED
 *    int32 strcompare (char *val1, char *val2, char op) VPFQUERY.C
 *    int32 icompare (int32 val1, int32 val2, char op) VPFQUERY.C
 *    int32 fcompare (float val1, float val2, char op) VPFQUERY.C
 *    void ll_reset (linked_list_type list) LINKLIST.C
 *    void free_row (row_type row, vpf_table_type table) VPFREAD.C
 *E
 *************************************************************************/
#ifdef PROTO
set_type query_table( char *expression, vpf_table_type table )
#else
set_type query_table( expression, table )
char *expression;
vpf_table_type table;
#endif
{
   row_type row;
   position_type pos;
   expr_type expr;
   register int32 i;
   int32 boolval=FALSE, booltemp=FALSE, join = OR;
   int32 lval, lval2, count;
   short sval,sval2;
   float fval, fval2;
   char tval, tval2, *tptr;
   linked_list_type exprlist;
   set_type select_set;
   int32 ipos;

#if 0 /* DGM */
   select_set = set_init(table.nrows+1);
#endif
   select_set = set_init (table.nrows);

   /* If the expression is "*" then just turn on all members of select_set */
   /* and return                                                           */
   if (strcmp (expression, "*") == 0)
      {
      set_on(select_set);
      return select_set;
      }

   exprlist = parse_expression( expression, table );

   if (!exprlist) return select_set;

   if (table.storage == DISK)
      {
      ipos = index_pos (1L, table);
      fseek( table.fp, ipos, SEEK_SET );
      }

   for (i=1;i<=table.nrows;i++) {

      if (table.storage == DISK)
         row = read_next_row(table);
      else
         row = get_row( i, table );

      pos = ll_first(exprlist);
      while (!ll_end(pos)) {
         ll_element( pos, &expr );
         switch (table.header[expr.field].type) {
            case 'I':
               if (table.header[expr.field].count == 1) {
                  get_table_element( expr.field, row, table, &lval, &count );
                  lval2 = atol(expr.value);
                  booltemp = icompare( lval, lval2, expr.op );
               } else {
                   xvt_note ("Selection may not be performed upon arrays");
               }
               break;
            case 'S':
               if (table.header[expr.field].count == 1) {
                  get_table_element( expr.field, row, table, &sval, &count );
                  sval2 = (short)atoi (expr.value);
                  booltemp = icompare( (int32)sval, (int32)sval2,
                                       expr.op );
               } else {
                   xvt_note ("Selection may not be performed upon arrays");
               }
               break;
            case 'T':
            case 'L':
               if (table.header[expr.field].count == 1) {
                  get_table_element( expr.field, row, table, &tval, &count );
                  tval2 = expr.value[0];
                  booltemp = bufcomp( &tval, &tval2, sizeof(tval), expr.op );
               } else {
                  tptr = (char *)get_table_element( expr.field, row, table,
                                   NULL, &count );
                  booltemp = strcompare( tptr, expr.value, expr.op );
                  xvt_free(tptr);
               }
               break;
            case 'F':
               if (table.header[expr.field].count == 1) {
                  get_table_element( expr.field, row, table, &fval, &count );
                  fval2 = (float)atof (expr.value);
                  booltemp = fcompare( fval, fval2, expr.op );
               } else {
                   xvt_note ("Selection may not be performed upon arrays");
               }
               break;
            default:
               xvt_note ("Field type not supported for query\n");
               break;
         }

         if (join==OR)
            boolval = boolval || booltemp;
         else
            boolval = boolval && booltemp;

         join = expr.join;

         pos = pos->next;
      }
      free_row( row, table );
      if (boolval) set_insert(i,select_set);
      boolval = FALSE;
      join = OR;

   }

   ll_reset(exprlist);

   return select_set;
}
 
/*************************************************************************
 *
 *N  query_table_row
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns 1 if the given row matches
 *     the given selection expression string, 0 otherwise.
 *
 *     The expression is strictly evaluated left to right.  No nesting
 *     is supported, so parentheses are not allowed.  The expression
 *     must match the form:
 *        <field><log op><value> [ <join> <field><log op><value>]
 *     where,
 *        <field> is a valid field name of the table.
 *        <log op> is one of the following: =, <, >, <=, >=, <> (not equal).
 *        <value> is a valid value for the field.
 *        <join> is either " AND " or " OR ".
 *     Any number of clauses (<field><log op><value>) may be joined
 *     together with AND or OR to form the expression.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    expression <input>==(char *) selection expression string.
 *    row        <input>==(row_type) VPF row structure.
 *    return     <output>==(int) 1 if row matches expression,
 *                               0 otherwise.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    June 1993                          VPFVIEW DOS 1.1
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *************************************************************************/
#ifdef PROTO
int32 query_table_row (char *expression, row_type row, vpf_table_type table)
#else
int32 query_table_row (expression, row, table)
char *expression;
row_type row;
vpf_table_type table;
#endif
{
   position_type pos;
   expr_type expr;
   int32 boolval=FALSE, booltemp=FALSE, join = OR;
   int32  lval, lval2, count;
   short sval, sval2;
   float fval, fval2;
   char tval, tval2, *tptr;
   linked_list_type exprlist;

   if (strcmp(expression,"*")==0) {
      return 1;
   }

   exprlist = parse_expression( expression, table );

   if (!exprlist) return 0;

   pos = ll_first(exprlist);
   while (!ll_end(pos)) {
      ll_element( pos, &expr );
      switch (table.header[expr.field].type) {
	  case 'I':
	     if (table.header[expr.field].count == 1)
           {
	        get_table_element( expr.field, row, table, &lval, &count );
	        lval2 = atol(expr.value);
	        booltemp = icompare( lval, lval2, expr.op );
	        }
        else
           {
	     }
	     break;
	  case 'S':
	     if (table.header[expr.field].count == 1)
           {
	        get_table_element( expr.field, row, table, &sval, &count );
	        sval2 = atoi(expr.value);
	        booltemp = icompare( (int32)sval, (int32)sval2, expr.op );
	        }
        else
           {
	        }
	     break;
	  case 'T':
      case 'L':
	     if (table.header[expr.field].count == 1)
           {
	        get_table_element( expr.field, row, table, &tval, &count );
	        tval2 = expr.value[0];
	        booltemp = bufcomp( &tval, &tval2, sizeof(tval), expr.op );
	        }
        else
           {
	        tptr = (char *)get_table_element( expr.field, row, table,  NULL, &count );
           booltemp = strcompare( tptr, expr.value, expr.op );
	        xvt_free(tptr);
	        }
	     break;
	  case 'F':
	     if (table.header[expr.field].count == 1)
           {
		     get_table_element( expr.field, row, table, &fval, &count );
		     if (!is_vpf_null_float(fval))
              {
		        fval2 = (float) atof(expr.value);
		        booltemp = fcompare( fval, fval2, expr.op );
		        }
           else
              booltemp = FALSE;
	        }
        else
           {
	        }
	     break;
	  default:
	     break;
      }

      if (join==OR)
         boolval = boolval || booltemp;
      else
         boolval = boolval && booltemp;

      join = expr.join;

      pos = pos->next;
   }

   ll_reset(exprlist);

   return boolval;
}
