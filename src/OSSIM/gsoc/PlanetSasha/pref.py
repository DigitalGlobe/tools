#!/usr/bin/env python
import sys

from PyQt4.QtCore import *
from PyQt4.QtGui import *

from preference_ui import Ui_Form
import os
import os.path
from xml.dom import minidom 

apppath = os.path.abspath(os.path.dirname(sys.argv[0]))
configfile = '%s/conf/conf.xml' % (apppath)

class PreferenceSetting(QWidget, Ui_Form):
    def __init__(self):
        QWidget.__init__(self)
        self.setupUi(self)
        self.connect(self.selectpref, SIGNAL("clicked()"), self.chooseossimpref)
        self.connect(self.selectgpsdevice, SIGNAL("clicked()"), self.choosegps)
        self.connect(self.selectgeonamesdb, SIGNAL("clicked()"), self.choosesqlitedb)
        self.connect(self.selectVrt, SIGNAL("clicked()"), self.choosevrtdir)
        self.connect(self.selectKml, SIGNAL("clicked()"), self.choosekmldir)
        self.connect(self.SaveSettings, SIGNAL("clicked()"), self.setossimpreference)
        self.connect(self.SaveSettings, SIGNAL("clicked()"), self.setgpsdevice)
        self.connect(self.SaveSettings, SIGNAL("clicked()"), self.setgeonamesdb)
        self.connect(self.SaveSettings, SIGNAL("clicked()"), self.SaveConnectionParam)
        self.connect(self.SaveSettings, SIGNAL("clicked()"), self.SaveTcpParam)
        self.connect(self.SaveSettings, SIGNAL("clicked()"), self.SaveGrassParam)
        self.connect(self.SaveSettings, SIGNAL("clicked()"), self.setfilemanager)
        self.connect(self.SaveSettings, SIGNAL("clicked()"), self.setvrtdir)
        self.connect(self.SaveSettings, SIGNAL("clicked()"), self.setkmldir)
        self.connect(self.SaveSettings, SIGNAL("clicked()"), self.printconf)
        #self.connect(self.SaveSettings, SIGNAL("clicked()"), self.quit)
        if os.path.isfile(configfile):
            host = self.parseOutputconf()['host']
            navport = self.parseOutputconf()['pport']
            dataport = self.parseOutputconf()['dport']
            ConnectionName = self.parseOutputconf()['connection']
            DbHost = self.parseOutputconf()['host']
            DbName = self.parseOutputconf()['dbname']
            DbPort = self.parseOutputconf()['pgport']
            DbUser = self.parseOutputconf()['user']
            DbPassword = self.parseOutputconf()['passwd']
            kmldir = self.parseOutputconf()['kmldir']
            vrtdir = self.parseOutputconf()['vrtdir']
            filemanager = self.parseOutputconf()['filemanager']
            gpsdevice = self.parseOutputconf()['gpsdevice']
            spatialitedb = self.parseOutputconf()['spatialitedb']
            ossimpreference = self.parseOutputconf()['ossimpreference']
            PointSize = self.parseOutputconf()['PointSize']
            LineWidth = self.parseOutputconf()['LineWidth']
            PenColor = self.parseOutputconf()['PenColor']
            BrushColor = self.parseOutputconf()['BrushColor']
            Thickness = self.parseOutputconf()['Thickness']
            Fill = self.parseOutputconf()['Fill']
            self.hostname.setText(host)
            self.navport.setText(navport)
            self.dataport.setText(dataport)
            self.ConnectionName.setText(ConnectionName)
            self.DbHost.setText(DbHost)
            self.DbName.setText(DbName)
            self.DbPort.setText(DbPort)
            self.DbUser.setText(DbUser)
            self.DbPassword.setText(DbPassword)
            self.KmlDir.setText(kmldir)
            self.VrtDir.setText(vrtdir)
            self.filemanager.setText(filemanager)
            self.gpsdevice.setText(gpsdevice)
            self.geonames.setText(spatialitedb)
            self.ossimpreference.setText(ossimpreference)
            self.PointSize.setText(PointSize)
            self.LineWidth.setText(LineWidth)
            self.PenColor.setText(PenColor)
            self.BrushColor.setText(BrushColor)
            self.Thickness.setText(Thickness)
            self.Fill.setText(Fill)
        

    def parseOutputconf(self):
        xmldoc = minidom.parse(configfile)
        outconf = {}
        planetsashaconf = xmldoc.firstChild
        if planetsashaconf.childNodes[1].childNodes[1].firstChild is not None:
            outconf['host'] = planetsashaconf.childNodes[1].childNodes[1].firstChild.data
        else :
            print 'host not found'
            outconf['host'] = 'None'
        if planetsashaconf.childNodes[1].childNodes[3].firstChild is not None:
            outconf['dport'] = planetsashaconf.childNodes[1].childNodes[3].firstChild.data
        else :
            print 'dport not found'
            outconf['pport'] = 'None'
        if planetsashaconf.childNodes[1].childNodes[5].firstChild is not None:
            outconf['pport'] = planetsashaconf.childNodes[1].childNodes[5].firstChild.data
        else :
            print 'pport not found'
            outconf['dport'] = 'None'
            
        if planetsashaconf.childNodes[3].childNodes[1].firstChild is not None:
            outconf['connection'] = planetsashaconf.childNodes[3].childNodes[1].firstChild.data
        else :
            print 'connection not found'
            outconf['connection'] = 'None'
        if planetsashaconf.childNodes[3].childNodes[3].firstChild is not None:
            outconf['dbhost'] = planetsashaconf.childNodes[3].childNodes[3].firstChild.data
        else :
            print 'dbhost not found'
            outconf['dbhost'] = 'None'
        if planetsashaconf.childNodes[3].childNodes[5].firstChild is not None:
            outconf['dbname'] = planetsashaconf.childNodes[3].childNodes[5].firstChild.data
        else :
            print 'dbname not found'
            outconf['dbname'] = 'None'
        if planetsashaconf.childNodes[3].childNodes[7].firstChild is not None:
            outconf['pgport'] = planetsashaconf.childNodes[3].childNodes[7].firstChild.data
        else :
            print 'pgport not found'
            outconf['pgport'] = 'None'
        if planetsashaconf.childNodes[3].childNodes[9].firstChild is not None:
            outconf['user'] = planetsashaconf.childNodes[3].childNodes[9].firstChild.data
        else :
            print 'user not found'
            outconf['user'] = 'None'
        if planetsashaconf.childNodes[3].childNodes[11].firstChild is not None:
            outconf['passwd'] = planetsashaconf.childNodes[3].childNodes[11].firstChild.data
        else :
            print 'passwd not found'
            outconf['passwd'] = 'None'
            
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

        if planetsashaconf.childNodes[7].childNodes[1].firstChild is not None:
            outconf['PointSize'] = planetsashaconf.childNodes[7].childNodes[1].firstChild.data
        else :
            print 'PointSize not found'
            outconf['PointSize'] = 'None'

        if planetsashaconf.childNodes[7].childNodes[3].firstChild is not None:
            outconf['LineWidth'] = planetsashaconf.childNodes[7].childNodes[3].firstChild.data
        else :
            print 'LineWidth not found'
            outconf['LineWidth'] = 'None'

        if planetsashaconf.childNodes[7].childNodes[5].firstChild is not None:
            outconf['PenColor'] = planetsashaconf.childNodes[7].childNodes[5].firstChild.data
        else :
            print 'PenColor not found'
            outconf['PenColor'] = 'None'

        if planetsashaconf.childNodes[7].childNodes[7].firstChild is not None:
            outconf['BrushColor'] = planetsashaconf.childNodes[7].childNodes[7].firstChild.data
        else :
            print 'BrushColor not found'
            outconf['BrushColor'] = 'None'

        if planetsashaconf.childNodes[7].childNodes[9].firstChild is not None:
            outconf['Thickness'] = planetsashaconf.childNodes[7].childNodes[9].firstChild.data
        else :
            print 'Thickness not found'
            outconf['Thickness'] = 'None'


        if planetsashaconf.childNodes[7].childNodes[11].firstChild is not None:
            outconf['Fill'] = planetsashaconf.childNodes[7].childNodes[11].firstChild.data
        else :
            print 'Fill not found'
            outconf['Fill'] = 'None'


        return  outconf


    def SaveConnectionParam(self):
        DBConnectionName = self.ConnectionName.text()
        DBHost = self.DbHost.text()
        DBDatabase = self.DbName.text()
        DBPort = self.DbPort.text()
        DBUsername = self.DbUser.text()
        DBPassword = self.DbPassword.text()
        param = """%s,%s,%s,%s,%s,%s""" % (DBConnectionName, DBHost, DBDatabase,DBPort, DBUsername, DBPassword)
        print DBConnectionName, DBHost, DBDatabase, DBPort, DBUsername, DBPassword


    def setossimpreference(self):
        OssimPref = self.ossimpreference.text()   
        print OssimPref
            
    def setfilemanager(self):
        FileMan = self.filemanager.text()
        print FileMan
        
    def chooseossimpref(self):
        OssimPref = QFileDialog.getOpenFileName(self,self.ossimpreference.text())
        self.ossimpreference.setText(OssimPref)
        
    def choosekmldir(self):
        kmldir = QFileDialog.getExistingDirectory(self, self.KmlDir.text(), "Select directory:", QFileDialog.ShowDirsOnly)
        self.KmlDir.setText(kmldir)
        
    def choosegps(self):
        GpsDev = QFileDialog.getOpenFileName(self,self.gpsdevice.text())
        self.gpsdevice.setText(GpsDev)
        
    def choosesqlitedb(self):
        geonamesdb = QFileDialog.getOpenFileName(self,self.geonames.text()) 
        self.geonames.setText(geonamesdb)
        
    def choosevrtdir(self):
        vrtdir = QFileDialog.getExistingDirectory(self, self.VrtDir.text(), "Select directory:", QFileDialog.ShowDirsOnly)
        self.VrtDir.setText(vrtdir)
        
    def setgpsdevice(self):
        GpsDev = self.gpsdevice.text()    
        print GpsDev

    def setgeonamesdb(self):
        geonamesdb = self.geonames.text()
        print geonamesdb
        
    def setvrtdir(self):
        vrtdir = self.VrtDir.text()
        print vrtdir
        
    def setkmldir(self):
        kmldir = self.KmlDir.text()
        print kmldir

    def SaveTcpParam(self):
        TCPhost = self.hostname.text()
        TCPnavport = self.navport.text()
        TCPdataport = self.dataport.text()
        TCPparam = """%s,%s,%s""" % (TCPhost, TCPnavport, TCPdataport)
        print TCPhost, TCPnavport, TCPdataport

    def SaveGrassParam(self):
        PointSize = self.PointSize.text()
        LineWidth = self.LineWidth.text()
        PenColor = self.PenColor.text()
        BrushColor = self.BrushColor.text()
        Thickness = self.Thickness.text()
        Fill = self.Fill.text()
        print PointSize, LineWidth, BrushColor, PenColor, Thickness
        
    def printconf(self):
        GpsDev = self.gpsdevice.text() 
        geonamesdb = self.geonames.text()
        vrtdir = self.VrtDir.text()
        kmldir = self.KmlDir.text()
        TCPhost = self.hostname.text()
        TCPnavport = self.navport.text()
        TCPdataport = self.dataport.text()
        FileMan = self.filemanager.text()
        OssimPref = self.ossimpreference.text()   
        DBConnectionName = self.ConnectionName.text()
        DBHost = self.DbHost.text()
        DBDatabase = self.DbName.text()
        DBPort = self.DbPort.text()
        DBUsername = self.DbUser.text()
        DBPassword = self.DbPassword.text()
        mpi = 0
        np = 0
        PointSize = self.PointSize.text()
        LineWidth = self.LineWidth.text()
        BrushColor = self.BrushColor.text()
        PenColor = self.PenColor.text()
        Thickness = self.Thickness.text()
        Fill = self.Fill.text()
        if self.mpi.isChecked():
            mpi = 1
            np = 'spinbox'
        xmltemplate = '''<?xml version="1.0"?>
        <PlanetSashaSettings>
        	<tcp>
        		<host>%s</host> 
        		<dport>%s</dport> 
        		<pport>%s</pport> 
        	</tcp> 
        	<postgres>
        		<connection>%s</connection>
        		<host>%s</host>
        		<dbname>%s</dbname>
        		<pgport>%s</pgport>
        		<user>%s</user>
        		<passwd>%s</passwd>
        	</postgres> 
        	<general>
        		<kmldir>%s</kmldir> 
        		<vrtdir>%s</vrtdir> 
        		<filemanager>%s</filemanager> 
        		<gpsdevice>%s</gpsdevice>
        		<ossimpreference>%s</ossimpreference>
        		<spatialitedb>%s</spatialitedb>
        		<mpi>%s</mpi>
        		<np>%s</np>
        	</general>
            <grass>
        		<PointSize>%s</PointSize> 
        		<LineWidht>%s</LineWidht> 
        		<BrushColor>%s</BrushColor> 
                <PenColor>%s</PenColor> 
                <Thickness>%s</Thickness>
                <Fill>%s</Fill>
        	</grass>
        </PlanetSashaSettings>''' % ( TCPhost, TCPdataport, TCPnavport, DBConnectionName, DBHost, DBDatabase, DBPort, DBUsername, DBPassword,
        kmldir, vrtdir, FileMan, GpsDev, OssimPref, geonamesdb, mpi, np, PointSize, LineWidth, BrushColor, PenColor, Thickness, Fill)
        f=open(configfile, 'w') 
        f.write(xmltemplate)
        f.close()
        
        
        


