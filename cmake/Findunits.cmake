# - Searches for an installation of the UNITS library
#
# Search is controlled by variable:
#
#   units_ROOT          Dir where units is installed
#
# Defines:
#
#   units_FOUND         True if units was found, else false
#   UNITS_INCLUDE_DIRS  The directories containing the header files
#   UNITS_LIBRARIES     Units libraries


set(units_FOUND FALSE)

# Look for the units header, first in the user-specified location and then in the system locations
set(UNITS_INCLUDE_DOC "The directory containing the units include file units/H5Object.hpp")
find_path(UNITS_INCLUDE_DIRS
          NAMES units/units.hpp
          PATHS ${units_ROOT}
          PATH_SUFFIXES include units
          DOC ${UNITS_INCLUDE_DOC}
          NO_DEFAULT_PATH)

set(UNITS_LIBRARIES_DOC "Units libraries")
find_library(UNITS_LIBRARIES
  NAMES units libunits unitslib
  PATHS ${units_ROOT}
  PATH_SUFFIXES lib units
  DOC ${UNITS_LIBRARIES_DOC}
  NO_DEFAULT_PATH)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(UNITS
  FOUND_VAR units_FOUND
  REQUIRED_VARS
    UNITS_INCLUDE_DIRS
    UNITS_LIBRARIES
)
