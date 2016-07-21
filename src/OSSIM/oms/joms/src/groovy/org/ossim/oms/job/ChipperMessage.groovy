package org.ossim.oms.job
import groovy.json.JsonBuilder
import groovy.json.JsonSlurper

class ChipperMessage implements Message
{
	String type = "ChipperMessage"
	String jobDir
	String jobId
	ChipperParams chipperParams
	ArchiveParams archiveParams

	static ChipperMessage newMessage(def options=[:])
	{
	  def result = new ChipperMessage(options)
	  
	  result.jobId =  UUID.randomUUID()
	  
	  result
	}
	String getType(){
		type
	}
	void setType(String type)
	{
		this.type = type
	}
	String getId()
	{
		this.jobId
	}
   String getJobId()
   {
   	this.jobId 
   }
   void setId(String id)
   {
   	this.jobId = id
   }
   void setJobId(String jobId)
	{
		this.jobId = jobId
	}
	String toJsonString()
	{
	  def tempMap = [:]
	  if(chipperParams)
	  {
	      tempMap = chipperParams.toMap()
	  }
	  if(archiveParams)
	  {
			tempMap.archive = archiveParams.toMap() 

	  }
	  tempMap.jobId  = jobId
	  tempMap.jobDir = jobDir
	  tempMap.type   = type
	  
	  def jsonBuilder = new JsonBuilder( tempMap )
	  
	  jsonBuilder.toString()           
	}

	def fromJson(def jsonObj) throws Exception
	{
	   archiveParams = new ArchiveParams()
	   chipperParams = new ChipperParams()         
	   if(jsonObj.type != type) throw new Exception("Message not of type ChipperMessage")
	   this.jobId  = jsonObj.jobId?:""
	   this.jobDir = jsonObj.jobDir?:null

	   jsonObj.inputs?.each{input->
	      def localFile = new LocalFile(file:input.file)
	         
	      input.entries.each{entry->
	          FileEntry fileEntry = new FileEntry(entry)
	          
	          localFile.addEntry(fileEntry)
	      }

	      chipperParams.addInput(localFile) 
	   }
	   if(jsonObj?.options?.srs) chipperParams.srs = jsonObj.options.srs
	   if(jsonObj?.options?.scale_2_8_bit) chipperParams.scale_2_8_bit = jsonObj.options.scale_2_8_bit
	   if(jsonObj?.options?.resampler_filter) chipperParams.resampler_filter = jsonObj.options.resampler_filter
	   if(jsonObj?.options?.combiner_type) chipperParams.combiner_type = jsonObj.options.combiner_type
	   if(jsonObj?.options?.operation) chipperParams.operation = jsonObj.options.operation
	   if(jsonObj?.options?.cut_wms_bbox_ll) chipperParams.cut_wms_bbox_ll = jsonObj.options.cut_wms_bbox_ll
	   if(jsonObj?.options?.meters) chipperParams.meters = jsonObj.options.meters
	   if(jsonObj?.options?.hist_op) chipperParams.hist_op = jsonObj.options.hist_op
	   if(jsonObj?.options?.output_file) chipperParams.output_file = jsonObj.options.output_file
	   if(jsonObj?.options?.writer) chipperParams.writer = jsonObj.options.writer
	   if(jsonObj?.writerProperties)
	   {
			chipperParams.writerProperties = jsonObj.writerProperties as HashMap

			chipperParams.writerProperties.each{k,v->
				chipperParams.writerProperties."${k}" = v.toString();
			}

	   }
	   if(jsonObj.archive)
	   {
	   	if(jsonObj.archive.type!=null) archiveParams.type = jsonObj.archive.type.toString() 
	   	if(jsonObj.archive.deleteInputAfterArchiving!=null){
	   		archiveParams.deleteInputAfterArchiving = "${jsonObj.archive.deleteInputAfterArchiving}".toBoolean() 
	   	}

	   	if(jsonObj.archive.inputFile!=null) {
				archiveParams.inputFile = jsonObj.archive.inputFile.toString()
	   	} 
	   	else
	   	{
	   		archiveParams.inputFile = jobDir	   		
	   	}

	   }

	   this
	}
	def fromJsonString(String jsonString) throws Exception
	{
	   def slurper = new JsonSlurper()
	   def jsonObj = slurper.parseText(jsonString)

	   fromJson(jsonObj)
	}
}
