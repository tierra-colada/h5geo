cmake_minimum_required(VERSION 3.16.0)

if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
message("-- Setting C++ standard - " ${CMAKE_CXX_STANDARD})

cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0077 NEW)
cmake_policy(SET CMP0087 NEW) # add generator expressions to `install` command

set(COMMON_PROJECT_VERSION 0.1.0)
project(h5geo VERSION ${COMMON_PROJECT_VERSION} LANGUAGES C CXX)

message("project: ${CMAKE_PROJECT_NAME} ${COMMON_PROJECT_VERSION}")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# OPTIONS
option(H5GEO_SUPERBUILD "Superbuild h5geo" OFF)
option(H5GEO_USE_THREADS "Use threads (enable std::execution)" ON)
option(H5GEO_USE_GDAL "Use GDAL (uses cmake official FindGDAL module)" ON)
option(H5GEO_BUILD_SHARED_LIBS "Build h5geo as shared lib" ON)
option(H5GEO_BUILD_TESTS "Build tests" ON)
option(H5GEO_BUILD_h5geopy "Build python wrapper (make sure to disable HDF5_USE_STATIC_LIBRARIES)" ON)
option(HDF5_USE_STATIC_LIBRARIES "Use static hdf5 lib" OFF)
option(HDF5_PREFER_PARALLEL "Prefer parallel hdf5 if available" OFF)

set(H5GEO_CHAR_ARR_SIZE "50" CACHE STRING "Number > 1 used to init char array for h5geo::Point for example")

set(gtest_force_shared_crt ON CACHE BOOL "Prevent overriding the parent project's compiler/linker settings on Windows")
set(GDAL_LIBS "" CACHE FILEPATH "Path to all GDAL dependcy (probably geos, proj etc.)")
set(GDAL_LIBS_DIR "" CACHE PATH "Path to the folder where GDAL dependcy reside (probably geos, proj etc.)")

# Add files to search path for targets needed
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# SUPERBUILD
if(H5GEO_SUPERBUILD)
  add_subdirectory(superbuild)
  return()
endif()

#-------------------------------------------------------------------
# NORMAL BUILD
include("cmake/h5geo-include.cmake")
include("cmake/h5geo-src.cmake")

if(H5GEO_BUILD_SHARED_LIBS)
  add_library(h5geo SHARED ${src_files_h5geo} ${include_files_h5geo})
else()
  add_library(h5geo STATIC ${src_files_h5geo} ${include_files_h5geo})
endif()
target_compile_definitions(h5geo PUBLIC H5GEO_CHAR_ARR_SIZE=${H5GEO_CHAR_ARR_SIZE})
target_include_directories(h5geo PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
install(TARGETS h5geo DESTINATION lib EXPORT h5geo-targets)

# EXPORT DLL
include(GenerateExportHeader)
generate_export_header(
  h5geo
  EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/include/h5geo/misc/h5geo_export.h
  )

# LINK DEPENDENCIES
find_package(Eigen3 REQUIRED)
find_package(ZLIB REQUIRED)
find_package(HDF5 REQUIRED)
find_package(h5gt REQUIRED)
find_package(magic_enum REQUIRED)
find_package(units REQUIRED CONFIG)
find_package(mio REQUIRED)

# we don't want to install third-party include dirs
target_include_directories(h5geo PUBLIC $<BUILD_INTERFACE:${EIGEN3_INCLUDE_DIRS}>)
target_link_libraries(h5geo PUBLIC ${HDF5_C_LIBRARIES})
target_include_directories(h5geo PUBLIC $<BUILD_INTERFACE:${HDF5_INCLUDE_DIRS}>)
target_compile_definitions(h5geo PUBLIC ${HDF5_DEFINITIONS})
# MAGIC_ENUM is PRIVATE as it is for internal purpose only
target_link_libraries(h5geo PRIVATE $<BUILD_INTERFACE:magic_enum::magic_enum>)
target_include_directories(h5geo PUBLIC $<BUILD_INTERFACE:${h5gt_INCLUDE_DIRS}>)
target_link_libraries(h5geo PUBLIC units::units)
# MIO is PRIVATE as it is for internal purpose only
target_link_libraries(h5geo PRIVATE mio::mio)

if(H5GEO_BUILD_h5geopy)
  add_subdirectory(src/h5geopy)
endif()

if(H5GEO_USE_THREADS)
  find_package(TBB REQUIRED)
  target_include_directories(h5geo PUBLIC ${TBB_INCLUDE_DIRS})
  target_link_libraries(h5geo PUBLIC ${TBB_LIBRARIES})
  target_compile_definitions(h5geo PUBLIC H5GEO_USE_THREADS)
  find_package(OpenMP REQUIRED)
  target_link_libraries(h5geo PUBLIC OpenMP::OpenMP_CXX)
endif()

if(H5GEO_USE_GDAL)
  find_package(GDAL REQUIRED)

  foreach(_lib ${GDAL_LIBS})
    if(EXISTS ${_lib})
      target_link_libraries(GDAL::GDAL INTERFACE ${_lib})
    endif()
  endforeach()

  if(IS_DIRECTORY ${GDAL_LIBS_DIR})
    file(GLOB _GDAL_LIBS "${GDAL_LIBS_DIR}/lib*")
    target_link_libraries(GDAL::GDAL INTERFACE ${_GDAL_LIBS})
  endif()

  target_link_libraries(h5geo PUBLIC GDAL::GDAL)
  target_compile_definitions(h5geo PUBLIC H5GEO_USE_GDAL)

  # 'GDAL_INCLUDE_DIRS' rpovided by FindGDAL.cmake returns path
  # to 'gdal.h' but we need the path to 'gdal/gdal.h'
  find_path(GDAL_TOP_LEVEL_INCLUDE_DIR gdal/gdal.h gdal/gdal_priv.h
    HINTS ${GDAL_INCLUDE_DIR}/..
    PATH_SUFFIXES include)

  target_include_directories(h5geo PUBLIC ${GDAL_TOP_LEVEL_INCLUDE_DIR})
  message("GDAL_INCLUDE_DIR: ${GDAL_INCLUDE_DIR}")
  message("GDAL_TOP_LEVEL_INCLUDE_DIR: ${GDAL_TOP_LEVEL_INCLUDE_DIR}")
endif()

if(H5GEO_BUILD_TESTS)
  include(CTest)
  add_subdirectory(tests)
endif()

#-----------------------------------------------------------------------------
# Create config files
#-----------------------------------------------------------------------------
set(INCLUDE_INSTALL_DIR "include/")
set(LIB_INSTALL_DIR "lib/")
# set(SYSCONFIG_INSTALL_DIR "etc/h5geo/")

include(CMakePackageConfigHelpers)
configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/h5geo-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/h5geo-config.cmake"
  INSTALL_DESTINATION "${LIB_INSTALL_DIR}/cmake/h5geo/"
  PATH_VARS INCLUDE_INSTALL_DIR
  )

install(EXPORT h5geo-targets DESTINATION "${LIB_INSTALL_DIR}/cmake/h5geo/")

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/h5geo-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
  )

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/h5geo-config.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/h5geo-config-version.cmake"
        DESTINATION "${LIB_INSTALL_DIR}/cmake/h5geo/"
        )

install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/" DESTINATION "include")

install(TARGETS h5geo DESTINATION "lib")
