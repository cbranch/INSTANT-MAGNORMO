#include "contactmodel.h"
#include "MAGNORMOBOT.h"

ContactModel::ContactModel(MAGNORMOBOT *contactData, QObject *parent) :
    QAbstractItemModel(parent),
    contactData(contactData)
{
}

Qt::ItemFlags ContactModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ContactModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

QVariant ContactModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

int ContactModel::rowCount(const QModelIndex &parent) const
{
    return 0;
}

int ContactModel::columnCount(const QModelIndex &parent) const
{
    return 0;
}

QModelIndex ContactModel::index(int row, int column, const QModelIndex &parent) const
{
    return QModelIndex();
}

QModelIndex ContactModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}
