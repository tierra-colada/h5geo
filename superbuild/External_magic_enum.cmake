message("external project: magic_enum")

# SET DIRS
set(EP_SOURCE_DIR "${CMAKE_BINARY_DIR}/magic_enum")
set(EP_BINARY_DIR "${CMAKE_BINARY_DIR}/magic_enum-build")
set(EP_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/magic_enum-install")
list(APPEND CMAKE_PREFIX_PATH ${EP_INSTALL_DIR})

#-----------------------------------------------------------------------------
set(magic_enum_ROOT PATH ${EP_INSTALL_DIR})
find_package(magic_enum)

set(DEPENDENCIES "")

if(NOT DEFINED magic_enum_FOUND OR NOT magic_enum_FOUND)
  ExternalProject_Add(magic_enum
    GIT_REPOSITORY "https://github.com/Neargye/magic_enum.git"
    GIT_TAG "v0.7.2"
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
      -DMAGIC_ENUM_OPT_BUILD_TESTS:BOOL=OFF
    DEPENDS ${DEPENDENCIES}
    )
else()
  # Add empty project that exports target magic_enum
  ExternalProject_Add(magic_enum
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
