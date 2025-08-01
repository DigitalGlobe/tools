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

#pragma once
#include <string>
#include <vector>

class TopologyInterpreter;

struct TopologyRelationshipRecord
{
  std::string OriginName;
  std::string DestinationName;
  std::string Relationship;

  TopologyRelationshipRecord(std::string originName, std::string destinationName, std::string relationship)
    : OriginName      (originName),
      DestinationName (destinationName),
      Relationship    (relationship)
  { }

  TopologyRelationshipRecord(const TopologyRelationshipRecord& other) 
    : OriginName      (other.OriginName),
      DestinationName (other.DestinationName),
      Relationship    (other.Relationship)
  {  }

  TopologyRelationshipRecord& operator=(const TopologyRelationshipRecord& other) 
  {
    if (this != &other)
    {
      OriginName = other.OriginName;
      DestinationName = other.DestinationName;
      Relationship = other.Relationship;
    }
    return *this;
  }

  ~TopologyRelationshipRecord(){}
private:
  // needs friendship in order to access private constructor
  friend class TopologyInterpreter;
  TopologyRelationshipRecord()
    : OriginName      (""),
      DestinationName (""),
      Relationship    ("")
  { }
};

class TopologyInterpreter
{
public:
  TopologyInterpreter(const std::string& topologyXML, const std::vector<std::string>& fcDefinitions);
  ~TopologyInterpreter(void);

  std::vector<TopologyRelationshipRecord> Records;
  std::string Name;

  TopologyInterpreter(const TopologyInterpreter& other){}
  TopologyInterpreter& operator= (const TopologyInterpreter& other){ return *this;}
};
