FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
SET(QT_USE_QTOPENGL ON)
INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
ADD_DEFINITIONS(${QT_DEFINITIONS} -Wall -O2)

SET(SOURCES
	main.cpp
	glwidget.cpp
	camera.cpp
	point.cpp
	bundleparser.cpp
	visiblepoint.cpp
	mainwindow.cpp
	imagelist.cpp
)
SET(HEADERS
	glwidget.h
	camera.h
	point.h
	bundleparser.h
	visiblepoint.h
	mainwindow.h
	imagelist.h
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
)
