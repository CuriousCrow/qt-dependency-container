QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_injecttest.cpp \
    testbeans.cpp

include(../ioc/ioc.pri)

HEADERS += \
    testbeans.h
