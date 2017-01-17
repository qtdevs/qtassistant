#-------------------------------------------------
#
# Project created by QtCreator 2017-01-07T10:10:21
#
#-------------------------------------------------

QT      += widgets
TEMPLATE = lib

TARGET   = org.qtdevs.assistant

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(cqportal/cqportal.pri)
include(sqldatas/sqldatas.pri)
include(translations/translations.pri)
include(htmlfeedback/htmlfeedback.pri)

SOURCES += main.cpp

HEADERS += \
    qtassistant.h \
    qtassistant_p.h
SOURCES += \
    qtassistant.cpp \
    manageevents.cpp \
    messageevents.cpp

OTHER_FILES += \
    addon/org.qtdevs.assistant.json

DESTDIR = "D:/CQPro/app"
