#include <QtGui/QApplication>
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"
#include <stdio.h>
#include "MAGNORMOBOT.h"
#include "Contact.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	qRegisterMetaType<Contact>("Contact");
    qRegisterMetaType<QSharedPointer<Contact> >("QSharedPointer<Contact>");

    QCoreApplication::setOrganizationName("MAGNORMO");
    QCoreApplication::setOrganizationDomain("joshfyne.com");
    QCoreApplication::setApplicationName("INSTANT_MAGNORMO");


    MainWindow w;
    w.startAccountManager(false);
    w.show();
    return a.exec();
}
