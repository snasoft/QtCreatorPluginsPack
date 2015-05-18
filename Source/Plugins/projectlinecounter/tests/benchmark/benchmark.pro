include(../common.pri)

TARGET = benchmarktest

SOURCES += \
    benchmarktest.cpp

#Местоположение этого DEFINES важно. Не перемещать! Не убирать в common.pri!
DEFINES += SRCDIR=\\\"$$PWD\\\"
