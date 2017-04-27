#!/usr/bin/env python
import sys
from PyQt4.QtCore import *
from PyQt4.QtGui import *
from grass.script.core import *
from GrassShell_ui import Ui_Form
import os
from OssimPython import *

grassenv = gisenv()
history = []


class GrShell(QWidget, Ui_Form):
    def __init__(self):
        QWidget.__init__(self)
        self.setupUi(self)
        self.connect(self.command, SIGNAL("returnPressed(void)"), self.run_command)
        self.connect(self.hist, SIGNAL("currentIndexChanged(int)"),self.readhistocommand)


    def readhistocommand(self,index):
        hcmd = self.hist.itemText(index)
        self.command.setText(hcmd)

    def run_command(self):
        grasshistory =  grassenv['GISDBASE']+'/'+grassenv['LOCATION_NAME']+'/'+ grassenv['MAPSET']+'/.bash_history'
        if not os.path.isfile(grasshistory):
            print grasshistory
            f = open(grasshistory, "w")
            f.flush()
            header = '#'
            f.write(header)
            f.close()
        cmd = str(self.command.text())
        history.append(cmd)
        grasshist = open(grasshistory,"r")
        hlines = grasshist.readlines()
        for i in hlines:
            history.append(i)
        thinhist = {}.fromkeys(history)
        thistory = thinhist.keys()
        thistory.sort()
        self.hist.addItems(thistory)
        stdouterr = os.popen4(cmd)[1].read()
        self.outimageinfo.append(stdouterr)
		
	    
