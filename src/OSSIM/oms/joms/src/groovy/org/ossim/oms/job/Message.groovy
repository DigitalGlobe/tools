package org.ossim.oms.job
import groovy.json.JsonSlurper

interface Message
{
   String getType()
   String getId()
   void setId(String id)
   //def execute()
   //void abort()
   String toJsonString()
   def fromJsonString(String jsonString) throws Exception
   def fromJson(def obj) throws Exception
}
