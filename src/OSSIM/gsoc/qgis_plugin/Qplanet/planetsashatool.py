from PyQt4.QtCore import *
from PyQt4.QtGui import *
from qgis.core import *
from qgis.gui import *

# Raster File Info Tool class
class PlanetSashaTool(QgsMapTool):
  def __init__(self, canvas):
    QgsMapTool.__init__(self,canvas)
    self.canvas=canvas

    self.cursor = QCursor(QPixmap(["16 16 3 1",
                                   "# c None","a c #000000",". c #ffffff",
                                   "########a########",
                                   "########a########",
                                   "########a########",
                                   "########a########",
                                   "########a########",
                                   "########a########",
                                   "########a########",
                                   "########a########",
                                   "aaaaaaaaaaaaaaaaa",
                                   "########a########",
                                   "########a########",
                                   "########a########",
                                   "########a########",
                                   "########a########",
                                   "########a########",
                                   "########a########",
                                   "########a########"]))
  
  def canvasPressEvent(self,event):
    pass
  
  def canvasMoveEvent(self,event):
    pass
  
  def canvasReleaseEvent(self,event):
    self.emit(SIGNAL("finished(PyQt_PyObject)"),
              {'x': event.pos().x(), 'y': event.pos().y()})
            
  #def activate(self):
  #  QgsMapTool.activate(self)
  #  self.canvas.setCursor(self.cursor)
  
  #def deactivate(self):
  #  QgsMapTool.deactivate(self)
  
  def isZoomTool(self):
    return False

