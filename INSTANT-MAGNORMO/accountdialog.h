#ifndef ACCOUNTDIALOG_H
#define ACCOUNTDIALOG_H

#include <QDialog>
#include "account.h"

namespace Ui {
    class AccountDialog;
}

class AccountDialog : public QDialog {
    Q_OBJECT
public:
    AccountDialog(QWidget *parent = 0);
    ~AccountDialog();

    Account *account;
protected:
    void changeEvent(QEvent *e);

private:
    Ui::AccountDialog *ui;

private slots:
    void on_buttonBox_accepted();
};

#endif // ACCOUNTDIALOG_H
