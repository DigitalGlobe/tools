#!/usr/bin/env python

import sys
import os
import subprocess
import string
import socket
import tempfile
import pygame
import time
from pysqlite2 import dbapi2 as sqlite3

from PyQt4.QtCore import *
from PyQt4.QtGui import *

from gui import GuiWidget
from pref import PreferenceSetting

from msgworn import worn
#from msgworntcp import worntcp

from Savekml import KmlView
from model import PlaceModel
from epsg import SearchEpsg
from LatLongUTMconversion import LLtoUTM
from VectorOp import VectorOperation
from episg import *
from configure import parseOutputconf
import gps
from wifi_joy import startj
from drawer import *
from log import *

try:
    _fromUtf8 = QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

apppath = os.path.abspath(os.path.dirname(sys.argv[0]))

splash = '%s/spla.png' % (apppath)
configfile = '%s/conf/conf.xml' % (apppath)


# clon = 0
# clat = 0
# item = 'LookAt'
head = '0'
ell = 23

try :
    from Gdata import Data
    from GrassShell import GrShell
    from grass.script.core import *
    from psinit import *
    GRASS = 1
except:
    GRASS = 0
    print "GRASS environment not found - set to disabled"




class PlanetSasha(QObject):
    def init(self,arg1):
        self.Value=0
        self.ValueJ=0
        self.setcmd = ''
        self.w = GuiWidget()
        self.w.ZoomSpinBox.setValue(0)
        self.w.RangeSpinBox.setValue(100000)
        self.w.ZoomSlider.setValue(0)
        self.w.RangeSlider.setValue(100000)
        self.queryvalue = 0
        self.head = 'Manual'
        self.item = self.GetViewType(0)
        self.w.tabWidget.removeTab(3)
        #self.w.tabWidget.removeTab(2)
        if GRASS == 0:
            self.w.tabWidget.removeTab(1)
            self.w.actionGrass.setEnabled(False)
            self.w.actionData_2.setEnabled(False)
            self.w.actionGrass.setVisible(False)
            self.w.actionData_2.setVisible(False)
            self.w.actionGrassshell.setEnabled(False)
            self.w.actionGrassshell.setVisible(False)
        self.vectoroperation = VectorOperation()
        self.kmlview = KmlView()
        self.placemodel = PlaceModel()
        self.SearchEpsg = SearchEpsg()
        self.w.Lon.setText("0")
        self.w.Lat.setText("0")
        self.fxvallon = self.SetPosition()[0]
        self.fxvallat = self.SetPosition()[1]
        self.slstep = 1
        #
        self.w.grassvectoroption.hide()
        # Hide/Show Slider
        self.w.actionHideSlider.setChecked(True)
        self.w.actionHideSpinbox.setChecked(True)
        self.w.actionHideStepTool.setChecked(True)
        self.w.actionHide_place_position.setChecked(True)
        self.w.RollSlider.hide()
        self.w.PitchSlider.hide()
        self.w.HandlingSlider.hide()
        self.w.ZoomSlider.hide()
        self.w.RangeSlider.hide()
        # Hide Panel
        self.connect(self.w.actionHideSlider, SIGNAL("triggered()"), 
                     self.hidetool)
        # Pan Toolbox
        self.w.north.setAutoRepeat(True)
        self.w.northeast.setAutoRepeat(True)
        self.w.south.setAutoRepeat(True)
        self.w.east.setAutoRepeat(True)
        self.w.west.setAutoRepeat(True)
        self.w.southwest.setAutoRepeat(True)
        self.w.northwest.setAutoRepeat(True)
        self.w.southeast.setAutoRepeat(True)
        self.connect(self.w.center, SIGNAL("clicked()"),
                     self.ResetPosition)
        self.connect(self.w.north, SIGNAL("clicked()"),
                     self.inclat)
        self.connect(self.w.northeast, SIGNAL("clicked()"),
                     self.inclatlon)
        self.connect(self.w.south, SIGNAL("clicked()"),
                     self.declat)
        self.connect(self.w.east, SIGNAL("clicked()"),
                     self.inclon)
        self.connect(self.w.west, SIGNAL("clicked()"),
                     self.declon)
        self.connect(self.w.southwest, SIGNAL("clicked()"),
                     self.declatlon)
        self.connect(self.w.northwest, SIGNAL("clicked()"),
                     self.inclatdeclon)
        self.connect(self.w.southeast, SIGNAL("clicked()"),
                     self.declatinclon)
        #self.connect(self.w.center, SIGNAL("clicked()"), setcenter)
        # View Slider/SpinBox
        # Zoom
        self.connect(self.w.ZoomSlider, SIGNAL("valueChanged(int)"), 
                     self.setValueZoomSpinBox)
        self.connect(self.w.ZoomSlider, SIGNAL("valueChanged(int)"), 
                     self.sendFunction)
        self.connect(self.w.ZoomSpinBox, SIGNAL("valueChanged(double)"), 
                     self.setValueZoomSlider)
        self.connect(self.w.ZoomSpinBox, SIGNAL("valueChanged(double)"), 
                     self.sendFunction)
        self.connect(self.w.ZoomSlider, SIGNAL("valueChanged()"), 
                     self.setValue)
        self.connect(self.w.ZoomSpinBox, SIGNAL("valueChanged()"), 
                     self.setValue)
        self.connect(self.w.hsZoom, SIGNAL("clicked()"),
                     self.Zoomceckbuttons)
        self.connect(self.w.hsZoom, SIGNAL("clicked()"),
                     self.hideslideZoom)
        self.w.ZoomSpinBox.setSuffix(' m')
        # Range
        self.connect(self.w.RangeSlider, SIGNAL("valueChanged(int)"), 
                     self.setValueRangeSpinBox)
        self.connect(self.w.RangeSlider, SIGNAL("valueChanged(int)"), 
                     self.sendFunction)
        self.connect(self.w.RangeSpinBox, SIGNAL("valueChanged(double)"), 
                     self.setValueRangeSlider)
        self.connect(self.w.RangeSpinBox, SIGNAL("valueChanged(double)"), 
                     self.sendFunction)
        self.connect(self.w.RangeSlider, SIGNAL("valueChanged()"), 
                     self.setValue)
        self.connect(self.w.RangeSpinBox, SIGNAL("valueChanged()"), 
                     self.setValue)
        self.connect(self.w.hsRange, SIGNAL("clicked()"),
                     self.Rangececkbuttons)
        self.connect(self.w.hsRange, SIGNAL("clicked()"),
                     self.hideslideRange)
        self.w.RangeSpinBox.setSuffix(' m')
        # Roll
        self.connect(self.w.RollSlider, SIGNAL("valueChanged(int)"), 
                     self.setValueRollSpinBox)
        self.connect(self.w.RollSlider, SIGNAL("valueChanged(int)"), 
                     self.sendFunction)
        self.connect(self.w.RollSpinBox, SIGNAL("valueChanged(double)"), 
                     self.setValueRollSlider)
        self.connect(self.w.RollSpinBox, SIGNAL("valueChanged(double)"), 
                     self.sendFunction)
        self.connect(self.w.RollSlider, SIGNAL("valueChanged()"), 
                     self.setValue)
        self.connect(self.w.RollSpinBox, SIGNAL("valueChanged()"), 
                     self.setValue)
        self.connect(self.w.hsRoll, SIGNAL("clicked()"),
                     self.Rollceckbuttons)
        self.connect(self.w.hsRoll, SIGNAL("clicked()"),
                     self.hideslideRoll)
        self.w.RollSpinBox.setSuffix('\xB0')
        # Pitch
        self.connect(self.w.PitchSlider, SIGNAL("valueChanged(int)"), 
                     self.setValuePitchSpinBox)
        self.connect(self.w.PitchSlider, SIGNAL("valueChanged(int)"), 
                     self.sendFunction)
        self.connect(self.w.PitchSpinBox, SIGNAL("valueChanged(double)"), 
                     self.setValuePitchSlider)
        self.connect(self.w.PitchSpinBox, SIGNAL("valueChanged(double)"), 
                     self.sendFunction)
        self.connect(self.w.PitchSlider, SIGNAL("valueChanged()"), 
                     self.setValue)
        self.connect(self.w.PitchSpinBox, SIGNAL("valueChanged()"), 
                     self.setValue)
        self.connect(self.w.hsPitch, SIGNAL("clicked()"),
                     self.Pitchceckbuttons)
        self.connect(self.w.hsPitch, SIGNAL("clicked()"),
                     self.hideslidePitch)
        self.w.PitchSpinBox.setSuffix('\xB0')
        # Head
        self.connect(self.w.HandlingSlider, SIGNAL("valueChanged(int)"), 
                     self.setValueHandlingSpinBox)
        self.connect(self.w.HandlingSlider, SIGNAL("valueChanged(int)"), 
                     self.sendFunction)
        self.connect(self.w.HandlingSpinBox, SIGNAL("valueChanged(double)"), 
                     self.setValueHandlingSlider)
        self.connect(self.w.HandlingSpinBox, SIGNAL("valueChanged(double)"), 
                     self.sendFunction)
        self.connect(self.w.HandlingSlider, SIGNAL("valueChanged()"), 
                     self.setValue)
        self.connect(self.w.HandlingSpinBox, SIGNAL("valueChanged()"), 
                     self.setValue)
        self.connect(self.w.Head, SIGNAL("currentIndexChanged(int)"),
                     self.GetHead)
        self.connect(self.w.hsHeading, SIGNAL("clicked()"),
                     self.Headingceckbuttons)
        self.connect(self.w.hsHeading, SIGNAL("clicked()"),
                     self.hideslideHead)
        self.w.HandlingSpinBox.setSuffix('\xB0')
        #self.connect(self.w.HandlingSlider, SIGNAL("valueChanged(int)"), 
        #             self.setValueHandlingSpinBox)
        # Speed
        self.connect(self.w.SpeedSpinBox, SIGNAL("valueChanged(double)"), 
                     self.setValueSpeedSpinBox)
        self.connect(self.w.SpeedSpinBox, SIGNAL("valueChanged()"), 
                     self.setValue)
        self.connect(self.w.SpeedMultipler, SIGNAL("valueChanged(double)"), 
                     self.setValueSpeedMultipler)
        self.connect(self.w.SpeedMultipler, SIGNAL("valueChanged()"), 
                     self.setValueM)
        #self.w.SpeedSpinBox.setSuffix(' m')
        # Zoom/Range Step-Nultipler
        self.connect(self.w.ZoomStepSpinBox, SIGNAL("valueChanged()"), 
                     self.setValueZM)
        self.connect(self.w.RangeStepSpinBox, SIGNAL("valueChanged()"),
                     self.setValueRM)
        self.connect(self.w.ZoomMultipler, SIGNAL("valueChanged(double)"), 
                     self.setValueZoomMultipler)
        self.connect(self.w.ZoomMultipler, SIGNAL("valueChanged()"), 
                     self.setValueZ)
        self.connect(self.w.RangeMultipler, SIGNAL("valueChanged()"), 
                     self.setValueR)
        # ACTIONS
        # GPS
        self.connect(self.w.actionGPS, SIGNAL("triggered()"), 
                     self.startstopgpsx)	
        self.connect(self.w.actionGPS, SIGNAL("triggered()"), 
                     self.GPSunceckbuttons)
        self.connect(self.w.actionGPS, SIGNAL("triggered()"), 
                     self.stopstartgpsx)
        #self.connect(self.w.actionNMEA, SIGNAL("triggered()"),
        #             self.GpsHandling)
        #self.connect(self.w.actionGPS, SIGNAL("triggered()"),
        #             self.GpsHandling)
        # LON LAT
        self.connect(self.w.actionLonLat, SIGNAL("triggered()"),
                     self.LonLatunceckbuttons)
        # GRASS
        #self.connect(self.w.actionGrass, SIGNAL("triggered()"),
        #             self.Grassunceckbuttons)
        # Joistick
        self.connect(self.w.actionJoystick, SIGNAL("triggered()"),
                     self.Joyunceckbuttons)
        self.connect(self.w.actionJoystick, SIGNAL("triggered()"), 
                     self.startstopjoy)	
        self.connect(self.w.actionJoystick, SIGNAL("triggered()"), 
                     self.stopstartjoy)	
        # Broadcast
        self.connect(self.w.actionBroadcast, SIGNAL("triggered()"), 
                     self.Serialunceckbuttons)
        self.connect(self.w.actionBroadcast, SIGNAL("triggered()"), 
                     self.startstoplog)
        self.connect(self.w.actionBroadcast, SIGNAL("triggered()"), 
                     self.stopstartlog)
        # HW
        self.connect(self.w.actionHW, SIGNAL("triggered()"),
                     self.Serialunceckbuttons2)
        self.connect(self.w.actionHW, SIGNAL("triggered()"), 
                     self.stopstartHW)
        self.connect(self.w.actionHW, SIGNAL("triggered()"),
                     self.startstopHW)
        self.connect(self.w.actionDB_setting, SIGNAL("triggered()"),
                     self.pgsetting)
        self.connect(self.w.actionPref, SIGNAL("triggered()"), self.preference)
        self.connect(self.w.actionDataexp, SIGNAL("triggered()"),
                     self.processdata)
        self.connect(self.w.actionData_2, SIGNAL("triggered()"),
                     self.processdata)
        self.connect(self.w.renderoptions, SIGNAL("clicked()"), self.getRenderOptions)
        self.connect(self.w.renderoptions, SIGNAL("clicked()"), self.showrenderoptions)
        # Coordinate Display UTM
        self.connect(self.w.ellipse, SIGNAL("currentIndexChanged(int)"),
                     self.ellipsesettings)
        # View Type
        self.connect(self.w.View, SIGNAL("currentIndexChanged(int)"),
                     self.GetViewType)
        # Send Position
        self.connect(self.w.SendPosition, SIGNAL("clicked()"),
                     self.SetLonLat)
        self.connect(self.w.SendPosition, SIGNAL("clicked()"),
                     self.SetJoyCoords)
        self.connect(self.w.SendPosition, SIGNAL("clicked()"),
                     self.ResetPosition)
        self.connect(self.w.RollSpinBox, SIGNAL("valueChanged(double)"), 
                     self.kmlview.setChangeRoll)
        self.connect(self.w.PitchSpinBox, SIGNAL("valueChanged(double)"), 
                     self.kmlview.setChangePitch)
        self.connect(self.w.HandlingSpinBox, SIGNAL("valueChanged(double)"), 
                     self.kmlview.setChangeHead)
        self.connect(self.w.ZoomSpinBox, SIGNAL("valueChanged(double)"), 
                     self.kmlview.setChangeZoom)
        self.connect(self.w.RangeSpinBox, SIGNAL("valueChanged(double)"), 
                     self.kmlview.setChangeRange)
        self.connect(self.w.actionSavekml, SIGNAL("triggered()"), 
                     self.kmldialog)
        # Place Model (send position to save kml model)
        self.connect(self.w.Lon, SIGNAL("textChanged(QString)"), 
                     self.placemodel.setLonValue)
        self.connect(self.w.Lat, SIGNAL("textChanged(QString)"), 
                     self.placemodel.setLatValue)
        self.connect(self.w.RollSpinBox, SIGNAL("valueChanged(double)"), 
                     self.placemodel.setChangeRoll)
        self.connect(self.w.PitchSpinBox, SIGNAL("valueChanged(double)"), 
                     self.placemodel.setChangePitch)
        self.connect(self.w.HandlingSpinBox, SIGNAL("valueChanged(double)"), 
                     self.placemodel.setChangeHead)
        self.connect(self.w.ZoomSpinBox, SIGNAL("valueChanged(double)"), 
                     self.placemodel.setChangeZoom)
        self.connect(self.w.RangeSpinBox, SIGNAL("valueChanged(double)"), 
                     self.placemodel.setChangeRange)
        self.connect(self.w.actionModel, SIGNAL("triggered()"), 
                     self.modeldialog)
        # Place Model (send position to save kml model-lite)
        #self.connect(self.w.Lon, SIGNAL("textChanged(QString)"), 
        #             self.placemodelite.setLonValue)
        #self.connect(self.w.Lat, SIGNAL("textChanged(QString)"), 
        #             self.placemodelite.setLatValue)
        #self.connect(self.w.RollSpinBox, SIGNAL("valueChanged(double)"), 
        #             self.placemodelite.setChangeRoll)
        #self.connect(self.w.PitchSpinBox, SIGNAL("valueChanged(double)"), 
        #             self.placemodelite.setChangePitch)
        #self.connect(self.w.HandlingSpinBox, SIGNAL("valueChanged(double)"), 
        #             self.placemodelite.setChangeHead)
        #self.connect(self.w.ZoomSpinBox, SIGNAL("valueChanged(double)"), 
        #             self.placemodelite.setChangeZoom)
        #self.connect(self.w.RangeSpinBox, SIGNAL("valueChanged(double)"), 
        #             self.placemodelite.setChangeRange)
        #
        #self.connect(self.w.RollSpinBox, SIGNAL("valueChanged(double)"), 
        #             self.gps.setChangeRoll)
        #self.connect(self.w.PitchSpinBox, SIGNAL("valueChanged(double)"), 
        #             self.gps.setChangePitch)
        #self.connect(self.w.HandlingSpinBox, SIGNAL("valueChanged(double)"), 
        #             self.gps.setChangeHead)
        #self.connect(self.w.ZoomSpinBox, SIGNAL("valueChanged(double)"), 
        #             self.gps.setChangeZoom)
        #self.connect(self.w.RangeSpinBox, SIGNAL("valueChanged(double)"), 
        #             self.gps.setChangeRange)
        #self.connect(self.w.View, SIGNAL("currentIndexChanged(QString) "), 
        #             self.gps.setChangeVtype)
        # Epsg search-tool
        self.connect(self.w.actionEpsg, SIGNAL("triggered()"), self.SEpsg)
        # Geonames sqlite DB    
        self.w.Place.setEditable(1)
        self.w.Place.setAutoCompletion(1)
        self.w.placezone.setEditable(1)
        self.w.placezone.setAutoCompletion(1)   
        self.connect(self.w.Place, SIGNAL("currentIndexChanged(int)"),
                     self.itemlist)
        self.connect(self.w.placezone, SIGNAL("currentIndexChanged(int)"), 
                     self.setplacezonecoords)
        self.connect(self.w.refreshsqlite, SIGNAL("clicked()"), self.refreshsqlitedb)
        # Vector GeoTransform
        self.connect(self.w.actionVectorOp, SIGNAL("triggered()"), self.Geom) 
        # Exit QApp
        #self.connect(self.w.actionExit, SIGNAL("triggered()"), qApp, SLOT("quit()"))
        self.connect(self.w.actionExit, SIGNAL("triggered()"), self.quitAll)
        self.joy = logJ()
        self.log = logS()
        #self.hw = HWS()
        self.gpsx = GpsT()
        #self.gt = gt(self.setcmd)
        #
        #self.w.lineEdit2 = QLineEdit(self.w.tab_6)
        #self.w.lineEdit2.setObjectName(_fromUtf8("lineEdit2"))
        #self.w.verticalLayout_8.addWidget(self.w.lineEdit2)
        compass_widget = DrawCompass(self)
        roll_widget = DrawRoll(self)
        self.w.rotationAngleSpinBox = QSpinBox()
        self.w.rotationAngleSpinBox.setRange(0, 359)
        self.w.rotationAngleSpinBox.setWrapping(True)
        self.w.rotationAngleSpinBox.setSuffix('\xB0')
        self.w.rotationAngleLabel = QLabel("&Heading:")
        self.w.rotationAngleLabel.setBuddy(self.w.rotationAngleSpinBox)
        self.w.verticalLayout_8.addWidget(compass_widget)
        self.w.HLayout = QHBoxLayout()
        self.w.HLayout.addWidget(self.w.rotationAngleLabel)
        self.w.HLayout.addWidget(self.w.rotationAngleSpinBox)
        self.w.verticalLayout_8.addLayout(self.w.HLayout)
        self.w.rotationAngleSpinBox.valueChanged.connect(compass_widget.setRotationAngle)
        self.w.rotationAngleSpinBox.hide()
        self.w.rotationAngleLabel.hide()
        #
        self.w.rotationRollSpinBox = QSpinBox()
        self.w.rotationRollSpinBox.setRange(0, 359)
        self.w.rotationRollSpinBox.setWrapping(True)
        self.w.rotationRollSpinBox.setSuffix('\xB0')
        self.w.rotationRollLabel = QLabel("&Roll:")
        self.w.rotationRollLabel.setBuddy(self.w.rotationRollLabel)
        self.w.verticalLayout_18.addWidget(roll_widget)
        self.w.HLayoutRoll = QHBoxLayout()
        self.w.HLayoutRoll.addWidget(self.w.rotationRollLabel)
        self.w.HLayoutRoll.addWidget(self.w.rotationRollSpinBox)
        self.w.verticalLayout_18.addLayout(self.w.HLayoutRoll)
        self.w.rotationRollSpinBox.valueChanged.connect(roll_widget.setRotationAngle)
        self.w.rotationRollSpinBox.hide()
        self.w.rotationRollLabel.hide()
        # spyderlib
        if arg1 == '-d':
            from spyderlib.widgets import internalshell
            dock = QDockWidget("Python Shell")
            self.pythonshell = internalshell.InternalShell(dock, namespace=globals(),commands=[], multithreaded=False)
            dock.setWidget(self.pythonshell)
            self.w.addDockWidget(Qt.BottomDockWidgetArea, dock)
        if GRASS == 1:
            self.gt = gt(self.setcmd)
            self.Gshell = GrShell()
            self.Data = Data()
            self.slvallon = setcenter()[0]
            self.slvallat = setcenter()[1]
            self.connect(self.w.Lon, SIGNAL("textChanged(QString)"), 
                         self.Data.setLonValue)
            self.connect(self.w.Lat, SIGNAL("textChanged(QString)"), 
                         self.Data.setLatValue)
            self.connect(self.w.GrassRLayer, SIGNAL("currentIndexChanged(int)"), self.selectraster)
            self.connect(self.w.GrassVLayer, SIGNAL("currentIndexChanged(int)"), self.selectvector)
            self.connect(self.w.refreshlayerlist, SIGNAL("clicked()"), self.refreshlayer)
            self.connect(self.w.refreshlayerlist, SIGNAL("clicked()"), self.showrenderoptions)
            self.connect(self.w.addRlayer, SIGNAL("clicked()"), self.addraster)
            self.connect(self.w.removeRlayer, SIGNAL("clicked()"), self.removeraster)
            self.connect(self.w.addVlayer, SIGNAL("clicked()"), self.addvector)
            self.connect(self.w.removeVlayer, SIGNAL("clicked()"), self.removevector)
            self.connect(self.w.actionGrass, SIGNAL("triggered()"),
                         self.Grassunceckbuttons)
            # Grass Shell
            self.connect(self.w.actionGrassshell, SIGNAL("triggered()"), 
                         self.GrassShell)
            self.vectors = VectorList()
            self.rasters = RasterList()
            vect = len(self.vectors)
            rast = len(self.rasters)
            numrow = max(vect,rast)
            self.w.tableWidget.setColumnCount(2)
            self.w.tableWidget.setRowCount(numrow)
            self.w.tableWidget.setEditTriggers(QTableWidget.NoEditTriggers)
            for i in range(rast):
                item = QTableWidgetItem(self.rasters[i])
                item.setTextAlignment(Qt.AlignCenter)
                item.setCheckState(Qt.Unchecked)
                self.w.tableWidget.setItem(i, 0, item)
            for i in range(vect):
                item = QTableWidgetItem(self.vectors[i])
                item.setTextAlignment(Qt.AlignCenter)
                item.setCheckState(Qt.Unchecked)
                self.w.tableWidget.setItem(i, 1, item)
            self.w.gcmd.setEditable(1)
            self.w.gcmd.setAutoCompletion(1)
            self.connect(self.w.update, SIGNAL("clicked()"),self.aggiorna)
            self.connect(self.w.getlisttoquery, SIGNAL("clicked()"),self.getrasterstate)
            self.connect(self.w.getlisttoquery, SIGNAL("clicked()"),self.getvectorstate)
            self.connect(self.w.gcmdexec, SIGNAL("clicked()"),self.commandlist)
            commandlist = self.commandlist()
            self.w.gcmd.addItems(commandlist)
            self.connect(self.w.gcmd, SIGNAL("currentIndexChanged(int)"), self.selectcmd)
            self.connect(self.w.gcmdexec, SIGNAL("clicked()"), 
                         self.startstopGt)
            self.connect(self.w.gcmdexec, SIGNAL("clicked()"), 
                         self.stopstartGt)
            self.w.gcmd.setEditable(1)
            self.w.gcmd.setAutoCompletion(1)
            self.connect(self.w.update, SIGNAL("clicked()"),self.aggiorna)
            self.connect(self.w.getlisttoquery, SIGNAL("clicked()"),self.getrasterstate)
            self.connect(self.w.getlisttoquery, SIGNAL("clicked()"),self.getvectorstate)
            self.connect(self.w.gcmdexec, SIGNAL("clicked()"),self.commandlist)
        self.w.show()
    
    
    def aggiorna(self):
        newlon = str(self.w.Lon.text())
        newlat = str(self.w.Lat.text())
        #'''
        try :
            if self.ViewType == 'LookAt':
                newlon = str(self.w.lookatLon.text())
                newlat = str(self.w.lookatLat.text())
                print 'query mode set to LookAt'
        except :
            print 'query mode set to eye'
        #'''
        self.w.longitude.setText(newlon)
        self.w.latitude.setText(newlat)

    def Jcoords(self):
        newlon = str(self.w.Lon.text())
        newlat = str(self.w.Lat.text())
        return newlon, newlat

    def getrasterstate(self):
        raster = RasterList()
        rast = len(raster)
        rastertoquery = []
        valori = []
        newlon = str(self.w.longitude.text())
        newlat = str(self.w.latitude.text())
        lonlat = str(newlon)+' '+str(newlat)
        f = tempfile.NamedTemporaryFile(delete=False)
        coordsfile = f.name
        f.write(lonlat)
        f.flush()
        f.close
        while 1:
            try:
                c = read_command('m.proj', input=coordsfile, flags='i')
                break
            except IOError:
                time.sleep(0.1) 
        os.unlink(f.name)
        lonlat = parse_key_val(c,'\t')
        lonlat = str(lonlat)
        lonlat = lonlat.replace('{','')
        lonlat = lonlat.replace('}','')
        lonlat = lonlat.replace("'",'')
        lonlat = lonlat.replace(':','')
        lonlat = lonlat.replace('|',' ')
        lonlat = lonlat.split(' ')
        lonlat = str(lonlat[0])+','+str(lonlat[1])
        htmlquery = []
        html = """<TABLE cellpadding="4" style="border: 1px solid \
        000000; border-collapse: collapse;" border="1"><TR><TD>Layer</TD>\
        <TD>Values</TD>"""
        htmlquery.append(html)
        for i in range(rast):
            cell = self.w.tableWidget.item(i, 0)
            if cell.checkState() == 2:
                while 1:
                    try:
                        s = read_command('r.what', input=cell.text(), east_north=lonlat) 
                        break
                    except IOError:
                        time.sleep(0.1)
                attr = str(s)
                attr = attr.replace("|",' ')
                attr = attr.replace('\n',' ')
                attr = attr.split(':')
                attr = str(attr)
                attr = attr.replace("'",'')
                attr = attr.replace('"','')
                attr = attr.replace('[','')
                attr = attr.replace(']','')
                results = str(cell.text())+'  : '+attr
                htmlvalue = '<TR><TD>%s</TD><TD>%s</TD>' % (cell.text() ,attr)
                htmlquery.append(htmlvalue)
                rastertoquery.append(results)
        htmlend = """</TR></TABLE>"""
        htmlquery.append(htmlend)
        rastertoquery = str(rastertoquery)
        rastertoquery = rastertoquery.replace("'",'')
        rastertoquery = rastertoquery.split(',')
        rastertoquery = str(rastertoquery)
        rastertoquery = rastertoquery.replace('[','')
        rastertoquery = rastertoquery.replace(']','')
        htmlquery = str(htmlquery)
        htmlquery = htmlquery.replace("', u'",'')
        htmlquery = htmlquery.replace("']",'')
        htmlquery = htmlquery.replace("['",'')
        htmlquery = htmlquery.replace("', '",'')
        self.w.QueryResultsRaster.setText(htmlquery)
        print rastertoquery

    def getvectorstate(self):
        vector = VectorList()
        vect = len(vector)
        vectortoquery = []
        newlon = str(self.w.longitude.text())
        newlat = str(self.w.latitude.text())
        lonlat = str(newlon)+' '+str(newlat)
        f = tempfile.NamedTemporaryFile(delete=False)
        coordsfile = f.name
        f.write(lonlat)
        f.flush()
        f.close
        while 1:
            try:
                c = read_command('m.proj', input=coordsfile, flags='i') 
                break
            except IOError:
                time.sleep(0.1)
        os.unlink(f.name)
        lonlat = parse_key_val(c,'\t')
        lonlat = str(lonlat)
        lonlat = lonlat.replace('{','')
        lonlat = lonlat.replace('}','')
        lonlat = lonlat.replace("'",'')
        lonlat = lonlat.replace(':','')
        lonlat = lonlat.replace('|',' ')
        lonlat = lonlat.split(' ')
        lonlat = str(lonlat[0])+','+str(lonlat[1])
        htmlquery = []
        html = """<TABLE cellpadding="4" style="border: 1px solid \
        000000; border-collapse: collapse;" border="1"><TR>\
        <TD>Layer</TD><TD>Values</TD>"""
        htmlquery.append(html)
        for i in range(vect):
            cell = self.w.tableWidget.item(i, 1)
            if cell.checkState() == 2:
                while 1:
                    try:
                        s = read_command('v.what', map=cell.text(), east_north=lonlat)
                        break
                    except IOError:
                        time.sleep(0.1)
                attr = parse_key_val(s,':')
                attr = str(attr)
                attr = attr.replace('{','')
                attr = attr.replace('}','')
                attr = attr.replace("'",'')
                attr = attr.replace(': None','')
                htmlvalue = '<TR><TD>%s</TD><TD>%s</TD>' % (cell.text() ,attr)
                htmlquery.append(htmlvalue)
                print attr
        htmlend = """</TR></TABLE>"""
        htmlquery.append(htmlend)
        htmlquery = str(htmlquery)
        htmlquery = htmlquery.replace("', u'",'')
        htmlquery = htmlquery.replace("']",'')
        htmlquery = htmlquery.replace("['",'')
        htmlquery = htmlquery.replace("', '",'')
        htmlquery = htmlquery.replace(",",'<BR>')
        self.w.QueryResultsVector.setText(str(htmlquery))
    
