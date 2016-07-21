// Sample: Process Topologies
//
// Copyright 2015 ESRI
// 
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
// 
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
// 
// See the use restrictions at <your File Geodatabase API install location>/license/userestrictions.txt.
// 

#include "LibXMLSession.h"
#include <libxml/parser.h>

LibXMLSession::LibXMLSession(void)
{
  // set up libxml
  xmlInitMemory();
  xmlInitParser();
}

LibXMLSession::~LibXMLSession(void)
{
  //tear down libxml
  xmlCleanupParser();
}
