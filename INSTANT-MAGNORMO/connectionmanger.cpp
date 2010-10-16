#include "connectionmanger.h"
#include "mainwindow.h"
#include "MAGNORMOBOT.h"
#include "account.h"
#include "contactmodel.h"

ConnectionManger::ConnectionManger(QObject *parent, ContactModel *cM) :
    QObject(parent),
    contacts(cM)
{
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

    MainWindow *mainWin = qobject_cast<MainWindow*>(parent());

    connect(bot, SIGNAL(connected()),mainWin, SLOT(connected()));
    connect(bot, SIGNAL(disconnected()),mainWin, SLOT(disconnected()));
    connect(bot, SIGNAL(openConversationWindow(QString)), mainWin, SLOT(startConversation(QString)), Qt::BlockingQueuedConnection);

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
