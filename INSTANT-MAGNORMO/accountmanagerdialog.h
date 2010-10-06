#ifndef ACCOUNTMANAGERDIALOG_H
#define ACCOUNTMANAGERDIALOG_H

#include <QDialog>
#include <QList>
#include <QListWidgetItem>
#include <QStringList>
#include "account.h"
#include "accountdialog.h"

namespace Ui {
    class accountmanagerdialog;
}

class accountmanagerdialog : public QDialog
{
    Q_OBJECT

public:
    explicit accountmanagerdialog(QWidget *parent = 0);
    ~accountmanagerdialog();

    void setAccountList(QList<Account*> accounts);
    QString username;
    QString password;
    QString server;
    int port;

private:
    Ui::accountmanagerdialog *ui;

    QListWidgetItem* makeListItem(Account *acc);

private slots:
    void on_addButton_clicked();
    void on_buttonBox_accepted();
};

#endif // ACCOUNTMANAGERDIALOG_H
