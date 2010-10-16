#ifndef CONNECTIONMANGER_H
#define CONNECTIONMANGER_H

#include <QObject>
#include <QList>
class MAGNORMOBOT;
class Account;
class ContactModel;

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

signals:

public slots:

};

#endif // CONNECTIONMANGER_H
