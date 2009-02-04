# - Find CGAL
# adapted from http://pgrouting.postlbs.org/browser/trunk/cmake/FindCGAL.cmake?rev=53
# Find the CGAL includes and client library
# This module defines
#  CGAL_INCLUDE_DIR, where to find CGAL.h
#  CGAL_LIBRARIES, the libraries needed to use CGAL.
#  CGAL_FOUND, If false, do not try to use CGAL.

IF(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES)
	SET(CGAL_FOUND TRUE)
ELSE(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES)
	FIND_PATH(CGAL_INCLUDE_DIR basic.h
		/usr/include/CGAL
		/usr/local/include/CGAL
		$ENV{ProgramFiles}/CGAL/*/include/CGAL
		$ENV{SystemDrive}/CGAL/*/include/CGAL
	)
	FIND_LIBRARY(CGAL_LIBRARIES NAMES CGAL libCGAL
		PATHS
		/usr/lib
		/usr/local/lib
		/usr/lib/CGAL
		/usr/lib64
		/usr/local/lib64
		/usr/lib64/CGAL
		$ENV{ProgramFiles}/CGAL/*/lib/ms
		$ENV{SystemDrive}/CGAL/*/lib/ms
	)
	
	IF(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES)
		SET(CGAL_FOUND TRUE)
		MESSAGE(STATUS "Found CGAL: ${CGAL_INCLUDE_DIR}, ${CGAL_LIBRARIES}")
		INCLUDE_DIRECTORIES(${CGAL_INCLUDE_DIR} $ENV{CGAL_CFG})
	ELSE(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES)
		SET(CGAL_FOUND FALSE)
		MESSAGE(STATUS "CGAL not found.")
	ENDIF(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES)
	
	MARK_AS_ADVANCED(CGAL_INCLUDE_DIR CGAL_LIBRARIES)
ENDIF(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES)
