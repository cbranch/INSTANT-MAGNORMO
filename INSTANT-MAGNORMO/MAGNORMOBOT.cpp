#include "MAGNORMOBOT.h"

MAGNORMOBOT::MAGNORMOBOT(QString username, QString password, QString server = QString(), int port = -1) :
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
  printf( "type: %d, subject: %s, message: %s, thread id: %s\n", msg.subtype(),
          msg.subject().c_str(), msg.body().c_str(), msg.thread().c_str() );
  printf("##############################################################\n");
  fflush(stdout);
  emit spewMessage(QString(msg.body().c_str()),QString(session->target().full().c_str()));

  std::string re = "MAGNORMO IS INDIFFERENT TO YOUR OPINIONS.";
  std::string sub;
  if( !msg.subject().empty() )
    sub = "Re: " +  msg.subject();
  session->send( re, sub );
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
    printf( "Got new session\n\n");
    //Make a new message stuff
    MessageStuff *ms = new MessageStuff();
    ms->session = session;
    ms->session->registerMessageHandler(this);
    ms->eventFilter = new MessageEventFilter(session);
    ms->eventFilter->registerMessageEventHandler(this);
    ms->stateFilter = new ChatStateFilter(session);
    ms->stateFilter->registerChatStateHandler(this);

    messageStuffMap[session->target().full().c_str()] = ms;
}

void MAGNORMOBOT::handleLog( LogLevel level, LogArea area, const std::string& message )
{
  printf("log: level: %d, area: %d, %s\n", level, area, message.c_str() );
}

void MAGNORMOBOT::handleItemAdded (const JID &jid)
{
}

void MAGNORMOBOT::handleItemSubscribed (const JID &jid)
{
}

void MAGNORMOBOT::handleItemRemoved (const JID &jid)
{
}

void MAGNORMOBOT::handleItemUpdated (const JID &jid)
{
}

void MAGNORMOBOT::handleItemUnsubscribed (const JID &jid)
{
}

void MAGNORMOBOT::handleRoster (const Roster &roster)
{
}

void MAGNORMOBOT::handleRosterPresence (const RosterItem &item, const std::string &resource, Presence::PresenceType presence, const std::string &msg)
{
    QSharedPointer<Contact> x(new Contact);
    x->jid = item.jid();
    x->name = QString::fromUtf8(item.name().c_str());
    if(item.groups().size()==0) {
        x->group = QString("WORTHLESS MINIONS");
    } else {
        x->group = QString((*item.groups().begin()).c_str());
    }
    x->presence = presence;
    emit contactPresenceUpdate(x);
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
