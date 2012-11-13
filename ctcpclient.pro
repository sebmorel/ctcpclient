#-------------------------------------------------
#
# Project created by QtCreator 2012-09-18T15:41:36
#
#-------------------------------------------------

QT       += network

QT       -= gui

CONFIG(debug, debug|release){
linux {
    TARGET = debug/ctcpclientd
}
windows {
    TARGET = ctcpclientd
}
    LIBS += -L../configparser/debug -lconfigparserd \
            -L../GlobalesFonctions/debug -lGlobalesFonctionsd
}
CONFIG(release, debug|release){
    TARGET = ctcpclient
    LIBS += -L../configparser/release -lconfigparser \
            -L../GlobalesFonctions/release -lGlobalesFonctions
}
TEMPLATE = lib

DEFINES += CTCPCLIENT_LIBRARY

SOURCES += ctcpclient.cpp \
    tcpfonctions.cpp \
    ctcpretour.cpp

HEADERS += ctcpclient.h\
        ctcpclient_global.h \
        tcpfonctions.h \
        ctcpretour.h

DEPENDPATH += ../configparser \
              ../GlobalesFonctions

INCLUDEPATH += ../configparser \
               ../GlobalesFonctions
