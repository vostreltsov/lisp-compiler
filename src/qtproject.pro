TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

QT += core
QT -= gui

SOURCES += \
    parser.cpp \
    main.cpp \
    lexer.cpp \
    dotcode.cpp \
    parser_funcs.cpp

HEADERS += \
    parser.hpp \
    dotcode.h \
    parser_structs.h \
    parser_funcs.h
