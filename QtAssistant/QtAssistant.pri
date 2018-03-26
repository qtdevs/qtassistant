INCLUDEPATH += $$PWD

include(SqlDatas/SqlDatas.pri)
include(HtmlDraw/HtmlDraw.pri)

HEADERS += \
    $$PWD/AssistantFilters.h \
    $$PWD/AssistantModule.h \
    $$PWD/AssistantModule_p.h

SOURCES += \
    $$PWD/AssistantEvents.cpp \
    $$PWD/AssistantFilters.cpp \
    $$PWD/AssistantModule.cpp \
    $$PWD/AssistantOrderes.cpp
