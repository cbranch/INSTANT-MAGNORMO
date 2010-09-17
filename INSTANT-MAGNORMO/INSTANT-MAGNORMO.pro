# -------------------------------------------------
# Project created by QtCreator 2010-09-15T16:26:59
# -------------------------------------------------
QT += network \
    multimedia
QMAKE_CXXFLAGS += -std=c++0x
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
    MAGNORMOBOT.cpp \
    statuswidget.cpp \
    contactmodel.cpp
HEADERS += mainwindow.h \
    contactlist.h \
    conversationwidget.h \
    accountdialog.h \
    MAGNORMOBOT.h \
    Contact.h \
    statuswidget.h \
    contactmodel.h \
    MessageStuff.h
FORMS += accountdialog.ui \
    conversationwidget.ui
RESOURCES += resources.qrc
