#!/usr/bin/env python
try:
    from osgeo import osr
    from osgeo import ogr
except ImportError:
    import osr
    import ogr

import string
import sys
import os
import math
from scipy import linalg
import numpy as np
from PyQt4.QtCore import *
from PyQt4.QtGui import *

from transvector_ui import Ui_Form

from epsg_list import codelist


class VectorOperation(QWidget, Ui_Form):
    def __init__(self):
        QWidget.__init__(self)
        #self.app = app
        self.setupUi(self)
        self.populatesrs()
        #self.connect(self.rpjtxtExit, SIGNAL("clicked()"), self.quit)
        #self.connect(self.helmertExit, SIGNAL("clicked()"), self.quit)
        #self.connect(self.rpjExit, SIGNAL("clicked()"), self.quit)
        #self.connect(self.trExit, SIGNAL("clicked()"), self.quit)
        #self.connect(self.rotExit, SIGNAL("clicked()"), self.quit)
        self.connect(self.rpjtxtInput, SIGNAL("clicked()"), self.txtOpenFileInput)
        self.connect(self.rpjtxtOutput, SIGNAL("clicked()"), self.txtOpenFileOutput)
        self.connect(self.rpjtxtGCP1, SIGNAL("clicked()"), self.txtOpenFileGCP1)
        self.connect(self.rpjtxtGCP2, SIGNAL("clicked()"), self.txtOpenFileGCP2)
        
        #self.connect(self.helmertInput, SIGNAL("clicked()"), self.hOpenFileInput)
        #self.connect(self.helmertOutput, SIGNAL("clicked()"), self.hOpenFileOutput)
        #self.connect(self.helmertGCP1, SIGNAL("clicked()"), self.hOpenFileGCP1)
        #self.connect(self.helmertGCP2, SIGNAL("clicked()"), self.hOpenFileGCP2)
        
        self.connect(self.rpjshpInput, SIGNAL("clicked()"), self.prjOpenFileInput2)
        self.connect(self.rpjshpOutput, SIGNAL("clicked()"), self.prjOpenFileOutput2)
        self.connect(self.rpjGCP1, SIGNAL("clicked()"), self.rpjOpenFileGCP1)
        self.connect(self.rpjGCP2, SIGNAL("clicked()"), self.rpjOpenFileGCP2)
        self.connect(self.trshpInput, SIGNAL("clicked()"), self.trOpenFileInput)
        self.connect(self.trshpOutput, SIGNAL("clicked()"), self.trOpenFileOutput)
        self.connect(self.rotshpInput, SIGNAL("clicked()"), self.rotOpenFileInput)
        self.connect(self.rotshpOutput, SIGNAL("clicked()"), self.rotOpenFileOutput)
        self.connect(self.rpjtxtGo, SIGNAL("clicked()"), self.exectxtprj)
        #self.connect(self.helmertGo, SIGNAL("clicked()"), self.exechelmert)
        self.connect(self.rpjGo, SIGNAL("clicked()"), self.execprj2)
        self.connect(self.trGo, SIGNAL("clicked()"), self.exectr)
        self.connect(self.rotGo, SIGNAL("clicked()"), self.execrot)
        self.connect(self.setsrs, SIGNAL("currentIndexChanged(int)"), self.setdstsrs)
        self.connect(self.openFileNameButton, SIGNAL("clicked()"), self.setOpenFileName)
        self.connect(self.saveFileNameButton, SIGNAL("clicked()"), self.setSaveFileName)
        self.connect(self.convert, SIGNAL("clicked()"), self.saveFunction)
        #self.connect(self.exit, SIGNAL("clicked()"), self.quit)


    def prjOpenFileInput2(self):
        string = ''
        ShapeName = QFileDialog.getOpenFileNames(self, self.tr("Input file"), self.rpjshpInput_file.text(), self.tr("All Files (*);;Shape Files (*.shp)"))
        for i in range(len(ShapeName)):
            string += ShapeName[i]+','
        self.rpjshpInput_file.setText(string[:-1])
    

    def prjOpenFileOutput2(self):
        prj_Output = QFileDialog.getExistingDirectory(self, self.tr("Open Directory"),"/home", QFileDialog.ShowDirsOnly | QFileDialog.DontResolveSymlinks);
        self.rpjshpOutput_file.setText(prj_Output)
    

    def execprj2(self):
        for i in range(len(self.rpjshpInput_file.text().split(','))):
            input = self.rpjshpInput_file.text().split(',')[i]
            outdir = self.rpjshpOutput_file.text()
            output_name = os.path.basename(str(input))
            output = os.path.join(str(outdir),output_name.split('.')[0])
            if self.rpjAffine.isChecked():
                # sTOsAffine(infile,outfile,locale,globale,srs_code)
                print self.sTOsAffine(input,output, unicode(self.rpjGCP1_file.text()), unicode(self.rpjGCP2_file.text()), str(dstsrs))
            else:
                print self.sTOsConforme(input,output, unicode(self.rpjGCP1_file.text()), unicode(self.rpjGCP2_file.text()), str(dstsrs))
    

    def setOpenFileName(self):
        string = ''
        fileName = QFileDialog.getOpenFileNames(self, self.tr("Input file"), self.openFileNameLabel.text(), self.tr("All Files (*);;Text Files (*.txt)"))
        for i in range(len(fileName)):
            string += fileName[i]+','
        self.openFileNameLabel.setText(string[:-1])
    
            
    def setSaveFileName(self):
        fileName = QFileDialog.getExistingDirectory(self, self.tr("Open Directory"),"/home", QFileDialog.ShowDirsOnly | QFileDialog.DontResolveSymlinks);
        self.saveFileNameLabel.setText(fileName)
    
            
    def saveFunction(self):
        for i in range(len(self.openFileNameLabel.text().split(','))):
            input = self.openFileNameLabel.text().split(',')[i]
            outdir = self.saveFileNameLabel.text()
            output_name = os.path.basename(str(input))
            output = os.path.join(str(outdir),str(output_name))
            if self.sdRadioButton.isChecked():
                print self.sdf(input, output)
            if self.dsRadioButton.isChecked():
                print self.dsf(input, output)
            if self.drRadioButton.isChecked():
                print self.drf(input, output)
            if self.rdRadioButton.isChecked():
                print self.rdf(input, output)
            if self.srRadioButton.isChecked():
                print self.sdrf(input, output)
            if self.rsRadioButton.isChecked():
                print self.rdsf(input, output)
    
            
    def dsf(self, input, output):
        L = np.loadtxt(str(input))
        Lx = L.shape[0]
        sesp = np.zeros((Lx,6),float)
        for i in np.arange(Lx):
            g = int(L[i,0])
            p = int((L[i,0] - int(L[i,0]))*60.)
            s = (((L[i,0] - int(L[i,0])) * 60. ) - p ) * 60.
            gg = int(L[i,1])
            pp = int((L[i,1] - int(L[i,1]))*60.)
            ss = (((L[i,1] - int(L[i,1])) * 60. ) - pp ) * 60.
            sesp[i,:] = np.hstack((g,p,s,gg,pp,ss))
            np.savetxt(output,sesp)
    
            
    def sdf(self, input, output):
        L = np.loadtxt(str(input))
        Lx = L.shape[0]
        sesp = np.zeros((Lx,2),float)
        for i in np.arange(Lx):
            lat = L[i,0]+(L[i,1]/60)+L[i,2]/3600
            lon = L[i,3]+(L[i,4]/60)+L[i,5]/3600
            sesp[i,:] = np.hstack((lat,lon))
            np.savetxt(output,sesp)
    

    def drf(self, input, output):
        L = np.loadtxt(str(input))
        Lx = L.shape[0]
        sesp = np.zeros((Lx,2),float)
        for i in np.arange(Lx):
            rad = (L[i,0] * pi ) / 180
            radd = (L[i,1] * pi ) / 180
            sesp[i,:] = np.hstack((rad,radd))
            np.savetxt(output,sesp)
    

    def rdf(self, input, output):
        L = np.loadtxt(str(input))
        Lx = L.shape[0]
        sesp = np.zeros((Lx,2),float)
        for i in np.arange(Lx):
            dec = (L[i,0] * 180 ) / pi
            decc = (L[i,1] * 180 ) / pi
            sesp[i,:] = np.hstack((dec,decc))
            np.savetxt(output,sesp)
    

    def sdrf(self, input, output):
        L = np.loadtxt(str(input))
        Lx = L.shape[0]
        sesp = np.zeros((Lx,2),float)
        for i in np.arange(Lx):
            dec = L[i,0]+(L[i,1]/60)+L[i,2]/3600
            rad = (dec * pi ) / 180.
            decc = L[i,3]+(L[i,4]/60)+L[i,5]/3600
            radd = (decc * pi ) / 180.
            sesp[i,:] = np.hstack((rad,radd))
            np.savetxt(output,sesp)
    

    def rdsf(self, input, output):
        L = np.loadtxt(str(input))
        Lx = L.shape[0]
        sesp = np.zeros((Lx,6),float)
        for i in np.arange(Lx):
            dec = (L[i,0] * 180 ) / pi
            g = int(dec)
            p = int((dec - int(dec)) * 60)
            s = (((dec - int(dec)) * 60) - p ) * 60
            decc = (L[i,1] * 180 ) / pi
            gg = int(decc)
            pp = int((decc - int(decc)) * 60)
            ss = (((decc - int(decc)) * 60) - pp ) * 60
            sesp[i,:] = np.hstack((g,p,s,gg,pp,ss))
            np.savetxt(output,sesp)
    

    def rdsff(self, radianti, sessagesimali):
        L = np.loadtxt(str(radianti))
        Lx = len(L)
        Lxx = np.zeros((Lx,1),float)
        for i in range(len(L)):
            dec = (L[i] * 180)/pi
            ses_g = int(dec)
            ses_p = int((dec - int(dec)) * 60)
            ses_s = (((dec - int(dec)) * 60) - ses_p ) * 60
            Lxx[i,0] = ses_g
            Lxx[i,1] = ses_p
            Lxx[i,2] = ses_s
            np.savetxt(sessagesimali,Lxx)
    
        
    def confirmQuit(self):
        return QMessageBox.question(self, "confirm", "Exit the Application",
        QMessageBox.No | QMessageBox.Yes, QMessageBox.Yes ) == QMessageBox.Yes
    

    def closeEvent(self, event=None):
        '''
        overrule QMaindow's close event
        '''
        if self.confirmQuit():
            #TODO - save some settings here????
            pass
        else:
            event.ignore()
    

    def quit(self):
        return QMessageBox.question(self, "confirm", "Exit the Application",
        QMessageBox.No | QMessageBox.Yes, QMessageBox.Yes ) == QMessageBox.Yes
        #self.app.closeAllWindows()
    

    def populatesrs(self):
        #codelist = []
        #f = open(str(epsgfile),'r')
        #f.seek(0)
        #for line in f:
        #    line = line.strip()
        #    codelist.append(line)
        self.setsrs.addItems(codelist)
    

    def setdstsrs(self,index):
        global dstsrs
        dstsrs = self.setsrs.itemText(index)
        return dstsrs
    

    def execprj(self):
        if self.rpjAffine.isChecked():
            # sTOsAffine(infile,outfile,locale,globale,srs_code)
            print self.sTOsAffine(unicode(self.rpjshpInput_file.text()),unicode(self.rpjshpOutput_file.text()), unicode(self.rpjGCP1_file.text()), unicode(self.rpjGCP2_file.text()), str(dstsrs))
        else:
            print self.sTOsConforme(unicode(self.rpjshpInput_file.text()),unicode(self.rpjshpOutput_file.text()), unicode(self.rpjGCP1_file.text()), unicode(self.rpjGCP2_file.text()), str(dstsrs))	
    
            
    def exectr(self):
        print self.sTOsTxy(unicode(self.trshpInput_file.text()),unicode(self.trshpOutput_file.text()), unicode(self.tx.text()), unicode(self.ty.text()))
    

    def execrot(self):
        print self.sTOsTxy(unicode(self.rotshpInput_file.text()),unicode(self.rotshpOutput_file.text()), unicode(self.angle.text()))
    

    def exectxtprj(self):
        if self.rpjtxtAffine.isChecked():
            print self.affine(unicode(self.rpjtxtGCP1_file.text()),unicode(self.rpjtxtGCP2_file.text()), unicode(self.rpjtxtInput_file.text()), unicode(self.rpjtxtOutput_file.text()))
        else:
            print self.conforme(unicode(self.rpjtxtGCP1_file.text()),unicode(self.rpjtxtGCP2_file.text()), unicode(self.rpjtxtInput_file.text()), unicode(self.rpjtxtOutput_file.text()))
    

    def exechelmert(self):
        print self.helmert(unicode(self.helmertGCP1_file.text()),unicode(self.helmertGCP2_file.text()), unicode(self.helmertInput_file.text()), unicode(self.helmertOutput_file.text()))
    

    def hOpenFileInput2(self):
        string = ''
        h_txtInput = QFileDialog.getOpenFileNames(self, self.tr("Input file"), self.helmertInput_file.text(), self.tr("All Files (*);;Shape Files (*.shp)"))
        for i in range(len(ShapeName)):
            string += h_txtInput[i]+','
        self.helmertInput_file.setText(string[:-1])
    

    def hOpenFileOutput2(self):
        h_txtOutput = QFileDialog.getExistingDirectory(self, self.tr("Open Directory"),"/home", QFileDialog.ShowDirsOnly | QFileDialog.DontResolveSymlinks);
        self.rpjshpOutput_file.setText(h_txtOutput)
    

    def hOpenFileInput(self):
        h_txtInput = QFileDialog.getOpenFileName(self,self.helmertInput_file.text())
        if not h_txtInput.isEmpty():
            self.helmertInput_file.setText(h_txtInput)
    

    def hOpenFileOutput(self):
        h_txtOutput = QFileDialog.getSaveFileName(self,self.helmertOutput_file.text())
        if not h_txtOutput.isEmpty():
            self.helmertOutput_file.setText(h_txtOutput)
    

    def hOpenFileGCP1(self):
        h_txtGCP1 = QFileDialog.getOpenFileName(self,self.helmertGCP1_file.text())
        if not h_txtGCP1.isEmpty():
            self.helmertGCP1_file.setText(h_txtGCP1)
    

    def hOpenFileGCP2(self):
        h_txtGCP2 = QFileDialog.getOpenFileName(self,self.helmertGCP2_file.text())
        if not h_txtGCP2.isEmpty():
            self.helmertGCP2_file.setText(h_txtGCP2)
    

    def prjOpenFileInput(self):
        prj_Input = QFileDialog.getOpenFileName(self,self.rpjshpInput_file.text())
        if not prj_Input.isEmpty():
            self.rpjshpInput_file.setText(prj_Input)
    

    def prjOpenFileOutput(self):
        prj_Output = QFileDialog.getSaveFileName(self,self.rpjshpOutput_file.text())
        if not prj_Output.isEmpty():
            self.rpjshpOutput_file.setText(prj_Output)
    

    def rpjOpenFileGCP1(self):
        prj_GCP1 = QFileDialog.getOpenFileName(self,self.rpjGCP1_file.text())
        if not prj_GCP1.isEmpty():
            self.rpjGCP1_file.setText(prj_GCP1)
    
            
    def rpjOpenFileGCP2(self):	
        prj_GCP2 = QFileDialog.getOpenFileName(self,self.rpjGCP2_file.text())
        if not prj_GCP2.isEmpty():
            self.rpjGCP2_file.setText(prj_GCP2)
    

    def trOpenFileInput2(self):
        string = ''
        tr_Input = QFileDialog.getOpenFileNames(self, self.tr("Input file"), self.trshpInput_file.text(), self.tr("All Files (*);;Shape Files (*.shp)"))
        for i in range(len(ShapeName)):
            string += tr_Input[i]+','
        self.trshpInput_file.setText(string[:-1])
    

    def trOpenFileOutput2(self):
        tr_Output = QFileDialog.getExistingDirectory(self, self.tr("Open Directory"),"/home", QFileDialog.ShowDirsOnly | QFileDialog.DontResolveSymlinks);
        self.trshpOutput_file.setText(tr_Output)
    

    def trOpenFileInput(self):
        tr_Input = QFileDialog.getOpenFileName(self,self.trshpInput_file.text())
        if not tr_Input.isEmpty():
            self.trshpInput_file.setText(tr_Input)	
    

    def trOpenFileOutput(self):
        tr_Output = QFileDialog.getSaveFileName(self,self.trshpOutput_file.text())
        if not tr_Output.isEmpty():
            self.trshpOutput_file.setText(tr_Output)
    
            
    def rotOpenFileInput2(self):
        string = ''
        rot_Input = QFileDialog.getOpenFileNames(self, self.tr("Input file"), self.rotshpInput_file.text(), self.tr("All Files (*);;Shape Files (*.shp)"))
        for i in range(len(ShapeName)):
            string += rot_Input[i]+','
        self.trshpInput_file.setText(string[:-1])
    

    def rotOpenFileOutput2(self):
        tr_Output = QFileDialog.getExistingDirectory(self, self.tr("Open Directory"),"/home", QFileDialog.ShowDirsOnly | QFileDialog.DontResolveSymlinks);
        self.trshpOutput_file.setText(tr_Output)
    

    def rotOpenFileInput(self):
        tr_Input = QFileDialog.getOpenFileName(self,self.trshpInput_file.text())
        if not tr_Input.isEmpty():
            self.trshpInput_file.setText(tr_Input)	
    

    def rotOpenFileOutput(self):
        rot_Output = QFileDialog.getSaveFileName(self,self.rotshpOutput_file.text())
        if not rot_Output.isEmpty():
            self.rotshpOutput_file.setText(rot_Output)
    

    def txtOpenFileInput(self):
        prj_txtInput = QFileDialog.getOpenFileName(self,self.rpjtxtInput_file.text())
        if not prj_txtInput.isEmpty():
            self.rpjtxtInput_file.setText(prj_txtInput)
    

    def txtOpenFileOutput(self):
        prj_txtOutput = QFileDialog.getSaveFileName(self,self.rpjtxtOutput_file.text())
        if not prj_txtOutput.isEmpty():
            self.rpjtxtOutput_file.setText(prj_txtOutput)
    

    def txtOpenFileGCP1(self):
        prj_txtGCP1 = QFileDialog.getOpenFileName(self,self.rpjtxtGCP1_file.text())
        if not prj_txtGCP1.isEmpty():
            self.rpjtxtGCP1_file.setText(prj_txtGCP1)
    

    def txtOpenFileGCP2(self):
        prj_txtGCP2 = QFileDialog.getOpenFileName(self,self.rpjtxtGCP2_file.text())
        if not prj_txtGCP2.isEmpty():
            self.rpjtxtGCP2_file.setText(prj_txtGCP2)
    
  
    def WalkAndTransform( self,geom ):
        if geom.GetGeometryCount() > 0:
            for i in range(geom.GetGeometryCount()):
                old_geom = geom.GetGeometryRef(i)
                new_geom = WalkAndTransform( old_geom )
                if new_geom is not old_geom:
                    geom.SetGeometryDirectly( new_geom )
            return geom
        for i in range(geom.GetPointCount()):
            xyz = (geom.GetX(i), geom.GetY(i), geom.GetZ(i))
            xyz = self.TXY( xyz )
            geom.SetPoint( i, xyz[0], xyz[1], xyz[2] )
        return geom
    

    def TXY( self,xyz ):
        x = xyz[0] + int(tx) 
        y = xyz[1] + int(ty) 
        z = xyz[2]
        return (float(x),float(y),float(z))
    

    def sTOsTxy(self,infile,outfile,tr_x,tr_y):
        i = 1
        global tx
        global ty
        global cntx
        global cnty
        while i < len(sys.argv):
            arg = sys.argv[i]
            if infile is None:
                infile = arg
            elif outfile is None:
                outfile = arg
            elif angle is None:
                angle = arg
            else:
                Usage()
            i = i + 1
        if outfile is None:
            Usage()
        in_ds = ogr.Open( str(infile), update = 0 )
        layer_name = None
        if layer_name is not None:
            in_layer = in_ds.GetLayerByName( layer_name )
        else:
            in_layer = in_ds.GetLayer( 0 )
            in_defn = in_layer.GetLayerDefn()
        ext = in_ds.GetLayerByIndex(0).GetExtent()
        tx = tr_x
        ty = tr_y
        cntx = ext[0] + ((ext[1]-ext[0])/2)
        cnty = ext[2] + ((ext[3]-ext[2])/2)
        shp_driver = ogr.GetDriverByName( 'ESRI Shapefile' )
        shp_driver.DeleteDataSource( str(outfile) )
        shp_ds = shp_driver.CreateDataSource( str(outfile) )
        shp_layer = shp_ds.CreateLayer( in_defn.GetName(),geom_type = in_defn.GetGeomType(),srs = in_layer.GetSpatialRef() )
        in_field_count = in_defn.GetFieldCount()
        for fld_index in range(in_field_count):
            src_fd = in_defn.GetFieldDefn( fld_index )
            fd = ogr.FieldDefn( src_fd.GetName(), src_fd.GetType() )
            fd.SetWidth( src_fd.GetWidth() )
            fd.SetPrecision( src_fd.GetPrecision() )
            shp_layer.CreateField( fd )
        in_feat = in_layer.GetNextFeature()
        while in_feat is not None:
            if in_feat.GetGeometryRef() is not None : 
                geom = in_feat.GetGeometryRef().Clone()
                geom = self.WalkAndTransform( geom )
                out_feat = ogr.Feature( feature_def = shp_layer.GetLayerDefn() )
                out_feat.SetFrom( in_feat )
                out_feat.SetGeometryDirectly( geom )
                shp_layer.CreateFeature( out_feat )
            out_feat.Destroy()
            in_feat.Destroy()
            in_feat = in_layer.GetNextFeature()
        shp_ds.Destroy()
        in_ds.Destroy()
    
        
    def WalkAndTransformRota( self,geom ):
        if geom.GetGeometryCount() > 0:
            for i in range(geom.GetGeometryCount()):
                old_geom = geom.GetGeometryRef(i)
                new_geom = WalkAndTransformRota( old_geom )
                if new_geom is not old_geom:
                    geom.SetGeometryDirectly( new_geom )
            return geom
        for i in range(geom.GetPointCount()):
            xyz = (geom.GetX(i), geom.GetY(i), geom.GetZ(i))
            xyz = self.rotaxy( xyz )
            geom.SetPoint( i, xyz[0], xyz[1], xyz[2] )
        return geom
    

    def rotaxy( self,xyz ):
        x = xyz[0] - cntx 
        y = xyz[1] - cnty 
        z = xyz[2]
        angle = angolo
        ar = angle * math.pi/180.
        R = np.zeros((2,2),float)
        R[0,0]=math.cos(ar)
        R[0,1]=math.sin(ar)
        R[1,0]=-math.sin(ar)
        R[1,1]=math.cos(ar)
        zed = np.zeros((2,1),float)
        zed[0,0]=x
        zed[1,0]=y
        zz = np.zeros((2,1),float)
        d = np.dot(R,zed)
        x = d[0] + cntx
        y = d[1] + cnty
        return (float(x),float(y),float(z))
    

    def sTOsRota(infile,outfile,ang):
        i = 1
        global angolo
        global cntx
        global cnty
        while i < len(sys.argv):
            arg = sys.argv[i]
            if infile is None:
                infile = arg
            elif outfile is None:
                outfile = arg
            elif angle is None:
                angle = arg
            else:
                Usage()
        i = i + 1
        if outfile is None:
            Usage()
        in_ds = ogr.Open( str(infile), update = 0 )
        layer_name = None
        if layer_name is not None:
            in_layer = in_ds.GetLayerByName( layer_name )
        else:
            in_layer = in_ds.GetLayer( 0 )
            in_defn = in_layer.GetLayerDefn()
        angolo = float(ang)
        ext = in_ds.GetLayerByIndex(0).GetExtent()
        cntx = ext[0] + ((ext[1]-ext[0])/2)
        cnty = ext[2] + ((ext[3]-ext[2])/2)
        shp_driver = ogr.GetDriverByName( 'ESRI Shapefile' )
        shp_driver.DeleteDataSource( str(outfile) )
        shp_ds = shp_driver.CreateDataSource( str(outfile) )
        shp_layer = shp_ds.CreateLayer( in_defn.GetName(),geom_type = in_defn.GetGeomType(),srs = in_layer.GetSpatialRef() )
        in_field_count = in_defn.GetFieldCount()
        for fld_index in range(in_field_count):
            src_fd = in_defn.GetFieldDefn( fld_index )
            fd = ogr.FieldDefn( src_fd.GetName(), src_fd.GetType() )
            fd.SetWidth( src_fd.GetWidth() )
            fd.SetPrecision( src_fd.GetPrecision() )
            shp_layer.CreateField( fd )
        in_feat = in_layer.GetNextFeature()
        while in_feat is not None:
            if in_feat.GetGeometryRef() is not None :
                geom = in_feat.GetGeometryRef().Clone()
                geom = self.WalkAndTransformRota( geom )
                out_feat = ogr.Feature( feature_def = shp_layer.GetLayerDefn() )
                out_feat.SetFrom( in_feat )
                out_feat.SetGeometryDirectly( geom )
                shp_layer.CreateFeature( out_feat )
            out_feat.Destroy()
            in_feat.Destroy()
            in_feat = in_layer.GetNextFeature()
        shp_ds.Destroy()
        in_ds.Destroy()
    
        
    def WalkAndTransformConf( self,geom ):
        if geom.GetGeometryCount() > 0:
            for i in range(geom.GetGeometryCount()):
                old_geom = geom.GetGeometryRef(i)
                new_geom = self.WalkAndTransformConf( old_geom )
                if new_geom is not old_geom:
                    geom.SetGeometryDirectly( new_geom )
            return geom
        for i in range(geom.GetPointCount()):
            xyz = (geom.GetX(i), geom.GetY(i), geom.GetZ(i))
            xyz = self.conformexy( xyz )
            geom.SetPoint( i, xyz[0], xyz[1], xyz[2] )
        return geom
    

    def conformexy( self,xyz ):
        L = LL
        G = GG
        x = xyz[0]
        y = xyz[1]
        z = xyz[2]
        A = np.zeros((2*L.shape[0],4),float)
        A[ ::2, 0] = 1.0
        A[1::2, 1] = 1.0
        A[ ::2, 2] = L[:,0]
        A[1::2, 2] = L[:,1]
        A[ ::2, 3] = L[:,1]
        A[1::2, 3] = -L[:,0]
        Y = np.zeros((2*G.shape[0],1),float)
        Y[ ::2, 0] = G[:,0]
        Y[1::2, 0] = G[:,1]
        N = np.dot(A.T.conj(), A)
        T = np.dot(A.T.conj(), Y)
        C = np.dot(linalg.inv(N), T)
        Lambda = abs(C[2]+C[3]*1j)
        Alpha = np.angle(C[2]+C[3]*1j)
        E0 = C[0]
        N0 = C[1]
        x = E0+xyz[0]*C[2]+xyz[1]*C[3]
        y = N0+xyz[1]*C[2]-xyz[0]*C[3]			
        return (float(x),float(y),float(z))
    

    def sTOsConforme(self,infile,outfile,locale,globale,srs_code):
        i = 1
        global LL
        global GG
        while i < len(sys.argv):
            arg = sys.argv[i]
            if infile is None:
                infile = arg
            elif outfile is None:
                outfile = arg
            elif locale is None:
                locale = arg
            elif globale is None:
                globale = arg
            elif dst_srs is None:
                dst_srs = arg
            else:
                Usage()
        i = i + 1
        if outfile is None:
            Usage()
        in_ds = ogr.Open( str(infile), update = 0 )
        layer_name = None
        if layer_name is not None:
            in_layer = in_ds.GetLayerByName( layer_name )
        else:
            in_layer = in_ds.GetLayer( 0 )
            in_defn = in_layer.GetLayerDefn()
        LL = np.loadtxt(str(locale))
        GG = np.loadtxt(str(globale))
        shp_driver = ogr.GetDriverByName( 'ESRI Shapefile' )
        shp_driver.DeleteDataSource( str(outfile) )
        shp_ds = shp_driver.CreateDataSource( str(outfile) )
        dst_srs = osr.SpatialReference()
        dst_srs.ImportFromEPSG(int(srs_code))
        shp_layer = shp_ds.CreateLayer( in_defn.GetName(),geom_type = in_defn.GetGeomType(),srs = dst_srs )
        in_field_count = in_defn.GetFieldCount()
        for fld_index in range(in_field_count):
            src_fd = in_defn.GetFieldDefn( fld_index )
            fd = ogr.FieldDefn( src_fd.GetName(), src_fd.GetType() )
            fd.SetWidth( src_fd.GetWidth() )
            fd.SetPrecision( src_fd.GetPrecision() )
            shp_layer.CreateField( fd )
        in_feat = in_layer.GetNextFeature()
        while in_feat is not None:
            if in_feat.GetGeometryRef() is not None :
                geom = in_feat.GetGeometryRef().Clone()
                geom = self.WalkAndTransformConf( geom )
                out_feat = ogr.Feature( feature_def = shp_layer.GetLayerDefn() )
                out_feat.SetFrom( in_feat )
                out_feat.SetGeometryDirectly( geom )
                shp_layer.CreateFeature( out_feat )
                out_feat.Destroy()
            in_feat.Destroy()
            in_feat = in_layer.GetNextFeature()
        shp_ds.Destroy()
        in_ds.Destroy()
    

    def WalkAndTransformAff( self,geom ):
        if geom.GetGeometryCount() > 0:
            for i in range(geom.GetGeometryCount()):
                old_geom = geom.GetGeometryRef(i)
                new_geom = self.WalkAndTransformAff( old_geom )
                if new_geom is not old_geom:
                    geom.SetGeometryDirectly( new_geom )
            return geom
        for i in range(geom.GetPointCount()):
            xyz = (geom.GetX(i), geom.GetY(i), geom.GetZ(i))
            xyz = self.affinexy( xyz )
            geom.SetPoint( i, xyz[0], xyz[1], xyz[2] )
        return geom
    

    def affinexy( self,xyz ):
        L = LL
        G = GG
        x = xyz[0]
        y = xyz[1]
        z = xyz[2]
        A = np.zeros((2 * L.shape[0],6),float)
        A[ ::2, 2] = 1.0
        A[1::2, 5] = 1.0
        A[ ::2, 0] = L[:,0]
        A[1::2, 4] = L[:,1]
        A[ ::2, 1] = L[:,1]
        A[1::2, 3] = L[:,0]
        Y = np.zeros((2*G.shape[0],1),float)
        Y[ ::2, 0] = G[:,0]
        Y[1::2, 0] = G[:,1]
        N = np.dot(A.T.conj(), A)
        T = np.dot(A.T.conj(), Y)
        C = np.dot(linalg.inv(N), T)
        E0 = C[2]
        N0 = C[5]
        x = E0+xyz[0]*C[0]+xyz[1]*C[1]
        y = N0+xyz[0]*C[3]+xyz[1]*C[4]
        return (float(x),float(y),float(z))
    

    def sTOsAffine(self,infile,outfile,locale,globale,srs_code):
        i = 1
        global LL
        global GG
        while i < len(sys.argv):
            arg = sys.argv[i]
            if infile is None:
                infile = arg
            elif outfile is None:
                outfile = arg
            elif locale is None:
                locale = arg
            elif globale is None:
                globale = arg
            else:
                Usage()
        i = i + 1
        if outfile is None:
            Usage()
        in_ds = ogr.Open( str(infile), update = 0 )
        layer_name = None
        if layer_name is not None:
            in_layer = in_ds.GetLayerByName( layer_name )
        else:
            in_layer = in_ds.GetLayer( 0 )
            in_defn = in_layer.GetLayerDefn()
        LL = np.loadtxt(str(locale))
        GG = np.loadtxt(str(globale))
        shp_driver = ogr.GetDriverByName( 'ESRI Shapefile' )
        shp_driver.DeleteDataSource( str(outfile) )
        shp_ds = shp_driver.CreateDataSource( str(outfile) )
        dst_srs = osr.SpatialReference()
        dst_srs.ImportFromEPSG(int(srs_code))
        shp_layer = shp_ds.CreateLayer( in_defn.GetName(),geom_type = in_defn.GetGeomType(),srs = dst_srs )
        in_field_count = in_defn.GetFieldCount()
        for fld_index in range(in_field_count):
            src_fd = in_defn.GetFieldDefn( fld_index )
            fd = ogr.FieldDefn( src_fd.GetName(), src_fd.GetType() )
            fd.SetWidth( src_fd.GetWidth() )
            fd.SetPrecision( src_fd.GetPrecision() )
            shp_layer.CreateField( fd )
        in_feat = in_layer.GetNextFeature()
        while in_feat is not None:
            if in_feat.GetGeometryRef() is not None :
                geom = in_feat.GetGeometryRef().Clone()
                geom = self.WalkAndTransformAff( geom )
                out_feat = ogr.Feature( feature_def = shp_layer.GetLayerDefn() )
                out_feat.SetFrom( in_feat )
                out_feat.SetGeometryDirectly( geom )
                shp_layer.CreateFeature( out_feat )
                out_feat.Destroy()
            in_feat.Destroy()
            in_feat = in_layer.GetNextFeature()
        shp_ds.Destroy()
        in_ds.Destroy()
    

    def affine(self, gcpD1, gcpD2, knowD1, output):
        L = np.loadtxt(str(gcpD1))
        A = np.zeros((2*L.shape[0],6),float)
        A[ ::2, 2] = 1.0
        A[1::2, 5] = 1.0
        A[ ::2, 0] = L[:,0]
        A[1::2, 4] = L[:,1]
        A[ ::2, 1] = L[:,1]
        A[1::2, 3] = L[:,0]
        G = np.loadtxt(str(gcpD2))
        Y = np.zeros((2*G.shape[0],1),float)
        Y[ ::2, 0] = G[:,0]
        Y[1::2, 0] = G[:,1]
        N = np.dot(A.T.conj(), A)
        T = np.dot(A.T.conj(), Y)
        C = np.dot(linalg.inv(N), T)
        E0 = C[2]
        N0 = C[5]
        LX = np.loadtxt(str(knowD1))
        ss = LX.shape[0]
        ENglob = np.zeros((ss,2),float)
        for i in np.arange(ss):
            E2 = E0+LX[i,0]*C[0]+LX[i,1]*C[1]
            N2 = N0+LX[i,0]*C[3]+LX[i,1]*C[4]
            ENglob[i,:] = np.hstack((E2,N2))
        savetxt(output,ENglob)
        sq = LX.shape[0]
        sqx = np.zeros((G.shape[0],1),float)
        sqy = np.zeros((G.shape[0],1),float)
        sqx[ ::1, 0] = G[:,0]
        sqy[::1, 0] = G[:,1]
        sizeq = sqx.shape[0]
        scartix = np.zeros((sizeq,1),float)
        scartiy = np.zeros((sizeq,1),float)
        scartiqx = np.zeros((sizeq,1),float)
        scartiqy = np.zeros((sizeq,1),float)
        sqm = np.zeros((sizeq,1),float)
        for i in np.arange(sizeq):
            Vx = E0 + LX[i,0] * C[0,0] + LX[i,1] * C[1,0] - sqx[i,0]
            Vy = N0 + LX[i,0] * C[3,0] + LX[i,1] * C[4,0] - sqy[i,0]
            sqmr = math.sqrt(Vx**2 + Vy**2)
            sqm[i,:] = sqmr
            scartiqx[i,0] = Vx**2
            scartiqy[i,0] = Vy**2
            scartix[i,0] = Vx
            scartiy[i,0] = Vx
        scartiq = np.concatenate((scartiqx,scartiqy))
        scarti = np.concatenate((scartix,scartiy))
        scartixy = np.concatenate((scartiqx,scartiqy),1)
        scartiqxy = np.concatenate((scartix,scartiy),1)
        sumscarti = sum(scartiq)
        varqp = sumscarti / ((2 * ss) - 4)
        varp = math.sqrt(varqp)
        varianzaq = varqp * linalg.inv(N)
        varianza = varp * linalg.inv(N)
        varqunitp = np.diag(varianzaq)
        varunitp = np.diag(varianza)
        prec = np.zeros((sq,2),float)
        for i in np.arange(sq):
            xx = math.sqrt((LX[i,0]**2) * (varqunitp[0]) + (LX[i,1]**2) * (varqunitp[1]) + varqunitp[2])
            yy = math.sqrt((LX[i,1]**2) * (varqunitp[0]) + (LX[i,0]**2) * (varqunitp[1]) + varqunitp[5])
            prec[i,:] = np.hstack((xx,yy))
        errore = np.concatenate((scartixy,scartiqxy,sqm),1)
        np.savetxt(output+str('_precision'), prec)
        np.savetxt(output+str('_error'), errore)
        err_med = sum(sqm) / sq		
        results = str('Trasformation Parameters :\n')
        results += str('Tx : ')+str(C[2,0])+'\n'
        results += str('Ty : ')+str(C[5,0])+'\n'
        results += str('Rigid Rotation X : ')+str(C[1,0])+'\n'
        results += str('Rigid Rotation Y : ')+str(C[3,0])+'\n'
        results += str('Scale Factor X : ')+str(C[0,0])+'\n'
        results += str('Scale Factor Y : ')+str(C[4,0])+'\n'
        results += '\n'
        results += str('Results : ')+'\n'
        results += str(ENglob)
        results += str("\n")
        results += str('Mean Error : ')+'\n'
        results += str(err_med)+'\n'
        results += '\n'
        results += str('Precision : ')+'\n'
        results += str(prec)+'\n'
        results += '\n'
        results += str('Error : ')+'\n'
        results += str(errore)
        results = results.replace('[','').replace(']','')
        self.textBrowser.setText(str(results))
    

    def conforme(self, gcpD1, gcpD2, knowD1, output):
        L = np.loadtxt(str(gcpD1))
        A = np.zeros((2*L.shape[0],4),float)
        A[ ::2, 0] = 1.0
        A[1::2, 1] = 1.0
        A[ ::2, 2] = L[:,0]
        A[1::2, 2] = L[:,1]
        A[ ::2, 3] = L[:,1]
        A[1::2, 3] = -L[:,0]
        G = np.loadtxt(str(gcpD2))
        Y = np.zeros((2*G.shape[0],1),float)
        Y[ ::2, 0] = G[:,0]
        Y[1::2, 0] = G[:,1]
        N = np.dot(A.T.conj(), A)
        T = np.dot(A.T.conj(), Y)
        C = np.dot(linalg.inv(N), T)
        Lambda = abs(C[2]+C[3]*1j)
        Alpha = np.angle(C[2]+C[3]*1j)
        E0 = C[0]
        N0 = C[1]
        LX = np.loadtxt(str(knowD1))
        ss = LX.shape[0]
        pq = G.shape[0]
        ENglob = np.zeros((ss,2),float)
        lam = math.sqrt(C[2]** + C[3]**2)
        alp = np.arctan(C[3] / C[2]) / (math.pi / 180.)
        for i in np.arange(ss):
            E2 = E0+LX[i,0]*C[2]+LX[i,1]*C[3]
            N2 = N0+LX[i,1]*C[2]-LX[i,0]*C[3]
            ENglob[i,:] = np.hstack((E2,N2))
        np.savetxt(output, ENglob)
        sq = LX.shape[0]
        sqx = np.zeros((G.shape[0],1),float)
        sqy = np.zeros((G.shape[0],1),float)
        sqx[ ::1, 0] = G[:,0]
        sqy[::1, 0] = G[:,1]
        sizeq = sqx.shape[0]
        scartix = np.zeros((sizeq,1),float)
        scartiy = np.zeros((sizeq,1),float)
        scartiqx = np.zeros((sizeq,1),float)
        scartiqy = np.zeros((sizeq,1),float)
        sqm = np.zeros((sizeq,1),float)
        for i in np.arange(sizeq):
            Vx = E0 + LX[i,0] * C[2,0] + LX[i,1] * C[3,0] - sqx[i,0]
            Vy = N0 + LX[i,1] * C[2,0] - LX[i,0] * C[3,0] - sqy[i,0]
            sqmr = math.sqrt(Vx**2 + Vy**2)
            sqm[i,:] = sqmr
            scartiqx[i,0] = Vx**2
            scartiqy[i,0] = Vy**2
            scartix[i,0] = Vx
            scartiy[i,0] = Vy
        scartiq = np.concatenate((scartiqx,scartiqy))
        scarti = np.concatenate((scartix,scartiy))
        scartixy = np.concatenate((scartiqx,scartiqy),1)
        scartiqxy = np.concatenate((scartix,scartiy),1)
        sumscarti = sum(scartiq)
        varqp = sumscarti / ((2 * sizeq) - 4)
        varp = math.sqrt(varqp)
        varianzaq = varqp * linalg.inv(N)
        varianza = varp * linalg.inv(N)
        varqunitp = np.diag(varianzaq)
        varunitp = np.diag(varianza)
        prec = np.zeros((sq,2),float)
        for i in np.arange(sq):
            xx = math.sqrt((LX[i,0]**2) * (varqunitp[2]) + (LX[i,1]**2) * (varqunitp[3]) + varqunitp[0])
            yy = math.sqrt((LX[i,1]**2) * (varqunitp[2]) + (LX[i,0]**2) * (varqunitp[3]) + varqunitp[1])
            prec[i,:] = np.hstack((xx,yy))
        errore = np.concatenate((scartixy,scartiqxy,sqm),1)
        np.savetxt(output+str('_precision'), prec)
        np.savetxt(output+str('_error'), errore)
        err_med = sum(sqm) / pq
        a = C[2,0]
        b = C[3,0]
        lamvar = math.sqrt(((((2 * a) / math.sqrt(a**2 + b**2))**2) * varqunitp[2]) + ((((2 * b) / math.sqrt(a**2 + b**2))**2) * varqunitp[3]))
        alphavar = math.sqrt(((((-b / a**2) / (1 + (b / a)**2))**2) * varqunitp[2]) + (((1 / a) / (1 + (b / a)**2))**2 * varqunitp[3])) / (math.pi / 180.)
        results = str('Trasformation Parameters :\n')
        results += str("\n")
        results += str('Tx : ')+str(C[0,0])+'\n'
        results += str('Ty : ')+str(C[1,0])+'\n'
        results += str('Rigid Rotation : ')+str(alp)+'\n'
        results += str('Scale Factor  : ')+str(lam)+'\n'
        results += str("\n")
        results += str('Trasformation Parameters Variance :\n')
        results += str("\n")
        results += str('Sigma quadro Rigid Rotation :')+str(alphavar)+'\n'
        results += str('Sigma quadro Scale Factor:')+str(lamvar)+'\n'
        results += str("\n")
        results += str('Results : ')+str("\n")
        results += str(ENglob)
        results += '\n'
        results += str('Mean Error : ')+'\n'
        results += str(err_med)+'\n'
        results += '\n'
        results += str('Precision : ')+'\n'
        results += str(prec)
        results += '\n'
        results += str('Error : ')+str("\n")
        results += str(errore)
        results = results.replace('[','').replace(']','')
        self.textBrowser.setText(str(results))
    

    def vcross(self,v):
        x, y, z = v
        mat = np.zeros((3,3))
        mat[0] = [ 0, -z,  y]
        mat[1] = [ z,  0, -x]
        mat[2] = [-y,  x,  0]
        return mat
    

    def block(self,v):
        return np.hstack((np.eye(3), -self.vcross(v), v[:, np.newaxis]))
    

    def helmert(self, gcp1, gcp2, inputf, output):
        pt1 = np.loadtxt(str(gcp1))
        pt2 = np.loadtxt(str(gcp2))
        A = []
        rhs = []
        for i in range(3):
            A.append(self.block(pt1[i]))
            rhs.append((pt2[i] - pt1[i])[:, np.newaxis])
        A = np.vstack(A)
        rhs = np.vstack(rhs)
        sol = np.linalg.lstsq(A, rhs)[0]
        res = rhs - np.dot(A, sol)
        XYZ = np.loadtxt(str(inputf))
        XYZsize = XYZ.shape[0]
        ENZglob = np.zeros((XYZsize,3),float)
        for i in np.arange(XYZsize):
            X = sol[0] + (1 + sol[6]) * ( XYZ[i,0] - sol[5] * XYZ[i,1]  + sol[4] * XYZ[i,2] ) 
            Y = sol[1] + (1 + sol[6]) * ( sol[5] * XYZ[i,0] + XYZ[i,1] - sol[3] * XYZ[i,2] ) 
            Z = sol[2] + (1 + sol[6]) * ( -sol[4] * XYZ[i,0] + sol[3] * XYZ[i,1] + XYZ[i,2] ) 
            ENZglob[i,:] = np.hstack((X,Y,Z))
        np.savetxt(output,ENZglob)
        hresults = str('Trasformation Parameters :\n')
        hresults += str("\n")
        hresults = str('Traslation :\n')
        hresults += str("\n")
        hresults += str('Tx : ')+str(sol[0])+'\n'
        hresults += str('Ty : ')+str(sol[1])+'\n'
        hresults += str('Tz : ')+str(sol[2])+'\n'
        hresults += str("\n")
        hresults += str('Rotation :\n ')
        hresults += str("\n")
        hresults += str('Rx : ')+str(sol[3])+'\n'
        hresults += str('Ry : ')+str(sol[4])+'\n'
        hresults += str('Rz : ')+str(sol[5])+'\n'
        hresults += str("\n")
        hresults += str('Scale Factor  :\n ')
        hresults += str("\n")
        hresults += str("S : ")+str(sol[6])+'\n'
        hresults += str("\n")
        hresults += str('Trasformation Residuals :\n')
        hresults += str("\n")
        hresults += str(res)+'\n'
        hresults += str('Results : \n')
        hresults += str("\n")
        hresults += str(ENZglob)
        hresults += '\n'
        hresults = hresults.replace('[','').replace(']','')
        self.helmertBrowser.setText(str(hresults))
    