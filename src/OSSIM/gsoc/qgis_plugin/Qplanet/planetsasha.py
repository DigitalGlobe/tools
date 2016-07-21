from PyQt4.QtCore import *
from PyQt4.QtGui import *
from qgis.core import *
from qgis.gui import *

from planetsashatool import PlanetSashaTool
import resources

import os
import tempfile

from ui_control import ui_Control

import sys
import os
import subprocess
import string
import socket
from pref import PreferenceSetting
from msgworn import worn
from msgworntcp import worntcp
from Savekml import KmlView

from dataprocess import datap

from model import PlaceModel
from modelite import PlaceModelite
from epsg import SearchEpsg
from LatLongUTMconversion import LLtoUTM
from VectorOp import VectorOperation
from episg import *
from pysqlite2 import dbapi2 as sqlite3


clon = 0
clat = 0
item = 'LookAt'
head = '0'
ell = 23
rasterpath = ""
vectorpath = ""
apppath = os.path.dirname(__file__)

tcpparam = '%s/conf/tcp.conf' % (apppath)

sqliteconf = '%s/conf/spatilite.conf' % (apppath)

sqlitedb = apppath+'/geonames.sqlite'

from gpst import *

webpage = '%s/gps/gpsy.html' % (apppath)
latfile = '%s/gps/lat.xml' % (apppath)
lonfile = '%s/gps/lon.xml' % (apppath)
kmlfile = '%s/gps/nmea.kml' % (apppath)
tcpparam = '%s/conf/tcp.conf' % (apppath)



