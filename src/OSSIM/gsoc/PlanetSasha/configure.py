#!/usr/bin/env python

import sys
import os
from xml.dom import minidom 


apppath = os.path.abspath(os.path.dirname(sys.argv[0]))
configfile = '%s/conf/conf.xml' % (apppath)

def parseOutputconf2():
    xmldoc = minidom.parse(configfile)
    outconf = {}
    planetsashaconf = xmldoc.firstChild
    if planetsashaconf.childNodes[5].childNodes[1].firstChild is not None:
        outconf['kmldir'] = planetsashaconf.childNodes[5].childNodes[1].firstChild.data
    else :
        print 'kmldir not found'
        outconf['kmldir'] = 'None'
            
    if planetsashaconf.childNodes[5].childNodes[3].firstChild is not None:
        outconf['vrtdir'] = planetsashaconf.childNodes[5].childNodes[3].firstChild.data
    else :
        print 'vrtdir not found'
        outconf['vrtdir'] = 'None'
            
    if planetsashaconf.childNodes[5].childNodes[5].firstChild is not None:
        outconf['filemanager'] = planetsashaconf.childNodes[5].childNodes[5].firstChild.data
    else :
        print 'filemanager not found'
        outconf['filemanager'] = 'None'
            
    if planetsashaconf.childNodes[5].childNodes[7].firstChild is not None:
        outconf['gpsdevice'] = planetsashaconf.childNodes[5].childNodes[7].firstChild.data
    else :
        print 'gpsdevice not found'
        outconf['gpsdevice'] = 'None'
        
    if planetsashaconf.childNodes[5].childNodes[9].firstChild is not None:
        outconf['ossimpreference'] = planetsashaconf.childNodes[5].childNodes[9].firstChild.data
    else :
        print 'ossimpreference not found'
        outconf['ossimpreference'] = 'None'
        
    if planetsashaconf.childNodes[5].childNodes[11].firstChild is not None:
        outconf['spatialitedb'] = planetsashaconf.childNodes[5].childNodes[11].firstChild.data
    else :
        print 'spatialitedb not found'
        outconf['spatialitedb'] = 'None'
    if planetsashaconf.childNodes[3].childNodes[1].firstChild is not None:
            outconf['pgHost'] = planetsashaconf.childNodes[3].childNodes[1].firstChild.data
    else :
        print 'pgHost not found'
        outconf['pgHost'] = 'None'
            
    if planetsashaconf.childNodes[3].childNodes[3].firstChild is not None:
        outconf['pgPort'] = planetsashaconf.childNodes[3].childNodes[3].firstChild.data
    else :
        print 'pgPort not found'
        outconf['pgPort'] = 'None'
    if planetsashaconf.childNodes[3].childNodes[5].firstChild is not None:
        outconf['pgUser'] = planetsashaconf.childNodes[3].childNodes[5].firstChild.data
    else :
        print 'pgUser not found'
        outconf['pgUser'] = 'None'
    if planetsashaconf.childNodes[3].childNodes[7].firstChild is not None:
        outconf['pgPasswd'] = planetsashaconf.childNodes[3].childNodes[7].firstChild.data
    else :
        print 'pgPasswd not found'
        outconf['pgPasswd'] = 'None'
    if planetsashaconf.childNodes[1].childNodes[1].firstChild is not None:
        outconf['tcpHost'] = planetsashaconf.childNodes[1].childNodes[1].firstChild.data
    else :
        print 'tcpHost not found'
        outconf['tcpHost'] = 'None'
        
    if planetsashaconf.childNodes[1].childNodes[3].firstChild is not None:
        outconf['tcpDport'] = planetsashaconf.childNodes[1].childNodes[3].firstChild.data
    else :
        print 'tcpDport not found'
        outconf['tcpDport'] = 'None'
            
    if planetsashaconf.childNodes[1].childNodes[5].firstChild is not None:
        outconf['tcpPport'] = planetsashaconf.childNodes[1].childNodes[5].firstChild.data
    else :
        print 'tcpPport not found'
        outconf['tcpPport'] = 'None'
            
    return  outconf




