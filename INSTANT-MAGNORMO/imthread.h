#ifndef IMTHREAD_H
#define IMTHREAD_H

#include <QThread>
#include <QString>
#include <QSharedPointer>
#include "Contact.h"

class MAGNORMOBOT;

class IMThread : public QThread
{
    Q_OBJECT

public:
    explicit IMThread(QString username, QString password, QString server, int port);

    void run();

    std::string username;
    std::string password;
    std::string server;
    int port;
    MAGNORMOBOT *bot;

signals:
    void contactDiscovered(QSharedPointer<Contact> contact);

public:
    void discoverContact(QSharedPointer<Contact> contact);
};

#endif // IMTHREAD_H
