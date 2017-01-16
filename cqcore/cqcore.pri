INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/cqglobaldefs.h \
    $$PWD/cqpersoninfo.h \
    $$PWD/cqpersoninfo_p.h \
    $$PWD/cqmemberinfo.h \
    $$PWD/cqmemberinfo_p.h \
    $$PWD/cqportal.h \
    $$PWD/cqportal_p.h

SOURCES += \
    $$PWD/cqpersoninfo.cpp \
    $$PWD/cqmemberinfo.cpp \
    $$PWD/cqportal.cpp \
    $$PWD/cqportal_p.cpp

HEADERS += \
    $$PWD/cqsqlite.h \
    $$PWD/cqsqlite_p.h

SOURCES += \
    $$PWD/cqsqlite.cpp

include(cqapi/cqapi.pri)
