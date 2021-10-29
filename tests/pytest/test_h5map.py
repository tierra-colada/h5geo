import numpy as np
from h5gtpy import h5gt
from h5geopy import h5geo
import os.path
import unittest
import pathlib

trig = False

class test_h5map(unittest.TestCase):
    def setUp(self):
        self.FILE_NAME = 'tmp/map.h5'
        self.MAP_NAME1 = 'path1/to/map'
        self.MAP_NAME2 = 'path2/to/map'

        global trig
        if trig:
            file = h5gt.File(self.FILE_NAME, h5gt.OpenFlag(h5gt.OpenOrCreate))
            self.mapContainer = h5geo.createMapContainer(file, h5geo.CreationType.OPEN_OR_CREATE)
        else:
            pathlib.Path('tmp').mkdir(exist_ok=True)
            file = h5gt.File(self.FILE_NAME, h5gt.OpenFlag(h5gt.OpenOrCreate | h5gt.Overwrite))
            self.mapContainer = h5geo.createMapContainer(file, h5geo.CreationType.CREATE_OR_OVERWRITE)

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
        h5File = self.mapContainer.getH5File()

    mapContainer = None
    p = h5geo.MapParam()
    FILE_NAME = None
    MAP_NAME1 = None
    MAP_NAME2 = None

    def test_createContainer(self):
        self.assertTrue(os.path.isfile(self.FILE_NAME))


    def test_createMapWithDifferentCreateFlags(self):
        map = self.mapContainer.createMap(self.MAP_NAME1, self.p, h5geo.CreationType.OPEN_OR_CREATE)
        self.assertFalse(map is None)

        map = self.mapContainer.createMap(self.MAP_NAME1, self.p, h5geo.CreationType.CREATE_OR_OVERWRITE)
        self.assertFalse(map is None)

        map = self.mapContainer.createMap(self.MAP_NAME1, self.p, h5geo.CreationType.CREATE_UNDER_NEW_NAME)
        self.assertFalse(map is None)

        map = self.mapContainer.createMap(self.MAP_NAME1, self.p, h5geo.CreationType.OPEN_OR_CREATE)
        self.assertFalse(map is None)

    def test_createAndGetMap(self):
        map = self.mapContainer.createMap(self.MAP_NAME1, self.p, h5geo.CreationType.CREATE_OR_OVERWRITE)
        self.mapContainer.getMap(self.MAP_NAME1)
        self.assertFalse(map is None)

    def test_createAndGetMapFromGroup(self):
        group = self.mapContainer.getH5File().createGroup(self.MAP_NAME2)
        self.mapContainer.createMap(group, self.p, h5geo.CreationType.CREATE_OR_OVERWRITE)
        map = self.mapContainer.getMap(self.MAP_NAME2)
        self.assertFalse(map is None)

    def test_writeAndGetDataFromMap(self):
        m = np.random.rand(self.p.nY, self.p.nX)
        m = np.asfortranarray(m)
        map = self.mapContainer.createMap(self.MAP_NAME1, self.p, h5geo.CreationType.CREATE_OR_OVERWRITE)
        self.assertTrue(map.writeData(m))

        M = map.getData()
        self.assertTrue(np.allclose(m, M))


if __name__ == '__main__':
    unittest.main()