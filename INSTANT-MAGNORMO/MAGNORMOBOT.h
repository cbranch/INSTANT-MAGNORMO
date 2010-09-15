#ifndef MAGNORMOBOT_H
#define MAGNORMOBOT_H

#include <gloox/messagesessionhandler.h>
#include <gloox/messageeventhandler.h>
#include <gloox/chatstatehandler.h>
#include <gloox/loghandler.h>
#include <gloox/connectionlistener.h>
#include <gloox/messagehandler.h>
#include <gloox/rosterlistener.h>
using namespace gloox;

#include <QThread>
#include <QString>
#include <QSharedPointer>
#include "Contact.h"

namespace gloox {
    class Client;
    class MessageEventFilter;
    class ChatStateFilter;
}

class MAGNORMOBOT : public QThread, MessageSessionHandler, ConnectionListener, LogHandler,
                           MessageEventHandler, MessageHandler, ChatStateHandler, RosterListener
{
    Q_OBJECT
 public:
    explicit MAGNORMOBOT(QString username, QString password, QString server, int port);

    virtual ~MAGNORMOBOT();

    void run();

    virtual void onConnect();
    virtual void onDisconnect( ConnectionError e );
    virtual bool onTLSConnect( const CertInfo& info );
    virtual void handleMessage( const Message& msg, MessageSession * /*session*/ );
    virtual void handleMessageEvent( const JID& from, MessageEventType event );
    virtual void handleChatState( const JID& from, ChatStateType state );
    virtual void handleMessageSession( MessageSession *session );
    virtual void handleLog( LogLevel level, LogArea area, const std::string& message );
    virtual void handleItemAdded (const JID &jid);
    virtual void handleItemSubscribed (const JID &jid);
    virtual void handleItemRemoved (const JID &jid);
    virtual void handleItemUpdated (const JID &jid);
    virtual void handleItemUnsubscribed (const JID &jid);
    virtual void handleRoster (const Roster &roster);
    virtual void handleRosterPresence (const RosterItem &item, const std::string &resource, Presence::PresenceType presence, const std::string &msg);
    virtual void handleSelfPresence (const RosterItem &item, const std::string &resource, Presence::PresenceType presence, const std::string &msg);
    virtual bool handleSubscriptionRequest (const JID &jid, const std::string &msg);
    virtual bool handleUnsubscriptionRequest (const JID &jid, const std::string &msg);
    virtual void handleNonrosterPresence (const Presence &presence);
    virtual void handleRosterError (const IQ &iq);

private:
    Client *j;
    MessageSession *m_session;
    MessageEventFilter *m_messageEventFilter;
    ChatStateFilter *m_chatStateFilter;


    std::string username;
    std::string password;
    std::string server;
    int port;

signals:
    void connected();
    void disconnected();
    void contactPresenceUpdate(QSharedPointer<Contact> contact);
};

#endif // MAGNORMOBOT_H
