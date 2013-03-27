# -------------------------------------------------
# Project created by QtCreator 2010-09-23T21:35:29
# -------------------------------------------------
QT += network
TARGET = ArnServer
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
OBJECTS_DIR = tmp
MOC_DIR = tmp
SOURCES += src/main.cpp \
    src/ServerMain.cpp \
    src/LinuxSignal.cpp \
    src/qgetopt.cpp \
    src/VcsGit.cpp
HEADERS += src/ServerMain.hpp \
    src/LinuxSignal.hpp \
    src/qgetopt.h \
    src/VcsGit.hpp
#INCLUDEPATH += ../ArnLib
#LIBS += ../ArnLib/libArn.a
LIBS += -lArn

# ########
# INSTALL#
# ########
target.path = /usr/local/house
INSTALLS += target
