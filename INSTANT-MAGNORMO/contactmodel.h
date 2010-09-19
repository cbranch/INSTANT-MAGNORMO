#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <QAbstractItemModel>
#include <QMap>
#include <QStringList>
class MAGNORMOBOT;

struct ContactGroup
{
    QString groupName;
    QStringList jids;

    ContactGroup() {}
    ContactGroup(QString name) : groupName(name) {}
};

class ContactModel : public QAbstractItemModel
{
Q_OBJECT
public:
    explicit ContactModel(MAGNORMOBOT *contactData, QObject *parent = 0);
    virtual ~ContactModel();

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    QList<QModelIndex> getIndexesOfJid(QString jid);

    enum UserRoles {
        JIDRole = Qt::UserRole,
        PresenceRole
    };

public slots:
    void updateContactPresence(QString jid);
    void addContact(QString jid);
    void updateContact(QString jid);
    void removeContact(QString jid);
    void refreshContacts();

protected:
    MAGNORMOBOT *contactData;

    QMap<QString, ContactGroup* > groups;
};

#endif // CONTACTMODEL_H