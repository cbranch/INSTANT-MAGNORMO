#include <QtGui/QApplication>
#include <QMessageBox>
#include "mainwindow.h"
#include <stdio.h>
#include "Contact.h"
#include "MAGNORMOBOT.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<QSharedPointer<Contact> >("QSharedPointer<Contact>");

    QCoreApplication::setOrganizationName("MAGNORMO");
    QCoreApplication::setOrganizationDomain("joshfyne.com");
    QCoreApplication::setApplicationName("INSTANT_MAGNORMO");

    MainWindow w;
    w.startAccountManager(false);
    w.show();
    return a.exec();
}
