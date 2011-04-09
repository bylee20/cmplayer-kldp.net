TEMPLATE = app
TARGET = interpreter
macx {
	CONFIG -= app_bundle
}

CONFIG += debug_and_release release
QT = core

HEADERS += \
    interpreter.hpp

SOURCES += \
    interpreter.cpp
