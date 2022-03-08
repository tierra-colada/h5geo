message("external project: GDAL")

# SET DIRS
set(EP_SOURCE_DIR "${CMAKE_BINARY_DIR}/GDAL")

if(WIN32)
  set(GDAL_URL "https://download.lfd.uci.edu/pythonlibs/x6hvwk7i/GDAL-3.4.1-cp39-cp39-win_amd64.whl")  # https://www.lfd.uci.edu/~gohlke/pythonlibs/#gdal
elseif(UNIX)
  set(GDAL_URL "https://master.dl.sourceforge.net/project/gdal-wheels-for-linux/GDAL-3.4.1-cp39-cp39-manylinux_2_5_x86_64.manylinux1_x86_64.whl?viasf=1")  # https://sourceforge.net/projects/gdal-wheels-for-linux/files/
else()
  message(FATAL_ERROR "Unsupported platform (in the future MACOS support will be added)")
endif()

set(DEPENDENCIES "")

ExternalProject_Add(GDAL
  DOWNLOAD_COMMAND ""
  DOWNLOAD_DIR ${EP_SOURCE_DIR}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ${Python_EXECUTABLE} -m pip install ${GDAL_URL}
  DEPENDS ${DEPENDENCIES}
)

message("Python_SITELIB: ${Python_SITELIB}")
set(GDAL_ROOT "${Python_SITELIB}/osgeo")
set(GDAL_DIR "${Python_SITELIB}/osgeo")
set(GDAL_INCLUDE_DIR "${Python_SITELIB}/osgeo/include") # needed by FindGDAL
if(WIN32)
  set(GDAL_LIBRARY "${Python_SITELIB}/osgeo/lib/gdal_i.lib")
else()
  set(GDAL_LIBRARY "${Python_SITELIB}/osgeo/lib/libgdal.so")
  set(GDAL_LIBS_DIR "${Python_SITELIB}/GDAL.libs")
  file(GLOB GDAL_LIBS
    "${GDAL_LIBS_DIR}/lib*"
  )
endif()