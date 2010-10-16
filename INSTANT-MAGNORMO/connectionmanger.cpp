#include "connectionmanger.h"

ConnectionManger::ConnectionManger(QObject *parent) :
    QObject(parent)
{
    connectedMapper = new QSignalMapper(this);
    disconnectedMapper = new QSignalMapper(this);
    contactListReceivedMapper = new QSignalMapper(this);
    contactPresenceUpdatedMapper = new QSignalMapper(this);
    contactAddedMapper = new QSignalMapper(this);
    contactUpdatedMapper = new QSignalMapper(this);
    contactRemovedMapper = new QSignalMapper(this);
    spewMessageMapper = new QSignalMapper(this);
    openConverstionWindowMapper = new QSignalMapper(this);
}

void ConnectionManger::connectAccount(Account *acc)
{
    MAGNORMOBOT *bot = new MAGNORMOBOT(acc->user, acc->password, acc->server, acc->port);
    connect(bot, SIGNAL(connected()), connectedMapper, SLOT(map()));
    connect(bot, SIGNAL(disconnected()), disconnectedMapper, SLOT(map()));
    connect(bot, SIGNAL(contactListReceived()), contactListReceivedMapper, SLOT(map()));
    connect(bot, SIGNAL(contactPresenceUpdated(QString)), contactPresenceUpdatedMapper, SLOT(map()));
    connect(bot, SIGNAL(contactAdded(QString)), contactAddedMapper, SLOT(map()));
    connect(bot, SIGNAL(contactUpdated(QString)), contactUpdatedMapper, SLOT(map()));
    connect(bot, SIGNAL(contactRemoved(QString)), contactRemovedMapper, SLOT(map()));
    connect(bot, SIGNAL(spewMessage(QString,QString)), spewMessageMapper, SLOT(map()));
    connect(bot, SIGNAL(openConversationWindow(QString)), openConverstionWindowMapper, SLOT(map()));

    bot->start();
}

void ConnectionManger::disconnectAccount(Account *acc)
{

}
