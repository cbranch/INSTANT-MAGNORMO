#include "contactmodel.h"
#include "MAGNORMOBOT.h"
#include <gloox/rostermanager.h>
#include <gloox/rosteritem.h>
#include <QIcon>

/*

void ContactList::plantContact(QSharedPointer<Contact> contact)
{
    map<string,QSharedPointer<Contact> >::iterator it;
    it = contactMap.find(contact->jid);
    // If it is the end of the contactMap this means that this is a
    // new contact which is not on our list yet so we need to add
    // them
    if(it==contactMap.end()) {
        // Add a new contact to our current list
        contactMap[contact->jid]=contact;
    // Else this is an existing contact so we need to update their
    // presence to update their new status on the list
    } else {
        switch(contact->presence) {
        case Presence::Available:
            printf("%s: Available\n",contact->name.toStdString().c_str());
            break;
        case Presence::Chat:
            printf("%s: Chat\n",contact->name.toStdString().c_str());
            break;
        case Presence::Away:
            printf("%s: Away\n",contact->name.toStdString().c_str());
            break;
        case Presence::DND:
            printf("%s: DND\n",contact->name.toStdString().c_str());
            break;
        case Presence::XA:
            printf("%s: XA\n",contact->name.toStdString().c_str());
            break;
        case Presence::Unavailable:
            printf("%s: Unavailable\n",contact->name.toStdString().c_str());
            // Remove the contact from our list of online peoples
            contactMap.erase(contact->jid);
            return;
        case Presence::Probe:
            printf("%s: Probe\n",contact->name.toStdString().c_str());
            break;
        case Presence::Error:
            printf("%s: Error\n",contact->name.toStdString().c_str());
            break;
        case Presence::Invalid:
            printf("%s: Invalid\n",contact->name.toStdString().c_str());
            break;
        }
        fflush(stdout);

        // Update the contacts presence status
        it->second->presence = contact->presence;
    }

    // Clear the whole tree and redraw it with updated icons and stuff
    contactTree->clear();

    map<QString,QTreeWidgetItem*> groupMap;
    map<QString,QTreeWidgetItem*>::iterator groupIt;

    // Iterate through all of our active contacts
    for(it=contactMap.begin();it!=contactMap.end();it++) {
        QString thisGroup = it->second->group;
        groupIt = groupMap.find(thisGroup);
        if(groupIt==groupMap.end()) {
            QTreeWidgetItem *gItem = new QTreeWidgetItem(contactTree);
            gItem->setExpanded(true);
            gItem->setBackgroundColor(0,QColor(Qt::red));
            gItem->setText(0,thisGroup);
            gItem->setData(0,Qt::UserRole, QString("GROUP"));
            groupMap[thisGroup] = gItem;
        }

        groupIt = groupMap.find(thisGroup);
        QTreeWidgetItem *item = new QTreeWidgetItem((*groupIt).second);
        item->setText(0, it->second->name);
        item->setData(0, Qt::UserRole, QString::fromUtf8(it->second->jid.c_str()));
        if (it->second->presence==Presence::Available) {
            item->setIcon(0, QIcon(":/icons/user-online"));
        } else if(it->second->presence==Presence::Away) {
            item->setIcon(0, QIcon(":/icons/user-away"));
        }
    }
    contactTree->sortItems(0,Qt::AscendingOrder);
}
*/

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
                    return QIcon(":/icons/user-online");
                if (!item->highestResource())
                    return QIcon(":/icons/user-online");
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

        case Qt::UserRole:
            return jid;
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
    ContactGroup *noGroup = new ContactGroup;
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
