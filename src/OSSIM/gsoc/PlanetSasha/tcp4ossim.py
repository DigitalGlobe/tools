#!/usr/bin/env python
import socket
from lxml import etree
	
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


def parseSignal(data):
	try :
		xmldoc = etree.fromstring(data)
		lon = xmldoc.xpath("//Camera/longitude/text()")
		lat = xmldoc.xpath("//Camera/latitude/text()")
		rl = xmldoc.xpath("//Camera/roll/text()")
		pit = xmldoc.xpath("//Camera/pitch/text()")
		head = xmldoc.xpath("//Camera/heading/text()")
		altitude = xmldoc.xpath("//Camera/altitude/text()")
		lookat_lat = xmldoc.xpath("//LookAt/latitude/text()")
		lookat_lon = xmldoc.xpath("//LookAt/longitude/text()")
		lookat_alt = xmldoc.xpath("//LookAt/altitude/text()")
		lookat_range = xmldoc.xpath("//LookAt/range/text()")
		lookat_heading = xmldoc.xpath("//LookAt/heading/text()")
		lookat_altmode = xmldoc.xpath("//LookAt/altitudeMode/text()")
		nav = {}
		nav['lon'] , nav['lat'] = float(lon[0]) , float(lat[0])
		nav['roll'] , nav['pitch'] , nav['gain'] , nav['msl'] = float(rl[0]), float(pit[0]), float(head[0]), float(altitude[0])
		nav['lookat_lon'] , nav['lookat_lat'] = float(lookat_lon[0]) , float(lookat_lat[0])
		nav['lookat_alt'] , nav['lookat_range'] , nav['lookat_heading'] , nav['lookat_altmode'] = float(lookat_alt[0]), float(lookat_range[0]), float(lookat_heading[0]), str(lookat_altmode[0])
		return nav
	except :
		print 'xml parsing problems in parseSignal'



def parsesignalLookAt(data):
	try :
		xmldoc = etree.fromstring(data)
		lookat_lat = xmldoc.xpath("//LookAt/latitude/text()")
		lookat_lon = xmldoc.xpath("//LookAt/longitude/text()")
		lookat_alt = xmldoc.xpath("//LookAt/altitude/text()")
		lookat_range = xmldoc.xpath("//LookAt/range/text()")
		lookat_heading = xmldoc.xpath("//LookAt/heading/text()")
		lookat_altmode = xmldoc.xpath("//LookAt/altitudeMode/text()")
		nav = {}
		nav['lookat_lon'] , nav['lookat_lat'] = float(lookat_lon[0]) , float(lookat_lat[0])
		nav['lookat_alt'] , nav['lookat_range'] , nav['lookat_heading'] , nav['lookat_altmode'] = float(lookat_alt[0]), float(lookat_range[0]), float(lookat_heading[0]), str(lookat_altmode[0])
		print nav
		return nav
	except :
		print 'xml parsing problems in parsesignalLookAt'



def navlogger(host,port):
	HOST = host
	PORT = port
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	s.bind((HOST, PORT))
	s.listen(1)
	conn, addr = s.accept()
	print 'Connected by', addr
	data = conn.recv(1024)
	try :
		parsed = parseSignal(data)
		print parsed
		if not data:
			conn.close()
	except :
		print 'parse error'
	conn.close()




#addzoom('/data/florida/Brevard.tif',-81.0009,29.0009,15000,'localhost',8000,7000)
#removefile('/data/florida/Brevard.tif','localhost',8000)
