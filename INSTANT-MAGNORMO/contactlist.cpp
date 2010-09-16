#include "contactlist.h"

ContactList::ContactList(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    this->setLayout(layout);
    contactTree = new QTreeWidget();
    contactTree->setRootIsDecorated(false);
    contactTree->setHeaderHidden(true);
    layout->addWidget(contactTree);
    contactTree->setColumnCount(1);

    connect(contactTree, SIGNAL(itemActivated(QTreeWidgetItem*,int)), SLOT(itemActivated(QTreeWidgetItem*,int)));
}

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

void ContactList::itemActivated(QTreeWidgetItem *item, int column)
{
    if (!item)
        return;
    emit conversationInitiated(item->data(0, Qt::UserRole).toString());
}
