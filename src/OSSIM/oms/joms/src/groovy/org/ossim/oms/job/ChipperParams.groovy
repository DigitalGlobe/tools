package org.ossim.oms.job
import joms.oms.Chipper

class ChipperParams{
	def inputs = [] 
	String srs = "EPSG:4326"
	String combiner_type = "ossimImageMosaic" // keyword: combiner_type values: any combiner such as ossimImageMosaic, ossimBlendMosaic, ossimFeatherMosaic
	String operation = "ortho"  // keyword: operation values: chip,hillshade,color-relief,ortho,psm, or 2cmv
	String cut_wms_bbox_ll // keyword cut_wms_bbox_ll
	String meters  // keyword meters
	String hist_op // keyword: hist_op and values: auto-minmax, std-stretch-1, std-stretch-2, or std-stretch-3
	String output_file
	String resampler_filter = "bilinear"
	String scale_2_8_bit = "false"
	String writer

	def writerProperties = [:]
	void setCutWmsBbox(double minx, double miny, double maxx, double maxy)
	{
	  cut_wms_bbox_ll = "${minx},${miny},${maxx},${maxy}".toString()
	}  
	def extractWmsBbox()
	{
	  def result
	  
	  if(cut_wms_bbox_ll)
	  {
	      def splitArray = cut_wms_bbox_ll.split(",")
	      def minx, miny, maxx, maxy
	      
	      (minx,miny,maxx,maxy) = splitArray
	      
	      result = [minx:minx,miny:miny,maxx:maxx,maxy:maxy]
	  }
	  result
	}
	def addInput(LocalFile file)
	{
	  inputs << file

	  this
	}

	def toMap()
	{
	  def inputArrayMap = []
	  def result = [:]
	  def options = convertOptionsToMap()
	  inputs.each{input->inputArrayMap << input.toMap()}
	  result.inputs = inputArrayMap
	  
	  result.options = options
	  
	  result.writerProperties = writerProperties      

	  result  
	}
	def convertOptionsToMap()
	{
		def options = [:]

		if(meters) options.meters="${meters}".toString()
		options.combiner_type=combiner_type.toString()
		if(srs) options.srs=srs.toString()        
		if(cut_wms_bbox_ll) options.cut_wms_bbox_ll=cut_wms_bbox_ll.toString()        
		if(output_file) options.output_file=output_file.toString()
		if(writer) options.writer=writer.toString()
		if(operation) options.operation=operation.toString()
		if(resampler_filter) options.resampler_filter=resampler_filter.toString()
		if(scale_2_8_bit) options.scale_2_8_bit=scale_2_8_bit.toString()

		options
	}
	Chipper newChipper()
	{
		def result = new Chipper()

		if(!result.initialize(toInitializationParams()))
		{
			result.delete()
			result=null
		}

		result
	}
	def toInitializationParams()
	{
		def result = convertOptionsToMap()
		int idx = 0
		inputs.each{f->
		   f.entries.each{entry->
		       result."image${idx}.file"  = "${f.file}".toString()        
		       result."image${idx}.entry" = "${entry.entryId}".toString()
		       ++idx
		   }        
		}
		writerProperties?.eachWithIndex{k,v,i->result."writer_property${i}" = "${k}=${v}".toString()}
	 
	 result
	}
}