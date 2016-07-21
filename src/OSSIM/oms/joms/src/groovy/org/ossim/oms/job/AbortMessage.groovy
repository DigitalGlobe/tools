package org.ossim.oms.job
import groovy.json.JsonBuilder
import groovy.json.JsonSlurper

class AbortMessage{
	def jobId

	def toJsonString()
	{
	  def tempMap = [type:"AbortMessage", jobId:jobId]
	  
	  def jsonBuilder = new JsonBuilder( tempMap )
	  
	  jsonBuilder.toString()           
	}
	def fromJsonString(def jsonString) throws Exception
	{
		def slurper = new JsonSlurper()
	   def jsonObj = slurper.parseText(jsonString)
	   jobId = jsonObj.jobId
	}
}