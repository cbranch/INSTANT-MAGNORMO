#ifndef CONTACT_H
#define CONTACT_H

#include <QMetaType>
#include <QString>
#include <gloox/presence.h>

using namespace gloox;
class MAGNORMOBOT;

struct Contact {
    QString name;
	QWeakPointer<MAGNORMOBOT> conduit;
    std::string jid;
    Presence::PresenceType presence;
};

Q_DECLARE_METATYPE(Contact)

#endif // CONTACT_H
