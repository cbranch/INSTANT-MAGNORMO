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
    layout->addWidget(contactTree);
    contactTree->setColumnCount(1);
}

void ContactList::plantContact(QSharedPointer<Contact> contact)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(contactTree);
    item->setText(0, contact->name);
    if (contact->online) {
        item->setIcon(0, QIcon(":/icons/user-online"));
    } else {
        item->setIcon(0, QIcon(":/icons/user-offline"));
    }
}
