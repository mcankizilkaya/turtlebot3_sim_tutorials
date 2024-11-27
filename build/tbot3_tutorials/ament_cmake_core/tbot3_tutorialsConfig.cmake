# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_tbot3_tutorials_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED tbot3_tutorials_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(tbot3_tutorials_FOUND FALSE)
  elseif(NOT tbot3_tutorials_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(tbot3_tutorials_FOUND FALSE)
  endif()
  return()
endif()
set(_tbot3_tutorials_CONFIG_INCLUDED TRUE)

# output package information
if(NOT tbot3_tutorials_FIND_QUIETLY)
  message(STATUS "Found tbot3_tutorials: 0.0.0 (${tbot3_tutorials_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'tbot3_tutorials' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${tbot3_tutorials_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(tbot3_tutorials_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${tbot3_tutorials_DIR}/${_extra}")
endforeach()
