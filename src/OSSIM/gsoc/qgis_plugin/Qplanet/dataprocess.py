#!/usr/bin/env python
import sys

from PyQt4.QtCore import *
from PyQt4.QtGui import *

from data_ui import Ui_Form
import os


from qgis.core import *
from qgis.gui import *

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

class datap(QWidget, Ui_Form):
    def __init__(self,iface):
        self.iface = iface
        self.canvas = iface.mapCanvas()
        QWidget.__init__(self)
        self.setupUi(self)
        self.Tile.setText("0")

        self.Rfilechange = ''
        self.Vfilechange = ''
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

        self.connect(self.ExportVect, SIGNAL("triggered()"), self.printsomething)

        #self.connect(self.update, SIGNAL("clicked()"),self.aggiorna)
        self.connect(self.update, SIGNAL("clicked()"),self.addlayer)
        self.connect(self.InputVector, SIGNAL("currentIndexChanged(int)"), self.selectVfile)
        self.connect(self.InputRaster, SIGNAL("currentIndexChanged(int)"), self.selectRfile)
        self.connect(self.ExportElev, SIGNAL("clicked()"), self.setSaveRaster)
        self.connect(self.ExportVect, SIGNAL("clicked()"), self.setSaveVector)


    def printsomething(self):
        QMessageBox.information(self.iface.mainWindow(),"About",str('hiiiiiii'))

    def addlayer(self):
        global Rlayer
        global Vlayer
        Rlayer = {}
        Vlayer = {}
        nLayers=self.canvas.layerCount()
        self.InputRaster.clear()
        self.InputVector.clear()
        for l in range(nLayers):
            layer = self.canvas.layer(l)
            #QMessageBox.information(self.iface.mainWindow(),"About",str(l))
            if layer and layer.type() == QgsMapLayer.RasterLayer:
                Rlayer[unicode(layer.name())] = unicode(layer.source())
                self.InputRaster.addItem(unicode(layer.name()))
            if layer and layer.type() == QgsMapLayer.VectorLayer:
                Vlayer[unicode(layer.name())] = unicode(layer.source())
                fieldss = layer.pendingFields()
                fields = []
                for i in fieldss:
                    fields.append(fieldss[i].name())
                Vlayer[unicode(layer.name()+'-fields')] = fields
                self.InputVector.addItem(unicode(layer.name()))
                #QMessageBox.information(self.iface.mainWindow(),"About",str(Vlayer))


    def selectVfile(self, index):
        if not self.InputVector.count(): return
        global Vfileselected
        Vfilename = self.InputVector.itemText(index)
        Vfileselected = Vlayer[str(Vfilename)]
        self.AttributeList.clear()
        self.HeightColumn.clear()
        for i in Vlayer[unicode(unicode(Vfilename)+'-fields')]:
             self.AttributeList.addItem(unicode(i))
             self.HeightColumn.addItem(unicode(i))
        return Vfileselected
        
    def selectRfile(self, index):
        if not self.InputRaster.count(): return 
        global Rfileselected
        Rfilename = self.InputRaster.itemText(index)
        Rfileselected = Rlayer[str(Rfilename)]
        return Rfileselected

    def setSaveRaster(self):
        SaveRaster = QFileDialog.getSaveFileName()
        if not SaveRaster.isEmpty():
            self.saveFileNameLabel.setText(SaveRaster)

    def setSaveVector(self):
        SaveVector = QFileDialog.getSaveFileName()
        if not SaveVector.isEmpty():
            self.saveFileNameLabel.setText(SaveVector)


    def setLoadRaster(self):
        InputRaster = QFileDialog.getOpenFileName()
        if not InputRaster.isEmpty():
            self.LoadRasterLabel.setText(InputRaster)
        
#    def setRFile(self, Rfilechange):
#        self.Rfilechange = Rfilechange
#        
#    def setVFile(self, Vfilechange):
#        self.Vfilechange = Vfilechange
#
#    def aggiorna(self):
#        self.InputRaster.clear()
#        self.InputVector.clear()
#        self.inputR.setText(str(self.Rfilechange))
#        self.InputRaster.addItem(self.Rfilechange)
#        self.inputV.setText(str(self.Vfilechange))
#        self.InputVector.addItem(self.Vfilechange)
         
        
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



    def setKmlInput(self):
        tessellate = 0
        extrude = 0
        if self.Tessellate.isChecked():
            tessellate = 1
        if self.Extrude.isChecked():
            extrude = 1
        ExportVector = QFileDialog.getSaveFileName()
        QMessageBox.information(self.iface.mainWindow(),"About",str(ExportVector))
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
