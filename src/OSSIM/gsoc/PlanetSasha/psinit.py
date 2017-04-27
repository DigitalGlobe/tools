from grass.script.core import *
from datetime import *
import osgeo.gdal as gdal
import os
from ogrTovrt import ogrvrt
import tempfile
import time

def GetVersion():
	while 1:
		try :
			version = read_command("g.version")
			if '6.5' in version :
				gversion = 65
			if '6.4' in version :
				gversion = 64
			if '7.0' in version :
				gversion = 70
			return gversion
			break
		except IOError:
			time.sleep(0.1)
	



def getEnv():
	while 1:
		try :
			grassenv = gisenv()
			return grassenv
			break
		except IOError:
			time.sleep(0.1)

		
def RasterList():
	while 1:
		try:
			r = list_strings('rast')
			break
		except IOError:
			time.sleep(0.1)
	raster = []
	for i in r:
		rname , mname = i.split('@', 2)
		if mname == getEnv()['MAPSET']:
			raster.append(rname)
	raster.sort()
	return raster


def VectorList():
	while 1:
		try:
			v = list_strings('vect')
			break
		except IOError:
			time.sleep(0.1)
	vector = []
	for i in v:
		vname , mname = i.split('@', 2)
		if mname == getEnv()['MAPSET']:
			vector.append(vname)
	vector.sort()
	return vector


def getRegionList():
	print 'return a list of aviaiable regions using g.list'
	while 1:
		try:
			rg = list_strings('region')
			break
		except IOError:
			time.sleep(0.1)
	region = []
	for i in v:
		rgname , mname = i.split('@', 2)
		if mname == getEnv()['MAPSET']:
			region.append(rgname)
	region.sort()
	return region



def setCLL():
	while 1:
		try:
			s = read_command("g.region", flags='c')
			break
		except IOError:
			time.sleep(0.1)
	region = parse_key_val(s, ':')
	clon_deg = region['east-west center']
	clat_deg = region['north-south center']
	centro_deg = (clon_deg,clat_deg)
	return centro_deg

	
def setCPRJ():
	while 1:
		try:
			s = read_command("g.region", flags='l')
			break
		except IOError:
			time.sleep(0.1)
	region = parse_key_val(s, ':')
	clon_deg = region['center longitude']
	clat_deg = region['center latitude']
	centro_deg = (clon_deg,clat_deg)
	return centro_deg



def getlonlat(x, y):
	# x,y=-103.749921939,44.4360855064
	grassversion = GetVersion()
	f = tempfile.NamedTemporaryFile(delete=False)
	coordsfile2 = f.name
	lonlat = str(x)+' '+str(y)
	f.write(lonlat)
	f.flush()
	f.close
	#while 1:
	try:
		xy = read_command('m.proj', input=coordsfile2, flags='id')
		if grassversion == 70 :
			xy = xy.replace('\n','').split('|')
			return xy
		if grassversion == 65 :
			xy = xy.replace('\t',' ').replace('\n','').split(' ')[:-1]
			return xy
		#break
	except IOError:
		print 'm.proj failed'
		time.sleep(0.1)
	os.unlink(f.name)
	


def projinfo():
	while 1:
		try :
			units = read_command("g.proj", flags='p')
			break
		except IOError:
			time.sleep(0.1)                
	units = units.replace('-','')
	#units = units.replace('\n','')
	units = parse_key_val(units, ':')
	units_key = units.keys()
	for i in units_key :
		key_value = str(units[i]).strip()
		units[i] =  key_value
	return units

	
def setcenter():
	clon = 0
	clat = 0
	proj_info = projinfo()
	unit = proj_info['units']
	if unit == 'metres'or unit == 'meters':
		clon = setCPRJ()[0]
		clat = setCPRJ()[1]
	if unit == 'degrees' or unit == 'degree':
		clon = setCLL()[0]
		clat = setCLL()[1]
	clon = str(clon)
	clat = str(clat)
	clon = clon.replace(':', " ")
	clat = clat.replace(':', " ")
	if clat[-1] == 'N':
		signlat = 1
	if clat[-1] == 'S':
		signlat = -1
	if clon[-1] == 'E':
		signlon = 1
	if clon[-1] == 'W':
		signlon = -1
	clat = clat[:-1] 
	clon = clon[:-1]
	clat = [float(i) for i in clat.split()]
	clon = [float(i) for i in clon.split()]
	if len(clon) > 2 :
		clat = (clat[0] + (clat[1] / 60) + clat[2] / 3600) * float(signlat)
		clon = (clon[0] + (clon[1] / 60) + clon[2] / 3600) * float(signlon)
	else :
		clat = (clat[0] + (clat[1] / 60)) * float(signlat)
		clon = (clon[0] + (clon[1] / 60)) * float(signlon)
	slvallon = clon 
	slvallat = clat
	centro = (clon,clat)
	return centro

	
def setMapCenter(mappa):
	now = datetime.now()
	region = now.strftime("region_%d_%m_%y_%H_%M_%S")
	run_command("g.region", flags='a')
	run_command("g.region", save=region)
	run_command("g.region", rast=mappa, flags='a')
	centro = setcenter()
	run_command("g.region", region=region)
	run_command("g.remove", region=region)
	print centro


