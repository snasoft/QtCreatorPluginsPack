include(../common.pri)

TARGET = linecounttest

SOURCES += \
    linecounttest.cpp

#�������������� ����� DEFINES �����. �� ����������! �� ������� � common.pri!
DEFINES += SRCDIR=\\\"$$PWD\\\"
