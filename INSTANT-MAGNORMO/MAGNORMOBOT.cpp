#include "MAGNORMOBOT.h"

MAGNORMOBOT::MAGNORMOBOT(QString username, QString password, QString server = QString(), int port = -1) :
    j(0),
    username(username.toUtf8().data()),
    password(password.toUtf8().data()),
    server(server.toUtf8().data()),
    port(port)
{
}

MAGNORMOBOT::~MAGNORMOBOT()
{
}

void MAGNORMOBOT::run()
{
    JID jid( username );
    j = new Client( jid, password, port );
    if (!server.empty())
        j->setServer(server);
    j->registerConnectionListener( this );
    j->registerMessageSessionHandler( this, 0 );
    j->rosterManager()->registerRosterListener(this);
    j->disco()->setVersion( "MAGNORMOBOT", GLOOX_VERSION, "Linux" );
    j->disco()->setIdentity( "client", "bot" );
    j->disco()->addFeature( XMLNS_CHAT_STATES );
    StringList ca;
    ca.push_back( "/path/to/cacert.crt" );
    j->setCACerts( ca );

    j->logInstance().registerLogHandler( LogLevelDebug, LogAreaAll, this );


    if( j->connect( false ) )
    {
        ConnectionError ce = ConnNoError;
        while( ce == ConnNoError )
        {
            ce = j->recv();
        }
        printf( "ce: %d\n", ce );
    }

    delete( j );
}

void MAGNORMOBOT::sendMessage(QString jid, QString msg)
{
    MessageStuff *ms;
    messageStuffIterator = messageStuffMap.find(jid);
    if(messageStuffIterator==messageStuffMap.end()) {
        // We need to create a new message stuff because we havent spoken to this
        // person yet
        MessageSession *session = new MessageSession(j,JID(jid.toStdString()));
        ms = createMessageStuff(session);
    } else {
        ms = (*messageStuffIterator).second;
    }
    printf("SENDING A FUCKING MESSAGE: %s\n",msg.toStdString().c_str());
    fflush(stdout);
    ms->session->send(msg.toStdString());
    emit spewMessage(msg,jid);
}

QString MAGNORMOBOT::getNameFromJid(QString jid)
{
    auto roster = j->rosterManager()->roster();
    auto iter = roster->find(jid.toUtf8().data());
    if (iter != roster->end())
        return QString::fromUtf8(iter->second->name().c_str());
    else
        return QString();
}

RosterItem* MAGNORMOBOT::getRosterItemFromJid(QString jid)
{
    auto roster = j->rosterManager()->roster();
    auto iter = roster->find(jid.toUtf8().data());
    if (iter != roster->end())
        return iter->second;
    else
        return 0;
}

void MAGNORMOBOT::onConnect()
{
    emit connected();
}

void MAGNORMOBOT::onDisconnect( ConnectionError e )
{
    emit disconnected();
    printf( "message_test: disconnected: %d\n", e );
    if( e == ConnAuthenticationFailed )
        printf( "auth failed. reason: %d\n", j->authError() );
}

bool MAGNORMOBOT::onTLSConnect( const CertInfo& info )
{
    time_t from( info.date_from );
    time_t to( info.date_to );

    printf( "status: %d\nissuer: %s\npeer: %s\nprotocol: %s\nmac: %s\ncipher: %s\ncompression: %s\n"
            "from: %s\nto: %s\n",
            info.status, info.issuer.c_str(), info.server.c_str(),
            info.protocol.c_str(), info.mac.c_str(), info.cipher.c_str(),
            info.compression.c_str(), ctime( &from ), ctime( &to ) );
    return true;
}

void MAGNORMOBOT::handleMessage( const Message& msg, MessageSession *session )
{
    // JID of who this message was from
    QString thisJID = QString(session->target().full().c_str());

    // Test to see if the chat window needed is open or not
    map<QString,MessageStuff*>::iterator msIt;
    msIt = messageStuffMap.find(thisJID);
    MessageStuff *ms = (*msIt).second;
    if(!ms->chatWindowOpen) {
        // If its not open it
        emit openConversationWindow(QString(thisJID));
        ms->chatWindowOpen=true;
    }

    // Puts the incoming message on the relevant conversation window
    emit spewMessage(QString(msg.body().c_str()),thisJID);
}

void MAGNORMOBOT::handleMessageEvent( const JID& from, MessageEventType event )
{
    printf( "received event: %d from: %s\n", event, from.full().c_str() );
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    fflush(stdout);
}

void MAGNORMOBOT::handleChatState( const JID& from, ChatStateType state )
{
    printf( "received state: %d from: %s\n", state, from.full().c_str() );
}

void MAGNORMOBOT::handleMessageSession( MessageSession *session )
{
    //Make a new message stuff
    createMessageStuff(session);
}

MessageStuff* MAGNORMOBOT::createMessageStuff(MessageSession *session)
{
    MessageStuff *ms = new MessageStuff();
    ms->session = session;
    ms->session->registerMessageHandler(this);
    ms->eventFilter = new MessageEventFilter(session);
    ms->eventFilter->registerMessageEventHandler(this);
    ms->stateFilter = new ChatStateFilter(session);
    ms->stateFilter->registerChatStateHandler(this);
    ms->chatWindowOpen = false;

    messageStuffMap[QString(session->target().full().c_str())] = ms;

    return ms;
}

void MAGNORMOBOT::handleLog( LogLevel level, LogArea area, const std::string& message )
{
    printf("log: level: %d, area: %d, %s\n", level, area, message.c_str() );
}

void MAGNORMOBOT::handleItemAdded (const JID &jid)
{
    emit contactAdded(QString::fromUtf8(jid.full().c_str()));
}

void MAGNORMOBOT::handleItemSubscribed (const JID &jid)
{
}

void MAGNORMOBOT::handleItemRemoved (const JID &jid)
{
    emit contactRemoved(QString::fromUtf8(jid.full().c_str()));
}

void MAGNORMOBOT::handleItemUpdated (const JID &jid)
{
    emit contactUpdated(QString::fromUtf8(jid.full().c_str()));
}

void MAGNORMOBOT::handleItemUnsubscribed (const JID &jid)
{
}

void MAGNORMOBOT::handleRoster (const Roster &roster)
{
    emit contactListReceived();
}

void MAGNORMOBOT::handleRosterPresence (const RosterItem &item, const std::string &resource, Presence::PresenceType presence, const std::string &msg)
{
    std::string jid = item.jid() + resource;
    emit contactPresenceUpdated(QString::fromUtf8(jid.c_str()));
}

void MAGNORMOBOT::handleSelfPresence (const RosterItem &item, const std::string &resource, Presence::PresenceType presence, const std::string &msg)
{
}

bool MAGNORMOBOT::handleSubscriptionRequest (const JID &jid, const std::string &msg)
{
    return true;
}

bool MAGNORMOBOT::handleUnsubscriptionRequest (const JID &jid, const std::string &msg)
{
    return true;
}

void MAGNORMOBOT::handleNonrosterPresence (const Presence &presence)
{
}

void MAGNORMOBOT::handleRosterError (const IQ &iq)
{
}
