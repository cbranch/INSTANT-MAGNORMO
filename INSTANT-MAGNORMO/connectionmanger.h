#ifndef CONNECTIONMANGER_H
#define CONNECTIONMANGER_H

#include <QObject>
#include <QList>
#include "MAGNORMOBOT.h"
#include "account.h"
#include "contactmodel.h"
#include "mainwindow.h"

class ConnectionManger : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionManger(QObject *parent = 0, ContactModel *contacts = 0);

    void connectAccount(Account *acc);
    void disconnectAccount(Account *acc);

private:
    QList<MAGNORMOBOT *> connectionList;
    ContactModel *contacts;
    MainWindow *mainWin;

signals:

public slots:

};

#endif // CONNECTIONMANGER_H
