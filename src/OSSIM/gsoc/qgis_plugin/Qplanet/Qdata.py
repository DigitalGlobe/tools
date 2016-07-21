#!/usr/bin/env python
import sys
import platform
from PyQt4.QtCore import *
from PyQt4.QtGui import *

from data_ui import Ui_Form
import os

iconpath = ""
extrudetype = ""
AltitudeMode = ""
HeightColumn = ""
AttributeList = ""
VectorLineColorName = ""
VectorLabelColorName = ""
VectorPolygonColorName = ""



apppath = os.path.dirname(__file__)

kmlfile = '%s/gps/nmea.kml' % (apppath)
imagepath = '%s/icons/' % (apppath)
vrtpath = '%s/vrt/' % (apppath)


class DataWork(QWidget, Ui_Form):
    def __init__(self):
        QWidget.__init__(self)
        self.setupUi(self)
        self.InputVector.addItem(unicode('maybe'))
        self.Tile.setText("0")
        
        self.lon = 0
        self.lat = 0
        
        self.connect(self.LabelAlpha, SIGNAL("valueChanged(double)"), 
                     self.SetLabelAlpha)
        self.connect(self.LineAlpha, SIGNAL("valueChanged(double)"), 
                     self.SetLineAlpha)
        self.connect(self.PolygonAlpha, SIGNAL("valueChanged(double)"), 
                     self.SetPolygonAlpha)
        self.connect(self.LineWidth, SIGNAL("valueChanged(double)"), 
                     self.SetLineWidth)
        self.connect(self.Offset, SIGNAL("valueChanged(double)"), 
                     self.SetOffset)

        self.connect(self.VectorLabelColor, SIGNAL("clicked()"), 
                     self.setVectorLabelColor)
        self.connect(self.VectorLineColor, SIGNAL("clicked()"), 
                     self.setVectorLineColor)
        self.connect(self.VectorPolygonColor, SIGNAL("clicked()"), 
                     self.setVectorPolygonColor)
        self.connect(self.SelectIcon, SIGNAL("currentIndexChanged(int)"),
                     self.GetIcon)
        self.connect(self.ExtrudeType, SIGNAL("currentIndexChanged(int)"),
                     self.GetExtrudeType)
        self.connect(self.AltitudeMode, SIGNAL("currentIndexChanged(int)"),
                     self.GetAltitudeMode)
        self.connect(self.colorModeEnum, SIGNAL("currentIndexChanged(int)"),
                     self.GetColorMode)

        self.connect(self.ExportVect, SIGNAL("triggered()"), self.printlayers)
        self.connect(self.update, SIGNAL("clicked()"),self.aggiorna)

    
    def setLonValue(self, lon):
        self.lon = lon

    def setLatValue(self, lat):
        self.lat = lat
        
    def aggiorna(self):
        newlon = str(self.lon)
        newlat = str(self.lat)
        self.longitude.setText(newlon)
        self.latitude.setText(newlat)
        
        
    
    def printlayers(self):
        nLayers=self.canvas.layerCount()
        self.InputVector.clear()
        for l in range(nLayers):
            layer = self.canvas.layer(l)
            if layer and layer.type() == QgsMapLayer.RasterLayer:
                self.InputVector.addItem(unicode(layer.source()))

    def SetLabelAlpha(self, ap):
        self.labelalpha = int(ap)
        self.LabelAlpha.setRange(0, 255)
        self.LabelAlpha.setValue(self.labelalpha)

    def SetLineAlpha(self, ap):
        self.linealpha = int(ap)
        self.LineAlpha.setRange(0, 255)
        self.LineAlpha.setValue(self.linealpha)

    def SetPolygonAlpha(self, ap):
        self.polygonalpha = int(ap)
        self.PolygonAlpha.setRange(0, 255)
        self.PolygonAlpha.setValue(self.polygonalpha)

    def SetLineWidth(self, wd):
        self.linewidth = int(wd)
        self.LineWidth.setRange(0, 99)
        self.LineWidth.setValue(self.linewidth)

    def SetOffset(self, ofst):
        self.offset = float(ofst)
        self.Offset.setRange(-10000, 1000000)
        self.Offset.setValue(self.offset)



    def setSaveRaster(self):
        SaveRaster = QFileDialog.getSaveFileName()
        if not SaveRaster.isEmpty():
            self.saveFileNameLabel.setText(SaveRaster)

    def setLoadRaster(self):
        InputRaster = QFileDialog.getOpenFileName()
        if not InputRaster.isEmpty():
            self.LoadRasterLabel.setText(InputRaster)

    def setKmlInput(self):
        tessellate = 0
        extrude = 0
        if self.Tessellate.isChecked():
            tessellate = 1
        if self.Extrude.isChecked():
            extrude = 1
        ExportVector = QFileDialog.getSaveFileName()
        if self.Point.isChecked():
            print extrudetype,'point', inputvector, ExportVector, 1, 'name', 0,\
                  'some desription here', VectorLabelColorName, 'labelscale', \
                  iconpath, tessellate, extrude, self.LineWidth.value(), \
                  VectorLineColorName, colormode, VectorPolygonColorName, \
                  AttributeList, 0, 0, 0, 0, 0, AltitudeMode, \
                  self.Offset.value(), HeightColumn, \
                  self.LabelAlpha.value(), self.LineAlpha.value(), \
                  self.PolygonAlpha.value()


        if self.Line.isChecked():
            print extrudetype,'line', inputvector, ExportVector, 1, \
                       'name', 0, 'some desription here', VectorLabelColorName, \
                       'labelscale', iconpath, tessellate, extrude, \
                       self.LineWidth.value(), VectorLineColorName, colormode, \
                       VectorPolygonColorName, AttributeList, 0, 0, 0, 0, 0, \
                       AltitudeMode, self.Offset.value(), HeightColumn, \
                       self.LabelAlpha.value(), self.LineAlpha.value(), \
                       self.PolygonAlpha.value()

        if self.Polygon.isChecked():
            print extrudetype,'polygon', inputvector, ExportVector, 2, \
                  'name', 0, 'some desription here', VectorLabelColorName, \
                  'labelscale', iconpath, tessellate, extrude, \
                  self.LineWidth.value(), VectorLineColorName, colormode, \
                  VectorPolygonColorName, AttributeList, 0, 0, 0, 0, 0, \
                  AltitudeMode, self.Offset.value(), HeightColumn, \
                  self.LabelAlpha.value(), self.LineAlpha.value(), \
                  self.PolygonAlpha.value()


    def setVectorLineColor(self):
        global VectorLineColor
        global VectorLineColorName
        VectorLineColor = QColorDialog.getColor()
        VectorLineColorName = VectorLineColor.name()
        if VectorLineColor.isValid():
            self.LineColor.setStyleSheet("QWidget { background-color: %s }"
                                           % VectorLineColor.name() )
            return VectorLineColor.name()
        return VectorLineColorName

    def setVectorLabelColor(self):
        global VectorLabelColor
        global VectorLabelColorName
        VectorLabelColor = QColorDialog.getColor()
        VectorLabelColorName = VectorLabelColor.name()
        if VectorLabelColor.isValid():
            self.LabelColor.setStyleSheet("QWidget { background-color: %s }"
                                            % VectorLabelColor.name() )
            return VectorLabelColor.name()
        return VectorLabelColorName

    def setVectorPolygonColor(self):
        global VectorPolygonColor
        global VectorPolygonColorName
        VectorPolygonColor = QColorDialog.getColor()
        VectorPolygonColorName = VectorPolygonColor.name()
        if VectorPolygonColor.isValid():
            self.PolyColor.setStyleSheet("QWidget { background-color: %s }"
                                           % VectorPolygonColor.name() )
            return VectorPolygonColor.name()
        return VectorPolygonColorName

    def GetIcon(self,index):
        global iconpath
        icon = self.SelectIcon.itemText(index)
        iconpath = imagepath+str(icon)+str('.png')
        return iconpath

    def GetExtrudeType(self, index):
        global extrudetype
        extrudetype = self.ExtrudeType.itemText(index)
        return extrudetype

    def GetAltitudeMode(self, index):
        global AltitudeMode
        AltitudeMode = self.AltitudeMode.itemText(index)
        return AltitudeMode

    def GetColorMode(self, index):
        global colormode
        colormode = self.colorModeEnum.itemText(index)
        return colormode

    def GetHeightColumn(self, index):
        global HeightColumn
        HeightColumn = self.HeightColumn.itemText(index)
        return HeightColumn

    def GetAttributeList(self, index):
        global AttributeList
        AttributeList = self.AttributeList.itemText(index)
        return AttributeList

    def WhichPlatform(self):
        #global platform
        platforms = platform.system()
        return platforms

    def filemanager(self):
        systemplatform = self.WhichPlatform()
        if systemplatform == 'Darwin':
            filemanager = 'open'
        else :
            for dirname in os.environ['PATH'].split(os.pathsep):
                if os.path.exists(os.path.join(dirname, 'thunar')):
                    filemanager = 'thunar'
                if os.path.exists(os.path.join(dirname, 'nautilus')):
                    filemanager = 'nautilus'
                if os.path.exists(os.path.join(dirname, 'konqueror')):
                    filemanager = 'konqueror'
        output = subprocess.Popen([filemanager ,str(vrtpath)], stdout=subprocess.PIPE).communicate()[0]
