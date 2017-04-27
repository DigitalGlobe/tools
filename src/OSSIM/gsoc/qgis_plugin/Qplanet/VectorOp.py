#!/usr/bin/env python
import sys

from PyQt4.QtCore import *
from PyQt4.QtGui import *

from transvector_ui import Ui_Form
import os
from Shp2Shp import sTOsTxy
from Shp2Shp import sTOsRota
from Shp2Shp import sTOsAffine
from Shp2Shp import sTOsConforme

apppath = os.path.dirname(__file__)


class VectorOperation(QWidget, Ui_Form):
    def __init__(self):
        QWidget.__init__(self)
        self.setupUi(self)
        self.connect(self.input_tr, SIGNAL("clicked()"), self.setInput_tr)
        self.connect(self.output_tr, SIGNAL("clicked()"), self.setOutput_tr)
        self.connect(self.traslate, SIGNAL("clicked()"), self.exectraslation)
        self.connect(self.input_rot, SIGNAL("clicked()"), self.setInput_rot)
        self.connect(self.output_rot, SIGNAL("clicked()"), self.setOutput_rot)
        self.connect(self.rotate, SIGNAL("clicked()"), self.execrotation)
        self.connect(self.gcp1, SIGNAL("clicked()"), self.setInput_gcp1)
        self.connect(self.gcp2, SIGNAL("clicked()"), self.setInput_gcp2)
        self.connect(self.input_rep, SIGNAL("clicked()"), self.setInput_rep)
        self.connect(self.output_rep, SIGNAL("clicked()"), self.setOutput_rep)
        
        
    def setInput_tr(self):
        Input = QFileDialog.getOpenFileName()
        if not Input.isEmpty():
            self.input_trlabel.setText(Input)
    
    def setOutput_tr(self):
        Output = QFileDialog.getSaveFileName()
        if not Output.isEmpty():
            self.output_trlabel.setText(Output)
            
    def exectraslation(self):
        infile = self.input_trlabel.text()
        outfile = self.output_trlabel.text()
        tr_x = self.tx.text()
        tr_y = self.ty.text()
        sTOsTxy(infile,outfile,tr_x,tr_y)
        
    def setInput_rot(self):
        Input = QFileDialog.getOpenFileName()
        if not Input.isEmpty():
            self.input_rotlabel.setText(Input)
    
    def setOutput_rot(self):
        Output = QFileDialog.getSaveFileName()
        if not Output.isEmpty():
            self.output_rotlabel.setText(Output)
            
    def execrotation(self):
        infile = self.input_rotlabel.text()
        outfile = self.output_rotlabel.text()
        ang = self.angle.text()
        sTOsRota(infile,outfile,ang)
        
    def setInput_rep(self):
        Input = QFileDialog.getOpenFileName()
        if not Input.isEmpty():
            self.input_replabel.setText(Input)
    
    def setOutput_rep(self):
        Output = QFileDialog.getSaveFileName()
        if not Output.isEmpty():
            self.output_replabel.setText(Output)
            
    def setInput_gcp1(self):
        Input = QFileDialog.getOpenFileName()
        if not Input.isEmpty():
            self.gcp1label.setText(Input)
    
    def setInput_gcp2(self):
        Input = QFileDialog.getOpenFileName()
        if not Input.isEmpty():
            self.gcp2label.setText(Input)
    
    def execreprojection(self):
        infile = self.input_replabel.text()
        outfile = self.output_replabel.text()
        locale = self.gcp1label.text()
        globale = self.gcp2label.text()
        if self.affine.isChecked():
            sTOsAffine(infile,outfile,locale,globale)
        if self.conforme.isChecked():
            sTOsConforme(infile,outfile,locale,globale)