//
// TableSchemaXML.h

/*
   Copyright © 2025 Esri

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

   A local copy of the license and additional notices are located with the
   source distribution at:

   http://github.com/Esri/file-geodatabase-api/FileGDB_API_1.5.1
*/

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