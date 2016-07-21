from PyQt4.QtCore import *
from PyQt4.QtGui import *

from ui import Ui_OssimPlanetSasha


class ui_Control(QMainWindow, Ui_OssimPlanetSasha):
    def __init__(self, parent, fl):
        QMainWindow.__init__(self, parent, fl)
        self.setupUi(self)
