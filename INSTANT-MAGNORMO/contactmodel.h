#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <QAbstractItemModel>
#include <QMap>
#include <QStringList>
#include <QList>
#include <QWeakPointer>
#include "MAGNORMOBOT.h"
#include "Contact.h"
#include "account.h"

struct ContactGroup
{
    QString groupName;
    QList<Contact> contacts;

    ContactGroup() {}
    ContactGroup(QString name) : groupName(name) {}
};

class ContactModel : public QAbstractItemModel
{
Q_OBJECT
public:
    explicit ContactModel(QObject *parent = 0);
    virtual ~ContactModel();

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    QList<QModelIndex> getIndexesOfContact(Contact contact);

    enum UserRoles {
        JIDRole = Qt::UserRole,
		ContactRole,
        PresenceRole
    };

	void addBot(MAGNORMOBOT *bot);

public slots:
    void updateContactPresence(QString jid);
    void addContact(QString jid);
    void updateContact(QString jid);
    void removeContact(QString jid);
	void receiveContactList();
    void refreshContacts(MAGNORMOBOT *bot);
	void removeContacts(MAGNORMOBOT *bot);

protected slots:
	void botDisconnected();
	void botDestroyed();

protected:
    QList<MAGNORMOBOT*> conduits;

    QMap<QString, ContactGroup* > groups;
	typedef QMap<QString, ContactGroup* >::iterator GroupIterator;
};

#endif // CONTACTMODEL_H
