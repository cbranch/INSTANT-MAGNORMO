#ifndef MAGNORMOBOT_H
#define MAGNORMOBOT_H

#include <gloox/client.h>
#include <gloox/messagesessionhandler.h>
#include <gloox/messageeventhandler.h>
#include <gloox/messageeventfilter.h>
#include <gloox/chatstatehandler.h>
#include <gloox/chatstatefilter.h>
#include <gloox/connectionlistener.h>
#include <gloox/disco.h>
#include <gloox/message.h>
#include <gloox/gloox.h>
#include <gloox/lastactivity.h>
#include <gloox/loghandler.h>
#include <gloox/logsink.h>
#include <gloox/connectiontcpclient.h>
#include <gloox/connectionsocks5proxy.h>
#include <gloox/connectionhttpproxy.h>
#include <gloox/messagehandler.h>
using namespace gloox;

#ifndef _WIN32
# include <unistd.h>
#endif

#include <stdio.h>
#include <string>

#include <cstdio> // [s]print[f]

#if defined( WIN32 ) || defined( _WIN32 )
# include <windows.h>
#endif

#include <QThread>
#include <QString>
#include <QSharedPointer>
#include "Contact.h"

class MAGNORMOBOT : public QThread, MessageSessionHandler, ConnectionListener, LogHandler,
                           MessageEventHandler, MessageHandler, ChatStateHandler
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
    void contactDiscovered(QSharedPointer<Contact> contact);
};

#endif // MAGNORMOBOT_H
