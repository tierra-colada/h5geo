import numpy as np
from h5gtpy import h5gt
from h5geopy import h5geo
import os.path
import unittest
import pathlib

trig = False

class test_h5well(unittest.TestCase):
    def setUp(self):
        self.FILE_NAME = 'tmp/well.h5'
        self.WELL_NAME = 'path/to/well'
        self.WELL_NAME2 = 'path/to/well2'
        self.DEV_NAME = 'my_deviation'
        self.LOG_TYPE = 'GK'
        self.LOG_NAME = 'my_log'

        global trig
        if trig:
            file = h5gt.File(self.FILE_NAME, h5gt.OpenFlag(h5gt.OpenOrCreate))
            self.wellContainer = h5geo.createWellContainer(file, h5geo.CreationType.OPEN_OR_CREATE)
        else:
            pathlib.Path('tmp').mkdir(exist_ok=True)
            file = h5gt.File(self.FILE_NAME, h5gt.OpenFlag(h5gt.OpenOrCreate | h5gt.Overwrite))
            self.wellContainer = h5geo.createWellContainer(file, h5geo.CreationType.CREATE_OR_OVERWRITE)

        trig = True

        self.wellParam = h5geo.WellParam()
        self.wellParam.headX = 444363
        self.wellParam.headY = 7425880
        self.wellParam.kb = 50.88
        self.wellParam.uwi = 'my_uwi'
        self.wellParam.spatialUnits = 'meter'

        self.devCurveParam = h5geo.DevCurveParam()
        self.devCurveParam.spatialUnits = 'meter'
        self.devCurveParam.temporalUnits = 'msec'
        self.devCurveParam.angularUnits = 'radian'

        self.logCurveParam = h5geo.LogCurveParam()
        self.logCurveParam.spatialUnits = 'cm'
        self.logCurveParam.dataUnits = 'kg/m2'

        path_to_current_file = os.path.realpath(__file__)
        current_directory = os.path.dirname(path_to_current_file)

        self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL = np.loadtxt(current_directory + '/data/well_dev', skiprows=11)
        self.LOG_MD_GR = np.loadtxt(current_directory + '/data/well_las', skiprows=72)

    def tearDown(self):
        h5File = self.wellContainer.getH5File()

    FILE_NAME = None
    WELL_NAME = None
    WELL_NAME2 = None
    DEV_NAME = None
    LOG_TYPE = None
    LOG_NAME = None

    MD_X_Y_Z_TVD_DX_DY_AZ_INCL = None
    LOG_MD_GR = None

    wellParam = None
    devCurveParam = None
    logCurveParam = None

    wellContainer = None

    def test_createContainer(self):
        self.assertTrue(os.path.isfile(self.FILE_NAME))

    def test_createWellWithDifferentCreateFlags(self):
        well = self.wellContainer.createWell(self.WELL_NAME, self.wellParam, h5geo.CreationType.OPEN_OR_CREATE)
        self.assertFalse(well is None)

        well = self.wellContainer.createWell(self.WELL_NAME, self.wellParam, h5geo.CreationType.CREATE_OR_OVERWRITE)
        self.assertFalse(well is None)

        well = self.wellContainer.createWell(self.WELL_NAME, self.wellParam, h5geo.CreationType.CREATE_UNDER_NEW_NAME)
        self.assertFalse(well is None)

        well = self.wellContainer.createWell(self.WELL_NAME, self.wellParam, h5geo.CreationType.OPEN_OR_CREATE)
        self.assertFalse(well is None)

    def test_createDevCurveWithDifferentCreateFlags(self):
        well = self.wellContainer.createWell(self.WELL_NAME, self.wellParam, h5geo.CreationType.CREATE_OR_OVERWRITE)
        self.assertFalse(well is None)

        devCurve = well.createDevCurve(self.DEV_NAME, self.devCurveParam, h5geo.CreationType.OPEN_OR_CREATE)
        self.assertFalse(devCurve is None)

        devCurve = well.createDevCurve(self.DEV_NAME, self.devCurveParam, h5geo.CreationType.CREATE_OR_OVERWRITE)
        self.assertFalse(devCurve is None)

        devCurve = well.createDevCurve(self.DEV_NAME, self.devCurveParam, h5geo.CreationType.CREATE_UNDER_NEW_NAME)
        self.assertFalse(devCurve is None)

        devCurve = well.createDevCurve(self.DEV_NAME, self.devCurveParam, h5geo.CreationType.OPEN_OR_CREATE)
        self.assertFalse(devCurve is None)

    def test_createLogCurveWithDifferentCreateFlags(self):
        well = self.wellContainer.createWell(self.WELL_NAME, self.wellParam, h5geo.CreationType.CREATE_OR_OVERWRITE)
        self.assertFalse(well is None)

        logCurve = well.createLogCurve(self.LOG_TYPE, self.LOG_NAME, self.logCurveParam, h5geo.CreationType.OPEN_OR_CREATE)
        self.assertFalse(logCurve is None)

        logCurve = well.createLogCurve(self.LOG_TYPE, self.LOG_NAME, self.logCurveParam, h5geo.CreationType.CREATE_OR_OVERWRITE)
        self.assertFalse(logCurve is None)

        logCurve = well.createLogCurve(self.LOG_TYPE, self.LOG_NAME, self.logCurveParam, h5geo.CreationType.CREATE_UNDER_NEW_NAME)
        self.assertFalse(logCurve is None)

        logCurve = well.createLogCurve(self.LOG_TYPE, self.LOG_NAME, self.logCurveParam, h5geo.CreationType.OPEN_OR_CREATE)
        self.assertFalse(logCurve is None)

    def test_writeReadDevCurve(self):
        well = self.wellContainer.createWell(self.WELL_NAME, self.wellParam, h5geo.CreationType.CREATE_OR_OVERWRITE)
        self.assertFalse(well is None)

        devCurve = well.createDevCurve(self.DEV_NAME, self.devCurveParam, h5geo.CreationType.OPEN_OR_CREATE)
        self.assertFalse(devCurve is None)

        self.assertTrue(devCurve.writeCurve(h5geo.DevDataType.MD, np.asfortranarray(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, 0])))
        self.assertTrue(devCurve.writeCurve(h5geo.DevDataType.X, np.asfortranarray(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, 1])))
        self.assertTrue(devCurve.writeCurve(h5geo.DevDataType.Y, np.asfortranarray(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, 2])))
        self.assertTrue(devCurve.writeCurve(h5geo.DevDataType.TVD, np.asfortranarray(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, 3])))

    def test_MdAzIncl2MdXYTvd(self):
        # self.wellContainer.getH5File().createDataSet(
        #     'MD_X_Y_Z_TVD_DX_DY_AZ_INCL',
        #     [self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL.shape[1], self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL.shape[0]],
        #     h5gt.AtomicDouble()).write_raw(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL)

        M = h5geo.MdAzIncl2MdXYTvd(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, [0, 7, 8]],
                                   self.wellParam.headX, self.wellParam.headY,
                                   'degree', False)

        # self.wellContainer.getH5File().createDataSet(
        #     'MdAzIncl2MdXYTvd', [M.shape[1], M.shape[0]],
        #     h5gt.AtomicDouble()).write_raw(M)

        std_dev = np.std(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, [1, 2, 4]] - M[:, [1, 2, 3]])
        MD_max = self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[-1, 0]    # [-1] returns last element

        self.assertTrue(std_dev/MD_max < 0.05)    # less than 5 %

    def test_MdAzIncl2MdXYTvd_XNorth(self):
        # self.wellContainer.getH5File().createDataSet(
        #     'MD_X_Y_Z_TVD_DX_DY_AZ_INCL',
        #     [self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL.shape[1], self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL.shape[0]],
        #     h5gt.AtomicDouble()).write_raw(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL)

        # here I set XNorth to True
        M = h5geo.MdAzIncl2MdXYTvd(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, [0, 7, 8]],
                                   self.wellParam.headY, self.wellParam.headX,
                                   'degree', True)

        # self.wellContainer.getH5File().createDataSet(
        #     'MdAzIncl2MdXYTvd', [M.shape[1], M.shape[0]],
        #     h5gt.AtomicDouble()).write_raw(M)

        # M[:, [2, 1, 3] intensionally as I test the case when XNorth True
        std_dev = np.std(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, [1, 2, 4]] - M[:, [2, 1, 3]])
        MD_max = self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[-1, 0]  # [-1] returns last element

        self.assertTrue(std_dev / MD_max < 0.05)  # less than 5 %

    def test_TvdXY2MdAzIncl(self):
        # self.wellContainer.getH5File().createDataSet(
        #     'MD_X_Y_Z_TVD_DX_DY_AZ_INCL',
        #     [self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL.shape[1], self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL.shape[0]],
        #     h5gt.AtomicDouble()).write_raw(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL)

        M = h5geo.TvdXY2MdAzIncl(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, [4, 1, 2]],
                                 self.wellParam.headX, self.wellParam.headY,
                                 False)

        MM = h5geo.MdAzIncl2MdXYTvd(M, self.wellParam.headX, self.wellParam.headY, 'radian', False)

        # self.wellContainer.getH5File().createDataSet(
        #     'MdAzIncl2MdXYTvd', [M.shape[1], M.shape[0]],
        #     h5gt.AtomicDouble()).write_raw(M)

        std_dev = np.std(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, [1, 2, 4]] - MM[:, [1, 2, 3]])
        MD_max = self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[-1, 0]    # [-1] returns last element

        self.assertTrue(std_dev/MD_max < 0.05)    # less than 5 %

    def test_TvdXY2MdAzIncl_XNorth(self):
        # self.wellContainer.getH5File().createDataSet(
        #     'MD_X_Y_Z_TVD_DX_DY_AZ_INCL',
        #     [self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL.shape[1], self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL.shape[0]],
        #     h5gt.AtomicDouble()).write_raw(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL)

        M = h5geo.TvdXY2MdAzIncl(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, [4, 2, 1]],
                                 self.wellParam.headX, self.wellParam.headY,
                                 True)

        MM = h5geo.MdAzIncl2MdXYTvd(M, self.wellParam.headX, self.wellParam.headY, 'radian', True)

        # self.wellContainer.getH5File().createDataSet(
        #     'MdAzIncl2MdXYTvd', [M.shape[1], M.shape[0]],
        #     h5gt.AtomicDouble()).write_raw(M)

        std_dev = np.std(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, [1, 2, 4]] - MM[:, [2, 1, 3]])
        MD_max = self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[-1, 0]    # [-1] returns last element

        self.assertTrue(std_dev/MD_max < 0.05)    # less than 5 %

    def test_TvdXY2MdXYTvd(self):
        M = h5geo.TvdXY2MdXYTvd(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, [4, 1, 2]],
                                self.wellParam.headX, self.wellParam.headY,
                                False)

        # self.wellContainer.getH5File().createDataSet(
        #     'TvdXY2MdXYTvd', [M.shape[1], M.shape[0]],
        #     h5gt.AtomicDouble()).write_raw(M)

        std_dev = np.std(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, 0] - M[:, 0])
        MD_max = self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[-1, 0]  # [-1] returns last element

        self.assertTrue(std_dev / MD_max < 0.05)  # less than 5 %

    def test_TvdXY2MdXYTvd_XNorth(self):
        M = h5geo.TvdXY2MdXYTvd(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, [4, 1, 2]],
                                self.wellParam.headX, self.wellParam.headY,
                                True)

        # self.wellContainer.getH5File().createDataSet(
        #     'TvdXY2MdXYTvd', [M.shape[1], M.shape[0]],
        #     h5gt.AtomicDouble()).write_raw(M)

        std_dev = np.std(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, 0] - M[:, 0])
        MD_max = self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[-1, 0]  # [-1] returns last element

        self.assertTrue(std_dev / MD_max < 0.05)  # less than 5 %

    def test_TvdDxDy2MdXYTvd(self):
        M = h5geo.TvdDxDy2MdXYTvd(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, [4, 5, 6]],
                                  self.wellParam.headX, self.wellParam.headY,
                                  False)

        # self.wellContainer.getH5File().createDataSet(
        #     'TvdDxDy2MdXYTvd', [M.shape[1], M.shape[0]],
        #     h5gt.AtomicDouble()).write_raw(M)

        std_dev = np.std(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, 0] - M[:, 0])
        MD_max = self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[-1, 0]  # [-1] returns last element

        self.assertTrue(std_dev / MD_max < 0.05)  # less than 5 %

    def test_TvdDxDy2MdXYTvd_XNorth(self):
        M = h5geo.TvdDxDy2MdXYTvd(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, [4, 5, 6]],
                                  self.wellParam.headX, self.wellParam.headY,
                                  True)

        # self.wellContainer.getH5File().createDataSet(
        #     'TvdDxDy2MdXYTvd', [M.shape[1], M.shape[0]],
        #     h5gt.AtomicDouble()).write_raw(M)

        std_dev = np.std(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, 0] - M[:, 0])
        MD_max = self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[-1, 0]  # [-1] returns last element

        self.assertTrue(std_dev / MD_max < 0.05)  # less than 5 %

    def test_TvdssXY2MdXYTvd(self):
        TvdssXY = self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, [4, 1, 2]]
        TvdssXY[:, 0] += self.wellParam.kb

        M = h5geo.TvdssXY2MdXYTvd(TvdssXY,
                                  self.wellParam.headX, self.wellParam.headY, self.wellParam.kb,
                                  False)

        # self.wellContainer.getH5File().createDataSet(
        #     'TvdssXY2MdXYTvd', [M.shape[1], M.shape[0]],
        #     h5gt.AtomicDouble()).write_raw(M)

        std_dev = np.std(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, 0] - M[:, 0])
        MD_max = self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[-1, 0]  # [-1] returns last element

        self.assertTrue(std_dev / MD_max < 0.05)  # less than 5 %

    def test_TvdssXY2MdXYTvd_XNorth(self):
        TvdssXY = self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, [4, 1, 2]]
        TvdssXY[:, 0] += self.wellParam.kb

        M = h5geo.TvdssXY2MdXYTvd(TvdssXY,
                                  self.wellParam.headX, self.wellParam.headY, self.wellParam.kb,
                                  True)

        # self.wellContainer.getH5File().createDataSet(
        #     'TvdssXY2MdXYTvd', [M.shape[1], M.shape[0]],
        #     h5gt.AtomicDouble()).write_raw(M)

        std_dev = np.std(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, 0] - M[:, 0])
        MD_max = self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[-1, 0]  # [-1] returns last element

        self.assertTrue(std_dev / MD_max < 0.05)  # less than 5 %

    def test_TvdssDxDy2MdXYTvd(self):
        TvdssXY = self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, [4, 5, 6]]
        TvdssXY[:, 0] += self.wellParam.kb

        M = h5geo.TvdssDxDy2MdXYTvd(TvdssXY,
                                    self.wellParam.headX, self.wellParam.headY, self.wellParam.kb,
                                    False)

        # self.wellContainer.getH5File().createDataSet(
        #     'TvdssDxDy2MdXYTvd', [M.shape[1], M.shape[0]],
        #     h5gt.AtomicDouble()).write_raw(M)

        std_dev = np.std(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, 0] - M[:, 0])
        MD_max = self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[-1, 0]  # [-1] returns last element

        self.assertTrue(std_dev / MD_max < 0.05)  # less than 5 %

    def test_TvdssDxDy2MdXYTvd_XNorth(self):
        TvdssXY = self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, [4, 5, 6]]
        TvdssXY[:, 0] += self.wellParam.kb

        M = h5geo.TvdssDxDy2MdXYTvd(TvdssXY,
                                    self.wellParam.headX, self.wellParam.headY, self.wellParam.kb,
                                    True)

        # self.wellContainer.getH5File().createDataSet(
        #     'TvdssDxDy2MdXYTvd', [M.shape[1], M.shape[0]],
        #     h5gt.AtomicDouble()).write_raw(M)

        std_dev = np.std(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, 0] - M[:, 0])
        MD_max = self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[-1, 0]  # [-1] returns last element

        self.assertTrue(std_dev / MD_max < 0.05)  # less than 5 %

    def test_writeReadLogCurve(self):
        well = self.wellContainer.createWell(self.WELL_NAME, self.wellParam, h5geo.CreationType.CREATE_OR_OVERWRITE)
        self.assertFalse(well is None)

        logCurve = well.createLogCurve(self.LOG_TYPE, self.LOG_NAME, self.logCurveParam, h5geo.CreationType.OPEN_OR_CREATE)

        self.assertFalse(logCurve is None)

        self.assertTrue(logCurve.writeCurve(h5geo.LogDataType.MD, np.asfortranarray(self.LOG_MD_GR[:, 0])))
        self.assertTrue(logCurve.writeCurve(h5geo.LogDataType.VAL, np.asfortranarray(self.LOG_MD_GR[:, 1])))

        self.assertTrue(np.allclose(self.LOG_MD_GR[:, 0], logCurve.getCurve(h5geo.LogDataType.MD)))
        self.assertTrue(np.allclose(self.LOG_MD_GR[:, 1], logCurve.getCurve(h5geo.LogDataType.VAL)))

    def test_getWellFromCurve(self):
        well = self.wellContainer.createWell(self.WELL_NAME, self.wellParam, h5geo.CreationType.CREATE_OR_OVERWRITE)
        self.assertFalse(well is None)

        logCurve = well.createLogCurve(self.LOG_TYPE, self.LOG_NAME, self.logCurveParam,
                                       h5geo.CreationType.OPEN_OR_CREATE)

        devCurve = well.createDevCurve(self.DEV_NAME, self.devCurveParam, h5geo.CreationType.OPEN_OR_CREATE)

        self.assertFalse(devCurve is None)
        self.assertTrue(devCurve.getWell())
        self.assertTrue(logCurve.getWell())
        self.assertTrue(devCurve.getWellContainer())
        self.assertTrue(logCurve.getWellContainer())

    def test_activeDevCurveTest(self):
        well = self.wellContainer.createWell(self.WELL_NAME, self.wellParam, h5geo.CreationType.CREATE_OR_OVERWRITE)
        self.assertFalse(well is None)

        devCurve = well.createDevCurve(self.DEV_NAME, self.devCurveParam, h5geo.CreationType.OPEN_OR_CREATE)

        self.assertTrue(devCurve.writeCurve(h5geo.DevDataType.MD, np.asfortranarray(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, 0])))
        self.assertTrue(devCurve.writeCurve(h5geo.DevDataType.X, np.asfortranarray(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, 1])))
        self.assertTrue(devCurve.writeCurve(h5geo.DevDataType.Y, np.asfortranarray(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, 2])))
        self.assertTrue(devCurve.writeCurve(h5geo.DevDataType.TVD, np.asfortranarray(self.MD_X_Y_Z_TVD_DX_DY_AZ_INCL[:, 4])))

        self.assertFalse(devCurve.isActive())
        self.assertTrue(devCurve.setActive())
        self.assertTrue(devCurve.isActive())

        activeCurve = well.getActiveDevCurve()
        self.assertFalse(activeCurve is None)

        self.assertTrue(
            np.allclose(devCurve.getCurve(h5geo.DevDataType.MD), activeCurve.getCurve(h5geo.DevDataType.MD)))
        self.assertTrue(
            np.allclose(devCurve.getCurve(h5geo.DevDataType.X), activeCurve.getCurve(h5geo.DevDataType.X)))
        self.assertTrue(
            np.allclose(devCurve.getCurve(h5geo.DevDataType.Y), activeCurve.getCurve(h5geo.DevDataType.Y)))
        self.assertTrue(
            np.allclose(devCurve.getCurve(h5geo.DevDataType.TVD), activeCurve.getCurve(h5geo.DevDataType.TVD)))

    def test_getCurveFromDifferentWell(self):
        wellName1 = '1R'
        wellName2 = '2R'
        devCurveName1 = '/1R/DEV/my_dev'
        logCurveType1 = '/1R/LOG/my_log_type'
        logCurveName1 = '/1R/LOG/my_log_type/my_log_name'
        devCurveName2 = '/2R/DEV/my_dev'
        logCurveType2 = '/2R/LOG/my_log_type'
        logCurveName2 = '/2R/LOG/my_log_type/my_log_name'

        well_1 = self.wellContainer.createWell(wellName1, self.wellParam, h5geo.CreationType.CREATE_OR_OVERWRITE)
        well_2 = self.wellContainer.createWell(wellName2, self.wellParam, h5geo.CreationType.CREATE_OR_OVERWRITE)

        self.assertFalse(well_1.createDevCurve(devCurveName2, self.devCurveParam, h5geo.CreationType.OPEN_OR_CREATE))
        self.assertFalse(well_1.createLogCurve(logCurveType2, logCurveName2, self.logCurveParam, h5geo.CreationType.OPEN_OR_CREATE))
        self.assertFalse(well_2.createDevCurve(devCurveName1, self.devCurveParam, h5geo.CreationType.OPEN_OR_CREATE))
        self.assertFalse(well_2.createLogCurve(logCurveType1, logCurveName1, self.logCurveParam, h5geo.CreationType.OPEN_OR_CREATE))

        self.assertTrue(well_1.createDevCurve(devCurveName1, self.devCurveParam, h5geo.CreationType.OPEN_OR_CREATE))
        self.assertTrue(well_1.createLogCurve(logCurveType1, logCurveName1, self.logCurveParam, h5geo.CreationType.OPEN_OR_CREATE))
        self.assertTrue(well_2.createDevCurve(devCurveName2, self.devCurveParam, h5geo.CreationType.OPEN_OR_CREATE))
        self.assertTrue(well_2.createLogCurve(logCurveType2, logCurveName2, self.logCurveParam, h5geo.CreationType.OPEN_OR_CREATE))

        self.assertTrue(well_1.getDevCurve(devCurveName2) is None)
        self.assertTrue(well_1.getLogCurve(logCurveType2, logCurveName2) is None)
        self.assertTrue(well_2.getDevCurve(devCurveName1) is None)
        self.assertTrue(well_2.getLogCurve(logCurveType1, logCurveName1) is None)

        self.assertFalse(well_1.getDevCurve(devCurveName1) is None)
        self.assertFalse(well_1.getLogCurve(logCurveType1, logCurveName1) is None)
        self.assertFalse(well_2.getDevCurve(devCurveName2) is None)
        self.assertFalse(well_2.getLogCurve(logCurveType2, logCurveName2) is None)


if __name__ == '__main__':
    unittest.main()