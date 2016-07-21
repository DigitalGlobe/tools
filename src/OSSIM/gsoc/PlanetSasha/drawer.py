from PyQt4.QtCore import *
from PyQt4.QtGui import *
import os
import sys
apppath = os.path.abspath(os.path.dirname(sys.argv[0]))


compass = '%s/images/compass3.png' % (apppath)
arrow = '%s/images/arrow3.png' % (apppath)
ghiera = '%s/images/ghiera.png' % (apppath)
sfondo = '%s/images/sfondo.png' % (apppath)

class DrawRoll(QWidget):
    def __init__(self, parent=None):
        QWidget.__init__(self) # , parent
        self.setWindowTitle('Euler')
        self.rotationAngle = 0
        #self.cmp = QtGui.QImage()
        #self.cmp.load("WHOI.png")
        self.cmp2 = QImage()
        self.cmp2.load(sfondo)
        self.cmp = QImage()
        self.cmp.load(ghiera)
        
    def sizeHint(self):
        return QSize(300,300)

    def resizeEvent(self, event=None):
        self.center_pos = QPoint(self.width()/2,self.height()/2)
        
    def setRotationAngle(self, degrees):
        self.rotationAngle = degrees
        self.update()

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.fillRect(event.rect(), QBrush(Qt.blue))
        cmp2 = self.cmp2.scaled(self.size(), Qt.KeepAspectRatio)
        cmp2Pos = QPoint((self.width() - cmp2.width())/2.0, (self.height() - cmp2.height())/2.0)
        cmp = self.cmp.scaled(self.size(), Qt.KeepAspectRatio)
        cmpPos = QPoint((self.width() - cmp.width())/2.0, (self.height() - cmp.height())/2.0)
        painter.drawImage(cmp2Pos, cmp)
        painter.translate(self.center_pos.x(), self.center_pos.y())
        painter.rotate(self.rotationAngle)
        painter.translate(-self.center_pos.x(), -self.center_pos.y())
        painter.drawImage(cmp2Pos, cmp2)


class DrawCompass(QWidget):
    def __init__(self, parent=None):
        QWidget.__init__(self) # , parent
        self.setWindowTitle('Euler')
        self.rotationAngle = 0
        #self.cmp = QtGui.QImage()
        #self.cmp.load("WHOI.png")
        self.cmp2 = QImage()
        self.cmp2.load(compass)
        self.cmp = QImage()
        self.cmp.load(arrow)

    def sizeHint(self):
        return QSize(300,300)

    def resizeEvent(self, event=None):
        self.center_pos = QPoint(self.width()/2,self.height()/2)

    def setRotationAngle(self, degrees):
        self.rotationAngle = degrees
        self.update()

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.fillRect(event.rect(), QBrush(Qt.blue))
        cmp2 = self.cmp2.scaled(self.size(), Qt.KeepAspectRatio)
        cmp2Pos = QPoint((self.width() - cmp2.width())/2.0, (self.height() - cmp2.height())/2.0)
        cmp = self.cmp.scaled(self.size(), Qt.KeepAspectRatio)
        cmpPos = QPoint((self.width() - cmp.width())/2.0, (self.height() - cmp.height())/2.0)
        painter.drawImage(cmp2Pos, cmp2)
        painter.translate(self.center_pos.x(), self.center_pos.y())
        painter.rotate(self.rotationAngle)
        painter.translate(-self.center_pos.x(), -self.center_pos.y())
        painter.drawImage(cmp2Pos, cmp)
