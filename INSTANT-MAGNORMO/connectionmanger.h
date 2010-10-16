#ifndef CONNECTIONMANGER_H
#define CONNECTIONMANGER_H

#include <QObject>
#include <QList>
#include <QSignalMapper>
#include "MAGNORMOBOT.h"
#include "account.h"
#include "contactmodel.h"

class ConnectionManger : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionManger(QObject *parent = 0, ContactModel *contacts = 0);

    void connectAccount(Account *acc);
    void disconnectAccount(Account *acc);

    QSignalMapper *connectedMapper;
    QSignalMapper *disconnectedMapper;
    QSignalMapper *spewMessageMapper;
    QSignalMapper *openConverstionWindowMapper;

private:
    QList<MAGNORMOBOT *> connectionList;
    ContactModel *contacts;

signals:

public slots:

};

#endif // CONNECTIONMANGER_H
