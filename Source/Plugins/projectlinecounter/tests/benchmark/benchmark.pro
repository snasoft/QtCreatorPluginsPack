include(../common.pri)

TARGET = benchmarktest

SOURCES += \
    benchmarktest.cpp

#�������������� ����� DEFINES �����. �� ����������! �� ������� � common.pri!
DEFINES += SRCDIR=\\\"$$PWD\\\"