def parseOutputconf():
    xmldoc = minidom.parse(configfile)
    outconf = {}
    planetsashaconf = xmldoc.firstChild
    if planetsashaconf.childNodes[1].childNodes[1].firstChild is not None:
        outconf['host'] = planetsashaconf.childNodes[1].childNodes[1].firstChild.data
    else :
        print 'host not found'
        outconf['host'] = 'None'
    if planetsashaconf.childNodes[1].childNodes[3].firstChild is not None:
        outconf['dport'] = planetsashaconf.childNodes[1].childNodes[3].firstChild.data
    else :
        print 'dport not found'
        outconf['pport'] = 'None'
    if planetsashaconf.childNodes[1].childNodes[5].firstChild is not None:
        outconf['pport'] = planetsashaconf.childNodes[1].childNodes[5].firstChild.data
    else :
        print 'pport not found'
        outconf['dport'] = 'None'
        
    if planetsashaconf.childNodes[3].childNodes[1].firstChild is not None:
        outconf['connection'] = planetsashaconf.childNodes[3].childNodes[1].firstChild.data
    else :
        print 'connection not found'
        outconf['connection'] = 'None'
    if planetsashaconf.childNodes[3].childNodes[3].firstChild is not None:
        outconf['dbhost'] = planetsashaconf.childNodes[3].childNodes[3].firstChild.data
    else :
        print 'dbhost not found'
        outconf['dbhost'] = 'None'
    if planetsashaconf.childNodes[3].childNodes[5].firstChild is not None:
        outconf['dbname'] = planetsashaconf.childNodes[3].childNodes[5].firstChild.data
    else :
        print 'dbname not found'
        outconf['dbname'] = 'None'
    if planetsashaconf.childNodes[3].childNodes[7].firstChild is not None:
        outconf['pgport'] = planetsashaconf.childNodes[3].childNodes[7].firstChild.data
    else :
        print 'pgport not found'
        outconf['pgport'] = 'None'
    if planetsashaconf.childNodes[3].childNodes[9].firstChild is not None:
        outconf['user'] = planetsashaconf.childNodes[3].childNodes[9].firstChild.data
    else :
        print 'user not found'
        outconf['user'] = 'None'
    if planetsashaconf.childNodes[3].childNodes[11].firstChild is not None:
        outconf['passwd'] = planetsashaconf.childNodes[3].childNodes[11].firstChild.data
    else :
        print 'passwd not found'
        outconf['passwd'] = 'None'
        
    if planetsashaconf.childNodes[5].childNodes[1].firstChild is not None:
        outconf['kmldir'] = planetsashaconf.childNodes[5].childNodes[1].firstChild.data
    else :
        print 'kmldir not found'
        outconf['kmldir'] = 'None'
        
    if planetsashaconf.childNodes[5].childNodes[3].firstChild is not None:
        outconf['vrtdir'] = planetsashaconf.childNodes[5].childNodes[3].firstChild.data
    else :
        print 'vrtdir not found'
        outconf['vrtdir'] = 'None'
        
    if planetsashaconf.childNodes[5].childNodes[5].firstChild is not None:
        outconf['filemanager'] = planetsashaconf.childNodes[5].childNodes[5].firstChild.data
    else :
        print 'filemanager not found'
        outconf['filemanager'] = 'None'
        
    if planetsashaconf.childNodes[5].childNodes[7].firstChild is not None:
        outconf['gpsdevice'] = planetsashaconf.childNodes[5].childNodes[7].firstChild.data
    else :
        print 'gpsdevice not found'
        outconf['gpsdevice'] = 'None'
        
    if planetsashaconf.childNodes[5].childNodes[9].firstChild is not None:
        outconf['ossimpreference'] = planetsashaconf.childNodes[5].childNodes[9].firstChild.data
    else :
        print 'ossimpreference not found'
        outconf['ossimpreference'] = 'None'
        
    if planetsashaconf.childNodes[5].childNodes[11].firstChild is not None:
        outconf['spatialitedb'] = planetsashaconf.childNodes[5].childNodes[11].firstChild.data
    else :
        print 'spatialitedb not found'
        outconf['spatialitedb'] = 'None'

    if planetsashaconf.childNodes[7].childNodes[1].firstChild is not None:
        outconf['PointSize'] = planetsashaconf.childNodes[7].childNodes[1].firstChild.data
    else :
        print 'PointSize not found'
        outconf['PointSize'] = 'None'

    if planetsashaconf.childNodes[7].childNodes[3].firstChild is not None:
        outconf['LineWidth'] = planetsashaconf.childNodes[7].childNodes[3].firstChild.data
    else :
        print 'LineWidth not found'
        outconf['LineWidth'] = 'None'

    if planetsashaconf.childNodes[7].childNodes[5].firstChild is not None:
        outconf['PenColor'] = planetsashaconf.childNodes[7].childNodes[5].firstChild.data
    else :
        print 'PenColor not found'
        outconf['PenColor'] = 'None'

    if planetsashaconf.childNodes[7].childNodes[7].firstChild is not None:
        outconf['BrushColor'] = planetsashaconf.childNodes[7].childNodes[7].firstChild.data
    else :
        print 'BrushColor not found'
        outconf['BrushColor'] = 'None'

    if planetsashaconf.childNodes[7].childNodes[9].firstChild is not None:
        outconf['Thickness'] = planetsashaconf.childNodes[7].childNodes[9].firstChild.data
    else :
        print 'Thickness not found'
        outconf['Thickness'] = 'None'


    if planetsashaconf.childNodes[7].childNodes[11].firstChild is not None:
        outconf['Fill'] = planetsashaconf.childNodes[7].childNodes[11].firstChild.data
    else :
        print 'Fill not found'
        outconf['Fill'] = 'None'


    return  outconf