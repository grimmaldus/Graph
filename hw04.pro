TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += \
    graph.c \
    heap.c \
    main.c \
    stack.c

QMAKE_CFLAGS += -std=c99 -pedantic -Wall -Wextra

HEADERS += \
    graph.h \
    graph-private.h \
    heap.h \
    stack.h

OTHER_FILES += \
    01_hrany.csv \
    01_uzly.csv
