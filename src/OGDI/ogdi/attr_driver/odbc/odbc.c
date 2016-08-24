/*********************************************************************

  CSOURCE_INFORMATION
  
  NAME
     odbc.c

  DESCRIPTION
     Implementation of the odbc attribute driver
  END_DESCRIPTION

  END_CSOURCE_INFORMATION

  Copyright (C) 1997 Logiciels et Applications Scientifiques (L.A.S.) Inc
  Permission to use, copy, modify and distribute this software and
  its documentation for any purpose and without fee is hereby granted,
  provided that the above copyright notice appear in all copies, that
  both the copyright notice and this permission notice appear in
  supporting documentation, and that the name of L.A.S. Inc not be used 
  in advertising or publicity pertaining to distribution of the software 
  without specific, written prior permission. L.A.S. Inc. makes no
  representations about the suitability of this software for any purpose.
  It is provided "as is" without express or implied warranty.

  ********************************************************************/

#include "odbc.h"

/*********************************************************************

  STRUCTURE_INFORMATION
  
  NAME
     odbcEnv

  DESCRIPTION
     The global environment handler. Set only at the first call to
     dyn_InitializeDBLink. To know if an allocation must be done,
     check the variable odbc_count.
  END_DESCRIPTION

  ATTRIBUTES
     HENV odbcEnv: Global environment handler
  END_ATTRIBUTES

  END_STRUCTURE_INFORMATION

  ********************************************************************/

SQLHENV odbcEnv;

/*********************************************************************

  STRUCTURE_INFORMATION
  
  NAME
     odbc_count

  DESCRIPTION
     Indicate the number of layers using this attribute driver.
  END_DESCRIPTION

  ATTRIBUTES
     int odbc_count: Qty of links.
  END_ATTRIBUTES

  END_STRUCTURE_INFORMATION

  ********************************************************************/

int odbc_count = 0;


/*********************************************************************

  STRUCTURE_INFORMATION
  
  NAME
     odbc_count

  DESCRIPTION
     Indicate the number of layers using this attribute driver.
  END_DESCRIPTION

  ATTRIBUTES
     int odbc_count: Qty of links.
  END_ATTRIBUTES

  END_STRUCTURE_INFORMATION

  ********************************************************************/