# Set Toolbar Action
    
    def GPSunceckbuttons(self):
        self.w.actionLonLat.setChecked(False)
        if GRASS != 0:
            self.w.actionGrass.setChecked(False)
        if self.w.actionJoystick.isChecked():
            self.joy.stop()
            self.w.actionJoystick.setChecked(False)
        if self.w.actionHW.isChecked():
            self.hw.stop()
            self.w.actionHW.setChecked(False)
        #if self.w.actionSerial.isChecked():
    
    
    def LonLatunceckbuttons(self):
        if self.w.actionGPS.isChecked():
            self.gpsx.stop()
            self.w.actionGPS.setChecked(False)
        if GRASS != 0:
            self.w.actionGrass.setChecked(False)
        if self.w.actionJoystick.isChecked():
            self.joy.stop()
            self.w.actionJoystick.setChecked(False)
        if self.w.actionHW.isChecked():
            self.hw.stop()
            self.w.actionHW.setChecked(False)
    
    
    def Grassunceckbuttons(self):
        if self.w.actionGPS.isChecked():
            self.gpsx.stop()
            self.w.actionGPS.setChecked(False)
        self.w.actionLonLat.setChecked(False)
        if self.w.actionJoystick.isChecked():
            self.joy.stop()
            self.w.actionJoystick.setChecked(False)
        if self.w.actionHW.isChecked():
            self.hw.stop()
            self.w.actionHW.setChecked(False)
    
    
    def Joyunceckbuttons(self):
        self.w.actionGPS.setChecked(False)
        self.w.actionLonLat.setChecked(False)
        if GRASS != 0:
            self.w.actionGrass.setChecked(False)
        self.w.actionHW.setChecked(False)
    
    
    def Serialunceckbuttons(self):
        self.w.actionGPS.setChecked(False)
        self.w.actionLonLat.setChecked(False)
        if GRASS != 0:
            self.w.actionGrass.setChecked(False)
        #self.w.actionJoystick.setChecked(False)
        self.w.actionHW.setChecked(False)


    def Serialunceckbuttons2(self):
        self.w.actionGPS.setChecked(False)
        self.w.actionLonLat.setChecked(False)
        if GRASS != 0:
            self.w.actionGrass.setChecked(False)
        

    

    
    def startstopjoy(self):
        if self.w.actionJoystick.isChecked():
            self.joy = logJ()
            self.joy.start()
            #print 'i am self jcords', self.Jcoords()
            self.joy.toggle(self.Jcoords()[0],self.Jcoords()[1])
            self.connect(self.w.Lon, SIGNAL("textChanged(QString)"), self.joy.setValueLonJ)
            self.connect(self.w.Lat, SIGNAL("textChanged(QString)"), self.joy.setValueLatJ)
            self.SetJoyCoords()
            self.w.Lon.setText(self.w.Lon.text())
            self.w.Lat.setText(self.w.Lat.text())
            print self.w.Lon.text(), self.w.Lat.text()
        else :
            self.joy.stop()
            
    
    
    def stopstartjoy(self):
        if not self.w.actionJoystick.isChecked():
            self.joy = logJ()
            self.joy.stop()
            self.joy.toggle(0,0)
            
    


    def startstopHW(self):
        if self.w.actionHW.isChecked():
            self.hw = HWS()
            self.hw.start()
            self.hw.ValUpdated.connect(self.w.textHW.setText)
            self.hw.toggle()
        else :
            #self.hw = HWS()
            self.hw.stop()

    def stopstartHW(self):
        if not self.w.actionHW.isChecked():
            self.hw = HWS()
            self.hw.stop()
            self.hw.toggle()
    

    def startstoplog(self):
        if self.w.actionBroadcast.isChecked():
            self.log = logS()
            self.log.start()
            self.log.LonUpdated.connect(self.w.Lon.setText)
            self.log.LatUpdated.connect(self.w.Lat.setText)
            self.log.RollUpdated.connect(self.w.RollSpinBox.setValue)
            self.log.PitchUpdated.connect(self.w.PitchSpinBox.setValue)
            self.log.GainUpdated.connect(self.w.HandlingSpinBox.setValue)
            self.log.AltUpdated2.connect(self.w.Alt.setText)
            self.log.AltUpdated.connect(self.w.ZoomSpinBox.setValue)
            self.log.LookAtLonUpdated.connect(self.w.lookatLon.setText)
            self.log.LookAtLatUpdated.connect(self.w.lookatLat.setText)
            self.log.LookAtAltitudeUpdated.connect(self.w.lookatAlt.setText)
            self.log.LookAtRangeUpdated.connect(self.w.RangeSpinBox.setValue)
            
            self.log.toggle()
        else :
            self.log.stop()
    
    
    def stopstartlog(self):
        if not self.w.actionBroadcast.isChecked():
            self.log = logS()
            self.log.stop()
            self.log.toggle()
    
    
    
    def startstopGt(self):
        if self.w.gcmdexec.isChecked():
            self.gt = gt(self.setcmd)
            self.gt.start()
            self.gt.toggle()
        else :
            self.gt.stop()
            
    def stopstartGt(self):
        if not self.w.gcmdexec.isChecked():
            self.gt = gt(self.setcmd)
            self.gt.stop()
            self.gt.toggle()
    


    def startstopgpsx(self):
        if self.w.actionGPS.isChecked():
            self.gpsx = GpsT()
            self.gpsx.start()
            self.gpsx.GPSlatitude.connect(self.w.GPSlat.setText)
            self.gpsx.GPSlongitude.connect(self.w.GPSlon.setText)
            self.gpsx.GPStime.connect(self.w.GPSTime.setText)
            self.gpsx.GPSutctime.connect(self.w.GPSUtctime.setText)
            self.gpsx.GPSaltitude.connect(self.w.GPSAltitude.setText)
            self.gpsx.GPSspeed.connect(self.w.GPSSpeed.setText)
            self.gpsx.GPStrack.connect(self.w.GPSTrack.setText)
            self.gpsx.GPSclimb.connect(self.w.GPSClimb.setText)
            self.gpsx.GPSeph.connect(self.w.GPSEph.setText)
            self.gpsx.GPSept.connect(self.w.GPSEpt.setText)
            self.gpsx.GPSepv.connect(self.w.GPSEpv.setText)
            self.gpsx.GPSepd.connect(self.w.GPSEpd.setText)
            self.gpsx.GPSepc.connect(self.w.GPSEpc.setText)
            self.gpsx.GPSeps.connect(self.w.GPSEps.setText)
            self.gpsx.GPSpdop.connect(self.w.GPSPdop.setText)
            self.gpsx.GPShdop.connect(self.w.GPSHdop.setText)
            self.gpsx.GPSvdop.connect(self.w.GPSVdop.setText)
            self.gpsx.GPStdop.connect(self.w.GPSTdop.setText)
            self.gpsx.GPSgdop.connect(self.w.GPSGdop.setText)
            self.gpsx.GPSsat.connect(self.w.Satellite.setText)
            print self.w.GPSlat.text()
            #print 'qualcosa'
            #if head == str('Manual'):
            #    heads = self.w.HandlingSlider.value()
            #if head == str('Auto'):
            #    heads = self.w.GPSTrack
            #print heads
            head = 'Manual'
            self.gpsx.toggle()
        else :
            self.gpsx.stop()
    
    
    def stopstartgpsx(self):
        if not self.w.actionGPS.isChecked():
            self.gpsx = GpsT()
            #self.gpsx.wait()
            self.gpsx.stop()
            self.gpsx.toggle()
    
    
