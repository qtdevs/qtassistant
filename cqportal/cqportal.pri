QT *= sql
INCLUDEPATH += $$PWD
DEFINES += TARGET=\\\"$$TARGET\\\"

HEADERS += $$PWD/CqApi/CqLib.h
LIBS    += -l$$PWD/CqApi/CqLib

HEADERS += \
    $$PWD/CqInterface.h \
    $$PWD/CqInterface_p.h \
    $$PWD/CqMemberInfo.h \
    $$PWD/CqMemberInfo_p.h \
    $$PWD/CqMessageFilter.h \
    $$PWD/CqMessageFilter_p.h \
    $$PWD/CqPersonInfo.h \
    $$PWD/CqPersonInfo_p.h \
    $$PWD/CqServiceModule.h \
    $$PWD/CqServiceModule_p.h \
    $$PWD/CqServicePortal.h \
    $$PWD/CqServicePortal_p.h \
    $$PWD/CqSqliteService.h \
    $$PWD/CqSqliteService_p.h

SOURCES += \
    $$PWD/CqInterface.cpp \
    $$PWD/CqMemberInfo.cpp \
    $$PWD/CqMessageFilter.cpp \
    $$PWD/CqPersonInfo.cpp \
    $$PWD/CqServiceModule.cpp \
    $$PWD/CqServiceModule_p.cpp \
    $$PWD/CqServicePortal.cpp \
    $$PWD/CqServicePortal_p.cpp \
    $$PWD/CqSqliteService.cpp
