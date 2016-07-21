# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/Users/epy/Desktop/X/gui/PlanetSasha_rev_ui.ui'
#
# Created: Mon Feb 21 16:47:20 2011
#      by: PyQt4 UI code generator 4.8.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_OssimPlanetSasha(object):
    def setupUi(self, OssimPlanetSasha):
        OssimPlanetSasha.setObjectName(_fromUtf8("OssimPlanetSasha"))
        OssimPlanetSasha.resize(744, 387)
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/icons/epi.gif")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        OssimPlanetSasha.setWindowIcon(icon)
        self.centralwidget = QtGui.QWidget(OssimPlanetSasha)
        self.centralwidget.setObjectName(_fromUtf8("centralwidget"))
        self.horizontalLayout = QtGui.QHBoxLayout(self.centralwidget)
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.tabWidget = QtGui.QTabWidget(self.centralwidget)
        self.tabWidget.setTabPosition(QtGui.QTabWidget.North)
        self.tabWidget.setTabShape(QtGui.QTabWidget.Rounded)
        self.tabWidget.setUsesScrollButtons(False)
        self.tabWidget.setDocumentMode(False)
        self.tabWidget.setTabsClosable(False)
        self.tabWidget.setMovable(True)
        self.tabWidget.setObjectName(_fromUtf8("tabWidget"))
        self.tab = QtGui.QWidget()
        self.tab.setObjectName(_fromUtf8("tab"))
        self.verticalLayout = QtGui.QVBoxLayout(self.tab)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.scrollArea_4 = QtGui.QScrollArea(self.tab)
        self.scrollArea_4.setStyleSheet(_fromUtf8(""))
        self.scrollArea_4.setWidgetResizable(True)
        self.scrollArea_4.setObjectName(_fromUtf8("scrollArea_4"))
        self.scrollAreaWidgetContents_4 = QtGui.QWidget()
        self.scrollAreaWidgetContents_4.setGeometry(QtCore.QRect(0, 0, 541, 645))
        self.scrollAreaWidgetContents_4.setObjectName(_fromUtf8("scrollAreaWidgetContents_4"))
        self.verticalLayout_5 = QtGui.QVBoxLayout(self.scrollAreaWidgetContents_4)
        self.verticalLayout_5.setObjectName(_fromUtf8("verticalLayout_5"))
        self.line_8 = QtGui.QFrame(self.scrollAreaWidgetContents_4)
        self.line_8.setFrameShape(QtGui.QFrame.HLine)
        self.line_8.setFrameShadow(QtGui.QFrame.Sunken)
        self.line_8.setObjectName(_fromUtf8("line_8"))
        self.verticalLayout_5.addWidget(self.line_8)
        self.horizontalLayout_6 = QtGui.QHBoxLayout()
        self.horizontalLayout_6.setObjectName(_fromUtf8("horizontalLayout_6"))
        self.label_3 = QtGui.QLabel(self.scrollAreaWidgetContents_4)
        self.label_3.setMinimumSize(QtCore.QSize(50, 0))
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.horizontalLayout_6.addWidget(self.label_3)
        self.Lat = QtGui.QLineEdit(self.scrollAreaWidgetContents_4)
        self.Lat.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Lat.setAlignment(QtCore.Qt.AlignCenter)
        self.Lat.setObjectName(_fromUtf8("Lat"))
        self.horizontalLayout_6.addWidget(self.Lat)
        self.Lon = QtGui.QLineEdit(self.scrollAreaWidgetContents_4)
        self.Lon.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Lon.setAlignment(QtCore.Qt.AlignCenter)
        self.Lon.setObjectName(_fromUtf8("Lon"))
        self.horizontalLayout_6.addWidget(self.Lon)
        self.Alt = QtGui.QLineEdit(self.scrollAreaWidgetContents_4)
        self.Alt.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Alt.setObjectName(_fromUtf8("Alt"))
        self.horizontalLayout_6.addWidget(self.Alt)
        self.verticalLayout_5.addLayout(self.horizontalLayout_6)
        self.line_5 = QtGui.QFrame(self.scrollAreaWidgetContents_4)
        self.line_5.setFrameShape(QtGui.QFrame.HLine)
        self.line_5.setFrameShadow(QtGui.QFrame.Sunken)
        self.line_5.setObjectName(_fromUtf8("line_5"))
        self.verticalLayout_5.addWidget(self.line_5)
        self.horizontalLayout_5 = QtGui.QHBoxLayout()
        self.horizontalLayout_5.setObjectName(_fromUtf8("horizontalLayout_5"))
        self.label_2 = QtGui.QLabel(self.scrollAreaWidgetContents_4)
        self.label_2.setMinimumSize(QtCore.QSize(50, 0))
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.horizontalLayout_5.addWidget(self.label_2)
        self.lookatLat = QtGui.QLineEdit(self.scrollAreaWidgetContents_4)
        self.lookatLat.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.lookatLat.setObjectName(_fromUtf8("lookatLat"))
        self.horizontalLayout_5.addWidget(self.lookatLat)
        self.lookatLon = QtGui.QLineEdit(self.scrollAreaWidgetContents_4)
        self.lookatLon.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.lookatLon.setObjectName(_fromUtf8("lookatLon"))
        self.horizontalLayout_5.addWidget(self.lookatLon)
        self.lookatAlt = QtGui.QLineEdit(self.scrollAreaWidgetContents_4)
        self.lookatAlt.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.lookatAlt.setObjectName(_fromUtf8("lookatAlt"))
        self.horizontalLayout_5.addWidget(self.lookatAlt)
        self.verticalLayout_5.addLayout(self.horizontalLayout_5)
        self.line_9 = QtGui.QFrame(self.scrollAreaWidgetContents_4)
        self.line_9.setFrameShape(QtGui.QFrame.HLine)
        self.line_9.setFrameShadow(QtGui.QFrame.Sunken)
        self.line_9.setObjectName(_fromUtf8("line_9"))
        self.verticalLayout_5.addWidget(self.line_9)
        self.horizontalLayout_11 = QtGui.QHBoxLayout()
        self.horizontalLayout_11.setObjectName(_fromUtf8("horizontalLayout_11"))
        self.label_9 = QtGui.QLabel(self.scrollAreaWidgetContents_4)
        self.label_9.setMinimumSize(QtCore.QSize(50, 0))
        self.label_9.setObjectName(_fromUtf8("label_9"))
        self.horizontalLayout_11.addWidget(self.label_9)
        self.NorthEast = QtGui.QCheckBox(self.scrollAreaWidgetContents_4)
        self.NorthEast.setText(_fromUtf8(""))
        self.NorthEast.setObjectName(_fromUtf8("NorthEast"))
        self.horizontalLayout_11.addWidget(self.NorthEast)
        self.Nord = QtGui.QLineEdit(self.scrollAreaWidgetContents_4)
        self.Nord.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Nord.setAlignment(QtCore.Qt.AlignCenter)
        self.Nord.setObjectName(_fromUtf8("Nord"))
        self.horizontalLayout_11.addWidget(self.Nord)
        self.East = QtGui.QLineEdit(self.scrollAreaWidgetContents_4)
        self.East.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.East.setAlignment(QtCore.Qt.AlignCenter)
        self.East.setObjectName(_fromUtf8("East"))
        self.horizontalLayout_11.addWidget(self.East)
        self.label_11 = QtGui.QLabel(self.scrollAreaWidgetContents_4)
        self.label_11.setObjectName(_fromUtf8("label_11"))
        self.horizontalLayout_11.addWidget(self.label_11)
        self.utmcode = QtGui.QLineEdit(self.scrollAreaWidgetContents_4)
        self.utmcode.setMinimumSize(QtCore.QSize(45, 0))
        self.utmcode.setMaximumSize(QtCore.QSize(45, 16777215))
        self.utmcode.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.utmcode.setText(_fromUtf8(""))
        self.utmcode.setAlignment(QtCore.Qt.AlignCenter)
        self.utmcode.setObjectName(_fromUtf8("utmcode"))
        self.horizontalLayout_11.addWidget(self.utmcode)
        self.ellipse = QtGui.QComboBox(self.scrollAreaWidgetContents_4)
        self.ellipse.setMaximumSize(QtCore.QSize(150, 26))
        self.ellipse.setStyleSheet(_fromUtf8(""))
        self.ellipse.setObjectName(_fromUtf8("ellipse"))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.ellipse.addItem(_fromUtf8(""))
        self.horizontalLayout_11.addWidget(self.ellipse)
        self.verticalLayout_5.addLayout(self.horizontalLayout_11)
        self.line_10 = QtGui.QFrame(self.scrollAreaWidgetContents_4)
        self.line_10.setFrameShape(QtGui.QFrame.HLine)
        self.line_10.setFrameShadow(QtGui.QFrame.Sunken)
        self.line_10.setObjectName(_fromUtf8("line_10"))
        self.verticalLayout_5.addWidget(self.line_10)
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.horizontalLayout_3.setObjectName(_fromUtf8("horizontalLayout_3"))
        self.Place = QtGui.QComboBox(self.scrollAreaWidgetContents_4)
        self.Place.setStyleSheet(_fromUtf8(""))
        self.Place.setObjectName(_fromUtf8("Place"))
        self.Place.addItem(_fromUtf8(""))
        self.Place.setItemText(0, _fromUtf8(""))
        self.horizontalLayout_3.addWidget(self.Place)
        self.line_17 = QtGui.QFrame(self.scrollAreaWidgetContents_4)
        self.line_17.setFrameShape(QtGui.QFrame.VLine)
        self.line_17.setFrameShadow(QtGui.QFrame.Sunken)
        self.line_17.setObjectName(_fromUtf8("line_17"))
        self.horizontalLayout_3.addWidget(self.line_17)
        self.refreshsqlite = QtGui.QToolButton(self.scrollAreaWidgetContents_4)
        self.refreshsqlite.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 blue);\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        self.refreshsqlite.setText(_fromUtf8(""))
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/mActionDraw.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.refreshsqlite.setIcon(icon1)
        self.refreshsqlite.setObjectName(_fromUtf8("refreshsqlite"))
        self.horizontalLayout_3.addWidget(self.refreshsqlite)
        self.line_18 = QtGui.QFrame(self.scrollAreaWidgetContents_4)
        self.line_18.setFrameShape(QtGui.QFrame.VLine)
        self.line_18.setFrameShadow(QtGui.QFrame.Sunken)
        self.line_18.setObjectName(_fromUtf8("line_18"))
        self.horizontalLayout_3.addWidget(self.line_18)
        self.placezone = QtGui.QComboBox(self.scrollAreaWidgetContents_4)
        self.placezone.setStyleSheet(_fromUtf8(""))
        self.placezone.setObjectName(_fromUtf8("placezone"))
        self.horizontalLayout_3.addWidget(self.placezone)
        self.SendPosition = QtGui.QPushButton(self.scrollAreaWidgetContents_4)
        self.SendPosition.setMaximumSize(QtCore.QSize(30, 16777215))
        self.SendPosition.setStyleSheet(_fromUtf8("QPushButton {\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
"     min-width: 44px;\n"
" }\n"
"\n"
" QPushButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QPushButton:flat {\n"
"     border: none; /* no border for a flat push button */\n"
" }\n"
"\n"
" QPushButton:default {\n"
"     border-color: navy; /* make the default button prominent */\n"
" }"))
        self.SendPosition.setObjectName(_fromUtf8("SendPosition"))
        self.horizontalLayout_3.addWidget(self.SendPosition)
        self.verticalLayout_5.addLayout(self.horizontalLayout_3)
        self.line_19 = QtGui.QFrame(self.scrollAreaWidgetContents_4)
        self.line_19.setFrameShape(QtGui.QFrame.HLine)
        self.line_19.setFrameShadow(QtGui.QFrame.Sunken)
        self.line_19.setObjectName(_fromUtf8("line_19"))
        self.verticalLayout_5.addWidget(self.line_19)
        self.horizontalLayout_7 = QtGui.QHBoxLayout()
        self.horizontalLayout_7.setObjectName(_fromUtf8("horizontalLayout_7"))
        self.label_4 = QtGui.QLabel(self.scrollAreaWidgetContents_4)
        self.label_4.setLayoutDirection(QtCore.Qt.RightToLeft)
        self.label_4.setObjectName(_fromUtf8("label_4"))
        self.horizontalLayout_7.addWidget(self.label_4)
        self.SpeedSpinBox = QtGui.QDoubleSpinBox(self.scrollAreaWidgetContents_4)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.SpeedSpinBox.sizePolicy().hasHeightForWidth())
        self.SpeedSpinBox.setSizePolicy(sizePolicy)
        self.SpeedSpinBox.setStyleSheet(_fromUtf8("background: rgb(231, 231, 231);"))
        self.SpeedSpinBox.setProperty(_fromUtf8("value"), 1.0)
        self.SpeedSpinBox.setObjectName(_fromUtf8("SpeedSpinBox"))
        self.horizontalLayout_7.addWidget(self.SpeedSpinBox)
        self.label_5 = QtGui.QLabel(self.scrollAreaWidgetContents_4)
        self.label_5.setLayoutDirection(QtCore.Qt.RightToLeft)
        self.label_5.setObjectName(_fromUtf8("label_5"))
        self.horizontalLayout_7.addWidget(self.label_5)
        self.SpeedMultipler = QtGui.QSpinBox(self.scrollAreaWidgetContents_4)
        self.SpeedMultipler.setStyleSheet(_fromUtf8("background: rgb(231, 231, 231);"))
        self.SpeedMultipler.setObjectName(_fromUtf8("SpeedMultipler"))
        self.horizontalLayout_7.addWidget(self.SpeedMultipler)
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_7.addItem(spacerItem)
        self.View = QtGui.QComboBox(self.scrollAreaWidgetContents_4)
        self.View.setStyleSheet(_fromUtf8(""))
        self.View.setObjectName(_fromUtf8("View"))
        self.View.addItem(_fromUtf8(""))
        self.View.addItem(_fromUtf8(""))
        self.horizontalLayout_7.addWidget(self.View)
        self.Head = QtGui.QComboBox(self.scrollAreaWidgetContents_4)
        self.Head.setStyleSheet(_fromUtf8(""))
        self.Head.setModelColumn(0)
        self.Head.setObjectName(_fromUtf8("Head"))
        self.Head.addItem(_fromUtf8(""))
        self.Head.setItemText(0, _fromUtf8(""))
        self.Head.addItem(_fromUtf8(""))
        self.Head.addItem(_fromUtf8(""))
        self.Head.addItem(_fromUtf8(""))
        self.Head.addItem(_fromUtf8(""))
        self.Head.addItem(_fromUtf8(""))
        self.Head.addItem(_fromUtf8(""))
        self.Head.addItem(_fromUtf8(""))
        self.Head.addItem(_fromUtf8(""))
        self.Head.addItem(_fromUtf8(""))
        self.Head.addItem(_fromUtf8(""))
        self.horizontalLayout_7.addWidget(self.Head)
        self.line = QtGui.QFrame(self.scrollAreaWidgetContents_4)
        self.line.setFrameShape(QtGui.QFrame.VLine)
        self.line.setFrameShadow(QtGui.QFrame.Sunken)
        self.line.setObjectName(_fromUtf8("line"))
        self.horizontalLayout_7.addWidget(self.line)
        self.verticalLayout_5.addLayout(self.horizontalLayout_7)
        self.line_20 = QtGui.QFrame(self.scrollAreaWidgetContents_4)
        self.line_20.setFrameShape(QtGui.QFrame.HLine)
        self.line_20.setFrameShadow(QtGui.QFrame.Sunken)
        self.line_20.setObjectName(_fromUtf8("line_20"))
        self.verticalLayout_5.addWidget(self.line_20)
        self.horizontalLayout_12 = QtGui.QHBoxLayout()
        self.horizontalLayout_12.setObjectName(_fromUtf8("horizontalLayout_12"))
        self.hsHeading = QtGui.QToolButton(self.scrollAreaWidgetContents_4)
        self.hsHeading.setStyleSheet(_fromUtf8("QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     font: bold; \n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 rgb(108, 183, 255) );\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        self.hsHeading.setCheckable(True)
        self.hsHeading.setObjectName(_fromUtf8("hsHeading"))
        self.horizontalLayout_12.addWidget(self.hsHeading)
        self.HandlingSpinBox = QtGui.QDoubleSpinBox(self.scrollAreaWidgetContents_4)
        self.HandlingSpinBox.setStyleSheet(_fromUtf8("border: 1px solid #8f8f91;\n"
"border-radius: 6px;\n"
"background: rgb(231, 231, 231);"))
        self.HandlingSpinBox.setAlignment(QtCore.Qt.AlignCenter)
        self.HandlingSpinBox.setButtonSymbols(QtGui.QAbstractSpinBox.NoButtons)
        self.HandlingSpinBox.setMinimum(-360.0)
        self.HandlingSpinBox.setMaximum(360.0)
        self.HandlingSpinBox.setObjectName(_fromUtf8("HandlingSpinBox"))
        self.horizontalLayout_12.addWidget(self.HandlingSpinBox)
        spacerItem1 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_12.addItem(spacerItem1)
        self.hsPitch = QtGui.QToolButton(self.scrollAreaWidgetContents_4)
        self.hsPitch.setStyleSheet(_fromUtf8("QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     font: bold; \n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 rgb(108, 183, 255) );\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        self.hsPitch.setCheckable(True)
        self.hsPitch.setObjectName(_fromUtf8("hsPitch"))
        self.horizontalLayout_12.addWidget(self.hsPitch)
        self.PitchSpinBox = QtGui.QDoubleSpinBox(self.scrollAreaWidgetContents_4)
        self.PitchSpinBox.setStyleSheet(_fromUtf8("border: 1px solid #8f8f91;\n"
"border-radius: 6px;\n"
"background: rgb(231, 231, 231);"))
        self.PitchSpinBox.setAlignment(QtCore.Qt.AlignCenter)
        self.PitchSpinBox.setButtonSymbols(QtGui.QAbstractSpinBox.NoButtons)
        self.PitchSpinBox.setMinimum(-90.0)
        self.PitchSpinBox.setMaximum(90.0)
        self.PitchSpinBox.setObjectName(_fromUtf8("PitchSpinBox"))
        self.horizontalLayout_12.addWidget(self.PitchSpinBox)
        spacerItem2 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_12.addItem(spacerItem2)
        self.hsRoll = QtGui.QToolButton(self.scrollAreaWidgetContents_4)
        self.hsRoll.setStyleSheet(_fromUtf8("QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     font: bold; \n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 rgb(108, 183, 255) );\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        self.hsRoll.setCheckable(True)
        self.hsRoll.setObjectName(_fromUtf8("hsRoll"))
        self.horizontalLayout_12.addWidget(self.hsRoll)
        self.RollSpinBox = QtGui.QDoubleSpinBox(self.scrollAreaWidgetContents_4)
        self.RollSpinBox.setStyleSheet(_fromUtf8("border: 1px solid #8f8f91;\n"
"border-radius: 6px;\n"
"background: rgb(231, 231, 231);"))
        self.RollSpinBox.setAlignment(QtCore.Qt.AlignCenter)
        self.RollSpinBox.setButtonSymbols(QtGui.QAbstractSpinBox.NoButtons)
        self.RollSpinBox.setAccelerated(False)
        self.RollSpinBox.setMinimum(-90.0)
        self.RollSpinBox.setMaximum(90.0)
        self.RollSpinBox.setObjectName(_fromUtf8("RollSpinBox"))
        self.horizontalLayout_12.addWidget(self.RollSpinBox)
        self.verticalLayout_5.addLayout(self.horizontalLayout_12)
        self.horizontalLayout_4 = QtGui.QHBoxLayout()
        self.horizontalLayout_4.setObjectName(_fromUtf8("horizontalLayout_4"))
        self.HandlingSlider = QtGui.QSlider(self.scrollAreaWidgetContents_4)
        self.HandlingSlider.setMinimum(-360)
        self.HandlingSlider.setMaximum(360)
        self.HandlingSlider.setOrientation(QtCore.Qt.Horizontal)
        self.HandlingSlider.setTickPosition(QtGui.QSlider.TicksBothSides)
        self.HandlingSlider.setTickInterval(10)
        self.HandlingSlider.setObjectName(_fromUtf8("HandlingSlider"))
        self.horizontalLayout_4.addWidget(self.HandlingSlider)
        self.verticalLayout_5.addLayout(self.horizontalLayout_4)
        self.PitchSlider = QtGui.QSlider(self.scrollAreaWidgetContents_4)
        self.PitchSlider.setMinimum(-90)
        self.PitchSlider.setMaximum(90)
        self.PitchSlider.setOrientation(QtCore.Qt.Horizontal)
        self.PitchSlider.setTickPosition(QtGui.QSlider.TicksBothSides)
        self.PitchSlider.setObjectName(_fromUtf8("PitchSlider"))
        self.verticalLayout_5.addWidget(self.PitchSlider)
        self.RollSlider = QtGui.QSlider(self.scrollAreaWidgetContents_4)
        self.RollSlider.setMinimum(-90)
        self.RollSlider.setMaximum(90)
        self.RollSlider.setOrientation(QtCore.Qt.Horizontal)
        self.RollSlider.setTickPosition(QtGui.QSlider.TicksBothSides)
        self.RollSlider.setTickInterval(10)
        self.RollSlider.setObjectName(_fromUtf8("RollSlider"))
        self.verticalLayout_5.addWidget(self.RollSlider)
        self.line_2 = QtGui.QFrame(self.scrollAreaWidgetContents_4)
        self.line_2.setStyleSheet(_fromUtf8(""))
        self.line_2.setFrameShape(QtGui.QFrame.HLine)
        self.line_2.setFrameShadow(QtGui.QFrame.Sunken)
        self.line_2.setObjectName(_fromUtf8("line_2"))
        self.verticalLayout_5.addWidget(self.line_2)
        self.horizontalLayout_8 = QtGui.QHBoxLayout()
        self.horizontalLayout_8.setObjectName(_fromUtf8("horizontalLayout_8"))
        self.hsZoom = QtGui.QToolButton(self.scrollAreaWidgetContents_4)
        self.hsZoom.setStyleSheet(_fromUtf8("QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     font: bold; \n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 rgb(108, 183, 255) );\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        self.hsZoom.setCheckable(True)
        self.hsZoom.setObjectName(_fromUtf8("hsZoom"))
        self.horizontalLayout_8.addWidget(self.hsZoom)
        spacerItem3 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_8.addItem(spacerItem3)
        self.ZoomSpinBox = QtGui.QDoubleSpinBox(self.scrollAreaWidgetContents_4)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.MinimumExpanding, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.ZoomSpinBox.sizePolicy().hasHeightForWidth())
        self.ZoomSpinBox.setSizePolicy(sizePolicy)
        self.ZoomSpinBox.setAutoFillBackground(True)
        self.ZoomSpinBox.setStyleSheet(_fromUtf8("background: rgb(231, 231, 231);"))
        self.ZoomSpinBox.setWrapping(False)
        self.ZoomSpinBox.setAlignment(QtCore.Qt.AlignCenter)
        self.ZoomSpinBox.setButtonSymbols(QtGui.QAbstractSpinBox.UpDownArrows)
        self.ZoomSpinBox.setAccelerated(True)
        self.ZoomSpinBox.setMinimum(-10000.0)
        self.ZoomSpinBox.setMaximum(27536977.99)
        self.ZoomSpinBox.setObjectName(_fromUtf8("ZoomSpinBox"))
        self.horizontalLayout_8.addWidget(self.ZoomSpinBox)
        self.label_6 = QtGui.QLabel(self.scrollAreaWidgetContents_4)
        self.label_6.setObjectName(_fromUtf8("label_6"))
        self.horizontalLayout_8.addWidget(self.label_6)
        self.ZoomStepSpinBox = QtGui.QDoubleSpinBox(self.scrollAreaWidgetContents_4)
        self.ZoomStepSpinBox.setStyleSheet(_fromUtf8("background: rgb(231, 231, 231);"))
        self.ZoomStepSpinBox.setButtonSymbols(QtGui.QAbstractSpinBox.UpDownArrows)
        self.ZoomStepSpinBox.setMinimum(0.0)
        self.ZoomStepSpinBox.setMaximum(100000.0)
        self.ZoomStepSpinBox.setSingleStep(1.0)
        self.ZoomStepSpinBox.setProperty(_fromUtf8("value"), 100.0)
        self.ZoomStepSpinBox.setObjectName(_fromUtf8("ZoomStepSpinBox"))
        self.horizontalLayout_8.addWidget(self.ZoomStepSpinBox)
        self.label_7 = QtGui.QLabel(self.scrollAreaWidgetContents_4)
        self.label_7.setObjectName(_fromUtf8("label_7"))
        self.horizontalLayout_8.addWidget(self.label_7)
        self.ZoomMultipler = QtGui.QSpinBox(self.scrollAreaWidgetContents_4)
        self.ZoomMultipler.setStyleSheet(_fromUtf8("background: rgb(231, 231, 231);"))
        self.ZoomMultipler.setMinimum(0)
        self.ZoomMultipler.setObjectName(_fromUtf8("ZoomMultipler"))
        self.horizontalLayout_8.addWidget(self.ZoomMultipler)
        self.verticalLayout_5.addLayout(self.horizontalLayout_8)
        self.ZoomSlider = QtGui.QSlider(self.scrollAreaWidgetContents_4)
        self.ZoomSlider.setMinimum(-10000)
        self.ZoomSlider.setMaximum(27536977)
        self.ZoomSlider.setProperty(_fromUtf8("value"), 5000)
        self.ZoomSlider.setOrientation(QtCore.Qt.Horizontal)
        self.ZoomSlider.setTickPosition(QtGui.QSlider.TicksBothSides)
        self.ZoomSlider.setTickInterval(1000000)
        self.ZoomSlider.setObjectName(_fromUtf8("ZoomSlider"))
        self.verticalLayout_5.addWidget(self.ZoomSlider)
        self.line_3 = QtGui.QFrame(self.scrollAreaWidgetContents_4)
        self.line_3.setFrameShape(QtGui.QFrame.HLine)
        self.line_3.setFrameShadow(QtGui.QFrame.Sunken)
        self.line_3.setObjectName(_fromUtf8("line_3"))
        self.verticalLayout_5.addWidget(self.line_3)
        self.horizontalLayout_13 = QtGui.QHBoxLayout()
        self.horizontalLayout_13.setObjectName(_fromUtf8("horizontalLayout_13"))
        self.hsRange = QtGui.QToolButton(self.scrollAreaWidgetContents_4)
        self.hsRange.setStyleSheet(_fromUtf8("QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     font: bold; \n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 rgb(108, 183, 255) );\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        self.hsRange.setCheckable(True)
        self.hsRange.setObjectName(_fromUtf8("hsRange"))
        self.horizontalLayout_13.addWidget(self.hsRange)
        spacerItem4 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_13.addItem(spacerItem4)
        self.RangeSpinBox = QtGui.QDoubleSpinBox(self.scrollAreaWidgetContents_4)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.MinimumExpanding, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.RangeSpinBox.sizePolicy().hasHeightForWidth())
        self.RangeSpinBox.setSizePolicy(sizePolicy)
        self.RangeSpinBox.setAutoFillBackground(True)
        self.RangeSpinBox.setStyleSheet(_fromUtf8("background: rgb(231, 231, 231);"))
        self.RangeSpinBox.setWrapping(True)
        self.RangeSpinBox.setAlignment(QtCore.Qt.AlignCenter)
        self.RangeSpinBox.setButtonSymbols(QtGui.QAbstractSpinBox.UpDownArrows)
        self.RangeSpinBox.setAccelerated(True)
        self.RangeSpinBox.setMinimum(-10000.0)
        self.RangeSpinBox.setMaximum(27536977.99)
        self.RangeSpinBox.setProperty(_fromUtf8("value"), 10000.0)
        self.RangeSpinBox.setObjectName(_fromUtf8("RangeSpinBox"))
        self.horizontalLayout_13.addWidget(self.RangeSpinBox)
        self.label = QtGui.QLabel(self.scrollAreaWidgetContents_4)
        self.label.setObjectName(_fromUtf8("label"))
        self.horizontalLayout_13.addWidget(self.label)
        self.RangeStepSpinBox = QtGui.QDoubleSpinBox(self.scrollAreaWidgetContents_4)
        self.RangeStepSpinBox.setStyleSheet(_fromUtf8("background: rgb(231, 231, 231);"))
        self.RangeStepSpinBox.setMinimum(0.0)
        self.RangeStepSpinBox.setMaximum(100000.0)
        self.RangeStepSpinBox.setSingleStep(1.0)
        self.RangeStepSpinBox.setProperty(_fromUtf8("value"), 100.0)
        self.RangeStepSpinBox.setObjectName(_fromUtf8("RangeStepSpinBox"))
        self.horizontalLayout_13.addWidget(self.RangeStepSpinBox)
        self.label_8 = QtGui.QLabel(self.scrollAreaWidgetContents_4)
        self.label_8.setObjectName(_fromUtf8("label_8"))
        self.horizontalLayout_13.addWidget(self.label_8)
        self.RangeMultipler = QtGui.QSpinBox(self.scrollAreaWidgetContents_4)
        self.RangeMultipler.setStyleSheet(_fromUtf8("background: rgb(231, 231, 231);"))
        self.RangeMultipler.setObjectName(_fromUtf8("RangeMultipler"))
        self.horizontalLayout_13.addWidget(self.RangeMultipler)
        self.verticalLayout_5.addLayout(self.horizontalLayout_13)
        self.RangeSlider = QtGui.QSlider(self.scrollAreaWidgetContents_4)
        self.RangeSlider.setMinimum(-10000)
        self.RangeSlider.setMaximum(27536977)
        self.RangeSlider.setOrientation(QtCore.Qt.Horizontal)
        self.RangeSlider.setTickPosition(QtGui.QSlider.TicksBothSides)
        self.RangeSlider.setTickInterval(1000000)
        self.RangeSlider.setObjectName(_fromUtf8("RangeSlider"))
        self.verticalLayout_5.addWidget(self.RangeSlider)
        self.line_4 = QtGui.QFrame(self.scrollAreaWidgetContents_4)
        self.line_4.setFrameShape(QtGui.QFrame.HLine)
        self.line_4.setFrameShadow(QtGui.QFrame.Sunken)
        self.line_4.setObjectName(_fromUtf8("line_4"))
        self.verticalLayout_5.addWidget(self.line_4)
        spacerItem5 = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout_5.addItem(spacerItem5)
        self.scrollArea_4.setWidget(self.scrollAreaWidgetContents_4)
        self.verticalLayout.addWidget(self.scrollArea_4)
        self.tabWidget.addTab(self.tab, _fromUtf8(""))
        self.tab_2 = QtGui.QWidget()
        self.tab_2.setObjectName(_fromUtf8("tab_2"))
        self.verticalLayout_2 = QtGui.QVBoxLayout(self.tab_2)
        self.verticalLayout_2.setObjectName(_fromUtf8("verticalLayout_2"))
        self.scrollArea_3 = QtGui.QScrollArea(self.tab_2)
        self.scrollArea_3.setWidgetResizable(True)
        self.scrollArea_3.setObjectName(_fromUtf8("scrollArea_3"))
        self.scrollAreaWidgetContents_3 = QtGui.QWidget()
        self.scrollAreaWidgetContents_3.setGeometry(QtCore.QRect(0, -89, 541, 308))
        self.scrollAreaWidgetContents_3.setObjectName(_fromUtf8("scrollAreaWidgetContents_3"))
        self.verticalLayout_19 = QtGui.QVBoxLayout(self.scrollAreaWidgetContents_3)
        self.verticalLayout_19.setObjectName(_fromUtf8("verticalLayout_19"))
        self.frame = QtGui.QFrame(self.scrollAreaWidgetContents_3)
        self.frame.setObjectName(_fromUtf8("frame"))
        self.horizontalLayout_14 = QtGui.QHBoxLayout(self.frame)
        self.horizontalLayout_14.setObjectName(_fromUtf8("horizontalLayout_14"))
        self.addRlayer = QtGui.QToolButton(self.frame)
        font = QtGui.QFont()
        font.setWeight(75)
        font.setBold(True)
        self.addRlayer.setFont(font)
        self.addRlayer.setStyleSheet(_fromUtf8("QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     font: bold; \n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 rgb(108, 183, 255) );\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        icon2 = QtGui.QIcon()
        icon2.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/element-cell.gif")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.addRlayer.setIcon(icon2)
        self.addRlayer.setObjectName(_fromUtf8("addRlayer"))
        self.horizontalLayout_14.addWidget(self.addRlayer)
        self.removeRlayer = QtGui.QToolButton(self.frame)
        font = QtGui.QFont()
        font.setWeight(75)
        font.setBold(True)
        self.removeRlayer.setFont(font)
        self.removeRlayer.setStyleSheet(_fromUtf8("QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     font: bold; \n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 rgb(108, 183, 255) );\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        icon3 = QtGui.QIcon()
        icon3.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/element-cell-delete.gif")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.removeRlayer.setIcon(icon3)
        self.removeRlayer.setObjectName(_fromUtf8("removeRlayer"))
        self.horizontalLayout_14.addWidget(self.removeRlayer)
        self.GrassRLayer = QtGui.QComboBox(self.frame)
        self.GrassRLayer.setObjectName(_fromUtf8("GrassRLayer"))
        self.horizontalLayout_14.addWidget(self.GrassRLayer)
        self.line_13 = QtGui.QFrame(self.frame)
        self.line_13.setFrameShape(QtGui.QFrame.VLine)
        self.line_13.setFrameShadow(QtGui.QFrame.Sunken)
        self.line_13.setObjectName(_fromUtf8("line_13"))
        self.horizontalLayout_14.addWidget(self.line_13)
        self.refreshlayerlist = QtGui.QToolButton(self.frame)
        self.refreshlayerlist.setStyleSheet(_fromUtf8("QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     font: bold; \n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 rgb(108, 183, 255) );\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        self.refreshlayerlist.setText(_fromUtf8(""))
        self.refreshlayerlist.setIcon(icon1)
        self.refreshlayerlist.setObjectName(_fromUtf8("refreshlayerlist"))
        self.horizontalLayout_14.addWidget(self.refreshlayerlist)
        self.line_12 = QtGui.QFrame(self.frame)
        self.line_12.setFrameShape(QtGui.QFrame.VLine)
        self.line_12.setFrameShadow(QtGui.QFrame.Sunken)
        self.line_12.setObjectName(_fromUtf8("line_12"))
        self.horizontalLayout_14.addWidget(self.line_12)
        self.GrassVLayer = QtGui.QComboBox(self.frame)
        self.GrassVLayer.setObjectName(_fromUtf8("GrassVLayer"))
        self.horizontalLayout_14.addWidget(self.GrassVLayer)
        self.addVlayer = QtGui.QToolButton(self.frame)
        font = QtGui.QFont()
        font.setWeight(75)
        font.setBold(True)
        self.addVlayer.setFont(font)
        self.addVlayer.setStyleSheet(_fromUtf8("QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     font: bold; \n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 rgb(108, 183, 255) );\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        icon4 = QtGui.QIcon()
        icon4.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/element-vector.gif")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.addVlayer.setIcon(icon4)
        self.addVlayer.setObjectName(_fromUtf8("addVlayer"))
        self.horizontalLayout_14.addWidget(self.addVlayer)
        self.removeVlayer = QtGui.QToolButton(self.frame)
        font = QtGui.QFont()
        font.setPointSize(11)
        font.setWeight(75)
        font.setBold(True)
        self.removeVlayer.setFont(font)
        self.removeVlayer.setStyleSheet(_fromUtf8("QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     font: bold; \n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 rgb(108, 183, 255) );\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        icon5 = QtGui.QIcon()
        icon5.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/element-vector-remove.gif")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.removeVlayer.setIcon(icon5)
        self.removeVlayer.setObjectName(_fromUtf8("removeVlayer"))
        self.horizontalLayout_14.addWidget(self.removeVlayer)
        self.renderoptions = QtGui.QToolButton(self.frame)
        self.renderoptions.setMinimumSize(QtCore.QSize(20, 0))
        self.renderoptions.setMaximumSize(QtCore.QSize(30, 16777215))
        self.renderoptions.setStyleSheet(_fromUtf8("QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     font: bold; \n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 rgb(108, 183, 255) );\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        icon6 = QtGui.QIcon()
        icon6.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/gimp_0.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.renderoptions.setIcon(icon6)
        self.renderoptions.setCheckable(True)
        self.renderoptions.setObjectName(_fromUtf8("renderoptions"))
        self.horizontalLayout_14.addWidget(self.renderoptions)
        self.verticalLayout_19.addWidget(self.frame)
        self.grassvectoroption = QtGui.QFrame(self.scrollAreaWidgetContents_3)
        self.grassvectoroption.setObjectName(_fromUtf8("grassvectoroption"))
        self.grassvectoroptions = QtGui.QHBoxLayout(self.grassvectoroption)
        self.grassvectoroptions.setObjectName(_fromUtf8("grassvectoroptions"))
        self.setPenColor = QtGui.QToolButton(self.grassvectoroption)
        self.setPenColor.setStyleSheet(_fromUtf8("QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     font: bold; \n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 rgb(108, 183, 255) );\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        self.setPenColor.setObjectName(_fromUtf8("setPenColor"))
        self.grassvectoroptions.addWidget(self.setPenColor)
        self.PenColor = QtGui.QLineEdit(self.grassvectoroption)
        self.PenColor.setMaximumSize(QtCore.QSize(90, 16777215))
        self.PenColor.setObjectName(_fromUtf8("PenColor"))
        self.grassvectoroptions.addWidget(self.PenColor)
        self.setBrushColor = QtGui.QToolButton(self.grassvectoroption)
        self.setBrushColor.setStyleSheet(_fromUtf8("QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     font: bold; \n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 rgb(108, 183, 255) );\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        self.setBrushColor.setObjectName(_fromUtf8("setBrushColor"))
        self.grassvectoroptions.addWidget(self.setBrushColor)
        self.BrushColor = QtGui.QLineEdit(self.grassvectoroption)
        self.BrushColor.setMaximumSize(QtCore.QSize(90, 16777215))
        self.BrushColor.setObjectName(_fromUtf8("BrushColor"))
        self.grassvectoroptions.addWidget(self.BrushColor)
        self.PointSize = QtGui.QLineEdit(self.grassvectoroption)
        self.PointSize.setMinimumSize(QtCore.QSize(30, 0))
        self.PointSize.setMaximumSize(QtCore.QSize(40, 16777215))
        self.PointSize.setObjectName(_fromUtf8("PointSize"))
        self.grassvectoroptions.addWidget(self.PointSize)
        self.LineWidth = QtGui.QSpinBox(self.grassvectoroption)
        self.LineWidth.setMaximumSize(QtCore.QSize(40, 16777215))
        self.LineWidth.setProperty(_fromUtf8("value"), 2)
        self.LineWidth.setObjectName(_fromUtf8("LineWidth"))
        self.grassvectoroptions.addWidget(self.LineWidth)
        self.Fill = QtGui.QCheckBox(self.grassvectoroption)
        self.Fill.setObjectName(_fromUtf8("Fill"))
        self.grassvectoroptions.addWidget(self.Fill)
        self.Thickness = QtGui.QCheckBox(self.grassvectoroption)
        self.Thickness.setObjectName(_fromUtf8("Thickness"))
        self.grassvectoroptions.addWidget(self.Thickness)
        self.checkBox = QtGui.QCheckBox(self.grassvectoroption)
        self.checkBox.setObjectName(_fromUtf8("checkBox"))
        self.grassvectoroptions.addWidget(self.checkBox)
        self.verticalLayout_19.addWidget(self.grassvectoroption)
        self.splitter_3 = QtGui.QSplitter(self.scrollAreaWidgetContents_3)
        self.splitter_3.setOrientation(QtCore.Qt.Vertical)
        self.splitter_3.setObjectName(_fromUtf8("splitter_3"))
        self.horizontalLayoutWidget = QtGui.QWidget(self.splitter_3)
        self.horizontalLayoutWidget.setObjectName(_fromUtf8("horizontalLayoutWidget"))
        self.horizontalLayout_2 = QtGui.QHBoxLayout(self.horizontalLayoutWidget)
        self.horizontalLayout_2.setMargin(0)
        self.horizontalLayout_2.setObjectName(_fromUtf8("horizontalLayout_2"))
        self.splitter_2 = QtGui.QSplitter(self.horizontalLayoutWidget)
        self.splitter_2.setOrientation(QtCore.Qt.Horizontal)
        self.splitter_2.setObjectName(_fromUtf8("splitter_2"))
        self.groupBox_2 = QtGui.QGroupBox(self.splitter_2)
        self.groupBox_2.setTitle(_fromUtf8(""))
        self.groupBox_2.setObjectName(_fromUtf8("groupBox_2"))
        self.verticalLayout_4 = QtGui.QVBoxLayout(self.groupBox_2)
        self.verticalLayout_4.setObjectName(_fromUtf8("verticalLayout_4"))
        self.frame1 = QtGui.QFrame(self.groupBox_2)
        self.frame1.setMaximumSize(QtCore.QSize(16777215, 58))
        self.frame1.setObjectName(_fromUtf8("frame1"))
        self.grassquery = QtGui.QHBoxLayout(self.frame1)
        self.grassquery.setObjectName(_fromUtf8("grassquery"))
        self.update = QtGui.QToolButton(self.frame1)
        self.update.setStyleSheet(_fromUtf8("QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     font: bold; \n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 rgb(108, 183, 255) );\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        icon7 = QtGui.QIcon()
        icon7.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/grass_icon.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.update.setIcon(icon7)
        self.update.setObjectName(_fromUtf8("update"))
        self.grassquery.addWidget(self.update)
        self.getlisttoquery = QtGui.QToolButton(self.frame1)
        self.getlisttoquery.setMinimumSize(QtCore.QSize(0, 20))
        self.getlisttoquery.setMaximumSize(QtCore.QSize(23, 23))
        self.getlisttoquery.setStyleSheet(_fromUtf8("QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     font: bold; \n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 rgb(108, 183, 255) );\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        self.getlisttoquery.setText(_fromUtf8(""))
        icon8 = QtGui.QIcon()
        icon8.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/gui-help.gif")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.getlisttoquery.setIcon(icon8)
        self.getlisttoquery.setObjectName(_fromUtf8("getlisttoquery"))
        self.grassquery.addWidget(self.getlisttoquery)
        self.longitude = QtGui.QLineEdit(self.frame1)
        self.longitude.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.longitude.setObjectName(_fromUtf8("longitude"))
        self.grassquery.addWidget(self.longitude)
        self.latitude = QtGui.QLineEdit(self.frame1)
        self.latitude.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.latitude.setObjectName(_fromUtf8("latitude"))
        self.grassquery.addWidget(self.latitude)
        self.gcmd = QtGui.QComboBox(self.frame1)
        self.gcmd.setObjectName(_fromUtf8("gcmd"))
        self.grassquery.addWidget(self.gcmd)
        self.gcmdexec = QtGui.QToolButton(self.frame1)
        self.gcmdexec.setStyleSheet(_fromUtf8("QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     font: bold; \n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 rgb(108, 183, 255) );\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        self.gcmdexec.setIcon(icon7)
        self.gcmdexec.setCheckable(True)
        self.gcmdexec.setObjectName(_fromUtf8("gcmdexec"))
        self.grassquery.addWidget(self.gcmdexec)
        self.verticalLayout_4.addWidget(self.frame1)
        self.splitter_5 = QtGui.QSplitter(self.groupBox_2)
        self.splitter_5.setOrientation(QtCore.Qt.Horizontal)
        self.splitter_5.setObjectName(_fromUtf8("splitter_5"))
        self.tableWidget = QtGui.QTableWidget(self.splitter_5)
        self.tableWidget.setObjectName(_fromUtf8("tableWidget"))
        self.tableWidget.setColumnCount(2)
        self.tableWidget.setRowCount(0)
        item = QtGui.QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(0, item)
        item.setIcon(icon2)
        item = QtGui.QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(1, item)
        item.setIcon(icon4)
        self.QueryResultsRaster = QtGui.QTextEdit(self.splitter_5)
        self.QueryResultsRaster.setObjectName(_fromUtf8("QueryResultsRaster"))
        self.QueryResultsVector = QtGui.QTextEdit(self.splitter_5)
        self.QueryResultsVector.setObjectName(_fromUtf8("QueryResultsVector"))
        self.verticalLayout_4.addWidget(self.splitter_5)
        self.horizontalLayout_2.addWidget(self.splitter_2)
        self.verticalLayout_19.addWidget(self.splitter_3)
        self.scrollArea_3.setWidget(self.scrollAreaWidgetContents_3)
        self.verticalLayout_2.addWidget(self.scrollArea_3)
        self.tabWidget.addTab(self.tab_2, _fromUtf8(""))
        self.tab_4 = QtGui.QWidget()
        self.tab_4.setObjectName(_fromUtf8("tab_4"))
        self.verticalLayout_16 = QtGui.QVBoxLayout(self.tab_4)
        self.verticalLayout_16.setObjectName(_fromUtf8("verticalLayout_16"))
        self.splitter = QtGui.QSplitter(self.tab_4)
        self.splitter.setOrientation(QtCore.Qt.Horizontal)
        self.splitter.setObjectName(_fromUtf8("splitter"))
        self.GpsGroupBox_2 = QtGui.QGroupBox(self.splitter)
        self.GpsGroupBox_2.setTitle(_fromUtf8(""))
        self.GpsGroupBox_2.setObjectName(_fromUtf8("GpsGroupBox_2"))
        self.verticalLayout_17 = QtGui.QVBoxLayout(self.GpsGroupBox_2)
        self.verticalLayout_17.setObjectName(_fromUtf8("verticalLayout_17"))
        self.tabWidget_2 = QtGui.QTabWidget(self.GpsGroupBox_2)
        self.tabWidget_2.setStyleSheet(_fromUtf8("background: rgb(231, 231, 231);"))
        self.tabWidget_2.setTabPosition(QtGui.QTabWidget.West)
        self.tabWidget_2.setTabShape(QtGui.QTabWidget.Rounded)
        self.tabWidget_2.setDocumentMode(True)
        self.tabWidget_2.setTabsClosable(False)
        self.tabWidget_2.setMovable(False)
        self.tabWidget_2.setObjectName(_fromUtf8("tabWidget_2"))
        self.tab_7 = QtGui.QWidget()
        self.tab_7.setObjectName(_fromUtf8("tab_7"))
        self.verticalLayout_10 = QtGui.QVBoxLayout(self.tab_7)
        self.verticalLayout_10.setObjectName(_fromUtf8("verticalLayout_10"))
        self.WebMap = QtWebKit.QWebView(self.tab_7)
        self.WebMap.setUrl(QtCore.QUrl(_fromUtf8("about:blank")))
        self.WebMap.setObjectName(_fromUtf8("WebMap"))
        self.verticalLayout_10.addWidget(self.WebMap)
        self.tabWidget_2.addTab(self.tab_7, _fromUtf8(""))
        self.tab_8 = QtGui.QWidget()
        self.tab_8.setObjectName(_fromUtf8("tab_8"))
        self.verticalLayout_11 = QtGui.QVBoxLayout(self.tab_8)
        self.verticalLayout_11.setObjectName(_fromUtf8("verticalLayout_11"))
        self.Satellite = QtGui.QTextBrowser(self.tab_8)
        self.Satellite.setObjectName(_fromUtf8("Satellite"))
        self.verticalLayout_11.addWidget(self.Satellite)
        self.tabWidget_2.addTab(self.tab_8, _fromUtf8(""))
        self.verticalLayout_17.addWidget(self.tabWidget_2)
        self.gridLayout_6 = QtGui.QGridLayout()
        self.gridLayout_6.setObjectName(_fromUtf8("gridLayout_6"))
        self.Latlabel_2 = QtGui.QLabel(self.GpsGroupBox_2)
        self.Latlabel_2.setStyleSheet(_fromUtf8(" QFrame, QLabel, QToolTip {\n"
"     border: 1px solid gray;\n"
"     border-radius: 4px;\n"
"     padding: 2px;\n"
"     background-image: url(images/welcome.png);\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Latlabel_2.setObjectName(_fromUtf8("Latlabel_2"))
        self.gridLayout_6.addWidget(self.Latlabel_2, 0, 0, 1, 1)
        self.Lonlabel_2 = QtGui.QLabel(self.GpsGroupBox_2)
        self.Lonlabel_2.setStyleSheet(_fromUtf8(" QFrame, QLabel, QToolTip {\n"
"     border: 1px solid gray;\n"
"     border-radius: 4px;\n"
"     padding: 2px;\n"
"     background-image: url(images/welcome.png);\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Lonlabel_2.setFrameShape(QtGui.QFrame.NoFrame)
        self.Lonlabel_2.setFrameShadow(QtGui.QFrame.Plain)
        self.Lonlabel_2.setObjectName(_fromUtf8("Lonlabel_2"))
        self.gridLayout_6.addWidget(self.Lonlabel_2, 0, 2, 1, 1)
        self.GPSlon_2 = QtGui.QLineEdit(self.GpsGroupBox_2)
        self.GPSlon_2.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
""))
        self.GPSlon_2.setAlignment(QtCore.Qt.AlignCenter)
        self.GPSlon_2.setReadOnly(True)
        self.GPSlon_2.setObjectName(_fromUtf8("GPSlon_2"))
        self.gridLayout_6.addWidget(self.GPSlon_2, 0, 3, 1, 1)
        self.GPSlat_2 = QtGui.QLineEdit(self.GpsGroupBox_2)
        self.GPSlat_2.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
""))
        self.GPSlat_2.setEchoMode(QtGui.QLineEdit.Normal)
        self.GPSlat_2.setAlignment(QtCore.Qt.AlignCenter)
        self.GPSlat_2.setReadOnly(True)
        self.GPSlat_2.setObjectName(_fromUtf8("GPSlat_2"))
        self.gridLayout_6.addWidget(self.GPSlat_2, 0, 1, 1, 1)
        self.verticalLayout_17.addLayout(self.gridLayout_6)
        self.groupBox = QtGui.QGroupBox(self.splitter)
        self.groupBox.setTitle(_fromUtf8(""))
        self.groupBox.setObjectName(_fromUtf8("groupBox"))
        self.verticalLayout_15 = QtGui.QVBoxLayout(self.groupBox)
        self.verticalLayout_15.setObjectName(_fromUtf8("verticalLayout_15"))
        self.scrollArea_2 = QtGui.QScrollArea(self.groupBox)
        self.scrollArea_2.setWidgetResizable(True)
        self.scrollArea_2.setObjectName(_fromUtf8("scrollArea_2"))
        self.scrollAreaWidgetContents = QtGui.QWidget()
        self.scrollAreaWidgetContents.setGeometry(QtCore.QRect(0, 0, 215, 173))
        self.scrollAreaWidgetContents.setObjectName(_fromUtf8("scrollAreaWidgetContents"))
        self.verticalLayout_9 = QtGui.QVBoxLayout(self.scrollAreaWidgetContents)
        self.verticalLayout_9.setObjectName(_fromUtf8("verticalLayout_9"))
        self.verticalLayout_6 = QtGui.QVBoxLayout()
        self.verticalLayout_6.setObjectName(_fromUtf8("verticalLayout_6"))
        self.scrollArea = QtGui.QScrollArea(self.scrollAreaWidgetContents)
        self.scrollArea.setStyleSheet(_fromUtf8(""))
        self.scrollArea.setWidgetResizable(True)
        self.scrollArea.setObjectName(_fromUtf8("scrollArea"))
        self.scrollAreaWidgetContents_2 = QtGui.QWidget()
        self.scrollAreaWidgetContents_2.setGeometry(QtCore.QRect(0, 0, 172, 560))
        self.scrollAreaWidgetContents_2.setObjectName(_fromUtf8("scrollAreaWidgetContents_2"))
        self.verticalLayout_12 = QtGui.QVBoxLayout(self.scrollAreaWidgetContents_2)
        self.verticalLayout_12.setObjectName(_fromUtf8("verticalLayout_12"))
        self.gridLayout_8 = QtGui.QGridLayout()
        self.gridLayout_8.setObjectName(_fromUtf8("gridLayout_8"))
        self.Datelabel = QtGui.QLabel(self.scrollAreaWidgetContents_2)
        self.Datelabel.setStyleSheet(_fromUtf8(" QFrame, QLabel, QToolTip {\n"
"     border: 1px solid gray;\n"
"     border-radius: 4px;\n"
"     padding: 2px;\n"
"     background-image: url(images/welcome.png);\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Datelabel.setAlignment(QtCore.Qt.AlignCenter)
        self.Datelabel.setObjectName(_fromUtf8("Datelabel"))
        self.gridLayout_8.addWidget(self.Datelabel, 0, 0, 1, 1)
        self.GPSTime = QtGui.QLineEdit(self.scrollAreaWidgetContents_2)
        self.GPSTime.setAutoFillBackground(False)
        self.GPSTime.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
"\n"
"\n"
"\n"
""))
        self.GPSTime.setAlignment(QtCore.Qt.AlignCenter)
        self.GPSTime.setReadOnly(True)
        self.GPSTime.setObjectName(_fromUtf8("GPSTime"))
        self.gridLayout_8.addWidget(self.GPSTime, 0, 1, 1, 1)
        self.Utclabel = QtGui.QLabel(self.scrollAreaWidgetContents_2)
        self.Utclabel.setStyleSheet(_fromUtf8(" QFrame, QLabel, QToolTip {\n"
"     border: 1px solid gray;\n"
"     border-radius: 4px;\n"
"     padding: 2px;\n"
"     background-image: url(images/welcome.png);\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Utclabel.setAlignment(QtCore.Qt.AlignCenter)
        self.Utclabel.setObjectName(_fromUtf8("Utclabel"))
        self.gridLayout_8.addWidget(self.Utclabel, 1, 0, 1, 1)
        self.GPSUtctime = QtGui.QLineEdit(self.scrollAreaWidgetContents_2)
        self.GPSUtctime.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
"\n"
"\n"
"\n"
""))
        self.GPSUtctime.setAlignment(QtCore.Qt.AlignCenter)
        self.GPSUtctime.setReadOnly(True)
        self.GPSUtctime.setObjectName(_fromUtf8("GPSUtctime"))
        self.gridLayout_8.addWidget(self.GPSUtctime, 1, 1, 1, 1)
        self.Altitudelabel = QtGui.QLabel(self.scrollAreaWidgetContents_2)
        self.Altitudelabel.setStyleSheet(_fromUtf8(" QFrame, QLabel, QToolTip {\n"
"     border: 1px solid gray;\n"
"     border-radius: 4px;\n"
"     padding: 2px;\n"
"     background-image: url(images/welcome.png);\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Altitudelabel.setAlignment(QtCore.Qt.AlignCenter)
        self.Altitudelabel.setObjectName(_fromUtf8("Altitudelabel"))
        self.gridLayout_8.addWidget(self.Altitudelabel, 2, 0, 1, 1)
        self.GPSAltitude = QtGui.QLineEdit(self.scrollAreaWidgetContents_2)
        self.GPSAltitude.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
"\n"
"\n"
"\n"
""))
        self.GPSAltitude.setAlignment(QtCore.Qt.AlignCenter)
        self.GPSAltitude.setReadOnly(True)
        self.GPSAltitude.setObjectName(_fromUtf8("GPSAltitude"))
        self.gridLayout_8.addWidget(self.GPSAltitude, 2, 1, 1, 1)
        self.Speedlabel = QtGui.QLabel(self.scrollAreaWidgetContents_2)
        self.Speedlabel.setStyleSheet(_fromUtf8(" QFrame, QLabel, QToolTip {\n"
"     border: 1px solid gray;\n"
"     border-radius: 4px;\n"
"     padding: 2px;\n"
"     background-image: url(images/welcome.png);\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Speedlabel.setAlignment(QtCore.Qt.AlignCenter)
        self.Speedlabel.setObjectName(_fromUtf8("Speedlabel"))
        self.gridLayout_8.addWidget(self.Speedlabel, 3, 0, 1, 1)
        self.GPSSpeed = QtGui.QLineEdit(self.scrollAreaWidgetContents_2)
        self.GPSSpeed.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
"\n"
"\n"
"\n"
""))
        self.GPSSpeed.setAlignment(QtCore.Qt.AlignCenter)
        self.GPSSpeed.setReadOnly(True)
        self.GPSSpeed.setObjectName(_fromUtf8("GPSSpeed"))
        self.gridLayout_8.addWidget(self.GPSSpeed, 3, 1, 1, 1)
        self.Climblabel = QtGui.QLabel(self.scrollAreaWidgetContents_2)
        self.Climblabel.setStyleSheet(_fromUtf8(" QFrame, QLabel, QToolTip {\n"
"     border: 1px solid gray;\n"
"     border-radius: 4px;\n"
"     padding: 2px;\n"
"     background-image: url(images/welcome.png);\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Climblabel.setAlignment(QtCore.Qt.AlignCenter)
        self.Climblabel.setObjectName(_fromUtf8("Climblabel"))
        self.gridLayout_8.addWidget(self.Climblabel, 5, 0, 1, 1)
        self.GPSClimb = QtGui.QLineEdit(self.scrollAreaWidgetContents_2)
        self.GPSClimb.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
"\n"
"\n"
"\n"
""))
        self.GPSClimb.setAlignment(QtCore.Qt.AlignCenter)
        self.GPSClimb.setReadOnly(True)
        self.GPSClimb.setObjectName(_fromUtf8("GPSClimb"))
        self.gridLayout_8.addWidget(self.GPSClimb, 5, 1, 1, 1)
        self.Pdoplabel = QtGui.QLabel(self.scrollAreaWidgetContents_2)
        self.Pdoplabel.setStyleSheet(_fromUtf8(" QFrame, QLabel, QToolTip {\n"
"     border: 1px solid gray;\n"
"     border-radius: 4px;\n"
"     padding: 2px;\n"
"     background-image: url(images/welcome.png);\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Pdoplabel.setAlignment(QtCore.Qt.AlignCenter)
        self.Pdoplabel.setObjectName(_fromUtf8("Pdoplabel"))
        self.gridLayout_8.addWidget(self.Pdoplabel, 6, 0, 1, 1)
        self.GPSPdop = QtGui.QLineEdit(self.scrollAreaWidgetContents_2)
        self.GPSPdop.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
"\n"
"\n"
"\n"
""))
        self.GPSPdop.setAlignment(QtCore.Qt.AlignCenter)
        self.GPSPdop.setReadOnly(True)
        self.GPSPdop.setObjectName(_fromUtf8("GPSPdop"))
        self.gridLayout_8.addWidget(self.GPSPdop, 6, 1, 1, 1)
        self.Hdoplabel = QtGui.QLabel(self.scrollAreaWidgetContents_2)
        self.Hdoplabel.setStyleSheet(_fromUtf8(" QFrame, QLabel, QToolTip {\n"
"     border: 1px solid gray;\n"
"     border-radius: 4px;\n"
"     padding: 2px;\n"
"     background-image: url(images/welcome.png);\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Hdoplabel.setAlignment(QtCore.Qt.AlignCenter)
        self.Hdoplabel.setObjectName(_fromUtf8("Hdoplabel"))
        self.gridLayout_8.addWidget(self.Hdoplabel, 7, 0, 1, 1)
        self.GPSHdop = QtGui.QLineEdit(self.scrollAreaWidgetContents_2)
        self.GPSHdop.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
"\n"
"\n"
"\n"
""))
        self.GPSHdop.setAlignment(QtCore.Qt.AlignCenter)
        self.GPSHdop.setReadOnly(True)
        self.GPSHdop.setObjectName(_fromUtf8("GPSHdop"))
        self.gridLayout_8.addWidget(self.GPSHdop, 7, 1, 1, 1)
        self.Vdoplabel = QtGui.QLabel(self.scrollAreaWidgetContents_2)
        self.Vdoplabel.setStyleSheet(_fromUtf8(" QFrame, QLabel, QToolTip {\n"
"     border: 1px solid gray;\n"
"     border-radius: 4px;\n"
"     padding: 2px;\n"
"     background-image: url(images/welcome.png);\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Vdoplabel.setAlignment(QtCore.Qt.AlignCenter)
        self.Vdoplabel.setObjectName(_fromUtf8("Vdoplabel"))
        self.gridLayout_8.addWidget(self.Vdoplabel, 8, 0, 1, 1)
        self.GPSVdop = QtGui.QLineEdit(self.scrollAreaWidgetContents_2)
        self.GPSVdop.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
"\n"
"\n"
"\n"
""))
        self.GPSVdop.setAlignment(QtCore.Qt.AlignCenter)
        self.GPSVdop.setReadOnly(True)
        self.GPSVdop.setObjectName(_fromUtf8("GPSVdop"))
        self.gridLayout_8.addWidget(self.GPSVdop, 8, 1, 1, 1)
        self.Tdoplabel = QtGui.QLabel(self.scrollAreaWidgetContents_2)
        self.Tdoplabel.setStyleSheet(_fromUtf8(" QFrame, QLabel, QToolTip {\n"
"     border: 1px solid gray;\n"
"     border-radius: 4px;\n"
"     padding: 2px;\n"
"     background-image: url(images/welcome.png);\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Tdoplabel.setAlignment(QtCore.Qt.AlignCenter)
        self.Tdoplabel.setObjectName(_fromUtf8("Tdoplabel"))
        self.gridLayout_8.addWidget(self.Tdoplabel, 9, 0, 1, 1)
        self.GPSTdop = QtGui.QLineEdit(self.scrollAreaWidgetContents_2)
        self.GPSTdop.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
"\n"
"\n"
"\n"
""))
        self.GPSTdop.setAlignment(QtCore.Qt.AlignCenter)
        self.GPSTdop.setReadOnly(True)
        self.GPSTdop.setObjectName(_fromUtf8("GPSTdop"))
        self.gridLayout_8.addWidget(self.GPSTdop, 9, 1, 1, 1)
        self.Gdoplabel = QtGui.QLabel(self.scrollAreaWidgetContents_2)
        self.Gdoplabel.setStyleSheet(_fromUtf8(" QFrame, QLabel, QToolTip {\n"
"     border: 1px solid gray;\n"
"     border-radius: 4px;\n"
"     padding: 2px;\n"
"     background-image: url(images/welcome.png);\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Gdoplabel.setAlignment(QtCore.Qt.AlignCenter)
        self.Gdoplabel.setObjectName(_fromUtf8("Gdoplabel"))
        self.gridLayout_8.addWidget(self.Gdoplabel, 10, 0, 1, 1)
        self.GPSGdop = QtGui.QLineEdit(self.scrollAreaWidgetContents_2)
        self.GPSGdop.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
"\n"
"\n"
"\n"
""))
        self.GPSGdop.setAlignment(QtCore.Qt.AlignCenter)
        self.GPSGdop.setReadOnly(True)
        self.GPSGdop.setObjectName(_fromUtf8("GPSGdop"))
        self.gridLayout_8.addWidget(self.GPSGdop, 10, 1, 1, 1)
        self.Ephlabel = QtGui.QLabel(self.scrollAreaWidgetContents_2)
        self.Ephlabel.setStyleSheet(_fromUtf8(" QFrame, QLabel, QToolTip {\n"
"     border: 1px solid gray;\n"
"     border-radius: 4px;\n"
"     padding: 2px;\n"
"     background-image: url(images/welcome.png);\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Ephlabel.setAlignment(QtCore.Qt.AlignCenter)
        self.Ephlabel.setObjectName(_fromUtf8("Ephlabel"))
        self.gridLayout_8.addWidget(self.Ephlabel, 11, 0, 1, 1)
        self.GPSEph = QtGui.QLineEdit(self.scrollAreaWidgetContents_2)
        self.GPSEph.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
"\n"
"\n"
"\n"
""))
        self.GPSEph.setAlignment(QtCore.Qt.AlignCenter)
        self.GPSEph.setReadOnly(True)
        self.GPSEph.setObjectName(_fromUtf8("GPSEph"))
        self.gridLayout_8.addWidget(self.GPSEph, 11, 1, 1, 1)
        self.Eptlabel = QtGui.QLabel(self.scrollAreaWidgetContents_2)
        self.Eptlabel.setStyleSheet(_fromUtf8(" QFrame, QLabel, QToolTip {\n"
"     border: 1px solid gray;\n"
"     border-radius: 4px;\n"
"     padding: 2px;\n"
"     background-image: url(images/welcome.png);\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Eptlabel.setAlignment(QtCore.Qt.AlignCenter)
        self.Eptlabel.setObjectName(_fromUtf8("Eptlabel"))
        self.gridLayout_8.addWidget(self.Eptlabel, 12, 0, 1, 1)
        self.GPSEpt = QtGui.QLineEdit(self.scrollAreaWidgetContents_2)
        self.GPSEpt.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
"\n"
"\n"
"\n"
""))
        self.GPSEpt.setAlignment(QtCore.Qt.AlignCenter)
        self.GPSEpt.setReadOnly(True)
        self.GPSEpt.setObjectName(_fromUtf8("GPSEpt"))
        self.gridLayout_8.addWidget(self.GPSEpt, 12, 1, 1, 1)
        self.Epslabel = QtGui.QLabel(self.scrollAreaWidgetContents_2)
        self.Epslabel.setStyleSheet(_fromUtf8(" QFrame, QLabel, QToolTip {\n"
"     border: 1px solid gray;\n"
"     border-radius: 4px;\n"
"     padding: 2px;\n"
"     background-image: url(images/welcome.png);\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Epslabel.setAlignment(QtCore.Qt.AlignCenter)
        self.Epslabel.setObjectName(_fromUtf8("Epslabel"))
        self.gridLayout_8.addWidget(self.Epslabel, 13, 0, 1, 1)
        self.GPSEps = QtGui.QLineEdit(self.scrollAreaWidgetContents_2)
        self.GPSEps.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
"\n"
"\n"
"\n"
""))
        self.GPSEps.setAlignment(QtCore.Qt.AlignCenter)
        self.GPSEps.setReadOnly(True)
        self.GPSEps.setObjectName(_fromUtf8("GPSEps"))
        self.gridLayout_8.addWidget(self.GPSEps, 13, 1, 1, 1)
        self.Epvlabel = QtGui.QLabel(self.scrollAreaWidgetContents_2)
        self.Epvlabel.setStyleSheet(_fromUtf8(" QFrame, QLabel, QToolTip {\n"
"     border: 1px solid gray;\n"
"     border-radius: 4px;\n"
"     padding: 2px;\n"
"     background-image: url(images/welcome.png);\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Epvlabel.setAlignment(QtCore.Qt.AlignCenter)
        self.Epvlabel.setObjectName(_fromUtf8("Epvlabel"))
        self.gridLayout_8.addWidget(self.Epvlabel, 14, 0, 1, 1)
        self.GPSEpv = QtGui.QLineEdit(self.scrollAreaWidgetContents_2)
        self.GPSEpv.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
"\n"
"\n"
"\n"
""))
        self.GPSEpv.setAlignment(QtCore.Qt.AlignCenter)
        self.GPSEpv.setReadOnly(True)
        self.GPSEpv.setObjectName(_fromUtf8("GPSEpv"))
        self.gridLayout_8.addWidget(self.GPSEpv, 14, 1, 1, 1)
        self.Epdlabel = QtGui.QLabel(self.scrollAreaWidgetContents_2)
        self.Epdlabel.setStyleSheet(_fromUtf8(" QFrame, QLabel, QToolTip {\n"
"     border: 1px solid gray;\n"
"     border-radius: 4px;\n"
"     padding: 2px;\n"
"     background-image: url(images/welcome.png);\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Epdlabel.setAlignment(QtCore.Qt.AlignCenter)
        self.Epdlabel.setObjectName(_fromUtf8("Epdlabel"))
        self.gridLayout_8.addWidget(self.Epdlabel, 15, 0, 1, 1)
        self.GPSEpd = QtGui.QLineEdit(self.scrollAreaWidgetContents_2)
        self.GPSEpd.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
"\n"
"\n"
"\n"
""))
        self.GPSEpd.setAlignment(QtCore.Qt.AlignCenter)
        self.GPSEpd.setReadOnly(True)
        self.GPSEpd.setObjectName(_fromUtf8("GPSEpd"))
        self.gridLayout_8.addWidget(self.GPSEpd, 15, 1, 1, 1)
        self.Epclabel = QtGui.QLabel(self.scrollAreaWidgetContents_2)
        self.Epclabel.setStyleSheet(_fromUtf8(" QFrame, QLabel, QToolTip {\n"
"     border: 1px solid gray;\n"
"     border-radius: 4px;\n"
"     padding: 2px;\n"
"     background-image: url(images/welcome.png);\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Epclabel.setAlignment(QtCore.Qt.AlignCenter)
        self.Epclabel.setObjectName(_fromUtf8("Epclabel"))
        self.gridLayout_8.addWidget(self.Epclabel, 16, 0, 1, 1)
        self.GPSEpc = QtGui.QLineEdit(self.scrollAreaWidgetContents_2)
        self.GPSEpc.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
"\n"
"\n"
"\n"
""))
        self.GPSEpc.setAlignment(QtCore.Qt.AlignCenter)
        self.GPSEpc.setReadOnly(True)
        self.GPSEpc.setObjectName(_fromUtf8("GPSEpc"))
        self.gridLayout_8.addWidget(self.GPSEpc, 16, 1, 1, 1)
        self.GPSTrack = QtGui.QLineEdit(self.scrollAreaWidgetContents_2)
        self.GPSTrack.setStyleSheet(_fromUtf8("QLineEdit {\n"
"     border: 1px solid gray;\n"
"     border-radius: 5px;\n"
"     padding: 0 8px;\n"
"     background: rgb(231, 231, 231);\n"
" }\n"
"\n"
"\n"
"\n"
""))
        self.GPSTrack.setAlignment(QtCore.Qt.AlignCenter)
        self.GPSTrack.setReadOnly(True)
        self.GPSTrack.setObjectName(_fromUtf8("GPSTrack"))
        self.gridLayout_8.addWidget(self.GPSTrack, 4, 1, 1, 1)
        self.Tracklabel = QtGui.QLabel(self.scrollAreaWidgetContents_2)
        self.Tracklabel.setStyleSheet(_fromUtf8(" QFrame, QLabel, QToolTip {\n"
"     border: 1px solid gray;\n"
"     border-radius: 4px;\n"
"     padding: 2px;\n"
"     background-image: url(images/welcome.png);\n"
"     background: rgb(231, 231, 231);\n"
" }"))
        self.Tracklabel.setAlignment(QtCore.Qt.AlignCenter)
        self.Tracklabel.setObjectName(_fromUtf8("Tracklabel"))
        self.gridLayout_8.addWidget(self.Tracklabel, 4, 0, 1, 1)
        self.verticalLayout_12.addLayout(self.gridLayout_8)
        self.scrollArea.setWidget(self.scrollAreaWidgetContents_2)
        self.verticalLayout_6.addWidget(self.scrollArea)
        self.horizontalLayout_9 = QtGui.QHBoxLayout()
        self.horizontalLayout_9.setObjectName(_fromUtf8("horizontalLayout_9"))
        self.GPSSend = QtGui.QCheckBox(self.scrollAreaWidgetContents)
        self.GPSSend.setStyleSheet(_fromUtf8(""))
        self.GPSSend.setObjectName(_fromUtf8("GPSSend"))
        self.horizontalLayout_9.addWidget(self.GPSSend)
        self.StartGps = QtGui.QPushButton(self.scrollAreaWidgetContents)
        self.StartGps.setStyleSheet(_fromUtf8("QPushButton {\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
"     min-width: 80px;\n"
" }\n"
"\n"
" QPushButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QPushButton:flat {\n"
"     border: none; /* no border for a flat push button */\n"
" }\n"
"\n"
" QPushButton:default {\n"
"     border-color: navy; /* make the default button prominent */\n"
" }"))
        self.StartGps.setCheckable(True)
        self.StartGps.setObjectName(_fromUtf8("StartGps"))
        self.horizontalLayout_9.addWidget(self.StartGps)
        self.verticalLayout_6.addLayout(self.horizontalLayout_9)
        self.verticalLayout_9.addLayout(self.verticalLayout_6)
        self.scrollArea_2.setWidget(self.scrollAreaWidgetContents)
        self.verticalLayout_15.addWidget(self.scrollArea_2)
        self.verticalLayout_16.addWidget(self.splitter)
        self.tabWidget.addTab(self.tab_4, _fromUtf8(""))
        self.tab_3 = QtGui.QWidget()
        self.tab_3.setObjectName(_fromUtf8("tab_3"))
        self.verticalLayout_7 = QtGui.QVBoxLayout(self.tab_3)
        self.verticalLayout_7.setObjectName(_fromUtf8("verticalLayout_7"))
        self.textHW = QtGui.QTextEdit(self.tab_3)
        self.textHW.setObjectName(_fromUtf8("textHW"))
        self.verticalLayout_7.addWidget(self.textHW)
        self.tabWidget.addTab(self.tab_3, _fromUtf8(""))
        self.horizontalLayout.addWidget(self.tabWidget)
        OssimPlanetSasha.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(OssimPlanetSasha)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 744, 22))
        self.menubar.setObjectName(_fromUtf8("menubar"))
        self.menuSasha = QtGui.QMenu(self.menubar)
        self.menuSasha.setObjectName(_fromUtf8("menuSasha"))
        self.menuView_2 = QtGui.QMenu(self.menuSasha)
        self.menuView_2.setObjectName(_fromUtf8("menuView_2"))
        self.menuTools = QtGui.QMenu(self.menubar)
        self.menuTools.setObjectName(_fromUtf8("menuTools"))
        self.menuGPS = QtGui.QMenu(self.menuTools)
        self.menuGPS.setObjectName(_fromUtf8("menuGPS"))
        self.menuData = QtGui.QMenu(self.menuTools)
        self.menuData.setObjectName(_fromUtf8("menuData"))
        OssimPlanetSasha.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(OssimPlanetSasha)
        self.statusbar.setObjectName(_fromUtf8("statusbar"))
        OssimPlanetSasha.setStatusBar(self.statusbar)
        self.toolBar = QtGui.QToolBar(OssimPlanetSasha)
        self.toolBar.setObjectName(_fromUtf8("toolBar"))
        OssimPlanetSasha.addToolBar(QtCore.Qt.TopToolBarArea, self.toolBar)
        self.dockWidget = QtGui.QDockWidget(OssimPlanetSasha)
        self.dockWidget.setObjectName(_fromUtf8("dockWidget"))
        self.dockWidgetContents_2 = QtGui.QWidget()
        self.dockWidgetContents_2.setObjectName(_fromUtf8("dockWidgetContents_2"))
        self.verticalLayout_13 = QtGui.QVBoxLayout(self.dockWidgetContents_2)
        self.verticalLayout_13.setObjectName(_fromUtf8("verticalLayout_13"))
        spacerItem6 = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout_13.addItem(spacerItem6)
        self.gridLayout = QtGui.QGridLayout()
        self.gridLayout.setObjectName(_fromUtf8("gridLayout"))
        self.west = QtGui.QToolButton(self.dockWidgetContents_2)
        self.west.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 blue);\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        icon9 = QtGui.QIcon()
        icon9.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/shapeimage_5.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.west.setIcon(icon9)
        self.west.setObjectName(_fromUtf8("west"))
        self.gridLayout.addWidget(self.west, 7, 1, 1, 1)
        self.east = QtGui.QToolButton(self.dockWidgetContents_2)
        self.east.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 blue);\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        icon10 = QtGui.QIcon()
        icon10.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/shapeimage_4.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.east.setIcon(icon10)
        self.east.setObjectName(_fromUtf8("east"))
        self.gridLayout.addWidget(self.east, 7, 4, 1, 1)
        self.center = QtGui.QToolButton(self.dockWidgetContents_2)
        self.center.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 blue);\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        self.center.setText(_fromUtf8(""))
        icon11 = QtGui.QIcon()
        icon11.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/pan.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.center.setIcon(icon11)
        self.center.setObjectName(_fromUtf8("center"))
        self.gridLayout.addWidget(self.center, 7, 3, 1, 1)
        self.southwest = QtGui.QToolButton(self.dockWidgetContents_2)
        self.southwest.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 blue);\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        self.southwest.setText(_fromUtf8(""))
        icon12 = QtGui.QIcon()
        icon12.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/shapeimage_8.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.southwest.setIcon(icon12)
        self.southwest.setObjectName(_fromUtf8("southwest"))
        self.gridLayout.addWidget(self.southwest, 9, 1, 1, 1)
        self.south = QtGui.QToolButton(self.dockWidgetContents_2)
        self.south.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 blue);\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        icon13 = QtGui.QIcon()
        icon13.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/shapeimage_6.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.south.setIcon(icon13)
        self.south.setObjectName(_fromUtf8("south"))
        self.gridLayout.addWidget(self.south, 9, 3, 1, 1)
        self.southeast = QtGui.QToolButton(self.dockWidgetContents_2)
        self.southeast.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 blue);\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        self.southeast.setText(_fromUtf8(""))
        icon14 = QtGui.QIcon()
        icon14.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/shapeimage_10.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.southeast.setIcon(icon14)
        self.southeast.setObjectName(_fromUtf8("southeast"))
        self.gridLayout.addWidget(self.southeast, 9, 4, 1, 1)
        self.north = QtGui.QToolButton(self.dockWidgetContents_2)
        self.north.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 blue);\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        self.north.setText(_fromUtf8(""))
        icon15 = QtGui.QIcon()
        icon15.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/shapeimage_3.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.north.setIcon(icon15)
        self.north.setObjectName(_fromUtf8("north"))
        self.gridLayout.addWidget(self.north, 4, 3, 1, 1)
        self.northwest = QtGui.QToolButton(self.dockWidgetContents_2)
        self.northwest.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 blue);\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        self.northwest.setText(_fromUtf8(""))
        icon16 = QtGui.QIcon()
        icon16.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/shapeimage_9.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.northwest.setIcon(icon16)
        self.northwest.setObjectName(_fromUtf8("northwest"))
        self.gridLayout.addWidget(self.northwest, 4, 1, 1, 1)
        self.northeast = QtGui.QToolButton(self.dockWidgetContents_2)
        self.northeast.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 blue);\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        self.northeast.setText(_fromUtf8(""))
        icon17 = QtGui.QIcon()
        icon17.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/shapeimage_7.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.northeast.setIcon(icon17)
        self.northeast.setObjectName(_fromUtf8("northeast"))
        self.gridLayout.addWidget(self.northeast, 4, 4, 1, 1)
        self.zoomDell = QtGui.QToolButton(self.dockWidgetContents_2)
        self.zoomDell.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 blue);\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        self.zoomDell.setText(_fromUtf8(""))
        icon18 = QtGui.QIcon()
        icon18.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/zoom-out.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.zoomDell.setIcon(icon18)
        self.zoomDell.setObjectName(_fromUtf8("zoomDell"))
        self.gridLayout.addWidget(self.zoomDell, 10, 1, 1, 1)
        self.clview = QtGui.QToolButton(self.dockWidgetContents_2)
        self.clview.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 blue);\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        self.clview.setText(_fromUtf8(""))
        icon19 = QtGui.QIcon()
        icon19.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/600px-Brosen_windrose.svg.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.clview.setIcon(icon19)
        self.clview.setObjectName(_fromUtf8("clview"))
        self.gridLayout.addWidget(self.clview, 10, 3, 1, 1)
        self.zoomAdd = QtGui.QToolButton(self.dockWidgetContents_2)
        self.zoomAdd.setStyleSheet(_fromUtf8(" QToolButton { /* all types of tool button */\n"
"     border: 2px solid #8f8f91;\n"
"     border-radius: 6px;\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n"
" }\n"
"\n"
" QToolButton[popupMode=\"1\"] { /* only for MenuButtonPopup */\n"
"     padding-right: 20px; /* make way for the popup button */\n"
" }\n"
"\n"
" QToolButton:pressed {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n"
" }\n"
"\n"
" QToolButton:checked {\n"
"     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                       stop: 0 white, stop: 1 blue);\n"
" }\n"
"\n"
" /* the subcontrols below are used only in the MenuButtonPopup mode */\n"
" QToolButton::menu-button {\n"
"     border: 2px solid gray;\n"
"     border-top-right-radius: 6px;\n"
"     border-bottom-right-radius: 6px;\n"
"     /* 16px width + 4px for border = 20px allocated above */\n"
"     width: 16px;\n"
" }\n"
"\n"
" QToolButton::menu-arrow {\n"
"     image: url(downarrow.png);\n"
" }\n"
"\n"
" QToolButton::menu-arrow:open {\n"
"     top: 1px; left: 1px; /* shift it a bit */\n"
" }"))
        self.zoomAdd.setText(_fromUtf8(""))
        icon20 = QtGui.QIcon()
        icon20.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/zoom-in.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.zoomAdd.setIcon(icon20)
        self.zoomAdd.setObjectName(_fromUtf8("zoomAdd"))
        self.gridLayout.addWidget(self.zoomAdd, 10, 4, 1, 1)
        self.verticalLayout_13.addLayout(self.gridLayout)
        spacerItem7 = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout_13.addItem(spacerItem7)
        self.dockWidget.setWidget(self.dockWidgetContents_2)
        OssimPlanetSasha.addDockWidget(QtCore.Qt.DockWidgetArea(1), self.dockWidget)
        self.dockWidget_2 = QtGui.QDockWidget(OssimPlanetSasha)
        self.dockWidget_2.setObjectName(_fromUtf8("dockWidget_2"))
        self.dockWidgetContents_3 = QtGui.QWidget()
        self.dockWidgetContents_3.setObjectName(_fromUtf8("dockWidgetContents_3"))
        self.verticalLayout_21 = QtGui.QVBoxLayout(self.dockWidgetContents_3)
        self.verticalLayout_21.setObjectName(_fromUtf8("verticalLayout_21"))
        self.verticalLayout_8 = QtGui.QVBoxLayout()
        self.verticalLayout_8.setObjectName(_fromUtf8("verticalLayout_8"))
        self.verticalLayout_21.addLayout(self.verticalLayout_8)
        self.dockWidget_2.setWidget(self.dockWidgetContents_3)
        OssimPlanetSasha.addDockWidget(QtCore.Qt.DockWidgetArea(1), self.dockWidget_2)
        self.dockWidget_3 = QtGui.QDockWidget(OssimPlanetSasha)
        self.dockWidget_3.setObjectName(_fromUtf8("dockWidget_3"))
        self.dockWidgetContents = QtGui.QWidget()
        self.dockWidgetContents.setObjectName(_fromUtf8("dockWidgetContents"))
        self.verticalLayout_3 = QtGui.QVBoxLayout(self.dockWidgetContents)
        self.verticalLayout_3.setObjectName(_fromUtf8("verticalLayout_3"))
        self.verticalLayout_18 = QtGui.QVBoxLayout()
        self.verticalLayout_18.setObjectName(_fromUtf8("verticalLayout_18"))
        self.verticalLayout_3.addLayout(self.verticalLayout_18)
        self.dockWidget_3.setWidget(self.dockWidgetContents)
        OssimPlanetSasha.addDockWidget(QtCore.Qt.DockWidgetArea(1), self.dockWidget_3)
        self.actionGPS = QtGui.QAction(OssimPlanetSasha)
        self.actionGPS.setCheckable(True)
        icon21 = QtGui.QIcon()
        icon21.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/satellite.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionGPS.setIcon(icon21)
        self.actionGPS.setObjectName(_fromUtf8("actionGPS"))
        self.actionGrass = QtGui.QAction(OssimPlanetSasha)
        self.actionGrass.setCheckable(True)
        self.actionGrass.setIcon(icon7)
        self.actionGrass.setObjectName(_fromUtf8("actionGrass"))
        self.actionJoystick = QtGui.QAction(OssimPlanetSasha)
        self.actionJoystick.setCheckable(True)
        icon22 = QtGui.QIcon()
        icon22.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/joystick.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionJoystick.setIcon(icon22)
        self.actionJoystick.setObjectName(_fromUtf8("actionJoystick"))
        self.actionLonLat = QtGui.QAction(OssimPlanetSasha)
        self.actionLonLat.setCheckable(True)
        self.actionLonLat.setChecked(False)
        self.actionLonLat.setIcon(icon19)
        self.actionLonLat.setObjectName(_fromUtf8("actionLonLat"))
        self.actionExit = QtGui.QAction(OssimPlanetSasha)
        icon23 = QtGui.QIcon()
        icon23.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/quit.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionExit.setIcon(icon23)
        self.actionExit.setObjectName(_fromUtf8("actionExit"))
        self.actionDB_setting = QtGui.QAction(OssimPlanetSasha)
        icon24 = QtGui.QIcon()
        icon24.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/db.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionDB_setting.setIcon(icon24)
        self.actionDB_setting.setObjectName(_fromUtf8("actionDB_setting"))
        self.actionNMEA = QtGui.QAction(OssimPlanetSasha)
        self.actionNMEA.setObjectName(_fromUtf8("actionNMEA"))
        self.actionData_2 = QtGui.QAction(OssimPlanetSasha)
        self.actionData_2.setCheckable(False)
        icon25 = QtGui.QIcon()
        icon25.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/gui-mapzoom.gif")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionData_2.setIcon(icon25)
        self.actionData_2.setObjectName(_fromUtf8("actionData_2"))
        self.actionSavekml = QtGui.QAction(OssimPlanetSasha)
        icon26 = QtGui.QIcon()
        icon26.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/saveinfo.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionSavekml.setIcon(icon26)
        self.actionSavekml.setObjectName(_fromUtf8("actionSavekml"))
        self.actionEpsg = QtGui.QAction(OssimPlanetSasha)
        self.actionEpsg.setObjectName(_fromUtf8("actionEpsg"))
        self.actionHidesliders = QtGui.QAction(OssimPlanetSasha)
        self.actionHidesliders.setCheckable(True)
        self.actionHidesliders.setObjectName(_fromUtf8("actionHidesliders"))
        self.actionHideSpinbox = QtGui.QAction(OssimPlanetSasha)
        self.actionHideSpinbox.setCheckable(True)
        self.actionHideSpinbox.setObjectName(_fromUtf8("actionHideSpinbox"))
        self.actionHide_place_position = QtGui.QAction(OssimPlanetSasha)
        self.actionHide_place_position.setCheckable(True)
        self.actionHide_place_position.setObjectName(_fromUtf8("actionHide_place_position"))
        self.actionHideStepTool = QtGui.QAction(OssimPlanetSasha)
        self.actionHideStepTool.setCheckable(True)
        self.actionHideStepTool.setObjectName(_fromUtf8("actionHideStepTool"))
        self.actionHideSlider = QtGui.QAction(OssimPlanetSasha)
        self.actionHideSlider.setCheckable(True)
        self.actionHideSlider.setObjectName(_fromUtf8("actionHideSlider"))
        self.actionJoystick_2 = QtGui.QAction(OssimPlanetSasha)
        self.actionJoystick_2.setIcon(icon22)
        self.actionJoystick_2.setObjectName(_fromUtf8("actionJoystick_2"))
        self.actionDataexp = QtGui.QAction(OssimPlanetSasha)
        icon27 = QtGui.QIcon()
        icon27.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/ingranaggi.gif")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionDataexp.setIcon(icon27)
        self.actionDataexp.setObjectName(_fromUtf8("actionDataexp"))
        self.actionVrt = QtGui.QAction(OssimPlanetSasha)
        icon28 = QtGui.QIcon()
        icon28.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/ingranaggio_icona.gif")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionVrt.setIcon(icon28)
        self.actionVrt.setObjectName(_fromUtf8("actionVrt"))
        self.actionGVrt = QtGui.QAction(OssimPlanetSasha)
        self.actionGVrt.setIcon(icon28)
        self.actionGVrt.setObjectName(_fromUtf8("actionGVrt"))
        self.actionGrassshell = QtGui.QAction(OssimPlanetSasha)
        icon29 = QtGui.QIcon()
        icon29.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/512 Terminal.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionGrassshell.setIcon(icon29)
        self.actionGrassshell.setObjectName(_fromUtf8("actionGrassshell"))
        self.actionModel = QtGui.QAction(OssimPlanetSasha)
        icon30 = QtGui.QIcon()
        icon30.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/cubo.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionModel.setIcon(icon30)
        self.actionModel.setObjectName(_fromUtf8("actionModel"))
        self.actionPref = QtGui.QAction(OssimPlanetSasha)
        icon31 = QtGui.QIcon()
        icon31.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/tools.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionPref.setIcon(icon31)
        self.actionPref.setShortcut(_fromUtf8(""))
        self.actionPref.setShortcutContext(QtCore.Qt.WidgetShortcut)
        self.actionPref.setObjectName(_fromUtf8("actionPref"))
        self.actionVectorOp = QtGui.QAction(OssimPlanetSasha)
        icon32 = QtGui.QIcon()
        icon32.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/SquadraCompasso.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionVectorOp.setIcon(icon32)
        self.actionVectorOp.setObjectName(_fromUtf8("actionVectorOp"))
        self.actionHW = QtGui.QAction(OssimPlanetSasha)
        self.actionHW.setCheckable(True)
        icon33 = QtGui.QIcon()
        icon33.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/keyser-tux-wifi-logo-2300.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionHW.setIcon(icon33)
        self.actionHW.setObjectName(_fromUtf8("actionHW"))
        self.actionBroadcast = QtGui.QAction(OssimPlanetSasha)
        self.actionBroadcast.setCheckable(True)
        icon34 = QtGui.QIcon()
        icon34.addPixmap(QtGui.QPixmap(_fromUtf8(":/icons/Ubuntu_connessione_Internet_.png")), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionBroadcast.setIcon(icon34)
        self.actionBroadcast.setObjectName(_fromUtf8("actionBroadcast"))
        self.actionCompass = QtGui.QAction(OssimPlanetSasha)
        self.actionCompass.setObjectName(_fromUtf8("actionCompass"))
        self.actionPan_Tool = QtGui.QAction(OssimPlanetSasha)
        self.actionPan_Tool.setObjectName(_fromUtf8("actionPan_Tool"))
        self.actionGt = QtGui.QAction(OssimPlanetSasha)
        self.actionGt.setCheckable(True)
        self.actionGt.setIcon(icon8)
        self.actionGt.setObjectName(_fromUtf8("actionGt"))
        self.menuView_2.addAction(self.actionHideSlider)
        self.menuView_2.addAction(self.actionCompass)
        self.menuView_2.addAction(self.actionPan_Tool)
        self.menuSasha.addAction(self.menuView_2.menuAction())
        self.menuSasha.addAction(self.actionPref)
        self.menuGPS.addAction(self.actionNMEA)
        self.menuData.addAction(self.actionDataexp)
        self.menuData.addAction(self.actionVrt)
        self.menuTools.addAction(self.menuData.menuAction())
        self.menuTools.addAction(self.menuGPS.menuAction())
        self.menuTools.addAction(self.actionEpsg)
        self.menubar.addAction(self.menuSasha.menuAction())
        self.menubar.addAction(self.menuTools.menuAction())
        self.toolBar.addAction(self.actionLonLat)
        self.toolBar.addAction(self.actionGrass)
        self.toolBar.addAction(self.actionGPS)
        self.toolBar.addAction(self.actionBroadcast)
        self.toolBar.addAction(self.actionJoystick)
        self.toolBar.addAction(self.actionHW)
        self.toolBar.addSeparator()
        self.toolBar.addAction(self.actionData_2)
        self.toolBar.addAction(self.actionGrassshell)
        self.toolBar.addSeparator()
        self.toolBar.addAction(self.actionSavekml)
        self.toolBar.addAction(self.actionVectorOp)
        self.toolBar.addAction(self.actionModel)
        self.toolBar.addSeparator()
        self.toolBar.addAction(self.actionExit)

        self.retranslateUi(OssimPlanetSasha)
        self.tabWidget.setCurrentIndex(0)
        self.Head.setCurrentIndex(1)
        self.tabWidget_2.setCurrentIndex(1)
        QtCore.QMetaObject.connectSlotsByName(OssimPlanetSasha)

    def retranslateUi(self, OssimPlanetSasha):
        OssimPlanetSasha.setWindowTitle(QtGui.QApplication.translate("OssimPlanetSasha", "OssimPlanetSasha", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Eye", None, QtGui.QApplication.UnicodeUTF8))
        self.Lat.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "Display Longitude value in decimal degrees", None, QtGui.QApplication.UnicodeUTF8))
        self.Lon.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "Display Latitude value in decimal degrees", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("OssimPlanetSasha", "LookAt", None, QtGui.QApplication.UnicodeUTF8))
        self.label_9.setText(QtGui.QApplication.translate("OssimPlanetSasha", "N/E", None, QtGui.QApplication.UnicodeUTF8))
        self.Nord.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "North - UTM coordinates - meters", None, QtGui.QApplication.UnicodeUTF8))
        self.East.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "East - UTM coordinates - meters", None, QtGui.QApplication.UnicodeUTF8))
        self.label_11.setText(QtGui.QApplication.translate("OssimPlanetSasha", "UTM", None, QtGui.QApplication.UnicodeUTF8))
        self.utmcode.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "UTM zone", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "Ellipsoid", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(0, QtGui.QApplication.translate("OssimPlanetSasha", "WGS-84", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(1, QtGui.QApplication.translate("OssimPlanetSasha", "Airy", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(2, QtGui.QApplication.translate("OssimPlanetSasha", "Australian National", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(3, QtGui.QApplication.translate("OssimPlanetSasha", "Bessel 1841", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(4, QtGui.QApplication.translate("OssimPlanetSasha", "Bessel 1841 (Nambia)", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(5, QtGui.QApplication.translate("OssimPlanetSasha", "Clarke 1866", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(6, QtGui.QApplication.translate("OssimPlanetSasha", "Clarke 1880", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(7, QtGui.QApplication.translate("OssimPlanetSasha", "Everest", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(8, QtGui.QApplication.translate("OssimPlanetSasha", "Fischer 1960 (Mercury)", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(9, QtGui.QApplication.translate("OssimPlanetSasha", "Fischer 1968", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(10, QtGui.QApplication.translate("OssimPlanetSasha", "GRS 1967", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(11, QtGui.QApplication.translate("OssimPlanetSasha", "GRS 1980", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(12, QtGui.QApplication.translate("OssimPlanetSasha", "Helmert 1906", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(13, QtGui.QApplication.translate("OssimPlanetSasha", "Hough", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(14, QtGui.QApplication.translate("OssimPlanetSasha", "International", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(15, QtGui.QApplication.translate("OssimPlanetSasha", "Krassovsky", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(16, QtGui.QApplication.translate("OssimPlanetSasha", "Modified Airy", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(17, QtGui.QApplication.translate("OssimPlanetSasha", "Modified Everest", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(18, QtGui.QApplication.translate("OssimPlanetSasha", "Modified Fischer 1960", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(19, QtGui.QApplication.translate("OssimPlanetSasha", "South American 1969", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(20, QtGui.QApplication.translate("OssimPlanetSasha", "WGS 60", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(21, QtGui.QApplication.translate("OssimPlanetSasha", "WGS 66", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(22, QtGui.QApplication.translate("OssimPlanetSasha", "WGS-72", None, QtGui.QApplication.UnicodeUTF8))
        self.ellipse.setItemText(23, QtGui.QApplication.translate("OssimPlanetSasha", "WGS-84", None, QtGui.QApplication.UnicodeUTF8))
        self.Place.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "Geonames Zone", None, QtGui.QApplication.UnicodeUTF8))
        self.refreshsqlite.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Lucida Grande\'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">reload sqlite db</p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.placezone.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "Geonames Place", None, QtGui.QApplication.UnicodeUTF8))
        self.SendPosition.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Go", None, QtGui.QApplication.UnicodeUTF8))
        self.label_4.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Pan-Step : Deg", None, QtGui.QApplication.UnicodeUTF8))
        self.SpeedSpinBox.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "Pan Step in decimal degrees", None, QtGui.QApplication.UnicodeUTF8))
        self.label_5.setText(QtGui.QApplication.translate("OssimPlanetSasha", "* 10^-", None, QtGui.QApplication.UnicodeUTF8))
        self.SpeedMultipler.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "PStep - 10^-x  multipler", None, QtGui.QApplication.UnicodeUTF8))
        self.View.setItemText(0, QtGui.QApplication.translate("OssimPlanetSasha", "LookAt", None, QtGui.QApplication.UnicodeUTF8))
        self.View.setItemText(1, QtGui.QApplication.translate("OssimPlanetSasha", "Camera", None, QtGui.QApplication.UnicodeUTF8))
        self.Head.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "Heading mode", None, QtGui.QApplication.UnicodeUTF8))
        self.Head.setItemText(1, QtGui.QApplication.translate("OssimPlanetSasha", "Manual", None, QtGui.QApplication.UnicodeUTF8))
        self.Head.setItemText(2, QtGui.QApplication.translate("OssimPlanetSasha", "Auto", None, QtGui.QApplication.UnicodeUTF8))
        self.Head.setItemText(3, QtGui.QApplication.translate("OssimPlanetSasha", "N", None, QtGui.QApplication.UnicodeUTF8))
        self.Head.setItemText(4, QtGui.QApplication.translate("OssimPlanetSasha", "E", None, QtGui.QApplication.UnicodeUTF8))
        self.Head.setItemText(5, QtGui.QApplication.translate("OssimPlanetSasha", "SE", None, QtGui.QApplication.UnicodeUTF8))
        self.Head.setItemText(6, QtGui.QApplication.translate("OssimPlanetSasha", "S", None, QtGui.QApplication.UnicodeUTF8))
        self.Head.setItemText(7, QtGui.QApplication.translate("OssimPlanetSasha", "SW", None, QtGui.QApplication.UnicodeUTF8))
        self.Head.setItemText(8, QtGui.QApplication.translate("OssimPlanetSasha", "W", None, QtGui.QApplication.UnicodeUTF8))
        self.Head.setItemText(9, QtGui.QApplication.translate("OssimPlanetSasha", "NW", None, QtGui.QApplication.UnicodeUTF8))
        self.Head.setItemText(10, QtGui.QApplication.translate("OssimPlanetSasha", "NE", None, QtGui.QApplication.UnicodeUTF8))
        self.hsHeading.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Heading ", None, QtGui.QApplication.UnicodeUTF8))
        self.hsPitch.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Pitch", None, QtGui.QApplication.UnicodeUTF8))
        self.hsRoll.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Roll ", None, QtGui.QApplication.UnicodeUTF8))
        self.hsZoom.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Zoom ", None, QtGui.QApplication.UnicodeUTF8))
        self.ZoomSpinBox.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "Zoom (altitude) value in meters", None, QtGui.QApplication.UnicodeUTF8))
        self.label_6.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Step : m", None, QtGui.QApplication.UnicodeUTF8))
        self.ZoomStepSpinBox.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "Zoom Step in meters", None, QtGui.QApplication.UnicodeUTF8))
        self.label_7.setText(QtGui.QApplication.translate("OssimPlanetSasha", "*10^", None, QtGui.QApplication.UnicodeUTF8))
        self.ZoomMultipler.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "ZStep - 10^x  multipler", None, QtGui.QApplication.UnicodeUTF8))
        self.hsRange.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Range ", None, QtGui.QApplication.UnicodeUTF8))
        self.RangeSpinBox.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "Range (distance) in meters", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Step : m", None, QtGui.QApplication.UnicodeUTF8))
        self.RangeStepSpinBox.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "Zoom Step in meters", None, QtGui.QApplication.UnicodeUTF8))
        self.label_8.setText(QtGui.QApplication.translate("OssimPlanetSasha", "*10^", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab), QtGui.QApplication.translate("OssimPlanetSasha", "Navigation", None, QtGui.QApplication.UnicodeUTF8))
        self.addRlayer.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "Add Raster Layer", None, QtGui.QApplication.UnicodeUTF8))
        self.addRlayer.setText(QtGui.QApplication.translate("OssimPlanetSasha", "+", None, QtGui.QApplication.UnicodeUTF8))
        self.removeRlayer.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "Remove Raster Layer", None, QtGui.QApplication.UnicodeUTF8))
        self.removeRlayer.setText(QtGui.QApplication.translate("OssimPlanetSasha", "-", None, QtGui.QApplication.UnicodeUTF8))
        self.GrassRLayer.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "Raster Layer", None, QtGui.QApplication.UnicodeUTF8))
        self.GrassVLayer.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "Vector Layer", None, QtGui.QApplication.UnicodeUTF8))
        self.addVlayer.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "Add Vector Layer", None, QtGui.QApplication.UnicodeUTF8))
        self.addVlayer.setText(QtGui.QApplication.translate("OssimPlanetSasha", "+", None, QtGui.QApplication.UnicodeUTF8))
        self.removeVlayer.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "Remove Vector Layer", None, QtGui.QApplication.UnicodeUTF8))
        self.removeVlayer.setText(QtGui.QApplication.translate("OssimPlanetSasha", "-", None, QtGui.QApplication.UnicodeUTF8))
        self.renderoptions.setText(QtGui.QApplication.translate("OssimPlanetSasha", "R", None, QtGui.QApplication.UnicodeUTF8))
        self.setPenColor.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Pen", None, QtGui.QApplication.UnicodeUTF8))
        self.PenColor.setText(QtGui.QApplication.translate("OssimPlanetSasha", "111,111,111", None, QtGui.QApplication.UnicodeUTF8))
        self.setBrushColor.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Brush", None, QtGui.QApplication.UnicodeUTF8))
        self.BrushColor.setText(QtGui.QApplication.translate("OssimPlanetSasha", "111,111,111", None, QtGui.QApplication.UnicodeUTF8))
        self.PointSize.setText(QtGui.QApplication.translate("OssimPlanetSasha", "1,1", None, QtGui.QApplication.UnicodeUTF8))
        self.Fill.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Fill", None, QtGui.QApplication.UnicodeUTF8))
        self.Thickness.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Thickness", None, QtGui.QApplication.UnicodeUTF8))
        self.checkBox.setText(QtGui.QApplication.translate("OssimPlanetSasha", "kml", None, QtGui.QApplication.UnicodeUTF8))
        self.gcmdexec.setText(QtGui.QApplication.translate("OssimPlanetSasha", "...", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget.horizontalHeaderItem(0).setText(QtGui.QApplication.translate("OssimPlanetSasha", "Raster", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget.horizontalHeaderItem(1).setText(QtGui.QApplication.translate("OssimPlanetSasha", "Vector", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_2), QtGui.QApplication.translate("OssimPlanetSasha", "Query", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget_2.setTabText(self.tabWidget_2.indexOf(self.tab_7), QtGui.QApplication.translate("OssimPlanetSasha", "Map", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget_2.setTabText(self.tabWidget_2.indexOf(self.tab_8), QtGui.QApplication.translate("OssimPlanetSasha", "Graph", None, QtGui.QApplication.UnicodeUTF8))
        self.Latlabel_2.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Lat", None, QtGui.QApplication.UnicodeUTF8))
        self.Lonlabel_2.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Lon", None, QtGui.QApplication.UnicodeUTF8))
        self.Datelabel.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Date", None, QtGui.QApplication.UnicodeUTF8))
        self.Utclabel.setText(QtGui.QApplication.translate("OssimPlanetSasha", "UTC", None, QtGui.QApplication.UnicodeUTF8))
        self.Altitudelabel.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Altitude", None, QtGui.QApplication.UnicodeUTF8))
        self.Speedlabel.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Speed", None, QtGui.QApplication.UnicodeUTF8))
        self.Climblabel.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Climb", None, QtGui.QApplication.UnicodeUTF8))
        self.Pdoplabel.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Pdop", None, QtGui.QApplication.UnicodeUTF8))
        self.Hdoplabel.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Hdop", None, QtGui.QApplication.UnicodeUTF8))
        self.Vdoplabel.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Vdop", None, QtGui.QApplication.UnicodeUTF8))
        self.Tdoplabel.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Tdop", None, QtGui.QApplication.UnicodeUTF8))
        self.Gdoplabel.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Gdop", None, QtGui.QApplication.UnicodeUTF8))
        self.Ephlabel.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Eph", None, QtGui.QApplication.UnicodeUTF8))
        self.Eptlabel.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Ept", None, QtGui.QApplication.UnicodeUTF8))
        self.Epslabel.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Eps", None, QtGui.QApplication.UnicodeUTF8))
        self.Epvlabel.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Epv", None, QtGui.QApplication.UnicodeUTF8))
        self.Epdlabel.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Epd", None, QtGui.QApplication.UnicodeUTF8))
        self.Epclabel.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Epc", None, QtGui.QApplication.UnicodeUTF8))
        self.Tracklabel.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Track", None, QtGui.QApplication.UnicodeUTF8))
        self.GPSSend.setText(QtGui.QApplication.translate("OssimPlanetSasha", "SendPosition", None, QtGui.QApplication.UnicodeUTF8))
        self.StartGps.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Start/Stop ", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_4), QtGui.QApplication.translate("OssimPlanetSasha", "GPS", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_3), QtGui.QApplication.translate("OssimPlanetSasha", "Data", None, QtGui.QApplication.UnicodeUTF8))
        self.menuSasha.setTitle(QtGui.QApplication.translate("OssimPlanetSasha", "Sasha", None, QtGui.QApplication.UnicodeUTF8))
        self.menuView_2.setTitle(QtGui.QApplication.translate("OssimPlanetSasha", "view", None, QtGui.QApplication.UnicodeUTF8))
        self.menuTools.setTitle(QtGui.QApplication.translate("OssimPlanetSasha", "Tools", None, QtGui.QApplication.UnicodeUTF8))
        self.menuGPS.setTitle(QtGui.QApplication.translate("OssimPlanetSasha", "GPS", None, QtGui.QApplication.UnicodeUTF8))
        self.menuData.setTitle(QtGui.QApplication.translate("OssimPlanetSasha", "Data", None, QtGui.QApplication.UnicodeUTF8))
        self.toolBar.setWindowTitle(QtGui.QApplication.translate("OssimPlanetSasha", "toolBar", None, QtGui.QApplication.UnicodeUTF8))
        self.dockWidget.setWindowTitle(QtGui.QApplication.translate("OssimPlanetSasha", "PanTool", None, QtGui.QApplication.UnicodeUTF8))
        self.west.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "West", None, QtGui.QApplication.UnicodeUTF8))
        self.west.setText(QtGui.QApplication.translate("OssimPlanetSasha", " W", None, QtGui.QApplication.UnicodeUTF8))
        self.east.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "East", None, QtGui.QApplication.UnicodeUTF8))
        self.east.setText(QtGui.QApplication.translate("OssimPlanetSasha", " E ", None, QtGui.QApplication.UnicodeUTF8))
        self.southwest.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "SouthWest", None, QtGui.QApplication.UnicodeUTF8))
        self.south.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "South", None, QtGui.QApplication.UnicodeUTF8))
        self.south.setText(QtGui.QApplication.translate("OssimPlanetSasha", " S ", None, QtGui.QApplication.UnicodeUTF8))
        self.southeast.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "SouthEast", None, QtGui.QApplication.UnicodeUTF8))
        self.north.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "North", None, QtGui.QApplication.UnicodeUTF8))
        self.northwest.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "NordWest", None, QtGui.QApplication.UnicodeUTF8))
        self.northeast.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "NordEast", None, QtGui.QApplication.UnicodeUTF8))
        self.dockWidget_2.setWindowTitle(QtGui.QApplication.translate("OssimPlanetSasha", "Heading°", None, QtGui.QApplication.UnicodeUTF8))
        self.dockWidget_3.setWindowTitle(QtGui.QApplication.translate("OssimPlanetSasha", "Roll°", None, QtGui.QApplication.UnicodeUTF8))
        self.actionGPS.setText(QtGui.QApplication.translate("OssimPlanetSasha", "GPS", None, QtGui.QApplication.UnicodeUTF8))
        self.actionGrass.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Grass", None, QtGui.QApplication.UnicodeUTF8))
        self.actionGrass.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "Grass", None, QtGui.QApplication.UnicodeUTF8))
        self.actionJoystick.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Joystick", None, QtGui.QApplication.UnicodeUTF8))
        self.actionJoystick.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "Joystick", None, QtGui.QApplication.UnicodeUTF8))
        self.actionLonLat.setText(QtGui.QApplication.translate("OssimPlanetSasha", "LonLat", None, QtGui.QApplication.UnicodeUTF8))
        self.actionLonLat.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "LonLat", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Preference", None, QtGui.QApplication.UnicodeUTF8))
        self.actionDB_setting.setText(QtGui.QApplication.translate("OssimPlanetSasha", "DB-setting", None, QtGui.QApplication.UnicodeUTF8))
        self.actionNMEA.setText(QtGui.QApplication.translate("OssimPlanetSasha", "NMEA", None, QtGui.QApplication.UnicodeUTF8))
        self.actionData_2.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Data", None, QtGui.QApplication.UnicodeUTF8))
        self.actionData_2.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "Data processing", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSavekml.setText(QtGui.QApplication.translate("OssimPlanetSasha", "savekml", None, QtGui.QApplication.UnicodeUTF8))
        self.actionEpsg.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Epsg", None, QtGui.QApplication.UnicodeUTF8))
        self.actionHidesliders.setText(QtGui.QApplication.translate("OssimPlanetSasha", "hideslider", None, QtGui.QApplication.UnicodeUTF8))
        self.actionHideSpinbox.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Spinbox", None, QtGui.QApplication.UnicodeUTF8))
        self.actionHide_place_position.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Place-Position", None, QtGui.QApplication.UnicodeUTF8))
        self.actionHideStepTool.setText(QtGui.QApplication.translate("OssimPlanetSasha", "StepTool", None, QtGui.QApplication.UnicodeUTF8))
        self.actionHideSlider.setText(QtGui.QApplication.translate("OssimPlanetSasha", "tabPosition", None, QtGui.QApplication.UnicodeUTF8))
        self.actionJoystick_2.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Joystick", None, QtGui.QApplication.UnicodeUTF8))
        self.actionDataexp.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Export", None, QtGui.QApplication.UnicodeUTF8))
        self.actionVrt.setText(QtGui.QApplication.translate("OssimPlanetSasha", "DataTools", None, QtGui.QApplication.UnicodeUTF8))
        self.actionGVrt.setText(QtGui.QApplication.translate("OssimPlanetSasha", "vrt", None, QtGui.QApplication.UnicodeUTF8))
        self.actionGrassshell.setText(QtGui.QApplication.translate("OssimPlanetSasha", "grassshell", None, QtGui.QApplication.UnicodeUTF8))
        self.actionModel.setText(QtGui.QApplication.translate("OssimPlanetSasha", "model", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPref.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Preference", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPref.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "Preference", None, QtGui.QApplication.UnicodeUTF8))
        self.actionVectorOp.setText(QtGui.QApplication.translate("OssimPlanetSasha", "VectorOp", None, QtGui.QApplication.UnicodeUTF8))
        self.actionHW.setText(QtGui.QApplication.translate("OssimPlanetSasha", "HW", None, QtGui.QApplication.UnicodeUTF8))
        self.actionHW.setToolTip(QtGui.QApplication.translate("OssimPlanetSasha", "Serial", None, QtGui.QApplication.UnicodeUTF8))
        self.actionBroadcast.setText(QtGui.QApplication.translate("OssimPlanetSasha", "actionBroadcast", None, QtGui.QApplication.UnicodeUTF8))
        self.actionCompass.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Compass", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPan_Tool.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Pan-Tool", None, QtGui.QApplication.UnicodeUTF8))
        self.actionGt.setText(QtGui.QApplication.translate("OssimPlanetSasha", "Gt", None, QtGui.QApplication.UnicodeUTF8))

from PyQt4 import QtWebKit
import resources_rc
