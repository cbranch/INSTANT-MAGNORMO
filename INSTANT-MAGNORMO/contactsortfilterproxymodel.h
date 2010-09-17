#ifndef CONTACTSORTFILTERPROXYMODEL_H
#define CONTACTSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class ContactSortFilterProxyModel : public QSortFilterProxyModel
{
Q_OBJECT
public:
    explicit ContactSortFilterProxyModel(QObject *parent = 0);

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

signals:

public slots:

};

#endif // CONTACTSORTFILTERPROXYMODEL_H
