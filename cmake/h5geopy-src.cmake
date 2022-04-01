set(src_files_h5geopy
  ${CMAKE_SOURCE_DIR}/src/h5geopy/h5base_py.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geopy/h5basecontainer_py.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geopy/h5baseobject_py.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geopy/h5devcurve_py.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geopy/h5deviation_py.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geopy/h5easyhull_py.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geopy/h5enum_py.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geopy/h5geo_py.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geopy/h5geofunctions_py.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geopy/h5interpolation_py.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geopy/h5logcurve_py.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geopy/h5map_py.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geopy/h5mapcontainer_py.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geopy/h5point_py.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geopy/h5basepoints_py.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geopy/h5points3_py.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geopy/h5seis_py.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geopy/h5seiscontainer_py.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geopy/h5segy_py.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geopy/h5sort_py.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geopy/h5well_py.cpp
  ${CMAKE_SOURCE_DIR}/src/h5geopy/h5wellcontainer_py.cpp
  )

if(H5GEO_USE_GDAL)
  list(APPEND src_files_h5geopy ${CMAKE_SOURCE_DIR}/src/h5geopy/h5sr_settings_py.cpp)
endif()
