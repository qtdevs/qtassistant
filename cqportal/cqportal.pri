QT *= sql
INCLUDEPATH += $$PWD
DEFINES += TARGET=\\\"$$TARGET\\\"

HEADERS += $$PWD/cqapi/cqapi.h
LIBS    += -l$$PWD/cqapi/cqapi

HEADERS += \
    $$PWD/cqglobalevents.h \
    $$PWD/cqpersoninfo.h \
    $$PWD/cqpersoninfo_p.h \
    $$PWD/cqmemberinfo.h \
    $$PWD/cqmemberinfo_p.h \
    $$PWD/cqportal.h \
    $$PWD/cqportal_p.h \
    $$PWD/cqsqlite.h \
    $$PWD/cqsqlite_p.h

SOURCES += \
    $$PWD/cqglobalevents.cpp \
    $$PWD/cqpersoninfo.cpp \
    $$PWD/cqmemberinfo.cpp \
    $$PWD/cqportal.cpp \
    $$PWD/cqportal_p.cpp \
    $$PWD/cqsqlite.cpp
