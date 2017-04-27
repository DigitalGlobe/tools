/******************************************************************************
 *
 * Component: OGDI VRF Driver
 * Purpose: Various VRF supporting functions.
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
 * $Log: utils.c,v $
 * Revision 1.21  2016/07/07 15:50:15  erouault
 * fix stack buffer overflow in vrf_GetMetadata() when reading the level in CAT files. Found by GCC 5.2 -faddress=sanitize
 *
 * Revision 1.20  2016/07/04 17:03:12  erouault
 * Error handling: Add a ecs_SetErrorShouldStop() function that can be
 *     used internally when the code is able to recover from an error. The user
 *     may decide if he wants to be resilient on errors by defining OGDI_STOP_ON_ERROR=NO
 *     as environment variable (the default being YES: stop on error).
 *     Add a ecs_SetReportErrorFunction() method to install a custom callback that
 *     will be called when OGDI_STOP_ON_ERROR=YES so that the user code is still
 *     aware of errors that occured. If not defined, the error will be logged in stderr.
 *
 * Revision 1.19  2016/07/04 12:49:56  erouault
 * VPF: Avoid a missing fcs file in a coverage to prevent opening any coverage of the library (fix opening of DNC17/COA17A dataset)
 *
 * Revision 1.18  2008/05/28 00:18:21  cbalint
 *    * fix minor printf format gcc warnings.
 *
 * Revision 1.17  2007/05/09 20:46:28  cbalint
 * From: Even Rouault <even.rouault@mines-paris.org>
 * Date: Friday 21:14:18
 *
 *         * fix filename case sensitivy problems (for Unix-like systems).
 *
 *         * fix incorrect use of sprintf in vrf_GetMetadata.
 *
 *         * report wgs84 instead of nad83, not sure whether that is true
 *         for all VPF products, but at least it's correct for VMAP products
 *         that *must* be WGS84. A better fix would be to read the VPF table
 *         that contains this information.
 *
 *         * fix a few minor memory leaks and memory usage issues.
 *
 *         * enable XMIN, YMIN, XMAX and YMAX columns to be of type double
 *         in EBR and FBR files (for read the VMAP2i 'MIG2i000' product).
 *
 *         * add .pjt and .tjt as possible extensions for join tables
 *         (VMAP2i 'MIG2i000' product).
 *
 *         * fix duplicated layers report (VMAP2i 'MIG2i000' product).
 *
 *         * handle 'L' (Latin1) type for text files (GEOCAPI 'MIGxxx' products).
 *
 *         * optionnaly, convert text to UTF-8 when environment variable
 *         CONVERT_OGDI_TXT_TO_UTF8 is defined. This part is not portable
 *         on Windows I guess (only tested on Linux) and maybe too specific.
 *
 *         * enable reading of VPF products without table indexes file
 *         (GEOCAPI 'MIG013' and 'MIG016' products). VPF norm says that when
 *         there is a variable length field in one table, an index should exist,
 *         but some test products don't follow this. The approach here is to read
 *         the whole table content and build the index in memory.
 *
 *  Modified Files:
 *  	ChangeLog ogdi/driver/vrf/feature.c ogdi/driver/vrf/object.c
 *  	ogdi/driver/vrf/utils.c ogdi/driver/vrf/vrf.c
 *  	ogdi/driver/vrf/vrfswq.c vpflib/musedir.c vpflib/strfunc.c
 *  	vpflib/vpfbrows.c vpflib/vpfprop.c vpflib/vpfquery.c
 *  	vpflib/vpfread.c vpflib/vpftable.c
 *
 * Revision 1.16  2004/10/26 19:57:36  warmerda
 * Fixed problem where "reg" regular expression was freed, but the change
 * was not recognised since compiled was not being reset.  Got rid of
 * compiled flag entirely.
 *
 * Revision 1.15  2004/10/25 19:34:31  warmerda
 * The Level "buffint" should be short, not int, when forming the metadata.
 *
 * Revision 1.14  2004/02/18 21:33:18  warmerda
 * free regex memory
 *
 * Revision 1.13  2003/05/21 18:47:31  warmerda
 * initialize spriv->tile[0].path in untiled (VITD) case
 *
 * Revision 1.12  2001/08/16 19:59:08  warmerda
 * partially rewrite vrf_build_coverage_capabilities to avoid repeating entries
 *
 * Revision 1.11  2001/07/05 14:16:06  warmerda
 * fixed vrf_GetMetadata error duplicating first layer in a class, bug 111181
 *
 * Revision 1.10  2001/06/29 19:17:00  warmerda
 * fixed unterminated 'temp' string
 *
 * Revision 1.9  2001/06/21 20:30:15  warmerda
 * added ECS_CVSID
 *
 * Revision 1.8  2001/06/13 17:33:59  warmerda
 * upgraded source headers
 *
 */

#include <stdarg.h>
#include "ecs.h"
#include "vrf.h"
#include "vpfprop.h"

ECS_CVSID("$Id: utils.c,v 1.21 2016/07/07 15:50:15 erouault Exp $");

#ifdef _WINDOWS
#define SEPARATOR '\\'
#else
#define SEPARATOR '/'
#endif

/* ----------------------------------------------------------------------
 *  vrf_parsePath: 
 *     
 *   decomposition du pathname en database, location et mapset
 * ----------------------------------------------------------------------
 */

#define SYNTAXERRORMESSAGE "Badly formed pathname: %s, must be fclass@coverage(expression)"

int vrf_parsePath(s,lpriv,sel)
     ecs_Server *s;
     LayerPrivateData *lpriv;
     ecs_LayerSelection *sel;
{
  return vrf_parsePathValue(s,sel->Select,&(lpriv->fclass),&(lpriv->coverage),&(lpriv->expression));
}

/* ----------------------------------------------------------------------
 *  vrf_parsePathValues: 
 *     
 *   decomposition du pathname en database, location et mapset
 * ----------------------------------------------------------------------
 */

static ecs_regexp *reg = NULL;

int vrf_parsePathValue(s,request,fclass,coverage,expression)
     ecs_Server *s;
     char *request;
     char **fclass;
     char **coverage;
     char **expression;
{
  char buffer[512],*temp;
  int i,pos;

  /* Found the first "(" */

  pos = 0;
  for(i=0;i<(int) strlen(request);i++) {
    if (request[i] == '(') {
      pos = i;
      break;
    }
  }

  temp = malloc(pos+1);
  if (temp == NULL) {
    ecs_SetError(&(s->result),1,"not enough memory");
    return 0;
  }
  strncpy(temp,request,pos);
  temp[pos] = '\0';

  if ((int) strlen(request) > pos) {
    *expression = malloc(strlen(request)-pos+1);
    if (*expression == NULL) {
      free(temp);
      ecs_SetError(&(s->result),1,"not enough memory");
      return 0;
    }
    strncpy(*expression,request+pos+1,strlen(request)-pos-2);
    (*expression)[strlen(request)-pos-2] = '\0';
  } else {
    free(temp);
    ecs_SetError(&(s->result),1,"no expressions set in this request");
    return 0;
  }

  if ( reg == NULL ) {
    reg = EcsRegComp("(.*)@(.*)");
  }

  if (!EcsRegExec(reg,temp,NULL)) {
    sprintf(buffer,SYNTAXERRORMESSAGE,request);
    ecs_SetError(&(s->result),1,buffer);
    free(temp);
    free(*expression);
    return 0;
  }

  if (!ecs_GetRegex(reg,1,fclass)) {
    ecs_SetError(&(s->result),1,"Not enough memory to allocate server");  
    free(temp);
    free(*expression);
    return 0;
  }

  if (strlen(*fclass) == 0) {
    sprintf(buffer,SYNTAXERRORMESSAGE,s->pathname);
    ecs_SetError(&(s->result),1,buffer);
    free(temp);
    free(*expression);
    return 0;
  }


  if (!ecs_GetRegex(reg,2,coverage)) {
    ecs_SetError(&(s->result),1,"Not enough memory to allocate server");  
    free(temp);
    free(*expression);
    return 0;
  }

  if (strlen(*coverage) == 0) {
    sprintf(buffer,SYNTAXERRORMESSAGE,s->pathname);
    ecs_SetError(&(s->result),1,buffer);
    free(temp);
    free(*expression);
    return 0;
  }

  free(temp);
  return 1;
}

/* ----------------------------------------------------------------------
 *  vrf_freePathRegex()
 *     
 *  free resources related to regex path checker.
 * ----------------------------------------------------------------------
 */

void vrf_freePathRegex()

{
    if( reg != NULL )
    {
        free( reg );
        reg = NULL;
    }
}

