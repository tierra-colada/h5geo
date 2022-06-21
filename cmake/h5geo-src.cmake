set(src_files_h5geo
  ${CMAKE_SOURCE_DIR}/src/h5geo/h5core_util.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geo/h5baseimpl.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geo/h5basecontainerimpl.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geo/h5baseobjectimpl.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geo/h5devcurveimpl.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geo/h5logcurveimpl.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geo/h5seisimpl.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geo/h5seiscontainerimpl.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geo/h5mapcontainerimpl.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geo/h5mapimpl.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geo/h5wellimpl.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geo/h5welltopsimpl.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geo/h5wellcontainerimpl.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geo/h5basepointsimpl.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geo/h5points1impl.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geo/h5points2impl.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geo/h5points3impl.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geo/h5points4impl.cpp
  )

if(H5GEO_USE_GDAL)
  list(APPEND src_files_h5geo ${CMAKE_SOURCE_DIR}/src/h5geo/h5core_sr_settings.cpp)
endif()
