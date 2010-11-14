#include "MAGNORMOBOT.h"
#include <QtGlobal>

MAGNORMOBOT::MAGNORMOBOT(Account *acc) :
    j(0),
    username(acc->user.toUtf8().data()),
    password(acc->password.toUtf8().data()),
    server(acc->server.toUtf8().data()),
    port(acc->port)
{
    this->acc = acc;
}

MAGNORMOBOT::~MAGNORMOBOT()
{
	delete j;
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
	vCardManager = new VCardManager(j);
    j->connect(true);
}

JID MAGNORMOBOT::stringToJid(QString jid)
{
	return JID(jid.toStdString());
}

QString MAGNORMOBOT::jidToString(const JID &jid)
{
	return QString::fromUtf8(jid.bare().c_str());
}

void MAGNORMOBOT::sendMessage(QString jid, QString msg)
{
    MessageStuff *ms;
    messageStuffIterator = messageStuffMap.find(jid);
    if(messageStuffIterator==messageStuffMap.end()) {
        // We need to create a new message stuff because we havent spoken to this
        // person yet
		JID jid2(jid.toStdString());
        MessageSession *session = new MessageSession(j,jid2);
        ms = createMessageStuff(session);
    } else {
        ms = (*messageStuffIterator).second;
    }
    qDebug("SENDING A FUCKING MESSAGE: %s\n",msg.toStdString().c_str());
    fflush(stdout);
    ms->session->send(msg.toStdString());
    QString showMsg = "<b><font color=\"blue\">I DECLARED:</font></b> " + msg;
    emit spewMessage(showMsg,jid);
}

void MAGNORMOBOT::dissapearedWindow(QString jid)
{
    messageStuffIterator = messageStuffMap.find(jid);
	if (messageStuffIterator != messageStuffMap.end()) {
		MessageStuff *ms;
		ms = (*messageStuffIterator).second;
		ms->chatWindowOpen = false;
	} else
		qWarning("MAGNORMOBOT::dissapearedWindow could not find jid");
}

QString MAGNORMOBOT::getNameFromJid(QString jid)
{
	gloox::Roster* roster = j->rosterManager()->roster();
	gloox::Roster::iterator iter = roster->find(jid.toUtf8().data());
    if (iter != roster->end()) {
        QString name = QString::fromUtf8(iter->second->name().c_str());
        if(name.length()==0)
            return jid;
        else
            return name;
    } else
        return jid;
}

RosterItem* MAGNORMOBOT::getRosterItemFromJid(QString jid)
{
    gloox::Roster* roster = j->rosterManager()->roster();
    gloox::Roster::iterator iter = roster->find(jid.toUtf8().data());
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
    qDebug( "message_test: disconnected: %d\n", e );
    if( e == ConnAuthenticationFailed )
        qDebug( "auth failed. reason: %d\n", j->authError() );
}

bool MAGNORMOBOT::onTLSConnect( const CertInfo& info )
{
    time_t from( info.date_from );
    time_t to( info.date_to );

    qDebug( "status: %d\nissuer: %s\npeer: %s\nprotocol: %s\nmac: %s\ncipher: %s\ncompression: %s\n"
            "from: %s\nto: %s\n",
            info.status, info.issuer.c_str(), info.server.c_str(),
            info.protocol.c_str(), info.mac.c_str(), info.cipher.c_str(),
            info.compression.c_str(), ctime( &from ), ctime( &to ) );
    return true;
}

void MAGNORMOBOT::handleMessage( const Message& msg, MessageSession *session )
{
    // JID of who this message was from
    QString thisJID = QString(session->target().bare().c_str());

    // Test to see if the chat window needed is open or not
    map<QString,MessageStuff*>::iterator msIt;
    msIt = messageStuffMap.find(thisJID);
	if (msIt == messageStuffMap.end()) {
		// Not in there yet
		qFatal("urrr how did we get here");
	}
    MessageStuff *ms = (*msIt).second;
    if(!ms->chatWindowOpen) {
        // If its not open it
		Contact contact(this, thisJID);
        emit openConversationWindow(contact);
        ms->chatWindowOpen=true;
    }

    // Puts the incoming message on the relevant conversation window
    QString showMsg = "<b><font color=\"red\">MINION SAYS:</font></b> " + QString(msg.body().c_str());
    emit spewMessage(showMsg,thisJID);
}

void MAGNORMOBOT::handleMessageEvent( const JID& from, MessageEventType event )
{
    qDebug( "received event: %d from: %s\n", event, from.full().c_str() );
    qDebug("++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    fflush(stdout);
}

void MAGNORMOBOT::handleChatState( const JID& from, ChatStateType state )
{
    qDebug( "received state: %d from: %s\n", state, from.full().c_str() );
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

    messageStuffMap[QString(session->target().bare().c_str())] = ms;

    return ms;
}

void MAGNORMOBOT::handleLog( LogLevel level, LogArea area, const std::string& message )
{
    qDebug("log: level: %d, area: %d, %s\n", level, area, message.c_str() );
}

void MAGNORMOBOT::handleItemAdded (const JID &jid)
{
    emit contactAdded(QString::fromUtf8(jid.bare().c_str()));
}

void MAGNORMOBOT::handleItemSubscribed (const JID &jid)
{
}

void MAGNORMOBOT::handleItemRemoved (const JID &jid)
{
    emit contactRemoved(QString::fromUtf8(jid.bare().c_str()));
}

void MAGNORMOBOT::handleItemUpdated (const JID &jid)
{
    emit contactUpdated(QString::fromUtf8(jid.bare().c_str()));
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

void MAGNORMOBOT::updateChatState(QString jid, ChatStateType state)
{
	messageStuffMap[jid]->stateFilter->setChatState(state);
}

Account* MAGNORMOBOT::getAccount() { return acc; }

QIcon MAGNORMOBOT::getAccountIcon()
{
    switch(acc->type) {
    case FACEBOOK:
        return QIcon(":/icons/facebook");
    case GTALK:
        return QIcon(":/icons/gtalk");
    case GAPP:
        return QIcon(":/icons/gtalk");
    }
    return QIcon();
}

void MAGNORMOBOT::handleVCard( const JID& jid, const VCard* vcard )
{
	emit vCardReceived(jidToString(jid), vcard);
}

void MAGNORMOBOT::handleVCardResult(VCardContext context, const JID& jid, StanzaError se)
{
	if(se != StanzaErrorUndefined) {
		qDebug("%s VCard for user %s failed with error %u", context == gloox::VCardHandler::StoreVCard ? "Storing" : "Fetching", jid.bare().c_str(), se);
    }
}

void MAGNORMOBOT::getVCardFromJid(QString jid)
{
	vCardManager->fetchVCard(stringToJid(jid), this);
}

void MAGNORMOBOT::disconnect()
{
    j->disconnect();
}