/* 
********************************************************************

FUNCTION_INFORMATION

NAME 
vrf_getFileNameFromFcs

DESCRIPTION 

    Extract from the FCS the important information needed
    by a given layer.

END_DESCRIPTION

PARAMETERS 

    INPUT 
    ecs_Server *s: Server info given by OGDI API 
    ecs_Layer *lpriv: The current layer private information

END_PARAMETERS

END_FUNCTION_INFORMATION

PSEUDO-CODE

Check with muse access see if in the coverage there is a FCS.  If it
exist, open it in fcsTable.

For each row in the fcs table 
Begin

    Get the current row in the table.

    If the first element of the row get the same name than the current
    feature class 
    Begin

        Set the variable found to 1

        Check if a join table exist and get it's name. Set isJointed
        to TRUE if one is found for this coverage.

        Get the feature table name attribute in the row (position 2).
        Set this value in lpriv->featureTableName.

        Get the primitive table name attribute in the row (position
        4).  Set this value in lpriv->primitiveTableName.

        If there is a joint table 
	Begin

            If the primitive table name is a join table.  
	    Begin

                Clear from the memory the lpriv->primitiveTableName.

                Get the join table name attribute in the row (position
                4).  Set this value in lpriv->joinTableName.

                Get the join table foreign key name attribute in the
                row (position 3).  Set this value in
                lpriv->joinTableForeignKeyName.

                Get the join table feature id name attribute in the
                row (position 5).  Set this value in
                lpriv->joinTableFeatureIdName.

                Check the remaining rows in the fcs table one by one
                Begin

                    Set buf2 at the second position of the row.

                    Set buf3 at the fourth position of the row.

                    If buf2 contain the joinTableName and buf3 don't
                    contain the featureTableName Begin

                        Get the primitive table name attribute in the
                        row (position 4).  Set this value in
                        lpriv->primitiveTableName.

                        Get the join table primitive id name attribute
                        in the row (position 3).  Set this value in
                        lpriv->featureTablePrimIdName.

                    End

                    Free buf2

                    Free buf3

                End

            End 
	    Else 
	    Begin

                // No links are define between the join table and the
                // feature table in fcs. We suppose a relation 1:1.

                Set the lpriv->joinTableName with the join filename
                found previously.

                Set lpriv->joinTableForeignKeyName to NULL.

                Get the feature table primitive id name attribute in
                the row (position 3).  Set this value in
                lpriv->featureTablePrimIdName.

            End

        End 
	Else 
	Begin

            // There is no join table

            Set the lpriv->joinTableName to NULL.

            Set lpriv->joinTableForeignKeyName to NULL.

            Get the feature table primitive id name attribute in the
            row (position 3).  Set this value in
            lpriv->featureTablePrimIdName.

        End

    End

    Free the row

End

Close the fcs table

Return a success message

******************************************************************** 
*/

int 
vrf_getFileNameFromFcs(s,lpriv)
     ecs_Server *s;
     LayerPrivateData *lpriv;
{
  char buffer[512];
  char *buf1;
  char *buf2;
  char *buf3;
  int count;
  int i,j;
  int found  = 0;
  row_type row, row2;
  char code;
  register ServerPrivateData *spriv = s->priv;
  char tempfilename[100];
  int isJointed;
  int feature_class_pos, table1_pos, table1_key_pos, table2_pos, table2_key_pos;
  static const char* extJointTables[] = { ".pjt", ".ajt", ".ljt", ".rjt", ".njt", ".tjt"};

  sprintf(buffer,"%s/%s/fcs",spriv->library,lpriv->coverage);
  if (muse_access(buffer,0) != 0) {
    sprintf(buffer,"%s/%s/FCS",spriv->library,lpriv->coverage);
    if (muse_access(buffer,0) != 0) {
      ecs_SetError(&(s->result),1,"Can't open the FCS table, invalid VRF coverage");
      return 0;
    }
  }

#ifdef TESTOPENTABLE
  printf("open lpriv->fcsTable:%s\n",buffer);
#endif
  


  lpriv->fcsTable = vpf_open_table(buffer, disk, "rb", NULL);
  if (lpriv->fcsTable.path == NULL) {
    ecs_SetError(&(s->result),1,"Can't open the FCS table, invalid VRF coverage");
    return 0;

  }

  feature_class_pos = table_pos("FEATURE_CLASS", lpriv->fcsTable);
  table1_pos = table_pos("TABLE1", lpriv->fcsTable);
  table1_key_pos = table_pos("TABLE1_KEY", lpriv->fcsTable);
  table2_pos = table_pos("TABLE2", lpriv->fcsTable);
  table2_key_pos = table_pos("TABLE2_KEY", lpriv->fcsTable);
  
  for (i = 1; i <= lpriv->fcsTable.nrows && !found; ++i) {
    row = get_row(i, lpriv->fcsTable);
    buf1 = justify((char*)get_table_element(feature_class_pos, row, lpriv->fcsTable, NULL, &count));
    if (stricmp(buf1,lpriv->fclass) == 0) {
      found = 1;
      
      /* Check if a join table exist and get it's name */
      
      isJointed = FALSE;
      for(j=0;j<sizeof(extJointTables) / sizeof(char*);j++)
      {
        strcpy(tempfilename,lpriv->fclass);
        strcat(tempfilename,extJointTables[j]);
        sprintf(buffer,"%s/%s/%s",spriv->library,lpriv->coverage,tempfilename);
        if (muse_access(buffer,0) == 0)
        {
          isJointed = TRUE;
          break;
        }
      }

      /* Access information in fcs */
      
      lpriv->featureTableName = justify((char *)get_table_element(table1_pos, row,
								  lpriv->fcsTable, NULL, &count));
      lpriv->primitiveTableName = justify((char *)get_table_element(table2_pos, row,
								    lpriv->fcsTable, NULL, &count));
      code = lpriv->primitiveTableName[strlen(lpriv->primitiveTableName)-2];
      
      if (isJointed) {
	if ( (code == 'j') || (code == 'J')) {
	  free(lpriv->primitiveTableName);
      lpriv->primitiveTableName = NULL;
      lpriv->joinTableName = justify((char *)get_table_element(table2_pos, row,
								   lpriv->fcsTable, NULL, &count));
      lpriv->joinTableForeignKeyName = justify((char *)get_table_element(table1_key_pos, row,
									     lpriv->fcsTable, NULL, &count));
      lpriv->joinTableFeatureIdName = justify((char *)get_table_element(table2_key_pos, row,
									    lpriv->fcsTable, NULL, &count));
      for (j = i+1; j <= lpriv->fcsTable.nrows && lpriv->primitiveTableName == NULL; ++j) {
	    row2 = get_row(j, lpriv->fcsTable);
	    
        buf2 = justify((char*)get_table_element(table1_pos, row2, lpriv->fcsTable, NULL, &count));
        buf3 = justify((char*)get_table_element(table2_pos, row2, lpriv->fcsTable, NULL, &count));
	    if ((stricmp(buf2,lpriv->joinTableName) == 0) && (stricmp(buf3,lpriv->featureTableName) != 0)) {
	      
          lpriv->primitiveTableName = justify((char *)get_table_element(table2_pos, row2,
									    lpriv->fcsTable, NULL, &count));
          lpriv->featureTablePrimIdName = justify((char *)get_table_element(table1_key_pos, row2,
										lpriv->fcsTable, NULL, &count));
	    }
	    free(buf2);
	    free(buf3);
        free_row(row2, lpriv->fcsTable);
	  }
      if (lpriv->primitiveTableName == NULL)
      {
        fprintf(stderr, "Invalid join table structure for feature %s\n", lpriv->featureTableName);
        return 0;
      }
	} else {
	  lpriv->joinTableName = malloc(strlen(tempfilename)+1);
	  strcpy(lpriv->joinTableName,tempfilename);
	  lpriv->joinTableForeignKeyName = NULL;
	  lpriv->joinTableFeatureIdName = NULL;
	  lpriv->featureTablePrimIdName = justify((char *)get_table_element(3, row, 
									    lpriv->fcsTable, NULL, &count));	    
	}
      } else {
	lpriv->joinTableName = NULL;
	lpriv->joinTableForeignKeyName = NULL;
	lpriv->featureTablePrimIdName = justify((char *)get_table_element(3, row, 
									  lpriv->fcsTable, NULL, &count));
      }

    }
    free(buf1);
    free_row(row, lpriv->fcsTable);
  }
  
#ifdef TESTOPENTABLE
  printf("close: spriv->fcsTable\n");
#endif
  
  vpf_close_table(&(lpriv->fcsTable));

  if (!found) {
    ecs_SetError(&(s->result),1,"Can't open the FCS table, invalid VRF coverage");
    return 0;
  } else {
    return 1;
  }
  
}

/* ----------------------------------------------------------
 * vrf_VerifyCATFile:
 *
 * Verify if s->pathname is really a LAT file 
 * ---------------------------------------------------------- 
 */
   

int 
vrf_verifyCATFile(s)
     ecs_Server *s;
{
  char buffer[512];
  register ServerPrivateData *spriv = s->priv;


  /* verification code must be inserted here */

  sprintf(buffer,"%s/cat",spriv->library);
  if (muse_access(buffer,0) != 0) {
    sprintf(buffer,"%s/CAT",spriv->library);
    if (muse_access(buffer,0) != 0) {
      ecs_SetError(&(s->result),1,"Can't open CAT file, invalid VRF database");
      return 0;
    }
  }

#ifdef TESTOPENTABLE
  printf("open spriv->catTable:%s\n",buffer);
#endif
  spriv->catTable = vpf_open_table(buffer, disk, "rb", NULL);
  if (spriv->catTable.path == NULL) {
    ecs_SetError(&(s->result),1,"Can't open CAT file, invalid VRF database");
    return 0;
  }

  return 1;
}


/*  -------------------------------------------------------------------------
 *  vrf_Getmetadata:
 *
 *     preparation de la fenetre globale pour le server                    
 *  --------------------------------------------------------------------------
 */

