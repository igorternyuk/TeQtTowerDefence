#-------------------------------------------------
#
# Project created by QtCreator 2017-10-09T17:37:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TeQtTowerDefence
TEMPLATE = app
CONFIG += c++1z

SOURCES += main.cpp\
    tower.cpp \
    game.cpp \
    enemy.cpp \
    buildingmodeicon.cpp \
    missile.cpp

HEADERS  += \
    game.hpp \
    tower.hpp \
    enemy.hpp \
    buildingmodeicon.hpp \
    missile.hpp \
    texItem.hpp

FORMS    +=

RESOURCES += \
    resources.qrc
