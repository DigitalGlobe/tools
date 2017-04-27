#!/usr/bin/env python
import sys
import platform
from PyQt4.QtCore import *
from PyQt4.QtGui import *
from data_ui import Ui_Form
import os
from grass.script.core import *
from g2k import GrassToKml
import zipfile
from xml.dom import minidom
import time
import osgeo.gdal as gdal
from configure import parseOutputconf
from psinit import RasterList, VectorList, getEnv

iconpath = ""
iconspath = ""
extrudetype = ""
AltitudeMode = ""
HeightColumn = ""
AttributeList = ""
VectorLineColorName = ""
VectorLabelColorName = ""
VectorPolygonColorName = ""
BkgOvlColorName = ""

apppath = os.path.abspath(os.path.dirname(sys.argv[0]))
configfile = '%s/conf/conf.xml' % (apppath)
imagepath = '%s/icons/' % (apppath)

class Data(QWidget, Ui_Form):
    def __init__(self):
        QWidget.__init__(self)
        self.rasterpath = os.path.join(getEnv()['GISDBASE'],getEnv()['LOCATION_NAME'],getEnv()['MAPSET'],'cellhd/')
        self.vectorpath = os.path.join(getEnv()['GISDBASE'],getEnv()['LOCATION_NAME'],getEnv()['MAPSET'],'vector/')
        self.setupUi(self)
        self.lon = 0
        self.lat = 0
        self.raster = RasterList()
        self.vector = VectorList()
        self.InputRaster.addItems(self.raster)
        self.InputVector.addItems(self.vector)
        rast = len(self.raster)
        numrow = rast
        self.data.setColumnCount(1)
        self.data.setRowCount(numrow)
        self.data.setEditTriggers(QTableWidget.NoEditTriggers)
        for i in range(rast):
            item = QTableWidgetItem(self.raster[i])
            item.setTextAlignment(Qt.AlignCenter)
            item.setCheckState(Qt.Unchecked)
            self.data.setItem(i, 0, item)
        self.Tile.setText("0")
        self.connect(self.InputVector, SIGNAL("currentIndexChanged(int)"),
                     self.GetAttribute)
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
        self.connect(self.InputRaster, SIGNAL("currentIndexChanged(int)"),
                     self.GetTile)
        self.connect(self.ExportElev, SIGNAL("clicked()"), 
                     self.ExportElevation)
        self.connect(self.ExportVect, SIGNAL("clicked()"), 
                     self.setKmlInput)
        self.connect(self.kmlbkgcolor, SIGNAL("clicked()"), 
                     self.setkmlbkgcolor)
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
        self.connect(self.HeightColumn, SIGNAL("currentIndexChanged(int)"),
                     self.GetHeightColumn)
        self.connect(self.AttributeList, SIGNAL("currentIndexChanged(int)"),
                     self.GetAttributeList)
        self.connect(self.textlegendcolor, SIGNAL("currentIndexChanged(int)"),
                     self.GetlegendColor)
        self.connect(self.TrueColor, SIGNAL("currentIndexChanged(int)"),
                     self.GetTrueColor)
        self.connect(self.AlphaBkg, SIGNAL("currentIndexChanged(int)"),
                     self.GetAlphaBkg)
        self.connect(self.PngRead, SIGNAL("currentIndexChanged(int)"),
                     self.GetPngRead)
        self.connect(self.PngKomp, SIGNAL("currentIndexChanged(int)"),
                     self.GetPngKomp)
        self.connect(self.exportoverlay, SIGNAL("clicked()"), 
                     self.makeoverview)
        self.connect(self.placemarks, SIGNAL("currentIndexChanged(int)"),
                     self.GetIcons)
        self.connect(self.refresh, SIGNAL("clicked()"), 
                     self.aggiorna)
        self.connect(self.overlaydir, SIGNAL("clicked()"),self.filemanager)
        self.connect(self.elevdir, SIGNAL("clicked()"),self.filemanager)



    def filemanagerdir(self,dir):
        filemanager = parseOutputconf()['filemanager']
        output = subprocess.Popen([filemanager ,str(dir)], stdout=subprocess.PIPE).communicate()[0]

    def setLonValue(self, lon):
        self.lon = lon
    

    def setLatValue(self, lat):
        self.lat = lat
    

    def aggiorna(self):
        newlon = str(self.lon)
        newlat = str(self.lat)
        self.longitude.setText(newlon)
        self.latitude.setText(newlat)
    

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
    

    def GetAttribute(self,index):
        global inputvector
        attr = []
        infile = self.InputVector.itemText(index)
        inputvector = os.path.join(self.vectorpath,infile,'head')
        attr = getFields(inputvector)
        self.AttributeList.clear()
        self.AttributeList.addItems(attr)
        self.HeightColumn.clear()
        self.HeightColumn.addItems(attr)
    

    def GetlegendFile(self, index):
        global legendfile
        legendfile = self.inputrasterlegend.itemText(index)
        return legendfile
    

    def GetlegendColor(self, index):
        global legendcolor
        legendcolor = self.textlegendcolor.itemText(index)
        return legendcolor
    

    def GetTrueColor(self, index):
        global TrueColorVal
        TrueColorVal = self.TrueColor.itemText(index)
        return TrueColorVal
    

    def GetAlphaBkg(self, index):
        global AlphaBkgVal
        AlphaBkgVal = self.AlphaBkg.itemText(index)
        return AlphaBkgVal
    

    def GetPngRead(self, index):
        global PngReadVal
        PngReadVal = self.PngRead.itemText(index)
        return PngReadVal
    

    def GetPngKomp(self, index):
        global PngKompVal
        PngKompVal = self.PngKomp.itemText(index)
        return PngKompVal
    

    def GetAttributes(self,index):
        infile = self.InputVector.itemText(index)
        infile = os.path.join(self.vectorpath,infile,'head')
        #print infile
        in_ds = ogr.Open( infile, update = 1 )
        in_layer = in_ds.GetLayer(1)
        attr = in_layer.GetLayerDefn().GetFieldCount()
        print attr
        attributi = []
        for i in range(attr):
            attname = in_layer.GetLayerDefn().GetFieldDefn(i).GetName()
            attributi.append(attname)
        print attributi
    

    def GetTile(self,index):
        global tiling
        global inputfile
        inputfile = self.InputRaster.itemText(index)
        tiling = getTile(inputfile)
        print tiling
    

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
            print 'extrudetype :', extrudetype
            print 'is a point'
            print 'inputvector :' , inputvector
            print 'ExportVector :', ExportVector
            print 'num layer :' , 1
            print 'name'
            print 'status' , 0
            print 'some desription here'
            print 'VectorLabelColorName' , VectorLabelColorName
            print 'labelscale'
            print 'iconpath' , iconpath
            print 'tessellate' , tessellate
            print 'extrude' , extrude
            print 'line width :' , self.LineWidth.value()
            print 'VectorLineColorName' , VectorLineColorName
            print 'colormode' , colormode
            print 'VectorPolygonColorName' , VectorPolygonColorName
            print 'AttributeList' , AttributeList
            print 0, 0, 0, 0, 0
            print 'AltitudeMode' , AltitudeMode
            print 'Offset' , self.Offset.value()
            print 'Height' , HeightColumn
            print 'label alpha' , self.LabelAlpha.value()
            print 'line alpha' , self.LineAlpha.value()
            print 'poly alpha' , self.PolygonAlpha.value()
            GrassToKml(extrudetype,'point', inputvector, ExportVector, 1, 
                       'name', 0, 'some desription here', VectorLabelColorName, 
                       'labelscale', iconpath, tessellate, extrude, 
                       self.LineWidth.value(), VectorLineColorName, colormode, 
                       VectorPolygonColorName, AttributeList, 0, 0, 0, 0, 0, 
                       AltitudeMode, self.Offset.value(), HeightColumn, 
                       self.LabelAlpha.value(), self.LineAlpha.value(), 
                       self.PolygonAlpha.value())

        if self.Line.isChecked():
            print extrudetype,'line', inputvector, ExportVector, 1, \
                       'name', 0, 'some desription here', VectorLabelColorName, \
                       'labelscale', iconpath, tessellate, extrude, \
                       self.LineWidth.value(), VectorLineColorName, colormode, \
                       VectorPolygonColorName, AttributeList, 0, 0, 0, 0, 0, \
                       AltitudeMode, self.Offset.value(), HeightColumn, \
                       self.LabelAlpha.value(), self.LineAlpha.value(), \
                       self.PolygonAlpha.value()
            GrassToKml(extrudetype,'line', inputvector, ExportVector, 1, 
                       'name', 0, 'some desription here', VectorLabelColorName, 
                       'labelscale', iconpath, tessellate, extrude, 
                       self.LineWidth.value(), VectorLineColorName, colormode, 
                       VectorPolygonColorName, AttributeList, 0, 0, 0, 0, 0, 
                       AltitudeMode, self.Offset.value(), HeightColumn, 
                       self.LabelAlpha.value(), self.LineAlpha.value(), 
                       self.PolygonAlpha.value())
        if self.Polygon.isChecked():
            print extrudetype,'polygon', inputvector, ExportVector, 2, \
                  'name', 0, 'some desription here', VectorLabelColorName, \
                  'labelscale', iconpath, tessellate, extrude, \
                  self.LineWidth.value(), VectorLineColorName, colormode, \
                  VectorPolygonColorName, AttributeList, 0, 0, 0, 0, 0, \
                  AltitudeMode, self.Offset.value(), HeightColumn, \
                  self.LabelAlpha.value(), self.LineAlpha.value(), \
                  self.PolygonAlpha.value()
            GrassToKml(extrudetype,'polygon', inputvector, ExportVector, 1, 
                       'name', 0, 'some desription here', VectorLabelColorName, 
                       'labelscale', iconpath, tessellate, extrude, 
                       self.LineWidth.value(), VectorLineColorName, colormode, 
                       VectorPolygonColorName, AttributeList, 0, 0, 0, 0, 0, 
                       AltitudeMode, self.Offset.value(), HeightColumn, 
                       self.LabelAlpha.value(), self.LineAlpha.value(), 
                       self.PolygonAlpha.value())
    

    def ExportElevation(self):
        ExportElev = QFileDialog.getSaveFileName()
        outname = ExportElev.split('/')[-1]
        if self.ManualTiling.isChecked():
            tile = self.Tile.text()
            start_command("r.planet.py", flags = '1' , elevation = inputfile , 
                          tiling = tile , output_directory=ExportElev , name=outname)
        if self.Autotiling.isChecked():
            start_command("r.planet.py" , elevation=inputfile , 
                          output_directory=ExportElev , name=outname)
    

    def setkmlbkgcolor(self):
        global kmlbkgcolor
        global kmlbkgcolorName
        kmlbkgcolor = QColorDialog.getColor()
        kmlbkgcolorName = kmlbkgcolor.name()
        if kmlbkgcolor.isValid():
            self.bkgcolor.setStyleSheet("QWidget { background-color: %s }"
                                          % kmlbkgcolor.name() )
            return kmlbkgcolor.name()
        return kmlbkgcolorName
    

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
    

    def GetIcons(self,index):
        global iconspath
        icons = self.placemarks.itemText(index)
        iconspath = imagepath+str(icons)+str('.png')
        return iconspath
    

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
        platforms = platform.system()
        return platforms
    
        
    def filemanager(self):
        filemanager = self.parseOutputconf()['filemanager']
        vrtdir = self.parseOutputconf()['vrtdir']
        output = subprocess.Popen([filemanager ,str(vrtdir)], stdout=subprocess.PIPE).communicate()[0]
    

    def makelegend(self, inputmap, output, lcolor, bkgcolor, flag):
        if self.flag_z.isChecked():
            start_command("d.png.legend", input = inputmap, output_directory = output, 
            color = lcolor, TRUECOLOR = TrueColorVal, TRANSPARENT = AlphaBkgVal, 
            PNG_READ = PngReadVal, BACKGROUNDCOLOR = bkgcolor, PNG_COMPRESSION = PngKompVal, 
            height = self.height.text(), width = self.width.text(), lines = self.AdvLines.text(), 
            thin = self.AdvThin.text(), labelnum = self.AdvLabelNum.text(), flags = flag)
            #print 'with image'
        else :
            start_command("d.png.legend", input = inputmap, output_directory = output, 
            color = lcolor, TRUECOLOR = TrueColorVal, TRANSPARENT = AlphaBkgVal, 
            PNG_READ = PngReadVal, BACKGROUNDCOLOR = bkgcolor, PNG_COMPRESSION = PngKompVal, 
            height = self.height.text(), width = self.width.text(), lines = self.AdvLines.text(), 
            thin = self.AdvThin.text(), labelnum = self.AdvLabelNum.text())
            #print 'without image'
    

    def addoverlay(self, name, outfile):
        kml_overlay = """<ScreenOverlay>
<name>%s</name>
<visibility>0</visibility>
<Icon>
<href>%s</href>
</Icon>
<overlayXY x="0" y="1" xunits="fraction" yunits="fraction"/>
<screenXY x="0" y="1" xunits="fraction" yunits="fraction"/>
<rotationXY x="0" y="0" xunits="fraction" yunits="fraction"/>
<size x="0.5" y="0" xunits="fraction" yunits="fraction"/>
</ScreenOverlay>""" % (name, outfile)
        return kml_overlay
    
    #def exportimage(self,infile,outfile):
    #    gdal.GetDriverByName('GTiff').CreateCopy(outfile,gdal.Open(infile))

        
    def addplacemark(self, name, icon, names, outimage, lon, lat, alt):
        place = """<name>%s</name>
        <open>1</open>
        <Style id="icon">
            <IconStyle>
                <Icon>
                    <href>%s</href>
                </Icon>
            </IconStyle>
        </Style>
        <Placemark>
            <name>%s</name>
            <visibility>0</visibility>
            <description><![CDATA[<font size="+1"> <a href="http://localhost"><b>Primo link</b></a> </font><br><br> <img height="375" src="%s" width="500"></td><td valign="top"><table width="180" cellspacing="0" cellpadding="0"><tr><td><h2><font size="5"><b>TITOLO</b></font></h2></td></tr><tr><td><p><a href="http://localhost">secondo link</a><br><a href="http://localhost">terzo link</a><br><a href="http://localhost">quarto link</a><br><a href="http://localhost">quinto link </a></p><p></td></tr></table>]]></description>     
        <styleUrl>#icon</styleUrl>
        <Point>
            <altitudeMode>relativeToGround</altitudeMode>
            <coordinates>%s,%s,%s</coordinates>
        </Point>     
    </Placemark>""" % (name, icon, names, outimage, lon, lat, alt) 
        #print place
        return place
    

    def makeoverview(self):
        name = self.legendname.text()
        color = legendcolor
        outputfile = QFileDialog.getSaveFileName()
        outputpath = os.path.dirname(str(outputfile))
        raster = self.raster
        rast = len(raster)
        kmlfile = []
        filelist = []
        if AlphaBkgVal == 'FALSE':
            bkgcolor = kmlbkgcolorName
            bkgcolor = str(bkgcolor)
            bkgcolor = bkgcolor.replace('#','')
        else :
            bkgcolor = '000000'
        flags = []
        if self.flag_v.isChecked():
            flags.append('-v')
        if self.flag_c.isChecked():
            flags.append('-c')
        if self.flag_n.isChecked():
            flags.append('-n')
        if self.flag_s.isChecked():
            flags.append('-s')
        if self.flag_f.isChecked():
            flags.append('-f')
        if self.flag_z.isChecked():
            flags.append('-z')
        flags = str(flags)
        flags = flags.replace("['",'')
        flags = flags.replace("']",'')
        flags = flags.replace("', '","")
        flag = flags.replace("-","")        
        for i in range(rast):
            cell = self.data.item(i, 0)
            if cell.checkState() == 2:
                input = str(cell.text())
                self.makelegend(input, outputpath, color, bkgcolor, flag)
                outfile = outputpath+'/'+input+'.png'
                outimage = outputpath+'/'+'image_'+input+'.png'
                ovl = self.addoverlay(input, outfile)
                if self.placemark.isChecked():
                    longitude = self.longitude.text()
                    latitude = self.latitude.text()
                    placemark = self.addplacemark('Placemarks', iconspath, input, outimage, longitude, latitude, 0)
                    #infile = os.path.join(self.rasterpath,input)
                    #self.exportimage(infile,outimage)
                kmlfile.append(ovl)
                kmlfile.append(placemark)
                filelist.append(outfile)
        kmlfile = str(kmlfile)
        kmlfile = kmlfile.replace("['","")
        kmlfile = kmlfile.replace("']","")
        kmlfile = kmlfile.replace("\\n","")
        overview = kmlfile.replace("', '","")
        overview = kmlfile.replace("', u'","")
        overview = kmlfile.replace("</Placemark>',","</Placemark>")
        openoverview = """<?xml version="1.0" encoding="UTF-8"?>
		<kml xmlns="http://www.opengis.net/kml/2.2">
		  <Document>
		<Folder>
		  <name>%s</name>
		  <visibility>0</visibility>""" % (name)
        closeoverview = """</Folder>
		</Document>
		</kml>""" 
        kmlout = outputfile 
        kml = openoverview+overview+closeoverview
        kml = kml.replace("</Placemark>',","</Placemark>")
        kml = kml.replace("'<ScreenOverlay>","<ScreenOverlay>")
        kml = kml.replace("', u'","")
        print kml
        f=open(kmlout, 'w')	
        f.write(kml)	
        #f.write(openoverview)
        #f.write(overview)
        #f.write(closeoverview)
        f.close()

        filelist.append(str(kmlout))
        self.filemanagerdir(outputpath)
        #print 'openoverview :', openoverview
        #print 'overview :', overview
        #print 'closeoverview :', closeoverview
	

    def parseOutputconf(self):
        xmldoc = minidom.parse(configfile)
        outconf = {}
        planetsashaconf = xmldoc.firstChild
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

        return  outconf
    
