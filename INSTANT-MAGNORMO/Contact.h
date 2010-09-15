#ifndef CONTACT_H
#define CONTACT_H

#include <QMetaType>
#include <QString>

struct Contact {
    QString name;
    std::string jid;
    bool online;
};

Q_DECLARE_METATYPE(Contact)

#endif // CONTACT_H
