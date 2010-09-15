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
    QTreeWidgetItem *item = new QTreeWidgetItem(contactTree);
    item->setText(0, contact->name);
    item->setData(0, Qt::UserRole, QString::fromUtf8(contact->jid.c_str()));
    if (contact->presence==Presence::Available) {
        item->setIcon(0, QIcon(":/icons/user-online"));
    } else if(contact->presence==Presence::Away) {
        item->setIcon(0, QIcon(":/icons/user-away"));
    }
}

void ContactList::itemActivated(QTreeWidgetItem *item, int column)
{
    if (!item)
        return;
    emit conversationInitiated(item->data(0, Qt::UserRole).toString());
}
