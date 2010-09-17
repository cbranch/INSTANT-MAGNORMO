#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QWidget>
#include <QModelIndex>

class QTreeView;

class ContactList : public QWidget
{
    friend class MainWindow;
Q_OBJECT
public:
    explicit ContactList(QWidget *parent = 0);

signals:
    void conversationInitiated(QString jid);

private slots:
    void contactActivated(const QModelIndex &index);

protected:
    QTreeView *contactTree;
};

#endif // CONTACTLIST_H
