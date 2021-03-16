# - Searches for an installation of the h5gt HDF5 library
#
# Defines:
#
#   h5gt_FOUND         True if h5gt was found, else false
#   h5gt_INCLUDE_DIRS  The directories containing the header files
#
# To specify an additional directory to search, set h5gt_ROOT.
#
# Based on: Siddhartha Chaudhuri, 2020
#

set(h5gt_FOUND FALSE)

# Look for the h5gt header, first in the user-specified location and then in the system locations
set(h5gt_INCLUDE_DOC "The directory containing the h5gt include file h5gt/H5Object.hpp")
find_path(h5gt_INCLUDE_DIRS NAMES H5Object.hpp
          PATHS ${h5gt_ROOT}
          PATH_SUFFIXES include/h5gt include/h5gt h5gt h5gt
          DOC ${h5gt_INCLUDE_DOC} NO_DEFAULT_PATH)
if(NOT h5gt_INCLUDE_DIRS)  # now look in system locations
  find_path(h5gt_INCLUDE_DIRS NAMES H5Object.hpp
            PATHS ${h5gt_ROOT}
            PATH_SUFFIXES include/h5gt include/h5gt h5gt h5gt
            DOC ${h5gt_INCLUDE_DOC})
endif(NOT h5gt_INCLUDE_DIRS)

if(h5gt_INCLUDE_DIRS)
  set(h5gt_FOUND TRUE)
endif()

if(h5gt_FOUND)
  message(STATUS "Found h5gt: headers at ${h5gt_INCLUDE_DIRS}, libraries at ${h5gt_LIBRARIES}")
else(h5gt_FOUND)
message(FATAL_ERROR "h5gt not found")
endif(h5gt_FOUND)