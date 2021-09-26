import numpy as np
from h5gtpy import h5gt
from h5geopy import h5geo
import os.path
import unittest
import pathlib

trig = False

class test_h5surf(unittest.TestCase):
    def setUp(self):
        self.FILE_NAME = 'tmp/surf.h5'
        self.SURF_NAME1 = 'path1/to/surface'
        self.SURF_NAME2 = 'path2/to/surface'

        global trig
        if trig:
            file = h5gt.File(self.FILE_NAME, h5gt.OpenFlag(h5gt.OpenOrCreate))
            self.surfContainer = h5geo.createSurfContainer(file, h5geo.CreationType.OPEN_OR_CREATE)
        else:
            pathlib.Path('tmp').mkdir(exist_ok=True)
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
        self.p.spatialUnits = 'cm'
        self.p.dataUnits = "m/s"

    def tearDown(self):
        h5File = self.surfContainer.getH5File()

    surfContainer = None
    p = h5geo.SurfParam()
    FILE_NAME = None
    SURF_NAME1 = None
    SURF_NAME2 = None

    def test_createContainer(self):
        self.assertTrue(os.path.isfile(self.FILE_NAME))


    def test_createSurfWithDifferentCreateFlags(self):
        surf = self.surfContainer.createSurf(self.SURF_NAME1, self.p, h5geo.CreationType.OPEN_OR_CREATE)
        self.assertFalse(surf is None)

        surf = self.surfContainer.createSurf(self.SURF_NAME1, self.p, h5geo.CreationType.CREATE_OR_OVERWRITE)
        self.assertFalse(surf is None)

        surf = self.surfContainer.createSurf(self.SURF_NAME1, self.p, h5geo.CreationType.CREATE_UNDER_NEW_NAME)
        self.assertFalse(surf is None)

        surf = self.surfContainer.createSurf(self.SURF_NAME1, self.p, h5geo.CreationType.OPEN_OR_CREATE)
        self.assertFalse(surf is None)

    def test_createAndGetSurf(self):
        surf = self.surfContainer.createSurf(self.SURF_NAME1, self.p, h5geo.CreationType.CREATE_OR_OVERWRITE)
        self.surfContainer.getSurf(self.SURF_NAME1)
        self.assertFalse(surf is None)

    def test_createAndGetSurfFromGroup(self):
        group = self.surfContainer.getH5File().createGroup(self.SURF_NAME2)
        self.surfContainer.createSurf(group, self.p, h5geo.CreationType.CREATE_OR_OVERWRITE)
        surf = self.surfContainer.getSurf(self.SURF_NAME2)
        self.assertFalse(surf is None)

    def test_writeAndGetDataFromSurf(self):
        m = np.random.rand(self.p.nY, self.p.nX)
        m = np.asfortranarray(m)
        surf = self.surfContainer.createSurf(self.SURF_NAME1, self.p, h5geo.CreationType.CREATE_OR_OVERWRITE)
        self.assertTrue(surf.writeData(m))

        M = surf.getData()
        self.assertTrue(np.allclose(m, M))


if __name__ == '__main__':
    unittest.main()