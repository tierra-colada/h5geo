"""API to work with geo-data (seismic, wells, maps, other in process) based on HDF5 and originally written in C++: https://github.com/tierra-colada/h5geo"""
import h5geopy._h5geo
import typing
import h5gtpy._h5gt
import numpy
import sys
_Shape = typing.Tuple[int, ...]

__all__ = [
    "BaseObjectParam",
    "CaseSensitivity",
    "ContainerType",
    "CreationType",
    "Delimiter",
    "DevCurveParam",
    "DevDataType",
    "Domain",
    "H5Base",
    "H5BaseContainer",
    "H5BaseObject",
    "H5DevCurve",
    "H5LogCurve",
    "H5Map",
    "H5MapContainer",
    "H5Points",
    "H5Seis",
    "H5SeisContainer",
    "H5Well",
    "H5WellContainer",
    "LogCurveParam",
    "LogDataType",
    "MapParam",
    "MdAzIncl2ALL",
    "MdAzIncl2MdXYTvd",
    "ObjectDeleter",
    "ObjectType",
    "Point3",
    "Point3Array",
    "PointsParam",
    "Endian",
    "SegyFormat",
    "SeisDataType",
    "SeisParam",
    "SurveyType",
    "TrajectoryFormat",
    "TvdDxDy2ALL",
    "TvdDxDy2MdAzIncl",
    "TvdDxDy2MdXYTvd",
    "TvdXY2ALL",
    "TvdXY2MdAzIncl",
    "TvdXY2MdXYTvd",
    "TvdssDxDy2ALL",
    "TvdssDxDy2MdAzIncl",
    "TvdssDxDy2MdXYTvd",
    "TvdssXY2ALL",
    "TvdssXY2MdAzIncl",
    "TvdssXY2MdXYTvd",
    "TextEncoding",
    "WellDataType",
    "WellName",
    "WellParam",
    "WellType",
    "createMapContainer",
    "createMapContainerByName",
    "createSeisContainer",
    "createSeisContainerByName",
    "createWellContainer",
    "createWellContainerByName",
    "getBinHeaderBytes",
    "getBinHeaderCount",
    "getBinHeaderNames",
    "getTraceHeaderBytes",
    "getTraceHeaderCount",
    "getTraceHeaderNames",
    "interp1Monotonic",
    "isDevCurve",
    "isGeoContainer",
    "isGeoContainerByType",
    "isGeoObject",
    "isGeoObjectByType",
    "isLogCurve",
    "isMap",
    "isSeis",
    "isWell",
    "openBaseContainer",
    "openBaseContainerByName",
    "openBaseObject",
    "openContainer",
    "openContainerByName",
    "openDevCurve",
    "openLogCurve",
    "openMap",
    "openMapContainer",
    "openMapContainerByName",
    "openObject",
    "openSeis",
    "openSeisContainer",
    "openSeisContainerByName",
    "openWell",
    "openWellContainer",
    "openWellContainerByName",
    "quickHull2D",
    "sort",
    "sort_rows",
    "sort_rows_unique",
    "sort_unique",
    "sr",
    "traj2ALL"
]


class BaseObjectParam():
    def __init__(self) -> None: ...
    @property
    def angularUnits(self) -> str:
        """
        :type: str
        """
    @angularUnits.setter
    def angularUnits(self, arg0: str) -> None:
        pass
    @property
    def dataUnits(self) -> str:
        """
        :type: str
        """
    @dataUnits.setter
    def dataUnits(self, arg0: str) -> None:
        pass
    @property
    def lengthUnits(self) -> str:
        """
        :type: str
        """
    @lengthUnits.setter
    def lengthUnits(self, arg0: str) -> None:
        pass
    @property
    def spatialReference(self) -> str:
        """
        :type: str
        """
    @spatialReference.setter
    def spatialReference(self, arg0: str) -> None:
        pass
    @property
    def temporalUnits(self) -> str:
        """
        :type: str
        """
    @temporalUnits.setter
    def temporalUnits(self, arg0: str) -> None:
        pass
    pass
class CaseSensitivity():
    """
    Members:

      CASE_SENSITIVE

      CASE_INSENSITIVE
    """
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    CASE_INSENSITIVE: h5geopy._h5geo.CaseSensitivity # value = <CaseSensitivity.CASE_INSENSITIVE: 2>
    CASE_SENSITIVE: h5geopy._h5geo.CaseSensitivity # value = <CaseSensitivity.CASE_SENSITIVE: 1>
    __members__: dict # value = {'CASE_SENSITIVE': <CaseSensitivity.CASE_SENSITIVE: 1>, 'CASE_INSENSITIVE': <CaseSensitivity.CASE_INSENSITIVE: 2>}
    pass
class ContainerType():
    """
    Members:

      MAP

      WELL

      SEISMIC
    """
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    MAP: h5geopy._h5geo.ContainerType # value = <ContainerType.MAP: 1>
    SEISMIC: h5geopy._h5geo.ContainerType # value = <ContainerType.SEISMIC: 3>
    WELL: h5geopy._h5geo.ContainerType # value = <ContainerType.WELL: 2>
    __members__: dict # value = {'MAP': <ContainerType.MAP: 1>, 'WELL': <ContainerType.WELL: 2>, 'SEISMIC': <ContainerType.SEISMIC: 3>}
    pass
class CreationType():
    """
    Members:

      OPEN

      CREATE

      OPEN_OR_CREATE

      CREATE_OR_OVERWRITE

      CREATE_UNDER_NEW_NAME
    """
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    CREATE: h5geopy._h5geo.CreationType # value = <CreationType.CREATE: 2>
    CREATE_OR_OVERWRITE: h5geopy._h5geo.CreationType # value = <CreationType.CREATE_OR_OVERWRITE: 4>
    CREATE_UNDER_NEW_NAME: h5geopy._h5geo.CreationType # value = <CreationType.CREATE_UNDER_NEW_NAME: 5>
    OPEN: h5geopy._h5geo.CreationType # value = <CreationType.OPEN: 1>
    OPEN_OR_CREATE: h5geopy._h5geo.CreationType # value = <CreationType.OPEN_OR_CREATE: 3>
    __members__: dict # value = {'OPEN': <CreationType.OPEN: 1>, 'CREATE': <CreationType.CREATE: 2>, 'OPEN_OR_CREATE': <CreationType.OPEN_OR_CREATE: 3>, 'CREATE_OR_OVERWRITE': <CreationType.CREATE_OR_OVERWRITE: 4>, 'CREATE_UNDER_NEW_NAME': <CreationType.CREATE_UNDER_NEW_NAME: 5>}
    pass
class Delimiter():
    """
    Members:

      TABULATION

      SEMICOLON

      DOT

      SPACE

      COMMA
    """
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    COMMA: h5geopy._h5geo.Delimiter # value = <Delimiter.COMMA: 16>
    DOT: h5geopy._h5geo.Delimiter # value = <Delimiter.DOT: 4>
    SEMICOLON: h5geopy._h5geo.Delimiter # value = <Delimiter.SEMICOLON: 2>
    SPACE: h5geopy._h5geo.Delimiter # value = <Delimiter.SPACE: 8>
    TABULATION: h5geopy._h5geo.Delimiter # value = <Delimiter.TABULATION: 1>
    __members__: dict # value = {'TABULATION': <Delimiter.TABULATION: 1>, 'SEMICOLON': <Delimiter.SEMICOLON: 2>, 'DOT': <Delimiter.DOT: 4>, 'SPACE': <Delimiter.SPACE: 8>, 'COMMA': <Delimiter.COMMA: 16>}
    pass
class DevCurveParam(BaseObjectParam):
    def __init__(self) -> None: ...
    @property
    def chunkSize(self) -> int:
        """
        :type: int
        """
    @chunkSize.setter
    def chunkSize(self, arg0: int) -> None:
        pass
    pass
class DevDataType():
    """
    Members:

      MD

      X

      Y

      Z

      TVD

      TVDSS

      DX

      DY

      AZIM

      INCL

      OWT

      TWT
    """
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    AZIM: h5geopy._h5geo.DevDataType # value = <DevDataType.AZIM: 9>
    DX: h5geopy._h5geo.DevDataType # value = <DevDataType.DX: 7>
    DY: h5geopy._h5geo.DevDataType # value = <DevDataType.DY: 8>
    INCL: h5geopy._h5geo.DevDataType # value = <DevDataType.INCL: 10>
    MD: h5geopy._h5geo.DevDataType # value = <DevDataType.MD: 1>
    OWT: h5geopy._h5geo.DevDataType # value = <DevDataType.OWT: 11>
    TVD: h5geopy._h5geo.DevDataType # value = <DevDataType.TVD: 5>
    TVDSS: h5geopy._h5geo.DevDataType # value = <DevDataType.TVDSS: 6>
    TWT: h5geopy._h5geo.DevDataType # value = <DevDataType.TWT: 12>
    X: h5geopy._h5geo.DevDataType # value = <DevDataType.X: 2>
    Y: h5geopy._h5geo.DevDataType # value = <DevDataType.Y: 3>
    Z: h5geopy._h5geo.DevDataType # value = <DevDataType.Z: 4>
    __members__: dict # value = {'MD': <DevDataType.MD: 1>, 'X': <DevDataType.X: 2>, 'Y': <DevDataType.Y: 3>, 'Z': <DevDataType.Z: 4>, 'TVD': <DevDataType.TVD: 5>, 'TVDSS': <DevDataType.TVDSS: 6>, 'DX': <DevDataType.DX: 7>, 'DY': <DevDataType.DY: 8>, 'AZIM': <DevDataType.AZIM: 9>, 'INCL': <DevDataType.INCL: 10>, 'OWT': <DevDataType.OWT: 11>, 'TWT': <DevDataType.TWT: 12>}
    pass
