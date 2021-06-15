#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "h5geo" for configuration "Release"
set_property(TARGET h5geo APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(h5geo PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/h5geo.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/h5geo.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS h5geo )
list(APPEND _IMPORT_CHECK_FILES_FOR_h5geo "${_IMPORT_PREFIX}/lib/h5geo.lib" "${_IMPORT_PREFIX}/lib/h5geo.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
