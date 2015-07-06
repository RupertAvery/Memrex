# Locate SDL2_image library
# This module defines
# SDL2_IMAGE_LIBRARY, the name of the library to link against
# SDL2_IMAGE_FOUND, if false, do not try to link to SDL2
# SDL2_IMAGE_INCLUDE_DIR, where to find SDL2_image.h

SET(SDL2_IMAGE_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	$ENV{SDL_WIN32_PATH}
)

FIND_PATH(SDL2_IMAGE_INCLUDE_DIR SDL_image.h
	HINTS
	$ENV{SDL2DIR}
	PATH_SUFFIXES include/SDL2 include
	PATHS ${SDL2_IMAGE_SEARCH_PATHS}
)

FIND_LIBRARY(SDL2_IMAGE_LIBRARY_TEMP
	NAMES SDL2_image
	HINTS
	$ENV{SDL2DIR}
	PATH_SUFFIXES lib64 lib lib/x86 lib/x64
	PATHS ${SDL2_IMAGE_SEARCH_PATHS}
)

IF(SDL2_IMAGE_LIBRARY_TEMP)
	# Set the final string here so the GUI reflects the final state.
	SET(SDL2_IMAGE_LIBRARY ${SDL2_IMAGE_LIBRARY_TEMP} CACHE STRING "Where the SDL2_image Library can be found")
	# Set the temp variable to INTERNAL so it is not seen in the CMake GUI
	SET(SDL2_IMAGE_LIBRARY_TEMP "${SDL2_IMAGE_LIBRARY_TEMP}" CACHE INTERNAL "")
ENDIF(SDL2_IMAGE_LIBRARY_TEMP)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2_IMAGE REQUIRED_VARS SDL2_IMAGE_LIBRARY SDL2_IMAGE_INCLUDE_DIR)
