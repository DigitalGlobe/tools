package org.ossim.oms.job

class FileEntry
{
	def entryId
	def histogram
	def overview
	def geometry
	def omd
	def validVertices

	def toMap()
	{
	  def result = [entryId:entryId]

	  if(histogram) result.histogram = histogram
	  if(overview) result.overview   = overview
	  if(geometry) result.geometry   = geometry
	  if(omd) result.omd             = omd
	  if(validVertices) result.validVertices = validVertices

	  result
	}
}