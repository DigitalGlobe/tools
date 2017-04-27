//
// TableSchemaXML.h

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

// A helper method for creating a field XML definition.
static void CreateStreetTypeField(std::string& fieldDef);

// A helper method for creating an index XML definition.
static void CreateStreetTypeIndex(std::string& indexDef);

// A helper method for creating a subtype XML definition.
static void CreateSubtypeDefinition(std::string& subtypeDef);

// A helper method for creating a altered subtype XML definition.
static void CreateSubtypeDefinitionAltered(std::string& subtypeDef);

// A helper method for creating the second subtype XML definition.
static void CreateSubtypeDefinition2(std::string& subtypeDef);

// A helper method for creating a domain
static void CreateDomainDefinition(std::string& domainDef);