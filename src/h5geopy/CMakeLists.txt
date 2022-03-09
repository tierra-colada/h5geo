project(h5geopy VERSION ${COMMON_PROJECT_VERSION} LANGUAGES C CXX)

message("project: ${PROJECT_NAME} ${COMMON_PROJECT_VERSION}")

set(HDF5_RUNTIME_DIR "" CACHE PATH "hdf5 runtime is a dependency of h5geopy")
set(UNITS_RUNTIME_DIR "" CACHE PATH "units runtime is a dependency of h5geopy")
set(TBB_RUNTIME_DIR "" CACHE PATH "TBB runtime is an optional dependency of h5geopy")
set(GDAL_RUNTIME_DIR "" CACHE PATH "GDAL runtime is an optional dependency of h5geopy")
set(H5GEO_RUNTIME_DIR "" CACHE PATH "h5geo runtime is a dependency of h5geopy
  (usually you don't need to set it)")
option(COPY_H5GEOPY_RUNTIME_DEPS "Copy hdf5 and h5geo runtime to python site-packages
  (otherwise it will simply add to ENV PATH these dirs in `__init__.py`)" ON)

include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11
  GIT_TAG        v2.9.0
)

# FetchContent_MakeAvailable(pybind11) # requires cmake ^14 
if(NOT pybind11_POPULATED)
  FetchContent_Populate(pybind11)
  add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR} EXCLUDE_FROM_ALL)  # we need to exclude pybind11 from installation
  message(STATUS "PYTHON_EXECUTABLE: ${PYTHON_EXECUTABLE}")
endif()

file(GLOB_RECURSE h5geopy_src
     RELATIVE ${PROJECT_SOURCE_DIR}
     ${CMAKE_SOURCE_DIR}/src/h5geopy/*.cpp
     )
file(GLOB_RECURSE h5geopy_h
     RELATIVE ${PROJECT_SOURCE_DIR}
     ${CMAKE_SOURCE_DIR}/include/h5geopy/*.h
     )

pybind11_add_module(_h5geo
  ${h5geopy_src}
  ${h5geopy_h}
  )
target_include_directories(_h5geo PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_directories(_h5geo PRIVATE pybind11)

if(NOT HDF5_USE_STATIC_LIBRARIES)
  target_link_libraries(_h5geo PRIVATE h5geo)
  target_link_libraries(_h5geo PRIVATE magic_enum::magic_enum)
  target_link_libraries(_h5geo PRIVATE mio::mio)
  target_link_libraries(_h5geo PRIVATE units::units)
  if(H5GEO_USE_GDAL)
    target_link_libraries(_h5geo PRIVATE GDAL::GDAL)
    target_include_directories(_h5geo PRIVATE ${GDAL_TOP_LEVEL_INCLUDE_DIR})
  endif()
else()
  message(FATAL_ERROR "To build h5geopy shared hdf5 lib must be found!")
endif()


#-----------------------------------------------------------------------------
# Install `h5geopy` as python package
#-----------------------------------------------------------------------------
configure_file(${CMAKE_SOURCE_DIR}/cmake/h5geopy-install.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/h5geopy-install.cmake
  @ONLY
  )

# $<CONFIG> at the end of filename is necessary for multiconfig generators
# such as MSVC or Ninja multiconfig (gives error without it:
# "Evaluation file to be written multiple times with different content."
file(GENERATE
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/h5geopy-install-$<CONFIG>.cmake
  INPUT ${CMAKE_CURRENT_BINARY_DIR}/h5geopy-install.cmake
  )

install(SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/h5geopy-install-$<CONFIG>.cmake)
