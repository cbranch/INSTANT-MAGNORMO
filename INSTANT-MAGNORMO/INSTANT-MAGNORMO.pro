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

  release:DESTDIR = ../Release
  debug:DESTDIR = ../Debug
  debug:QMAKE_CXXFLAGS = /Fd"..\\Debug\\"
  OBJECTS_DIR = $$DESTDIR/.obj
  MOC_DIR = $$DESTDIR/.moc
  RCC_DIR = $$DESTDIR/.rcc
  UI_DIR = $$DESTDIR/.ui
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
    accountmanagerdialog.cpp \
    account.cpp \
    connectionmanger.cpp
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
    accountmanagerdialog.h \
    connectionmanger.h
FORMS += accountdialog.ui \
    conversationwidget.ui \
    accountmanagerdialog.ui
RESOURCES += resources.qrc
