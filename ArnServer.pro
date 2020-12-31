# -------------------------------------------------
# Project created by QtCreator 2010-09-23T21:35:29
# -------------------------------------------------

CONFIG += ArnLibCompile

# Usage of internal mDNS code (no external dependency)
CONFIG += mDnsIntern

QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-deprecated-declarations

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

OTHER_FILES += \
    README.md \
    Install.md \
    config/arnserver.secrets

INCLUDEPATH += src $$PWD/../include

greaterThan(QT_MAJOR_VERSION, 4) {
    ARNLIB = Arn5
} else {
    ARNLIB = Arn4
}

ArnLibCompile {
    ARN += server
    ARN += discover
    include(../ArnLib/src/ArnLib.pri)
    INCLUDEPATH += $$PWD/../ArnLib/src
} else {
    win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ArnLib/release/ -l$${ARNLIB}
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ArnLib/debug/ -l$${ARNLIB}
    else:unix: LIBS += -L$$OUT_PWD/../ArnLib/ -l$${ARNLIB}
}

!mDnsIntern {
    win32:CONFIG(release, debug|release): LIBS +=  -ldns_sd
    else:win32:CONFIG(debug, debug|release): LIBS +=  -ldns_sd
    else:unix: LIBS += -ldns_sd
}


### Install
win32 {
target.path = $$OUT_PWD/../lib
} else {
target.path = /usr/local/bin
}
INSTALLS += target
