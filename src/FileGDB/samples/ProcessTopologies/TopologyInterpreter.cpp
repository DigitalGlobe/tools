// Sample: Process Topologies
//
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

#include "TopologyInterpreter.h"
#include <libxml/parser.h>
#include <map>

namespace
{
  // manages proper creation and teardown of a libXML document - declare on stack
  struct LibXMLDocManager
  {
    LibXMLDocManager(void)
    {
      Doc = xmlNewDoc(BAD_CAST "1.0");
    }

    LibXMLDocManager(const std::string& sourceXML)
    {
      const xmlChar* pBytes = reinterpret_cast<const xmlChar*>(sourceXML.c_str());
      Doc = xmlReadDoc(pBytes, "noname.xml", 0, 0);
    }

    ~LibXMLDocManager()
    {
      xmlFreeDoc(Doc);
    }

    xmlDocPtr Doc;

  private:
    LibXMLDocManager(const LibXMLDocManager&)
    {      /*don't do this*/    }

    LibXMLDocManager& operator=(const LibXMLDocManager&)
    {      /*don't do this*/  return *this;    }
  };

  void GetContent(xmlNode* node, std::string& text)
  {
    (0 != node) && (0 != node->children) && (0 != node->children->content ) 
      ? text = reinterpret_cast<char*>(node->children->content) 
      : text = "";
  }

  xmlNode* FindNode(xmlNode* node, const xmlChar* nodeName)
  {
    xmlNode* iterator = node;
    while ((0 != iterator) && (xmlStrcmp(iterator->name, nodeName)))
    {
      iterator = iterator->next;
    }
    return iterator;
  }

  void AddMapping(const std::string& fcXML, std::map<std::string, std::string>& mapping)
  {
    LibXMLDocManager fcDoc(fcXML);
    xmlNode* root_element = xmlDocGetRootElement(fcDoc.Doc);
    xmlNode* dsidNode = FindNode(root_element->children, reinterpret_cast<const xmlChar*>("DSID"));
    std::string dsid;
    GetContent(dsidNode, dsid);
    xmlNode* nameNode = FindNode(root_element->children, reinterpret_cast<const xmlChar*>("Name"));
    std::string name;
    GetContent(nameNode, name);
    if(mapping.end() == mapping.find(dsid))
      mapping.insert(std::pair<std::string, std::string>(dsid, name));
  }
};

TopologyInterpreter::TopologyInterpreter(const std::string& topologyXML, const std::vector<std::string>& fcDefinitions)
{
  //build map to translate
  std::map<std::string, std::string> numbersToNames;
  for (size_t idx = 0; idx < fcDefinitions.size(); idx++)
    AddMapping(fcDefinitions[idx], numbersToNames);

  LibXMLDocManager topologyMgr(topologyXML);
  xmlNode* root_element = xmlDocGetRootElement(topologyMgr.Doc);

  // get the topology name
  xmlNode* nameNode = FindNode(root_element->children, reinterpret_cast<const xmlChar*>("Name"));
  GetContent(nameNode, Name);

  xmlNode* rulesNode = FindNode(root_element->children, reinterpret_cast<const xmlChar*>("TopologyRules"));
  xmlNode* ruleNode = rulesNode->children;

  while (0 != ruleNode)
  {
    xmlNode* searchNode = FindNode(ruleNode->children, reinterpret_cast<const xmlChar*>("TopologyRuleType"));
    std::string rule;
    GetContent(searchNode, rule);

    searchNode = FindNode(searchNode, reinterpret_cast<const xmlChar*>("OriginClassID"));
    std::string originClassID;
    GetContent(searchNode, originClassID);

    searchNode = FindNode(searchNode, reinterpret_cast<const xmlChar*>("DestinationClassID"));
    std::string destinationClassID;
    GetContent(searchNode, destinationClassID);

    Records.push_back(TopologyRelationshipRecord(numbersToNames[originClassID], 
                                                 numbersToNames[destinationClassID], 
                                                 rule));
    // iterate
    ruleNode = ruleNode->next;
  } // while loop on rules

}

TopologyInterpreter::~TopologyInterpreter(void)
{ }