class Domain():
    """
    Members:

      TVD

      TVDSS

      TWT

      OWT
    """
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    OWT: h5geopy._h5geo.Domain # value = <Domain.OWT: 8>
    TVD: h5geopy._h5geo.Domain # value = <Domain.TVD: 1>
    TVDSS: h5geopy._h5geo.Domain # value = <Domain.TVDSS: 2>
    TWT: h5geopy._h5geo.Domain # value = <Domain.TWT: 4>
    __members__: dict # value = {'TVD': <Domain.TVD: 1>, 'TVDSS': <Domain.TVDSS: 2>, 'TWT': <Domain.TWT: 4>, 'OWT': <Domain.OWT: 8>}
    pass
class H5Base():
    def Delete(self) -> None: ...
    def clone(self) -> H5Base: ...
    pass
class H5BaseContainer(H5Base):
    def __eq__(self, arg0: H5BaseContainer) -> bool: ...
    def __ne__(self, arg0: H5BaseContainer) -> bool: ...
    @typing.overload
    def createPoints(self, arg0: h5gtpy._h5gt.Group, arg1: PointsParam, arg2: CreationType) -> H5Points: ...
    @typing.overload
    def createPoints(self, arg0: str, arg1: PointsParam, arg2: CreationType) -> H5Points: ...
    def getH5File(self) -> h5gtpy._h5gt.File: ...
    def getObjGroupList(self, arg0: ObjectType) -> typing.List[h5gtpy._h5gt.Group]: ...
    def getObjNameList(self, arg0: ObjectType) -> typing.List[str]: ...
    def isEqual(self, arg0: H5BaseContainer) -> bool: ...
    @typing.overload
    def openPoints(self, arg0: h5gtpy._h5gt.Group) -> H5Points: ...
    @typing.overload
    def openPoints(self, arg0: str) -> H5Points: ...
    __hash__ = None
    pass
class H5BaseObject(H5Base):
    def __eq__(self, arg0: H5BaseObject) -> bool: ...
    def __ne__(self, arg0: H5BaseObject) -> bool: ...
    def createCoordinateTransformationToReadData(self, arg0: str) -> OGRCoordinateTransformation: ...
    def createCoordinateTransformationToWriteData(self, arg0: str) -> OGRCoordinateTransformation: ...
    def getAngularUnits(self) -> str: ...
    def getDataUnits(self) -> str: ...
    def getFullName(self) -> str: ...
    def getH5File(self) -> h5gtpy._h5gt.File: ...
    def getLengthUnits(self) -> str: ...
    def getName(self) -> str: ...
    def getObjG(self) -> h5gtpy._h5gt.Group: ...
    def getObjGroupList(self, arg0: ObjectType) -> typing.List[h5gtpy._h5gt.Group]: ...
    def getObjNameList(self, arg0: ObjectType) -> typing.List[str]: ...
    def getSpatialReference(self) -> str: ...
    def getTemporalUnits(self) -> str: ...
    def isEqual(self, arg0: H5BaseObject) -> bool: ...
    def setAngularUnits(self, arg0: str) -> bool: ...
    def setDataUnits(self, arg0: str) -> bool: ...
    def setLengthUnits(self, arg0: str) -> bool: ...
    @typing.overload
    def setSpatialReference(self, arg0: str) -> bool: ...
    @typing.overload
    def setSpatialReference(self, arg0: str, arg1: str) -> bool: ...
    def setTemporalUnits(self, arg0: str) -> bool: ...
    __hash__ = None
    pass
class H5DevCurve(H5BaseObject, H5Base):
    @typing.overload
    def getCurve(self, name: DevDataType, units: str = str(), doCoordTransform: bool = False) -> numpy.ndarray[numpy.float64, _Shape[m, 1]]: ...
    @typing.overload
    def getCurve(self, name: str, units: str = str(), doCoordTransform: bool = False) -> numpy.ndarray[numpy.float64, _Shape[m, 1]]: ...
    def getDevCurveD(self) -> typing.Optional[h5gtpy._h5gt.DataSet]: ...
    def getNCurves(self) -> int: ...
    def getNSamp(self) -> int: ...
    def getRelativeCurveName(self) -> str: ...
    def isActive(self) -> bool: ...
    def openWell(self) -> H5Well: ...
    def openWellContainer(self) -> H5WellContainer: ...
    def setActive(self) -> bool: ...
    def updateMdAzimIncl(self) -> None: ...
    def updateTvdDxDy(self) -> None: ...
    def writeAZIM(self, v: numpy.ndarray[numpy.float64, _Shape[m, 1]], units: str = str()) -> bool: ...
    def writeDX(self, v: numpy.ndarray[numpy.float64, _Shape[m, 1]], units: str = str()) -> bool: ...
    def writeDY(self, v: numpy.ndarray[numpy.float64, _Shape[m, 1]], units: str = str()) -> bool: ...
    def writeINCL(self, v: numpy.ndarray[numpy.float64, _Shape[m, 1]], units: str = str()) -> bool: ...
    def writeMD(self, v: numpy.ndarray[numpy.float64, _Shape[m, 1]], units: str = str()) -> bool: ...
    def writeOWT(self, v: numpy.ndarray[numpy.float64, _Shape[m, 1]], units: str = str()) -> bool: ...
    def writeTVD(self, v: numpy.ndarray[numpy.float64, _Shape[m, 1]], units: str = str()) -> bool: ...
    pass
class H5LogCurve(H5BaseObject, H5Base):
    @typing.overload
    def getCurve(self, name: LogDataType, units: str = str()) -> numpy.ndarray[numpy.float64, _Shape[m, 1]]: ...
    @typing.overload
    def getCurve(self, name: str, units: str = str()) -> numpy.ndarray[numpy.float64, _Shape[m, 1]]: ...
    def getLogCurveD(self) -> typing.Optional[h5gtpy._h5gt.DataSet]: ...
    def getRelativeCurveName(self) -> str: ...
    def openWell(self) -> H5Well: ...
    def openWellContainer(self) -> H5WellContainer: ...
    @typing.overload
    def writeCurve(self, name: LogDataType, data: numpy.ndarray[numpy.float64, _Shape[m, 1]], units: str = str()) -> bool: ...
    @typing.overload
    def writeCurve(self, name: str, data: numpy.ndarray[numpy.float64, _Shape[m, 1]], units: str = str()) -> bool: ...
    pass
class H5Map(H5BaseObject, H5Base):
    def addAttribute(self, map: H5Map, name: str = str()) -> bool: ...
    def addExternalAttribute(self, map: H5Map, name: str = str()) -> bool: ...
    def getAttribute(self, name: str) -> H5Map: ...
    def getData(self, dataUnits: str = str()) -> numpy.ndarray[numpy.float64, _Shape[m, n]]: ...
    def getDomain(self) -> Domain: ...
    def getMapD(self) -> typing.Optional[h5gtpy._h5gt.DataSet]: ...
    def getOrigin(self, lengthUnits: str = str(), doCoordTransform: bool = False) -> numpy.ndarray[numpy.float64, _Shape[m, 1]]: ...
    def getPoint1(self, lengthUnits: str = str(), doCoordTransform: bool = False) -> numpy.ndarray[numpy.float64, _Shape[m, 1]]: ...
    def getPoint2(self, lengthUnits: str = str(), doCoordTransform: bool = False) -> numpy.ndarray[numpy.float64, _Shape[m, 1]]: ...
    def openMapContainer(self) -> H5MapContainer: ...
    def removeAttribute(self, name: str) -> bool: ...
    def setDomain(self, arg0: Domain) -> bool: ...
    def setOrigin(self, xy: numpy.ndarray[numpy.float64, _Shape[2, 1]], lengthUnits: str = str(), doCoordTransform: bool = False) -> bool: ...
    def setPoint1(self, xy: numpy.ndarray[numpy.float64, _Shape[2, 1]], lengthUnits: str = str(), doCoordTransform: bool = False) -> bool: ...
    def setPoint2(self, xy: numpy.ndarray[numpy.float64, _Shape[2, 1]], lengthUnits: str = str(), doCoordTransform: bool = False) -> bool: ...
    def writeData(self, data: numpy.ndarray[numpy.float64, _Shape[m, n]], dataUnits: str = str()) -> bool: ...
    pass
