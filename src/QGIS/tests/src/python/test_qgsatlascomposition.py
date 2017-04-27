# -*- coding: utf-8 -*-
'''
test_qgsatlascomposition.py
                     --------------------------------------
               Date                 : Oct 2012
               Copyright            : (C) 2012 by Dr. Hugo Mercier
               email                : hugo dot mercier at oslandia dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
'''
import qgis  # NOQA
import os
import glob
import shutil
import tempfile
from qgis.testing import start_app, unittest
from utilities import unitTestDataPath
from qgis.PyQt.QtCore import QFileInfo, QRectF, qWarning
from qgis.core import QGis, QgsVectorLayer, QgsMapLayerRegistry, QgsMapSettings, QgsCoordinateReferenceSystem, \
    QgsComposition, QgsFillSymbolV2, QgsSingleSymbolRendererV2, QgsComposerLabel, QgsComposerMap, QgsFontUtils, \
    QgsRectangle, QgsComposerLegend, QgsFeature, QgsGeometry, QgsPoint, QgsRendererCategoryV2, QgsCategorizedSymbolRendererV2, QgsMarkerSymbolV2
from qgscompositionchecker import QgsCompositionChecker

start_app()


class TestQgsAtlasComposition(unittest.TestCase):

    def testCase(self):
        self.TEST_DATA_DIR = unitTestDataPath()
        tmppath = tempfile.mkdtemp()
        for file in glob.glob(os.path.join(self.TEST_DATA_DIR, 'france_parts.*')):
            shutil.copy(os.path.join(self.TEST_DATA_DIR, file), tmppath)
        vectorFileInfo = QFileInfo(tmppath + "/france_parts.shp")
        mVectorLayer = QgsVectorLayer(vectorFileInfo.filePath(), vectorFileInfo.completeBaseName(), "ogr")

        QgsMapLayerRegistry.instance().addMapLayers([mVectorLayer])

        # create composition with composer map
        self.mapSettings = QgsMapSettings()
        layerStringList = []
        layerStringList.append(mVectorLayer.id())
        self.mapSettings.setLayers(layerStringList)
        self.mapSettings.setCrsTransformEnabled(True)
        self.mapSettings.setMapUnits(QGis.Meters)

        # select epsg:2154
        crs = QgsCoordinateReferenceSystem()
        crs.createFromSrid(2154)
        self.mapSettings.setDestinationCrs(crs)

        self.mComposition = QgsComposition(self.mapSettings)
        self.mComposition.setPaperSize(297, 210)

        # fix the renderer, fill with green
        props = {"color": "0,127,0"}
        fillSymbol = QgsFillSymbolV2.createSimple(props)
        renderer = QgsSingleSymbolRendererV2(fillSymbol)
        mVectorLayer.setRendererV2(renderer)

        # the atlas map
        self.mAtlasMap = QgsComposerMap(self.mComposition, 20, 20, 130, 130)
        self.mAtlasMap.setFrameEnabled(True)
        self.mComposition.addComposerMap(self.mAtlasMap)

        # the atlas
        self.mAtlas = self.mComposition.atlasComposition()
        self.mAtlas.setCoverageLayer(mVectorLayer)
        self.mAtlas.setEnabled(True)
        self.mComposition.setAtlasMode(QgsComposition.ExportAtlas)

        # an overview
        mOverview = QgsComposerMap(self.mComposition, 180, 20, 50, 50)
        mOverview.setFrameEnabled(True)
        mOverview.setOverviewFrameMap(self.mAtlasMap.id())
        self.mComposition.addComposerMap(mOverview)
        nextent = QgsRectangle(49670.718, 6415139.086, 699672.519, 7065140.887)
        mOverview.setNewExtent(nextent)

        # set the fill symbol of the overview map
        props2 = {"color": "127,0,0,127"}
        fillSymbol2 = QgsFillSymbolV2.createSimple(props2)
        mOverview.setOverviewFrameMapSymbol(fillSymbol2)

        # header label
        self.mLabel1 = QgsComposerLabel(self.mComposition)
        self.mComposition.addComposerLabel(self.mLabel1)
        self.mLabel1.setText("[% \"NAME_1\" %] area")
        self.mLabel1.setFont(QgsFontUtils.getStandardTestFont())
        self.mLabel1.adjustSizeToText()
        self.mLabel1.setSceneRect(QRectF(150, 5, 60, 15))

        qWarning(
            "header label font: %s exactMatch:%s" % (self.mLabel1.font().toString(), self.mLabel1.font().exactMatch()))

        # feature number label
        self.mLabel2 = QgsComposerLabel(self.mComposition)
        self.mComposition.addComposerLabel(self.mLabel2)
        self.mLabel2.setText("# [%$feature || ' / ' || $numfeatures%]")
        self.mLabel2.setFont(QgsFontUtils.getStandardTestFont())
        self.mLabel2.adjustSizeToText()
        self.mLabel2.setSceneRect(QRectF(150, 200, 60, 15))

        qWarning("feature number label font: %s exactMatch:%s" % (
                 self.mLabel2.font().toString(), self.mLabel2.font().exactMatch()))

        self.filename_test()
        self.autoscale_render_test()
        self.autoscale_render_test_old_api()
        self.fixedscale_render_test()
        self.predefinedscales_render_test()
        self.hidden_render_test()
        self.legend_test()

        shutil.rmtree(tmppath, True)

    def filename_test(self):
        self.mAtlas.setFilenamePattern("'output_' || $feature")
        self.mAtlas.beginRender()
        for i in range(0, self.mAtlas.numFeatures()):
            self.mAtlas.prepareForFeature(i)
            expected = "output_%d" % (i + 1)
            assert self.mAtlas.currentFilename() == expected
        self.mAtlas.endRender()

    def autoscale_render_test(self):
        self.mAtlasMap.setAtlasDriven(True)
        self.mAtlasMap.setAtlasScalingMode(QgsComposerMap.Auto)
        self.mAtlasMap.setAtlasMargin(0.10)

        self.mAtlas.beginRender()

        for i in range(0, 2):
            self.mAtlas.prepareForFeature(i)
            self.mLabel1.adjustSizeToText()

            checker = QgsCompositionChecker('atlas_autoscale%d' % (i + 1), self.mComposition)
            checker.setControlPathPrefix("atlas")
            myTestResult, myMessage = checker.testComposition(0, 200)

            assert myTestResult
        self.mAtlas.endRender()

        self.mAtlasMap.setAtlasDriven(False)
        self.mAtlasMap.setAtlasScalingMode(QgsComposerMap.Fixed)
        self.mAtlasMap.setAtlasMargin(0)

    def autoscale_render_test_old_api(self):
        self.mAtlas.setComposerMap(self.mAtlasMap)
        self.mAtlas.setFixedScale(False)
        self.mAtlas.setMargin(0.10)

        self.mAtlas.beginRender()

        for i in range(0, 2):
            self.mAtlas.prepareForFeature(i)
            self.mLabel1.adjustSizeToText()

            checker = QgsCompositionChecker('atlas_autoscale_old_api%d' % (i + 1), self.mComposition)
            checker.setControlPathPrefix("atlas")
            myTestResult, myMessage = checker.testComposition(0, 200)

            assert myTestResult
        self.mAtlas.endRender()

        self.mAtlas.setFixedScale(True)
        self.mAtlas.setMargin(0)
        self.mAtlas.setComposerMap(None)
        self.mAtlasMap.setAtlasDriven(False)

    def fixedscale_render_test(self):
        self.mAtlasMap.setNewExtent(QgsRectangle(209838.166, 6528781.020, 610491.166, 6920530.620))
        self.mAtlasMap.setAtlasDriven(True)
        self.mAtlasMap.setAtlasScalingMode(QgsComposerMap.Fixed)

        self.mAtlas.beginRender()

        for i in range(0, 2):
            self.mAtlas.prepareForFeature(i)
            self.mLabel1.adjustSizeToText()

            checker = QgsCompositionChecker('atlas_fixedscale%d' % (i + 1), self.mComposition)
            checker.setControlPathPrefix("atlas")
            myTestResult, myMessage = checker.testComposition(0, 200)

            assert myTestResult
        self.mAtlas.endRender()

    def predefinedscales_render_test(self):
        self.mAtlasMap.setNewExtent(QgsRectangle(209838.166, 6528781.020, 610491.166, 6920530.620))
        self.mAtlasMap.setAtlasDriven(True)
        self.mAtlasMap.setAtlasScalingMode(QgsComposerMap.Predefined)

        scales = [1800000, 5000000]
        self.mAtlas.setPredefinedScales(scales)
        for i, s in enumerate(self.mAtlas.predefinedScales()):
            assert s == scales[i]

        self.mAtlas.beginRender()

        for i in range(0, 2):
            self.mAtlas.prepareForFeature(i)
            self.mLabel1.adjustSizeToText()

            checker = QgsCompositionChecker('atlas_predefinedscales%d' % (i + 1), self.mComposition)
            checker.setControlPathPrefix("atlas")
            myTestResult, myMessage = checker.testComposition(0, 200)

            assert myTestResult
        self.mAtlas.endRender()

    def hidden_render_test(self):
        self.mAtlasMap.setNewExtent(QgsRectangle(209838.166, 6528781.020, 610491.166, 6920530.620))
        self.mAtlasMap.setAtlasScalingMode(QgsComposerMap.Fixed)
        self.mAtlas.setHideCoverage(True)

        self.mAtlas.beginRender()

        for i in range(0, 2):
            self.mAtlas.prepareForFeature(i)
            self.mLabel1.adjustSizeToText()

            checker = QgsCompositionChecker('atlas_hiding%d' % (i + 1), self.mComposition)
            checker.setControlPathPrefix("atlas")
            myTestResult, myMessage = checker.testComposition(0, 200)

            assert myTestResult
        self.mAtlas.endRender()

        self.mAtlas.setHideCoverage(False)

    def sorting_render_test(self):
        self.mAtlasMap.setNewExtent(QgsRectangle(209838.166, 6528781.020, 610491.166, 6920530.620))
        self.mAtlasMap.setAtlasScalingMode(QgsComposerMap.Fixed)
        self.mAtlas.setHideCoverage(False)

        self.mAtlas.setSortFeatures(True)
        self.mAtlas.setSortKeyAttributeIndex(4)  # departement name
        self.mAtlas.setSortAscending(False)

        self.mAtlas.beginRender()

        for i in range(0, 2):
            self.mAtlas.prepareForFeature(i)
            self.mLabel1.adjustSizeToText()

            checker = QgsCompositionChecker('atlas_sorting%d' % (i + 1), self.mComposition)
            checker.setControlPathPrefix("atlas")
            myTestResult, myMessage = checker.testComposition(0, 200)

            assert myTestResult
        self.mAtlas.endRender()

    def filtering_render_test(self):
        self.mAtlasMap.setNewExtent(QgsRectangle(209838.166, 6528781.020, 610491.166, 6920530.620))
        self.mAtlasMap.setAtlasScalingMode(QgsComposerMap.Fixed)
        self.mAtlas.setHideCoverage(False)

        self.mAtlas.setSortFeatures(False)

        self.mAtlas.setFilterFeatures(True)
        self.mAtlas.setFeatureFilter("substr(NAME_1,1,1)='P'")  # select only 'Pays de la loire'

        self.mAtlas.beginRender()

        for i in range(0, 1):
            self.mAtlas.prepareForFeature(i)
            self.mLabel1.adjustSizeToText()

            checker = QgsCompositionChecker('atlas_filtering%d' % (i + 1), self.mComposition)
            checker.setControlPathPrefix("atlas")
            myTestResult, myMessage = checker.testComposition(0, 200)

            assert myTestResult
        self.mAtlas.endRender()

    def legend_test(self):
        self.mAtlasMap.setAtlasDriven(True)
        self.mAtlasMap.setAtlasScalingMode(QgsComposerMap.Auto)
        self.mAtlasMap.setAtlasMargin(0.10)

        # add a point layer
        ptLayer = QgsVectorLayer("Point?crs=epsg:4326&field=attr:int(1)&field=label:string(20)", "points", "memory")

        pr = ptLayer.dataProvider()
        f1 = QgsFeature(1)
        f1.initAttributes(2)
        f1.setAttribute(0, 1)
        f1.setAttribute(1, "Test label 1")
        f1.setGeometry(QgsGeometry.fromPoint(QgsPoint(-0.638, 48.954)))
        f2 = QgsFeature(2)
        f2.initAttributes(2)
        f2.setAttribute(0, 2)
        f2.setAttribute(1, "Test label 2")
        f2.setGeometry(QgsGeometry.fromPoint(QgsPoint(-1.682, 48.550)))
        pr.addFeatures([f1, f2])

        # categorized symbology
        r = QgsCategorizedSymbolRendererV2("attr", [QgsRendererCategoryV2(1, QgsMarkerSymbolV2.createSimple({"color": "255,0,0"}), "red"),
                                                    QgsRendererCategoryV2(2, QgsMarkerSymbolV2.createSimple({"color": "0,0,255"}), "blue")])
        ptLayer.setRendererV2(r)

        QgsMapLayerRegistry.instance().addMapLayer(ptLayer)

        # add the point layer to the map settings
        layers = self.mapSettings.layers()
        layers = [ptLayer.id()] + layers
        self.mapSettings.setLayers(layers)

        # add a legend
        legend = QgsComposerLegend(self.mComposition)
        legend.moveBy(200, 100)
        # sets the legend filter parameter
        legend.setComposerMap(self.mAtlasMap)
        legend.setLegendFilterOutAtlas(True)
        self.mComposition.addComposerLegend(legend)

        self.mAtlas.beginRender()

        self.mAtlas.prepareForFeature(0)
        self.mLabel1.adjustSizeToText()

        checker = QgsCompositionChecker('atlas_legend', self.mComposition)
        myTestResult, myMessage = checker.testComposition()
        assert myTestResult

        self.mAtlas.endRender()

        # restore state
        self.mapSettings.setLayers([layers[1]])
        self.mComposition.removeComposerItem(legend)
        QgsMapLayerRegistry.instance().removeMapLayer(ptLayer.id())


if __name__ == '__main__':
    unittest.main()