char *odbcerrorlist[] = {
  "Not enough memory"
};

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_InitializeDBLink

   DESCRIPTION
      Initialize the database link
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server object attributes
	 ecs_Layer *l: Layer object attributes
      OUTPUT
	 char **error: Error message (if it's the case)
   END_PARAMETERS

   RETURN_VALUE
      int : An boolean code. 0 if the operation is a success. a code if the operation failed.

   END_FUNCTION_INFORMATION

   PSEUDO_CODE

   1. Set the private data structure to l->attribute_priv and set attributes in
   it to NULL.

   2. If odbc_count equal 0, the call SQLAllocEnv and set the odbcEnv

   3. Increment odbc_count and set l->attribute_priv.isLinked to FALSE.

   4. Call SQLAllocConnect to initialize the link between SQL and this application.
   If an error occur, call dyn_DeinitializeDBLink and return an error code

   5. Call SQLConnect to connect to the ODBC with the attributes initialisation 
   in l->AttributeDriverLinkPtr. Set l->AttributeDriverHandle with it. If an 
   error occur, call dyn_DeinitializeDBLink and return an error code

   6. Call SQLAllocStmt to prepare the memory of l->attribute_priv.odbcSQLInfo.
   If an error occur, call dyn_DeinitializeDBLink and return an error code

   7. Call SQLPrepare to prepare the SQL request execution with the information
   in l->AttributeDriverLinkPtr.SelectionRequest.
   If an error occur, call dyn_DeinitializeDBLink and return an error code

   8. Get the quantity of attribute columns in the selection with 
   SQLColAttributes and set l->attribute_priv.nb_field.

   9. Set l->attribute_priv.isLinked to TRUE and return a success message.

   ********************************************************************
   */

int dyn_InitializeDBLink(s,l,error)
     ecs_Server *s;
     ecs_Layer *l;
     char **error;
{
  char buffer[512];
  unsigned char sqlmessage[SQL_MAX_MESSAGE_LENGTH];
  unsigned char sqlstate[32];
  SQLINTEGER truc;
  short length;
  PrivateODBCInfo *apriv;
  char **temp=NULL;
  
  l->attribute_priv = (void *) malloc(sizeof(PrivateODBCInfo));
  apriv = (PrivateODBCInfo *) l->attribute_priv;
  if (l->attribute_priv == NULL) {
    *error = odbcerrorlist[0];
    return 1;
  }
  apriv->nb_field = 0;
  apriv->isSelected = FALSE;
  apriv->attributes = NULL;

  if (odbc_count == 0) {
    if (SQLAllocEnv(&odbcEnv) != SQL_SUCCESS) {
      SQLError(odbcEnv, SQL_NULL_HDBC, SQL_NULL_HSTMT, sqlstate,
	       &truc, sqlmessage, SQL_MAX_MESSAGE_LENGTH - 1,
	       &length);
      *error = (char *)sqlmessage;
      dyn_DeinitializeDBLink(s,l,temp);
      return 1;			
    }

  }

  odbc_count++;
  apriv->isLinked = FALSE;
  l->AttributeDriverHandle = (void *) &(apriv->odbcHandle);

  if (SQLAllocConnect(odbcEnv,&(apriv->odbcHandle)) != SQL_SUCCESS) {
    SQLError(odbcEnv, SQL_NULL_HDBC, SQL_NULL_HSTMT, sqlstate,
	     &truc, sqlmessage, SQL_MAX_MESSAGE_LENGTH - 1,
	     &length); 

    *error = (char *)sqlmessage;
    dyn_DeinitializeDBLink(s,l,temp);
    return 1;
  }
  
  if (SQLConnect((SQLHDBC) apriv->odbcHandle, 
		 (SQLCHAR *) l->AttributeDriverLinkPtr->InformationSource,
		 (SQLSMALLINT) SQL_NTS, 
		 (SQLCHAR *) l->AttributeDriverLinkPtr->UserDescription, 
		 (SQLSMALLINT) SQL_NTS, 
		 (SQLCHAR *) l->AttributeDriverLinkPtr->AutorizationDescription, 
		 (SQLSMALLINT) SQL_NTS) != SQL_SUCCESS) {
    SQLError(odbcEnv, apriv->odbcHandle, SQL_NULL_HSTMT, sqlstate,
	     &truc, sqlmessage, SQL_MAX_MESSAGE_LENGTH - 1,
	     &length); 
    *error = (char *)sqlmessage;
    dyn_DeinitializeDBLink(s,l,temp);
    return 1;
  }

  if (SQLAllocStmt(apriv->odbcHandle,&(apriv->odbcSqlInfo)) != SQL_SUCCESS) {
    SQLError(odbcEnv, apriv->odbcHandle, SQL_NULL_HSTMT, sqlstate,
	     &truc, sqlmessage, SQL_MAX_MESSAGE_LENGTH - 1,
	     &length); 
    *error = (char *)sqlmessage;

    dyn_DeinitializeDBLink(s,l,temp);
    return 1;
  }
  
  if (SQLPrepare(apriv->odbcSqlInfo,(unsigned char *)(l->AttrRequest),SQL_NTS) != SQL_SUCCESS) {
    SQLError(odbcEnv, apriv->odbcHandle, SQL_NULL_HSTMT, sqlstate,
	     &truc, sqlmessage, SQL_MAX_MESSAGE_LENGTH - 1,
	     &length); 
    *error = (char *)sqlmessage;
    dyn_DeinitializeDBLink(s,l,temp);
    return 1;
  }

  SQLColAttributes(apriv->odbcSqlInfo,0,SQL_COLUMN_COUNT,
		   buffer,255,&length,(SQLINTEGER *)&(apriv->nb_field));
  apriv->isLinked = TRUE;  

  return 0;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_DeinitializeDBLink

   DESCRIPTION
      Deinitialize the database link 
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server object attributes
	 ecs_Layer *l: Layer object attributes
      OUTPUT
	 char **error: Error message (if it's the case)
   END_PARAMETERS

   RETURN_VALUE
      int : An boolean code. 0 if the operation is a success. a code if the operation failed.

   END_FUNCTION_INFORMATION

   PSEUDO_CODE

   1. If the selection is link (l->attribute_priv.isLinked).
   Begin
   
      1.1. Call SQLFreeStmt
      1.2. Call SQLDisconnect
      1.3. Call SQLFreeConnect

   End

   2. Decrement odbc_count

   3. If the odbc_count is to 0, call SQLFreeEnv

   ********************************************************************
   */

int dyn_DeinitializeDBLink(s,l,error)
     ecs_Server *s;
     ecs_Layer *l;
     char **error;
{
  PrivateODBCInfo *apriv;

  apriv = (PrivateODBCInfo *) l->attribute_priv;
  if (apriv->isLinked == TRUE) {
    SQLFreeStmt(apriv->odbcSqlInfo,SQL_DROP);
    SQLDisconnect(apriv->odbcHandle);
    SQLFreeConnect(apriv->odbcHandle);
  }

  if (apriv != NULL)
    free(apriv);
  l->attribute_priv = NULL;
  l->AttributeDriverHandle = NULL;
  odbc_count--;
  if (odbc_count <= 0) {
    SQLFreeEnv(odbcEnv);
  }

  return 0;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_GetColumnsInfo

   DESCRIPTION
      Get the column information from the selection string
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server object attributes
	 ecs_Layer *l: Layer object attributes
      OUTPUT
	 int *columns_qty: Quantity of columns defined in attr
	 ecs_ObjAttribute **attr: A list of attribute information
	 char **error: Error message (if it's the case)
   END_PARAMETERS

   RETURN_VALUE
      int : An boolean code. 0 if the operation is a success. a code if the operation failed.

   END_FUNCTION_INFORMATION

   PSEUDO_CODE

   1. Initialize a table of l->attribute_priv.nb_field elements ecs_ObjAttribute
   in attr.

   2. For each field i
   Begin

      2.1. Call SQLColAttributes to retreive the name of the attribute and set
      it in attr[i].name.

      2.1. Call SQLColAttributes to retreive the type of the attribute and set
      it in attr[i].type.

      2.1. Call SQLColAttributes to retreive the lenght of the attribute and set
      it in attr[i].lenght.

      2.1. Call SQLColAttributes to retreive the precision of the attribute and set
      it in attr[i].precision.

      2.1. Call SQLColAttributes to retreive the nullable of the attribute and set
      it in attr[i].nullable.

   End

   3. Set columns_qty with l->attribute_priv.nb_field

   4. Return a success

   ********************************************************************
   */

int dyn_GetColumnsInfo(s,l,columns_qty,attr,error)
     ecs_Server *s;
     ecs_Layer *l;
     int *columns_qty;
     ecs_ObjAttribute **attr;
     char **error;
{
  int i,j;
  PrivateODBCInfo *apriv = (PrivateODBCInfo *) l->attribute_priv;
  int buffer[514];
  SDWORD count;
  char name[33];
  SWORD length;
  int precision;
  int type;
  int nullable;
  int readlength;

  *attr = malloc(sizeof(ecs_ObjAttribute) * apriv->nb_field);
  if (*attr == NULL) {
    *error = odbcerrorlist[0];
    return 1;
  }

  for(i=0;i<apriv->nb_field;++i) {
    SQLColAttributes((SQLHSTMT) apriv->odbcSqlInfo,(SQLUSMALLINT) (i+1),SQL_COLUMN_NAME,
		     name,32,&length,&count);
    SQLColAttributes((SQLHSTMT) apriv->odbcSqlInfo,(SQLUSMALLINT) (i+1),SQL_COLUMN_TYPE,
		     buffer,513,&length,(SQLINTEGER *)&(type));
    SQLColAttributes((SQLHSTMT) apriv->odbcSqlInfo,(SQLUSMALLINT) (i+1),SQL_COLUMN_LENGTH,
		     buffer,513,&length,(SQLINTEGER *)&(readlength));    
    SQLColAttributes((SQLHSTMT) apriv->odbcSqlInfo,(SQLUSMALLINT) (i+1),SQL_COLUMN_PRECISION,
		     buffer,513,&length,(SQLINTEGER *)&(precision));
    SQLColAttributes((SQLHSTMT) apriv->odbcSqlInfo,(SQLUSMALLINT) (i+1),SQL_COLUMN_NULLABLE,
		     buffer,513,&length,(SQLINTEGER *)&(nullable));

    (*attr)[i].name = malloc(strlen(name)+1);
    if ((*attr)[i].name == NULL) {
      for(j=0;j<i;j++)
	free((*attr)[j].name);
      free(*attr);
      *error = odbcerrorlist[0];
      return 1;

    }
    strcpy((*attr)[i].name,name);

    switch(type) {
    case SQL_CHAR: (*attr)[i].type = Char;
      break;
    case SQL_VARCHAR: (*attr)[i].type = Varchar;
      break;
    case SQL_LONGVARCHAR: (*attr)[i].type = Longvarchar;
      break;		
    case SQL_DECIMAL: (*attr)[i].type = Decimal;
      break;	
    case SQL_NUMERIC: (*attr)[i].type = Numeric;
      break;	
    case SQL_SMALLINT: (*attr)[i].type = Smallint;
      break;
    case SQL_INTEGER: (*attr)[i].type = Integer;
      break;
    case SQL_REAL: (*attr)[i].type = Real;
      break;
    case SQL_FLOAT: (*attr)[i].type = Float;
      break;
    case SQL_DOUBLE: (*attr)[i].type = Double;
      break;
    }

    (*attr)[i].lenght = readlength;
    (*attr)[i].precision = precision;
    (*attr)[i].nullable = nullable;
  }

  *columns_qty = apriv->nb_field;

  return 0;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_SelectAttributes

   DESCRIPTION
      Select the attributes defined in the selection request with
      the attributes set in the arguments.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server object attributes
	 ecs_Layer *l: Layer object attributes
	 int attribute_qty: The lenght of the attribute list
	 char **attribute_list: List of attributes to be bind with the selection request
      OUTPUT
	 char **error: Error message (if it's the case)
   END_PARAMETERS

   RETURN_VALUE
      int : An boolean code. 0 if the operation is a success. a code if the operation failed.

   END_FUNCTION_INFORMATION

   PSEUDO_CODE

   1. For each attribute in attribute_list
   Begin
      1.1. Call SQLBindParameter with this attribute. If an error occur, 
      return an error message
   End
   2. Call SQLExecute, if an error occur, return an error message
   3. Call SQLFetch. If the result is a success, set l->attribute_priv.isSelected to
   true. If the return code is SQL_NO_DATA_FOUND, set it to false. Else, return an error
   message.
   4. Free the old l->attribute_priv.attribute if it's different than NULL.
   Set the l->attribute_priv.attributes to NULL
   5. If l->attribute_priv.isSelected is true
   Begin
      5.1. For each field i
      Begin
         Call SQLGetData for this field and concatenate the result to l->attribute_priv.attributes
      End
   End
   6. Return 0

   ********************************************************************
   */

int dyn_SelectAttributes(s,l,attribute_qty,attribute_list,error)
     ecs_Server *s;
     ecs_Layer *l;
     int attribute_qty;
     char **attribute_list;
     char **error;
{
  int i;
  PrivateODBCInfo *apriv = (PrivateODBCInfo *) l->attribute_priv;
  char theKey[128];
  char buffer[1024],buffer2[256];
  unsigned char sqlmessage[SQL_MAX_MESSAGE_LENGTH];
  unsigned char sqlstate[32];
  int truc;
  SDWORD length;
  short collength;
  RETCODE retcode;
  short count;
  int type;

  for(i=0;i<attribute_qty;i++) {
    strcpy(theKey,attribute_list[i]);
    length = SQL_NTS;
    if (SQLBindParameter(apriv->odbcSqlInfo, (SQLUSMALLINT) (i+1), SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 127, 0,
			 (SQLPOINTER) theKey, (SQLINTEGER) 0, (SQLINTEGER *) &length) != SQL_SUCCESS) {
      SQLError(odbcEnv, apriv->odbcHandle, apriv->odbcSqlInfo, sqlstate,
	       (SQLINTEGER *)&truc, sqlmessage, SQL_MAX_MESSAGE_LENGTH - 1,
	       &count); 
      *error = (char *)sqlmessage;
      return 1;
    }
  }

  if (SQLExecute(apriv->odbcSqlInfo) != SQL_SUCCESS) {
    SQLError(odbcEnv, apriv->odbcHandle, apriv->odbcSqlInfo, sqlstate,
	     (SQLINTEGER *)&truc, sqlmessage, SQL_MAX_MESSAGE_LENGTH - 1,
	     &count); 
    *error = (char *)sqlmessage;
    SQLFreeStmt(apriv->odbcSqlInfo,SQL_CLOSE);
    return 1;
  } 

  /*
    3. Call SQLFetch. If the result is a success, set l->attribute_priv.isSelected to
    true. If the return code is SQL_NO_DATA_FOUND, set it to false. Else, return an error
    message.
    */

  retcode = SQLFetch(apriv->odbcSqlInfo); 
  if ((retcode == SQL_SUCCESS) || (retcode == SQL_SUCCESS_WITH_INFO)) {
    apriv->isSelected = TRUE;
  } else {
    if (retcode == SQL_NO_DATA_FOUND) {
      apriv->isSelected = FALSE;
    } else {
      SQLError(odbcEnv, apriv->odbcHandle, apriv->odbcSqlInfo, sqlstate,
	       (SQLINTEGER *)&truc, sqlmessage, SQL_MAX_MESSAGE_LENGTH - 1,
	       &count); 
      SQLFreeStmt(apriv->odbcSqlInfo,SQL_CLOSE);
      return 1;
    }
  }

  /*
    4. Free the old l->attribute_priv.attribute if it's different than NULL.
    Set the l->attribute_priv.attributes to NULL
    */

  if (apriv->attributes != NULL)
    free(apriv->attributes);
  apriv->attributes = NULL;
  strcpy(buffer,"");

  if (apriv->isSelected == TRUE) {
    for(i = 0; i < apriv->nb_field; ++i) {
      SQLGetData(apriv->odbcSqlInfo,(SQLUSMALLINT) (i+1), SQL_C_CHAR,
		 buffer2, 255, &length);
      SQLColAttributes(apriv->odbcSqlInfo,(SQLUSMALLINT) (i+1),SQL_COLUMN_TYPE,
		       buffer,32,&collength,(SQLINTEGER *)&(type));
      
      if ((type < 2) || (type > 8)) {
	sprintf(&(buffer[strlen(buffer)]),"{%s} ",buffer2);
      } else  {
	sprintf(&(buffer[strlen(buffer)]),"%s ",buffer2);
      }
    }

    apriv->attributes = malloc(strlen(buffer)+1);
    if (apriv->attributes == NULL) {
      SQLFreeStmt(apriv->odbcSqlInfo,SQL_CLOSE);
      *error = odbcerrorlist[0];
      return 1;
    }
    strcpy(apriv->attributes,buffer);
  }

  SQLFreeStmt(apriv->odbcSqlInfo,SQL_CLOSE);  

  return 0;

}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_IsSelected

   DESCRIPTION
      This method indicate to the ogdi if the function is inside the selection
      request execute in SelectAttributes
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server object attributes
	 ecs_Layer *l: Layer object attributes
      OUTPUT
	 short *isSelected: Indication if the current object 
	 char **error: Error message (if it's the case)
   END_PARAMETERS

   RETURN_VALUE
      int : An boolean code. 0 if the operation is a success. a code if the operation failed.

   END_FUNCTION_INFORMATION

   PSEUDO_CODE

   1. Return l->attribute_priv.isSelected

   ********************************************************************
   */

int dyn_IsSelected(s,l,isSelected,error)
     ecs_Server *s;
     ecs_Layer *l;
     short *isSelected;
     char **error;
{
  PrivateODBCInfo *apriv = (PrivateODBCInfo *) l->attribute_priv;
  *isSelected = apriv->isSelected;
  return 0;
}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      dyn_GetSelectedAttributes

   DESCRIPTION
      Get the record in the table selected previously with dyn_SelectAttributes
   END_DESCRIPTION

   PARAMETERS
      INPUT
         ecs_Server *s: Server object attributes
	 ecs_Layer *l: Layer object attributes
      OUTPUT
         char **attributes: A string with all the attributes
	 char **error: Error message (if it's the case)
   END_PARAMETERS

   RETURN_VALUE
      int : An boolean code. 0 if the operation is a success. a code if the operation failed.

   END_FUNCTION_INFORMATION

   PSEUDO_CODE

   1. Return l->attribute_priv.attributes

   ********************************************************************
   */

int dyn_GetSelectedAttributes(s,l,attributes,error)
     ecs_Server *s;
     ecs_Layer *l;
     char **attributes;
     char **error;
{
  PrivateODBCInfo *apriv = (PrivateODBCInfo *) l->attribute_priv;
  *attributes = apriv->attributes;
  return 0;
}







