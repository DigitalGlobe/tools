#!/usr/bin/env python

try:
    from osgeo import osr
    from osgeo import ogr
except ImportError:
    import osr
    import ogr

import string
import sys
import os
from math import pi
from scipy import io, linalg
import scipy.linalg
import scipy.io
from numpy import zeros
from numpy import dot
from math import cos
from math import sin
import time

infile = None
outfile = None
layer_name = None

def WalkAndTransform( geom ):
	if geom.GetGeometryCount() > 0:
		for i in range(geom.GetGeometryCount()):
			old_geom = geom.GetGeometryRef(i)
			new_geom = WalkAndTransform( old_geom )
			if new_geom is not old_geom:
				geom.SetGeometryDirectly( new_geom )
		return geom
	for i in range(geom.GetPointCount()):
		xyz = (geom.GetX(i), geom.GetY(i), geom.GetZ(i))
		xyz = TXY( xyz )
		geom.SetPoint( i, xyz[0], xyz[1], xyz[2] )
	return geom




def TXY( xyz ):
	x = xyz[0] + int(tx) 
	y = xyz[1] + int(ty) 
	z = xyz[2]
	return (float(x),float(y),float(z))


def sTOsTxy(infile,outfile,tr_x,tr_y):
	i = 1
	global tx
	global ty
	global cntx
	global cnty
	while i < len(sys.argv):
		arg = sys.argv[i]
		if infile is None:
			infile = arg
		elif outfile is None:
			outfile = arg
		elif angle is None:
			angle = arg
		else:
			Usage()
		i = i + 1
	if outfile is None:
		Usage()
	in_ds = ogr.Open( str(infile), update = 0 )
	if layer_name is not None:
		in_layer = in_ds.GetLayerByName( layer_name )
	else:
		in_layer = in_ds.GetLayer( 0 )
		in_defn = in_layer.GetLayerDefn()
	ext = in_ds.GetLayerByIndex(0).GetExtent()
	tx = tr_x
	ty = tr_y
	cntx = ext[0] + ((ext[1]-ext[0])/2)
	cnty = ext[2] + ((ext[3]-ext[2])/2)
	shp_driver = ogr.GetDriverByName( 'ESRI Shapefile' )
	shp_driver.DeleteDataSource( str(outfile) )
	shp_ds = shp_driver.CreateDataSource( str(outfile) )
	shp_layer = shp_ds.CreateLayer( in_defn.GetName(),geom_type = in_defn.GetGeomType(),srs = in_layer.GetSpatialRef() )
	in_field_count = in_defn.GetFieldCount()
	for fld_index in range(in_field_count):
		src_fd = in_defn.GetFieldDefn( fld_index )
		fd = ogr.FieldDefn( src_fd.GetName(), src_fd.GetType() )
		fd.SetWidth( src_fd.GetWidth() )
		fd.SetPrecision( src_fd.GetPrecision() )
		shp_layer.CreateField( fd )
	in_feat = in_layer.GetNextFeature()
	while in_feat is not None:
		geom = in_feat.GetGeometryRef().Clone()
		geom = WalkAndTransform( geom )
		out_feat = ogr.Feature( feature_def = shp_layer.GetLayerDefn() )
		out_feat.SetFrom( in_feat )
		out_feat.SetGeometryDirectly( geom )
		shp_layer.CreateFeature( out_feat )
		out_feat.Destroy()
		in_feat.Destroy()
		in_feat = in_layer.GetNextFeature()
	shp_ds.Destroy()
	in_ds.Destroy()


def WalkAndTransformRota( geom ):
	if geom.GetGeometryCount() > 0:
		for i in range(geom.GetGeometryCount()):
			old_geom = geom.GetGeometryRef(i)
			new_geom = WalkAndTransformRota( old_geom )
			if new_geom is not old_geom:
				geom.SetGeometryDirectly( new_geom )
		return geom
	for i in range(geom.GetPointCount()):
		xyz = (geom.GetX(i), geom.GetY(i), geom.GetZ(i))
		xyz = rotaxy( xyz )
                time.sleep(1)
		geom.SetPoint( i, xyz[0], xyz[1], xyz[2] )
	return geom




def rotaxy( xyz ):
	x = xyz[0] - cntx 
	y = xyz[1] - cnty 
	z = xyz[2]
	angle = angolo
	ar = angle * pi/180.
	R = zeros((2,2),float)
	R[0,0]=cos(ar)
	R[0,1]=sin(ar)
	R[1,0]=-sin(ar)
	R[1,1]=cos(ar)
	zed = zeros((2,1),float)
	zed[0,0]=x
	zed[1,0]=y
	zz = zeros((2,1),float)
	d = dot(R,zed)
	x = d[0] + cntx
	y = d[1] + cnty
	return (float(x),float(y),float(z))


