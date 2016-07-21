# -*- coding: utf-8 -*-

"""
***************************************************************************
    ModelerDialog.py
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

import codecs
import sys
import os

from qgis.PyQt import uic
from qgis.PyQt.QtCore import Qt, QRectF, QMimeData, QPoint, QPointF, QSettings, QByteArray
from qgis.PyQt.QtWidgets import QGraphicsView, QTreeWidget, QMessageBox, QFileDialog, QTreeWidgetItem
from qgis.PyQt.QtGui import QIcon, QImage, QPainter
from qgis.core import QgsApplication
from processing.core.ProcessingConfig import ProcessingConfig
from processing.core.ProcessingLog import ProcessingLog
from processing.gui.HelpEditionDialog import HelpEditionDialog
from processing.gui.AlgorithmDialog import AlgorithmDialog
from processing.modeler.ModelerParameterDefinitionDialog import ModelerParameterDefinitionDialog
from processing.modeler.ModelerAlgorithm import ModelerAlgorithm, ModelerParameter
from processing.modeler.ModelerParametersDialog import ModelerParametersDialog
from processing.modeler.ModelerUtils import ModelerUtils
from processing.modeler.ModelerScene import ModelerScene
from processing.modeler.WrongModelException import WrongModelException
from processing.core.alglist import algList

pluginPath = os.path.split(os.path.dirname(__file__))[0]
WIDGET, BASE = uic.loadUiType(
    os.path.join(pluginPath, 'ui', 'DlgModeler.ui'))


class ModelerDialog(BASE, WIDGET):

    CANVAS_SIZE = 4000

    def __init__(self, alg=None):
        super(ModelerDialog, self).__init__(None)
        self.setupUi(self)

        self.zoom = 1

        self.setWindowFlags(Qt.WindowMinimizeButtonHint |
                            Qt.WindowMaximizeButtonHint |
                            Qt.WindowCloseButtonHint)

        settings = QSettings()
        self.splitter.restoreState(settings.value("/Processing/splitterModeler", QByteArray()))
        self.restoreGeometry(settings.value("/Processing/geometryModeler", QByteArray()))

        self.tabWidget.setCurrentIndex(0)
        self.scene = ModelerScene(self)
        self.scene.setSceneRect(QRectF(0, 0, self.CANVAS_SIZE, self.CANVAS_SIZE))

        self.view.setScene(self.scene)
        self.view.setAcceptDrops(True)
        self.view.ensureVisible(0, 0, 10, 10)

        def _dragEnterEvent(event):
            if event.mimeData().hasText():
                event.acceptProposedAction()
            else:
                event.ignore()

        def _dropEvent(event):
            if event.mimeData().hasText():
                text = event.mimeData().text()
                if text in ModelerParameterDefinitionDialog.paramTypes:
                    self.addInputOfType(text, event.pos())
                else:
                    alg = algList.getAlgorithm(text)
                    if alg is not None:
                        self._addAlgorithm(alg.getCopy(), event.pos())
                event.accept()
            else:
                event.ignore()

        def _dragMoveEvent(event):
            if event.mimeData().hasText():
                event.accept()
            else:
                event.ignore()

        def _wheelEvent(event):
            self.view.setTransformationAnchor(QGraphicsView.AnchorUnderMouse)
            factor = 1.05
            if event.delta() > 0:
                factor = 1 / factor
            self.view.scale(factor, factor)
            self.repaintModel()

        def _enterEvent(e):
            QGraphicsView.enterEvent(self.view, e)
            self.view.viewport().setCursor(Qt.ArrowCursor)

        def _mousePressEvent(e):
            QGraphicsView.mousePressEvent(self.view, e)
            self.view.viewport().setCursor(Qt.ArrowCursor)

        def _mouseReleaseEvent(e):
            QGraphicsView.mouseReleaseEvent(self.view, e)
            self.view.viewport().setCursor(Qt.ArrowCursor)

        self.view.setDragMode(QGraphicsView.ScrollHandDrag)
        self.view.dragEnterEvent = _dragEnterEvent
        self.view.dropEvent = _dropEvent
        self.view.dragMoveEvent = _dragMoveEvent
        self.view.wheelEvent = _wheelEvent
        self.view.enterEvent = _enterEvent
        self.view.mousePressEvent = _mousePressEvent
        self.view.mouseReleaseEvent = _mouseReleaseEvent

        def _mimeDataInput(items):
            mimeData = QMimeData()
            text = items[0].text(0)
            mimeData.setText(text)
            return mimeData

        self.inputsTree.mimeData = _mimeDataInput

        self.inputsTree.setDragDropMode(QTreeWidget.DragOnly)
        self.inputsTree.setDropIndicatorShown(True)

        def _mimeDataAlgorithm(items):
            item = items[0]
            if isinstance(item, TreeAlgorithmItem):
                mimeData = QMimeData()
                mimeData.setText(item.alg.commandLineName())
            return mimeData

        self.algorithmTree.mimeData = _mimeDataAlgorithm

        self.algorithmTree.setDragDropMode(QTreeWidget.DragOnly)
        self.algorithmTree.setDropIndicatorShown(True)

        # Set icons
        self.btnOpen.setIcon(QgsApplication.getThemeIcon('/mActionFileOpen.svg'))
        self.btnSave.setIcon(QgsApplication.getThemeIcon('/mActionFileSave.svg'))
        self.btnSaveAs.setIcon(QgsApplication.getThemeIcon('/mActionFileSaveAs.svg'))
        self.btnExportImage.setIcon(QgsApplication.getThemeIcon('/mActionSaveMapAsImage.png'))
        self.btnExportPython.setIcon(QgsApplication.getThemeIcon('/console/iconSaveAsConsole.png'))
        self.btnEditHelp.setIcon(QIcon(os.path.join(pluginPath, 'images', 'edithelp.png')))
        self.btnRun.setIcon(QIcon(os.path.join(pluginPath, 'images', 'runalgorithm.png')))

        if hasattr(self.searchBox, 'setPlaceholderText'):
            self.searchBox.setPlaceholderText(self.tr('Search...'))
        if hasattr(self.textName, 'setPlaceholderText'):
            self.textName.setPlaceholderText(self.tr('[Enter model name here]'))
        if hasattr(self.textGroup, 'setPlaceholderText'):
            self.textGroup.setPlaceholderText(self.tr('[Enter group name here]'))

        # Connect signals and slots
        self.inputsTree.doubleClicked.connect(self.addInput)
        self.searchBox.textChanged.connect(self.fillAlgorithmTree)
        self.algorithmTree.doubleClicked.connect(self.addAlgorithm)

        self.btnOpen.clicked.connect(self.openModel)
        self.btnSave.clicked.connect(self.save)
        self.btnSaveAs.clicked.connect(self.saveAs)
        self.btnExportImage.clicked.connect(self.exportAsImage)
        self.btnExportPython.clicked.connect(self.exportAsPython)
        self.btnEditHelp.clicked.connect(self.editHelp)
        self.btnRun.clicked.connect(self.runModel)

        if alg is not None:
            self.alg = alg
            self.textGroup.setText(alg.group)
            self.textName.setText(alg.name)
            self.repaintModel()

        else:
            self.alg = ModelerAlgorithm()
            self.alg.modelerdialog = self

        self.fillInputsTree()
        self.fillAlgorithmTree()

        self.view.centerOn(0, 0)
        self.alg.setModelerView(self)
        self.help = None
        # Indicates whether to update or not the toolbox after
        # closing this dialog
        self.update = False

        self.hasChanged = False

    def closeEvent(self, evt):
        settings = QSettings()
        settings.setValue("/Processing/splitterModeler", self.splitter.saveState())
        settings.setValue("/Processing/geometryModeler", self.saveGeometry())

        if self.hasChanged:
            ret = QMessageBox.question(
                self, self.tr('Unsaved changes'),
                self.tr('There are unsaved changes in model. Continue?'),
                QMessageBox.Yes | QMessageBox.No, QMessageBox.No)

            if ret == QMessageBox.Yes:
                evt.accept()
            else:
                evt.ignore()
        else:
            evt.accept()

    def editHelp(self):
        if self.alg.provider is None:
            # Might happen if model is opened from modeler dialog
            self.alg.provider = algList.getProviderFromName('model')
        alg = self.alg.getCopy()
        dlg = HelpEditionDialog(alg)
        dlg.exec_()
        if dlg.descriptions:
            self.alg.helpContent = dlg.descriptions
            self.hasChanged = True

    def runModel(self):
        if len(self.alg.algs) == 0:
            QMessageBox.warning(self, self.tr('Empty model'),
                                self.tr("Model doesn't contains any algorithms and/or "
                                        "parameters and can't be executed"))
            return

        if self.alg.provider is None:
            # Might happen if model is opened from modeler dialog
            self.alg.provider = algList.getProviderFromName('model')
        alg = self.alg.getCopy()
        dlg = AlgorithmDialog(alg)
        dlg.exec_()

    def save(self):
        self.saveModel(False)

    def saveAs(self):
        self.saveModel(True)

    def exportAsImage(self):
        filename = unicode(QFileDialog.getSaveFileName(self,
                                                       self.tr('Save Model As Image'), '',
                                                       self.tr('PNG files (*.png *.PNG)')))
        if not filename:
            return

        if not filename.lower().endswith('.png'):
            filename += '.png'

        totalRect = QRectF(0, 0, 1, 1)
        for item in self.scene.items():
            totalRect = totalRect.united(item.sceneBoundingRect())
        totalRect.adjust(-10, -10, 10, 10)

        img = QImage(totalRect.width(), totalRect.height(),
                     QImage.Format_ARGB32_Premultiplied)
        img.fill(Qt.white)
        painter = QPainter()
        painter.setRenderHint(QPainter.Antialiasing)
        painter.begin(img)
        self.scene.render(painter, totalRect, totalRect)
        painter.end()

        img.save(filename)

    def exportAsPython(self):
        filename = unicode(QFileDialog.getSaveFileName(self,
                                                       self.tr('Save Model As Python Script'), '',
                                                       self.tr('Python files (*.py *.PY)')))
        if not filename:
            return

        if not filename.lower().endswith('.py'):
            filename += '.py'

        text = self.alg.toPython()
        with codecs.open(filename, 'w', encoding='utf-8') as fout:
            fout.write(text)
        QMessageBox.information(self, self.tr('Model exported'),
                                self.tr('Model was correctly exported.'))

    def saveModel(self, saveAs):
        if unicode(self.textGroup.text()).strip() == '' \
                or unicode(self.textName.text()).strip() == '':
            QMessageBox.warning(
                self, self.tr('Warning'), self.tr('Please enter group and model names before saving')
            )
            return
        self.alg.name = unicode(self.textName.text())
        self.alg.group = unicode(self.textGroup.text())
        if self.alg.descriptionFile is not None and not saveAs:
            filename = self.alg.descriptionFile
        else:
            filename = unicode(QFileDialog.getSaveFileName(self,
                                                           self.tr('Save Model'),
                                                           ModelerUtils.defaultModelsFolder(),
                                                           self.tr('Processing models (*.model)')))
            if filename:
                if not filename.endswith('.model'):
                    filename += '.model'
                self.alg.descriptionFile = filename
        if filename:
            text = self.alg.toJson()
            try:
                fout = codecs.open(filename, 'w', encoding='utf-8')
            except:
                if saveAs:
                    QMessageBox.warning(self, self.tr('I/O error'),
                                        self.tr('Unable to save edits. Reason:\n %s') % unicode(sys.exc_info()[1]))
                else:
                    QMessageBox.warning(self, self.tr("Can't save model"),
                                        self.tr("This model can't be saved in its "
                                                "original location (probably you do not "
                                                "have permission to do it). Please, use "
                                                "the 'Save as...' option."))
                return
            fout.write(text)
            fout.close()
            self.update = True
            QMessageBox.information(self, self.tr('Model saved'),
                                    self.tr('Model was correctly saved.'))

            self.hasChanged = False

    def openModel(self):
        filename = unicode(QFileDialog.getOpenFileName(self,
                                                       self.tr('Open Model'), ModelerUtils.defaultModelsFolder(),
                                                       self.tr('Processing models (*.model *.MODEL)')))
        if filename:
            try:
                alg = ModelerAlgorithm.fromFile(filename)
                self.alg = alg
                self.alg.setModelerView(self)
                self.textGroup.setText(alg.group)
                self.textName.setText(alg.name)
                self.repaintModel()

                self.view.centerOn(0, 0)
                self.hasChanged = False
            except WrongModelException as e:
                ProcessingLog.addToLog(ProcessingLog.LOG_ERROR,
                                       self.tr('Could not load model %s\n%s') % (filename, e.msg))
                QMessageBox.critical(self, self.tr('Could not open model'),
                                     self.tr('The selected model could not be loaded.\n'
                                             'See the log for more information.'))
            except Exception as e:
                ProcessingLog.addToLog(ProcessingLog.LOG_ERROR,
                                       self.tr('Could not load model %s\n%s') % (filename, e.args[0]))
                QMessageBox.critical(self, self.tr('Could not open model'),
                                     self.tr('The selected model could not be loaded.\n'
                                             'See the log for more information.'))

    def repaintModel(self):
        self.scene = ModelerScene()
        self.scene.setSceneRect(QRectF(0, 0, ModelerAlgorithm.CANVAS_SIZE,
                                       ModelerAlgorithm.CANVAS_SIZE))
        self.scene.paintModel(self.alg)
        self.view.setScene(self.scene)

    def addInput(self):
        item = self.inputsTree.currentItem()
        paramType = unicode(item.text(0))
        self.addInputOfType(paramType)

    def addInputOfType(self, paramType, pos=None):
        if paramType in ModelerParameterDefinitionDialog.paramTypes:
            dlg = ModelerParameterDefinitionDialog(self.alg, paramType)
            dlg.exec_()
            if dlg.param is not None:
                if pos is None:
                    pos = self.getPositionForParameterItem()
                if isinstance(pos, QPoint):
                    pos = QPointF(pos)
                self.alg.addParameter(ModelerParameter(dlg.param, pos))
                self.repaintModel()
                # self.view.ensureVisible(self.scene.getLastParameterItem())
                self.hasChanged = True

    def getPositionForParameterItem(self):
        MARGIN = 20
        BOX_WIDTH = 200
        BOX_HEIGHT = 80
        if self.alg.inputs:
            maxX = max([i.pos.x() for i in self.alg.inputs.values()])
            newX = min(MARGIN + BOX_WIDTH + maxX, self.CANVAS_SIZE - BOX_WIDTH)
        else:
            newX = MARGIN + BOX_WIDTH / 2
        return QPointF(newX, MARGIN + BOX_HEIGHT / 2)

    def fillInputsTree(self):
        icon = QIcon(os.path.join(pluginPath, 'images', 'input.png'))
        parametersItem = QTreeWidgetItem()
        parametersItem.setText(0, self.tr('Parameters'))
        for paramType in ModelerParameterDefinitionDialog.paramTypes:
            paramItem = QTreeWidgetItem()
            paramItem.setText(0, paramType)
            paramItem.setIcon(0, icon)
            paramItem.setFlags(Qt.ItemIsEnabled | Qt.ItemIsSelectable | Qt.ItemIsDragEnabled)
            parametersItem.addChild(paramItem)
        self.inputsTree.addTopLevelItem(parametersItem)
        parametersItem.setExpanded(True)

    def addAlgorithm(self):
        item = self.algorithmTree.currentItem()
        if isinstance(item, TreeAlgorithmItem):
            alg = algList.getAlgorithm(item.alg.commandLineName())
            self._addAlgorithm(alg.getCopy())

    def _addAlgorithm(self, alg, pos=None):
        dlg = alg.getCustomModelerParametersDialog(self.alg)
        if not dlg:
            dlg = ModelerParametersDialog(alg, self.alg)
        dlg.exec_()
        if dlg.alg is not None:
            if pos is None:
                dlg.alg.pos = self.getPositionForAlgorithmItem()
            else:
                dlg.alg.pos = pos
            if isinstance(dlg.alg.pos, QPoint):
                dlg.alg.pos = QPointF(pos)
            from processing.modeler.ModelerGraphicItem import ModelerGraphicItem
            for i, out in enumerate(dlg.alg.outputs):
                dlg.alg.outputs[out].pos = dlg.alg.pos + QPointF(ModelerGraphicItem.BOX_WIDTH, (i + 1.5)
                                                                 * ModelerGraphicItem.BOX_HEIGHT)
            self.alg.addAlgorithm(dlg.alg)
            self.repaintModel()
            self.hasChanged = True

    def getPositionForAlgorithmItem(self):
        MARGIN = 20
        BOX_WIDTH = 200
        BOX_HEIGHT = 80
        if self.alg.algs:
            maxX = max([alg.pos.x() for alg in self.alg.algs.values()])
            maxY = max([alg.pos.y() for alg in self.alg.algs.values()])
            newX = min(MARGIN + BOX_WIDTH + maxX, self.CANVAS_SIZE - BOX_WIDTH)
            newY = min(MARGIN + BOX_HEIGHT + maxY, self.CANVAS_SIZE
                       - BOX_HEIGHT)
        else:
            newX = MARGIN + BOX_WIDTH / 2
            newY = MARGIN * 2 + BOX_HEIGHT + BOX_HEIGHT / 2
        return QPointF(newX, newY)

    def fillAlgorithmTree(self):
        self.fillAlgorithmTreeUsingProviders()
        self.algorithmTree.sortItems(0, Qt.AscendingOrder)

        text = unicode(self.searchBox.text())
        if text != '':
            self.algorithmTree.expandAll()

    def fillAlgorithmTreeUsingProviders(self):
        self.algorithmTree.clear()
        text = unicode(self.searchBox.text())
        allAlgs = algList.algs
        for providerName in allAlgs.keys():
            name = 'ACTIVATE_' + providerName.upper().replace(' ', '_')
            if not ProcessingConfig.getSetting(name):
                continue
            groups = {}
            algs = allAlgs[providerName].values()

            # Add algorithms
            for alg in algs:
                if not alg.showInModeler or alg.allowOnlyOpenedLayers:
                    continue
                if alg.commandLineName() == self.alg.commandLineName():
                    continue
                if text == '' or text.lower() in alg.name.lower():
                    if alg.group in groups:
                        groupItem = groups[alg.group]
                    else:
                        groupItem = QTreeWidgetItem()
                        name = alg.i18n_group or alg.group
                        groupItem.setText(0, name)
                        groupItem.setToolTip(0, name)
                        groups[alg.group] = groupItem
                    algItem = TreeAlgorithmItem(alg)
                    groupItem.addChild(algItem)

            if len(groups) > 0:
                providerItem = QTreeWidgetItem()
                provider = algList.getProviderFromName(providerName)
                providerItem.setText(0, provider.getDescription())
                providerItem.setToolTip(0, provider.getDescription())
                providerItem.setIcon(0, provider.getIcon())
                for groupItem in groups.values():
                    providerItem.addChild(groupItem)
                self.algorithmTree.addTopLevelItem(providerItem)
                providerItem.setExpanded(text != '')
                for groupItem in groups.values():
                    if text != '':
                        groupItem.setExpanded(True)

        self.algorithmTree.sortItems(0, Qt.AscendingOrder)


class TreeAlgorithmItem(QTreeWidgetItem):

    def __init__(self, alg):
        QTreeWidgetItem.__init__(self)
        self.alg = alg
        icon = alg.getIcon()
        name = alg.displayName()
        self.setIcon(0, icon)
        self.setToolTip(0, name)
        self.setText(0, name)
