!exists($$PWD/libkode/common) {
    error("Please do git submodule update --init --recursive")
}

include(libkode/libkode.pri)

INCLUDEPATH += libkode
INCLUDEPATH += libkode/common
INCLUDEPATH += libkode/schema
INCLUDEPATH += libkode/code_generation

SOURCES += \
    kodemain.cpp
