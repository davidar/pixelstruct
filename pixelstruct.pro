QT       += core gui

QMAKE_LFLAGS += -F /System/Library/Frameworks -F /System/Library/Frameworks/IOKit.framework -F /System/Library/Frameworks/CoreFoundation.framework
LIBS += -framework IOKit -framework CoreFoundation

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pixelstruct
TEMPLATE = app

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

