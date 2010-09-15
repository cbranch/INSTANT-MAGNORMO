#include "MAGNORMOBOT.h"
#include <gloox/rostermanager.h>
#include <gloox/rosteritem.h>
#include "Contact.h"
#include <QSharedPointer>

MAGNORMOBOT::MAGNORMOBOT(QString username, QString password, QString server = QString(), int port = -1) :
    m_session(0),
    m_messageEventFilter(0),
    m_chatStateFilter(0),
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
    Roster *roster = j->rosterManager()->roster();
    for (Roster::const_iterator i = roster->begin(); i != roster->end(); i++) {
        QSharedPointer<Contact> x(new Contact);
        x->jid = i->first;
        RosterItem *item = i->second;
        x->name = QString::fromUtf8(item->name().c_str());
        x->online = item->online();
        emit contactDiscovered(x);
    }
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

void MAGNORMOBOT::handleMessage( const Message& msg, MessageSession * /*session*/ )
{
  printf( "type: %d, subject: %s, message: %s, thread id: %s\n", msg.subtype(),
          msg.subject().c_str(), msg.body().c_str(), msg.thread().c_str() );

  std::string re = "You said:\n" + msg.body() + "\nMAGNORMO DISAGREES.\n";
  std::string sub;
  if( !msg.subject().empty() )
    sub = "Re: " +  msg.subject();

  m_messageEventFilter->raiseMessageEvent( MessageEventDisplayed );
#if defined( WIN32 ) || defined( _WIN32 )
  Sleep( 1000 );
#else
  sleep( 1 );
#endif
  m_messageEventFilter->raiseMessageEvent( MessageEventComposing );
  m_chatStateFilter->setChatState( ChatStateComposing );
#if defined( WIN32 ) || defined( _WIN32 )
  Sleep( 2000 );
#else
  sleep( 2 );
#endif
  m_session->send( re, sub );

  if( msg.body() == "quit" )
    j->disconnect();
}

void MAGNORMOBOT::handleMessageEvent( const JID& from, MessageEventType event )
{
  printf( "received event: %d from: %s\n", event, from.full().c_str() );
}

void MAGNORMOBOT::handleChatState( const JID& from, ChatStateType state )
{
  printf( "received state: %d from: %s\n", state, from.full().c_str() );
}

void MAGNORMOBOT::handleMessageSession( MessageSession *session )
{
  printf( "got new session\n");
  // this example can handle only one session. so we get rid of the old session
  j->disposeMessageSession( m_session );
  m_session = session;
  m_session->registerMessageHandler( this );
  m_messageEventFilter = new MessageEventFilter( m_session );
  m_messageEventFilter->registerMessageEventHandler( this );
  m_chatStateFilter = new ChatStateFilter( m_session );
  m_chatStateFilter->registerChatStateHandler( this );
}

void MAGNORMOBOT::handleLog( LogLevel level, LogArea area, const std::string& message )
{
  printf("log: level: %d, area: %d, %s\n", level, area, message.c_str() );
}
