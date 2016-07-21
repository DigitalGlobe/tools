#!/usr/bin/env python

try:
    from osgeo import osr
    from osgeo import ogr
except ImportError:
    import osr
    import ogr

from OssimPython import OssimHeight
import os.path
import zipfile


def WalkAndTransform( geom,tr ):
    if geom.GetGeometryCount() > 0:
        for i in range(geom.GetGeometryCount()):
            old_geom = geom.GetGeometryRef(i)
            new_geom = WalkAndTransform( old_geom )
            if new_geom is not old_geom:
                geom.SetGeometryDirectly( new_geom )
        return geom
    for i in range(geom.GetPointCount()):
        xyz = (geom.GetX(i), geom.GetY(i), geom.GetZ(i))
        if tr == '3D':
            xyz = SetOssimHeight( xyz )
        if tr == 'offset':
            xyz = SetOffset( xyz )
        if tr == 'Attribute':
            xyz = SetAttributeHeight( xyz )
        geom.SetPoint( i, xyz[0], xyz[1], xyz[2] )
    return geom


def SetOssimHeight( xyz ):
    x = xyz[0]
    y = xyz[1]
    z = OssimHeight(y,x)
    z = float(z) + off
    return (float(x),float(y),float(z))



def SetOffset( xyz ):
    x = xyz[0]
    y = xyz[1]
    z = float(off)
    return (float(x),float(y),float(z))

def SetAttributeHeight( xyz ):
    x = xyz[0]
    y = xyz[1]
    z = float(AttHeight)
    return (float(x),float(y),float(z))


def OpenKml(name, status, description):
    openkml = """<?xml version="1.0" encoding="UTF-8"?>
    <kml xmlns="http://www.opengis.net/kml/2.2">
        <Document>
            <name>%s</name>
            <open>%s</open>
            <description>%s</description>""" % (name, status, description)
    return openkml



def InitStyle(colorlabel,colormode,icon, colorline,tessellate,width,colorpolygon, LabelAlpha, LineAlpha, PolygonAlpha):
    labalpha = hex(LabelAlpha)
    linalpha = hex(LineAlpha)
    polalpha = hex(PolygonAlpha)
    labalpha = labalpha.split('x')
    linalpha = linalpha.split('x')
    polalpha = polalpha.split('x')
    labalpha = labalpha[-1]
    linalpha = linalpha[-1]
    polalpha = polalpha[-1]
    if len(labalpha) == 1:
        labalpha = str('0')+labalpha
    if len(linalpha) == 1:
        linalpha = str('0')+linalpha
    if len(polalpha) == 1:
        polalpha = str('0')+polalpha
    colorlabel = colorlabel[1:]
    colorlabelR = colorlabel[0:2]
    colorlabelG = colorlabel[2:4]
    colorlabelB = colorlabel[4:6]
    colorlabel = colorlabelB+colorlabelG+colorlabelR
    colorlabel = str(labalpha)+colorlabel
    colorline = colorline[1:]
    colorlineR = colorline[0:2]
    colorlineG = colorline[2:4]
    colorlineB = colorline[4:6]
    colorline = colorlineB+colorlineG+colorlineR
    colorline = str(linalpha)+colorline
    colorpolygon = colorpolygon[1:]
    colorpolygonR = colorpolygon[0:2]
    colorpolygonG = colorpolygon[2:4]
    colorpolygonB = colorpolygon[4:6]
    colorpolygon = colorpolygonB+colorpolygonG+colorpolygonR
    colorpolygon = str(polalpha)+colorpolygon
    iconname = icon.split('/')[-1]
    Style = """<Style id="Mystyle">
    <LabelStyle>
        <color>%s</color>
        <colorMode>%s</colorMode>
        <scale>1</scale>
    </LabelStyle>
    <IconStyle>
        <Icon>
            <href>%s</href>
        </Icon>
    </IconStyle>
    <LineStyle>
        <color>%s</color>
        <colorMode>%s</colorMode>
        <tessellate>%s</tessellate>
        <width>%s</width>
    </LineStyle>
    <PolyStyle>
        <color>%s</color>
        <colorMode>%s</colorMode>
    </PolyStyle>
</Style>""" % (colorlabel,colormode,iconname, colorline, colormode, tessellate, 
               width,colorpolygon,colormode)
    return Style



def SetPlacemark(namelabels, attributi, lon, lat, alt, rangex, tilt):
    """<LookAt>
    <longitude>%s</longitude>
    <latitude>%s</latitude>
    <altitude>%s</altitude>
    <range>%s</range>
    <tilt>%s</tilt>
    <heading>0</heading>
    </LookAt> lon, lat, alt, rangex, tilt """
    Placemark = """<Placemark>
    <name>%s</name>
    <description><![CDATA[<table border="1">%s</table>]]></description>
    <visibility>0</visibility>
    <styleUrl>#Mystyle</styleUrl>""" % (namelabels, attributi)
    return Placemark





