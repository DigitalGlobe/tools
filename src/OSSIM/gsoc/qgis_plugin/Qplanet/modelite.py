#!/usr/bin/env python
import os
import datetime
from pysqlite2 import dbapi2 as sqlite3

# da qt creo un dizionario : sqlitedb,table,latitude,longitude,pointname,lookat,position,model

import sys

from PyQt4.QtCore import *
from PyQt4.QtGui import *

from modelite_ui import Ui_Form
import os

AltitudeMode = ""
apppath = os.path.dirname(__file__)
#apppath = os.path.abspath(os.path.dirname(sys.argv[0]))

class PlaceModelite(QWidget, Ui_Form):
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
        self.connect(self.save, SIGNAL("clicked()"), self.pointdict)
        self.connect(self.altitudemode, SIGNAL("currentIndexChanged(int)"),
                     self.GetAltitudeMode)
        self.connect(self.setcamera, SIGNAL("clicked()"),self.aggiorna_camera)
        self.connect(self.setlonlat, SIGNAL("clicked()"),self.set_position)
        self.connect(self.selectmodel, SIGNAL("clicked()"), self.setOpenModel)

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

    def set_position(self):
        newlon = str(self.lon)
        newlat = str(self.lat)
        self.MLongitude.setText(newlon)
        self.MLatitude.setText(newlat)
        self.MAltitude.setText(str(self.Zoomchange))

    def GetAltitudeMode(self, index):
        global AltitudeMode
        AltitudeMode = self.altitudemode.itemText(index)
        return AltitudeMode

    def setOpenModel(self):
        Mpath = QFileDialog.getOpenFileName(self,self.model.text(),self.tr("ALL Files (*);;Collada Files (*.dae)"))
        if not Mpath.isEmpty():
            self.model.setText(Mpath)

    def pointdict(self):
        name = self.name.text()
        ptd = {}
        ptd['name'] = '%s' % (self.name.text())
        ptd['model'] = '%s' % (self.model.text())
        ptd['dbname'] = '%s' % (self.dbname.text())
        ptd['dbtable'] = '%s' % (self.dbtable.text())
        ptd['altitudemode'] = AltitudeMode
        ptd['longitude'] = '%s' % (self.MLongitude.text())
        ptd['latitude'] = '%s' % (self.MLatitude.text())
        ptd['altitude'] = '%s' % (self.MAltitude.text())
        ptd['heading'] = '%s' % (self.MHeading.text())
        ptd['tilt'] = '%s' % (self.MTilt.text())
        ptd['roll'] = '%s' % (self.MRoll.text())
        ptd['Xscale'] = '%s' % (self.Xscale.text())
        ptd['Yscale'] = '%s' % (self.Yscale.text())
        ptd['Zscale'] = '%s' % (self.Zscale.text())
        ptd['LookHeading'] = '%s' % (self.CameraHeading.text())
        ptd['LookTilt'] = '%s' % (self.CameraTilt.text())
        ptd['LookLatitude'] = '%s' % (self.CameraLatitude.text())
        ptd['LookLongitude'] = '%s' % (self.CameraLongitude.text())
        ptd['LookRange'] = '%s' % (self.CameraRange.text())
        ptd['LookAltitude'] = '%s' % (self.CameraAltitude.text())    
        ptd['time'] = ''
        print ptd
        # if db and table no exists yet
        database_name = ptd['dbname']
        sqlitetable = ptd['dbname']
        db_connection = sqlite3.connect(database_name)
        db_cursor = db_connection.cursor()
        db_cursor.execute('CREATE TABLE %s (latitude REAL, longitude REAL, name TEXT, model TEXT, altitudemode TEXT, altitude REAL, heading REAL, tilt REAL, roll REAL, Xscale REAL, Yscale REAL, Zscale REAL, LookHeading REAL, LookTilt REAL, LookLatitude REAL, LookLongitude REAL, LookRange REAL, LookAltitude REAL, time REAL);' % (sqlitetable))
        db_cursor.execute("SELECT AddGeometryColumn(%s,'geom', 4326, 'POINT', 2);" % (sqlitetable))
        data = ''
        lat = data['latitude']
        lon = data['longitude']
        name = data['name']
        name = "'"+name+"'"
        geom = 'GeomFromText('+"'"+'POINT('+str(data['longitude'])+' '+str(data['latitude'])+')'+"'"+', 4326)'
        db_cursor.execute("INSERT INTO %s (latitude, longitude, name, geom) VALUES (%s, %s, %s, %s);" % (sqlitetable,lat,lon,name,geom)) 
        db_connection.commit()

#    def insertpoint(sqlitedb,table,):
#        database_name = sqlitedb
#        sqlitetable = str("'")+sqlitetable+str("'")
#        db_connection = sqlite3.connect(database_name)
#        db_cursor = db_connection.cursor()
#        db_cursor.execute('CREATE TABLE %s (latitude REAL, longitude REAL, name TEXT);' % (sqlitetable)) 
#        db_cursor.execute("SELECT AddGeometryColumn(%s,'geom', 4326, 'POINT', 2);" % (sqlitetable))
#        #data = ???
#        lat = data['latitude']
#        lon = data['longitude']
#        name = data['name']
#        name = "'"+name+"'"
#        geom = 'GeomFromText('+"'"+'POINT('+str(data['longitude'])+' '+str(data['latitude'])+')'+"'"+', 4326)'
#        db_cursor.execute("INSERT INTO %s (latitude, longitude, name, geom) VALUES (%s, %s, %s, %s);" % (sqlitetable,lat,lon,name,geom)) 
#        db_connection.commit()