class H5MapContainer(H5BaseContainer, H5Base):
    @typing.overload
    def createMap(self, arg0: h5gtpy._h5gt.Group, arg1: MapParam, arg2: CreationType) -> H5Map: ...
    @typing.overload
    def createMap(self, arg0: str, arg1: MapParam, arg2: CreationType) -> H5Map: ...
    @typing.overload
    def openMap(self, arg0: h5gtpy._h5gt.Group) -> H5Map: ...
    @typing.overload
    def openMap(self, arg0: str) -> H5Map: ...
    pass
class H5Points(H5BaseObject, H5Base):
    def getContainer(self) -> H5BaseContainer: ...
    def getData(self, lengthUnits: str = str(), temporalUnits: str = str(), doCoordTransform: bool = False) -> typing.List[Point3]: ...
    def getDomain(self) -> Domain: ...
    def getPointsD(self) -> typing.Optional[h5gtpy._h5gt.DataSet]: ...
    def setDomain(self, arg0: Domain) -> bool: ...
    def writeData(self, data: typing.List[Point3], lengthUnits: str = str(), temporalUnits: str = str(), doCoordTransform: bool = False) -> bool: ...
    pass
class H5Seis(H5BaseObject, H5Base):
    def addPKeySort(self, pKeyName: str) -> bool: ...
    def checkSampleLimits(self, fromSampInd: int, nSamp: int) -> typing.Tuple[int, bool]: 
        """
        check 'fromSampInd' and 'nSamp' and diminish 'nSamp' to fit in data limits (if 'fromSampInd' is inside limit)`fromSampInd` first index (the value should be less then number of samples)`nSamp` number of samples (to read for example). Return corrected `nSamp`
        """
    def checkTraceHeaderLimits(self, fromHdr: int, nHdr: int) -> typing.Tuple[int, bool]: 
        """
        check 'fromHdr' and 'nHdr' and diminish 'nHdr' to fit in data limits (if 'fromHdr' is inside limit)`fromHdr` first header (usually there are 78 headers so the value should be less then this value)`nHdr` number of headers (to read for example). Return corrected `nHdr`
        """
    def checkTraceLimits(self, fromTrc: int, nTrc: int) -> typing.Tuple[int, bool]: 
        """
        check 'fromTrc' and 'nTrc' and diminish 'nTrc' to fit in data limits (if 'fromTrc' is inside limit)`fromTrc` first trace (the value should be less then number of traces)`nTrc` number of traces (to read for example). Return corrected `nTrc`
        """
    @typing.overload
    def getBinHeader(self) -> typing.Dict[str, float]: ...
    @typing.overload
    def getBinHeader(self, hdrName: str, unitsFrom: str = str(), unitsTo: str = str()) -> float: ...
    def getBinHeaderD(self) -> typing.Optional[h5gtpy._h5gt.DataSet]: ...
    def getBinHeaderIndex(self, hdrName: str) -> int: ...
    def getBoundary(self, units: str = str(), doCoordTransform: bool = False) -> numpy.ndarray[numpy.float64, _Shape[m, n]]: ...
    def getBoundaryD(self) -> typing.Optional[h5gtpy._h5gt.DataSet]: ...
    def getDataType(self) -> SeisDataType: ...
    def getDomain(self) -> Domain: ...
    def getFirstSample(self, trcInd: int, units: str = str()) -> float: 
        """
        in units according to `Domain` (`METER` or `SECOND` or else...)
        """
    def getIndexesG(self) -> typing.Optional[h5gtpy._h5gt.Group]: ...
    def getLastSample(self, trcInd: int, units: str = str()) -> float: 
        """
        in units according to `Domain` (`METER` or `SECOND` or else...)
        """
    def getNBinHdr(self) -> int: 
        """
        get number of bin headers
        """
    def getNSamp(self) -> int: 
        """
        get number of samples (i.e. trace length)
        """
    def getNTextHdrRows(self) -> int: 
        """
        get number lines of text header
        """
    def getNTrc(self) -> int: 
        """
        get number of traces
        """
    def getNTrcHdr(self) -> int: 
        """
        get number of trace headers (usually 78)
        """
    def getPKeyIndexes(self, pName: str, pMin: float, pMax: float) -> numpy.ndarray[numpy.uint64, _Shape[m, 1]]: ...
    def getPKeyNames(self) -> typing.List[str]: 
        """
        get primary key names (usually they are used in sorting)
        """
    def getPKeySize(self, pName: str) -> int: ...
    def getPKeyValues(self, pkey: str, unitsFrom: str = str(), unitsTo: str = str()) -> numpy.ndarray[numpy.float64, _Shape[m, 1]]: ...
    def getSRD(self, units: str = str()) -> float: ...
    def getSampRate(self, units: str = str()) -> float: 
        """
        in units according to `Domain` (`METER` or `SECOND` or else...)
        """
    def getSamples(self, trcInd: int, units: str = str()) -> numpy.ndarray[numpy.float64, _Shape[m, 1]]: 
        """
        in units according to `Domain` (`METER` or `SECOND` or else...)
        """
    def getSortG(self) -> typing.Optional[h5gtpy._h5gt.Group]: ...
    def getSortedData(self, keyList: typing.List[str], minList: typing.List[float], maxList: typing.List[float], fromSampInd: int = 0, nSamp: int = sys.maxint, readTraceByTrace: bool = True, dataUnits: str = str()) -> typing.Tuple[numpy.ndarray[numpy.float32, _Shape[m, n]], numpy.ndarray[numpy.float64, _Shape[m, n]], numpy.ndarray[numpy.uint64, _Shape[m, 1]]]: 
        """
        Get sorted data based on precalculated primary sort keys (e.g. before using it you should prepare primary sort keys with `addPKeySort(...)` method).Return `TRACE` (traces matrix), `HDR` (hdr matrix) and `idx` (vector of trace indexes read)
        """
    def getSurveyType(self) -> SurveyType: ...
    def getTextHeader(self) -> typing.List[str]: ...
    def getTextHeaderD(self) -> typing.Optional[h5gtpy._h5gt.DataSet]: ...
    def getTrace(self, fromTrc: int, nTrc: int = 1, fromSampInd: int = 0, nSamp: int = sys.maxint, dataUnits: str = str()) -> numpy.ndarray[numpy.float32, _Shape[m, n]]: 
        """
        Get block of traces. If `nTrc` or `nSamp` exceed max values then these values are changed to max allowed (that is why they are not `const`)
        """
    def getTraceD(self) -> typing.Optional[h5gtpy._h5gt.DataSet]: ...
    @typing.overload
    def getTraceHeader(self, fromTrc: int, nTrc: int = 1, fromHdr: int = 0, nHdr: int = sys.maxint, unitsFrom: typing.List[str] = list(), unitsTo: typing.List[str] = list()) -> numpy.ndarray[numpy.float64, _Shape[m, n]]: 
        """
        Get block of trace headers. If `nTrc` or `nHdr` exceed max values then these values are changed to max allowed (that is why they are not `const`)
        """
    @typing.overload
    def getTraceHeader(self, hdrName: str, fromTrc: int = 0, nTrc: int = sys.maxint, unitsFrom: str = str(), unitsTo: str = str()) -> numpy.ndarray[numpy.float64, _Shape[m, 1]]: ...
    @typing.overload
    def getTraceHeader(self, hdrNames: typing.List[str], trcHdrInd: typing.List[int], unitsFrom: typing.List[str] = list(), unitsTo: typing.List[str] = list()) -> numpy.ndarray[numpy.float64, _Shape[m, n]]: ...
    @typing.overload
    def getTraceHeader(self, trcInd: typing.List[int], trcHdrInd: typing.List[int], unitsFrom: typing.List[str] = list(), unitsTo: typing.List[str] = list()) -> numpy.ndarray[numpy.float64, _Shape[m, n]]: ...
    def getTraceHeaderD(self) -> typing.Optional[h5gtpy._h5gt.DataSet]: ...
    def getTraceHeaderIndex(self, hdrName: str) -> int: ...
    @typing.overload
    def getTraceHeaderMax(self) -> typing.Dict[str, float]: ...
    @typing.overload
    def getTraceHeaderMax(self, hdrName: str, unitsFrom: str = str(), unitsTo: str = str()) -> float: ...
    @typing.overload
    def getTraceHeaderMin(self) -> typing.Dict[str, float]: ...
    @typing.overload
    def getTraceHeaderMin(self, hdrName: str, unitsFrom: str = str(), unitsTo: str = str()) -> float: ...
    def getUValG(self) -> typing.Optional[h5gtpy._h5gt.Group]: ...
    def hasPKeySort(self, pKeyName: str) -> bool: ...
    def openSeisContainer(self) -> H5SeisContainer: ...
    def removePKeySort(self, pKeyName: str) -> bool: ...
    def setDataType(self, arg0: SeisDataType) -> bool: ...
    def setDomain(self, arg0: Domain) -> bool: ...
    def setNSamp(self, arg0: int) -> bool: ...
    def setNTrc(self, arg0: int) -> bool: ...
    def setSRD(self, value: float, units: str = str()) -> bool: ...
    def setSampRate(self, value: float, units: str = str()) -> bool: ...
    def setSurveyType(self, arg0: SurveyType) -> bool: ...
    def updateBoundary(self) -> bool: ...
    def updatePKeySort(self, pKeyName: str) -> bool: ...
    def updateTraceHeaderLimits(self, nTrcBuffer: int = int(1e7)) -> bool: ...
    @typing.overload
    def writeBinHeader(self, binHdr: numpy.ndarray[numpy.float64, _Shape[m, 1]]) -> bool: ...
    @typing.overload
    def writeBinHeader(self, binHdr: typing.List[float]) -> bool: ...
    @typing.overload
    def writeBinHeader(self, hdrName: str, value: float, unitsFrom: str = str(), unitsTo: str = str()) -> bool: ...
    def writeBoundary(self, boundary: numpy.ndarray[numpy.float64, _Shape[m, 2]], lengthUnits: str = str(), doCoordTransform: bool = False) -> bool: 
        """
        write boundary of 2d (a line) or 3d (usually convex hull or concave hull) seismic survey. Input argument is `MatrixX2d` where first col - `X` coord, second - `Y` coord
        """
    def writeTextHeader(self, txtHdr: typing.List[str]) -> bool: ...
    def writeTrace(self, TRACE: numpy.ndarray[numpy.float32, _Shape[m, n]], fromTrc: int = 0, fromSampInd: int = 0) -> bool: ...
    @typing.overload
    def writeTraceHeader(self, HDR: numpy.ndarray[numpy.float64, _Shape[m, n]], fromTrc: int = 0, fromHdrInd: int = 0) -> bool: ...
    @typing.overload
    def writeTraceHeader(self, hdrName: str, hdr: numpy.ndarray[numpy.float64, _Shape[m, n]], fromTrc: int = 0, unitsFrom: str = str(), unitsTo: str = str()) -> bool: ...
    pass
