# Find the QtWidgets library

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(CGAL REQUIRED)
FIND_PACKAGE(GMP REQUIRED)
SET(QT_USE_QTOPENGL ON)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

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
TARGET_LINK_LIBRARIES(pixelstruct
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTMAIN_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    ${OPENGL_gl_LIBRARY}
    ${OPENGL_glu_LIBRARY}
    ${CGAL_LIBRARIES}
    ${GMP_LIBRARIES}
)
