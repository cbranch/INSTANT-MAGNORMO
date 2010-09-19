#include "contactlist.h"
#include <QTreeView>
#include <QVBoxLayout>
#include <QIcon>
#include "contactsortfilterproxymodel.h"

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

void ContactList::setModel(QAbstractItemModel *model)
{
    ContactSortFilterProxyModel *proxyModel = new ContactSortFilterProxyModel(contactTree);
    proxyModel->setDynamicSortFilter(true);
    proxyModel->setSourceModel(model);
    contactTree->setModel(proxyModel);
    contactTree->setSortingEnabled(true);
    contactTree->sortByColumn(0, Qt::AscendingOrder);
    connect(model, SIGNAL(modelReset()), contactTree, SLOT(expandAll()));
}

void ContactList::contactActivated(const QModelIndex &index)
{
    QVariant data = index.data(Qt::UserRole);
    if (data.isValid())
        emit conversationInitiated(data.toString());
}
