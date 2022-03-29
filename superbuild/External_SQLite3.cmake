set(proj SQLite3)
message("external project: ${proj}")

# SET DIRS
set(EP_SOURCE_DIR "${CMAKE_BINARY_DIR}/${proj}")
set(EP_BINARY_DIR "${CMAKE_BINARY_DIR}/${proj}-build")
set(EP_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${proj}-install")
list(APPEND CMAKE_PREFIX_PATH ${EP_INSTALL_DIR})

#-----------------------------------------------------------------------------
set(SQLite3_ROOT ${EP_INSTALL_DIR})
find_package(SQLite3)

if(WIN32)
  set(SQLite3_EXE "${SQLite3_ROOT}/bin/sqlite3.exe")
  set(SQLite3_LIBRARIES "${SQLite3_ROOT}/lib/sqlite3.lib")
  list(APPEND GDAL_RUNTIME_DIRS "${SQLite3_ROOT}/bin")
else()
  set(SQLite3_EXE "${SQLite3_ROOT}/bin/sqlite3")
  set(SQLite3_LIBRARIES "${SQLite3_ROOT}/lib/libsqlite3.so")
  list(APPEND GDAL_RUNTIME_DIRS "${SQLite3_ROOT}/lib")
endif()
set(SQLite3_INCLUDE_DIRS "${SQLite3_ROOT}/include")
list(APPEND GDAL_LIBS ${SQLite3_LIBRARIES})

set(DEPENDENCIES "")

ExternalProject_Add(${proj}
  GIT_REPOSITORY "https://github.com/tierra-colada/sqlite-amalgamation.git"
  GIT_TAG "f2ada7ab2237696ed641f24fc2b07ad41ab83395"
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
    -DSQLITE_OMIT_DEPRECATED:BOOL=OFF # must be OFF or undefined external symbols on Windows
    -DSQLITE_OMIT_PROGRESS_CALLBACK:BOOL=OFF # must be OFF or undefined external symbols on Windows
    -DSQLITE_OMIT_SHARED_CACHE:BOOL=OFF # must be OFF or undefined external symbols on Windows
    # recommended options would define SQLITE_OMIT_DEPRECATED and SQLITE_OMIT_DECLTYPE,
    # which would cause build errors in Python, so go with default options instead
    -DBUILD_RECOMMENDED_OPTS:BOOL=ON # if python build fails then set it to OFF
  DEPENDS ${DEPENDENCIES}
  )
