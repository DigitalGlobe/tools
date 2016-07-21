#!/usr/bin/env python
import sys

from PyQt4.QtCore import *
from PyQt4.QtGui import *
#from grass.script.core import *
from GPS_ui import Ui_Form
import os
from gpst import *
import socket
from PyQt4.QtWebKit import *

apppath = os.path.dirname(__file__)
#webpage = 'http://localhost/~tiamo/ka-map/htdocs/index_aqua.html'
webpage = '%s/gps/gpsy.html' % (apppath)
latfile = '%s/gps/lat.xml' % (apppath)
lonfile = '%s/gps/lon.xml' % (apppath)
kmlfile = '%s/gps/nmea.kml' % (apppath)
tcpparam = '%s/conf/tcp.conf' % (apppath)

class GpsClass(QWidget, Ui_Form):
    def __init__(self,parent,flags):
        QWidget.__init__(self,parent,flags)
        self.setupUi(self)
        self.GPSlatitude = ""
        self.thread = GpsT()
        self.thread.start()
        self.Rangechange = 0
        self.Zoomchange = 0
        self.Rollchange = 0
        self.Pitchchange = 0
        self.Headchange = 0
        self.Vtypechange = ''
        self.connect(self.StartGps, SIGNAL("clicked()"),
                     self.toggleThread)
        #self.connect(self.StartGps, SIGNAL("clicked()"), self.imworking)
        self.connect(self.thread, SIGNAL("gpslat"), 
                     self.setCrossClassLat)
        self.connect(self.thread, SIGNAL("gpslon"), 
                     self.setCrossClassLon)
        self.connect(self.thread, SIGNAL("gpssat"), 
                     self.setCrossClassSat)
        self.connect(self.thread, SIGNAL("gpseph"), 
                     self.setCrossClassEph)
        self.connect(self.thread, SIGNAL("gpsaltitude"), 
                     self.setCrossClassAltitude)
        self.connect(self.thread, SIGNAL("gpsclimb"), 
                     self.setCrossClassClimb)
        self.connect(self.thread, SIGNAL("gpstime"), 
                     self.setCrossClassTime)
        self.connect(self.thread, SIGNAL("gpsspeed"), 
                     self.setCrossClassSpeed)
        self.connect(self.thread, SIGNAL("gpsepv"), 
                     self.setCrossClassEpv)
        self.connect(self.thread, SIGNAL("gpsept"), 
                     self.setCrossClassEpt)
        self.connect(self.thread, SIGNAL("utctime"), 
                     self.setCrossClassUtctime)
        self.connect(self.thread, SIGNAL("gpstrack"), 
                     self.setCrossClassTrack)
        self.connect(self.thread, SIGNAL("gpseps"), 
                     self.setCrossClassEps)
        self.connect(self.thread, SIGNAL("gpsepd"), 
                     self.setCrossClassEpc)
        self.connect(self.thread, SIGNAL("gpsepc"), 
                     self.setCrossClassEpd)
        self.connect(self.thread, SIGNAL("gpspdop"), 
                     self.setCrossClassPdop)
        self.connect(self.thread, SIGNAL("gpshdop"), 
                     self.setCrossClassHdop)
        self.connect(self.thread, SIGNAL("gpstdop"), 
                     self.setCrossClassTdop)
        self.connect(self.thread, SIGNAL("gpsvdop"), 
                     self.setCrossClassVdop)
        self.connect(self.thread, SIGNAL("gpsgdop"), 
                     self.setCrossClassGdop)
        url = QUrl(webpage)
        self.WebMap.load(url)
        self.WebMap.show()
        frame = self.WebMap.page().mainFrame()
        self.timer = QTimer()
        QObject.connect(self.timer, SIGNAL("timeout()"),self.runStatus)
        self.timer.start(1000)



    def runStatus(self):
        self.thread.msleep(1)

    def toggleThread(self):
        self.thread.toggle()

    def imworking(self):
        QMessageBox.information(self, 'debug', str('pressesd')+' '+str('start'))

    def setparamconnection(self):
        try:
            TCP = open(tcpparam, "r")
            K = TCP.read()
            Y = K.rsplit(',')
            host = str(Y[0])
            nav = str(Y[1])
            data = str(Y[2])
            return host, nav, data
        except :
            print 'Use preference Panel to set preference'
            #self.worningmessagetcp()

    def setChangeRoll(self, Rollchange):
        self.Rollchange = Rollchange

    def setChangeZoom(self, Zoomchange):
        self.Zoomchange = Zoomchange

    def setChangeRange(self, Rangechange):
        self.Rangechange = Rangechange

    def setChangePitch(self, Pitchchange):
        self.Pitchchange = Pitchchange

    def setChangeHead(self, Headchange):
        self.Headchange = Headchange

    def setChangeVtype(self, Vtypechange):
        self.Vtypechange = Vtypechange


    def setCrossClassLat(self, value):
        #global CrossClassLat
        self.CrossClassLat = value
        self.GPSlat.setText(self.CrossClassLat)
        if self.Headchange is not None:
            host = self.setparamconnection()[0]
            nav = self.setparamconnection()[1]
            if self.GPSSend.isChecked():
                if self.Headchange == str('Manual'):
                    heads = self.HandlingSlider.value()
                if self.Headchange == str('Auto'):
                    heads = self.CrossClassTrack
                ossimxml = '<Set target=":navigator" vref="wgs84"><%s>\
                <longitude>%s</longitude><latitude>%s</latitude>\
                <altitude>%s</altitude><heading>%s</heading><pitch>%s</pitch>\
                <roll>%s</roll><altitudeMode>absolute</altitudeMode><range>%s</range>\
                </%s></Set>' % (self.Vtypechange, unicode(self.CrossClassLon), 
                unicode(self.CrossClassLat), self.Zoomchange, self.Headchange, 
                self.Pitchchange, self.Rollchange, self.Rangechange, self.Vtypechange)
                ossim = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                ossim.connect((host, int(nav)))
                ossim.send(ossimxml)
                ossim.close()
                print ossimxml
                f = open(lonfile, "w")
                f.write(unicode(self.CrossClassLon))
                f.close
                g = open(latfile, "w")
                g.write(unicode(self.CrossClassLat))
                g.close
                h = open(kmlfile, "w")
                output = """<?xml version="1.0" encoding="UTF-8"?>
        <kml xmlns="http://earth.google.com/kml/2.0">
            <Placemark>
                <name>Place</name>
                <Style id="randomColorIcon"> 
                  <IconStyle> 
                     <color>ff00ff00</color> 
                     <colorMode>random</colorMode> 
                     <scale>1.1</scale> 
                     <Icon> 
                        <href>red_circle.png</href> 
                     </Icon> 
                  </IconStyle> 
               </Style>
            	<description><![CDATA[<table border="1"><tr>
            	<td>Date</td><td> %s</td>
            	</tr></table>]]></description>
            	<LookAt>
            		<longitude>%s</longitude>
            		<latitude>%s</latitude>
            		<range>200</range>
            		<tilt>0</tilt>
            		<heading>0</heading>
            	</LookAt>
            	<Point>
            		<coordinates>%s,%s,0</coordinates>
            	</Point>
            </Placemark>
        </kml>""" % (unicode(self.CrossClassTime), unicode(self.CrossClassLon),
                     unicode(self.CrossClassLat), unicode(self.CrossClassLon),
                     unicode(self.CrossClassLat))
                lonlat = unicode(self.CrossClassLon)+' '+unicode(self.CrossClassLat)
                h.write(output)
                h.close
                coordsfile = apppath+'/lonlatfile'
                f = open(coordsfile, "w")
                f.write(lonlat)
                f.flush()
                f.close

    def setCrossClassLat2(self, value):
        self.CrossClassLat = value
        self.GPSlat.setText(self.CrossClassLat)
        #if self.actionGPS.isChecked():
        #    self.Lat.setText(self.CrossClassLat)

    def setCrossClassLon(self, value):
        self.CrossClassLon = value
        self.GPSlon.setText(self.CrossClassLon)
        #if self.actionGPS.isChecked():
        #    self.Lon.setText(self.CrossClassLon)

    def setCrossClassSat(self, value):
        self.CrossClassSat = value
        self.Satellite.setText(self.CrossClassSat)

    def setCrossClassEph(self, value):
        self.CrossClassEph = value
        self.GPSEph.setText(self.CrossClassEph)

    def setCrossClassAltitude(self, value):
        self.CrossClassAltitude = value
        self.GPSAltitude.setText(self.CrossClassAltitude)

    def setCrossClassClimb(self, value):
        self.CrossClassClimb = value
        self.GPSClimb.setText(self.CrossClassClimb)

    def setCrossClassTime(self, value):
        self.CrossClassTime = value
        self.GPSTime.setText(self.CrossClassTime)

    def setCrossClassSpeed(self, value):
        self.CrossClassSpeed = value
        self.GPSSpeed.setText(self.CrossClassSpeed)

    def setCrossClassEpv(self, value):
        self.CrossClassEpv = value
        self.GPSEpv.setText(self.CrossClassEpv)

    def setCrossClassEpt(self, value):
        self.CrossClassEpt = value
        self.GPSEpt.setText(self.CrossClassEpt)

    def setCrossClassUtctime(self, value):
        self.CrossClassUtctime = value
        self.GPSUtctime.setText(self.CrossClassUtctime)

    def setCrossClassTrack(self, value):
        self.CrossClassTrack = value
        self.GPSTrack.setText(self.CrossClassTrack)

    def setCrossClassEpd(self, value):
        self.CrossClassEpd = value
        self.GPSEpd.setText(self.CrossClassEpd)

    def setCrossClassEps(self, value):
        self.CrossClassEps = value
        self.GPSEps.setText(self.CrossClassEps)

    def setCrossClassEpc(self, value):
        self.CrossClassEpc = value
        self.GPSEpc.setText(self.CrossClassEpc)

    def setCrossClassPdop(self, value):
        self.CrossClassPdop = value
        self.GPSPdop.setText(self.CrossClassPdop)

    def setCrossClassHdop(self, value):
        self.CrossClassHdop = value
        self.GPSHdop.setText(self.CrossClassHdop)

    def setCrossClassTdop(self, value):
        self.CrossClassTdop = value
        self.GPSTdop.setText(self.CrossClassTdop)

    def setCrossClassVdop(self, value):
        self.CrossClassVdop = value
        self.GPSVdop.setText(self.CrossClassVdop)

    def setCrossClassGdop(self, value):
        self.CrossClassGdop = value
        self.GPSGdop.setText(self.CrossClassGdop)
