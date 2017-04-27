#!/usr/bin/env python
import sys

from PyQt4.QtCore import *
from PyQt4.QtGui import *

from preference_ui import Ui_Form
import os

#apppath = os.path.abspath(os.path.dirname(sys.argv[0]))

apppath = os.path.dirname(__file__)
tcpconf = '%s/conf/tcp.conf' % (apppath)
geonames = '%s/conf/geonames.conf' % (apppath)

class PreferenceSetting(QWidget, Ui_Form):
    def __init__(self):
        QWidget.__init__(self)
        self.setupUi(self)
        self.connect(self.selectpref, SIGNAL("clicked()"), self.setossimpreference)
        self.connect(self.selectgeonamesdb, SIGNAL("clicked()"), self.setgeonamesdb)
        self.connect(self.SaveTCPSettings, SIGNAL("clicked()"), self.SaveTcpParam)
        self.connect(self.selectkmlDir, SIGNAL("clicked()"), self.setkmldir)
        
    def setossimpreference(self):
        OssimPref = QFileDialog.getOpenFileName(self,self.ossimpreference.text())
        if not OssimPref.isEmpty():
            self.ossimpreference.setText(OssimPref)

    # cambiare da file a directory
    def setkmldir(self):
        kmldirectory = QFileDialog.getOpenFileName(self,self.kmldir.text())
        if not kmldirectory.isEmpty():
            self.kmldir.setText(kmldirectory)

    def setgeonamesdb(self):
        GeonamesDb = QFileDialog.getOpenFileName(self,self.geonames.text())
        if not GeonamesDb.isEmpty():
            self.geonames.setText(GeonamesDb)

    def SaveTcpParam(self):
        TCPhost = self.hostname.text()
        TCPnavport = self.navport.text()
        TCPdataport = self.dataport.text()
        TCPparam = """%s,%s,%s""" % (TCPhost, TCPnavport, TCPdataport)
        f=open(tcpconf, 'w')
        f.write(TCPparam)
        f.close()
        print TCPhost, TCPnavport, TCPdataport


