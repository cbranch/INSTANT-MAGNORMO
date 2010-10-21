#include "contactmodel.h"
#include "MAGNORMOBOT.h"
#include <gloox/rostermanager.h>
#include <gloox/rosteritem.h>
#include <QIcon>

ContactModel::ContactModel(QObject *parent) :
    QAbstractItemModel(parent)
{
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
            if(index.column()==1)
                return groups.keys().at(index.row());
        }
    } else {
        // Contacts
        ContactGroup *group = reinterpret_cast<ContactGroup *>(index.internalPointer());
        Contact contact = group->contacts.at(index.row());
        MAGNORMOBOT *bot = contact.conduit.data();
        switch (role) {
        case Qt::DisplayRole:
            if(index.column()==1)
                return bot->getNameFromJid(contact.jid);
        case Qt::DecorationRole:
            {
                RosterItem *item = bot->getRosterItemFromJid(contact.jid);
                if (!item)
                    return QIcon(":/icons/user-offline");
                if (!item->highestResource())
                    return QIcon(":/icons/user-offline");
                switch (item->highestResource()->presence()) {
                case Presence::Available:
                case Presence::Chat:
                    if(index.column()==1)
                        return QIcon(":/icons/user-online");
                    else {
                        return bot->getAccountIcon();
                    }
                case Presence::Away:
                case Presence::DND:
                case Presence::XA:
                    if(index.column()==1)
                        return QIcon(":/icons/user-away");
                    else {
                        return bot->getAccountIcon();
                    }
                default:
                    return QIcon(":/icons/user-offline");
                }
                break;
            }

        case Qt::ToolTipRole:
        case ContactModel::JIDRole:
            return contact.jid;

        case ContactModel::ContactRole:
            return QVariant::fromValue(contact);

        case ContactModel::PresenceRole:
            {
                RosterItem *item = bot->getRosterItemFromJid(contact.jid);
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
            return groups.values()[parent.row()]->contacts.size();
        } else {
            // Contacts
            return 0;
        }
    }
}

int ContactModel::columnCount(const QModelIndex &parent) const
{
    return 2;
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

void ContactModel::addBot(MAGNORMOBOT *bot)
{
	if (conduits.contains(bot)) {
		qWarning("ContactModel is already tracking %p\n", bot);
		return;
	}
	connect(bot, SIGNAL(contactPresenceUpdated(QString)), SLOT(updateContactPresence(QString)));
    connect(bot, SIGNAL(contactAdded(QString)), SLOT(addContact(QString)));
    connect(bot, SIGNAL(contactUpdated(QString)), SLOT(updateContact(QString)));
    connect(bot, SIGNAL(contactRemoved(QString)), SLOT(removeContact(QString)));
	connect(bot, SIGNAL(contactListReceived()), SLOT(receiveContactList()));
    refreshContacts(bot);
}

QList<QModelIndex> ContactModel::getIndexesOfContact(Contact contact)
{
    QList<QModelIndex> indexes;
    int i = 0;
	for (GroupIterator iter = groups.begin(); iter != groups.end(); iter++, i++) {
        ContactGroup *group = iter.value();
        int pos = group->contacts.indexOf(contact);
        while (pos != -1) {
            indexes.append(createIndex(pos, 0, group));
			pos = group->contacts.indexOf(contact, pos + 1);
        }
    }
    return indexes;
}

void ContactModel::updateContactPresence(QString jid)
{
	Contact contact(qobject_cast<MAGNORMOBOT*>(sender()), jid);
    QList<QModelIndex> indexes = getIndexesOfContact(contact);
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
	Contact contact(qobject_cast<MAGNORMOBOT*>(sender()), jid);
    QList<QModelIndex> indexes = getIndexesOfContact(contact);
    foreach (QModelIndex index, indexes) {
        emit dataChanged(index, index);
    }
}

void ContactModel::removeContact(QString jid)
{
    qDebug("HURR A CONTACT WAS REMOVED YOU SHOULD CODE THIS");
}

void ContactModel::receiveContactList()
{
    refreshContacts(qobject_cast<MAGNORMOBOT*>(sender()));
}

void ContactModel::refreshContacts(MAGNORMOBOT *bot)
{
    if (!bot->j)
        return;

    beginResetModel();
    removeContacts(bot);
	// Ensure we have the homeless group
	QString noGroupName = tr("WORTHLESS MINIONS");
	GroupIterator noGroupIter = groups.find(noGroupName);
	ContactGroup *noGroup;
	if (noGroupIter == groups.end()) {
		noGroup = new ContactGroup(noGroupName);
		noGroupIter = groups.insert(noGroupName, noGroup);
	} else {
		noGroup = noGroupIter.value();
	}

	gloox::Roster* roster = bot->j->rosterManager()->roster();
	for (gloox::Roster::iterator iter = roster->begin(); iter != roster->end(); iter++) {
        Contact contact(bot, QString::fromUtf8(iter->first.c_str()));
        RosterItem *item = iter->second;
        if (item->groups().empty()) {
            // Add to no-group
            noGroup->contacts.append(contact);
        } else {
			gloox::StringList itemgroups = item->groups();
			for (gloox::StringList::iterator groupIter = itemgroups.begin(); groupIter != itemgroups.end(); groupIter++) {
                QString groupName = QString::fromUtf8(groupIter->c_str());
				GroupIterator group = groups.find(groupName);
                if (group == groups.end())
                    group = groups.insert(groupName, new ContactGroup(groupName));
                group.value()->contacts.append(contact);
            }
        }
    }
    endResetModel();
}

void ContactModel::removeContacts(MAGNORMOBOT *bot)
{
	// Filter out contacts and groups belonging to the magnormobot
	for (GroupIterator group = groups.begin(); group != groups.end(); group++) {
		ContactGroup *cg = group.value();
		for (QList<Contact>::iterator contact = cg->contacts.begin();
				contact != cg->contacts.end(); contact++) {
			if (contact->conduit.data() == bot)
				cg->contacts.erase(contact);
		}
		if (cg->contacts.empty()) {
			delete *group;
			groups.erase(group);
		}
	}
}