class H5SeisContainer(H5BaseContainer, H5Base):
    @typing.overload
    def createSeis(self, arg0: h5gtpy._h5gt.Group, arg1: SeisParam, arg2: CreationType) -> H5Seis: ...
    @typing.overload
    def createSeis(self, arg0: str, arg1: SeisParam, arg2: CreationType) -> H5Seis: ...
    @typing.overload
    def openSeis(self, arg0: h5gtpy._h5gt.Group) -> H5Seis: ...
    @typing.overload
    def openSeis(self, arg0: str) -> H5Seis: ...
    pass
class H5Well(H5BaseObject, H5Base):
    @typing.overload
    def createDevCurve(self, arg0: h5gtpy._h5gt.Group, arg1: DevCurveParam, arg2: CreationType) -> H5DevCurve: ...
    @typing.overload
    def createDevCurve(self, arg0: str, arg1: DevCurveParam, arg2: CreationType) -> H5DevCurve: ...
    @typing.overload
    def createLogCurve(self, arg0: h5gtpy._h5gt.Group, arg1: LogCurveParam, arg2: CreationType) -> H5LogCurve: ...
    @typing.overload
    def createLogCurve(self, arg0: str, arg1: str, arg2: LogCurveParam, arg3: CreationType) -> H5LogCurve: ...
    def openActiveDevCurve(self) -> H5DevCurve: ...
    def getActiveDevG(self) -> typing.Optional[h5gtpy._h5gt.Group]: ...
    def getDevCurveGroupList(self) -> typing.List[h5gtpy._h5gt.Group]: ...
    def getDevCurveNameList(self) -> typing.List[str]: ...
    def getDevG(self) -> typing.Optional[h5gtpy._h5gt.Group]: ...
    def getHeadCoord(self, lengthUnits: str = str(), doCoordTransform: bool = False) -> numpy.ndarray[numpy.float64, _Shape[m, 1]]: ...
    def getKB(self, lengthUnits: str = str()) -> float: ...
    def getLogCurveGroupList(self) -> typing.List[h5gtpy._h5gt.Group]: ...
    def getLogCurveNameList(self) -> typing.List[str]: ...
    def getLogG(self) -> typing.Optional[h5gtpy._h5gt.Group]: ...
    def getLogTypeG(self, arg0: str) -> typing.Optional[h5gtpy._h5gt.Group]: ...
    def getLogTypeList(self) -> typing.List[str]: ...
    def getUWI(self) -> str: ...
    @typing.overload
    def openDevCurve(self, arg0: h5gtpy._h5gt.Group) -> H5DevCurve: ...
    @typing.overload
    def openDevCurve(self, arg0: str) -> H5DevCurve: ...
    @typing.overload
    def openLogCurve(self, arg0: h5gtpy._h5gt.Group) -> H5LogCurve: ...
    @typing.overload
    def openLogCurve(self, arg0: str, arg1: str) -> H5LogCurve: ...
    def openWellContainer(self) -> H5WellContainer: ...
    def setHeadCoord(self, xy: numpy.ndarray[numpy.float64, _Shape[2, 1]], lengthUnits: str = str(), doCoordTransform: bool = False) -> bool: ...
    def setKB(self, val: float, lengthUnits: str = str()) -> bool: ...
    def setUWI(self, arg0: str) -> bool: ...
    pass
class H5WellContainer(H5BaseContainer, H5Base):
    @typing.overload
    def createWell(self, arg0: h5gtpy._h5gt.Group, arg1: WellParam, arg2: CreationType) -> H5Well: ...
    @typing.overload
    def createWell(self, arg0: str, arg1: WellParam, arg2: CreationType) -> H5Well: ...
    @typing.overload
    def openWell(self, arg0: h5gtpy._h5gt.Group) -> H5Well: ...
    @typing.overload
    def openWell(self, arg0: str) -> H5Well: ...
    def openWellByUWI(self, arg0: str) -> H5Well: ...
    pass
class LogCurveParam(BaseObjectParam):
    def __init__(self) -> None: ...
    @property
    def chunkSize(self) -> int:
        """
        :type: int
        """
    @chunkSize.setter
    def chunkSize(self, arg0: int) -> None:
        pass
    pass
class LogDataType():
    """
    Members:

      MD

      VAL
    """
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    MD: h5geopy._h5geo.LogDataType # value = <LogDataType.MD: 1>
    VAL: h5geopy._h5geo.LogDataType # value = <LogDataType.VAL: 2>
    __members__: dict # value = {'MD': <LogDataType.MD: 1>, 'VAL': <LogDataType.VAL: 2>}
    pass
class MapParam(BaseObjectParam):
    def __init__(self) -> None: ...
    @property
    def X0(self) -> float:
        """
        :type: float
        """
    @X0.setter
    def X0(self, arg0: float) -> None:
        pass
    @property
    def X1(self) -> float:
        """
        :type: float
        """
    @X1.setter
    def X1(self, arg0: float) -> None:
        pass
    @property
    def X2(self) -> float:
        """
        :type: float
        """
    @X2.setter
    def X2(self, arg0: float) -> None:
        pass
    @property
    def Y0(self) -> float:
        """
        :type: float
        """
    @Y0.setter
    def Y0(self, arg0: float) -> None:
        pass
    @property
    def Y1(self) -> float:
        """
        :type: float
        """
    @Y1.setter
    def Y1(self, arg0: float) -> None:
        pass
    @property
    def Y2(self) -> float:
        """
        :type: float
        """
    @Y2.setter
    def Y2(self, arg0: float) -> None:
        pass
    @property
    def domain(self) -> Domain:
        """
        :type: Domain
        """
    @domain.setter
    def domain(self, arg0: Domain) -> None:
        pass
    @property
    def nX(self) -> int:
        """
        :type: int
        """
    @nX.setter
    def nX(self, arg0: int) -> None:
        pass
    @property
    def nY(self) -> int:
        """
        :type: int
        """
    @nY.setter
    def nY(self, arg0: int) -> None:
        pass
    @property
    def xChunkSize(self) -> int:
        """
        :type: int
        """
    @xChunkSize.setter
    def xChunkSize(self, arg0: int) -> None:
        pass
    @property
    def yChunkSize(self) -> int:
        """
        :type: int
        """
    @yChunkSize.setter
    def yChunkSize(self, arg0: int) -> None:
        pass
    pass
class ObjectDeleter():
    def __call__(self, arg0: H5Base) -> None: ...
    pass
class ObjectType():
    """
    Members:

      MAP

      WELL

      LOGCURVE

      DEVCURVE

      SEISMIC
    """
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    DEVCURVE: h5geopy._h5geo.ObjectType # value = <ObjectType.DEVCURVE: 4>
    LOGCURVE: h5geopy._h5geo.ObjectType # value = <ObjectType.LOGCURVE: 3>
    MAP: h5geopy._h5geo.ObjectType # value = <ObjectType.MAP: 1>
    SEISMIC: h5geopy._h5geo.ObjectType # value = <ObjectType.SEISMIC: 5>
    WELL: h5geopy._h5geo.ObjectType # value = <ObjectType.WELL: 2>
    __members__: dict # value = {'MAP': <ObjectType.MAP: 1>, 'WELL': <ObjectType.WELL: 2>, 'LOGCURVE': <ObjectType.LOGCURVE: 3>, 'DEVCURVE': <ObjectType.DEVCURVE: 4>, 'SEISMIC': <ObjectType.SEISMIC: 5>}
    pass
