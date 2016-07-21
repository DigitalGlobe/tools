#---------------------------------------------------------------------
# 
# Planet Sasha - Plugin to Communicate troght tcp to Ossimplanet 
#                (3D geospatial data visualization )
#               
#               
#
# Copyright (C) 2009  Massimo Di Stefano
#
# EMAIL: massimodisasha (at) yahoo.it
# WEB  : www.geofemengineering.it
#
#---------------------------------------------------------------------
# 
# licensed under the terms of GNU GPL 2
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
# 
#---------------------------------------------------------------------

from planetsasha import PlanetSashaPlugin

def name():
  return "PlanetSasha"

def description():
  return "Plugin to Communicate troght tcp to Ossimplanet "

def version():
  return "Version 0.1"

def qgisMinimumVersion():
  return "1.0.0"

def authorName():
  return "Massimo Di Stefano "

def homepage():
  return "http://www.geofemengineering.it/qgis"

def classFactory(iface):
  return PlanetSashaPlugin(iface)
