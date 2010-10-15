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

    qApp->setStyleSheet("QWidget { background: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #fff, stop: 1 #ccc); border-style: none; } QLineEdit { border-style: solid; }");

    MainWindow w;
    w.startAccountManager(false);
    w.show();
    return a.exec();
}
