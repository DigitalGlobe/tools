# -*- coding: utf-8 -*-

"""
***************************************************************************
    extentSelector.py
    ---------------------
    Date                 : December 2010
    Copyright            : (C) 2010 by Giuseppe Sucameli
    Email                : brush dot tyler at gmail dot com
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
"""

__author__ = 'Giuseppe Sucameli'
__date__ = 'December 2010'
__copyright__ = '(C) 2010, Giuseppe Sucameli'
# This will get replaced with a git SHA1 when you do a git archive
__revision__ = '$Format:%H$'

from qgis.PyQt.QtCore import pyqtSignal
from qgis.PyQt.QtWidgets import QWidget
from qgis.PyQt.QtGui import QColor
from qgis.core import QgsPoint, QgsRectangle, QGis
from qgis.gui import QgsMapTool, QgsMapToolEmitPoint, QgsRubberBand

from .ui_extentSelector import Ui_GdalToolsExtentSelector as Ui_ExtentSelector


class GdalToolsExtentSelector(QWidget, Ui_ExtentSelector):
    selectionStarted = pyqtSignal()
    selectionStopped = pyqtSignal()
    selectionPaused = pyqtSignal()
    newExtentDefined = pyqtSignal()

    def __init__(self, parent=None):
        QWidget.__init__(self, parent)
        self.canvas = None
        self.tool = None
        self.previousMapTool = None
        self.isStarted = False

        self.setupUi(self)

        self.x1CoordEdit.textChanged.connect(self.coordsChanged)
        self.x2CoordEdit.textChanged.connect(self.coordsChanged)
        self.y1CoordEdit.textChanged.connect(self.coordsChanged)
        self.y2CoordEdit.textChanged.connect(self.coordsChanged)
        self.btnEnable.clicked.connect(self.start)

    def setCanvas(self, canvas):
        self.canvas = canvas
        self.tool = RectangleMapTool(self.canvas)
        self.previousMapTool = self.canvas.mapTool()
        self.tool.rectangleCreated.connect(self.fillCoords)
        self.tool.deactivated.connect(self.pause)

    def stop(self):
        if not self.isStarted:
            return
        self.isStarted = False
        self.btnEnable.setVisible(False)
        self.tool.reset()
        self.canvas.unsetMapTool(self.tool)
        if self.previousMapTool != self.tool:
            self.canvas.setMapTool(self.previousMapTool)
        #self.coordsChanged()
        self.selectionStopped.emit()

    def start(self):
        prevMapTool = self.canvas.mapTool()
        if prevMapTool != self.tool:
            self.previousMapTool = prevMapTool
        self.canvas.setMapTool(self.tool)
        self.isStarted = True
        self.btnEnable.setVisible(False)
        self.coordsChanged()
        self.selectionStarted.emit()

    def pause(self):
        if not self.isStarted:
            return

        self.btnEnable.setVisible(True)
        self.selectionPaused.emit()

    def setExtent(self, rect):
        if self.tool.setRectangle(rect):
            self.newExtentDefined.emit()

    def getExtent(self):
        return self.tool.rectangle()

    def isCoordsValid(self):
        try:
            QgsPoint(float(self.x1CoordEdit.text()), float(self.y1CoordEdit.text()))
            QgsPoint(float(self.x2CoordEdit.text()), float(self.y2CoordEdit.text()))
        except ValueError:
            return False

        return True

    def coordsChanged(self):
        rect = None
        if self.isCoordsValid():
            point1 = QgsPoint(float(self.x1CoordEdit.text()), float(self.y1CoordEdit.text()))
            point2 = QgsPoint(float(self.x2CoordEdit.text()), float(self.y2CoordEdit.text()))
            rect = QgsRectangle(point1, point2)

        self.setExtent(rect)

    def fillCoords(self):
        rect = self.getExtent()
        self.blockSignals(True)
        if rect is not None:
            self.x1CoordEdit.setText(unicode(rect.xMinimum()))
            self.x2CoordEdit.setText(unicode(rect.xMaximum()))
            self.y1CoordEdit.setText(unicode(rect.yMaximum()))
            self.y2CoordEdit.setText(unicode(rect.yMinimum()))
        else:
            self.x1CoordEdit.clear()
            self.x2CoordEdit.clear()
            self.y1CoordEdit.clear()
            self.y2CoordEdit.clear()
        self.blockSignals(False)
        self.newExtentDefined.emit()


class RectangleMapTool(QgsMapToolEmitPoint):
    rectangleCreated = pyqtSignal()

    def __init__(self, canvas):
        self.canvas = canvas
        QgsMapToolEmitPoint.__init__(self, self.canvas)

        self.rubberBand = QgsRubberBand(self.canvas, QGis.Polygon)
        self.rubberBand.setColor(QColor(255, 0, 0, 100))
        self.rubberBand.setWidth(2)

        self.reset()

    def reset(self):
        self.startPoint = self.endPoint = None
        self.isEmittingPoint = False
        self.rubberBand.reset(QGis.Polygon)

    def canvasPressEvent(self, e):
        self.startPoint = self.toMapCoordinates(e.pos())
        self.endPoint = self.startPoint
        self.isEmittingPoint = True

        self.showRect(self.startPoint, self.endPoint)

    def canvasReleaseEvent(self, e):
        self.isEmittingPoint = False
        self.rectangleCreated.emit()

    def canvasMoveEvent(self, e):
        if not self.isEmittingPoint:
            return

        self.endPoint = self.toMapCoordinates(e.pos())
        self.showRect(self.startPoint, self.endPoint)

    def showRect(self, startPoint, endPoint):
        self.rubberBand.reset(QGis.Polygon)
        if startPoint.x() == endPoint.x() or startPoint.y() == endPoint.y():
            return

        point1 = QgsPoint(startPoint.x(), startPoint.y())
        point2 = QgsPoint(startPoint.x(), endPoint.y())
        point3 = QgsPoint(endPoint.x(), endPoint.y())
        point4 = QgsPoint(endPoint.x(), startPoint.y())

        self.rubberBand.addPoint(point1, False)
        self.rubberBand.addPoint(point2, False)
        self.rubberBand.addPoint(point3, False)
        self.rubberBand.addPoint(point4, True)  # true to update canvas
        self.rubberBand.show()

    def rectangle(self):
        if self.startPoint is None or self.endPoint is None:
            return None
        elif self.startPoint.x() == self.endPoint.x() or self.startPoint.y() == self.endPoint.y():
            return None

        return QgsRectangle(self.startPoint, self.endPoint)

    def setRectangle(self, rect):
        if rect == self.rectangle():
            return False

        if rect is None:
            self.reset()
        else:
            self.startPoint = QgsPoint(rect.xMaximum(), rect.yMaximum())
            self.endPoint = QgsPoint(rect.xMinimum(), rect.yMinimum())
            self.showRect(self.startPoint, self.endPoint)
        return True

    def deactivate(self):
        QgsMapTool.deactivate(self)
        self.deactivated.emit()
