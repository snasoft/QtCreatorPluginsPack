include(../common.pri)

TARGET = linecounttest

SOURCES += \
    linecounttest.cpp

#Местоположение этого DEFINES важно. Не перемещать! Не убирать в common.pri!
DEFINES += SRCDIR=\\\"$$PWD\\\"
