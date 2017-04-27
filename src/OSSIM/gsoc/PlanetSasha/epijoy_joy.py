#!/usr/bin/env python
from __future__ import division
import pygame 
from pygame.locals import * 
from sys import exit 
import socket
import time
from numpy import array
from numpy import sum
from numpy import arange
from datetime import datetime
#import psycopg2
import sys
import os

kmlpath = os.path.abspath(os.path.dirname(sys.argv[0]))

# Qlon,Qlat

def startj(Qlon,Qlat):
	pygame.init()

	joystick = None 
	event_text = []
	joystick_buttons = [] 
	zoom = []
	rangex= []

	zl = 1.
	x = 1
	rl = 1.
	y = 10
	tp = str('LookAt')
	lon = 0.
	lat = 0.
	lati = []
	longi = []
	lati.append(Qlon)
	longi.append(Qlat)
	zlat = array(lati)
	zlon = array(longi)
	longi = []
	head = 0.
	heads = []
	hd = array(heads)
	j = 0.
	h = 0.
	pit = 0.
	pitch = []
	pt = array(pitch)
	p = 0.
	rol = 0.
	roll = []
	rll = array(roll)
	r = 0.
	n = 1.7
	up = None
	down = None
	rx = None
	sx = None
	vallon = 0
	ff = 0
	t = datetime.now()
	np=t.strftime("track_%Y_%m_%d_%H_%M_%S")
	tempo=t.strftime("%Y-%m-%d %H:%M:%S")

	ossim = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	ossim.connect(("127.0.0.1", 7000))

	if pygame.joystick.get_count() > 0:
		joystick = pygame.joystick.Joystick(0)
		joystick.init()
	if joystick is None:
		print "Sorry, you need a joystick for this!"
		exit()
	print 'first of while'
	while True:
		for event in pygame.event.get():
			if event.type == QUIT:
				exit()

		if zl == 0:
			zl = 1.
			x = 1.
		if rl == 0:
			rl = 1.
			y = 10.

		if joystick.get_numaxes() >= 2:
			h0 = joystick.get_hat(0)
			axis_p = joystick.get_axis(3)
			axis_y = joystick.get_axis(0)
			axis_x = joystick.get_axis(1)
			axis_h = joystick.get_axis(2)
			b0 = joystick.get_button(0)
			b1 = joystick.get_button(1)
			b2 = joystick.get_button(2)
			b3 = joystick.get_button(3)
			b4 = joystick.get_button(4)
			b5 = joystick.get_button(5)
			b10 = joystick.get_button(6)
			b11 = joystick.get_button(7)
			#b8 = joystick.get_button(8)
			#b9 = joystick.get_button(9)
			#b10 = joystick.get_button(10)
			#b11 = joystick.get_button(11)


			if b10 == 1:
				tp = str('Camera')
			if b10 == 0:
				tp = str('LookAt')

			if b11 == 1:
				zoom = []
				rangex= []
				zl = 1.
				x = 1
				rl = 1.
				y = 10
				tp = str('LookAt')
				lon = 0.
				lat = 0.
				lati = []
				longi = []
				zlat = array(lati)
				zlon = array(longi)
				longi = []
				head = 0.
				heads = []
				hd = array(heads)
				j = 0.
				h = 0.
				pit = 0.
				pitch = []
				pt = array(pitch)
				p = 0.
				rol = 0.
				roll = []
				rll = array(roll)
				r = 0.
				n = 1.7
				up = None
				down = None
				rx = None
				sx = None
				vallon = 0
				ff = 0

			if b3 == 1:
				pygame.quit()
				t = datetime.now()
				w=t.strftime("waypoint_%Y_%m_%d_%H_%M_%S")
				kml = '%s/kml/joynav_%s.kml' % (kmlpath,w)
				name = str('name')+'%s'
				output = """<?xml version="1.0" encoding="UTF-8"?>
				<kml xmlns="http://earth.google.com/kml/2.0">
				<Placemark>
				<name>%s</name>
				<Style id="randomColorIcon"> 
				  <IconStyle> 
				     <color>ff00ff00</color> 
				     <colorMode>random</colorMode> 
				     <scale>1.1</scale> 
				     <Icon> 
				        <extrude>1</extrude>
				        <altitudeMode>relativeToGround</altitudeMode>
				        <href>%s/icons/red_circle.png</href> 
				     </Icon> 
				  </IconStyle> 
				</Style>
				<description><![CDATA[]]></description>
				<LookAt>
					<longitude>%s</longitude>
					<latitude>%s</latitude>
					<range>%s</range>
					<tilt>%s</tilt>
					<heading>%s</heading>
				</LookAt>
				<Point>
				    <extrude>1</extrude>
				    <altitudeMode>relativeToGround</altitudeMode>
					<coordinates>%s,%s,%s</coordinates>
				</Point>
				</Placemark>
				</kml>""" % (w,kmlpath,lon,lat,rl,pit,head,lon,lat,zl)
				#print output
				f=open(kml, 'w')
				f.flush()
				f.write(output)
				f.close()
				print 'kml saved in :' , kmlpath+'/kml/'

				tt = datetime.now()
				nn=tt.strftime("%m%d%H%M%S")
				tempox=tt.strftime("%Y-%m-%d %H:%M:%S")
				#print nn
				#try:
				#	conn = psycopg2.connect(paramstring);
				#except:
				#	print "I am unable to connect to the database"
				#curs = conn.cursor()
				#template="""INSERT INTO %s (latitude, longitude,roll, pitch, head, the_geom) VALUES ('%s', %s, %s, %s, %s, GeomFromText( 'POINT(%s %s)', 4326))"""
				#sql=(template%(np,lat,lon,rl,pit,head,lon,lat))
				#curs.execute(sql)
				#conn.commit()

			if b10 == 0:
				zl = 1.
				if b1 == 1:
					for i in range(y , y+1):
						rangex.append( i )
						r = array(rangex)
						rl = sum(r)
						y = rl * pow(10,-1)

				if b0 == 1:
					for i in range(y , y+1):
						rangex[-1:] = []
						r = array(rangex)
						rl = sum(r)
						y = rl * pow(10,-1)

			if b10 == 1:
				rl = 1.
				if b1 == 1:
					for i in arange(x , x+1):
						zoom.append( i )
						z = array(zoom)
						zl = sum(z)
						x = zl * pow(10,-1)

				if b0 == 1:
					for i in range(x , x+1):
						zoom[-1:] = []
						z = array(zoom)
						zl = sum(z)
						x = zl * pow(10,-1)

			if abs(axis_x) < 0.015:
				axis_x = 0.
			if abs(axis_y) < 0.015:
				axis_y = 0.
			if abs(axis_h) < 0.015:
				axis_h = 0.
			if abs(axis_p) < 0.015:
				axis_p = 0.
			axis_v = axis_p
			axis_p = (axis_p * (1 + axis_p))
			if axis_p < 0.0:
				n = 0.007
				axis_x = axis_x * 0.5
				axis_y = axis_y * 0.5
			if axis_p > 0.0:
				n = 0.7
				axis_x = axis_x * 1.5
				axis_y = axis_y * 1.5
			axis_v = (axis_v * (1 + axis_v)) * n
			#print axis_x, axis_y, axis_h, axis_v, axis_p

			if h0 == (0, 1):
				if pit <= 91:
					for i in arange(j , j+1):
						pitch = [sum(pt)]
						a = abs(axis_p) * 1.5
						pitch.insert( i+1,a )
						pt = array(pitch)
						pit = sum(pt)
						j = 0.1
						if abs(pit) > 91:
							pit = -pit

			if h0 == (0, -1):
				if pit <= 91:
					for i in arange(j , j+1):
						pitch = [sum(pt)]
						a = abs(axis_p) * 1.5
						pitch = [sum(pitch) - a]
						pt = array(pitch)
						pit = sum(pt)
						j = 0.1
						if abs(pit) > 91:
							pit = -pit

			if h0 == (-1, 0):
				if rol <= 91:
					for i in arange(j , j+1):
						roll = [sum(rll)]
						a = abs(axis_p) * 1.5
						roll.insert( i+1,a )
						rll = array(roll)
						rol = sum(rll)
						j = 0.1
						if abs(rol) > 91:
							rol = -rol

			if h0 == (1, 0):
				if rol <= 91:
					for i in arange(j , j+1):
						roll = [sum(rll)]
						a = abs(axis_p) * 1.5
						roll = [sum(roll) - a]
						rll = array(roll)
						rol = sum(rll)
						j = 0.1
						if abs(rol) > 91:
							rol = -rol

			if h0 == (1, 1):
				if pit <= 91:
					for i in arange(j , j+1):
						pitch = [sum(pt)]
						a = abs(axis_p) * 1.5
						pitch.insert( i+1,a )
						pt = array(pitch)
						pit = sum(pt)
						j = 0.1
						if abs(pit) > 91:
							pit = -pit

				if rol <= 91:
					for i in arange(j , j+1):
						roll = [sum(rll)]
						a = abs(axis_p) * 1.5
						roll = [sum(roll) - a]
						rll = array(roll)
						rol = sum(rll)
						j = 0.1
						if abs(rol) > 91:
							rol = -rol

			if h0 == (-1,-1):
				if pit <= 91:
					for i in arange(j , j+1):
						pitch = [sum(pt)]
						a = abs(axis_p) * 1.5
						pitch = [sum(pitch) - a]
						pt = array(pitch)
						pit = sum(pt)
						j = 0.1
						if abs(pit) > 91:
							pit = -pit

				if rol <= 91:
					for i in arange(j , j+1):
						roll = [sum(rll)]
						a = abs(axis_p) * 1.5
						roll.insert( i+1,a )
						rll = array(roll)
						rol = sum(rll)
						j = 0.1
						if abs(rol) > 91:
							rol = -rol

			if h0 == (1,-1):
				if pit <= 91:
					for i in arange(j , j+1):
						pitch = [sum(pt)]
						a = abs(axis_p) * 1.5
						pitch = [sum(pitch) - a]
						pt = array(pitch)
						pit = sum(pt)
						j = 0.1
						if abs(pit) > 91:
							pit = -pit

				if rol <= 91:
					for i in arange(j , j+1):
						roll = [sum(rll)]
						a = abs(axis_p) * 1.5
						roll = [sum(roll) - a]
						rll = array(roll)
						rol = sum(rll)
						j = 0.1
						if abs(rol) > 91:
							rol = -rol

			if h0 == (-1,1):
				if pit <= 91:
					for i in arange(j , j+1):
						pitch = [sum(pt)]
						a = abs(axis_p) * 1.5
						pitch.insert( i+1,a )
						pt = array(pitch)
						pit = sum(pt)
						j = 0.1
						if abs(pit) > 91:
							pit = -pit

				if rol <= 91:
					for i in arange(j , j+1):
						roll = [sum(rll)]
						a = abs(axis_p) * 1.5
						roll.insert( i+1,a )
						rll = array(roll)
						rol = sum(rll)
						j = 0.1
						if abs(rol) > 91:
							rol = -rol


			if ff >= 0:
				if axis_x < 0:
					up = 0
				if axis_y == 0:
					rx = 0
				if up == rx == 0:
					for i in arange(j , j+1):
						lati = [sum(zlat)]
						a = abs(axis_v) * abs(axis_x)
						lati.insert( i+1,a )
						zlat = array(lati)
						lat = sum(zlat) 
						j = 0.1

				if axis_x == 0:
					up = 0
				if axis_y > 0:
					rx = 0
				if up == rx == 0:
					for i in arange(j , j+1): 
						longi = [sum(zlon)]
						a = abs(axis_y) * abs(axis_v)
						longi.insert( i+1,a )
						zlon = array(longi)
						lon = sum(zlon) 
						j = 0.1

				if axis_x > 0:
					up = 2
				if axis_y == 0:
					rx = 2
				if up == rx == 2:
					for i in arange(j , j+1):
						lati = [sum(zlat)]
						a = abs(axis_v) * abs(axis_x)
						lati = [sum(lati) - a]
						zlat = array(lati)
						lat = sum(zlat)
						j = 0.1


				if axis_x == 0:
					up = 1
				if axis_y < 0:
					rx = 1
				if up == rx == 1:
					for i in arange(j , j+1):
						longi = [sum(zlon)]
						a = abs(axis_y) * abs(axis_v)
						longi = [sum(longi) - a]
						zlon = array(longi)
						lon = sum(zlon)
						j = 0.1



				if axis_x < 0:
					up = 0
				if axis_y > 0:
					rx = 0
				if up == rx == 0:
					for i in arange(j , j+1):
						longi = [sum(zlon)]
						a = abs(axis_y) * abs(axis_v)
						longi.insert( i+1,a )
						zlon = array(longi)
						lon = sum(zlon) 
						lati = [sum(zlat)]
						a = abs(axis_v) * abs(axis_x)
						lati.insert( i+1,a )
						zlat = array(lati)
						lat = sum(zlat) 
						j = 0.1

				if axis_x > 0:
					up = 2
				if axis_y > 0:
					rx = 2
				if up == rx == 2:
					for i in arange(j , j+1):
						longi = [sum(zlon)]
						a = abs(axis_y) * abs(axis_v)
						longi.insert( i+1,a )
						zlon = array(longi)
						lon = sum(zlon)
						lati = [sum(zlat)]
						a = abs(axis_v) * abs(axis_x)
						lati = [sum(lati) - a]
						zlat = array(lati)
						lat = sum(zlat)
						j = 0.1

				if axis_x > 0:
					up = 1
				if axis_y < 0:
					rx = 1
				if up == rx == 1:
					for i in arange(j , j+1):
						lati = [sum(zlat)]
						a = abs(axis_v) * abs(axis_x)
						lati = [sum(lati) - a]
						zlat = array(lati)
						lat = sum(zlat)
						longi = [sum(zlon)]
						a = abs(axis_y) * abs(axis_v)
						longi = [sum(longi) - a]
						zlon = array(longi)
						lon = sum(zlon)
						j = 0.1
						#lon = -lon

				if axis_x < 0:
					up = 3
				if axis_y < 0:
					rx = 3
				if up == rx == 3:
					for i in arange(j , j+1):
						lati = [sum(zlat)]
						a = abs(axis_v) * abs(axis_x)
						lati.insert( i+1,a )
						zlat = array(lati)
						lat = sum(zlat)
						longi = [sum(zlon)]
						a = abs(axis_y) * abs(axis_v)
						longi = [sum(longi) - a]
						zlon = array(longi)
						lon = sum(zlon)
						j = 0.1

				if head >= 0:
					if axis_h > 0:
						for i in arange(j , j+1):
							heads = [sum(hd)]
							a = abs(axis_h) * abs(axis_h)
							heads.insert( i+1,a )
							hd = array(heads)
							head = sum(hd)
							j = 0.1
						time.sleep(0.02)
					if axis_h < 0:
						for i in arange(j , j+1):
							heads = [sum(hd)]
							a = abs(axis_h) * abs(axis_h)
							heads = [sum(heads) - a]
							hd = array(heads)
							head = sum(hd)
							j = 0.1

				if head <= 0:
					if axis_h < 0:
						for i in arange(j , j+1):
							heads = [sum(hd)]
							a = abs(axis_h) * abs(axis_h)
							heads.insert( i+1,a )
							hd = array(heads)
							head = sum(hd)
							j = 0.1
							head = -head

					if axis_h > 0:
						for i in arange(j , j+1):
							heads = [sum(hd)]
							a = abs(axis_h) * abs(axis_h)
							heads = [sum(heads) - a]
							hd = array(heads)
							head = sum(hd)
							j = 0.1
							head = -head


				time.sleep(0.05)
				valat = lat
				outn = 0
				offN = 0
				if lat > 90:
					outn = lat - 90
					offN = abs(outn / 90)
					if int(offN) == 0.:
						valat = (90 - outn)
					#	print valat
					if int(offN) > 0:
						valat = 90 - (lat - (90*int(offN)))
				#if lat < -90:
				#	outs = lat + 90
				#	offS = abs(outs / 90)
				#	if int(offS) == 1:
				#		valat = -90 + (-90 + lat)
				#	if int(offS) > 1:
				#		valat = -90 + (lat + (90*int(offS)))
				print valat, outn , offN , lat 
					
				ossimxml = '<Set target=":navigator" vref="wgs84"><%s><longitude>%s</longitude><latitude>%s</latitude><altitude>%s</altitude><heading>%s</heading><pitch>%s</pitch><roll>%s</roll><altitudeMode>absolute</altitudeMode><range>%s</range></%s></Set>' % (tp,lon,lat,zl,head,pit,rol,rl,tp)
				ossim.send(ossimxml)
				#print ossimxml
	ossim.close()
