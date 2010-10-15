# -------------------------------------------------
# Project created by QtCreator 2010-09-15T16:26:59
# -------------------------------------------------
QT += network \
    multimedia
LIBS = -lgloox
!win32:LIBS += -lgnutls -lresolv -pthread
win32 {
  LIBS += -lzlibwapi
  debug:LIBS += -L../Debug
  release:LIBS += -L../Release
  DEFINES += ZLIB_WINAPI GLOOX_IMPORTS
  INCLUDEPATH += ../Include
}

TARGET = INSTANT-MAGNORMO
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    contactlist.cpp \
    conversationwidget.cpp \
    accountdialog.cpp \
    MAGNORMOBOT.cpp \
    statuswidget.cpp \
    contactmodel.cpp \
    contactsortfilterproxymodel.cpp \
    accountmanagerdialog.cpp
HEADERS += mainwindow.h \
    contactlist.h \
    conversationwidget.h \
    accountdialog.h \
    MAGNORMOBOT.h \
    Contact.h \
    statuswidget.h \
    contactmodel.h \
    MessageStuff.h \
    contactsortfilterproxymodel.h \
    account.h \
    accountmanagerdialog.h
FORMS += accountdialog.ui \
    conversationwidget.ui \
    accountmanagerdialog.ui
RESOURCES += resources.qrc
