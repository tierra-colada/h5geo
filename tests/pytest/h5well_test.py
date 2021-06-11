import numpy as np
from h5gtpy import h5gt
from h5geopy import h5geo
import os.path
import unittest

trig = False

class h5well_test(unittest.TestCase):
    def setUp(self):
        global trig
        if trig:
            file = h5gt.File(self.FILE_NAME, h5gt.OpenFlag(h5gt.OpenOrCreate))
            self.surfContainer = h5geo.createSurfContainer(file, h5geo.CreationType.OPEN_OR_CREATE)
        else:
            file = h5gt.File(self.FILE_NAME, h5gt.OpenFlag(h5gt.OpenOrCreate | h5gt.Overwrite))
            self.surfContainer = h5geo.createSurfContainer(file, h5geo.CreationType.CREATE_OR_OVERWRITE)

        trig = True

        self.p.X0 = 0
        self.p.Y0 = 10
        self.p.dX = 2
        self.p.dY = 4
        self.p.nX = 10
        self.p.nY = 20
        self.p.domain = h5geo.Domain.TVD
        self.p.spatialUnits = h5geo.SpatialUnits.CENTIMETER
        self.p.temporalUnits = h5geo.TemporalUnits.MILLISECOND
        self.p.dataUnits = "m/s"

    def tearDown(self):
        h5File = self.surfContainer.getH5File()

    MD_X_Y_Z_TVD_DX_DY_AZ_INCL = np.loadtxt('well_dev', skiprows=11)

    wellContainer = None
    p = h5geo.SurfParam()
    FILE_NAME = "surf.h5"
    SURF_NAME1 = "path1/to/surface"
    SURF_NAME2 = "path2/to/surface"

    def test_createContainer(self):
        self.assertTrue(os.path.isfile(self.FILE_NAME))