# Quit Application
        
    def quitAll(self):
        qApp.quit()
    
    
# Hide/Show widget
    
    def Headingceckbuttons(self):
        self.w.hsPitch.setChecked(False)
        self.w.hsRoll.setChecked(False)
    
    
    def Pitchceckbuttons(self):
        self.w.hsHeading.setChecked(False)
        self.w.hsRoll.setChecked(False)
    
    
    def Rollceckbuttons(self):
        self.w.hsHeading.setChecked(False)
        self.w.hsPitch.setChecked(False)
    
        
    def hideslideHead(self):
        if self.w.hsHeading.isChecked():
            self.w.PitchSlider.hide()
            self.w.RollSlider.hide()
            self.w.HandlingSlider.show()
        else :
            self.w.HandlingSlider.hide()
            self.w.PitchSlider.hide()
            self.w.RollSlider.hide()
    
    
    def hideslidePitch(self):
        if self.w.hsPitch.isChecked():
            self.w.HandlingSlider.hide()
            self.w.RollSlider.hide()
            self.w.PitchSlider.show()
        else :
            self.w.HandlingSlider.hide()
            self.w.PitchSlider.hide()
            self.w.RollSlider.hide()
    
    
    def hideslideRoll(self):
        if self.w.hsRoll.isChecked():
            self.w.HandlingSlider.hide()
            self.w.PitchSlider.hide()
            self.w.RollSlider.show()
        else :
            self.w.HandlingSlider.hide()
            self.w.PitchSlider.hide()
            self.w.RollSlider.hide()
    
            
    def Zoomceckbuttons(self):
        self.w.hsRange.setChecked(False)
    
    
    def Rangececkbuttons(self):
        self.w.hsZoom.setChecked(False)
    
        
    def hideslideZoom(self):
        if self.w.hsZoom.isChecked():
            self.w.RangeSlider.hide()
            self.w.ZoomSlider.show()
        else :
            self.w.RangeSlider.hide()
            self.w.ZoomSlider.hide()
    
    
    def hideslideRange(self):
        if self.w.hsRange.isChecked():
            self.w.ZoomSlider.hide()
            self.w.RangeSlider.show()
        else :
            self.w.RangeSlider.hide()
            self.w.ZoomSlider.hide()
    
    
    def hidetool(self):
        if self.w.actionHideSlider.isChecked():
            self.w.tabWidget.show()
        else :
            self.w.tabWidget.hide()    
    
            
