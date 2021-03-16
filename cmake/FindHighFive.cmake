# - Searches for an installation of the h5gt HDF5 library
#
# Defines:
#
#   h5gt_FOUND         True if h5gt was found, else false
#   h5gt_INCLUDE_DIRS  The directories containing the header files
#   h5gt_LIBRARIES     Libraries to link (h5gt is header-only but depends on libhdf5 which is not)
#   h5gt_LIBRARY_DIRS  Directories for libraries. Pass this to the linker as well.
#   h5gt_CFLAGS        Extra compiler flags
#   h5gt_IS_PARALLEL   Whether underlying HDF5 implementation supports parallel I/O
#
# To specify an additional directory to search, set h5gt_ROOT.
#
# Author: Siddhartha Chaudhuri, 2020
#

SET(h5gt_FOUND FALSE)

# Look for the h5gt header, first in the user-specified location and then in the system locations
SET(h5gt_INCLUDE_DOC "The directory containing the h5gt include file h5gt/H5Object.hpp")
FIND_PATH(h5gt_INCLUDE_DIRS NAMES H5Object.hpp
          PATHS ${h5gt_ROOT}
          PATH_SUFFIXES include/h5gt include/h5gt h5gt h5gt
          DOC ${h5gt_INCLUDE_DOC} NO_DEFAULT_PATH)
IF(NOT h5gt_INCLUDE_DIRS)  # now look in system locations
  FIND_PATH(h5gt_INCLUDE_DIRS NAMES H5Object.hpp
            PATHS ${h5gt_ROOT}
            PATH_SUFFIXES include/h5gt include/h5gt h5gt h5gt
            DOC ${h5gt_INCLUDE_DOC})
ENDIF(NOT h5gt_INCLUDE_DIRS)

IF(h5gt_INCLUDE_DIRS)

  # Work around a FindHDF5 bug (?) that causes system paths to be ignored if HDF5_ROOT is specified
  SET(HDF5_ROOT ${h5gt_ROOT})
  FIND_PACKAGE(HDF5 QUIET)
  IF(NOT HDF5_FOUND)
    SET(HDF5_ROOT )
    FIND_PACKAGE(HDF5 REQUIRED)
  ENDIF()

  IF(HDF5_FOUND)
    SET(h5gt_INCLUDE_DIRS ${HDF5_INCLUDE_DIRS} ${h5gt_INCLUDE_DIRS})
    SET(h5gt_LIBRARIES ${h5gt_LIBRARIES} ${HDF5_LIBRARIES})
    SET(h5gt_LIBRARY_DIRS ${h5gt_LIBRARY_DIRS} ${HDF5_LIBRARY_DIRS})
    SET(h5gt_CFLAGS "${h5gt_CFLAGS} ${HDF5_DEFINITIONS}")
    SET(h5gt_IS_PARALLEL ${HDF5_IS_PARALLEL})

    SET(h5gt_FOUND TRUE)
  ENDIF(HDF5_FOUND)

ENDIF(h5gt_INCLUDE_DIRS)

IF(h5gt_FOUND)
  IF(NOT h5gt_FIND_QUIETLY)
    MESSAGE(STATUS "Found h5gt: headers at ${h5gt_INCLUDE_DIRS}, libraries at ${h5gt_LIBRARIES}")
  ENDIF(NOT h5gt_FIND_QUIETLY)
ELSE(h5gt_FOUND)
  IF(h5gt_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "h5gt not found")
  ENDIF(h5gt_FIND_REQUIRED)
ENDIF(h5gt_FOUND)