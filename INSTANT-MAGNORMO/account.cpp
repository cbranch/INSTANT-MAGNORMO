#include "account.h"
#include <QHash>

bool Account::operator ==(const Account &other)
{
	return user == other.user
		&& password == other.password
		&& server == other.server
		&& port == other.port
		&& type == other.type
		&& active == other.active;
}

uint qHash(const Account &key)
{
	QString x;
	x += key.user;
	x += key.password;
	x += key.server;
	x += key.port;
	x += (char)key.type;
	x += (char)key.active;
	return qHash(x);
}