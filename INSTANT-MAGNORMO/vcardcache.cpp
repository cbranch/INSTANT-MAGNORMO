#include "vcardcache.h"
#include <QDir>
#include "MAGNORMOBOT.h"

VCardCache::VCardCache(MAGNORMOBOT *bot, QObject *parent)
	: QObject(parent),
	database(QSqlDatabase::addDatabase("QSQLITE")),
	bot(bot)
{
	database.setDatabaseName(QDir::temp().filePath(QString("magnormocards")));
	if (!database.open()) {
		qWarning("VCARD CACHE CANNOT BE OPENED YOUR TEMP DIRECTORY IS DUMB");
		return;
	}
	// If we need to fetch a vcard, we can cache and pass the vcard straight through
	connect(bot, SIGNAL(vCardReceived(QString, const gloox::VCard *)), SLOT(cacheVCard(QString, const gloox::VCard *)));
	connect(bot, SIGNAL(vCardReceived(QString, const gloox::VCard *)), SIGNAL(vCardReceived(QString, const gloox::VCard *)));
}

VCardCache::~VCardCache()
{
}

void VCardCache::fetchVCard(QString jid)
{
	bot->getVCardFromJid(jid);
}

void VCardCache::cacheVCard(QString jid, const gloox::VCard *vcard)
{
}