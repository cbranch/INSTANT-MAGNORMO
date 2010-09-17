#include "contactmodel.h"
#include "MAGNORMOBOT.h"
#include <gloox/rostermanager.h>
#include <gloox/rosteritem.h>
#include <QIcon>

ContactModel::ContactModel(MAGNORMOBOT *contactData, QObject *parent) :
    QAbstractItemModel(parent),
    contactData(contactData)
{
    connect(contactData, SIGNAL(contactPresenceUpdated(QString)), SLOT(updateContactPresence(QString)));
    connect(contactData, SIGNAL(contactAdded(QString)), SLOT(addContact(QString)));
    connect(contactData, SIGNAL(contactUpdated(QString)), SLOT(updateContact(QString)));
    connect(contactData, SIGNAL(contactRemoved(QString)), SLOT(removeContact(QString)));
    connect(contactData, SIGNAL(contactListReceived()), SLOT(refreshContacts()));
    refreshContacts();
}

ContactModel::~ContactModel()
{
    qDeleteAll(groups);
}

Qt::ItemFlags ContactModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ContactModel::data(const QModelIndex &index, int role) const
{
    if (!index.parent().isValid()) {
        // Groups
        switch (role) {
        case Qt::DisplayRole:
            return groups.keys().at(index.row());
        }
    } else {
        // Contacts
        ContactGroup *group = reinterpret_cast<ContactGroup *>(index.internalPointer());
        QString jid = group->jids.at(index.row());
        switch (role) {
        case Qt::DisplayRole:
            return contactData->getNameFromJid(jid);
        case Qt::DecorationRole:
            {
                RosterItem *item = contactData->getRosterItemFromJid(jid);
                if (!item)
                    return QIcon(":/icons/user-offline");
                if (!item->highestResource())
                    return QIcon(":/icons/user-offline");
                switch (item->highestResource()->presence()) {
                case Presence::Available:
                case Presence::Chat:
                    return QIcon(":/icons/user-online");
                case Presence::Away:
                case Presence::DND:
                case Presence::XA:
                    return QIcon(":/icons/user-away");
                default:
                    return QIcon(":/icons/user-offline");
                }
                break;
            }

        case ContactModel::JIDRole:
            return jid;

        case ContactModel::PresenceRole:
            {
                RosterItem *item = contactData->getRosterItemFromJid(jid);
                if (!item)
                    return Presence::Unavailable;
                if (!item->highestResource())
                    return Presence::Unavailable;
                return item->highestResource()->presence();
            }
        }
    }
    return QVariant();
}

QVariant ContactModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (section) {
    case 0:
        return tr("Contact");
    }
    return QVariant();
}

int ContactModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        // Number of groups
        return groups.size();
    } else {
        if (!parent.internalPointer()) {
            // Contacts in group
            return groups.values()[parent.row()]->jids.size();
        } else {
            // Contacts
            return 0;
        }
    }
}

int ContactModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QModelIndex ContactModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        // Group
        return createIndex(row, column);
    } else {
        // Contacts
        return createIndex(row, column, groups.values()[parent.row()]);
    }
}

QModelIndex ContactModel::parent(const QModelIndex &child) const
{
    if (!child.internalPointer())
        return QModelIndex();
    ContactGroup *group = reinterpret_cast<ContactGroup *>(child.internalPointer());
    return createIndex(groups.keys().indexOf(group->groupName), 0);
}

QList<QModelIndex> ContactModel::getIndexesOfJid(QString jid)
{
    QList<QModelIndex> indexes;
    int i = 0;
    for (auto iter = groups.begin(); iter != groups.end(); iter++, i++) {
        ContactGroup *group = iter.value();
        int pos = group->jids.indexOf(jid);
        if (pos != -1) {
            indexes.append(createIndex(pos, 0, group));
        }
    }
    return indexes;
}

void ContactModel::updateContactPresence(QString jid)
{
    QList<QModelIndex> indexes = getIndexesOfJid(jid);
    foreach (QModelIndex index, indexes) {
        emit dataChanged(index, index);
    }
}

void ContactModel::addContact(QString jid)
{
    qDebug("HURR A CONTACT WAS ADDED YOU SHOULD CODE THIS");
}

void ContactModel::updateContact(QString jid)
{
    qDebug("HURR A CONTACT WAS UPDATED YOU SHOULD CODE THIS A BIT BETTER");
    QList<QModelIndex> indexes = getIndexesOfJid(jid);
    foreach (QModelIndex index, indexes) {
        emit dataChanged(index, index);
    }
}

void ContactModel::removeContact(QString jid)
{
    qDebug("HURR A CONTACT WAS REMOVED YOU SHOULD CODE THIS");
}

void ContactModel::refreshContacts()
{
    if (!contactData->j)
        return;

    beginResetModel();
    qDeleteAll(groups);
    groups.clear();
    ContactGroup *noGroup = new ContactGroup("WORTHLESS MINIONS");
    groups.insert(noGroup->groupName, noGroup);
    auto roster = contactData->j->rosterManager()->roster();
    for (auto iter = roster->begin(); iter != roster->end(); iter++) {
        QString jid = QString::fromUtf8(iter->first.c_str());
        RosterItem *item = iter->second;
        if (item->groups().empty()) {
            // Add to no-group
            noGroup->jids.append(jid);
        } else {
            auto itemgroups = item->groups();
            for (auto groupIter = itemgroups.begin(); groupIter != itemgroups.end(); groupIter++) {
                QString groupName = QString::fromUtf8(groupIter->c_str());
                auto group = groups.find(groupName);
                if (group == groups.end())
                    group = groups.insert(groupName, new ContactGroup(groupName));
                group.value()->jids.append(jid);
            }
        }
    }
    endResetModel();
}
