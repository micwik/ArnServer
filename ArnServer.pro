# CONFIG += ArnLibCompile

# Usage of internal mDNS code (no external dependency)
# CONFIG += mDnsIntern

# Usage of float as real type, default is double. Must be same in ArnLib pro-file.
# DEFINES += ARNREAL_FLOAT

QMAKE_CXXFLAGS_WARN_ON = -Wall -Wno-deprecated-declarations

QT += network
TARGET = ArnServer
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

greaterThan(QT_MAJOR_VERSION, 4) {
    ARNLIB = Arn5
} else {
    ARNLIB = Arn4
}

UI_DIR = build$$BUILD_POSTFIX
MOC_DIR = build$$BUILD_POSTFIX
RCC_DIR = build$$BUILD_POSTFIX
OBJECTS_DIR = build$$BUILD_POSTFIX
# DESTDIR = deploy$$BUILD_POSTFIX

ArnLibCompile {
    ARN += server
    ARN += discover
    include(../ArnLib/src/ArnLib.pri)
    INCLUDEPATH += $$PWD/../ArnLib/src
} else {
    #INCLUDEPATH += $$PWD/../ArnLib/src
    #QMAKE_RPATHDIR += $$OUT_PWD/../ArnLib/$$DESTDIR

    #win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ArnLib/$$DESTDIR -l$${ARNLIB}
    #else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ArnLib/$$DESTDIR -l$${ARNLIB}
    #else:unix: LIBS += -L$$OUT_PWD/../ArnLib/$$DESTDIR -l$${ARNLIB}

    LIBS += -l$${ARNLIB}
}

!mDnsIntern {
    win32:CONFIG(release, debug|release): LIBS +=  -ldns_sd
    else:win32:CONFIG(debug, debug|release): LIBS +=  -ldns_sd
    else:unix: LIBS += -ldns_sd
}

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

# INCLUDEPATH += src $$PWD/../include


### Install
win32 {
target.path = $$OUT_PWD/../lib
} else {
target.path = /usr/local/bin
}
INSTALLS += target
