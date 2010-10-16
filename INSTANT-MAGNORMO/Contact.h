#ifndef CONTACT_H
#define CONTACT_H

#include <QMetaType>
#include <QString>
#include <QWeakPointer>
#include "MAGNORMOBOT.h"

class Contact
{
public:
	QWeakPointer<MAGNORMOBOT> conduit;
	QString jid;

	Contact() { }
	Contact(MAGNORMOBOT *conduit, QString jid) : conduit(conduit), jid(jid) { }
	bool operator ==(const Contact &other) { return conduit == other.conduit && jid == other.jid; }
};

Q_DECLARE_METATYPE(Contact)

#endif // CONTACT_H
