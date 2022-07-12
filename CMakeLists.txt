cmake_minimum_required(VERSION 3.16.0)

if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
message(STATUS "Setting C++ standard - " ${CMAKE_CXX_STANDARD})

cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0077 NEW)
cmake_policy(SET CMP0087 NEW) # add generator expressions to `install` command

set(COMMON_PROJECT_VERSION 0.4.0)
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

set(H5GEO_CHAR_ARRAY_SIZE "50" CACHE STRING "Number > 1 used to init char array for h5geo::Point3 for example")

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
  add_library(h5geo SHARED ${src_files_h5geo} ${include_files_h5geo_public} ${include_files_h5geo_private})
else()
  add_library(h5geo STATIC ${src_files_h5geo} ${include_files_h5geo_public} ${include_files_h5geo_private})
endif()
target_compile_definitions(h5geo PUBLIC H5GEO_CHAR_ARRAY_SIZE=${H5GEO_CHAR_ARRAY_SIZE})
target_include_directories(h5geo PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include>
  )

set(h5geo_export_header ${PROJECT_BINARY_DIR}/h5geo_export.h)
list(APPEND include_files_h5geo_public ${h5geo_export_header})

# EXPORT DLL
include(GenerateExportHeader)
generate_export_header(
  h5geo
  EXPORT_FILE_NAME 
    ${h5geo_export_header}
  )

set_target_properties(h5geo
                      PROPERTIES 
                        PUBLIC_HEADER "${include_files_h5geo_public}"
                        PRIVATE_HEADER "${include_files_h5geo_private}"
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
target_link_libraries(h5geo PRIVATE units::units)
# MIO is PRIVATE as it is for internal purpose only
target_link_libraries(h5geo PRIVATE mio::mio)

if(H5GEO_USE_THREADS)
  find_package(TBB REQUIRED)
  target_link_libraries(h5geo PRIVATE TBB::tbb)
  target_compile_definitions(h5geo PUBLIC H5GEO_USE_THREADS)
  find_package(OpenMP REQUIRED)
  target_link_libraries(h5geo PRIVATE OpenMP::OpenMP_CXX)
endif()

if(H5GEO_USE_GDAL)
  find_package(GDAL REQUIRED)

  foreach(_lib ${GDAL_LIBS})
    if(EXISTS ${_lib})
      target_link_libraries(GDAL::GDAL INTERFACE ${_lib})
    endif()
  endforeach()

  if(IS_DIRECTORY ${GDAL_LIBS_DIR})
    file(GLOB GDAL_LIBS "${GDAL_LIBS_DIR}/lib*")
    target_link_libraries(GDAL::GDAL INTERFACE ${GDAL_LIBS})
  endif()

  target_link_libraries(h5geo PRIVATE GDAL::GDAL)
  target_compile_definitions(h5geo PUBLIC H5GEO_USE_GDAL)
endif()

if(H5GEO_BUILD_h5geopy)
  add_subdirectory(src/h5geopy)
endif()

if(H5GEO_BUILD_TESTS)
  include(CTest)
  add_subdirectory(tests)
endif()

#-----------------------------------------------------------------------------
# Create config files
#-----------------------------------------------------------------------------
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/h5geo-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/h5geo-config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/h5geo
  PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_SYSCONFDIR
  )

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/h5geo-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
  )

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/h5geo-config.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/h5geo-config-version.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/h5geo
        )

install(TARGETS h5geo
        EXPORT h5geo-targets
        RUNTIME
          DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY
          DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE
          DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/h5geo
        PRIVATE_HEADER
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/h5geo/private
        )

install(EXPORT h5geo-targets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/h5geo)
