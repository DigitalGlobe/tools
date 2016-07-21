#!/usr/bin/env python
import gps
import os
from PyQt4.QtCore import *


class GpsT(QThread):
    def __init__(self, parent = None):
        QThread.__init__(self, parent)
        self.alive = 1
        self.running = 0
        self.crossData = ""
        self.GPSlatitude = ""
        self.GPSlongitude = ""
        self.GPSsat = ""
        self.GPStime = ""
        self.GPSeph = ""
        self.GPSspeed = ""
        self.GPSaltitude = ""
        self.GPSepv = ""
        self.GPSept = ""
        self.GPSclimb = ""
        self.satellite = ""
        self.GPSutctime = ""
        self.GPStrack = ""
        self.GPSepd = ""
        self.GPSeps = ""
        self.GPSepc = ""
        self.GPSpdop = ""
        self.GPShdop = ""
        self.GPSvdop = ""
        self.GPStdop = ""
        self.GPSgdop = ""

    def run(self):
        while self.alive:
            while self.running:
                os.system('clear')
                session.query('admosyq')
                satellitelist=[]
                html = """<TABLE cellpadding="4" style="border: 1px solid \
                #000000; border-collapse: collapse;" border="1"><TR><TD>PRN</TD>\
                <TD>E</TD><TD>Az</TD><TD>Ss</TD><TD>Used</TD></TR>"""
                satellitelist.append(html)
                for i in session.satellites:                    
                    table = """<TR><TD>%s</TD><TD>%s</TD><TD>%s</TD><TD>%s</TD>\
                    <TD>%s</TD>""" % (i.PRN, i.elevation, i.azimuth, i.ss, i.used)
                    satellitelist.append(table)
                htmlend = """</TR></TABLE>"""
                satellitelist.append(htmlend)
                satstringa = str(satellitelist)
                satstringa = satstringa.replace(",","")
                satstringa = satstringa.replace("[","")
                satstringa = satstringa.replace("]","")
                satstringa = satstringa.replace("'","")
                self.GPSlatitude = str(session.fix.latitude)
                self.GPSlongitude = str(session.fix.longitude)
                self.GPStime = str(session.utc)
                self.GPSutctime = str(session.fix.time)
                self.GPSaltitude = str(session.fix.altitude)
                self.GPSeph = str(session.fix.eph)
                self.GPSepv = str(session.fix.epv)
                self.GPSept = str(session.fix.ept)
                self.GPSspeed = str(session.fix.speed)
                self.GPSclimb = str(session.fix.climb)
                self.GPStrack = str(session.fix.track)
                self.GPSepd = str(session.fix.epd)
                self.GPSeps = str(session.fix.eps)
                self.GPSepc = str(session.fix.epc)
                self.GPSpdop = str(session.pdop)
                self.GPShdop = str(session.hdop)
                self.GPSvdop = str(session.vdop)
                self.GPStdop = str(session.tdop)
                self.GPSgdop = str(session.gdop)
                self.GPSsat = satstringa
                    
                self.emit(SIGNAL("gpslat"), self.GPSlatitude)
                self.emit(SIGNAL("gpslon"), self.GPSlongitude)
                self.emit(SIGNAL("gpssat"), self.GPSsat)
                self.emit(SIGNAL("gpseph"), self.GPSeph)
                self.emit(SIGNAL("gpsaltitude"), self.GPSaltitude)
                self.emit(SIGNAL("gpsclimb"), self.GPSclimb)
                self.emit(SIGNAL("gpstime"), self.GPStime)
                self.emit(SIGNAL("gpsspeed"), self.GPSspeed)
                self.emit(SIGNAL("gpsepv"), self.GPSepv)
                self.emit(SIGNAL("gpsept"), self.GPSept)
                self.emit(SIGNAL("gpstrack"), self.GPStrack)
                self.emit(SIGNAL("gpsepd"), self.GPSepd)
                self.emit(SIGNAL("gpseps"), self.GPSeps)
                self.emit(SIGNAL("gpsepc"), self.GPSepc)
                self.emit(SIGNAL("gpspdop"), self.GPSpdop)
                self.emit(SIGNAL("gpshdop"), self.GPShdop)
                self.emit(SIGNAL("gpsvdop"), self.GPSvdop)
                self.emit(SIGNAL("gpstdop"), self.GPStdop)
                self.emit(SIGNAL("gpsgdop"), self.GPSgdop)
                self.emit(SIGNAL("utctime"), self.GPSutctime)

                self.msleep(1000)
            self.msleep(1000)

    def toggle(self):
        global session
        session = gps.gps()
        self.running = not self.running
        self.emit(SIGNAL("gpslat"), self.GPSlatitude)
        self.emit(SIGNAL("gpslon"), self.GPSlongitude)
        self.emit(SIGNAL("gpssat"), self.GPSsat)
        self.emit(SIGNAL("gpseph"), self.GPSeph)
        self.emit(SIGNAL("gpsaltitude"), self.GPSaltitude)
        self.emit(SIGNAL("gpsmsl"), self.GPSclimb)
        self.emit(SIGNAL("gpstime"), self.GPStime)
        self.emit(SIGNAL("gpsspeed"), self.GPSspeed)
        self.emit(SIGNAL("gpsepv"), self.GPSepv)
        self.emit(SIGNAL("gpsept"), self.GPSept)
        self.emit(SIGNAL("gpstrack"), self.GPStrack)
        self.emit(SIGNAL("gpsepd"), self.GPSepd)
        self.emit(SIGNAL("gpseps"), self.GPSeps)
        self.emit(SIGNAL("gpsepc"), self.GPSepc)
        self.emit(SIGNAL("gpspdop"), self.GPSpdop)
        self.emit(SIGNAL("gpshdop"), self.GPShdop)
        self.emit(SIGNAL("gpsvdop"), self.GPSvdop)
        self.emit(SIGNAL("gpstdop"), self.GPStdop)
        self.emit(SIGNAL("gpsgdop"), self.GPSgdop)
        self.emit(SIGNAL("utctime"), self.GPSutctime)

    def stopserial(self):
        pass

    def stop(self):
        self.alive = 0

    def startserial(self):
        pass