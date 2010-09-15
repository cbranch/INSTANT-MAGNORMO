#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QWidget>
class QTreeWidget;
#include "Contact.h"
#include <QSharedPointer>

class ContactList : public QWidget
{
Q_OBJECT
public:
    explicit ContactList(QWidget *parent = 0);

signals:

public slots:
    void plantContact(QSharedPointer<Contact> contact);

protected:
    QTreeWidget *contactTree;

};

#endif // CONTACTLIST_H
