//
// Copyright 2017 ESRI
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

#pragma once

class LibXMLSession
{
public:
  LibXMLSession(void);
  ~LibXMLSession(void);
private:
  LibXMLSession(const LibXMLSession& other){}
  LibXMLSession& operator=(const LibXMLSession& other){return *this;}
};
