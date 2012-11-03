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
    attrnodes.cpp \
    syntaxdotcode.cpp

HEADERS += \
    parser.hpp \
    parser_structs.h \
    parser_funcs.h \
    attrnodes.h \
    syntaxdotcode.h \
    nodetypes.h
