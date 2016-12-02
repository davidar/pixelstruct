QT += widgets core gui opengl

QMAKE_LFLAGS += -F /System/Library/Frameworks -F /System/Library/Frameworks/IOKit.framework -F /System/Library/Frameworks/CoreFoundation.framework
LIBS += -framework IOKit -framework CoreFoundation

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pixelstruct
TEMPLATE = app

INCLUDEPATH += /usr/local/include

@
CONFIG -= x86 ppc
CONFIG += x86_64
@


SOURCES += ./src/main.cpp \
    ./src/glwidget.cpp \
    ./src/camera.cpp \
    ./src/point.cpp \
    ./src/bundleparser.cpp \
    ./src/visiblepoint.cpp \
    ./src/mainwindow.cpp \
    ./src/imagelist.cpp \
    ./src/plane.cpp \
    ./src/commonplane.cpp \
    ./src/triangulation.cpp \
    ./src/photo.cpp

HEADERS  += ./src/glwidget.h \
    ./src/camera.h \
    ./src/point.h \
    ./src/bundleparser.h \
    ./src/visiblepoint.h \ 
    ./src/mainwindow.h \
    ./src/imagelist.h \
    ./src/plane.h \
    ./src/commonplane.h \
    ./src/triangulation.h \
    ./src/photo.h


#-------------------------------------------------
# CGAL LIBRARY
#-------------------------------------------------
macx: LIBS += -L$$PWD/../../../../../../opt/local/lib/ -lCGAL

INCLUDEPATH += $$PWD/../../../../../../opt/local/include
DEPENDPATH += $$PWD/../../../../../../opt/local/include
#-------------------------------------------------
# CGAL CORE LIBRARY
#-------------------------------------------------
#macx: LIBS += -L$$PWD/../../../../../../opt/local/lib/ -lCGAL_Core.11

INCLUDEPATH += $$PWD/../../../../../../opt/local/include
DEPENDPATH += $$PWD/../../../../../../opt/local/include
#-------------------------------------------------
# CGAL IMAGE IO LIBRARY
#-------------------------------------------------
macx: LIBS += -L$$PWD/../../../../../../opt/local/lib/ -lCGAL_ImageIO

INCLUDEPATH += $$PWD/../../../../../../opt/local/include
DEPENDPATH += $$PWD/../../../../../../opt/local/include
#-------------------------------------------------
# BOOST THREAD LIBRARY
#-------------------------------------------------
macx: LIBS += -L$$PWD/../../../../../../opt/local/lib/ -lboost_thread-mt

INCLUDEPATH += $$PWD/../../../../../../opt/local/include
DEPENDPATH += $$PWD/../../../../../../opt/local/include
#-------------------------------------------------
# BOOST SYSTEM LIBRARY
#-------------------------------------------------
macx: LIBS += -L$$PWD/../../../../../../opt/local/lib/ -lboost_system-mt

INCLUDEPATH += $$PWD/../../../../../../opt/local/include
DEPENDPATH += $$PWD/../../../../../../opt/local/include
#-------------------------------------------------
# GMP LIBRARY
#-------------------------------------------------
macx: LIBS += -L$$PWD/../../../../../../opt/local/lib/ -lgmp

INCLUDEPATH += $$PWD/../../../../../../opt/local/include
DEPENDPATH += $$PWD/../../../../../../opt/local/include
#-------------------------------------------------
# MPFR LIBRARY
#-------------------------------------------------
macx: LIBS += -L$$PWD/../../../../../../opt/local/lib/ -lmpfr

INCLUDEPATH += $$PWD/../../../../../../opt/local/include
DEPENDPATH += $$PWD/../../../../../../opt/local/include
