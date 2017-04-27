package org.ossim.oms.job

import org.apache.commons.compress.archivers.tar.TarArchiveOutputStream 
import org.apache.commons.compress.archivers.zip.ZipArchiveOutputStream
import org.apache.commons.compress.archivers.ArchiveOutputStream
import org.apache.commons.compress.compressors.gzip.GzipCompressorOutputStream
import java.io.OutputStream
import java.io.InputStream
import org.ossim.oms.job.ArchiveParams

class Archive
{
	ArchiveParams archiveParams
	/**
	*
	*/
	private void writeStreamToOutputStream( InputStream inputStream, ArchiveOutputStream output, 
														 Integer blockSize = 4096 )
	{
		byte[] buffer = new byte[blockSize]; // To hold file contents
		int bytes_read; // How many bytes in buffer
		while ( ( bytes_read = inputStream.read( buffer ) ) != -1 )
		{
		  output.write( buffer, 0, bytes_read ); // write
		}
	}	
	/**
	*
	*/
	private void writeFileToOutputStream( File inputFile, OutputStream output, Integer blockSize = 4096 )
	{
		def from = new FileInputStream( inputFile ); // Create input stream
		writeStreamToOutputStream( from, output, blockSize )
		from.close()
		from = null
	}
	/**
	* recurse and write all files to the archive stream
	*/
	private void archiveDir(File dir, def archiveOs, String relativePath = "")
	{
	    def suffix = relativePath?:""
	    def dirPath = dir.path as File
	    //def name = dirPath.name as File

	    //println "${dirPath} and SUFFIX = ${suffix}"
	    dir.list().each{ file->
	        def inputFile = new File(dirPath,file)
	        def entryName = new File(suffix, file).toString()
	        //println inputFile
	        if(inputFile.isFile())
	        {   
	           def entry = archiveOs.createArchiveEntry(inputFile, entryName);  
	           archiveOs.putArchiveEntry(entry)
	           writeFileToOutputStream(inputFile, archiveOs);
	           archiveOs.closeArchiveEntry()
	        }
	        else if(inputFile.isDirectory())
	        {
	           def entry = archiveOs.createArchiveEntry(inputFile, entryName);  
	           archiveOs.putArchiveEntry(entry)
	           //writeFileToOutputStream(inputFile, archiveOs);
	           archiveOs.closeArchiveEntry()
	           
	           archiveDir(new File(dir, file), archiveOs, new File(relativePath, file).toString())             
	        }
	    }
	}
	private void deleteEachFile(File file)
	{
		if(file.isFile())
		{
			file.delete()
		}
		else if(file.isDirectory())
		{
			file.eachFile{deleteEachFile(it as File)}
			file.delete()
		}
	}
	void deleteInput()
	{
		deleteEachFile(archiveParams.inputFileAsFile);
	}
	def execute()
	{
		def result = false
		def outputStream
		def archiveOs
		def os
		if(archiveParams)
		{
			def archiveName = archiveParams.getDefaultArchiveName()
			if(archiveName)
			{
				def outputFile = archiveParams.getDefaultOutputFile()
				outputStream = new FileOutputStream(outputFile)
				def inputFile = archiveParams.inputFileAsFile
				if(outputStream&&inputFile.isDirectory())
				{
					def inputName = archiveParams.getDefaultArchiveName()

					switch(archiveParams.type.toLowerCase())
					{
						case "zip":
							archiveOs = new ZipArchiveOutputStream(outputStream)
							result = true
							break
						case "tgz":
						 	os = new GzipCompressorOutputStream(outputStream)
    						archiveOs = new TarArchiveOutputStream(os)
 							result = true
							break
					}					
					def entry = archiveOs.createArchiveEntry(inputFile, inputName)
					archiveOs?.putArchiveEntry(entry)   
					archiveOs?.closeArchiveEntry()       
					archiveDir(inputFile, archiveOs, inputName) 
					archiveOs?.finish()
					archiveOs?.close()
					os?.close()
					outputStream?.close()
					if(archiveParams.deleteInputAfterArchiving)
					{
						deleteInput()
					}
				}
			}
		}
		else
		{

		}
		result
	}
}