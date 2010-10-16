#ifndef CONNECTIONMANGER_H
#define CONNECTIONMANGER_H

#include <QObject>
#include <QList>
#include <QSignalMapper>
#include "MAGNORMOBOT.h"
#include "account.h"

class ConnectionManger : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionManger(QObject *parent = 0);

    void connectAccount(Account *acc);
    void disconnectAccount(Account *acc);

private:
    QList<MAGNORMOBOT *> connectionList;
    QSignalMapper *signalMapper;

signals:

public slots:

};

#endif // CONNECTIONMANGER_H
