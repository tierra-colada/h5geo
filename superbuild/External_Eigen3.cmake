message("external project: Eigen3")

# SET DIRS
set(EP_SOURCE_DIR "${CMAKE_BINARY_DIR}/Eigen3")
set(EP_BINARY_DIR "${CMAKE_BINARY_DIR}/Eigen3-build")
set(EP_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/Eigen3-install")
list(APPEND CMAKE_PREFIX_PATH ${EP_INSTALL_DIR})

#-----------------------------------------------------------------------------
set(Eigen3_ROOT PATH ${EP_INSTALL_DIR})
find_package(Eigen3)

set(DEPENDENCIES "")

if(NOT DEFINED EIGEN3_FOUND OR NOT EIGEN3_FOUND)
  ExternalProject_Add(Eigen3
    GIT_REPOSITORY "https://gitlab.com/libeigen/eigen.git"
    GIT_TAG "3.4.0"
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
      # Lib settings
      -DBUILD_TESTING:BOOL=OFF
    DEPENDS ${DEPENDENCIES}
    )
else()
  # Add empty project that exports target Eigen3
  ExternalProject_Add(Eigen3
    SOURCE_DIR ${EP_SOURCE_DIR}
    BINARY_DIR ${EP_BINARY_DIR}
    INSTALL_DIR ${EP_INSTALL_DIR}
    DOWNLOAD_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    DEPENDS ${DEPENDENCIES}
    )
endif()
