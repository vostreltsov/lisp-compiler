TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

QT += core
QT -= gui

SOURCES += \
    parser.cpp \
    main.cpp \
    lexer.cpp \
    parser_funcs.cpp \
    semanticanalyzer.cpp \
    errors.cpp \
    binarywriter.cpp

HEADERS += \
    parser.hpp \
    parser_structs.h \
    parser_funcs.h \
    nodetypes.h \
    semanticanalyzer.h \
    errors.h \
    binarywriter.h
