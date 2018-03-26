QT *= sql
INCLUDEPATH += $$PWD
DEFINES += TARGET=\\\"$$TARGET\\\"

HEADERS += $$PWD/CoolQApi/CoolQLib.h
LIBS    += -l$$PWD/CoolQApi/CoolQLib

HEADERS += \
    $$PWD/CoolQInterface.h \
    $$PWD/CoolQInterface_p.h \
    $$PWD/CoolQMemberInfo.h \
    $$PWD/CoolQMemberInfo_p.h \
    $$PWD/CoolQMessageFilter.h \
    $$PWD/CoolQMessageFilter_p.h \
    $$PWD/CoolQPersonInfo.h \
    $$PWD/CoolQPersonInfo_p.h \
    $$PWD/CoolQServiceEngine.h \
    $$PWD/CoolQServiceEngine_p.h \
    $$PWD/CoolQServiceModule.h \
    $$PWD/CoolQServiceModule_p.h \
    $$PWD/CoolQSqliteService.h \
    $$PWD/CoolQSqliteService_p.h

SOURCES += \
    $$PWD/CoolQInterface.cpp \
    $$PWD/CoolQMemberInfo.cpp \
    $$PWD/CoolQMessageFilter.cpp \
    $$PWD/CoolQPersonInfo.cpp \
    $$PWD/CoolQServiceEngine.cpp \
    $$PWD/CoolQServiceEngine_p.cpp \
    $$PWD/CoolQServiceModule.cpp \
    $$PWD/CoolQServiceModule_p.cpp \
    $$PWD/CoolQSqliteService.cpp