class Point3():
    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, arg0: float, arg1: float, arg2: float) -> None: ...
    def getName(self) -> str: ...
    def setName(self, arg0: str) -> None: ...
    def setX(self, arg0: float) -> None: ...
    def setY(self, arg0: float) -> None: ...
    def setZ(self, arg0: float) -> None: ...
    def x(self) -> float: ...
    def y(self) -> float: ...
    def z(self) -> float: ...
    pass
class Point3Array():
    def __bool__(self) -> bool: 
        """
        Check whether the list is nonempty
        """
    @typing.overload
    def __delitem__(self, arg0: int) -> None: 
        """
        Delete the list elements at index ``i``

        Delete list elements using a slice object
        """
    @typing.overload
    def __delitem__(self, arg0: slice) -> None: ...
    @typing.overload
    def __getitem__(self, arg0: int) -> Point3: 
        """
        Retrieve list elements using a slice object
        """
    @typing.overload
    def __getitem__(self, s: slice) -> Point3Array: ...
    @typing.overload
    def __init__(self) -> None: 
        """
        Copy constructor
        """
    @typing.overload
    def __init__(self, arg0: Point3Array) -> None: ...
    @typing.overload
    def __init__(self, arg0: typing.Iterable) -> None: ...
    def __iter__(self) -> typing.Iterator: ...
    def __len__(self) -> int: ...
    @typing.overload
    def __setitem__(self, arg0: int, arg1: Point3) -> None: 
        """
        Assign list elements using a slice object
        """
    @typing.overload
    def __setitem__(self, arg0: slice, arg1: Point3Array) -> None: ...
    def append(self, x: Point3) -> None: 
        """
        Add an item to the end of the list
        """
    def clear(self) -> None: 
        """
        Clear the contents
        """
    @typing.overload
    def extend(self, L: Point3Array) -> None: 
        """
        Extend the list by appending all the items in the given list

        Extend the list by appending all the items in the given list
        """
    @typing.overload
    def extend(self, L: typing.Iterable) -> None: ...
    def insert(self, i: int, x: Point3) -> None: 
        """
        Insert an item at a given position.
        """
    @typing.overload
    def pop(self) -> Point3: 
        """
        Remove and return the last item

        Remove and return the item at index ``i``
        """
    @typing.overload
    def pop(self, i: int) -> Point3: ...
    pass
class PointsParam(BaseObjectParam):
    def __init__(self) -> None: ...
    @property
    def chunkSize(self) -> int:
        """
        :type: int
        """
    @chunkSize.setter
    def chunkSize(self, arg0: int) -> None:
        pass
    @property
    def domain(self) -> Domain:
        """
        :type: Domain
        """
    @domain.setter
    def domain(self, arg0: Domain) -> None:
        pass
    @property
    def nPoints(self) -> int:
        """
        :type: int
        """
    @nPoints.setter
    def nPoints(self, arg0: int) -> None:
        pass
    pass
class Endian():
    """
    Members:

      Little

      Big
    """
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    Big: h5geopy._h5geo.Endian # value = <Endian.Big: 1>
    Little: h5geopy._h5geo.Endian # value = <Endian.Little: 0>
    __members__: dict # value = {'Little': <Endian.Little: 0>, 'Big': <Endian.Big: 1>}
    pass
class SegyFormat():
    """
    Members:

      FourByte_IBM

      FourByte_IEEE

      FourByte_integer
    """
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    FourByte_IBM: h5geopy._h5geo.SegyFormat # value = <SegyFormat.FourByte_IBM: 0>
    FourByte_IEEE: h5geopy._h5geo.SegyFormat # value = <SegyFormat.FourByte_IEEE: 1>
    FourByte_integer: h5geopy._h5geo.SegyFormat # value = <SegyFormat.FourByte_integer: 2>
    __members__: dict # value = {'FourByte_IBM': <SegyFormat.FourByte_IBM: 0>, 'FourByte_IEEE': <SegyFormat.FourByte_IEEE: 1>, 'FourByte_integer': <SegyFormat.FourByte_integer: 2>}
    pass
class SeisDataType():
    """
    Members:

      STACK

      PRESTACK
    """
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    PRESTACK: h5geopy._h5geo.SeisDataType # value = <SeisDataType.PRESTACK: 2>
    STACK: h5geopy._h5geo.SeisDataType # value = <SeisDataType.STACK: 1>
    __members__: dict # value = {'STACK': <SeisDataType.STACK: 1>, 'PRESTACK': <SeisDataType.PRESTACK: 2>}
    pass
class SeisParam(BaseObjectParam):
    def __init__(self) -> None: ...
    @property
    def dataType(self) -> SeisDataType:
        """
        :type: SeisDataType
        """
    @dataType.setter
    def dataType(self, arg0: SeisDataType) -> None:
        pass
    @property
    def domain(self) -> Domain:
        """
        :type: Domain
        """
    @domain.setter
    def domain(self, arg0: Domain) -> None:
        pass
    @property
    def nSamp(self) -> int:
        """
        :type: int
        """
    @nSamp.setter
    def nSamp(self, arg0: int) -> None:
        pass
    @property
    def nTrc(self) -> int:
        """
        :type: int
        """
    @nTrc.setter
    def nTrc(self, arg0: int) -> None:
        pass
    @property
    def srd(self) -> float:
        """
        :type: float
        """
    @srd.setter
    def srd(self, arg0: float) -> None:
        pass
    @property
    def stdChunk(self) -> int:
        """
        :type: int
        """
    @stdChunk.setter
    def stdChunk(self, arg0: int) -> None:
        pass
    @property
    def surveyType(self) -> SurveyType:
        """
        :type: SurveyType
        """
    @surveyType.setter
    def surveyType(self, arg0: SurveyType) -> None:
        pass
    @property
    def trcChunk(self) -> int:
        """
        :type: int
        """
    @trcChunk.setter
    def trcChunk(self, arg0: int) -> None:
        pass
    pass
class SurveyType():
    """
    Members:

      TWO_D

      THREE_D
    """
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    THREE_D: h5geopy._h5geo.SurveyType # value = <SurveyType.THREE_D: 2>
    TWO_D: h5geopy._h5geo.SurveyType # value = <SurveyType.TWO_D: 1>
    __members__: dict # value = {'TWO_D': <SurveyType.TWO_D: 1>, 'THREE_D': <SurveyType.THREE_D: 2>}
    pass
class TrajectoryFormat():
    """
    Members:

      MD_AZIM_INCL

      TVD_X_Y

      TVD_DX_DY

      TVDSS_X_Y

      TVDSS_DX_DY
    """
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    MD_AZIM_INCL: h5geopy._h5geo.TrajectoryFormat # value = <TrajectoryFormat.MD_AZIM_INCL: 1>
    TVDSS_DX_DY: h5geopy._h5geo.TrajectoryFormat # value = <TrajectoryFormat.TVDSS_DX_DY: 5>
    TVDSS_X_Y: h5geopy._h5geo.TrajectoryFormat # value = <TrajectoryFormat.TVDSS_X_Y: 4>
    TVD_DX_DY: h5geopy._h5geo.TrajectoryFormat # value = <TrajectoryFormat.TVD_DX_DY: 3>
    TVD_X_Y: h5geopy._h5geo.TrajectoryFormat # value = <TrajectoryFormat.TVD_X_Y: 2>
    __members__: dict # value = {'MD_AZIM_INCL': <TrajectoryFormat.MD_AZIM_INCL: 1>, 'TVD_X_Y': <TrajectoryFormat.TVD_X_Y: 2>, 'TVD_DX_DY': <TrajectoryFormat.TVD_DX_DY: 3>, 'TVDSS_X_Y': <TrajectoryFormat.TVDSS_X_Y: 4>, 'TVDSS_DX_DY': <TrajectoryFormat.TVDSS_DX_DY: 5>}
    pass
class TextEncoding():
    """
    Members:

      ASCII

      EBCDIC
    """
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    ASCII: h5geopy._h5geo.TextEncoding # value = <TextEncoding.ASCII: 0>
    EBCDIC: h5geopy._h5geo.TextEncoding # value = <TextEncoding.EBCDIC: 1>
    __members__: dict # value = {'ASCII': <TextEncoding.ASCII: 0>, 'EBCDIC': <TextEncoding.EBCDIC: 1>}
    pass
