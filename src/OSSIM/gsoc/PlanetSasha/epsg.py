#!/usr/bin/env python
import sys

from PyQt4.QtCore import *
from PyQt4.QtGui import *

from epsg_ui import Ui_Form
import os
from episg import *
apppath = os.path.abspath(os.path.dirname(sys.argv[0]))
epsgfile = str(apppath)+'/epsg'

class SearchEpsg(QWidget, Ui_Form):
    def __init__(self):
        QWidget.__init__(self)
        self.setupUi(self)
        self.connect(self.Search, SIGNAL("clicked()"), self.setinput)
        self.connect(self.OutList, SIGNAL("currentIndexChanged(int)"), self.outinfo)

    def setinput(self):
        if self.Code_rb.isChecked():
            print 'code'
            outlist = guioption(epsgfile,'code')
            self.OutList.clear()
            self.OutList.addItems(outlist)
        if self.Title_rb.isChecked():
            print 'title'
            outlist = guioption(epsgfile,'title')
            self.OutList.clear()
            self.OutList.addItems(outlist)
        if self.Params_rb.isChecked():
            print 'param'
            outlist = guioption(epsgfile,'param')
            self.OutList.clear()
            self.OutList.addItems(outlist)


    def outinfo(self,index):
        if self.Code_rb.isChecked():
            if self.Code_cb.isChecked():
                lst = 'c'
            if self.Params_cb.isChecked():
                lst = 'p'
            if self.Title_cb.isChecked():
                lst = 't'
            if self.All_cb.isChecked():
                lst = 'a'
            typeout = self.OutList.itemText(index)
            output = rep3(epsgfile,'code',str(typeout),lst)
            self.printout.setText(str(output)) 
        if self.Title_rb.isChecked():
            if self.Code_cb.isChecked():
                lst = 'c'
            if self.Params_cb.isChecked():
                lst = 'p'
            if self.Title_cb.isChecked():
                lst = 't'
            if self.All_cb.isChecked():
                lst = 'a'
            typeout = self.OutList.itemText(index)
            output = rep3(epsgfile,'title',str(typeout),lst) 
            self.printout.setText(str(output)) 
        if self.Params_rb.isChecked():
            if self.Code_cb.isChecked():
                lst = 'c'
            if self.Params_cb.isChecked():
                lst = 'p'
            if self.Title_cb.isChecked():
                lst = 't'
            if self.All_cb.isChecked():
                lst = 'a'
            typeout = self.OutList.itemText(index)
            output = rep3(epsgfile,'param',str(typeout),lst) 
            self.printout.setText(str(output)) 