# Set Values Slider/SpinBox  
  
    def setValueZoomSpinBox(self, z):
        self.ZoomValue = float(z)
        self.w.ZoomSpinBox.setRange(-10000, 27536977)
        zstep = float(self.w.ZoomStepSpinBox.value())
        zmult = float(self.w.ZoomMultipler.value())
        zstep = zstep * (10 ** zmult)
        self.w.ZoomSpinBox.setSingleStep(zstep)
        self.w.ZoomSpinBox.setValue(self.ZoomValue)
    
    
    def setValueZoomSlider(self, z):
        self.ZoomValue = int(z)
        self.w.ZoomSlider.setMinimum(-10000)
        self.w.ZoomSlider.setMaximum(27536977)
        self.w.ZoomSlider.setValue(self.ZoomValue)
    
    
    def setValueRangeSpinBox(self, r):
        self.RangeValue = float(r)
        self.w.RangeSpinBox.setRange(-10000, 27536977)
        rstep = float(self.w.RangeStepSpinBox.value())
        rmult = float(self.w.RangeMultipler.value())
        rstep = rstep * (10 ** rmult)
        self.w.RangeSpinBox.setSingleStep(rstep)
        self.w.RangeSpinBox.setValue(self.RangeValue)
    
    
    def setValueRangeSlider(self, r):
        self.RangeValue = int(r)
        self.w.RangeSlider.setMinimum(-10000)
        self.w.RangeSlider.setMaximum(27536977)
        self.w.RangeSlider.setValue(self.RangeValue)
    
    
    def setValueRollSpinBox(self, rl):
        self.RollValue = float(rl)
        self.w.RollSpinBox.setRange(-90, 90)
        self.w.RollSpinBox.setSingleStep(1)
        self.w.RollSpinBox.setValue(self.RollValue)
        self.w.rotationRollSpinBox.setRange(-360, 360)
        self.w.rotationRollSpinBox.setSingleStep(1)
        self.w.rotationRollSpinBox.setValue(self.RollValue)
    
    
    def setValueRollSlider(self, rl):
        self.RollValue = int(rl)
        self.w.RollSlider.setMinimum(-90)
        self.w.RollSlider.setMaximum(90)
        self.w.RollSlider.setValue(self.RollValue)
    
    
    def setValuePitchSpinBox(self, p):
        self.PitchValue = float(p)
        self.w.PitchSpinBox.setRange(-90, 90)
        self.w.PitchSpinBox.setSingleStep(1)
        self.w.PitchSpinBox.setValue(self.PitchValue)
    
    
    def setValuePitchSlider(self, p):
        self.PitchValue = int(p)
        self.w.PitchSlider.setMinimum(-90)
        self.w.PitchSlider.setMaximum(90)
        self.w.PitchSlider.setValue(self.PitchValue)
    
    
    def setValueHandlingSpinBox(self, h):
        self.HandlingValue = float(h)
        self.w.HandlingSpinBox.setRange(-360, 360)
        self.w.HandlingSpinBox.setSingleStep(1)
        self.w.HandlingSpinBox.setValue(self.HandlingValue)
        self.w.rotationAngleSpinBox.setRange(-360, 360)
        self.w.rotationAngleSpinBox.setSingleStep(1)
        self.w.rotationAngleSpinBox.setValue(self.HandlingValue)
        
    
    
    def setValueHandlingSlider(self, h):
        self.HandlingValue = int(h)
        self.w.HandlingSlider.setMinimum(-360)
        self.w.HandlingSlider.setMaximum(360)
        self.w.HandlingSlider.setValue(self.HandlingValue)
    
    
    def setValueSpeedSpinBox(self, s):
        self.SpeedValue = float(s)
        self.w.SpeedSpinBox.setRange(0.01, 1)
        self.w.SpeedSpinBox.setSingleStep(0.01)
        self.w.SpeedSpinBox.setValue(self.SpeedValue)
    
    
    def setValueSpeedMultipler(self, s):
        self.SpeedM = float(s)
        self.w.SpeedMultipler.setRange(1, 10)
        self.w.SpeedMultipler.setSingleStep(1)
        self.w.SpeedMultipler.setValue(self.SpeedM)
    
    
    def setValueZoomMSpinBox(self, s):
        self.ZoomValue = float(s)
        self.w.ZoomSpinBox.setRange(0.01, 1)
        self.w.ZoomSpinBox.setSingleStep(0.01)
        self.w.ZoomSpinBox.setValue(self.ZoomValue)
    
    
    def setValueZoomMultipler(self, s):
        self.ZoomStepValue = float(s)
        self.w.ZoomMultipler.setRange(1, 10)
        self.w.ZoomMultipler.setSingleStep(1)
        self.w.ZoomMultipler.setValue(self.ZoomStepValue)
    
    