/* fix invalid 'sprintf(x, "%s", x)' usage, which works 'fine' with -O0 compilation flag but not with -O */
static int rec_sprintf(char* str, const char* format, ...)
{
  char* temp = malloc(250000);
  int ret;
  va_list ap;
  va_start(ap, format);
  ret = vsprintf(temp, format, ap);
  va_end(ap);
  strcpy(str, temp);
  free(temp);
  return ret;
}


int
vrf_GetMetadata(s)
     ecs_Server *s;
{

  int i,j;
  char *buf1;
  char *buf2;
  char *bufname;
  char *bufdesc;
  int count;
  row_type row;
  row_type rowcat;
  row_type rowcomp;
  row_type rowfca;
  float buffloat;
  register ServerPrivateData *spriv = s->priv;
  char buffer[256];
  char tab[3][7]={"char","float","int"};
  char *covname;

  int z,k,it_pos,val_pos,des_pos,att_pos;
  char *item_buf, *att_buf, *des_buf, *tval;
  vpf_table_type     table;
  storage_type       stor = disk;
  date_type datee;
  /* row_type           row;*/
  float              fval;
  int32           n;
  int intval;
  int existtableflag;
  char separator[2]={SEPARATOR,'\0'};
  int flag;
  int test;
  
  /* int32 count; */

  buf1 = NULL;

  /* build the begining of metadatastring*/
  rec_sprintf (spriv->metadatastring,"{<Grassland>displaymetada { { CURRENT DATABASE:%s\n\nDATA HEADER TABLE(DHT):\n\n",spriv->database);


  /* code pour recuperer les valeurs de DHT */

  sprintf(buffer,"%s%sdht",spriv->database,separator);

  if (muse_access(buffer,0) ==0) {

#ifdef TESTOPENTABLE
    printf("open spriv->dhtTable:%s\n",buffer);
#endif

    spriv->dhtTable = vpf_open_table(buffer, disk, "rb", NULL);
    for (i = 1; i <= spriv->dhtTable.nrows; ++i) {
      row = get_row(i, spriv->dhtTable);

      test=table_pos("DATABASE_NAME",spriv->dhtTable);
      if (test >= 0) {
	buf1 = justify((char*)get_table_element(table_pos("DATABASE_NAME",spriv->dhtTable), row, spriv->dhtTable, NULL, &count));
	rec_sprintf(spriv->metadatastring,"%sDatabase_name: %s\n",spriv->metadatastring,buf1);
	free(buf1); 
      }

      test=table_pos("DATABASE_DESC",spriv->dhtTable);
      if (test >= 0) {
	buf1 = justify((char*)get_table_element(table_pos("DATABASE_DESC",spriv->dhtTable), row, spriv->dhtTable, NULL, &count));
	rec_sprintf(spriv->metadatastring,"%sDatabase_description: %s\n",spriv->metadatastring,buf1);
	free(buf1);
      }

      test=table_pos("MEDIA_STANDARD",spriv->dhtTable);
      if (test >= 0) {
	buf1 = justify((char*)get_table_element(table_pos("MEDIA_STANDARD",spriv->dhtTable), row, spriv->dhtTable, NULL, &count));
	rec_sprintf(spriv->metadatastring,"%sMedia_Standard: %s\n",spriv->metadatastring,buf1);
	free(buf1);	
      }

      test=table_pos("ORIGINATOR",spriv->dhtTable);
      if (test >= 0) {
	buf1 = justify((char*)get_table_element(table_pos("ORIGINATOR",spriv->dhtTable), row, spriv->dhtTable, NULL, &count));
	rec_sprintf(spriv->metadatastring,"%sOriginator: %s\n",spriv->metadatastring,buf1);
	free(buf1);
      }

      test=table_pos("ADDRESSEE",spriv->dhtTable);
      if (test >= 0) {
	buf1 = justify((char*)get_table_element(table_pos("ADDRESSEE",spriv->dhtTable), row, spriv->dhtTable, NULL, &count));
	rec_sprintf(spriv->metadatastring,"%sAddressee: %s\n",spriv->metadatastring,buf1);
	free(buf1);
      }


      test=table_pos("DOWNGRADE_DATE",spriv->dhtTable);
      if (test >= 0) {
	get_table_element(table_pos("DOWNGRADE_DATE",spriv->dhtTable), row, spriv->dhtTable, (void *)&datee, &count);
	rec_sprintf(spriv->metadatastring,"%sDowngrade_date: %s\n",spriv->metadatastring,datee);
      }

      test=table_pos("RELEASABILITY",spriv->dhtTable);
      if (test >= 0) {
	buf1 = justify((char*)get_table_element(table_pos("RELEASABILITY",spriv->dhtTable), row, spriv->dhtTable, NULL, &count));
	rec_sprintf(spriv->metadatastring,"%sReleasability: %s\n",spriv->metadatastring,buf1);
	free(buf1);	
      }

      test = table_pos("OTHER_STD_NAME", spriv->dhtTable);
      if (test >= 0) {
	buf1 = justify((char*)get_table_element(table_pos("OTHER_STD_NAME", spriv->dhtTable), row, spriv->dhtTable, NULL, &count));
	rec_sprintf(spriv->metadatastring,"%sOther_STD_name: %s\n",spriv->metadatastring,buf1);
	free(buf1);	
      }

	
      test = table_pos("OTHER_STD_DATE",spriv->dhtTable);
      if (test >= 0) {
	get_table_element(table_pos("OTHER_STD_DATE",spriv->dhtTable), row, spriv->dhtTable, (void *)&datee, &count);
	rec_sprintf(spriv->metadatastring,"%sOther_std_date: %s\n",spriv->metadatastring,datee);
      }

      test = table_pos("OTHER_STD_VER",spriv->dhtTable);
      if (test >= 0) {
	buf1 = justify((char*)get_table_element(table_pos("OTHER_STD_VER",spriv->dhtTable), row, spriv->dhtTable, NULL, &count));
	rec_sprintf(spriv->metadatastring,"%sOther_STD_ver: %s\n",spriv->metadatastring,buf1);
	free(buf1);
      }

      test = table_pos("OTHER_STD_VER",spriv->dhtTable); 
      if (test >= 0) {
	buf1 = justify((char*)get_table_element(table_pos("OTHER_STD_VER",spriv->dhtTable), row, spriv->dhtTable, NULL, &count));
	rec_sprintf(spriv->metadatastring,"%sEdition_number: %s\n",spriv->metadatastring,buf1);
	free(buf1);	
      }

      test=table_pos("EDITION_DATE",spriv->dhtTable);
      if (test >= 0) {
	get_table_element(table_pos("EDITION_DATE",spriv->dhtTable), row, spriv->dhtTable, (void *)&datee, &count);
	rec_sprintf(spriv->metadatastring,"%sEdition_date: %s\n",spriv->metadatastring,datee); 
      }

      free_row(row, spriv->dhtTable); 		
    }
  }
  vpf_close_table(&(spriv->dhtTable));

  /* 
     code pour recuperer les valeurs de LAT 
     */

  rec_sprintf (spriv->metadatastring,"%s\n\n\nLIBRARY ATTRIBUTE TABLE(LAT):\n\n",spriv->metadatastring);

  for (i = 1; i <= spriv->latTable.nrows; ++i) {
    row = get_row(i, spriv->latTable);
    buf1 = justify((char*)get_table_element(1, row, spriv->latTable, NULL, &count));
    rec_sprintf(spriv->metadatastring,"%sCoverage name: %s\n",spriv->metadatastring,buf1);
    free(buf1);
    get_table_element(2, row, spriv->latTable, &buffloat, &count);
    rec_sprintf(spriv->metadatastring,"%sXMIN: %f\n",spriv->metadatastring,buffloat);
    get_table_element(3, row, spriv->latTable, &buffloat, &count);
    rec_sprintf(spriv->metadatastring,"%sYMIN: %f\n",spriv->metadatastring,buffloat);
    get_table_element(4, row, spriv->latTable, &buffloat, &count);
    rec_sprintf(spriv->metadatastring,"%sXMAX: %f\n",spriv->metadatastring,buffloat);
    get_table_element(5, row, spriv->latTable, &buffloat, &count);	
    rec_sprintf(spriv->metadatastring,"%sYMAX: %f\n",spriv->metadatastring,buffloat);	
    free_row(row, spriv->latTable); 		
  }

  /*ferme la liste de la ddb et ouvre la liste de la librairie*/

  rec_sprintf(spriv->metadatastring,"%s } { ",spriv->metadatastring);

  /* code pour recuperer les valeurs dans LHT */
  
  sprintf(buffer,"%s%slht",spriv->library,separator);
  if (muse_access(buffer,0) ==0)
    {
      rec_sprintf(spriv->metadatastring,"%sCURRENT DATABASE:%s\n\nLIBRARY LIBRARY HEADER TABLE(LHT):\n\n",spriv->metadatastring,spriv->database);

#ifdef TESTOPENTABLE
      printf("open spriv->lhtTable:%s\n",buffer);
#endif
      spriv->lhtTable = vpf_open_table(buffer,disk,"rb",NULL);

      for (i = 1; i <= spriv->lhtTable.nrows; ++i) {
	row = get_row(i, spriv->lhtTable);

	test=table_pos("PRODUCT_TYPE",spriv->lhtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(table_pos("PRODUCT_TYPE",spriv->lhtTable), row, spriv->lhtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sProduct_type: %s\n",spriv->metadatastring,buf1);
	  free(buf1);	
	}

	test=table_pos("LIBRARY_NAME",spriv->lhtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(table_pos("LIBRARY_NAME",spriv->lhtTable), row, spriv->lhtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sLibrary_name: %s\n",spriv->metadatastring,buf1);
	  free(buf1);	
	}

	test=table_pos("DESCRIPTION",spriv->lhtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(table_pos("DESCRIPTION",spriv->lhtTable), row, spriv->lhtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sDescription: %s\n",spriv->metadatastring,buf1);
	  free(buf1); 
	}

	
	test=table_pos("SOURCE_SERIES",spriv->lhtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(table_pos("SOURCE_SERIES",spriv->lhtTable), row, spriv->lhtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sSource_series: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}

	test=table_pos("SOURCE_ID",spriv->lhtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(table_pos("SOURCE_ID",spriv->lhtTable), row, spriv->lhtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sSource_ID: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}

	test=table_pos("SOURCE_EDITION",spriv->lhtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(table_pos("SOURCE_EDITION",spriv->lhtTable), row, spriv->lhtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sSource_edition: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}

	test=table_pos("SOURCE_NAME",spriv->lhtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(table_pos("SOURCE_NAME",spriv->lhtTable), row, spriv->lhtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sSource_name: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}

	test=table_pos("SOURCE_DATE",spriv->lhtTable);
	if (test >= 0) {
	  get_table_element(table_pos("SOURCE_DATE",spriv->lhtTable), row, spriv->lhtTable, (void *)&datee, &count);
	  rec_sprintf(spriv->metadatastring,"%sSource_date: %s\n",spriv->metadatastring,datee);
	}

	test=table_pos("DOWNGRADING",spriv->lhtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(table_pos("DOWNGRADING",spriv->lhtTable), row, spriv->lhtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sDowngrading: %s\n",spriv->metadatastring,buf1);
	  free(buf1);	
	}

	test=table_pos("DOWNGRADING_DATE",spriv->lhtTable);
	if (test >= 0) {
	  get_table_element(table_pos("DOWNGRADING_DATE",spriv->lhtTable), row, spriv->lhtTable, (void *)&datee, &count);
	  rec_sprintf(spriv->metadatastring,"%sDowngrading_date: %s\n",spriv->metadatastring,datee);
	}
	
	test=table_pos("RELEASABILITY",spriv->lhtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(table_pos("RELEASABILITY",spriv->lhtTable), row, spriv->lhtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sReleasability: %s\n",spriv->metadatastring,buf1);
	  free(buf1);	
	}
	free_row(row, spriv->lhtTable); 		
      }
    }



  /* code pour recuperer les valeurs dans CAT */
  rec_sprintf (spriv->metadatastring,"%s\n\n\nCOVERAGE ATTRIBUTE TABLE(CAT):\n\n",spriv->metadatastring);	

  for (i = 1; i <= spriv->catTable.nrows; ++i) {
    int buffint = 0;
    short buffshort = 0;

    row = get_row(i, spriv->catTable);
    buf1 = justify( (char *) get_table_element(1, row, spriv->catTable, NULL, &count));
    rec_sprintf(spriv->metadatastring,"%sCoverage_name: %s\n",spriv->metadatastring,buf1);
    free(buf1);
    buf1 = justify( (char *) get_table_element(2, row, spriv->catTable, NULL, &count));
    rec_sprintf(spriv->metadatastring,"%sDescription: %s\n",spriv->metadatastring,buf1);
    free(buf1);
    if( spriv->catTable.header[3].type == 'I' )
    {
      get_table_element(3, row, spriv->catTable, &buffint, &count);
    }
    else if( spriv->catTable.header[3].type == 'S' )
    {
      get_table_element(3, row, spriv->catTable, &buffshort, &count);
      buffint = buffshort;
    }
    rec_sprintf(spriv->metadatastring,"%sLevel: %d\n",spriv->metadatastring,buffint);
    free_row(row, spriv->catTable); 		
  }


  /* code pour recuperer les valeurs dans GRT */

	 
  sprintf(buffer,"%s%sgrt",spriv->library,separator);

  if (muse_access(buffer,0) ==0)
    {
      rec_sprintf (spriv->metadatastring,"%s\n\n\nGEOGRAPHIC REFERENCE TABLE(GRT):\n\n",spriv->metadatastring);
#ifdef TESTOPENTABLE
      printf("open spriv->grtTable:%s\n",buffer);
#endif
      spriv->grtTable = vpf_open_table(buffer,disk,"rb",NULL);

      for (i = 1; i <= spriv->grtTable.nrows; ++i) {
	row = get_row(i, spriv->grtTable);


	test=table_pos("DATA_TYPE",spriv->grtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->grtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sData_type: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}

	test=table_pos("UNITS",spriv->grtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->grtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sUnits: %s\n",spriv->metadatastring,buf1);
	  free(buf1);	
	}

	test=table_pos("ELLIPSOID",spriv->grtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->grtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sEllipsoid: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}

	test=table_pos("ELLIPSOID_DETAIL",spriv->grtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->grtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sEllipsoid_detail: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}
	
	test=table_pos("VERT_DATUM_REF",spriv->grtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->grtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sVert_datum_ref: %s\n",spriv->metadatastring,buf1);
	  free(buf1);	
	}

	test=table_pos("VERT_DATUM_CODE",spriv->grtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->grtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sVert_datum_code: %s\n",spriv->metadatastring,buf1);
	  free(buf1);	
	}

	test=table_pos("SOUND_DATUM_NAME",spriv->grtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->grtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sSound_datum_name: %s\n",spriv->metadatastring,buf1);
	  free(buf1);	
	}

	test=table_pos("SOUND_DATUM_CODE",spriv->grtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->grtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sSound_datum_code: %s\n",spriv->metadatastring,buf1);
	  free(buf1);	
	}

	test=table_pos("GEO_DATUM_NAME",spriv->grtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->grtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sGeo_datum_name: %s\n",spriv->metadatastring,buf1);
	  free(buf1);	
	}

	test=table_pos("GEO_DATUM_CODE",spriv->grtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->grtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sGeo_datum_code: %s\n",spriv->metadatastring,buf1);
	  free(buf1);	
	}

	test=table_pos("PROJECTION NAME",spriv->grtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->grtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sProjection_name: %s\n",spriv->metadatastring,buf1);
	  free(buf1);	
	}
	
	free_row(row, spriv->grtTable); 		
      }
    }
  vpf_close_table(&(spriv->grtTable));

  /* code pour recuperer les valeurs dans DQT */


  sprintf(buffer,"%s%sdqt",spriv->library,separator);
  if (muse_access(buffer,0) ==0)
    {
      rec_sprintf (spriv->metadatastring,"%s\n\n\nDATA QUALITY TABLE(DQT):\n\n",spriv->metadatastring);
#ifdef TESTOPENTABLE
      printf("open spriv->dqtTable:%s\n",buffer);
#endif
      spriv->dqtTable = vpf_open_table(buffer,disk,"rb",NULL);

      for (i = 1; i <= spriv->dqtTable.nrows; ++i) {
	row = get_row(i, spriv->dqtTable);

	test=table_pos("VPF_LEVEL",spriv->dqtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->dqtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sVpf_level: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}

	test=table_pos("VPF_LEVEL_NAME",spriv->dqtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->dqtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sVpf_level_name: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}

	test=table_pos("FEATURE_COMPLETE",spriv->dqtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->dqtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sFeature_complete: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}

	test=table_pos("ATTRIB_COMPLETE",spriv->dqtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->dqtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sAttrib_complete: %s\n",spriv->metadatastring,buf1);
	  free(buf1);	
	}

	test=table_pos("LOGICAL_CONSIST",spriv->dqtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->dqtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sLogical_consist: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}

	test=table_pos("EDITION_NUM",spriv->dqtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->dqtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sEdition_num: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}


	test=table_pos("CREATION_DATE",spriv->dqtTable);
	if (test >= 0) {
	  get_table_element(table_pos("CREATION_DATE",spriv->dqtTable), row, spriv->dqtTable, (void *)&datee, &count);
	  rec_sprintf(spriv->metadatastring,"%sCration date: %s\n",spriv->metadatastring,datee);
	}

	test=table_pos("REVISION_DATE",spriv->dqtTable);
	if (test >= 0) {
	  get_table_element(table_pos("REVISION_DATE",spriv->dqtTable), row, spriv->dqtTable, (void *)&datee, &count);
	  rec_sprintf(spriv->metadatastring,"%sRevision date: %s\n",spriv->metadatastring,datee);
	}
	

	test=table_pos("SPEC_NAME",spriv->dqtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->dqtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sSpec_name: %s\n",spriv->metadatastring,buf1);
	  free(buf1);	
	}


	test=table_pos("SPEC_DATE",spriv->dqtTable);
	if (test >= 0) {
	  get_table_element(table_pos("SPEC_DATE",spriv->dqtTable), row, spriv->dqtTable, (void *)&datee, &count);
	  rec_sprintf(spriv->metadatastring,"%sSpecification date: %s\n",spriv->metadatastring,datee);
	}

	test=table_pos("EARLIEST_SOURCE",spriv->dqtTable);
	if (test >= 0) {
	  get_table_element(table_pos("EARLIEST_SOURCE",spriv->dqtTable), row, spriv->dqtTable, (void *)&datee, &count);
	  rec_sprintf(spriv->metadatastring,"%sEarliest source: %s\n",spriv->metadatastring,datee);
	}

	test=table_pos("LATEST_SOURCE",spriv->dqtTable);
	if (test >= 0) {
	  get_table_element(table_pos("LATEST_SOURCE",spriv->dqtTable), row, spriv->dqtTable, (void *)&datee, &count);
	  rec_sprintf(spriv->metadatastring,"%sLatest source: %s\n",spriv->metadatastring,datee);
	}


	test=table_pos("QUANT_ATT_ACC",spriv->dqtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->dqtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sQuant_att_acc: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}

	test=table_pos("QUAL_ATT_ACC",spriv->dqtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->dqtTable, NULL, &count)); 
	  rec_sprintf(spriv->metadatastring,"%sQual_att_acc: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}

	test=table_pos("COLLECTION_SPEC",spriv->dqtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->dqtTable, NULL, &count)); 
	  rec_sprintf(spriv->metadatastring,"%sCollection_spec: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}

	test=table_pos("SOURCE_FILE_NAME",spriv->dqtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->dqtTable, NULL, &count)); 
	  rec_sprintf(spriv->metadatastring,"%sSource_file_name: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}

	test=table_pos("ABS_HORIZ_ACC",spriv->dqtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->dqtTable, NULL, &count));
	  rec_sprintf(spriv->metadatastring,"%sAbs_horiz_acc: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}

	test=table_pos("ABS_HORIZ_UNITS",spriv->dqtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->dqtTable, NULL, &count)); 
	  rec_sprintf(spriv->metadatastring,"%sAbs_horiz_units: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}

	test=table_pos("ABS_VERT_ACC",spriv->dqtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->dqtTable, NULL, &count)); 
	  rec_sprintf(spriv->metadatastring,"%sAbs_vert_acc: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}

	test=table_pos("ABS_VERT_UNITS",spriv->dqtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->dqtTable, NULL, &count)); 
	  rec_sprintf(spriv->metadatastring,"%sAbs_vert_units: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}

	test=table_pos("REL_HORIZ_ACC",spriv->dqtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->dqtTable, NULL, &count)); 
	  rec_sprintf(spriv->metadatastring,"%s:Rel_horiz_acc: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}

	test=table_pos("REL_HORIZ_UNITS",spriv->dqtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->dqtTable, NULL, &count)); 
	  rec_sprintf(spriv->metadatastring,"%s:Rel_horiz_units: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}

	test=table_pos("REL_VERT_ACC",spriv->dqtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->dqtTable, NULL, &count)); 
	  rec_sprintf(spriv->metadatastring,"%s:Rel_vert_acc %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}

	test=table_pos("REL_VERT_UNITS",spriv->dqtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->dqtTable, NULL, &count)); 
	  rec_sprintf(spriv->metadatastring,"%s:Rel_vert_units: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}

	test=table_pos("COMMENTS",spriv->dqtTable);
	if (test >= 0) {
	  buf1 = justify((char*)get_table_element(test, row, spriv->dqtTable, NULL, &count)); 
	  rec_sprintf(spriv->metadatastring,"%sComments: %s\n",spriv->metadatastring,buf1);
	  free(buf1);
	}
	
	free_row(row, spriv->dqtTable); 		
      }
    }
  vpf_close_table(&(spriv->dqtTable));


  /*ferme la premiere partie de la chaine*/

  rec_sprintf(spriv->metadatastring,"%s} } } { ",spriv->metadatastring);


  /* 
     code pour construire la chaine des coverages.Pour chaque coverage
     ouvre les fichiers ../coverage/INT.VDT, ../coverage/FLOAT.VDT,
     ../coverage/CHAR.VDT
     */

 
  for (z = 1; z <= spriv->catTable.nrows; z++) {
    existtableflag=0;
    rowcat = get_row(z, spriv->catTable); 

    /*ajoute debut chaine*/
    rec_sprintf(spriv->metadatastring,"%s {",spriv->metadatastring);

    /*ajoute class et nom coverage*/
    covname = justify( (char *) get_table_element(1, rowcat, spriv->catTable, NULL, &count));
    rec_sprintf(spriv->metadatastring,"%s family %s class",spriv->metadatastring,covname);

    /*ajoute description et debut du covinfo*/
    buf1 = justify( (char *) get_table_element(2, rowcat, spriv->catTable, NULL, &count));

    rec_sprintf(spriv->metadatastring,"%s {%s} {",spriv->metadatastring, buf1);
    free(buf1);

    /********/

    /*ouvre fcs*/
    sprintf(buffer,"%s/%s/fcs",spriv->library,covname);
#ifdef TESTOPENTABLE
    printf("open spriv->fcsTable:%s\n",buffer);
#endif
    spriv->fcsTable = vpf_open_table(buffer, disk, "rb", NULL);
    
    /*ouvre fca*/
    sprintf(buffer,"%s/%s/fca",spriv->library,covname);
    if (muse_access(buffer,0) == 0) {

#ifdef TESTOPENTABLE
      printf("open spriv->fcaTable:%s\n",buffer);
#endif
	spriv->fcaTable = vpf_open_table(buffer, disk, "rb", NULL);
		
	/* We do not want to abort on a whole library because a FCS file is */
	/* missing. That happens for example with DNC17/COA17A/ENV */
	if (spriv->fcsTable.path == NULL) 
	  {
            char szErrorMessage[128];
            sprintf(szErrorMessage, "Can't open the FCS table of '%s', invalid VRF coverage",
                    covname);
	    rec_sprintf(spriv->metadatastring,"%s <ERROR>Cannot open %s/%s/fcs</ERROR>",
			spriv->metadatastring,spriv->library,covname );
	    if( ecs_SetErrorShouldStop(&(s->result),1,szErrorMessage) )
	    {
                free(covname);
                vpf_close_table(&(spriv->lhtTable));
                vpf_close_table(&(spriv->fcaTable));
                free_row(rowcat, spriv->catTable);
		return 0;
	    }
	  }
	  else
	  {
	      rec_sprintf(spriv->metadatastring,"%s<Grassland>displaymetadata {",spriv->metadatastring);
	  }
  
	for (i = 1; i <= spriv->fcsTable.nrows; ++i) 
	  {
	    flag=0;
	    row = get_row(i, spriv->fcsTable);
	    rowcomp=get_row(i-1, spriv->fcsTable);
	    if (i>=2)
	      {	
		buf2 = justify((char*)get_table_element(1, rowcomp, spriv->fcsTable, NULL, &count));
		buf1 = justify((char*)get_table_element(1, row, spriv->fcsTable, NULL, &count));
	      }else{  
				
		buf1 = justify((char*)get_table_element(1, row, spriv->fcsTable, NULL, &count));
		buf2 = justify( (char *) get_table_element(2, row, spriv->lhtTable, NULL, &count));
	      } 

	    if (strcmp(buf1,buf2)!=0)
	      {
		for (j = 1;j<= spriv->fcaTable.nrows; j++)
		  {
		    rowfca = get_row(j, spriv->fcaTable);
		    bufname = justify((char*)get_table_element(table_pos("FCLASS",spriv->fcaTable), rowfca, spriv->fcaTable, NULL, &count));
		    bufdesc = justify((char*)get_table_element(table_pos("DESCR",spriv->fcaTable), rowfca, spriv->fcaTable, NULL, &count));
		    /*intbuf = get_table_element(, rowfca, spriv->fcaTable, NULL, &count));*/
		    if (flag==0)
                    {
			if (strcmp(buf1,bufname)==0)
                        {
			    rec_sprintf(spriv->metadatastring,"%s { %s { %s } } ",spriv->metadatastring,bufname,bufdesc);
			    flag=1;
                        }
                    }
                    free_row(rowfca, spriv->fcaTable);
                    free(bufname);
                    free(bufdesc);
		  }
	      }

	    free(buf1);
	    free(buf2);
            free_row(row, spriv->fcsTable);
	    free_row(rowcomp, spriv->fcsTable);
	  }
	
	  if (spriv->fcsTable.path != NULL)  {
		rec_sprintf(spriv->metadatastring,"%s }",spriv->metadatastring);
	  }
	
	vpf_close_table(&(spriv->fcaTable));		
    }
    else
    {
	rec_sprintf(spriv->metadatastring,"%s<Grassland>displaymetadata { }",spriv->metadatastring);
    }

    rec_sprintf(spriv->metadatastring,"%s {",spriv->metadatastring);
    vpf_close_table(&(spriv->fcsTable));
 

    /********/
    /*	 free(buf1);  */
    /*lit les metadonnees de INT FLOAT et CHAR.VDT dans une boucle*/
	 
    for (j=0;j<3;j++)
      {
	sprintf(buffer,"%s%s%s%s%s.vdt",spriv->library,separator,covname,separator,tab[j]);

	if (muse_access(buffer,0) ==0)
	  {
	    existtableflag=1;
	    rec_sprintf(spriv->metadatastring,"%s \nVALUE DESCRIPTION TABLE (%s.vdt)\n\n",spriv->metadatastring,tab[j]);
	
	
#ifdef TESTOPENTABLE
	    printf("open table:%s\n",buffer);
#endif
	    table   = vpf_open_table(buffer,disk,"rb",NULL);
	    it_pos  = table_pos("TABLE",table);
	    val_pos = table_pos("VALUE",table);
	    des_pos = table_pos("DESCRIPTION",table);
	    att_pos = table_pos("ATTRIBUTE",table);

	    for(k=1;k<=table.nrows;k++) 
	      /*for(k=1;k<=10;k++) */
	      {
		if (stor == disk)
		  row = read_next_row(table);
		else
		  row = get_row(k,table);

		item_buf = (char *)get_table_element( it_pos,row,table,NULL,&n);
		rightjust(item_buf);

	    
		att_buf  = (char *)get_table_element(att_pos,row,table,NULL,&n);
		rightjust(att_buf);

		des_buf  = (char *)get_table_element(des_pos,row,table,NULL,&n);

		
		if ((strnicmp ("char",tab[j],5)) ==0) {
		  tval = (char *)get_table_element(val_pos,row,table,NULL,&n);
		  sprintf(buffer,"    %s =  %s  \n",tval,des_buf);
		  rec_sprintf(spriv->metadatastring,"%s%s",spriv->metadatastring,buffer);
		  free(tval);
		}
	      
		if ((strnicmp ("int",tab[j],6)) ==0
                    && table.header[val_pos].type == 'I' ) {
                    get_table_element(val_pos,row,table,&intval,&n);   
                    sprintf(buffer,"    %12ld =  %s  \n",
                            (long)intval,des_buf);
                    rec_sprintf(spriv->metadatastring,"%s%s",
                            spriv->metadatastring,buffer);
		}		 

		if ((strnicmp ("int",tab[j],6)) ==0
                    && table.header[val_pos].type == 'S' ) {
                    short	short_val;

                    get_table_element(val_pos,row,table,&short_val,&n);   
                    sprintf(buffer,"    %12ld =  %s  \n",
                            (long)short_val,des_buf);
                    rec_sprintf(spriv->metadatastring,"%s%s",
                            spriv->metadatastring,buffer);
		}		 


		if ((strnicmp ("float",tab[j],6) ==0)) {
		  get_table_element(val_pos,row,table,&fval,&n);
		  sprintf(buffer,"    %12f =  %s  \n",fval,des_buf);
		  rec_sprintf(spriv->metadatastring,"%s%s",spriv->metadatastring,buffer);
		}
	    
                free(des_buf);	  
                free(item_buf);
                free(att_buf);
                free_row(row, table);
	      }  /** for k on table.nrows **/
#ifdef TESTOPENTABLE
	    printf("close: table\n");
#endif
	    vpf_close_table(&table);
	  } /**if muse access**/
	

      }  /**for j=1,j<=3**/

    free(covname);
    
    if (existtableflag==0)
      rec_sprintf(spriv->metadatastring,"%snodata",spriv->metadatastring);

    /* ferme la chaine covinfo*/
    rec_sprintf(spriv->metadatastring,"%s} } } ",spriv->metadatastring);	  
    free_row(rowcat, spriv->catTable);
  } /**(i = 1; i <= spriv->catTable.nrows**/
  /*ferme la chaine generale*/
  rec_sprintf(spriv->metadatastring,"%s }",spriv->metadatastring);
  /*printf ("%s",spriv->metadatastring);*/
  vpf_close_table(&(spriv->lhtTable));

  return 1;
}


/*  -------------------------------------------------------------------------
 *  vrf_initRegionWithDefault:
 *
 *     preparation de la fenetre globale pour le server                    
 *  --------------------------------------------------------------------------
 */

int
vrf_initRegionWithDefault(s)
     ecs_Server *s;
{
  int i;
  int found = 0;
  char *buf1;
  int count;
  row_type row;
  float buffloat;
  register ServerPrivateData *spriv = s->priv;

  /* code to get global bounding box of library is inserted here */

  for (i = 1; i <= spriv->latTable.nrows && !found; ++i) {
    row = get_row(i, spriv->latTable);
    buf1 = justify((char*)get_table_element(1, row, spriv->latTable, NULL, &count));
    if (stricmp(buf1,spriv->libname) == 0) {
      found = 1;
      get_table_element(5, row, spriv->latTable, &buffloat, &count);
      s->globalRegion.north = buffloat;
      get_table_element(3, row, spriv->latTable, &buffloat, &count);
      s->globalRegion.south = buffloat;
      get_table_element(4, row, spriv->latTable, &buffloat, &count);
      s->globalRegion.east = buffloat;
      get_table_element(2, row, spriv->latTable, &buffloat, &count);	
      s->globalRegion.west = buffloat;
    }
    free(buf1);
    free_row(row, spriv->latTable); 		
  }

  if (!found) {
    ecs_SetError(&(s->result),1,"Can't find entry in LAT table, invalid VRF library");
    return 0;		
  }

  /* If the west is higher than east, add 360 deg to east */
  
  if (s->globalRegion.west > s->globalRegion.east)
    s->globalRegion.east += 360.0;

  s->globalRegion.ns_res = 0.01;
  s->globalRegion.ew_res = 0.01;

  dyn_SelectRegion(s,&(s->globalRegion));

  return 1;
}


/*  -------------------------------------------------------------------------
 *  vrf_initTiling:
 *
 *     Lecture de la table de reference de toutes les tuiles                    
 *  --------------------------------------------------------------------------
 */

int
vrf_initTiling(s)
     ecs_Server *s;
{
  char buffer[256];
  int i;
  int32 fac_id,count;
  void *dummy = NULL;
  vpf_table_type tile_table, mbr_tile_table;
  register ServerPrivateData *spriv = s->priv;
  
  sprintf(buffer,"%s/tileref/tileref.aft",spriv->library);
  if (muse_access(buffer,0)!=0) {
    sprintf(buffer,"%s/TILEREF/TILEREF.AFT",spriv->library);
    if (muse_access(buffer,0)!=0) {
      spriv->isTiled = 0;
      spriv->tile = (VRFTile *) malloc(sizeof(VRFTile));
      spriv->tile[0].isSelected = 1;
      spriv->tile[0].xmin = (float) s->globalRegion.south;
      spriv->tile[0].xmax = (float) s->globalRegion.north;
      spriv->tile[0].ymin = (float) s->globalRegion.west;
      spriv->tile[0].ymax = (float) s->globalRegion.east;
      spriv->tile[0].path = NULL;
      spriv->nbTile = 1;
      return 1;
    }
  }
  

  spriv->isTiled = 1;
#ifdef TESTOPENTABLE
  printf("open tile_table:%s\n",buffer);
#endif
  tile_table = vpf_open_table(buffer,ram,"rb",NULL);


  spriv->tile = (VRFTile *) malloc(sizeof(VRFTile) * tile_table.nrows);
  if (spriv->tile == NULL) {
#ifdef TESTOPENTABLE
    printf("close: tile_table\n");
#endif
    
    vpf_close_table(&(tile_table));
    ecs_SetError(&(s->result),1,"Can't allocate enough memory to read tile reference");
    return 0;	
  }
  memset( spriv->tile, 0, sizeof(VRFTile) * tile_table.nrows );

  sprintf(buffer,"%s/tileref/fbr",spriv->library);
  if (muse_access(buffer,0)!=0) {
    sprintf(buffer,"%s/TILEREF/FBR",spriv->library);
    if (muse_access(buffer,0)!=0) {
#ifdef TESTOPENTABLE
      printf("close: tile_table\n");
#endif
      vpf_close_table(&tile_table);
      ecs_SetError(&(s->result),1,"Can't open tileref/fbr file");
      return 0;	
    }
  }
  
#ifdef TESTOPENTABLE
  printf("open mbr_tile_table:%s\n",buffer);
#endif

#ifdef VRF_DEBUG
  printf("NbTile in _initTiling:%d\n",spriv->nbTile);
#endif
  
  mbr_tile_table = vpf_open_table(buffer,ram,"rb",NULL);
  
  spriv->nbTile = tile_table.nrows;
  for (i = 0; i < spriv->nbTile; ++i) {
    
    /* Check if fac_id exist */
    
    if (table_pos("FAC_ID", tile_table) == -1) {
      fac_id = i+1;
    } else {
      named_table_element("FAC_ID",i+1,tile_table,&fac_id,&count);
    }

    spriv->tile[i].path = justify((char *) named_table_element("TILE_NAME",i+1,tile_table,dummy,&count));
    
    named_table_element("XMIN",fac_id,mbr_tile_table,&(spriv->tile[i].xmin),&count);
    named_table_element("XMAX",fac_id,mbr_tile_table,&(spriv->tile[i].xmax),&count);
    named_table_element("YMIN",fac_id,mbr_tile_table,&(spriv->tile[i].ymin),&count);
    named_table_element("YMAX",fac_id,mbr_tile_table,&(spriv->tile[i].ymax),&count);
    
    spriv->tile[i].isSelected = 0;
  }
  
#ifdef TESTOPENTABLE
  printf("close: tile_table\n");
  printf("close: mbr_tile_table\n");
#endif

  vpf_close_table(&tile_table);
  vpf_close_table(&mbr_tile_table);
  
  return 1;
}


int vrf_IsOutsideRegion(n,s,e,w,region)
     double n,s,e,w;
     ecs_Region *region;
{
  if ((n <= region->south) || 
      (s >= region->north) || 
      (e <= region->west)  || 
      (w >= region->east)) {	
    return 1;
  }
  return 0;
}


void vrf_AllFClass(s,coverage)
     ecs_Server *s;
     char *coverage;
{  
  vpf_table_type table;
  row_type row;
  unsigned int i, j, n, k, count=0;
  char *temp;
  char *name, *fclass, **list;
  char ftype[8] = {'A', 'L', 'T', 'P', 'a', 'l', 't', 'p' };
  BOOLEAN found;
  char buffer[256];
  register ServerPrivateData *spriv = s->priv;

  /* Build path to feature class atrribute table */

  sprintf(buffer,"%s/%s/fcs",spriv->library,coverage);
  if (muse_access(buffer,0) != 0) {
    sprintf(buffer,"%s/%s/FCS",spriv->library,coverage);
  }

  if (muse_access(buffer,0) == 0) {
#ifdef TESTOPENTABLE
    printf("open table:%s\n",buffer);
#endif

    table = vpf_open_table (buffer, DISK, "rb", NULL);
    list =(char**) malloc ((table.nrows+1) * sizeof(char *));

    for (i=0; i < (unsigned int) table.nrows; i++) {

      row = get_row ((i+1), table);
      fclass = (char*)get_table_element (1, row, table, NULL, &n); /* Get feature class name */
      fclass = justify (fclass);

      /* Now find the name of the feature table that matches the feature class */

      name = (char*)get_table_element (2, row, table, NULL, &n);
      temp = (char*) malloc (strlen (fclass) + 1);
      strncpy (temp, name, strlen (fclass));
	
      if (strcmp (fclass, temp) != 0) {
	free (name);
	name = (char*) get_table_element (4, row, table, NULL, &n);
      }
      free (temp);

      /* Start the name list with the first record */

      if (i == 0) {
	list[count] = (char*) malloc ((n+1) * sizeof (char));
	strcpy (list[count], name);
	count++;
      }

      /* Check to see if the feature class name has already been added to the list */
      
      found = FALSE;
      for (j=0; j<count; j++) {
	if (strncmp (fclass, list[j],strlen(fclass)) == 0) {
	  found = TRUE;
	  break;
	}
      }
      
      if (found == FALSE) {
	list[count] = (char*) malloc ((n+1) * sizeof (char));
	strcpy (list[count], name);
	count++;
      }
      
      free (name);
      free_row (row, table);
    }

#ifdef TESTOPENTABLE
    printf("close: table\n");
#endif

    vpf_close_table (&table);

    /* Add the list of Area fclass */

    ecs_AddText(&(s->result)," ");

    for(k=0; k < 4; ++k) {

      ecs_AddText(&(s->result),"{ ");
      
      for (i=0; i<count; i++) {
	
	found = FALSE;
	
	for(j = 0; j < strlen(list[i]); ++j) {
	  if (list[i][j] == '.') {
	    found = TRUE;
	    break;
	  }
	}
	
	if (found) {

	  if (list[i][j+1] == ftype[k] || list[i][j+1] == ftype[k+4]) {
	    strncpy(buffer,list[i],j);
	    buffer[j] = 0;
	    ecs_AddText(&(s->result),buffer);
	    ecs_AddText(&(s->result)," ");
	  }
	}
      }

      ecs_AddText(&(s->result),"} ");
    }
		
		
    for (i=0; i<count; i++) 
      free (list[i]);

    free ((char*)list);
  }
 
}


int vrf_feature_class_dictionary(s,request)
     ecs_Server *s;
     char *request;
{
  int                i,k,it_pos,val_pos,des_pos,att_pos;
  short int sintval;
  char               *line,temp[128],temp2[128],*item_buf, *att_buf, *des_buf, *tval;
  vpf_table_type     ft,table,nar,fcstable;
  storage_type       stor = disk;
  row_type           row;
  float              fval;
  int32           ival,n;
  char buffer[128];
  register ServerPrivateData *spriv = s->priv;
  char *buf1;
  int found  = 0;
  char *featureTableName;
  char *fclass;
  char *coverage;
  char *expression;
  int32 count;

  /*
    Extract the request info
    */

  if (!vrf_parsePathValue(s,request,&fclass,&coverage,&expression))
    return FALSE;

  /* 
     Print the main informations in the dictionary
     */

  sprintf(buffer,"FEATURE CLASS: %s \nCOVERAGE     : %s \n",fclass,coverage);
  if (!ecs_SetText(&(s->result),buffer)) {
    free(fclass); free(coverage); free(expression); return FALSE;
  }

  /*
    Found in the FCS the tables
    */

  sprintf(buffer,"%s/%s/fcs",spriv->library,coverage);
  if (muse_access(buffer,0) != 0) {
    sprintf(buffer,"%s/%s/FCS",spriv->library,coverage);
    if (muse_access(buffer,0) != 0) {
      ecs_SetError(&(s->result),1,"Can't open the FCS table, invalid VRF coverage");
      free(fclass); free(coverage); free(expression); 
      return FALSE;
    }
  }

  
#ifdef TESTOPENTABLE
  printf("open fcstable:%s\n",buffer);
#endif
  fcstable = vpf_open_table(buffer, disk, "rb", NULL);
  if (fcstable.path == NULL) {
    ecs_SetError(&(s->result),1,"Can't open the FCS table, invalid VRF coverage");
    free(fclass); free(coverage); free(expression); 
    return FALSE;
  }
  
  for (i = 1; i <= fcstable.nrows && !found; ++i) {
    row = get_row(i, fcstable);
    buf1 = justify((char*)get_table_element(1, row, fcstable, 
					    NULL, &count));
    if (stricmp(buf1,fclass) == 0) {
      found = 1;
      featureTableName = justify((char *)get_table_element(2, row, fcstable, NULL, &count));
      sprintf(buffer,"%s/%s/%s",spriv->library,coverage,featureTableName);	 
      free(featureTableName);
    }
    free(buf1);
    free_row(row, fcstable); 		
  }
  
#ifdef TESTOPENTABLE
  printf("close: fcstable\n");
#endif
  
  vpf_close_table(&fcstable);
  
  /*
    For each attribute. Get all the informations.
    */
  
#ifdef TESTOPENTABLE
  printf("open ft:%s\n",buffer);
#endif
  
  ft = vpf_open_table(buffer,stor,"rb",NULL);
  
  sprintf(buffer,"ATTRIBUTES:\n");
  if (!ecs_AddText(&(s->result),buffer)) {
    free(fclass); free(coverage); free(expression); return FALSE;
  }
   
  for(i=0;i<ft.nfields;i++) {
    sprintf(buffer,"   %s - %s \n",ft.header[i].name,ft.header[i].description);
    if (!ecs_AddText(&(s->result),buffer)) {
      free(fclass); free(coverage); free(expression); return FALSE;
    }
     
    if(ft.header[i].vdt[0] != '\0')
      /** create table name **/
      { 
	sprintf(temp,"%s\\%s",spriv->library,coverage);

	/** add to the path the new table name **/
	strcpy(temp2,temp);
	strcat(temp2,"\\");
	strcat(temp2,ft.header[i].vdt);
	if(muse_access(temp2,0) == 0) {
#ifdef TESTOPENTABLE
	  printf("open table:%s\n",temp2);
#endif
	  table   = vpf_open_table(temp2,stor,"rb",NULL);
	  it_pos  = table_pos("TABLE",table);
	  val_pos = table_pos("VALUE",table);
	  des_pos = table_pos("DESCRIPTION",table);
	  att_pos = table_pos("ATTRIBUTE",table);

	  for(k=1;k<=table.nrows;k++) {
	    if (stor == disk)
	      row = read_next_row(table);
	    else
	      row = get_row(k,table);
	    item_buf = (char *)get_table_element( it_pos,row,table,NULL,&n);
	    rightjust(item_buf);
	    if (strlen(item_buf) > strlen(fclass))
	      item_buf[strlen(fclass)] = '\0';
	    
	    att_buf  = (char *)get_table_element(att_pos,row,table,NULL,&n);
	    rightjust(att_buf);
	    if(stricmp(item_buf,fclass) == 0 &&
	       stricmp(att_buf,ft.header[i].name) == 0) { 
	      des_buf  = (char *)get_table_element(des_pos,row,table,NULL,&n);
	      switch (ft.header[i].type) {
	      case 'T':
          case 'L':
		tval = (char *)get_table_element(val_pos,row,table,NULL,&n);
		sprintf(buffer,"    %s =  %s  \n",tval,des_buf);
		if (!ecs_AddText(&(s->result),buffer)) {
		  free(fclass); free(coverage); free(expression); return FALSE;
		}
		free(tval);break;
	      case 'I':
		get_table_element(val_pos,row,table,&ival,&n);
		sprintf(buffer,"    %12d =  %s  \n",ival,des_buf);
		if (!ecs_AddText(&(s->result),buffer)) {
		  free(fclass); free(coverage); free(expression); return FALSE;
		}
		break;
	      case 'S':
		get_table_element(val_pos,row,table,&sintval,&n);
		sprintf(buffer,"    %d =  %s  \n",sintval,des_buf);
		if (!ecs_AddText(&(s->result),buffer)) {
		  free(fclass); free(coverage); free(expression); return FALSE;
		}
		break;
	      case 'F':
		get_table_element(val_pos,row,table,&fval,&n);
		sprintf(buffer,"    %12f =  %s  \n",fval,des_buf);
		if (!ecs_AddText(&(s->result),buffer)) {
		  free(fclass); free(coverage); free(expression); return FALSE;
		}
    		break;
	      } /** switch **/
	      free(des_buf);
	    }  /** if strcmp **/
	    
	    free(item_buf);
	    free(att_buf);
	    free_row(row,table);
	  }  /** for k on table.nrows **/
#ifdef TESTOPENTABLE
	  printf("close: table\n");
#endif
	  vpf_close_table(&table);
	} /** if access **/
      } /** if there is vdt **/
  }  /** for fields **/

  /** get information from narrative file if it exists **/
  if(ft.narrative[0] != '\0') {
    sprintf(temp,"%s%s",ft.path,ft.narrative);
    if (muse_access(temp,0)==0) {
      sprintf(buffer,"\n\n");
      if (!ecs_AddText(&(s->result),buffer)) {
	free(fclass); free(coverage); free(expression); return FALSE;
      }
#ifdef TESTOPENTABLE
      printf("open nar:%s\n",temp);
#endif
      nar = vpf_open_table(temp,disk,"rb",NULL);
      for (i=1;i<=nar.nrows;i++) {
	row = read_next_row(nar);
	line = (char *)get_table_element(1,row,nar,NULL,&n);
	rightjust(line);
	sprintf(buffer,"%s\n",line);
	if (!ecs_AddText(&(s->result),buffer)) {
	  free(fclass); free(coverage); free(expression); return FALSE;
	}
	free(line);
	free_row(row,nar);
      }

#ifdef TESTOPENTABLE
      printf("close: nar\n");
#endif
      vpf_close_table(&nar);
    }
  }

  /** clean up **/
#ifdef TESTOPENTABLE
  printf("close: ft\n");
#endif
  vpf_close_table(&ft);

  free(fclass); 
  free(coverage); 
  free(expression); 
  

  return 1;
}

static void  vrf_build_layer_capabilities( ecs_Server *s, const char *coverage,
                                           const char *name )

{
    char	line[512];
    char	short_name[128];
    const char  *family;
    int		i;

/* -------------------------------------------------------------------- */
/*      Establish the family of this feature type, and the shortened name.*/
/* -------------------------------------------------------------------- */
    for( i = 0; name[i] != '.' && name[i] != '\0'; i++ ) {}

    if( strncmp(name+i,".A",2) == 0 || strncmp(name+i,".a",2) == 0 )
        family = "Area";
    else if( strncmp(name+i,".L",2) == 0 || strncmp(name+i,".l",2) == 0 )
        family = "Line";
    else if( strncmp(name+i,".p",2) == 0 || strncmp(name+i,".p",2) == 0 )
        family = "Point";
    else if( strncmp(name+i,".T",2) == 0 || strncmp(name+i,".t",2) == 0 )
        family = "Text";
    else
    {
        /* It isn't a geographic feature type, skip it */
        return;
    }

    strncpy( short_name, name, i );
    short_name[i] = '\0';

/* -------------------------------------------------------------------- */
/*      Create the various entries.                                     */
/* -------------------------------------------------------------------- */
    ecs_AddText(&(s->result),
                "      <FeatureType>\n");
    
    sprintf( line, 
             "        <Name>%s@%s(*)</Name>\n", 
             short_name, coverage );
    ecs_AddText(&(s->result), line);
    
    ecs_AddText(&(s->result),
                "        <SRS>PROJ4:+proj=longlat +datum=wgs84</SRS>\n" );

    sprintf( line, 
             "        <Family>%s</Family>\n", 
             family );
    ecs_AddText(&(s->result), line);
    
    sprintf( line, 
             "        <QueryExpression qe_prefix=\"%s@%s(\"\n"
             "                         qe_suffix=\")\"\n"
             "                         qe_format=\"restricted_where\" />\n",
             short_name, coverage );
    ecs_AddText(&(s->result), line);

    sprintf(line, 
            "        <LatLonBoundingBox minx=\"%.9f\"  miny=\"%.9f\"\n"
            "                           maxx=\"%.9f\"  maxy=\"%.9f\" />\n",
            s->globalRegion.west, s->globalRegion.south, 
            s->globalRegion.east, s->globalRegion.north );
    
    ecs_AddText(&(s->result),line);
    
    sprintf(line, 
            "        <BoundingBox minx=\"%.9f\"  miny=\"%.9f\"\n"
            "                     maxx=\"%.9f\"  maxy=\"%.9f\"\n"
            "                     resx=\"%.9f\"  resy=\"%.9f\" />\n",
            s->globalRegion.west, s->globalRegion.south, 
            s->globalRegion.east, s->globalRegion.north,
            s->globalRegion.ew_res, s->globalRegion.ns_res );
    ecs_AddText(&(s->result),line);
      
    ecs_AddText(&(s->result),
                "      </FeatureType>\n");
}

/* based on vrf_AllFClass() */					

static void 
vrf_build_coverage_capabilities( ecs_Server *s, const char *coverage)

{  
    vpf_table_type table;
    row_type row;
    unsigned int i, n;
    char *name, *fclass;
    char buffer[256];
    register ServerPrivateData *spriv = s->priv;

    /* Build path to feature class atrribute table */

    sprintf(buffer,"%s/%s/fcs",spriv->library,coverage);
    if (muse_access(buffer,0) != 0) {
        sprintf(buffer,"%s/%s/FCS",spriv->library,coverage);
    }

    if (muse_access(buffer,0) == 0) {
        char **list;
        int count = 0, j;
        
        table = vpf_open_table (buffer, DISK, "rb", NULL);

        list = (char**) malloc ((table.nrows+1) * sizeof(char *));

        for (i=0; i < (unsigned int) table.nrows; i++) {

            row = get_row ((i+1), table);
            fclass = (char*)get_table_element (1, row, table, NULL, &n); /* Get feature class name */
            fclass = justify (fclass);

            /* Now find the name of the feature table that matches the feature class */

            name = (char*)get_table_element (2, row, table, NULL, &n);
            if (strncmp (fclass, name, strlen(fclass)) != 0) {
                free (name);
                name = (char*) get_table_element (4, row, table, NULL, &n);
            }
            free( fclass );

            if (!is_join(name))
            {
              /* Have we already processed this name? */
              for( j = 0; j < count && strcmp(list[j],name) != 0; j++ ) {}
  
              if( j == count )
              {
                  vrf_build_layer_capabilities( s, coverage, name );
                  list[count++] = name;
              }
              else
                  free( name );
            }
            else
              free(name);

            free_row (row, table);
        }
        
        vpf_close_table (&table);

        for( i = 0; i < count; i++ )
            free( list[i] );

        free( list );
    }
}

int vrf_build_capabilities(ecs_Server *s, const char *request)
{
    ServerPrivateData *spriv = (ServerPrivateData *) s->priv;

    ecs_SetText(&(s->result), "" );
    
    ecs_AddText(&(s->result),
                "<?xml version=\"1.0\" ?>\n"
                "<OGDI_Capabilities version=\"3.1\">\n"
                "  <Capability>\n"
                "    <Extension>ogdi_unique_identity</Extension>\n"
                "  </Capability>\n" );

    if( strcmp(request,"ogdi_server_capabilities") != 0 )
    {
        int		i;

        ecs_AddText(&(s->result),
                    "  <FeatureTypeList>\n" );

        for (i = 1; i <= spriv->catTable.nrows; ++i) 
        {
            row_type row;
            char *coverage;
            char *description;
            int	count;

            row = get_row(i, spriv->catTable);
            coverage = justify( (char *) get_table_element(1, row, spriv->catTable, NULL, &count));
            description = justify( (char *) get_table_element(2, row, spriv->catTable, NULL, &count));
 
            free_row(row, spriv->catTable);

            ecs_AddText(&(s->result),
                        "    <FeatureTypeList>\n" );
            
            ecs_AddText(&(s->result),"      <Name>" );
            ecs_AddText(&(s->result),coverage);
            ecs_AddText(&(s->result),"</Name>\n" );

            ecs_AddText(&(s->result),"      <Title>" );
            ecs_AddText(&(s->result),description);
            ecs_AddText(&(s->result),"</Title>\n" );

            vrf_build_coverage_capabilities(s,coverage);	

            free(coverage);
            free(description); 

            ecs_AddText(&(s->result),
                        "    </FeatureTypeList>\n" );
        }	
        
        ecs_AddText(&(s->result),
                    "  </FeatureTypeList>\n" );
    }

    ecs_AddText(&(s->result),
                "</OGDI_Capabilities>\n" );
    return TRUE;
}


#ifndef _WINDOWS

/*
int stricmp(a,b)
     const char *a;
     const char *b;
{
  return strcasecmp(a,b);
}

int _stricmp(a,b)
     const char *a;
     const char *b;
{
  return strcasecmp(a,b);
}
*/

#endif

