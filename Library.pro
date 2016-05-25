#-------------------------------------------------
#
# Project created by QtCreator 2016-05-17T12:29:25
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TRANSLATIONS += ru.ts

TARGET = Library
TEMPLATE = app

SOURCES += main.cpp\
    JsonParser.cpp \
    ReportGenerator.cpp

HEADERS  += \
    ReportGenerator.h \
    JsonParser.h

include(BLL/BLL.pri)
include(GUI/GUI.pri)

INCLUDEPATH += \
    $$PWD/..\
    $$PWD/BLL \
    $$PWD/GUI

RESOURCES += \
    Resource.qrc
