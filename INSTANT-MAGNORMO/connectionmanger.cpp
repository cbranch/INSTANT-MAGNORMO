#include "connectionmanger.h"

ConnectionManger::ConnectionManger(QObject *parent, ContactModel *cM) :
    QObject(parent),
    contacts(cM)
{
    connectedMapper = new QSignalMapper(this);
    disconnectedMapper = new QSignalMapper(this);
    spewMessageMapper = new QSignalMapper(this);
    openConverstionWindowMapper = new QSignalMapper(this);
}

void ConnectionManger::connectAccount(Account *acc)
{
    // Check to see if this account is already connected
    QList<MAGNORMOBOT*>::iterator itr;
    for(itr=connectionList.begin();itr<connectionList.end();++itr) {
        MAGNORMOBOT *b = *itr;
        Account *thisAccount = b->getAccount();
        if(&thisAccount==&acc) {
            qDebug("Already connected to the requested account");
            return;
        }
    }

    MAGNORMOBOT *bot = new MAGNORMOBOT(acc);

    contacts->addBot(bot);

    connect(bot, SIGNAL(connected()), connectedMapper, SLOT(map()));
    connect(bot, SIGNAL(disconnected()), disconnectedMapper, SLOT(map()));
    connect(bot, SIGNAL(spewMessage(QString,QString)), spewMessageMapper, SLOT(map()));
    connect(bot, SIGNAL(openConversationWindow(QString)), openConverstionWindowMapper, SLOT(map()));

    bot->start();


    connectionList.append(bot);
}

void ConnectionManger::disconnectAccount(Account *acc)
{
    // Find the account we want to disconnect from in the list
    QList<MAGNORMOBOT*>::iterator itr;
    for(itr=connectionList.begin();itr<connectionList.end();++itr) {
        MAGNORMOBOT *b = *itr;
        Account *thisAccount = b->getAccount();
        if(&thisAccount==&acc) {
            // Do the disconnection stuff here
        }
    }
}
