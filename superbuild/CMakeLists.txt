project(h5geo_superbuild VERSION ${COMMON_PROJECT_VERSION} LANGUAGES C CXX)
message("project: ${PROJECT_NAME}")

find_package(Git REQUIRED)

include(ExternalProject)

set(ep_common_c_flags "${CMAKE_C_FLAGS_INIT} ${ADDITIONAL_C_FLAGS}")
set(ep_common_cxx_flags "${CMAKE_CXX_FLAGS_INIT} ${ADDITIONAL_CXX_FLAGS}")

include("External_Eigen3.cmake")
include("External_ZLIB.cmake")
include("External_HDF5.cmake")
include("External_h5gt.cmake")
include("External_magic_enum.cmake")
include("External_UNITS.cmake")

if(H5GEO_USE_THREADS)
  include("External_TBB.cmake")
endif()

include("External_h5geo.cmake")