def sTOsRota(infile,outfile,ang):
	i = 1
	global angolo
	global cntx
	global cnty
	while i < len(sys.argv):
		arg = sys.argv[i]
		if infile is None:
			infile = arg
		elif outfile is None:
			outfile = arg
		elif angle is None:
			angle = arg
		else:
			Usage()
		i = i + 1
	if outfile is None:
		Usage()
	in_ds = ogr.Open( str(infile), update = 0 )
	if layer_name is not None:
		in_layer = in_ds.GetLayerByName( layer_name )
	else:
		in_layer = in_ds.GetLayer( 0 )
		in_defn = in_layer.GetLayerDefn()
	angolo = float(ang)
	ext = in_ds.GetLayerByIndex(0).GetExtent()
	cntx = ext[0] + ((ext[1]-ext[0])/2)
	cnty = ext[2] + ((ext[3]-ext[2])/2)
	shp_driver = ogr.GetDriverByName( 'ESRI Shapefile' )
	shp_driver.DeleteDataSource( str(outfile) )
	shp_ds = shp_driver.CreateDataSource( str(outfile) )
	shp_layer = shp_ds.CreateLayer( in_defn.GetName(),geom_type = in_defn.GetGeomType(),srs = in_layer.GetSpatialRef() )
	in_field_count = in_defn.GetFieldCount()
	for fld_index in range(in_field_count):
		src_fd = in_defn.GetFieldDefn( fld_index )
		fd = ogr.FieldDefn( src_fd.GetName(), src_fd.GetType() )
		fd.SetWidth( src_fd.GetWidth() )
		fd.SetPrecision( src_fd.GetPrecision() )
		shp_layer.CreateField( fd )
	in_feat = in_layer.GetNextFeature()
	while in_feat is not None:
		geom = in_feat.GetGeometryRef().Clone()
		geom = WalkAndTransformRota( geom )
		out_feat = ogr.Feature( feature_def = shp_layer.GetLayerDefn() )
		out_feat.SetFrom( in_feat )
		out_feat.SetGeometryDirectly( geom )
		shp_layer.CreateFeature( out_feat )
		out_feat.Destroy()
		in_feat.Destroy()
		in_feat = in_layer.GetNextFeature()
	shp_ds.Destroy()
	in_ds.Destroy()

def WalkAndTransformConf( geom ):
	if geom.GetGeometryCount() > 0:
		for i in range(geom.GetGeometryCount()):
			old_geom = geom.GetGeometryRef(i)
			new_geom = WalkAndTransformConf( old_geom )
			if new_geom is not old_geom:
				geom.SetGeometryDirectly( new_geom )
		return geom
	for i in range(geom.GetPointCount()):
		xyz = (geom.GetX(i), geom.GetY(i), geom.GetZ(i))
		xyz = conformexy( xyz )
		geom.SetPoint( i, xyz[0], xyz[1], xyz[2] )
	return geom


def conformexy( xyz ):
	L = LL
	G = GG
	x = xyz[0]
	y = xyz[1]
	z = xyz[2]
	A = zeros((2*L.shape[0],4),float)
	A[ ::2, 0] = 1.0
	A[1::2, 1] = 1.0
	A[ ::2, 2] = L[:,0]
	A[1::2, 2] = L[:,1]
	A[ ::2, 3] = L[:,1]
	A[1::2, 3] = -L[:,0]
	Y = zeros((2*G.shape[0],1),float)
	Y[ ::2, 0] = G[:,0]
	Y[1::2, 0] = G[:,1]
	N = dot(A.T.conj(), A)
	T = dot(A.T.conj(), Y)
	C = dot(linalg.inv(N), T)
	Lambda = abs(C[2]+C[3]*1j)
	Alpha = angle(C[2]+C[3]*1j)
	E0 = C[0]
	N0 = C[1]
	x = E0+xyz[0]*C[2]+xyz[1]*C[3]
	y = N0+xyz[1]*C[2]-xyz[0]*C[3]			
	return (float(x),float(y),float(z))



def sTOsConforme(infile,outfile,locale,globale):
	i = 1
	global LL
	global GG
	while i < len(sys.argv):
		arg = sys.argv[i]
		if infile is None:
			infile = arg
		elif outfile is None:
			outfile = arg
		elif locale is None:
			locale = arg
		elif globale is None:
			globale = arg
