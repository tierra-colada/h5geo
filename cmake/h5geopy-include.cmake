set(include_files_h5geopy
  ${CMAKE_SOURCE_DIR}/include/h5geopy/h5base_py.h
  ${CMAKE_SOURCE_DIR}/include/h5geopy/h5basecontainer_py.h
  ${CMAKE_SOURCE_DIR}/include/h5geopy/h5baseobject_py.h
  ${CMAKE_SOURCE_DIR}/include/h5geopy/h5devcurve_py.h
  ${CMAKE_SOURCE_DIR}/include/h5geopy/h5deviation_py.h
  ${CMAKE_SOURCE_DIR}/include/h5geopy/h5easyhull_py.h
  ${CMAKE_SOURCE_DIR}/include/h5geopy/h5enum_py.h
  ${CMAKE_SOURCE_DIR}/include/h5geopy/h5geo_py.h
  ${CMAKE_SOURCE_DIR}/include/h5geopy/h5geofunctions_py.h
  ${CMAKE_SOURCE_DIR}/include/h5geopy/h5interpolation_py.h
  ${CMAKE_SOURCE_DIR}/include/h5geopy/h5logcurve_py.h
  ${CMAKE_SOURCE_DIR}/include/h5geopy/h5map_py.h
  ${CMAKE_SOURCE_DIR}/include/h5geopy/h5mapcontainer_py.h
  ${CMAKE_SOURCE_DIR}/include/h5geopy/h5point_py.h
  ${CMAKE_SOURCE_DIR}/include/h5geopy/h5points_py.h
  ${CMAKE_SOURCE_DIR}/include/h5geopy/h5seis_py.h
  ${CMAKE_SOURCE_DIR}/include/h5geopy/h5seiscontainer_py.h
  ${CMAKE_SOURCE_DIR}/include/h5geopy/h5segy_py.h
  ${CMAKE_SOURCE_DIR}/include/h5geopy/h5sort_py.h
  ${CMAKE_SOURCE_DIR}/include/h5geopy/h5well_py.h
  ${CMAKE_SOURCE_DIR}/include/h5geopy/h5wellcontainer_py.h
  )

if(H5GEO_USE_GDAL)
  list(APPEND include_files_h5geopy ${CMAKE_SOURCE_DIR}/include/h5geopy/h5sr_settings_py.h)
endif()
