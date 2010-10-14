#include <QtGui/QApplication>
#include <QMessageBox>
#include "mainwindow.h"
#include "accountmanagerdialog.h"
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

    accountmanagerdialog *managerDialog = new accountmanagerdialog();
    switch(managerDialog->exec()) {
        case QDialog::Accepted:
        {
            QList<Account*> activeAccounts = managerDialog->getActiveAccounts();
            // For now we will select the first active one as we dont have multiple account support yet
            if(activeAccounts.size()==0) {
                printf("You suck balls\n");
                return -666;
            }
            Account* acc = activeAccounts.first();

            // Kick off the main window
            MainWindow w;
            w.connectAccount(acc->user, acc->password, acc->server, acc->port);
            w.show();
            return a.exec();
        }
        case QDialog::Rejected:
        {
            printf("You suck balls\n");
            return -666;
        }
    }

}
