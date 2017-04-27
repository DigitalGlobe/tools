#!/usr/bin/env python
import sys

from PyQt4.QtCore import *
from PyQt4.QtGui import *

from model_ui import Ui_Form
import os
import zipfile
from xml.dom import minidom 
import subprocess

#Modelpath = ""
AltitudeMode = ""


apppath = os.path.abspath(os.path.dirname(sys.argv[0]))
#imagepath = '%s/icons/' % (apppath)
kmlpath = '%s/kml/' % (apppath)
configfile = '%s/conf/conf.xml' % (apppath)


class PlaceModel(QWidget, Ui_Form):
    def __init__(self):
        QWidget.__init__(self)
        self.setupUi(self)
        self.lon = 0
        self.lat = 0
        self.Rollchange = 0
        self.Pitchchange = 0
        self.Headchange = 0
        self.Zoomchange = 0
        self.Rangechange = 0
        self.connect(self.save, SIGNAL("clicked()"), self.savekml)
        self.connect(self.altitudemode, SIGNAL("currentIndexChanged(int)"),
                     self.GetAltitudeMode)
        self.connect(self.setcamera, SIGNAL("clicked()"),self.aggiorna_camera)
        self.connect(self.setmodel, SIGNAL("clicked()"),self.aggiorna_model)
        self.connect(self.setlonlat, SIGNAL("clicked()"),self.set_position)
        self.connect(self.selectmodel, SIGNAL("clicked()"), self.setOpenModel)
        self.connect(self.reset, SIGNAL("clicked()"), self.resetvalue)
        self.connect(self.opendir, SIGNAL("clicked()"),self.filemanager)


    def compress_kml(self,outfile,icon):
        directory = os.path.dirname(str(outfile))
        iconname = icon.split('/')[-1]
        icontosave = directory+'/'+iconname
        string = 'cp %s %s' % (icon, icontosave)
        cp = os.system(string)
        outfilename = outfile.split('/')
        outfilename = outfilename[-1]
        kmz = outfile.split('.')[0]+'.kmz'
        zip = zipfile.ZipFile(str(kmz), 'w')
        zip.write(str(outfile))
        zip.write(str(icontosave))
        zip.close
    
    def resetvalue(self):
        self.CameraLongitude.setText('')
        self.CameraLatitude.setText('')
        self.CameraTilt.setText(str(''))
        self.CameraHeading.setText(str(''))
        self.CameraAltitude.setText(str(''))
        self.CameraRange.setText(str(''))
        self.ModelLongitude.setText('')
        self.ModelLatitude.setText('')
        self.ModelTilt.setText(str(''))
        self.ModelHeading.setText(str(''))
        self.ModelAltitude.setText(str(''))
        self.ModelRange.setText(str(''))
        self.MLongitude.setText('')
        self.MLatitude.setText('')
        self.modelpath.setText('')#
        self.kmlname.setText('')
        self.kmllabel.setText('')
        self.MAltitude.setText('')
        self.MHeading.setText('')
        self.MTilt.setText('')
        self.MRoll.setText('')
    

    def parseOutputconf(self):
        xmldoc = minidom.parse(configfile)
        outconf = {}
        planetsashaconf = xmldoc.firstChild
        if planetsashaconf.childNodes[5].childNodes[1].firstChild is not None:
            outconf['kmldir'] = planetsashaconf.childNodes[5].childNodes[1].firstChild.data
        else :
            print 'kmldir not found'
            outconf['kmldir'] = 'None'

        if planetsashaconf.childNodes[5].childNodes[3].firstChild is not None:
            outconf['vrtdir'] = planetsashaconf.childNodes[5].childNodes[3].firstChild.data
        else :
            print 'vrtdir not found'
            outconf['vrtdir'] = 'None'

        if planetsashaconf.childNodes[5].childNodes[5].firstChild is not None:
            outconf['filemanager'] = planetsashaconf.childNodes[5].childNodes[5].firstChild.data
        else :
            print 'filemanager not found'
            outconf['filemanager'] = 'None'

        if planetsashaconf.childNodes[5].childNodes[7].firstChild is not None:
            outconf['gpsdevice'] = planetsashaconf.childNodes[5].childNodes[7].firstChild.data
        else :
            print 'gpsdevice not found'
            outconf['gpsdevice'] = 'None'

        if planetsashaconf.childNodes[5].childNodes[9].firstChild is not None:
            outconf['ossimpreference'] = planetsashaconf.childNodes[5].childNodes[9].firstChild.data
        else :
            print 'ossimpreference not found'
            outconf['ossimpreference'] = 'None'

        if planetsashaconf.childNodes[5].childNodes[11].firstChild is not None:
            outconf['spatialitedb'] = planetsashaconf.childNodes[5].childNodes[11].firstChild.data
        else :
            print 'spatialitedb not found'
            outconf['spatialitedb'] = 'None'

        return  outconf
    

    def filemanager(self):
        filemanager = self.parseOutputconf()['filemanager']
        kmldir = self.parseOutputconf()['kmldir']
        output = subprocess.Popen([filemanager ,str(kmldir)], stdout=subprocess.PIPE).communicate()[0]


    def setLonValue(self, lon):
        self.lon = lon

    def setLatValue(self, lat):
        self.lat = lat

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

    def aggiorna_camera(self):
        newlon = str(self.lon)
        newlat = str(self.lat)
        self.CameraLongitude.setText(newlon)
        self.CameraLatitude.setText(newlat)
        #self.Roll.setText(str(self.Rollchange))
        self.CameraTilt.setText(str(self.Pitchchange))
        self.CameraHeading.setText(str(self.Headchange))
        self.CameraAltitude.setText(str(self.Zoomchange))
        self.CameraRange.setText(str(self.Rangechange))

    def aggiorna_model(self):
        newlon = str(self.lon)
        newlat = str(self.lat)
        self.ModelLongitude.setText(newlon)
        self.ModelLatitude.setText(newlat)
        #self.Roll.setText(str(self.Rollchange))
        self.ModelTilt.setText(str(self.Pitchchange))
        self.ModelHeading.setText(str(self.Headchange))
        self.ModelAltitude.setText(str(self.Zoomchange))
        self.ModelRange.setText(str(self.Rangechange))


    def set_position(self):
        newlon = str(self.lon)
        newlat = str(self.lat)
        self.MLongitude.setText(newlon)
        self.MLatitude.setText(newlat)


    def GetAltitudeMode(self, index):
        global AltitudeMode
        AltitudeMode = self.altitudemode.itemText(index)
        return AltitudeMode

    def setOpenModel(self):
        Mpath = QFileDialog.getOpenFileName(self,self.modelpath.text(),self.tr("ALL Files (*);;Collada Files (*.dae)"))
        if not Mpath.isEmpty():
            self.modelpath.setText(Mpath)

    def savekml(self):
        kml = """<?xml version='1.0' encoding='UTF-8'?>
		<kml xmlns='http://earth.google.com/kml/2.1'>
		<Folder>
		    <name>%s</name>
		    <description><![CDATA[Created with <a href="http://www.geofemengineering.it">Planet-Sasha</a>]]></description>
		    <DocumentSource>PlanetSasha</DocumentSource>
		<visibility>1</visibility>
		<LookAt>
		<heading>%s</heading>
		<tilt>%s</tilt>
		<latitude>%s</latitude>
		<longitude>%s</longitude>
		<range>%s</range>
		<altitude>%s</altitude>
		</LookAt>
		<Folder>
		<name>Tour</name>
		<Placemark>
		<name><![CDATA[Camera]]></name>
		<visibility>1</visibility>
		<LookAt>
		<heading>%s</heading>
		<tilt>%s</tilt>
		<latitude>%s</latitude>
		<longitude>%s</longitude>
		<range>%s</range>
		<altitude>%s</altitude>
		</LookAt>
		</Placemark>
		</Folder>
		  <Placemark>
		    <name>%s</name>
		    <description><![CDATA[]]></description>
		    <Style id='default'>
		    </Style>
		    <Model>
		        <altitudeMode>%s</altitudeMode>
		        <Location>
		            <longitude>%s</longitude>
		            <latitude>%s</latitude>
		            <altitude>%s</altitude>
		        </Location>
		        <Orientation>
		            <heading>%s</heading>
		            <tilt>%s</tilt>
		            <roll>%s</roll>
		        </Orientation>
		        <Scale>
		            <x>%s</x>
		            <y>%s</y>
		            <z>%s</z>
		        </Scale>
		        <Link>
		            <href>%s</href>
		        </Link>
		    </Model>
		  </Placemark></Folder>
		</kml>""" % (self.kmlname.text(), self.CameraHeading.text(), self.CameraTilt.text(), self.CameraLatitude.text(), self.CameraLongitude.text(), self.CameraRange.text(), self.CameraAltitude.text(), self.ModelHeading.text(), self.ModelTilt.text(), self.ModelLatitude.text(), self.ModelLongitude.text(), self.ModelRange.text(), self.ModelAltitude.text(), self.kmllabel.text(), AltitudeMode, self.MLongitude.text(), self.MLatitude.text(), self.MAltitude.text(), self.MHeading.text(), self.MTilt.text(), self.MRoll.text(), self.Xscale.text(), self.Yscale.text(), self.Zscale.text(), self.modelpath.text())
        kmldir = self.parseOutputconf()['kmldir']
        kmltosave = kmldir+'/'+self.kmlname.text()+'.kml'
        f=open(kmltosave, 'w')
        f.write(kml)
        f.close()
        kmz = self.compress_kml(str(kmltosave),str(self.modelpath.text()))