class PlanetSashaPlugin(object):
    def __init__(self, iface):
        self.iface = iface
        self.canvas = iface.mapCanvas()
        self.vectoroperation = VectorOperation()
        self.kmlview = KmlView()
        self.datap = datap(self.iface)
        self.preferencesetting = PreferenceSetting()
        self.placemodel = PlaceModel()
        self.placemodelite = PlaceModelite()
        self.searchepsg = SearchEpsg()

        
        
    def initGui(self):
        self.toolBar = self.iface.addToolBar("PlanetSasha")
        self.action = QAction(QIcon(":/epi.png"),
        "PlanetSasha", self.iface.mainWindow())
        self.action.setWhatsThis("PlanetSasha Plugin")
        QObject.connect(self.action, SIGNAL("activated()"), self.run)
        self.toolBar.addAction(self.action)
        #self.iface.addToolBarIcon(self.action)
        self.iface.addPluginToMenu("&PlanetSasha", self.action)
        self.tool = PlanetSashaTool(self.canvas)

        self.datagenerate = QAction(QIcon(":/icons/mActionNewVectorLayer.png"),
        "Data Conversion", self.iface.mainWindow())
        self.datagenerate.setWhatsThis("Data Conversion")
        QObject.connect(self.datagenerate, SIGNAL("activated()"), self.dataw)
        self.toolBar.addAction(self.datagenerate)
        #self.iface.addToolBarIcon(self.datagenerate)
        self.iface.addPluginToMenu("&PlanetSasha", self.datagenerate)

        self.savewaypoint = QAction(QIcon(":/icons/mActionNewBookmark.png"),
        "Save Placemark", self.iface.mainWindow())
        self.savewaypoint.setWhatsThis("Save Placemark")
        QObject.connect(self.savewaypoint, SIGNAL("activated()"), self.kmldialog)
        self.toolBar.addAction(self.savewaypoint)
        #self.iface.addToolBarIcon(self.savewaypoint)
        self.iface.addPluginToMenu("&PlanetSasha", self.savewaypoint)

        self.place3dmodel = QAction(QIcon(":/icons/cubo.png"),
        "Place 3d model", self.iface.mainWindow())
        self.place3dmodel.setWhatsThis("Place 3d model")
        QObject.connect(self.place3dmodel, SIGNAL("activated()"), self.modeldialog)
        self.toolBar.addAction(self.place3dmodel)
        #self.iface.addToolBarIcon(self.place3dmodel)
        self.iface.addPluginToMenu("&PlanetSasha", self.place3dmodel)        

        self.processvectordata = QAction(QIcon(":/icons/SquadraCompasso.png"),
        "Traslate / Rotate / Reeproject - vector shape file", self.iface.mainWindow())
        self.processvectordata.setWhatsThis("Vector placement")
        QObject.connect(self.processvectordata, SIGNAL("activated()"), self.Geom)
        self.toolBar.addAction(self.processvectordata)
        #self.iface.addToolBarIcon(self.processvectordata)
        self.iface.addPluginToMenu("&PlanetSasha", self.processvectordata)

        self.epsgcode = QAction(QIcon(":/icons/600px-Brosen_windrose.svg.png"),
        "Epsg finder", self.iface.mainWindow())
        self.epsgcode.setWhatsThis("Epsg finder")
        QObject.connect(self.epsgcode, SIGNAL("activated()"), self.SEpsg)
        self.toolBar.addAction(self.epsgcode)
        #self.iface.addToolBarIcon(self.epsgcode)
        self.iface.addPluginToMenu("&PlanetSasha", self.epsgcode)

        self.setpreference = QAction(QIcon(":/icons/mActionOptions.png"),
        "Preference", self.iface.mainWindow())
        self.setpreference.setWhatsThis("Preference")
        QObject.connect(self.setpreference, SIGNAL("activated()"), self.preference)
        self.toolBar.addAction(self.setpreference)
        #self.iface.addToolBarIcon(self.setpreference)
        self.iface.addPluginToMenu("&PlanetSasha", self.setpreference)        
        
    def unload(self):
        self.iface.removePluginMenu("&PlanetSasha",self.action)
        self.iface.removeToolBarIcon(self.action)
        
    def pick(self):
        self.canvas.setMapTool(self.tool)
        
    def run(self):
        QObject.connect(self.tool, SIGNAL("finished(PyQt_PyObject)"),
        self.printoutcoords)
        self.saveTool = self.canvas.mapTool()
        self.canvas.setMapTool(self.tool)
        flags = Qt.WindowTitleHint | Qt.WindowSystemMenuHint | Qt.WindowMaximizeButtonHint | Qt.WindowCloseButtonHint  
        self.pluginGui = ui_Control(self.iface.mainWindow(), flags)
        self.pluginGui.setAttribute(Qt.WA_DeleteOnClose)
        expText = ""        
        self.Value=0
        self.ZoomValue = 0
        self.RangeValue = 0
        QObject.connect(self.pluginGui.actionExit, SIGNAL("triggered()"), self.kmlview, SLOT("close()"))
        QObject.connect(self.pluginGui.actionExit, SIGNAL("triggered()"), self.datap, SLOT("close()"))
        QObject.connect(self.pluginGui.actionExit, SIGNAL("triggered()"), self.preferencesetting, SLOT("close()"))
        QObject.connect(self.pluginGui.actionExit, SIGNAL("triggered()"), self.placemodel, SLOT("close()"))
        QObject.connect(self.pluginGui.actionExit, SIGNAL("triggered()"), self.placemodelite, SLOT("close()"))
        QObject.connect(self.pluginGui.actionExit, SIGNAL("triggered()"), self.searchepsg, SLOT("close()"))
        QObject.connect(self.pluginGui.actionExit, SIGNAL("triggered()"), self.vectoroperation, SLOT("close()"))   
        self.slvallon = 0
        self.slvallat = 0
        self.pluginGui.Lon.setText("0")
        self.pluginGui.Lat.setText("0")
        self.fxvallon = self.SetPosition()[0]
        self.fxvallat = self.SetPosition()[1]
        self.slstep = 1
        tables = self.gettablelist()
        self.pluginGui.Place.clear()
        self.pluginGui.Place.addItems(tables)
        self.pluginGui.actionHide.setChecked(True)
        self.GPSlatitude = ""
        self.thread = GpsT()
        self.thread.start()
        QObject.connect(self.pluginGui.StartGps, SIGNAL("clicked()"),
                     self.toggleThread)
        QObject.connect(self.thread, SIGNAL("gpslat"), 
                     self.setCrossClassLat)
        QObject.connect(self.thread, SIGNAL("gpslon"), 
                     self.setCrossClassLon)
        #QObject.connect(self.thread, SIGNAL("gpssat"), 
        #             self.setCrossClassSat)
        QObject.connect(self.thread, SIGNAL("gpseph"), 
                     self.setCrossClassEph)
        QObject.connect(self.thread, SIGNAL("gpsaltitude"), 
                     self.setCrossClassAltitude)
        QObject.connect(self.thread, SIGNAL("gpsclimb"), 
                     self.setCrossClassClimb)
        QObject.connect(self.thread, SIGNAL("gpstime"), 
                     self.setCrossClassTime)
        QObject.connect(self.thread, SIGNAL("gpsspeed"), 
                     self.setCrossClassSpeed)
        QObject.connect(self.thread, SIGNAL("gpsepv"), 
                     self.setCrossClassEpv)
        QObject.connect(self.thread, SIGNAL("gpsept"), 
                     self.setCrossClassEpt)
        QObject.connect(self.thread, SIGNAL("utctime"), 
                     self.setCrossClassUtctime)
        QObject.connect(self.thread, SIGNAL("gpstrack"), 
                     self.setCrossClassTrack)
        QObject.connect(self.thread, SIGNAL("gpseps"), 
                     self.setCrossClassEps)
        QObject.connect(self.thread, SIGNAL("gpsepd"), 
                     self.setCrossClassEpc)
        QObject.connect(self.thread, SIGNAL("gpsepc"), 
                     self.setCrossClassEpd)
        QObject.connect(self.thread, SIGNAL("gpspdop"), 
                     self.setCrossClassPdop)
        QObject.connect(self.thread, SIGNAL("gpshdop"), 
                     self.setCrossClassHdop)
        QObject.connect(self.thread, SIGNAL("gpstdop"), 
                     self.setCrossClassTdop)
        QObject.connect(self.thread, SIGNAL("gpsvdop"), 
                     self.setCrossClassVdop)
        QObject.connect(self.thread, SIGNAL("gpsgdop"), 
                     self.setCrossClassGdop)
			
        QObject.connect(self.pluginGui.actionPicker, SIGNAL("triggered()"),self.pick) 
        QObject.connect(self.pluginGui.actionHide, SIGNAL("triggered()"),self.hide)                
        QObject.connect(self.pluginGui.actionExit, SIGNAL("triggered()"), self.pluginGui, SLOT("close()"))
        
        
        
        QObject.connect(self.pluginGui.ZoomSlider, SIGNAL("valueChanged(int)"), 
                     self.setValueZoomSpinBox)
        QObject.connect(self.pluginGui.ZoomSlider, SIGNAL("valueChanged(int)"), 
                     self.sendFunction)
        QObject.connect(self.pluginGui.ZoomSpinBox, SIGNAL("valueChanged(double)"), 
                     self.setValueZoomSlider)
        QObject.connect(self.pluginGui.ZoomSpinBox, SIGNAL("valueChanged(double)"), 
                     self.setValueZoomSpinBox)
        QObject.connect(self.pluginGui.ZoomSpinBox, SIGNAL("valueChanged(double)"), 
                     self.sendFunction)
        QObject.connect(self.pluginGui.ZoomSpinBox, SIGNAL("valueChanged()"), 
                     self.setValue)
                     
                     
        QObject.connect(self.pluginGui.RollSlider, SIGNAL("valueChanged(int)"), 
                     self.setValueRollSpinBox)
        QObject.connect(self.pluginGui.RollSlider, SIGNAL("valueChanged(int)"), 
                     self.sendFunction)
        QObject.connect(self.pluginGui.RollSpinBox, SIGNAL("valueChanged(double)"), 
                     self.setValueRollSlider)
        QObject.connect(self.pluginGui.RollSpinBox, SIGNAL("valueChanged(double)"), 
                     self.setValueRollSpinBox)
        QObject.connect(self.pluginGui.RollSpinBox, SIGNAL("valueChanged(double)"), 
                     self.sendFunction)
        QObject.connect(self.pluginGui.RollSpinBox, SIGNAL("valueChanged()"), 
                     self.setValue)             
        
        
        QObject.connect(self.pluginGui.RangeSlider, SIGNAL("valueChanged(int)"), 
                     self.setValueRangeSpinBox)
        QObject.connect(self.pluginGui.RangeSlider, SIGNAL("valueChanged(int)"), 
                     self.sendFunction)
        QObject.connect(self.pluginGui.RangeSpinBox, SIGNAL("valueChanged(double)"), 
                     self.setValueRangeSlider)
        QObject.connect(self.pluginGui.RangeSpinBox, SIGNAL("valueChanged(double)"), 
                     self.setValueRangeSpinBox)
        QObject.connect(self.pluginGui.RangeSpinBox, SIGNAL("valueChanged(double)"), 
                     self.sendFunction)
        QObject.connect(self.pluginGui.RangeSpinBox, SIGNAL("valueChanged()"), 
                     self.setValue)
               
        QObject.connect(self.pluginGui.PitchSlider, SIGNAL("valueChanged(int)"), 
                     self.setValuePitchSpinBox)
        QObject.connect(self.pluginGui.PitchSlider, SIGNAL("valueChanged(int)"), 
                     self.sendFunction)
        QObject.connect(self.pluginGui.PitchSpinBox, SIGNAL("valueChanged(double)"), 
                     self.setValuePitchSlider)
        QObject.connect(self.pluginGui.PitchSpinBox, SIGNAL("valueChanged(double)"), 
                     self.sendFunction)
        QObject.connect(self.pluginGui.PitchSpinBox, SIGNAL("valueChanged()"), 
                     self.setValue)
                     
        QObject.connect(self.pluginGui.HandlingSlider, SIGNAL("valueChanged(int)"), 
                     self.setValueHandlingSpinBox)
        QObject.connect(self.pluginGui.HandlingSlider, SIGNAL("valueChanged(int)"), 
                     self.sendFunction)
        QObject.connect(self.pluginGui.HandlingSpinBox, SIGNAL("valueChanged(double)"), 
                     self.setValueHandlingSlider)
        QObject.connect(self.pluginGui.HandlingSpinBox, SIGNAL("valueChanged(double)"), 
                     self.sendFunction)
        QObject.connect(self.pluginGui.HandlingSpinBox, SIGNAL("valueChanged()"), 
                     self.setValue)
                     
        QObject.connect(self.pluginGui.SpeedSpinBox, SIGNAL("valueChanged(double)"), 
                     self.setValueSpeedSpinBox)
        QObject.connect(self.pluginGui.SpeedSpinBox, SIGNAL("valueChanged()"), 
                     self.setValue)
        QObject.connect(self.pluginGui.SpeedMultipler, SIGNAL("valueChanged(double)"), 
                     self.setValueSpeedMultipler)
        QObject.connect(self.pluginGui.SpeedMultipler, SIGNAL("valueChanged()"), 
                     self.setValueM)
                     
        QObject.connect(self.pluginGui.ZoomStepSpinBox, SIGNAL("valueChanged()"), 
                     self.setValueZM)
        QObject.connect(self.pluginGui.ZoomMultipler, SIGNAL("valueChanged(double)"), 
                     self.setValueZoomMultipler)
        QObject.connect(self.pluginGui.ZoomMultipler, SIGNAL("valueChanged()"), 
                     self.setValueZ)
                     
        #QObject.connect(self.pluginGui.qgisVlayer, SIGNAL("currentIndexChanged(int)"), 
        #             self.datap.setfile)
        #QObject.connect(self.pluginGui.Lon, SIGNAL("textChanged(QString)"), 
        #             self.datap.setfile)
                     
        self.pluginGui.north.setAutoRepeat(True)
        self.pluginGui.northeast.setAutoRepeat(True)
        self.pluginGui.south.setAutoRepeat(True)
        self.pluginGui.east.setAutoRepeat(True)
        self.pluginGui.west.setAutoRepeat(True)
        self.pluginGui.southwest.setAutoRepeat(True)
        self.pluginGui.northwest.setAutoRepeat(True)
        self.pluginGui.southeast.setAutoRepeat(True)

        QObject.connect(self.pluginGui.actionPref, SIGNAL("triggered()"), self.preference)

        QObject.connect(self.pluginGui.north, SIGNAL("clicked()"),
                     self.inclat)
        QObject.connect(self.pluginGui.northeast, SIGNAL("clicked()"),
                     self.inclatlon)
        QObject.connect(self.pluginGui.south, SIGNAL("clicked()"),
                     self.declat)
        QObject.connect(self.pluginGui.east, SIGNAL("clicked()"),
                     self.inclon)
        QObject.connect(self.pluginGui.west, SIGNAL("clicked()"),
                     self.declon)
        QObject.connect(self.pluginGui.southwest, SIGNAL("clicked()"),
                     self.declatlon)
        QObject.connect(self.pluginGui.northwest, SIGNAL("clicked()"),
                     self.inclatdeclon)
        QObject.connect(self.pluginGui.southeast, SIGNAL("clicked()"),
                     self.declatinclon)
        QObject.connect(self.pluginGui.Head, SIGNAL("currentIndexChanged(int)"),
                     self.GetHead)
        QObject.connect(self.pluginGui.ellipse, SIGNAL("currentIndexChanged(int)"),
                     self.ellipsesettings)
        #QObject.connect(self.pluginGui.View, SIGNAL("currentIndexChanged(int)"),
        #             self.GetViewType)
        QObject.connect(self.pluginGui.SendPosition, SIGNAL("clicked()"),
                     self.SetLonLat)
        QObject.connect(self.pluginGui.SendPosition, SIGNAL("clicked()"),
                     self.ResetPosition)
        QObject.connect(self.pluginGui.Lon, SIGNAL("textChanged(QString)"), 
                     self.kmlview.setLonValue)
        QObject.connect(self.pluginGui.Lat, SIGNAL("textChanged(QString)"), 
                     self.kmlview.setLatValue)
        #
        #QObject.connect(self.pluginGui.Lon, SIGNAL("textChanged(QString)"), 
        #             self.datap.setLonValue)
        #QObject.connect(self.pluginGui.Lat, SIGNAL("textChanged(QString)"), 
        #             self.datap.setLatValue)
        #QObject.connect(self.pluginGui.RollSpinBox, SIGNAL("valueChanged(double)"), 
        #             self.datap.setChangeRoll)
        #QObject.connect(self.pluginGui.PitchSpinBox, SIGNAL("valueChanged(double)"), 
        #             self.datap.setChangePitch)
        #QObject.connect(self.pluginGui.HandlingSpinBox, SIGNAL("valueChanged(double)"), 
        #             self.datap.setChangeHead)
        #QObject.connect(self.pluginGui.ZoomSpinBox, SIGNAL("valueChanged(double)"), 
        #             self.datap.setChangeZoom)
        #QObject.connect(self.pluginGui.RangeSpinBox, SIGNAL("valueChanged(double)"), 
        #             self.datap.setChangeRange)
        #QObject.connect(self.pluginGui.qgislayer, SIGNAL("currentIndexChanged(QString)"), 
        #             self.datap.setRFile)
        #QObject.connect(self.pluginGui.qgisVlayer, SIGNAL("currentIndexChanged(QString)"),
        #             self.datap.setVFile)
        # SIGNAL("currentIndexChanged(int)"QObject.connect(self.pluginGui.qgislayer, SIGNAL("itemText( index )"),self.datap.setFile)
        QObject.connect(self.pluginGui.RollSpinBox, SIGNAL("valueChanged(double)"), 
                     self.kmlview.setChangeRoll)
        QObject.connect(self.pluginGui.PitchSpinBox, SIGNAL("valueChanged(double)"), 
                     self.kmlview.setChangePitch)
        QObject.connect(self.pluginGui.HandlingSpinBox, SIGNAL("valueChanged(double)"), 
                     self.kmlview.setChangeHead)
        QObject.connect(self.pluginGui.ZoomSpinBox, SIGNAL("valueChanged(double)"), 
                     self.kmlview.setChangeZoom)
        QObject.connect(self.pluginGui.RangeSpinBox, SIGNAL("valueChanged(double)"), 
                     self.kmlview.setChangeRange)
        #
        QObject.connect(self.pluginGui.Lon, SIGNAL("textChanged(QString)"), 
                     self.placemodel.setLonValue)
        QObject.connect(self.pluginGui.Lat, SIGNAL("textChanged(QString)"), 
                     self.placemodel.setLatValue)
        QObject.connect(self.pluginGui.RollSpinBox, SIGNAL("valueChanged(double)"), 
                     self.placemodel.setChangeRoll)
        QObject.connect(self.pluginGui.PitchSpinBox, SIGNAL("valueChanged(double)"), 
                     self.placemodel.setChangePitch)
        QObject.connect(self.pluginGui.HandlingSpinBox, SIGNAL("valueChanged(double)"), 
                     self.placemodel.setChangeHead)
        QObject.connect(self.pluginGui.ZoomSpinBox, SIGNAL("valueChanged(double)"), 
                     self.placemodel.setChangeZoom)
        QObject.connect(self.pluginGui.RangeSpinBox, SIGNAL("valueChanged(double)"), 
                     self.placemodel.setChangeRange)
        #
        QObject.connect(self.pluginGui.Lon, SIGNAL("textChanged(QString)"), 
                     self.placemodelite.setLonValue)
        QObject.connect(self.pluginGui.Lat, SIGNAL("textChanged(QString)"), 
                     self.placemodelite.setLatValue)
        QObject.connect(self.pluginGui.RollSpinBox, SIGNAL("valueChanged(double)"), 
                     self.placemodelite.setChangeRoll)
        QObject.connect(self.pluginGui.PitchSpinBox, SIGNAL("valueChanged(double)"), 
                     self.placemodelite.setChangePitch)
        QObject.connect(self.pluginGui.HandlingSpinBox, SIGNAL("valueChanged(double)"), 
                     self.placemodelite.setChangeHead)
        QObject.connect(self.pluginGui.ZoomSpinBox, SIGNAL("valueChanged(double)"), 
                     self.placemodelite.setChangeZoom)
        QObject.connect(self.pluginGui.RangeSpinBox, SIGNAL("valueChanged(double)"), 
                     self.placemodelite.setChangeRange)
        QObject.connect(self.pluginGui.actionSavekml, SIGNAL("triggered()"), 
                     self.kmldialog)
        QObject.connect(self.pluginGui.actionModel, SIGNAL("triggered()"), 
                     self.modeldialog)
        QObject.connect(self.pluginGui.actionQaddfiles, SIGNAL("triggered()"), self.populaterRcombobox)
        QObject.connect(self.pluginGui.actionEpsg, SIGNAL("triggered()"), self.SEpsg)
        QObject.connect(self.pluginGui.Place, SIGNAL("currentIndexChanged(int)"),
                     self.itemlist)
        QObject.connect(self.pluginGui.placezone, SIGNAL("currentIndexChanged(int)"), 
                     self.setplacezonecoords)
        QObject.connect(self.pluginGui.actionVectorOp, SIGNAL("triggered()"), self.Geom)
        QObject.connect(self.pluginGui.addqlayer, SIGNAL("clicked()"),
                     self.sendfile)
        QObject.connect(self.pluginGui.removeqlayer, SIGNAL("clicked()"),
                     self.removefile)
        QObject.connect(self.pluginGui.qgislayer, SIGNAL("currentIndexChanged(int)"), self.selectfile)
        #QObject.connect(self.pluginGui.qgislayer, SIGNAL("currentIndexChanged(int)"), self.printlayers)
        QObject.connect(self.pluginGui.actionQaddfiles, SIGNAL("triggered()"), self.populaterVcombobox)
        QObject.connect(self.pluginGui.addVlayer, SIGNAL("clicked()"), self.sendVfile)
        QObject.connect(self.pluginGui.removeVlayer, SIGNAL("clicked()"), self.removeVfile)
        QObject.connect(self.pluginGui.qgisVlayer, SIGNAL("currentIndexChanged(int)"), self.selectVfile)
        QObject.connect(self.pluginGui.actionDataexp, SIGNAL("triggered()"),
                     self.dataw)
        #QObject.connect(self.pluginGui.Lon, SIGNAL("textChanged(QString)"), self.pdata.setLonValue)
        #QObject.connect(self.pluginGui.Lat, SIGNAL("textChanged(QString)"), self.pdata.setLatValue) 
        self.timer = QTimer()
        QObject.connect(self.timer, SIGNAL("timeout()"),self.runStatus)
        self.timer.start(1000)

        self.pluginGui.show()




    def runStatus(self):
        self.thread.msleep(1)

    def toggleThread(self):
        self.thread.toggle()

    def hide(self):
        if self.pluginGui.actionHide.isChecked():
            self.pluginGui.tabWidget.show()
        else :
            self.pluginGui.tabWidget.hide()

    def setCrossClassLat(self, value):
        #global CrossClassLat
        self.CrossClassLat = value
        self.pluginGui.GPSlat.setText(self.CrossClassLat)
        if self.pluginGui.GPSSend.isChecked():
            self.QgsSetCenter(self.CrossClassLat,self.CrossClassLon)

    def setCrossClassLat2(self, value):
        self.CrossClassLat = value
        self.pluginGui.GPSlat.setText(self.CrossClassLat)
        if self.pluginGui.GPSSend.isChecked():
            self.QgsSetCenter(self.CrossClassLat,self.CrossClassLon)


    def setCrossClassLon(self, value):
        self.CrossClassLon = value
        self.pluginGui.GPSlon.setText(self.CrossClassLon)


    #def setCrossClassSat(self, value):
    #    self.CrossClassSat = value
    #    self.pluginGui.Satellite.setText(self.CrossClassSat)

    def setCrossClassEph(self, value):
        self.CrossClassEph = value
        self.pluginGui.GPSEph.setText(self.CrossClassEph)

    def setCrossClassAltitude(self, value):
        self.CrossClassAltitude = value
        self.pluginGui.GPSAltitude.setText(self.CrossClassAltitude)

    def setCrossClassClimb(self, value):
        self.CrossClassClimb = value
        self.pluginGui.GPSClimb.setText(self.CrossClassClimb)

    def setCrossClassTime(self, value):
        self.CrossClassTime = value
        self.pluginGui.GPSTime.setText(self.CrossClassTime)

    def setCrossClassSpeed(self, value):
        self.CrossClassSpeed = value
        self.pluginGui.GPSSpeed.setText(self.CrossClassSpeed)

    def setCrossClassEpv(self, value):
        self.CrossClassEpv = value
        self.pluginGui.GPSEpv.setText(self.CrossClassEpv)

    def setCrossClassEpt(self, value):
        self.CrossClassEpt = value
        self.pluginGui.GPSEpt.setText(self.CrossClassEpt)

    def setCrossClassUtctime(self, value):
        self.CrossClassUtctime = value
        self.pluginGui.GPSUtctime.setText(self.CrossClassUtctime)

    def setCrossClassTrack(self, value):
        self.CrossClassTrack = value
        self.pluginGui.GPSTrack.setText(self.CrossClassTrack)

    def setCrossClassEpd(self, value):
        self.CrossClassEpd = value
        self.pluginGui.GPSEpd.setText(self.CrossClassEpd)

    def setCrossClassEps(self, value):
        self.CrossClassEps = value
        self.pluginGui.GPSEps.setText(self.CrossClassEps)

    def setCrossClassEpc(self, value):
        self.CrossClassEpc = value
        self.pluginGui.GPSEpc.setText(self.CrossClassEpc)

    def setCrossClassPdop(self, value):
        self.CrossClassPdop = value
        self.pluginGui.GPSPdop.setText(self.CrossClassPdop)

    def setCrossClassHdop(self, value):
        self.CrossClassHdop = value
        self.pluginGui.GPSHdop.setText(self.CrossClassHdop)

    def setCrossClassTdop(self, value):
        self.CrossClassTdop = value
        self.pluginGui.GPSTdop.setText(self.CrossClassTdop)

    def setCrossClassVdop(self, value):
        self.CrossClassVdop = value
        self.pluginGui.GPSVdop.setText(self.CrossClassVdop)

    def setCrossClassGdop(self, value):
        self.CrossClassGdop = value
        self.pluginGui.GPSGdop.setText(self.CrossClassGdop)

    def ellipsesettings(self,index):
        global ell
        #self.ellipsecode = ''
        ell = self.pluginGui.ellipse.itemText(index)
        if ell == 'Airy':
            ell = 1
        if ell == 'Australian National':
            ell = 2
        if ell == 'Bessel 1841':
            ell = 3
        if ell == 'Bessel 1841 (Nambia)':
            ell = 4
        if ell == 'Clarke 1866':
            ell = 5
        if ell == 'Clarke 1880':
            ell = 6
        if ell == 'Everest':
            ell = 7
        if ell == 'Fischer 1960 (Mercury)':
            ell = 8
        if ell == 'Fischer 1968':
            ell = 9
        if ell == 'GRS 1967':
            ell = 10
        if ell == 'GRS 1980':
            ell = 11
        if ell == 'Helmert 1906':
            ell = 12
        if ell == 'Hough':
            ell = 13
        if ell == 'International':
            ell = 14
        if ell == 'Krassovsky':
            ell = 15
        if ell == 'Modified Airy':
            ell = 16
        if ell == 'Modified Everest':
            ell = 17
        if ell == 'Modified Fischer 1960':
            ell = 18
        if ell == 'South American 1969':
            ell = 19
        if ell == 'WGS 60':
            ell = 20
        if ell == 'WGS 66':
            ell = 21
        if ell == 'WGS-72':
            ell = 22
        if ell == 'WGS-84':
            ell = 23
        return ell

    def setValueZoomSpinBox(self, z):
        self.ZoomValue = float(z)
        self.pluginGui.ZoomSpinBox.setRange(-100, 27536977)
        zstep = float(self.pluginGui.ZoomStepSpinBox.value())
        zmult = float(self.pluginGui.ZoomMultipler.value())
        zstep = zstep * (10 ** zmult)
        self.pluginGui.ZoomSpinBox.setSingleStep(zstep)
        self.pluginGui.ZoomSpinBox.setValue(self.ZoomValue)
        
    def setValueZoomSlider(self, z):
        self.ZoomValue = int(z)
        self.pluginGui.ZoomSlider.setMinimum(-100)
        self.pluginGui.ZoomSlider.setMaximum(27536977)
        self.pluginGui.ZoomSlider.setValue(self.ZoomValue)

    def setValueRangeSpinBox(self, r):
        self.RangeValue = float(r)
        self.pluginGui.RangeSpinBox.setRange(-100, 27536977)
        rstep = float(self.pluginGui.RangeStepSpinBox.value())
        rmult = float(self.pluginGui.RangeMultipler.value())
        rstep = rstep * (10 ** rmult)
        self.pluginGui.RangeSpinBox.setSingleStep(rstep)
        self.pluginGui.RangeSpinBox.setValue(self.RangeValue)
        
        
        
    def setValueRangeSlider(self, r):
        self.RangeValue = int(r)
        self.pluginGui.RangeSlider.setMinimum(-100)
        self.pluginGui.RangeSlider.setMaximum(27536977)
        self.pluginGui.RangeSlider.setValue(self.RangeValue)
        

    def setValueRollSpinBox(self, rl):
        self.RollValue = float(rl)
        self.pluginGui.RollSpinBox.setRange(-90, 90)
        self.pluginGui.RollSpinBox.setSingleStep(1)
        self.pluginGui.RollSpinBox.setValue(self.RollValue)
        
        
    def setValueRollSlider(self, rl):
        self.RollValue = int(rl)
        self.pluginGui.RollSlider.setMinimum(-90)
        self.pluginGui.RollSlider.setMaximum(90)
        self.pluginGui.RollSlider.setValue(self.RollValue)

    def setValuePitchSpinBox(self, p):
        self.PitchValue = float(p)
        self.pluginGui.PitchSpinBox.setRange(-90, 90)
        self.pluginGui.PitchSpinBox.setSingleStep(1)
        self.pluginGui.PitchSpinBox.setValue(self.PitchValue)
        
        
    def setValuePitchSlider(self, p):
        self.PitchValue = int(p)
        self.pluginGui.PitchSlider.setMinimum(-90)
        self.pluginGui.PitchSlider.setMaximum(90)
        self.pluginGui.PitchSlider.setValue(self.PitchValue)

    def setValueHandlingSpinBox(self, h):
        self.HandlingValue = float(h)
        self.pluginGui.HandlingSpinBox.setRange(-360, 360)
        self.pluginGui.HandlingSpinBox.setSingleStep(1)
        self.pluginGui.HandlingSpinBox.setValue(self.HandlingValue)
        
    def setValueHandlingSlider(self, h):
        self.HandlingValue = int(h)
        self.pluginGui.HandlingSlider.setMinimum(-360)
        self.pluginGui.HandlingSlider.setMaximum(360)
        self.pluginGui.HandlingSlider.setValue(self.HandlingValue)

    def setValueSpeedSpinBox(self, s):
        self.SpeedValue = float(s)
        self.pluginGui.SpeedSpinBox.setRange(0.01, 1)
        self.pluginGui.SpeedSpinBox.setSingleStep(0.01)
        self.pluginGui.SpeedSpinBox.setValue(self.SpeedValue)

    def setValueSpeedMultipler(self, s):
        self.SpeedM = float(s)
        self.pluginGui.SpeedMultipler.setRange(1, 10)
        self.pluginGui.SpeedMultipler.setSingleStep(1)
        self.pluginGui.SpeedMultipler.setValue(self.SpeedM)

    def setValueZoomMSpinBox(self, s):
        self.ZoomValue = float(s)
        self.pluginGui.ZoomSpinBox.setRange(0.01, 1)
        self.pluginGui.ZoomSpinBox.setSingleStep(0.01)
        self.pluginGui.ZoomSpinBox.setValue(self.ZoomValue)

    def setValueZoomMultipler(self, s):
        self.ZoomStepValue = float(s)
        self.pluginGui.ZoomMultipler.setRange(1, 10)
        self.pluginGui.ZoomMultipler.setSingleStep(1)
        self.pluginGui.ZoomMultipler.setValue(self.ZoomStepValue)
        
    def GetHead(self,index):
        global head
        head = self.pluginGui.Head.itemText(index)
        if head == 'N':
            head = '0'
        if head == 'NE':
            head = '45' 
        if head == 'E':
            head = '90'
        if head == 'SE':
            head = '135'
        if head == 'S':
            head = '180'
        if head == 'SW':
            head = '-135'
        if head == 'W':
            head = '-90'
        if head == 'NW':
            head = '-45'
        if head == 'Auto':
            head = 'angle'
        if head == 'Manual':
            head = 'Manual'
        return head

    def GetViewType(self,index):
        ViewT = self.pluginGui.View.itemText(index)
        return ViewT

    def CeckViewTypeState(self):
        if not self.pluginGui.actionLonLat.isChecked():
            self.worningmessage()
            
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
            print 'use preference setting to set preference'
            self.worningmessagetcp()

    def sendFunction(self):
        heads = head
        host = self.setparamconnection()[0]
        nav = self.setparamconnection()[1]
        if head == str('Manual'):
            heads = self.pluginGui.HandlingSpinBox.value()
        if self.pluginGui.actionLonLat.isChecked():
            (z, e, n) = LLtoUTM(ell, self.fxvallat, self.fxvallon)
            self.pluginGui.Nord.setText(str(n))
            self.pluginGui.East.setText(str(e))
            self.pluginGui.utmcode.setText(str(z))
            ossimxml = '<Set target=":navigator" vref="wgs84">\
            <%s><longitude>%s</longitude><latitude>%s</latitude>\
            <altitude>%s</altitude><heading>%s</heading><pitch>%s</pitch>\
            <roll>%s</roll><altitudeMode>absolute</altitudeMode>\
            <range>%s</range></%s></Set>' % (item, unicode(self.fxvallon), 
                                             unicode(self.fxvallat), 
                                             self.pluginGui.ZoomSpinBox.value(), heads, 
                                             self.pluginGui.PitchSpinBox.value() , 
                                             self.pluginGui.RollSpinBox.value(),
                                             self.pluginGui.RangeSpinBox.value(), item)

        ossim = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        ossim.connect((host, int(nav))) 
        try:
            ossim.send(ossimxml)
            ossim.close()
        except:
            self.CeckViewTypeState()

    def ResetPosition(self):
        heads = head
        host = self.setparamconnection()[0]
        nav = self.setparamconnection()[1]
        if self.pluginGui.actionLonLat.isChecked():
            self.fxvallon = self.SetPosition()[0]
            self.fxvallat = self.SetPosition()[1]
            (z, e, n) = LLtoUTM(ell, self.fxvallat, self.fxvallon)
            self.pluginGui.Nord.setText(str(n))
            self.pluginGui.East.setText(str(e))
            self.pluginGui.utmcode.setText(str(z))
            ossimxml = '<Set target=":navigator" vref="wgs84"><%s>\
            <longitude>%s</longitude><latitude>%s</latitude>\
            <altitude>%s</altitude><heading>%s</heading><pitch>%s</pitch>\
            <roll>%s</roll><altitudeMode>absolute</altitudeMode>\
            <range>%s</range></%s></Set>' % (item, unicode(self.fxvallon), 
                                             unicode(self.fxvallat), 
                                             self.pluginGui.ZoomSpinBox.value(), heads, 
                                             self.pluginGui.PitchSpinBox.value(), 
                                             self.pluginGui.RollSpinBox.value(),
                                             self.pluginGui.RangeSpinBox.value(), item)
            self.pluginGui.Lat.setText(unicode(self.fxvallat))
            self.pluginGui.Lon.setText(unicode(self.fxvallon))

        ossim = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        ossim.connect((host, int(nav)))  
        try:
            ossim.send(ossimxml)
            ossim.close()
        except:
            self.CeckViewTypeState()


    def QgsSetCenter(self,x,y):
        ext=self.canvas.extent() 
        dx = int(5)
        dy = int(5)
        xmin = float(x) - dx
        ymin = float(y) - dy
        xmax = float(x) + dx
        ymax = float(y) + dy
        self.canvas.setExtent(QgsRectangle(xmin, ymin, xmax, ymax ))
        self.canvas.refresh()

    def QgsRecenter(self,dx,dy):
        ext=self.canvas.extent() 
        #dx = dx
        #dy = dy
        self.canvas.setExtent(QgsRectangle(ext.xMinimum()+dx, ext.yMinimum()+dy, ext.xMaximum()+dx, ext.yMaximum()+dy ))
        self.canvas.refresh()
        #QMessageBox.information(self.iface.mainWindow(),"About2",str(filename)+'---'+str(RlayerDict))



    def inclat(self):
        host = self.setparamconnection()[0]
        nav = self.setparamconnection()[1]
        step = float(self.pluginGui.SpeedSpinBox.value())
        mult = float(self.pluginGui.SpeedMultipler.value())
        step = step * (10 ** -mult)
        heads = head
        if head == str('Manual'):
            heads = self.pluginGui.HandlingSpinBox.value()
        if self.pluginGui.actionLonLat.isChecked():
            self.fxvallat += step
            (z, e, n) = LLtoUTM(ell, self.fxvallat, self.fxvallon)
            self.pluginGui.Nord.setText(str(n))
            self.pluginGui.East.setText(str(e))
            self.pluginGui.utmcode.setText(str(z))
            if self.pluginGui.QgsSync.isChecked():
                self.QgsRecenter(0,step)
            ossimxml = '<Set target=":navigator" vref="wgs84"><%s>\
            <longitude>%s</longitude><latitude>%s</latitude>\
            <altitude>%s</altitude><heading>%s</heading><pitch>%s</pitch>\
            <roll>%s</roll><altitudeMode>absolute</altitudeMode>\
            <range>%s</range></%s></Set>' % (item, unicode(self.fxvallon), 
                                             unicode(self.fxvallat), 
                                             self.pluginGui.ZoomSpinBox.value(), heads, 
                                             self.pluginGui.PitchSpinBox.value(), 
                                             self.pluginGui.RollSpinBox.value(),
                                             self.pluginGui.RangeSpinBox.value(), item)
            self.pluginGui.Lat.setText(unicode(self.fxvallat))
            self.pluginGui.Lon.setText(unicode(self.fxvallon))

        ossim = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        ossim.connect((host, int(nav)))  
        try:
            ossim.send(ossimxml)
            ossim.close()
        except:
            self.CeckViewTypeState()

    def declat(self):
        host = self.setparamconnection()[0]
        nav = self.setparamconnection()[1]
        step = float(self.pluginGui.SpeedSpinBox.value())
        mult = float(self.pluginGui.SpeedMultipler.value())
        heads = head
        step = step * (10 ** -mult)
        if head == str('Manual'):
            heads = self.pluginGui.HandlingSpinBox.value()
        if self.pluginGui.actionLonLat.isChecked():
            self.fxvallat -= step
            (z, e, n) = LLtoUTM(ell, self.fxvallat, self.fxvallon)
            self.pluginGui.Nord.setText(str(n))
            self.pluginGui.East.setText(str(e))
            self.pluginGui.utmcode.setText(str(z))
            if self.pluginGui.QgsSync.isChecked():
                self.QgsRecenter(0,-step)
            ossimxml = '<Set target=":navigator" vref="wgs84"><%s>\
            <longitude>%s</longitude><latitude>%s</latitude>\
            <altitude>%s</altitude><heading>%s</heading>\
            <pitch>%s</pitch><roll>%s</roll><altitudeMode>absolute</altitudeMode>\
            <range>%s</range></%s></Set>' % (item, unicode(self.fxvallon), 
                                             unicode(self.fxvallat), 
                                             self.pluginGui.ZoomSpinBox.value(), heads, 
                                             self.pluginGui.PitchSpinBox.value(), 
                                             self.pluginGui.RollSpinBox.value(),
                                             self.pluginGui.RangeSpinBox.value(), item)
            self.pluginGui.Lat.setText(unicode(self.fxvallat))
            self.pluginGui.Lon.setText(unicode(self.fxvallon))
        
        ossim = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        ossim.connect((host, int(nav)))  
        try:
            ossim.send(ossimxml)
            ossim.close()
        except:
            self.CeckViewTypeState()

    def inclon(self):
        host = self.setparamconnection()[0]
        nav = self.setparamconnection()[1]
        step = float(self.pluginGui.SpeedSpinBox.value())
        mult = float(self.pluginGui.SpeedMultipler.value())
        #self.fxvallon += step
        heads = head
        step = step * (10 ** -mult)
        if head == str('Manual'):
            heads = self.pluginGui.HandlingSpinBox.value()
        if self.pluginGui.actionLonLat.isChecked():
            self.fxvallon += step
            (z, e, n) = LLtoUTM(ell, self.fxvallat, self.fxvallon)
            self.pluginGui.Nord.setText(str(n))
            self.pluginGui.East.setText(str(e))
            self.pluginGui.utmcode.setText(str(z))
            if self.pluginGui.QgsSync.isChecked():
                self.QgsRecenter(step,0)
            ossimxml = '<Set target=":navigator" vref="wgs84"><%s>\
            <longitude>%s</longitude><latitude>%s</latitude>\
            <altitude>%s</altitude><heading>%s</heading><pitch>%s</pitch>\
            <roll>%s</roll><altitudeMode>absolute</altitudeMode>\
            <range>%s</range></%s></Set>' % (item, unicode(self.fxvallon), 
                                             unicode(self.fxvallat), 
                                             self.pluginGui.ZoomSpinBox.value(), heads, 
                                             self.pluginGui.PitchSpinBox.value(), 
                                             self.pluginGui.RollSpinBox.value(),
                                             self.pluginGui.RangeSpinBox.value(), item)
            self.pluginGui.Lat.setText(unicode(self.fxvallat))
            self.pluginGui.Lon.setText(unicode(self.fxvallon))
        
        ossim = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        ossim.connect((host, int(nav)))  
        try:
            ossim.send(ossimxml)
            ossim.close()
        except:
            self.CeckViewTypeState()

    def declon(self):
        host = self.setparamconnection()[0]
        nav = self.setparamconnection()[1]
        step = float(self.pluginGui.SpeedSpinBox.value())
        mult = float(self.pluginGui.SpeedMultipler.value())
        heads = head
        step = step * (10 ** -mult)
        if head == str('Manual'):
            heads = self.pluginGui.HandlingSpinBox.value()
        if self.pluginGui.actionLonLat.isChecked():
            self.fxvallon -= step
            (z, e, n) = LLtoUTM(ell, self.fxvallat, self.fxvallon)
            self.pluginGui.Nord.setText(str(n))
            self.pluginGui.East.setText(str(e))
            self.pluginGui.utmcode.setText(str(z))
            if self.pluginGui.QgsSync.isChecked():
                self.QgsRecenter(-step,0)
            ossimxml = '<Set target=":navigator" vref="wgs84"><%s>\
            <longitude>%s</longitude><latitude>%s</latitude>\
            <altitude>%s</altitude><heading>%s</heading><pitch>%s</pitch>\
            <roll>%s</roll><altitudeMode>absolute</altitudeMode>\
            <range>%s</range></%s></Set>' % (item, unicode(self.fxvallon), 
                                             unicode(self.fxvallat), 
                                             self.pluginGui.ZoomSpinBox.value(), heads, 
                                             self.pluginGui.PitchSpinBox.value(), 
                                             self.pluginGui.RollSpinBox.value(),
                                             self.pluginGui.RangeSpinBox.value(), item)
            self.pluginGui.Lat.setText(unicode(self.fxvallat))
            self.pluginGui.Lon.setText(unicode(self.fxvallon))
        
        ossim = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        ossim.connect((host, int(nav)))  
        try:
            ossim.send(ossimxml)
            ossim.close()
        except:
            self.CeckViewTypeState()

    def inclatlon(self):
        host = self.setparamconnection()[0]
        nav = self.setparamconnection()[1]
        step = float(self.pluginGui.SpeedSpinBox.value())
        mult = float(self.pluginGui.SpeedMultipler.value())
        heads = head
        step = step * (10 ** -mult)
        if head == str('Manual'):
            heads = self.pluginGui.HandlingSpinBox.value()
        if self.pluginGui.actionLonLat.isChecked():
            self.fxvallat += step
            self.fxvallon += step
            (z, e, n) = LLtoUTM(ell, self.fxvallat, self.fxvallon)
            self.pluginGui.Nord.setText(str(n))
            self.pluginGui.East.setText(str(e))
            self.pluginGui.utmcode.setText(str(z))
            if self.pluginGui.QgsSync.isChecked():
                self.QgsRecenter(step,step)
            ossimxml = '<Set target=":navigator" vref="wgs84"><%s>\
            <longitude>%s</longitude><latitude>%s</latitude>\
            <altitude>%s</altitude><heading>%s</heading><pitch>%s</pitch>\
            <roll>%s</roll><altitudeMode>absolute</altitudeMode>\
            <range>%s</range></%s></Set>' % (item, unicode(self.fxvallon), 
                                             unicode(self.fxvallat), 
                                             self.pluginGui.ZoomSpinBox.value(), heads, 
                                             self.pluginGui.PitchSpinBox.value(), 
                                             self.pluginGui.RollSpinBox.value(), 
                                             self.pluginGui.RangeSpinBox.value(), item)
            self.pluginGui.Lat.setText(unicode(self.fxvallat))
            self.pluginGui.Lon.setText(unicode(self.fxvallon))
        
        ossim = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        ossim.connect((host, int(nav)))  
        try:
            ossim.send(ossimxml)
            ossim.close()
        except:
            self.CeckViewTypeState()

    def declatlon(self):
        host = self.setparamconnection()[0]
        nav = self.setparamconnection()[1]
        step = float(self.pluginGui.SpeedSpinBox.value())
        mult = float(self.pluginGui.SpeedMultipler.value())
        heads = head
        step = step * (10 ** -mult)
        if head == str('Manual'):
            heads = self.pluginGui.HandlingSpinBox.value()
        if self.pluginGui.actionLonLat.isChecked():
            self.fxvallat -= step
            self.fxvallon -= step
            (z, e, n) = LLtoUTM(ell, self.fxvallat, self.fxvallon)
            self.pluginGui.Nord.setText(str(n))
            self.pluginGui.East.setText(str(e))
            self.pluginGui.utmcode.setText(str(z))
            if self.pluginGui.QgsSync.isChecked():
                self.QgsRecenter(-step,-step)
            ossimxml = '<Set target=":navigator" vref="wgs84"><%s>\
            <longitude>%s</longitude><latitude>%s</latitude>\
            <altitude>%s</altitude><heading>%s</heading><pitch>%s</pitch>\
            <roll>%s</roll><altitudeMode>absolute</altitudeMode>\
            <range>%s</range></%s></Set>' % (item, unicode(self.fxvallon), 
                                             unicode(self.fxvallat), 
                                             self.pluginGui.ZoomSpinBox.value(), heads, 
                                             self.pluginGui.PitchSpinBox.value() , 
                                             self.pluginGui.RollSpinBox.value(), 
                                             self.pluginGui.RangeSpinBox.value(), item)
            self.pluginGui.Lat.setText(unicode(self.fxvallat))
            self.pluginGui.Lon.setText(unicode(self.fxvallon))
        
        ossim = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        ossim.connect((host, int(nav)))  
        try:
            ossim.send(ossimxml)
            ossim.close()
        except:
            self.CeckViewTypeState()

    def inclatdeclon(self):
        host = self.setparamconnection()[0]
        nav = self.setparamconnection()[1]
        step = float(self.pluginGui.SpeedSpinBox.value())
        mult = float(self.pluginGui.SpeedMultipler.value())
        step = step * (10 ** -mult)
        heads = head
        if head == str('Manual'):
            heads = self.pluginGui.HandlingSpinBox.value()
        if self.pluginGui.actionLonLat.isChecked():
            self.fxvallat += step
            self.fxvallon -= step
            (z, e, n) = LLtoUTM(ell, self.fxvallat, self.fxvallon)
            self.pluginGui.Nord.setText(str(n))
            self.pluginGui.East.setText(str(e))
            self.pluginGui.utmcode.setText(str(z))
            if self.pluginGui.QgsSync.isChecked():
                self.QgsRecenter(-step,step)
            ossimxml = '<Set target=":navigator" vref="wgs84"><%s>\
            <longitude>%s</longitude><latitude>%s</latitude>\
            <altitude>%s</altitude><heading>%s</heading><pitch>%s</pitch>\
            <roll>%s</roll><altitudeMode>absolute</altitudeMode>\
            <range>%s</range></%s></Set>' % (item, unicode(self.fxvallon), 
                                             unicode(self.fxvallat), 
                                             self.pluginGui.ZoomSpinBox.value(), heads, 
                                             self.pluginGui.PitchSpinBox.value(), 
                                             self.pluginGui.RollSpinBox.value(), 
                                             self.pluginGui.RangeSpinBox.value(), item)
            self.pluginGui.Lat.setText(unicode(self.fxvallat))
            self.pluginGui.Lon.setText(unicode(self.fxvallon))
        
        ossim = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        ossim.connect((host, int(nav)))  
        try:
            ossim.send(ossimxml)
            ossim.close()
        except:
            self.CeckViewTypeState()

    def declatinclon(self):
        host = self.setparamconnection()[0]
        nav = self.setparamconnection()[1]
        step = float(self.pluginGui.SpeedSpinBox.value())
        mult = float(self.pluginGui.SpeedMultipler.value())
        step = step * (10 ** -mult)
        heads = head
        if head == str('Manual'):
            heads = self.pluginGui.HandlingSpinBox.value()
        if self.pluginGui.actionLonLat.isChecked():
            self.fxvallat -= step
            self.fxvallon += step
            (z, e, n) = LLtoUTM(ell, self.fxvallat, self.fxvallon)
            self.pluginGui.Nord.setText(str(n))
            self.pluginGui.East.setText(str(e))
            self.pluginGui.utmcode.setText(str(z))
            if self.pluginGui.QgsSync.isChecked():
                self.QgsRecenter(step,-step)
            ossimxml = '<Set target=":navigator" vref="wgs84"><%s>\
            <longitude>%s</longitude><latitude>%s</latitude>\
            <altitude>%s</altitude><heading>%s</heading><pitch>%s</pitch>\
            <roll>%s</roll><altitudeMode>absolute</altitudeMode>\
            <range>%s</range></%s></Set>' % (item, unicode(self.fxvallon), 
                                             unicode(self.fxvallat), 
                                             self.pluginGui.ZoomSpinBox.value(), heads, 
                                             self.pluginGui.PitchSpinBox.value(), 
                                             self.pluginGui.RollSpinBox.value(), 
                                             self.pluginGui.RangeSpinBox.value(), item)
            self.pluginGui.Lat.setText(unicode(self.fxvallat))
            self.pluginGui.Lon.setText(unicode(self.fxvallon))
        
        ossim = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        ossim.connect((host, int(nav)))  
        try:
            ossim.send(ossimxml)
            ossim.close()
        except:
            self.CeckViewTypeState()

    def SetLonLat(self):
        host = self.setparamconnection()[0]
        nav = self.setparamconnection()[1]
        heads = head
        if head == str('Manual'):
            heads = self.pluginGui.HandlingSpinBox.value()
        (z, e, n) = LLtoUTM(ell, float(self.pluginGui.Lat.text()), float(self.pluginGui.Lon.text()))
        self.pluginGui.Nord.setText(str(n))
        self.pluginGui.East.setText(str(e))
        self.pluginGui.utmcode.setText(str(z))
        #self.QgsRecenter(0,0)
        ossimxml = '<Set target=":navigator" vref="wgs84"><%s>\
        <longitude>%s</longitude><latitude>%s</latitude>\
        <altitude>%s</altitude><heading>%s</heading><pitch>%s</pitch>\
        <roll>%s</roll><altitudeMode>absolute</altitudeMode>\
        <range>%s</range></%s></Set>' % (item, unicode(self.pluginGui.Lon.text()), 
                                         unicode(self.pluginGui.Lat.text()), 
                                         self.pluginGui.ZoomSpinBox.value(), heads, 
                                         self.pluginGui.PitchSpinBox.value(), 
                                         self.pluginGui.RollSpinBox.value(), 
                                         self.pluginGui.RangeSpinBox.value(), item)
        ossim = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        ossim.connect((host, int(nav)))  
        try:
            ossim.send(ossimxml)
            ossim.close()
        except:
            self.CeckViewTypeState()


    def receiveposition(self):
        PORT = 9000
        service = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        service.bind((socket.gethostbyname(socket.gethostname()), PORT))
        service.listen(4096)
        print "listening on port", PORT
        channel, info = service.accept()
        print "connection from", info
        while 1:
            # serve forever
            buf = channel.recv(2**10)
            if buf != "":
                print buf
 
        channel.close()
        service.close()




    def setValue(self, v):
        self.Value = v

    def setValueM(self, v):
        self.Value = v

    def setValueZ(self, v):
        self.Value = v

    def setValueZM(self, v):
        self.Value = v

    def SetPosition(self):
        global centro
        lon = float(self.pluginGui.Lon.text())
        lat = float(self.pluginGui.Lat.text())
        centro = (lon,lat)
        return centro
    
    def processdata(self):
        self.pdata.show()
    
    def preference(self):
        #self.preferencesetting = PreferenceSetting()
        self.preferencesetting.show()

    def worningmessage(self):
        self.worn = worn()
        self.worn.show()

    def worningmessagetcp(self):
        self.worntcp = worntcp()
        self.worntcp.show()

    def kmldialog(self):
        #self.kmlview = KmlView()
        self.kmlview.show()
        
    def dataw(self):
        #self.datap = datap()
        self.datap.show()

    def modeldialog(self):
        #self.placemodel = PlaceModel()
        self.placemodel.show()

    #def modeldialog(self):
    #    self.placemodelite.show()

    def SEpsg(self):
        #self.searchepsg = SearchEpsg()
        self.searchepsg.show()

    def Geom(self):
        #self.Geom = Geom()
        #self.vectoroperation = VectorOperation()
        self.vectoroperation.show()

    def itemlist(self,index):
        Zone = self.pluginGui.Place.itemText(index)
        database_name = sqlitedb
        db_connection = sqlite3.connect(database_name)
        db_cursor = db_connection.cursor()
        listatabelle = db_cursor.execute("SELECT name,latitude,longitude FROM %s ;" % (Zone))
        tabelle = listatabelle.fetchall()
        tablelist = []
        allist = []
        for i in tabelle:
            tablelist.append(i[0])
            allist.append(i[0]+' '+str(i[1])+' '+str(i[2]))
        allist.sort()
        tablelist.sort()
        self.pluginGui.placezone.clear()
        self.pluginGui.placezone.addItems(allist)
        db_connection.commit()

    def setplacezonecoords(self,index):
        Placename = self.pluginGui.placezone.itemText(index)
        st = unicode(Placename)
        st = st.split(' ')
        lat = st[-2]
        lon = st[-1]
        self.pluginGui.Lon.setText(lon)
        self.pluginGui.Lat.setText(lat)

