cmake_minimum_required(VERSION 3.15.0)

if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0077 NEW)

set(COMMON_PROJECT_VERSION 0.1.0)
project(h5geo_dep_test VERSION ${COMMON_PROJECT_VERSION} LANGUAGES C CXX)
message("project: ${CMAKE_PROJECT_NAME}")

# Add files to search path for targets needed
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../../cmake)

add_executable(h5geo_dep_test
  "src/main.cpp"
  )

set(HDF5_ROOT "C:/apps/MSVC_apps_debug/HDF5/1.12.0-zlib")
# set(HDF5_DIR "C:/apps/MSVC_apps_debug/HDF5/1.12.0-zlib/cmake/hdf5")
set(Eigen3_ROOT "C:/apps/eigen")
set(magic_enum_ROOT "C:/apps/magic_enum")
set(h5gt_ROOT "C:/Users/tasik/Documents/Git_Projects/h5gt")
set(h5geo_DIR "C:/Users/tasik/Documents/Git_Projects/h5geo/install/lib/cmake/h5geo")

# LINK DEPENDENCIES
find_package(Eigen3 3.3 REQUIRED)
find_package(HDF5 REQUIRED)
find_package(h5gt REQUIRED)
find_package(magic_enum REQUIRED)
find_package(h5geo CONFIG REQUIRED)

# we don't want to install third-party include dirs
target_include_directories(h5geo_dep_test PRIVATE ${EIGEN3_INCLUDE_DIRS})
target_link_libraries(h5geo_dep_test PRIVATE ${HDF5_C_LIBRARIES})
target_include_directories(h5geo_dep_test PRIVATE ${HDF5_INCLUDE_DIRS})
target_compile_definitions(h5geo_dep_test PRIVATE ${HDF5_DEFINITIONS})
target_link_libraries(h5geo_dep_test PRIVATE magic_enum::magic_enum)
target_include_directories(h5geo_dep_test PRIVATE ${h5gt_INCLUDE_DIRS})
target_link_libraries(h5geo_dep_test PRIVATE h5geo)