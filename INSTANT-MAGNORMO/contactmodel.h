#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <QAbstractItemModel>
class MAGNORMOBOT;

class ContactModel : public QAbstractItemModel
{
Q_OBJECT
public:
    explicit ContactModel(MAGNORMOBOT *contactData, QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

protected:
    MAGNORMOBOT *contactData;
};

#endif // CONTACTMODEL_H
