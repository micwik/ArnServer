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
    src/qgetopt.cpp
!win32 {
SOURCES +=  src/LinuxSignal.cpp \
    src/VcsGit.cpp
}
HEADERS += src/ServerMain.hpp \
    src/qgetopt.h
!win32 {
HEADERS += src/LinuxSignal.hpp \
    src/VcsGit.hpp
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ArnLib/release/ -lArn
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ArnLib/debug/ -lArn
else:unix: LIBS += -L$$OUT_PWD/../ArnLib/ -lArn

INCLUDEPATH += src $$PWD/.. $$PWD/../include


### Install
win32 {
target.path = $$OUT_PWD/../lib
} else {
target.path = /usr/local/house
}
INSTALLS += target

OTHER_FILES += \
    README.md