def getFields(mappa):
	while 1:
		try:
			s = read_command('v.info', map=infile, flags='c') 
			break
		except IOError:
			time.sleep(0.1)
	attr = parse_key_val(s,'|')
	attr = attr.values()
	return attr


def getTile(mappa):
	while 1:
		try:
			res = read_command("r.info", map=inputfile, flags='s')
			break
		except IOError:
			time.sleep(0.1)
	res = parse_key_val(res, '=')
	ewres = float(res['ewres'])
	nsres = float(res['nsres'])
	tiling = ((108000 + nsres) / ewres )
	return tiling



	
		
def initVrt():
	rasterpath = os.path.join(getEnv()['GISDBASE'], getEnv()['LOCATION_NAME'], getEnv()['MAPSET'], 'cellhd')
	vrtdirR = os.path.join(getEnv()['GISDBASE'], getEnv()['LOCATION_NAME'], getEnv()['MAPSET'], 'vrt','raster/')
	vrtdirV = os.path.join(getEnv()['GISDBASE'], getEnv()['LOCATION_NAME'], getEnv()['MAPSET'], 'vrt', 'vector/')
	r = os.path.dirname(vrtdirR)
	v = os.path.dirname(vrtdirV)
	if not os.path.exists(r):
		os.makedirs(r)
	if not os.path.exists(v):
		os.makedirs(v)
	for i in RasterList():
		mapfile = os.path.join(rasterpath, i)
		vrtfilename =  i + '.vrt'
		output = os.path.join(vrtdirR, vrtfilename)
		print output
		try :
			gdal.GetDriverByName('VRT').CreateCopy(output,gdal.Open(mapfile))
			os.system('ossim-img2rr --create-histogram %s' % (output) )
		except :
			print 'vrt creation failed for map %s ' % (i)
	for i in VectorList():
		vectorpath = os.path.join(getEnv()['GISDBASE'], getEnv()['LOCATION_NAME'], getEnv()['MAPSET'], 'vector' )
		mapfile = os.path.join(vectorpath, i, 'head')
		vrtfilename =  i + '.vrt'
		outvrt = os.path.join(vrtdirV, vrtfilename)
		try :
			ogrvrt(mapfile,outvrt)
		except :
			print 'ogrvrt failed for map : %s' % (i) 
		#outomd = output.replace('.vrt','.omd')
		#if os.path.isfile(outomd):
		#	os.remove(outomd)
		#opions = {}
		# options['brush'], options['pen'], options['size'], options['fill'], options['thickness'] = ... ... ... ... ...
		#makestile(output, options['brush'], options['pen'], options['size'], options['fill'], options['thickness'])
    


def getMapsets():
	print 'ask grass which mapset are avaiable'
	print 'investigate how to hallow access to other mapsets'


def main():
	info = {}
	info['Version'] = GetVersion()
	info['Raster'] = RasterList()
	info['Vector'] = VectorList()
	info['CenterLL'] = setcenter()
	x, y = setcenter()[0], setcenter()[1]
	info['CenterXY'] = getlonlat(x, y)
	return info
	
	


def make3d():
	print 'for now make a gneral raster and add an option for hgt'


def saveRegion():
	print "run g.region save usando parametri custom (letti daplanetsasha : x,y(centro) e deriva in base allo zoom, l'estensione)"


def SetRegion():
	print "set region using name"



def GetGRASSCmds(bin = True, scripts = True, gui_scripts = True):
	"""!Create list of all available GRASS commands to use when
	parsing string from the command line
	"""
	gisbase = os.environ['GISBASE']
	cmd = list()
	if bin is True:
		for file in os.listdir(os.path.join(gisbase, 'bin')):
			if subprocess.mswindows:
				EXT_BIN = '.exe'
				EXT_SCT = '.bat'
			else:
				EXT_BIN = ''
				EXT_SCT = ''
			if not EXT_BIN or file[-4:] == EXT_BIN or file[-4:] == EXT_SCT:
				cmd.append(file)
        
        # add special call for setting vector colors
		cmd.append('vcolors')
	if scripts:
		cmd = cmd + os.listdir(os.path.join(gisbase, 'scripts')) 
	if gui_scripts:
		os.environ["PATH"] = os.getenv("PATH") + os.pathsep + os.path.join(gisbase, 'etc', 'gui', 'scripts')
		os.environ["PATH"] = os.getenv("PATH") + os.pathsep + os.path.join(gisbase, 'etc', 'wxpython', 'scripts')
		cmd = cmd + os.listdir(os.path.join(gisbase, 'etc', 'gui', 'scripts'))

	if subprocess.mswindows:
		for idx in range(len(cmd)):
			if cmd[idx][-4:] in (EXT_BIN, EXT_SCT):
				cmd[idx] = cmd[idx][:-4]
    
	return cmd

#grassCmd = {}
#grassCmd['all'] = GetGRASSCmds()
#grassCmd['script'] = GetGRASSCmds(bin = False)


#info = main()
	