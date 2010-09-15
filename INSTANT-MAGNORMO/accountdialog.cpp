#include "accountdialog.h"
#include "ui_accountdialog.h"

AccountDialog::AccountDialog(QWidget *parent) :
    QDialog(parent),
    port(-1),
    ui(new Ui::AccountDialog)
{
    ui->setupUi(this);
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
            username = ui->usernameEdit->text() + QString("/");
        else
            username = ui->usernameEdit->text() + QString("@gmail.com/");
        break;

    case 1: // Google Talk for apps
        username = ui->usernameEdit->text() + QString("/");
        server = QString("talk.google.com");
        port = 5222;
        break;

    case 2: // Facebook
        username = ui->usernameEdit->text() + QString("@chat.facebook.com/");
        port = 5222;
        break;
    }


    password = ui->passwordEdit->text();
    this->accept();
}
