#include "imthread.h"
#include "MAGNORMOBOT.h"

IMThread::IMThread(QString username, QString password, QString server = QString(), int port = -1) :
    username(username.toStdString()),
    password(password.toStdString()),
    server(server.toStdString()),
    port(port)
{
}

void IMThread::run()
{
    bot = new MAGNORMOBOT(this);
    bot->start(username, password, server, port);
}

void IMThread::discoverContact(QSharedPointer<Contact> contact)
{
    emit contactDiscovered(contact);
}