# Set Heading Mode    
    
    def GetHead(self,index):
        #global head
        head = self.w.Head.itemText(index)
        print head
        if head == 'N':
            self.head = '0'
        if head == 'NE':
            self.head = '45' 
        if head == 'E':
            self.head = '90'
        if head == 'SE':
            self.head = '135'
        if head == 'S':
            self.head = '180'
        if head == 'SW':
            self.head = '-135'
        if head == 'W':
            self.head = '-90'
        if head == 'NW':
            self.head = '-45'
        if head == 'Auto':
            self.head = 'angle'
        if head == 'Manual':
            self.head = 'Manual'
        return self.head
    
    
# Set View Type
    
    def GetViewType(self,index):
        ViewT = self.w.View.itemText(index)
        zoomV = self.w.ZoomSlider.value()
        rangeV = self.w.RangeSlider.value()
        self.ViewType = ViewT
        if ViewT == 'Camera':
            print 'Camera'
            self.w.RangeSlider.setValue(0)
            self.w.RangeSpinBox.setValue(0)
            self.w.ZoomSpinBox.setValue(rangeV)
            self.w.ZoomSlider.setValue(rangeV)
        if ViewT == 'LookAt':
            print 'LookAt'
            self.w.ZoomSpinBox.setValue(0)
            self.w.ZoomSlider.setValue(0)
            self.w.RangeSlider.setValue(zoomV)
            self.w.RangeSpinBox.setValue(zoomV)
        return ViewT
    
    
    def CeckViewTypeState(self):
        if not self.w.actionLonLat.isChecked():
            if GRASS !=0:
                if not self.w.actionGrass.isChecked():
                    self.worningmessage('check a view type')
    
        
