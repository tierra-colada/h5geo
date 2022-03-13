message("external project: SQLite3")

# SET DIRS
set(EP_SOURCE_DIR "${CMAKE_BINARY_DIR}/SQLite3")
set(EP_BINARY_DIR "${CMAKE_BINARY_DIR}/SQLite3-build")
set(EP_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/SQLite3-install")
list(APPEND CMAKE_PREFIX_PATH ${EP_INSTALL_DIR})

#-----------------------------------------------------------------------------
set(SQLite3_ROOT ${EP_INSTALL_DIR})
if(WIN32)
  set(SQLite3_RUNTIME_DIR "${SQLite3_ROOT}/bin")
  set(SQLite3_EXE "${SQLite3_ROOT}/bin/sqlite3.exe")
  set(SQLite3_LIBRARIES "${SQLite3_ROOT}/lib/sqlite3.lib")
else()
  set(SQLite3_RUNTIME_DIR "${SQLite3_ROOT}/lib")
  set(SQLite3_EXE "${SQLite3_ROOT}/bin/sqlite3")
  set(SQLite3_LIBRARIES "${SQLite3_ROOT}/lib/libsqlite3.so")
endif()
set(SQLite3_INCLUDE_DIRS "${SQLite3_ROOT}/include")
find_package(SQLite3)

set(DEPENDENCIES "")

if(NOT DEFINED SQLite3_FOUND OR NOT SQLite3_FOUND)
  ExternalProject_Add(SQLite3
    GIT_REPOSITORY "https://github.com/tierra-colada/sqlite-amalgamation.git"
    GIT_TAG "3fad63ffdead0a53750e388d5681f22ac65c55eb"
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
      # executable is needed by PROJ lib
      -DBUILD_SHELL:BOOL=ON
      # if PROJ is built as SHARED then it asks sqlite to be SHARED as well
      -DSQLITE_BUILD_SHARED:BOOL=ON 
      # GDAL recommends
      -DSQLITE_ENABLE_RTREE:BOOL=ON 
      -DSQLITE_ENABLE_MATH_FUNCTIONS:BOOL=ON 
      -DSQLITE_ENABLE_COLUMN_METADATA:BOOL=ON 
      -DSQLITE_OMIT_DECLTYPE:BOOL=OFF # must be OFF if SQLITE_ENABLE_COLUMN_METADATA is ON
      # recommended options would define SQLITE_OMIT_DEPRECATED and SQLITE_OMIT_DECLTYPE,
      # which would cause build errors in Python, so go with default options instead
      -DBUILD_RECOMMENDED_OPTS:BOOL=ON # if python build fails then set it to OFF
    DEPENDS ${DEPENDENCIES}
    )
else()
  # Add empty project that exports target SQLite3
  ExternalProject_Add(SQLite3
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