class WellDataType():
    """
    Members:

      WELL

      DEV

      LOG

      INTERVAL_VEL

      AVG_VEL

      RMS_VEL

      FWAL

      CHECKSHOTS

      WELL_TIE
    """
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    AVG_VEL: h5geopy._h5geo.WellDataType # value = <WellDataType.AVG_VEL: 5>
    CHECKSHOTS: h5geopy._h5geo.WellDataType # value = <WellDataType.CHECKSHOTS: 8>
    DEV: h5geopy._h5geo.WellDataType # value = <WellDataType.DEV: 2>
    FWAL: h5geopy._h5geo.WellDataType # value = <WellDataType.FWAL: 7>
    INTERVAL_VEL: h5geopy._h5geo.WellDataType # value = <WellDataType.INTERVAL_VEL: 4>
    LOG: h5geopy._h5geo.WellDataType # value = <WellDataType.LOG: 3>
    RMS_VEL: h5geopy._h5geo.WellDataType # value = <WellDataType.RMS_VEL: 6>
    WELL: h5geopy._h5geo.WellDataType # value = <WellDataType.WELL: 1>
    WELL_TIE: h5geopy._h5geo.WellDataType # value = <WellDataType.WELL_TIE: 9>
    __members__: dict # value = {'WELL': <WellDataType.WELL: 1>, 'DEV': <WellDataType.DEV: 2>, 'LOG': <WellDataType.LOG: 3>, 'INTERVAL_VEL': <WellDataType.INTERVAL_VEL: 4>, 'AVG_VEL': <WellDataType.AVG_VEL: 5>, 'RMS_VEL': <WellDataType.RMS_VEL: 6>, 'FWAL': <WellDataType.FWAL: 7>, 'CHECKSHOTS': <WellDataType.CHECKSHOTS: 8>, 'WELL_TIE': <WellDataType.WELL_TIE: 9>}
    pass
class WellName():
    """
    Members:

      FROM_LAS

      FROM_FILE_NAME
    """
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    FROM_FILE_NAME: h5geopy._h5geo.WellName # value = <WellName.FROM_FILE_NAME: 1>
    FROM_LAS: h5geopy._h5geo.WellName # value = <WellName.FROM_LAS: 0>
    __members__: dict # value = {'FROM_LAS': <WellName.FROM_LAS: 0>, 'FROM_FILE_NAME': <WellName.FROM_FILE_NAME: 1>}
    pass
class WellParam(BaseObjectParam):
    def __init__(self) -> None: ...
    @property
    def headX(self) -> float:
        """
        :type: float
        """
    @headX.setter
    def headX(self, arg0: float) -> None:
        pass
    @property
    def headY(self) -> float:
        """
        :type: float
        """
    @headY.setter
    def headY(self, arg0: float) -> None:
        pass
    @property
    def kb(self) -> float:
        """
        :type: float
        """
    @kb.setter
    def kb(self, arg0: float) -> None:
        pass
    @property
    def uwi(self) -> str:
        """
        :type: str
        """
    @uwi.setter
    def uwi(self, arg0: str) -> None:
        pass
    pass
class WellType():
    """
    Members:

      PROPOSED

      DRY

      OIL

      GAS

      WATER

      CONDENSATE
    """
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    CONDENSATE: h5geopy._h5geo.WellType # value = <WellType.CONDENSATE: 32>
    DRY: h5geopy._h5geo.WellType # value = <WellType.DRY: 2>
    GAS: h5geopy._h5geo.WellType # value = <WellType.GAS: 8>
    OIL: h5geopy._h5geo.WellType # value = <WellType.OIL: 4>
    PROPOSED: h5geopy._h5geo.WellType # value = <WellType.PROPOSED: 1>
    WATER: h5geopy._h5geo.WellType # value = <WellType.WATER: 16>
    __members__: dict # value = {'PROPOSED': <WellType.PROPOSED: 1>, 'DRY': <WellType.DRY: 2>, 'OIL': <WellType.OIL: 4>, 'GAS': <WellType.GAS: 8>, 'WATER': <WellType.WATER: 16>, 'CONDENSATE': <WellType.CONDENSATE: 32>}
    pass
@typing.overload
def MdAzIncl2ALL(M: numpy.ndarray[numpy.float32, _Shape[m, n]], x0: float, y0: float, kb: float, angularUnitsFrom: str, angularUnitsTo: str, XNorth: bool) -> numpy.ndarray[numpy.float32, _Shape[m, n]]:
    """
    Convert matrix `M` (whose columns represent MD Az Incl respectively)to `MD_X_Y_Z_TVD_DX_DY_AZ_INCL` (Z the same as TVDSS). Set `XNorth` to `True` if `X` axis points to the North
    """
@typing.overload
def MdAzIncl2ALL(M: numpy.ndarray[numpy.float64, _Shape[m, n]], x0: float, y0: float, kb: float, angularUnitsFrom: str, angularUnitsTo: str, XNorth: bool) -> numpy.ndarray[numpy.float64, _Shape[m, n]]:
    pass
@typing.overload
def MdAzIncl2MdXYTvd(M: numpy.ndarray[numpy.float32, _Shape[m, n]], x0: float, y0: float, angularUnits: str, XNorth: bool) -> numpy.ndarray[numpy.float32, _Shape[m, n]]:
    """
    Convert matrix `M` (whose columns represent MD Az Incl respectively)to `MD_X_Y_TVD`. Set `XNorth` to `True` if `X` axis points to the North
    """
@typing.overload
def MdAzIncl2MdXYTvd(M: numpy.ndarray[numpy.float64, _Shape[m, n]], x0: float, y0: float, angularUnits: str, XNorth: bool) -> numpy.ndarray[numpy.float64, _Shape[m, n]]:
    pass
@typing.overload
def TvdDxDy2ALL(M: numpy.ndarray[numpy.float32, _Shape[m, n]], x0: float, y0: float, kb: float, angularUnits: str, XNorth: bool) -> numpy.ndarray[numpy.float32, _Shape[m, n]]:
    """
    Convert matrix `M` (whose columns represent TVD DX DY respectively)to `MD_X_Y_Z_TVD_DX_DY_AZ_INCL` (Z the same as TVDSS). Set `XNorth` to `True` if `X` axis points to the North
    """
@typing.overload
def TvdDxDy2ALL(M: numpy.ndarray[numpy.float64, _Shape[m, n]], x0: float, y0: float, kb: float, angularUnits: str, XNorth: bool) -> numpy.ndarray[numpy.float64, _Shape[m, n]]:
    pass
@typing.overload
def TvdDxDy2MdAzIncl(M: numpy.ndarray[numpy.float32, _Shape[m, n]], angularUnits: str, XNorth: bool) -> numpy.ndarray[numpy.float32, _Shape[m, n]]:
    """
    Convert matrix `M` (whose columns represent TVD DX DY respectively)to `MD_Az_Incl`. Set `XNorth` to `True` if `X` axis points to the North
    """
@typing.overload
def TvdDxDy2MdAzIncl(M: numpy.ndarray[numpy.float64, _Shape[m, n]], angularUnits: str, XNorth: bool) -> numpy.ndarray[numpy.float64, _Shape[m, n]]:
    pass
@typing.overload
def TvdDxDy2MdXYTvd(M: numpy.ndarray[numpy.float32, _Shape[m, n]], x0: float, y0: float, XNorth: bool) -> numpy.ndarray[numpy.float32, _Shape[m, n]]:
    """
    Convert matrix `M` (whose columns represent TVD DX DY respectively)to `MD_X_Y_TVD`. Set `XNorth` to `True` if `X` axis points to the North
    """
@typing.overload
def TvdDxDy2MdXYTvd(M: numpy.ndarray[numpy.float64, _Shape[m, n]], x0: float, y0: float, XNorth: bool) -> numpy.ndarray[numpy.float64, _Shape[m, n]]:
    pass
@typing.overload
def TvdXY2ALL(M: numpy.ndarray[numpy.float32, _Shape[m, n]], x0: float, y0: float, kb: float, angularUnits: str, XNorth: bool) -> numpy.ndarray[numpy.float32, _Shape[m, n]]:
    """
    Convert matrix `M` (whose columns represent TVD X Y respectively)to `MD_X_Y_Z_TVD_DX_DY_AZ_INCL` (Z the same as TVDSS). Set `XNorth` to `True` if `X` axis points to the North
    """
@typing.overload
def TvdXY2ALL(M: numpy.ndarray[numpy.float64, _Shape[m, n]], x0: float, y0: float, kb: float, angularUnits: str, XNorth: bool) -> numpy.ndarray[numpy.float64, _Shape[m, n]]:
    pass
@typing.overload
def TvdXY2MdAzIncl(M: numpy.ndarray[numpy.float32, _Shape[m, n]], x0: float, y0: float, angularUnits: str, XNorth: bool) -> numpy.ndarray[numpy.float32, _Shape[m, n]]:
    """
    Convert matrix `M` (whose columns represent TVD X Y respectively)to `MD_Az_Incl`. Set `XNorth` to `True` if `X` axis points to the North
    """
@typing.overload
def TvdXY2MdAzIncl(M: numpy.ndarray[numpy.float64, _Shape[m, n]], x0: float, y0: float, angularUnits: str, XNorth: bool) -> numpy.ndarray[numpy.float64, _Shape[m, n]]:
    pass
