FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(CGAL REQUIRED)
SET(QT_USE_QTOPENGL ON)
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS} -Wall -O2)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_BINARY_DIR}
	${QT_INCLUDE_DIR}
# 	${CGAL_INCLUDE_DIR} # including this directory causes conflicts with GMP headers
)

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
)

QT4_AUTOMOC(${SOURCES})

ADD_EXECUTABLE(pixelstruct ${SOURCES} ${HEADERS})
TARGET_LINK_LIBRARIES(pixelstruct
	${QT_QTCORE_LIBRARY}
	${QT_QTGUI_LIBRARY}
	${QT_QTMAIN_LIBRARY}
	${QT_QTOPENGL_LIBRARY}
	${OPENGL_gl_LIBRARY}
	${OPENGL_glu_LIBRARY}
	${CGAL_LIBRARIES}
)
