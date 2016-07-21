#!/usr/bin/env python
import sys

from PyQt4.QtCore import *
from PyQt4.QtGui import *

from kmlsave_ui import Ui_Form
import os
from __builtin__ import hex
import zipfile
import subprocess

from configure import parseOutputconf
#from xml.dom import minidom 
import codecs

iconpath = ""
extrudetype = ""
AltitudeMode = ""
VectorLineColorName = ""
VectorLabelColorName = ""
VectorPolygonColorName = ""

apppath = os.path.abspath(os.path.dirname(sys.argv[0]))
imagepath = '%s/icons/' % (apppath)
filem = '%s/conf/filem.conf' % (apppath)
configfile = '%s/conf/conf.xml' % (apppath)


class KmlView(QWidget, Ui_Form):
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
        self.connect(self.linecolor, SIGNAL("clicked()"), self.setVectorLineColor)
        self.connect(self.labelcolor, SIGNAL("clicked()"), self.setVectorLabelColor)
        self.connect(self.polygoncolor, SIGNAL("clicked()"), self.setVectorPolygonColor)
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
        self.connect(self.save, SIGNAL("clicked()"), self.savekml)
        self.connect(self.SelectIcon, SIGNAL("currentIndexChanged(int)"),
                     self.GetIcon)
        self.connect(self.update, SIGNAL("clicked()"),self.aggiorna)
        self.connect(self.opendir, SIGNAL("clicked()"),self.filemanager)
        self.connect(self.addlink, SIGNAL("clicked()"),self.addlinkf)
        self.connect(self.clean, SIGNAL("clicked()"),self.cleantext)
        #icon = self.SelectIcon.itemText(index)
        self.iconpath = imagepath+str(self.SelectIcon.itemText(1))+str('.png')
        print self.iconpath
        
        
        
    def filemanager(self):
        filemanager = parseOutputconf()['filemanager']
        kmldir = parseOutputconf()['kmldir']
        output = subprocess.Popen([filemanager ,str(kmldir)], stdout=subprocess.PIPE).communicate()[0]


    def compress_kml(self,outfile,icon):
        directory = os.path.dirname(str(outfile))
        iconname = icon.split('/')[-1]
        icontosave = directory+'/'+iconname
        string = 'cp %s %s' % (icon, icontosave)
        cp = os.system(string)
        outfilename = outfile.split('/')
        outfilename = outfilename[-1]
        kmz = outfile.split('.')[0]+'.kmz'
        zip = zipfile.ZipFile(str(kmz), 'w')
        zip.write(str(outfile))
        zip.write(str(icontosave))
        zip.close


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

    def aggiorna(self):
        newlon = str(self.lon)
        newlat = str(self.lat)
        self.longitude.setText(newlon)
        self.latitude.setText(newlat)
        self.Roll.setText(str(self.Rollchange))
        self.Pitch.setText(str(self.Pitchchange))
        self.Head.setText(str(self.Headchange))
        self.Zoom.setText(str(self.Zoomchange))
        self.Range.setText(str(self.Rangechange))


    def GetIcon(self,index):
        #global iconpath
        icon = self.SelectIcon.itemText(index)
        self.iconpath = imagepath+str(icon)+str('.png')
        return self.iconpath


    def setVectorLineColor(self):
        global VectorLineColor
        global VectorLineColorName
        VectorLineColor = QColorDialog.getColor()
        VectorLineColorName = VectorLineColor.name()
        if VectorLineColor.isValid():
            self.linecolorlabel.setStyleSheet("QWidget { background-color: %s }" % VectorLineColor.name() )
            return VectorLineColor.name()
        return VectorLineColorName

    def setVectorLabelColor(self):
        global VectorLabelColor
        global VectorLabelColorName
        VectorLabelColor = QColorDialog.getColor()
        VectorLabelColorName = VectorLabelColor.name()
        if VectorLabelColor.isValid():
            self.labelcolorlabel.setStyleSheet("QWidget { background-color: %s }" % VectorLabelColor.name() )
            return VectorLabelColor.name()
        return VectorLabelColorName


    def setVectorPolygonColor(self):
        global VectorPolygonColor
        global VectorPolygonColorName
        VectorPolygonColor = QColorDialog.getColor()
        VectorPolygonColorName = VectorPolygonColor.name()
        if VectorPolygonColor.isValid():
            self.polygoncolorlabel.setStyleSheet("QWidget { background-color: %s }" % VectorPolygonColor.name() )
            return VectorPolygonColor.name()
        return VectorPolygonColorName


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


    def textlink(self): 
        link = '<a href="hjkkk"><span style=" text-decoration: underline; color:#0000ff;">hj</span></a>'
        return link
        

    def addlinkf(self):
        link = self.textlink()
        #self.description.setHtml(unicode(link))
        self.description.append(QString(link))
        
    def cleantext(self):
        self.description.setHtml('')

    def savekml(self):
        # vedi di aggiungere zoom,range e view type ... magari link a immagini ???
        # aggiungi un "salva in sqlite" il db deve essere inizializzato nelle preferenze
        LabelAlpha = self.LabelAlpha.value()
        LineAlpha = self.LineAlpha.value()
        PolygonAlpha = self.PolygonAlpha.value()
        labalpha = hex(int(LabelAlpha))
        linalpha = hex(int(LineAlpha))
        polalpha = hex(int(PolygonAlpha))
        labalpha = labalpha.split('x')
        linalpha = linalpha.split('x')
        polalpha = polalpha.split('x')
        labalpha = labalpha[-1]
        linalpha = linalpha[-1]
        polalpha = polalpha[-1]
        if len(labalpha) == 1:
            labalpha = str('0')+labalpha
        if len(linalpha) == 1:
            linalpha = str('0')+linalpha
        if len(polalpha) == 1:
            polalpha = str('0')+polalpha
        colorlabel = VectorLabelColorName
        colorline = VectorLineColorName
        colorpolygon = VectorPolygonColorName
        colorlabel = colorlabel[1:]
        colorlabelR = colorlabel[0:2]
        colorlabelG = colorlabel[2:4]
        colorlabelB = colorlabel[4:6]
        colorlabel = colorlabelB+colorlabelG+colorlabelR
        colorlabel = str(labalpha)+colorlabel
        colorline = colorline[1:]
        colorlineR = colorline[0:2]
        colorlineG = colorline[2:4]
        colorlineB = colorline[4:6]
        colorline = colorlineB+colorlineG+colorlineR
        colorline = str(linalpha)+colorline
        colorpolygon = colorpolygon[1:]
        colorpolygonR = colorpolygon[0:2]
        colorpolygonG = colorpolygon[2:4]
        colorpolygonB = colorpolygon[4:6]
        colorpolygon = colorpolygonB+colorpolygonG+colorpolygonR
        colorpolygon = str(polalpha)+colorpolygon
        tessellate = 0
        extrude = 0
        kmldirectory = parseOutputconf()['kmldir']
        if self.Tessellate.isChecked():
            tessellate = 1
        if self.Extrude.isChecked():
            extrude = 1
        kml = 	"""<?xml version="1.0" encoding="UTF-8"?>
<kml xmlns="http://www.opengis.net/kml/2.2">
<Document>
<name>%s</name>
<open>0</open>
<description>some desription here</description><Style id="Mystyle">
<LabelStyle>
<color>%s</color>
<colorMode>normal</colorMode>
<scale>1</scale>
</LabelStyle>
<IconStyle>
<Icon>
<href>%s</href>
</Icon>
</IconStyle>
<LineStyle>
<color>%s</color>
<colorMode>normal</colorMode>
<tessellate>%s</tessellate>
<width>%s</width>
</LineStyle>
<PolyStyle>
<color>%s</color>
<colorMode>normal</colorMode>
</PolyStyle>
</Style><Placemark>
<name>%s</name>
<description><![CDATA[<div name="" id="id_main" align="center" dir="ltr" width="680"><table width="680" height="375"><tr>%s<td></td>]]></description>
<visibility>0</visibility>
<styleUrl>#Mystyle</styleUrl><LookAt> 
<longitude>%s</longitude> 
<latitude>%s</latitude> 
<altitude>%s</altitude> 
<range>%s</range> 
<tilt>%s</tilt> 
<heading>%s</heading> 
<altitudeMode>relativeToGround</altitudeMode> 
</LookAt><Point>
<extrude>%s</extrude>
<altitudeMode>relativeToGround</altitudeMode>
<coordinates>%s,%s,%s</coordinates>
</Point></Placemark></Document></kml>""" % (self.kmlname.text(), colorlabel, self.iconpath, colorline, tessellate, self.LineWidth.value(), colorpolygon, self.kmllabel.text(), self.description.toHtml(), self.lon, self.lat, self.Zoomchange, self.Rangechange, self.Pitchchange, self.Headchange, extrude, self.lon, self.lat, self.Offset.value())
        kmldir = str(kmldirectory)+'/'
        kmltosave = kmldir+self.kmlname.text()+'.kml'
        f=codecs.open(kmltosave, 'w', 'UTF8' )
        f.write(kml)
        f.close()
        kmz = self.compress_kml(str(kmltosave),str(self.iconpath))
