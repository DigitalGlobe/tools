# -*- coding: utf-8 -*-

"""
***************************************************************************
    lasground_new.py
    Date                 : May 2016
    Copyright            : (C) 2016 by Martin Isenburg
    Email                : martin near rapidlasso point com
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
"""

__author__ = 'Martin Isenburg'
__date__ = 'May 2016'
__copyright__ = '(C) 2016 by Martin Isenburg'
# This will get replaced with a git SHA1 when you do a git archive
__revision__ = '$Format:%H$'

import os
from LAStoolsUtils import LAStoolsUtils
from LAStoolsAlgorithm import LAStoolsAlgorithm

from processing.core.parameters import ParameterBoolean
from processing.core.parameters import ParameterNumber
from processing.core.parameters import ParameterSelection


class lasground_new(LAStoolsAlgorithm):
    TERRAIN = "TERRAIN"
    TERRAINS = ["wilderness", "nature", "town", "city", "metro", "custom"]
    GRANULARITY = "GRANULARITY"
    GRANULARITIES = ["coarse", "default", "fine", "extra_fine", "ultra_fine", "hyper_fine"]
    STEP = "STEP"
    BULGE = "BULGE"
    SPIKE = "SPIKE"
    DOWN_SPIKE = "DOWN_SPIKE"
    OFFSET = "OFFSET"

    def defineCharacteristics(self):
        self.name, self.i18n_name = self.trAlgorithm('lasground_new')
        self.group, self.i18n_group = self.trAlgorithm('LAStools')
        self.addParametersVerboseGUI()
        self.addParametersPointInputGUI()
        self.addParametersIgnoreClass1GUI()
        self.addParametersHorizontalAndVerticalFeetGUI()
        self.addParameter(ParameterSelection(lasground_new.TERRAIN,
                                             self.tr("terrain type"), lasground_new.TERRAINS, 1))
        self.addParameter(ParameterSelection(lasground_new.GRANULARITY,
                                             self.tr("preprocessing"), lasground_new.GRANULARITIES, 1))
        self.addParameter(ParameterNumber(lasground_new.STEP,
                                          self.tr("step (for 'custom' terrain only)"), 25.0))
        self.addParameter(ParameterNumber(lasground_new.BULGE,
                                          self.tr("bulge (for 'custom' terrain only)"), 2.0))
        self.addParameter(ParameterNumber(lasground_new.SPIKE,
                                          self.tr("spike (for 'custom' terrain only)"), 1.0))
        self.addParameter(ParameterNumber(lasground_new.DOWN_SPIKE,
                                          self.tr("down spike (for 'custom' terrain only)"), 1.0))
        self.addParameter(ParameterNumber(lasground_new.OFFSET,
                                          self.tr("offset (for 'custom' terrain only)"), 0.05))
        self.addParametersPointOutputGUI()
        self.addParametersAdditionalGUI()

    def processAlgorithm(self, progress):
        commands = [os.path.join(LAStoolsUtils.LAStoolsPath(), "bin", "lasground_new")]
        self.addParametersVerboseCommands(commands)
        self.addParametersPointInputCommands(commands)
        self.addParametersIgnoreClass1Commands(commands)
        self.addParametersHorizontalAndVerticalFeetCommands(commands)
        method = self.getParameterValue(lasground_new.TERRAIN)
        if (method == 5):
            commands.append("-step")
            commands.append(unicode(self.getParameterValue(lasground_new.STEP)))
            commands.append("-bulge")
            commands.append(unicode(self.getParameterValue(lasground_new.BULGE)))
            commands.append("-spike")
            commands.append(unicode(self.getParameterValue(lasground_new.SPIKE)))
            commands.append("-spike_down")
            commands.append(unicode(self.getParameterValue(lasground_new.DOWN_SPIKE)))
            commands.append("-offset")
            commands.append(unicode(self.getParameterValue(lasground_new.OFFSET)))
        else:
            commands.append("-" + lasground_new.TERRAINS[method])
        granularity = self.getParameterValue(lasground_new.GRANULARITY)
        if (granularity != 1):
            commands.append("-" + lasground_new.GRANULARITIES[granularity])
        self.addParametersPointOutputCommands(commands)
        self.addParametersAdditionalCommands(commands)

        LAStoolsUtils.runLAStools(commands, progress)
