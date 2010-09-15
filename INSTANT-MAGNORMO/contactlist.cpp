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
    if(it==contactMap.end()) {
        // Add a new contact to our current list
        contactMap[contact->jid]=contact;
    } else {
        // Update the existing contacts presence value
        if(contact->presence==Presence::Unavailable) {
            contactMap.erase(it);
        } else {
            it->second->presence = contact->presence;
        }
    }

    // Clear the whole tree and redraw it
    contactTree->clear();

    for(it=contactMap.begin();it!=contactMap.end();it++) {
        QTreeWidgetItem *item = new QTreeWidgetItem(contactTree);
        item->setText(0, it->second->name);
        item->setData(0, Qt::UserRole, QString::fromUtf8(it->second->jid.c_str()));
        if (it->second->presence==Presence::Available) {
            item->setIcon(0, QIcon(":/icons/user-online"));
        } else if(it->second->presence==Presence::Away) {
            //item->setIcon(0, QIcon(":/icons/user-away"));
        }
    }
}

void ContactList::itemActivated(QTreeWidgetItem *item, int column)
{
    if (!item)
        return;
    emit conversationInitiated(item->data(0, Qt::UserRole).toString());
}
