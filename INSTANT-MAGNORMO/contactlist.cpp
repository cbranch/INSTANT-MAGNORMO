#include "contactlist.h"
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QIcon>
#include "Contact.h"

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
    if (contact->online) {
        item->setIcon(0, QIcon(":/icons/user-online"));
    } else {
        item->setIcon(0, QIcon(":/icons/user-offline"));
    }
}

void ContactList::itemActivated(QTreeWidgetItem *item, int column)
{
    if (!item)
        return;
    emit conversationInitiated(item->data(0, Qt::UserRole).toString());
}
