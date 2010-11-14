#include "contactlist.h"
#include <QTreeView>
#include <QVBoxLayout>
#include <QIcon>
#include "contactmodel.h"
#include "contactsortfilterproxymodel.h"

ContactList::ContactList(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
	layout->setMargin(0);
    this->setLayout(layout);
    contactTree = new QTreeView();
    contactTree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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
	contactTree->setIndentation(0);
    contactTree->sortByColumn(0, Qt::AscendingOrder);
	contactTree->setColumnWidth(0,18);
	contactTree->setRootIsDecorated(false);
    connect(model, SIGNAL(modelReset()), contactTree, SLOT(expandAll()));
	connect(model, SIGNAL(modelReset()), SLOT(setGroupProperties()));
	connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)), SLOT(setGroupProperties(QModelIndex,int,int)));
}

void ContactList::contactActivated(const QModelIndex &index)
{
	QVariant data = index.data(ContactModel::ContactRole);
    if (data.isValid())
		emit conversationInitiated(data.value<Contact>());
}

void ContactList::setGroupProperties()
{
	QAbstractItemModel *model = contactTree->model();
	QModelIndex root = contactTree->rootIndex();
	int count = model->rowCount(root);
	for (int i = 0; i <= count; i++)
		contactTree->setFirstColumnSpanned(i, root, true);
}

void ContactList::setGroupProperties(const QModelIndex &parent, int start, int end)
{
	if (!parent.isValid())
		for (int i = start; i <= end; i++)
			contactTree->setFirstColumnSpanned(i, parent, true);
}
