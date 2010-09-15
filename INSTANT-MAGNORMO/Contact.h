#ifndef CONTACT_H
#define CONTACT_H

#include <QString>

struct Contact {
    QString name;
    std::string jid;
    bool online;
};

#endif // CONTACT_H
