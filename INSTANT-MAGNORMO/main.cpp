#include <QtGui/QApplication>
#include <QMessageBox>
#include "mainwindow.h"
#include "accountdialog.h"
#include <stdio.h>
#include "imthread.h"

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
    w.imThread = new IMThread(account->username, account->password, account->server, account->port);
    w.connectThread();
    w.show();
    w.imThread->start();
    return a.exec();
}
