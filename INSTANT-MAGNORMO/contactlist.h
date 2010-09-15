#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QWidget>
#include "Contact.h"
#include <QSharedPointer>

class QTreeWidget;
class QTreeWidgetItem;

class ContactList : public QWidget
{
    friend class MainWindow;
Q_OBJECT
public:
    explicit ContactList(QWidget *parent = 0);

signals:
    void conversationInitiated(QString jid);

public slots:
    void plantContact(QSharedPointer<Contact> contact);

private slots:
    void itemActivated(QTreeWidgetItem *item, int column);

protected:
    QTreeWidget *contactTree;

};

#endif // CONTACTLIST_H
