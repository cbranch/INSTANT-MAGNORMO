# -------------------------------------------------
# Project created by QtCreator 2010-09-15T16:26:59
# -------------------------------------------------
QT += network \
    multimedia
LIBS = -lgloox \
    -lgnutls \
    -lresolv \
    -pthread
TARGET = INSTANT-MAGNORMO
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    contactlist.cpp \
    conversationwidget.cpp \
    accountdialog.cpp \
    imthread.cpp \
    MAGNORMOBOT.cpp \
    statuswidget.cpp
HEADERS += mainwindow.h \
    contactlist.h \
    conversationwidget.h \
    accountdialog.h \
    MAGNORMOBOT.h \
    imthread.h \
    Contact.h \
    statuswidget.h
FORMS += accountdialog.ui
RESOURCES += resources.qrc