# Set Configuration
              
    def setparamconnection(self):
        try :
            host = str(parseOutputconf()['host'])
            nav = parseOutputconf()['pport']
            data = parseOutputconf()['dport']
            return host, nav, data
        except :
            print 'Use preference Panel to set preference'
            self.worningmessage('Use the preference setting to set TCP preference')
    

    def setgrassparam(self):
        try :
            PointSize = str(parseOutputconf()['PointSize'])
            LineWidth = str(parseOutputconf()['LineWidth'])
            PenColor = str(parseOutputconf()['PenColor'])
            BrushColor = str(parseOutputconf()['BrushColor'])
            Thickness = parseOutputconf()['Thickness']
            Fill = parseOutputconf()['Fill']
            print PointSize, LineWidth, PenColor, BrushColor, Thickness, Fill
            return PointSize, LineWidth, PenColor, BrushColor, Thickness, Fill
        except :
            print 'Use preference Panel to set preference'
            self.worningmessage('Use the preference setting to set TCP preference')

    
# Set Position Messages    
    
    def makeactiontemplate(self, item, lon, lat, zoom, heads, pitch, roll, range):
        ossimxml = '<Set target=":navigator" vref="wgs84"><%s>\
        <longitude>%s</longitude><latitude>%s</latitude>\
        <altitude>%s</altitude><heading>%s</heading><pitch>%s</pitch>\
        <roll>%s</roll><altitudeMode>absolute</altitudeMode>\
        <range>%s</range></%s></Set>' % (item, lon, lat, zoom, heads, pitch, roll, range, item)
        return ossimxml
    

    def getxy(self):
        print "fai come sopra"


    def MakeConection(self,ossimxml):
        host = self.setparamconnection()[0].split()
        nav = self.setparamconnection()[1]
        for i in host :
            try:
                ossim = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                ossim.connect((i, int(nav)))
                ossim.send(ossimxml)
                ossim.close()
            except:
                if not self.w.actionBroadcast.isChecked():
                    self.CeckViewTypeState()
                #if not self.w.actionHW.isChecked():
                #    self.CeckViewTypeState()


    def getViewVal(self):
        #lon,lat,zoom,heads,pitch,roll,range = 0,0,0,0,0,0,0
        pitch = self.w.PitchSlider.value()
        roll = self.w.RollSlider.value()
        range = self.w.RangeSlider.value()
        zoom = self.w.ZoomSlider.value()
        heads = self.head
        lon = self.w.Lon.text()
        lat = self.w.Lat.text()
        if heads == str('Manual'):
            heads = self.w.HandlingSlider.value()
        if self.w.actionLonLat.isChecked():
            lat = self.fxvallat
            lon = self.fxvallon
            if self.w.NorthEast.isChecked():
                (z, e, n) = LLtoUTM(ell, lat, lon)
                self.w.Nord.setText(str(n))
                self.w.East.setText(str(e))
                self.w.utmcode.setText(str(z))
        if GRASS != 0:
            if self.w.actionGrass.isChecked():
                lon = self.slvallon
                lat = self.slvallat
                if self.w.NorthEast.isChecked():
                    xy = getlonlat(lon,lat)
                    self.w.Nord.setText(xy[1])
                    self.w.East.setText(xy[0])
        if self.w.actionGPS.isChecked():
            lon = self.w.GPSlon.text()
            lat = self.w.GPSlat.text()
        pos = [lon,lat,zoom,heads,pitch,roll,range]
        return pos
            


    def sendFunction(self):
        pos = self.getViewVal()
        #print pos
        ossimxml = self.makeactiontemplate(self.item, pos[0], pos[1], pos[2], pos[3], pos[4], pos[5], pos[6])
        self.MakeConection(ossimxml)
        self.w.Lat.setText(unicode(pos[1]))
        self.w.Lon.setText(unicode(pos[0]))
        #print 'sendFunction commented'
    

    def ResetPosition(self):
        print 'reset position'
        head = self.head
        if head == str('Manual'):
            self.heads = self.w.HandlingSlider.value()
        if self.w.actionLonLat.isChecked():
            self.fxvallon = self.SetPosition()[0]
            self.fxvallat = self.SetPosition()[1]
            if self.w.NorthEast.isChecked():
                (z, e, n) = LLtoUTM(ell, self.fxvallat, self.fxvallon)
                self.w.Nord.setText(str(n))
                self.w.East.setText(str(e))
                self.w.utmcode.setText(str(z))
            ossimxml = self.makeactiontemplate(self.item, unicode(self.fxvallon), unicode(self.fxvallat), 
                                             self.w.ZoomSlider.value(), 0, 0, 
                                             0, self.w.RangeSlider.value())
            self.w.Lat.setText(unicode(self.fxvallat))
            self.w.Lon.setText(unicode(self.fxvallon))
            self.MakeConection(ossimxml)
        if GRASS != 0:
            if self.w.actionGrass.isChecked():
                self.slvallon = setcenter()[0]
                self.slvallat = setcenter()[1]
                print self.slvallon, self.slvallat
                if self.w.NorthEast.isChecked():
                    xy = getlonlat(self.slvallon,self.slvallat)
                    self.w.Nord.setText(xy[1])
                    self.w.East.setText(xy[0])
            ossimxml = self.makeactiontemplate(self.item, unicode(self.slvallon), unicode(self.slvallat), 
                                             self.w.ZoomSlider.value(), 0, 0, 
                                             0, self.w.RangeSlider.value())
            self.w.Lat.setText(unicode(self.slvallat))
            self.w.Lon.setText(unicode(self.slvallon))
            self.MakeConection(ossimxml)
        if self.w.actionGPS.isChecked():
            #self.CrossClassLon = float(self.CrossClassLon)
            #self.CrossClassLat = float(self.CrossClassLat)
            coordsfile = apppath+'/lonlatfile'
            f = open(coordsfile, "r")
            lonlat = f.readline()
            lonlat = lonlat.split(' ')
            lon = lonlat[0]
            lat = lonlat[1]
            ossimxml = self.makeactiontemplate(self.item, lon, lat, self.w.ZoomSlider.value(), self.heads, 
                                             self.w.PitchSlider.value(), self.w.RollSlider.value(),
                                             self.w.RangeSlider.value())
            self.w.Lat.setText(unicode(self.CrossClassLat))
            self.w.Lon.setText(unicode(self.CrossClassLon))
            self.MakeConection(ossimxml)
            
    


    def pan(self, action):
        step = float(self.w.SpeedSpinBox.value())
        mult = float(self.w.SpeedMultipler.value())
        step = step * (10 ** -mult)
        if self.w.actionLonLat.isChecked():
            if action == 'inclat' :
                self.fxvallat += step
            if action == 'inclon' :
                self.fxvallon += step
            if action == 'declat' :
                self.fxvallat -= step
            if action == 'declon' :
                self.fxvallon -= step
            if action == 'inclatlon' :
                self.fxvallat += step
                self.fxvallon += step
            if action == 'declatlon' :
                self.fxvallat -= step
                self.fxvallon -= step
            if action == 'inclatdeclon' :
                self.fxvallat += step
                self.fxvallon -= step
            if action == 'declatinclon' :
                self.fxvallat -= step 
                self.fxvallon += step
        if GRASS != 0:
            if self.w.actionGrass.isChecked():
                if action == 'inclat' :
                    self.slvallat += step
                if action == 'inclon' :
                    self.slvallon += step
                if action == 'declat' :
                    self.slvallat -= step
                if action == 'declon' :
                    self.slvallon -= step
                if action == 'inclatlon' :
                    self.slvallat += step
                    self.slvallon += step
                if action == 'declatlon' :
                    self.slvallat -= step
                    self.slvallon -= step
                if action == 'inclatdeclon' :
                    self.slvallat += step
                    self.slvallon -= step
                if action == 'declatinclon' :
                    self.slvallat -= step 
                    self.slvallon += step
        pos = self.getViewVal()
        ossimxml = self.makeactiontemplate(self.item, pos[0], pos[1], pos[2], pos[3], pos[4], pos[5], pos[6])
        self.MakeConection(ossimxml)
        self.w.Lat.setText(unicode(pos[1]))
        self.w.Lon.setText(unicode(pos[0]))

    

    def inclat(self):
        self.pan('inclat')

    def inclon(self):
        self.pan('inclon')

    def declat(self):
        self.pan('declat')

    def declon(self):
        self.pan('declon')

    def inclatlon(self):
        self.pan('inclatlon')

    def declatlon(self):
        self.pan('declatlon')

    def inclatdeclon(self):
        self.pan('inclatdeclon')

    def declatinclon(self):
        self.pan('declatinclon')
    

    def SetJoyCoords(self):
        if self.w.actionJoystick.isChecked():
            a = self.w.Lon.text()
            b = self.w.Lat.text()
            aa = float(a) + 1
            bb = float(b) + 1
            self.w.Lon.setText(str(aa))
            self.w.Lat.setText(str(bb))
            self.w.Lon.setText(str(a))
            self.w.Lat.setText(str(b))
    
    
    def SetLonLat(self):
        host = self.setparamconnection()[0]
        nav = self.setparamconnection()[1]
        heads = head
        if head == str('Manual'):
            heads = self.w.HandlingSlider.value()
        if self.w.actionLonLat.isChecked():
            if self.w.NorthEast.isChecked():
                (z, e, n) = LLtoUTM(ell, float(self.w.Lat.text()), float(self.w.Lon.text()))
                self.w.Nord.setText(str(n))
                self.w.East.setText(str(e))
                self.w.utmcode.setText(str(z))
        if GRASS != 0:
            if self.w.actionGrass.isChecked():
                if self.w.NorthEast.isChecked():
                    xy = getlonlat(self.fxvallon,self.fxvallat)
                    print xy
        ossimxml = self.makeactiontemplate(self.item, unicode(self.w.Lon.text()), unicode(self.w.Lat.text()), 
                                         self.w.ZoomSlider.value(), heads, self.w.PitchSlider.value(), 
                                         self.w.RollSlider.value(), self.w.RangeSlider.value())
        ossim = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        ossim.connect((host, int(nav)))  
        try:
            ossim.send(ossimxml)
            ossim.close()
        except:
            if not self.w.actionBroadcast.isChecked():
                self.CeckViewTypeState()
    
    
