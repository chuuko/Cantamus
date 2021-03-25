#-------------------------------------------------
#
# Project created by QtCreator 2013-11-19T12:02:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += gui multimedia widgets

TARGET = Cantamus
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    pianoroll.cpp \
    helpbrowser.cpp \
    notecontainer.cpp \
    editorplace.cpp \
    globals.cpp \
    noterect.cpp \
    waveform.cpp

HEADERS  += mainwindow.h \
    pianoroll.h \
    helpbrowser.h \
    notecontainer.h \
    handlevsq.h \
    editorplace.h \
    globals.h \
    noterect.h \
    otoparser.h \
    waveform.h

FORMS    += mainwindow.ui \
    helpbrowser.ui

RESOURCES += \
    cantamus.qrc

unix:LIBS = -L/usr/local/lib64 -lcantamus
