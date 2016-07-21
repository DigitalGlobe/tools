#!/usr/bin/env python
import sys

from PyQt4.QtCore import *
from PyQt4.QtGui import *

from planetsashaui import Ui_OssimPlanetSasha

class GuiWidget(QMainWindow, Ui_OssimPlanetSasha):
    def __init__(self):
        QMainWindow.__init__(self)
        self.setupUi(self)
        