#		elif layer_name is None:
#			layer_name = arg
		else:
			Usage()
		i = i + 1
	if outfile is None:
		Usage()
	in_ds = ogr.Open( str(infile), update = 0 )
	if layer_name is not None:
		in_layer = in_ds.GetLayerByName( layer_name )
	else:
		in_layer = in_ds.GetLayer( 0 )
		in_defn = in_layer.GetLayerDefn()
	LL = io.read_array(str(locale))
	GG = io.read_array(str(globale))
	shp_driver = ogr.GetDriverByName( 'ESRI Shapefile' )
	shp_driver.DeleteDataSource( str(outfile) )
	shp_ds = shp_driver.CreateDataSource( str(outfile) )
	shp_layer = shp_ds.CreateLayer( in_defn.GetName(),geom_type = in_defn.GetGeomType(),srs = in_layer.GetSpatialRef() )
	in_field_count = in_defn.GetFieldCount()
	for fld_index in range(in_field_count):
		src_fd = in_defn.GetFieldDefn( fld_index )
		fd = ogr.FieldDefn( src_fd.GetName(), src_fd.GetType() )
		fd.SetWidth( src_fd.GetWidth() )
		fd.SetPrecision( src_fd.GetPrecision() )
		shp_layer.CreateField( fd )
	in_feat = in_layer.GetNextFeature()
	while in_feat is not None:
		geom = in_feat.GetGeometryRef().Clone()
		geom = WalkAndTransformConf( geom )
		out_feat = ogr.Feature( feature_def = shp_layer.GetLayerDefn() )
		out_feat.SetFrom( in_feat )
		out_feat.SetGeometryDirectly( geom )
		shp_layer.CreateFeature( out_feat )
		out_feat.Destroy()
		in_feat.Destroy()
		in_feat = in_layer.GetNextFeature()
	shp_ds.Destroy()
	in_ds.Destroy()


def WalkAndTransformAff( geom ):
	if geom.GetGeometryCount() > 0:
		for i in range(geom.GetGeometryCount()):
			old_geom = geom.GetGeometryRef(i)
			new_geom = WalkAndTransformAff( old_geom )
			if new_geom is not old_geom:
				geom.SetGeometryDirectly( new_geom )
		return geom
	for i in range(geom.GetPointCount()):
		xyz = (geom.GetX(i), geom.GetY(i), geom.GetZ(i))
		xyz = affinexy( xyz )
		geom.SetPoint( i, xyz[0], xyz[1], xyz[2] )
	return geom



def affinexy( xyz ):
	L = LL
	G = GG
	x = xyz[0]
	y = xyz[1]
	z = xyz[2]
	A = zeros((2 * L.shape[0],6),float)
	A[ ::2, 2] = 1.0
	A[1::2, 5] = 1.0
	A[ ::2, 0] = L[:,0]
	A[1::2, 4] = L[:,1]
	A[ ::2, 1] = L[:,1]
	A[1::2, 3] = L[:,0]
	Y = zeros((2*G.shape[0],1),float)
	Y[ ::2, 0] = G[:,0]
	Y[1::2, 0] = G[:,1]
	N = dot(A.T.conj(), A)
	T = dot(A.T.conj(), Y)
	C = dot(linalg.inv(N), T)
	E0 = C[2]
	N0 = C[5]
	x = E0+xyz[0]*C[0]+xyz[1]*C[1]
	y = N0+xyz[0]*C[3]+xyz[1]*C[4]
	return (float(x),float(y),float(z))


def sTOsAffine(infile,outfile,locale,globale):
	i = 1
	global LL
	global GG
	while i < len(sys.argv):
		arg = sys.argv[i]
		if infile is None:
			infile = arg
		elif outfile is None:
			outfile = arg
		elif locale is None:
			locale = arg
		elif globale is None:
			globale = arg
#		elif layer_name is None:
#			layer_name = arg
		else:
			Usage()
		i = i + 1
	if outfile is None:
		Usage()
	in_ds = ogr.Open( str(infile), update = 0 )
	if layer_name is not None:
		in_layer = in_ds.GetLayerByName( layer_name )
	else:
		in_layer = in_ds.GetLayer( 0 )
		in_defn = in_layer.GetLayerDefn()
	LL = io.read_array(str(locale))
	GG = io.read_array(str(globale))
	shp_driver = ogr.GetDriverByName( 'ESRI Shapefile' )
	shp_driver.DeleteDataSource( str(outfile) )
	shp_ds = shp_driver.CreateDataSource( str(outfile) )
	shp_layer = shp_ds.CreateLayer( in_defn.GetName(),geom_type = in_defn.GetGeomType(),srs = in_layer.GetSpatialRef() )
	in_field_count = in_defn.GetFieldCount()
	for fld_index in range(in_field_count):
		src_fd = in_defn.GetFieldDefn( fld_index )
		fd = ogr.FieldDefn( src_fd.GetName(), src_fd.GetType() )
		fd.SetWidth( src_fd.GetWidth() )
		fd.SetPrecision( src_fd.GetPrecision() )
		shp_layer.CreateField( fd )
	in_feat = in_layer.GetNextFeature()
	while in_feat is not None:
		geom = in_feat.GetGeometryRef().Clone()
		geom = WalkAndTransformAff( geom )
		out_feat = ogr.Feature( feature_def = shp_layer.GetLayerDefn() )
		out_feat.SetFrom( in_feat )
		out_feat.SetGeometryDirectly( geom )
		shp_layer.CreateFeature( out_feat )
		out_feat.Destroy()
		in_feat.Destroy()
		in_feat = in_layer.GetNextFeature()
	shp_ds.Destroy()
	in_ds.Destroy()


if __name__ == '__main__':
	for arg in sys.argv[1:]:
		sTOsRota(infile,outfile,tr_x,tr_y)