# Set Cross Widget Values
    
    def setValue(self, v):
        self.Value = v
    
    
    def setValueM(self, v):
        self.Value = v
    
    
    def setValueZ(self, v):
        self.Value = v
    
    
    def setValueR(self, v):
        self.Value = v
    
    
    def setValueRM(self, v):
        self.Value = v
    
    
    def setValueZM(self, v):
        self.Value = v
    
    
# Set ellipsoid (for UTM display)
    
    def ellipsesettings(self,index):
        global ell
        ell = self.w.ellipse.itemText(index)
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
    

    
    def SetPosition(self):
        #global centro
        lon = float(self.w.Lon.text())
        lat = float(self.w.Lat.text())
        centro = (lon,lat)
        return centro
    
    
    def PrintPosition(self, lon, lat, zoom, roll, pitch, heading, range):
        testo = '''Longitude : %s
        Latitude : %s 
        Altitude : %s
        Heading : %s
        Roll : %s
        Pitch : %s
        Range : %s
        ''' % (lon, lat, zoom, roll, pitch, heading, range)
    
    
# Set GRASS Vector/Raster 
   

    
        
    def refreshlayer(self):
        vector = VectorList()
        raster = RasterList()
        self.w.GrassRLayer.clear()
        self.w.GrassVLayer.clear()
        self.w.GrassRLayer.addItems(raster)
        self.w.GrassVLayer.addItems(vector)
        
    def showrenderoptions(self):
        if self.w.renderoptions.isChecked():
            self.w.grassvectoroption.show()
        else :
            self.w.grassvectoroption.hide()


    def getRenderOptions(self):
        PointSize = self.w.PointSize.text()
        LineWidth = self.w.LineWidth.value()
        PenColor = self.w.PenColor.text()
        BrushColor = self.w.BrushColor.text()
        #Tikchness = self.w.Tikchness.text()
        print PointSize, LineWidth, PenColor, BrushColor




    def commandlist(self):
        grassCmd = []
        grassCmd = GetGRASSCmds()
        #grassCmd.append(grassCmds)
        #grassCmds = self.GetGRASSCmds(bin = False)
        #grassCmd.append(grassCmds)
        return grassCmd
    
    #def addcmd(self):
    #    commandlist = self.commandlist()
    #    self.w.gcmd.addItems(commandlist)
    
    
    def selectcmd(self,index):
        #global setcmd
        self.setcmd = self.w.gcmd.itemText(index)
        return self.setcmd
    
            
    def selectraster(self,index):
        global inputR
        inputR = self.w.GrassRLayer.itemText(index)
        return inputR
    
        
    def selectvector(self,index):
        global inputV
        inputV = self.w.GrassVLayer.itemText(index)
        return inputV
    
        
    def addraster(self):
        while 1:
            try:
                #host = str(parseOutputconf()['host'])
                #dport = parseOutputconf()['dport']
                #pport = parseOutputconf()['pport']
                #print host, dport, pport
                host = str(parseOutputconf()['host']).split()
                print host
                for i in host :
                    print i
                    try:
                        run_command('r.planet.py', flags = 'a', map = inputR, host = str(i), dport = str(parseOutputconf()['dport']), pport = str(parseOutputconf()['pport']))
                    except IOError:
                        time.sleep(0.1)
                break
            except IOError:
                time.sleep(0.1)
        print 'add', inputR
    
        
    def removeraster(self):
        while 1:
            try:
                run_command('r.planet.py', flags = 'r', map = inputR, host = str(parseOutputconf()['host']), dport = parseOutputconf()['dport'], pport = parseOutputconf()['pport'])
                break
            except IOError:
                time.sleep(0.1)
        print 'removed', inputR
    
        
    def addvector(self):
        database_name = parseOutputconf()['spatialitedb']
        print database_name
        while 1:
            try:
                print inputV
                mapset = getEnv()['MAPSET']
                #instruction = "v.planet.py -a map='%s'@'%s' brush=111,111,111 pen=111,111,111 size=1,1 fill=0 "  % (inputV, mapset)
                #print instruction
                #os.system(instruction)
                #PointSize = str(parseOutputconf()['PointSize'])
                #LineWidth = str(parseOutputconf()['LineWidth'])
                #PenColor = str(parseOutputconf()['PenColor'])
                #BrushColor = str(parseOutputconf()['BrushColor'])
                Thickness = parseOutputconf()['Thickness']
                Fill = parseOutputconf()['Fill']
                PointSize = self.w.PointSize.text()
                LineWidth = self.w.LineWidth.value()
                PenColor = self.w.PenColor.text()
                BrushColor = self.w.BrushColor.text()
                
                run_command('v.planet.py', flags = 'a', map = str(inputV)+'@'+str(mapset), host = str(parseOutputconf()['host']), dport = parseOutputconf()['dport'], pport = parseOutputconf()['pport'], brush = BrushColor, pen = PenColor, size = PointSize, fill = Fill)
                break
            except IOError:
                time.sleep(0.2)
        print 'add', inputV
    
        
    def removevector(self):
        while 1:
            try:
                #mapsets = read_command('g.mapsets', flags ='l')
                #mapsets.replace('\n','').split()
                #for i in mapsets :
                #    print inputV+str(i)
                #    run_command('v.planet.py', flags = 'r', map = inputV+'@'+str(i), dport = 8000, pport = 7000)
                mapset = getEnv()['MAPSET']
                run_command('v.planet.py', flags = 'r', map = str(inputV)+'@'+str(mapset), host = str(parseOutputconf()['host']), dport = parseOutputconf()['dport'], pport = parseOutputconf()['pport'])
                time.sleep(0.1)
                break
            except IOError:
                time.sleep(0.1)
        print 'removed', inputV
    
            
# Show External Widgets
    
    def pgsetting(self):
        self.pgconn = PgConn()
        self.pgconn.show()
    
    
    def preference(self):
        self.preferencesetting = PreferenceSetting()
        self.preferencesetting.show()
    
    
    def worningmessage(self,text):
        self.worn = worn()
        self.worn.label.setText(text)
        self.worn.show()
    
    
#    def worningmessagetcp(self):
#        self.worntcp = worntcp()
#        self.worntcp.show()
#    
    
    def processdata(self):
        self.Data.show()
    
    
    def dataprocess(self):
        self.DataW.show()
        #self.compass.show()
    
    
    def GrassShell(self):
        if GRASS != 0:
            self.Gshell.show()
    
        
    def kmldialog(self):
        self.kmlview.show()
    
    
    def modeldialog(self):
        self.placemodel.show()
    
     
    def SEpsg(self):
        self.searchepsg = SearchEpsg()
        self.searchepsg.show()
    
    
    def Geom(self):
        self.vectoroperation.show()
    
    
