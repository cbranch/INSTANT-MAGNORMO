#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>

enum AccountType {
    FACEBOOK,
    GTALK,
    GAPP
};

struct Account {
    QString user;
    QString password;
    QString server;
    int port;
    AccountType type;
    bool active;

	bool operator ==(const Account &other);
};

uint qHash(const Account &key);

#endif // ACCOUNT_H

