#include "contactsortfilterproxymodel.h"
#include "contactmodel.h"
#include <gloox/presence.h>

ContactSortFilterProxyModel::ContactSortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

bool ContactSortFilterProxyModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    if (index.data(ContactModel::JIDRole).isValid()) {
        if (index.data(ContactModel::PresenceRole) == gloox::Presence::Unavailable)
            return false;
        else
            return true;
    }
    return true;
}

bool ContactSortFilterProxyModel::lessThan(const QModelIndex &left,
                                      const QModelIndex &right) const
{
    QVariant leftData = sourceModel()->data(left);
    QVariant rightData = sourceModel()->data(right);
    QString leftString = leftData.toString();
    QString rightString = rightData.toString();
    return QString::localeAwareCompare(leftString, rightString) < 0;
}
