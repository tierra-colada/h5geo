project(h5geopy VERSION ${COMMON_PROJECT_VERSION} LANGUAGES C CXX)
message("project: ${PROJECT_NAME}")

include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11
  GIT_TAG        v2.6
)

# FetchContent_MakeAvailable(pybind11) # requires cmake ^14 
if(NOT pybind11_POPULATED)
  FetchContent_Populate(pybind11)
  add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR} EXCLUDE_FROM_ALL)  # we need to exclude pybind11 from installation
  message("PYTHON_EXECUTABLE: ${PYTHON_EXECUTABLE}")
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

install(SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/h5geopy-install.cmake)