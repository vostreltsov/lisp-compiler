TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS_DEBUG += -std=c99
QMAKE_CFLAGS_RELEASE += -std=c99

SOURCES += \
    parser.tab.c \
    main.c \
    lexer.c \
    dotcode.c \
    parser_funcs.c

HEADERS += \
    parser.tab.h \
    dotcode.h \
    parser_structs.h \
    parser_funcs.h
