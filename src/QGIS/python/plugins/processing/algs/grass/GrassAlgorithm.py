# -*- coding: utf-8 -*-

"""
***************************************************************************
    GrassAlgorithm.py
    ---------------------
    Date                 : August 2012
    Copyright            : (C) 2012 by Victor Olaya
    Email                : volayaf at gmail dot com
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
"""

__author__ = 'Victor Olaya'
__date__ = 'August 2012'
__copyright__ = '(C) 2012, Victor Olaya'

# This will get replaced with a git SHA1 when you do a git archive

__revision__ = '$Format:%H$'

import os
import time
import uuid
import importlib
import re

from qgis.PyQt.QtCore import QCoreApplication
from qgis.PyQt.QtGui import QIcon

from qgis.core import QgsRasterLayer
from qgis.utils import iface

from processing.core.GeoAlgorithm import GeoAlgorithm
from processing.core.ProcessingConfig import ProcessingConfig
from processing.core.ProcessingLog import ProcessingLog
from processing.core.GeoAlgorithmExecutionException import GeoAlgorithmExecutionException

from processing.core.parameters import (getParameterFromString,
                                        ParameterVector,
                                        ParameterMultipleInput,
                                        ParameterExtent,
                                        ParameterNumber,
                                        ParameterSelection,
                                        ParameterRaster,
                                        ParameterTable,
                                        ParameterBoolean,
                                        ParameterString,
                                        ParameterPoint)
from processing.core.outputs import (getOutputFromString,
                                     OutputRaster,
                                     OutputVector,
                                     OutputFile,
                                     OutputHTML)

from .GrassUtils import GrassUtils

from processing.tools import dataobjects, system

pluginPath = os.path.normpath(os.path.join(
    os.path.split(os.path.dirname(__file__))[0], os.pardir))


