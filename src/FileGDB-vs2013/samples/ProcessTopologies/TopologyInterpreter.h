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
