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
    explicit IMThread(QString username, QString password);

    void run();

    std::string username;
    std::string password;
    MAGNORMOBOT *bot;

signals:
    void contactDiscovered(QSharedPointer<Contact> contact);

public:
    void discoverContact(QSharedPointer<Contact> contact);
};

#endif // IMTHREAD_H
