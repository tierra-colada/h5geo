# from h5gtpy import h5gt
# from h5geopy import _h5geo
# import h5gtpy._h5gt

# from pointpy import point
# from linepy import line

# import point
# import line

from h5gtpy import h5gt
from h5geopy import h5geo

if __name__ == '__main__':
    FILE_NAME = 'tmp/well.h5'
    WELL_NAME = 'path/to/well'
    WELL_NAME2 = 'path/to/well2'
    DEV_NAME = 'my_deviation'
    LOG_TYPE = 'GK'
    LOG_NAME = 'my_log'

    wellParam = h5geo.WellParam()
    wellParam.headX = 444363
    wellParam.headY = 7425880
    wellParam.kb = 50.88
    wellParam.uwi = 'my_uwi'
    wellParam.spatialUnits = h5geo.SpatialUnits.METER

    devCurveParam = h5geo.DevCurveParam()
    devCurveParam.spatialUnits = h5geo.SpatialUnits.METER
    devCurveParam.temporalUnits = h5geo.TemporalUnits.MILLISECOND
    devCurveParam.angleUnits = h5geo.AngleUnits.RADIAN



    wellContainer = h5geo.createWellContainerByName(FILE_NAME, h5geo.CreationType.CREATE_OR_OVERWRITE)

    well = wellContainer.createWell('well.h5', wellParam, h5geo.CreationType.CREATE_OR_OVERWRITE)
    # well.createDevCurve('what/root', devCurveParam, h5geo.CreationType.OPEN_OR_CREATE)
    well.createDevCurve('/from/root', devCurveParam, h5geo.CreationType.OPEN_OR_CREATE)

    a = 1

    # p1 = point.Point(1, 2, 3)
    # p2 = point.Point(3, 2, 1)
    #
    # l = line.Line(p1, p2)
    # lp1 = l.getPoint1()

    # p1 = _h5gt.Point(1, 2, 3)
    # p2 = _h5gt.Point(3, 2, 1)

    # line = _h5geo.Line(p1, p2)
    # lp1 = line.getPoint1()

    # p1 = h5gt.Point(1,2,3)
    # line = h5geo.Line()
    # point = h5geo.getPoint_func(1, 2, 3)
    #
    # p1 = point.Point(1, 2, 3)
    # p2 = point.Point(3, 2, 1)
    #
    # l = line.Line(p1, p2)
    #
    # point = h5geo.getPoint_func(1, 2, 3)

    open_flag = h5geo.CreationType.CREATE_OR_OVERWRITE
    # surf_container = h5geo.openSurfContainerByName('surf_container.h5')
    surf_container = h5geo.createSurfContainerByName('surf_container.h5', open_flag)

    surf_param = h5geo.SurfParam()
    surf_param.X0 = 0
    surf_param.Y0 = 0
    surf_param.dX = 1
    surf_param.dY = 1
    surf_param.nX = 100
    surf_param.nY = 100
    surf_param.domain = h5geo.Domain.OWT
    surf_param.spatialUnits = h5geo.SpatialUnits.METER
    surf_param.temporalUnits = h5geo.TemporalUnits.SECOND
    surf_param.dataUnits = "myDataUnits"

    surf_param_1 = h5geo.SurfParam(1, 1, 0, 0, 100, 100, h5geo.Domain.OWT, h5geo.SpatialUnits.METER,
                                   h5geo.TemporalUnits.SECOND, "myDataUnits")

    surf = surf_container.createSurf("mySurf", surf_param, h5geo.CreationType.OPEN_OR_CREATE)

    # test_file = h5gt.File("test_file.h5", h5gt.OpenFlag(h5gt.ReadWrite | h5gt.Create | h5gt.Truncate))
    test_container = h5geo.H5TestContainer("test_file.h5")
    file_back = test_container.getH5File()
    id = file_back.getId(False)
    print(file_back.getFileName())

    f = surf.getH5File()
    id = f.getId(True)
    str = surf.getName()
    f.createGroup("myGroup")
    # g = surf.getObjG()
    # f = surf.getH5File_2()

    file = surf_container.getH5File()
    file.createGroup("myGroup")
    c = 1
