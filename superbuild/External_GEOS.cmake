set(proj GEOS)
message("external project: ${proj}")

# SET DIRS
set(EP_SOURCE_DIR "${CMAKE_BINARY_DIR}/${proj}")
set(EP_BINARY_DIR "${CMAKE_BINARY_DIR}/${proj}-build")
set(EP_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${proj}-install")
list(APPEND CMAKE_PREFIX_PATH ${EP_INSTALL_DIR})

#-----------------------------------------------------------------------------
set(GEOS_ROOT ${EP_INSTALL_DIR})
set(GEOS_DIR "${GEOS_ROOT}/lib/cmake/GEOS")

if(WIN32)
  set(GEOS_LIBRARIES "${GEOS_ROOT}/lib/geos_c.lib")
  list(APPEND GDAL_RUNTIME_DIRS "${GEOS_ROOT}/bin")
else()
  set(GEOS_LIBRARIES "${GEOS_ROOT}/lib/libgeos_c.so")
  list(APPEND GDAL_RUNTIME_DIRS "${GEOS_ROOT}/lib")
endif()
list(APPEND GDAL_LIBS ${GEOS_LIBRARIES})

set(DEPENDENCIES "")

ExternalProject_Add(${proj}
  GIT_REPOSITORY "https://github.com/libgeos/geos.git"
  GIT_TAG "3.10.2"
  SOURCE_DIR ${EP_SOURCE_DIR}
  BINARY_DIR ${EP_BINARY_DIR}
  INSTALL_DIR ${EP_INSTALL_DIR}
  CMAKE_CACHE_ARGS
    # CMake settings
    -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
    -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
    -DCMAKE_CXX_STANDARD:STRING=${CMAKE_CXX_STANDARD}
    -DCMAKE_CXX_STANDARD_REQUIRED:BOOL=${CMAKE_CXX_STANDARD_REQUIRED}
    -DCMAKE_CXX_EXTENSIONS:BOOL=${CMAKE_CXX_EXTENSIONS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    # GNUInstallDirs expands CMAKE_INSTALL_LIBDIR to 'lib' or to'lib64' depending on the OS
    -DCMAKE_INSTALL_LIBDIR:PATH=lib
    # Lib settings
    -DBUILD_TESTING:BOOL=OFF
  DEPENDS ${DEPENDENCIES}
  )