class GrassAlgorithm(GeoAlgorithm):

    GRASS_OUTPUT_TYPE_PARAMETER = 'GRASS_OUTPUT_TYPE_PARAMETER'
    GRASS_MIN_AREA_PARAMETER = 'GRASS_MIN_AREA_PARAMETER'
    GRASS_SNAP_TOLERANCE_PARAMETER = 'GRASS_SNAP_TOLERANCE_PARAMETER'
    GRASS_REGION_EXTENT_PARAMETER = 'GRASS_REGION_PARAMETER'
    GRASS_REGION_CELLSIZE_PARAMETER = 'GRASS_REGION_CELLSIZE_PARAMETER'
    GRASS_REGION_ALIGN_TO_RESOLUTION = '-a_r.region'

    OUTPUT_TYPES = ['auto', 'point', 'line', 'area']

    def __init__(self, descriptionfile):
        GeoAlgorithm.__init__(self)
        self.hardcodedStrings = []
        self.descriptionFile = descriptionfile
        self.defineCharacteristicsFromFile()
        self.numExportedLayers = 0

    def getCopy(self):
        newone = GrassAlgorithm(self.descriptionFile)
        newone.provider = self.provider
        return newone

    def getIcon(self):
        return QIcon(os.path.join(pluginPath, 'images', 'grass.svg'))

    def help(self):
        return False, 'http://grass.osgeo.org/grass64/manuals/' + self.grassName + '.html'

    def getParameterDescriptions(self):
        descs = {}
        _, helpfile = self.help()
        try:
            infile = open(helpfile)
            lines = infile.readlines()
            for i in range(len(lines)):
                if lines[i].startswith('<DT><b>'):
                    for param in self.parameters:
                        searchLine = '<b>' + param.name + '</b>'
                        if searchLine in lines[i]:
                            i += 1
                            descs[param.name] = (lines[i])[4:-6]
                            break

            infile.close()
        except Exception:
            pass
        return descs

    def defineCharacteristicsFromFile(self):
        lines = open(self.descriptionFile)
        line = lines.readline().strip('\n').strip()
        self.grassName = line
        line = lines.readline().strip('\n').strip()
        self.name = line
        self.i18n_name = QCoreApplication.translate("GrassAlgorithm", line)
        if " - " not in self.name:
            self.name = self.grassName + " - " + self.name
            self.i18n_name = self.grassName + " - " + self.i18n_name
        line = lines.readline().strip('\n').strip()
        self.group = line
        self.i18n_group = QCoreApplication.translate("GrassAlgorithm", line)
        hasRasterOutput = False
        hasVectorInput = False
        vectorOutputs = 0
        line = lines.readline().strip('\n').strip()
        while line != '':
            try:
                line = line.strip('\n').strip()
                if line.startswith('Hardcoded'):
                    self.hardcodedStrings.append(line[len('Hardcoded|'):])
                elif line.startswith('Parameter'):
                    parameter = getParameterFromString(line)
                    self.addParameter(parameter)
                    if isinstance(parameter, ParameterVector):
                        hasVectorInput = True
                    if isinstance(parameter, ParameterMultipleInput) \
                       and parameter.datatype < 3:
                        hasVectorInput = True
                elif line.startswith('*Parameter'):
                    param = getParameterFromString(line[1:])
                    param.isAdvanced = True
                    self.addParameter(param)
                else:
                    output = getOutputFromString(line)
                    self.addOutput(output)
                    if isinstance(output, OutputRaster):
                        hasRasterOutput = True
                    elif isinstance(output, OutputVector):
                        vectorOutputs += 1
                    if isinstance(output, OutputHTML):
                        self.addOutput(OutputFile("rawoutput", output.description +
                                                  " (raw output)", "txt"))
                line = lines.readline().strip('\n').strip()
            except Exception as e:
                ProcessingLog.addToLog(
                    ProcessingLog.LOG_ERROR,
                    self.tr('Could not open GRASS algorithm: %s.\n%s' % (self.descriptionFile, line)))
                raise e
        lines.close()

        self.addParameter(ParameterExtent(
            self.GRASS_REGION_EXTENT_PARAMETER,
            self.tr('GRASS region extent'))
        )
        if hasRasterOutput:
            self.addParameter(ParameterNumber(
                self.GRASS_REGION_CELLSIZE_PARAMETER,
                self.tr('GRASS region cellsize (leave 0 for default)'),
                0, None, 0.0))
        if hasVectorInput:
            param = ParameterNumber(self.GRASS_SNAP_TOLERANCE_PARAMETER,
                                    'v.in.ogr snap tolerance (-1 = no snap)',
                                    -1, None, -1.0)
            param.isAdvanced = True
            self.addParameter(param)
            param = ParameterNumber(self.GRASS_MIN_AREA_PARAMETER,
                                    'v.in.ogr min area', 0, None, 0.0001)
            param.isAdvanced = True
            self.addParameter(param)
        if vectorOutputs == 1:
            param = ParameterSelection(self.GRASS_OUTPUT_TYPE_PARAMETER,
                                       'v.out.ogr output type',
                                       self.OUTPUT_TYPES)
            param.isAdvanced = True
            self.addParameter(param)

    def getDefaultCellsize(self):
        cellsize = 0
        for param in self.parameters:
            if param.value:
                if isinstance(param, ParameterRaster):
                    if isinstance(param.value, QgsRasterLayer):
                        layer = param.value
                    else:
                        layer = dataobjects.getObjectFromUri(param.value)
                    cellsize = max(cellsize, (layer.extent().xMaximum()
                                              - layer.extent().xMinimum())
                                   / layer.width())
                elif isinstance(param, ParameterMultipleInput):

                    layers = param.value.split(';')
                    for layername in layers:
                        layer = dataobjects.getObjectFromUri(layername)
                        if isinstance(layer, QgsRasterLayer):
                            cellsize = max(cellsize, (
                                layer.extent().xMaximum()
                                - layer.extent().xMinimum())
                                / layer.width()
                            )

        if cellsize == 0:
            cellsize = 100
        return cellsize

    def processAlgorithm(self, progress):
        if system.isWindows():
            path = GrassUtils.grassPath()
            if path == '':
                raise GeoAlgorithmExecutionException(
                    self.tr('GRASS folder is not configured.\nPlease '
                            'configure it before running GRASS algorithms.'))

        commands = []
        self.exportedLayers = {}
        outputCommands = []

        # If GRASS session has been created outside of this algorithm then
        # get the list of layers loaded in GRASS otherwise start a new
        # session
        existingSession = GrassUtils.sessionRunning
        if existingSession:
            self.exportedLayers = GrassUtils.getSessionLayers()
        else:
            GrassUtils.startGrassSession()

        # 1: Export layer to grass mapset

        for param in self.parameters:
            if isinstance(param, ParameterRaster):
                if param.value is None:
                    continue
                value = param.value

                # Check if the layer hasn't already been exported in, for
                # example, previous GRASS calls in this session
                if value in self.exportedLayers.keys():
                    continue
                else:
                    self.setSessionProjectionFromLayer(value, commands)
                    commands.append(self.exportRasterLayer(value))
            if isinstance(param, ParameterVector):
                if param.value is None:
                    continue
                value = param.value
                if value in self.exportedLayers.keys():
                    continue
                else:
                    self.setSessionProjectionFromLayer(value, commands)
                    commands.append(self.exportVectorLayer(value))
            if isinstance(param, ParameterTable):
                pass
            if isinstance(param, ParameterMultipleInput):
                if param.value is None:
                    continue
                layers = param.value.split(';')
                if layers is None or len(layers) == 0:
                    continue
                if param.datatype == ParameterMultipleInput.TYPE_RASTER:
                    for layer in layers:
                        if layer in self.exportedLayers.keys():
                            continue
                        else:
                            self.setSessionProjectionFromLayer(layer, commands)
                            commands.append(self.exportRasterLayer(layer))
                elif param.datatype in [ParameterMultipleInput.TYPE_VECTOR_ANY,
                                        ParameterMultipleInput.TYPE_VECTOR_LINE,
                                        ParameterMultipleInput.TYPE_VECTOR_POLYGON,
                                        ParameterMultipleInput.TYPE_VECTOR_POINT]:
                    for layer in layers:
                        if layer in self.exportedLayers.keys():
                            continue
                        else:
                            self.setSessionProjectionFromLayer(layer, commands)
                            commands.append(self.exportVectorLayer(layer))

        self.setSessionProjectionFromProject(commands)

        region = \
            unicode(self.getParameterValue(self.GRASS_REGION_EXTENT_PARAMETER))
        regionCoords = region.split(',')
        command = 'g.region'
        command += ' n=' + unicode(regionCoords[3])
        command += ' s=' + unicode(regionCoords[2])
        command += ' e=' + unicode(regionCoords[1])
        command += ' w=' + unicode(regionCoords[0])
        cellsize = self.getParameterValue(self.GRASS_REGION_CELLSIZE_PARAMETER)
        if cellsize:
            command += ' res=' + unicode(cellsize)
        else:
            command += ' res=' + unicode(self.getDefaultCellsize())
        alignToResolution = \
            self.getParameterValue(self.GRASS_REGION_ALIGN_TO_RESOLUTION)
        if alignToResolution:
            command += ' -a'
        commands.append(command)

        # 2: Set parameters and outputs

        command = self.grassName
        command += ' ' + ' '.join(self.hardcodedStrings)

        for param in self.parameters:
            if param.value is None or param.value == '':
                continue
            if param.name in [self.GRASS_REGION_CELLSIZE_PARAMETER, self.GRASS_REGION_EXTENT_PARAMETER,
                              self.GRASS_MIN_AREA_PARAMETER, self.GRASS_SNAP_TOLERANCE_PARAMETER,
                              self.GRASS_OUTPUT_TYPE_PARAMETER, self.GRASS_REGION_ALIGN_TO_RESOLUTION]:
                continue
            if isinstance(param, (ParameterRaster, ParameterVector)):
                value = param.value
                if value in self.exportedLayers.keys():
                    command += ' %s="%s"' % (param.name, self.exportedLayers[value])
                else:
                    command += ' %s="%s"' % (param.name, value)
            elif isinstance(param, ParameterMultipleInput):
                s = param.value
                for layer in self.exportedLayers.keys():
                    s = s.replace(layer, self.exportedLayers[layer])
                s = s.replace(';', ',')
                command += ' %s="%s"' % (param.name, s)
            elif isinstance(param, ParameterBoolean):
                if param.value:
                    command += ' ' + param.name
            elif isinstance(param, ParameterSelection):
                idx = int(param.value)
                command += ' ' + param.name + '=' + unicode(param.options[idx])
            elif isinstance(param, ParameterString):
                command += ' ' + param.name + '="' + unicode(param.value) + '"'
            elif isinstance(param, ParameterPoint):
                command += ' ' + param.name + '=' + unicode(param.value)
            else:
                command += ' ' + param.name + '="' + unicode(param.value) + '"'

        uniqueSufix = unicode(uuid.uuid4()).replace('-', '')
        for out in self.outputs:
            if isinstance(out, OutputFile):
                command += ' > ' + out.value
            elif not isinstance(out, OutputHTML):
                # We add an output name to make sure it is unique if the session
                # uses this algorithm several times.
                uniqueOutputName = out.name + uniqueSufix
                command += ' ' + out.name + '=' + uniqueOutputName

                # Add output file to exported layers, to indicate that
                # they are present in GRASS
                self.exportedLayers[out.value] = uniqueOutputName

        command += ' --overwrite'
        commands.append(command)

        # 3: Export resulting layers to a format that qgis can read

        for out in self.outputs:
            if isinstance(out, OutputRaster):
                filename = out.getCompatibleFileName(self)

                # Raster layer output: adjust region to layer before
                # exporting
                commands.append('g.region rast=' + out.name + uniqueSufix)
                outputCommands.append('g.region rast=' + out.name
                                      + uniqueSufix)
                if self.grassName == 'r.composite':
                    command = 'r.out.tiff -t --verbose'
                    command += ' input='
                    command += out.name + uniqueSufix
                    command += ' output="' + filename + '"'
                    commands.append(command)
                    outputCommands.append(command)
                else:
                    command = 'r.out.gdal -c createopt="TFW=YES,COMPRESS=LZW"'
                    command += ' input='

                if self.grassName == 'r.horizon':
                    command += out.name + uniqueSufix + '_0'
                else:
                    command += out.name + uniqueSufix
                    command += ' output="' + filename + '"'
                    commands.append(command)
                    outputCommands.append(command)

            if isinstance(out, OutputVector):
                filename = out.getCompatibleFileName(self)
                command = 'v.out.ogr -s -c -e -z input=' + out.name + uniqueSufix
                command += ' dsn="' + os.path.dirname(filename) + '"'
                command += ' format=ESRI_Shapefile'
                command += ' olayer="%s"' % os.path.splitext(os.path.basename(filename))[0]
                typeidx = \
                    self.getParameterValue(self.GRASS_OUTPUT_TYPE_PARAMETER)
                outtype = ('auto' if typeidx
                           is None else self.OUTPUT_TYPES[typeidx])
                command += ' type=' + outtype
                commands.append(command)
                outputCommands.append(command)

        # 4: Run GRASS

        loglines = []
        loglines.append(self.tr('GRASS execution commands'))
        for line in commands:
            progress.setCommand(line)
            loglines.append(line)
        if ProcessingConfig.getSetting(GrassUtils.GRASS_LOG_COMMANDS):
            ProcessingLog.addToLog(ProcessingLog.LOG_INFO, loglines)
        GrassUtils.executeGrass(commands, progress, outputCommands)

        for out in self.outputs:
            if isinstance(out, OutputHTML):
                with open(self.getOutputFromName("rawoutput").value) as f:
                    rawOutput = "".join(f.readlines())
                with open(out.value, "w") as f:
                    f.write("<pre>%s</pre>" % rawOutput)

        # If the session has been created outside of this algorithm, add
        # the new GRASS layers to it otherwise finish the session
        if existingSession:
            GrassUtils.addSessionLayers(self.exportedLayers)
        else:
            GrassUtils.endGrassSession()

    def exportVectorLayer(self, orgFilename):

        # TODO: improve this. We are now exporting if it is not a shapefile,
        # but the functionality of v.in.ogr could be used for this.
        # We also export if there is a selection
        if not os.path.exists(orgFilename) or not orgFilename.endswith('shp'):
            layer = dataobjects.getObjectFromUri(orgFilename, False)
            if layer:
                filename = dataobjects.exportVectorLayer(layer)
        else:
            layer = dataobjects.getObjectFromUri(orgFilename, False)
            if layer:
                useSelection = \
                    ProcessingConfig.getSetting(ProcessingConfig.USE_SELECTED)
                if useSelection and layer.selectedFeatureCount() != 0:
                    filename = dataobjects.exportVectorLayer(layer)
                else:
                    filename = orgFilename
            else:
                filename = orgFilename
        destFilename = self.getTempFilename()
        self.exportedLayers[orgFilename] = destFilename
        command = 'v.in.ogr'
        min_area = self.getParameterValue(self.GRASS_MIN_AREA_PARAMETER)
        command += ' min_area=' + unicode(min_area)
        snap = self.getParameterValue(self.GRASS_SNAP_TOLERANCE_PARAMETER)
        command += ' snap=' + unicode(snap)
        command += ' dsn="%s"' % os.path.dirname(filename)
        command += ' layer="%s"' % os.path.basename(filename)[:-4]
        command += ' output=' + destFilename
        command += ' --overwrite -o'
        return command

    def setSessionProjectionFromProject(self, commands):
        if not GrassUtils.projectionSet:
            proj4 = iface.mapCanvas().mapSettings().destinationCrs().toProj4()
            command = 'g.proj'
            command += ' -c'
            command += ' proj4="' + proj4 + '"'
            commands.append(command)
            GrassUtils.projectionSet = True

    def setSessionProjectionFromLayer(self, layer, commands):
        if not GrassUtils.projectionSet:
            qGisLayer = dataobjects.getObjectFromUri(layer)
            if qGisLayer:
                proj4 = unicode(qGisLayer.crs().toProj4())
                command = 'g.proj'
                command += ' -c'
                command += ' proj4="' + proj4 + '"'
                commands.append(command)
                GrassUtils.projectionSet = True

    def exportRasterLayer(self, layer):
        destFilename = self.getTempFilename()
        self.exportedLayers[layer] = destFilename
        if bool(re.match('netcdf', layer, re.I)) or bool(re.match('hdf', layer, re.I)):
            command = 'r.in.gdal'
        else:
            command = 'r.external -r'
        command += ' input="' + layer + '"'
        command += ' band=1'
        command += ' output=' + destFilename
        command += ' --overwrite -o'
        return command

    def getTempFilename(self):
        filename = 'tmp' + unicode(time.time()).replace('.', '') \
            + unicode(system.getNumExportedLayers())
        return filename

    def commandLineName(self):
        return 'grass:' + self.name[:self.name.find(' ')]

    def checkBeforeOpeningParametersDialog(self):
        return GrassUtils.checkGrassIsInstalled()

    def checkParameterValuesBeforeExecuting(self):
        name = self.commandLineName().replace('.', '_')[len('grass:'):]
        try:
            module = importlib.import_module('processing.algs.grass.ext.' + name)
        except ImportError:
            return
        if hasattr(module, 'checkParameterValuesBeforeExecuting'):
            func = getattr(module, 'checkParameterValuesBeforeExecuting')
            return func(self)
