#include "contactlist.h"
#include <QTreeView>
#include <QVBoxLayout>
#include <QIcon>

ContactList::ContactList(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    this->setLayout(layout);
    contactTree = new QTreeView();
    //contactTree->setRootIsDecorated(false);
    contactTree->setHeaderHidden(true);
    layout->addWidget(contactTree);

    connect(contactTree, SIGNAL(activated(QModelIndex)), SLOT(contactActivated(QModelIndex)));
}

void ContactList::contactActivated(const QModelIndex &index)
{
    QVariant data = index.data(Qt::UserRole);
    if (data.isValid())
        emit conversationInitiated(data.toString());
}
