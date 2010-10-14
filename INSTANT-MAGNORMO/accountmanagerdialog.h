#ifndef ACCOUNTMANAGERDIALOG_H
#define ACCOUNTMANAGERDIALOG_H

#include <QDialog>
#include <QList>
#include <QListWidgetItem>
#include <QStringList>
#include <QSettings>
#include "account.h"
#include "accountdialog.h"
#include <stdio.h>

namespace Ui {
    class accountmanagerdialog;
}

class accountmanagerdialog : public QDialog
{
    Q_OBJECT

public:
    explicit accountmanagerdialog(QWidget *parent = 0);
    ~accountmanagerdialog();

    QList<Account*> getActiveAccounts();

private:
    Ui::accountmanagerdialog *ui;

    QListWidgetItem* makeListItem(Account *acc);
    Account* getAccount(QListWidgetItem *item);
    void writeAccounts();
    void readAccounts();

private slots:
    void on_removeButton_clicked();
    void on_toggleButton_clicked();
    void on_editButton_clicked();
    void on_addButton_clicked();
    void on_buttonBox_accepted();
};

#endif // ACCOUNTMANAGERDIALOG_H
