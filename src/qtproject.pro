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
    syntaxdotcode.cpp \
    semanticanalyzer.cpp \
    errors.cpp

HEADERS += \
    parser.hpp \
    parser_structs.h \
    parser_funcs.h \
    syntaxdotcode.h \
    nodetypes.h \
    semanticanalyzer.h \
    errors.h
