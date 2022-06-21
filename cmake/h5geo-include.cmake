set(include_files_h5geo_private
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5enum_string.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5segy.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5deviation.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5easyhull.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5interpolation.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5sort.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5polyfit.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5enum.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5enum_operators.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5coreimpl.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5core_util.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5core_utilimpl.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5basecontainerimpl.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5baseimpl.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5baseobjectimpl.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5devcurveimpl.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5logcurveimpl.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5seisimpl.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5seiscontainerimpl.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5mapcontainerimpl.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5mapimpl.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5wellcontainerimpl.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5wellimpl.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5welltopsimpl.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5point.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5basepointsimpl.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5points1impl.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5points2impl.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5points3impl.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5points4impl.h
  )

if(H5GEO_USE_GDAL)
  list(APPEND include_files_h5geo_private ${CMAKE_SOURCE_DIR}/include/h5geo/private/h5core_sr_settings.h)
endif()

set(include_files_h5geo_public
  ${CMAKE_SOURCE_DIR}/include/h5geo/h5core.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/h5base.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/h5basecontainer.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/h5baseobject.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/h5devcurve.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/h5logcurve.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/h5seis.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/h5seiscontainer.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/h5mapcontainer.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/h5map.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/h5well.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/h5welltops.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/h5wellcontainer.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/h5basepoints.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/h5points1.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/h5points2.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/h5points3.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/h5points4.h
  ${CMAKE_SOURCE_DIR}/include/h5geo/h5points.h
  )