@typing.overload
def TvdXY2MdXYTvd(M: numpy.ndarray[numpy.float32, _Shape[m, n]], x0: float, y0: float, XNorth: bool) -> numpy.ndarray[numpy.float32, _Shape[m, n]]:
    """
    Convert matrix `M` (whose columns represent TVD X Y respectively)to `MD_X_Y_TVD`. Set `XNorth` to `True` if `X` axis points to the North
    """
@typing.overload
def TvdXY2MdXYTvd(M: numpy.ndarray[numpy.float64, _Shape[m, n]], x0: float, y0: float, XNorth: bool) -> numpy.ndarray[numpy.float64, _Shape[m, n]]:
    pass
@typing.overload
def TvdssDxDy2ALL(M: numpy.ndarray[numpy.float32, _Shape[m, n]], x0: float, y0: float, kb: float, angularUnits: str, XNorth: bool) -> numpy.ndarray[numpy.float32, _Shape[m, n]]:
    """
    Convert matrix `M` (whose columns represent TVDSS DX DY respectively)to `MD_X_Y_Z_TVD_DX_DY_AZ_INCL` (Z the same as TVDSS). Set `XNorth` to `True` if `X` axis points to the North
    """
@typing.overload
def TvdssDxDy2ALL(M: numpy.ndarray[numpy.float64, _Shape[m, n]], x0: float, y0: float, kb: float, angularUnits: str, XNorth: bool) -> numpy.ndarray[numpy.float64, _Shape[m, n]]:
    pass
@typing.overload
def TvdssDxDy2MdAzIncl(M: numpy.ndarray[numpy.float32, _Shape[m, n]], kb: float, angularUnits: str, XNorth: bool) -> numpy.ndarray[numpy.float32, _Shape[m, n]]:
    """
    Convert matrix `M` (whose columns represent TVDSS DX DY respectively)to `MD_Az_Incl`. Set `XNorth` to `True` if `X` axis points to the North
    """
@typing.overload
def TvdssDxDy2MdAzIncl(M: numpy.ndarray[numpy.float64, _Shape[m, n]], kb: float, angularUnits: str, XNorth: bool) -> numpy.ndarray[numpy.float64, _Shape[m, n]]:
    pass
@typing.overload
def TvdssDxDy2MdXYTvd(M: numpy.ndarray[numpy.float32, _Shape[m, n]], x0: float, y0: float, kb: float, XNorth: bool) -> numpy.ndarray[numpy.float32, _Shape[m, n]]:
    """
    Convert matrix `M` (whose columns represent TVDSS DX DY respectively)to `MD_X_Y_TVD`. Set `XNorth` to `True` if `X` axis points to the North
    """
@typing.overload
def TvdssDxDy2MdXYTvd(M: numpy.ndarray[numpy.float64, _Shape[m, n]], x0: float, y0: float, kb: float, XNorth: bool) -> numpy.ndarray[numpy.float64, _Shape[m, n]]:
    pass
@typing.overload
def TvdssXY2ALL(M: numpy.ndarray[numpy.float32, _Shape[m, n]], x0: float, y0: float, kb: float, angularUnits: str, XNorth: bool) -> numpy.ndarray[numpy.float32, _Shape[m, n]]:
    """
    Convert matrix `M` (whose columns represent TVDSS X Y respectively)to `MD_X_Y_Z_TVD_DX_DY_AZ_INCL` (Z the same as TVDSS). Set `XNorth` to `True` if `X` axis points to the North
    """
@typing.overload
def TvdssXY2ALL(M: numpy.ndarray[numpy.float64, _Shape[m, n]], x0: float, y0: float, kb: float, angularUnits: str, XNorth: bool) -> numpy.ndarray[numpy.float64, _Shape[m, n]]:
    pass
@typing.overload
def TvdssXY2MdAzIncl(M: numpy.ndarray[numpy.float32, _Shape[m, n]], x0: float, y0: float, kb: float, angularUnits: str, XNorth: bool) -> numpy.ndarray[numpy.float32, _Shape[m, n]]:
    """
    Convert matrix `M` (whose columns represent TVDSS X Y respectively)to `MD_Az_Incl`. Set `XNorth` to `True` if `X` axis points to the North
    """
@typing.overload
def TvdssXY2MdAzIncl(M: numpy.ndarray[numpy.float64, _Shape[m, n]], x0: float, y0: float, kb: float, angularUnits: str, XNorth: bool) -> numpy.ndarray[numpy.float64, _Shape[m, n]]:
    pass
@typing.overload
def TvdssXY2MdXYTvd(M: numpy.ndarray[numpy.float32, _Shape[m, n]], x0: float, y0: float, kb: float, XNorth: bool) -> numpy.ndarray[numpy.float32, _Shape[m, n]]:
    """
    Convert matrix `M` (whose columns represent TVDSS X Y respectively)to `MD_X_Y_TVD`. Set `XNorth` to `True` if `X` axis points to the North
    """
@typing.overload
def TvdssXY2MdXYTvd(M: numpy.ndarray[numpy.float64, _Shape[m, n]], x0: float, y0: float, kb: float, XNorth: bool) -> numpy.ndarray[numpy.float64, _Shape[m, n]]:
    pass
def createMapContainer(arg0: h5gtpy._h5gt.File, arg1: CreationType) -> H5MapContainer:
    pass
def createMapContainerByName(arg0: str, arg1: CreationType) -> H5MapContainer:
    pass
def createSeisContainer(arg0: h5gtpy._h5gt.File, arg1: CreationType) -> H5SeisContainer:
    pass
def createSeisContainerByName(arg0: str, arg1: CreationType) -> H5SeisContainer:
    pass
def createWellContainer(arg0: h5gtpy._h5gt.File, arg1: CreationType) -> H5WellContainer:
    pass
def createWellContainerByName(arg0: str, arg1: CreationType) -> H5WellContainer:
    pass
def getBinHeaderBytes() -> typing.Tuple[typing.List[str], typing.List[str]]:
    pass
def getBinHeaderCount(arg0: typing.List[str], arg1: typing.List[str]) -> None:
    pass
def getBinHeaderNames() -> typing.Tuple[typing.List[str], typing.List[str]]:
    pass
def getTraceHeaderBytes() -> typing.Tuple[typing.List[str], typing.List[str]]:
    pass
def getTraceHeaderCount(arg0: typing.List[str], arg1: typing.List[str]) -> None:
    pass
def getTraceHeaderNames() -> typing.Tuple[typing.List[str], typing.List[str]]:
    pass
@typing.overload
def interp1Monotonic(x: numpy.ndarray[numpy.float32, _Shape[m, 1]], y: numpy.ndarray[numpy.float32, _Shape[m, 1]], xnew: numpy.ndarray[numpy.float32, _Shape[m, 1]], extrapVal: float) -> numpy.ndarray[numpy.float32, _Shape[m, 1]]:
    """
    assume that x is strictly monotonic (either increasing or decreasing)

    assume that x is strictly monotonic (either increasing or decreasing)
    """
@typing.overload
def interp1Monotonic(x: numpy.ndarray[numpy.float64, _Shape[m, 1]], y: numpy.ndarray[numpy.float64, _Shape[m, 1]], xnew: numpy.ndarray[numpy.float64, _Shape[m, 1]], extrapVal: float) -> numpy.ndarray[numpy.float64, _Shape[m, 1]]:
    pass
def isDevCurve(arg0: h5gtpy._h5gt.Group) -> bool:
    pass
def isGeoContainer(arg0: h5gtpy._h5gt.File) -> bool:
    pass
def isGeoContainerByType(arg0: h5gtpy._h5gt.File, arg1: ContainerType) -> bool:
    pass
def isGeoObject(arg0: h5gtpy._h5gt.Group) -> bool:
    pass
def isGeoObjectByType(arg0: h5gtpy._h5gt.Group, arg1: ObjectType) -> bool:
    pass
def isLogCurve(arg0: h5gtpy._h5gt.Group) -> bool:
    pass
def isMap(arg0: h5gtpy._h5gt.Group) -> bool:
    pass
def isSeis(arg0: h5gtpy._h5gt.Group) -> bool:
    pass
def isWell(arg0: h5gtpy._h5gt.Group) -> bool:
    pass
def openBaseContainer(arg0: h5gtpy._h5gt.File) -> H5BaseContainer:
    pass
def openBaseContainerByName(arg0: str) -> H5BaseContainer:
    pass
def openBaseObject(arg0: h5gtpy._h5gt.Group) -> H5BaseObject:
    pass
def openContainer(arg0: h5gtpy._h5gt.File) -> H5BaseContainer:
    pass
def openContainerByName(arg0: str) -> H5BaseContainer:
    pass
def openDevCurve(arg0: h5gtpy._h5gt.Group) -> H5DevCurve:
    pass
def openLogCurve(arg0: h5gtpy._h5gt.Group) -> H5LogCurve:
    pass
