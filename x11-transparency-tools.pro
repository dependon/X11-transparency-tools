#-------------------------------------------------
#
# Project created by QtCreator 2021-04-09T09:42:10
#
#-------------------------------------------------

QT       += core gui x11extras widgets

CONFIG += c++11 link_pkgconfig
PKGCONFIG += xcb-ewmh x11
TARGET = x11-transparency-tools
TEMPLATE = app
QT_DEVICE_PIXEL_RATIO=1
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    trayicon.cpp

HEADERS += \
        mainwindow.h \
    setdesktop.h \
    trayicon.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
target.path = /usr/bin

desktop.path = /usr/share/applications/
desktop.files = $$PWD/install/x11-transparency-tools.desktop

icon.path= /usr/share/icons
icon.files= $$PWD/install/x11-transparency-tools.svg

!isEmpty(target.path): INSTALLS += target desktop icon

INSTALLS +=
RESOURCES += \
    qrc.qrc

