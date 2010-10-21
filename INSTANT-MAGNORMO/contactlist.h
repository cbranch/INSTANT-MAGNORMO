#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QWidget>
#include <QModelIndex>
#include "Contact.h"
class QTreeView;

class ContactList : public QWidget
{
    friend class MainWindow;
Q_OBJECT
public:
    explicit ContactList(QWidget *parent = 0);

    void setModel(QAbstractItemModel *model);

signals:
    void conversationInitiated(Contact);

private slots:
    void contactActivated(const QModelIndex &index);
	void setGroupProperties();
	void setGroupProperties(const QModelIndex &parent, int start, int end);

protected:
    QTreeView *contactTree;
};

#endif // CONTACTLIST_H
