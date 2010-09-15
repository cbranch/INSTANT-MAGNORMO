#include "contactlist.h"
#include <QTreeWidget>
#include <QVBoxLayout>

ContactList::ContactList(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    this->setLayout(layout);
    contactTree = new QTreeWidget();
    layout->addWidget(contactTree);
    contactTree->setColumnCount(1);
}
