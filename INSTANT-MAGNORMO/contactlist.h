#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QWidget>
class QTreeWidget;

class ContactList : public QWidget
{
Q_OBJECT
public:
    explicit ContactList(QWidget *parent = 0);

signals:

public slots:

protected:
    QTreeWidget *contactTree;

};

#endif // CONTACTLIST_H
