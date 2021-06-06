# from h5gtpy import h5gt
from h5geopy import h5geo
# import h5gtpy._h5gt

if __name__ == '__main__':
    open_flag = h5geo.CreationType.OPEN_OR_CREATE
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

    f = surf.getH5File()
    # g = surf.getObjG()
    # f = surf.getH5File_2()

    file = surf_container.getH5File()
    file.createGroup("myGroup")
    c = 1