def OpenPoint(extrude, altitudemode):
    openPoint = """<Point>
    <extrude>%s</extrude>
    <altitudeMode>%s</altitudeMode>
    <coordinates>""" % (extrude, altitudemode)
    return openPoint


def ClosePoint():
    closePoint = """</coordinates>
    </Point></Placemark>"""
    return closePoint


##########################

def OpenPolygon(extrude, altitudemode):
    openPolygon = """<Polygon>
        <extrude>%s</extrude>
        <altitudeMode>%s</altitudeMode>""" % (extrude, altitudemode)
    return openPolygon


def ClosePolygon():
    closePolygon = """</Polygon></Placemark>"""
    return closePolygon

def OpenOutherBoundary():
    OpenOutherBoundary = "<outerBoundaryIs><LinearRing><coordinates>"
    return OpenOutherBoundary

def CloseOutherBoundary():
    CloseOutheBoundary = "</coordinates></LinearRing></outerBoundaryIs>"
    return CloseOutheBoundary

def OpenInnerBoundary():
    OpenInnerBoundary = "<innerBoundaryIs><LinearRing><coordinates>"
    return OpenInnerBoundary

def CloseInnerBoundary():
    CloseInnerrBoundary = "</coordinates></LinearRing></innerBoundaryIs>"
    return CloseInnerrBoundary

###########################

def CloseKml():
    CloseKml = """</Document></kml>"""
    return CloseKml



def OpenLine(extrude, tessellate, altitudemode):
    openLine = """<LineString>
    <extrude>%s</extrude>
    <tessellate>%s</tessellate>
    <altitudeMode>%s</altitudeMode>
    <coordinates>""" % (extrude, tessellate, altitudemode)
    return openLine
    

def CloseLine():
    closeLine = """</coordinates>
    </LineString></Placemark>"""
    return closeLine
    

def compress_kml(outfile,icon):
    directory = os.path.dirname(str(outfile))
    iconname = icon.split('/')[-1]
    icontosave = directory+'/'+iconname
    string = 'cp %s %s' % (icon, icontosave)
    cp = os.system(string)
    outfilename = outfile.split('/')
    outfilename = outfilename[-1]
    kmz = outfile.split('.')[0]+'.kmz'
    zip = zipfile.ZipFile(str(kmz), 'w')
    zip.write(str(outfile))
    zip.write(str(icontosave))
    zip.close 


