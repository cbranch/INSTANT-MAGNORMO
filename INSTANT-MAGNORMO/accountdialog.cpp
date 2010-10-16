#include "accountdialog.h"
#include "ui_accountdialog.h"

AccountDialog::AccountDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AccountDialog)
{
    ui->setupUi(this);
    account = new Account();
}

AccountDialog::AccountDialog(QWidget *parent, Account *acc) :
    QDialog(parent),
    ui(new Ui::AccountDialog)
{
    ui->setupUi(this);
    account = acc;
    int snip = acc->user.indexOf("@");
    if(snip!=-1)
        acc->user = acc->user.left(snip);

    int index = 0;
    switch(acc->type) {
    case GTALK:
        index = 0;
        break;
    case GAPP:
        index = 1;
        break;
    case FACEBOOK:
        index = 2;
        break;
    }
    ui->serverList->setCurrentIndex(index);
    ui->usernameEdit->setText(acc->user);
    ui->passwordEdit->setText(acc->password);
}

AccountDialog::~AccountDialog()
{
    delete ui;
}

void AccountDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void AccountDialog::on_buttonBox_accepted()
{
    switch (ui->serverList->currentIndex()) {

    case 0: // GTalk
        if (ui->usernameEdit->text().endsWith(QString("@gmail.com")))
            account->user = ui->usernameEdit->text() + QString("/");
        else
            account->user = ui->usernameEdit->text() + QString("@gmail.com/");

        account->port = -1;
        account->type = GTALK;
        break;

    case 1: // Google Talk for apps
        account->user = ui->usernameEdit->text() + QString("/");
        account->server = QString("talk.google.com");
        account->port = 5222;
        account->type = GAPP;
        break;

    case 2: // Facebook
        account->user = ui->usernameEdit->text() + QString("@chat.facebook.com/");
        account->port = 5222;
        account->type = FACEBOOK;
        break;
    }

    account->password = ui->passwordEdit->text();
    account->active = true;
    this->accept();
}
