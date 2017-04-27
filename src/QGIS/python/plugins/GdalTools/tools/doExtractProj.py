# -*- coding: utf-8 -*-

"""
***************************************************************************
    doExtractProj.py
    ---------------------
    Date                 : August 2011
    Copyright            : (C) 2011 by Alexander Bruy
    Email                : alexander dot bruy at gmail dot com
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
"""

__author__ = 'Alexander Bruy'
__date__ = 'August 2011'
__copyright__ = '(C) 2011, Alexander Bruy'
# This will get replaced with a git SHA1 when you do a git archive
__revision__ = '$Format:%H$'

from qgis.PyQt.QtCore import Qt, QCoreApplication, QThread, QMutex
from qgis.PyQt.QtWidgets import QDialog, QDialogButtonBox, QApplication, QMessageBox
from qgis.PyQt.QtGui import QCursor

from .ui_dialogExtractProjection import Ui_GdalToolsDialog as Ui_Dialog
from . import GdalTools_utils as Utils

import os.path

req_mods = {"osgeo": "osgeo [python-gdal]"}
try:
    from osgeo import gdal
    from osgeo import osr
except ImportError as e:
    error_str = e.args[0]
    error_mod = error_str.replace("No module named ", "")
    if error_mod in req_mods:
        error_str = error_str.replace(error_mod, req_mods[error_mod])
    raise ImportError(error_str)


class GdalToolsDialog(QDialog, Ui_Dialog):

    def __init__(self, iface):
        QDialog.__init__(self, iface.mainWindow())
        self.setupUi(self)
        self.iface = iface

        self.inSelector.setType(self.inSelector.FILE)

        self.recurseCheck.hide()

        self.okButton = self.buttonBox.button(QDialogButtonBox.Ok)
        self.cancelButton = self.buttonBox.button(QDialogButtonBox.Cancel)

        self.inSelector.selectClicked.connect(self.fillInputFileEdit)
        self.batchCheck.stateChanged.connect(self.switchToolMode)

    def switchToolMode(self):
        self.recurseCheck.setVisible(self.batchCheck.isChecked())

        self.inSelector.clear()

        if self.batchCheck.isChecked():
            self.inFileLabel = self.label.text()
            self.label.setText(QCoreApplication.translate("GdalTools", "&Input directory"))

            self.inSelector.selectClicked.disconnect(self.fillInputFileEdit)
            self.inSelector.selectClicked.connect(self.fillInputDir)
        else:
            self.label.setText(self.inFileLabel)

            self.inSelector.selectClicked.connect(self.fillInputFileEdit)
            self.inSelector.selectClicked.disconnect(self.fillInputDir)

    def fillInputFileEdit(self):
        lastUsedFilter = Utils.FileFilter.lastUsedRasterFilter()
        inputFile = Utils.FileDialog.getOpenFileName(self, self.tr("Select the file to analyse"), Utils.FileFilter.allRastersFilter(), lastUsedFilter)
        if not inputFile:
            return
        Utils.FileFilter.setLastUsedRasterFilter(lastUsedFilter)
        self.inSelector.setFilename(inputFile)

    def fillInputDir(self):
        inputDir = Utils.FileDialog.getExistingDirectory(self, self.tr("Select the input directory with files to Assign projection"))
        if not inputDir:
            return
        self.inSelector.setFilename(inputDir)

    def reject(self):
        QDialog.reject(self)

    def accept(self):
        self.inFiles = None
        if self.batchCheck.isChecked():
            self.inFiles = Utils.getRasterFiles(self.inSelector.filename(), self.recurseCheck.isChecked())
        else:
            self.inFiles = [self.inSelector.filename()]

        self.progressBar.setRange(0, len(self.inFiles))

        QApplication.setOverrideCursor(QCursor(Qt.WaitCursor))
        self.okButton.setEnabled(False)

        self.extractor = ExtractThread(self.inFiles, self.prjCheck.isChecked())
        self.extractor.fileProcessed.connect(self.updateProgress)
        self.extractor.processFinished.connect(self.processingFinished)
        self.extractor.processInterrupted.connect(self.processingInterrupted)

        self.buttonBox.rejected.disconnect(self.reject)
        self.buttonBox.rejected.connect(self.stopProcessing)

        self.extractor.start()

    def updateProgress(self):
        self.progressBar.setValue(self.progressBar.value() + 1)

    def processingFinished(self):
        self.stopProcessing()
        QMessageBox.information(self, self.tr("Finished"), self.tr("Processing completed."))

    def processingInterrupted(self):
        self.restoreGui()

    def stopProcessing(self):
        if self.extractor is not None:
            self.extractor.stop()
            self.extractor = None

        self.restoreGui()

    def restoreGui(self):
        self.progressBar.setRange(0, 100)
        self.progressBar.setValue(0)

        QApplication.restoreOverrideCursor()

        self.buttonBox.rejected.disconnect(self.stopProcessing)
        self.buttonBox.rejected.connect(self.reject)

        self.okButton.setEnabled(True)

# ----------------------------------------------------------------------


def extractProjection(filename, createPrj):
    raster = gdal.Open(unicode(filename))

    crs = raster.GetProjection()
    geotransform = raster.GetGeoTransform()

    raster = None

    outFileName = os.path.splitext(unicode(filename))[0]

    # create prj file requested and if projection available
    if crs != "" and createPrj:
        # convert CRS into ESRI format
        tmp = osr.SpatialReference()
        tmp.ImportFromWkt(crs)
        tmp.MorphToESRI()
        crs = tmp.ExportToWkt()
        tmp = None

        prj = open(outFileName + '.prj', 'wt')
        prj.write(crs)
        prj.close()

    # create wld file
    wld = open(outFileName + '.wld', 'wt')
    wld.write("%0.8f\n" % geotransform[1])
    wld.write("%0.8f\n" % geotransform[4])
    wld.write("%0.8f\n" % geotransform[2])
    wld.write("%0.8f\n" % geotransform[5])
    wld.write("%0.8f\n" % (geotransform[0] + 0.5 * geotransform[1] + 0.5 * geotransform[2]))
    wld.write("%0.8f\n" % (geotransform[3] + 0.5 * geotransform[4] + 0.5 * geotransform[5]))
    wld.close()


class ExtractThread(QThread):

    def __init__(self, files, needPrj):
        QThread.__init__(self, QThread.currentThread())
        self.inFiles = files
        self.needPrj = needPrj

        self.mutex = QMutex()
        self.stopMe = 0

    def run(self):
        self.mutex.lock()
        self.stopMe = 0
        self.mutex.unlock()

        interrupted = False

        for f in self.inFiles:
            extractProjection(f, self.needPrj)
            self.fileProcessed.emit()

            self.mutex.lock()
            s = self.stopMe
            self.mutex.unlock()
            if s == 1:
                interrupted = True
                break

        if not interrupted:
            self.processFinished.emit()
        else:
            self.processIterrupted.emit()

    def stop(self):
        self.mutex.lock()
        self.stopMe = 1
        self.mutex.unlock()

        QThread.wait(self)
