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
SOURCES += main.cpp \
    ServerMain.cpp \
    LinuxSignal.cpp \
    qgetopt.cpp \
    VcsGit.cpp
HEADERS += ServerMain.hpp \
    LinuxSignal.hpp \
    qgetopt.h \
    VcsGit.hpp
#INCLUDEPATH += ../ArnLib
#LIBS += ../ArnLib/libArn.a
LIBS += -lArn

# ########
# INSTALL#
# ########
target.path = /usr/local/house
INSTALLS += target