# code not used :
    
    def modeldialog(self):
        self.placemodel.show()
        #lite
    
    
    def setparamconnection2(self):
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
            self.worningmessage('Use the preference setting to set TCP preference')
    
    def GpsHandling(self):
        #print self.item,self.w.ZoomSlider.value(),head,self.w.PitchSlider.value(),
        #self.w.RollSlider.value(),self.w.RangeSlider.value()
        #self.gps = GpsClass(self.item,self.w.ZoomSlider.value(),head,
        #self.w.PitchSlider.value(),self.w.RollSlider.value(),self.w.RangeSlider.value())
        self.gps.show()
    
    
    def GetType(self,index):
        global Gtype
        Gtype = self.w.GrassType.itemText(index)
        if Gtype == 'region':
            while 1:
                try:
                    rg = list_strings('region')
                    break
                except IOError:
                    time.sleep(0.1)
            region = []
            for i in rg:
                rgname = i.split('@', 2)[0]
                region.append(rgname)
            region.sort()
            self.w.InputType.clear()
            self.w.InputType.addItems(region)
        if Gtype == 'vect':
            while 1:
                try:
                    vc = list_strings('vect')
                    break
                except IOError:
                    time.split(0.1)
            vector = []
            for i in vc:
                vecname = i.split('@', 2)[0]
                vector.append(vecname)
            vector.sort()
            self.w.InputType.clear()
            self.w.InputType.addItems(vector)
        if Gtype == 'rast':
            while 1:
                try:
                    rs = list_strings('rast')
                    break
                except IOError:
                    time.sleep(0.1)
            raster = []
            for i in rs:
                rasname = i.split('@', 2)[0]
                raster.append(rasname)
            raster.sort()
            self.w.InputType.clear()
            self.w.InputType.addItems(raster)
        #return GType
    
        
# Refresh SQLite
        
    def refreshsqlitedb(self):
        print 'refresh sqlite'
        tables = self.gettablelist()
        if tables is not None :
            self.w.Place.clear()
            self.w.Place.addItems(tables)
    
    
    def itemlist(self,index):
        Zone = self.w.Place.itemText(index)
        database_name = parseOutputconf()['spatialitedb']
        db_connection = None
        try :
            db_connection = sqlite3.connect(database_name)
        except :
            self.worningmessage('spatialitedb not found')
        if db_connection is not None:
            db_connection = sqlite3.connect(database_name)
            db_cursor = db_connection.cursor()
            try :
                listatabelle = db_cursor.execute("SELECT name,latitude,longitude FROM %s ;" % (Zone))
                tabelle = listatabelle.fetchall()
                tablelist = []
                allist = []
                for i in tabelle:
                    tablelist.append(i[0])
                    allist.append(i[0]+' '+str(i[1])+' '+str(i[2]))
                allist.sort()
                tablelist.sort()
                self.w.placezone.clear()
                self.w.placezone.addItems(allist)
                db_connection.commit()
            except :
                print 'reload sqlite'
    
        
    def setplacezonecoords(self,index):
        Placename = self.w.placezone.itemText(index)
        st = unicode(Placename)
        st = st.split(' ')
        try :
            lat = st[-2]
            lon = st[-1]
            self.w.Lon.setText(lon)
            self.w.Lat.setText(lat)
        except :
            pass
    
        
    def gettablelist(self):
        #database_name = sqlitedb
        database_name = parseOutputconf()['spatialitedb']
        print database_name
        db_connection = None
        try :
            db_connection = sqlite3.connect(database_name)
        except :
            self.worningmessage('spatialitedb not found')
        if db_connection is not None:
            db_cursor = db_connection.cursor()
            listatabelle = db_cursor.execute("SELECT name FROM sqlite_master where type = 'table';")
            tabelle = listatabelle.fetchall()
            tablelist = []
            for i in tabelle:
                tablelist.append(i[0])
            db_connection.commit()
            tablelist.sort()
            return tablelist
    
    


    
class GpsT(QThread):
    GPSlatitude = pyqtSignal(str)
    GPSlongitude = pyqtSignal(str)
    GPSsat = pyqtSignal(str)
    GPStime = pyqtSignal(str)
    GPSeph = pyqtSignal(str)
    GPSspeed = pyqtSignal(str)
    GPSaltitude = pyqtSignal(str)
    GPSepv = pyqtSignal(str)
    GPSept = pyqtSignal(str)
    GPSclimb = pyqtSignal(str)
    satellite = pyqtSignal(str)
    GPSutctime = pyqtSignal(str)
    GPStrack = pyqtSignal(str)
    GPSepd = pyqtSignal(str)
    GPSeps = pyqtSignal(str)
    GPSepc = pyqtSignal(str)
    GPSpdop = pyqtSignal(str)
    GPShdop = pyqtSignal(str)
    GPSvdop = pyqtSignal(str)
    GPStdop = pyqtSignal(str)
    GPSgdop = pyqtSignal(str)
    GPSsat = pyqtSignal(str)
    def __init__(self, parent = None):
        QThread.__init__(self, parent)
        self.alive = 1
        self.running = 0
        print "vivo 1"
    
        
    def run(self):
        print "vivo 2"
        #session = gps.gps()
        while self.alive:
            while self.running:
                os.system('clear')
                session.query('admosyq')
                satellitelist=[]
                html = """<TABLE cellpadding="4" style="border: 1px solid \
                #000000; border-collapse: collapse;" border="1"><TR><TD>PRN</TD>\
                <TD>E</TD><TD>Az</TD><TD>Ss</TD><TD>Used</TD></TR>"""
                satellitelist.append(html)
                for i in session.satellites:                    
                    table = """<TR><TD>%s</TD><TD>%s</TD><TD>%s</TD><TD>%s</TD>\
                    <TD>%s</TD>""" % (i.PRN, i.elevation, i.azimuth, i.ss, i.used)
                    satellitelist.append(table)
                htmlend = """</TR></TABLE>"""
                satellitelist.append(htmlend)
                satstringa = str(satellitelist)
                satstringa = satstringa.replace(",","")
                satstringa = satstringa.replace("[","")
                satstringa = satstringa.replace("]","")
                satstringa = satstringa.replace("'","")
                GPSlatitudex = str(session.fix.latitude)
                self.GPSlatitude.emit(str(session.fix.latitude))
                self.GPSlongitude.emit(str(session.fix.longitude))
                self.GPStime.emit(str(session.utc))
                self.GPSutctime.emit(str(session.fix.time))
                self.GPSaltitude.emit(str(session.fix.altitude))
                self.GPSeph.emit(str(session.fix.eph))
                self.GPSepv.emit(str(session.fix.epv))
                self.GPSept.emit(str(session.fix.ept))
                self.GPSspeed.emit(str(session.fix.speed))
                self.GPSclimb.emit(str(session.fix.climb))
                self.GPStrack.emit(str(session.fix.track))
                self.GPSepd.emit(str(session.fix.epd))
                self.GPSeps.emit(str(session.fix.eps))
                self.GPSepc.emit(str(session.fix.epc))
                self.GPSpdop.emit(str(session.pdop))
                self.GPShdop.emit(str(session.hdop))
                self.GPSvdop.emit(str(session.vdop))
                self.GPStdop.emit(str(session.tdop))
                self.GPSgdop.emit(str(session.gdop))
                self.GPSsat.emit(str(satstringa))
                #print satstringa
                self.msleep(1000)
            #self.msleep(1000)
    
    
    def toggle(self):
        print "vivo 3"
        global session
        session = gps.gps()
        if self.running:
            self.running = 0
        else :
            self.running = 1
    
    
    def stop(self):
        print "vivo 4"
        self.alive = 0
        self.running = 0
        self.wait()
        

class logJ(QThread):
    def __init__(self, parent = None):
        QThread.__init__(self, parent)
        self.valuelonJ = ""
        self.valuelatJ = ""
        self.alive = 1
        self.running = 0
    
    
    def run(self):
        while self.alive:
            while self.running:
                try :
                    startj(float(self.jlon), float(self.jlat))
                except :
                    print 'exit from Joy mode'
    
    
    def toggle(self,x,y):
        self.jlon = x
        self.jlat = y
        #pygame.init()
        if self.running:
            self.running = 0
        else :
            self.running = 1
    
    
    def stop(self):
        pygame.quit()
        self.alive = 0
        self.running = 0
        self.wait()
    
    
    def setValueLonJ(self, valuelonJ):
        self.valuelonJ = valuelonJ
    
    
    def setValueLatJ(self, valuelatJ):
        self.valuelatJ = valuelatJ
    
    
    def aggiorna(self):
        newvaluelon = str(self.valuelonJ)
        newvaluelat = str(self.valuelatJ)
        position = (newvaluelon,newvaluelat)
        print position
        return position
    


if __name__ == "__main__":
    import sys
    import time
    app = QApplication(sys.argv)
    ss = None
    arg1 = ''
    if len(sys.argv) > 1:
        arg1 = sys.argv[1]
    splash_pix = QPixmap(splash)
    splash = QSplashScreen(splash_pix, Qt.WindowStaysOnTopHint)
    #splash_pix_mask = QPixmap(splash_mask)
    #splash.setMask(splash_pix_mask.mask())
    splash.setMask(splash_pix.mask())
    splash.show()
    time.sleep(1)
    app.processEvents()
    p = PlanetSasha()
    p.init(arg1)
    splash.close()
    sys.exit(app.exec_())





