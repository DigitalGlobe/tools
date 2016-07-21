package org.ossim.oms.job

class LocalFile
{
	def file
	def entries = []

	def addEntry(FileEntry entry)
	{
	  entries << entry

	  this
	}
	def toMap()
	{
	  def result = [file:file]
	  def entryList = []
	  entries.each{entry->
	      entryList<<entry.toMap()
	  }
	  
	  result.entries = entryList
	  result.type = "LOCAL_FILE"
	  result
	}

}
