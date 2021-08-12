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
option(H5GEO_BUILD_SHARED_LIBS "Build h5geo as shared lib" ON)
option(H5GEO_BUILD_TESTS "Build tests" ON)
option(H5GEO_BUILD_h5geopy "Build python wrapper (make sure to disable HDF5_USE_STATIC_LIBRARIES)" ON)
option(HDF5_USE_STATIC_LIBRARIES "Use static hdf5 lib" OFF)
option(HDF5_PREFER_PARALLEL "Prefer parallel hdf5 if available" OFF)

set(gtest_force_shared_crt ON CACHE BOOL "Prevent overriding the parent project's compiler/linker settings on Windows")

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
find_package(Eigen3 3.3 REQUIRED)
find_package(ZLIB REQUIRED)
find_package(HDF5 REQUIRED)
find_package(h5gt REQUIRED)
find_package(magic_enum REQUIRED)
find_package(units REQUIRED)

# we don't want to install third-party include dirs
target_include_directories(h5geo PUBLIC $<BUILD_INTERFACE:${EIGEN3_INCLUDE_DIRS}>)
target_link_libraries(h5geo PUBLIC ${HDF5_C_LIBRARIES})
target_include_directories(h5geo PUBLIC ${HDF5_INCLUDE_DIRS})
target_compile_definitions(h5geo PUBLIC ${HDF5_DEFINITIONS})
target_link_libraries(h5geo PUBLIC $<BUILD_INTERFACE:magic_enum::magic_enum>)
target_include_directories(h5geo PUBLIC $<BUILD_INTERFACE:${h5gt_INCLUDE_DIRS}>)
if(H5GEO_BUILD_SHARED_LIBS)
  target_link_libraries(h5geo PUBLIC units-shared)
else()
  target_link_libraries(h5geo PUBLIC units-static)
endif()

if(H5GEO_BUILD_h5geopy)
  add_subdirectory(src/h5geopy)
endif()

if(H5GEO_USE_THREADS)
  # on windows TBB is included by default
  if(NOT MSVC)
    find_package(TBB REQUIRED)
    target_link_libraries(h5geo PUBLIC TBB::tbb)
  endif()
  target_compile_definitions(h5geo PUBLIC H5GEO_USE_THREADS)
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
