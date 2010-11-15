#ifndef VCARDCACHE_H
#define VCARDCACHE_H

#include <QObject>
#include <QSqlDatabase>
#include <gloox/vcard.h>
class MAGNORMOBOT;

class VCardCache : public QObject
{
	Q_OBJECT

public:
	VCardCache(MAGNORMOBOT *bot, QObject *parent);
	~VCardCache();

public slots:
	void fetchVCard(QString jid);

signals:
	void vCardReceived(QString jid, const gloox::VCard *vcard);

protected slots:
	void cacheVCard(QString jid, const gloox::VCard *vcard);

private:
	QSqlDatabase database;
	MAGNORMOBOT *bot;
	
};

#endif // VCARDCACHE_H
