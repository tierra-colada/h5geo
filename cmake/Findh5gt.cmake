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
find_path(h5gt_INCLUDE_DIRS NAMES h5gt/H5Object.hpp
          PATHS ${h5gt_ROOT}
          PATH_SUFFIXES include h5gt
          DOC ${h5gt_INCLUDE_DOC} NO_DEFAULT_PATH)

if(h5gt_INCLUDE_DIRS)
  set(h5gt_FOUND TRUE)
endif()

if(h5gt_FOUND)
  message(STATUS "Found h5gt: headers at ${h5gt_INCLUDE_DIRS}")
else(h5gt_FOUND)
  message(STATUS "Could NOT find h5gt")
endif(h5gt_FOUND)