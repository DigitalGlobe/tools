/*********************************************************************

  CSOURCE_INFORMATION
  
  NAME
     odbc.h

  DESCRIPTION
     Header file of the odbc attribute driver
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

#ifndef ODBCDRIVER
#define ODBCDRIVER

#include "ecs.h"
#include "sql.h"
#include "sqlext.h"

/*********************************************************************

  STRUCTURE_INFORMATION
  
  NAME
     PrivateODBCInfo

  DESCRIPTION
     The private information of this attribute driver. 
  END_DESCRIPTION

  ATTRIBUTES
     HSTMT odbcSqlInfo: SQL request handler
     int nb_field: Field quantity in the odbcSqlInfo structure
     int isLinked: Indicate if a link with odbc is active
     int isSelected: Indicate if the current request get something in it
     char *attributes: The attribute list.
  END_ATTRIBUTES

  END_STRUCTURE_INFORMATION

  ********************************************************************/


typedef struct {
  SQLHDBC odbcHandle;
  SQLHSTMT odbcSqlInfo;
  int nb_field;
  int isLinked;
  int isSelected;
  char *attributes;
} PrivateODBCInfo;

#endif






