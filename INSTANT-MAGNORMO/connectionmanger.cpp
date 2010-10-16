#include "connectionmanger.h"

ConnectionManger::ConnectionManger(QObject *parent) :
    QObject(parent)
{
    signalMapper = new QSignalMapper(this);
}

void ConnectionManger::connectAccount(Account *acc)
{
    MAGNORMOBOT *bot = new MAGNORMOBOT(acc->user, acc->password, acc->server, acc->port);
    connect(bot, SIGNAL(connected()), signalMapper, SLOT(map()));
    connect(bot, SIGNAL(disconnected()), signalMapper, SLOT(map()));

    bot->start();
}

void ConnectionManger::disconnectAccount(Account *acc)
{

}
