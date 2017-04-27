#!/usr/bin/env python
import socket
	
def addfile(output,host,dport):
	ossim_data_xml = "<Add target=':idolbridge'><Image groupType='groundTexture'><filename>%s</filename> <id>%s</id><name>%s</name></Image></Add>" % (output,output,output)
	ossimdata = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	ossimdata.connect((host, int(dport)))
	ossimdata.send(ossim_data_xml)
	ossimdata.close()
	
def zoomto(lon,lat,distance,host,pport):
	ossim_zoom_xml = '<Set target=":navigator" vref="wgs84"><Camera><longitude>%s</longitude><latitude>%s</latitude><altitude>%s</altitude><heading>0</heading><pitch>0</pitch><roll>0</roll><altitudeMode>absolute</altitudeMode><range>%s</range></Camera></Set>' % (lon, lat, distance, distance)
	ossimposition = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	ossimposition.connect((host, int(pport)))  
	ossimposition.send(ossim_zoom_xml)
	ossimposition.close()
	
def removefile(output,host,dport):
	ossimdata = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	ossimdata.connect((host, int(dport)))
	ossim_data_xml = "<Remove target=':idolbridge' id='%s' />" % (output)
	ossimdata.send(ossim_data_xml)
	ossimdata.close()
	
	
def addzoom(output,lon,lat,distance,host,dport,pport):
	addfile(output,host,dport)
	zoomto(lon,lat,distance,host,pport)


addzoom('/data/florida/Brevard.tif',-81.0009,29.0009,15000,'localhost',8000,7000)
removefile('/data/florida/Brevard.tif','localhost',8000)
