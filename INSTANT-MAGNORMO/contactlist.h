#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QTreeWidget>
#include <QVBoxLayout>
#include <QIcon>
#include <QWidget>
#include <QSharedPointer>
#include <gloox/presence.h>
#include "Contact.h"
#include <map>
#include <stdio.h>

using namespace std;
using namespace gloox;

class QTreeWidget;
class QTreeWidgetItem;

class ContactList : public QWidget
{
    friend class MainWindow;
Q_OBJECT
public:
    explicit ContactList(QWidget *parent = 0);
    QString getContactName(string JID);

signals:
    void conversationInitiated(QString jid);

public slots:
    void plantContact(QSharedPointer<Contact> contact);

private slots:
    void itemActivated(QTreeWidgetItem *item, int column);

protected:
    QTreeWidget *contactTree;
    map<string,QSharedPointer<Contact> > contactMap;
};

#endif // CONTACTLIST_H