def GrassToKml(ExtrudeType, featuretype, infile, outfile, num_lay, name, status, 
               description, colorlabel, labelscale, icon, tessellate, extrude, 
               width, colorline, colormode, colorpolygon, namelabel, lon, lat, 
               alt, rangex, tilt, altitudemode, offset,heightfield,LabelAlpha, 
               LineAlpha, PolygonAlpha):
    global off
    global namelabels
    global AttHeight
    off = offset
    out00 = OpenKml(name, status, description)
    out11 = InitStyle(colorlabel, colormode, icon, colorline, tessellate, width, 
                      colorpolygon, LabelAlpha, LineAlpha, PolygonAlpha)
    f=open(outfile, 'w')
    f.write(out00)
    f.write(out11)
    num = num_lay
    print num
    for l in range(num):
    #if num is not None:
        in_ds = ogr.Open( str(infile), update = 1 )
        in_layer = in_ds.GetLayer( l )
        if in_layer is not None:
            in_defn = in_layer.GetLayerDefn()
            in_field_count = in_defn.GetFieldCount()
            in_feat = in_layer.GetNextFeature()
            in_sr = in_layer.GetSpatialRef()
            out_sr = osr.SpatialReference()
            out_sr.ImportFromEPSG(4326)
            attr = in_layer.GetLayerDefn().GetFieldCount()
            while in_feat is not None:
                geom = in_feat.GetGeometryRef()
                tipo = geom.GetGeometryType()
                geom.AssignSpatialReference(in_sr)
                geom.TransformTo(out_sr)
                attributi = []
                field = dict()
                for i in range(attr):
                    attname = in_layer.GetLayerDefn().GetFieldDefn(i).GetName()
                    attvalue = in_feat.GetField(in_layer.GetLayerDefn().GetFieldDefn(i).GetName())
                    field[attname] = attvalue
                    attributi.append(str('<TR><TD>')+str(attname)+str(':')+str('</TD><TD>')+str(attvalue)+str('</TD>')) 
                print attributi
                namelabels = field.get(str(namelabel))
                print namelabels
                AttHeight = field.get(str(heightfield))
                attributi = str(attributi)
                attributi = attributi.replace('[','')
                attributi = attributi.replace(']','')
                attributi = attributi.replace("'",'')
                attributi = attributi.replace(", ",'')
                if ExtrudeType == '3D':
                    geom = WalkAndTransform( geom, '3D' )
                if ExtrudeType == 'Offset':
                    geom = WalkAndTransform( geom, 'offset' )
                if ExtrudeType == 'Attribute':
                    geom = WalkAndTransform( geom, 'Attribute' )
                coords = str(geom)
                print coords
                holed = '),('
                if holed not in coords:
                    coords = coords.strip()
                    coords = coords.replace('(','')
                    coords = coords.replace(')','')
                    coords = coords.replace('L','')
                    coords = coords.replace('I','')
                    coords = coords.replace('N','')
                    coords = coords.replace('E','')
                    coords = coords.replace('S','')
                    coords = coords.replace('T','')
                    coords = coords.replace('R','')
                    coords = coords.replace('G','')
                    coords = coords.replace('P','')
                    coords = coords.replace('O','')
                    coords = coords.replace('Y','')
                    coords = coords.strip()
                    coords = coords.split(',')
                    number = len(coords)
                    for i in range(number):
                        coords[i] = coords[i].split(' ')
                        coords[i] = ','.join(coords[i])
                    coords = str(coords)
                    coords = coords.replace("'",'')
                    coords = coords.replace(", ",' ')
                    coords = str(coords)
                    coords = coords.replace('[','')
                    coords = coords.replace(']','')
                    if featuretype == 'polygon':
                        opp = OpenOutherBoundary()
                        clp = CloseOutherBoundary()
                        coords = str(opp)+coords+str(clp)
                    #print coords

                if holed in coords:
                    coords = coords.strip()
                    coords = coords.replace('L','')
                    coords = coords.replace('I','')
                    coords = coords.replace('N','')
                    coords = coords.replace('E','')
                    coords = coords.replace('S','')
                    coords = coords.replace('T','')
                    coords = coords.replace('R','')
                    coords = coords.replace('G','')
                    coords = coords.replace('P','')
                    coords = coords.replace('O','')
                    coords = coords.replace('Y','')
                    coords = coords.strip()
                    list_hole = coords.split('),(')
                    num_hole = len(list_hole)
                    print num_hole
                    jk = []
                    for i in range(num_hole):                        
                        coordsh = list_hole[i]
                        coordsh = coordsh.replace('(','')
                        coordsh = coordsh.replace(')','')
                        coordsh = coordsh.split(',')
                        numberh = len(coordsh)
                        for j in range(numberh):
                            coordsh[j] = coordsh[j].split(' ')
                            coordsh[j] = ','.join(coordsh[j])
                        coordsh = str(coordsh)
                        coordsh = coordsh.replace("'",'')
                        coordsh = coordsh.replace(", ",' ')
                        coordsh = coordsh.replace('[','')
                        coordsh = coordsh.replace(']','')
                        oph = OpenInnerBoundary()
                        clh = CloseInnerBoundary()
                        if i == 0:
                            oph = OpenOutherBoundary()
                            clh = CloseOutherBoundary()
                        coordsh = str(oph)+coordsh+str(clh)
                        jk.append(coordsh)
                    jk = str(jk)
                    jk = jk.strip()
                    jk = jk.replace('[','')
                    jk = jk.replace(']','')
                    #jk = jk.replace(',','')
                    jk = jk.replace("'",'')
                    jk = jk.replace(">, <",'><')
                    jk = jk.strip()
                    coords = jk                        
                Place = SetPlacemark(namelabels, attributi, lon, lat, alt, rangex, tilt)
                if featuretype == 'polygon':
                    Feature = OpenPolygon(extrude, altitudemode)
                if featuretype == 'line':
                    Feature = OpenLine(extrude, tessellate, altitudemode)
                if featuretype == 'point':
                    Feature = OpenPoint(extrude, altitudemode)
                f.write(Place)
                f.write(Feature)
                out3 = coords 
                f.write(out3)
                if featuretype == 'polygon':
                    out4 = ClosePolygon()
                if featuretype == 'line':
                    out4 = CloseLine()
                if featuretype == 'point':
                    out4 = ClosePoint()
                f.write(out4)
                in_feat = in_layer.GetNextFeature()
    in_ds.Destroy()
    out5 = CloseKml()
    f.write(out5)
    f.close()
    kmz = compress_kml(str(outfile),str(icon))
    if __name__ == '__main__':
        for arg in sys.argv[1:]:
            grass2kml(infile,num_lay)
