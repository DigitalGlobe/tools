#---------------------------------------------------------------------
# 
# PlanetSasha - A QGIS plugin to integrate Ossimplanet
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

INST_DIR = /home/aaronr/.qgis/python/plugins/PlanetSasha

PYRCC = /usr/bin/pyrcc4
PYUIC = /usr/bin/pyuic4

RC_PY_FILE = resources_rc.py

all: $(RC_PY_FILE)

install: all
	mkdir -p $(INST_DIR)
	cp *.py $(INST_DIR)/
	cp *.png $(INST_DIR)/

clean:
	rm -f $(RC_PY_FILE)
	rm -f *.pyc

$(RC_PY_FILE): resources.qrc
	$(PYRCC) -o $(RC_PY_FILE) resources.qrc


