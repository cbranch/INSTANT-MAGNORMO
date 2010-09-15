# -------------------------------------------------
# Project created by QtCreator 2010-09-15T16:26:59
# -------------------------------------------------
QT += network \
    multimedia
LIBS = -L/usr/local/lib \
    -lgloox
TARGET = INSTANT-MAGNORMO
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    contactlist.cpp \
    conversationwidget.cpp \
    accountdialog.cpp \
    imthread.cpp \
    MAGNORMOBOT.cpp
HEADERS += mainwindow.h \
    contactlist.h \
    conversationwidget.h \
    accountdialog.h \
    MAGNORMOBOT.h \
    imthread.h \
    Contact.h
FORMS += accountdialog.ui
RESOURCES += resources.qrc