# 301

    def populaterRcombobox(self):
        global RlayerDict
        RlayerDict = {}
        nLayers=self.canvas.layerCount()
        self.pluginGui.qgislayer.clear()
        for l in range(nLayers):
            layer = self.canvas.layer(l)
            if layer and layer.type() == QgsMapLayer.RasterLayer:
                #self.pluginGui.qgislayer.addItem(unicode(layer.source()))
                RlayerDict[unicode(layer.name())] = unicode(layer.source())
                self.pluginGui.qgislayer.addItem(unicode(layer.name()))    
        #QMessageBox.information(self.iface.mainWindow(),"About",str(RlayerDict))

# 312

    def selectfile(self,index):
        if not self.pluginGui.qgislayer.count(): return 
        global fileselected
        filename = self.pluginGui.qgislayer.itemText(index)
        #QMessageBox.information(self.iface.mainWindow(),"About2",str(filename)+'---'+str(RlayerDict))
        fileselected = RlayerDict[str(filename)]
        #fileselected = self.pluginGui.qgislayer.itemText(index)
        #QMessageBox.information(self.iface.mainWindow(),"About",str(fileselected)+'------'+str(RlayerDict))
        return fileselected

    def populaterVcombobox(self):
        global VlayerDict
        VlayerDict = {}
        nLayers=self.canvas.layerCount()
        self.pluginGui.qgisVlayer.clear()
        for l in range(nLayers):
            layer = self.canvas.layer(l)
            if layer and layer.type() == QgsMapLayer.VectorLayer:
                VlayerDict[unicode(layer.name())] = unicode(layer.source())
                self.pluginGui.qgisVlayer.addItem(unicode(layer.name()))

    def selectVfile(self,index):
        if not self.pluginGui.qgisVlayer.count(): return 
        global Vfileselected
        Vfilename = self.pluginGui.qgisVlayer.itemText(index)
        Vfileselected = VlayerDict[str(Vfilename)]
        #Vfileselected = self.pluginGui.qgisVlayer.itemText(index)
        return Vfileselected

    def sendfile(self):
        if not fileselected :return
        host = self.setparamconnection()[0]
        data = self.setparamconnection()[2]
        ossim = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        ossim.connect((host, int(data)))
        ossimxml = "<Add target=':idolbridge'><Image groupType='groundTexture'><filename>%s</filename> <id>%s</id><name>%s</name></Image></Add>" % (fileselected,fileselected,fileselected)
        ossim.send(ossimxml)
        ossim.close()

     
    def removefile(self):
        host = self.setparamconnection()[0]
        data = self.setparamconnection()[2]
        ossim = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        ossim.connect((host, int(data)))
        ossimxml = "<Remove target=':idolbridge' id='%s' />" % (fileselected)
        ossim.send(ossimxml)
        ossim.close()

    def sendVfile(self):
        if not Vfileselected :return
        host = self.setparamconnection()[0]
        data = self.setparamconnection()[2]
        ossim = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        ossim.connect((host, int(data)))
        ossimxml = "<Add target=':idolbridge'><Image groupType='groundTexture'><filename>%s</filename> <id>%s</id><name>%s</name></Image></Add>" % (Vfileselected,Vfileselected,Vfileselected)
        ossim.send(ossimxml)
        ossim.close()

        
        
    def removeVfile(self):
        host = self.setparamconnection()[0]
        data = self.setparamconnection()[2]
        ossim = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        ossim.connect((host, int(data)))
        ossimxml = "<Remove target=':idolbridge' id='%s' />" % (Vfileselected)
        ossim.send(ossimxml)
        ossim.close()

    def addRasterToQgis(self):
        QgsApplication.setPrefixPath("/Users/Shared/source/qgis/mac/xcode/build/native/Debug/Qgis.app/Contents/MacOS", True)
        QgsApplication.initQgis()
        fileName = "/path/to/raster/file.tif"
        fileInfo = QFileInfo(fileName)
        baseName = fileInfo.baseName()
        rlayer = QgsRasterLayer(fileName, baseName)
        QgsMapLayerRegistry.instance().addMapLayer(rlayer)

    def gettablelist(self):
        database_name = sqlitedb
        db_connection = sqlite3.connect(database_name)
        db_cursor = db_connection.cursor()
        listatabelle = db_cursor.execute("SELECT name FROM sqlite_master where type = 'table';")
        tabelle = listatabelle.fetchall()
        tablelist = []
        for i in tabelle:
            tablelist.append(i[0])
        db_connection.commit()
        tablelist.sort()
        return tablelist
        
    def printoutcoords(self,xy):
        transform = self.canvas.getCoordinateTransform()
        pt = transform.toMapCoordinates(xy['x'],xy['y'])
        #QMessageBox.information(self.iface.mainWindow(),"About coordinate",str(dir(self.canvas.mapRenderer().destinationSrs())))
        #srcprj = self.canvas.mapRenderer().destinationSrs()
        srcprj = self.canvas.currentLayer().srs()
        destprj = QgsCoordinateReferenceSystem(4326)
        xform = QgsCoordinateTransform(srcprj,destprj)
        pt1 = xform.transform(pt)  
        #QMessageBox.information(self.iface.mainWindow(),"About coordinate",str(srcprj.srsid())+'----'+str(pt1.x())+str(pt1.y()))
        # render
        # check if coordinates are in lon-lat decimal-degres dimension
        # if no , convert it to epsg 4326 (lonlat wgs 84)
        
        self.pluginGui.Lon.setText(str(pt1.x()))
        self.pluginGui.Lat.setText(str(pt1.y()))
        host = self.setparamconnection()[0]
        nav = self.setparamconnection()[1]
        heads = head
        self.fxvallon = pt1.x()
        self.fxvallat = pt1.y()		
        if head == str('Manual'):
            heads = self.pluginGui.HandlingSpinBox.value()
        ossimxml = '<Set target=":navigator" vref="wgs84"><%s>\
            <longitude>%s</longitude><latitude>%s</latitude>\
            <altitude>%s</altitude><heading>%s</heading><pitch>%s</pitch>\
            <roll>%s</roll><altitudeMode>absolute</altitudeMode>\
            <range>%s</range></%s></Set>' % (item, unicode(pt1.x()), 
                                             unicode(pt1.y()), 
                                             self.pluginGui.ZoomSpinBox.value(), heads, 
                                             self.pluginGui.PitchSpinBox.value(), 
                                             self.pluginGui.RollSpinBox.value(), 
                                             self.pluginGui.RangeSpinBox.value(), item)
        ossim = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        ossim.connect((host, int(nav)))  
        try:
            ossim.send(ossimxml)
            ossim.close()
        except:
            self.CeckViewTypeState()