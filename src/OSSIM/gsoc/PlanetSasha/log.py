from PyQt4.QtCore import *
from PyQt4.QtGui import *
from configure import parseOutputconf
import socket
from tcp4ossim import parseSignal # , parsesignalLookAt
#import pygame
import os
import time
from datetime import datetime


class HWS(QThread):
    ValUpdated = pyqtSignal(str)
    def __init__(self, parent = None):
        QThread.__init__(self, parent)
        self.alive = 0
        self.running = 0

    def run(self):
        #HOST = str(parseOutputconf()['host'])
        #PORT = 19000
        #s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        #s.bind((HOST, PORT))
        #s.listen(1)
        #conn, addr = s.accept()
        #print 'Connected by', addr
        while self.alive:
            while self.running:
                #data = conn.recv(1048)
                #try :
                #    parsed = parseSignal(data)
                #    self.ValUpdated.emit(str(parsed['lon']))
                #except :
                #    print 'parsing error'
                now = datetime.now()
                self.ValUpdated.emit(str(now))
                time.sleep(0.5)

    def toggle(self):
        if self.running:
            self.running = 0
        else :
            self.running = 1

    def stop(self):
        print 'you have to sleep'
        self.alive = 0
        self.running = 0
        self.wait()


class logS(QThread):
    LonUpdated = pyqtSignal(str)
    LatUpdated = pyqtSignal(str)
    RollUpdated = pyqtSignal(float)
    PitchUpdated = pyqtSignal(float)
    GainUpdated = pyqtSignal(float)
    AltUpdated2 = pyqtSignal(str)
    AltUpdated = pyqtSignal(float)
    LookAtLonUpdated = pyqtSignal(str)
    LookAtLatUpdated = pyqtSignal(str)
    LookAtAltitudeUpdated = pyqtSignal(str)
    LookAtRangeUpdated = pyqtSignal(float)
    #GainUpdated = pyqtSignal(float)
    
    def __init__(self, parent = None):
        QThread.__init__(self, parent)
        self.alive = 1
        self.running = 0
    
    
    def run(self):
        HOST = str(parseOutputconf()['host']) # 'localhost'
        PORT = 9000
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((HOST, PORT))
        s.listen(1)
        conn, addr = s.accept()
        print 'Connected by', addr
        while self.alive:
            while self.running:
                data = conn.recv(1048)
                #print data
                #print type(data)
                #time.sleep(0.5)
                try :
                    parsed = parseSignal(data)
                    self.LonUpdated.emit(str(parsed['lon']))
                    self.LatUpdated.emit(str(parsed['lat']))
                    self.RollUpdated.emit(float(parsed['roll']))
                    self.PitchUpdated.emit(float(parsed['pitch']))
                    self.GainUpdated.emit(float(parsed['gain']))
                    self.AltUpdated2.emit(str(parsed['msl']))
                    self.AltUpdated.emit(float(parsed['msl']))
                    self.LookAtLonUpdated.emit(str(parsed['lookat_lon']))
                    self.LookAtLatUpdated.emit(str(parsed['lookat_lat']))
                    self.LookAtAltitudeUpdated.emit(str(parsed['lookat_alt']))
                    self.LookAtRangeUpdated.emit(float(parsed['lookat_range']))
                
                #print parsed
                #navlogger('localhost', 9000)
                #self.msleep(500)
                except :
                    print 'parsing error'
    
    """
    def setValueLookAtLon(self, valueLookAtLon):
        self.valueLookAtLon = valueLookAtLon

    def setValueLookAtLat(self, valueLookAtLat):
        self.valueLookAtLat = valueLookAtLat

    def setValueLookAtAlt(self, valueLookAtAlt):
        self.valueLookAtAlt = valueLookAtAlt

    def setValueLookAtRange(self, valueLookAtRange):
        self.valueLookAtRange = valueLookAtRange
   
    
    def setValueLon(self, valueLon):
        self.valueLon = valueLon
    
    
    def setValueLat(self, valueLat):
        self.valueLat = valueLat
    
    
    def setValueRoll(self, valueRoll):
        self.valueRoll = valueRoll
    
    
    def setValuePitch(self, valuePitch):
        self.valuePitch = valuePitch
    
    
    def setValueGain(self, valueGain):
        self.valueGain = valueGain
    
    
    def setValueAlt(self, valueAlt):
        self.valueAlt = valueAlt

    def setValueRange(self, valueRange):
        self.valueRange = valueRange
    """
    
    
    def toggle(self):
        if self.running:
            self.running = 0
        else :
            self.running = 1
    
    
    def stop(self):
        self.alive = 0
        self.running = 0
        self.wait()


"""
class logJ1(QThread):
    def __init__(self, parent = None):
        QThread.__init__(self, parent)
        self.valuelonJ = ""
        self.valuelatJ = ""
        self.alive = 1
        self.running = 0


    def run(self):
        while self.alive:
            while self.running:
                try :
                    startj(float(self.jlon), float(self.jlat))
                except :
                    print 'exit from Joy mode'


    def toggle(self,x,y):
        self.jlon = x
        self.jlat = y
        #pygame.init()
        if self.running:
            self.running = 0
        else :
            self.running = 1


    def stop(self):
        pygame.quit()
        self.alive = 0
        self.running = 0
        self.wait()


    def setValueLonJ(self, valuelonJ):
        self.valuelonJ = valuelonJ


    def setValueLatJ(self, valuelatJ):
        self.valuelatJ = valuelatJ


    def aggiorna(self):
        newvaluelon = str(self.valuelonJ)
        newvaluelat = str(self.valuelatJ)
        position = (newvaluelon,newvaluelat)
        print position
        return position
"""

class gt(QThread):
    def __init__(self, command, parent = None):
        QThread.__init__(self, parent)
        self.alive = 1
        self.running = 0
        self.command = command


    def run(self):
        os.system(str(self.command))
        #os.system('g.region')


    def toggle(self):
        if self.running:
            self.running = 0
        else :
            self.running = 1


    def stop(self):
        self.alive = 0
        self.running = 0
        self.wait()

