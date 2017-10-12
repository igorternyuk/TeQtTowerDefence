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
    projectile.cpp \
    enemy.cpp \
    redtower.cpp \
    greentower.cpp \
    browntower.cpp \
    buildingmodeicon.cpp

HEADERS  += \
    game.hpp \
    tower.hpp \
    projectile.hpp \
    browntower.hpp \
    enemy.hpp \
    greentower.hpp \
    redtower.hpp \
    buildingmodeicon.hpp

FORMS    +=

RESOURCES += \
    resources.qrc
