#include <QtGui/QApplication>
#include <QMessageBox>
#include "mainwindow.h"
#include "accountmanager.h"
#include <stdio.h>
#include "Contact.h"
#include "MAGNORMOBOT.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<QSharedPointer<Contact> >("QSharedPointer<Contact>");

    QCoreApplication::setOrganizationName("MAGNORMO");
    QCoreApplication::setOrganizationDomain("joshfyne.com");
    QCoreApplication::setApplicationName("INSTANT MAGNORMO");

    AccountManager *accountMgr = new AccountManager();
    if(!accountMgr->startDialog())
        return -666;

    MainWindow w;
    w.connectAccount(accountMgr->activeUser, accountMgr->activePass, accountMgr->activeServer, accountMgr->activePort);
    w.show();
    return a.exec();
}
