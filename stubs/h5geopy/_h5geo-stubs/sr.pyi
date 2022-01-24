import h5geopy._h5geo.sr
import typing
import h5geopy._h5geo

__all__ = [
    "getAngularUnits",
    "getDomain",
    "getDomainEnum",
    "getLengthUnits",
    "getSpatialReference",
    "getTemporalUnits",
    "setAngularUnits",
    "setDomain",
    "setLengthUnits",
    "setSpatialReference",
    "setSpatialReferenceFromUserInput",
    "setTemporalUnits"
]


def getAngularUnits() -> str:
    pass
def getDomain() -> str:
    pass
def getDomainEnum() -> h5geopy._h5geo.Domain:
    pass
def getLengthUnits() -> str:
    pass
def getSpatialReference() -> OGRSpatialReference:
    pass
def getTemporalUnits() -> str:
    pass
def setAngularUnits(arg0: str) -> None:
    pass
@typing.overload
def setDomain(arg0: h5geopy._h5geo.Domain) -> None:
    pass
@typing.overload
def setDomain(arg0: str) -> None:
    pass
def setLengthUnits(arg0: str) -> None:
    pass
def setSpatialReference(arg0: OGRSpatialReference) -> None:
    pass
@typing.overload
def setSpatialReferenceFromUserInput(arg0: str) -> None:
    pass
@typing.overload
def setSpatialReferenceFromUserInput(arg0: str, arg1: str) -> None:
    pass
def setTemporalUnits(arg0: str) -> None:
    pass
