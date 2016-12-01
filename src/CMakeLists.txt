# Include this package's headers first
include_directories( BEFORE ./ ./include ../../include ../../../include)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
cmake_minimum_required(VERSION 2.8.11)
if(POLICY CMP0043)
  cmake_policy(SET CMP0043 OLD)
endif()

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)


# CGAL
find_package(Qt5 COMPONENTS Xml Script OpenGL Svg)
find_package(CGAL COMPONENTS Qt5)
#include(${CGAL_USE_FILE})
set(CGAL_DONT_OVERRIDE_CMAKE_FLAGS TRUE CACHE BOOL "Force CGAL to maintain CMAKE flags")

find_package(CGAL REQUIRED)
find_package(OpenGL REQUIRED)

find_package(GMP REQUIRED)
set(QT_USE_QTOPENGL ON)



# QGLViwer needs Qt5 configured with QtOpenGL and QtXml support

set( QT_USE_QTOPENGL TRUE )
set( QT_USE_QTXML    TRUE )
set( QT_USE_QTMAIN   TRUE )

#include( ${CGAL_USE_FILE} )



include_directories(${Qt5Widgets_INCLUDE_DIRS})
ADD_DEFINITIONS(${QT_DEFINITIONS} -Wall -O2)

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
include_directories(${Qt5Widgets_INCLUDES})

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})

# Executables fail to build with Qt 5 in the default configuration
# without -fPIE. We add that here.
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${QT_INCLUDE_DIR}
    ${GMP_INCLUDE_DIR}
    # including the following directory causes conflicts with GMP headers
    #${CGAL_INCLUDE_DIR} 
)
ADD_DEFINITIONS(${CGAL_DEFINITIONS})


#if ( CGAL_FOUND AND CGAL_Qt5_FOUND AND Qt5_FOUND )
if ( CGAL_FOUND AND Qt5_FOUND )

add_definitions(-DQT_NO_KEYWORDS)



SET(SOURCES
    main.cpp
    glwidget.cpp
    camera.cpp
    point.cpp
    bundleparser.cpp
    visiblepoint.cpp
    mainwindow.cpp
    imagelist.cpp
    plane.cpp
    commonplane.cpp
    triangulation.cpp
    photo.cpp
)
SET(HEADERS
    glwidget.h
    camera.h
    point.h
    bundleparser.h
    visiblepoint.h
    mainwindow.h
    imagelist.h
    plane.h
    commonplane.h
    triangulation.h
    photo.h
)


ADD_EXECUTABLE(pixelstruct ${SOURCES} ${HEADERS})

# Link with Qt libraries
TARGET_LINK_LIBRARIES(pixelstruct
    ${QT5_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
    ${GMP_LIBRARIES}
)

add_to_cached_list( CGAL_EXECUTABLE_TARGETS pixelstruct )

# Link the executable to CGAL and third-party libraries
target_link_libraries(pixelstruct   ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} )



else()

  message(STATUS "NOTICE: This demo requires CGAL and Qt5, and will not be compiled.")

endif()




