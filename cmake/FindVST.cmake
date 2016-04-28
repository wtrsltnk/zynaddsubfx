# - Find VST
# Find the native VST includes and library
#
#  VST_INCLUDE_DIRS - where to find audioeffectx.h, etc.
#  VST_FOUND        - True if VST found.

#=============================================================================
# Copyright 2001-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distributed this file outside of CMake, substitute the full
#  License text for the above reference.)

IF (VST_INCLUDE_DIR)
  # Already in cache, be silent
  SET(VST_FIND_QUIETLY TRUE)
ENDIF (VST_INCLUDE_DIR)

FIND_PATH(VST_INCLUDE_DIR 
		NAMES
				audioeffectx.h
		PATHS
				"${CMAKE_PREFIX_PATH}/VST3 SDK/public.sdk/source/vst2.x"
				"${CMAKE_PREFIX_PATH}/VST3 SDK/pluginterfaces/vst2.x"
)
	
# Per-recommendation
SET(VST_INCLUDE_DIRS "${VST_INCLUDE_DIR}")

# handle the QUIETLY and REQUIRED arguments and set VST_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(VST DEFAULT_MSG VST_INCLUDE_DIRS)
