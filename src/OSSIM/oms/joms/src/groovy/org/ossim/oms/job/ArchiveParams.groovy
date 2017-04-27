package org.ossim.oms.job

import java.io.OutputStream

class ArchiveParams
{
	/**
	*  values can be : <empty>, "zip" or "tgz"
	*/
	String type

	/**
	* The input directory to archive
	*/
	String inputFile

	boolean deleteInputAfterArchiving

	Archive newArchive()
	{
		new Archive(archiveParams:this)
	}
	def getInputFileAsFile()
	{
		inputFile as File
	}
	def getDefaultArchiveName()
	{
		def result

		if(inputFile)
		{
			result = inputFileAsFile.name
		}

		result
	}
	def getDefaultOutputFile()
	{
		"${inputFileAsFile.path}.${type}".toString()
	}
	def toMap()
	{
		[
			type:type,
			inputFile:inputFile,
			deleteInputAfterArchiving:deleteInputAfterArchiving
		]
	}
}