#include <QtGui/QApplication>
#include <QMessageBox>
#include "mainwindow.h"
#include "accountdialog.h"
#include <stdio.h>
#include "Contact.h"
#include "MAGNORMOBOT.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<QSharedPointer<Contact> >("QSharedPointer<Contact>");

    AccountDialog *account = new AccountDialog();
    switch (account->exec()) {
    case QDialog::Accepted:
        break;
    case QDialog::Rejected:
        {
        printf("You suck balls\n");
        return 0;
        }
    }
    MainWindow w;
    w.connectAccount(account->username, account->password, account->server, account->port);
    w.show();
    w.bot->start();
    return a.exec();
}
