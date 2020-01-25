#-------------------------------------------------
#
# Project created by QtCreator 2018-09-15T18:59:57
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zmqPix
TEMPLATE = app
DESTDIR = ../zmqApp

#RC_ICONS += images/modbus.ico
#Output
UI_DIR      = uic
MOC_DIR     = moc
OBJECTS_DIR = obj

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    zsub.cpp \
    zpub.cpp \
    zThread.cpp

HEADERS += \
        mainwindow.h \
    zThread.h \
    zsub.h \
    zpub.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    resource.qrc

#------------------------------------------------
#SET INCLUDEPATH & LIBS when not using PKG-CONFIG
macx: {
#    INCLUDEPATH += /usr/local/include
#    LIBS += -L"/usr/local/lib" -lzmq
#    ICON = modbus.icns
PKG_CONFIG = /usr/local/bin/pkg-config
    }
unix:!macx {
#    LIBS += -lzmq
PKG_CONFIG = /usr/bin/pkg-config
    }

#------------------------------------
#USING PKG-CONFIG for include and lib
QT_CONFIG -= no-pkg-config
CONFIG  += link_pkgconfig
PKGCONFIG += libzmq


DISTFILES +=
