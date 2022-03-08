project(h5geo_superbuild VERSION ${COMMON_PROJECT_VERSION} LANGUAGES C CXX)
message("project: ${PROJECT_NAME}")

find_package(Git REQUIRED)

include(ExternalProject)

set(ep_common_c_flags "${CMAKE_C_FLAGS_INIT} ${ADDITIONAL_C_FLAGS}")
set(ep_common_cxx_flags "${CMAKE_CXX_FLAGS_INIT} ${ADDITIONAL_CXX_FLAGS}")

if(H5GEO_BUILD_h5geopy)
  find_package(Python REQUIRED)
endif()

include("External_Eigen3.cmake")
include("External_mio.cmake")
include("External_ZLIB.cmake")
include("External_HDF5.cmake")
include("External_h5gt.cmake")
include("External_magic_enum.cmake")
include("External_units.cmake")

if(H5GEO_USE_THREADS)
  include("External_TBB.cmake")
endif()

if(H5GEO_USE_GDAL)
  if(H5GEO_BUILD_h5geopy)
    include("External_GDAL.cmake")
  else()
    message(FATAL_ERROR "GDAL maybe built only with python support enabled (i.e. H5GEO_BUILD_h5geopy ON)")
  endif()
endif()

include("External_h5geo.cmake")
