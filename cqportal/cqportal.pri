QT *= sql
INCLUDEPATH += $$PWD
DEFINES += TARGET=\\\"$$TARGET\\\"

HEADERS += $$PWD/cqapi/cqapi.h \
    $$PWD/cqengine.h \
    $$PWD/cqengine_p.h \
    $$PWD/cqmodule.h \
    $$PWD/cqmodule_p.h
LIBS    += -l$$PWD/cqapi/cqapi

HEADERS += \
    $$PWD/cqglobalevents.h \
    $$PWD/cqpersoninfo.h \
    $$PWD/cqpersoninfo_p.h \
    $$PWD/cqmemberinfo.h \
    $$PWD/cqmemberinfo_p.h \
    $$PWD/cqsqlite.h \
    $$PWD/cqsqlite_p.h

SOURCES += \
    $$PWD/cqglobalevents.cpp \
    $$PWD/cqpersoninfo.cpp \
    $$PWD/cqmemberinfo.cpp \
    $$PWD/cqsqlite.cpp \
    $$PWD/cqengine.cpp \
    $$PWD/cqengine_p.cpp \
    $$PWD/cqmodule.cpp