def openMap(arg0: h5gtpy._h5gt.Group) -> H5Map:
    pass
def openMapContainer(arg0: h5gtpy._h5gt.File) -> H5MapContainer:
    pass
def openMapContainerByName(arg0: str) -> H5MapContainer:
    pass
def openObject(arg0: h5gtpy._h5gt.Group) -> H5BaseObject:
    pass
def openSeis(arg0: h5gtpy._h5gt.Group) -> H5Seis:
    pass
def openSeisContainer(arg0: h5gtpy._h5gt.File) -> H5SeisContainer:
    pass
def openSeisContainerByName(arg0: str) -> H5SeisContainer:
    pass
def openWell(arg0: h5gtpy._h5gt.Group) -> H5Well:
    pass
def openWellContainer(arg0: h5gtpy._h5gt.File) -> H5WellContainer:
    pass
def openWellContainerByName(arg0: str) -> H5WellContainer:
    pass
@typing.overload
def quickHull2D(M: numpy.ndarray[numpy.float32, _Shape[m, 2]]) -> numpy.ndarray[numpy.float32, _Shape[m, 2]]:
    """
    Quick Hull 2D algorithm https://en.wikipedia.org/wiki/quickHull. `M` is a two column matrix: first col - X, second - Y. Return two column matrix (XY)
    """
@typing.overload
def quickHull2D(M: numpy.ndarray[numpy.float64, _Shape[m, 2]]) -> numpy.ndarray[numpy.float64, _Shape[m, 2]]:
    pass
@typing.overload
def sort(v: numpy.ndarray[numpy.float32, _Shape[m, 1]]) -> typing.Tuple[numpy.ndarray[numpy.int64, _Shape[m, 1]], numpy.ndarray[numpy.float32, _Shape[m, 1]]]:
    """
    return indices such that `v_sorted = v(ind)`. Input parameter `v` is a vector

    return indices such that `v_sorted = v(ind)` and also return `v_sorted`. Input parameter `v` is a vector
    """
@typing.overload
def sort(v: numpy.ndarray[numpy.float32, _Shape[m, 1]]) -> numpy.ndarray[numpy.int64, _Shape[m, 1]]:
    pass
@typing.overload
def sort(v: numpy.ndarray[numpy.float64, _Shape[m, 1]]) -> typing.Tuple[numpy.ndarray[numpy.int64, _Shape[m, 1]], numpy.ndarray[numpy.float64, _Shape[m, 1]]]:
    pass
@typing.overload
def sort(v: numpy.ndarray[numpy.float64, _Shape[m, 1]]) -> numpy.ndarray[numpy.int64, _Shape[m, 1]]:
    pass
@typing.overload
def sort_rows(M: numpy.ndarray[numpy.float32, _Shape[m, n]]) -> numpy.ndarray[numpy.int64, _Shape[m, 1]]:
    """
    sorts the rows of a matrix in ascending order based on the elements in the first column. When the first column contains repeated elements, sortrows sorts according to the values in the next column and repeats this behavior for succeeding equal values. M_sorted = M(ind, Eigen::all)

    sorts the rows of a matrix in ascending order based on the elements in the first column. When the first column contains repeated elements, sortrows sorts according to the values in the next column and repeats this behavior for succeeding equal values. also calculates M_sorted = M(ind, Eigen::all)
    """
@typing.overload
def sort_rows(M: numpy.ndarray[numpy.float32, _Shape[m, n]]) -> typing.Tuple[numpy.ndarray[numpy.int64, _Shape[m, 1]], numpy.ndarray[numpy.float32, _Shape[m, n]]]:
    pass
@typing.overload
def sort_rows(M: numpy.ndarray[numpy.float64, _Shape[m, n]]) -> numpy.ndarray[numpy.int64, _Shape[m, 1]]:
    pass
@typing.overload
def sort_rows(M: numpy.ndarray[numpy.float64, _Shape[m, n]]) -> typing.Tuple[numpy.ndarray[numpy.int64, _Shape[m, 1]], numpy.ndarray[numpy.float64, _Shape[m, n]]]:
    pass
@typing.overload
def sort_rows_unique(M: numpy.ndarray[numpy.float32, _Shape[m, n]]) -> typing.Tuple[numpy.ndarray[numpy.int64, _Shape[m, 1]], numpy.ndarray[numpy.float32, _Shape[m, n]], numpy.ndarray[numpy.int64, _Shape[m, 2]]]:
    """
    find unique rows, sort them, identify unique rows start and end row-indices and return row-indices such that M_sorted = M(ind, Eigen::all).Return indices `ind`, `urows` and `urows_from_size`

    find unique rows, sort them, identify unique rows start and end row-indices and return row-indices such that M_sorted = M(ind, Eigen::all).Return indices `ind`, `urows` and `urows_from_size`. Also return `M_sorted`
    """
@typing.overload
def sort_rows_unique(M: numpy.ndarray[numpy.float32, _Shape[m, n]]) -> typing.Tuple[numpy.ndarray[numpy.int64, _Shape[m, 1]], numpy.ndarray[numpy.float32, _Shape[m, n]], numpy.ndarray[numpy.int64, _Shape[m, 2]], numpy.ndarray[numpy.float32, _Shape[m, n]]]:
    pass
@typing.overload
def sort_rows_unique(M: numpy.ndarray[numpy.float64, _Shape[m, n]]) -> typing.Tuple[numpy.ndarray[numpy.int64, _Shape[m, 1]], numpy.ndarray[numpy.float64, _Shape[m, n]], numpy.ndarray[numpy.int64, _Shape[m, 2]]]:
    pass
@typing.overload
def sort_rows_unique(M: numpy.ndarray[numpy.float64, _Shape[m, n]]) -> typing.Tuple[numpy.ndarray[numpy.int64, _Shape[m, 1]], numpy.ndarray[numpy.float64, _Shape[m, n]], numpy.ndarray[numpy.int64, _Shape[m, 2]], numpy.ndarray[numpy.float64, _Shape[m, n]]]:
    pass
@typing.overload
def sort_unique(v: numpy.ndarray[numpy.float32, _Shape[m, 1]]) -> typing.Tuple[numpy.ndarray[numpy.int64, _Shape[m, 1]], numpy.ndarray[numpy.float32, _Shape[m, 1]], numpy.ndarray[numpy.int64, _Shape[m, 2]]]:
    """
    find unique elements, sort them, identify unique values start and end indices and return indices `ind` such that v_sorted = v(ind), `uvals` vector and two column matrix `uvals_from_size` wherefirst col - start index, second col - number of elements. Each row can be considered as v_sorted.segment(uvals_from_size.row(n)) gives the same unique value uval.

    find unique elements, sort them, identify unique values start and end indices and return indices `ind` such that v_sorted = v(ind), `uvals` vector and two column matrix `uvals_from_size` wherefirst col - start index, second col - number of elements. Each row can be considered as v_sorted.segment(uvals_from_size.row(n)) gives the same unique value uval. Also return `v_sorted`
    """
@typing.overload
def sort_unique(v: numpy.ndarray[numpy.float32, _Shape[m, 1]]) -> typing.Tuple[numpy.ndarray[numpy.int64, _Shape[m, 1]], numpy.ndarray[numpy.float32, _Shape[m, 1]], numpy.ndarray[numpy.int64, _Shape[m, 2]], numpy.ndarray[numpy.float32, _Shape[m, 1]]]:
    pass
@typing.overload
def sort_unique(v: numpy.ndarray[numpy.float64, _Shape[m, 1]]) -> typing.Tuple[numpy.ndarray[numpy.int64, _Shape[m, 1]], numpy.ndarray[numpy.float64, _Shape[m, 1]], numpy.ndarray[numpy.int64, _Shape[m, 2]], numpy.ndarray[numpy.float64, _Shape[m, 1]]]:
    pass
@typing.overload
def sort_unique(v: numpy.ndarray[numpy.float64, _Shape[m, 1]]) -> typing.Tuple[numpy.ndarray[numpy.int64, _Shape[m, 1]], numpy.ndarray[numpy.float64, _Shape[m, 1]], numpy.ndarray[numpy.int64, _Shape[m, 2]]]:
    pass
@typing.overload
def traj2ALL(M: numpy.ndarray[numpy.float32, _Shape[m, n]], x0: float, y0: float, kb: float, angularUnitsFrom: str, angularUnitsTo: str, trajFormat: TrajectoryFormat, XNorth: bool) -> numpy.ndarray[numpy.float32, _Shape[m, n]]:
    """
    Convert matrix `M` of any trajectory format to `MD_X_Y_Z_TVD_DX_DY_AZ_INCL` (Z the same as TVDSS). Set `XNorth` to `True` if `X` axis points to the North
    """
@typing.overload
def traj2ALL(M: numpy.ndarray[numpy.float64, _Shape[m, n]], x0: float, y0: float, kb: float, angularUnitsFrom: str, angularUnitsTo: str, trajFormat: TrajectoryFormat, XNorth: bool) -> numpy.ndarray[numpy.float64, _Shape[m, n]]:
    pass
