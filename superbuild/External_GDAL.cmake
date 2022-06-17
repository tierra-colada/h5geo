set(proj GDAL)
message("external project: ${proj}")

# SET DIRS
set(EP_SOURCE_DIR "${CMAKE_BINARY_DIR}/${proj}")
set(EP_BINARY_DIR "${CMAKE_BINARY_DIR}/${proj}-build")
set(EP_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${proj}-install")
list(APPEND CMAKE_PREFIX_PATH ${EP_INSTALL_DIR})

#-----------------------------------------------------------------------------
set(GDAL_ROOT ${EP_INSTALL_DIR})
set(GDAL_DIR "${GDAL_ROOT}/lib/cmake/gdal")

if(WIN32)
  list(APPEND GDAL_RUNTIME_DIRS "${GDAL_ROOT}/bin")
else()
  list(APPEND GDAL_RUNTIME_DIRS "${GDAL_ROOT}/lib")
endif()

set(DEPENDENCIES ZLIB HDF5 SQLite3 PROJ GEOS)

ExternalProject_Add(${proj}
  GIT_REPOSITORY "https://github.com/OSGeo/gdal.git"
  GIT_TAG "68ceec0be4541da11cf9860141fb6aeae22f95e4"
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
    # find package dirs
    -DZLIB_ROOT:PATH=${ZLIB_ROOT}
    -DPROJ_DIR:PATH=${PROJ_DIR}
    -DGEOS_DIR:PATH=${GEOS_DIR}
    -DHDF5_ROOT:PATH=${HDF5_ROOT}
    -DSQLite3_INCLUDE_DIR:PATH=${SQLite3_INCLUDE_DIRS}
    -DSQLite3_LIBRARY:PATH=${SQLite3_LIBRARIES}
    -DSQLite3_HAS_RTREE:BOOL=ON 
    -DSQLite3_HAS_COLUMN_METADATA:BOOL=ON 
    # GDAL uses CMAKE's findPython (Python_EXECUTABLE) while h5gt and h5geo use pybind11 (PYTHON_EXECUTABLE)
    -DPython_EXECUTABLE:PATH=${PYTHON_EXECUTABLE}
  DEPENDS ${DEPENDENCIES}
  )

# copy site-packages from GDAL subdir to Slicer's python
if(WIN32)
  set(gdal_site_packages_dir ${EP_INSTALL_DIR}/Lib/site-packages)
else()
  set(gdal_site_packages_dir ${EP_INSTALL_DIR}/lib/python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}/site-packages)
endif()

ExternalProject_Add(${proj}-copy-site-packages
  SOURCE_DIR ${EP_SOURCE_DIR}
  BINARY_DIR ${EP_BINARY_DIR}
  INSTALL_DIR ${EP_INSTALL_DIR}
  DOWNLOAD_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  # https://cmake.org/cmake/help/latest/manual/cmake.1.html#command-line-tool-mode
  INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory 
    ${gdal_site_packages_dir}
    ${Python_SITELIB}
  DEPENDS ${proj}
  )
