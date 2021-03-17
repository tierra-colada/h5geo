cmake_minimum_required(VERSION 3.15.0)

if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0077 NEW)

set(COMMON_PROJECT_VERSION 0.1.0)
project(h5geo VERSION ${COMMON_PROJECT_VERSION} LANGUAGES C CXX)
message("project: ${CMAKE_PROJECT_NAME}")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# OPTIONS
option(H5GEO_SUPERBUILD "Superbuild h5geo (doesn't build THREADS lib or TBB)" OFF)
option(H5GEO_USE_THREADS "Use threads (enable std::execution)" ON)
option(H5GEO_BUILD_SHARED_LIBS "Build h5geo as shared lib" ON)
option(H5GEO_BUILD_TESTS "Build tests" ON)

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
include("cmake/include_files.cmake")
include("cmake/src_files.cmake")

if(H5GEO_BUILD_SHARED_LIBS)
  add_library(h5geo SHARED ${src_files} ${include_files})
else()
  add_library(h5geo STATIC ${src_files} ${include_files})
endif()
target_include_directories(h5geo PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# EXPORT DLL
include(GenerateExportHeader)
generate_export_header(
  h5geo
  EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/include/h5geo_export.h
  )

# LINK DEPENDENCIES
find_package(Eigen3 3.3 REQUIRED)
find_package(ZLIB REQUIRED)
find_package(HDF5 REQUIRED)
find_package(h5gt REQUIRED)
find_package(magic_enum REQUIRED)

target_include_directories(h5geo PUBLIC ${EIGEN3_INCLUDE_DIRS})
target_link_libraries(h5geo PUBLIC ${HDF5_C_LIBRARIES})
target_include_directories(h5geo PUBLIC ${HDF5_INCLUDE_DIRS})
target_compile_definitions(h5geo PUBLIC ${HDF5_DEFINITIONS})
target_link_libraries(h5geo PUBLIC magic_enum::magic_enum)
target_include_directories(h5geo PUBLIC ${h5gt_INCLUDE_DIRS})

if(H5GEO_USE_THREADS)
  find_package(TBB REQUIRED)
  target_link_libraries(h5geo PUBLIC TBB::tbb)
endif()

if(H5GEO_BUILD_TESTS)
  include(CTest)
  add_subdirectory(tests)
endif()
